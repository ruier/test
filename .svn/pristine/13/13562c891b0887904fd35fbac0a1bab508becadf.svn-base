/* usbPciStub.c - system specific USB functions */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,11jul12,d_l  written based on fsl_p5020_ds/01b.
*/

#include <vxWorks.h>
#include "config.h"
#include <usb/usbPlatform.h>
#include <usb/pciConstants.h>

/* USB register base addresses */

LOCAL UINT32 usbPhyBase[]  = { USB1_PHY_BASE, USB2_PHY_BASE };
LOCAL UINT32 usbEhciBase[] = { USB1_BASE,     USB2_BASE     };

/*******************************************************************************
*
* ehciInit - initialize the on-chip EHCI controller
*
* This routine initializes the on-chip EHCI controller. It uses default UTMI 
* PHY.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciInit
    (
    int unit
    )
    {
    /* enable usb phy */

    *(volatile UINT32 *)USB_PHY_EOR(usbPhyBase[unit]) = USB_PHY_EN_OVERRIDE;

    /* snoop size 4 GB */

    *(volatile UINT32 *)EHCI_SNOOP1(usbEhciBase[unit]) = EHCI_SNOOP_SIZE_2G;
    *(volatile UINT32 *)EHCI_SNOOP2(usbEhciBase[unit]) = 0x80000000 | EHCI_SNOOP_SIZE_2G;

    /* recommended priority and age count threshold */

    *(volatile UINT32 *)EHCI_PRI_CTRL(usbEhciBase[unit]) = EHCI_PRI_EN;
    *(volatile UINT32 *)EHCI_AGE_CNT_THRESH(usbEhciBase[unit]) = 0x00000080;

    /* rd prefetch 32 byte base on burst size 8 x 4 */

    *(volatile UINT32 *)EHCI_SI_CTRL(usbEhciBase[unit]) = 0x00000001;

    /* enable the usb interface */

    *(volatile UINT32 *)EHCI_CONTROL(usbEhciBase[unit]) = (EHCI_CONTROL_UTMI_PHY_EN |
                                                   EHCI_CONTROL_USB_EN);
    }

/*******************************************************************************
*
* ehciPostResetHook - post reset hook of the on-chip EHCI controller
*
* This routine sets the on-chip EHCI controller to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciPostResetHook
    (
    int unit
    )
    {
    *(volatile UINT32 *)EHCI_USBMODE(usbEhciBase[unit]) |= LONGSWAP(EHCI_USBMODE_HOST);
    }

/*******************************************************************************
*
* ehci0Init - initialize the on-chip EHCI controller (USB1)
*
* This routine initializes the on-chip EHCI controller (USB1).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0Init (void)
    {
    ehciInit(0);
    }

/*******************************************************************************
*
* ehci0PostResetHook - post reset hook of the on-chip EHCI controller (USB1)
*
* This routine sets the on-chip EHCI controller (USB1) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0PostResetHook (void)
    {
    ehciPostResetHook(0);
    }

/*******************************************************************************
*
* ehci1Init - initialize the on-chip EHCI controller (USB2)
*
* This routine initializes the on-chip EHCI controller (USB2).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1Init (void)
    {
    ehciInit(1);
    }

/*******************************************************************************
*
* ehci1PostResetHook - post reset hook of the on-chip EHCI controller (USB2)
*
* This routine sets the on-chip EHCI controller (USB2) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1PostResetHook (void)
    {
    ehciPostResetHook(1);
    }

