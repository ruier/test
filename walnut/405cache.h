/* 405cache.h - optimized 405 data cache flush and invalidate functions */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
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

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,03sep04,mdo  Documentation fixes for apigen
01b,12jun02,pch  SPR 74987: C++
01a,21may01,mcg  created
*/

/*
This file contains the definitions of optimized 405 data cache flush and
invalidate functions.
*/

#ifndef INCcacheh
#define INCcacheh

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef _ASMLANGUAGE

#include "cacheLib.h"

/*
 *  405 data cache functions
 */

void    cacheFlush405 (CACHE_TYPE cache, void * adrs, size_t bytes);
void    cacheInvalidate405 (CACHE_TYPE cache, void * adrs, size_t bytes);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif  /* INCcacheh */
