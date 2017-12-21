/* usrUsbTgtPrnInit.c - Initialization of the printer function driver */

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
01b,05aug11,ljg  Modify Capabilities String (WIND00291584)
01a,21feb11,ljg  Created. 
*/

/*
DESCRIPTION

This file contains the initialization function for the printer function driver
on the peripheral.

INCLUDE FILES: stdio.h string.h usb/usbPlatform.h usb/usbLib.h usb/usbTgt.h 
               usb/usbTgtPrn.h

*/

#ifndef __INCusrUsbTgtPrnInitc
#define __INCusrUsbTgtPrnInitc

/* includes */

#include <stdio.h>
#include <string.h>
#include <usb/usbPlatform.h>
#include <usb/usbLib.h>
#include <usb/usbTgt.h>
#include <usb/usbTgtPrn.h>         

#ifdef __cplusplus
extern "C" {
#endif

/* Selection 1*/

#if defined(INCLUDE_USBTGT_PRN_1)

#ifndef USBTGT_PRN_ATTACH_TCD_NAME1
#   define USBTGT_PRN_ATTACH_TCD_NAME1     {"/usbTgt"}
#endif

#ifndef USBTGT_PRN_ATTACH_TCD_UNIT1
#   define USBTGT_PRN_ATTACH_TCD_UNIT1     0
#endif

#ifndef USBTGT_PRN_NAME1
#   define USBTGT_PRN_NAME1                "usbTgtPrn"
#endif

#ifndef USBTGT_PRN_UNIT1
#   define USBTGT_PRN_UNIT1                0
#endif

#ifndef USBTGT_PRN_CONFIG_NUM1
#   define USBTGT_PRN_CONFIG_NUM1          1
#endif

#ifndef USBTGT_PRN_CAPS1                   /* Capabilities String */
#   define USBTGT_PRN_CAPS1                "MFG:WRS;CMD:PS,BDC,D4;MDL:Emulator USB;CLS:PRINTER;DES:WRS Emulator USB;"
#endif

#endif /* INCLUDE_USBTGT_PRN_1 */

/* Selection 2*/

#if defined(INCLUDE_USBTGT_PRN_2)

#ifndef USBTGT_PRN_ATTACH_TCD_NAME2
#   define USBTGT_PRN_ATTACH_TCD_NAME2     {"/usbTgt"}
#endif

#ifndef USBTGT_PRN_ATTACH_TCD_UNIT2
#   define USBTGT_PRN_ATTACH_TCD_UNIT2     0
#endif

#ifndef USBTGT_PRN_NAME2
#   define USBTGT_PRN_NAME2                "usbTgtPrn"
#endif

#ifndef USBTGT_PRN_UNIT2
#   define USBTGT_PRN_UNIT2                1
#endif

#ifndef USBTGT_PRN_CONFIG_NUM2
#   define USBTGT_PRN_CONFIG_NUM2          1
#endif

#ifndef USBTGT_PRN_CAPS2                   /* Capabilities String */
#   define USBTGT_PRN_CAPS2                "MFG:WRS;CMD:PS,BDC,D4;MDL:Emulator USB;CLS:PRINTER;DES:WRS Emulator USB;" 
#endif

#endif /* INCLUDE_USBTGT_PRN_2 */

USR_USBTGT_PRN_CONFIG usrUsbTgtPrnConfigTable[] =
    {
#if defined (INCLUDE_USBTGT_PRN_1)
        {
        USBTGT_PRN_ATTACH_TCD_NAME1,      /* tcdName */ 
        USBTGT_PRN_ATTACH_TCD_UNIT1,      /* tcdUnit */ 
        USBTGT_PRN_NAME1,                 /* PrnName */ 
        USBTGT_PRN_UNIT1,                 /* PrnUnit */
        USBTGT_PRN_CONFIG_NUM1,           /* uAimConfigNum */
        USBTGT_PRN_CAPS1
        },
#endif /* INCLUDE_USBTGT_PRN_1 */

#if defined (INCLUDE_USBTGT_PRN_2)
        {
        USBTGT_PRN_ATTACH_TCD_NAME2,      /* tcdName */ 
        USBTGT_PRN_ATTACH_TCD_UNIT2,      /* tcdUnit */ 
        USBTGT_PRN_NAME2,                 /* PrnName */ 
        USBTGT_PRN_UNIT2,                 /* PrnUnit */
        USBTGT_PRN_CONFIG_NUM2,           /* uAimConfigNum */
        USBTGT_PRN_CAPS2
        },
#endif /* INCLUDE_USBTGT_PRN_2 */
    }; 

/*******************************************************************************
*
* usrUsbTgtPrnInit - initialize the printer function drivers
*
* This routine intializes the printer fucntion drivers by calling the
* usbTgtPrnInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtPrnInit
    (
    void
    )
    {
    
    return usbTgtPrnInit(USBTGT_PRN_COUNT);
   
    }

/*******************************************************************************
*
* usrUsbTgtPrnUnInit - uninitialize the printer function drivers
*
* This routine unintializes the printer fucntion drivers by calling the
* usbTgtPrnUnInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS  usrUsbTgtPrnUnInit 
    (
    void
    )
    {

    return usbTgtPrnUnInit();

    }

#ifdef __cplusplus
}
#endif

#endif  /* __INCusrUsbTgtPrnInitc */

