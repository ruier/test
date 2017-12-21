/* sysNet.h - system-dependent Network Header File */

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,21mar08,wap  Add VxBus support, switch to VxBus EMAC driver
01c,31oct06,rec  vxWorks 6.5 update
01b,17mar06,mmi	 moved MAX_MAC_ADRS to config.h
01a,16mar06,mmi	 Created based on ads860 version 01c
*/


#ifndef  __INCsysNeth
#define  __INCsysNeth 

#ifdef __cplusplus
    extern "C" {
#endif

#include "vxWorks.h"
#include "config.h"
#include "zmiiLib.h"

#ifdef INCLUDE_EMAC_VXB_END

#define MAC_OFFSET_EMAC0	0

#ifdef USE_BOTH_EMACS
#define MAC_OFFSET_EMAC1	1
#endif /* USE_BOTH_EMACS */

#endif /* INCLUDE_EMAC_VXB_END */

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

