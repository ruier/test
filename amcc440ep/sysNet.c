/* sysNet.c - amcc440ep bsp system network initialization */

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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* 
 * Copyright 2006, 2008, 2011, 2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,26apr12,wqi  Fixed a memory corruption caused by MAC address get and set.
                 (WIND00344736)  
01j,18may11,swu  fix bootApp couldn't boot issue.(WIND00250960)
01i,28aug08,d_l  break ENET_DEFAULT to three bytes.(WIND00121398)
01h,21mar08,wap  Add VxBus support, switch to VxBus EMAC driver
01g,05apr06,mmi  change emacEnd.h path, removed EMAC_IBM_440EP and DEBUG code 
01f,05apr06,mmi  add #include "sysNet.h" 
01e,04apr06,mmi  mods the generic emacEnd.c driver 
01d,03apr06,mmi  mods to fix the display of devices for M command 
01c,15mar06,mmi  mods to add support for M command
01b,15mar06,mmi  clean mod history, copyrights
01a,09mar06,mmi  create based on bamboo version 01r 

*/

/*
DESCRIPTION
This library contains board-specific routines for network subsystems.
This library will support either the 405GP/440GP/440EP integrated Ethernet
cores called EMAC, or an Allied Telesyn AT-2701FTX PCI adapter which
contains the AMD PCnet-FAST+ (AM79C972) controller.
*/

/* This file contributes nothing if INCLUDE_NETWORK is not defined */

#ifdef INCLUDE_NETWORK

/* includes */

#include "vxLib.h"
#include "stdio.h"
#include "ctype.h"
#include "config.h"
#include "sysNet.h"

#ifdef INCLUDE_LN97XEND
#  include "drv/pci/pciConfigLib.h"
#  include "drv/end/ln97xEnd.h"
#endif  /* INCLUDE_LN97XEND */

#ifdef INCLUDE_FEI82557END
#  include "sysFei82557End.c"
#endif /* INCLUDE_FEI82557END */


/* imports */


/* globals */

#ifdef INCLUDE_LN97XEND
unsigned char sysEnetAddr [6];              /* MAC address for the Am79C97x */
char        sys97xLoadString[100];        /* load string for sys97xLoad   */
#endif	/* INCLUDE_LN97XEND */

/* Forward declarations */

#ifdef INCLUDE_LN97XEND
STATUS sysln97xEndBldLoadStr(void);
#endif	/* INCLUDE_LN97XEND */

#ifdef INCLUDE_EMAC_VXB_END


/* globals */

const char *sysNetDevName[MAX_MAC_DEVS] = {"emac"};


IMPORT int dynamicMacAddrGen
    (
    UINT8 * ifName,		/* interface name */
    int ifUnit,			/* interface unit */
    UINT8 * ifMacAddr,		/* address buffer */
    int ifMacAddrLen		/* length of buffer */
    );

/* locals */
LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN + 1 ] = {
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_0, CUST_ENET4, CUST_ENET5, 0 },
	{ WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_1, CUST_ENET4, CUST_ENET5, 0 },
};

#define NV_MAC_ADRS_OFFSET 0x100

#endif	/* INCLUDE_EMAC_VXB_END */

/* Local definitions */

/*
 * define to use PCI memory, undef for PCI I/O
 *
 * It should work either way.  If not, perhaps the
 * host <=> PCI bridge is not set up properly.
 *
 * This has no effect unless INCLUDE_LN97XEND
 * \NOMANUAL
 */
#define PCI_NETWORK_USE_MEM


/******************************************************************************
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
* ERRNO
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
* ERRNO
*/
void sysNetHwInit2
    (
    void
    )
    {
#if defined(INCLUDE_PCI) && defined(INCLUDE_FEI82557END)
    sys557PciInit (); 		/* Inet 82557/9 device */
#endif /* defined(INCLUDE_PCI) && defined(INCLUDE_FEI82557END) */

    return;
    }

#ifdef INCLUDE_LN97XEND
/******************************************************************************
*
* sysln97xEndBldLoadStr - determines arguments for and builds the ln97xEndLoad
*                         parameter string
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
* ERRNO
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
     * Each PCI slot on the Walnut,Ebony or Bamboo board is connected to a different
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
* ERRNO
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
* ERRNO
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
* ERRNO
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
/******************************************************************************
*
* sysLanIbmEmacEnetAddrGet - get the IBM EMAC Ethernet controller's MAC address
*
* Returns the Ethernet controller's MAC address to the END driver.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysLanIbmEmacEnetAddrGet
    (
    EMAC_DRV_CTRL * pDrvCtrl,
    UINT8 *   enetAdrs
    )
    {
    UINT8   enetFixed [7] = {ENET_DEFAULT0, ENET_DEFAULT1, ENET_DEFAULT2,
                             0, 0, 0, 0};
    UINT8 * pEnetFixed    = enetFixed;

    /* Read and store the variable address bytes from NVRAM */
    if (pDrvCtrl->unit == 0)
        sysNvRamGet (pEnetFixed + 3, 3, NV_ENET_OFFSET_0);
#ifdef  USE_BOTH_EMACS
    else if (pDrvCtrl->unit == 1)
        sysNvRamGet (pEnetFixed + 3, 3, NV_ENET_OFFSET_1);
#endif  /* USE_BOTH_EMACS */
    else
        return(ERROR);

    /* Set the fixed manufacturer's address bytes */
    bcopy(pEnetFixed, enetAdrs, 6);

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
* RETURNS: OK, always.
*
* ERRNO
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
    sysNvRamSet (bytes, 3, NV_ENET_OFFSET_0);

    return (OK);
    }

# ifdef USE_BOTH_EMACS
/******************************************************************************
*
* sysLanIbmEmacEnetAddr1Set - set the Ethernet address for EMAC1
*
* This routine sets the variable portion of the Ethernet address, the first
* three bytes contain a fixed manufacturer's code, while the last 3 bytes are
* set locally, to generate a unique Ethernet address on the local network.
*
* RETURNS: OK, always.
*
* ERRNO
*
* \NOMANUAL
*/

STATUS sysLanIbmEmacEnetAddr1Set
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
    sysNvRamSet (bytes, 3, NV_ENET_OFFSET_1);

    return (OK);
    }
# endif /* USE_BOTH_EMACS */

#endif  /* INCLUDE_EMAC_VXB_END */


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

    /*
     * The address offsets into NVRAM and glbEnetAddr[] are:
     *
     *	cpm0 - offset 0
     *
     */

    if (strcmp (ifName, "emac") == 0 )
	{

	if (ifUnit >= MAX_MAC_ADRS || ifUnit < 0 )
	    return(ERROR);

	*pOffset = ifUnit * MAC_ADRS_LEN;
	*ppEnet  = (char*)glbEnetAddr[ifUnit];

	}
    else
    	return(ERROR);

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
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

#if (NV_RAM_SIZE != NONE)

    /* get MAC address from NvRAM. */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
#endif /* (NV_RAM_SIZE != NONE) */

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
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

#if (NV_RAM_SIZE != NONE)

    /* check MAC address in NvRAM. */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
    if (0 == memcmp (ifMacAddr, pEnet, ifMacAddrLen))
	{ 

	/* same address so don't erase and rewrite flash */

	printf("Address unchanged\n");
	return (OK);
	}

    sysNvRamSet (ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
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
    if (index < 2)
        return (0);  /* first 2 index are emac */
    return (-1);    /* Address are defined according to sysNetDevName */ 
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
    if (index < 2)
        return (index);       /* first 2 index are emac (0, 1) */
    return (-1);   /* remaining three are motfec */
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

#endif /* INCLUDE_NETWORK */
