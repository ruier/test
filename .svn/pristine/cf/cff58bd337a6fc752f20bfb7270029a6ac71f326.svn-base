/* sysBusPci.c - Motorola MPC82xx HIP4 platform-specific PCI bus support */

/*
 * Copyright (c) 2003-2004, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,21jan11,agf  clean-up compiler warnings
01c,23sep04,mdo  Documentation fixes for apigen
01b,05aug03,scm  update to support basic PCI-to-PCI bridging...
01a,24apr03,scm  Motorola MPC82xx HIP4 File created from ads826x BSP.
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

#ifdef INCLUDE_PCCARD
IMPORT void sysPci1420BridgePreConfigInit ();
#endif /* INCLUDE_PCCARD */

LOCAL PCI_SYSTEM sysParams;

/*******************************************************************************
*
* sysPciAutoConfig - PCI autoconfig support routine
*
* This routine automatically configures the PCI bus.
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysPciAutoConfig (void)
    {
    /* 32-bit Prefetchable Memory Space */

    sysParams.pciMem32       = CPU_PCI_NO_PRE_MEM_ADRS; /* 32 bit prefetchable memory location */
    sysParams.pciMem32Size   = CPU_PCI_NO_PRE_MEM_SIZE; /* 32 bit prefetchable memory size */

    /* 32-bit Non-Prefetchable Memory Space */

    sysParams.pciMemIo32     = CPU_PCI_MEM_ADRS; /* 32 bit non-prefetchable memory location */
    sysParams.pciMemIo32Size = CPU_PCI_MEM_SIZE; /* 32 bit non-prefetchable memory size  */

    /* 32-bit PCI I/O Space - Remaining memory in the I/O space */

    sysParams.pciIo32        = CPU_PCI_IO32_ADRS; /* 32 bit io location */;
    sysParams.pciIo32Size    = CPU_PCI_IO32_SIZE; /* 32 bit io size */

    /* 16-bit ISA I/O Space - First block of memory in the I/O space */

    sysParams.pciIo16        = CPU_PCI_IO16_ADRS; /* 16 bit io location */
    sysParams.pciIo16Size    = CPU_PCI_IO16_SIZE; /* 16 bit io size */

    /* Configuration space parameters */

    sysParams.cacheSize      = ( _CACHE_ALIGN_SIZE / 4 );
    sysParams.maxLatency     = PCI_LAT_TIMER;

    /*
     * Interrupt routing strategy
     * across PCI-to-PCI Bridges
     */

    sysParams.autoIntRouting = FALSE;

    /* Device inclusion and interrupt routing routines */

    sysParams.includeRtn     = sysPciAutoconfigInclude;
    sysParams.intAssignRtn   = sysPciAutoconfigIntrAssign;

    /*
     * PCI-to-PCI Bridge Pre-
     * and Post-enumeration init
     * routines
     */

    /*
     * Perform any needed PCI Host Bridge
     * Initialization that needs to be done
     * before pciAutoConfig is invoked here
     * utilizing the information in the
     * newly-populated sysParams structure.
     */

#ifdef INCLUDE_PCCARD
    sysParams.bridgePreConfigInit  = sysPci1420BridgePreConfigInit;
#else
    sysParams.bridgePreConfigInit  = sysPciAutoconfigPreEnumBridgeInit;
#endif /* INCLUDE_PCCARD */

    pciAutoConfig (&sysParams);

    /*
     * Perform any needed post-enumeration
     * PCI Host Bridge Initialization here
     * utilizing the information in the
     * sysParams structure that has been
     * updated as a result of the scan
     * and configuration passes.
     */
    sysParams.bridgePostConfigInit =
                        sysPciAutoconfigPostEnumBridgeInit;

    }

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* This routine configures additional PCI bus devices.
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*
* ERRNO
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /* Don't want to auto configure the host bridge */
    if(((pLoc->bus == 0) && (pLoc->device == PQII_MPC8265_ID)) ||
       (devVend == PCI_DEV_ID_8265))
      return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* This routine assigns and interrupt to a device.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
#ifndef PQII_EXTND_PCI_MASTER
     if (pLoc->bus != 0)
      return ((UCHAR)0xFF);
#endif

    /* assign interrupt to appropriate device... */
    switch (pLoc->device)
        {
        case PQII_CARD_BUS_ID :
            /* PCMCIA Card Bus Interrupts */
            if (pLoc->function==0)
              return (UCHAR)PIC1_PC_INTA_IRQ;
            else
              return (UCHAR)PIC1_PC_INTB_IRQ;

#ifdef PQII_EXTND_PCI_MASTER
        /* When configured to master extended backplane INT_A shared between PMC & PCI slot */
        case PQII_PMC_ID :
        case PQII_EXT_SLOT_1 :
            /* device off PMC Connector, and external slot INT_A */
            return (UCHAR)PIC1_INTA_IRQ;

        case PQII_EXT_SLOT_2 :
            /* external slot INT_B */
            return (UCHAR)PIC1_INTB_IRQ;

        case PQII_EXT_SLOT_3 :
            /* external slot INT_C */
            return (UCHAR)PIC1_INTC_IRQ;

#else
        case PQII_PMC_ID :
            /* device off PMC Connector */
            return (UCHAR)PIC1_INTA_IRQ;

#endif  /* PQII_EXTND_PCI_MASTER */

        default:
            return ((UCHAR)0xFF);

       }
    }


/*******************************************************************************
*
* sysPciAutoconfigPreEnumBridgeInit - PCI autoconfig support routine
*
* This routine is a PCI configuration support routine.
*
* RETURNS: N/A
*
* ERRNO
*/


void sysPciAutoconfigPreEnumBridgeInit
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
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
* This routine is a PCI configuration support routine.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysPciAutoconfigPostEnumBridgeInit
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    return;
    }
