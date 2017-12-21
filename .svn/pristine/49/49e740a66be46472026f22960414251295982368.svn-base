/* sysBusPci.c - Motorola MPC82xx HIP4 platform-specific PCI bus support */

/*
 * Copyright (c) 2002, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,30may13,d_l  Fix prevent error. (WIND00419006)
01a,04jul02,dtr  Motorola MPC82xx HIP4 File created from sandpoint BSP.
*/

/*
DESCRIPTION
This is the  platform specific pciAutoConfigLib information.
*/


/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysLib.h"

#include "drv/pci/pciConfigLib.h" 	
#include "drv/pci/pciIntLib.h" 	
#include "drv/pci/pciAutoConfigLib.h"	
#include "sysBusPci.h"

/* static file scope locals */
IMPORT void sysPciOutLong(UINT32*,UINT32);
IMPORT UINT32 sysPciInLong (UINT32*); 
LOCAL PCI_SYSTEM sysParams;


/* INT LINE TO IRQ assignment for MPC8266ADS-PCI board. */

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL}, /* slot 1 */
    {PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL}, /* slot 2 */
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL}, /* slot 3 */
};


/*******************************************************************************
*
* sysPciAutoConfig - PCI autoconfig support routine
*
* RETURNS: N/A
*/


void sysPciAutoConfig (void)
    {

    /* 32-bit Prefetchable Memory Space */

    sysParams.pciMem32 		= PCI_MEM_ADRS;
    sysParams.pciMem32Size 	= PCI_MEM_SIZE;

    /* 32-bit Non-prefetchable Memory Space */

    sysParams.pciMemIo32 	= PCI_MEMIO_ADRS;
    sysParams.pciMemIo32Size 	= PCI_MEMIO_SIZE;

    /* 32-bit PCI I/O Space */

    sysParams.pciIo32 		= PCI_IO_ADRS;
    sysParams.pciIo32Size 	= PCI_IO_SIZE;

    /* Configuration space parameters */

    sysParams.maxBus 		= 0;
    sysParams.cacheSize 	= ( _CACHE_ALIGN_SIZE / 4 );
    sysParams.maxLatency 	= PCI_LAT_TIMER;

    /*
     * Interrupt routing strategy
     * across PCI-to-PCI Bridges
     */

    sysParams.autoIntRouting 	= TRUE;

    /* Device inclusion and interrupt routing routines */

    sysParams.includeRtn 	= sysPciAutoconfigInclude;
    sysParams.intAssignRtn 	= sysPciAutoconfigIntrAssign;

    /*
     * PCI-to-PCI Bridge Pre-
     * and Post-enumeration init
     * routines
     */

    sysParams.bridgePreConfigInit =
			sysPciAutoconfigPreEnumBridgeInit;
    sysParams.bridgePostConfigInit =
			sysPciAutoconfigPostEnumBridgeInit;
    /*
     * Perform any needed PCI Host Bridge
     * Initialization that needs to be done
     * before pciAutoConfig is invoked here 
     * utilizing the information in the 
     * newly-populated sysParams structure. 
     */

    pciAutoConfig (&sysParams);

    /*
     * Perform any needed post-enumeration
     * PCI Host Bridge Initialization here
     * utilizing the information in the 
     * sysParams structure that has been 
     * updated as a result of the scan 
     * and configuration passes. 
     */

    }

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    
    /* Don't want to auto configure the bridge */ 
     if ( devVend == PCI_DEV_ID_8266 )
      return(ERROR);
 
    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5))       				&& 
	(((pLoc->device) - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS) 	&&
	(((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
	{
	tmpChar = 
	    sysPciIntRoute [((pLoc->device) - PCI_SLOT1_DEVNO)][(pin-1)];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }


/*******************************************************************************
*
* sysPciAutoconfigPreEnumBridgeInit - PCI autoconfig support routine
*
* RETURNS: N/A
*/


void sysPciAutoconfigPreEnumBridgeInit
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    return;
    }


/*******************************************************************************
*
* sysPciAutoconfigPostEnumBridgeInit - PCI autoconfig support routine
*
* RETURNS: N/A
*/

void sysPciAutoconfigPostEnumBridgeInit
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    return;
    }







