/* usrUsbPlxTcdnit.c - Initialization of the USB PLX TCD drivers */

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
01b,03sep12,s_z  Change USB_PLX_DMA_ALWAYS_SHORT_PACKET as TRUE
01a,12jul12,s_z  Writen
*/

/*
DESCRIPTION

This file includes 
*/

#ifndef __INCusrUsbPlxTcdInitc
#define __INCusrUsbPlxTcdInitc

/* includes */

#include <usb/usb.h>
#include <usb/usbTgt.h>


#define USB_PLX_DMA_ENABLE 

#ifdef USB_PLX_DMA_ENABLE
#define USB_PLX_SINGLE_DMA_TYPE (0x1)
#define USB_PLX_SG_DMA_TYPE     (0x2)
#define USB_PLX_DMA_TYPE        USB_PLX_SINGLE_DMA_TYPE
#else
#define USB_PLX_DMA_TYPE 0
#endif /* USB_PLX_DMA_ENABLE */


/*
 * If the controller will always receive/transfer short packets? 
 * Set this value as TRUE.
 *
 * For example, when the function driver is RNDIS, the conroller
 * may receive many random packets with the packet size less than
 * (1514 + 60) bytes. This will break the DMA's way to recognize
 * the received packet size. Since the controller may receive multiple
 * packets in its FIFO, but can not detect each packet's size.
 */
 
#define USB_PLX_DMA_ALWAYS_SHORT_PACKET  {(TRUE)}

/*
 * From the internal testing, the PLX USB3380/USB3382 controller 
 * can not pass the USB3 training when plugged in the USB host.
 * It will be reset repeatly and can not enum by the USB host with a 
 * differnet connection.
 * 
 * For example, USB3380 controller works fine when directly connect
 * to the USB host, but will reset repeatly when connect to USB host over 
 * one USB anylizer. 
 * 
 */
 
/* Enable auto polarity */

#define USB_PLX_TCD_AUTO_POLARITY_ENABLE    (0)
#define USB_PLX_TCD_AUTO_POLARITY_DISABLE   (1)

#define USB_PLX_TCD_AUTO_POLARITY_SET       (USB_PLX_TCD_AUTO_POLARITY_DISABLE)


#   ifndef USB_PLX_TCD_COUNT
#   define USB_PLX_TCD_COUNT    1
#   endif


/* 
 * In the design, one usb controller will emulate one usb device.
 * One usb device can have more than one configuration and each configuration
 * can have more than one interfaces. But one usb device will only have one 
 * VID/PID and one device describing string. 
 */

#   ifndef USB_PLX_TCD_VENDOR_ID
#   define USB_PLX_TCD_VENDOR_ID     {0x1439}  /* Wind River Systems */
#   endif

#   ifndef USB_PLX_TCD_PRODUCT_ID
#   define USB_PLX_TCD_PRODUCT_ID    {0xFF00} /* PID for Emulator from 0xFF00 */
#   endif

#   ifndef USB_PLX_TCD_BCD_DEVICE
#   define USB_PLX_TCD_BCD_DEVICE    {0x0200} /* Bcd Device */
#   endif

#   ifndef USB_PLX_TCD_MFG_STRING
#   define USB_PLX_TCD_MFG_STRING    {"Wind River Systems"}
#   endif

#   ifndef USB_PLX_TCD_PROD_STRING
#   define USB_PLX_TCD_PROD_STRING   {"USB PLX Target Device"}
#   endif

#   ifndef USB_PLX_TCD_SERIAL_STRING
#   define USB_PLX_TCD_SERIAL_STRING {"0123456789012"} 
#   endif

/* This should be the name which need be attached by the function drivers */

#   ifndef USB_PLX_TCD_NAME
#   define USB_PLX_TCD_NAME          {"/usbTgt"}
#   endif

#   ifndef USB_PLX_CONFIGURATION_NUM
#   define USB_PLX_CONFIGURATION_NUM {1}
#   endif

#   ifndef USB_PLX_ISR_TASK_PRIORITY
#   define USB_PLX_ISR_TASK_PRIORITY {50}
#   endif

UINT8  gUsbPlxTcdCount    = USB_PLX_TCD_COUNT;
UINT16 gUsbPlxTcdVid[] =USB_PLX_TCD_VENDOR_ID;
UINT16 gUsbPlxTcdPid[] = USB_PLX_TCD_PRODUCT_ID;
UINT16 gUsbPlxTcdBcdDevice[] = USB_PLX_TCD_BCD_DEVICE;
char * gUsbPlxTcdMfgString[] = USB_PLX_TCD_MFG_STRING;
char * gUsbPlxTcdProdString[] = USB_PLX_TCD_PROD_STRING;
char * gUsbPlxTcdSerialString[] = USB_PLX_TCD_SERIAL_STRING;
char * gUsbPlxTcdName[] = USB_PLX_TCD_NAME;
UINT8  gUsbPlxTcdConfigNum[] = USB_PLX_CONFIGURATION_NUM;
UINT8  gUsbPlxTcdIsrPriority[] = USB_PLX_ISR_TASK_PRIORITY;
UINT8  gUsbPlxTcdDmaType = USB_PLX_DMA_TYPE;
UINT8  gUsbPlxIsrTaskFlag[5] = {0, 0, 0, 0, 0};
UINT8  gUsbPlxTcdAlwaysShortPkt[] = USB_PLX_DMA_ALWAYS_SHORT_PACKET;


extern STATUS usbPlxTcdVxbDeregister 
    (
    void
    );

extern void usbPlxTcdVxbRegister
    (
    void
    );

/*******************************************************************************
*
* usrUsbPlxTcdConfig - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbPlxTcdInit 
    (
    void
    )
    {
    usbPlxTcdVxbRegister();   
    
    return;
    }

/*******************************************************************************
*
* usrUsbPlxTcdUnInit - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbPlxTcdUnInit 
    (
    void
    )
    {
    usbPlxTcdVxbDeregister();  
    
    return;
    }

/*******************************************************************************
*
* usrUsbPlxTcdIsrTaskFlagSet - set the isr task flag
*
* This routine is used to set the isr task flag. 
* 
* <uIndex> - the controller instance index, 0 based
* <flag>   - 0 : normal process, 1 : ignore irqs
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbPlxTcdIsrTaskFlagSet 
    (
    UINT8  uIndex, /* The contorller instance index */
    UINT8  uFlag   /* 0 - normal, 1.- irq ignore */
    )
    {
    gUsbPlxIsrTaskFlag[uIndex] = uFlag;  

    return;
    }

/*******************************************************************************
*
* usrUsbPlxTcdIsrTaskFlagGet - get the isr task flag
*
* This routine is used to get the isr task flag. 
* 
* <uIndex> - the controller instance index, 0 based
*
* RETURNS: flag value, 0 : normal process, 1 : ignore irqs
*
* ERRNO: N/A
*/

UINT8 usrUsbPlxTcdIsrTaskFlagGet 
    (
    UINT8  uIndex /* The contorller instance index */
    )
    {
    return gUsbPlxIsrTaskFlag[uIndex];  
    }

/*******************************************************************************
*
* usrUsbPlxTcdParameterSet - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: OK/ERROR.
*
* ERRNO: N/A
*/

void usrUsbPlxTcdParameterSet
    (
    UINT32 uType,
    UINT8  uIndex,
    void * pValue
    )
    {
    /* The use of the routine */

#ifdef INCLUDE_USB_PLX_TCD 

#define USB_PLX_TCD_PARAM_TYPE_COUNT         0x1
#define USB_PLX_TCD_PARAM_TYPE_VENDOR_ID     0x2
#define USB_PLX_TCD_PARAM_TYPE_PRODUCT_ID    0x3
#define USB_PLX_TCD_PARAM_TYPE_MFG_STRING    0x4
#define USB_PLX_TCD_PARAM_TYPE_PROD_STRING   0x5
#define USB_PLX_TCD_PARAM_TYPE_SERIAL_STRING 0x6
#define USB_PLX_TCD_PARAM_TYPE_TCD_NAME      0x7

    printf ("Set USB_PLX TCD parameters : \n"
            "uType    - USB_PLX_TCD_PARAM_TYPE\n"
            "         : 0x1 - USB_PLX TCD count\n"
            "         : 0x2 - USB_PLX TCD VENDOR_ID\n"
            "         : 0x3 - USB_PLX TCD PRODUCT_ID\n"
            "         : 0x4 - USB_PLX TCD MFG_STRING\n"
            "         : 0x5 - USB_PLX TCD PROD_STRING\n"
            "         : 0x6 - USB_PLX TCD SERIAL_STRING\n"
            "         : 0x7 - USB_PLX TCD TCD_NAME\n"
            "uIndex   - Parameter Index\n"
            "uValue   - The value to be set\n" );


    {
    switch (uType)
        {
        case USB_PLX_TCD_PARAM_TYPE_COUNT:
            gUsbPlxTcdCount = *((UINT8 *)pValue);
            break;
        case USB_PLX_TCD_PARAM_TYPE_VENDOR_ID:
            gUsbPlxTcdVid[uIndex] = *(UINT16 *)pValue;
            break;
        case USB_PLX_TCD_PARAM_TYPE_PRODUCT_ID:
            gUsbPlxTcdPid[uIndex] = *(UINT16 *)pValue;
            break;
        case USB_PLX_TCD_PARAM_TYPE_MFG_STRING:
            gUsbPlxTcdMfgString[uIndex] = (char *)pValue;
            break;
        case USB_PLX_TCD_PARAM_TYPE_PROD_STRING:
            gUsbPlxTcdProdString[uIndex] = (char *)pValue;
            break;
        case USB_PLX_TCD_PARAM_TYPE_SERIAL_STRING:
            gUsbPlxTcdSerialString[uIndex] = (char *)pValue;
            break;
        case USB_PLX_TCD_PARAM_TYPE_TCD_NAME:
            gUsbPlxTcdName[uIndex] = (char *)pValue;
            break;
        default:                
            break;
        }

    }
#endif /* INCLUDE_USB_PLX_TCD */
    }

#endif /* __INCusrUsbPlxTcdInitc */

