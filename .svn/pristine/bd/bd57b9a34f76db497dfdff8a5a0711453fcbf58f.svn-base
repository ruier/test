/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2007, 2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,20may13,wyt  Added support for P1023RDB
01f,07jun12,mpc  Fixed potential memory corruption caused by sysNvRamGet().
                 (WIND00101057)
01e,12oct10,y_y  add p1021mds qeFcc support.
01d,12aug08,b_m  switch to use new vxbus etsec and ucc driver.
01c,03jul07,b_m  reverse motetsec mac address (workaround for WIND00097836).
01b,28may07,b_m  add MDS8568 qeFcc support.
01a,10may07,b_m  derived from version 01g of cds8548/sysNet.c.
*/

/*
DESCRIPTION

This module provides BSP functionality to support the bootrom 'M' command to
modify MAC addresses of on-board network interfaces.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"

/* defines */

#define MAC_OFFSET_MOT_ETSEC     2
#define MAC_OFFSET_QE_FCC        5

/* globals */

const char *sysNetDevName[MAX_MAC_DEVS] = {"dtsec", "motetsec", "qefcc"};

/* locals */

LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN] = {
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_0 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_1 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_2 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_3 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_4 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_5 },
    { FSL_ENET0, FSL_ENET1, FSL_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_6 },
};

LOCAL UINT8 sysInvalidAddr[2][MAC_ADRS_LEN] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};


/*******************************************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a device string index.
*
* RETURNS: index access device name in sysNetDevName
*
* ERRNO: N/A
*/

int sysMacIndex2Dev
    (
    int index
    )
    {
    if (index <= 1)
        return 0;
    else if (index <= 4 && index > 1)
        return 1;
    else
        return 2;
    }

/*******************************************************************************
*
* sysMacIndex2Unit - convert index range to unit number
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a unit number.
*
* RETURNS: unit number of indexed device
*
* ERRNO: N/A
*/

int sysMacIndex2Unit
    (
    int index
    )
    {
    if (index <= 1)
        return (index);
    else if (index <= 4 && index > 1)
        return (index - MAC_OFFSET_MOT_ETSEC);
    else
        return (index - MAC_OFFSET_MOT_ETSEC - MAC_OFFSET_QE_FCC);
    }

/*******************************************************************************
*
* sysMacOffsetGet - Calculate table offset
*
* This routine calculates which table entry corresponds to the specified
* interface. Two values are calculated and returned in the parameters pointed
* to by ppEnet and pOffset.
*
* RETURNS: ERROR if the interface is not known; OK otherwise
*
* ERRNO: N/A
*/

STATUS sysMacOffsetGet
    (
    char *  ifName,     /* interface name */
    int     ifUnit,     /* interface unit */
    char ** ppEnet,     /* pointer to glbEnetAddr[][] entry */
    int *   pOffset     /* offset in NVRAM */
    )
    {

    if (ifUnit > 3)
        return ERROR;

    if (strcmp(ifName, "dtsec") == 0)
        {
        *pOffset = ifUnit * (MAC_ADRS_LEN + 2);
        *ppEnet  = (char *)glbEnetAddr[ifUnit];
        }
    else if ((strcmp(ifName, "mottsec") == 0) || (strcmp(ifName, "motetsec") == 0))
        {
        *pOffset = (ifUnit + MAC_OFFSET_MOT_ETSEC) * (MAC_ADRS_LEN + 2);
        *ppEnet  = (char*)glbEnetAddr[ifUnit + MAC_OFFSET_MOT_ETSEC];
        }
    else if (strcmp(ifName, "qefcc") == 0)
        {
        *pOffset = (ifUnit + MAC_OFFSET_MOT_ETSEC + MAC_OFFSET_QE_FCC) * (MAC_ADRS_LEN + 2);
        *ppEnet  = (char*)glbEnetAddr[ifUnit + MAC_OFFSET_MOT_ETSEC + MAC_OFFSET_QE_FCC];
        }
    else
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* sysNetMacAddrGet - Get interface MAC address
*
* This routine gets the current MAC address from the network interface, and
* stores it in the ifMacAddr buffer provided by the caller.
*
* If the network interface cannot be queried about the MAC address,
* this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    /*
     * None of our interfaces can be queried directly.
     * Return ERROR to indicate that we need to use RAM/NVRAM instead.
     */

    return ERROR;
    }

/*******************************************************************************
*
* sysNetMacAddrSet - Save interface MAC address
*
* This routine saves the MAC address specified in ifMacAddr to the appropriate
* location in NVRam (if possible) and update the specified interface to use
* the specified MAC address.
*
* If the network interface MAC address cannot be set,
* this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacAddrSet
    (
    char *  ifName,
    int     ifUnit,
    char * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return ERROR;

#if (NV_RAM_SIZE != NONE)
    (void) sysNvRamGet(pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);

    if (memcmp(ifMacAddr, pEnet, ifMacAddrLen) == 0)
        {
        printf("Address unchanged\n");
        return OK;
        }

    sysNvRamSet(ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
#endif

    /* mac address in memory only */

    memcpy(ifMacAddr, pEnet, ifMacAddrLen);

    return OK;
    }

/*******************************************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
* This routine gets the current MAC address from the Non Volatile RAM, and store
* it in the ifMacAddr buffer provided by the caller.
*
* It is not required for the BSP to provide NVRAM to store the MAC address.
* Also, some interfaces do not allow the MAC address to be set by software.
* In either of these cases, this routine simply returns ERROR.
*
* Given a MAC address m0:m1:m2:m3:m4:m5, the byte order of ifMacAddr is:
*       m0 @ ifMacAddr
*       m1 @ ifMacAddr + 1
*       m2 @ ifMacAddr + 2
*       m3 @ ifMacAddr + 3
*       m4 @ ifMacAddr + 4
*       m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacNVRamAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return ERROR;

#if (NV_RAM_SIZE != NONE)
    (void) sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
#endif

    if (memcmp(pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0)
        return ERROR;
    if (memcmp(pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0)
        return ERROR;

    /* mac address in memory only */

    memcpy(ifMacAddr, pEnet, ifMacAddrLen);

    return OK;
    }
