/* usbPciStub.c - system-specific USB functions */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,01mar12,syt  fixed compile warnings.
01a,18jul11,b_m  written.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"

/* externs */

IMPORT void   sysWrite32 (UINT32 address, UINT32 value);

/*******************************************************************************
*
* sysUsbInit - initialize the on-chip USB controllers
*
* This routine initializes the on-chip USB controllers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysUsbInit (void)
    {
    /*
     * USB0: Synopsis OTG
     *
     * Note: the OTG port reset should be done at the beginning,
     *       otherwise USB1 & USB2 registers will all read as 0.
     */

    sysWrite32(USB0_RST_CTL, USB_PORT_RESET | USB_POR);
    sysWrite32(USB0_CLK_CTL, USB0_CLK_48MHZ);
    sysWrite32(USB0_RST_CTL, 0);
    sysWrite32(USB0_TUNE, USB_TUNE_VALUE);
    
    /* USB1: OHCI & EHCI host */

    sysWrite32(USB1_RST_CTL, USB_PORT_RESET | USB_POR);
    sysWrite32(USB1_CLK_CTL, USB_CLK_48MHZ);
    sysWrite32(USB1_RST_CTL, 0);
    sysWrite32(USB1_CFG, USB_CFG_VALUE);
    sysWrite32(USB1_TUNE, USB_TUNE_VALUE);

    /* USB2: OHCI & EHCI host */

    sysWrite32(USB2_RST_CTL, USB_PORT_RESET | USB_POR);
    sysWrite32(USB2_CLK_CTL, USB_CLK_48MHZ);
    sysWrite32(USB2_RST_CTL, 0);
    sysWrite32(USB2_CFG, USB_CFG_VALUE);
    sysWrite32(USB2_TUNE, USB_TUNE_VALUE);
    }
