/* usrUsn2HelperInit.c - Generation 2 USB Helper file initialization */

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
01c,13jan10,ghs  vxWorks 6.9 LP64 adapting
01b,24apr09,w_x  added default macro definitions
01a,17nov08,adh  written
*/

#ifndef __INCusrUsb2HelperInitc
#define __INCusrUsb2HelperInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <usb2/usb2Helper.h>

#ifndef USB_GEN2_HELPER_LATENCY
#define USB_GEN2_HELPER_LATENCY                 1000
#endif /* USB_GEN2_HELPER_LATENCY */

#ifndef USB_GEN2_HELPER_PRIORITY
#define USB_GEN2_HELPER_PRIORITY                150
#endif /* USB_GEN2_HELPER_PRIORITY */

#ifndef USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE
#define USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE    100
#endif /* USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE */

/*******************************************************************************
*
* usrUsb2HelperInit - initialize the USB GEN2 class driver helper
*
* This routine initializes the USB GEN2 class driver helper.
*
* ERRNO: N/A
*/

STATUS usrUsb2HelperInit()
    {
    return usb2DeviceDeleteHelperInit(USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE,
            USB_GEN2_HELPER_PRIORITY,
            USB_GEN2_HELPER_LATENCY);
    }

#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2HelperInitc */
