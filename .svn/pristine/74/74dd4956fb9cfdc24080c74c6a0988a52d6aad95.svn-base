/* sysNet.h - system-dependent Network Header File */

/*
 * Copyright (c) 2001-2002, 2004-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,07aug07,agf  move MAX_MAC_DEVS from sysNet.h to config.h
01d,10jan05,k_p  copied from the BSP wrSbcPowerQuiccII and modified the macro
                 MAX_MAC_DEVS
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

