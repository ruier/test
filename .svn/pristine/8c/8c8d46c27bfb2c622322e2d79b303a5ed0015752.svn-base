/* usrMacAddr.c - system-dependent Network Library */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,18jan13,x_s  Added NV_MAC_ADRS_OFFSET definition if BSP not defined.
01b,15jan13,wyt  handle situation of no NVRAM on board.
01a,08jan13,wyt  created.
*/

/*
DESCRIPTION

This module provides functionality to support the bootrom 'M' command to
modify MAC addresses of on-board network interfaces.

This file will replace sysNet.c in BSP direcory. BSPs need to provide several
macro defines and global variables.

#define MAX_MAC_DEVS  (Number of network device classes)

#define MAX_MAC_ADRS  (Number of MAC address)

#define MAC_ADRS_LEN  6

const char *sysNetDevName[MAX_MAC_DEVS] should be defined and initialized
as device name strings.

int sysNetDevNum[MAX_MAC_DEVS] is used to define the number of each
type of device. The order of the members MUST correspond to the order of
sysNetDevName[MAX_MAC_DEVS].

UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN] is needed to provide
default MAC addresses.

For example:

\cs
#ifdef INCLUDE_USR_MACADDR

#define MAX_MAC_DEVS 3
#define MAX_MAC_ADRS 5
#define MAC_ADRS_LEN 6

#define INTERFACE_1_NUM 2
#define INTERFACE_2_NUM 2
#define INTERFACE_3_NUM 1

const char *sysNetDevName[MAX_MAC_DEVS] = {
    "interface1",
    "interface2",
    "interface3"
};

int sysNetDevNum[MAX_MAC_DEVS] = {
    INTERFACE_1_NUM,
    INTERFACE_2_NUM,
    INTERFACE_3_NUM
};

UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN] = {
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_0 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_1 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_2 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_3 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3, CUST_ENET4, CUST_ENET5_4 }
};

#endif
\ce

*/

/* includes */

/* defines */

#define MAC_ADRS_PAD    2

#ifndef NV_MAC_ADRS_OFFSET
/* default offset is 0x200 */

#   define NV_MAC_ADRS_OFFSET 0x200
#endif /* ! NV_MAC_ADRS_OFFSET */

/* globals */

extern const char *sysNetDevName[MAX_MAC_DEVS];
extern int sysNetDevNum[MAX_MAC_DEVS];
extern UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN];

/* locals */

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
    int i;
    int num = 0;

    for(i = 0; i < MAX_MAC_DEVS; i++)
        {
        num += sysNetDevNum[i];
        if (num > index)
            return i;
        }

    return 0;
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
    int i, delta;
    int num = 0;

    for(i = 0; i < MAX_MAC_DEVS; i++)
        {
        delta = index - num;
        if (delta < sysNetDevNum[i])
            return delta;
        num += sysNetDevNum[i];
        }

    return 0;
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
    int i;
    int index = 0;

    for (i = 0; i < MAX_MAC_DEVS; i++)
        {
        if (strcmp(ifName, sysNetDevName[i]) == 0)
            {
            if (ifUnit >= sysNetDevNum[i])
                return ERROR;

            /* MAC address in NVRAM is aligned to 8 bytes boundary */

            *pOffset = (ifUnit + index) * (MAC_ADRS_LEN + MAC_ADRS_PAD);
            *ppEnet  = (char *)glbEnetAddr[ifUnit + index];
            return OK;
            }
       index += sysNetDevNum[i];
       }

    return ERROR;
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
* sysNetMacAddrCheck - MAC address invalid check
*
* This routine checks whether the MAC address is multicast address,
* or other invalid values.
*
* RETURNS: OK, if MAC address valid, ERROR otherwise
*
* ERRNO: N/A
*/

LOCAL STATUS sysNetMacAddrCheck
    (
    char * ifMacAddr
    )
    {
    if (memcmp(ifMacAddr, sysInvalidAddr[0], MAC_ADRS_LEN) == 0)
        return ERROR;
    if (memcmp(ifMacAddr, sysInvalidAddr[1], MAC_ADRS_LEN) == 0)
        return ERROR;
    if ((ifMacAddr[0] & 0x01) != 0)
        return ERROR;

    return OK;
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
    char *  ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;
    char macAddr[MAC_ADRS_LEN + 1];

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return ERROR;

    if (sysNetMacAddrCheck(ifMacAddr) == ERROR)
        {
        memcpy(ifMacAddr, pEnet, ifMacAddrLen);
        printf("Invalid MAC Address!\n");
        }

    if (sysNvRamGet(macAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset) != OK)
        {
        printf("Failed to access NVRAM.\n");
        return ERROR;
        }

    if (memcmp(ifMacAddr, macAddr, ifMacAddrLen) == 0)
        {
        printf("Address unchanged.\n");
        return OK;
        }

    (void) sysNvRamSet(ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset);
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
    char macAddr[MAC_ADRS_LEN + 1];

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return ERROR;


    if (sysNvRamGet (macAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET + offset) != OK)
        {
        memcpy(ifMacAddr, pEnet, ifMacAddrLen);
        return OK;
        }

    if (sysNetMacAddrCheck(macAddr) == OK)
        memcpy(pEnet, macAddr, ifMacAddrLen);

    memcpy(ifMacAddr, pEnet, ifMacAddrLen);
    return OK;
    }

