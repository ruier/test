/* sysALib.s - Assembly support file and init routines */
	
/*
 * Copyright (c) 2005, 2008, 2009, 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,21aug12,l_z  Better implementation for sysPCGet. (WIND00076160)
01d,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01c,10feb09,b_m  remove sysInXxx(), sysOutXxx() and sysPciXxx()
01b,30oct08,x_z  remove all codes related to BUCSR(spr1013).(WIND00127193)
01a,03oct05,dtr  Created from cds85xx/01m
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include <config.h>
#include <sysLib.h>
#include <sysL2Cache.h>
#include <arch/ppc/mmuE500Lib.h>
	
	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_EXPORT(sysIvprSet)
	FUNC_EXPORT(vxL2CTLSet)
	FUNC_EXPORT(vxL2CTLGet)
	FUNC_EXPORT(sysCacheFlush)
	FUNC_IMPORT(usrInit)
        FUNC_EXPORT(sysPCGet)   /* get the value of the PC register */
        FUNC_EXPORT(sysL1Csr1Set)
        FUNC_EXPORT(sysTimeBaseLGet)
	FUNC_EXPORT(sysTas)
	_WRS_TEXT_SEG_START
        DATA_IMPORT(inFullVxWorksImage)
	
#define	CACHE_ALIGN_SHIFT	5	/* Cache line size == 2**5 */
	
FUNC_LABEL(_sysInit)		
FUNC_BEGIN(sysInit)

	mr  r8, r3
        xor p0,p0,p0
	mtspr TCR,p0
	xor   r6, r6, r6
	msync
	isync
	mtspr L1CSR0, r6		/* Disable the Data cache */
        li   r6, 0x0002              
	msync
	isync
	mtspr L1CSR0, r6		/* Invalidate the Data cache */
        li    r6, 0x0000              
        msync
	isync
	mtspr L1CSR1, r6	 /* Disable the Instrunction cache */
        li   r6, 0x0002              
        msync
	isync
	mtspr L1CSR1, r6	/* Invalidate the Instruction cache */
        isync
        li   r6, 0x0000              
	msync
	isync
	mtspr L1CSR1, r6        /* Disable the Instruction cache*/
	msync
	isync

        mbar 0

	lis  r6, HIADJ(inFullVxWorksImage)
	addi r6, r6,LO(inFullVxWorksImage)
	li   r7, TRUE
	stw  r7, 0(r6)

	/* initialize the stack pointer */
	
	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
	addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
	mr      r3, r8
	
	b usrInit
FUNC_END(sysInit)


	.balign 32
/******************************************************************************
*
* vxL2CTLSet - Set the value of the L@ cache control register
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLSet
*     (
*     UINT32 value,
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/
	
FUNC_BEGIN(vxL2CTLSet)
	mbar	0 
	isync
	stw	p0,0(p1)
	lwz	p0,0(p1)
	mbar	0
	isync
	blr
FUNC_END(vxL2CTLSet)

	.balign 32
/******************************************************************************
*
* vxL2CTLGet - Get the value of the L2 cache control register
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLGet
*     (
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/
FUNC_BEGIN(vxL2CTLGet)
	mbar    0
	isync
	lwz	p1,0x0(p0)
	addi    p0,p1,0x0
	mbar    0
	isync
	blr
FUNC_END(vxL2CTLGet)

/****************************************************
 * sysCacheFlush just flushes cache - assume int lock
 * p0 - cache line num
 * p1 - buffer origin
 * p2 - cache align size
 */
	
FUNC_BEGIN(sysCacheFlush)
	/*
	 * p3 contains the count of cache lines to be fetched & flushed.
	 * Convert to a count of pages covered, and fetch a word from
	 * each page to ensure that all addresses involved are in
	 * the TLB so that reloads do not disrupt the flush loop.
	 * A simple shift without round-up is sufficient because
	 * the p3 value is always a multiple of the shift count.
	 */
	srwi	p3, p0, MMU_RPN_SHIFT - CACHE_ALIGN_SHIFT
	mtspr	CTR, p3
        addi    p6,p1,0	
        li      p5,MMU_PAGE_SIZE
	subf    p3,p5,p1
	
	/*
	 * There might be a page boundary between here and the end of
	 * the function, so make sure both pages are in the I-TLB.
	 */
	b	cacheL2DisableLoadItlb
cacheL2DisableLoadDtlb:
	add     p3,p3,p5
	lbzu	p4,0(p3)
	bdnz	cacheL2DisableLoadDtlb
	mtctr   p0         /* Load counter with number of cache lines */
	subf	p1, p2, p1 /* buffer points to text  - cache line size */
l2DisableFlush:
        add	p1, p2, p1		  /* +  cache line size */
	lbzu	p3, 0x0(p1)	       	  /* flush the data cache block */
        bdnz    l2DisableFlush     /* loop till cache ctr is zero */
	sync
	isync
	mtctr   p0         /* Load counter with number of cache lines */
        addi    p1, p6, 0
	subf	p1, p2, p1 /* buffer points to text  - cache line size */
l2DisableClear:
	add	p1, p2, p1  /* point to next cache line */
	dcbf	0,p1			    /* flush newly-loaded line */
	bdnz	l2DisableClear	    /* repeat for all sets and ways */
	sync
	isync	
	blr
cacheL2DisableLoadItlb:
	b	cacheL2DisableLoadDtlb
FUNC_END(sysCacheFlush)

/******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* SYNOPSIS
* \ss
* UINT32 sysPCGet
*     (
*     void
*     )
* \se
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
	mflr    r3
	blr
FUNC_END(sysPCGet)

/******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* SYNOPSIS
* \ss
* UINT32 sysTimeBaseLGet(void)
* \se
*
* This routine will read the contents the lower half of the Time
* Base Register (TBL - TBR 268).
*
* RETURNS: value of TBR 268 (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
    mfspr       r3, 268
    bclr        20,0                    /* Return to caller */
FUNC_END(sysTimeBaseLGet)

/******************************************************************************
*
* sysL1Csr1Set - Set the value of L1CSR1
*
* SYNOPSIS
* \ss
* void sysL1Csr1Set
*     (
*     UINT32
*     )
* \se
*
* RETURNS: none
*/

FUNC_BEGIN(sysL1Csr1Set)
        msync
        isync
        mtspr   L1CSR1, r3
        msync
        isync
        blr
FUNC_END(sysL1Csr1Set)

FUNC_BEGIN(sysIvprSet)
		mtspr  IVPR,p0
		blr
FUNC_END(sysIvprSet)

/*******************************************************************************
*
* vxTas - this routine performs the atomic test and set for the PowerPC arch.
*
* RETURN: None.
*/

FUNC_BEGIN(sysTas)
	lis	r4, 0x8000	/* set the upper bit of r4 */
	ori     r4, r4, 0x0000
	eieio			/* simple ordered store using eieio */

	lwarx	r5, 0, r3	/* load and reserve */
	cmpwi	r5, 0		/* done if word */
	bne	sysTasEnd	/* not equal to 0 */

	stwcx.	r4, 0, r3	/* try to store non-zero */
	eieio			/* preserve load/store order */
	bne-	sysTas
	li	r3, 0x01
	blr
sysTasEnd:
	li	r3, 0
	blr
FUNC_END(sysTas)

