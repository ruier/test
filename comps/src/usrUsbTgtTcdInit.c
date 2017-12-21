/* usrUsbTgtTcdInit.c - Initialization of the USB TCD drivers */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,16may12,s_z  Add the PLX USB 3.0 TCD support (WIND00306135)
01e,21apr11,s_z  Correct the description of vxbUsbTcdUnRegister routine
01d,22mar11,s_z  Changes for unused routines removed
01c,09mar11,s_z  Code clean up
01b,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01a,12oct10,s_z  created
*/

/*
DESCRIPTION

This file includes the defination and the routines to configure and initialize
the USB target controller drivers.

INCLUDE FILES: usb/usb.h, usb/usbTgt.h
*/

#ifndef __INCusrUsbTgtTcdInitc
#define __INCusrUsbTgtTcdInitc

/* includes */

#include <usb/usb.h>
#include <usb/usbTgt.h>

/* Include the source files if needed */

#ifdef INCLUDE_USB_MHDRC_TCD
#include "usrUsbMhdrcTcdInit.c"
#endif /* INCLUDE_USB_MHDRC_TCD */

#ifdef INCLUDE_USB_PLX_TCD
#include "usrUsbPlxTcdInit.c"
#endif /* INCLUDE_USB_PLX_TCD */


/*******************************************************************************
*
* vxbUsbTcdUnRegister - unregister the usb target fucntion drivers from vxBus
*
* This routine is used to unregister the usb target fucntion drivers from vxBus
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void vxbUsbTcdUnRegister
    (
    void
    )
    {
    
#if defined(INCLUDE_USB_MHDRC_TCD) || defined(INCLUDE_USB_MHDRC_OCD_TCD)

    usbVxbMhdrcTcdDeregister();

#endif /* INCLUDE_USB_MHDRC_TCD || ...*/

#if defined(INCLUDE_USB_PLX_TCD)

    usbPlxTcdVxbDeregister();

#endif /* INCLUDE_USB_PLX_TCD */

    return;
    }

/*******************************************************************************
*
* vxbUsbTcdRegister - register the usb target fucntion drivers to vxBus
*
* This routine is used to register the usb target fucntion drivers to vxBus
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void vxbUsbTcdRegister
    (
    void
    )
    {
    
#if defined(INCLUDE_USB_MHDRC_TCD) || defined(INCLUDE_USB_MHDRC_OCD_TCD)

    usbVxbMhdrcTcdRegister();

#endif /* INCLUDE_USB_MHDRC_TCD || ... */

#if defined(INCLUDE_USB_PLX_TCD)

    usbPlxTcdVxbRegister();

#endif /* INCLUDE_USB_PLX_TCD */

    return;
    }

/*****************************************************************************
*
* usbTcdInstantiate - instantiate the USB Target Controller Driver
*
* This routine instantiates the USB Target Controller Driver and allows
* the controller driver to be included with the vxWorks image and
* not be registered with vxBus. Devices will remain orphan devices
* until the Host Controller Init routine is called
*
* The routine call Target Controller's instantiate routine, itself does nothing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbTcdInstantiate
    (
    void
    )
    {
#ifdef INCLUDE_USB_MHDRC_TCD

#endif
    return;
    }

#endif /* __INCusrUsbTgtTcdInitc */
