/* sysNet.h - system-dependent Network Header File */

/*
 * Copyright (c) 2001, 2002, 2004, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,21aug08,d_l  remove ENET_DEFAULT.(WIND00121398)
01c,19jul04,rec  unit test changes
                 add sysMacIndex2Dev sysMacIndex2Unit needed by bootConfig.c
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,13jun01,g_h  adopted from T2 wrSbc8260/sysNet.h
*/


#ifndef  __INCsysNeth
#define  __INCsysNeth

#ifdef __cplusplus
    extern "C" {
#endif

#include <vxWorks.h>
#include "config.h"

#define MAX_MAC_DEVS 2 /* two network devices (fcc, scc) */

extern const char *sysNetDevName[MAX_MAC_DEVS];

int sysMacIndex2Dev (int index);

int sysMacIndex2Unit (int index);

STATUS sysMacOffsetGet (char *ifName, int ifUnit, char **ppEnet,
                              int * pOffset);

STATUS sysNetMacNVRamAddrGet (char *ifName, int ifUnit, UINT8 *ifMacAddr,
                                         int ifMacAddrLen);
STATUS sysNetMacAddrGet (char *ifName, int ifUnit, UINT8 *ifMacAddr,
                                 int ifMacAddrLen);
STATUS sysNetMacAddrSet (char *ifName, int ifUnit, UINT8 *ifMacAddr,
                                int ifMacAddrLen);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif   /* __INCsysNeth  */

