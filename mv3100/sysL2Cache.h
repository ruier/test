/* sysL2Cache.h -  L2 cache defines */

/*
 * Copyright (c) 2003-2005, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01e,07may13,d_l  Fixed build warnings.
01d,15feb05,cak  Added bit and mask definitions for the L2 error registers.
01c,07feb05,cak  Added some CONFIG definitions.
01b,23aug04,scb  Starting point for "Big Easy".
01a,11jun03,dtr  Created
*/

#ifndef __INCsysL2Cacheh
#define __INCsysL2Cacheh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* L2 Cache/SRAM Configuration Definitions */

#define	L2_CACHE_SRAM_CONFIG_1		1
#define	L2_CACHE_SRAM_CONFIG_2		2
#define	L2_CACHE_SRAM_CONFIG_3		3
#define	L2_CACHE_SRAM_CONFIG_4		4
#define	L2_CACHE_SRAM_CONFIG_5		5
#define	L2_CACHE_SRAM_CONFIG_6		6

/* L2 Cache defines */

/* L2CTL BIT MASKS and BIT SHIFTS */

#define M85XX_L2CTL_L2E_BIT      31
#define M85XX_L2CTL_L2E_MSK      ((UINT32)(1 << M85XX_L2CTL_L2E_BIT))

#define M85XX_L2CTL_L2I_BIT      30
#define M85XX_L2CTL_L2I_MSK      (1 << M85XX_L2CTL_L2I_BIT) 

#define M85XX_L2CTL_L2SIZ_BIT    28
#define M85XX_L2CTL_L2SIZ_MSK    (3 << M85XX_L2CTL_L2SIZ_BIT) /* Read Only */

/* Size vs bit value */ 

#define L2SIZ_512KB		3
#define L2SIZ_256KB		2
#define L2SIZ_128KB		1
#define L2SIZ_0KB		0

#define L2SRAM_BLK_CACHE	0
#define L2SRAM_BLK_SRAM		1
#define L2SRAM_BLK_1_CACHE	0
#define L2SRAM_BLK_0_SRAM	1
#define L2SRAM_CACHE_SRAM	2
#define L2SRAM_SRAM		3

#define M85XX_L2CTL_L2BLKSIZ_BIT 	26
#define M85XX_L2CTL_L2BLKSIZ_MSK 	(3<<M85XX_L2CTL_L2BLKSIZ_BIT)

#define M85XX_L2CTL_L2DO_BIT     	22
#define M85XX_L2CTL_L2DO_MSK           	(1<M85XX_L2CTL_L2DO_BIT)

#define M85XX_L2CTL_L2IO_BIT     	21
#define M85XX_L2CTL_L2IO_MSK     	(1 << M85XX_L2CTL_L2IO_BIT)

#define M85XX_L2CTL_MODE_BIT		M85XX_L2CTL_L2IO_BIT
#define M85XX_L2CTL_MODE_MASK		(3 << M85XX_L2CTL_L2IO_BIT)
#define L2_MODE_INST_AND_DATA		(0 << M85XX_L2CTL_L2IO_BIT)
#define L2_MODE_I_ONLY			(1 << M85XX_L2CTL_L2IO_BIT)
#define L2_MODE_D_ONLY			(2 << M85XX_L2CTL_L2IO_BIT)

#define M85XX_L2CTL_L2MEXTDIS_BIT 	20
#define M85XX_L2CTL_L2MEXTDIS_MSK 	(1 << M85XX_L2CTL_L2MEXTDIS_BIT)

#define M85XX_L2CTL_L2INTDIS_BIT  	19
#define M85XX_L2CTL_L2INTDIS_MSK  	(1 << M85XX_L2CTL_L2INTDIS_BIT)

#define M85XX_L2CTL_L2SRAM_BIT    	16
#define M85XX_L2CTL_L2SRAM_MSK    	(7 << M85XX_L2CTL_L2SRAM_BIT)

#define M85XX_L2CTL_L2LO_BIT      	13
#define M85XX_L2CTL_L2LO_MSK      	(1 << M85XX_L2CTL_L2LO_BIT)

#define M85XX_L2CTL_L2SLC_BIT     	12 
#define M85XX_L2CTL_L2SLC_MSK     	(1 << M85XX_L2CTL_L2SLC_BIT)

#define M85XX_L2CTL_L2LFR_BIT     	10
#define M85XX_L2CTL_L2LFR_MSK     	(1 <<  M85XX_L2CTL_L2LFR_BIT)

#define M85XX_L2CTL_L2LFRID_BIT   	8
#define M85XX_L2CTL_L2LFRID_MSK   	(3 << M85XX_L2CTL_L2LFRID_BIT)


/* L2CWAR BIT MASK */

#define M85XX_L2CEWAR_ADDR_MSK          0xFFFFFF00

/* L2CEWCR BIT MASK and BIT SHIFT*/

#define M85XX_L2CEWCR_E_BIT            	31
#define M85XX_L2CEWCR_E_MSK            	((UINT32)(1 << M85XX_L2CEWCR_E_BIT))

#define M85XX_L2CEWCR_LOCK_BIT         	30
#define M85XX_L2CEWCR_LOCK_MSK         	(1 << M85XX_L2CEWCR_LOCK_BIT)

#define M85XX_L2CEWCR_SIZMASK_BIT      	0
#define M85XX_L2CEWCR_SIZMASK_MSK      	(0xFFFFFF << M85XX_L2CEWCR_SIZMASK_BIT)

/* L2SRBAR MASK and SHIFT */

#define M85XX_L2SRBAR_ADDR_BIT         	14
#define M85XX_L2SRBAR_ADDR_MSK         	((UINT32)(0xFFFFC << M85XX_L2SRBAR_ADDR_BIT))

/* L2 Error Detect Register Bit & Mask definitions */

#define M85XX_L2ERRDET_MULL2ERR_BIT	31
#define M85XX_L2ERRDET_MULL2ERR_MSK	(1 << M85XX_L2ERRDET_MULL2ERR_BIT)
#define M85XX_L2ERRDET_TPARERR_BIT	4
#define M85XX_L2ERRDET_TPARERR_MSK	(1 << M85XX_L2ERRDET_TPARERR_BIT)
#define M85XX_L2ERRDET_MBECCERR_BIT	3
#define M85XX_L2ERRDET_MBECCERR_MSK	(1 << M85XX_L2ERRDET_MBECCERR_BIT)
#define M85XX_L2ERRDET_SBECCERR_BIT	2
#define M85XX_L2ERRDET_SBECCERR_MSK	(1 << M85XX_L2ERRDET_SBECCERR_BIT)
#define M85XX_L2ERRDET_L2CFGERR_BIT	0
#define M85XX_L2ERRDET_L2CFGERR_MSK	(1 << M85XX_L2ERRDET_L2CFGERR_BIT)

/* L2 Error Interrupt Enable Register Bit & Mask Definitions */

#define M85XX_L2ERRINTEN_TPARINTEN_BIT	4
#define M85XX_L2ERRINTEN_TPARINTEN_MSK	(1 << M85XX_L2ERRINTEN_TPARINTEN_BIT)
#define M85XX_L2ERRINTEN_MBECCINTEN_BIT	3
#define M85XX_L2ERRINTEN_MBECCINTEN_MSK	(1 << M85XX_L2ERRINTEN_MBECCINTEN_BIT)
#define M85XX_L2ERRINTEN_SBECCINTEN_BIT	2
#define M85XX_L2ERRINTEN_SBECCINTEN_MSK	(1 << M85XX_L2ERRINTEN_SBECCINTEN_BIT)
#define M85XX_L2ERRINTEN_L2CFGINTEN_BIT	0
#define M85XX_L2ERRINTEN_L2CFGINTEN_MSK	(1 << M85XX_L2ERRINTEN_L2CFGINTEN_BIT)

/* L2 Error Control Register Bit & Mask Definitions */

#define M85XX_L2ERRCTL_L2CTHRESH_BIT	16
#define M85XX_L2ERRCTL_L2CTHRESH_MSK	(0xFF << M85XX_L2ERRCTL_L2CTHRESH_BIT)

/* L2 Error Attributes Capture Register Bit & Mask Definitions */

#define M85XX_L2ERRATTR_VALINFO_BIT	0
#define M85XX_L2ERRATTR_VALINFO_MSK	(1 << M85XX_L2ERRATTR_VALINFO_BIT)

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCsysL2Cacheh */
