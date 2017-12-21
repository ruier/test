/*  sysL2Cache.h - defines registers and values for L2 cache control */

/* Copyright 2007 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01b,11may07,jjm  Initial version, based on IBM 750GX/GL EVB BSP
01a,02sep05,avb  written from sp7447/sysL2BackCache.h
*/

/*
DESCRIPTION

This file defines registers and values needed in defining L2 cache functions.
*/

#ifndef __INCsysL2Cacheh
#define __INCsysL2Cacheh

#ifdef __cplusplus
extern "C" {
#endif

#include <vxWorks.h>

/* L2 Cache Definitions for MPC74xx and PPC750FX/GX with L2 cache. */

#define L2CR_REG                1017     /* l2CR register number */

#define BUFFER_ADDR             0x100000 /* buffer address for read */
#define L1CACHE_ALIGN_SIZE      32       /* 32 bytes */

#define L2CACHE_SIZE_2M     0x200000 /* 2MB */
#define L2CACHE_SIZE_1M		0x100000 /* 1MB */
#define L2CACHE_SIZE_512K	0x80000	 /* 512KB */
#define L2CACHE_SIZE_256K	0x40000	 /* 256KB */

/*
 * Values for L2CR register: 
 */

/* L2 Cache Enable */
#define L2CR_EN_U               0x8000  /* L2CR enable bit */
#define L2CR_DISABLE_MASK_U     0x7fff	/* Disable L2 - upper 16 bits  */

/* L2 Data Parity generation and checking enable */
#define L2CR_PE                 0x4000

/* L2 data only */
#define L2DO_MPC745X            0x0001  /* upper 16-bit */
#define L2DO_PPC750             0x0040  /* upper 16-bit */

/* L2 Ins. only mode */
#define L2IO_MPC745X            0x0010  /* upper 16-bit */
#define L2IO_PPC750             0x0008  /* lower 16-bit, GX only */

/* L2 Global invalidate */
#define L2CR_GBL_INV_U   	0x0020
    
/* L2 Ram Control (ZZ enable) */
#define L2CR_RAMCTL             0x0010

/* L2 Write Through */
#define L2CR_WT                 0x0008

/* L2 Test Support */
#define L2CR_TS                 0x0004

/* L2 Output Hold */
#define L2CR_OH_0_5ns           0x0000
#define L2CR_OH_1_0ns           0x0001
#define L2CR_OH_MORE            0x0002
#define L2CR_OH_EVEN_MORE       0x0003

#define L2CR_HWFLUSH		0x0800  /* HW flush for 74X0 lower 16 bits*/
    
    
#define L2CR_CFG		0


#define L2CR_IP			0x0001  /* Invalidation in progress */

#ifndef _ASMLANGUAGE

IMPORT void  sysL2CacheGlobalInv(void);
IMPORT void  sysL2CacheHWFlush(void);
IMPORT void  sysL2CacheSWFlush(void);
IMPORT void  sysL2CacheInit(void);
IMPORT void  sysL2CacheEnable(CACHE_TYPE cache);
IMPORT void  sysL2CacheDisable(void);
IMPORT void  sysL2CacheSetWT(void);    

#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCsysL2Cacheh */
