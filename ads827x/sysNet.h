/* sysNet.h - system-dependent Network Header File */

/* Copyright 2001,2002,2004,2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,16feb07,rec  vxWorks 6.5 update
01c,19jul04,rec  unit test changes
                 add sysMacIndex2Dev sysMacIndex2Unit needed by bootConfig.c
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,13jun01,g_h	 adopted from T2 wrSbc8260/sysNet.h
*/


#ifndef  __INCsysNeth
#define  __INCsysNeth 

#ifdef __cplusplus
    extern "C" {
#endif

#include "vxWorks.h"
#include "config.h"

extern const char *sysNetDevName[MAX_MAC_DEVS];

/* Prototypes */

int sysMacIndex2Dev (int index);

int sysMacIndex2Unit (int index);

STATUS sysMacOffsetGet (char *ifName, int ifUnit, char **ppEnet, 
                              int *	pOffset);
			      
STATUS sysNetMacNVRamAddrGet (char *ifName, int ifUnit, UINT8 *ifMacAddr, 
                              int ifMacAddrLen);
STATUS sysNetMacAddrGet (char *ifName, int ifUnit, UINT8 *ifMacAddr, 
                              int ifMacAddrLen);
STATUS sysNetMacAddrSet (char *ifName, int ifUnit, UINT8 *ifMacAddr, 
                              int ifMacAddrLen);

#ifdef __cplusplus
    }
#endif

#endif   /* __INCsysNeth  */

