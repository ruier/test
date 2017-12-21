/* sysALib.s - Motorola RDB8323 system-dependent assembly routines */

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
01d,10sep09,x_z  move vxSvrGet() to vxALib.s
01c,02jun09,e_d  code cleanup
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
#include <config.h>
#include <regs.h>
#include <sysLib.h>

    /* globals */

    FUNC_EXPORT(_sysInit)       /* start of system code */
    FUNC_EXPORT(sysMsrGet)      /* get the value of the MSR register */
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
* sysInit (void)              /@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

FUNC_BEGIN(_sysInit)

    mr  r7 ,r3  /* store startType in r7 */

    /* clear the MSR */

    xor r3, r3, r3
    sync
    mtmsr   r3
    isync

    /* clear the SPRGs */

    xor     r0,r0,r0
    mtspr   272,r0
    mtspr   273,r0
    mtspr   274,r0
    mtspr   275,r0

    /* clear the Segment registers */

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

    /* clear SDR1 */

    mtspr   25,r0
    isync

    /* return from decrementer exceptions */

    addis   r4, r0, 0x4c00
    addi    r4, r4, 0x0064          /* load rfi (0x4c000064) to r4      */
    stw     r4, 0x900(r0)           /* store rfi at 0x00000900          */

    /* initialize the stack pointer */

    lis     sp, HI(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)

    /* initialize the BAT register */

    li  p3,0            /* clear r3 */

    isync
    mtspr   IBAT0U,p3       /* SPR 528 (IBAT0U) */
    isync

    mtspr   IBAT0L,p3       /* SPR 529 (IBAT0L) */
    isync

    mtspr   IBAT1U,p3       /* SPR 530 (IBAT1U) */
    isync

    mtspr   IBAT1L,p3       /* SPR 531 (IBAT1L) */
    isync

    mtspr   IBAT2U,p3       /* SPR 532 (IBAT2U) */
    isync

    mtspr   IBAT2L,p3       /* SPR 533 (IBAT2L) */
    isync

    mtspr   IBAT3U,p3       /* SPR 534 (IBAT3U) */
    isync

    mtspr   IBAT3L,p3       /* SPR 535 (IBAT3L) */
    isync

    mtspr   DBAT0U,p3       /* SPR 536 (DBAT0U) */
    isync

    mtspr   DBAT0L,p3       /* SPR 537 (DBAT0L) */
    isync

    mtspr   DBAT1U,p3       /* SPR 538 (DBAT1U) */
    isync

    mtspr   DBAT1L,p3       /* SPR 539 (DBAT1L) */
    isync

    mtspr   DBAT2U,p3       /* SPR 540 (DBAT2U) */
    isync

    mtspr   DBAT2L,p3       /* SPR 541 (DBAT2L) */
    isync

    mtspr   DBAT3U,p3       /* SPR 542 (DBAT3U) */
    isync

    mtspr   DBAT3L,p3       /* SPR 543 (DBAT3L) */
    isync

    /* invalidate entries within both TLBs */

    li      r4,32
    mtctr   r4          /* CTR = 32  */
    xor     r3,r3,r3        /* r3 = 0    */

    isync               /* context sync req'd before tlbie */
sysALoop:
    tlbie   r3
    addi    r3,r3,0x1000        /* increment bits 15-19 */
    bdnz    sysALoop        /* decrement CTR, branch if CTR != 0 */
    sync                /* sync instr req'd after tlbie      */

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
    lis     r3, HIADJ(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP) /* set bits */
    ori     r3, r3, LO(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP) /* set bits */
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

    b   usrInit         /* never returns - starts up kernel */
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
* vxHid2Get - Read a UINT32 value from HID2 register.
*
* This function read a UINT32 value from HID2 registe
*
* RETURNS: the HID2 register value.
*
* UINT32 vxHid2Get()
*
*/

FUNC_BEGIN(vxHid2Get)
    mfspr   r3, 1011
    blr
FUNC_END(vxHid2Get)

/*******************************************************************************
*
* vxHid2Set - writes a UINT32 value to HID2 register.
*
* This function writes a UINT32 to HID2 register.
*
* RETURNS: N/A
*
* void vxHid2Set
*      (
*      UINT32       /@ Set value for Hid2 @/
*      )
*
*/

FUNC_BEGIN(vxHid2Set)
    mtspr   1011, r3
    blr
FUNC_END(vxHid2Set)
