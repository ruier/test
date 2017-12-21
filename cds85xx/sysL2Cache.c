/* sysL2Cache.c -  L2 cache routines */

/*
 * Copyright (c) 2003-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01h,26sep07,pdg  Fixed doc errors
01g,05sep07,jrp  APIGEN Updates
01f,30jul06,tor  merge PCI modifications
01e,13oct04,dtr  Add external write alloc func and setting cache to data-only
01d,04oct04,mdo  Documentation fixes for apigen
01c,10sep03,dtr  Tidy up init routine.
01b,05aug03,dtr  Removing some magic numbers.
01a,11jun03,dtr  Created
*/

/*
DESCRIPTION

*/

#include "vxWorks.h"
#include "config.h"
#include "cacheLib.h"
#include "sysL2Cache.h"

/* L2 Cache Routines */

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT UINT32 vxL2CTLGet(VUINT32 *);
IMPORT void vxL2CTLSet(UINT32,VUINT32*);
IMPORT char * cachePpcReadOrigin;

LOCAL void sysL2CacheInvFunc();
LOCAL void sysL2CacheEnable();
LOCAL void sysL2CacheDisable();
LOCAL void sysL2CacheFlush(int cacheType);

IMPORT void sysCacheFlush();

STATUS sysL2ExtWriteBufferAlloc (char *adrs,UINT size,BOOL lock);

/* 4096 == (Min Cache Size / L2 Cache Line Size) */
int l2CacheLineNum = (4096 * L2_CACHE_SIZE); 


/******************************************************************************
*
* sysL2CacheInit - L2 cache initialization
*
* L2 cache initialization routine of blk size etc if only L2 cache
* else expect L2 setup by sysL2SramEnable. Attaches hooks into kernel 
* cache code.
*
* RETURNS: N/A
*
* ERRNO
*/  
void sysL2CacheInit (void)
    {
    volatile UINT32 l2CtlVal;
    int n;

    /* Get L2 cache parameters */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

#ifndef INCLUDE_L2_SRAM
    vxL2CTLSet(l2CtlVal&(~M85XX_L2CTL_L2E_MSK),M85XX_L2CTL(CCSBAR));
    l2CtlVal = (L2_CACHE_SIZE << M85XX_L2CTL_L2BLKSIZ_BIT);
     /* Set L2CTL */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
#endif

    _pSysL2CacheInvFunc = (VOIDFUNCPTR)sysL2CacheInvFunc;
    _pSysL2CacheEnable = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    _pSysL2CacheFlush = sysL2CacheFlush;

    /* Clean external write region registers */
    for ( n=0 ; n < NO_L2_EXT_WRITE_REGIONS ; n++)
	{
	*M85XX_L2CEWARn(CCSBAR,n) = 0;
	*M85XX_L2CEWCRn(CCSBAR,n) = 0;
	}

    }


/******************************************************************************
*
* sysL2CacheInvFunc - Invalidates L2 cache
*
* This routine Invalidates the L2 Cache.
*
* RETURNS: N/A
*
* ERRNO
*/  

LOCAL void sysL2CacheInvFunc (void)
    {
    volatile UINT32 l2CtlVal;

    /* Get present value */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));
    /* Invalidate entire L2 */
    l2CtlVal |=  M85XX_L2CTL_L2I_MSK;
    /* Set L2CTL */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    while (vxL2CTLGet(M85XX_L2CTL(CCSBAR)) & M85XX_L2CTL_L2I_MSK)
        /* do nothing */;

    }

/******************************************************************************
*
* sysL2CacheEnable - Enables L2 cache
*
* This routine Enables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO
*/  
LOCAL void sysL2CacheEnable
    (
    int cacheType
    )
    {
    volatile UINT32 l2CtlVal;
    int key;

    key = intLock();
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));
    l2CtlVal |= M85XX_L2CTL_L2DO_MSK ;
     /* Set L2CTL */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    l2CtlVal |= M85XX_L2CTL_L2E_MSK ;
     /* Set L2CTL */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    intUnlock(key);
    
    }

/******************************************************************************
*
* sysL2CacheDisable - Disables L2 cache
*
* This routine Disables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO
*/  
LOCAL void sysL2CacheDisable
    (
    int cacheType
    )
    {
    volatile UINT32 l2CtlVal;
    int key;

    key = intLock();
    /* Set L2E to disabled */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~((UINT32)M85XX_L2CTL_L2E_MSK);
    /* Set L2CTL */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    intUnlock(key);
    }

/******************************************************************************
*
* sysL2CacheFlush - Flushes L1 and L2 cache lines
*
* This routine Flushes L1 and L2 cache lines.  Required to make sure L2
* is actually flushed. Uses cachePpcReadOrigin as a safe region which 
* equates to start of text section.
*
* RETURNS: N/A
*
* ERRNO
* 
* \NOMANUAL
*/  
LOCAL void sysL2CacheFlush
    (
    int cacheType
    )
    {
    int noOfCacheLines,key;

    key = intLock();
    noOfCacheLines = (ppcE500DCACHE_LINE_NUM + l2CacheLineNum );

    sysCacheFlush(noOfCacheLines,cachePpcReadOrigin, ppcE500CACHE_ALIGN_SIZE);

    intUnlock(key);

    }

/************************************************************************
*
* sysL2ExtWriteBufferAlloc - get an L2 address to write 
*
* Establishes address which can be written direct to L2
* externally. Optionally lock into L2.  Requires power of 2 size
* and address alignment >= 256 bytes.
* 
* RETURNS: OK, or ERROR
*
* ERRNO
*/  

STATUS sysL2ExtWriteBufferAlloc
    (
    char *adrs,
    UINT size,
    BOOL lock
    )
    {
    int n;

    if (((UINT)ROUND_DOWN((UINT)adrs,size) != (UINT)adrs) ||
        ((UINT)ROUND_DOWN((UINT)adrs,0x100) != (UINT)adrs))
	{
	printf ("l2 extrenal write region setup failed - check alignment\n"); 
	return (ERROR);
	}

    n = 0;

    while ( (*M85XX_L2CEWCRn(CCSBAR,n) & M85XX_L2CEWCR_E_MSK) == 
	 M85XX_L2CEWCR_E_MSK)
	{
	n++;
	if ( n >= NO_L2_EXT_WRITE_REGIONS )
	    break;
	}

    if( n >= NO_L2_EXT_WRITE_REGIONS )
	{
	printf ("l2 extrenal write region setup failed-no more available\n"); 
	return (ERROR);
	}


    *M85XX_L2CEWARn(CCSBAR,n) = ((UINT)adrs & M85XX_L2CEWAR_ADDR_MSK);
    *M85XX_L2CEWCRn(CCSBAR,n) = M85XX_L2CEWCR_E_MSK | ((~(size - 1)) >> 8) | (lock << M85XX_L2CEWCR_LOCK_BIT);
    
    return(OK);
    }
