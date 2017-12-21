/* sysNet.h - system-dependent Network Header File */

/* Copyright 1984-2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,23mar07,pmr  move ENET_DEFAULT definition to config.h
01a,18jul06,wap  Fix support for little-endian mode
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

