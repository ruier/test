/* sysALib.s - Assembly support file and init routines */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
01a,10jul09,x_z  derived from version 01b of fsl_mds8568/sysALib.s.
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language. It contains the entry code, sysInit(), for VxWorks images
that start running from RAM, such as 'vxWorks'. These images are
loaded into memory by some external program (e.g., a boot ROM) and then
started. The routine sysInit() must come first in the text segment.
Its job is to perform the minimal setup needed to call the generic C
routine usrInit().

sysInit() masks interrupts in the processor and the interrupt
controller and sets the initial stack pointer.  Other hardware and
device initialization is performed later in the sysHwInit routine in
sysLib.c.

NOTE
The routines in this module don't use the "C" frame pointer or
establish a stack frame.

INCLUDE FILES: sysLib.h asm.h mmuE500Lib.h

SEE ALSO:
\tb "PowerPC e500 CoreFamily Reference Manual",
\tb "MPC8569E PowerQUICC III Integrated Processor Reference Manual",
\tb "MPC8569E-MDS-PB HW User Guide"
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include "config.h"
#include <sysLib.h>
#include <sysL2Cache.h>
#include <arch/ppc/mmuE500Lib.h>

#define CACHE_ALIGN_SHIFT   5   /* Cache line size == 2^5 */

    /* globals */

    FUNC_EXPORT(sysInit)
    FUNC_EXPORT(_sysInit)

    FUNC_EXPORT(vxL2CTLSet)
    FUNC_EXPORT(vxL2CTLGet)
    FUNC_EXPORT(sysCacheFlush)
    FUNC_EXPORT(sysTas)

    /* externals */

    FUNC_IMPORT(usrInit)

    _WRS_TEXT_SEG_START

    DATA_IMPORT(inFullVxWorksImage)

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up the
* stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().
* This stack is used only by usrInit() and is never used again. Memory
* for the stack must be accounted for when determining the system load
* address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*
* sysInit (sysStartType)    /@ THIS IS NOT A CALLABLE ROUTINE @/
*/

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)

    mr      r8, r3
    xor     r3, r3,r3
    mtspr   TCR, r3
    xor     r6, r6, r6
    msync
    isync

    mtspr   L1CSR0, r6          /* Disable the Data cache           */
    li      r6, _PPC_L1CSR_FI
    msync
    isync

    mtspr   L1CSR0, r6          /* Invalidate the Data cache        */
    li      r6, 0x0000
    msync
    isync

    mtspr   L1CSR1, r6          /* Disable the Instruction cache    */
    li      r6, _PPC_L1CSR_FI
    msync
    isync

    mtspr   L1CSR1, r6          /* Invalidate the Instruction cache */
    isync

    li      r6, _PPC_L1CSR_E
    msync
    isync

    mtspr   L1CSR1, r6          /* Enable the Instruction cache     */
    msync
    isync

    mbar 0

    lis     r6, HIADJ(inFullVxWorksImage)
    addi    r6, r6,LO(inFullVxWorksImage)
    li      r7, TRUE
    stw     r7, 0(r6)

    /* initialize the stack pointer */

    lis     sp, HIADJ(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
    mr      r3, r8

    b usrInit
FUNC_END(sysInit)

    .balign 32
/*******************************************************************************
*
* vxL2CTLSet - Set the value of the L2 cache control register
*
* This routine writes the value to the L2 cache control register and returns the
* value written.
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
* RETURNS: the value in the L2CTL reg
*/

FUNC_BEGIN(vxL2CTLSet)
    mbar    0
    isync
    stw     p0, 0(p1)
    lwz     p0, 0(p1)
    mbar    0
    isync
    blr
FUNC_END(vxL2CTLSet)

    .balign 32
/*******************************************************************************
*
* vxL2CTLGet - Get the value of the L2 cache control register
*
* This routine gets the value of the L2 cache control register.
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLGet
*     (
*     UINT32 *addr
*     )
* \se
*
* RETURNS: the value in the L2CTL reg
*/

FUNC_BEGIN(vxL2CTLGet)
    mbar    0
    isync
    lwz     p1, 0x0(p0)
    addi    p0, p1, 0x0
    mbar    0
    isync
    blr
FUNC_END(vxL2CTLGet)

/*******************************************************************************
 * sysCacheFlush just flushes cache - assume int lock
 *
 * This routine flushes cache - assume int lock.
 *
* SYNOPSIS
* \ss
* void sysCacheFlush
*     (
*     int    lineNum,   /@ cache line num   @/
*     char * origBuf,   /@ buffer origin    @/
*     int *  alignSize, /@ cache align size @/
*     )
* \se
*
* RETURNS: N/A
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

    srwi    p3,  p0, MMU_RPN_SHIFT - CACHE_ALIGN_SHIFT
    mtspr   CTR, p3
    addi    p6,  p1,0
    li      p5,  MMU_PAGE_SIZE
    subf    p3,  p5,p1

    /*
     * There might be a page boundary between here and the end of
     * the function, so make sure both pages are in the I-TLB.
     */

    b   cacheL2DisableLoadItlb
cacheL2DisableLoadDtlb:
    add     p3, p3, p5
    lbzu    p4, 0(p3)
    bdnz    cacheL2DisableLoadDtlb
    mtctr   p0              /* Load counter with number of cache lines  */
    subf    p1, p2, p1      /* buffer points to text  - cache line size */
l2DisableFlush:
    add p1, p2, p1          /* +  cache line size                       */
    lbzu    p3, 0x0(p1)     /* flush the data cache block               */
    bdnz    l2DisableFlush  /* loop till cache ctr is zero              */
    sync
    isync
    mtctr   p0              /* Load counter with number of cache lines  */
    addi    p1, p6, 0
    subf    p1, p2, p1      /* buffer points to text  - cache line size */
l2DisableClear:
    add     p1, p2, p1      /* point to next cache line                 */
    dcbf    0, p1           /* flush newly-loaded line                  */
    bdnz    l2DisableClear  /* repeat for all sets and ways             */
    sync
    isync
    blr
cacheL2DisableLoadItlb:
    b   cacheL2DisableLoadDtlb
FUNC_END(sysCacheFlush)

/*******************************************************************************
*
* vxTas - perform the atomic test and set for the PowerPC arch
*
* This routine performs the atomic test and set for the PowerPC arch.
*
* RETURN: N/A
*/

FUNC_BEGIN(sysTas)
    lis     r4, 0x8000  /* set the upper bit of r4 */
    ori     r4, r4, 0x0000
    eieio           /* simple ordered store using eieio */

    lwarx   r5, 0, r3   /* load and reserve */
    cmpwi   r5, 0       /* done if word */
    bne sysTasEnd   /* not equal to 0 */

    stwcx.  r4, 0, r3   /* try to store non-zero */
    eieio           /* preserve load/store order */
    bne-    sysTas
    li  r3, 0x01
    blr
sysTasEnd:
    li  r3, 0
    blr
FUNC_END(sysTas)

