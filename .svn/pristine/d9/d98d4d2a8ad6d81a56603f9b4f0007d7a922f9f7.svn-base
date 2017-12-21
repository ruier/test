/* usrWdbBanner.c - print banner to the console */

/*
 * Copyright (c) 1984-2011 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01s,25mar11,rlp  Added SSL support.
01r,03jan11,rlp  Updated WDB banner copyright date (CQ:WIND00248954).
01q,22oct10,rlp  Modified WDB banner message (WIND00227819).
01p,05jan10,rlp  Updated WDB banner copyright date
01o,08sep09,rlp  Updated WDB banner copyright date.
01p,18aug09,rlp  Updated WDB banner copyright date (WIND00177197).
01o,24feb09,rlp  Removed unsupported INCLUDE_WDB_COMM_NETROM components.
01n,28jul08,dbt  Added support for WDB MIPC.
01m,29apr08,kkz  Updating Copyright to 2008
01l,09jan07,kk   update Copyright to 2007
01k,01mar06,kk   update copyright to 2006
01j,31may05,dbt  Added support for TIPC connection.
01i,17feb05,tam  Updated Copyright to 2005
01h,29jul04,md   Added ED&R info; SPR 78837
01g,15dec03,dat  Copyright 2004
01f,06may02,sbs  updating copyright
01e,29jan02,g_h  Added WDB_COMM_VTMD as a possible WDB_COMM_TYPE_STR
01d,12sep01,jhw  Added WDB_PIPE as a possible WDB_COMM_TYPE_STR
01c,21aug01,jhw  Added WDB_COMM_TYPE to banner (SPR 9559)
01b,01mar01,zl   updated copyright year.
01b,17jan01,sn   simplify job of host val  
01a,08oct97,ms   taken from usrWdb.c
*/

/*
DESCRIPTION
Print a short banner, like the bootPrintLogo banner
*/

/* Defines */

#ifdef INCLUDE_WDB_COMM_NETWORK
#ifdef INCLUDE_WDB_SSL
#define WDB_COMM_TYPE_STR "WDB_COMM_NETWORK over SSL"
#else  /* INCLUDE_WDB_SSL */
#define WDB_COMM_TYPE_STR "WDB_COMM_NETWORK"
#endif  /* INCLUDE_WDB_SSL */
#endif /* INCLUDE_WDB_COMM_NETWORK */

#ifdef INCLUDE_WDB_COMM_SERIAL
#define WDB_COMM_TYPE_STR "WDB_COMM_SERIAL"
#endif /* INCLUDE_WDB_COMM_SERIAL */

#ifdef INCLUDE_WDB_COMM_TYCODRV_5_2
#define WDB_COMM_TYPE_STR "WDB_COMM_TYCODRV_5_2"
#endif /* INCLUDE_WDB_COMM_TYCODRV_5_2 */

#ifdef INCLUDE_WDB_COMM_VTMD
#define WDB_COMM_TYPE_STR "WDB_COMM_VTMD"
#endif /* INCLUDE_WDB_COMM_VTMD */

#ifdef INCLUDE_WDB_COMM_END
#define WDB_COMM_TYPE_STR "WDB_COMM_END"
#endif /* INCLUDE_WDB_COMM_END */

#ifdef INCLUDE_WDB_COMM_CUSTOM
#define WDB_COMM_TYPE_STR "WDB_COMM_CUSTOM"
#endif /* INCLUDE_WDB_COMM_CUSTOM */

#ifdef INCLUDE_WDB_COMM_PIPE
#define WDB_COMM_TYPE_STR "WDB_COMM_PIPE"
#endif /* INCLUDE_WDB_COMM_PIPE */

#ifdef INCLUDE_WDB_COMM_TIPC
#define WDB_COMM_TYPE_STR "WDB_COMM_TIPC"
#endif /* INCLUDE_WDB_COMM_TIPC */

#ifdef INCLUDE_WDB_COMM_MIPC
#define WDB_COMM_TYPE_STR "WDB_COMM_MIPC"
#endif /* INCLUDE_WDB_COMM_MIPC */

#ifndef WDB_COMM_TYPE_STR
#define WDB_COMM_TYPE_STR "Unknown"
#endif /* WDB_COMM_TYPE_STR */

/******************************************************************************
*
* usrWdbBanner - print the banner information to the Console 
*
* This routine prints out the banner information to the Console. 
*
* RETURNS :
* None 
*
* \NOMANUAL
*/
 
void usrWdbBanner (void)
    {
#ifdef	INCLUDE_SHELL_BANNER
    return;
#else
    printf ("\n\n");
    printf ("%23s\n\n",runtimeName);
    printf ("Copyright 1984-%s  Wind River Systems, Inc.\n\n", (__DATE__ + 7));
    printf ("             CPU: %s\n", sysModel ());
    printf ("    Runtime Name: %s\n", runtimeName);
    printf (" Runtime Version: %s\n", runtimeVersion);
    printf ("     BSP version: " BSP_VERSION BSP_REV "\n");
    printf ("         Created: %s\n", creationDate);
    printf ("ED&R Policy Mode: %s\n",
#ifdef INCLUDE_EDR_SYSDBG_FLAG
	    edrIsDebugMode() ? "Lab" : "Deployed");
#else
	    "Permanently Deployed");
#endif
    printf ("   WDB Comm Type: %s\n", WDB_COMM_TYPE_STR );
    printf ("             WDB: %s.\n\n",
            ((wdbRunsExternal () || wdbRunsTasking ()) ?
	     (IS_WDB_ENABLED() ? "Ready" : "Agent Disabled")
	     	: "Agent configuration failed"));
#endif
    }
