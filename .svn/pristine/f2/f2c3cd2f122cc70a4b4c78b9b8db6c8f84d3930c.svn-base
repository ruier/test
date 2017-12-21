/* sysCache.c -  L2/L3 cache routines */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,26mar12,h_k  cleaned up RESERVED_ATOMIC. (WIND00341977)
01l,27jan12,agf  minor comment edits
01k,02nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
01j,08dec10,agf  add #error if using L3 cache software w/o L2 software
                 WIND00241106: flush L2 cache before disabling it
01i,22jun10,pmr  disable rev1 errata workarounds
01h,24mar10,pmr  L2/L3 error detection
01g,10mar10,pmr  Support 2nd CPC. Change L3 configuration to copyback.
01f,18feb10,pmr  lock flash clear in L2 invalidate
01e,13oct09,pmr  configure L3 for write-through mode
01d,26aug09,pmr  renamed sysCache.c, added L3 defines
01c,19aug09,pmr  cleanup
01b,12jan09,pmr  fix warning
01a,01dec08,pmr  Created from ads8544/01c
*/

/*
DESCRIPTION:
L2/L3 cache support routines including populating hooks for cache 
library access.

INCLUDE FILES: sysCache.h
*/

#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <cacheLib.h>
#include "config.h"
#include "sysCache.h"


#ifdef INCLUDE_L3_CACHE
#ifndef INCLUDE_L2_CACHE
#error "The L3 cache software is being included without also including the L2 cache software."
#endif 
#endif /* INCLUDE_L3_CACHE */

#ifdef INCLUDE_L2_CACHE

IMPORT UINT32              vxL2CSR0Get(void);
IMPORT void                vxL2CSR0Set(UINT32);

IMPORT VOIDFUNCPTR         _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR         _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR         _pSysL2CacheDisable;

#if	defined(INCLUDE_L3_CACHE) && \
	(!defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00))
IMPORT STATUS cacheAimE500DisableD_Impl (void);		/* private to WRS */
IMPORT UINT32 sysCpcRegSet(UINT32, UINT32);
#endif	/* INCLUDE_L3_CACHE && (!INCLUDE_AMP || INCLUDE_AMP_CPU_00) */

/******************************************************************************
*
* sysL2CacheInvFunc - Invalidates L2 cache
*
* This routine Invalidates the L2 Cache.  Invalidating during invalidation 
* causes undefined results.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL2CacheInvFunc
    (
    )
    {
    int             key       = intCpuLock();  /* avoid multiple invalidation */
    volatile UINT32 l2Csr0Val = vxL2CSR0Get();

    vxL2CSR0Set(l2Csr0Val | P4080_L2CSR0_L2FI_MSK | P4080_L2CSR0_L2LFC_MSK);
    while (vxL2CSR0Get() & (P4080_L2CSR0_L2FI_MSK | P4080_L2CSR0_L2LFC_MSK));

    intCpuUnlock(key);
    }

/******************************************************************************
*
* sysL2CacheEnable - Enables L2 cache
*
* This routine Enables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  
LOCAL void sysL2CacheEnable
    (
    int cacheType
    )
    {
    volatile UINT32 l2Csr0Val;
    int key;

    key = intCpuLock();

    l2Csr0Val = vxL2CSR0Get();

#ifdef INCLUDE_L2_CACHE_ERROR_RECOVERY
    l2Csr0Val |= (P4080_L2CSR0_L2E_MSK | P4080_L2CSR0_L2PE_MSK);
#else
    l2Csr0Val |= (P4080_L2CSR0_L2E_MSK);
#endif
     /* Set L2CSR0 */
    vxL2CSR0Set(l2Csr0Val);
    intCpuUnlock(key);
    }

/******************************************************************************
*
* sysL2CacheDisable - Disables L2 cache
*
* This routine flushes and disables the L2 cache.  vxL2CSR0Set() does not rely
* on memory and this function only uses memory saved prior to the flush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  
LOCAL void sysL2CacheDisable
    (
    int cacheType
    )
    {
    int             key       = intCpuLock();
    volatile UINT32 l2Csr0Val = vxL2CSR0Get();

    if (l2Csr0Val & P4080_L2CSR0_L2E_MSK)    /* only flush if enabled */
        {
        vxL2CSR0Set(l2Csr0Val | P4080_L2CSR0_L2FL_MSK);

	/* wait until complete */

        while (vxL2CSR0Get()  & P4080_L2CSR0_L2FL_MSK);

        l2Csr0Val = vxL2CSR0Get();

        l2Csr0Val &= ~((UINT32)(P4080_L2CSR0_L2E_MSK | P4080_L2CSR0_L2PE_MSK));

        vxL2CSR0Set(l2Csr0Val);
        }

    intCpuUnlock(key);
    }


/******************************************************************************
*
* sysL2CacheInit - L2 cache initialization
*
* This library implements support for the E500 level 2 cache.
* L2 cache initialization routine of blk size etc if only L2 cache
* else expect L2 setup by sysL2SramEnable. Attaches hooks into kernel 
* cache code.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  
void sysL2CacheInit(void)
    {
    volatile UINT32 l2Csr0Val;

    /* Get L2 cache parameters */
    l2Csr0Val = vxL2CSR0Get();

    vxL2CSR0Set(l2Csr0Val&(~P4080_L2CSR0_L2E_MSK));

    _pSysL2CacheInvFunc = (VOIDFUNCPTR)sysL2CacheInvFunc;
    _pSysL2CacheEnable  = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    }

#endif /* INCLUDE_L2_CACHE */

#ifdef INCLUDE_L3_CACHE
#if !defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00)

/******************************************************************************
*
* sysL3CacheFlushDisable - Flushes and disables L3 cache
*
* This routine flushes and disables the L3 cache.
*
* The L3 cache is shared by multiple cores which may not even run vxWorks.
* Per section 8.2.1 of the P4080 RM, to flush the CPC cache ... clear all bits
* in CPCPAR0..15 to prevent new transactions from allocating.  However, when
* testing, it appears that if a flush is performed on one core while another
* core is flushing here then data corruption occurs.
* This issue cannot be elimiated unless all cores are synchronized using a 
* spin lock synchronization mechanism in the caller of this function.
* In order to avoid it happening, the L3 cache is disabled only on the physical
* Core0 when it reboots.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL3CacheFlushDisable (void)
    {
    int cache, allocation, key;

    key = intCpuLock ();

    cacheAimE500DisableD_Impl();

#ifdef INCLUDE_L2_CACHE
    sysL2CacheDisable(-1);
#endif 

    for (cache=0; cache < NUM_L3_CACHES; cache++)
        {
        if (*CPCCSR0(cache) & CPC_ENABLE) 
            {
            for (allocation = 0; allocation < 16; allocation++)
                *(CPCPAR(cache,allocation)) = 0;

#if (L3_WRITE_THRU_MODE == 0)
            sysCpcRegSet (CPCCSR0(cache), *CPCCSR0(cache) | CPC_FLUSH);
#endif
            while (*CPCCSR0(cache) & CPC_FLUSH);

            sysCpcRegSet(CPCCSR0(cache), *CPCCSR0(cache) | CPC_CLEAR_LOCKS);
            while ( (*CPCCSR0(cache) & CPC_CLEAR_LOCKS) != 0);

            sysCpcRegSet (CPCCSR0(cache), *CPCCSR0(cache) & ~CPC_ENABLE);
            }
        }

    intCpuUnlock (key);
    }

/******************************************************************************
*
* sysL3CacheInvalEnable - Invalidates and enables L3 cache
*
* This routine invalidates and enables the L3 cache.
*
* The L3 cache is shared by multiple cores which may not even run vxWorks.
* If the cache invalidate bits CPCFI or CPCLFC are set while they are already
* set it "causes undefined results" per section 8.2.1 of the P4080 RM.
* In order to avoid it happening, the L3 cache is enabled only on the physical
* Core0 at boot-up time.
* Since the flush disabled all L3 allocations, enable them (to the reset 
* default value).
* 
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL3CacheInvalEnable
    (
    int cacheType  /* not used */
    )
    {
    int cache, allocation, key;

    key = intCpuLock ();

    for (cache=0; cache < NUM_L3_CACHES; cache++)
        {
        if ((*CPCCSR0(cache) & (CPC_INVALIDATE|CPC_ENABLE)) == 0)
            {
            sysCpcRegSet(CPCHDBCR(cache),*CPCHDBCR(cache)| CPC_ERRATA_A00N);

            sysCpcRegSet(CPCCSR0(cache), *CPCCSR0(cache) | CPC_INVALIDATE);
    
            while ( (*CPCCSR0(cache) & CPC_INVALIDATE) != 0);

            sysCpcRegSet(CPCHDBCR(cache),*CPCHDBCR(cache)| CPC_ERRATA_A00N);

            for (allocation = 0; allocation < 16; allocation++)
                *(CPCPAR(cache,allocation)) = CPCPAR_RESET_VALUE;

#ifndef L3_DISABLED
            sysCpcRegSet (CPCCSR0(cache), CPC_ENABLE_W_MODE);
#endif
            }
        }

    intCpuUnlock (key);
    }

/******************************************************************************
*
* sysL3CacheInit - L3 cache initialization
*
* This library implements support for the CoreNet Platform Cache (CPC); a level 
* 3 (L3) cache.  The CPC is used on QorIQ multi-core processors such as the 
* P4080.  The CPC has the following features:
* \is
* \i o Write back or write through operation.
* \i o Support for individual line locking.
* \i o SRAM and partial I/O stash mode.
* \i o Decorated storage support.
* \ie
*
* This library only supports the write back or write through operation 
* feature.
*
* The L3 cache is enabled when physical core0 boots up and disabled when
* the core0 gets rebooted.
* 
* The flush/disable and invalidate/enable routines are NOT plugged into 
* the AIM interface because the stated need for synchronization across 
* all cores is required and because L3 is coherent across all cores.
*
* This routine simply calls sysL3CacheInvalEnable();
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

LOCAL void sysL3CacheInit (void)
    {
    sysL3CacheInvalEnable(0);
    }

/************************************************************************
*
* sysL3ExtWriteBufferAlloc - get an L3 address to write
*
* Establishes address which can be written direct to L3 externally.
* Optionally lock into L3.  Requires power of 2 size and address
* alignment >= 256 bytes.
*   
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/  
STATUS sysL3ExtWriteBufferAlloc
    (
    char *adrs,
    UINT size,
    BOOL lock
    )
    {
    /* TODO: determine unit from address */
    int unit = 0;
    UINT32 cpcewcr_reg;
    UINT32 cpcewbar_reg;
    CPC_CPCEWCR cpcewcr;

    if (((UINT)ROUND_DOWN((UINT)adrs,size) != (UINT)adrs) ||
        ((UINT)ROUND_DOWN((UINT)adrs,0x100) != (UINT)adrs))
        {
	if(_func_logMsg != NULL)
	    _func_logMsg (
		"l3 external write region setup failed - check alignment\n",
		1,2,3,4,5,6); 

        return(ERROR);
        }

    cpcewcr_reg = CPCEWCR0(CCSBAR + (unit?CPC2_BASE_ADDR:CPC1_BASE_ADDR));
    cpcewbar_reg = CPCEWBAR0(CCSBAR + (unit?CPC2_BASE_ADDR:CPC1_BASE_ADDR));
    cpcewcr.value = * (UINT32 *) cpcewcr_reg;
    
    if (cpcewcr.field.e == 1)
        {
        cpcewcr_reg = CPCEWCR1(CCSBAR + (unit?CPC2_BASE_ADDR:CPC1_BASE_ADDR));
        cpcewbar_reg = CPCEWBAR1(CCSBAR + (unit?CPC2_BASE_ADDR:CPC1_BASE_ADDR));
        cpcewcr.value = * (UINT32 *) cpcewcr_reg;
        
        if (cpcewcr.field.e == 1)
            {
	    if(_func_logMsg != NULL)
		_func_logMsg (
		"l3 external write region setup failed-no more available\n",
		1,2,3,4,5,6);

            return(ERROR);
            }
        }

    cpcewcr.field.e = 1;
    cpcewcr.field.lock = lock;
    cpcewcr.field.size = size;
    cpcewcr.field.eaddrbase = 0;

    * (UINT32 *) cpcewbar_reg = (UINT32) adrs & 0xffffffc0;
    * (UINT32 *) cpcewcr_reg = cpcewcr.value;
    
    return(OK);
    }
#endif	/* !INCLUDE_AMP || INCLUDE_AMP_CPU_00 */


#ifdef _WRS_CONFIG_UNIT_TEST_CODE_BSP

/************************************************************************
*
* testCache - test L3 cache flush/invalidate in a multi-core environment.
*
* With interrupts locked out, write a sequence of 8 different patterns 
* to a 1 MB buffer, reading back what was written.  This pattern test
* is run 10 times with a 10 tick taskDelay between each run.  If lucky,
* this core is writing a pattern while L3 is being flushed on another 
* core then disabled.  If the data written during the flush is NOT 
* actually flushed then the data read back (bypassing the disabled cache)
* does not match the expected data.
*
* USAGE:
* 1) Build an SMP image with VX_SMP_NUM_CPUS 6 and boot to a shell.
* 2) -> sysClkRateSet(1000)
* 3) -> period 8,testCache
* 4) -> period 8,testCache2
*
* Repeat the test w/ vmLockTest built in:
* 1) -> sysClkRateSet(1000)
* 2) -> cacheDataMode = 2; # to fake out mmuE500Enable()
* 3) -> period 8,testCache
* 4) -> vxTestV2 "-em -v 0 -l 100 -tc vmLockTest"
*
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/  
#define TEST_BFR_SZ 0x100000

int testCacheFailures = 0;

void testCache()
    {
    unsigned int    patterns[] = {0xAAAAAAAA,0x55555555,0x11223344,0x55667788,
                                  0x11111111,0xFFFFFFFF,0x44444444,0x88888888};
    volatile int    i,j,k;
    unsigned int *  buffer;

    if (testCacheFailures < 100)
        {
        buffer = (unsigned int *) memalign(TEST_BFR_SZ,TEST_BFR_SZ);

        for (k=0; k < 100; k++)
            {
            int key = intCpuLock();
    
            for (j=0; j < NELEMENTS(patterns); j++)
                {
                for (i=0; i < TEST_BFR_SZ/sizeof(unsigned int); i++)
                    buffer[i] = patterns[j];
    
                for (i=0; i < TEST_BFR_SZ/sizeof(unsigned int); i++)
                    if (buffer[i] != patterns[j])
                        {
                        testCacheFailures++;
                        if (testCacheFailures < 100)
                            logMsg("Error @0x%8.8x, expected:0x%8.8x, "
                               "actual:0x%8.8x\n", &(buffer[i]), patterns[j], 
                               buffer[i],0,0,0);
                        else if (testCacheFailures == 100)
                            logMsg("testCacheFailures exceeded limit\n",
                                   0,0,0,0,0,0);
                        }
                }
    
            intCpuUnlock(key);
            taskDelay(9);
            }
        free(buffer);
        }
    else
        taskDelay(10);
    }

void testCache2()
    {
#if !defined(INCLUDE_AMP) || defined(INCLUDE_AMP_CPU_00)
    volatile int i;

    if (testCacheFailures < 100)
        for (i=0; i< 100; i++)
            {
            sysL3CacheFlushDisable();
            taskDelay(3);
            sysL3CacheInvalEnable(0);
            taskDelay(7);
            }
    else
        taskDelay(10);
#else
    if (testCacheFailures >= 100)
	taskDelay(10);
#endif /* !INCLUDE_AMP || INCLUDE_AMP_CPU_00 */
    }

#endif /* _WRS_CONFIG_UNIT_TEST_CODE_BSP */

#endif /* INCLUDE_L3_CACHE */
