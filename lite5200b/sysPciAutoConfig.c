/* sysPciAutoConfig.c - Freescale Lite5200B PCI autoconfig support */

/*
 * Copyright (c) 2003-2005, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,06oct08,b_m  code cleanup.
01f,18jan07,b_m  Add pci irq routing for lite5200b.
01e,11aug05,j_b  add wireless LAN support
01d,23dec04,k_p  indentation and cleanups and warning fixes.
01c,04feb04,bjn  adding IS_ICECUBE_OLD
01b,23sep03,pdr  excluded MPC5200 for PCI autoconfig
01a,07Aug03,pkr  written
*/

/* includes */

#include <vxWorks.h>
#include <logLib.h>
#include <taskLib.h>
#include "config.h"

#include <drv/pci/pciAutoConfigLib.h>
#include <pci/pciAutoConfigLib.c>

/* typedefs */

/* globals */

PCI_SYSTEM sysParams;

/* locals */

/*
 * PCI interrupt routing:
 *
 * The PCI interrupt routing depends on the slot which the PCI card
 * is plugged in.
 *
 *   Slot1         Slot2
 *   +---+         +---+
 *   | A |----+    | A |----+
 *   |   |    |    |   |    |
 *   | B |---+|    | B |---+|
 *   |   |   ||    |   |   ||
 *   | C |--+||    | C |--+||
 *   |   |  |||    |   |  |||
 *   | D |-+|||    | D |-+|||
 *   +---+ ||||    +---+ ||||
 *         ||||          ||||
 *         ||||          ||||
 *         ||||          ||||                          MPC5200B
 *         ||||          ||||                         +--------+
 *   -A----|||+----------+|||-----------------------A-|  IRQ0  | <=> INTA
 *         |||            |||                         |        |
 *   -B----||+------------||+-----------------------B-|  IRQ1  | <=> INTB
 *         ||             ||                          |        |
 *   -C----|+-------------|+------------------------C-|  IRQ2  | <=> INTC
 *         |              |                           |        |
 *   -D----+--------------+-------------------------D-|  IRQ3  | <=> INTD
 *                                                    +--------+
 *
 * See Schematics (SPF-20860_A_color.pdf) Page 9,14,15
 *
 * The interrupt pin 1,2,3 or 4 describes the INTA...INTD
 * The interrupt line describes the interrupt level set by the host (autocfg)
 *
 * Slot1 is on AD24 (devNum = 24 (0x18))
 * Slot2 is on AD25 (devNum = 25 (0x19))
 * MPC5200B is on AD26 (devNum = 26 (0x1a))
 *
 */

static UCHAR intLine [][4] =
{
    /* IntA  IntB  IntC  IntD */
    {  0x00, 0x01, 0x02, 0x03  },   /* device number 24 - AD24 */
    {  0x01, 0x02, 0x03, 0x00  },   /* device number 25 - AD25 */
};

/* forward declarations */

LOCAL UCHAR  sysPciAutoConfigIntAsgn ( PCI_SYSTEM * pSys, PCI_LOC * pFunc,
                                       UCHAR intPin );
LOCAL STATUS sysPciAutoConfigInclude ( PCI_SYSTEM *pSys, PCI_LOC *pciLoc,
                                       UINT devVend );

/* Latency Timer value - 64 PCI clocks */

#define PCI_LAT_TIMER   0x40

/* subroutines */

/*******************************************************************************
*
* sysPciAutoConfigInclude - Determine if function is to be autoConfigured
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
* RETURNS: TRUE if function is to be included in automatic configuration,
* FALSE otherwise.
*/

LOCAL int sysPciAutoConfigInclude
    (
    PCI_SYSTEM *    pSys,       /* input: AutoConfig system information */
    PCI_LOC *       pLoc,       /* input: PCI address of this function  */
    UINT            devVend     /* input: Device/vendor ID number       */
    )
    {
    BOOL    retVal    = OK;

    switch (devVend)
        {

        /*
         * If it's the MPC5200 then exclude it as it is already be configured.
         *
         * Note that ERROR is returned, so the device will not be
         * auto-configured.
         */

        case MPC5200_PCI_ID:
            retVal = ERROR;
            break;

        default:
            retVal = OK;
            break;
        }

    return (retVal);
    }

/*******************************************************************************
*
* sysPciMinDevGet - get PCI minimum devices
*
* This function gets number of PCI minimum devices.
*
* RETURNS: PCI_MIN_DEV
*
*/

LOCAL int sysPciMinDevGet (void)
    {
    return PCI_MIN_DEV;
    }

/*******************************************************************************
*
* sysPciAutoConfigIntAssign - Assign the "interrupt line" value.
*
* This routine assign the interrupt line value.
*
* RETURNS: "interrupt line" value.
*/

LOCAL UCHAR  sysPciAutoConfigIntAsgn
    (
    PCI_SYSTEM *    pSys,
    PCI_LOC    *    pLoc,
    UCHAR           intPin
    )
    {
    UCHAR irqValue = 0xff;

    if ((intPin == 0)                 ||
        (pLoc->device >= PCI_MAX_DEV) ||
        (pLoc->device < (sysPciMinDevGet())))
        {
        return irqValue;
        }

    if (pLoc->bus == MPC5200_PCI_BRIDGE)
        {
        /*
         * two slots
         *  INTA (intpin 1) for Slot 1 (routes to IRQ0)
         *  INTA (intpin 1) for Slot 2 (routes to IRQ1)
         * .... See table above...
         */

        if ((pLoc->device != 0x18) && (pLoc->device != 0x19))
            {
            logMsg ("PCI Interrupt error in device %d\n",pLoc->device,2,3,4,5,6);
            irqValue = 0xff;
            }
        else
            {
            irqValue = intLine [(pLoc->device)-PCI_MIN_DEV][(intPin - 1)];
            }
        }

    return irqValue;
    }

/*******************************************************************************
*
* sysPciAutoConfig - PCI autoConfig support routine
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

    sysParams.pciMem32          = BUS_PCI_MEM_ADRS;
    sysParams.pciMem32Size      = PCI_MEM_SIZE;

    /* 32-bit non-Prefetchable Memory Space */

    sysParams.pciMemIo32        = BUS_PCI_NO_PRE_MEM_ADRS;
    sysParams.pciMemIo32Size    = PCI_NO_PRE_MEM_SIZE;

    /* 16-bit ISA I/O Space - First block of memory in the I/O space */

    sysParams.pciIo16           = BUS_PCI_IO_ADRS;
    sysParams.pciIo16Size       = 0xffff;

    /* 32-bit PCI I/O Space - Remaining memory in the I/O space */

    sysParams.pciIo32           = BUS_PCI_IO_ADRS + 0x10000;
    sysParams.pciIo32Size       = PCI_IO_SIZE - 0x10000;

    /* Configuration space parameters */

    sysParams.maxBus                = PCI_MAX_BUS;
    sysParams.cacheSize             = ( _CACHE_ALIGN_SIZE / 4 );
    sysParams.maxLatency            = PCI_LAT_TIMER;
    sysParams.autoIntRouting        = TRUE;
    sysParams.includeRtn            = sysPciAutoConfigInclude;
    sysParams.intAssignRtn          = sysPciAutoConfigIntAsgn;
    sysParams.bridgePreConfigInit   = NULL;
    sysParams.bridgePostConfigInit  = NULL;

    pciAutoConfig (&sysParams);
    }
