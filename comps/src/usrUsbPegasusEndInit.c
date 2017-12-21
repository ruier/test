/* usrUsbPegasusEndInit.c - Initialization of the USB END driver */

/*
 * Copyright (c) 2000-2005, 2007-2011, 2013 Wind River Systems, Inc.
 *
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
02o,06may13,s_z  Remove compiler warning (WIND00356717)
02n,16mar11,j_x  Add multiple inclusion guard symbol (WIND00247788)
02m,07dec10,ghs  Remove compile warning
02l,07sep10,ghs  Use "usb0" as usb pegasus default name
02k,02sep10,ghs  Use OS_THREAD_FAILURE to check taskSpawn failure (WIND00229830)
02j,01sep10,ghs  Change pegasus default name (WIND00231161)
02i,28jul10,ghs  Fix some spelling mistakes(WIND00223831)
02h,05jul10,m_y  Modify for coding convention
02g,05jul10,ghs  Modify display logs(WIND00216586)
02f,27apr10,j_x  Error debug adaption
02e,18mar10,j_x  Changed for USB debug (WIND00184542)
02d,13jan10,ghs  vxWorks 6.9 LP64 adapting
02c,10jun09,s_z  Remove the MsgQ(pegasusCallbackQueue) and pegasusClientThread,
                 instead of calling the process routine(
                 pegasusAttachCallbackInternal)directlly in pegasusAttachCallback
                 (WIND00169470)
02b,28aug08,s_z  usbPegasusDevTbl[unitNum] should be NULL when you got error
                 when initiliazation and plugging out(WIND00132505)
02a,27feb07,jrp  Fix for defect WIND00088255
01z,12dec05,ami  Fix for exception thrown on in-initialization for Driver with
                 Device Connected (SPR #112015)
01y,08sep05,ami  variable name taskID changed to a more meaning ful name
                 pegaususTaskId (SPR #111526)
01x,10aug05,ami  Deinitialization function added; Initializaiton function given
                 a return value
01w,21apr05,hch  Remove the muxAddrResFuncAdd since it is not necessary in
                 ethernet device driver
01v,18apr05,hch  Fix for multiple pegasus device from the same vendor
01u,07apr05,hch  Removed diab compiler warning with ipEtherResolvRtn
01t,03mar05,hch  Fix SPR #92207 for routeDelete error on T221.
                 Change the default PEGASUS_MAX_DEVS to 1
                 Remove PEGASUS_DESTINATION_ADDRESS parameter
                 Improved the finding pegaus device entry algorithm.
01s,23feb05,hch  Fixed SPR #105739 for multiple pegasus device support
01r,09feb05,hch  Fix the compiler warning for SPR #104605
01q,14jan05,hch  Fixed SPR #100806 on integrity checking
01p,13jan05,hch  Fixed SPR #98213 about missing ipDetach when device is removed
01o,09jan05,hch  Fixed SPR #105506 for vendor id and product id configuration
01n,05jan05,ami  while (condition) replaced with while (TRUE)
01m,28dec04,mta  SPR104608:Warning Removal
01l,15oct04,ami  Apigen Changes
01k,06jun04,hch  replace arpresolve with ipEtherResolvRtn
01j,16sep03,cfc  Merge changes from USB1.1 Cumulative Patch
01i,05sep03,jln  merge fix for SPR 88094
01h,05sep03,jac  Add IPv6 compatible code
01g,03sep03,jac  Add Pegasus II device and vendor IDs and make compatible with
                 IPv4/IPv6 dual mode stack
01f,25apr02,wef  add support for Micro Connector device.
01e,05mar02,wef  cleaned up printf's
01d,17dec01,wef  merge from veloce
01c,04dec01,wef  Pulled code from src/test/usb/usbPegasusTest.c here and cleaned
                 up
01b,10jun01,wef  moved end attach functionality to src/test/usb/usbPegasusTest.c
01a,23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB END pegasus driver.  This assumes the
USB host stack has already been initialized and has a host controller
driver attached.

INCLUDE FILES: ipProto.h, usb/ossLib.h, usb/usbQueueLib.h,
drv/usb/usbPegasusEnd.h

*/

#ifndef __INCusrUsbPegasusEndInitc
#define __INCusrUsbPegasusEndInitc


/* includes */

#include "ipProto.h"
#include "usb/ossLib.h"
#include "usb/usbQueueLib.h"
#include "drv/usb/usbPegasusEnd.h"

/* defines */

#define MAX_PEGASUS_LOADSTR_SZ   80

#ifndef PEGASUS_MAX_DEVS
#define PEGASUS_MAX_DEVS 1    /* Maximum Pegasus Device number */
#endif

#ifndef PEGASUS_IP_ADDRESS
#define PEGASUS_IP_ADDRESS    {"90.0.0.50"}  /* USB Pegasus Device
                                              * IP Address */
#endif

#ifndef PEGASUS_NET_MASK
#define PEGASUS_NET_MASK      {0xffff0000}   /* USB Pegasus Device
                                              * Net Mask */

#endif

#ifndef PEGASUS_TARGET_NAME
#define PEGASUS_TARGET_NAME   {"usbTarg0"}   /* USB Pegasus Device
                                              * Target Name */
#endif

#ifndef PEGASUS_END_NAME
#define PEGASUS_END_NAME      "usb"          /* UBS Pegasus Device End Name */
#endif

/* externals */

IMPORT END_OBJ *       usbPegasusEndLoad ();
IMPORT int             ifconfig();

/* globals */

char * usbPegasusEndName = PEGASUS_END_NAME;

/* locals */


LOCAL TASK_ID pegaususTaskId = OS_THREAD_FAILURE; /* variable to store the task ID */
                                        /* for device connect and disconnect */
                                        /* thread */

/* USB Pegasus Device IP Address array */

LOCAL const char * pegasusIpAdrs[PEGASUS_MAX_DEVS] =  PEGASUS_IP_ADDRESS;

/* USB Pegasus Device Net Mask array */

LOCAL const UINT32 pegasusNetMask[PEGASUS_MAX_DEVS] = PEGASUS_NET_MASK;

#if (!defined(INCLUDE_IPV4) && !defined(INCLUDE_IPV6))

/* USB Pegasus Device Target Name array */

LOCAL const char * pegasusTargetName[PEGASUS_MAX_DEVS] =
                             PEGASUS_TARGET_NAME;

#endif

/* Multiple Pegasus device support array */

LOCAL USBD_NODE_ID usbPegasusDevTbl[PEGASUS_MAX_DEVS];

LOCAL END_OBJ *    pEnd;

/******************************************************************************
*
* sysUsbEndPegasusLoad - load (create) USB END device
*
* This routine loads the usb end device with initial parameters specified by
* values given in the BSP configuration files (config.h).
*
* RETURNS: pointer to END object or ERROR.
*
* ERRNO: none
*/

END_OBJ * sysUsbPegasusEndLoad
    (
    char * pParamStr,   /* ptr to initialization parameter string */
    void * pDev         /* ptr to pegasus device */
    )
    {
    /*
     * The usbEnd driver END_LOAD_STRING should be:
     * "<vendorId>:<nodeId>:<noofRxbuffers>:<noofIRPs>"
     */

    char paramStr [END_INIT_STR_MAX];   /* from end.h */
    static char usbEndParam[MAX_PEGASUS_LOADSTR_SZ];

    if (strlen (pParamStr) == 0)
        {

        /*
         * muxDevLoad() calls us twice.  If the string is
         * zero length, then this is the first time through
         * this routine, so we just return the device name.
         */

        pEnd = usbPegasusEndLoad(pParamStr);
        }
    else
        {

        /*
         * On the second pass though here, we actually create
         * the initialization parameter string on the fly.
         * Note that we will be handed our unit number on the
         * second pass through and we need to preserve that information.
         * So we use the unit number handed from the input string.
         */

        if (pDev == NULL)
            return NULL;

        snprintf (paramStr, END_INIT_STR_MAX - 1, "%c:\n", * pParamStr);

        snprintf (usbEndParam,
                 MAX_PEGASUS_LOADSTR_SZ - 1,
                 "%d:%d:%d\n",
                 ((USB_PEGASUS_DEV *)pDev)->nodeId,
                 PEGASUS_NO_IN_BFRS,
                 PEGASUS_NO_OUT_IRPS);
        if (END_INIT_STR_MAX > strlen (usbEndParam))
           strncat(paramStr,usbEndParam, END_INIT_STR_MAX - strlen (paramStr) - 1);

        if ((pEnd = usbPegasusEndLoad (paramStr)) == (END_OBJ *)NULL)
            {
            USB1_END_ERR("usb network device failed usbEndLoad routine \n",
                         1, 2, 3, 4, 5, 6);
            }
        }

    return (pEnd);
    }



/******************************************************************************
*
* usbLoadPegasus - load (create) USB END device
*
* This function is taken from usrEndLibInit() and modified suitably to load
* the end driver when USB Ethernet device is dynamically attached or detached.
*
* RETURNS: OK or ERROR.
*
* ERRNO: none
*/

STATUS usbLoadPegasus
    (
    int               unitNum,
    USB_PEGASUS_DEV * pDev
    )
    {

    END_OBJ*      pCookie = NULL;

    pCookie = (END_OBJ *) muxDevLoad(unitNum,
                                     sysUsbPegasusEndLoad,
                                     NULL,
                                     1,
                                     (void *)pDev);

    if (pCookie == NULL)
        {
        USB1_END_ERR("invalid parameter of pCookie \n",
                     1, 2, 3, 4, 5, 6);
        return(ERROR);
        }
    else
        {
        if (muxDevStart(pCookie) == ERROR)
            {
            USB1_END_ERR("muxDevStart failed for Pegasus device \n",
                         1, 2, 3, 4, 5, 6);
            return(ERROR);
            }
        }

    return (OK);

    }

/******************************************************************************
*
* usbPegasusEndStart -  make the network device operational
*
* After muxDevStart has been executed, this then attaches an IP address
* to the USB device and then starts the it.
*
* RETURNS: N/A
*
* ERRNO: none
*/

void usbPegasusEndStart
    (
    int       unitNum,          /* unit number */
    char *    pAddrString,      /* enet address */
    int       netmask           /* netmask */
    )
    {
    M2_INTERFACETBL  endM2Tbl;

    pEnd = endFindByName (usbPegasusEndName, unitNum);

    if (pEnd == NULL)
        {
        USB1_END_ERR("Could not find %s%d \n",
                     usbPegasusEndName, unitNum, 3, 4, 5, 6);
        return;
        }

    if (muxIoctl(pEnd, EIOCGMIB2, (caddr_t) &endM2Tbl) == ERROR)
        {
        USB1_END_ERR("Cannot perform EIOCMIB2 ioctl \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    if (ipAttach(unitNum, usbPegasusEndName) != OK)
        {
        USB1_END_ERR("Failed to attach to device %s \n",
                     usbPegasusEndName, 2, 3, 4, 5, 6);
        return;
        }

#if (defined(INCLUDE_IPV4) || defined(INCLUDE_IPV6))
    {
    char buf[64];

#ifdef INCLUDE_IPV4
    /*
     * Create the ifconfig string.
     *
     * Convert the netmask to a form of 255.255.255.255 to conform with
     * ipnet
     */
    snprintf(buf, 63, "%s%d inet up add %s netmask %3.3d.%3.3d.%3.3d.%3.3d",
            usbPegasusEndName, unitNum, pAddrString,
            ((netmask & 0xFF000000)>>24), ((netmask & 0xFF0000) >> 16),
            ((netmask & 0xFF00)>>8), (netmask & 0xFF));

    if (ifconfig(buf) == ERROR)
        {
        USB1_END_ERR("Failed to configure %s%d for IPv4 TCP/IP stack \n",
                     usbPegasusEndName, unitNum, 3, 4, 5, 6);
        return;
        }
#endif

#if (defined(INCLUDE_IPV6) && defined(INCLUDE_USB_PEGASUS_END_IPV6_INIT))

    snprintf(buf, 63, "%s%d inet6 add %s prefixlen %d",
            usbPegasusEndName,
            unitNum,
            PEGASUS_IPV6_ADDRESS,
            PEGASUS_PREFIXLEN);

    if (ip6Attach(unitNum, usbPegasusEndName) != OK)
        {
        USB1_END_ERR("Failed to attach IPv6 TCP/IP stack to device %s \n",
                     usbPegasusEndName, 2, 3, 4, 5, 6);
        return;
        }

    if (ifconfig(buf) == ERROR)
        {
        USB1_END_ERR("Failed to configure %s%d for IPv6 TCP/IP stack \n",
                     usbPegasusEndName, unitNum, 3, 4, 5, 6);
        return;
        }
#endif

    snprintf(buf, 63, "%s%d up", usbPegasusEndName, unitNum);
    if (ifconfig(buf) == ERROR)
        {
        USB1_END_ERR("Failed to bring %s up \n",
                     buf, 2, 3, 4, 5, 6);
        return;
        }
    }
#else
    if (usrNetIfConfig (usbPegasusEndName,
                        unitNum,
                        pAddrString,
                        (char *)pegasusTargetName[unitNum],
                        (int)netmask)
                        != OK)
        {
        USB1_END_ERR("Failed to configure %s%d for IP \n",
                     usbPegasusEndName, unitNum, 3, 4, 5, 6);
        return;
        }
#endif

    USB1_END_INFO("Attached TCP/IP interface to %s unit %d \n",
                  usbPegasusEndName, unitNum, 3, 4, 5, 6);
    }


/***************************************************************************
*
* pegasusAttachCallbackInternal - handles device insertions / removals.
*
* This routine is the configuration level handler that monitors device
* insertions and removals.  Upon device connection, it starts the USB
* network device.
*
* This routine is changed from pegasusClientThread to the process function
* which will be called directlly in pegasusAttachCallback routine.
*
* RETURNS: N/A
*
* ERRNO: none
*/

LOCAL void pegasusAttachCallbackInternal
    (
    UINT16            attachCode,    /* Attach or Detach */
    USB_PEGASUS_DEV * pDev           /* Device pointer */
    )
    {
    int index, unitNum;
    int noOfSupportedDevices = (sizeof (pegasusAdapterList) /
                                 (3 * sizeof (UINT16)));

    /* Perameter verification */

    if (pDev == NULL || (pDev->vendorId == 0 && pDev->productId == 0))
        {
        return;
        }

    for (index = 0; index < noOfSupportedDevices; index++)
        {
        if (pDev->vendorId == pegasusAdapterList[index][0] &&
            pDev->productId == pegasusAdapterList[index][1])
            {
            break;
            }
        }
    if (index == noOfSupportedDevices)
        {
        USB1_END_ERR("Unsupported Device \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Device is connected */

    if (attachCode == USB_PEGASUS_ATTACH)
        {
        USB1_END_DBG("%s[L%d]: Loading Pegasus Device \n",
                     __FILE__, __LINE__, 3, 4, 5, 6);
        /* Find unused device */

        for (unitNum = 0; unitNum < PEGASUS_MAX_DEVS; unitNum++)
            {
            if (usbPegasusDevTbl[unitNum] == 0)
                break;
            }

        /* continue the loop if no devices are available in table */

        if (unitNum == PEGASUS_MAX_DEVS)
            {
            USB1_END_ERR("Maximum number of USB Pegasus Device exceeded \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }

        if (usbPegasusDevLock ((USBD_NODE_ID) pDev->nodeId) != OK)
            {
            USB1_END_ERR("usbPegasusDevLock() returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            }
        else
            {

            /* Store the info about the newly found device */

            usbPegasusDevTbl[unitNum] = pDev->nodeId;

            if (usbLoadPegasus(unitNum, pDev) == OK)
                {

                /* Attach IP address*/

                usbPegasusEndStart(unitNum,
                                   (char *)pegasusIpAdrs[unitNum],
                                   (int)pegasusNetMask[unitNum]);
                }
            else
                {
                usbPegasusDevTbl[unitNum] = 0;
                USB1_END_ERR("usbLoadPegasus() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }
            }
        }

    /* Device was removed */

    else if (attachCode == USB_PEGASUS_REMOVE)
        {
        USB1_END_DBG("%s[L%d]: Pegasus Device Unload in progress \n",
                     __FILE__, __LINE__, 3, 4, 5, 6);
        /* Find device in table*/

        for (unitNum = 0; unitNum < PEGASUS_MAX_DEVS; unitNum++)
            {
            if (usbPegasusDevTbl[unitNum] == (USBD_NODE_ID)pDev->nodeId)
                break;
            }

        /* continue the loop if no devices are available in table */

        if (unitNum == PEGASUS_MAX_DEVS)
            {
            USB1_END_ERR("No entry are found initialized for this device \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        if (usbPegasusDevUnlock ((USBD_NODE_ID) pDev->nodeId) != OK)
            {
            USB1_END_ERR("usbPegasusDevUnlock() returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            }

#if (defined(INCLUDE_IPV4) || defined(INCLUDE_IPV6))
        {
        char buf[16];

        snprintf(buf, 15, "%s%d down", usbPegasusEndName, unitNum);

        if (ifconfig(buf) == ERROR)
            {
            USB1_END_ERR("Failed to bring %s down \n",
                         buf, 2, 3, 4, 5, 6);
            }
        }
#endif

        if (ipDetach(unitNum, usbPegasusEndName) != OK)
            {
            USB1_END_ERR("Failed to detach device %s \n",
                         usbPegasusEndName, 2, 3, 4, 5, 6);
            }

        if (muxDevUnload(usbPegasusEndName, unitNum) != OK)
            {
            USB1_END_ERR("muxDevUnload failed \n",
                         1, 2, 3, 4, 5, 6);
            }

        /* Mark the table entry as not used */

        usbPegasusDevTbl[unitNum] = 0;

        USB1_END_INFO("Pegasus Device Unloaded successfully\n",
                      1, 2, 3, 4, 5, 6);
        }
    }


/***************************************************************************
*
* usbPegasusAttachCallback - configuration level callback
*
* When a Pegasus device is inserted, this will get called.  It passes the
* attach message to tUsbPgs task.
*
* RETURNS: N/A
*
* ERRNO: none
*/

VOID usbPegasusAttachCallback
    (
    pVOID             arg,          /* caller-defined argument */
    USB_PEGASUS_DEV * pDev,         /* pointer to Pegasus Device */
    UINT16            attachCode    /* attach code */
    )

    {

    /*
    * In this routine, call the process routine directly to replace the
    * the MsgQ policy.
    */

    pegasusAttachCallbackInternal (attachCode, pDev);
    }

/*****************************************************************************
*
* usrUsbPegasusEndInit - initialize the USB END Pegasus driver
*
* This function initializes the USB END Pegasus driver and spawns a task
* to manage device insertion / removals.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

STATUS usrUsbPegasusEndInit (void)
    {
    int     cnt;

    if (usbPegasusEndInit () != OK)
        {
        USB1_END_ERR("usrUsbPegasusEndInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Initialize multi device support */

    for (cnt = 0; cnt < PEGASUS_MAX_DEVS; cnt++ )
        {
        usbPegasusDevTbl[cnt] = 0;
        }

    if (usbPegasusDynamicAttachRegister (usbPegasusAttachCallback, NULL) != OK)
        {
        USB1_END_ERR("usbPegasusDynamicAttachRegister() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    USB1_END_INFO("USB END Pegasus Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;
    }


/*****************************************************************************
*
* usrUsbPegasusEndDown - un-initialize the USB END Pegasus driver
*
* This function un-intializes the pegasus end driver. It also unregisters the
* callback function.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

STATUS usrUsbPegasusEndDown (void)
    {

    int     cnt = 0;        /* counter */

    /* unregister the callback function */

    if (usbPegasusDynamicAttachUnregister (usbPegasusAttachCallback, NULL) ==
        ERROR)

        {

        /* Error unregistering the attach callback function */

        USB1_END_ERR("Error unregistering the attach callback function \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /*
     * wait till the attach callback gets called with attach code as
     * USB_PEGASUS_REMOVE and the device is removed.
     */

     while (TRUE)
        {
        /* all table entries usbPegasusDevTbl [] to become NULL */

        for (cnt = 0; cnt < PEGASUS_MAX_DEVS; cnt++)
            {
            if (usbPegasusDevTbl[cnt] != 0)
                goto LOOP;
            }
        break;
LOOP:
        sleep (1);
        }

    /* destroy the thread handling device connection and disconnection */

    if (taskDelete (pegaususTaskId) == ERROR)
        {

        /* Error destroying the task */

        USB1_END_ERR("Error destroying the task \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    pegaususTaskId = (TASK_ID) NULL;


    /* uninitialize the pegasus class driver file */

    if (usbPegasusEndUninit() != OK)

        {
        USB1_END_ERR("Error uninitializing the pegasus class driver \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    return OK;
    }


#endif /* __INCusrUsbPegasusEndInitc */


