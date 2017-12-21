/* sysALib.s - Motorola ads834x system-dependent assembly routines */

/*
 * Copyright (c) 2005, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,31dec09,x_z  code clean.
01c,10sep09,x_z  move vxSvrGet() to vxALib.s.
01b,15aug05,dtr  support startType.
01a,15jan05,dtr  written from ads826x sysALib.s
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
#include <cacheLib.h>
#include <arch/ppc/cachePpcLib.h>
#include "config.h"
#include <regs.h>
#include <sysLib.h>

    /* defines */

    /* Invalidate both cachesinvalidate bits */

#   define M83XX_HID0_INIT_VAL  (_PPC_HID0_ICFI | _PPC_HID0_DCFI | \
                                 _PPC_HID0_PAR | _PPC_HID0_EMCP)

    /* globals */

    FUNC_EXPORT(_sysInit)      /* start of system code */
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
*
* sysInit (void)              /@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

FUNC_BEGIN(_sysInit)
    mr      r11, r3  /* store startType in r7 */

    /* clear the MSR */

    xor     r3, r3, r3
    sync
    mtmsr   r3
    isync

    /* clear the SPRGs */

    xor     r0, r0, r0
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

    mflr    r4          /* save link register so we can use it for */
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

    mtmsr   r3  /* restore saved value */
    isync

    /* return from decrementer exceptions */

    addis   r4, r0, 0x4c00
    addi    r4, r4, 0x0064  /* load rfi (0x4c000064) to r4 */
    stw     r4, 0x900(r0)   /* store rfi at 0x00000900 */

    /* initialize the stack pointer */

    lis     sp, HI(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)

    /* initialize the BAT register */

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
    lis     r3, HIADJ(M83XX_HID0_INIT_VAL)
    ori     r3, r3, LO(M83XX_HID0_INIT_VAL)
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
    mr      r3, r11                 /* load startType */
    /* jump to usrInit */
    b   usrInit                     /* never returns - starts up kernel */
FUNC_END(_sysInit)

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
        mtspr   HID2, r3
        blr
FUNC_END(vxHid2Set)

