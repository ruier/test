/* sysALib.s - Assembly support file and init routines */
	
/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,30oct08,x_z  remove all codes related to BUCSR(spr1013).(WIND00127193)
01h,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01g,30jun08,dtr  Initialize sp for Wrload Image Build.
01f,11apr08,dtr  Update for WRLoad images.
01e,11jan08,dtr  Remove enable branch bprediction from sysInit, put in
                 sysHwInit.
01d,19dec07,dtr  Add support for AMP.
01c,14nov07,dtr  Move SPRG6 to SPRG6_W.
01b,21sep07,dtr  Enable timebase clock.
01a,07feb07,dtr  Created from cds8548/01a
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
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
	FUNC_EXPORT(sysCacheEnable)
	FUNC_IMPORT(usrInit)
	FUNC_EXPORT(sysInWord)
	FUNC_EXPORT(sysOutWord)
	FUNC_EXPORT(sysInLong)
	FUNC_EXPORT(sysOutLong)
        FUNC_EXPORT(sysInByte)
	FUNC_EXPORT(sysOutByte)
	FUNC_EXPORT(sysPciRead32)
	FUNC_EXPORT(sysPciWrite32)
	FUNC_EXPORT(sysPciInByte)
	FUNC_EXPORT(sysPciOutByte)
	FUNC_EXPORT(sysPciInWord)
	FUNC_EXPORT(sysPciOutWord)
	FUNC_EXPORT(sysPciInLong)
	FUNC_EXPORT(sysPciOutLong)
        FUNC_EXPORT(sysPCGet)   /* get the value of the PC register */
        FUNC_EXPORT(sysL1Csr1Set)
        FUNC_EXPORT(sysTimeBaseLGet)
#ifdef INCLUDE_L1_IPARITY_HDLR_INBSP
        FUNC_EXPORT(sysIParityHandler)
        FUNC_EXPORT(sysIvor1Set)
        FUNC_EXPORT(jumpIParity)
#endif  /* INCLUDE_L1_IPARITY_HDLR_INBSP */
	FUNC_EXPORT(sysTas)

	FUNC_EXPORT(sysCpu1Start)
	FUNC_EXPORT(sysCpu1Stop)
#if defined(_WRS_CONFIG_SMP)
	FUNC_EXPORT(sysSmpTimeBaseSet)
#endif	/* _WRS_CONFIG_SMP */

	FUNC_IMPORT(sysCpu1Loop)
	
	_WRS_TEXT_SEG_START
        DATA_IMPORT(inFullVxWorksImage)
	
#define	CACHE_ALIGN_SHIFT	5	/* Cache line size == 2**5 */
	
FUNC_LABEL(_sysInit)		
FUNC_BEGIN(sysInit)
#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
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
	mtspr L1CSR1, r6        /* temp disabled Enable the Instruction cache*/


        isync
        li	r7, 0x0000
	msync
	isync
	mtspr	L1CSR1, r7	


	msync
	isync


        /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */
	
	xor   r0,r0,r0
	
        mtspr SRR0, r0
        mtspr SRR1, r0
        mtspr CSRR0, r0
        mtspr CSRR1, r0
        mtspr MCSRR0, r0
        mtspr MCSRR1, r0
        mtspr ESR, r0
        mtspr MCSR, r0
        mtspr DEAR, r0
        mtspr DBCR0, r0
        mtspr DBCR1, r0        
        mtspr DBCR2, r0
        mtspr IAC1, r0
        mtspr IAC2, r0
        mtspr DAC1, r0
        mtspr DAC2, r0
        
        mfspr r1, DBSR
        mtspr DBSR, r1
        
        mtspr PID0, r0
        mtspr PID1, r0
        mtspr PID2, r0
        mtspr TCR, r0
        mtspr 1013, r0

	mtspr MAS4, r0
	mtspr MAS6, r0
	isync

	lis   r1,HI(VEC_BASE_ADRS)
	ori   r1,r1,LO(VEC_BASE_ADRS)
        mtspr IVPR, r1

        /* Set up vector offsets */
        addi  r3, r0, 0x100
        mtspr IVOR0, r3
        addi  r3, r0, 0x200
        mtspr IVOR1, r3
        addi  r3, r0, 0x300
        mtspr IVOR2, r3
        addi  r3, r0, 0x400
        mtspr IVOR3, r3
        addi  r3, r0, 0x500
        mtspr IVOR4, r3
        addi  r3, r0, 0x600
        mtspr IVOR5, r3
        addi  r3, r0, 0x700
        mtspr IVOR6, r3
        /* skipping IVOR7 0x800: no FPU on e500 */
        addi  r3, r0, 0x900
        mtspr IVOR8, r3
        /* skipping IVOR9 0xa00: no aux processor on e500 */
        addi  r3, r0, 0xb00
        mtspr IVOR10, r3
        addi  r3, r0, 0xc00
        mtspr IVOR11, r3
        addi  r3, r0, 0xd00
        mtspr IVOR12, r3
        addi  r3, r0, 0xe00
        mtspr IVOR13, r3
        addi  r3, r0, 0xf00
        mtspr IVOR14, r3
        addi  r3, r0, 0x1000
        mtspr IVOR15, r3
        addi  r3, r0, 0x1100       /* SPU is e500 specific */
        mtspr IVOR32, r3
        addi  r3, r0, 0x1200       /* FP data is e500 specific */
        mtspr IVOR33, r3
        addi  r3, r0, 0x1300       /* FP round is e500 specific */
        mtspr IVOR34, r3
        addi  r3, r0, 0x1400       /* perf mon is e500 specific */
        mtspr IVOR35, r3

	li    r2,0x1e
	mtspr MMUCSR0, r2
	isync
		
	li    r3,4
	li    r4,0
	tlbivax r4,r3
	nop

	
        /*
         * Write TLB entry for initial program memory page
         *
         * - Specify EPN, RPN, and TSIZE as appropriate for system
         * - Set valid bit
         * - Specify TID=0
         * - Specify TS=0 or else MSR[IS,DS] must be set to correspond to TS=1
         * - Specify storage attributes (W, I, M, G, E, U0 - U3) as appropriate
         * - Enable supervisor mode fetch, read, and write access (SX, SR, SW)
         */

	/*
         * TLB1 #0.  Initial TLB
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1000          /* TLBSEL = TLB1(CAM) , ESEL = 2*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_4K /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0, 0xffff    /* EPN = RESET ADDR */
        ori    r6,r6,0xf00a          /* WIMGE = 01010 */
        mtspr  MAS2, r6
	
        lis    r7, HI(RAM_LOW_ADRS) /* Physcial translated address */
        ori    r7,r7,LO((RAM_LOW_ADRS & 0xf000)| 0x015)         /* Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe                       
        tlbsync                      
        /*
         * TLB1 #1.  Main SDRAM - Not Cached
	 *           LOCAL_MEM_LOCAL_ADRS -> LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1001           /* TLBSEL = TLB1(CAM) , ESEL = 1*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_1G  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(PHYS_MEM_START) /* EPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r6,r6,0x000a          /* WIMGE = 01010*/
        mtspr  MAS2, r6
        addis  r7,0,HI(PHYS_MEM_START)  /* RPN = LOCAL_MEM_LOCAL_ADRS */
        ori    r7,r7,0x003f          /* User/Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe                       
        tlbsync                      


	/*
         * TLB1 #2.  CCSBAR 
         * Attributes: UX/UW/UR/SX/SW/SR
         */

        addis  r4,0,0x1002          /* TLBSEL = TLB1(CAM) , ESEL = 2*/
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0xC000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_1M /* TS = 0, TSIZE = 64 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,HI(CCSBAR) /* EPN = CCSBAR */
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6

        addis  r7,0,HI(CCSBAR) /* RPN = CCSBAR */
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe                       
        tlbsync                      
	
        /*
         * TLB1 #0.  ROM - non-cached 0xf0000000 -> 0xf00000000.  
	 * 256MB
         * Attributes: SX/SW/SR **PROTECTED**
         */

        addis  r4,0,0x1003           /* TLBSEL = TLB1(CAM) , ESEL = 0 */ 
        ori    r4,r4,0x0000
        mtspr  MAS0, r4
        addis  r5,0,0x8000           /* V = 1, IPROT = 1, TID = 0*/
        ori    r5,r5,_MMU_TLB_SZ_256M  /* TS = 0, TSIZE = 256 MByte page size*/
        mtspr  MAS1, r5
        addis  r6,0,0xf000           /* EPN = 0xf0000000*/
        ori    r6,r6,0x000a          /* WIMGE = 01010 */
        mtspr  MAS2, r6
        addis  r7,0,0xf000           /* RPN = 0xf0000000*/
        ori    r7,r7,0x0015          /* Supervisor XWR*/
        mtspr  MAS3, r7
        isync
        msync
        tlbwe         
        tlbsync                     
	
	li     r7,12
	mtctr  r7
clearTLBs:
        addis  r4,r4,0x0001
        mtspr  MAS0, r4
        lis    r5,0x0000           /* V = 1, IPROT = 1, TID = 0*/
        mtspr  MAS1, r5
	isync
        msync
        tlbwe         
        tlbsync                     
		
	bdnz   clearTLBs
	li      r3, BOOT_WARM_AUTOBOOT

#if 0 /* useful to wait here for OCD as it allows you to set hardware 
	breakpoints up after starting the second core. You will need to 
	setPC to the next address after this instructions - use the physical 
	address not the boot pages address which confuses the OCD */ 
stayHereWaitForDebugger:	
	b stayHereWaitForDebugger
#endif

	/* Setup stack pointer */
	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
	addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
	
	/* Memory etc already setup jump to alternate boot image */
        lis     r6, HI(usrInit)
        ori     r6, r6, LO(usrInit)
	mtctr	r6
	bctr
_sysInitEnd:	
	.space (0xffc - (_sysInitEnd-_sysInit))
_resetVector:	
	b sysInit
#else /* INCLUDE_WRLOAD_IMAGE_BUILD */
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

#ifdef	_WRS_CONFIG_SMP
	mfspr	r3, HID1
	isync
	andi.   r3,r3,1
	mtspr   SPRG6_W,r3
	cmpwi   r3, 0
	beq	doCpu0Stack
	lis     sp,HIADJ(CPU1_STACK_START_ADR)
	addi    sp, sp, LO(CPU1_STACK_START_ADR)
	b	doneCpu0Stack
#endif	/* _WRS_CONFIG_SMP */
	
	/* initialize the stack pointer */
doCpu0Stack:
	/* insert protection from decrementer exceptions */

	xor	p0, p0, p0
	LOADPTR	(p1, 0x4c000064)	/* load rfi (0x4c000064) to p1	*/
	stw	p1, 0x900(r0)		/* store rfi at 0x00000900	*/
	
	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
doneCpu0Stack:

	addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
#ifdef _WRS_CONFIG_SMP
	mfspr	r3, HID1
	isync
	andi.   r3, r3, 1
	cmpwi   r3, 1
	bne	bootUsrInit
	b	sysCpu1Loop
#endif	/* _WRS_CONFIG_SMP */
bootUsrInit:		
	mr      r3, r8
		
	b usrInit
#endif  /* INCLUDE_WRLOAD_IMAGE_BUILD */
FUNC_END(sysInit)

FUNC_BEGIN(sysCacheEnable)
        li      r3, 0x0002              
	msync
	isync
	mtspr   L1CSR0, r3		/* Invalidate the Data cache */
        li	r3, 0x0001
	msync
	isync
	mtspr	L1CSR0, r3		/* enable the instruction cache */
		
        li      r3, 0x0002              
	msync
	isync
	mtspr   L1CSR1, r3		/* Invalidate the Data cache */
	msync
	isync
        li	r3, 0x0001
	msync
	isync
	mtspr	L1CSR1, r3		/* enable the instruction cache */
		
	msync
	isync
	blr
FUNC_END(sysCacheEnable)
	
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

/*****************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.

* UCHAR sysInByte
*     (
*     UCHAR *  pAddr		/@ Virtual I/O addr to read from @/
*     )

*/

FUNC_BEGIN(sysInByte)
	eieio			/* Sync I/O operation */
	sync
	lbzx	p0,r0,p0	/* Read byte from I/O space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysInByte)
	
/******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A

* VOID sysOutByte
*     (
*     UCHAR *  pAddr,		/@ Virtual I/O addr to write to @/
*     UCHAR    data		/@ data to be written @/
*     )

*/

FUNC_BEGIN(sysOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	eieio			/* Sync I/O operation */
	sync
	bclr	20,0		/* Return to caller */
FUNC_END(sysOutByte)
	
/*****************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified 
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.

* USHORT sysInWord
*     (
*     ULONG  address,		/@ addr to read from @/
*     )

*/

FUNC_BEGIN(sysInWord)
	eieio			/* Sync I/O operation */
	sync
        lhbrx   p0,r0,p0	/* Read and swap */
        bclr    20,0		/* Return to caller */
FUNC_END(sysInWord)


/*****************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified 
* address.
*
* RETURNS: N/A

* VOID sysOutWord
*     (
*     ULONG address,		/@ Virtual addr to write to @/
*     UINT16  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysOutWord)
        sthbrx  p1,r0,p0	/* Write with swap to address */
	eieio			/* Sync I/O operation */
	sync
        bclr    20,0		/* Return to caller */
FUNC_END(sysOutWord)

/*****************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysInLong
*     (
*     ULONG  address,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysInLong)
	eieio			/* Sync I/O operation */
	sync
        lwbrx   p0,r0,p0	/* Read and swap from address */
        bclr    20,0		/* Return to caller */
FUNC_END(sysInLong)


/******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A

* VOID sysOutLong
*     (
*     ULONG   address,		/@ Virtual addr to write to @/
*     ULONG   data		/@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysOutLong)
        stwbrx  p1,r0,p0	/* store data as little-endian */
	eieio			/* Sync I/O operation */
	sync
        bclr    20,0
FUNC_END(sysOutLong)

/******************************************************************************
*
* sysPciRead32 - read 32 bit PCI data
*
* This routine will read a 32-bit data item from PCI (I/O or
* memory) space.
*
* RETURNS: N/A

* VOID sysPciRead32
*     (
*     ULONG *  pAddr,		/@ Virtual addr to read from @/
*     ULONG *  pResult		/@ location to receive data @/
*     )

*/

FUNC_BEGIN(sysPciRead32)
	eieio			/* Sync I/O operation */
        lwbrx   p0,r0,p0	/* get the data and swap the bytes */
        stw     p0,0(p1)	/* store into address ptd. to by p1 */
        bclr    20,0
FUNC_END(sysPciRead32)

/******************************************************************************
*
* sysPciWrite32 - write a 32 bit data item to PCI space
*
* This routine will store a 32-bit data item (input as big-endian)
* into PCI (I/O or memory) space in little-endian mode.
*
* RETURNS: N/A

* VOID sysPciWrite32
*     (
*     ULONG *  pAddr,		/@ Virtual addr to write to @/
*     ULONG   data		/@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysPciWrite32)
        stwbrx  p1,r0,p0	/* store data as little-endian */
        bclr    20,0
FUNC_END(sysPciWrite32)

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.

* UINT8 sysPciInByte
*     (
*     UINT8 *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInByte)
	eieio			/* Sync I/O operation */
        lbzx    p0,r0,p0	/* Read byte from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInByte)
	
/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.

* USHORT sysPciInWord
*     (
*     USHORT *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInWord)
	eieio			/* Sync I/O operation */
        lhbrx   p0,r0,p0	/* Read and swap from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInWord)
	
/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysPciInLong
*     (
*     ULONG *  pAddr,		/@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInLong)
	eieio			/* Sync I/O operation */
        lwbrx   p0,r0,p0	/* Read and swap from PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciInLong)
	
/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* RETURNS: N/A

* VOID sysPciOutByte
*     (
*     UINT8 *  pAddr,		/@ Virtual addr to write to @/
*     UINT8  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutByte)
        stbx    p1,r0,p0	/* Write a byte to PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutByte)
	
/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutWord
*     (
*     USHORT *  pAddr,		/@ Virtual addr to write to @/
*     USHORT  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutWord)
        sthbrx  p1,r0,p0	/* Write with swap to PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutWord)
	
/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutLong
*     (
*     ULONG *  pAddr,		/@ Virtual addr to write to @/
*     ULONG  data		/@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutLong)
        stwbrx  p1,r0,p0	/* Write big-endian long to little-endian */
        mr      p0,p1		/* PCI space */
        bclr    20,0		/* Return to caller */
FUNC_END(sysPciOutLong)

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
        mflr	r4 /* Save LR value */

        bl      Next    /* Set PC */
Next:
        mflr    r3 /* Get PC */
		mtlr    r4 /* Restor LR value */
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

#if defined(INCLUDE_L1_IPARITY_HDLR)
# include "sysL1ICacheParity.s"
#elif defined(INCLUDE_L1_IPARITY_HDLR_INBSP)
	
#define DETECT_EXCHDL_ADRS(ivor)  \
        mfspr   p0, IVPR;         \
        mfspr   p1, ivor;         \
        or      p1, p1, p0;       \
        mfspr   p0, MCSRR0;       \
        cmpw    p0, p1;           \
        beq     faultDetected;

/*********************************************************************
 *
 * sysIParityHandler - This routine is call for a machine check. 
 * This routine will invalidate the instruction cache for the address 
 * in MCSRRO. If only instruction parity error then it will return from
 * machine check else it will go to standard machine check handler.
 */ 
FUNC_BEGIN(sysIParityHandler)
        /* Save registers used */
     
        mtspr   SPRG4_W ,p0
        mtspr   SPRG5_W ,p1
        mfcr    p0
        mtspr   SPRG6_W ,p0
            
        /* check for ICPERR */
        mfspr   p1, MCSR
        andis.  p1, p1, 0x4000
        beq     ppcE500Mch_norm

        /* check if mcsrr0 is pointing to 1st instr of exception handler */
        DETECT_EXCHDL_ADRS(IVOR0)
        DETECT_EXCHDL_ADRS(IVOR1)
        DETECT_EXCHDL_ADRS(IVOR2)
        DETECT_EXCHDL_ADRS(IVOR3)
        DETECT_EXCHDL_ADRS(IVOR4)
        DETECT_EXCHDL_ADRS(IVOR5)
        DETECT_EXCHDL_ADRS(IVOR6)
        DETECT_EXCHDL_ADRS(IVOR8)
        DETECT_EXCHDL_ADRS(IVOR10)
        DETECT_EXCHDL_ADRS(IVOR11)
        DETECT_EXCHDL_ADRS(IVOR12)
        DETECT_EXCHDL_ADRS(IVOR13)
        DETECT_EXCHDL_ADRS(IVOR14)
        DETECT_EXCHDL_ADRS(IVOR15)
        DETECT_EXCHDL_ADRS(IVOR32)
        DETECT_EXCHDL_ADRS(IVOR33)
        DETECT_EXCHDL_ADRS(IVOR34)
        DETECT_EXCHDL_ADRS(IVOR35)

        /* p0 here has mcsrr0 value, round to cache line boundary */
        rlwinm  p0, p0, 0, 0, 31 - CACHE_ALIGN_SHIFT 
        /* invalidate instruction cache */
        icbi    r0, p0
        isync
#ifdef INCLUDE_SHOW_ROUTINES
        /* Add 1 to instrParityCount to measure no of parity errors */ 
        lis     p0, HIADJ(instrParityCount)
        addi    p0, p0, LO(instrParityCount)
        lwz     p1, 0(p0)
        addi    p1, p1, 1
        stw     p1, 0(p0)
#endif
        /* return after invalidate */
        mfspr   p0, SPRG6_R
        mtcr    p0
        mfspr   p0, SPRG4_R
        mfspr   p1, SPRG5_R
        isync     
        rfmci /*.long   0x4c00004c*/

ppcE500Mch_norm:
        mfspr   p0, SPRG6_R
        mtcr    p0
        mfspr   p0, SPRG4_R
        mfspr   p1, SPRG5_R
        ba      0x200       /* _EXC_OFF_MACH */

faultDetected:
        /* rebooting, no need to save regs */
        bl      chipErrataCpu29Print
        li      p0, BOOT_NORMAL
        b       sysToMonitor      /* reset */
FUNC_END(sysIParityHandler)

/* Branch to above handler copied to _EXC_OFF_END */
FUNC_BEGIN(jumpIParity)
	ba     sysIParityHandler
FUNC_END(jumpIParity)

/****************************************************************
 * sysIvor1Set - fills in value of IVOR1 register to override 
 * standard machine check handler for L1 instruction parity recovery.
 */
FUNC_BEGIN(sysIvor1Set)
        mtspr IVOR1, p0
	blr
FUNC_END(sysIvor1Set)

#endif  /* INCLUDE_L1_IPARITY_HDLR */

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


#define WRITEADR(reg1, reg2, addr32, val) \
	lis	reg1, HI(addr32); \
	ori	reg1, reg1, LO(addr32); \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, 0(reg1)


/*******************************************************************************
*
* sysCpu1Start - this routine starts CPU 1.
*
* RETURN: None.
*/

FUNC_BEGIN(sysCpu1Start)
	WRITEADR(r4, r5, EEB_PCR(CCSBAR), 0x03000000)
	isync
	sync
	eieio
	blr
FUNC_END(sysCpu1Start)


/*******************************************************************************
*
* sysCpu1Stop - this routine stops CPU 1.
*
* RETURN: None.
*/

FUNC_BEGIN(sysCpu1Stop)
	WRITEADR(r4, r5, EEB_PCR(CCSBAR), 0x01000000)
	isync
	sync
	eieio
	blr
FUNC_END(sysCpu1Stop)

#if defined(_WRS_CONFIG_SMP)
/*******************************************************************************
*
* sysSmpTimeBaseSet - This routine sets the timebase, and ensures
* that it is enabled in HID0.  It is intended to be called only by
* sysSmpTimeBaseInit, as part of the initial timebase synchronization
* process.  (vxTimeBaseSet cannot be used for this purpose because,
* in VxWorks SMP, vxTimeBaseSet is a no-op to prevent loss of
* synchronization due to applications setting the timebase.)
*
* RETURN: None.
*/

FUNC_BEGIN(sysSmpTimeBaseSet)
	/*
	 * No need to clear TBL first, since the TB
	 * is not running when this function is called.
	 */
	mttbl   p1
	mttbu	p0
	mfspr	p0, HID0
	oris	p0, p0, _PPC_HID0_TBEN
	mtspr	HID0, p0
	isync		/* Context sync req'd after setting HID0[TBEN] */
	blr
FUNC_END(sysSmpTimeBaseSet)
#endif	/* _WRS_CONFIG_SMP */
