/* sysALib.s - Freescale Lite5200B system-dependent assembly routines */

/*
 * Copyright (c) 2003-2004, 2006, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,10sep09,x_z  remove vxSVRGet()
01e,03feb06,pkr  add read function for SVR
01d,24jan06,pkr  remove VWARE
01c,22dec04,k_p  updated Copyright information.
01b,22dec04,k_p  teamF1,Update for vxWorks 6.0
01a,25Jun03,pkr written
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
initial stack pointer (to STACK_ADRS which is defined in configAll.h),
then jumps to usrInit.
Most other hardware and device initialization is performed later by
sysHwInit().
*/

#define _ASMLANGUAGE

/* includes */
#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "regs.h"
#include "asm.h"


        /* globals */

        FUNC_EXPORT(_sysInit)       /* start of system code */
        FUNC_EXPORT(vxMBarGet)
        FUNC_EXPORT(vxMBarSet)
        FUNC_EXPORT(sysInByte)
        FUNC_EXPORT(sysOutByte)
        FUNC_EXPORT(sysInWord)
        FUNC_EXPORT(sysOutWord)
        FUNC_EXPORT(sysInLong)
        FUNC_EXPORT(sysOutLong)
        FUNC_EXPORT(sysPciInByte)
        FUNC_EXPORT(sysPciOutByte)
        FUNC_EXPORT(sysPciInWord)
        FUNC_EXPORT(sysPciOutWord)
        FUNC_EXPORT(sysPciInLong)
        FUNC_EXPORT(sysPciOutLong)
        FUNC_EXPORT(sysClearBATsInvalidateTLBs)
        FUNC_EXPORT(sysInvalidateTLBs)
        FUNC_EXPORT(sysMinimumBATsInit)

    /* externals */

    FUNC_IMPORT(usrInit)

    _WRS_TEXT_SEG_START
/***************************************************************************
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
* SYNOPSIS
* \ss
* sysInit
*     (
*     void     /@ THIS IS NOT A CALLABLE ROUTINE @/
*     )
* \se
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*/

FUNC_BEGIN(_sysInit)


        /* save startType */

        or      r31,r3,r3

    /* disable external interrupts (by zeroing out msr) */

    xor r5,r5,r5
    isync
    mtmsr   r5
    isync

/* define the macro USE_EMULATOR if the VxWorks image is loaded from the emulator
 * and if the MBAR is at reset value (0x80000000).
 */
#undef USE_EMULATOR
#ifdef USE_EMULATOR
        /* Setup MBAR mirror register. MBAR is at 0x80000000 after reset. */
    lis r3,HI(MBAR_RESET_VALUE)
    ori r3,r3,LO(MBAR_RESET_VALUE)
    mtspr   MBAR,r3

        /* store new MBAR value to MBAR */
        lis     r4,LO(MBAR_VALUE)     /* this is to load the high address part into the LSB */
        ori     r4,r4,HI(MBAR_VALUE)
        stw     r4,MBAR_IPBI(r3)
        eieio

        /* setup new MBAR */
    lis r3,HI(MBAR_VALUE)
    ori r3,r3,LO(MBAR_VALUE)
    mtspr   MBAR,r3
#endif

        /* invalidate and disable the MPU's data/instruction caches */
    mfspr   r6,HID0
    ori r5,r5,(_PPC_HID0_ICE | _PPC_HID0_DCE)
    andc    r6,r6,r5    /* clear cache enable bits in r6 */
    mr  r5,r6
    ori r5,r5,(_PPC_HID0_ICE | _PPC_HID0_DCE | _PPC_HID0_ICFI | _PPC_HID0_DCFI)
        sync                    /* r5 has DCE,ICE,ICFI,DCFI set */
    mtspr   HID0,r5     /* invalidate both caches with 2 stores */
    mtspr   HID0,r6     /* leaving them both disabled */
    isync

    mtspr   SPRG0,r3
    mtspr   SPRG1,r4
    bl  sysClearBATs
    bl  sysInvalidateTLBs
    bl  sysClearSegs
    mfspr   r3,SPRG0
    mfspr   r4,SPRG1

    /* Zero-out registers: r0 & SPRGs */
        xor     r0,r0,r0
    mtspr   SPRG0,r0
    mtspr   SPRG1,r0
    mtspr   SPRG2,r0
    mtspr   SPRG3,r0

    /* Turn on FP (temporarily) */
        ori r3,r0,_PPC_MSR_FP
        mtmsr   r3
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
        mflr    r3
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
     * Turn on FP and recoverable interrupt RI.
     * Do not enable Machine Check until the vectors are set up.
     */
    ori r4,r0,(_PPC_MSR_FP | _PPC_MSR_RI)
    mtmsr   r4
    sync

        /* fake a Decrementer Exception Handler (i.e. rfi @ 0x900) */
    lis     r3,HI(0x4C000064)
    ori r3,r3,LO(0x4C000064)    /* load rfi (0x4c000064) to */
    stw     r3,0x900(r0)        /* store rfi at 0x00000900 */

    /* PKR bl   sysMinimumBATsInit */

#if FALSE                   /* XXX TPR NO SDA for now */
    /* initialize Small Data Area (SDA) start address */
    lis     r2, HI(_SDA2_BASE_)
    ori     r2, r2, LO(_SDA2_BASE_)

    lis     r13, HI(_SDA_BASE_)
    ori r13, r13, LO(_SDA_BASE_)
#endif

    /* initialize the stack pointer */
    lis sp, HI(RAM_LOW_ADRS)    /* NOTE:  sp = r1 */
    ori sp, sp, LO(RAM_LOW_ADRS)
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

    /* li   r3, BOOT_WARM_AUTOBOOT */

        /* Jump to usrInit with arg indicating type of boot (startType) */

        or     r3,r31,r31

    isync
    b   usrInit     /* never returns - starts up kernel */
                /* (jumps to usrConfig.c )          */
FUNC_END(_sysInit)


/******************************************************************************
*
* vxMBarGet - Get the current MBAR value
*/

FUNC_BEGIN(vxMBarGet)
    mfspr   p0, MBAR
    blr
FUNC_END(vxMBarGet)


/******************************************************************************
*
* vxMBarSet - Set the new MBAR value
*/

FUNC_BEGIN(vxMBarSet)
    mtspr   MBAR, p0
    blr
FUNC_END(vxMBarSet)

#include "sysCacheLockALib.s"

/***************************************************************************
*
* sysClearBATsInvalidateTLBs - clear the BAT's register and invalidate the TLB's
*
* This routine will clear the BAT's register & invalidate the TLB's register.
*
* SYNOPSIS
* \ss
* void sysClearBATsInvalidateTLBs
*     (
*     void
*     )
* \se
*
* SEE ALSO: sysClearBATs(), sysInvalidateTLBs(), sysMinimumBATsInit()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysClearBATsInvalidateTLBs)
    mfmsr   r3
    xor r4,r4,r4
    ori r4,r4,0x30
    andc    r3,r3,r4
    sync
    mtmsr   r3
    isync
    mflr    r4
    bl  sysClearBATs
    bl  sysInvalidateTLBs
    mtlr    r4
    blr
FUNC_END(sysClearBATsInvalidateTLBs)

/***************************************************************************
*
* sysClearBATs - clear all the BAT's register
*
* This routine will zero the BAT's register.
*
* SYNOPSIS
* \ss
* void sysClearBATs
*     (
*     void
*     )
* \se
*
* SEE ALSO: sysClearBATsInvalidateTLBs(), sysInvalidateTLBs(), sysMinimumBATsInit()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysClearBATs)

        /* zero out the BAT registers */
    xor r3,r3,r3
    isync
    mtspr   IBAT0U,r3   /* clear all upper BATS first */
    mtspr   IBAT1U,r3
    mtspr   IBAT2U,r3
    mtspr   IBAT3U,r3
    mtspr   IBAT4U,r3
    mtspr   IBAT5U,r3
    mtspr   IBAT6U,r3
    mtspr   IBAT7U,r3
    mtspr   DBAT0U,r3
    mtspr   DBAT1U,r3
    mtspr   DBAT2U,r3
    mtspr   DBAT3U,r3
    mtspr   DBAT4U,r3
    mtspr   DBAT5U,r3
    mtspr   DBAT6U,r3
    mtspr   DBAT7U,r3

    mtspr   IBAT0L,r3   /* then clear lower BATS */
    mtspr   IBAT1L,r3
    mtspr   IBAT2L,r3
    mtspr   IBAT3L,r3
    mtspr   IBAT4L,r3
    mtspr   IBAT5L,r3
    mtspr   IBAT6L,r3
    mtspr   IBAT7L,r3
    mtspr   DBAT0L,r3
    mtspr   DBAT1L,r3
    mtspr   DBAT2L,r3
    mtspr   DBAT3L,r3
    mtspr   DBAT4L,r3
    mtspr   DBAT5L,r3
    mtspr   DBAT6L,r3
    mtspr   DBAT7L,r3
    isync
    blr
FUNC_END(sysClearBATs)

/***************************************************************************
*
* sysClearSegs - clearing all the SEG's register.
*
* This routine will zero the SEG's register.
*
* SYNOPSIS
* \ss
* void sysClearSegs
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysClearSegs)

    /* Init the Segment registers */
    xor r3, r3, r3
        isync
        mtsr    0,r3
        mtsr    1,r3
        mtsr    2,r3
        mtsr    3,r3
        mtsr    4,r3
        mtsr    5,r3
        mtsr    6,r3
        mtsr    7,r3
        mtsr    8,r3
        mtsr    9,r3
        mtsr    10,r3
        mtsr    11,r3
        mtsr    12,r3
        mtsr    13,r3
        mtsr    14,r3
        mtsr    15,r3
        isync
    blr
FUNC_END(sysClearSegs)

/***************************************************************************
*
* sysInvalidateTLBs - invalidate all the BAT's register
*
* This routine will invalidate the BAT's register.
*
* SYNOPSIS
* \ss
* void sysInvalidateTLBs
*     (
*     void
*     )
* \se
*
* SEE ALSO: sysClearBATs(), sysClearBATsInvalidateTLBs(), sysMinimumBATsInit()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysInvalidateTLBs)

    isync
    /* invalidate entries within both TLBs */
    li  r3,128
    mtctr   r3      /* CTR = 32  */
    xor r3,r3,r3    /* r3 = 0    */
    isync           /* context sync req'd before tlbie */
tlbloop:
    tlbie   r3
    sync            /* sync instr req'd after tlbie      */
    addi    r3,r3,0x1000    /* increment bits 15-19 */
    bdnz    tlbloop     /* decrement CTR, branch if CTR != 0 */
    isync
    blr
FUNC_END(sysInvalidateTLBs)

/***************************************************************************
*
* sysMinimumBATsInit - initialize the minimum BAT's register
*
* This routine will initialize the minimum BAT's register.
*
* SYNOPSIS
* \ss
* void sysMinimumBATsInit
*     (
*     void
*     )
* \se
*
* NOTE: When the MMU is disabled, the processor is said to be in Real Addressing
*       Mode. In this mode, all memory accesses are governed by a default set of
*       bit values for the WIMG attribute bits. For data accesses, the default
*       WIMG = 0011 and for instruction fetches default WIMG=0001. In both cases,
*       the guarded (G) bit is set and the cache-inhibit (I) bit is clear. In
*       other words, in real addressing mode, the entire address space of the
*       processor is cacheable ("NOT cache-inhibited") and guarded -- independent
*       of whether the caches are enabled or not.
*
*       The guarded attribute merely prevents out-of-order and speculative
*       *loads*. More details are avalible in section 5.2.1.5 of the PowerPC
*       programming environments manual for a more complete explanation of the
*       guarded attribute. While guarded is a necessary condition for those
*       memory spaces in which devices reside, it is not a sufficient condition.
*       Memory accesses to devices must be both guarded and cache inhibited.
*       Physically disabling the data cache does not provide this second,
*       equally necessary, condition.
*
*       Above, I used the term "NOT cache-inhibited" to draw attention to an
*       important distinction between cache-inhibited memory accesses and
*       cacheable memory accesses that are made while the cache itself is
*       disabled. A naive interpretation of "cacheability" holds that these two
*       concepts are equivalent -- they are not. To prevent out-of-order
*       *stores* to devices, we must mark the memory addresses at which those
*       devices reside as cache inhibited. The only way to do this is to enable
*       the MMU. So it holds that in order to enforce in-order loads AND stores,
*       we must enable the MMU and mark the appropriate memory regions as CI & G.
*
* SEE ALSO: sysClearBATs(), sysInvalidateTLBs(), sysClearBATsInvalidateTLBs()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysMinimumBATsInit)

    lis     r3, HI(0x00000032)
    ori     r3, r3, LO(0x00000032)
    sync
    mtspr   DBAT0L,r3
    isync

    lis     r3, HI(0x000007FF)  /* SDRAM (64M block) */
    ori     r3, r3, LO(0x000007FF)
    sync
    mtspr   DBAT0U,r3
    isync

    lis     r3, HI(0xF000002A)
    ori     r3, r3, LO(0xF000002A)
    sync
    mtspr   DBAT1L,r3
    isync

    lis     r3, HI(0xF0001FFF)  /* Pheripherals (256M block) */
    ori     r3, r3, LO(0xF0001FFF)  /* (eeprom,serial,ictrl)    */
    sync                /* (PCI register space)     */
    mtspr   DBAT1U,r3
    isync

    lis     r3, HI(0x8000002A)
    ori     r3, r3, LO(0x8000002A)
    sync
    mtspr   DBAT2L,r3
    isync

    lis     r3, HI(0x800001FF)  /* 0x80000000 (16M block) */
    ori     r3, r3, LO(0x800001FF)
    sync
    mtspr   DBAT2U,r3
    isync

    /* Turn on Data Relocation */
    sync
    mfmsr   r3
    ori     r3, r3, _PPC_MSR_DR
    sync
    mtmsr   r3
    isync
    blr
FUNC_END(sysMinimumBATsInit)

/**********************************************************************
* General system Input/Output ASM Routines
*
* If  INCLUDE_C_IO_ROUTINES is not defined, then it is assumed these
* routines are supplied in assembler code ( typically in sysALib.s ).
*
*/

#ifndef INCLUDE_C_IO_ROUTINES
/***************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* SYNOPSIS
* \ss
* UCHAR sysPciInByte
*     (
*     ULONG address
*     )
* \se
*
* SEE ALSO: sysPciInWord(), sysPciInLong(), sysPciOutByte(), sysPciOutWord(),
*           sysPciOutLong()
*
* RETURNS: byte from address.
*/

FUNC_BEGIN(sysInByte)
FUNC_BEGIN(sysPciInByte)
    lbzx    r3,r0,r3    /* Read byte from PCI space */
    sync
    bclr    20,0        /* Return to caller */
FUNC_END(sysPciInByte)
FUNC_END(sysInByte)

/***************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* SYNOPSIS
* \ss
* void sysPciOutByte
*     (
*     ULONG address
*     UCHAR data
*     )
* \se
*
* SEE ALSO: sysPciInByte(), sysPciInWord(), sysPciInLong(), sysPciOutWord(),
*           sysPciOutLong()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutByte)
FUNC_BEGIN(sysPciOutByte)
    stbx    r4,r0,r3    /* Write a byte to PCI space */
    sync
    bclr    20,0        /* Return to caller */
FUNC_END(sysPciOutByte)
FUNC_END(sysOutByte)

/***************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.  It uses the load halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* USHORT sysPciInWord
*     (
*     ULONG address
*     )
* \se
*
* SEE ALSO: sysPciInByte(), sysPciInLong(), sysPciOutByte(), sysPciOutWord(),
*           sysPciOutLong()
*
* RETURNS: word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysInWord)
FUNC_BEGIN(sysPciInWord)
    lhbrx   r3,r0,r3    /* Read and swap */
    sync
    bclr    20,0
FUNC_END(sysPciInWord)
FUNC_END(sysInWord)

/***************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.  It uses the store halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysPciOutWord
*     (
*     ULONG  address
*     USHORT data
*     )
* \se
*
* SEE ALSO: sysPciInByte(), sysPciInWord(), sysPciInLong(), sysPciOutByte(),
*           sysPciOutLong()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutWord)
FUNC_BEGIN(sysPciOutWord)
    sthbrx  r4,r0,r3    /* Write with swap to address */
    sync
    bclr    20,0        /* Return to caller */
FUNC_END(sysPciOutWord)
FUNC_END(sysOutWord)

/***************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.  It uses the load word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* ULONG sysPciInLong
*     (
*     ULONG address
*     )
* \se
*
* SEE ALSO: sysPciInByte(), sysPciInWord(), sysPciOutByte(), sysPciOutWord(),
*           sysPciOutLong()
*
* RETURNS: long (32-bit big-endian) from address.
*/

FUNC_BEGIN(sysInLong)
FUNC_BEGIN(sysPciInLong)
    lwbrx   r3,r0,r3    /* Read and swap from address */
    sync
    bclr    20,0        /* Return to caller */
FUNC_END(sysPciInLong)
FUNC_END(sysInLong)

/***************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.  It uses the store word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysPciOutLong
*     (
*     ULONG address
*     ULONG data
*     )
* \se
*
* SEE ALSO: sysPciInByte(), sysPciInWord(), sysPciInLong(), sysPciOutWord(),
*           sysPciOutByte()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutLong)
FUNC_BEGIN(sysPciOutLong)
    stwbrx  r4,r0,r3    /* store data as little-endian */
    sync
    bclr    20,0
FUNC_END(sysPciOutLong)
FUNC_END(sysOutLong)

#endif /* INCLUDE_C_IO_ROUTINES */
