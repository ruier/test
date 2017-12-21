/* configNet.h - network configuration header */

/*
 * Copyright (c) 2008 Wind River Systems, Inc
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,25nov08,y_w  written from 01c version of mv5500 BSP
*/

#ifndef __INCnetConfigh
#define __INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <end.h>

/* defines */

#define END_BUFF_LOAN           1
#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2

#ifdef INCLUDE_END

/* The END_LOAD_STRING is defined empty and created dynamicaly */

#define END_LOAD_STRING ""      /* created in sys<device>End.c */

#ifdef INCLUDE_SMEND
#   define SMEND_LOAD_STRING    ""
#   define SMEND_LOAD_FUNC      sysSmEndLoad
IMPORT END_OBJ* SMEND_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_SMEND */

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
#ifdef INCLUDE_SMEND
    { 0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_SMEND */
    { 0, END_TBL_END, NULL, 0, NULL, FALSE},
    };

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#endif /* INCLUDE_END */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCnetConfigh */

