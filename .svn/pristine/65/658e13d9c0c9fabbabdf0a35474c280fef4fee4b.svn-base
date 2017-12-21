/* sysBusPci.c - amcc_taishan IBM PLB_PCIX bridge autoconfig support */

/*
******************************************************************************
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
 * Copyright (c) 2001 - 2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01j,03dec07,x_s  Updated for supporting vxbPpc440gxPci   
01i,30aug04,mdo  Documentation fixes for apigen
01h,03dec03,agf  fix compiler warnings
01g,27aug03,jtp  SPR77524 handle subordinate bridges with type 1 cycle
01f,12aug03,jtp  Formal code inspection changes: update to pciAutoCfg()
01e,18jul03,jtp  Removed unneeded macro
01d,02jul03,jtp  Support PCI
01c,03may02,pch  more for SPR 74434: special-case the host bridge so
                 ibmPciConfig{Read,Write} can access its configuration
                 registers.
01b,29apr02,pch  SPR 74434: fix PCI mappings
01a,06dec01,mcg  Created, from walnut/sysBusPci.c version 01b.  Converted
		 to PLB_PCIX bridge, moved board dependent interrupt
		 information to board.h file, changes to ConfigRead and
		 ConfigWrite functions, added special cycle support.
*/

/*
DESCRIPTION

This file contains functions needed to initialize and kick off the PCI-X
auto-configuration process, functions needed by the PCI-X auto-configurations
routines to perform configuration cycles (mechanism 0), and a function to
initialize the PCI-X bridge itself.  This driver supports the IBM PLB_PCIX
Bridge controller core. 

INCLUDE FILES: 
*/

/* includes */

#include "vxWorks.h"
#include "logLib.h"
#include "taskLib.h"
#include "config.h"

/* defines */

#define PCI_NO_INT 0xFF
/* typedefs */

/* globals */

/* forward declarations */

UCHAR sysPciAutoConfigIntAsgn ( PCI_SYSTEM * pSys, PCI_LOC * pFunc,
    UCHAR intPin );
STATUS sysPciAutoConfigInclude ( PCI_SYSTEM *pSys, PCI_LOC *pciLoc,
    UINT devVend );

/* subroutines */

/******************************************************************************
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
*
* ERRNO
*/

STATUS sysPciAutoConfigInclude
    (
    PCI_SYSTEM *pSys,       /* input: AutoConfig system information */
    PCI_LOC *	pciLoc,		/* input: PCI address of this function */
    UINT     	devVend     /* input: Device/vendor ID number      */
    )
    {
    BOOL retVal = OK;


    /* If it's the host bridge then exclude it */

    if ((pciLoc->bus == 0) && (pciLoc->device == 0) && (pciLoc->function == 0))
        return ERROR;


    switch(devVend)
        {

        /* TODO - add any excluded devices by device/vendor ID here */

        default:
            retVal = OK;
            break;
        }

    return retVal;
    }

/******************************************************************************
*
* sysPciAutoConfigIntAsgn - Assign the "interrupt line" value
*
* This routine assigns the interrupt line value to a PCI device.
*
* RETURNS: "interrupt line" value.
*
* ERRNO
*/

UCHAR sysPciAutoConfigIntAsgn
    (
    PCI_SYSTEM * pSys,          /* input: AutoConfig system information */
    PCI_LOC * pFunc,
    UCHAR intPin                /* input: interrupt pin number */
    )
    {
    UCHAR irqValue = 0xff;    /* Calculated value                */
    UINT8 slot;
    if (intPin == 0)
        return irqValue;

    slot = (pFunc->device & 0x07);

    switch (slot) 
        {
        case 1:
            irqValue = INT_VEC_EXT_IRQ_0;
            break; 
        case 2:
            irqValue = INT_VEC_EXT_IRQ_1;
            break; 
        case 3:
            irqValue = INT_VEC_EXT_IRQ_2;
            break; 
        case 4:
            irqValue = INT_VEC_EXT_IRQ_3;
            break;
        default:
            break;
        } 
#if 0 /* need to replace  later */
    PCI_AUTO_DEBUG_MSG("intAssign called for device [%d %d %d] IRQ: %d\n",
                pFunc->bus, pFunc->device, pFunc->function,
                irqValue, 0, 0 );
#endif
    return (irqValue);
    }
