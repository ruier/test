/* usrUsn2EndInit.c - Generation 2 USB-Ethernet class driver configlette file */

/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,01sep10,ghs  Change default ip address (WIND00231161)
01c,13jan10,ghs  vxWorks 6.9 LP64 adapting
01b,02jul09,s_z  move the hardware related initialization routine to this file
01a,12nov08,s_z  written

*/
#ifndef __INCusrUsb2EndInitc
#define __INCusrUsb2EndInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <usb2/usb2End.h>

#ifdef INCLUDE_USB_GEN2_PEGASUS
#include <usb2/usb2Pgs.h>
#endif

#ifdef INCLUDE_USB_GEN2_DM960X
#include <usb2/usb2Dm960x.h>
#endif

#ifdef INCLUDE_USB_GEN2_ASIX
#include <usb2/usb2Asix.h>
#endif

/* defines */

/* Default device name */
#ifndef USB_GEN2_END_NAME
#define USB_GEN2_END_NAME                  "usb2End"
#endif

/* Default IP address and netmask */

/* TODO: to support more pegasus devices */


#ifndef USB_GEN2_END_IP_ADDRESS
#define USB_GEN2_END_IP_ADDRESS           {"90.0.0.50"}
#endif

#ifndef USB_GEN2_END_NET_MASK
#define USB_GEN2_END_NET_MASK             {0xFFFF0000}
#endif

#ifndef USB_GEN2_END_COMMON_TASK_PRIORITY
#define USB_GEN2_END_COMMON_TASK_PRIORITY (100)
#endif

#ifndef USB_GEN2_END_MAX_DEVS
#define USB_GEN2_END_MAX_DEVS             (1)
#endif

/*
 * Define the globles for the parameters defined by the workbench or
 * the configuration, which will be used by usb2End.c
 */

const char * g_UsbGen2EndName          = USB_GEN2_END_NAME;
const INT8   g_UsbGen2EndNameLen       = (sizeof(USB_GEN2_END_NAME) + 1 );

const char * g_UsbGen2EndIpAddress[USB_GEN2_END_MAX_DEVS] = USB_GEN2_END_IP_ADDRESS;

const UINT32 g_UsbGen2EndNetMask[USB_GEN2_END_MAX_DEVS] = USB_GEN2_END_NET_MASK;

const INT8   g_UsbGen2EndMaxDevs = USB_GEN2_END_MAX_DEVS;

const UINT8  g_UsbGen2EndCommonTaskPriority  = USB_GEN2_END_COMMON_TASK_PRIORITY;

/*******************************************************************************
*
* usrUsb2PgsInit - initializes the GEN2 Usb-Ehternet class driver.
*
* This function is initializing the GEN2 Usb-Ehternet class driver.
* If called automatically by the project facility
*
* RETURNS: If the Usb-Ehternet driver could be initialized or not
*
* ERRNO: N/A
*/

STATUS usrUsb2EndInit()
    {

    #ifdef INCLUDE_USB_GEN2_PEGASUS
    usb2PgsInit(NULL);
    #endif

    #ifdef INCLUDE_USB_GEN2_DM960X
    usb2Dm960xInit(NULL);
    #endif

    #ifdef INCLUDE_USB_GEN2_ASIX
    usb2AsixInit(NULL);
    #endif

    return usb2EndInit(USB_GEN2_END_NAME);
    }


#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2EndInitc */





