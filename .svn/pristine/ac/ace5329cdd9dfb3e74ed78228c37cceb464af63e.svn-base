/* sysALib.s - LSI Acp34xx board system-dependent assembly routines */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,22mar12,x_s  corrected data cache invalidation ordering. (WIND00315270)
01f,22mar11,x_z  replaced lwsync with msync for PPC476 REV2 erratum #41.
01e,04mar11,x_s  added SMP support.
01d,21dec10,x_z  moved CCR0 initialization to romInit.s.
01c,20dec10,x_z  added initialization for hard float point.
01b,26oct10,x_z  added support for uAMP.
01a,26jan10,x_s  written.
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
The routines in this module don't use the "C" frame pointer %r11@ ! or
establish a stack frame.

INCLUDE FILES: sysLib.h asm.h regs.h

SEE ALSO:
\tb "VxWorks Programmer's Guide: Configuration",
\tb "PowerPC 476FP Embedded Processor Core User's Manual",
\tb "Axxia Communication Processor (ACP3400) Technical Guide",
\tb "Axxia Communication Processor ACP3448 Evaluation Kit User Guide"
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include <arch/ppc/mmu476Lib.h>
#include "config.h"

/*
 * DBSR register initialization
 *
 * This is a write-1s-to-clear register. The value below clears all
 * debug event status bits but leaves the Most Recent Reset status bits
 * untouched.
 */

#define DBSR_INIT       (_DBSR_IDE      | _DBSR_UDE   | \
           _DBSR_ICMP  | _DBSR_BRT      | _DBSR_IRPT  | _DBSR_TRAP  | \
           _DBSR_IAC1  | _DBSR_IAC2     | _DBSR_IAC3  | _DBSR_IAC4  | \
           _DBSR_DAC1R | _DBSR_DAC1W    | _DBSR_DAC2R | _DBSR_DAC2W | \
           _DBSR_RET   | _DBSR_IAC12ATS | _DBSR_IAC34ATS)

    /* globals */

    FUNC_EXPORT(_sysInit)   /* start of system code */
    FUNC_EXPORT(sysInByte)
    FUNC_EXPORT(sysOutByte)
    FUNC_EXPORT(sysInWord)
    FUNC_EXPORT(sysOutWord)
    FUNC_EXPORT(sysInLong)
    FUNC_EXPORT(sysOutLong)
    FUNC_EXPORT(sysDcbf)
    FUNC_EXPORT(sysCacheInv)
    FUNC_EXPORT(sysBootStub)

#if defined(_WRS_CONFIG_SMP)
    FUNC_EXPORT(sysSmpTimeBaseSet)
#endif  /* _WRS_CONFIG_SMP */

    /* externals */

    FUNC_IMPORT(usrInit)

    _WRS_TEXT_SEG_START

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

FUNC_BEGIN(_sysInit)

#ifndef INCLUDE_WRLOAD_IMAGE_BUILD

    /*
     * Save sysStartType in r7 as it is unused.
     * Future modifications to this function SHOULD NOT USE r7.
     */

    mr      r7, r3

#endif /* !INCLUDE_WRLOAD_IMAGE_BUILD */

#ifdef _WRS_HARDWARE_FP

    /* Set MSR to a known state and turn on FP */

    li      r3, _PPC_MSR_FP
    sync
    mtmsr   r3
    isync
    sync

    /* Init the floating point control/status register */

    mtfsfi  7, 0x0
    mtfsfi  6, 0x0
    mtfsfi  5, 0x0
    mtfsfi  4, 0x0
    mtfsfi  3, 0x0
    mtfsfi  2, 0x0
    mtfsfi  1, 0x0
    mtfsfi  0, 0x0
    isync

    /* Initialize the floating point data registers to a known state */

    bl  ifpdrValue
    .float  1.0
ifpdrValue:
    mfspr   r3, LR
    lfs     0, 0(r3)
    lfs     1, 0(r3)
    lfs     2, 0(r3)
    lfs     3, 0(r3)
    lfs     4, 0(r3)
    lfs     5, 0(r3)
    lfs     6, 0(r3)
    lfs     7, 0(r3)
    lfs     8, 0(r3)
    lfs     9, 0(r3)
    lfs     10, 0(r3)
    lfs     11, 0(r3)
    lfs     12, 0(r3)
    lfs     13, 0(r3)
    lfs     14, 0(r3)
    lfs     15, 0(r3)
    lfs     16, 0(r3)
    lfs     17, 0(r3)
    lfs     18, 0(r3)
    lfs     19, 0(r3)
    lfs     20, 0(r3)
    lfs     21, 0(r3)
    lfs     22, 0(r3)
    lfs     23, 0(r3)
    lfs     24, 0(r3)
    lfs     25, 0(r3)
    lfs     26, 0(r3)
    lfs     27, 0(r3)
    lfs     28, 0(r3)
    lfs     29, 0(r3)
    lfs     30, 0(r3)
    lfs     31, 0(r3)
    sync

    /* Set MSR to a known state and turn off FP */

    andi.   r3, r3, 0
    sync
    mtmsr   r3
    isync
    sync

#endif /* INCLUDE_HW_FP */

#ifdef INCLUDE_WRLOAD_IMAGE_BUILD

    li      r3, BOOT_WARM_AUTOBOOT

    /* Setup stack pointer */

    lis     sp, HIADJ(RAM_LOW_ADRS)
    addi    sp, sp, LO(RAM_LOW_ADRS)
    addi    sp, sp, -FRAMEBASESZ    /* get frame stack */

    /* Memory etc already setup jump to alternate boot image */

    lis     r6, HI(usrInit)
    ori     r6, r6, LO(usrInit)
    mtctr   r6
    bctr

#else /* !INCLUDE_WRLOAD_IMAGE_BUILD */

    xor     r0, r0, r0
    addi    r1, r0, -1

#ifdef CLEAR_DEBUG_REGISTERS
    /*
     * Clear DBCR0 register (disable all debug events)
     * Reset all other debugging registers associated with DBCR0.
     * Although the PPC476 is defined to reset some of the debug event enables
     * during the reset operation, this is not required by the  architecture
     * and hence the initialization software should not assume this behavior.
     * Software should disable all debug events in order to  prevent
     * non-deterministic behavior on the trace interface to the core.
     */

    mtspr   DBCR0, r0
    mtspr   DBCR1, r0
    mtspr   DBCR2, r0
    mtspr   IAC1, r0
    mtspr   IAC2, r0
    mtspr   IAC3, r0
    mtspr   IAC4, r0
    mtspr   DAC1, r0
    mtspr   DAC2, r0
    mtspr   DVC1, r0
    mtspr   DVC2, r0

    /* Clear DBSR register (initialize all debug event status) */

    addis   r0, r0, HI(DBSR_INIT)
    ori     r0, r0, LO(DBSR_INIT)
    mtspr   DBSR, r0    /* write 1s to clear */
#endif /* CLEAR_DEBUG_REGISTERS */

    /* Configure timer facilities */

    mtspr   DEC, r0
    mtspr   TBL, r0
    mtspr   TBU, r0
    mtspr   TSR, r1
    mtspr   TCR, r0

    /* Clear some misc registers */

    mtspr   ESR, r0         /* clear Exception Syndrome Reg     */
    mtspr   XER, r0         /*  clear Fixed-Point Exception Reg */

    /*  Initialize the stack pointer */

    lis     sp, HIADJ(_sysInit)
    addi    sp, sp, LO(_sysInit)

    addi    sp, sp, -FRAMEBASESZ    /* carve stack frame */

    mr      r3, r7                  /* set start type */

    /* ensure disabling of interrupt is complete before calling usrInit */

    isync

    bl  usrInit

#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */

FUNC_END(_sysInit)

/*******************************************************************************
*
* sysInByte - read a byte from a specified address
*
* This function reads a byte from a specified address.
*
* SYNOPSIS
* \ss
* UINT8 sysInByte
*     (
*     UINT8 * addr  /@ address of data @/
*     )
* \se
*
* RETURNS: byte from address.
*/

FUNC_BEGIN(sysInByte)
    lbzx    r3, r0, r3
    eieio
    blr
FUNC_END(sysInByte)

/*******************************************************************************
*
* sysInWord - read a word (16-bit big-endian) from a specified address
*
* This function reads a word from a specified address.
* It uses the load halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* UINT16 sysInWord
*     (
*     UINT16 * addr  /@ address to read data from @/
*     )
* \se
*
* RETURNS: word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysInWord)
    lhbrx   r3, r0, r3
    eieio
    blr
FUNC_END(sysInWord)

/*******************************************************************************
*
* sysInLong - read a long from a specified address.
*
* This function reads a long from a specified address.
* It uses the load word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* UINT32 sysInLong
*     (
*     UINT32 * addr /@ address to read data from @/
*     )
* \se
*
* RETURNS: long (32-bit big-endian) from address.
*/

FUNC_BEGIN(sysInLong)
    lwbrx   r3, r0, r3
    eieio
    blr
FUNC_END(sysInLong)

/*******************************************************************************
*
* sysOutByte - write a byte to a specified address.
*
* This function writes a byte to a specified address.
*
* SYNOPSIS
* \ss
* void sysOutByte
*     (
*     UINT8 * addr,    /@ address to write @/
*     UINT8   data     /@ byte (8-bit data) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutByte)
    stbx    r4, r0, r3
    eieio
    blr
FUNC_END(sysOutByte)

/*******************************************************************************
*
* sysOutWord - write a word (16-bit big-endian) to a specified address.
*
* This function writes a word to a specified address.
* It uses the store halfword byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysOutWord
*     (
*     UINT16 * addr,    /@ address to write @/
*     UINT16   data     /@ word (16-bit big-endian) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutWord)
    sthbrx  r4, r0, r3
    eieio
    blr
FUNC_END(sysOutWord)

/*******************************************************************************
*
* sysOutLong - writes a long (32-bit big-endian) to a specified address.
*
* This function writes a long to a specified address.
* It uses the store word byte-reversed instruction.
*
* SYNOPSIS
* \ss
* void sysOutLong
*     (
*     UINT32 * addr,    /@ address to write @/
*     UINT32   data     /@ long (32-bit big-endian) to write @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutLong)
    stwbrx  r4,r0,r3
    eieio
    blr
FUNC_END(sysOutLong)

/*******************************************************************************
*
* sysDcbf - flush one cache line
*
* This routine flushed the cache line specified by <adrs>.
*
* void sysDcbf (UINT32 adrs)
*
* RETURNS : N/A
*/

FUNC_BEGIN(sysDcbf)
    clrrwi  p0,p0,_CPU_CACHE_ALIGN_SHIFT /* round address */
    dcbf    0, p0
    msync
    blr
FUNC_END(sysDcbf)

/*******************************************************************************
*
* sysCacheInv - invalidate L1 and L2 cache
*
* This routine invalidates the entire L1 and L2 cache.
*
* void sysCacheInv ()
*
* RETURNS : N/A
*/

FUNC_BEGIN(sysCacheInv)
    dci     0         /* invalidate L1 D-cache */
    dci     2         /* invalidate L2 cache */
    isync

    ici     0         /* invalidate I-cache */
    isync

    blr
FUNC_END(sysCacheInv)

/*******************************************************************************
*
* sysBootStub - jump from 0x0 to bootrom
*
* This routine jumps from 0x0 to bootrom and need be copied to physical address
* 0x0 before the first time other physical cores than core 0 are waked up.
*
* NOTE: ACP34XX_BOOT_STUB_SIZE in lsi_acp34xx.h must be updated if you modify
* this routine.
*
* void sysBootStub ()
*
* RETURNS : N/A
*/

FUNC_BEGIN(sysBootStub)
    lis     p0, HI(ROM_TEXT_ADRS)
    ori     p0, p0, LO(ROM_TEXT_ADRS)
    mtctr   p0
    isync
    bctr
FUNC_END(sysBootStub)

#if defined(_WRS_CONFIG_SMP)

/*******************************************************************************
*
* sysSmpTimeBaseSet - set the timebase
*
* This routine sets the timebase.  It is intended to be called only by 
* sysSmpTimeBaseInit, as part of the initial timebase synchronization process.  
* (vxTimeBaseSet cannot be used for this purpose because, in VxWorks SMP, 
* vxTimeBaseSet is a no-op to prevent loss of synchronization due to applications 
* setting the timebase.)
*
* void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO)
* 
* RETURNS : N/A
*/

FUNC_BEGIN(sysSmpTimeBaseSet)
	/*
	 * No need to clear TBL first, since the TB
	 * is not running when this function is called.
	 */

	mttbl   p1
	mttbu	p0
	blr
FUNC_END(sysSmpTimeBaseSet)

#endif	/* _WRS_CONFIG_SMP */
