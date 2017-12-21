/* sysBusPci.c - Motorola MPC85xx platform-specific PCI bus support */

/*
 * Copyright (c) 2002-2005, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01g,13jul10,sye  PCI mapping changed by FIX WIND00163710, included sATA 
                 controller in PCI autoconfig. (WIND00135823)
01f,20oct05,cak  Fix PCI mapping error.
01e,15may05,efb  Excluded sATA controller from PCI autoconfig.
01d,10dec04,cak  PCI/PCI-X Bus Interface support. 
01c,23aug04,scb  Starting point for "Big Easy".
01b,31jul03,dtr  Temporary fix for bad interrupt wiring on board.
01a,04jul02,dtr  Motorola MPC85xx file created from ads826x BSP.
*/

/*
DESCRIPTION
This is the  platform specific pciAutoConfigLib information.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysLib.h"
#include "config.h"
#include "drv/pci/pciConfigLib.h" 	

/* defines */

/* typedefs */

/* globals */

/* locals */

LOCAL PCI_SYSTEM sysParams;

LOCAL UCHAR intLine [][4] = INT_LINES;

/* forward declarations */

void sysPciAutoConfig (void);
LOCAL UCHAR sysPciAutoConfigIntAsgn (PCI_SYSTEM * pSys, PCI_LOC * pFunc,
                                     UCHAR intPin);
LOCAL STATUS sysPciAutoConfigInclude (PCI_SYSTEM * pSys, PCI_LOC * pciLoc,
                                      UINT devVend);

/* externals */

/*******************************************************************************
*
* sysPciAutoConfig - PCI autoconfig support routine
*
* This routine instantiates the PCI_SYSTEM structure needed to configure
* the system. This consists of assigning address ranges to each category
* of PCI system resource: Prefetchable and Non-Prefetchable 32-bit Memory, and
* 16- and 32-bit I/O. Global values for the Cache Line Size and Maximum 
* Latency are also specified. Finally, the four supplemental routines for 
* device inclusion/exclusion, interrupt assignment, and pre- and 
* post-enumeration bridge initialization are specified.
*
* RETURNS: N/A
*/

void sysPciAutoConfig (void)
    {

    /* 32-bit Prefetchable Memory Space */

    sysParams.pciMem32 		= PCI_MSTR_MEM_BUS;
    sysParams.pciMem32Size 	= PCI_MSTR_MEM_SIZE;

    /* 32-bit Non-prefetchable Memory Space */

    sysParams.pciMemIo32 	= PCI_MSTR_MEMIO_BUS;
    sysParams.pciMemIo32Size 	= PCI_MSTR_MEMIO_SIZE;

    /* 32-bit PCI I/O Space */

    sysParams.pciIo32 		= PCI_MSTR_IO_BUS;
    sysParams.pciIo32Size 	= PCI_MSTR_IO_SIZE;

    /* 16-bit I/O size */

    sysParams.pciIo16 		= ISA_MSTR_IO_BUS  + ISA_LEGACY_SIZE;
    sysParams.pciIo16Size	= ISA_MSTR_IO_SIZE - ISA_LEGACY_SIZE;

    /* Configuration space parameters */

    sysParams.maxBus 		= 0;
    sysParams.cacheSize 	= PCI_CLINE_SZ;
    sysParams.maxLatency 	= PCI_LAT_TIMER;

    /* Interrupt routing strategy across PCI-to-PCI Bridges */

    sysParams.autoIntRouting 	= TRUE;

    /* Device inclusion and interrupt routing routines */

    sysParams.includeRtn 	= sysPciAutoConfigInclude;
    sysParams.intAssignRtn 	= sysPciAutoConfigIntAsgn;

    /* PCI-to-PCI Bridge Pre- and Post-enumeration init routines */

    sysParams.bridgePreConfigInit = NULL;
    sysParams.bridgePostConfigInit = NULL;

    pciAutoConfig (&sysParams);

    }

/*******************************************************************************
*
* sysPciAutoConfigInclude - PCI autoconfig support routine
*
* This function is called with PCI bus, device, function, and vendor
* information.  It returns an indication of whether or not the particular
* function should be included in the automatic configuration process.
* This capability is useful if it is desired that a particular function
* NOT be automatically configured.  Of course, if the device is not
* included in automatic configuration, it will be unusable unless the
* user's code made provisions to configure the function outside of the
* the automatic process.
*
* RETURNS: OK if function is to be included in automatic configuration,
* ERROR otherwise.
*/

LOCAL STATUS sysPciAutoConfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pciLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    STATUS status = OK;
    
    /* Don't want to auto configure the bridge */ 

    if ((pciLoc->bus == PCI_HOST_BRIDGE_BUS_NUM) && 
        (pciLoc->device == PCI_HOST_BRIDGE_DEV_NUM))
        status = ERROR;

    return status; 
    }

/*******************************************************************************
*
* sysPciAutoConfigIntAsgn - PCI autoconfig support routine
*
* This routine is used in the assignment of an "interrupt Line" value,
* for a PCI device, based on the value found in the INT_LINES table.
* 
* RETURNS: PCI "interrupt line" value
*/

LOCAL UCHAR sysPciAutoConfigIntAsgn
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pFunc,			/* pointer to function in question */
    UCHAR intPin			/* contents of PCI int pin register */
    )
    {
    UCHAR irqValue = 0xff;

    if (intPin == 0)
        return irqValue;

    irqValue = intLine [(pFunc->device)][(intPin - 1)];

    return (irqValue);
    }
