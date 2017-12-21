/* usrWdbBp.c - configuration file for WDB agent breakpoints */

/*
 * Copyright (c) 1997,2006,2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,19mar10,rlp  Removed system mode support for LP64 adaptation.
01c,10dec09,rlp  Adapted to LP64 model.
01b,18oct06,rlp  Modified breakpoint structure (REQ00007655).
01a,21jul97,ms   taken from 01y of usrWdb.c
*/

/*
DESCRIPTION

This library configures and initializes the WDB agents system
breakpoint library.
*/

#include <wdb/wdbBpLib.h>

/* locals */

#ifndef	_WRS_ARCH_NO_SYSTEM_MODE_AGENT
LOCAL struct wdb_brkpt	wdbBreakPoints [WDB_BP_MAX];
#endif	/* _WRS_ARCH_NO_SYSTEM_MODE_AGENT */

/******************************************************************************
*
* usrWdbBp - configure and initialize the WDB agents breakpoint library
*
*/

void usrWdbBp (void)
    {
#ifndef	_WRS_ARCH_NO_SYSTEM_MODE_AGENT
    wdbSysBpLibInit (wdbBreakPoints, WDB_BP_MAX);
    wdbBpSysEnterHook = wdbSysBpDisableAll;
    wdbBpSysExitHook = wdbSysBpEnableAll;
#else	/* _WRS_ARCH_NO_SYSTEM_MODE_AGENT */
    wdbSysBpLibInit (NULL, 0);
#endif	/* _WRS_ARCH_NO_SYSTEM_MODE_AGENT */
    }

