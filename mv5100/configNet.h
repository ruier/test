/* configNet.h - network configuration header */

/*
 * Copyright 2001-2002, 2004, 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1999-2000 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01g,26nov07,y_w  Change INCLUDE_END to INCLUDE_FEI_END to fix WIND00044162.
01f,10may06,rcs  Fixed CQ:WIND00044162
01e,23feb04,jln  add SMEND support
01d,12jun02,kab  SPR 74987: cplusplus protection
01c,03may01,mot  Add FEI driver support
01b,16aug00,dmw  Wind River coding standards review.
01a,13mar00,dmw  Written (from version 01b of mcpn765/configNet.h).
*/
 
#ifndef INCnetConfigh
#define INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif

#include "vxWorks.h"
#include "end.h"

/* include fei driver */
#ifdef INCLUDE_FEI_END
#define END_LOAD_FUNC   sysFei82557EndLoad
#define END_BUFF_LOAN  TRUE 
#define END_LOAD_STRING  ""             /* created in sysEnd.c */

IMPORT END_OBJ* END_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_FEI_END */

#ifdef INCLUDE_MCG_END
#define END_LOAD_FUNC	sysEndLoad
#define END_BUFF_LOAN  TRUE 
#define END_LOAD_STRING  ""             /* created in sysEnd.c */

IMPORT END_OBJ* END_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_MCG_END */

#ifdef INCLUDE_SMEND
#   define SMEND_LOAD_STRING    ""
#   define SMEND_LOAD_FUNC      sysSmEndLoad

IMPORT END_OBJ* SMEND_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_SMEND */

END_TBL_ENTRY endDevTbl [] =
    {
#ifdef INCLUDE_FEI_END
        { 0, END_LOAD_FUNC, END_LOAD_STRING, END_BUFF_LOAN, NULL, FALSE},
#endif
#ifdef INCLUDE_SECONDARY_ENET
        { 1, END_LOAD_FUNC, END_LOAD_STRING, END_BUFF_LOAN, NULL, FALSE},
#endif /* INCLUDE_SECONDARY_ENET */

#ifdef INCLUDE_SMEND
        { 0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif
        { 0, END_TBL_END, NULL, 0, NULL, FALSE},
    };

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS 
#   define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#ifdef __cplusplus
    }
#endif

#endif /* INCnetConfigh */
