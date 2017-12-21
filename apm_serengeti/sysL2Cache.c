/* sysL2Cache.c - PowerPC 465 L2 cache routines */

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
01c,10feb12,syt  updated L2 Cache Enable function with ASM realization.
01b,26dec11,x_s  change the flush base address to fit AMP also.
01a,29aug11,x_s  initial creation.
*/

/*
DESCRIPTION 
This routine supports PowerPC465 L2 cache including populating hooks for cache 
library access.

The PPC465 core provides an unified Level-2 cache, and contains a function that 
manages the transfers between PPC465 CPU core caches and the Processor Local Bus
(PLB). The storage capacity of the cache arrays supported is 256KB L2 Array sizes 
via configuration bits. 

The L2C Array has a 128 Byte line size, which is equivalent of 8 data beats of 
PLB transfer, and is structured as 4 way set associative cache being managed 
by a true Least-Recently-Used, LRU, replacement algorithm. The L2C controller 
interfaces to the First-Level L1 Instruction cache and Data cache, and to the PLB(v5). 

The L2C implements multi-processor hardware support for memory coherency. ECC 
protection of the tag and data arrays provides high reliability.

SEE ALSO:
\tb " PPC465 Processor Complex User's Manual"
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <cacheLib.h>

/* locals */

LOCAL void sysL2CacheInvFunc();
LOCAL void sysL2CacheDisable();
LOCAL void sysL2CacheFlush();

/* externs */

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;

IMPORT void sysCacheFlush (UINT32 cacheSize, UINT32 * address, UINT32 lineSize);
IMPORT UINT32 sysL2CacheRegRead(UINT32 reg);
IMPORT void sysL2CacheRegWrite(UINT32 reg, UINT32 val);
IMPORT UINT32 vxCcr1Get (void);
IMPORT void vxCcr1Set (UINT32);
IMPORT void sysL2CacheEnable (void);
    
/*******************************************************************************
*
* sysL2CacheInit - initialize L2 cache
*
* This routine attaches L2 cache hooks into kernel cache code.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

void sysL2CacheInit(void)
    {    
    _pSysL2CacheInvFunc = sysL2CacheInvFunc;
    _pSysL2CacheEnable = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    _pSysL2CacheFlush = sysL2CacheFlush;
    }

/*******************************************************************************
*
* sysL2CacheInvFunc - invalidate L2 cache
*
* This routine invalidates the L2 Cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL2CacheInvFunc(void)
    {
    volatile UINT32 l2Cr0Val;

    l2Cr0Val = sysL2CacheRegRead(L2CR0);
    
    /* if cache is inhibited, just return */
    
    if (!(l2Cr0Val & L2CR0_AS_256K))
        return;
    
    sysL2CacheRegWrite(L2CR0, (l2Cr0Val | L2CR0_TAI));
    
    while (sysL2CacheRegRead(L2CR0) & L2CR0_TAI);
    
    return;
    }

/*******************************************************************************
*
* sysL2CacheDisable - disable L2 cache
*
* This routine Disables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL2CacheDisable(void)
    {
    volatile UINT32 l2Cr0Val;
   
    l2Cr0Val = sysL2CacheRegRead(L2CR0);
    
    /* if cache is inhibited already, just return */
    
    if (!(l2Cr0Val & L2CR0_AS_256K))
        return;
    
    sysL2CacheRegWrite(L2CR0, (l2Cr0Val & ~L2CR0_AS_MASK));
    vxCcr1Set(vxCcr1Get() & ~_PPC_CCR1_L2COBE);        

    return;
    }

/*******************************************************************************
*
* sysL2CacheFlush - flush L2 cache
*
* This routine flushes L2 cache. Uses cachePpcReadOrigin as a safe region which 
* equates to start of text section.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL2CacheFlush(void)
    {
    volatile UINT32 l2Cr0Val;

    l2Cr0Val = sysL2CacheRegRead(L2CR0);

    /* if cache is inhibited already, just return */

    if (!(l2Cr0Val & L2CR0_AS_256K))
        return;

    /* take the address LOCAL_MEM_LOCAL_ADRS + 0x10000 as the flush base address */

    sysCacheFlush(2 * L2_CACHE_SIZE, (UINT32 *)(LOCAL_MEM_LOCAL_ADRS + 0x10000), L2_CACHE_LINE_SIZE);
    }
