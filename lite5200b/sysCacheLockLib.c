/* sysCacheLockLib.c - Cache locking support routines. */

/*
 * Copyright (c) 2000-2001, 2005-2006, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,12apr10,jpb  Added logLib.h for _func_logMsg.
01g,02feb06,pkr  add support for MKL build
01f,08feb05,j_b  Replace cachePpcDisable with common cacheDisable.
01e,14dec01,g_h  Add (char*) to SYSL1CACHELOCK_STARTADRS &
                 SYSL1CACHELOCK_ENDADRS to clear DIAB warning.
01d,05nov01,g_h  Adding the macro INCLUDE_CACHE_SUPPORT.
01c,25oct01,g_h  Remove underscore from functions name.
01b,12oct01,g_h  Remove DIAB dependences.
01a,12dec00,mno  created (teamf1)

*/


/*
DESCRIPTION

This module contains code to lock and unlock L1 cache. The routines
sysCacheLock/Unlock can be used to lock and unlock the L1 cache.

This module supports PPC processors that support separate data and instruction
caches (harvard-style) for the L1. The lock routine will lock the entire
cache, after loading it with specified code or data region. When the cache is
locked, data/instruction access to regions not locked in the cache will not
hit the cache.

L1 locking routines can be used to lock only one region of memory. Multiple
calls to lock a region will result in unlocking the previous region. The
region being locked must be aligned on a 32-byte boundary, and must not be
larger than the L1 cache size.

The region being locked in the L1 cache must be aligned
on a 32-byte boundary.

SEE ALSO:
*/


#if defined(INCLUDE_CACHE_SUPPORT) && !defined(INCLUDE_MEM_ALLOT)

/* includes */

#include <vxWorks.h>
#include <cacheLib.h>
#include <intLib.h>
#include <string.h>
#include <vmLib.h>
#include <logLib.h>			/* _func_logMsg */
#include <private/vmLibP.h>
#include <arch/ppc/mmuPpcLib.h>
#include <tickLib.h>
#include <sysCacheLockLib.h>


/* externs */

IMPORT STATUS cachePpcDisable(CACHE_TYPE cache);

/* defines */

#define CACHE_LOCK_AND_ENABLE_RTN_SIZE  ((char*)SYSL1CACHELOCK_ENDADRS - \
                                         (char*)SYSL1CACHELOCK_STARTADRS)

/***********************************************************************
*
* sysCacheLock - locks specified data/instruction region
*
* This routine locks the specified region into the cache, <cacheType>. The
* region to be locked is specified by the start address, <adrs>, and the size,
* <bytes>. The region -- the start address, and size -- must be aligned on a
* 32-byte boundary.
*
* Multiple calls to this routine will silently unlock the specified cache to
* lock the new region.
*
* RETURNS: OK or error if cache is locked, or if cache is not supported.
*
* SEE ALSO: sysCacheUnlock()
*/

STATUS sysCacheLock
    (
    CACHE_TYPE cacheType,
    void * adrs,
    UINT32 bytes
    )
    {
    int x;
    VOIDFUNCPTR cacheEnableAndLockRtn;
    UINT32 cacheEnableAndLockRtnSize;

    if (!ALIGNED(adrs, 32))
        {
	if (_func_logMsg)
		(*_func_logMsg)("\n Memory Address not aligned on 32 byte boundry\n",1,2,3,4,5,6);
        return (ERROR);
        }

    if ((cacheType != _INSTRUCTION_CACHE) &&
        (cacheType != _DATA_CACHE))
        return (ERROR);

    x=intLock();

    /* if l1 is locked, unlock it */
    if ((vxHid0Get() & _PPC_HID0_DLOCK) || (vxHid0Get() & _PPC_HID0_ILOCK))
        sysCacheUnlock(cacheType);

    cacheDisable(cacheType);
    if (cacheType == _DATA_CACHE)
        cacheEnableAndLockRtn = sysL1CacheLock;
    else
        {
        cacheEnableAndLockRtnSize =
            ROUND_UP(CACHE_LOCK_AND_ENABLE_RTN_SIZE, MMU_PPC_PAGE_SIZE);

        cacheEnableAndLockRtn = (VOIDFUNCPTR)
            cacheDmaMalloc (cacheEnableAndLockRtnSize);

        if (cacheEnableAndLockRtn == NULL)
            {
            intUnlock (x);
            return (ERROR);
            }

        VM_STATE_SET (NULL, (char *) cacheEnableAndLockRtn,
                      cacheEnableAndLockRtnSize,
                      VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
                      VM_STATE_CACHEABLE_NOT  | VM_STATE_MEM_COHERENCY_NOT);

        bcopy ((char*)SYSL1CACHELOCK_STARTADRS,
               (char *)cacheEnableAndLockRtn,
               CACHE_LOCK_AND_ENABLE_RTN_SIZE);
        }

    cacheEnableAndLockRtn (cacheType, adrs, bytes);
    intUnlock(x);

    if (cacheType == _INSTRUCTION_CACHE)
        cacheDmaFree ((char *)cacheEnableAndLockRtn);

    return (OK);
    }

/****************************************************************************
*
* sysCacheUnlock - Unlocks the previous locked cache
*
* This routines unlocks the specified cache, <cacheType>
*
* RETURNS: OK or error if cache is not supported.
*
* SEE ALSO: sysCacheLock()
*/

STATUS sysCacheUnlock
    (
    CACHE_TYPE cacheType
    )
    {
    int x;

    if ((cacheType != _INSTRUCTION_CACHE) &&
        (cacheType != _DATA_CACHE))
        return (ERROR);

    x = intLock();
    sysL1CacheUnlock (cacheType);
    intUnlock(x);

    return (OK);
    }

#endif /* INCLUDE_CACHE_SUPPORT */

