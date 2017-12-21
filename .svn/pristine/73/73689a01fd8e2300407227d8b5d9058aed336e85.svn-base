/* sysL3Cache.c - L3 cache library */

/*
 * Copyright 2002-2004, 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1996-2003 Motorola, Inc., All Rights Reserved. */

/*
modification history
--------------------
01i,09mar09,e_d  Fix L3Cache initialization issue.(WIND00158826)
01h,08oct07,pch  CQ100009: handle MPC745x errata #35
01g,01nov04,mdo  Documentation fixes for apigen
01f,25oct04,dtr  SPR 102861 - remove sysL3CacheEnable from L3CacheInit.
01e,25oct03,cak  Minor fix to sysL3CachePmEnable() routine.
01d,09sep03,cak  Added Private Memory support.
01c,21may03,scb  Check L3cr against desired program value before enabling.
01b,01apr03,scb  Correctly set L3CR_L3CLKEN_MASK in L3 cache control reg.
01a,01oct02,scb  Copied from hxeb100 base (ver 01a).
*/

/*
DESCRIPTION

This library provides L3 cache support. 

Support is included for initializing, invalidating, enabling, 
flushing, disabling, locking and unlocking the L3 Cache.  Parameters 
used to configure the L3 controller are gathered from the L3 Cache 
Record in the VPD.

Support is also included for configuring the L3 cache as private
memory.

INCLUDE FILES
*/

/* includes */

#include "vxWorks.h"
#include "sysCache.h"
#include "sysMotVpd.h"

/* defines */

/* forward declarations */

STATUS sysL3CacheInit (void);
void sysL3CacheFlushDisable (void);
void sysL3CacheInvEnable (void);
void sysL3CacheFlush (void);
void sysL3CacheInv (void);
void sysL3CacheLock (void);
void sysL3CacheUnlock (void);
void sysL3CacheEnable (void);
void sysL3CacheDisable (void);
#ifdef INCLUDE_CACHE_L3_PM
    STATUS sysL3CachePmEnable (UINT32);
    LOCAL void sysL3CachePmBaseAddrSet (UINT32);
    LOCAL STATUS sysL3CachePmScrub (UINT32, UINT32);
    LOCAL void sysL3CacheZeroFlush (UINT32);
    void sysL3CachePmDisable (void);
#endif /* INCLUDE_CACHE_L3_PM */

/* externals */

IMPORT UINT sysL3crGet (void);
IMPORT void sysL3crPut (UINT regVal);
IMPORT UINT sysL3pmGet (void);
IMPORT void sysL3pmPut (UINT regVal);

IMPORT VOIDFUNCPTR _pSysL3CacheInvalEnableFunc;
IMPORT VOIDFUNCPTR _pSysL3CacheFlushDisableFunc;

LOCAL VPD_PACKET * pVpdL3CachePacket = NULL;
LOCAL UINT l3crSave = 0x00000000;  /* Saved value for L3cr programming */

/******************************************************************************
*
* sysL3CacheInit - initialize the L3 cache 
*
* This routine initializes and enables L3 cache support. 
*
* RETURNS: N/A
*
* ERRNO 
*/ 

STATUS sysL3CacheInit (void)
    {

    /* Extract L3 Cache Configuration from VPD */

    if (pVpdL3CachePacket == NULL)
	{

	/* Extract a VPD L3 Cache Packet */

	if (sysMotVpdPktGet(VPD_PID_L3, 0, &sysVpdPkts[0], &pVpdL3CachePacket)
	    == ERROR)
	    return (ERROR);
	}

    /* hook up the L3 functions */

    _pSysL3CacheInvalEnableFunc = sysL3CacheInvEnable;
    _pSysL3CacheFlushDisableFunc = sysL3CacheFlushDisable;

    return OK;
    }

/******************************************************************************
*
* sysL3CacheInvEnable - invalidate and enable L3 cache 
*
* This routine invalidates and enables the L3 cache 
*
* RETURNS: N/A 
*
* ERRNO 
*/ 

void sysL3CacheInvEnable(void)
    {
    UINT l3crVal = 0x00000000;  /* current L3CR values are not preserved */
    UINT l3crPmen = 0x0;
    UINT l3crPmSize = 0x0;

    /* If L3 cache is disabled, then go ahead and enable it, else do nothing */

    if ((sysL3crGet() != l3crSave) || (l3crSave == 0))
	{

	/* Need to capture state of private memory enable and size bits */

	l3crPmen = (sysL3crGet() & L3CR_PMEN_MASK);
	l3crPmSize = (sysL3crGet() & L3CR_PMSIZ_MASK);

	/*
	 * L3 cache is either not enabled or the programming of the L3
	 * cache control register has been corrupted.  We need to program
	 * the L3 cache control register with the proper mode bits set and
	 * ultimately set the enable bit in this register.  Because we don't
	 * know for sure whether the cache has simply been disabled or if 
	 * some of the L3 cache control mode bit settings have been
	 * compromised, we need to start from a known state.  We will
	 * accomplish this by disabling the L3 cache.  
	 */

	sysL3CacheDisable ();

	/* 
	 * Configure the L3CR Register:
	 *
	 * L3CLK   -> L3 Clock Ratio
	 * L3RT    -> L3 SRAM Type
	 * L3SIZ   -> L3 Size
	 * L3IO    -> L3 Instruction-only mode
	 * L3SPO   -> L3 Sample Point Override
	 * L3CKSP  -> L3 Clock Sample Point
	 * L3PSP   -> L3 P-Clock Sample Point
	 * L3REP   -> L3 Replacement Algorithm
	 * L3NIRCA -> L3 non-integer ratios clock adjustment
	 * L3DO    -> L3 Data-only Mode
	 *
	 * The replacement algorithm and the mode (instruction, data, both)
	 * are user-configurable, via a set of definitions in the board
	 * header file.  All other fields are programmed according to VPD
	 * data.
	 */

	/* L3 Clock Ratio */

	l3crVal |= 
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_CLK_RATIO]) << L3CR_L3CLK_BIT);

	/* Replacement Algorithm */

	l3crVal |= L3_REP_ALG;

	/* L3 Cache Size */

	if (l3crPmen == 0x0)
	    l3crVal |=
	    (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) << L3CR_L3SIZ_BIT);
	else
	    {
	    if (l3crPmSize == 0x0)
		{
		if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
		    l3crVal |= (L3CR_PMSIZ_1MB << L3CR_PMSIZ_BIT);
		else
		    {
		    l3crVal |= (L3CR_PMSIZ_1MB << L3CR_PMSIZ_BIT);
		    l3crVal |= (L3CR_L3SIZ_1MB << L3CR_L3SIZ_BIT);
		    }
		}
	    else
		{
		if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
		    l3crVal |= (L3CR_PMSIZ_1MB << L3CR_PMSIZ_BIT);
		else
		    {
		    l3crVal |= (L3CR_PMSIZ_2MB << L3CR_PMSIZ_BIT);
		    }
		}
	    }

	/* L3 Cache Mode */

	l3crVal |= L3_MODE;

	/* L3 Sample Point Override */

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_SMPL_PNT_OVERRIDE]) << 
	 L3CR_L3SPO_BIT);

	/* L3 Clock Sample Point */

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_CACHE_CLK_SMPL_PNT]) <<
	 L3CR_L3CKSP_BIT);

	/* L3 P-Clock Sample Point */

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_PROC_CLK_SMPL_PNT]) <<
	 L3CR_L3PSP_BIT);

	/* L3 SRAM Type */

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_SRAM_TYPE]) <<
	 L3CR_L3RT_BIT);

	/* L3 Non-Integer Ratios Clock Adjustment for SRAM */

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_SRAM_CLK_CTRL]) <<
	 L3CR_L3NIRCA_BIT);

	/* Write to L3CR Register */

	sysL3crPut(l3crVal);

	/* set L3CR[5] reserved bit to 1 */

	sysL3crPut(sysL3crGet() | L3CR_RES_INIT_MASK);

	/* set L3CR[L3CLKEN] = 1 */

	sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK);

	/* Globally invalidate the L3 Cache */

	sysL3CacheInv();

	/* Clear L3CR[L3CLKEN] */

	sysL3crPut(sysL3crGet() & ~L3CR_L3CLKEN_MASK);

	/* Configure Data and Address Parity */

	l3crVal = sysL3crGet();

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_DATA_ERROR_DETECT]) <<
	 L3CR_L3PE_BIT);

	l3crVal |=
	(((UINT)pVpdL3CachePacket->data[VPD_L3C_ADDR_ERROR_DETECT]) <<
	 L3CR_L3APE_BIT);
    
        l3crVal &= ~(L3CR_L3IO_MASK | L3CR_L3DO_MASK);

	sysL3crPut(l3crVal);

	/* Wait 100 processor cycles - 1 microsecond */

	sysUsDelay(1);

	/* Set L3 Cache Enable & L3 Clock Enable */

	if (l3crPmen == 0x0)
	    sysL3crPut(sysL3crGet() | (L3CR_L3E_MASK | L3CR_L3CLKEN_MASK));
	else
	    {
	    if (l3crPmSize == 0x0)
		{
		if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
		    sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK | l3crPmen);
		else
		    {
		    sysL3crPut(sysL3crGet() | l3crPmen |
			       (L3CR_L3E_MASK | L3CR_L3CLKEN_MASK)); 
		    }
		}
	    else
		{
		sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK | l3crPmen);
		}
	    }

	/* Wait 100 processor cycles - 1 microsecond */

	sysUsDelay(1);

	l3crSave = sysL3crGet ();
	}
    }

#ifdef INCLUDE_CACHE_L3_PM
/******************************************************************************
*
* sysL3CacheZeroFlush - Perform a zero-flush of the L3 cache private memory
*
* This routine performs a zero-flush of the L3 cache private memory.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheZeroFlush
    (
    UINT32 addr         /* cache line address to zero-flush */
    )
    {
    WRS_ASM ("dcbz 0,3");
    WRS_ASM ("sync;isync");
    WRS_ASM ("dcbf 0,3");
    WRS_ASM ("sync;isync");
    }

/******************************************************************************
*
* sysL3CachePmScrub - Scrub the L3 Private Memory.
*
* This routine scrubs the L3 private memory to remove any latent parity
* errors.
*
* RETURNS: N/A
*
* ERRNO 
*/

STATUS sysL3CachePmScrub 
    (
    UINT32 addr,	/* start address of private memory */
    UINT32 size		/* size of private memory */
    )
    {
    UINT32 pmAddr = addr;
    UINT32 i;

    /* Zero-flush the cache */

    for (i = 0; i < size ; i += _CACHE_ALIGN_SIZE)
	{
	pmAddr = addr + i;
	sysL3CacheZeroFlush (pmAddr);     /* call dcbz dcbf routine */
	}
    return (OK);
    }

/******************************************************************************
*
* sysL3CachePmBaseAddrSet - Program the L3 Private Memory Address Register
*
* This routine programs the L3 Private Memory Address Register (L3PM).
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CachePmBaseAddrSet 
    (
    UINT32 addr		/* start address of private memory */
    )
    {

    /* Program the L3 Private Memory Address Register */

    sysL3pmPut(addr >> 4);

    }

/*****************************************************************************
*
* sysL3CachePmEnable - Configure the L3 cache as private memory
*
* This routine configures the L3 cache, all or part, as private memory.
* Keep in mind when calling this routine that the size can only be 1MB
* or 2MB.
*
* RETURNS: OK, or ERROR if unable to configure the L3 srams as private memory
*
* ERRNO 
*/

STATUS sysL3CachePmEnable
    (
    UINT32 size		/* size of private memory */
    )
    {
    UINT32 pmSize = 0x0;
    UINT32 cacheSize = 0x0;
    UINT32 l3crVal = 0x0;

    /* Ensure a valid size for the private memory, as well as L3 cache */

#if (! defined INCLUDE_CACHE_L3)

    /* Extract L3 Cache Configuration from VPD */

    if (pVpdL3CachePacket == NULL)
	{

	/* Extract a VPD L3 Cache Packet */

	if (sysMotVpdPktGet(VPD_PID_L3, 0, &sysVpdPkts[0], &pVpdL3CachePacket)
	    == ERROR)
	    return (ERROR);
	}

    /* Entire L3 must be configured as private memory */

    if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
	{
	pmSize = SIZE_1MB;
	cacheSize = 0x0;
	}
    else
	{
	pmSize = SIZE_2MB;
	cacheSize = 0x0;
	}
#else
    if (size == SIZE_1MB)
	{
	if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
	    {
	    pmSize = size;
	    cacheSize = 0x0;
	    }
	else
	    {
	    pmSize = size;
	    cacheSize = SIZE_1MB;
	    }
	}
    else   /* user requests 2MB, or any value other than 1MB */
	{
	if (((UINT)pVpdL3CachePacket->data[VPD_L3C_SIZE]) == 0x0)
	    {
	    pmSize = SIZE_1MB;
	    cacheSize = 0x0;
	    }
	else
	    {
	    pmSize = size;
	    cacheSize = 0x0;
	    }
	}
#endif /* INCLUDE_CACHE_L3 */

    if (pmSize != size)
	{

	/* 
	 * The user has requested a private memory size incompatible 
	 * with the current cache configuration.
	 */

	return(ERROR);
	}

#if (! defined INCLUDE_CACHE_L3)

    /* 
     * The L3 Cache has not yet been properly configured, 
     * do this now.  The VPD will be read and the L3CR register
     * will be properly programmed.  This will also temporarily
     * enable the L3 SRAMs as cache, before we reconfigure for
     * private memory.
     */

    sysL3CacheInvEnable(); 
#endif /* INCLUDE_CACHE_L3 */

    /* Disable and Flush the L3 Cache */

    sysL3CacheFlushDisable();

    /* Retrieve Current L3CR Settings */

    l3crVal = sysL3crGet ();

    /* Program the L3 Private Memory Base Address Register */

    sysL3CachePmBaseAddrSet(INCLUDE_CACHE_L3_PM_BASE);

    /* 
     * Preserve the following L3CR parameters:
     *
     * L3 Clock Ratio
     * Replacement Algorithm
     * L3 Cache Mode
     * L3 Sample Point Override
     * L3 Clock Sample Point
     * L3 P-Clock Sample Point
     * L3 SRAM Type
     * L3 Non-Integer Ratios Clock Adjustment for SRAM
     */

    l3crVal &= (L3CR_L3CLK_MASK | L3CR_L3REP_MASK | L3CR_L3IO_MASK |
		L3CR_L3DO_MASK | L3CR_L3SPO_MASK | L3CR_L3CKSP_MASK |
		L3CR_L3PSP_MASK | L3CR_L3RT_MASK | L3CR_L3NIRCA_MASK); 

    /* Set L3 cache size to 1MB, this is valid for all configurations */

    l3crVal |= (L3CR_PMSIZ_1MB << L3CR_PMSIZ_BIT); 

    /* set the Private Memory size */

    if (pmSize == SIZE_1MB)
	l3crVal |= (L3CR_PMSIZ_1MB << L3CR_PMSIZ_BIT);
    else if (pmSize == SIZE_2MB)
	l3crVal |= (L3CR_PMSIZ_2MB << L3CR_PMSIZ_BIT);

    /* write to L3CR register */

    sysL3crPut(l3crVal);

    /* set L3CR[5] reserved bit to 1 */

    sysL3crPut(sysL3crGet() | L3CR_RES_INIT_MASK);

    /* set L3CR[L3CLKEN] = 1 */

    sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK);

    /* Globally invalidate the L3 Cache */

    sysL3CacheInv();

    /* Clear L3CR[L3CLKEN] */

    sysL3crPut(sysL3crGet() & ~L3CR_L3CLKEN_MASK);

    /* Wait 100 processor cycles - 1 microsecond */

    sysUsDelay(1);

    /* Configure Data and Address Parity & set L3CR[L3CLKEN] = 1 */

    l3crVal = sysL3crGet();

    l3crVal |=
    (((UINT)pVpdL3CachePacket->data[VPD_L3C_DATA_ERROR_DETECT]) <<
     L3CR_L3PE_BIT);

    l3crVal |=
    (((UINT)pVpdL3CachePacket->data[VPD_L3C_ADDR_ERROR_DETECT]) <<
     L3CR_L3APE_BIT);

    l3crVal |= L3CR_L3CLKEN_MASK;

    sysL3crPut(l3crVal);

    /* Wait 100 processor cycles - 1 microsecond */

    sysUsDelay(1);

    /* Set L3 Cache Enable, Clock Enable, Private Memory Enable */

#if (! defined INCLUDE_CACHE_L3)

    sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK | L3CR_PMEN_MASK);

#else
    if (cacheSize != 0x0)
	sysL3crPut(sysL3crGet() | (L3CR_L3E_MASK | L3CR_L3CLKEN_MASK |
				   L3CR_PMEN_MASK));
    else
	sysL3crPut(sysL3crGet() | L3CR_L3CLKEN_MASK | L3CR_PMEN_MASK);
#endif /* INCLUDE_CACHE_L3 */

    /* Wait 100 processor cycles - 1 microsecond */

    sysUsDelay(1);

    /* Scrub the Private Memory */

    sysL3CachePmScrub(INCLUDE_CACHE_L3_PM_BASE, pmSize);

    return (OK);
    } 

/******************************************************************************
*
* sysL3CachePmDisable - Disable the L3 private memory
*
* This routine disables the L3 private memory
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CachePmDisable (void)
    {

    /* Disable the L3 private memory by clearing the PMEN bit in L3CR */

    sysL3crPut(sysL3crGet() & ~L3CR_PMEN_MASK);
    sysL3CachePmBaseAddrSet(0x0);
    }
#endif /* INCLUDE_CACHE_L3_PM */

/******************************************************************************
*
* sysL3CacheFlushDisable - flush and disable the L3 cache
*
* This routine flushes and disables the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheFlushDisable (void)
    {
    UINT l3crVal;

    /* If L3 cache is enabled, go ahead and flush it, otherwise do nothing */

    if ((sysL3crGet() & L3CR_L3E_MASK) == L3CR_L3E_MASK)
	{

	/* save L3CR contents */

	l3crVal = sysL3crGet();

	/* Flush the L3 Cache */

	sysL3CacheFlush();

	/* restore L3CR to original value */

	sysL3crPut(l3crVal);  

	/* Disable the L3 Cache */

	sysL3CacheDisable(); 
	}
    }

/******************************************************************************
*
* sysL3CacheFlush - flush the L3 cache
*
* This routine flushes the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheFlush (void)
    {

    /* Lock the L3 Cache */

    sysL3CacheLock();

    /* perform hardware flush be setting HWF bit in L3CR */

    sysL3crPut(sysL3crGet() | L3CR_L3HWF_MASK);

    /* poll L3CR until HWF bit is cleared, flush complete */

    while (sysL3crGet() & L3CR_L3HWF_MASK)   
	;
    }

/******************************************************************************
*
* sysL3CacheLock - Lock the L3 cache
*
* This routine locks the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheLock (void)
    {

    /*
     * Lock the L3cache by setting the L3IO (L3 Instruction-only mode)
     * and L3DO (L3 Data-only mode) bits.
     */

    sysL3crPut(sysL3crGet() | (L3CR_L3IO_MASK | L3CR_L3DO_MASK));
    }

/******************************************************************************
*
* sysL3CacheUnlock - Unlock the L3 cache
*
* This routine unlocks the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheUnlock (void)
    {

    /* Unlock the L3 Cache by clearing the L3IO and L3DO bits */

    sysL3crPut(sysL3crGet() & ~(L3CR_L3IO_MASK | L3CR_L3DO_MASK));
    }

/******************************************************************************
*
* sysL3CacheInv - Globally invalidate the L3 cache
*
* This routine globally invalidates the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheInv (void)
    {

    /* 
     * Perform Global Invalidation:
     *
     * DSSALL
     * SYNC
     * set L3CR[L3I] = 1
     * poll L3CR[L3I] until clear
     */

    DSSALL;

    SYNC;

    sysL3crPut(sysL3crGet() | L3CR_L3I_MASK);

    while (sysL3crGet() & L3CR_L3I_MASK)
	;
    }

/******************************************************************************
*
* sysL3CacheEnable - Enable the L3 cache
*
* This routine enables the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheEnable (void)
    {

#ifdef	_MPC7457_ERRATA_35
    /* Enable the L3 cache by restoring the L3CR setting computed during init */

    sysL3crPut(l3crSave);
#else	/* _MPC7457_ERRATA_35 */
    /* Enable the L3 cache by setting the L3E bit in the L3CR register */

    sysL3crPut(sysL3crGet() | L3CR_L3E_MASK);
#endif	/* _MPC7457_ERRATA_35 */
    }

/******************************************************************************
*
* sysL3CacheDisable - Disable the L3 cache
*
* This routine disables the L3 cache.
*
* RETURNS: N/A
*
* ERRNO 
*/

void sysL3CacheDisable (void)
    {

    /* Disable the L3 Cache by clearing the L3E bit in the L3CR register */

#ifdef	_MPC7457_ERRATA_35
    sysL3crPut(sysL3crGet() & ~(L3CR_L3E_MASK | L3CR_L3PE_MASK | L3CR_L3CLKEN_MASK));
#else	/* _MPC7457_ERRATA_35 */
    sysL3crPut(sysL3crGet() & ~L3CR_L3E_MASK);
#endif	/* _MPC7457_ERRATA_35 */
    }
