/* sysNet.h - system-dependent Network Header File */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,22nov06,d_c  Move MAX_MAC_DEVS and ENET_DEFAULT from sysNet.h to config.h
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

