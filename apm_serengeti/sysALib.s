/* sysALib.s - APM Serengeti system-dependent assembly routines */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,10feb12,syt  added 32 bit width little endian register read/write
                 and L2 cache enable/flush functions.
01d,07sep11,x_s  added SMP support.
01c,29aug11,x_s  added L2 cache support.
01b,06aug11,syt  updated CCR1 initialization value.
01a,05jun11,syt  initial creation.
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
\tb "PPC465 Processor Complex User's Manual",
\tb "APM86290 Embedded Processor User's Manual",
\tb "APM Serengeti Board Schematics".
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include "config.h"

/* globals */

FUNC_EXPORT(_sysInit)       /* start of system code */
FUNC_EXPORT(sysRead32)
FUNC_EXPORT(sysWrite32)
FUNC_EXPORT(sysRead32Le)
FUNC_EXPORT(sysWrite32Le)
FUNC_EXPORT(sysCacheFlush)
FUNC_EXPORT(sysL2CacheRegRead)
FUNC_EXPORT(sysL2CacheRegWrite)
FUNC_EXPORT(sysL2CacheEnable)

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
*
* sysInit (sysStartType)    /@ THIS IS NOT A CALLABLE ROUTINE @/
*/

FUNC_BEGIN(_sysInit)

    lis sp, HIADJ(_sysInit)
    addi    sp, sp, LO(_sysInit)
    addi    sp, sp, -FRAMEBASESZ    /* carve stack frame */

    bl  usrInit

FUNC_END(_sysInit)

/*******************************************************************************
*
* sysRead32 - read a 32 bit register
*
* This function reads a 32 bit register at <offset> and returns the value.
* The <offset> could be a memory space or IO space address.
*
* UINT32 sysRead32 (UINT32 * address)
*
* RETURNS: the value read from the register
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysRead32)

    lwzx    r3, 0, r3  /* Read 32 bits from an address */
    eieio
    blr
FUNC_END(sysRead32)

/*******************************************************************************
*
* sysWrite32 - write a 32 bit register
*
* This function writes a value to a 32-bit register at <address>.
* The <address> could be a memory space or IO space address.
*
* void sysWrite32 (UINT32 * address, UINT32 value)
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysWrite32)

    stwx    r4, 0, r3   /* Write 32 bits to an address */
    eieio               /* Sync I/O operation */
    blr                 /* Return to caller */
FUNC_END(sysWrite32)

/*******************************************************************************
*
* sysReadLe32 - read a 32 bit register with little endian
*
* This function reads a 32 bit register at <offset> and returns the value.
* The <offset> could be a memory space or IO space address.
*
* UINT32 sysRead32 (UINT32 * address)
*
* RETURNS: the value read from the register
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysRead32Le)

    lwbrx   r3, 0, r3  /* Read 32 bits from an address */
    eieio
    blr
FUNC_END(sysRead32Le)

/*******************************************************************************
*
* sysWriteLe32 - write a 32 bit register with little endian
*
* This function writes a value to a 32-bit register at <address>.
* The <address> could be a memory space or IO space address.
*
* void sysWrite32 (UINT32 * address, UINT32 value)
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysWrite32Le)

    stwbrx  r4, 0, r3   /* Write 32 bits to an address */
    eieio               /* Sync I/O operation */
    blr                 /* Return to caller */
FUNC_END(sysWrite32Le)

/*******************************************************************************
*
* sysCacheFlush - flushes cache 
*
* This routine flushs cache and assumes int lock.
*
* void sysCacheFlush (UINT32 cacheSize, UINT32 * address, UINT32 lineSize)
*
* p0 - cache size
* p1 - buffer origin
* p2 - cache line size
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysCacheFlush)

    /*
     * disable interrupts and MMU. This will ensure that no
     * TLB misses occur that could confuse our block change to
     * the TLB entries, upset the STID value, incur a change of
     * MMU context (PID), or the like.
     */

    mfmsr   r8
    INT_MASK(r8,r6)
    MMU_MASK(r6,r6)     /* clear _PPC_MSR_DS/_PPC_MSR_IS */
    mtmsr   r6
    isync

    mr      r6, p1
    
    add     r7, p1, p0
startFlushLoop1:
    lwz     r0, 0(p1)
    add     p1, p1, p2
    cmplw   p1, r7
    blt     startFlushLoop1

    mr      p1, r6
startFlushLoop2:
    xor     r0, r0, r0
    dcbf    r0, p1
    add     p1, p1, p2
    cmplw   p1, r7
    blt     startFlushLoop2
    msync

    mtmsr   r8          /* restore interrupt & MMU state */
    isync

    blr
FUNC_END(sysCacheFlush)

/*******************************************************************************
*
* sysL2CacheRegRead - read a 32 bit L2 cache register
*
* This routine reads a 32 bit register at <offset> and returns the value.
*
* UINT32 sysRead32 (UINT32 address)
*
* RETURNS: the value read from the register
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysL2CacheRegRead)
    mtdcr   L2DCDCRAI, r3
    mfdcr   r3, L2DCDCRDI
    blr
FUNC_END(sysL2CacheRegRead)

/*******************************************************************************
*
* sysL2CacheRegWrite - write a 32 bit L2 cache register 
*
* This function writes a value to a 32-bit register at <address>.
*
* void sysL2CacheRegWrite (UINT32 address, UINT32 value)
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_BEGIN(sysL2CacheRegWrite)
    mtdcr   L2DCDCRAI, r3
    mtdcr   L2DCDCRDI, r4   
    isync 
    blr
FUNC_END(sysL2CacheRegWrite)

/*******************************************************************************
*
* sysL2CacheEnable - enable L2 Cache
*
* This routine enables PPC465's L2 Cache arrays
*
* RETURNS: N/A
*
*/

FUNC_BEGIN(sysL2CacheEnable)
    li      r3, L2ERAPR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SLVERAPR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4
  
    li      r3, L2CR0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR0_AS_256K | 1 << L2CR0_TAA_SHIFT | 1 << L2CR0_DAA_SHIFT)
    ori     r4, r4, 0
  
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR2
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR2_DEFAULT_VAL)
    ori     r4, r4, LO(L2CR2_DEFAULT_VAL)
    mtdcr   L2DCDCRDI, r4

    li      r3, L2CR3
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    lis     r4, HI(L2CR3_MSTPD)
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FAMAR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FER0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2FER1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2MCSR
    mtdcr   L2DCDCRAI, r3
    li      r4, -1
    mtdcr   L2DCDCRDI, r4

    li      r3, L2MCRER
    mtdcr   L2DCDCRAI, r3
    li      r4, -1
    mtdcr   L2DCDCRDI, r4

    li      r3, L2DBCR
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2DBSR
    mtdcr   L2DCDCRAI, r3
    li      r3, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SLVAC0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SNPAC0
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    li      r3, L2SNPAC1
    mtdcr   L2DCDCRAI, r3
    li      r4, 0
    mtdcr   L2DCDCRDI, r4

    mfspr   r3, CCR1
    ori     r3, r3, _PPC_CCR1_L2COBE
    mtspr   CCR1, r3

    isync

    blr
FUNC_END(sysL2CacheEnable)

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
