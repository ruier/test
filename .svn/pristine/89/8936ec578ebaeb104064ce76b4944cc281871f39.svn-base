/* l2Lib.c - IBM 440GX L2 cache controller support */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBMÆS OR ITS LICENSORÆS
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2003-2004, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
#include "copyright_wrs.h"

/*
modification history
--------------------
01f,02sep08,x_f  Replace sysDcrInLong with dcrInLong, 
                 sysDcrOutLong with dcrOutLong
01e,30aug04,mdo  Documentation fixes for apigen
01d,03sep03,jtp  Support L2 cache via cacheLib function pointer.
01c,27aug03,jtp  Note L2 data parity is not presently supported
01b,06aug03,jtp  Formal code inspection changes. cacheEnable/Disable
		 and interrupt locking; sysUsDelay for array waits.
01a,21jan03,saw  created 
*/

/*
DESCRIPTION

This library contains support for the on-chip L2 cache controller, which
Provides L2 cache or on-chip memory capability.

INCLUDE FILES:

*/

/* includes */

/* Externals */

IMPORT UINT32 dcrInLong(UINT32);
IMPORT void   dcrOutLong(UINT32,UINT32);

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;

/* macros */

/* L2 global cache operation timeout constants */

#define L2CACHEOP_LOOP_USEC	   (1*1000) /* 1ms expressed in us */
#define L2CACHEOP_TIMEOUT_LOOPS	   100	    /* number of loops before timeout */


/***************************************************************************
*
* l2CacheDeactivate - turn off L2 cache mode of SRAM0 device
*
* This routine disables the L2 cache configuration of the onchip SRAM0
* device.
*
* Only sysHwInit() and the architecture cacheLib functions should call
* this function. At a minimum, the caller must ensure that interrupts
* are locked out before calling it.
*
* RETURNS:
*
* ERRNO
*/

LOCAL void l2CacheDeactivate(void)
    {
    UINT32 tmp;

    /* turn off L2 cache unit */
    tmp = dcrInLong(L2C0_CFG) & 
	       ~(L2C_CFG_L2M_MASK | L2C_CFG_SS_MASK | 
		 L2C_CFG_ICU_MASK | L2C_CFG_DCU_MASK);
    dcrOutLong(L2C0_CFG, tmp | 
	       (L2C_CFG_L2M_DISABLE | L2C_CFG_SS_256KB |
		L2C_CFG_ICU_DISABLE | L2C_CFG_DCU_DISABLE));
    SYNC;

    /* disable snooping regions */
    tmp = dcrInLong(L2C0_SNP0) & ~L2C_SNPX_ESR_MASK;
    dcrOutLong(L2C0_SNP0, tmp | L2C_SNPX_ESR_DISABLE);
    tmp = dcrInLong(L2C0_SNP1) & ~L2C_SNPX_ESR_MASK;
    dcrOutLong(L2C0_SNP1, tmp | L2C_SNPX_ESR_DISABLE);
    }

/***************************************************************************
*
* l2SramInit - configure SRAM0 and enable as a memory array
*
* This routine disables any previous memory or L2 cache configuration of
* the onchip SRAM0 device, and enables it for access as a memory region.
*
* Only sysHwInit() and the architecture cacheLib functions should call
* this function. At a minimum, the caller must ensure that interrupts
* are locked out before calling it.
*
* RETURNS:
*
* ERRNO
*/

void l2SramInit(void)
    {
    UINT32 tmp;

    SYNC;

    l2CacheDeactivate();

    /* Clear Bus Error Status Registers */
    dcrOutLong(SRAM0_BESR0, 0);
    dcrOutLong(SRAM0_BESR1, 0);

    /* Data Parity is not supported yet */
    tmp = dcrInLong(SRAM0_DPC) & ~SRAM_DPC_DPC_MASK;
    dcrOutLong(SRAM0_DPC, tmp | SRAM_DPC_DPC_DISABLE);

    /* Clear Power Management Register */
    tmp = dcrInLong(SRAM0_PMEG) & ~SRAM_PMEG_MASK;
    dcrOutLong(SRAM0_PMEG, tmp);

    /* Memory configuration Registers */
    tmp = dcrInLong(SRAM0_SB0CR) & ~SRAM_SBXCR_MASK;
    dcrOutLong(SRAM0_SB0CR, tmp |
		SRAM_SBXCR_BAS_ENCODE(SRAM0_BASE_PLB) | SRAM_SBXCR_BS_64KB |
		SRAM_SBXCR_BU_RW | SRAM_SBXCR_UA_ENCODE(SRAM0_BASE_PLB_UA));
    tmp = dcrInLong(SRAM0_SB1CR) & ~SRAM_SBXCR_MASK;
    dcrOutLong(SRAM0_SB1CR, tmp |
		SRAM_SBXCR_BAS_ENCODE(SRAM0_BASE_PLB+0x00010000) | 
		SRAM_SBXCR_BS_64KB |
		SRAM_SBXCR_BU_RW | SRAM_SBXCR_UA_ENCODE(SRAM0_BASE_PLB_UA));
    tmp = dcrInLong(SRAM0_SB2CR) & ~SRAM_SBXCR_MASK;
    dcrOutLong(SRAM0_SB2CR, tmp |
		SRAM_SBXCR_BAS_ENCODE(SRAM0_BASE_PLB+0x00020000) | 
		SRAM_SBXCR_BS_64KB |
		SRAM_SBXCR_BU_RW | SRAM_SBXCR_UA_ENCODE(SRAM0_BASE_PLB_UA));
    tmp = dcrInLong(SRAM0_SB3CR) & ~SRAM_SBXCR_MASK;
    dcrOutLong(SRAM0_SB3CR, tmp |
		SRAM_SBXCR_BAS_ENCODE(SRAM0_BASE_PLB+0x00030000) | 
		SRAM_SBXCR_BS_64KB |
		SRAM_SBXCR_BU_RW | SRAM_SBXCR_UA_ENCODE(SRAM0_BASE_PLB_UA));

    return;
    }


#if (defined(INCLUDE_L2_CACHE) && defined(USER_D_CACHE_ENABLE) && \
     USER_D_CACHE_MODE != CACHE_DISABLED)

/***************************************************************************
*
* l2CacheCommand - issue an L2 cache command and wait for completion
*
* This routine issues an L2 cache command and loops waiting for
* completion. If the call does not complete, it will time out.
*
* RETURNS:  OK on success, ERROR on timeout
*
* ERRNO
*/

LOCAL STATUS l2CacheCommand
    (
    UINT32 command		/* L2C0_CMD register value to set */
    )
    {
    int count;

    /* set L2_ADDR with all zeros */
    dcrOutLong(L2C0_ADDR, 0);

    /* issue command via L2_CMD */
    dcrOutLong(L2C0_CMD, command);

    /* poll L2C0_SR for completion */
    for (count = L2CACHEOP_TIMEOUT_LOOPS;
        count > 0 && (dcrInLong(L2C0_SR) & L2C_SR_CC) == 0;
        count--)
        {
        sysUsDelay(L2CACHEOP_LOOP_USEC);
        }

    if (count <= 0)
        {
        return ERROR;
        }

    return OK;
    }


/***************************************************************************
*
* l2CacheEnable - configure SRAM0 as an L2 cache
*
* This routine disables any previous memory configuration of the onchip
* SRAM0 device, and enables it for use as an L2 cache.  If any errors
* occur, the SRAM0 L2 cache mode will be disabled and the SRAM will be
* left in an unconfigured state.
*
* Only the architecture cacheLib functions should call this function. At
* a minimum, the caller must ensure that interrupts are locked out
* before calling it.
*
* RETURNS:  OK if L2 cache was enabled; ERROR if timeout occurred
*
* ERRNO
*/

LOCAL STATUS l2CacheEnable(void)
    {
    UINT32 mode;
    int tmp;

    /* Disable SRAM memory mode by turning off DPC and bank registers */
    tmp = dcrInLong(SRAM0_DPC) & ~SRAM_DPC_DPC_MASK;
    dcrOutLong(SRAM0_DPC, tmp | SRAM_DPC_DPC_DISABLE);

    tmp = dcrInLong(SRAM0_SB0CR) & ~SRAM_SBXCR_BU_MASK;
    dcrOutLong(SRAM0_SB0CR, tmp | SRAM_SBXCR_BU_DIS);
    tmp = dcrInLong(SRAM0_SB1CR) & ~SRAM_SBXCR_BU_MASK;
    dcrOutLong(SRAM0_SB1CR, tmp | SRAM_SBXCR_BU_DIS);
    tmp = dcrInLong(SRAM0_SB2CR) & ~SRAM_SBXCR_BU_MASK;
    dcrOutLong(SRAM0_SB2CR, tmp | SRAM_SBXCR_BU_DIS);
    tmp = dcrInLong(SRAM0_SB3CR) & ~SRAM_SBXCR_BU_MASK;
    dcrOutLong(SRAM0_SB3CR, tmp | SRAM_SBXCR_BU_DIS);
    SYNC;

    /* turn on L2 cache unit, but leave I and D access disabled */
    tmp = dcrInLong(L2C0_CFG) & 
	       ~(L2C_CFG_L2M_MASK | L2C_CFG_SS_MASK | 
		 L2C_CFG_ICU_MASK | L2C_CFG_DCU_MASK);
    dcrOutLong(L2C0_CFG, tmp | 
	       (L2C_CFG_L2M_ENABLE | L2C_CFG_SS_256KB |
		L2C_CFG_ICU_DISABLE | L2C_CFG_DCU_DISABLE));
    SYNC;

    /*
     * clear all data and errors from L2 Cache; if any of these fail,
     * abort setting up L2 as cache.
     */

    /* issue HCLEAR command */
    if (l2CacheCommand(L2C_CMD_HCC_ENABLE) != OK)
        {
        l2CacheDeactivate();
        return ERROR;
        }

    /* clear cache errors via L2C0_CMD[CCP] */
    if (l2CacheCommand(L2C_CMD_CCP_ENABLE) != OK)
        {
        l2CacheDeactivate();
        return ERROR;
        }

    /* clear tag errors via L2C_CMD[CTE] */
    if (l2CacheCommand(L2C_CMD_CTE_ENABLE) != OK)
        {
        l2CacheDeactivate();
        return ERROR;
        }

    /* configure L2C0_SNP0 and L2C0_SNP1 to enable snooping */
    tmp = dcrInLong(L2C0_SNP0) & 
          ~(L2C_SNPX_BA_MASK | L2C_SNPX_SSR_MASK | L2C_SNPX_ESR_MASK);
    dcrOutLong(L2C0_SNP0, tmp |
                  (0 | L2C_SNPX_SSR_16GB | L2C_SNPX_ESR_ENABLE));
    tmp = dcrInLong(L2C0_SNP1) &
          ~(L2C_SNPX_BA_MASK | L2C_SNPX_SSR_MASK | L2C_SNPX_ESR_MASK);
    dcrOutLong(L2C0_SNP1, tmp |
                  (0 | L2C_SNPX_SSR_16GB | L2C_SNPX_ESR_DISABLE));
    SYNC;

    /* complete activation of SRAM0 as L2 cache */

    /* enable PPC440GX I-port/D-port with desired modes       */
    /* L2C_CFG[L2M] = L2 mode enabled.                        */
    /* L2C_CFG[ICU] = ???                                     */
    /* L2C_CFG[DCU] = ???                                     */
    /* L2C_CFG[DCW] = enable all ways.                        */
    /* L2C_CFG[TPC] = enable tag parity.                      */
    /* L2C_CFG[CPC] = enable cache parity.                    */
    /* L2C_CFG[FRAN]= enable fast read acknowledge.           */
    /* L2C_CFG[SS]  = 256KB SRAM size.                        */
    /* L2C_CFG[CPIM]= disable cache parity interrupt.         */
    /* L2C_CFG[TPIM]= disable tag parity interrupt.           */
    /* L2C_CFG[LIM] = disable LRU code point error interrupt. */
    /* L2C_CFG[PMUX]= disable monitor snooping.               */
    /* L2C_CFG[PMIM]= disable performance monitor interrupt.  */
    /* L2C_CFG[TPEI]= disable tag parity error inject.        */
    /* L2C_CFG[CPEI]= disable cache parity error inject.      */
    /* L2C_CFG[NAM] = enable abort mode for best performance. */
    /* L2C_CFG[SMCM]= enable self modifying code support.     */

    /*
     * Turn on both L2 ICU and DCU for now, to be consistent with
     * vxWorks cache library implementation for 440GX.  In theory
     * you could set only ICU or only DCU.
     */
    mode = L2C_CFG_ICU_ENABLE | L2C_CFG_DCU_ENABLE;

    tmp = dcrInLong(L2C0_CFG) &
          ~(L2C_CFG_L2M_MASK |
            L2C_CFG_ICU_MASK | L2C_CFG_DCU_MASK |
            L2C_CFG_DCW_MASK | L2C_CFG_TPC_MASK |
            L2C_CFG_CPC_MASK | L2C_CFG_FRAN_MASK |
            L2C_CFG_SS_MASK | L2C_CFG_CPIM_MASK |
            L2C_CFG_TPIM_MASK | L2C_CFG_LIM_MASK |
            L2C_CFG_PMUX_MASK | L2C_CFG_PMIM_MASK |
            L2C_CFG_TPEI_MASK | L2C_CFG_CPEI_MASK |
            L2C_CFG_NAM_MASK | L2C_CFG_SMCM_MASK);
    dcrOutLong(L2C0_CFG, tmp |
                  L2C_CFG_L2M_ENABLE |
                  mode |
                  L2C_CFG_DCW_ENABLE0123 | L2C_CFG_TPC_ENABLE |
                  L2C_CFG_CPC_ENABLE | L2C_CFG_FRAN_ENABLE |
                  L2C_CFG_SS_256KB | L2C_CFG_CPIM_DISABLE |
                  L2C_CFG_TPIM_DISABLE | L2C_CFG_LIM_DISABLE |
                  L2C_CFG_PMUX_DISABLE | L2C_CFG_PMIM_DISABLE |
                  L2C_CFG_TPEI_DISABLE | L2C_CFG_CPEI_DISABLE |
                  L2C_CFG_NAM_ENABLE | L2C_CFG_SMCM_ENABLE);

    /* msync */
    SYNC;

    return OK;
    }

/***************************************************************************
*
* l2CacheDisable - deactivate L2 cache mode of SRAM0 device
*
* This routine disables the L2 cache mode of the onchip SRAM0 device.
* Only the architecture cacheLib functions should call this function. At
* a minimum, the caller must ensure that interrupts are locked out
* before calling it.
*
* RETURNS:
*
* ERRNO
*/

LOCAL void l2CacheDisable(void)
    {
    l2SramInit();
    }

/***************************************************************************
*
* l2CacheInit - inform cacheLib how to activate/deactivate the L2 cache
*
* This routine sets function pointers that inform the architecture
* library portion of cacheLib of entry points for enabling and disabling
* the L2 cache.  This routine should only be called before the cache is
* first enabled, by sysHwInit().
*
* RETURNS:
*
* ERRNO
*/

void l2CacheInit(void)
    {
    /* inform cache library of our L2 cache functions */
    _pSysL2CacheInvFunc = NULL;
    _pSysL2CacheEnable  = (VOIDFUNCPTR)l2CacheEnable;
    _pSysL2CacheFlush   = NULL;
    _pSysL2CacheDisable = (VOIDFUNCPTR)l2CacheDisable;
    }

#endif /* INCLUDE_L2_CACHE && USER_D_CACHE_ENABLE &&
	  USER_D_CACHE_MODE != CACHE_DISABLED */
