/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
01b,22oct07,y_w  fixed gnu compiler warnings and some comments.
01a,24jul07,y_w  derived from version 01c of wrSbc834x/sysNet.c.
*/

/*
DESCRIPTION

This module provides BSP functionality to support the
bootrom 'M' command to modify MAC addresses of on-board
network interfaces.

MAC address routines provided by the BSP in this file are:
   sysMacIndex2Dev()
   sysMacIndex2Unit()
   sysMacOffsetGet()
   sysNetMacNVRamAddrGet()
   sysNetMacAddrGet()
   sysNetMacAddrSet()

This board provides storage in EEPROM for the MAC addresses
of the tsiG interfaces.  This library also implements
a RAM buffer to represent the contents of the Flash.
The RAM buffer contains two entries, contained in the array
glbEnetAddr[][].
*/

#ifdef ETHERNET_MAC_HANDLER

#include <vxWorks.h>
#include "config.h"
#include <ctype.h>
#include <fioLib.h>
#include "sysNet.h"

/* locals */

/* defines */
#define NV_MAC_ADRS_OFFSET  0x1000		/* 0xe0005000 */

/* globals */

/* locals */

/*
 * glbEnetAddr assignment order:
 *   tsiG0
 *   tsiG1
 */

/* terminating zero in glbEnetAddr entry required for call to sysNvRamGet */

LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1] = {
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_0, 0 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_1, 0 },
};

const char *sysNetDevName[] = {"tsiG"};

LOCAL UINT8 sysInvalidAddr[3][MAC_ADRS_LEN] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
	{ 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5 }
};

/***********************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a device string index e.g. tsiG.
*
* RETURNS: index access device name in sysNetDevName
*
* ERRNO
*/

int sysMacIndex2Dev
    (
    int index
    )
    {
    return (0);      /* always tsiG */
    }

/***********************************************************
*
* sysMacIndex2Unit - convert index range to unit number
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a unit number.
*
* RETURNS: unit number of indexed device
*
* ERRNO
*/

int sysMacIndex2Unit
    (
    int index
    )
    {
    return (index);   /* index is unit number */
    }

/***********************************************************
*
* sysMacOffsetGet - Calculate table offset
*
* This routine calculates which table entry corresponds to
* the specified interface.
*
* Two values are calculated and returned in the parameters
* pointed to by ppEnet and pOffset.
*
* RETURNS: ERROR if the interface is not known; OK otherwise
*
* ERRNO
*/

STATUS sysMacOffsetGet
    (
    char *	ifName,		/* interface name */
    int		ifUnit,		/* interface unit */
    char **	ppEnet,		/* pointer to glbEnetAddr[][] entry */
    int *   pOffset     /* offset in NvRAM */
    )
    {

    int index = MAX_MAC_DEVS - 1;  /* index into device names: always 0 */

    /*
     * The order of addresses in NvRAM and glbEnetAddr[] are:
     *
     *   tsiG0 
     *   tsiG1
     *
     */

    /* validate device name */

    if ( strcmp(ifName, sysNetDevName[index]) == 0 )
        {
        /* validate unit number  */

        if ( (ifUnit < 0) || (ifUnit > 1) )
            return (ERROR);

        /* return MAC addr and NvRAM offset */
        *ppEnet  = (char*)glbEnetAddr[ifUnit];
        *pOffset = ifUnit * MAC_ADRS_LEN;

        return(OK);
        }

    /* device not found */
    return(ERROR);
    }

/***********************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  Non Volatile RAM, and stores it in the ifMacAddr
*  buffer provided by the caller.
*
*  It is not required for the BSP to provide NvRAM to store
*  the MAC address.  Also, some interfaces do not allow
*  the MAC address to be set by software.  In either of
*  these cases, this routine simply returns ERROR.
*
*  Given a MAC address m0:m1:m2:m3:m4:m5, the byte order
*  of ifMacAddr is:
*	m0 @ ifMacAddr
*	m1 @ ifMacAddr + 1
*	m2 @ ifMacAddr + 2
*	m3 @ ifMacAddr + 3
*	m4 @ ifMacAddr + 4
*	m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacNVRamAddrGet
    (
    char *	ifName,
    int		ifUnit,
    UINT8 *	ifMacAddr,
    int		ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

    /* get MAC address from NvRAM. */

#if (NV_RAM_SIZE != 0)
	sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
#endif /* (NV_RAM_SIZE != NONE) */


    /* check MAC address is valid */

    if ( memcmp(pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0 )
        return(ERROR);
    if ( memcmp(pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0 )
        return(ERROR);
    if ( memcmp(pEnet, sysInvalidAddr[2], MAC_ADRS_LEN) == 0 )
        return(ERROR);

    /* copy MAC address to pointer for return */

    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

/***********************************************************
*
* sysNetMacAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  network interface, and stores it in the ifMacAddr
*  buffer provided by the caller.
*
*  If the network interface cannot be queried about the
*  MAC address, this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacAddrGet
    (
    char *	ifName,
    int		ifUnit,
    UINT8 *	ifMacAddr,
    int		ifMacAddrLen
    )
    {
    /*
     * None of our interfaces can be queried directly.
     * Return ERROR to indicate that we need to use
     * RAM/NvRAM instead.
     */

    return(ERROR);
    }

/***********************************************************
*
* sysNetMacAddrSet - Save interface MAC address
*
*  This routine saves the MAC address specified in
*  ifMacAddr to the appropriate location in NvRAM (if
*  possible).
*
*  If the network interface MAC address cannot be set,
*  this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacAddrSet
    (
    char *	ifName,
    int		ifUnit,
    UINT8 *	ifMacAddr,
    int		ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        {
        return(ERROR);
        }

#if (NV_RAM_SIZE != 0)
    /* check MAC address in NvRAM */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
    if (memcmp (ifMacAddr, pEnet, ifMacAddrLen) == 0)
        {

        /* same address so don't erase and rewrite flash */

        printf("Address unchanged\n");
        return(OK);
        }

    sysNvRamSet ((char *)ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
#endif /* (NV_RAM_SIZE != NONE) */

    /* copy MAC address to RAM buffer */

    memcpy (pEnet, ifMacAddr, ifMacAddrLen);

    return (OK);
    }

#endif /* ETHERNET_MAC_HANDLER */

