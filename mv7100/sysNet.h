/* sysNet.h - system-dependent Network Header File */

/*
 * Copyright (c) 2005, 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,17dec09,e_d  remove M command support
01c,12jan09,e_d  add M command support
01b,21aug08,d_l  remove ENET_DEFAULT.(WIND00121398)
01a,14oct05,dtr  created from ads834x/01c
*/


#ifndef  __INCsysNeth
#define  __INCsysNeth

#ifdef __cplusplus
    extern "C" {
#endif

#include <vxWorks.h>
#include <config.h>

IMPORT VPD_PACKET * sysVpdPkts [];    /* board vpd packets */

STATUS sysNetMacNVRamAddrGet (char *ifName, int ifUnit, UINT8 *ifMacAddr,
                              int ifMacAddrLen);

#ifdef __cplusplus
    }
#endif

#endif   /* __INCsysNeth  */

