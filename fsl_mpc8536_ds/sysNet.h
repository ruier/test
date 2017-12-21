/* sysNet.h - system-dependent Network Header File */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,20oct08,b_m  created.
*/

#ifndef __INCsysNeth
#define __INCsysNeth

#ifdef __cplusplus
extern "C" {
#endif

#include <vxWorks.h>
#include "config.h"

extern const char *sysNetDevName[MAX_MAC_DEVS];

int    sysMacIndex2Dev  (int);
int    sysMacIndex2Unit (int);
STATUS sysMacOffsetGet  (char *, int, char **, int *);
STATUS sysNetMacAddrGet (char *, int, UINT8 *, int);
STATUS sysNetMacAddrSet (char *, int, UINT8 *, int);
STATUS sysNetMacNVRamAddrGet (char *, int, UINT8 *, int);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsysNeth */
