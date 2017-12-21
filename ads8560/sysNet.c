/* sysNet.c - system-dependent Network Library */

/* Copyright 1984-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,01aug07,wap	 written
*/

/*
DESCRIPTION

This module provides BSP functionality to support the
bootrom 'M' command to modify MAC addresses of on-board
network interfaces.

MAC adddress routines provided by the BSP in this file are:
    sysNetMacNVRamAddrGet()
    sysNetMacAddrGet()
    sysNetMacAddrSet()

This board provides storage in flash for the MAC addresses
of the motfcc and mottsec interfaces.  This library also
implements a RAM buffer to represent the contents of the
flash.  The RAM buffer contains eight entries, which is
more than currently needed by this board, but can be
considered as room for expansion in future boards using
a derivative of this BSP.  This RAM buffer is contained
in the array glbEnetAddr[][].
*/

#ifdef ETHERNET_MAC_HANDLER

#include "vxWorks.h"
#include "config.h"
#include "sysNet.h"

/* locals */

const char *sysNetDevName[MAX_MAC_DEVS] = {"mottsec", "motfcc" };
/* globals */

IMPORT int dynamicMacAddrGen
    (
    UINT8 * ifName,		/* interface name */
    int ifUnit,			/* interface unit */
    UINT8 * ifMacAddr,		/* address buffer */
    int ifMacAddrLen		/* length of buffer */
    );

/* locals */

/*
 * This version of the BSP includes support for storing MAC addresses
 * for all 4 ethernet ports in flash. This allows the addresses to be
 * modified from the bootrom using the 'M' command. However, older
 * versions of the BSP lacked this support, relying instead on addresses
 * that were hardcoded into the BSP source. A problem can arise if a
 * board with an old bootrom using hardcoded MAC addresses is used to
 * load a new VxWorks image with flash-based MAC address support: the
 * image will be unable to find valid addresses in the flash, which
 * will force a fallback to the hardcoded addresses in this table.
 * The problem is, the older hardcoded addresses were arbitrarily
 * chosen and did not use the Freescale OUI, which is technically not
 * valid. We want to use fallback addresses based on the Freescale OUI
 * here, but if we do that, the VxWorks image's MAC addresses won't
 * match those in the bootrom. This can lead to networking problems,
 * because other hosts and/or switches on the local network may have
 * learned the MAC address used by the bootrom, and not respond
 * immediately to ethernet frames the VxWorks image, which will have
 * a different address in them.
 *
 * To avoid this problem, we keep an alternate table of fallback MAC
 * addresses that matches those from the previous BSP, which can be
 * used to maintain backward compatibility.
 */

#ifdef INCLUDE_MAC_COMPAT
LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1] = {
    { 0x00, 0xa0, 0x1e, 0x11, 0x11, 0x00, 0 }, /* mottsec0 */
    { 0x00, 0xa0, 0x1e, 0x11, 0x11, 0x01, 0 }, /* mottsec1 */
    { 0x08, 0x00, 0x3e, 0x33, 0x02, 0x01, 0 }, /* motfcc0 */
    { 0x08, 0x00, 0x3e, 0x33, 0x02, 0x02, 0 }, /* motfcc1 */
};
#else /* INCLUDE_MAC_COMPAT */
LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1] = {
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_0, CUST_ENET4, CUST_ENET5, 0 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_1, CUST_ENET4, CUST_ENET5, 0 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_2, CUST_ENET4, CUST_ENET5, 0 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_3, CUST_ENET4, CUST_ENET5, 0 },
};
#endif /* INCLUDE_MAC_COMPAT */

LOCAL UINT8 sysInvalidAddr[2][MAC_ADRS_LEN] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

/***********************************************************
*
* sysMacOffsetGet - Calculate table offset
*
*  This routine calculates which table entry corresponds to
*  the specified interface.
*
*  Two values are calculated and returned in the parameters
*  pointed to by ppEnet and pOffset.
*
* RETURNS: ERROR if the interface is not known; OK otherwise
*/

STATUS sysMacOffsetGet
    (
    char *	ifName,		/* interface name */
    int		ifUnit,		/* interface unit */
    char **	ppEnet,		/* pointer to glbEnetAddr[][] entry */
    int *	pOffset		/* offset in NVRAM */
    )
    {
    int index;

    for (index = 0; index < MAX_MAC_DEVS; index++)
        {
        /* search through device names */
        if (strcmp (ifName, sysNetDevName[index]) == 0)
            break;
        }

    if (index == MAX_MAC_DEVS)
        return (ERROR);

    *ppEnet  = (char*)glbEnetAddr[(index * 2) + ifUnit];
    *pOffset = ((index * 2) + ifUnit) * MAC_ADRS_LEN;

    return(OK);
    }

/***********************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  Non Volatile RAM, and store it in the ifMacAddr
*  buffer provided by the caller.
*
*  It is not required for the BSP to provide NVRAM to store
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
    char * pEnet;
    UINT8 nvAddr[MAC_ADRS_LEN + 1];

    if (ifMacAddrLen > MAC_ADRS_LEN)
        return (ERROR);

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

#if (NV_RAM_SIZE != NONE)
    /* get MAC address from NvRAM. */
    sysNvRamGet ((char *)nvAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
    if (nvAddr[0] != WR_ENET0 ||
        nvAddr[1] != WR_ENET1 ||
        nvAddr[2] != WR_ENET2)
        {
        printf("%s%d: warning: using hard-coded "
            "address %02x:%02x:%02x:%02x:%02x:%02x\n",
            ifName, ifUnit, pEnet[0], pEnet[1], pEnet[2],
            pEnet[3], pEnet[4], pEnet[5]);
        }
    else
        memcpy (pEnet, nvAddr, MAC_ADRS_LEN);
#endif /* (NV_RAM_SIZE != NONE) */

    if (memcmp (pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0)
        return(ERROR);
    if (memcmp (pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0)
        return(ERROR);

    /* mac address in memory only */
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
     * RAM/NVRAM instead.
     */
    return(ERROR);
    }

/***********************************************************
*
* sysNetMacAddrSet - Save interface MAC address
*
*  This routine saves the MAC address specified in
*  ifMacAddr to the appropriate location in NVRam (if
*  possible) and update the specified interface to use
*  the specified MAC address.
*
*  If the network interface MAC address cannot be set,
*  this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
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
    char * pEnet;
    UINT8 swap[MAC_ADRS_LEN];

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

    /*
     * Some versions of bootConfig are broken and needlessly
     * swap the MAC address, thinking it needs to be adjusted
     * depending on the host byte order (it doesn't). Unfortunately,
     * it forgets to swap it back. We try and detect this and
     * unswap it if necessary.
     */

    if (ifMacAddr[0] != WR_ENET0 ||
        ifMacAddr[1] != WR_ENET1 ||
        ifMacAddr[2] != WR_ENET2)
        {
        swap[0] = ifMacAddr[0];
        swap[1] = ifMacAddr[1];
        swap[2] = ifMacAddr[2];
        swap[3] = ifMacAddr[3];
        swap[4] = ifMacAddr[4];
        swap[5] = ifMacAddr[5];
        ifMacAddr[0] = swap[5];
        ifMacAddr[1] = swap[4];
        ifMacAddr[2] = swap[3];
        ifMacAddr[3] = swap[2];
        ifMacAddr[4] = swap[1];
        ifMacAddr[5] = swap[0];
        }

#if (NV_RAM_SIZE != NONE)
    /* check MAC address in NvRAM. */
    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
    if (0 == memcmp (ifMacAddr, pEnet, ifMacAddrLen))
	{ 
	/* same address so don't erase and rewrite flash */
	printf("Address unchanged\n");
	return (OK);
	}

    sysNvRamSet ((char *)ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
#endif /* (NV_RAM_SIZE != NONE) */

    /* mac address in memory only */
    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

/***********************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a device string index e.g. motfcc.
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
    return (0);
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
    return (index);
    }


#ifdef ENET_MAC_DEBUG

/***********************************************************
*
* sysNetMacAddrClear - clear MAC address in FLASH/NVRAM
*
*  This routine clears the storage locations in NVRAM
*  reserved for the MAC address of the specified interface.
*
*  This is useful for debugging the 'M' command.  To use
*  this, boot vxWorks and run sysNetMacAddrClear() from
*  the shell.  After running this command, you can test
*  the bootrom 'M' command under conditions similar to a
*  new, unprogrammed board.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*/

int sysNetMacAddrClear
    (
    char *	ifName,
    int		ifUnit
    )
    {
    UINT8 	ifMacAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    int   offset;
    char *pEnet;

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

    sysNvRamSet (ifMacAddr, 6, NV_MAC_ADRS_OFFSET+offset);
    memcpy (ifMacAddr, pEnet, 6);
    return(0);
    }
#endif /* ENET_MAC_DEBUG */

#endif /* ETHERNET_MAC_HANDLER */
