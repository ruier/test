/* usrUsbShow.c - USB configuration display routine (lsusb)  */

/*
 * Copyright (c) 2007, 2008, 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,28feb12,ljg  Add declaration of usrUsbWaitTimeOutValueGet (WIND00330641)
01j,11aug11,j_x  Use USB ossLib to allocate buffer (WIND00266669) 
01i,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01h,24jun10,m_y  Add valid check after allocate resource,
                 replace LOCALASSERT to if judgement (WIND00183499)
01g,05jul10,ghs  Fix conflict symbol defines(WIND00221550)
01f,24may10,m_y  Add parameter valid check,
                 modify URB timeout value for usb robustness (WIND00183499)
01e,18may10,w_x  Move some structures from usb2MscP.h to usb2Msc.h
01d,27apr10,j_x  Add GEN2 class driver show (WIND00184395)
01c,18mar10,j_x  Port to VxWorks6.9 (WIND00184542)
01b,32Jan08,jrp  Fixed endian problems.
01a,06Jul07,jrp  Created.
*/


/*
 DESCRIPTION

This file is used to implement the lsusb command that can be used to show
the vxWorks USB configuration.  This is loosely based on lsusb from unix.
The general is ->lsusb <verbosity>.  The verbosity is defined as follows:
   0 - minimal information.  Very close to lsusb
   1 - Dump the descriptors and list the port information
   2 - Same as "1" but includes port speed and other port status

INCLUDE FILES: vxWorks.h, stdio.h, string.h, ctype.h, logLib.h, usb/usbHst.h,
               usb/usbLib.h, usb/usbdMisc.h, usb/usbHubMisc.h, 
               ../src/hwif/busCtlr/usb/hub/usbHubCommon.h, usb/usbOsal.h
*/

#ifndef __INCusrUsbShowc
#define __INCusrUsbShowc

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "logLib.h"

#include <usb/usbHst.h>
#include <usb/usbLib.h>
#include <usb/usbdMisc.h>
#include <usb/usbHubMisc.h>
#include <../src/hwif/busCtlr/usb/hub/usbHubCommon.h>
#include <usb/usbOsal.h>

#ifdef INCLUDE_USB_GEN2_HELPER
#include <usb2/usb2Mse.h>
#include <usb2/usb2Kbd.h>
#include <usb2/usb2Prn.h>
#include <usb2/usb2End.h>
#include <usb2/usb2Msc.h>
#endif

/* defines */

#ifndef USB_SHOW_LEVEL
#define USB_SHOW_LEVEL            0
#endif /* OHCD debug information */

/*
 * An assert macro that cannot be defanged
 */
#define LOCALASSERT(condition)                                      \
    if ((condition) != TRUE)                                        \
    {                                                               \
    printf("\n Unexpected Error File %s Line %d",__FILE__,__LINE__);\
    exit(ERROR);                                                    \
    }                                                               \
/*
 * These should be in a header file somewhere
 */
#define USBHUB_NODETYPE_HUB      (0x02)
#define USBHUB_NODETYPE_DEVICE   (0x01)
#define USBHUB_NODETYPE_NONE     (0x00)
#define USBHST_MAX_DESCRIPTOR_LEN 256
/*
 * These definitions are from the USB 2.0 Specification Table 11.24.2.7.1
 */
#define M_PORT_CONNECTION 0x01
#define M_PORT_ENABLE 0x02
#define M_PORT_SUSPEND 0x04
#define M_PORT_OVER_CURRENT 0x08
#define M_PORT_RESET 0x10
#define M_PORT_POWER 0x100
#define M_PORT_LOW_SPEED 0x200
#define M_PORT_HIGH_SPEED 0x400
#define M_PORT_TEST 0x800
#define M_PORT_INDICATOR_CONTROL 0x1000

/* globals */

BOOL bDeviceDescriptor = FALSE;

/* imports */

IMPORT pUSB_HUB_BUS_INFO gpGlobalBus;

#ifdef INCLUDE_USB_GEN2_HELPER
IMPORT USB2_CLASS_DRIVER * pUsb2MseDriver;
IMPORT USB2_CLASS_DRIVER * pUsb2KbdDriver;
IMPORT USB2_CLASS_DRIVER * pUsb2PrnDriver;
IMPORT USB2_CLASS_DRIVER * pUsb2EndDriver;
IMPORT USB2_CLASS_DRIVER * pUsb2MscDriver;
#endif

IMPORT int usrUsbWaitTimeOutValueGet(void);

/* forward declarations */

#ifdef INCLUDE_USB_GEN2_HELPER

LOCAL void usbGen2GenericShow
    (
    USB2_CLASS_DRIVER * pUsb2ClassDriver
    );

LOCAL void usbDeviceInfoShow
    (
    FILE * pLogFile,
    USB2_CLASS_DEVICE * pDevice
    );

LOCAL void usbConfigInfoShow
    (
    FILE * pLogFile,
    UINT32 hDevice,
    UINT8  configIndex
    );

LOCAL void usbInterfaceInfoShow
    (
    FILE * pLogFile,
    UINT32 hDevice,
    USBHST_INTERFACE_DESCRIPTOR * pInterface
    );

LOCAL void usbHidInfoShow
    (
    FILE * pLogFile,
    UINT32 hDevice,
    USBHST_HID_DESCRIPTOR * pHid
    );

LOCAL void usbEndpointInfoShow
    (
    FILE * pLogFile,
    UINT32 hDevice,
    USBHST_ENDPOINT_DESCRIPTOR * pEndpoint
    );

#endif


/*
 * Debugging tunable from the shell
 */

volatile UINT32 usbShowDebug = 0;

#define DEBUGMSG(fmt,p1,p2,p3,p4,p5,p6)  if (usbShowDebug) \
        logMsg(fmt,p1,p2,p3,p4,p5,p6);

/*******************************************************************************
 *
 * usbHubGetInfoCallback - callback used when a request is made to the USBD
 *
 * This callback is called by the USBD when a request is complete.  This
 * was specified in the USBD Request Block (URB) that was filled when the
 * request was submited.
 *
 * For usbShow, this callback is only used during a request for the hub
 * port status (immediately below).  The callback is registered when the
 * USB Request Block (URB) is submitted to the USB stack.
 *
 * Parameters:
 *    pUrb - Pointer to the original URB.  (IN)
 *
 * RETURNS: USBHST_SUCCESS.
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL USBHST_STATUS usbHubGetInfoCallback
    (
    pUSBHST_URB pUrb
    )
    {
    OS_EVENT_ID * EventId;

    if ((NULL == pUrb) || (NULL == pUrb->pContext))
        {
        USB_HUB_ERR("Invalid Parameter\n", 1, 2, 3, 4, 5, 6);
        return USBHST_INVALID_PARAMETER;
        }

    /*
     * An OS_EVENT_ID is a wrapper for a vxWorks semaphore.  This is kept this
     * way to be consistent within the USB software.  The context pointer
     * was defined in the following routine to the be the semaphore ID to
     * be given upon completion of the USBD operation.
     */

    EventId = (OS_EVENT_ID * )pUrb->pContext;

    /*
     * Call OS_RELEASE_EVENT () translates directly to a semGive();
     */

    OS_RELEASE_EVENT(*EventId);

    /* Return USBHST_SUCCESS*/

    return USBHST_SUCCESS;
    }


/*******************************************************************************
 *
 * usbHubGetInfo - submit a blocking get port status request to the hub.
 *
 * This routine illustrates the method an application program communicates
 * with the USBD and ultimately the USB device.  This routine will submit
 * Get Port Status (See section 11.24.7 of USB Specification) request to the
 * specified Hub via the USBD.  Once the request is submitted, this routine
 * will pend on a semaphore until the callback routine (defined above and
 * specified in the submittal) fires and releases the semaphore.
 *
 * The data returned by the USBD is as defined by the USB specification and
 * is translated elsewhere
 *
 * Parameters:
 *   pHub - This is a pointer to internal data structure that represents the
 * USB hub that contains the port of interest.  This is obtained by the
 * getPortStatus () routine defined elsewhere.
 *
 *   wIndex - This corresponds to the wIndex field of the Get Port Status
 * request.  This is the defined the USB specification to be the port number on
 * the hub.  The specification requires that this be a non-zero number.
 *
 *   pBuffer - This points to a byte array to hold the returned values.
 *
 *   pBufferLength - The specification requires a buffer length of 4.  This
 * is checked and upon sucessful return, should contain a 4.  Looking at this
 * value is how we determine if the underlying USBD transaction was successful.
 *
 * Note that this function exists in slightly different form in the source.
 * However that function, as of vxWorks 6.5, was not part of the exported API.
 *
 * RETURNS: USBHST_SUCCESS, USBHST_FAILURE, USBHST_INVALID_PARAMETERS if
 * parameters are wrong.
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL USBHST_STATUS usbHubGetInfo
    (
    pUSB_HUB_INFO  pHub,
    UINT16         wIndex,
    UINT16 *       pBuffer,
    UINT8  *       pBufferLength
    )
    {
    /*
     * Hold the bmRequestType.  In this case it known to be 0xA3
     */

    UINT8 bmRequest;

    /*
     * EventId is the semaphore upon which this routine will pend while waiting
     * for the transaction to complete
     */

    OS_EVENT_ID EventId;

    /*
     * URB - USB Request Block.  This is the data structure submitted to the
     * USBD to perform the request.
     */

    USBHST_URB ControlURB;

    /*
     * Control transaction require a setup packet
     */

    pUSBHST_SETUP_PACKET pSetupPacket;

    /* This is to store the request result */

    USBHST_STATUS Result;

    /*
     * Validate the input parameters.  This is always a good idea, but is
     * even more important when submitting requests to the USBD.  The USBD
     * contains some error checking, but by its nature has trust the higher
     * level software to do the right things.
     *
     * If pHub  or pBuffer is NULL then return USBHST_INVALID_PARAMETER
     */

    if ((NULL == pHub) || (NULL == pBuffer))
        {
        /* Debug Message */

        DEBUGMSG("usbHubGetInfo: pHub is NULL \n", 0, 0, 0, 0, 0, 0);

        return USBHST_INVALID_PARAMETER;

        }/* End of (NULL == pHub */

     /*
      * The USB Specification requires a data buffer of 4 butes.
      */

    if (4 != *pBufferLength)
        {
        DEBUGMSG(
            "usbHubGetInfo: pBufferLength ==%d, not 4!\n",
            *pBufferLength, 0, 0, 0, 0, 0);

        /* Return USBHST_INVALID_PARAMETER */

        return USBHST_INVALID_PARAMETER;

        } /* End of if ( 4 != *pBufferLength) */

    /*
     * Call OS_CREATE_EVENT() to create an OS_EVENT_ID event. If this fails
     * return USBHST_FAILURE.  OS_CREATE_EVENT is a wrapper for a semxCreate()
     *
     * If the call fails there are some very serious problems.
     */

    EventId= OS_CREATE_EVENT(OS_EVENT_NON_SIGNALED);
    if (NULL == EventId)
        {
        DEBUGMSG("usbHubGetINfo: Event creation failed \n", 0, 0, 0, 0, 0, 0);

        return USBHST_FAILURE;

        }/* End of (NULL == EventId) */

    /*
     * Allocate memory for the setup packet. OS_MALLOC obtains it memory from
     * a special USB memory partition.
     */

    pSetupPacket = OS_MALLOC(sizeof(USBHST_SETUP_PACKET));

    /*
     * Once again, if this fails there are serious problems.
     */

    if (pSetupPacket == NULL)
        {
        DEBUGMSG(" Out of memory!!.\n", 0, 0, 0, 0, 0, 0);

        /*
         * Always try to clean up.  If the case of memory faults it is probably
         * futile, but try anyway.
         */

        OS_DESTROY_EVENT(EventId);
        return USBHST_INSUFFICIENT_MEMORY;
        }

    /*
     * Initialize the memory allocated
     */

    OS_MEMSET(pSetupPacket, 0, sizeof(USBHST_SETUP_PACKET));

    /*
     * Call USBHST_FILL_SETUP_PACKET() to populate the USBHST_SETUP_PACKET
     * structure. This will become part of the URB submitted to the
     */

    bmRequest = USB_PORT_TARGET_GET;
    USBHST_FILL_SETUP_PACKET(  (pSetupPacket),          /* pSetup */
                                bmRequest,              /* uRequestType */
                                USB_GET_STATUS,         /* uRequest */
                                0,                      /* uValue */
                                wIndex,                 /* uIndex */
                                *pBufferLength);        /* uSize */

    /*
     * Call USBHST_FILL_CONTROL_URB() macro to populate a URB structure with
     * hubs device handle, endpoint as the USB_HUB_DEFAULT_ENDPOINT, transfer
     * buffer as pBuffer, transfer length as *pBufferLength,
     * USBHST_SETUP_PACKET structure defined above, usbShowCallback as the
     * callback and the OS_EVENT_ID as the pContext.
     */

     USBHST_FILL_CONTROL_URB(  (&ControlURB),           /* Urb */
                               pHub->uDeviceHandle,     /* Device */
                               USB_HUB_DEFAULT_ENDPOINT,/* EndPointAddress */
                               (UCHAR *)pBuffer,        /* TransferBuffer */
                               *pBufferLength,          /* TransferLength */
                               USBHST_SHORT_TRANSFER_OK,/* TransferFlags */
                               pSetupPacket,            /* SetupPacket */
                               usbHubGetInfoCallback,   /* Callback */
                               &EventId,                /* pContext */
                               USBHST_FAILURE           /* Status */
                               );

    /*
     * The usbHstURBSubmit() provides the URB defined above to the USBD.
     * Failure can returned if there is a system error or some of the
     * parameters are incorrect.  If the submittal fails, try to clean up.
     */

    Result = usbHstURBSubmit(&ControlURB);

    if (USBHST_SUCCESS != Result)
        {
        DEBUGMSG(
            "usbHubGetInfo: Submit USB request failed =%d\n",
                Result, 0, 0, 0, 0, 0);

        OS_DESTROY_EVENT(EventId);

        /* Release the memory allocated for the setup packet */

        OS_FREE(pSetupPacket);

        return Result;

        }/* End of (USBHST_SUCCESS !=Result */

    /*
     * OS_WAIT_FOR_EVENT is a wrapper for semTake().  We pend on this
     * semphore waiting for the USBD to complete the transaction
     */

    usbHstWaitUrbComplete(&ControlURB,
                          EventId,
                          usrUsbWaitTimeOutValueGet());

    /*
     * Set the ControlURB.pContext to NULL to prevent the URB's callback
     * function to be called after the EventId be destroied.
     */

    ControlURB.pContext = NULL;

    /*
     * Once the transaction is complete there is no more need for the
     * semaphore
     */

    OS_DESTROY_EVENT(EventId);

    /* If the return result is successful,then convert the data */

    Result = ControlURB.nStatus;
    *pBufferLength = (ControlURB.uTransferLength);
    *pBuffer = FROM_LITTLEL(*pBuffer);

    /* Release the memory allocated for the setup packet */

    OS_FREE(pSetupPacket);

    /* Return Result.*/

    return Result;
    }


/*******************************************************************************
 *
 * searchHub - search a USB hub looking for a particular device.
 *
 * This routine searches a USB hub looking for a port with a particular
 * handle.  This routine will look at each port on a hub.  If it finds
 * an additional USB hub on a port, it will recursively search that hub as
 * well.
 *
 * The data returned by the USBD is as defined by the USB specification and
 * is translated elsewhere
 *
 * Parameters:
 *   pHub - This is a pointer to internal data structure that represents the
 *   USB hub that contains the port of interest.  This is normally the root
 *   hub on a bus (IN).
 *
 *   nodeId - This is the vxWorks device handle that identifies the port of
 *   interest (IN).
 *
 *   ppParent - On successful return, this will be update with the pointer
 *   to the parent hub of the port of interest.  Note that because searches
 *   all downstream hubs on a bus, this may be different from pHub. (IN/OUT)
 *
 *   ppPortInfo - On successful return, this will be updated with the port
 *   info structure on the port of interest. (IN/OUT)
 *
 *   pPort - On successful return, this contains the *index* of the port
 *   structure.  By vxWorks convention you add one to the index to get the
 *   port number (OUT)
 *
 *
 * RETURNS: OK, !OK
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL STATUS searchHub
    (
    pUSB_HUB_INFO        pHub,
    UINT32               nodeId,
    pUSB_HUB_INFO       *ppParent,
    USB_HUB_PORT_INFO  **ppPortInfo,
    INT16               *pPort
    )
    {
    UINT16 numPorts;
    int port;
    USB_HUB_PORT_INFO *pPortInfo;

    /*
     * Undocumented routine usbHubPortCountGet obtains the number of
     * ports on the hub being examined.
     */

    if (usbHubPortCntGet(pHub->uDeviceHandle,&numPorts ) != OK)
        {
        DEBUGMSG("\n usbHubPortCntGet failed %s %d",(int)__FILE__,__LINE__,
                0, 0, 0, 0);
        return(ERROR);
        }

    /*
     * Look at the port info for each port on the hub and see if the handle
     * on that port matches the nodeId that we are looking for
     */

    for (port = 0; port < (INT32) numPorts; port++)
        {
        pPortInfo = pHub->pPortList[port];
        if (pPortInfo != NULL)
            {
            /*
             * This routine returns a success one of two ways - either by
             * detecting the desired device on the current hub or by a
             * successful recursive call to itself in the case where the
             * device is on a downstream hub
             */

            if (pPortInfo->uDeviceHandle == nodeId)
                {
                *ppParent = pHub;
                *ppPortInfo = pPortInfo;
                *pPort = port;

                /*
                 * This return will cause the unwind to start
                 */

                return (OK);
                }
            else
                {
                if (pPortInfo->pHub != NULL)
                    {
                    /*
                     * This port has a USB hub attached to it.  Go search
                     * that hub.  Note that the assumption is made that all
                     * the hubs have been initialized.  If this routine is
                     * invoked while the hubs are initializing it is possible
                     * we will miss part of the hub tree
                     */

                    if (searchHub(pPortInfo->pHub, nodeId, ppParent, ppPortInfo,
                        pPort)  == OK)
                        {
                        return( OK);
                        }
                    }
                }
            }
        }

    /*
     * When we're are done and haven't found anything, that is not necessarily
     * an error.  It's just not one this hub/bus.  The device could possibly be
     * on another bus.  Hence we return !OK rather than ERROR.
     */

    return (!OK);
    }

/*******************************************************************************
 *
 * getPortStatus - This routine gets and returns the port status of the
 * port specified by the node id.  The status returned is that data defined
 * by the USB 2.0 specification section 11.24.2.7 Get Port Status.
 *
 * The three helper routines above are used in the process.
 *
 *   nodeId - This is the vxWorks device handle that identifies the port of
 *  interest (IN).
 *
 *   pBuffer - A 4 byte buffer that will hold the port status.  See the USB
 *   2.0 specification for definition of the bits returned (IN/OUT).
 *
 * Implentation Note:  This routine is somewhat inefficent in that it
 * rediscovers some information already known.  The reasoning is that this
 * routine is also meant to be illustrative in how to interface to the USBD.
 * Also this routine should probably have been in the exportable API, but as
 * of vxWorks 6.5 was not.  Wind River may move this routine - or an equivalent
 * to the USB libraries in subsequent releases
 *
 * Parameters:
 *   nodeId - This is vxWorks nodeId or handle for the desired port (IN)
 *
 *   pBuffer - Pointer to a buffer of at least 4 bytes in length.  The
 *     buffer is updated with the port status. (IN/OUT)
 *
 * RETURNS: OK, ERROR
 * A ERROR return indicates that the requested nodeId was not found.
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL STATUS getPortStatus
    (
    UINT32  nodeId,
    UINT16 *pBuffer
    )
    {
    pUSB_HUB_BUS_INFO pBusList = gpGlobalBus;
    pUSB_HUB_INFO pRootHub;
    pUSB_HUB_INFO pParent;
    USB_HUB_PORT_INFO *pPortInfo;
    UINT16 portStatus;
    INT32 bufLen;
    INT16 portIndex;

    /*
     * We will look on each bus
     */

    while (NULL != pBusList)
        {
        pRootHub = pBusList->pRootHubInfo;
        DEBUGMSG ("\n Root hub - 0x%X  devid - 0x%x",(int)pRootHub,
                (int) pRootHub->uDeviceHandle, 0, 0, 0, 0);

        if (pRootHub->uDeviceHandle == nodeId)
            {
            DEBUGMSG("\n Device is a root hub ", 0, 0, 0, 0, 0, 0);
            return (ERROR);
            }

        /*
         *  searchHub() is a recursive routine that will search all ports
         *  on the rootHub.  If it encounters a port with a hub on it, it
         *  recurses into itself and searches that hub as well.
         */

        if (searchHub(pRootHub, nodeId, &pParent, &pPortInfo, &portIndex) == OK)
            {
            /*
             * The search routine found the port with the approriate nodeId.
             * It returned the information necessary to make the call into
             * the USBD for the port status.  The port nubmer is the index
             * +1 by convention.
             */

            bufLen = 4;
            DEBUGMSG ("\n Getting info on port %d\n", portIndex+1, 0, 0, 0, 0, 0);
            usbHubGetInfo (pParent, portIndex+1, &portStatus, (UINT8 *)&bufLen);
            DEBUGMSG ("\n bufLen = %d Status 0x%4.4x %x %x %x",bufLen,
                portStatus, 0, 0, 0, 0);

            /* Mind the endianess */

            *pBuffer = OS_UINT16_LE_TO_CPU (portStatus);
            return(OK);
            }   /* Endif */
        else
            {
            /*
             * The desired device was not found on this bus.  Go look at
             * the next bus
             */

            pBusList = pBusList->pNextBus;
            }
        }/* END while */
    return (ERROR);
    }


/*******************************************************************************
 *
 *  printPortStatus - This is a routine outputs a series of strings using
 *  a readability indentation and the port Status bits returned by the
 *  getPortStatus above.
 *
 * Parameters:
 *   fp - File handle to the desired output device (IN)
 *
 *   indent - Readability indent string (IN)
 *
 *   portStatus - The bits returned by the getPortStatus routine
 *
 * RETURNS: N/A
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL VOID printPortStatus
    (
    FILE * fp,
    char * indent,
    UINT16 portStatus
    )
    {
    DEBUGMSG("\n Raw status 0x%X",portStatus, 0, 0, 0, 0, 0);
    portStatus = FROM_LITTLEW(portStatus);
    fprintf(fp,"\n %s Port Connection Status: %s is present. (0x%4.4X)",indent,
        (portStatus & M_PORT_CONNECTION ? "A device" : "no device"),portStatus);

    fprintf(fp,"\n %s Port is %s.", indent,
        (portStatus & M_PORT_ENABLE ? "enabled" : "disabled"));

    fprintf(fp,"\n %s Port is %s.", indent,
        (portStatus & M_PORT_SUSPEND ? "suspended or resuming" :
            "not suspended"));

    fprintf(fp,"\n %s Port is %s.", indent,
        (portStatus & M_PORT_OVER_CURRENT ? "in an overcurrent situation" :
            "not in an overcurrent situation"));

    fprintf(fp,"\n %s Reset signalling %s being asserted.", indent,
        (portStatus & M_PORT_RESET ? "is" :
            "is not"));

    fprintf(fp,"\n %s Port Power %s in the powered of state.", indent,
        (portStatus & M_PORT_POWER ? "is not" : "is"));

    /*
     * Speed
     */

    fprintf(fp,"\n %s Device is attached as a ",indent);
    if (portStatus & M_PORT_LOW_SPEED)
        {
        fprintf(fp,"LOW SPEED");
        }
    else if (portStatus & M_PORT_HIGH_SPEED)
        {
        fprintf(fp,"HIGH SPEED");
        }
    else
        {
        fprintf(fp,"FULL SPEED");
        }
    fprintf(fp," device.");

    fprintf(fp,"\n %s This device %s Port Test Mode.",indent,
        (portStatus & M_PORT_TEST ? "is in" : "is not in"));

    return;
    }


/*******************************************************************************
 *
 * getUsbClass - This is a very simple routine that returns an appropriate
 * static string for the corresponding passed in class, subclass and protocol
 *
 * Parameters:
 *   class - The class of the device (IN)
 *
 *   subClass - The sub class of the device (IN)
 *
 *   protocol - The protocol of the device (IN)
 *
 *   portStatus - The bits returned by the getPortStatus routine
 *
 * RETURNS: Pointer to a static string
 *
 * ERRNO: None
 *
 * \NOMANUAL
 */

LOCAL const char *getUSBClass
    (
    int class,
    int subClass,
    int protocol
    )
    {
    /*
     * The following is from the March 2, 2006 Class code definitions
     * from usb.org
     */

    static const char device_00[]={"Specified in the interface descriptor"};
    static const char device_01[]={"Audio device"};
    static const char device_02[]={"Communications and CDC Control device "};
    static const char device_03[]={"Human Interface Device"};
    static const char device_03_00[]={"Boot Interface Subclass"};
    static const char device_03_01[]={"Keyboard"};
    static const char device_03_02[]={"Mouse"};
    static const char device_05[]={"Physical device "};
    static const char device_06[]={"Still Image device"};
    static const char device_07[]={"Printer device"};
    static const char device_07_01[]={"Unidirectional interface"};
    static const char device_07_02[]={"Bi-directional interface"};
    static const char device_07_03[]={"1284.4 compatible bi-directional interface"};
    static const char device_08[]={"Mass Storage device"};
    static const char device_08_00[]={"SCSI command set not reported"};
    static const char device_08_01[]={"RBC"};
    static const char device_08_02[]={"MMC-5 (ATAPI)"};
    static const char device_08_03[]={"UFI"};
    static const char device_08_04[]={"SCSI transparent command set"};
    static const char device_08_05[]={"LSD FS"};
    static const char device_08_06[]={"IEEE 1667"};
    static const char device_08_10[]={"CBI (with command completion interrupt)"};
    static const char device_08_11[]={"CBI (with no command completion interrupt)"};
    static const char device_08_12[]={"BBB (Bulk-Only)"};
    static const char device_08_13[]={"UAS"};
    static const char device_09[]={"Hub"};
    static const char device_09_00[]={"Full Speed Hub"};
    static const char device_09_01[]={"High Speed Hub with 1 TT"};
    static const char device_09_02[]={"High Speed Hub with Multiple TTs"};
    static const char device_0A[]={"CDC-Data device"};
    static const char device_0B[]={"Smart Card device"};
    static const char device_0D[]={"Content Security"};
    static const char device_0E[]={"Video device"};
    static const char device_DC[]={"Diagnostic device "};
    static const char device_E0[]={"Wireless Controller"};
    static const char device_EF[]={"Miscellaneous"};
    static const char device_FE[]={"Application Specific"};
    static const char device_FF[]={"Vendor Specific"};
    static const char device_xx[]={"Unknown"};
    static const char device_yy[]={"None"};

    /*
     * Very simple - just switch on the device code and if appropriate look at
     * the subclass and interface.  Makes for a nasty big switch though
     */

    switch (class)
        {
        case 0x00:
            if (bDeviceDescriptor == TRUE)
                return(device_00);
            else
                return(device_xx);

        case 0x01:
            return(device_01);

        case 0x02:
            return(device_02);

        case 0x03:
            if ((subClass == -1) && (protocol ==-1))
                return(device_03);
            switch (subClass)
                {
                case 0x00:
                    return(device_yy);
                case 0x01:
                    return(device_03_00);
                }
            switch (protocol)
                {
                case 0x00:
                    return(device_yy);
                case 0x01:
                    return(device_03_01);
                case 0x02:
                    return(device_03_02);
                }
        return(device_xx);

        case 0x05:
            return(device_05);

        case 0x06:
            return(device_06);

        case 0x07:
            if ((subClass == -1) && (protocol == -1))
                return(device_07);
            switch (subClass)
                {
                case 0x01:
                return(device_07);
                }
            switch (protocol)
                {
                case 0x01:
                    return(device_07_01);
                case 0x02:
                    return(device_07_02);
                case 0x03:
                    return(device_07_03);
                case 0xFF:
                    return(device_FF);
                }
        return(device_xx);

        case 0x08:
            if ((subClass == -1) && (protocol == -1))
                return(device_08);
            switch (subClass)
                {
                case 0x00:
                    return(device_08_00);
                case 0x01:
                    return(device_08_01);
                case 0x02:
                    return(device_08_02);
                case 0x04:
                    return(device_08_03);
                case 0x06:
                    return(device_08_04);
                case 0x07:
                    return(device_08_05);
                case 0x08:
                    return(device_08_06);
                case 0xFF:
                    return(device_FF);
                }
            switch (protocol)
                {
                case 0x00:
                    return(device_08_10);
                case 0x01:
                    return(device_08_11);
                case 0x50:
                    return(device_08_12);
                case 0x62:
                    return(device_08_13);
                case 0xFF:
                    return(device_FF);
                }
        return(device_xx);

        case 0x09:
        switch (protocol)
            {
            case 0x00:
                return(device_09_00);

            case 0x01:
                return(device_09_01);

            case 0x02:
                return(device_09_02);

            default:
                return(device_09);
            }

        case 0x0A:
            return(device_0A);

        case 0x0B:
            return(device_0B);

        case 0x0D:
        return(device_0D);

        case 0x0E:
            return(device_0E);

        case 0xDC:
            return(device_DC);

        case 0xE0:
            return(device_E0);

        case 0xEF:
            return(device_EF);

        case 0xFE:
            return(device_FE);

        case 0xFF:
            return(device_FF);

        default:
            return(device_xx);
        }
    }


/*******************************************************************************
 * getStringDescr - Obtain the string that is contained in the descriptor
 *
 * This routine issues a GET DESCRIPTOR to the USBD to return the desired
 * english string at the specified index.
 *
 * Parameters:
 *   nodeId - The vxWorks node id of the device in question (IN)
 *
 *   stringId - The string Id number of the desired string (IN)
 *
 *   pBuffer - Pointer to a buffer of at least maxLen lenght to hold the
 *      results (IN/OUT)
 *
 *   maxLen - The maximum length of the returned  data
 *
 * RETURNS: None.
 *
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

LOCAL VOID getStringDescr
    (
    UINT32 nodeId,   /* vxWorks node id */
    UINT8  stringId, /* The string id number */
    char  *pBuffer,  /* The buffer to recieve the string */
    INT32  maxLen    /* Length of the recieving buffer in bytes */
    )
    {
    char *pString;
    UINT32 actLen;
    INT32 i;
    INT32 ii;

    /*
     * If an index is == 0 there is no string data.  Note that this statement
     * is based on experience - I've been unable to find anything in the spec
     * that says the above.
     */

    if (stringId <= 0)
        {
        pBuffer[0] = 0;
        return;
        }

    /*
     * pString is working storage that will freed prior to return
     */

    pString = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN * 2);
    
    if (pString == NULL)
        return;


    /* Read device descriptor to get string Ids*/

    actLen = maxLen;
    if (usbHstGetDescriptor(nodeId,
                            USBHST_STRING_DESC,
                            stringId,
                            0x0409, /* Unicode code for english */
                            &actLen,
                            (UCHAR *) pString) != USBHST_SUCCESS)
        {
        strncpy(pBuffer, "Failed to get strings", maxLen);
        OS_FREE(pString);
        return;
        }

    /*
     * Unicode is 32 bits, hence each character is stored in a word.  Put
     * the characters into a byte buffer.
     *
     * String descriptors are often filled with cruft.  Go through it and
     * terminate the string if anything is non-printable.  They are also
     * *not* null terminated.
     */

    for (i = 2,ii=0; i + 2 <= actLen && ii < maxLen; i += 2, ii++)
        {
        pBuffer[ii] = pString [i];
        if (!isprint((int)pBuffer[ii]))
            {
            pBuffer[ii] = 0;
            break;
            }
        }
    pBuffer[maxLen-1] = 0;    /* Just for safety */

    OS_FREE(pString);
    return;
    }


/*******************************************************************************
 * printInterfDescriptor - Print out the interface descriptor associatied
 * with a given nodeId.  The interface descriptor is passed in.
 *
 * Parameters:
 *   fp - the handle of the desired output device (IN)
 *
 *   indent - Character string holding the reability indent (IN)
 *
 *   nodeId - The vxWorks node ID of the device of interest (IN)
 *
 *   pInterfaceDescr - Pointer to the input interface descriptor
 *
 * RETURNS: None.
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

static void printInterfDescriptor
    (
    FILE                        *fp,
    char                        *indent,
    UINT32                       nodeId,
    USBHST_INTERFACE_DESCRIPTOR *pInterfaceDescr
    )
    {
    char *pString;
    int i;
    USBHST_ENDPOINT_DESCRIPTOR *pEndpointDescr;
    USBHST_INTERFACE_DESCRIPTOR *pTemp;
    UINT16 bufLen;
    char strAttribute[255];

    pString = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pString == NULL)
        return;

    getStringDescr(nodeId, (int) pInterfaceDescr->iInterface,
                   pString,USBHST_MAX_DESCRIPTOR_LEN);

    fprintf(fp, "\n %s        Interface Descriptor Length: 0x%2.2X",indent,
        (int) pInterfaceDescr->bLength);                /* bLength */
    fprintf(fp, "\n %s        Interface Descriptor Type: 0x%2.2X",indent,
        (int) pInterfaceDescr->bDescriptorType);
    fprintf(fp, "\n %s        Interface Number: 0x%2.2X",indent,
        (int) pInterfaceDescr->bInterfaceNumber);
    fprintf(fp, "\n %s        Interface Alternate Setting: 0x%2.2X",indent,
        (int) pInterfaceDescr->bAlternateSetting);
    fprintf(fp, "\n %s        Interface numEndpoints: 0x%2.2X",indent,
        (int) pInterfaceDescr->bNumEndpoints);
    fprintf(fp, "\n %s        Interface Class: 0x%2.2X",indent,
        (int) pInterfaceDescr->bInterfaceClass);
    fprintf(fp, "\n %s        Interface SubClass: 0x%2.2X", indent,
        (int) pInterfaceDescr->bInterfaceSubClass);
    fprintf(fp, "\n %s        Interface Protocol: 0x%2.2X",indent,
        (int)pInterfaceDescr->bInterfaceProtocol);
    fprintf(fp, "\n %s        Translation of above: %s",indent,
            getUSBClass((int)pInterfaceDescr->bInterfaceClass, -1, -1));
    fprintf(fp, "\n %s        Interface Index: 0x%2.2X (\"%s\")",indent,
        (int) pInterfaceDescr->iInterface,pString);

    /*
     * Now printout the endpoints
     */

    bufLen = (UINT16)USBHST_MAX_DESCRIPTOR_LEN;

    /*
     * The parse routine modifies the pointer
     */

    pTemp = pInterfaceDescr;
    for (i = 0; i < (int)pInterfaceDescr->bNumEndpoints; i++)
        {
        fprintf(fp, "\n %s        Endpoint Index: %d",indent,i);
        if ((pEndpointDescr = usbDescrParseSkip ((pUINT8 *)&pTemp,
            (pUINT16)&bufLen, USBHST_ENDPOINT_DESC)) != NULL)
            {
            fprintf(fp, "\n %s          Endpoint Descriptor length: 0x%2.2X",
                indent, pEndpointDescr->bLength);
            fprintf(fp, "\n %s          Endpoint Descriptor Type: 0x%2.2X",
                indent, pEndpointDescr->bDescriptorType);
            fprintf(fp,"\n %s          Endpoint Address Mask: 0x%2.2X",indent,
                pEndpointDescr->bEndpointAddress);
            fprintf(fp,"\n %s              Endpoint number: 0x%2.2X",indent,
                (pEndpointDescr->bEndpointAddress & 0x07));
            fprintf(fp,"\n %s              Endpoint Direction: %s",indent,
                 ((pEndpointDescr->bEndpointAddress & 0x80) ? "IN" : "OUT"));
            fprintf(fp,"\n %s          Attributes Mask: 0x%2.2X",indent,
                pEndpointDescr->bmAttributes);

            /*
             * Extract the transfer type (lower two bits)
             */

            switch (pEndpointDescr->bmAttributes & 0x03)
                {
                case USBHST_CONTROL_TRANSFER:
                strcpy(strAttribute,"Control");
                break;

                case USBHST_ISOCHRONOUS_TRANSFER:
                strcpy(strAttribute,"Isochronous");
                break;

                case USBHST_BULK_TRANSFER:
                strcpy(strAttribute,"Bulk");
                break;

                case USBHST_INTERRUPT_TRANSFER:
                strcpy(strAttribute,"Interrupt");
                break;

                default:
                strcpy(strAttribute,"Unknown Transfer Type");
                }
            fprintf(fp,"\n %s              Transfer Type: %s.",indent,
                strAttribute);

            switch ((pEndpointDescr->bmAttributes & 0x0C) >> 2)
                {
                case 00:
                strcpy(strAttribute,"No Synchronization");
                break;

                case 01:
                strcpy(strAttribute,"Asynchronous");
                break;

                case 02:
                strcpy(strAttribute,"Adaptive");
                break;

                case 03:
                strcpy(strAttribute,"Synchronous");
                break;

                default:
                strcpy(strAttribute,"Unknown Synchronization Type");
                }
            fprintf(fp,"\n %s              Synchronization Type: %s.",indent,
                strAttribute);
            switch ((pEndpointDescr->bmAttributes & 0x30) >> 4)
                {
                case 00:
                strcpy(strAttribute,"Data Endpoint");
                break;

                case 01:
                strcpy(strAttribute,"Feedback Endpoint");
                break;

                case 02:
                strcpy(strAttribute,"Implicit Feedback data endpoint");
                break;

                default:
                strcpy(strAttribute,"Reserved");
                }
            fprintf(fp,"\n %s              Usage Type: %s.",indent,strAttribute);
            fprintf(fp,"\n %s          Maximum Packet Size: 0x%4.4X (%d)", indent,
                FROM_LITTLEW(pEndpointDescr->wMaxPacketSize),
                FROM_LITTLEW(pEndpointDescr->wMaxPacketSize));
            fprintf(fp,"\n %s          Interval: 0x%2.2X",indent,
                (int)pEndpointDescr->bInterval);
            }
        else
            {
            printf("\n %s          Unable to locate endpoint descriptor",indent);
            }
        }
    OS_FREE(pString);
    return;
    }


/*******************************************************************************
 * printConfigDescriptor - Print out the configuration descriptor passed
 *
 * RETURNS: None.
 *
 * Parameters:
 *   fp - the handle of the desired output device (IN)
 *
 *   indent - Character string holding the reability indent (IN)
 *
 *   nodeId - The vxWorks node ID of the device of interest (IN)
 *
 *   pConfigDescr - Pointer to the input configuration descriptor (IN)
 *
 *   descrLength - Length of the input configuration descriptor (IN)
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

 static void printConfigDescriptor
    (
    FILE                     *fp,
    char                     *indent,
    UINT32                    nodeId,
    USBHST_CONFIG_DESCRIPTOR *pConfigDescr,
    UINT16                    descrLength
    )
    {
    int altIndex=0;
    char *pConfigString;
    USBHST_CONFIG_DESCRIPTOR *pTemp;
    USBHST_INTERFACE_DESCRIPTOR *pInterfaceDescr;

    pConfigString = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pConfigString == NULL)
        return;

    getStringDescr(nodeId, (int) pConfigDescr->iConfiguration,
        pConfigString,USBHST_MAX_DESCRIPTOR_LEN);

    fprintf(fp,"\n %s     Config Descriptor Length: 0x%2.2X",indent,
        (int) pConfigDescr->bLength);                /* bLength */
    fprintf(fp,"\n %s     Config Descriptor Type: 0x%2.2X",indent,
        (int) pConfigDescr->bDescriptorType);	    /* bDescriptorType */
    fprintf(fp,"\n %s     Config Descriptor Total Length: 0x%4.4X (%d)",indent,
        (int) FROM_LITTLEW(pConfigDescr->wTotalLength),
        (int) FROM_LITTLEW(pConfigDescr->wTotalLength));           /* wTotalLength */
    fprintf(fp,"\n %s     Config Descriptor Number of Interfaces: 0x%2.2X",indent,
        (int) pConfigDescr->bNumInterfaces);	         /* bNumInterfaces */
    fprintf(fp,"\n %s     Config Descriptor Configuration Value: 0x%2.2X",indent,
        (int) pConfigDescr->bConfigurationValue);	 /* bConfigurationValue */
    fprintf(fp,"\n %s     Config Descriptor String Index: 0x%4.4X (\"%s\")",indent,
        FROM_LITTLEW((int) pConfigDescr->iConfiguration),pConfigString);
        /* iConfiguration */
    fprintf(fp,"\n %s     Config Descriptor Attribute byte: 0x%2.2X",indent,
        (int) pConfigDescr->bmAttributes);		    /* bmAttributes */
    fprintf(fp,"\n %s         Device %s bus powered (valid only for usb1.x)",
        indent, ((pConfigDescr->bmAttributes & 0x80) ? "can be" : "cannot be"));
    fprintf(fp,"\n %s         Device %s self powered", indent,
        ((pConfigDescr->bmAttributes & 0x40) ? "can be" : "cannot be"));
    fprintf(fp,"\n %s         Device %s remote wakeup capability", indent,
         ((pConfigDescr->bmAttributes & 0x20) ? "does have" : "does not have"));
    fprintf(fp,"\n %s     Config Descriptor Max Power: 0x%X (%d ma)",indent,
        (int) pConfigDescr->MaxPower,((int)pConfigDescr->MaxPower)*2);

    /*
     * The configuration descriptor contains the number of
     * interfaces supported by the configuration.  Display
     * each of those.  There can be any number of alternate interfaces
     * limited only by the length of the configuration descriptors.
     * usbDescrParse updates the pointer and the length remaining
     */

     pTemp = pConfigDescr;
     while ((pInterfaceDescr = usbDescrParseSkip((pUINT8 *)&pTemp,
                                (pUINT16) &descrLength,
                                USBHST_INTERFACE_DESC)) != NULL)
        {
        fprintf(fp,"\n %s     --------------------------",indent);
        fprintf(fp,"\n %s     Configuration Interface %d",
                 indent, altIndex++);
        printInterfDescriptor(fp, indent, nodeId, pInterfaceDescr);
        }
    OS_FREE(pConfigString);
    return;
    }


/*******************************************************************************
 * printDeviceDescriptor - Print a USB Device Descriptor
 *
 * This functions prints out the various fields within a standard
 * device descriptor.
 *
 * Parameters:
 *   FIlE *fp - Output handle - usually stdout (IN/OUT)
 *
 *   int level - Level of detail - (IN)
 *
 *   char *indent - Indentation string - (IN)
 *
 *   pDevDescr - Device descriptor to be displayed - (IN)
 *
 * RETURNS: N/A
 *
 * ERRNO: None
 */

static void printDeviceDescriptor
    (
    FILE                     *fp,           /* Open output descriptor IN */
    int                       verboseLevel, /* Level argument from command line IN */
    char                     *indent,       /* Level of recursion indication IN */
    UINT32                    nodeId,       /* vxWorks node id IN */
    USBHST_DEVICE_DESCRIPTOR *pDevDescr     /* Device descriptor of interest IN */
    )
    {
    char *pManufacturer;
    char *pProduct;
    char *pSerial;

    /*
    * We need space for the string descriptors
    */
    pManufacturer = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pManufacturer == NULL)
        return;

    pProduct = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pProduct == NULL)
        {
        OS_FREE(pManufacturer);
        return;
        }

    pSerial = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pSerial == NULL)
        {
        OS_FREE(pManufacturer);
        OS_FREE(pProduct);
        return;
        }

    getStringDescr(nodeId, pDevDescr->iManufacturer,
        pManufacturer,USBHST_MAX_DESCRIPTOR_LEN);

    getStringDescr(nodeId, pDevDescr->iProduct,
        pProduct,USBHST_MAX_DESCRIPTOR_LEN);

    getStringDescr(nodeId, pDevDescr->iSerialNumber,
        pSerial,USBHST_MAX_DESCRIPTOR_LEN);

    if (verboseLevel < 1)
        {
        /*
         * For pretty.  Sometimes the devices don't have a manufacturer
         */

        if (strlen(pManufacturer) > 0)
            {
            fprintf(fp,"\n Bus %3.3d Device %3.3d: ID %4.4X:%4.4X %s %s %s",
                ((nodeId >> 8) & 0xFF), (nodeId & 0xFF),
                pDevDescr->idVendor,pDevDescr->bcdDevice,
                pManufacturer,pProduct,pSerial);
            }
        else
            {
            fprintf(fp,"\n Bus %3.3d Device %3.3d: ID %4.4X:%4.4X %s %s",
                ((nodeId >> 8) & 0xFF), (nodeId & 0xFF),
                pDevDescr->idVendor,pDevDescr->bcdDevice,
                pProduct,pSerial);
           }
        }
    else
        {
        fprintf(fp,"\n %s Node ID: 0x%X (Bus - 0x%x Address - 0x%X)",indent,
            nodeId, ((nodeId >> 8) & 0xFF), (nodeId & 0xFF));
        fprintf(fp,"\n %s Descriptor Type: 0x%X",indent,
                    pDevDescr->bDescriptorType);
        fprintf(fp,"\n %s USB Release (BCD): 0x%4.4X",indent,
                    FROM_LITTLEW(pDevDescr->bcdUSB));
        fprintf(fp,"\n %s Device Class: 0x%2.2X",indent,pDevDescr->bDeviceClass  );
        fprintf(fp,"\n %s Device SubClass: 0x%2.2X",indent,
                    pDevDescr->bDeviceSubClass);
        fprintf(fp,"\n %s Device Protocol: 0x%2.2X",indent,
                    pDevDescr->bDeviceProtocol);
        fprintf(fp,"\n %s Translation of above: %s",indent,
                getUSBClass((int)pDevDescr->bDeviceClass, -1, -1));
        fprintf(fp,"\n %s Max Packet Size 0: 0x%2.2X (%d)",indent,
                pDevDescr->bMaxPacketSize,(int)pDevDescr->bMaxPacketSize);
        fprintf(fp,"\n %s Vendor Code: 0x%4.4X",indent,
                FROM_LITTLEW(pDevDescr->idVendor));
        fprintf(fp,"\n %s Product Code: 0x%4.4X",indent,
                FROM_LITTLEW(pDevDescr->idProduct));
        fprintf(fp,"\n %s Device Version: 0x%4.4X",indent,
                FROM_LITTLEW(pDevDescr->bcdDevice));
        fprintf(fp,"\n %s Manufacturer Index: 0x%2.2X (\"%s\")",indent,
            (UINT8)pDevDescr->iManufacturer, pManufacturer);
        fprintf(fp,"\n %s Product Index: 0x%2.2X (\"%s\")",indent,
            (UINT8)pDevDescr->iProduct, pProduct);
        fprintf(fp,"\n %s Serial Number Index: 0x%2.2X (\"%s\")",indent,
            (UINT8)pDevDescr->iSerialNumber, pSerial);
        fprintf(fp,"\n %s Number of Configurations: 0x%2.2X",indent,
                pDevDescr->bNumConfigurations);
        }

    /*
     * Clean up and exit
     */

    OS_FREE(pManufacturer);
    OS_FREE(pProduct);
    OS_FREE(pSerial);
    return;
    }


/*******************************************************************************
* enumerateHub - This is a recursive routine that enumeratea a USB hub.
*
* This functions prints out the various fields within a standard
* device descriptor.
*
* Parameters:
*   FIlE *fp - Output handle - usually stdout (IN)
*
*   int level - Level of detail  (IN)
*
*   char *indent - Indentation string  (IN)
*
*   pDevDescr - Device descriptor to be displayed - (IN)
*
* RETURNS: N/A
*
* ERRNO: None
*/

void enumerateHub
    (
    FILE  *fp,
    int    verboseLevel,
    int    callLevel,
    char  *idString,
    UINT32 hubId
    )
    {
    int i;
    INT32 portIndex;
    UINT32 nodeId;
    UINT32 actLen;
    STATUS status;
    UINT16 nodeType;
    UINT16 numPorts;
    char *portIdString;
    char *indent;
    char hubIdString[255];
    USBHST_CONFIG_DESCRIPTOR *pConfigDescr;
    USBHST_DEVICE_DESCRIPTOR *pDevDescr;
    UINT16 portStatus = 0;

    pDevDescr = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
    if (pDevDescr == NULL)
        return;

    /*
     * portIdString allows us to identify the actual physical port
     */

    portIdString = OSS_CALLOC(strlen(idString) + 20);
    if (portIdString == NULL)
        {
        OS_FREE(pDevDescr);
        return;
        }

    /*
     * indent is a readability indentation
     */

    indent = OSS_CALLOC(callLevel+1);
    if (indent == NULL)
        {
        OS_FREE(portIdString);
        OS_FREE(pDevDescr);
        return;
        }

    memset(indent,'-',callLevel);
    indent[callLevel] = 0;

    /*
     * This is an undocumented (as of vxWorks 6.5) routine that returns the
     * number of ports on a hub
     */

    usbHubPortCntGet((UINT32)hubId, &numPorts);

    /*
     * To make things more readable, do each port on a hub.  Afterwards,
     * pass back over and then enumerate any hubs that are found with a
     * recursive call.  It would be easier to do the recursion as each
     * hub is found, but once again it is less readable.
     *
     * verboseLevel == 0 is the lowest level of detail.  We do not do a lot of
     * interesting things if the verboseLevel == 0
     */

    if (verboseLevel > 0)
        {
        fprintf(fp, "\n======================================================");
        fprintf(fp,"\n \n %s The hub at %s has %d available ports",indent,
                idString, numPorts);
       }

    /*
     * Loop over all the ports in the hub.
     */

    for (portIndex=0; portIndex < (INT32) numPorts; portIndex++)
        {
        if (verboseLevel > 0)
            {
            fprintf(fp, "\n--------------------------------------------------");
            sprintf(portIdString,"%s Port:%d",idString, portIndex);
            fprintf(fp, "\n \n %s %s",indent, portIdString);
            }

        /*
         * usbHubNodeIDGet is undocumented.  The real important thing is
         * that it returns the node type and node id or node device handle.
         */

        if (usbHubNodeIDGet (hubId, portIndex, &nodeType, &nodeId) !=
                             USBHST_SUCCESS)
            {
            fprintf(fp,"\n %s Unexpected Error on port %s",indent,portIdString);
            continue;
            }

        /*
         * We can have different types of devices or nothing attached to
         * a port.  Printout the data accordingly.
         */

        switch (nodeType)
            {
            case USBHUB_NODETYPE_NONE:
            if (verboseLevel > 0)
                {
                fprintf(fp,"\n %s Nothing connected to %s",indent,
                        portIdString);
                }

            break;

            case USBHUB_NODETYPE_HUB:
            case USBHUB_NODETYPE_DEVICE:
            /* */
            if (verboseLevel > 0)
                {
                fprintf(fp,"\n %s A device is connected to %s.",indent,
                    portIdString);
                }

            /*
             * Get the device descriptor and print out the device
             * information.  Note that if the verbosity level is below
             * 1 abbreviated information is printed.
             */

            actLen = USBHST_MAX_DESCRIPTOR_LEN;
            status = usbHstGetDescriptor( (UINT32) nodeId,
                                    USBHST_DEVICE_DESC, 0, 0, &actLen,
                                    (UINT8 *) pDevDescr) ;
            if (status == USBHST_SUCCESS)
                {
                printDeviceDescriptor (fp, verboseLevel, indent, nodeId,
                                pDevDescr);
                }
            else
                {
                fprintf(fp," %s Error getting device descriptor for %s",indent,
                        portIdString);
                break;
                }
            if (verboseLevel > 0)
                {
                if (verboseLevel > 1)
                    {
                    /*
                     * Get and display the port status.  This includes things
                     * like the connection speed
                     */

                     if (nodeType != USBHUB_NODETYPE_HUB)
                         {
                         getPortStatus (nodeId,&portStatus);
                         printPortStatus(fp,indent,portStatus);
                         }
                     }
                /*
                 * For each configuration supported by the device, get the
                 * configuration descriptor.
                 */

                pConfigDescr = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN*2);
                if (pConfigDescr == NULL)
                    {
                    break;
                    }

                for (i = 0; i < (int)pDevDescr->bNumConfigurations; i++)
                    {
                    memset(pConfigDescr, 0 , USBHST_MAX_DESCRIPTOR_LEN*2);
                    actLen = USBHST_MAX_DESCRIPTOR_LEN*2;
                    if ((status = usbHstGetDescriptor((UINT32) nodeId,
                                                       USBHST_CONFIG_DESC,
                                                       i,
                                                       0,
                                                       &actLen,
                                                       (UINT8 *) pConfigDescr))
                         == USBHST_SUCCESS)
                        {
                        fprintf(fp,"\n %s Configuration index %d:",indent,i);
                        printConfigDescriptor(fp,indent,nodeId,pConfigDescr,
                                    actLen);
                        }
                    else
                        {
                        fprintf(fp,
                            "\n Error getting configuration descriptor for"
                            " configuration index %d",i);
                        continue;
                        }

                    }
                 OS_FREE(pConfigDescr);
                 }   /* endif if (verboseLevel > 0) */
            break;
            }   /* End Switch */
        } /* End of For Loop */
    /*
     * Now pass through the hub ports again and enumerate any hubs we
     * found
     */

    for (portIndex=0; portIndex < (INT32) numPorts; portIndex++)
        {
        nodeType = USBHUB_NODETYPE_NONE;
        sprintf(portIdString,"%s Port:%d",idString, portIndex);

        /*
         * If the nodeType is unrecognized, it can't be enumerated
         * anyway so there is no error routine here.  Errors in the device
         * are reported above.
         */

        if (usbHubNodeIDGet (hubId, portIndex, &nodeType, &nodeId) ==
            USBHST_SUCCESS)
            {
            if (nodeType == USBHUB_NODETYPE_HUB)
                {
                /* Recursive hub enumureation routine */
                sprintf(hubIdString,"%s HubPort:%d",idString, portIndex);
                enumerateHub(fp, verboseLevel, callLevel+1,
                    hubIdString, nodeId);
                }
            }
        }
    OS_FREE (pDevDescr);
    OS_FREE (portIdString);
    OS_FREE (indent);
    return;
    }


/*******************************************************************************
 * USBShow - Enumerate all USB host controllers in the system.
 *
 * This routine enumerates all USB host controllers, hubs, and devices
 * currently connected to the system.  The caller must register with the
 * USBD prior to calling this function and supply its USBD_CLIENT_HANDLE
 * in <clientHandle>.
 *
 * Parameters:
 *  fout - Filehandle of the output device (IN)
 *
 *  level - Verbosity level (IN)
 *
 * RETURNS: OK, or RET_CONTINUE if USBD returns an error.
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

LOCAL STATUS USBShow
    (
     FILE *fout,
     int   level
    )
    {
    USBHST_DEVICE_DESCRIPTOR *pDevDescr;
    UINT16 busCount;    /* Number of USB host controllers */
    UINT16 busIndex;    /* current bus index */
    UINT32 rootHub;
    UINT32 actLen;
    char idString[20];
    STATUS status;
    /*
     * Retrieve the number of USB host busses in the system.
     * This is an undocumented routine
     */

    busCount = usbdBusCntGet ();

    fprintf (fout,"bus count = %d\n", busCount);

    /* Retrieve the root hub id for each host controller and enumerate it. */

    for (busIndex = 0; busIndex < busCount; busIndex++)
        {
        sprintf(idString,"Bus:%3.3d",busIndex);
        /*
         * Each Bus has a root hub.  Enumerate that root hub
         * NOTE:  This uses an undocumented routine
         */

        if (usbHubGetRootHubHandle(busIndex, &rootHub) != OK)
            {
            logMsg("Unexpected failure %s %d",(int)__FILE__,__LINE__,0,0,0,0);
            return (ERROR);
            }
        if (level > 0)
            {
            fprintf (fout,"\n=============================");
            fprintf (fout,"\nEnumerating Root Hub bus %d\n", busIndex);
            }
        /*
         * Note: USBHST_MAX_DESCRIPTOR_LEN is used rather than
         * USB_DEVICE_DESCR_LEN
         * just to give a little bit of wiggle room.  This is true of descr
         * types as well.
         */

        pDevDescr = OSS_CALLOC(USBHST_MAX_DESCRIPTOR_LEN);
        if (pDevDescr == NULL)
            {
            return ERROR;
            }

        actLen = USBHST_MAX_DESCRIPTOR_LEN;
        status = usbHstGetDescriptor((UINT32) rootHub,
                                     USBHST_DEVICE_DESC, 0, 0, &actLen,
                                     (UINT8 *) pDevDescr) ;
        if (status == USBHST_SUCCESS)
            {
            printDeviceDescriptor (fout,level, "", rootHub, pDevDescr);

            /* Recursive hub enumureation routine */

            enumerateHub(fout, level, 0, idString, rootHub);
            }
        else
            {
            fprintf(fout," %s Error getting device descriptor for %s","",
                    idString);
            }
        OS_FREE(pDevDescr);
        } /* End of bus for loop */
    return(OK);
    }


/*******************************************************************************
 * lsusb - show usb devices on the system.
 *
 * This is the first entry point to the usbShow subsystem.
 *
 * The routine takes one argument - level.
 *
 * Level 0 - The default.  This is very simple listing of the usb ports
 * Level 1 - Listing of all the usb ports with descriptor dumps.  This can
 *           be a lot of ouput
 * Level 2 - Level 2 with the addtion of port status information.
 *
 * Note:  This routine is not an "instanteous snapshot" of the USB system. It
 * will gather data for display over time and should not be used by USB
 * configuration is occuring.  If the routine is invoked while the USB is
 * process of configuring itself, erronous information may be displayed.
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

void lsusb (int level)
    {
    USBShow(stdout,level);
    fprintf(stdout,"\n");   /* for the status value */
    }


/*******************************************************************************
 * usbShow - show usb devices on the system by using the pre defined level
 *
 * This is the second entry point to the usbShow subsystem. This is modeled
 * roughly after the linux lsusb (contains no gpl code) but with some
 * enhancements to make it more useful for vxWorks programmers.
 *
 * The routine takes one argument - USB_SHOW_LEVEL.
 *
 * Level 0 - The default.  This is very simple listing of the usb ports
 * Level 1 - Listing of all the usb ports with descriptor dumps.  This can
 *           be a lot of ouput
 * Level 2 - Level 2 with the addtion of port status information.
 *
 * Note:  This routine is not an "instanteous snapshot" of the USB system. It
 * will gather data for display over time and should not be used by USB
 * configuration is occuring.  If the routine is invoked while the USB is
 * process of configuring itself, erronous information may be displayed.
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

void usbShow (void)
    {
    USBShow(stdout, USB_SHOW_LEVEL);
    fprintf(stdout, "\n");
    }


#ifdef INCLUDE_USB_GEN2_HELPER

/*******************************************************************************
 * usbGen2Show - show attached usb devices which using GEN2 class driver
 *
 * This is the third entry point to the usbShow subsystem. The routine show all
 * attached usb devices which using GEN2 class driver
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

void usbGen2Show (void)
    {
    #ifdef INCLUDE_USB_GEN2_MOUSE_INIT
    usbGen2GenericShow(pUsb2MseDriver);
    #endif
    #ifdef INCLUDE_USB_GEN2_KEYBOARD_INIT
    usbGen2GenericShow(pUsb2KbdDriver);
    #endif
    #ifdef INCLUDE_USB_GEN2_PRINTER_INIT
    usbGen2GenericShow(pUsb2PrnDriver);
    #endif
    #ifdef INCLUDE_USB_GEN2_END_INIT
    usbGen2GenericShow(pUsb2EndDriver);
    #endif
    #ifdef INCLUDE_USB_GEN2_STORAGE_INIT
    usbGen2GenericShow(pUsb2MscDriver);
    #endif

    return;
    }


/*******************************************************************************
 * usbGen2GenericShow - print GEN2 class driver generic and specific information
 *
 * This routine show generic and specific class driver information
 *
 * Parameters:
 *   USB2_CLASS_DRIVER * pUsb2ClassDriver
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbGen2GenericShow
    (
    USB2_CLASS_DRIVER * pUsb2ClassDriver
    )
    {
    int                     fd            = 0;
    UINT32                  uIdleTime     = 0;
    UINT32                  uProtocol     = 0;
    UINT32                  uMode         = 0;
    UINT32                  uLed          = 0;
    UINT32                  uPortStatus   = 0;
    UINT32                  uLength       = 0;
    int                     uEndFlag      = 0;
    ULONG                   uEndBuf       = 0;
    UINT8 *                 pEndAddr      = NULL;
    LIST *                  pList         = NULL;
    USB2_CLASS_DEVICE *     pDevice       = NULL;
    USB2_CLASS_NODE *       pNode         = NULL;
    pUSBD_DEVICE_INFO       pDeviceInfo   = NULL;
    USB2_PRN_CAPABILITIES * pCapabilities = NULL;
    USB2_END_DEVICE *       pEndDevice    = NULL;
    USB2_MSC_DEVICE *       pMscDevice    = NULL;
    BOOL                    bIdleTime     = FALSE;
    BOOL                    bProtocol     = FALSE;
    BOOL                    bMode         = FALSE;
    BOOL                    bLed          = FALSE;
    BOOL                    bPortStatus   = FALSE;
    BOOL                    bCapabilities = FALSE;
    BOOL                    bEndFlag      = FALSE;
    BOOL                    bEndBuf       = FALSE;
    BOOL                    bEndAddr      = FALSE;
    BOOL                    bIsEnd        = FALSE;
    BOOL                    bIsMsc        = FALSE;
    char                    SpeedType   [USBHST_MAX_DESCRIPTOR_LEN];
    char                    ProtocolType[USBHST_MAX_DESCRIPTOR_LEN];
    char                    IdleTime    [USBHST_MAX_DESCRIPTOR_LEN];
    char                    Mode        [USBHST_MAX_DESCRIPTOR_LEN];
    char                    Led         [USBHST_MAX_DESCRIPTOR_LEN];
    char                    PortStatus  [USBHST_MAX_DESCRIPTOR_LEN];
    char                    EndFlag     [USBHST_MAX_DESCRIPTOR_LEN];
    char                    EndBuf      [USBHST_MAX_DESCRIPTOR_LEN];
    char                    EndPhyAddr  [USBHST_MAX_DESCRIPTOR_LEN];
#ifdef INCLUDE_USB_GEN2_END_INIT
    /* Ethernet specific */
    const char Ethernet[]    = "usb2End";
#endif
#ifdef INCLUDE_USB_GEN2_STORAGE_INIT
    /* Mass Storage  specific */
    const char MassStorage[] = "usb2Msc";
#endif
    if (pUsb2ClassDriver == NULL)
        return;
    if (usb2DriverIsEmpty(pUsb2ClassDriver) == TRUE)
        return;
    else
        {
        pList = pUsb2ClassDriver->pDeviceList;
        pNode = (USB2_CLASS_NODE *)lstFirst(pList);
        while(pNode != NULL)
            {
            uIdleTime       = 0;
            uProtocol       = 0;
            uMode           = 0;
            uLed            = 0;
            uPortStatus     = 0;
            uLength         = 0;
            uEndFlag        = 0;
            uEndBuf         = 0;
            bIdleTime       = FALSE;
            bProtocol       = FALSE;
            bMode           = FALSE;
            bLed            = FALSE;
            bPortStatus     = FALSE;
            bCapabilities   = FALSE;
            bEndFlag        = FALSE;
            bEndBuf         = FALSE;
            bIsEnd          = FALSE;
            bIsMsc          = FALSE;
            bEndAddr        = FALSE;
            pEndDevice      = NULL;
            pCapabilities   = NULL;
            pDevice         = pNode->pClassDevice;

            printf("\nFound usb device %s \n", pDevice->pDeviceName);
            usbDeviceInfoShow(stdout, pDevice);
            printf("\n");

            /* Get the device information, for the descriptors */

            usbdTranslateDeviceHandle(pDevice->hDevice, &pDeviceInfo);

            if (NULL == pDeviceInfo )
                {
                DEBUGMSG("Error: Get device information failed \n",
                         1, 2, 3, 4, 5, 6);
                }
            else
                {
                switch (pDeviceInfo->uDeviceSpeed)
                    {
                    case USBHST_HIGH_SPEED:
                    strcpy(SpeedType, "High");
                    break;

                    case USBHST_FULL_SPEED:
                    strcpy(SpeedType, "Full");
                    break;

                    case USBHST_LOW_SPEED:
                    strcpy(SpeedType, "Low");
                    break;

                    default:
                    strcpy(SpeedType, "Unknown Speed Type");
                    }

                fprintf(stdout,
                        "Current status: \n"                                   \
                        "Device handle:                     0x%8.8X \n"        \
                        "Device speed:                      %s \n"             \
                        "Device address:                    0x%2.2X \n"        \
                        "Bus index:                         0x%2.2X \n"        \
                        "Attached on hub port index:        0x%2.2X \n"        \
                        "Parent hub tier:                   0x%2.2X \n"        \
                        "Composite device:                  %s \n"             \
                        "Current configuration value:       0x%2.2X \n"        \
                        "Current interface number:          0x%2.2X \n"        \
                        "Current alternate setting:         0x%2.2X \n",
                        FROM_LITTLEL(pDeviceInfo->hDevice),
                        SpeedType,
                        pDeviceInfo->uDeviceAddress,
                        pDeviceInfo->uBusIndex,
                        pDeviceInfo->uParentPortIndex,
                        pDeviceInfo->uParentTier,
                        (pDeviceInfo->bIsComposite == TRUE) ? "Yes" :"No",
                        pDevice->bConfigurationIndex + 1,
                        pDevice->bInterfaceNumber,
                        pDevice->bAlternateSetting
                        );

#ifdef INCLUDE_USB_GEN2_END_INIT
                if ((strstr(pDevice->pDeviceName, Ethernet)) != NULL)
                    {
                    pEndDevice = pDeviceInfo->pInterfacesInfo->pDriverData;
                    if (pEndDevice != NULL)
                        {
                        bIsEnd = TRUE;
                        if (usb2EndIoctl(pEndDevice, EIOCGFLAGS, &uEndFlag) == OK)
                            {
                            bEndFlag = TRUE;
                            }
                        if (usb2EndIoctl(pEndDevice, EIOCGFBUF, &uEndBuf) == OK)
                            {
                            bEndBuf = TRUE;
                            }
                        pEndAddr = (UINT8 *)OSS_CALLOC
                                   ((size_t)END_HADDR_LEN (&pEndDevice->endObj));
                        if(pEndAddr == NULL)
                            {
                            DEBUGMSG("pEndAddr is null \n", 1, 2, 3, 4, 5, 6);
                            }
                        else
                            {
                            if (usb2EndIoctl(pEndDevice, EIOCGADDR, pEndAddr) == OK)
                                {
                                bEndAddr = TRUE;
                                }
                            else
                                DEBUGMSG("EIOCSADDR \n", 1, 2, 3, 4, 5, 6);
                            }
                        }
                    }
#endif

#ifdef INCLUDE_USB_GEN2_STORAGE_INIT
                if ((strstr(pDevice->pDeviceName, MassStorage)) != NULL)
                    {
                    pMscDevice = pDeviceInfo->pInterfacesInfo->pDriverData;
                    if (pMscDevice != NULL)
                        {
                        bIsMsc = TRUE;
                        }
                    }
#endif
                }

            printf("\n");
            printf("Class driver specific: \n");

            fd = open(pDevice->pDeviceName, O_RDONLY, 0);
            if (fd < 1)
                {
                DEBUGMSG("Error: Open device failed \n",
                         1, 2, 3, 4, 5, 6);
                }
            else
                {
                /* HID specific */

                if (ioctl(fd, USB2_IOCTL_HID_IDLE_TIME_GET, &uIdleTime) == OK)
                    {
                    bIdleTime = TRUE;
                    }
                if (ioctl(fd, USB2_IOCTL_HID_PROTO_GET, &uProtocol) == OK)
                    {
                    bProtocol = TRUE;
                    }

                /* Keyboard specific */

                if (ioctl(fd, SIO_KYBD_MODE_GET, &uMode) == OK)
                    {
                    bMode = TRUE;
                    }
                if (ioctl(fd, SIO_KYBD_LED_GET, &uLed) == OK)
                    {
                    bLed = TRUE;
                    }

                /* Printer specific */

                if (ioctl(fd, USB2_PRN_IOCTL_GET_PORT_STATUS, &uPortStatus) == OK)
                    {
                    bPortStatus = TRUE;
                    }
                if (ioctl(fd, USB2_PRN_IOCTL_GET_CAPABILITIES_LENGTH, &uLength) == OK)
                    {
                    pCapabilities = OSS_CALLOC(uLength);
                    if (pCapabilities == NULL)
                        {
                        DEBUGMSG("MALLOC failed for pCapabilities \n",
                                 1, 2, 3, 4, 5, 6);
                        }
                    else
                        {
                        if(ioctl(fd, USB2_PRN_IOCTL_GET_CAPABILITIES_FULL,
                                 pCapabilities) == OK)
                            {
                            bCapabilities = TRUE;
                            }
                        }
                    }

                if(bProtocol == TRUE)
                    {
                    if ((UINT8)uProtocol == 0)
                        strcpy(ProtocolType, "Boot protocol");
                    else if ((UINT8)uProtocol == 1)
                        strcpy(ProtocolType, "Report protocol");
                    else
                        sprintf(ProtocolType, "0x%2.2X (Unknown protocol)",
                                (UINT8)uProtocol);
                    }
                else
                    strcpy(ProtocolType, "Unsupport");

                if(bIdleTime == FALSE)
                    strcpy(IdleTime, "Unsupport");
                else
                    sprintf(IdleTime, "0x%2.2X", (UINT8)uIdleTime);

                if(bMode == FALSE)
                    strcpy(Mode, "Unsupport");
                else if (uMode == 1)
                    strcpy(Mode, "SIO_KYBD_MODE_RAW");
                else if (uMode == 2)
                    strcpy(Mode, "SIO_KYBD_MODE_ASCII");
                else if (uMode == 3)
                    strcpy(Mode, "SIO_KYBD_MODE_UNICODE");
                else
                    strcpy(Mode, "Unknown mode");

                if(bLed == FALSE)
                    strcpy(Led, "Unsupport");
                else
                    sprintf(Led, "NUM(%s) CAP(%s) SCR(%s)",
                            ((UINT8)uLed & 1) ? "On" : "Off",
                            ((UINT8)uLed & 2) ? "On" : "Off",
                            ((UINT8)uLed & 4) ? "On" : "Off");

                if(bPortStatus == FALSE)
                    strcpy(PortStatus, "Unsupport");
                else
                    sprintf(PortStatus, "0x%2.2X (%s, %s, %s)",
                            (UINT8)uPortStatus,
                            ((UINT8)uPortStatus & 0x08) ? "No Error" : "Error",
                            ((UINT8)uPortStatus & 0x10) ? "Selected" : "Not Selected",
                            ((UINT8)uPortStatus & 0x20) ? "Paper Empty" : "Paper Not Empty");

                fprintf(stdout,
                        "HID specific: \n"                                     \
                        "Current idle rate:                 %s \n"             \
                        "Current active protocol:           %s \n"             \
                        "Keyboard specific: \n"                                \
                        "Current mode:                      %s \n"             \
                        "Current LED state:                 %s \n"             \
                        "Printer specific: \n"                                 \
                        "Current port status:               %s \n"             \
                        "Capabilities string:               %s \n",
                        IdleTime,
                        ProtocolType,
                        Mode,
                        Led,
                        PortStatus,
                        (bCapabilities == TRUE) ?
                            (char *)pCapabilities->pBuffer : "Unsupport"
                        );

                if (pCapabilities != NULL)
                    OSS_FREE(pCapabilities);

                close(fd);
                }

            /* Ethernet specific print */

            if (bIsEnd == TRUE)
                {
                if(bEndFlag == FALSE)
                    strcpy(EndFlag, "Unsupport");
                else
                    sprintf(EndFlag, "0x%8.8X", uEndFlag);

                if(bEndBuf == FALSE)
                    strcpy(EndBuf, "Unsupport");
                else
                    sprintf(EndBuf, "0x%8.8X (%d)", uEndBuf, uEndBuf);

                if((bEndAddr == FALSE) && (pEndAddr != NULL))
                    strcpy(EndPhyAddr, "Unsupport");
                else
                    sprintf(EndPhyAddr, "%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",
                            pEndAddr[0], pEndAddr[1], pEndAddr[2],
                            pEndAddr[3], pEndAddr[4], pEndAddr[5]);

                fprintf(stdout,
                        "Ethernet specific: \n"                              \
                        "Device flag:                       %s \n"           \
                        "Minimum 1st buffer:                %s \n"           \
                        "Physical address:                  %s \n",
                        EndFlag,
                        EndBuf,
                        EndPhyAddr
                        );

                if(pEndAddr != NULL)
                    OSS_FREE(pEndAddr);
                }

            /* Mass Storage specific print */

            if (bIsMsc == TRUE)
                {
                fprintf(stdout,
                        "Mass Storage specific: \n"                          \
                        "Max lun:                           0x%2.2x \n",
                        pMscDevice->maxLun
                        );
                }

            pNode = (USB2_CLASS_NODE *) lstNext((NODE *) pNode);
            }
        }
    return;
    }


/*******************************************************************************
 * usbDeviceInfoShow - Show GEN2 class driver device descriptor
 *
 * This function prints out the various fields within a standard
 * device descriptor.
 *
 * Parameters:
 *   FILE * pLogFile,
 *   USB2_CLASS_DEVICE * pDevice
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbDeviceInfoShow
    (
    FILE *              pLogFile,
    USB2_CLASS_DEVICE * pDevice
    )
    {
    UINT32 hDevice = 0;
    UINT32 actLen = 0;
    UINT8 i = 0;
    USBHST_STATUS status = USBHST_FAILURE;
    USBHST_DEVICE_DESCRIPTOR * pDevDescr = NULL;
    char * pManufacturer = NULL;
    char * pProduct = NULL;
    char * pSerial = NULL;

    hDevice = pDevice->hDevice;
    actLen = sizeof(USBHST_DEVICE_DESCRIPTOR);

    /* Get the device descriptor and print out the device information */

    pDevDescr = (USBHST_DEVICE_DESCRIPTOR *)OS_MALLOC(actLen);

    if (pDevDescr == NULL)
        {
        DEBUGMSG("MALLOC failed for pDevDescr \n",1, 2, 3, 4, 5, 6);
        return;
        }

    status = usbHstGetDescriptor(hDevice,
                                 USBHST_DEVICE_DESC,
                                 0,
                                 0,
                                 &actLen,
                                 (UCHAR *)pDevDescr);

    if (status != USBHST_SUCCESS)
        {
        DEBUGMSG("Get device desc failed, hDevice=0x%x, status=%d\n",
                 hDevice, status, 3, 4, 5, 6);

        OS_FREE(pDevDescr);
        return;
        }

    /* Get string descriptors */

    pManufacturer = (char *)OS_MALLOC(USBHST_MAX_DESCRIPTOR_LEN);

    if (pManufacturer == NULL)
        {
        DEBUGMSG("MALLOC failed for pManufacturer \n",
                 1, 2, 3, 4, 5, 6);
        OS_FREE(pDevDescr);
        return;
        }

    pProduct = (char *)OS_MALLOC(USBHST_MAX_DESCRIPTOR_LEN);

    if (pProduct == NULL)
        {
        DEBUGMSG("MALLOC failed for pProduct \n",
                 1, 2, 3, 4, 5, 6);
        OS_FREE(pDevDescr);
        OS_FREE(pManufacturer);
        return;
        }

    pSerial = (char *)OS_MALLOC(USBHST_MAX_DESCRIPTOR_LEN);

    if (pSerial == NULL)
        {
        DEBUGMSG("MALLOC failed for pSerial \n",
                 1, 2, 3, 4, 5, 6);
        OS_FREE(pDevDescr);
        OS_FREE(pManufacturer);
        OS_FREE(pProduct);
        return;
        }

    getStringDescr(hDevice, pDevDescr->iManufacturer,
                   pManufacturer,USBHST_MAX_DESCRIPTOR_LEN);

    getStringDescr(hDevice, pDevDescr->iProduct,
                   pProduct,USBHST_MAX_DESCRIPTOR_LEN);

    getStringDescr(hDevice, pDevDescr->iSerialNumber,
                   pSerial,USBHST_MAX_DESCRIPTOR_LEN);

    bDeviceDescriptor = TRUE;

    fprintf(pLogFile, "\n"                                          \
            "Device Descriptor: \n"                                 \
            "bcdUSB:                            0x%4.4X \n"         \
            "bDeviceClass:                      0x%2.2X (%s) \n"    \
            "bDeviceSubClass:                   0x%2.2X (%s) \n"    \
            "bDeviceProtocol:                   0x%2.2X (%s) \n"    \
            "bMaxPacketSize0:                   0x%2.2X \n"         \
            "idVendor:                          0x%4.4X \n"         \
            "idProduct:                         0x%4.4X \n"         \
            "bcdDevice:                         0x%4.4X \n"         \
            "iManufacture:                      0x%2.2X %s \n"      \
            "iProduct:                          0x%2.2X %s \n"      \
            "iSerialNumber:                     0x%2.2X %s \n"      \
            "bNumberConfigurations:             0x%2.2X \n",
            FROM_LITTLEW(pDevDescr->bcdUSB),
            pDevDescr->bDeviceClass,
            getUSBClass(pDevDescr->bDeviceClass, -1, -1),
            pDevDescr->bDeviceSubClass,
            getUSBClass(pDevDescr->bDeviceClass, pDevDescr->bDeviceSubClass, -1),
            pDevDescr->bDeviceProtocol,
            getUSBClass(pDevDescr->bDeviceClass, -1, pDevDescr->bDeviceProtocol),
            pDevDescr->bMaxPacketSize,
            FROM_LITTLEW(pDevDescr->idVendor),
            FROM_LITTLEW(pDevDescr->idProduct),
            FROM_LITTLEW(pDevDescr->bcdDevice),
            pDevDescr->iManufacturer,
            pManufacturer,
            pDevDescr->iProduct,
            pProduct,
            pDevDescr->iSerialNumber,
            pSerial,
            pDevDescr->bNumConfigurations
            );
    for (i=0; i < pDevDescr->bNumConfigurations; i++)
        {
        usbConfigInfoShow(pLogFile, hDevice, i);
        }

    OS_FREE(pDevDescr);
    OS_FREE(pManufacturer);
    OS_FREE(pProduct);
    OS_FREE(pSerial);
    return;
    }


/*******************************************************************************
 * usbConfigInfoShow - Show GEN2 class driver configuration descriptor
 *
 * This function prints out the various fields within a standard
 * configuration descriptor.
 *
 * Parameters:
 *   FILE * pLogFile,
 *   UINT32 hDevice,
 *   UINT8  configIndex
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbConfigInfoShow
    (
    FILE * pLogFile,
    UINT32 hDevice,
    UINT8  configIndex
    )
    {
    UINT32 actLen = 0;
    USBHST_STATUS status = USBHST_FAILURE;
    USBHST_CONFIG_DESCRIPTOR * pConfigDescr = NULL;
    char * pConfigString = NULL;
    USBHST_CONFIG_DESCRIPTOR * pTemp = NULL;
    USBHST_INTERFACE_DESCRIPTOR * pInterfaceDescr = NULL;

    actLen = sizeof(USBHST_CONFIG_DESCRIPTOR);

    /* Get the config descriptor and print out the config information */

    pConfigDescr = (USBHST_CONFIG_DESCRIPTOR *)OS_MALLOC(actLen);

    if (pConfigDescr == NULL)
        {
        DEBUGMSG("MALLOC failed for pConfigDescr \n",
                 1, 2, 3, 4, 5, 6);
        return;
        }

    status = usbHstGetDescriptor(hDevice,
                                 USBHST_CONFIG_DESC,
                                 configIndex,
                                 0,
                                 &actLen,
                                 (UCHAR *)pConfigDescr);
    if (status != USBHST_SUCCESS)
        {
        DEBUGMSG("Get config desc failed, hDevice=0x%x, status=0x%x\n",
                 hDevice, status, 3, 4, 5, 6);

        OS_FREE(pConfigDescr);
        return;
        }

    actLen = OS_UINT32_LE_TO_CPU(pConfigDescr->wTotalLength);

    OS_FREE(pConfigDescr);
    pConfigDescr = NULL;
    status = USBHST_FAILURE;

    pConfigDescr = (USBHST_CONFIG_DESCRIPTOR *)OS_MALLOC(actLen);

    if (pConfigDescr == NULL)
        {
        DEBUGMSG("MALLOC failed for pConfigDescr \n",
                 1, 2, 3, 4, 5, 6);
        return;
        }

    status = usbHstGetDescriptor(hDevice,
                                 USBHST_CONFIG_DESC,
                                 configIndex,
                                 0,
                                 &actLen,
                                 (UCHAR *)pConfigDescr);
    if (status != USBHST_SUCCESS)
        {
        DEBUGMSG("Get total config desc failed, hDevice=0x%x, status=0x%x\n",
                 hDevice, status, 3, 4, 5, 6);

        OS_FREE(pConfigDescr);
        return;
        }

    pConfigString = (char *)OS_MALLOC(USBHST_MAX_DESCRIPTOR_LEN);

    if (pConfigString == NULL)
        {
        DEBUGMSG("MALLOC failed for pConfigString \n",
                 1, 2, 3, 4, 5, 6);
        OS_FREE(pConfigDescr);
        return;
        }

    getStringDescr(hDevice, pConfigDescr->iConfiguration,
                   pConfigString, USBHST_MAX_DESCRIPTOR_LEN);

    fprintf(pLogFile, "\n"                                                 \
            "Configuration Descriptor: \n"                                 \
            "wTotalLength:                      0x%4.4X \n"                \
            "bNumInterfaces:                    0x%2.2X \n"                \
            "bConfigurationValue:               0x%2.2X \n"                \
            "iConfiguration:                    0x%2.2X %s \n"             \
            "bmAttributes:                      0x%2.2X (%s %s)\n"      \
            "MaxPower:                          0x%2.2X (%d mA)\n",
            FROM_LITTLEW(pConfigDescr->wTotalLength),
            pConfigDescr->bNumInterfaces,
            pConfigDescr->bConfigurationValue,
            pConfigDescr->iConfiguration,
            pConfigString,
            pConfigDescr->bmAttributes,
            ((pConfigDescr->bmAttributes & 0x40) ? "Self Powered" : "Bus Powered"),
            ((pConfigDescr->bmAttributes & 0x20) ? "Remote Wakeup" : ""),
            pConfigDescr->MaxPower,
            pConfigDescr->MaxPower * 2
            );
    OS_FREE(pConfigString);

    pTemp = pConfigDescr;
    while ((pInterfaceDescr = usbDescrParseSkip((pUINT8 *)&pTemp,
                                                 (pUINT16)&actLen,
                                                 USBHST_INTERFACE_DESC))
            != NULL)
        {
        usbInterfaceInfoShow(pLogFile, hDevice, pInterfaceDescr);
        }

    OS_FREE(pConfigDescr);
    return;
    }


/*******************************************************************************
 * usbInterfaceInfoShow - Show GEN2 class driver interface descriptor
 *
 * This function prints out the various fields within a standard
 * interface descriptor.
 *
 * Parameters:
 *   FILE * pLogFile,
 *   UINT32 hDevice,
 *   USBHST_INTERFACE_DESCRIPTOR * pInterface
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbInterfaceInfoShow
    (
    FILE *                        pLogFile,
    UINT32                        hDevice,
    USBHST_INTERFACE_DESCRIPTOR * pInterface
    )
    {
    char * pInterfaceString = NULL;
    UINT8 i = 0;
    USBHST_INTERFACE_DESCRIPTOR * pTemp = NULL;
    USBHST_ENDPOINT_DESCRIPTOR * pEndpointDescr = NULL;
    USBHST_HID_DESCRIPTOR * pHidDescr = NULL;
    UINT16 actLen = 0;

    actLen = (UINT16)USBHST_MAX_DESCRIPTOR_LEN;

    pInterfaceString = (char *)OS_MALLOC(USBHST_MAX_DESCRIPTOR_LEN);

    if (pInterfaceString == NULL)
        {
        DEBUGMSG("MALLOC failed for pInterfaceString \n",
                 1, 2, 3, 4, 5, 6);
        return;
        }

    getStringDescr(hDevice, pInterface->iInterface,
                   pInterfaceString, USBHST_MAX_DESCRIPTOR_LEN);

    bDeviceDescriptor = FALSE;

    fprintf(pLogFile, "\n"                                              \
            "Interface Descriptor: \n"                                  \
            "bInterfaceNumber:                  0x%2.2X \n"             \
            "bAlternateSetting:                 0x%2.2X \n"             \
            "bNumberEndpoints:                  0x%2.2X \n"             \
            "bInterfaceClass:                   0x%2.2X (%s)\n"         \
            "bInterfaceSubClass:                0x%2.2X (%s)\n"         \
            "bInterfaceProtocol:                0x%2.2X (%s)\n"         \
            "iInterface:                        0x%2.2X %s \n",
            pInterface->bInterfaceNumber,
            pInterface->bAlternateSetting,
            pInterface->bNumEndpoints,
            pInterface->bInterfaceClass,
            getUSBClass(pInterface->bInterfaceClass,
                        -1,
                        -1),
            pInterface->bInterfaceSubClass,
            getUSBClass(pInterface->bInterfaceClass,
                        pInterface->bInterfaceSubClass,
                        -1),
            pInterface->bInterfaceProtocol,
            getUSBClass(pInterface->bInterfaceClass,
                        -1,
                        pInterface->bInterfaceProtocol),
            pInterface->iInterface,
            pInterfaceString
            );
    OS_FREE(pInterfaceString);

    if (pInterface->bInterfaceClass == 0x03)
        {
        pTemp = pInterface;
        if ((pHidDescr = usbDescrParseSkip ((pUINT8 *)&pTemp,
            (pUINT16)&actLen, USBHST_HID_DESC_TYPE)) != NULL)
            usbHidInfoShow(pLogFile, hDevice, pHidDescr);
        }

    pTemp = pInterface;
    for (i = 0; i < pInterface->bNumEndpoints; i++)
        {
        if ((pEndpointDescr = usbDescrParseSkip ((pUINT8 *)&pTemp,
            (pUINT16)&actLen, USBHST_ENDPOINT_DESC)) != NULL)
            usbEndpointInfoShow(pLogFile, hDevice, pEndpointDescr);
        }

    return;
    }


/*******************************************************************************
 * usbDeviceInfoShow - Show GEN2 class driver endpoint descriptor
 *
 * This functions prints out the various fields within a standard
 * endpoint descriptor.
 *
 * Parameters:
 *   FILE * pLogFile,
 *   UINT32 hDevice,
 *   USBHST_ENDPOINT_DESCRIPTOR * pEndpoint
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbEndpointInfoShow
    (
    FILE *                       pLogFile,
    UINT32                       hDevice,
    USBHST_ENDPOINT_DESCRIPTOR * pEndpoint
    )
    {
    char strAttribute[USBHST_MAX_DESCRIPTOR_LEN];

    switch (pEndpoint->bmAttributes & 0x03)
        {
        case USBHST_CONTROL_TRANSFER:
        strcpy(strAttribute,"Control");
        break;

        case USBHST_ISOCHRONOUS_TRANSFER:
        strcpy(strAttribute,"Isochronous");
        break;

        case USBHST_BULK_TRANSFER:
        strcpy(strAttribute,"Bulk");
        break;

        case USBHST_INTERRUPT_TRANSFER:
        strcpy(strAttribute,"Interrupt");
        break;

        default:
        strcpy(strAttribute,"Unknown Transfer Type");
        }
    fprintf(pLogFile, "\n"                                                      \
            "Endpoint Descriptor: \n"                                           \
            "bEndpointAddress:                  0x%2.2X "                       \
            "(Endpoint Number: 0x%2.2X, Endpoint Direction: %s) \n"             \
            "bmAttributes:                      0x%2.2X (Transfer Type: %s) \n" \
            "wMaxPacketSize:                    0x%4.4X \n"                     \
            "bInterval:                         0x%2.2X \n",
            pEndpoint->bEndpointAddress,
            pEndpoint->bEndpointAddress & 0x0f,
            (pEndpoint->bEndpointAddress & 0x80) ? "IN" : "OUT",
            pEndpoint->bmAttributes,
            strAttribute,
            FROM_LITTLEW(pEndpoint->wMaxPacketSize),
            pEndpoint->bInterval
            );

    return;
    }


/*******************************************************************************
 * usbDeviceInfoShow - Show GEN2 class driver HID descriptor
 *
 * This functions prints out the various fields within a class specific
 * HID descriptor.
 *
 * Parameters:
 *   FILE * pLogFile,
 *   UINT32 hDevice,
 *   USBHST_HID_DESCRIPTOR * pHid
 *
 * RETURNS: NA
 *
 * \NOMANUAL
 */

LOCAL void usbHidInfoShow
    (
    FILE *                  pLogFile,
    UINT32                  hDevice,
    USBHST_HID_DESCRIPTOR * pHid
    )
    {
    char classType[USBHST_MAX_DESCRIPTOR_LEN];

    switch (pHid->bClsDescriptorType)
        {
        case 0x21:
        strcpy(classType,"(HID)");
        break;

        case 0X22:
        strcpy(classType,"(Report)");
        break;

        case 0x23:
        strcpy(classType,"(Physical descriptor)");
        break;

        default:
        strcpy(classType,"");
        }
    fprintf(pLogFile, "\n"                                          \
            "HID Descriptor: \n"                                    \
            "bcdHID:                            0x%4.4X \n"         \
            "bCountryCode:                      0x%2.2X \n"         \
            "bNumDescriptors:                   0x%2.2X \n"         \
            "bDescriptorType:                   0x%2.2X %s \n"      \
            "wDescriptorLength:                 0x%4.4X \n",
            FROM_LITTLEW(pHid->bcdHID),
            pHid->bCountryCode,
            pHid->bNumDescriptors,
            pHid->bClsDescriptorType,
            classType,
            FROM_LITTLEW(pHid->wDescriptorLength)
            );

    return;
    }

#endif

#endif /* __INCusrUsbShowc */
