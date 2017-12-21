/* sysALib.s - wrSbc85xx system-dependent assembly routines */

/*
 * Copyright (c) 2002-2004, 2007, 2008, 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01r,21aug12,l_z  Better implementation for sysPCGet. (WIND00076160)
01q,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01p,30oct08,x_z  remove all codes related to BUCSR(spr1013).(WIND00127193)
01o,05mar07,l_z  Save system start parameter in sysInit function. (WIND00086526)
01n,10sep04,mil  Changed macro L1_ERRATA_FIX_REV2 to INCLUDE_L1_IPARITY_HDLR.
01m,19aug04,dtr  Mod for ICacheParity handler.
01l,28jul04,mil  Modified L1 parity handler to match errata workaround.
01k,14jul04,mil  Added sysTimeBaseLGet.
01j,10jun04,mil  Added L1 errata workaround.
01i,05may04,mil  Changed TLB mappings, removed NO_VX_BOOTROM.
01h,02feb04,mil  Fixed sysCacheFlush routine.
01g,13sep03,mil  Ensure contiguous cache flush.
01f,04sep03,dtr  Adding in stack frame pointer init.
01e,02sep03,dtr  Adding sysPCGet function to query if rom resident image.
01d,04aug03,dtr  Fix vision vxworks image.
01c,29jul03,dtr  Removing magic numbers.
01b,07jul03,mil  Added VISION_VXWORKS for no bootrom and local bus access
                 window with bootrom.
01a,03oct02,dtr  Created.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "config.h"
#include "sysLib.h"
#include "sysL2Cache.h"
#include "arch/ppc/mmuE500Lib.h"
	
	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_EXPORT(vxL2CTLSet)
	FUNC_EXPORT(vxL2CTLGet)
	FUNC_EXPORT(sysCacheFlush)
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
        FUNC_EXPORT(sysTimeBaseLGet)
        FUNC_EXPORT(sysL1Csr1Set)
	_WRS_TEXT_SEG_START
        DATA_IMPORT(inFullVxWorksImage)

#define	CACHE_ALIGN_SHIFT	5	/* Cache line size == 2**5 */

FUNC_BEGIN(sysInit)
FUNC_LABEL(_sysInit)

        /* save startType */
        mr      r2,r3 
        
        /*
         * TLB1 #0:  Should have mapped the last 4kB by reset logic.
         *
         * TLB1 #1:  RAM - Protected, global, TS=0, cached
         * 0x00000000..0x0fffffff => 0x000000000..0x0fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

        addis   r4, 0, 0x1001           /* TLBSEL = TLB1(CAM), ESEL = 1 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0xC000           /* V = 1, IPROT = 1, TID = 0 */
        ori     r5, r5, 0x0900          /* TS = 0, TSIZE = 256 MB */
        mtspr   MAS1, r5
        addis   r6, 0, DDR_SDRAM_ADRS1 >> 16    /* EPN = 0x00000000 */
        ori     r6, r6, 0x0000          /* WIMGE = 00000 */
        mtspr   MAS2, r6
        addis   r7, 0, DDR_SDRAM_ADRS1 >> 16    /* RPN = 0x00000000 */
        ori     r7, r7, 0x003f          /* UXSX-UWSW-URSR = 11-11-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /*
         * TLB1 #2:  RAM - Protected, global, TS=0, cached
         * 0x10000000..0x1fffffff => 0x100000000..0x1fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

        addis   r4, 0, 0x1002           /* TLBSEL = TLB1(CAM), ESEL = 2 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0xC000           /* V = 1, IPROT = 1, TID = 0 */
        ori     r5, r5, 0x0900          /* TS = 0, TSIZE = 256 MB */
        mtspr   MAS1, r5
        addis   r6, 0, DDR_SDRAM_ADRS2 >> 16    /* EPN = 0x10000000 */
        ori     r6, r6, 0x0000          /* WIMGE = 00000 */
        mtspr   MAS2, r6
        addis   r7, 0, DDR_SDRAM_ADRS2 >> 16    /* RPN = 0x10000000 */
        ori     r7, r7, 0x003f          /* UXSX-UWSW-URSR = 11-11-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /*
         * for ICE-loaded vxWorks image in which the processor is not
         * initialized by the bootrom, CCSRBAR may still reside at the
         * reset location and not yet moved.  Use best effort to probe
         * and move if necessary.
         */

        /*
         * TLB1 #4:  CCSBAR_RST - Global, TS=0, non-cached
         * 0xff700000..0xff7fffff => 0xff7000000..0xff7fffff (1 MB)
         * Attributes: SW-URSR (no exec, user not writable)
         */

        addis   r4, 0, 0x1004           /* TLBSEL = TLB1(CAM), ESEL = 4 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0x8000           /* V = 1, IPROT = 0, TID = 0 */
        ori     r5, r5, 0x0500          /* TS = 0, TSIZE = 1 MB */
        mtspr   MAS1, r5
        addis   r6, 0, CCSBAR_RST >> 16 /* EPN = 0xff700000 */
        ori     r6, r6, 0x001e          /* WIMGE = 11110 */
        mtspr   MAS2, r6
        addis   r7, 0, CCSBAR_RST >> 16 /* RPN = 0xff700000 */
        ori     r7, r7, 0x0007          /* UXSX-UWSW-URSR = 00-01-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /*
         * TLB1 #3:  CCSRBAR - Protected, global, TS=0, non-cached
         * 0xfe000000..0xfe0fffff => 0xfe0000000..0xfe0fffff (1 MB)
         * Attributes: SW-URSR (no exec, user not writable)
         */

        addis   r4, 0, 0x1003           /* TLBSEL = TLB1(CAM), ESEL = 3 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0xC000           /* V = 1, IPROT = 1, TID = 0 */
        ori     r5, r5, 0x0500          /* TS = 0, TSIZE = 1 MB */
        mtspr   MAS1, r5
        addis   r6, 0, CCSBAR >> 16     /* EPN = 0xff700000 */
        ori     r6, r6, 0x001e          /* WIMGE = 11110 */
        mtspr   MAS2, r6
        addis   r7, 0, CCSBAR >> 16     /* RPN = 0xff700000 */
        ori     r7, r7, 0x0007          /* UXSX-UWSW-URSR = 00-01-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /* check reset location, move CCSRBAR if found, or hang if lost */

        lis     r7, HIADJ(CCSBAR_RST)
        addi    r7, r7,LO(CCSBAR_RST)           /* r7 = 0xff700000 */
        lwz     r6, 0(r7)                       /* r6 = current CCSBAR val */
        srwi    r7, r7, 12                      /* r7 = 0xff700000 >> 12 */
        cmplw   r6, r7				/* eq => found at reset loc */
        beq     ccsrbarMove

        lis     r5, HIADJ(CCSBAR)
        addi    r5, r5,LO(CCSBAR)               /* r5 = 0xfe000000 */
        lwz     r4, 0(r5)                       /* r4 = current CCSBAR val */
        srwi    r5, r5, 12                      /* r5 = 0xfe000000 >> 12 */
        cmplw   r4, r5				/* eq => found at moved loc */
        beq     ccsrbarOk

ccsrbarLocLost:					/* hangs here if */
        b       ccsrbarLocLost			/* lost track of CCSRBAR */

ccsrbarMove:
        /* move CCSRBAR base address from reset of 0xff700000 */

        lis     r7, HIADJ(CCSBAR_RST)
        addi    r7, r7,LO(CCSBAR_RST)           /* r7 = 0xff700000 */
        lwz     r6, 0(r7)                       /* r6 = current CCSBAR val */
        isync                                   /* context sync */
        lis     r6, HIADJ(CCSBAR >> 12)
        addi    r6, r6, LO(CCSBAR >> 12)
        stw     r6, 0(r7)                       /* CCSRBAR = CCSBAR>>12 */
        lis     r7, HIADJ(0x200)
        addi    r7, r7,LO(0x200)                /* r7 = 0x200 */
        lwz     r6, 0(r7)                       /* r6 = don't cares */
        isync                                   /* context sync */
        lis     r7, HIADJ(CCSBAR)
        addi    r7, r7,LO(CCSBAR)               /* r7 = CCSBAR */
        lwz     r6, 0(r7)                       /* r6 = new CCSBAR val */
        isync                                   /* context sync */

ccsrbarOk:
        /* overwrite TLB entry of CCSBAR_RST to prevent overlap with ROM */

        /*
         * TLB1 #4:  UTIL_ADRS - Protected, global, TS=0, non-cached
         * 0x28000000..0x28ffffff => 0x280000000..0x28ffffff (16 MB)
         * Attributes: UWSW-URSR (no exec)
         */

        addis   r4, 0, 0x1004           /* TLBSEL = TLB1(CAM), ESEL = 4 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0xC000           /* V = 1, IPROT = 1, TID = 0 */
        ori     r5, r5, 0x0700          /* TS = 0, TSIZE = 16 MB */
        mtspr   MAS1, r5
        addis   r6, 0, UTIL_ADRS >> 16  /* EPN = 0x28000000 */
        ori     r6, r6, 0x001e          /* WIMGE = 11110 */
        mtspr   MAS2, r6
        addis   r7, 0, UTIL_ADRS >> 16  /* RPN = 0x28000000 */
        ori     r7, r7, 0x000f          /* UXSX-UWSW-URSR = 00-11-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /*
         * TLB1 #0:  ROM - Protected, global, TS=0, non-cached
         * 0xff000000..0xffffffff => 0xff0000000..0xffffffff (16 MB)
         * Attributes: UXSX-URSR (not writable)
         */

        addis   r4, 0, 0x1000           /* TLBSEL = TLB1(CAM), ESEL = 0 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0xC000           /* V = 1, IPROT = 1, TID = 0 */
        ori     r5, r5, 0x0700          /* TS = 0, TSIZE = 16 MB */
        mtspr   MAS1, r5
        addis   r6, 0, BOOT_FLASH_MAP_ADRS >> 16 /* EPN = 0xff000000 */
        ori     r6, r6, 0x001e          /* WIMGE = 11110 */
        mtspr   MAS2, r6
        addis   r7, 0, BOOT_FLASH_MAP_ADRS >> 16 /* RPN = 0xff000000 */
        ori     r7, r7, 0x0033          /* UXSX-UWSW-URSR = 11-00-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

#ifdef  INCLUDE_LOCAL_SDRAM
        /*
         * TLB1 #5:  SDRAM - Global, TS=0, cached
         * 0x20000000..0x23ffffff => 0x200000000..0x23ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

        addis   r4, 0, 0x1005           /* TLBSEL = TLB1(CAM), ESEL = 5 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0x8000           /* V = 1, IPROT = 0, TID = 0 */
        ori     r5, r5, 0x0800          /* TS = 0, TSIZE = 64 MB */
        mtspr   MAS1, r5
        addis   r6, 0, LOCAL_SDRAM1_ADRS >> 16  /* EPN = 0x20000000 */
        ori     r6, r6, 0x0000          /* WIMGE = 00000 */
        mtspr   MAS2, r6
        addis   r7, 0, LOCAL_SDRAM1_ADRS >> 16  /* RPN = 0x20000000 */
        ori     r7, r7, 0x003f          /* UXSX-UWSW-URSR = 11-11-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync

        /*
         * TLB1 #6:  SDRAM - Global, TS=0, cached
         * 0x24000000..0x27ffffff => 0x240000000..0x27ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

        addis   r4, 0, 0x1006           /* TLBSEL = TLB1(CAM), ESEL = 6 */
        ori     r4, r4, 0x0000
        mtspr   MAS0, r4
        addis   r5, 0, 0x8000           /* V = 1, IPROT = 0, TID = 0 */
        ori     r5, r5, 0x0800          /* TS = 0, TSIZE = 64 MB */
        mtspr   MAS1, r5
        addis   r6, 0, LOCAL_SDRAM2_ADRS >> 16  /* EPN = 0x24000000 */
        ori     r6, r6, 0x0000          /* WIMGE = 00000 */
        mtspr   MAS2, r6
        addis   r7, 0, LOCAL_SDRAM2_ADRS >> 16  /* RPN = 0x24000000 */
        ori     r7, r7, 0x003f          /* UXSX-UWSW-URSR = 11-11-11 */
        mtspr   MAS3, r7
        tlbwe
        tlbsync
#endif  /* INCLUDE_LOCAL_SDRAM */

	/* Add mapping for L2SRAM here if necessary */

	/* stop the timer */
	xor	r6, r6, r6
	mtspr	TCR, r6

        /* setup cache clean */

        li      r6, 0x0000
        msync
        isync
        mtspr   L1CSR0, r6              /* disable data cache */

        li      r7, 0x0002              /* r7 = DCFI */
        msync
        isync
        mtspr   L1CSR0, r7              /* invalidate the data cache */

        li      r6, 0x0000
        msync
        isync
        mtspr   L1CSR1, r6              /* disable instrunction cache */

        li      r7, 0x0002
        msync
        isync
        mtspr L1CSR1, r7                /* invalidate the instruction cache */
#if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
        isync

        li      r7, 0x0001
        msync
        isync
        mtspr   L1CSR1, r7              /* enable the instruction cache */
#endif  /* INCLUDE_CACHE_SUPPORT && USER_I_CACHE_ENABLE */
        msync
        isync

#if FALSE  /* LAWBAR not needed for 8 MB boot flash at 0xff800000 */
	/* Memory mapped region base address */
	
        lis     r6, HIADJ(M85XX_LAWBAR4(CCSBAR))
        addi    r6, r6, LO(M85XX_LAWBAR4(CCSBAR))
	
	/* Initialise the Local Address Windows */
        lis     r7, HIADJ(BOOT_FLASH_ADRS >> LAWBAR_ADRS_SHIFT)
        addi    r7, r7, LO(BOOT_FLASH_ADRS >> LAWBAR_ADRS_SHIFT)
	stw     r7, 0x0(r6)

        lis     r6, HIADJ(M85XX_LAWAR4(CCSBAR))
        addi    r6, r6, LO(M85XX_LAWAR4(CCSBAR))

        lis     r7, HIADJ (LAWAR_ENABLE | \
		           LAWAR_TGTIF_LBC | \
	                   LAWAR_SIZE_8MB )
        addi    r7, r7, LO (LAWAR_ENABLE | \
	                    LAWAR_TGTIF_LBC | \
	                    LAWAR_SIZE_8MB)
	stw     r7, 0x0(r6)

        mbar 0
#endif  /* FALSE */

	/* flag TRUE for inFullVxWorksImage */

	lis	r6, HIADJ(inFullVxWorksImage)
	addi	r6, r6, LO(inFullVxWorksImage)
	li	r7, TRUE
	stw	r7, 0(r6)

	/* initialize the stack pointer */
	
	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
        addi    sp, sp, -FRAMEBASESZ	/* get frame stack */

	mr  r3, r2	
	b	usrInit
FUNC_END(sysInit)

	
FUNC_BEGIN(vxL2CTLSet)
	mbar	0 
	isync
	stw	p0,0(p1)
	lwz	p0,0(p1)
	mbar	0
	isync
	blr
FUNC_END(vxL2CTLSet)


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
 *
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

