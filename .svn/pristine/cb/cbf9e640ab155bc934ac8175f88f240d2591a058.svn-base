/* configNet.h - network configuration header */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 2002-2004 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01e,09may07,y_w  Add shared-memory related definition.
01d,09mar04,cak  Split the enabling of the primary and secondary 
                 ethernets into two.
01c,10jan03,efb  Added defines for MV64360 gigabit ethernet.
01b,29aug02,cak  #define END_LOAD_FUNC NULL to prevent compile-time warning.
01a,06mar02,cak  Ported. (from ver 01c, mcp820/configNet.h)
*/
 
#ifndef __INCnetConfigh
#define __INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include "vxWorks.h"
#include "end.h"

#define END_LOAD_FUNC    NULL 
#define END_BUFF_LOAN    1
#define END_LOAD_STRING  ""		

#ifdef INCLUDE_GEISC_END
#   define GEISC_LOAD_FUNC     sysMv64360EndLoad
#   define GEISC0_STRING       ""
#   define GEISC1_STRING       ""
IMPORT END_OBJ* GEISC_LOAD_FUNC (char *, void *);
#endif /* INCLUDE_GEISC_END */

#ifdef INCLUDE_SMEND
#   define SMEND_LOAD_STRING    ""
#   define SMEND_LOAD_FUNC      sysSmEndLoad
IMPORT END_OBJ* SMEND_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_SMEND */

END_TBL_ENTRY endDevTbl[] =
{
#ifdef INCLUDE_PRIMARY_ENET
    { 0, GEISC_LOAD_FUNC, GEISC0_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_PRIMARY_ENET */
#ifdef INCLUDE_SECONDARY_ENET
    { 1, GEISC_LOAD_FUNC, GEISC1_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_SECONDARY_ENET */
#ifdef INCLUDE_SMEND
    { 0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif
    { 0, END_TBL_END, NULL, 0, NULL, FALSE}
};

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS 
#   define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCnetConfigh */
