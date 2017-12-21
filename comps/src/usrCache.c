/* usrCache.c - cache configuration file */

/*
 * Copyright (c) 1998, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,05dec12,h_k  fixed dependency issues.
01c,10nov12,h_k  changed the L2CSR1 setting configurable for e6500.
01b,01nov12,x_s  added cache configuration for e6500.
01a,02jun98,ms   written
*/

/*
DESCRIPTION
User configurable cache initialization.
*/

#ifdef PPC_e6500
/* IMPORT */

#ifdef	INCLUDE_L2_CACHE
#ifdef	INCLUDE_L2_CACHE_ERROR_RECOVERY
IMPORT BOOL l2CacheErrorRecovery;
IMPORT BOOL l2CacheErrorInjection;
IMPORT UINT l2CacheErrorIntEnable;
IMPORT UINT l2CacheErrorCpcThresh;
#endif	/* INCLUDE_L2_CACHE_ERROR_RECOVERY */

IMPORT UINT32 l2CacheRegAddr;
IMPORT UINT32 l2CacheNumbers;
IMPORT UINT32 l2CacheCsr1;

IMPORT void cacheL2E6500LibInit (void);
void cacheL2LibInit (void);
#endif	/* INCLUDE_L2_CACHE */

#ifdef	INCLUDE_L3_CACHE
#ifdef	INCLUDE_L3_CACHE_ERROR_RECOVERY
IMPORT BOOL l3CacheErrorRecovery;
IMPORT BOOL l3CacheErrorInjection;
IMPORT UINT l3CacheErrorIntEnable;
IMPORT UINT l3CacheErrorCpcThresh;
#endif	/* INCLUDE_L3_CACHE_ERROR_RECOVERY */

IMPORT UINT32 l3CacheRegAddr;
IMPORT UINT32 l3CacheNumbers;
IMPORT UINT32 l3CacheWriteMode;

IMPORT void cacheL3E6500LibInit (void);
void cacheL3LibInit (void);
#endif	/* INCLUDE_L3_CACHE */

#ifdef	INCLUDE_CACHE_ERROR_HANDLER
IMPORT BOOL l1CacheErrorRecovery;
IMPORT BOOL l1CacheErrorInjection;
#endif	/* INCLUDE_CACHE_ERROR_HANDLER */

#ifndef	CCSBAR
#define CCSBAR	0xFE000000
#endif	/* !CCSBAR */

/* Cache Recovery defines */

#ifndef L1_CACHE_ERROR_INJECTION
#define L1_CACHE_ERROR_INJECTION FALSE
#endif

#ifndef L2_CACHE_ERROR_INJECTION
#define L2_CACHE_ERROR_INJECTION FALSE
#endif

#ifndef L2_CACHE_ERROR_INT_ENABLE
#define L2_CACHE_ERROR_INT_ENABLE 0xed
#endif

#ifndef L2_CACHE_ERROR_L2CTHRESH
#define L2_CACHE_ERROR_L2CTHRESH 0x0
#endif

#ifndef L3_CACHE_ERROR_INJECTION
#define L3_CACHE_ERROR_INJECTION FALSE
#endif

#ifndef L3_CACHE_ERROR_INT_ENABLE
#define L3_CACHE_ERROR_INT_ENABLE 0xec
#endif

#ifndef L3_CACHE_ERROR_CPCTHRESH
#define L3_CACHE_ERROR_CPCTHRESH 0x0
#endif

/* L2 Cache defines */

#ifndef L2_CACHE_REG_BASE_ADDR
#define L2_CACHE_REG_BASE_ADDR CCSBAR
#endif

#ifndef L2_CACHE_NUMBERS
#define L2_CACHE_NUMBERS 3
#endif

/* L3 Cache defines */

#ifndef L3_CACHE_REG_BASE_ADDR
#define L3_CACHE_REG_BASE_ADDR CCSBAR
#endif

#ifndef L3_CACHE_NUMBERS
#define L3_CACHE_NUMBERS 3
#endif

#ifndef L3_CACHE_WRITE_MODE
#define L3_CACHE_WRITE_MODE 0
#endif

#ifdef	INCLUDE_CACHE_ERROR_HANDLER
void cacheErrorHandlerConfig (void);
#endif	/* INCLUDE_CACHE_ERROR_HANDLER */
#endif /* PPC_e6500 */

/******************************************************************************
*
* usrCacheEnable - configure the processor caches
*/

STATUS usrCacheEnable (void)
    {
#if defined(PPC_e6500) && defined(INCLUDE_L2_CACHE)
    cacheL2LibInit();
#   ifdef INCLUDE_L3_CACHE
    cacheL3LibInit();
#   endif /* INCLUDE_L3_CACHE */
#endif /* PPC_e6500 && INCLUDE_L2_CACHE */

#ifdef  USER_I_CACHE_ENABLE
    cacheEnable (INSTRUCTION_CACHE);            /* enable instruction cache */
#endif  /* USER_I_CACHE_ENABLE */

#ifdef  USER_D_CACHE_ENABLE
    cacheEnable (DATA_CACHE);                   /* enable data cache */
#endif  /* USER_D_CACHE_ENABLE */

#if	defined(USER_B_CACHE_ENABLE) && (CPU==MC68060)
    cacheEnable (BRANCH_CACHE);                 /* enable branch cache */
#endif  /* USER_B_CACHE_ENABLE */

#if defined(PPC_e6500) && defined(INCLUDE_CACHE_ERROR_HANDLER)
    cacheErrorHandlerConfig();
#endif /* PPC_e6500 && INCLUDE_CACHE_ERROR_HANDLER */

    return (OK);
    }

#ifdef PPC_e6500
#ifdef	INCLUDE_L2_CACHE
/*******************************************************************************
*
* cacheL2LibInit - configure the L2 cache
*
* This routine implements the configuration of L2 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

void cacheL2LibInit (void)
    {
    l2CacheRegAddr = L2_CACHE_REG_BASE_ADDR;
    l2CacheNumbers = L2_CACHE_NUMBERS;
#ifdef	L2_CACHE_CSR1_VAL
    l2CacheCsr1    = L2_CACHE_CSR1_VAL;
#endif	/* L2_CACHE_CSR1_VAL */

    cacheL2E6500LibInit();
    }
#endif	/* INCLUDE_L2_CACHE */

#ifdef	INCLUDE_L3_CACHE
/*******************************************************************************
*
* cacheL3LibInit - configure the L3 cache
*
* This routine implements the configuration of L3 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

void cacheL3LibInit (void)
    {
    l3CacheRegAddr = L3_CACHE_REG_BASE_ADDR;
    l3CacheNumbers = L3_CACHE_NUMBERS;
    l3CacheWriteMode = L3_CACHE_WRITE_MODE;
    cacheL3E6500LibInit();
    }
#endif	/* INCLUDE_L3_CACHE */

#ifdef	INCLUDE_CACHE_ERROR_HANDLER
/*******************************************************************************
*
* cacheErrorHandlerConfig - configure the L1/L2/L3 cache error handler
*
* This routine implements the cache error handler configuration for L1/L2/L3 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/  

void cacheErrorHandlerConfig (void)
    {
#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
    l1CacheErrorRecovery = TRUE;
    l1CacheErrorInjection = L1_CACHE_ERROR_INJECTION;
    cacheL1E6500RecoveryEnable();    
#endif /* INCLUDE_L1_CACHE_ERROR_RECOVERY */

#ifdef INCLUDE_L2_CACHE
#   ifdef INCLUDE_L2_CACHE_ERROR_RECOVERY
    l2CacheErrorRecovery = TRUE;
    l2CacheErrorInjection = L2_CACHE_ERROR_INJECTION;
    l2CacheErrorIntEnable = L2_CACHE_ERROR_INT_ENABLE;
    l2CacheErrorCpcThresh = L2_CACHE_ERROR_L2CTHRESH;
    cacheL2E6500RecoveryEnable();
#   endif /* INCLUDE_L2_CACHE_ERROR_RECOVERY */

#   if defined(INCLUDE_L3_CACHE) && defined(INCLUDE_L3_CACHE_ERROR_RECOVERY)
    l3CacheErrorRecovery = TRUE;
    l3CacheErrorInjection = L3_CACHE_ERROR_INJECTION;
    l3CacheErrorIntEnable = L3_CACHE_ERROR_INT_ENABLE;
    l3CacheErrorCpcThresh = L3_CACHE_ERROR_CPCTHRESH;
    cacheL3E6500RecoveryEnable();
#   endif /* INCLUDE_L3_CACHE && INCLUDE_L3_CACHE_ERROR_RECOVERY */
#endif /* INCLUDE_L2_CACHE */
    }
#endif	/* INCLUDE_CACHE_ERROR_HANDLER */
#endif /* PPC_e6500 */
