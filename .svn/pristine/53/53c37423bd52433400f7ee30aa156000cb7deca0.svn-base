/* configNet.h - network configuration header */

/*
 * Copyright (c) 1999,2002-2003,2006,2008 Wind River Systems, Inc. 
 *                                                            
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.             
 */
	 
/*
modification history
--------------------
01h,12aug08,x_f  Switch to VxBus END and PCI drivers
01g,21feb06,dtr  SPR #117719
01f,27jan06,dtr  Tidyup - remove fei.
01e,20aug03,dtr  Adding in TSEC config.
01d,31jul03,dtr  Adding fei secondary enet.
01c,07jul03,mil  Added FCC END.
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,10oct99,mtl  written from yk 750 by teamF1
*/

#if (defined(INCLUDE_END) && defined(INCLUDE_NETWORK))

#ifndef __INCnetConfigh
#define __INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <end.h>

/* defines */

#ifdef INCLUDE_SMEND
#   define SMEND_LOAD_STRING    ""
#   define SMEND_LOAD_FUNC      sysSmEndLoad
IMPORT END_OBJ* SMEND_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_SMEND */

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] = {
#ifdef INCLUDE_SMEND
    { 0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif /* INCLUDE_SMEND */
    { 0, END_TBL_END, NULL, 0, NULL}     /* must be last */
};

#endif /* __INCnetConfigh */

#ifdef __cplusplus
    }
#endif

#endif /* (defined (INCLUDE_END) && defined (INCLUDE_NETWORK)) */

