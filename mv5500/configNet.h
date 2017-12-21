/* configNet.h - network configuration header */

/*
 * Copyright 2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 2002 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01c,22sep07,y_w add SM device support.
01b,07oct04,jln spr#102216
01a,28oct02,scb Adapted from hxeb100 base (version 01d).
*/
 
#ifndef __INCnetConfigh
#define __INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include "vxWorks.h"
#include "end.h"

#define END_BUFF_LOAN   1

#ifdef INCLUDE_GEI_END
#   define GEI_LOAD_FUNC sysGei8254xEndLoad
#   define GEI_STRING ""

IMPORT END_OBJ* GEI_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_GEI_END */

#ifdef INCLUDE_WANCOM_END
#   define  WANCOM_LOAD_FUNC               sysWancomEndLoad
#   define  WANCOM0_STRING ""

IMPORT END_OBJ* WANCOM_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_WANCOM_END */

#ifdef INCLUDE_SMEND
#   define SMEND_LOAD_STRING    ""
#   define SMEND_LOAD_FUNC      sysSmEndLoad
IMPORT END_OBJ* SMEND_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_SMEND */

END_TBL_ENTRY endDevTbl [] =
{

#ifdef INCLUDE_GEI_END
    { 0, GEI_LOAD_FUNC, GEI_STRING, END_BUFF_LOAN, NULL, FALSE},
#endif /* INCLUDE_GEI_END */

#ifdef INCLUDE_WANCOM_END
    { 0, WANCOM_LOAD_FUNC, WANCOM0_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_WANCOM_END */

#ifdef INCLUDE_SMEND
    { 0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_SMEND */

    { 0, END_TBL_END, NULL, 0, NULL, FALSE},
};

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS 
#   define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCnetConfigh */
