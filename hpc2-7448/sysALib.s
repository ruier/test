/* sysALib.s - Freescale HPC-II system-dependent assembly routines */

/* Copyright 2006 Wind River Systems, Inc. */
/* Copyright 1996-1999 Motorola, Inc. */
/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01b,01feb06,dtr  tidyup
01a,25jul05,avb  written from Motorola sp7447
*/

/*
DESCRIPTION
This module contains the entry code, sysInit(), for VxWorks images that start
running from RAM, such as 'vxWorks'. These images are loaded into memory
by some external program (e.g., a boot ROM) and then started.
The routine sysInit() must come first in the text segment. Its job is to perform
the minimal setup needed to call the generic C
routine usrInit() with parameter BOOT_COLD.

The routine sysInit() typically masks interrupts in the processor, sets the
initial stack pointer to _sysInit then jumps to usrInit.
Most other hardware and device initialization is performed later by
sysHwInit().
*/

/* includes */

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include "config.h"
#include <regs.h>
#include <asm.h>
#ifdef INCLUDE_CACHE_L2
#include "sysL2Cache.h"
#endif
/* defines */

/*
 * Some releases of h/arch/ppc/toolPpc.h had bad definitions of
 * LOADPTR and LOADVAR. So we will define it correctly.
 * [REMOVE THESE FOR NEXT MAJOR RELEASE].
 *
 * LOADPTR initializes a register with a 32 bit constant, presumably the
 * address of something.
 */

#undef LOADPTR
#define	LOADPTR(reg,const32) \
	  addis reg,r0,HIADJ(const32); addi reg,reg,LO(const32)

/*
 * LOADVAR initializes a register with the contents of a specified memory
 * address. The difference being that the value loaded is the contents of
 * the memory location and not just the address of it.
 */

#undef LOADVAR
#define	LOADVAR(reg,addr32) \
	  addis reg,r0,HIADJ(addr32); lwz reg,LO(addr32)(reg)


/* globals */

	FUNC_EXPORT(_sysInit)		/* start of system code */
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
        FUNC_EXPORT(sysInWord)
        FUNC_EXPORT(sysOutWord)
        FUNC_EXPORT(sysInLong)
        FUNC_EXPORT(sysOutLong)
        FUNC_EXPORT(sysMemProbeSup)
        FUNC_EXPORT(sysPciErr)

	/* externals */

        FUNC_IMPORT(usrInit)

    _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*
* sysInit (int startType)    /@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

FUNC_BEGIN(_sysInit)

	/* save startType parameter */
	mr	r31, r3

	/* disable interrupts */

	xor	p0, p0, p0
        mtmsr   p0                      /* clear the MSR register  */

        /* Zero-out registers: r0 & SPRGs */

        xor     r0,r0,r0
        mtspr   272,r0
        mtspr   273,r0
        mtspr   274,r0
        mtspr   275,r0

        /*
         *      Set MPU/MSR to a known state
         *      Turn on FP
         */

        andi.   r3, r3, 0
        ori     r3, r3, 0x2000
        sync
        mtmsr   r3
        isync
        sync

        /* Init the floating point control/status register */

        mtfsfi  7,0x0
        mtfsfi  6,0x0
        mtfsfi  5,0x0
        mtfsfi  4,0x0
        mtfsfi  3,0x0
        mtfsfi  2,0x0
        mtfsfi  1,0x0
        mtfsfi  0,0x0
        isync

        /* Initialize the floating point data registers to a known state */

        bl      ifpdrValue
        .long   0x3f800000      /* 1.0 */
ifpdrValue:
        mfspr   r3,LR
        lfs     f0,0(r3)
        lfs     f1,0(r3)
        lfs     f2,0(r3)
        lfs     f3,0(r3)
        lfs     f4,0(r3)
        lfs     f5,0(r3)
        lfs     f6,0(r3)
        lfs     f7,0(r3)
        lfs     f8,0(r3)
        lfs     f9,0(r3)
        lfs     f10,0(r3)
        lfs     f11,0(r3)
        lfs     f12,0(r3)
        lfs     f13,0(r3)
        lfs     f14,0(r3)
        lfs     f15,0(r3)
        lfs     f16,0(r3)
        lfs     f17,0(r3)
        lfs     f18,0(r3)
        lfs     f19,0(r3)
        lfs     f20,0(r3)
        lfs     f21,0(r3)
        lfs     f22,0(r3)
        lfs     f23,0(r3)
        lfs     f24,0(r3)
        lfs     f25,0(r3)
        lfs     f26,0(r3)
        lfs     f27,0(r3)
        lfs     f28,0(r3)
        lfs     f29,0(r3)
        lfs     f30,0(r3)
        lfs     f31,0(r3)
        sync

        /*
         *      Set MPU/MSR to a known state
         *      Turn off FP
         */

        andi.   r3, r3, 0
        sync
        mtmsr   r3
        isync
        sync

        /* Init the Segment registers */

        andi.   r3, r3, 0
        isync
        mtsr    0,r3
        isync
        mtsr    1,r3
        isync
        mtsr    2,r3
        isync
        mtsr    3,r3
        isync
        mtsr    4,r3
        isync
        mtsr    5,r3
        isync
        mtsr    6,r3
        isync
        mtsr    7,r3
        isync
        mtsr    8,r3
        isync
        mtsr    9,r3
        isync
        mtsr    10,r3
        isync
        mtsr    11,r3
        isync
        mtsr    12,r3
        isync
        mtsr    13,r3
        isync
        mtsr    14,r3
        isync
        mtsr    15,r3
        isync


        /* insert protection from decrementer exceptions */

	xor	p0, p0, p0
	LOADPTR (p1, 0x4c000064)        /* load rfi (0x4c000064) to p1      */
        stw     p1, 0x900(r0)           /* store rfi at 0x00000900          */

	/* initialize the stack pointer */
	
	lis     sp, HI(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
	
        /* Turn off data and instruction cache control bits */

        mfspr   r3, HID0
        isync
        rlwinm  r4, r3, 0, 18, 15       /* r4 has ICE and DCE bits cleared */
        sync
        isync
        mtspr   HID0, r4                /* HID0 = r4 */
        isync


        /* Get cpu type */

        mfspr   r28, PVR
        rlwinm  r28, r28, 16, 16, 31

        /* invalidate the MPU's data/instruction caches */

        lis     r3, 0x0
	cmpli   0, 0, r28, CPU_TYPE_7457
	beq     cpuIs7450
	cmpli   0, 0, r28, CPU_TYPE_7447A
	beq     cpuIs7450
	cmpli   0, 0, r28, CPU_TYPE_7448
	beq     cpuIs7450

cpuIs7450:
	mfspr   r3, HID0
	sync
	oris    r3,r3,0x0400            # Set TBEN
	oris    r3,r3,0x0010            # Set DPM
	mtspr   HID0, r3
	sync

	mfspr   r3,1014   /* read from MSSCR0 */
	sync
	/* workaround for Tsi108 Errata #2 */
	oris    r3,r3,0x0100   /* set EIDIS bit = intervention off */
	rlwinm	r3,r3,0,6,31   /* set DTQ = 0 (8 entries) */
	ori     r3,r3,0x0003   /* set L2PFE bits = b11 */
	mtspr   1014,r3  /* write to MSSCR0 */
	sync

cpuIs750:
	/* Also handles 7400, 7410 and 744x/745x */
#ifdef USER_I_CACHE_ENABLE
        mfspr   r3,HID0

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x8800    /* Setup bit pattern for ICE/ICFI */
        or      r3,r4,r3
        isync
        mtspr   HID0,r3         /* set ICE/ICFI */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x0800    /* Setup bit pattern for ICFI */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear IFCI (bit 16) */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x2000    /* Setup bit pattern for ILOCK */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear ILOCK (bit 18) */
        isync
        sync
#endif
        b       cacheEnableDone

cacheEnableDone:
	/* All processor types end up here */
	/* disable instruction and data translations in the MMU */
	sync
	mfmsr	r3			/* get the value in msr *
					/* clear bits IR and DR */
	
	rlwinm	r4, r3, 0, _PPC_MSR_BIT_DR+1, _PPC_MSR_BIT_IR - 1

	mtmsr	r4			/* set the msr */
	isync
	sync				/* SYNC */

	/* initialize the BAT register */

	li	p3,0	 		/* clear p0 */
	
	isync
	mtspr	IBAT0U,p3		/* SPR 528 (IBAT0U) */
	isync

	mtspr	IBAT0L,p3		/* SPR 529 (IBAT0L) */
	isync

	mtspr	IBAT1U,p3		/* SPR 530 (IBAT1U) */
	isync

	mtspr	IBAT1L,p3		/* SPR 531 (IBAT1L) */
	isync

	mtspr	IBAT2U,p3		/* SPR 532 (IBAT2U) */
	isync

	mtspr	IBAT2L,p3		/* SPR 533 (IBAT2L) */
	isync

	mtspr	IBAT3U,p3		/* SPR 534 (IBAT3U) */
	isync

	mtspr	IBAT3L,p3		/* SPR 535 (IBAT3L) */
	isync

	mtspr	DBAT0U,p3		/* SPR 536 (DBAT0U) */
	isync

	mtspr	DBAT0L,p3		/* SPR 537 (DBAT0L) */
	isync

	mtspr	DBAT1U,p3		/* SPR 538 (DBAT1U) */
	isync

	mtspr	DBAT1L,p3		/* SPR 539 (DBAT1L) */
	isync

	mtspr	DBAT2U,p3		/* SPR 540 (DBAT2U) */
	isync

	mtspr	DBAT2L,p3		/* SPR 541 (DBAT2L) */
	isync

	mtspr	DBAT3U,p3		/* SPR 542 (DBAT3U) */
	isync

	mtspr	DBAT3L,p3		/* SPR 543 (DBAT3L) */
	isync

	/* invalidate entries within both TLBs */

	li	p1,128
	xor	p0,p0,p0		/* p0 = 0    */
	mtctr	p1			/* CTR = 32  */

	isync				/* context sync req'd before tlbie */
sysALoop:
	tlbie	p0
	addi	p0,p0,0x1000		/* increment bits 15-19 */
	bdnz	sysALoop		/* decrement CTR, branch if CTR != 0 */
	sync				/* sync instr req'd after tlbie      */

	/* initialize Small Data Area (SDA) start address */

#if	FALSE				/* XXX TPR NO SDA for now */
	lis     r2, HI(_SDA2_BASE_)
	addi    r2, r2, LO(_SDA2_BASE_)

	lis     r13, HI(_SDA_BASE_)
	addi    r13, r13, LO(_SDA_BASE_)
#endif

	addi	sp, sp, -FRAMEBASESZ	/* get frame stack */
/*	li      r3, BOOT_WARM_AUTOBOOT */
	mr	r3, r31			/* restore startType parameter */
	b	usrInit			/* never returns - starts up kernel */
FUNC_END(_sysInit)


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


/*******************************************************************************
*
* sysMemProbeSup - sysBusProbe support routine
*
* This routine is called to try to read byte, word, or long, as specified
* by length, from the specified source to the specified destination.
*
* RETURNS: OK if successful probe, else ERROR

* STATUS sysMemProbeSup
*     (
*     int         length, /@ length of cell to test (1, 2, 4) @/
*     char *      src,    /@ address to read @/
*     char *      dest    /@ address to write @/
*     )

*/

FUNC_BEGIN(sysMemProbeSup)
        addi    p7, p0, 0       /* save length to p7 */
        xor     p0, p0, p0      /* set return status */
        cmpwi   p7, 1           /* check for byte access */
        bne     sbpShort        /* no, go check for short word access */
        lbz     p6, 0(p1)       /* load byte from source */
        stb     p6, 0(p2)       /* store byte to destination */
        isync                   /* enforce for immediate exception handling */
        blr
sbpShort:
        cmpwi   p7, 2           /* check for short word access */
        bne     sbpWord         /* no, check for word access */
        lhz     p6, 0(p1)       /* load half word from source */
        sth     p6, 0(p2)       /* store half word to destination */
        isync                   /* enforce for immediate exception handling */
        blr
sbpWord:
        cmpwi   p7, 4           /* check for short word access */
        bne     sysProbeExc     /* no, check for double word access */
        lwz     p6, 0(p1)       /* load half word from source */
        stw     p6, 0(p2)       /* store half word to destination */
        isync                   /* enforce for immediate exception handling */
        blr
sysProbeExc:
        li      p0, -1          /* shouldn't ever get here, but... */
        blr

FUNC_END(sysMemProbeSup)

/*******************************************************************************
*
* sysPciErr - Error return for PCI exception trap handler
*
* This routine is called from sysPciTrap if the PCI configuration access
* generates an exception.  This code sets the return value to -1.
*
* NOTE:  
* The Tsi108 host bridge causes machine check exceptions to
* occur when PCI configuration reads are issued to a non-present device.
*
* NOMANUAL
*/

FUNC_BEGIN(sysPciErr)
	li	p0, -1
	blr
FUNC_END(sysPciErr)

#ifdef INCLUDE_CACHE_L2
#include "sysL2Cache.s"
#endif
