/* sysL2Cache.c -  L2 cache routines */

/*
 * Copyright (c) 2003-2006, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01j,07may13,d_l  Fixed prevent errors.
01i,12dec06,jch  (WRS) Updated for VxWorks 6.x compatibility.
01h,12apr05,cak  Fixed the cache flushing routine.
01g,10mar05,efb  Removed intLocking from cache control routines.
01f,15feb05,cak  Added support for handling of L2 cache/SRAM errors. 
01e,01feb05,cak  Expansion of L2 cache support to include full cache/SRAM
                 support.
01d,23aug04,scb  Starting point for "Big Easy".
01c,10sep03,dtr  Tidy up init routine.
01b,05aug03,dtr  Removing some magic numbers.
01a,11jun03,dtr  Created
*/

/*
DESCRIPTION
Support for L2 Cache.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "cacheLib.h"
#include "sysL2Cache.h"
#include "sysMpc8540Error.h"

/* defines */

#undef	DEBUG_CACHE
#define	L2_CACHE_THRESHOLD	0x1

/* typedefs */

/* globals */

/* locals */

LOCAL int l2CacheLineNum = 0; 

/* forward declarations */

void sysL2CacheInit (void);
LOCAL void sysL2CacheInvFunc (void);
LOCAL void sysL2CacheEnable (void);
LOCAL void sysL2CacheDisable (void);
LOCAL void sysL2CacheFlush (void);
#ifdef DEBUG_CACHE
void sysCacheTest (void);
void sysCacheErrorInjectTest (UINT32);
#endif
STATUS sysL2CacheErrIntEnable (void);
LOCAL void sysL2CacheInt (int);

/* externals */

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT UINT32 sysL2CTLGet();
IMPORT UINT32 sysTimeBaseLGet();
IMPORT UINT32 sysTimeBaseUGet();
IMPORT void sysL2CTLSet(UINT32);
IMPORT char * cachePpcReadOrigin;
IMPORT void sysCacheErrorTest(UINT32);
IMPORT void sysMpc8540ErrorLog (ERROR_MSG *);

/******************************************************************************
*
* sysL2CacheInit - Initialize L2 cache function pointers
*
* This routine initializes the L2 cache function pointers as defined in
* cacheArchLib.c. 
*
* RETURNS: NA
*/  

void sysL2CacheInit (void)
    {

    /* hook up the L2 functions */

    _pSysL2CacheInvFunc = sysL2CacheInvFunc;
    _pSysL2CacheEnable = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    _pSysL2CacheFlush = sysL2CacheFlush;
    }

/******************************************************************************
*
* sysL2CacheInvFunc - Invalidates L2 cache
*
* This routine invalidates the L2 cache.
*
* RETURNS: NA
*/  

LOCAL void sysL2CacheInvFunc (void)
    {
    volatile UINT32 l2CtlVal;
    int key;

    key = intLock();

    /* Get present value */

    l2CtlVal = sysL2CTLGet ();

    /* Invalidate entire L2 */

    l2CtlVal |=  M85XX_L2CTL_L2I_MSK;

    /* Set L2CTL */

    sysL2CTLSet (l2CtlVal);

    /* Poll L2CTL until L2I bit is cleared - invalidate is complete */

    while (sysL2CTLGet() & M85XX_L2CTL_L2I_MSK)
    	;
    intUnlock(key);
    }

/******************************************************************************
*
* sysL2CacheFlush - Flush the L2 cache
* 
* This routine flushes the L2 cache, and also the L1 cache, as this is  
* required to make sure the L2 is actually flushed.  We will cover a total
* of 1MB advancing by one cache line (0x20 bytes) with each read.  We will
* start at RAM_LOW_ADRS which is the first cacheable DRAM address.  
*
* RETURNS: N/A
*/  

LOCAL void sysL2CacheFlush (void)
    {
    int noOfLinesToRead = 0x8000;
    int x;
    volatile UINT32 temp;
    volatile UINT32 addr = RAM_LOW_ADRS;
    int key;

    key = intLock();

    for (x = 0; x < noOfLinesToRead; x++)
	{
	temp = *(UINT32 *)addr;
	addr += 0x20;
	}
    intUnlock(key);
    }

/******************************************************************************
*
* sysL2CacheEnable - Enables L2 cache
*
* This routine enables the L2 cache/SRAM.
*
* RETURNS: NA
*/  

LOCAL void sysL2CacheEnable (void)
    {
    volatile UINT32 l2CtlVal = 0;
    LOCAL UINT l2ctlSave = 0x00000000;  /* Saved value for L2ctl programming */

    /* If L2 cache is disabled, then go ahead and enable it, else do nothing */

    if ((sysL2CTLGet() != l2ctlSave) || (l2ctlSave == 0))
        {

        /*
         * L2 cache is either not enabled or the programming of the L2
         * cache control register has been corrupted.  We need to program
         * the L2 cache control register with the proper mode bits set and
         * ultimately set the enable bit in this register.  Because we don't
         * know for sure whether the cache has simply been disabled or if
         * some of the L2 cache control mode bit settings have been
         * compromised, we need to start from a known state.  We will
         * accomplish this by disabling the L2 cache. 
         */

        sysL2CacheDisable ();

	/*
	 * Disable all L2 error detection
	 *
	 * By setting TPARDIS, MBECCDIS, SBECDISS, L2CFGDIS in the
	 * L2ERRDIS (0x20E44) register we disable the L2's error
	 * detection circuitry.
	 */

	CCSR_WRITE32 (CCSBAR, CCSR_L2ERRDIS, (UINT32)0x0000001D);

	switch (L2_CACHE_SRAM_CONFIG)
	    {
	    case L2_CACHE_SRAM_CONFIG_2:

	    /* 
	     * Configure one block (block 1, 128 Kbytes) as cache, 
	     * block 0 remains unused.
	     */

	    l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_BLK_1_CACHE << M85XX_L2CTL_L2SRAM_BIT);
	    l2CtlVal |= (L2_MODE);
	    l2CacheLineNum = (4096 * L2SIZ_128KB);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0,0x00000000);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1,0x00000000);
	    break;

	    case L2_CACHE_SRAM_CONFIG_3:

	    /* 
	     * Configure one block (block 0) as SRAM, and one block 
	     * (block 1) as cache, each 128 Kbytes in size.
	     */
     
	    l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_CACHE_SRAM << M85XX_L2CTL_L2SRAM_BIT);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0,
                (UINT32)(MPC8540_L2_SRAM_BASE & M85XX_L2SRBAR_ADDR_MSK));
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1,0x00000000);
	    l2CtlVal |= (L2_MODE);
	    l2CtlVal &= ~M85XX_L2CTL_L2MEXTDIS_MSK; /* enable access */
	    l2CacheLineNum = (4096 * L2SIZ_128KB);
	    break;

	    case L2_CACHE_SRAM_CONFIG_4:

	    /* Configure all 256 Kbytes as one block of SRAM */

	    l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_BLK_SRAM << M85XX_L2CTL_L2SRAM_BIT);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0,
                (UINT32)(MPC8540_L2_SRAM_BASE & M85XX_L2SRBAR_ADDR_MSK));
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1,0x00000000);
	    l2CtlVal &= ~M85XX_L2CTL_L2MEXTDIS_MSK; /* enable access */
	    l2CtlVal |= M85XX_L2CTL_L2INTDIS_MSK; /* disable intervention */
	    l2CacheLineNum = 0;
	    break;

	    case L2_CACHE_SRAM_CONFIG_5:

	    /* 
	     * Configure both 128 Kbyte blocks as SRAM, for a 
	     * total of 256 Kbytes of SRAM.
	     */

	    l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_SRAM << M85XX_L2CTL_L2SRAM_BIT);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0,
                (UINT32)(MPC8540_L2_SRAM_BASE & M85XX_L2SRBAR_ADDR_MSK));
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1,
                (UINT32)((MPC8540_L2_SRAM_BASE+0x20000) & 
	        M85XX_L2SRBAR_ADDR_MSK));
	    l2CtlVal &= ~M85XX_L2CTL_L2MEXTDIS_MSK; /* enable access */
	    l2CtlVal |= M85XX_L2CTL_L2INTDIS_MSK; /* disable intervention */
	    l2CacheLineNum = 0;
	    break;

	    case L2_CACHE_SRAM_CONFIG_6:

	    /* 
	     * Configure one block (block 0, 128 Kbytes) as SRAM, 
	     * block 1 remains unused.
	     */

	    l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_BLK_0_SRAM << M85XX_L2CTL_L2SRAM_BIT);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0,
                (UINT32)(MPC8540_L2_SRAM_BASE & M85XX_L2SRBAR_ADDR_MSK));
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1, 0x00000000);
	    l2CtlVal &= ~M85XX_L2CTL_L2MEXTDIS_MSK; /* enable access */
	    l2CtlVal |= M85XX_L2CTL_L2INTDIS_MSK; /* disable intervention */
	    l2CacheLineNum = 0;
	    break;

	    case L2_CACHE_SRAM_CONFIG_1:
	    default:

	    /* Configure all 256 Kbytes as one block of cache */

	    l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT);
	    l2CtlVal |= (L2SRAM_BLK_CACHE << M85XX_L2CTL_L2SRAM_BIT);
	    l2CtlVal |= (L2_MODE);
	    l2CacheLineNum = (4096 * L2SIZ_256KB);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR0, 0x00000000);
	    CCSR_WRITE32 (CCSBAR, CCSR_L2SRBAR1, 0x00000000);
	    break;
	    }

	/* Set L2CTL */

	sysL2CTLSet (l2CtlVal);

	l2CtlVal |= M85XX_L2CTL_L2E_MSK ;

	/* Set L2CTL */

	sysL2CTLSet (l2CtlVal);

        /*
         * Save the properly programmed value to test against the next time
         * we are called.
         */

        l2ctlSave = sysL2CTLGet ();

	}
    }

/******************************************************************************
*
* sysL2CacheDisable - Disables L2 cache
*
* This routine disables the L2 cache/SRAM.
*
* RETURNS: N/A
*/  

LOCAL void sysL2CacheDisable (void)
    {
    volatile UINT32 l2CtlVal;

    /* If L2 cache is enabled, go ahead and disable it, otherwise do nothing */

    if ((sysL2CTLGet() & M85XX_L2CTL_L2E_MSK) == (UINT32)M85XX_L2CTL_L2E_MSK)
        {

        /* Flush the L2 cache */

        sysL2CacheFlush();

	/* Set L2E to disabled */

	l2CtlVal = sysL2CTLGet ();
	l2CtlVal &= ~((UINT32)M85XX_L2CTL_L2E_MSK);
	sysL2CTLSet (l2CtlVal);
	}
    }

#ifdef DEBUG_CACHE
/******************************************************************************
*
* sysCacheTest - Test flushing and invalidating of cache 
*
* RETURNS: NA
*/  
   
void sysCacheTest (void)
    {
    UINT32 addr;
    UINT32 temp;
    int x;
    int y;
    UINT32 timeBaseL1;
    UINT32 timeBaseL2;

    /* Initially load the cache */

    addr = 0x10000000;
    timeBaseL1= sysTimeBaseLGet();
    for (x = 0; x < 4096; x++)
	{
	temp = *(UINT32 *)addr; /* read from DRAM, load into cache */
	addr+=32;
	}
    timeBaseL2= sysTimeBaseLGet();
    printf("TimeBase at beginning of load = 0x%x\n\r", timeBaseL1);
    printf("Timebase at end of load = 0x%x\n\r", timeBaseL2);
    printf("Time to load DRAM into cache = 0x%x\n\r", timeBaseL2 - timeBaseL1);

    /* Timing Loop - without flushing */

    timeBaseL1 = sysTimeBaseLGet();
    for (y = 0; y < 100; y++)
	{
	addr = 0x10000000;
	for (x = 0; x < 4096; x++)
	    {
	    temp = *(UINT32 *)addr; /* should be reading from cache */
	    addr+=32;	/* increment by 32 bytes, or one cache line */
	    }
	}
    timeBaseL2 = sysTimeBaseLGet();
    printf("Beginning of timing loop (no flush) = 0x%x\n\r", timeBaseL1);
    printf("End of timing loop (no flush) = 0x%x\n\r", timeBaseL2);
    printf("Time to loop without flush = 0x%x\n\r", timeBaseL2 - timeBaseL1);

    /* Timing Loop - with flushing */

    timeBaseL1 = sysTimeBaseLGet();
    for (y = 0; y < 100; y++)
	{
	addr = 0x10000000;
	for (x = 0; x < 4096; x++)
	    {
	    temp = *(UINT32 *)addr; /* should be reading from cache */
	    addr+=32;	/* increment by 32 bytes, or one cache line */
	    }
	sysL2CacheFlush();
        sysL2CacheInvFunc();
	}
    timeBaseL2 = sysTimeBaseLGet();
    printf("Beginning of timing loop (with flush) = 0x%x\n\r", timeBaseL1);
    printf("End of timing loop (with flush) = 0x%x\n\r", timeBaseL2);
    printf("Time to loop with flushing = 0x%x\n\r", timeBaseL2 - timeBaseL1);

    /* Time the cache flushing routine */

    timeBaseL1 = sysTimeBaseLGet();
    sysL2CacheFlush();
    timeBaseL2 = sysTimeBaseLGet();
    printf("Before flush....0x%x\n\r", timeBaseL1);
    printf("After flush....0x%x\n\r", timeBaseL2);
    printf("Time = 0x%x\n\r", timeBaseL2 - timeBaseL1);
    }

/******************************************************************************
*
* sysCacheErrorInjectTest - test L2 error handling infrastructure 
*
* This routine is used to test the L2 error handling infrastructure,
* which includes the handling of L2 error interrupts, and logging
* of L2 errors through the error handling subsystem. This routine is
* used to generate a tag parity error.
*
* RETURNS: NA
*/
  
void sysCacheErrorInjectTest 
    (
    UINT32 addr	/* cacheable, yet reserved memory address */
    )
    {

    /* invalidate the cache */

    sysL2CacheInvFunc();

    /* set error injection registers */

    /* tag parity error */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRINJCTL, 0x00010000);

    /* initiate transaction to generate an error */

    sysCacheErrorTest(addr);
    } 
#endif /* DEBUG_CACHE */

/******************************************************************************
*
* sysL2CacheErrIntEnable - Enable L2 Cache/SRAM error interrupts
*
* This routine enables L2 Cache/SRAM error interrupts.
*
* RETURNS: ERROR if intConnect or intEnable returned ERROR, OK otherwise.
*/

STATUS sysL2CacheErrIntEnable (void)
    {
    static BOOL l2CacheIntInitDone = FALSE;

    /*
     * ensure HID1[RFXE] is cleared so that assertion of
     * core_fault_in cannot cause a machine check for L2
     * multiple-bit ECC error, instead an interrupt will be
     * generated
     */

    vxHid1Set (vxHid1Get() & ~0x00020000);

    /* clear any reported errors */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRDET, (VUINT32) (M85XX_L2ERRDET_MULL2ERR_MSK |
                  M85XX_L2ERRDET_TPARERR_MSK |
                  M85XX_L2ERRDET_MBECCERR_MSK |
                  M85XX_L2ERRDET_SBECCERR_MSK |
                  M85XX_L2ERRDET_L2CFGERR_MSK));

    /* generate interrupt upon error */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRINTEN, (M85XX_L2ERRINTEN_TPARINTEN_MSK |
                  M85XX_L2ERRINTEN_MBECCINTEN_MSK |
                  M85XX_L2ERRINTEN_SBECCINTEN_MSK |
                  M85XX_L2ERRINTEN_L2CFGINTEN_MSK));

    /* Connect an interrupt handler if none has already been connected */

    if (!l2CacheIntInitDone)
        {
	if (intConnect ((VOIDFUNCPTR *)((int)EPIC_L2CACHE_INT_VEC),
                        (VOIDFUNCPTR)sysL2CacheInt,0) == ERROR)
	    {
	    return (ERROR);
	    }
        }

    /* enable the L2 error reporting */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRDIS, 0x0);

    /* Set the L2 Cache Threshold in L2ERRCTL, and clear current count */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRCTL,
                  (L2_CACHE_THRESHOLD << M85XX_L2ERRCTL_L2CTHRESH_BIT));

    l2CacheIntInitDone = TRUE;

    /* Enable the interrupt */

    if (intEnable (EPIC_L2CACHE_INT_VEC) != OK)
        return (ERROR);

    return (OK);
    }

/******************************************************************************
*
* sysL2CacheInt - Interrupt handling routine for L2 Cache/SRAM errors
*
* This function is invoked when an L2 Cache/SRAM error occurs. It will
* call the error scanning function to do a scan of the L2 error detect
* register to verify that an associated error actually occurred.  If the
* error indeed occurred, the scanning function will log the error.
*
* RETURNS: NA
*/

LOCAL void sysL2CacheInt
    (
    int param ARG_UNUSED
    )
    {
    UINT32 id = 0x0;
    UINT32 mask = 0x0;
    UINT32 l2ErrDetReg = 0x0;
#ifdef INCLUDE_ERROR_HANDLING
    ERROR_MSG myErrorMsg;
#endif

    /* we have recieved an L2 error, so disable the L2 */

    sysL2CacheDisable();

    l2ErrDetReg = (CCSR_READ32(CCSBAR, CCSR_L2ERRDET));

    if ((l2ErrDetReg & M85XX_L2ERRDET_TPARERR_MSK) == 
	M85XX_L2ERRDET_TPARERR_MSK)
        {
        mask = M85XX_L2ERRDET_TPARERR_MSK;
        id = L2CACHE_ERR_TPARERR;
        }
    else if ((l2ErrDetReg & M85XX_L2ERRDET_MBECCERR_MSK) == 
	     M85XX_L2ERRDET_MBECCERR_MSK)
        {
        mask = M85XX_L2ERRDET_MBECCERR_MSK;
        id = L2CACHE_ERR_MBECCERR;
        }
    else if ((l2ErrDetReg & M85XX_L2ERRDET_SBECCERR_MSK) == 
	     M85XX_L2ERRDET_SBECCERR_MSK)
        {
        mask = M85XX_L2ERRDET_SBECCERR_MSK;
        id = L2CACHE_ERR_SBECCERR;
        }
    else if ((l2ErrDetReg & M85XX_L2ERRDET_L2CFGERR_MSK) == 
	     M85XX_L2ERRDET_L2CFGERR_MSK)
        {
        mask = M85XX_L2ERRDET_L2CFGERR_MSK;
        id = L2CACHE_ERR_L2CFGERR;
        }

#ifdef INCLUDE_ERROR_HANDLING

    myErrorMsg.id = id;

    myErrorMsg.params[0] = (CCSR_READ32(CCSBAR,CCSR_L2ERRDET));
    myErrorMsg.params[1] = (CCSR_READ32(CCSBAR,CCSR_L2ERRATTR));

    if ((myErrorMsg.params[1] & (UINT32)M85XX_L2ERRATTR_VALINFO_MSK)
        == (UINT32)M85XX_L2ERRATTR_VALINFO_MSK)
        {
        myErrorMsg.params[2] = (CCSR_READ32(CCSBAR,CCSR_L2ERRADDR));
        myErrorMsg.params[3] = (CCSR_READ32(CCSBAR,CCSR_L2CAPTDATAHI));
        myErrorMsg.params[4] = (CCSR_READ32(CCSBAR,CCSR_L2CAPTDATALO));
        myErrorMsg.params[5] = (CCSR_READ32(CCSBAR,CCSR_L2CAPTECC));
        }

    /* log the error */

    sysMpc8540ErrorLog(&myErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

    /* clear the error */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRDET, mask|0x80000000);

    /* Clear Error Capture Registers */

    CCSR_WRITE32 (CCSBAR, CCSR_L2ERRATTR,0x00000000);
    }
