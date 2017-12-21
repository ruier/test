/* usrvxTestSysParams.c - vxTest System Params module source */

/* Copyright (c) 2005-2007 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01g,21feb07,pes  Add support for new MIPS lib structure.
01f,29mar06,slk SPR 119252: add support for vxTestTimestampApiStrict
01e,07feb06,rlg fix pcPentium name problem
01d,18jan06,jmt Fix pcPentium compiler warnings
01c,11jan06,jmt Add NVRAM system parameters
01b,20dec05,rlg added get routines to support tmSysctl
01a,07dec05,jmt creation
*/

/*
DESCRIPTION
This module provides system parameter get functions 

INCLUDE FILES: 	vxTest.h 
*/

/* includes */
#include <vxWorks.h>
#include <aimCacheLib.h>
#include <aimMmuBaseLib.h>
#include "vxTest.h"


/* typedefs */

/* external symbols */
extern MMU_ARCH_LIB_ATTRS mmuArchLibAttrs;
extern CACHEGLOBALATTR cacheAimInfo[];

/* globals */

/* locals */

/* forward declarations */


/***************************************************************************
*
* vxTestSysModelGet - return SYS_MODEL  
*
* RETURNS: SYS_MODEL.
*           
* ERRNO:
*/
const char * vxTestSysModelGet ()
    {
    return SYS_MODEL;
    }


/***************************************************************************
*
* vxTestSysClkRateGet - return SYS_CLK_RATE
*
* RETURNS: SYS_CLK_RATE.
*           
* ERRNO:
*/
int vxTestSysClkRateGet ()
    {
    return SYS_CLK_RATE;
    }


/***************************************************************************
*
* vxTestSysClkRateMaxGet - return SYS_CLK_RATE_MAX
*
* RETURNS: SYS_CLK_RATE_MAX.
*           
* ERRNO:
*/
int vxTestSysClkRateMaxGet ()
    {
    return SYS_CLK_RATE_MAX;
    }

/***************************************************************************
*
* vxTestSysClkRateMinGet - return SYS_CLK_RATE_MIN
*
* RETURNS: SYS_CLK_RATE_MIN.
*           
* ERRNO:
*/
int vxTestSysClkRateMinGet ()
    {
    return SYS_CLK_RATE_MIN;
    }

/***************************************************************************
*
* vxTestAuxClkRateGet - return AUX_CLK_RATE
*
* RETURNS: AUX_CLK_RATE.
*           
* ERRNO:
*/
int vxTestAuxClkRateGet ()
    {
    return AUX_CLK_RATE;
    }

/***************************************************************************
*
* vxTestAuxClkRateMaxGet - return AUX_CLK_RATE_MAX
*
* RETURNS: AUX_CLK_RATE_MAX.
*           
* ERRNO:
*/
int vxTestAuxClkRateMaxGet ()
    {
    return AUX_CLK_RATE_MAX;
    }

/***************************************************************************
*
* vxTestAuxClkRateMinGet - return AUX_CLK_RATE_MIN
*
* RETURNS: AUX_CLK_RATE_MIN.
*           
* ERRNO:
*/
int vxTestAuxClkRateMinGet ()
    {
    return AUX_CLK_RATE_MIN;
    }

/***************************************************************************
*
* vxTestBspVersionGet - return the bsp version number
*
* RETURNS: the concatination of BSP_VERSION and BSP_REV
*
* ERRNO:
*/
const char * vxTestBspVersionGet ()
    {
    return (BSP_VERSION BSP_REV);

    }

/***************************************************************************
*
* vxTestCpuFamilyGet - return the cpu family string
*
* RETURNS: cpu family string
*
* ERRNO:
*/
const char * vxTestCpuFamilyGet ()
    {
#ifdef CPU_VARIANT
    /* skip over the _ 
    *  actually want to point at array[1] (mti24kx) not
    *  array[0] (_mti24kx)
    *
    * NOTE MKSTR is defined in config/comps/src/usrWdbCore.c
    */
    return(MKSTR (CPU_VARIANT)+1);
#else
    return ("");
#endif
    }

/*************************************************************************** 
*
* vxTestHwArchGet - return the architecture string 
* 
* RETURNS: architecture string *
*
* ERRNO:
*/
const char * vxTestHwArchGet ()
    {
#if ((CPU == I80X86) || (CPU == PENTIUM) || (CPU == PENTIUM2) || \
     (CPU == PENTIUM3) || (CPU == PENTIUM4))
       return("PENTIUM");
#elif _WRS_MIPS_32BIT_CPU || _WRS_MIPS_64BIT_CPU
       return("MIPS");
#elif (CPU == ARM)
       return("ARM");
#elif ((CPU == PPC) || (CPU == PPC32) || (CPU == PPC603) || \
       (CPU == PPC604) || (CPU == PPC403) || (CPU == PPC405) || \
       (CPU == PPC440) || (CPU == PPC860) || (CPU == PPC970))
       return("PPC");
#elif ((CPU == SH) || (CPU == SH32))
       return("SH");
#elif (CPU == COLDFIRE)
       return("COLDFIRE");
#elif (CPU == I960)
       return("I960");
#else
       return("HW_ARCH not defined");
#endif
    }

/***************************************************************************
*
* vxTestCacheNameGet - return the name of the cache
*
* RETURNS: cache name  string (either d 0r i )
*
* ERRNO: returns "" if not data or instruction cache 
*/
const char * vxTestCacheNameGet (int index)
    {
    if (index == CACHE_TYPE_D)
        return(DCACHE_NAME);
    else if (index == CACHE_TYPE_I)
        return(ICACHE_NAME);
    else
        /* doesn't return an error string - cases where null is valid */
        return ("");
    }

/***************************************************************************
*
* vxTestCacheSizeGet - return the size of the cache
*
* RETURNS: cache size based up CACHE_TYPE
*
* ERRNO: returns 0  if not data or instruction cache
*/
u_int vxTestCacheSizeGet (int index)
    {
    if (index == CACHE_TYPE_D)
        return(cacheAimInfo[CACHE_TYPE_D].cacheSize);
    else if (index == CACHE_TYPE_I)
        return(cacheAimInfo[CACHE_TYPE_I].cacheSize);
    else
        return (0);
    }

/***************************************************************************
*
* vxTestCacheLineSizeGet - return the size of the cache
*
* RETURNS: cache line size based up CACHE_TYPE
*
* ERRNO: returns 0  if not data or instruction cache
*/
u_int vxTestCacheLineSizeGet (int index)
    {
    if (index == CACHE_TYPE_D)
        return(cacheAimInfo[CACHE_TYPE_D].cacheLineSize);
    else if (index == CACHE_TYPE_I)
        return(cacheAimInfo[CACHE_TYPE_I].cacheLineSize);
    else
        return (0);
    }

/***************************************************************************
*
* vxTestPageSizeGet - return the size of the mmu page
*
* RETURNS: mmu page size
*
* ERRNO: returns 0  if not data or instruction cache
*/
u_int vxTestPageSizeGet ()
    {
        return(mmuArchLibAttrs.pageSize);
    }

/***************************************************************************
*
* vxTestNvRamSizeGet - return NV_RAM_SIZE
*
* RETURNS: NV_RAM_SIZE.
*           
* ERRNO:
*/
int vxTestNvRamSizeGet ()
    {
    return NV_RAM_SIZE;
    }

/***************************************************************************
*
* vxTestNvRamWriteableSizeGet - return Writeable NVRAM size
*
* RETURNS: writeable NVRAM size.
*           
* ERRNO:
*/
int vxTestNvRamWriteableSizeGet ()
    {
#ifdef NV_RAM_SIZE_WRITEABLE
    return NV_RAM_SIZE_WRITEABLE;
#else
    return NV_RAM_SIZE;
#endif   /* ifdef NV_RAM_SIZE_WRITEABLE */
    }

/***************************************************************************
*
* vxTestNvRamBootOffsetGet - return NV_BOOT_OFFSET
*
* RETURNS: NV_BOOT_OFFSET.
*           
* ERRNO:
*/
int vxTestNvRamBootOffsetGet ()
    {
    return NV_BOOT_OFFSET;
    }

/***************************************************************************
*
* vxTestTimestampApiStrict - return TIMESTAMP_API_STRICT
*
* RETURNS: TIMESTAMP_API_STRICT.
*           
* ERRNO: N/A
*/

BOOL vxTestTimestampApiStrict ()
    {
    return TIMESTAMP_API_STRICT;
    }
