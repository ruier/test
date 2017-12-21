/* usrAimMmu.c - AIM MMU configuration routine */

/* Copyright 1984-2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01a,30jun04,dtr  Created
*/

/* DESCRIPTION :
 * Sets up some initialisation parameters for the PTE memory pool and
 * sets PTE write protection if included. It is call from aimMmuLibInit.
 */

IMPORT UINT initNumPageTables;
IMPORT UINT incrNumPageTables;
IMPORT UINT initNumRegionTables;
IMPORT UINT incrNumRegionTables;
IMPORT BOOL enablePteProtection;

#ifndef AIM_MMU_INIT_PT_NUM
#define AIM_MMU_INIT_PT_NUM 0x40
#endif
#ifndef AIM_MMU_INIT_PT_INCR
#define AIM_MMU_INIT_PT_INCR 0x20
#endif
#ifndef AIM_MMU_INIT_RT_NUM
#define AIM_MMU_INIT_RT_NUM 0x10
#endif
#ifndef AIM_MMU_INIT_RT_INCR
#define AIM_MMU_INIT_RT_INCR 0x10
#endif

void usrAimMmuConfig()
    {
    /* These are functional defaults but require configlette for user */
#ifdef INCLUDE_AIM_MMU_MEM_POOL_CONFIG
    initNumPageTables = AIM_MMU_INIT_PT_NUM;
    incrNumPageTables = AIM_MMU_INIT_PT_INCR;
    initNumRegionTables = AIM_MMU_INIT_RT_NUM;
    incrNumRegionTables = AIM_MMU_INIT_RT_INCR;
#endif /* INCLUDE_AIM_MMU_MEM_POOL_CONFIG */
#ifdef INCLUDE_AIM_MMU_PT_PROTECTION
    enablePteProtection = TRUE;
#endif /* INCLUDE_AIM_MMU_PT_PROTECTION */

    }


