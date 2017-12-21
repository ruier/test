/* configNet.h - network configuration header */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,09jan13,wyt  created
*/

#ifndef __INCnetConfigh
#define __INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* includes */

#include <vxWorks.h>
#include <end.h>

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS         (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
    {0, END_TBL_END, NULL, 0, NULL}     /* must be last */
    };

#ifdef __cplusplus
    }
#endif

#endif /* __INCnetConfigh */

