/* sysALib.s - Freescale ADS5121e system-dependent assembly routines */

/*
 * Copyright (c) 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,10sep09,x_z  move vxSvrGet() to vxALib.s
01a,07sep07,b_m  derived from version 01b of ads834x/sysALib.s.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <cacheLib.h>
#include <arch/ppc/cachePpcLib.h>
#include "config.h"

    /* globals */

    FUNC_EXPORT(_sysInit)       /* start of system code */
    FUNC_EXPORT(sysMsrGet)      /* get the value of the MSR register */
    FUNC_EXPORT(sysInByte)
    FUNC_EXPORT(sysInWord)
    FUNC_EXPORT(sysInLong)
    FUNC_EXPORT(sysOutByte)
    FUNC_EXPORT(sysOutWord)
    FUNC_EXPORT(sysOutLong)
    FUNC_EXPORT(sysPciInByte)
    FUNC_EXPORT(sysPciOutByte)
    FUNC_EXPORT(sysPciInWord)
    FUNC_EXPORT(sysPciOutWord)
    FUNC_EXPORT(sysPciInLong)
    FUNC_EXPORT(sysPciOutLong)
    FUNC_EXPORT(vxHid2Get)
    FUNC_EXPORT(vxHid2Set)

    /* externals */

    FUNC_IMPORT(usrInit)

    _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  This code does the following:
*
* .CS
*     sets the MSR to all 0 which, among other things:
*         disables external interrupts
*         disables machine check interrupts
*         selects exception prefix 0x000n_nnnn
*         disables instruction and data address translation
*     clears the SPRGs
*     clears the segment registers
*     places an rfi instruction at the decrementer exception vector
*     initializes the stack pointer
*     disables instruction and data translations in the MMU
*     initializes the BAT registers
*     invalidates all entries in instruction and data TLBs
*     clear HID0 , among other things:
*         disables instruction and data caches
*     invalidates instruction and data caches
*     sets up the stack
* .CE
*
* At the end it jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*
* sysInit (void)            /@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

FUNC_BEGIN(_sysInit)

    mr      r7 ,r3          /* store startType in r7 */

    xor     r3, r3, r3
    sync

    /* clear MSR */

    mtmsr   r3
    isync

    /* clear SPRGs */

    mtspr   SPRG0, r3
    mtspr   SPRG1, r3
    mtspr   SPRG2, r3
    mtspr   SPRG3, r3
    isync

    /* clear segment registers */

    mtsr    0, r3
    isync
    mtsr    1, r3
    isync
    mtsr    2, r3
    isync
    mtsr    3, r3
    isync
    mtsr    4, r3
    isync
    mtsr    5, r3
    isync
    mtsr    6, r3
    isync
    mtsr    7, r3
    isync
    mtsr    8, r3
    isync
    mtsr    9, r3
    isync
    mtsr    10, r3
    isync
    mtsr    11, r3
    isync
    mtsr    12, r3
    isync
    mtsr    13, r3
    isync
    mtsr    14, r3
    isync
    mtsr    15, r3
    isync

    /* clear SDR1 */

    mtspr   25, r3
    isync

    /* initialize the FPU's registers */

    /* save current value of MSR */
    mfmsr   r3

    /* Turn on FP */
    li      r4, _PPC_MSR_FP     /* turn on FP so we can init it */
    sync
    mtmsr   r4
    isync

    /* Init the floating point control/status register */
    mtfsfi  7, 0
    mtfsfi  6, 0
    mtfsfi  5, 0
    mtfsfi  4, 0
    mtfsfi  3, 0
    mtfsfi  2, 0
    mtfsfi  1, 0
    mtfsfi  0, 0
    isync

    /*
     * Initialize the floating point data registers to a known state.
     * First save the link register.  By doing a bl to ifpdrValue,
     * the link register has the address of the IEEE value for 1.0
     * The lr can now be moved to a register for use as a pointer for
     * the IEEE value of 1.0
     */

    mflr    r4      /* save link register so we can use it for */
                    /* initializing the fp registers with a good number */

    bl      ifpdrValue  /* link register now has pointer to IEEE value */
    .long   0x3f800000  /* IEEE single-precision value of 1.0 */

ifpdrValue:
    mflr    r5          /* move current lr to r3 for use as pointer */
    mtlr    r4          /* restore lr to previous value */
    lfs     f0, 0(r5)   /* load all 32 FP regs with 1.0 */
    lfs     f1, 0(r5)
    lfs     f2, 0(r5)
    lfs     f3, 0(r5)
    lfs     f4, 0(r5)
    lfs     f5, 0(r5)
    lfs     f6, 0(r5)
    lfs     f7, 0(r5)
    lfs     f8, 0(r5)
    lfs     f9, 0(r5)
    lfs     f10, 0(r5)
    lfs     f11, 0(r5)
    lfs     f12, 0(r5)
    lfs     f13, 0(r5)
    lfs     f14, 0(r5)
    lfs     f15, 0(r5)
    lfs     f16, 0(r5)
    lfs     f17, 0(r5)
    lfs     f18, 0(r5)
    lfs     f19, 0(r5)
    lfs     f20, 0(r5)
    lfs     f21, 0(r5)
    lfs     f22, 0(r5)
    lfs     f23, 0(r5)
    lfs     f24, 0(r5)
    lfs     f25, 0(r5)
    lfs     f26, 0(r5)
    lfs     f27, 0(r5)
    lfs     f28, 0(r5)
    lfs     f29, 0(r5)
    lfs     f30, 0(r5)
    lfs     f31, 0(r5)
    sync

    /*  Restore MPU/MSR to a known state  */

    mtmsr  r3   /* restore saved value */
    isync

    /* return from decrementer exceptions */

    addis   r4, r0, 0x4c00
    addi    r4, r4, 0x0064          /* load rfi (0x4c000064) to r4  */
    stw     r4, 0x900(r0)           /* store rfi at 0x00000900      */

    /* initialize the stack pointer */

    lis     sp, HI(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)

    /* initialize BAT registers */

    li      p3, 0           /* clear r3 */
    isync

    mtspr   IBAT0U, p3      /* SPR 528 (IBAT0U) */
    isync

    mtspr   IBAT0L, p3      /* SPR 529 (IBAT0L) */
    isync

    mtspr   IBAT1U, p3      /* SPR 530 (IBAT1U) */
    isync

    mtspr   IBAT1L, p3      /* SPR 531 (IBAT1L) */
    isync

    mtspr   IBAT2U, p3      /* SPR 532 (IBAT2U) */
    isync

    mtspr   IBAT2L, p3      /* SPR 533 (IBAT2L) */
    isync

    mtspr   IBAT3U, p3      /* SPR 534 (IBAT3U) */
    isync

    mtspr   IBAT3L, p3      /* SPR 535 (IBAT3L) */
    isync

    mtspr   DBAT0U, p3      /* SPR 536 (DBAT0U) */
    isync

    mtspr   DBAT0L, p3      /* SPR 537 (DBAT0L) */
    isync

    mtspr   DBAT1U, p3      /* SPR 538 (DBAT1U) */
    isync

    mtspr   DBAT1L, p3      /* SPR 539 (DBAT1L) */
    isync

    mtspr   DBAT2U, p3      /* SPR 540 (DBAT2U) */
    isync

    mtspr   DBAT2L, p3      /* SPR 541 (DBAT2L) */
    isync

    mtspr   DBAT3U, p3      /* SPR 542 (DBAT3U) */
    isync

    mtspr   DBAT3L, p3      /* SPR 543 (DBAT3L) */
    isync

    /* invalidate entries within both TLBs */

    li      r4, 32
    mtctr   r4              /* CTR = 32  */
    xor     r3, r3, r3      /* r3 = 0    */

    isync                   /* context sync req'd before tlbie */
sysALoop:
    tlbie   r3
    addi    r3, r3, 0x1000  /* increment bits 15-19 */
    bdnz    sysALoop        /* decrement CTR, branch if CTR != 0 */
    sync                    /* sync instr req'd after tlbie      */

    /* clear HID0 */

    li      r3, 0
    sync
    isync
    mtspr   HID0, r3

    /*
     * Invalidate both caches by setting and then clearing the cache
     * invalidate bits
     */

    mfspr   r3, HID0
    lis     r3, HIADJ(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP)
    ori     r3, r3, LO(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP)
    sync
    isync
    mtspr   HID0, r3

    /* clear HID0, which clears cache invalidate bits */

    li      r3, 0
    sync
    isync
    mtspr   HID0, r3

    lis     sp, HIADJ(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
    mr      r3, r7                  /* load startType */
    b       usrInit                 /* never returns - starts up kernel */

FUNC_END(_sysInit)

/*******************************************************************************
*
* sysMsrGet - Get the value of the MSR (Machine State Register)
*
* This routine returns the value of the MSR.
*
* RETURNS: the Machine State Register (MSR) value.
*
* UINT32 sysMsrGet (void)
*
*/

FUNC_BEGIN(sysMsrGet)
    mfmsr   p0
    blr
FUNC_END(sysMsrGet)

/*******************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.
*
* UCHAR sysInByte
*     (
*     UCHAR *   pAddr       /@ Virtual I/O addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysInByte)
    lbzx    p0, r0, p0      /* Read byte from I/O space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysInByte)


/*******************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.
*
* USHORT sysInWord
*     (
*     ULONG address,        /@ addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysInWord)
    lhbrx   p0, r0, p0      /* Read and swap */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysInWord)

/*******************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*
* ULONG sysInLong
*     (
*     ULONG address,        /@ Virtual addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysInLong)
    lwbrx   p0, r0, p0      /* Read and swap from address */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysInLong)

/*******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A
*
* VOID sysOutByte
*     (
*     UCHAR *   pAddr,      /@ Virtual I/O addr to write to @/
*     UCHAR     data        /@ data to be written @/
*     )
*
*/

FUNC_BEGIN(sysOutByte)
    stbx    p1, r0, p0      /* Write a byte to PCI space */
    sync
    bclr    20, 0           /* Return to caller */
FUNC_END(sysOutByte)


/*******************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified
* address.
*
* RETURNS: N/A
*
* VOID sysOutWord
*     (
*     ULONG     address,    /@ Virtual addr to write to @/
*     UINT16    data        /@ Data to be written       @/
*     )
*
*/

FUNC_BEGIN(sysOutWord)
    sthbrx  p1, r0, p0      /* Write with swap to address */
    sync
    bclr    20, 0           /* Return to caller */
FUNC_END(sysOutWord)


/*******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A
*
* VOID sysOutLong
*     (
*     ULONG     address,    /@ Virtual addr to write to @/
*     ULONG     data        /@ Data to be written @/
*     )
*
*/

FUNC_BEGIN(sysOutLong)
    stwbrx  p1, r0, p0      /* store data as little-endian */
    sync
    bclr    20, 0
FUNC_END(sysOutLong)

/*******************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.
*
* UINT8 sysPciInByte
*     (
*     UINT8 *  pAddr,       /@ Virtual addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysPciInByte)
    lbzx    p0, r0, p0      /* Read byte from PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciInByte)

/*******************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.
*
* USHORT sysPciInWord
*     (
*     USHORT *  pAddr,      /@ Virtual addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysPciInWord)
    lhbrx   p0, r0, p0      /* Read and swap from PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciInWord)

/*******************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*
* ULONG sysPciInLong
*     (
*     ULONG *  pAddr,       /@ Virtual addr to read from @/
*     )
*
*/

FUNC_BEGIN(sysPciInLong)
    lwbrx   p0, r0, p0      /* Read and swap from PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciInLong)

/*******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* RETURNS: N/A
*
* VOID sysPciOutByte
*     (
*     UINT8 *   pAddr,      /@ Virtual addr to write to @/
*     UINT8     data        /@ Data to be written       @/
*     )
*
*/

FUNC_BEGIN(sysPciOutByte)
    stbx    p1, r0, p0      /* Write a byte to PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciOutByte)

/*******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A
*
* VOID sysPciOutWord
*     (
*     USHORT *  pAddr,      /@ Virtual addr to write to @/
*     USHORT    data        /@ Data to be written       @/
*     )
*
*/

FUNC_BEGIN(sysPciOutWord)
    sthbrx  p1, r0, p0      /* Write with swap to PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciOutWord)

/*******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A
*
* VOID sysPciOutLong
*     (
*     ULONG *   pAddr,      /@ Virtual addr to write to @/
*     ULONG     data        /@ Data to be written       @/
*     )
*
*/

FUNC_BEGIN(sysPciOutLong)
    stwbrx  p1, r0, p0      /* Write big-endian long to little-endian */
    mr      p0, p1          /* PCI space */
    bclr    20, 0           /* Return to caller */
FUNC_END(sysPciOutLong)


/*******************************************************************************
*
* vxHid2Get - Get the value of the HID2
*
* This routine returns the value of the HID2.
*
* RETURNS: the HID2 value.
*
* UINT32 vxHid2Get (void)
*
*/

FUNC_BEGIN(vxHid2Get)
    mfspr   r3, HID2
    blr
FUNC_END(vxHid2Get)

/*******************************************************************************
*
* vxHid2Set - Get the value of the HID2
*
* This routine returns the value of the HID2.
*
* RETURNS: the HID2 value.
*
* VOID vxHid2Set
*     (
*     UINT32    reg         /@ HID2 value to write to @/
*     )
*
*/

FUNC_BEGIN(vxHid2Set)
    mtspr   HID2, r3
    blr
FUNC_END(vxHid2Set)
