/* sysNet.c - IBM Walnut/Ebony system network initialization */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 1999-2004, 2008-2009, 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01w,07jun12,mpc  Fixed potential memory corruption caused by sysNvRamGet().
                 (WIND00101057)
01v,11jan11,pch  CQ250219: move defn of MAC_ADRS_LEN from sysNet.c to config.h
01u,04jan11,pch  CQ248235: add MAC set/get support
		 (from wrSbc405gp vn 01f,08jan07,rec)
01t,20apr09,wap  Switch to VxBus EMAC driver
01s,28aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01r,30aug04,mdo  Documentation fixes for apigen
01q,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
01p,26aug03,jln  change malXXX to ibmMalXXX
01o,15aug03,jln  remove 2 TX channels for 440GP, change loadstring format
01n,20jul03,jln  update to support drv/end/emacEnd.c
01m,25apr02,pch  comment cleanup re PCI_NETWORK_USE_MEM
01l,09jan02,pch  recombine walnut and ebony
01k,30nov01,pch  LN97 must use PCI memory space, not PCI I/O space
01j,29nov01,mcg  added support for two EMACs
01i,06nov01,jtp  ebony version 01a based on walnut/sysNet.h 01h
01h,10oct01,mcg  Changes for new emac driver parameters, 2 TX channel support,
		 and new malInit for multiple MALs.
01g,27jul01,kab  Changed INCLUDE_PCI_NETWORK to INCLUDE_LN97XEND; related
		 cleanup
01f,10may01,mcg  Support for Emac driver multi-channel changes
01e,27nov00,s_m  updated path to header file
01d,11may00,mcg  modified sysln97xEndBldLoadStr for new PCI auto config support
01c,24apr00,mcg  fixed return types on sysLan97x functions
01b,22mar00,mcg  added sysLanIbmEmacEnetAddrSet and Get
		 added sysIbmEmacEndBldLoadStr
01a,04oct99,mcg  create from Spruce version 01a
*/

/*
DESCRIPTION
This library contains board-specific routines for network subsystems.
This library will support either the 405GP/440GP integrated Ethernet
cores called EMAC, or an Allied Telesyn AT-2700TX PCI adapter which
contains the AMD PCnet-FAST+ (AM79C972) controller.
*/

/* This file contributes nothing if INCLUDE_END is not defined */

#ifdef INCLUDE_END

/* includes */

#include "vxLib.h"
#include "stdio.h"
#include "ctype.h"

#ifdef INCLUDE_LN97XEND
#  include "drv/pci/pciConfigLib.h"
#  include "drv/end/ln97xEnd.h"
#endif	/* INCLUDE_LN97XEND */

/***** GLOBALS *****/

#ifdef INCLUDE_LN97XEND
unsigned char sysEnetAddr [6];              /* MAC address for the Am79C97x */
char          sys97xLoadString[100];        /* load string for sys97xLoad   */
#endif	/* INCLUDE_LN97XEND */

/* Forward declarations */
#ifdef INCLUDE_LN97XEND
STATUS sysln97xEndBldLoadStr(void);
#endif	/* INCLUDE_LN97XEND */

/* Local definitions */

/*
 * define to use PCI memory, undef for PCI I/O
 *
 * It should work either way.  If not, perhaps the
 * host <=> PCI bridge is not set up properly.
 *
 * This has no effect unless INCLUDE_LN97XEND
 */

#define PCI_NETWORK_USE_MEM

/* see FIXME ahead of sysMacIndex2Dev() */

#if defined(ETHERNET_MAC_HANDLER) || !defined(PRJ_BUILD)
/*
 * glbEnetAddr assignment order:
 *   emac0
 */

/* terminating zero in glbEnetAddr entry required for call to sysNvRamGet */

LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1] = {
    { ENET_DEFAULT0, ENET_DEFAULT1, ENET_DEFAULT2, 0, 0, 0, 0 },
};

const char *sysNetDevName[MAX_MAC_DEVS] = {"emac"};

#endif /* ETHERNET_MAC_HANDLER || !PRJ_BUILD */

/**************************************************************************
*
* sysNetHwInit - initialize the network interface
*
* This routine initializes the network hardware to a quiescent state.  It
* does not connect interrupts.
*
* Only polled mode operation is possible after calling this routine.
* Interrupt mode operation is possible after the memory system has been
* initialized and sysNetHwInit2() has been called.
*
* RETURNS: N/A
*
* SEE ALSO: sysNetHwInit2()
*/

void sysNetHwInit
    (
    void
    )
    {
    /*
     * Build the load string for the ln97x PCI Ethernet END driver.
     * Some of the parameters cannot be determined until runtime when the
     * PCI slot the card is plugged in is known.
     */
#ifdef INCLUDE_LN97XEND
     sysln97xEndBldLoadStr();
#endif	/* INCLUDE_LN97XEND */

    /*
     * Moved call to sysIbmEmacEndBldLoadStr() to sysNetHwInit2 because MAL
     * init has to be run first and requires malloc.
     */

	return;
    }

/******************************************************************************
*
* sysNetHwInit2 - initialize additional features of the network interface
*
* This routine completes initialization needed for interrupt mode operation
* of the network device drivers.  Interrupt handlers can be connected.
* Interrupt or DMA operations can begin.
*
* RETURNS: N/A
*
*/
void sysNetHwInit2
    (
    void
    )
    {
    return;
    }

#ifdef INCLUDE_LN97XEND
/******************************************************************************
*
* sysln97xEndBldLoadStr - determines arguments, builds ln97xEndLoad parameter string
*
* Searches for the PCI Ethernet card, gets the base memory address of the card
* that was assigned during PCI configuration, selects the correct interrupt
* vector based on the PCI slot, reads the MAC address of the card, and calls
* ln97xEndLoad with all of these parameters.
* This function also enables the Ethernet card for PCI memory cycles and
* and bus mastership.
*
* RETURNS: ERROR if the PCI card is not found
*
*/
STATUS sysln97xEndBldLoadStr
    (
    void
    )
    {
    int i;
    UCHAR intvec;
    UCHAR intlvl;
    void * pciMemAddr;
    void * pciIOAddr;
    int bus;
    int device;
    int function;

    /* Initially set the PCI memory and I/O addresses to NONE */

    pciMemAddr = (void *)NONE;
    pciIOAddr = (void *)NONE;

    /*
     * Find and configure the first AMD Am79C97x device on the PCI bus.
     */
    if (pciFindDevice(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_79C97X, 0,
		      &bus, &device, &function))
	{
	return(ERROR);
	}

#ifdef PCI_NETWORK_USE_MEM
    /*
     * Base Address 1 of the 97x contains the PCI Memory space base address.
     * Translate the PCI address into a processor address.  The 97x does not
     * support prefetching.
     */
    ibmPciConfigRead(bus, device, function,
		     PCI_CFG_BASE_ADDRESS_1, 4, &pciMemAddr);
    pciMemAddr = (void *)((UINT)pciMemAddr & 0xFFFFFFF0);
    pciMemAddr = PCI_MEMIO2LOCAL(pciMemAddr);
#else	/* PCI_NETWORK_USE_MEM */
    /*
     * Base Address 0 of the 97x contains the PCI I/O space base address.
     * Translate the PCI address into a processor address.
     */
    ibmPciConfigRead(bus, device, function,
		     PCI_CFG_BASE_ADDRESS_0, 4, &pciIOAddr);
    pciIOAddr = (void *)((UINT)pciIOAddr & 0xFFFFFFFC);
    pciIOAddr = PCI_IO2LOCAL(pciIOAddr);
#endif	/* PCI_NETWORK_USE_MEM */

    /*
     * Each PCI slot on the Walnut or Ebony board is connected to a different
     * pin on the CPU core's integrated Universal Interrupt Controller(s).
     * The UIC pin was written to the Interrupt Line config register
     * in each device by the PCI auto-configuration code.
     */
    ibmPciConfigRead(bus, device, function, PCI_CFG_DEV_INT_LINE, 1, &intvec);
    intlvl = intvec;

    /*
     * Copy the hardware address from the APROM area to the board_cfg structure
     * and elsewhere.  The PCI Ethernet adapter has a serial EEPROM on-board
     * that contains the MAC address of the card. When the card first powers up,
     * the address is copied into the APROM registers.  The value in the APROM
     * regs will be read from the card and placed into the global variable
     * sysEnetAddr so the driver can find it.
     */
    for (i=0; i<6; i++)
	{
#ifdef PCI_NETWORK_USE_MEM
	sysEnetAddr[i] = sysInByte((UINT)pciMemAddr + APROM01 + i);
#else	/* PCI_NETWORK_USE_MEM */
	sysEnetAddr[i] = sysInByte((UINT)pciIOAddr + APROM01 + i);
#endif	/* PCI_NETWORK_USE_MEM */
	}

    /*
     * Build the initialization string.  It looks like this:
     *
     * <devMemAddr>:<devIoAddr>:<pciMemBase>:<vecNum>:<intLvl>:<memAdrs>:
     * <memSize>:<memWidth>:<csr3b>:<offset>:<flags>
     *
     * The unit number will be tacked onto the beginning by muxEndLoad
     */

    sprintf(sys97xLoadString, "0x%x:0x%x:0x%x:%d:%d:0x%x:0:0x%x:0:0:0",
			       (UINT)pciMemAddr, (UINT)pciIOAddr,
			       (UINT)PCI_SLV_MEM_BUS,
			       intvec, intlvl, NONE, NONE);

    return(OK);

    }

/******************************************************************************
*
* sysLan97xIntEnable - enable the LAN interrupt
*
* Enables the interrupt in the UIC corresponding to the PCI slot where
* the Ethernet card was found
*
* RETURNS: N/A
*
*/

STATUS sysLan97xIntEnable
    (
    UINT intlvl
    )
    {
    return (intEnable (intlvl));
    }

/******************************************************************************
*
* sysLan97xIntDisable - disable the LAN interrupt
*
* Disables the interrupt in the UIC corresponding to the PCI slot where
* the Ethernet card was found
*
* RETURNS: N/A
*
*/

STATUS sysLan97xIntDisable
    (
    UINT intlvl
    )
    {
    return (intDisable (intlvl));
    }

/******************************************************************************
*
* sysLan97xEnetAddrGet - get the Ethernet controller's MAC address
*
* Returns the Ethernet controller's MAC address to the END driver.
*
* RETURNS: N/A
*
*/

STATUS sysLan97xEnetAddrGet
    (
    LN_97X_DRV_CTRL *pDrvCtrl,
    char *enetAdrs
    )
    {
    memcpy(enetAdrs, sysEnetAddr, sizeof(sysEnetAddr));
    return(OK);
    }
#endif /* INCLUDE_LN97XEND */

#ifdef INCLUDE_EMAC_VXB_END
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
*       m0 @ ifMacAddr
*       m1 @ ifMacAddr + 1
*       m2 @ ifMacAddr + 2
*       m3 @ ifMacAddr + 3
*       m4 @ ifMacAddr + 4
*       m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacNVRamAddrGet
    (
    char *      ifName,
    int         ifUnit,
    UINT8 *     ifMacAddr,
    int         ifMacAddrLen
    )
    {
    static UINT8 enetFixed[MAC_ADRS_LEN + 1] = {ENET_DEFAULT0, ENET_DEFAULT1,
                                                ENET_DEFAULT2, 0, 0, 0, 0};

    int len = (ifMacAddrLen > MAC_ADRS_LEN + 1) ?  MAC_ADRS_LEN + 1 : ifMacAddrLen;

    if (ifUnit != 0)
	return (ERROR);

    sysNvRamGet ((char *)enetFixed + 3, 3, NV_ENET_OFFSET_0);

#ifdef	DEBUG_MAC
    printf("read %02x:%02x:%02x from NVRAM @ 0x%x\n",
	   enetFixed[3], enetFixed[4], enetFixed[5], NV_ENET_OFFSET_0);
#endif	/* DEBUG_MAC */

    bcopy((char *)enetFixed, (char *)ifMacAddr, len);

    return (OK);
    }

/******************************************************************************
*
* sysLanIbmEmacEnetAddr0Set - set the Ethernet address for EMAC0
*
* This routine sets the variable portion of the Ethernet address, the first
* three bytes contain a fixed manufacturer's code, while the last 3 bytes are
* set locally, to generate a unique Ethernet address on the local network.
*
* RETURN: OK, always.
*
* \NOMANUAL
*/

STATUS sysLanIbmEmacEnetAddr0Set
    (
    char byte5,                 /* Ethernet address high order byte */
    char byte4,
    char byte3,
    char byte2,
    char byte1,
    char byte0                  /* Ethernet address low order byte */
    )
    {
    char        bytes[3];

    /* Save the lower three bytes in NVRAM */

    bytes[0] = byte2;
    bytes[1] = byte1;
    bytes[2] = byte0;

#ifdef	DEBUG_MAC
    printf("writing %02x:%02x:%02x to NVRAM @ 0x%x\n",
	   bytes[0], bytes[1], bytes[2], NV_ENET_OFFSET_0);
#endif	/* DEBUG_MAC */
    sysNvRamSet (bytes, 3, NV_ENET_OFFSET_0);

    return (OK);
    }
#endif	/* INCLUDE_EMAC_VXB_END */

/*
 * FIXME: we need this code in legacy bootrom builds, and really don't want it
 * in vxWorks image builds, but legacy image builds and legacy bootrom builds
 * use the same sysLib.o.  So, we have to include it in all legacy builds
 * (!PRJ_BUILD).
 */

#if defined(ETHERNET_MAC_HANDLER) || !defined(PRJ_BUILD)
/***********************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a device string index e.g. emac.
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
    return (0);      /* always emac */
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
    int *       pOffset         /* offset in NvRAM */
    )
    {

    int index = MAX_MAC_DEVS - 1;  /* index into device names: always 0 */

    /*
     * The order of addresses in NvRAM and glbEnetAddr[] are:
     *
     *   emac0
     *
     */

    /* validate device name */

    if ( strcmp(ifName, sysNetDevName[index]) == 0 )
	{
	/* validate unit number  */

	if (ifUnit != 0)
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
    int   temp;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
	{
	return(ERROR);
	}

    /* check MAC address in NvRAM */

    sysNetMacNVRamAddrGet (ifName, ifUnit, (UINT8 *)pEnet, ifMacAddrLen);

#ifdef	DEBUG_MAC
    printf("requested %02x:%02x:%02x:%02x:%02x:%02x\n",
	   ifMacAddr[0], ifMacAddr[1], ifMacAddr[2],
	   ifMacAddr[3], ifMacAddr[4], ifMacAddr[5]);
    printf("  found   %02x:%02x:%02x:%02x:%02x:%02x\n",
	   pEnet[0], pEnet[1], pEnet[2],
	   pEnet[3], pEnet[4], pEnet[5]);
#endif	/* DEBUG_MAC */

    if (memcmp (ifMacAddr, pEnet, ifMacAddrLen) == 0)
	{

	/* same address so don't erase and rewrite */

	printf("Address unchanged\n");
	return(OK);
	}

    /* 1st 3 params are unused, may as well pass zero */
    sysLanIbmEmacEnetAddr0Set (0,0,0, ifMacAddr[3], ifMacAddr[4], ifMacAddr[5]);

    /* copy MAC address to RAM buffer */

    memcpy (pEnet + 3, ifMacAddr + 3, ifMacAddrLen - 3);

#ifdef	DEBUG_MAC
    printf("returning %02x:%02x:%02x:%02x:%02x:%02x\n",
	   pEnet[0], pEnet[1], pEnet[2],
	   pEnet[3], pEnet[4], pEnet[5]);
#endif	/* DEBUG_MAC */

    return (OK);
    }
#endif /* ETHERNET_MAC_HANDLER || !PRJ_BUILD */

#endif /* INCLUDE_END */
