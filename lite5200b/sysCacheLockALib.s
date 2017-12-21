/* sysCacheLockALib.s  - cache locking assembly routines */

/*
 * Copyright (c) 2000-2001 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,25oct01,g_h  Remove underscore from functions name.
01a,12dec00,ksn  created (teamf1)         
*/


/* includes */
        
#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"        
#include "cacheLib.h"
#include "config.h"
#include "sysCacheLockLib.h"
               
        /* globals */
        
        FUNC_EXPORT(sysL1CacheLock)
        FUNC_EXPORT(sysL1CacheUnlock)

        DATA_EXPORT(SYSL1CACHELOCK_STARTADRS)
        DATA_EXPORT(SYSL1CACHELOCK_ENDADRS)
                
        _WRS_TEXT_SEG_START

/**************************************************************************
*	
* sysL1CacheLock - enable the cache, and load the cache with specified 
*                  range of address and lock
*	
* This routine enables the cache , and load the cache with specified range
* of address and locks.
*
* SYNOPSIS
* \ss
* void sysL1CacheLock
*     (
*     CACHE_TYPE cacheType
*     void * adrs
*     size_t bytes
*     )
* \se
*
* SEE ALSO: sysL1CacheUnlock()
*
* RETURNS: N/A
*
*/                
                    
SYSL1CACHELOCK_STARTADRS:
FUNC_BEGIN(sysL1CacheLock)
        mflr   r27
        mr     r28, r3
        mr     r29, r4
        mr     r30, r5

        cmpwi  r28, _DATA_CACHE
        beq    EnDataCache
        cmpwi  r28, _INSTRUCTION_CACHE
        beq    EnInstCache

EnDataCache:
        mfspr  r3, HID0
#if   (CPU == PPC603)        
        ori    r3, r3, _PPC_HID0_DCE
#elif (CPU == PPC604)
        ori    r3, r3, _PPC_HID0_DCE | _PPC_HID0_DCFI
#endif                
        sync
        mtspr  HID0, r3
        sync

LdNLkPpcDataCache:
        add    r30, r29, r30
loadD: 
        lwz    r20, 0(r29)
        addi   r29, r29, 32
        cmplw  r29, r30
        bge    lockD
        b      loadD

lockD:
        mfspr  r3, HID0
        ori    r3, r3, _PPC_HID0_DLOCK
        sync
        mtspr  HID0, r3
        sync
        b      RETLK

EnInstCache:    
        mfspr  r3, HID0
#if   (CPU == PPC603)        
        ori    r3, r3, _PPC_HID0_ICE
#elif (CPU == PPC604)
        ori    r3, r3, _PPC_HID0_ICE | _PPC_HID0_ICFI
#endif                
        isync
        mtspr  HID0, r3
        sync
       
LdNLkPpcInstCache: 
        add    r30, r29, r30
        lis    r3,  0x0000
        ori    r3, r3, 0x0001
        mtlr   r29

loadI:  divw.  r3, r3, r3
        beqlr+

        addi   r29, r29, 32
        mtlr   r29
        cmplw  r29, r30
        bge    lockI
        b      loadI

lockI:  
        mfspr  r3, HID0
        ori    r3, r3, _PPC_HID0_ILOCK
        isync
        mtspr  HID0, r3
        sync

RETLK:
        sync
        mtlr   r27
        blr
SYSL1CACHELOCK_ENDADRS:
FUNC_END(sysL1CacheLock)        
        
/**************************************************************************
*	
* sysL1CacheUnlock - unlock the cache
*
* This routine unlocks previously locked cache.
*
* SYNOPSIS
* \ss
* void sysL1CacheUnlock
*     (
*     CACHE_TYPE cacheType
*     )
* \se
*
* SEE ALSO: sysL1CacheLock()
*
* RETURNS: N/A
*/                

FUNC_BEGIN(sysL1CacheUnlock)        
        cmpwi  r3, _INSTRUCTION_CACHE
        beq    UnLkPpcInstCache
        
UnLkPpcDataCache:  
        mfspr  r3, HID0
        lis    r4, 0xFFFF
        ori    r4, r4, 0xEFFF
        and    r3, r3, r4
        sync
        mtspr  HID0, r3
        sync
        b      RETUNLK                
                 
UnLkPpcInstCache:       
        mfspr  r3, HID0
        lis    r4, 0xFFFF
        ori    r4, r4, 0xDFFF
        and    r3, r3, r4
        isync
        mtspr  HID0, r3
        isync
        
RETUNLK:
        blr                

FUNC_END(sysL1CacheUnlock)
