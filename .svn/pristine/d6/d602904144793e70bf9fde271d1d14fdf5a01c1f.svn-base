/* sysCacheLockLib.h - cache locking support header */

/*
 * Copyright (c) 2000-2001 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,25oct01,g_h  Remove underscore from functions name.
01a,12dec00,ksn  created (teamf1)
*/

#ifndef __INCsysCacheLockLibH
#define __INCsysCacheLockLibH

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
    
/* defines */

    
#ifndef _ASMLANGUAGE
STATUS sysCacheLock(CACHE_TYPE cacheType, void * adrs, size_t bytes);
STATUS sysCacheUnlock(CACHE_TYPE cacheType);
    
extern void  sysL1CacheLock(CACHE_TYPE cacheType, void * adrs, size_t bytes);
extern void  sysL1CacheUnlock(CACHE_TYPE cacheType);    
extern void  SYSL1CACHELOCK_STARTADRS(void);
extern void  SYSL1CACHELOCK_ENDADRS(void);
#endif    
#ifdef __cplusplus
}
#endif

#endif /* __INCsysCacheLockLibH */

