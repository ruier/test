/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2003-2004, 2006, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
02e,21jan11,agf  clean-up compiler warnings
02d,21aug06,wap  Remove unused variable
02c,21aug06,wap  Get rid of redundant byte swapping code
02b,24sep04,mdo  Documentation fixes for apigen
02a,29jul04,rec  adapted from wrSbc8260Atm version 02b
01e,16jul03,j_b  remove vware support
01d,12jun03,j_b  support only 4 units (from 8); cleanup
01c,24apr03,jb3  Add SetIPAdrs for multi net support.
01b,23apr03,jb3  Add IP address set and get for Multi Enet support.
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01e)
*/

/*
DESCRIPTION

This module provides BSP functionality to support the
bootrom 'M' command to modify MAC addresses of on-board
network interfaces.

MAC address routines provided by the BSP in this file are:
   sysNetMacNVRamAddrGet()
   sysNetMacAddrGet()
   sysNetMacAddrSet()

This board provides storage in flash for the MAC addresses
of the motfcc and motscc interfaces.  This library also
implements a RAM buffer to represent the contents of the
flash.  The RAM buffer contains two entries.  This RAM buffer 
is contained in the array glbEnetAddr[][].
*/

#ifdef ETHERNET_MAC_HANDLER

#include "vxWorks.h"
#include "config.h"
#include "wrSbcPowerQuiccII.h"
#include "ctype.h"
#include "fioLib.h"

/* locals */

/* defines */

#define MAX_LINE 80

/* globals */

/* locals */

/*
 * glbEnetAddr and glbIPAddr assignment order 
 *   motscc0
 *   motfcc0
 *   motfcc1
 *   motfcc2
 */

/* terminating zero in glbEnetAddr entry required for call to sysNvRamGet */

LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1] = {
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_0, CUST_ENET4, CUST_ENET5, 0 },
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_1, CUST_ENET4, CUST_ENET5, 0 },
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_2, CUST_ENET4, CUST_ENET5, 0 },
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_3, CUST_ENET4, CUST_ENET5, 0 },
};

const char *sysNetDevName[MAX_MAC_DEVS] = {"motscc", "motfcc"};

LOCAL UINT8 sysInvalidAddr[2][MAC_ADRS_LEN] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

static UINT8 glbIPAddr[MAX_IP_ADRS][IP_ADRS_LEN] = {
	"192.168.000.001",
	"192.168.001.001",
	"192.168.002.001",
	"192.168.003.001",
};

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
    if (index == 0)
        return (0);  /* first index is motscc */
    return (1);      /* remaining are motfcc */
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
    if (index == 0)
        return (0);       /* first index is motscc (0) */
    return (index - 1);   /* remaining three are motfcc (0,1,2) */
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
    int *	pOffset		/* offset in NVRAM */
    )
    {
    int index;

    /*
     * The address offsets into NVRAM and glbEnetAddr[] are:
     *
     *	motscc0 - offset 0
     *  motfcc0 - offset 1
     *  motfcc1 - offset 2
     *  motfcc2 - offset 3
     *
     */
     
    for (index = 0; index < MAX_MAC_DEVS; index++)
        {

        /* search through device names */

	if ( strcmp(ifName, sysNetDevName[index]) == 0 )
	    {
	    
	    
            if (index == 0) /* motscc has only one unit */
                {
                if (ifUnit != 0)
                    return (ERROR);
                *ppEnet  = (char*)glbEnetAddr[0];
	        *pOffset = 0;
                }
            else /* motfcc has three devs */
                {
                if ((ifUnit < 0) || (ifUnit > 2))
                    return (ERROR);
                 *ppEnet  = (char*)glbEnetAddr[index + ifUnit];
                 *pOffset = (index + ifUnit) * MAC_ADRS_LEN;
                }
            
            return(OK);
	    }
	}
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

#if (NV_RAM_SIZE != NONE)
    /* get MAC address from NvRAM. */
    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
#endif /* (NV_RAM_SIZE != NONE) */

    if ( memcmp(pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0 )
        return(ERROR);
    if ( memcmp(pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0 )
        return(ERROR);
	
    /* copy mmac address to memory for local use */

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

#if (NV_RAM_SIZE != NONE)

    /* check MAC address in NvRAM. */
    
    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
    if (0 == memcmp (ifMacAddr, pEnet, ifMacAddrLen))
        {

        /* same address so don't erase and rewrite flash */

        printf("Address unchanged\n");
        return(OK);
        }

    sysNvRamSet ((char*)ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
#endif /* (NV_RAM_SIZE != NONE) */

    /* copy mmac address to memory for local use */
    
    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

#endif /* ETHERNET_MAC_HANDLER */

/***********************************************************************
*
* sysIPAddrGet - gets the IP address string
*
* This routine gets the IP address string used by the IP stack
* device.  xxx.xxx.xxx.xxx:mmmmmmmm where mm is mask in hex.
*
* RETURNS: OK
*
* ERRNO
*
* SEE ALSO: sysIPAddrSet()
*/

STATUS sysIPAddrGet
    (
    int     unit ,
    UINT8 * addr     /* Location address is returned in   */
    )
    {
    int   offset;
    char *pEnet;

    if ((unit < 0) || (unit >= MAX_MAC_ADRS))
        {
        return ERROR;
        }
    pEnet  = (char*)glbIPAddr[unit];
    offset = unit * IP_ADRS_LEN;

#if (NV_RAM_SIZE != NONE)
    {
        sysNvRamGet ((char*)addr, IP_ADRS_LEN, NV_IP_ADRS_OFFSET + offset);

        /* valid address ? */
        if ( ( *(addr+ 3) == '.' ) && 
             ( *(addr+ 7) == '.' ) && 
             ( *(addr+11) == '.' ) && 
             ( *(addr+15) == ':' ) )
            {
            return OK;
            }
    }
    /* nope set defaults */
    sysNvRamSet (pEnet,IP_ADRS_LEN,NV_IP_ADRS_OFFSET + offset);
#endif /* (NV_RAM_SIZE != NONE) */

    /* IP address in memory only */
    memcpy (addr,pEnet,IP_ADRS_LEN);


    return (OK);
    }
