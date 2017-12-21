/* usrUsbMhdrcTcdnit.c - Initialization of the USB MHDRC TCD drivers */

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
01f,15oct12,s_z  Add USBTGT_TCD_MAX_CONFIG_COUNT (WIND00382057)
01e,22mar11,s_z  Changes for unused routines removed
01d,11mar11,s_z  Code clean up 
01c,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01b,22feb11,w_x  updated string descriptors to match cdf contents
01a,12jul10,s_z  created
*/

/*
DESCRIPTION

This file includes 
*/

#ifndef __INCusrUsbMhdrcTcdInitc
#define __INCusrUsbMhdrcTcdInitc

/* includes */

#include <usb/usb.h>
#include <usb/usbTgt.h>

#   ifndef USB_MHDRC_TCD_COUNT
#   define USB_MHDRC_TCD_COUNT    1
#   endif


/* 
 * In the design, one usb controller will emulate one usb device.
 * One usb device can have more than one configuration and each configuration
 * can have more than one interfaces. But one usb device will only have one 
 * VID/PID and one device describing string. 
 */

#   ifndef USB_MHDRC_TCD_VENDOR_ID
#   define USB_MHDRC_TCD_VENDOR_ID     {0x1439}  /* Wind River Systems */
#   endif

#   ifndef USB_MHDRC_TCD_PRODUCT_ID
#   define USB_MHDRC_TCD_PRODUCT_ID    {0xFF00} /* PID for Emulator from 0xFF00 */
#   endif

#   ifndef USB_MHDRC_TCD_BCD_DEVICE
#   define USB_MHDRC_TCD_BCD_DEVICE    {0x0200} /* Bcd Device */
#   endif

#   ifndef USB_MHDRC_TCD_MFG_STRING
#   define USB_MHDRC_TCD_MFG_STRING    {"Wind River Systems"}
#   endif

#   ifndef USB_MHDRC_TCD_PROD_STRING
#   define USB_MHDRC_TCD_PROD_STRING   {"USB MHDRC Target Device"}
#   endif

#   ifndef USB_MHDRC_TCD_SERIAL_STRING
#   define USB_MHDRC_TCD_SERIAL_STRING {"0123456789012"} 
#   endif

/* This should be the name which need be attached by the function drivers */

#   ifndef USB_MHDRC_TCD_NAME
#   define USB_MHDRC_TCD_NAME          {"/usbTgt"}
#   endif


UINT8  gUsbMhcrcTcdCount    = USB_MHDRC_TCD_COUNT;
UINT16 gUsbMhdrcTcdVid[] =USB_MHDRC_TCD_VENDOR_ID;
UINT16 gUsbMhdrcTcdPid[] = USB_MHDRC_TCD_PRODUCT_ID;
UINT16 gUsbMhdrcTcdBcdDevice[] = USB_MHDRC_TCD_BCD_DEVICE;
char * gUsbMhdrcTcdMfgString[] = USB_MHDRC_TCD_MFG_STRING;
char * gUsbMhdrcTcdProdString[] = USB_MHDRC_TCD_PROD_STRING;
char * gUsbMhdrcTcdSerialString[] = USB_MHDRC_TCD_SERIAL_STRING;
char * gUsbMhdrcTcdName[] = USB_MHDRC_TCD_NAME;

extern void usbVxbMhdrcTcdRegister (void);
extern STATUS usbVxbMhdrcTcdDeregister (void);

UINT16 usbTgtMhdrcTcdVidGet
    (
    UINT8 uTcdUnin
    )
    {
    if (sizeof (gUsbMhdrcTcdVid) / sizeof (UINT16) <= uTcdUnin)
        {
        return gUsbMhdrcTcdVid[0];
        }
    else
        {
        return gUsbMhdrcTcdVid[uTcdUnin];
        }
    }

UINT16 usbTgtMhdrcTcdVidSet
    (
    UINT8 uTcdUnin
    )
    {

    return 0x0000;

    }

/*******************************************************************************
*
* usrUsbTgtMhdrcTcdConfig - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: OK/ERROR.
*
* ERRNO: N/A
*/
STATUS usrUsbTgtMhdrcTcdInit 
    (
    void
    )
    {
    usbVxbMhdrcTcdRegister();   

    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtMhdrcTcdUnInit - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: OK/ERROR.
*
* ERRNO: N/A
*/
STATUS usrUsbTgtMhdrcTcdUnInit 
    (
    void
    )
    {
    usbVxbMhdrcTcdDeregister();  
    
    return OK;
    }


/*******************************************************************************
*
* usrUsbTgtConfig - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: OK/ERROR.
*
* ERRNO: N/A
*/

void usrUsbTgtParameterSet
    (
    UINT32 uType,
    UINT8  uSubType,
    UINT8  uIndex,
    void * pValue
    )
    {
    /* The use of the routine */

#ifdef INCLUDE_USB_MHDRC_TCD 

#define MHDRC_TCD_PARAM_SUBTYPE_COUNT         0x1
#define MHDRC_TCD_PARAM_SUBTYPE_VENDOR_ID     0x2
#define MHDRC_TCD_PARAM_SUBTYPE_PRODUCT_ID    0x3
#define MHDRC_TCD_PARAM_SUBTYPE_MFG_STRING    0x4
#define MHDRC_TCD_PARAM_SUBTYPE_PROD_STRING   0x5
#define MHDRC_TCD_PARAM_SUBTYPE_SERIAL_STRING 0x6
#define MHDRC_TCD_PARAM_SUBTYPE_TCD_NAME      0x7

    printf ("Set MHDRC TCD parameters : \n"
            "uType    - USB_MHDRC_TCD_PARAM_TYPE()\n"
            "uSubType - MHDRC TCD count\n"
            "         : 0x1 - MHDRC TCD count\n"
            "         : 0x2 - MHDRC TCD VENDOR_ID\n"
            "         : 0x3 - MHDRC TCD PRODUCT_ID\n"
            "         : 0x4 - MHDRC TCD MFG_STRING\n"
            "         : 0x5 - MHDRC TCD PROD_STRING\n"
            "         : 0x6 - MHDRC TCD SERIAL_STRING\n"
            "         : 0x7 - MHDRC TCD TCD_NAME\n"
            "uIndex   - Parameter Index\n"
            "uValue   - The value to be set\n" );


    {
    switch (uSubType)
        {
        case MHDRC_TCD_PARAM_SUBTYPE_COUNT:
            gUsbMhcrcTcdCount = *((UINT8 *)pValue);
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_VENDOR_ID:
            gUsbMhdrcTcdVid[uIndex] = *(UINT16 *)pValue;
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_PRODUCT_ID:
            gUsbMhdrcTcdPid[uIndex] = *(UINT16 *)pValue;
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_MFG_STRING:
            gUsbMhdrcTcdMfgString[uIndex] = (char *)pValue;
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_PROD_STRING:
            gUsbMhdrcTcdProdString[uIndex] = (char *)pValue;
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_SERIAL_STRING:
            gUsbMhdrcTcdSerialString[uIndex] = (char *)pValue;
            break;
        case MHDRC_TCD_PARAM_SUBTYPE_TCD_NAME:
            gUsbMhdrcTcdName[uIndex] = (char *)pValue;
            break;
        default:                
            break;
        }

    }
#endif /* INCLUDE_USB_MHDRC_TCD */

    }

#endif /* __INCusrUsbMhdrcTcdInitc */
