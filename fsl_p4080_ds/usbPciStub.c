/* usbPciStub.c - system specific USB functions */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10jun09,syt  derived from version 01a of fsl_mds8536_ds/usbPciStub.c.
*/

#include <vxWorks.h>
#include "config.h"

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
    /* enable ports */

    *(volatile UINT32 *)EHCI_PORTSC(USB1_BASE) = LONGSWAP(EHCI_PORTSC_ULPI);

    /* recommended values if not setting repeat mode */

    *(volatile UINT32 *)EHCI_BURSTSIZE(USB1_BASE) = LONGSWAP(0x00000808);

    /* snoop size 2 GB */

    *(volatile UINT32 *)EHCI_SNOOP1(USB1_BASE) = EHCI_SNOOP_SIZE_2G;

    /* recommended priority and age count threshold */

    *(volatile UINT32 *)EHCI_PRI_CTRL(USB1_BASE) = 0x00000008;
    *(volatile UINT32 *)EHCI_AGE_CNT_THRESH(USB1_BASE) = 0x00000080;

    /* rd prefetch 32byte base on burst size 8 x 4 */

    *(volatile UINT32 *)EHCI_SI_CTRL(USB1_BASE) = 0x00000001;

    /* enable the usb interface */

    *(volatile UINT32 *)EHCI_CONTROL(USB1_BASE) = EHCI_CONTROL_USB_EN;
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
    *(volatile UINT32 *)EHCI_USBMODE(USB1_BASE) |= LONGSWAP(EHCI_USBMODE_HOST);
    }

