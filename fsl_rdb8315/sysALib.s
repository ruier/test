/* sysALib.s - Freescale MPC8315ERDB system-dependent assembly routines */

/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,02jan10,x_z  move vxSvrGet() to vxALib.s.
01b,11dec09,x_z  Code clean.
01a,09may08,z_l  Created, routines are derived from version 01a
                 of fsl_rdb8313/sysALib.s
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system.  The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include <arch/ppc/cachePpcLib.h>
#include "config.h"
#include <regs.h>
#include <sysLib.h>

    /* globals */

    FUNC_EXPORT(_sysInit)      /* start of system code */
    FUNC_EXPORT(sysInWord)
    FUNC_EXPORT(sysInByte)
    FUNC_EXPORT(sysInLong)
    FUNC_EXPORT(sysOutLong)
    FUNC_EXPORT(sysOutWord)
    FUNC_EXPORT(sysOutByte)
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
*     clear HID0, among other things:
*         disables instruction and data caches
*         invalidates instruction and data caches
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

* sysInit (void)              /@ THIS IS NOT A CALLABLE ROUTINE @/
*/

FUNC_BEGIN(_sysInit)

    mr      r7, r3  /* store startType in r7 */

    /* clear the MSR */

    xor    r3, r3, r3
    sync
    mtmsr  r3
    isync

    /* clear the SPRGs */

    xor     r0,r0, r0
    mtspr   SPRG0, r0
    mtspr   SPRG1, r0
    mtspr   SPRG2, r0
    mtspr   SPRG3, r0

    /* clear the Segment registers */

    andi.   r3, r3, 0
    isync
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

    mtspr   SDR1, r0
    isync

    /* return from decrementer exceptions */

    addis   r4, r0, 0x4c00
    addi    r4, r4, 0x0064          /* load rfi (0x4c000064) to r4 */
    stw     r4, 0x900(r0)           /* store rfi at 0x00000900     */

    /* initialize the stack pointer */

    lis     sp, HI(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)

    /* initialize the BAT register */

    li      p3, 0           /* clear r3 */

    isync
    mtspr    IBAT0U, p3      /* SPR 528 (IBAT0U) */
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

    mtspr   IBAT4U, p3      /* SPR 560 (IBAT4U) */
    isync

    mtspr   IBAT4L, p3      /* SPR 561 (IBAT4L) */
    isync

    mtspr   IBAT5U, p3      /* SPR 562 (IBAT5U) */
    isync

    mtspr   IBAT5L, p3      /* SPR 563 (IBAT5L) */
    isync

    mtspr   IBAT6U, p3      /* SPR 564 (IBAT6U) */
    isync

    mtspr   IBAT6L, p3      /* SPR 565 (IBAT6L) */
    isync

    mtspr   IBAT7U, p3      /* SPR 566 (IBAT7U) */
    isync

    mtspr   IBAT7L, p3      /* SPR 567 (IBAT7L) */
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

    mtspr   DBAT4U, p3      /* SPR 568 (DBAT4U) */
    isync

    mtspr   DBAT4L, p3      /* SPR 569 (DBAT4L) */
    isync

    mtspr   DBAT5U, p3      /* SPR 538 (DBAT5U) */
    isync

    mtspr   DBAT5L, p3      /* SPR 570 (DBAT5L) */
    isync

    mtspr   DBAT6U, p3      /* SPR 571 (DBAT6U) */
    isync

    mtspr   DBAT6L, p3      /* SPR 572 (DBAT6L) */
    isync

    mtspr   DBAT7U, p3      /* SPR 573 (DBAT7U) */
    isync

    mtspr   DBAT7L, p3      /* SPR 574 (DBAT7L) */
    isync

    /* invalidate entries within both TLBs */

    li      r4, 64
    mtctr   r4              /* CTR = 64  */
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
    mr      r3, r7 /* load startType */

    /* jump to usrInit */

    b    usrInit            /* never returns - starts up kernel */
FUNC_END(_sysInit)

/*******************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This routine reads a byte from a specified io address.
*
* RETURNS: byte from address.

* UCHAR sysInByte
*     (
*     UCHAR *  pAddr        /@ Virtual I/O addr to read from @/
*     )
*/

FUNC_BEGIN(sysInByte)
    sync
    lbzx    p0,r0,p0    /* Read byte from I/O space */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysInByte)

/*******************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This routine reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.

* USHORT sysInWord
*     (
*     ULONG  address,        /@ addr to read from @/
*     )
*/

FUNC_BEGIN(sysInWord)
    sync
    lhbrx   p0,r0,p0    /* Read and swap */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysInWord)

/*******************************************************************************
*
* sysInLong - reads a long from an address.
*
* This routine reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysInLong
*     (
*     ULONG  address,        /@ Virtual addr to read from @/
*     )
*/

FUNC_BEGIN(sysInLong)
    sync
    lwbrx   p0,r0,p0    /* Read and swap from address */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysInLong)

/*******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This routine writes a byte to a specified io address.
*
* RETURNS: N/A

* VOID sysOutByte
*     (
*     UCHAR *  pAddr,        /@ Virtual I/O addr to write to @/
*     UCHAR    data          /@ data to be written           @/
*     )
*/

FUNC_BEGIN(sysOutByte)
    sync
    stbx    p1,r0,p0    /* Write a byte to PCI space */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysOutByte)

/*******************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This routine writes a swapped word to a specified
* address.
*
* RETURNS: N/A

* VOID sysOutWord
*     (
*     ULONG address,        /@ Virtual addr to write to @/
*     UINT16  data          /@ Data to be written       @/
*     )
*/

FUNC_BEGIN(sysOutWord)
    sync
    sthbrx  p1,r0,p0        /* Write with swap to address */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysOutWord)

/*******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A

* VOID sysOutLong
*     (
*     ULONG   address,        /@ Virtual addr to write to @/
*     ULONG   data        /@ Data to be written @/
*     )
*/

FUNC_BEGIN(sysOutLong)
    sync
    stwbrx  p1,r0,p0    /* store data as little-endian */
    isync
    bclr    20,0
FUNC_END(sysOutLong)

/*******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This routine writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutLong
*     (
*     ULONG *  pAddr,    /@ Virtual addr to write to @/
*     ULONG  data        /@ Data to be written       @/
*     )
*/

FUNC_BEGIN(sysPciOutLong)
    sync
    stwbrx  p1,r0,p0    /* Write big-endian long to little-endian */
    mr      p0,p1       /* PCI space */
    isync
    bclr    20,0        /* Return to caller */
FUNC_END(sysPciOutLong)

/*******************************************************************************
*
* vxHid2Get - get the content of HID2
*
* This routine read the content of the HID2 register.
*
* RETURNS: the content of HID2
*/

FUNC_BEGIN(vxHid2Get)
    mfspr   r3, HID2
    blr
FUNC_END(vxHid2Get)

/*******************************************************************************
*
* vxHid2Set - set the content of HID2
*
* This routine takes a doubleword address argument and loads the HID2 register
* with the contents of that address
*
* RETURNS: N/A
*/

FUNC_BEGIN(vxHid2Set)
    mtspr   1011, r3
    blr
FUNC_END(vxHid2Set)

