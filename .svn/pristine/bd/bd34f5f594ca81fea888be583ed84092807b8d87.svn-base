/* usrBanner.c - print the shell banner */

/*
 * Copyright (c) 1998, 2001-2002, 2004-2005, 2007-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01o,25mar11,rlp  Added SSL support.
01n,16nov10,rlp  Fixed LP64 build warning.
01m,22oct10,rlp  Modified WDB banner message (WIND00227819).
01l,13sep10,v_r  Fixed build warning.
01k,28may10,jpb  Added more support for LP64 adaptation.
01k,20may10,rlp  Removed warnings.
01j,23sep09,pad  Changed memory size computation for VxWorks 64-bit.
01i,07may09,jmp  Adapted for 64-bit support.
		 Modified banner to also display memory size in MBytes.
01i,24feb09,rlp  Removed unsupported INCLUDE_WDB_COMM_NETROM components.
01h,28jul08,dbt  Added support for WDB MIPC.
01g,16mar07,v_r  Account for HITACHI_SH_KERNEL_ON_SDRAM in usrBanner().
01f,04jun05,dbt  Added support for WDB TIPC.
01e,29jul04,md   Added ED&R info; SPR 78837
01d,29jan02,g_h  Added WDB_COMM_VTMD as a possible WDB_COMM_TYPE_STR
01c,12sep01,jhw  Added WDB_PIPE as a possible WDB_COMM_TYPE_STR
		 Fixed display alignment of WDB Comm Type.
01b,21aug01,jhw  Added WDB_COMM_TYPE to banner (SPR 9559)
01a,02jun98,ms   written
*/

/*
DESCRIPTION

*/

#ifdef _WRS_CONFIG_LP64
#include <adrSpaceLib.h>
#endif

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
* usrBanner - print the banner information to the Target Shell
*
* This routine prints out the banner information to the Target Shell.
*
* RETURNS :
* None
*
* NOMANUAL
*/
 
void usrBanner (void)
    {
    size_t memSize;	/* memory size in bytes */
    size_t nMBytes;	/* memory size in megabytes */
#ifdef _WRS_CONFIG_LP64
    ADR_SPACE_INFO adrsSpaceInfo;
#endif

#ifndef _WRS_CONFIG_LP64
#ifdef  HITACHI_SH_KERNEL_ON_SDRAM
    memSize = sysMemTop () - (char *)FREE_RAM_ADRS;
#else   /* HITACHI_SH_KERNEL_ON_SDRAM */
    memSize = sysMemTop () - (char *)LOCAL_MEM_LOCAL_ADRS;
#endif  /* HITACHI_SH_KERNEL_ON_SDRAM */

#else	/* _WRS_CONFIG_LP64 */
    /*
     * For VxWorks 64-bit the total size of physical memory available to
     * VxWorks is displayed in the banner.
     */

    if (adrSpaceInfoGet (&adrsSpaceInfo) == ERROR)
	memSize = 0;
    else
	memSize = adrsSpaceInfo.physTotalPages * adrsSpaceInfo.physAllocUnit;
#endif	/* _WRS_CONFIG_LP64 */

    printLogo (); /* print out the VxWorks logo */

    printf (" CPU: %s.  Processor #%d.\n", sysModel (), sysProcNumGet ());

    nMBytes = memSize / (1024 * 1024);

    printf (" Memory Size: %#lx", (ULONG) memSize);

    if (nMBytes > 0)
	{
	if ((memSize % (1024 * 1024)) == 0)
	    printf (" (%dMb)", (UINT32) nMBytes);
	else
	    printf (" (~%dMb)", (UINT32) nMBytes);
	}
    printf (".");
    
    printf ("  BSP version " BSP_VERSION BSP_REV ".\n");
    printf (" Created: %s\n", creationDate);
    printf (" ED&R Policy Mode: %s\n",
#ifdef INCLUDE_EDR_SYSDBG_FLAG
	    edrIsDebugMode() ? "Lab" : "Deployed");
#else
	    "Permanently Deployed");
#endif
#if defined (INCLUDE_WDB) && defined (INCLUDE_WDB_BANNER)
    printf (" WDB Comm Type: %s\n", WDB_COMM_TYPE_STR );
    printf (" WDB: %s.\n", ((wdbRunsExternal () || wdbRunsTasking ()) ?
    			    (IS_WDB_ENABLED() ?  "Ready" : "Agent Disabled")
			    	: "Agent configuration failed") );
#endif
    }
