/* usrUsbTgtKbdInit.c - Initialization of the keyboard function driver */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01a,11apr11,ljg  Created. 
*/

/*
DESCRIPTION

This file contains the initialization function for the keyboard function driver
on the peripheral side.

INCLUDE FILES: stdio.h string.h usb/usbPlatform.h usb/usbLib.h usb/usbTgt.h
               usb/usbTgtKbd.h

*/

#ifndef __INCusrUsbTgtKbdInitc
#define __INCusrUsbTgtKbdInitc

/* includes */

#include <stdio.h>
#include <string.h>
#include <usb/usbPlatform.h>
#include <usb/usbLib.h>
#include <usb/usbTgt.h>
#include <usb/usbTgtKbd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Selection 1*/

#if defined(INCLUDE_USBTGT_KBD_1)

#ifndef USBTGT_KBD_ATTACH_TCD_NAME1
#   define USBTGT_KBD_ATTACH_TCD_NAME1             {"/usbTgt"}
#endif

#ifndef USBTGT_KBD_ATTACH_TCD_UNIT1
#   define USBTGT_KBD_ATTACH_TCD_UNIT1             0
#endif

#ifndef USBTGT_KBD_CONFIG_NUM1
#   define USBTGT_KBD_CONFIG_NUM1                  1
#endif

#ifndef USBTGT_KBD_NAME1
#   define USBTGT_KBD_NAME1                        "usbTgtKbd"
#endif

#ifndef USBTGT_KBD_UNIT1
#   define USBTGT_KBD_UNIT1                        0
#endif

#endif /* INCLUDE_USBTGT_KBD_1 */

/* Selection 2 */

#if defined(INCLUDE_USBTGT_KBD_2)

#ifndef USBTGT_KBD_ATTACH_TCD_NAME2
#   define USBTGT_KBD_ATTACH_TCD_NAME2             {"/usbTgt"}
#endif

#ifndef USBTGT_KBD_ATTACH_TCD_UNIT2
#   define USBTGT_KBD_ATTACH_TCD_UNIT2             0
#endif

#ifndef USBTGT_KBD_CONFIG_NUM2
#   define USBTGT_KBD_CONFIG_NUM2                  1
#endif

#ifndef USBTGT_KBD_NAME2
#   define USBTGT_KBD_NAME2                        "usbTgtKbd"
#endif

#ifndef USBTGT_KBD_UNIT2
#   define USBTGT_KBD_UNIT2                        1
#endif

#endif /* INCLUDE_USBTGT_KBD_2 */

USR_USBTGT_KBD_CONFIG usrUsbTgtKbdConfigTable[] =
    {
#if defined (INCLUDE_USBTGT_KBD_1)
        {
        USBTGT_KBD_ATTACH_TCD_NAME1,        /* tcdName */ 
        USBTGT_KBD_ATTACH_TCD_UNIT1,        /* tcdUnit */ 
        USBTGT_KBD_NAME1,                   /* kbdName */ 
        USBTGT_KBD_UNIT1,                   /* kbdUnit */
        USBTGT_KBD_CONFIG_NUM1              /* uAimConfigNum */
        },
#endif /* INCLUDE_USBTGT_KBD_1 */

#if defined (INCLUDE_USBTGT_KBD_2)
        {
        USBTGT_KBD_ATTACH_TCD_NAME2,        /* tcdName */ 
        USBTGT_KBD_ATTACH_TCD_UNIT2,        /* tcdUnit */ 
        USBTGT_KBD_NAME2,                   /* kbdName */ 
        USBTGT_KBD_UNIT2,                   /* kbdUnit */
        USBTGT_KBD_CONFIG_NUM2              /* uAimConfigNum */
        },
#endif /* INCLUDE_USBTGT_KBD_2 */
    }; 

#define USBTGT_KBD_COUNT NELEMENTS(usrUsbTgtKbdConfigTable)

/*******************************************************************************
*
* usrUsbTgtKbdInit - initialize the keyboard function drivers
*
* This routine intializes the keyboard fucntion drivers by calling the
* usbTgtKbdInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtKbdInit
    (
    void
    )
    {

    return usbTgtKbdInit(USBTGT_KBD_COUNT);

    }


/*******************************************************************************
*
* usrUsbTgtKbdUnInit - uninitialize the keyboard function drivers
*
* This routine unintializes the keyboard fucntion drivers by calling the
* usbTgtKbdUnInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS  usrUsbTgtKbdUnInit 
    (
    void
    )
    {

    return usbTgtKbdUnInit();

    }

#ifdef __cplusplus
}
#endif

#endif  /* __INCusrUsbTgtKbdInitc */

