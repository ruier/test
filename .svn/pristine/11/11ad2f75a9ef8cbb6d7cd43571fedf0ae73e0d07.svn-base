/* usrUsbBulkDevInit.c - USB Mass Storage Bulk driver initialization */

/*
 * Copyright (c) 2000-2001, 2003-2006, 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
03d,06may13,s_z  Remove compiler warning (WIND00356717)
03c,29feb12,ljg  Correct bio chain (WIND00335705)
03b,06sep11,ljg  Add timeout when take semaphore in usbBulkXbdEject (WIND00243983)
03a,25aug11,ljg  Release usbBulkXbdMutex in usbBulkXbdStrategy (WIND00293322)
02z,18apr11,j_x  Mis-ordered set of inputs to the memset() function (WIND00267485)
02y,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
02x,10nov10,ghs  Change fsmName size to avoid buffer overflow (WIND00240908)
02w,02sep10,ghs  Use OS_THREAD_FAILURE to check taskSpawn failure (WIND00229830)
02v,30jul10,m_y  Modify to remove compile warning
02u,21jul10,m_y  Move the action of destroy device structure into the
                 bulkClientThread (WIND00183499)
02t,05jul10,m_y  Modify routine bulkUnmountDrive to improve usb stack
                 robustness (WIND00183499)
02s,10jun10,ghs  Do not add any bio when device is unmounting(WIND00210276)
02r,11may10,ghs  Check return error after read or write(WIND00201676)
02q,27apr10,j_x  Error debug adaption
02p,21apr10,ghs  Do not mount drive when device is unmounting(WIND00207995)
02o,21mar10,w_x  Fix deadlock between tErfTask and tBulkClnt (WIND00200901)
02n,18mar10,j_x  Changed for USB debug (WIND00184542)
02m,13jan10,ghs  vxWorks 6.9 LP64 adapting
02l,09sep09,w_x  Adjusted usbBulkXbdStrategy semapahore give order (WIND00174505)
02k,04sep08,w_x  Added mutex to sync up eject and device removal (WIND00131719)
02j,02sep08,w_x  Added devUnmap after xbdDetach when unmounting (WIND00131719)
02i,29aug08,w_x  Added flag to stop further IO when unmounting device (WIND00131719)
02h,17jul08,w_x  Never break out tBulkClnt task
02g,29aug06,dee  WIND00063436 - bio's never properly queued in
                 usbBulkXbdStrategy()
02f,07oct05,hch  Add USB_BULK_NON_REMOVABLE_DISK configuration parameter
                 Add usbBulkStatusCheckBypass usbBulkStatusCheckBypassSet funcs
02e,21sep05,hch  Add zero size disk support and XBD instantiation
                 Fix SPR #112282 and SPR #111530
02d,31aug05,ami  Typo in the function bulkUnMountDrive rectified (SPR#111718)
02c,24aug05,ami  Name of variable taskId changed as bulkTaskId
02b,10aug05,hch  Added the fsm name mapping to maintain backward compatibility
02a,08aug05,hch  Modified the usrUsbBulkDevDown for the XBD uninitialization
01z,08aug05,ami  Add usrUsbBulkDevDown to uninitialize the bulk driver
01y,22jul05,hch  Incorporate review comments for USB XBD implementaion.
01x,10jun05,hch  Change to XBD model for removable file system support
01w,10jan05,ami  usrUsbRawWrite Function Added (SPR #96907 Fixed)
01v,03jan05,ami  SPR #104762 fix
01u,28dec04,mta  SPR104541:Warning Removal
01t,21dec04,mta  SPR104531:Warning Removal
01s,15oct04,ami  Apigen Changes
01r,04oct04,pdg  SPR 96905 fix
01q,18aug04,hch  fix compiler warning
01p,16jul04,hch  changed the modification history for coding convention
01o,03jun04,hch  fix warning for iosDevFind
01n,26feb04,cfc  Merge fix for SPR93796 from adh
01m,05dec03,cfc  Merge support for Multi-LUN devices
01l,07nov03,mdg  added support for multiple logical units and show functions.
01k,22oct03,cfc  code cleanup
01j,17oct03,cfc  Fix for Cache Creation
01i,16sep03,cfc  Merge from USB1.1 cumulative patch
01h,16sep03,tor  Fixes for SPR #90460, #90480, #90491 per Sony NSCE.
01g,08sep03,jln  fix wrong argument for iosDevFind
01f,06sep03,jln  merge fixes for SPR 90460, 89384, 85490, 88739
01g,11sep03,???  changed the priority of tBulkCnt task
01f,06sep03,cfc  Fix for unmounting drive
01e,20dec01,wef  declare usrFdiskPartRead.
01d,07dec01,wef  Fixed more warnings.
01c,13nov01,wef  Removed warnings, added Cstruct bio layer for dosFs2.
01b,12apr01,wef  added support for multiple devices, added new param to
                 usbBulkBlkDevCreate () for read/ write type.
01a,10dec00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB Mass Storage (Bulk-Only) class
driver and places it in the driver table.  On boot, it can be refered to by
the name given specified by the BULK_DRIVE_NAME configuration parameter.
This configlette assumes the USB host stack has already been initialized and
has a host controller driver attached.

INCLUDE FILES: usb/usbdLib.h, usb/usbQueueLib.h,usbBulkDevLib.h,
usbBulkDevLibP.h, fsMonitor.h

*/

#ifndef __INCusrUsbBulkDevInitc
#define __INCusrUsbBulkDevInitc

/* includes */
#include <usb/usbdLib.h>
#include <usb/usbQueueLib.h>
#include <usb/usbOsal.h>
#include <drv/usb/usbBulkDevLib.h>
#include <drv/usb/usbBulkDevLibP.h>
#include <drv/xbd/bio.h>
#include <drv/manager/device.h>
#include <fsMonitor.h>

/* defines */

#ifndef BULK_MAX_DEVS
#define BULK_MAX_DEVS            2         /* Maximum number of bulk devices */
#endif

#ifndef BULK_MAX_DRIVE
#define BULK_MAX_DRIVE           MAX_LUN  /* Maximum number of logical units */
#endif

#ifndef BULK_DRIVE_NAME
#define BULK_DRIVE_NAME          "/bd"                  /* Device name base */
#endif

#ifndef BULK_MAX_DRV_NAME_SZ
#define BULK_MAX_DRV_NAME_SZ      20                    /* Max size of device name */
#endif

#ifndef USB_BULK_NON_REMOVABLE_DISK
#define USB_BULK_NON_REMOVABLE_DISK FALSE
#endif

#undef DEBUG_USB_RAW            /* To be defined only if usrUsbRawRead
                                 * and usrUsbRawWrite functions are used
                                 */

/* Default wait time for XBD sync up semaphore */

#ifndef USB_MSC_XBD_SYNC_WAIT_TIMEOUT
#define USB_MSC_XBD_SYNC_WAIT_TIMEOUT \
    OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(5000)
#endif

/* locals */


LOCAL QUEUE_HANDLE      bulkCallbackQueue;

LOCAL USB_MESSAGE       bulkDeviceStatus;

LOCAL TASK_ID           bulkTaskId = OS_THREAD_FAILURE;   /* ID for task */

USBD_NODE_ID            bulkNodeId;

/* Multiple Bulk device support type */
typedef struct USBBULKDRIVE
    {
    char              usbBulkDrvName[BULK_MAX_DRIVE][BULK_MAX_DRV_NAME_SZ];
    XBD  *            pusbBulkXbdDev[BULK_MAX_DRIVE];
    USBD_NODE_ID      usbBulkDrvXbdNodeId;
    UINT8             usbBulkDrvNumLun;
    } UsbBulkDevice;

LOCAL UsbBulkDevice  usbBulkDeviceTbl[BULK_MAX_DEVS];

LOCAL BOOL usbBulkStatusCheckBypassFlag = USB_BULK_NON_REMOVABLE_DISK;

LOCAL int usbBulkXbdStrategy  (XBD *, struct bio *);
LOCAL int usbBulkXbdLunDump   (XBD *, sector_t, void *, size_t);
LOCAL void usbBulkXbdService   (pUSB_BULK_DEV_XBD_LUN);
LOCAL struct bio * usbBulkGetNextBio (pUSB_BULK_DEV_XBD_LUN);
LOCAL void usbBulkExecBio  (pUSB_BULK_DEV_XBD_LUN, struct bio *);
LOCAL VOID bulkAttachCallback (pVOID, USBD_NODE_ID, UINT16);
LOCAL VOID bulkClientThread (void);
LOCAL STATUS bulkMountDrive (USBD_NODE_ID, UINT8, UsbBulkDevice *);
LOCAL STATUS bulkUnmountDrive (USBD_NODE_ID, UINT8,  UsbBulkDevice *);
LOCAL int findPerDevId (USBD_NODE_ID);
LOCAL int findUnusedDevice (void);
LOCAL void usbBulkXbdCreateSyncHandler (UINT16, UINT16, void *, void *);

LOCAL struct xbd_funcs usbBulkXbdLunFuncs =
    {
    usbBulkDevIoctl,
    usbBulkXbdStrategy,
    usbBulkXbdLunDump,
    };

/* externals */

LOCAL MUTEX_HANDLE bulkXbdMutex;


/*************************************************************************
*
* bulkMountDrive - mounts a drive to the DOSFS.
*
* if lunNumber = 0xff then mount on all possible LUN
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL STATUS bulkMountDrive
    (
    USBD_NODE_ID   nodeId,               /* attach code */
    UINT8          lun,                  /* lun number to mount */
    UsbBulkDevice *pBulkDevice           /* USB Bulk Device Ptr  */
    )

    {
    device_t retVal;
    USB_BULK_DEV_XBD_LUN * pBulkDevLun;
    XBD *                  pBulkXbdDev;

    /* Create the block device with in the driver */

    pBulkXbdDev =  (XBD *) usbBulkBlkDevCreate (nodeId,
                                                lun,
                                                0,
                                                0,
                                                USB_SCSI_FLAG_READ_WRITE10);

    if (pBulkXbdDev == NULL)
        {
        USB1_BLK_ERR("usbBulkBlkDevCreate() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    pBulkDevLun = (USB_BULK_DEV_XBD_LUN *) pBulkXbdDev;

    pBulkDevice->pusbBulkXbdDev[lun] = pBulkXbdDev;

    /* If this lun is unmounting, do not mount it any more */

    if (pBulkDevLun->unmounting == TRUE)
        return ERROR;

    /* register new event handler for base device synchronization */
    if (erfHandlerRegister (xbdEventCategory, xbdEventInstantiated,
                            usbBulkXbdCreateSyncHandler,pBulkDevLun,
                            0) != OK)
        {
        return (ERROR);
        }

    /* Spawn the sevice task for this XBD */
    if ((pBulkDevLun->usbBulkXbdService = taskSpawn("tUsbBulkXbdService",
                                                    150,
                                                    0,
                                                    4096,
                                                    (FUNCPTR)usbBulkXbdService,
                                                    (ULONG)pBulkDevLun,
                                                    0,0,0,0,0,0,0,0,0))
                                                == OS_THREAD_FAILURE)
        {
        USB1_BLK_ERR("usbBulkXbdService taskSpawn returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        erfHandlerUnregister(xbdEventCategory, xbdEventInstantiated,
                             usbBulkXbdCreateSyncHandler, pBulkDevLun);
        return ERROR;
        }

    if (pBulkDevLun->usbBulkXbdInserted == TRUE)
        {
        /* full size disk file system initialization */
        if (xbdAttach(pBulkXbdDev, &usbBulkXbdLunFuncs,
                      pBulkDevice->usbBulkDrvName[lun],
                      pBulkXbdDev->xbd_blocksize,
                      pBulkXbdDev->xbd_nblocks, &retVal) == OK)
            {
            erfEventRaise(xbdEventCategory, xbdEventPrimaryInsert,
                              ERF_ASYNC_PROC, (void *)(ULONG)retVal, NULL);
            semTake (pBulkDevLun->usbBulkXbdSyncId, WAIT_FOREVER);
            }
        else
            {
            USB1_BLK_ERR("xbdAttach returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                                  usbBulkXbdCreateSyncHandler, pBulkDevLun);
            taskDelete (pBulkDevLun->usbBulkXbdService);
            pBulkDevLun->usbBulkXbdService = OS_THREAD_FAILURE;
            return ERROR;
            }
         }
     else
        {
        /* 0 size XBD instantiation */
        if (xbdAttach(pBulkXbdDev, &usbBulkXbdLunFuncs,
            pBulkDevice->usbBulkDrvName[lun],
            512,0, &retVal) == OK)
            {
            erfEventRaise(xbdEventCategory, xbdEventPrimaryInsert,
                           ERF_ASYNC_PROC, (void *)(ULONG)retVal, NULL);
            semTake (pBulkDevLun->usbBulkXbdSyncId,WAIT_FOREVER);
            }
         else
            {
            USB1_BLK_ERR("xbdAttach returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
            usbBulkXbdCreateSyncHandler, pBulkDevLun);
            taskDelete (pBulkDevLun->usbBulkXbdService);
            pBulkDevLun->usbBulkXbdService = OS_THREAD_FAILURE;
            return ERROR;
            }
        }
    return OK;
    }

/*************************************************************************
*
* bulkUnmountDrive - unmounts a drive to the DOSFS.
*
* This function unmounts the drive from the DOSFS File System
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL STATUS bulkUnmountDrive
    (
    USBD_NODE_ID   nodeID,           /* node ID */
    UINT8          lun,              /* lun number to unmount */
    UsbBulkDevice *pBulkDevice       /* USB Bulk Device Ptr  */
    )
    {
    XBD * pUsbBulkXbdDev = pBulkDevice->pusbBulkXbdDev[lun];
    pUSB_BULK_DEV_XBD_LUN pUsbBulkDevLun =
                                        (pUSB_BULK_DEV_XBD_LUN)(pUsbBulkXbdDev);
    struct bio * bio;

    /* file system uninitalization */

    if (pUsbBulkXbdDev != NULL)
        {
        /* Take the mutex before unmounting the device */
        if (pUsbBulkDevLun->usbBulkEjectMutex != NULL)
            {
            semTake(pUsbBulkDevLun->usbBulkEjectMutex,WAIT_FOREVER);
            }

        /* Set the unmounting flag to stop any further IO */
        pUsbBulkDevLun->unmounting = TRUE;

        erfEventRaise(xbdEventCategory, xbdEventRemove,
                        ERF_ASYNC_PROC,
                        (void *)(ULONG)pUsbBulkXbdDev->xbd_dev.dv_dev,
                        NULL);

        xbdDetach (pUsbBulkXbdDev);

        devUnmap((struct device *)pUsbBulkXbdDev);

        /* Execute all the remaining bios and wait for more bios when dv_dev is
         * not NULLDEV
         */
        while (((bio = usbBulkGetNextBio(pUsbBulkDevLun))!=NULL) ||
               (pUsbBulkXbdDev->xbd_dev.dv_dev != NULLDEV))
            {
            if (bio != NULL)
                bio_done(bio, ENXIO);
            else
                taskDelay(1);
            }

        /* before deleting the usbBulkXbdService Task, wait for all BIO's to
         * be serviced.
         * This is important because it may be possible that while we are
         * deleting the thread some BIOs are submitted but yet to be serviced.
         */

        while (pUsbBulkDevLun->bioStatus)
            taskDelay (1);

        if (pUsbBulkDevLun->usbBulkXbdInstantiated != TRUE)
            {
            erfHandlerUnregister (xbdEventCategory,
                                  xbdEventInstantiated,
                                  usbBulkXbdCreateSyncHandler,
                                  pUsbBulkDevLun);
            }


        if (pUsbBulkDevLun ->usbBulkXbdService != OS_THREAD_FAILURE
            && pUsbBulkDevLun ->usbBulkXbdService != 0)
            {
            taskDelete(pUsbBulkDevLun->usbBulkXbdService);
            pUsbBulkDevLun->usbBulkXbdService = OS_THREAD_FAILURE;
            }

        pBulkDevice->pusbBulkXbdDev[lun] = NULL;

        /* Release the mutex */
        if (pUsbBulkDevLun->usbBulkEjectMutex != NULL)
            {
            semGive(pUsbBulkDevLun->usbBulkEjectMutex);
            }
        }

    return OK;
    }

/*************************************************************************
*
* bulkAttachCallback - user attach callback for USB BULK class driver
*
* This is a callback function for USB BULK class driver
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL VOID bulkAttachCallback
    (
    pVOID        arg,           /* caller-defined argument */
    USBD_NODE_ID nodeId,        /* pointer to BULK Device */
    UINT16       attachCode     /* attach code */
    )
    {

    usbQueuePut (bulkCallbackQueue,
                0,                     /* msg */
                attachCode,            /* wParam */
                (UINT32) nodeId,       /* lParam */
                5000);

    }

/***************************************************************************
*
* bulkClientThread - handle control of drives being plugged / unplugged
*
* This function controls what happens when a new drive gets plugged in
* or when an existing drive gets removed.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL VOID bulkClientThread(void)
    {
    UINT8 lunNumber;
    UINT8 lun;
    int   tableIndex;

    /* wait for attach/detach events */
    while (TRUE)
        {
       /*
        * the queue parameters will be:
        * lParam = nodeId
        * wParam = attach code
        */

        usbQueueGet (bulkCallbackQueue, &bulkDeviceStatus, OSS_BLOCK);


        /* If attached. Only one device is supported at a time */
        OSS_MUTEX_TAKE(bulkXbdMutex, OSS_BLOCK);

        if (bulkDeviceStatus.wParam == USB_BULK_ATTACH)
            {
            bulkNodeId = (USBD_NODE_ID) bulkDeviceStatus.lParam;

            /* Find the total number of logical units that this USB mass
             * storage device supports
             */

            lunNumber = usbBulkGetMaxLun(bulkNodeId) + 1;

            USB1_BLK_INFO("Found bulk device with %d logical units at node %x \n",
                          lunNumber, bulkNodeId, 3, 4, 5, 6);

            /* Find unused device */
            tableIndex = findUnusedDevice();

            /*
             * release semaphore and continue the loop if no devices
             * are available in table
             */
            if (tableIndex == 0xFF)
                {
                USB1_BLK_ERR("Maximum number of USB Bulk Mass Storage "
                             "devices exceeded \n",
                             1, 2, 3, 4, 5, 6);

                OSS_MUTEX_RELEASE(bulkXbdMutex);
                continue;
                }

            /* Lock the device for protection */
            if (usbBulkDevLock (bulkNodeId) != OK)
                {
                USB1_BLK_ERR("usbBulkDevLock() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                OSS_MUTEX_RELEASE(bulkXbdMutex);
                /*
                * Do not break out this control task, we are expected to be
                * always online to process mounting and unmounting devices
                */
                continue;
                }

            /* Store the info about the newly found device */
            usbBulkDeviceTbl[tableIndex].usbBulkDrvXbdNodeId = bulkNodeId;
            usbBulkDeviceTbl[tableIndex].usbBulkDrvNumLun = lunNumber;

            /* Mount the drives, if there are any cards inserted */
            for (lun = 0; lun < lunNumber; lun++)
                {

                /* Mount the drive to the DOS file system */
                if (bulkMountDrive(bulkNodeId, lun,
                    &usbBulkDeviceTbl[tableIndex]) == ERROR)
                    {
                    USB1_BLK_ERR("Failed to mount %s \n",
                                 usbBulkDeviceTbl[tableIndex].usbBulkDrvName[lun],
                                 2, 3, 4, 5, 6);
                    /* Don't bother trying to mount the rest */
                    /* break; */
                    }
                }
            }

        /* Device was removed */

        else if (bulkDeviceStatus.wParam == USB_BULK_REMOVE)
            {
            bulkNodeId = (USBD_NODE_ID) bulkDeviceStatus.lParam;

            /* Find device in table */
            tableIndex = findPerDevId(bulkNodeId);

            /*
             * release semaphore and continue the loop if no devices
             * are available in table
             */

            if (tableIndex == 0xFF)
                {
                OSS_MUTEX_RELEASE(bulkXbdMutex);
                continue;
                }

            USB1_BLK_INFO("Bulk device with %d logical units at node %x detached \n",
                          usbBulkDeviceTbl[tableIndex].usbBulkDrvNumLun,
                          bulkNodeId, 3, 4, 5, 6);

            if (usbBulkDeviceTbl[tableIndex].usbBulkDrvXbdNodeId != 0)
                {
                for (lun = 0; lun <
                    usbBulkDeviceTbl[tableIndex].usbBulkDrvNumLun; lun++)
                    {
                    /* Unmount the drive to the DOS file system */
                    if (bulkUnmountDrive(bulkNodeId, lun,
                        &usbBulkDeviceTbl[tableIndex]) != OK)
                        USB1_BLK_ERR("bulkUnmountDrive() returned ERROR \n",
                                     1, 2, 3, 4, 5, 6);
                    }

                /* Mark the table entry as not used */
                usbBulkDeviceTbl[tableIndex].usbBulkDrvXbdNodeId = 0;
                }

            /* Unlock the BULK device structure, so that it gets destroyed */
            if (usbBulkDevUnlock (bulkNodeId) != OK)
                {
                USB1_BLK_ERR("usbBulkDevUnlock() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                OSS_MUTEX_RELEASE(bulkXbdMutex);
               /*
                * Do not break out this control task, we are expected to be
                * always online to process mounting and unmounting devices
                */
                continue;
                }

            /*
             * reduce the bulk device's lockcount, if the count reach to 0
             * destroy the device's structure
             */

            usbBulkDevDestroyByNode(bulkNodeId);

            /* Mark bulk node structure as dead */
            bulkNodeId = 0;
            }
        else
            {
            USB1_BLK_ERR("bulkClientThread: unknown bulkDeviceStatus \n",
                         1, 2, 3, 4, 5, 6);
            }

        OSS_MUTEX_RELEASE(bulkXbdMutex);
        }
    }
/*************************************************************************
*
* usrUsbBulkDevInit - initializes USB BULK Mass storage driver
*
* This function initializes the BULK driver and registers a CBI - BULK
* drive with the USBD.  In addition, it also spawns a task to handle
* plugging / unplugging activity.
*
* RETURNS: OK or ERROR if un-able to intialize the bulk class driver
*
* ERRNO: none
*
*\NOMANUAL
*/

STATUS usrUsbBulkDevInit (void)

    {
    int     cnt = 0;
    int     i = 0;
    char    fsmName[BULK_MAX_DRV_NAME_SZ];

    /* Initialize the BULK class driver */

    if (usbBulkDevInit () != OK)
        {
        USB1_BLK_ERR("usbBulkDevInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /*  This queue is used to pass status parameters to the task spawned below */

    if (usbQueueCreate (128, &bulkCallbackQueue) != OK)
        {
        USB1_BLK_ERR("callback queue creation error \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Initialize multi device support */

    for (cnt = 0; cnt < BULK_MAX_DEVS; cnt++ )
        {
        for (i = 0; i < BULK_MAX_DRIVE; i++)
            {
            memset( (void *)usbBulkDeviceTbl[cnt].usbBulkDrvName[i],
                   0, BULK_MAX_DRV_NAME_SZ);
            snprintf(usbBulkDeviceTbl[cnt].usbBulkDrvName[i], BULK_MAX_DRV_NAME_SZ - 1,
                     "%s%d", BULK_DRIVE_NAME, (i + (cnt*BULK_MAX_DRIVE)));
            usbBulkDeviceTbl[cnt].pusbBulkXbdDev[i] = NULL;

            /* fsm name mapping the device name */
            snprintf(fsmName, BULK_MAX_DRV_NAME_SZ - 1, 
                     "%s:1", usbBulkDeviceTbl[cnt].usbBulkDrvName[i]);
            fsmNameInstall(fsmName, usbBulkDeviceTbl[cnt].usbBulkDrvName[i]);

            /*
             * The above name mapping is for device with partition table,
             * The name mapping below is for device without partition table
             */

            snprintf(fsmName, BULK_MAX_DRV_NAME_SZ - 1, 
                     "%s:0", usbBulkDeviceTbl[cnt].usbBulkDrvName[i]);
            fsmNameInstall(fsmName, usbBulkDeviceTbl[cnt].usbBulkDrvName[i]);
            }
        usbBulkDeviceTbl[cnt].usbBulkDrvNumLun = 0;
        usbBulkDeviceTbl[cnt].usbBulkDrvXbdNodeId = 0;
        }

    if (OSS_MUTEX_CREATE(&bulkXbdMutex) != OK)
        {
        (void) usbQueueDestroy (bulkCallbackQueue);
        (void) usbBulkDevShutDown (OK);
        return ERROR;
        }

    /* Spawn a task to manage drive removal and insertion */

    if ((bulkTaskId = taskSpawn ("tBulkClnt",
                5,
                0,
                20000,
                (FUNCPTR) bulkClientThread,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )) == OS_THREAD_FAILURE)
        {
        bulkTaskId = OS_THREAD_FAILURE;
        (void) usbQueueDestroy (bulkCallbackQueue);
        (void) usbBulkDevShutDown (OK);
        return ERROR;
        }

    /* Register for dynamic attach callback */

    if (usbBulkDynamicAttachRegister (bulkAttachCallback, (pVOID)NULL) != OK)
        {
        USB1_BLK_ERR("usbBulkDynamicAttachRegister() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        taskDelete (bulkTaskId);
        bulkTaskId = OS_THREAD_FAILURE;
        (void) usbQueueDestroy (bulkCallbackQueue);
        (void) usbBulkDevShutDown (OK);
        return ERROR;
        }

    USB1_BLK_INFO("USB BULK Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;
    }

#ifdef DEBUG_USB_RAW

/*************************************************************************
*
* usrUsbRawRead - raw read from the device
*
* This function is to raw read from the device. This function is used for
* debugging purpose. It receives raw data over the bulk - in pipe.
*
* RETURNS: OK on success, or ERROR
*
* ERRNO: none
*/

STATUS usrUsbRawRead
    (
    char * devName,       /* Name of the Device */
    int    blkNum,        /* Block Number */
    char * pData          /* Pointer to hold the data sent */
    )
    {
    STATUS retval;
    int       devCnt;
    int       drvCnt;
    BOOL      deviceLocated = FALSE;
    XBD *     pUsbBulkXbdDev;
    pUSB_BULK_DEV_XBD_LUN pUsbBulkDevLun;

    /* if no valid data pointer, return ERROR */

    if (pData == NULL)
        {
        return ERROR;
        }

    /* get device name */
    for (devCnt = 0; devCnt < BULK_MAX_DEVS; devCnt++)
        {
        for (drvCnt = 0; drvCnt < BULK_MAX_DRIVE; drvCnt++)
            {
            /*
             * If the device is located, set the flag and break
             * out of the loop.
             */

            if (strcmp( usbBulkDeviceTbl[devCnt].usbBulkDrvName[drvCnt],
                       devName) == 0 )
                {
                deviceLocated = TRUE;
                break;
                }
            }
        /* If device is already located, do not loop */

        if (deviceLocated)
            break;
        }

    /*
     * If the device is not located or the corresponding entry in the bulk
     * device table is NULL (which means that the device is not connected),
     * return ERROR(deviceLocated set to TRUE, does not mean the device is
     * connected).
     */

    if (deviceLocated == FALSE ||
        (usbBulkDeviceTbl[devCnt].pusbBulkXbdDev[drvCnt] == NULL))
        return(ERROR);

    pUsbBulkXbdDev = usbBulkDeviceTbl[devCnt].pusbBulkXbdDev[drvCnt];
    pUsbBulkDevLun = (pUSB_BULK_DEV_XBD_LUN)pUsbBulkXbdDev;

    /* Read the data sent from the device */

    retval = pUsbBulkDevLun->usbBulkDevRd(pUsbBulkXbdDev, blkNum, 1, pData);

    return ((retval != OK)? ERROR : OK);
    }


/*************************************************************************
*
* usrUsbRawWrite - raw write to the device
*
* This function is to raw write to the device . The function is used for
* debuging purpose and send raw data over the bulk - out pipe.
*
* RETURNS: OK on success, or ERROR
*
* ERRNO: none
*/

STATUS usrUsbRawWrite
    (
    char * devName,          /* Name of the device */
    int    blkNum,           /* Block Number */
    char * pData             /* Data to be sent */
    )
    {
    STATUS    retval;
    XBD *     pUsbBulkXbdDev;
    int       devCnt;
    int       drvCnt;
    BOOL      deviceLocated = FALSE;
    pUSB_BULK_DEV_XBD_LUN pUsbBulkDevLun;

    /* if no valid data pointer, return ERROR */

    if (pData == NULL)
        {
        return ERROR;
        }

    /* get device name */

    for (devCnt = 0; devCnt < BULK_MAX_DEVS; devCnt++)
        {
        for (drvCnt = 0; drvCnt < BULK_MAX_DRIVE; drvCnt++)
            {
            /*
             * If the device is located, set the flag and break
             * out of the loop.
             */

            if(strcmp( usbBulkDeviceTbl[devCnt].usbBulkDrvName[drvCnt],
                       devName) == 0 )
                {
                deviceLocated = TRUE;
                break;
                }
            }

        /* If device is already located, do not loop */

        if (deviceLocated)
            break;
        }


    /*
     * If the device is not located or the corresponding entry in the bulk
     * device table is NULL (which means that the device is not connected),
     * return ERROR(deviceLocated set to TRUE, does not mean the device is
     * connected).
     */

    if (deviceLocated == FALSE ||
        (usbBulkDeviceTbl[devCnt].pusbBulkXbdDev[drvCnt] == NULL))
        return(ERROR);

    pUsbBulkXbdDev = usbBulkDeviceTbl[devCnt].pusbBulkXbdDev[drvCnt];
    pUsbBulkDevLun = (pUSB_BULK_DEV_XBD_LUN)pUsbBulkXbdDev;

    /* write to the device */

    retval = pUsbBulkDevLun->usbBulkDevWrt(pUsbBulkXbdDev, blkNum, 1, pData);
    return ((retval != OK)? ERROR : OK);
    }

#endif

/*************************************************************************
*
* findPerDevId - find device in the table knowing its nodeId and lun
*
* This function is used to find the device in the table by <nodeID>. If
* the device is not found for the specifed <nodeID>, 0xFF is retured
*
* RETURNS: the index holding the device specified by <nodeId> or 0xFF if
* not able to locate the device
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL int findPerDevId
    (
    USBD_NODE_ID nodeId
    )
    {
    int i;

    for (i = 0; i < BULK_MAX_DEVS; i++)
        {
        if(usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId == nodeId )
            {
            break;
            }
        }

    if (i == BULK_MAX_DEVS)
        {
        return(0xFF);
        }

    return(i);
}

/*************************************************************************
*
* findUnusedDevice - find unused device in the table
*
* This funtion finds the unused devices in the table
*
* RETURNS: the table row number of 0xFF if no unused device found
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL int findUnusedDevice (void)
    {
    int i;

    for (i = 0; i < BULK_MAX_DEVS; i++)
        {
        if( usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId == 0 )
            {
            break;
            }
        }

    if (i == BULK_MAX_DEVS)
        {
        return(0xFF);
        }

    return(i);
    }

/***************************************************************************
*
* usrUsbBulkShow - shows routine for displaying all bulk devices.
*
* This function displays all the bulk devices
*
* RETURNS: N/A
*
* ERRNO: none
*/

void usrUsbBulkShow (void)
    {
    int i;
    int j;

    for (i = 0; i < BULK_MAX_DEVS; i++)
        {
        if (usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId != 0)
            {
            for (j = 0; j < usbBulkDeviceTbl[i].usbBulkDrvNumLun; j++)
                {
                printf("%s on USB Node 0x%x LUN %d\n",
                       usbBulkDeviceTbl[i].usbBulkDrvName[j],
                       (UINT32)usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId,
                       j);
                usbBulkDriveShow(usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId, j);
                }
            }
        }
    }

/***************************************************************************
*
* usrUsbBulkDriveEmtpy - routine to check if drive has media inserted.
*
* This routine simply returns the Empty flag for the drive from the
*  usbBulk structure.
*
* RETURNS: TRUE if drive is Empty, FALSE if there is media in the drive
*
* ERRNO: none
*/

BOOL usrUsbBulkDriveEmpty
    (
    char *driveName
    )
    {
    int i;
    int j;

    for (i = 0; i < BULK_MAX_DEVS; i++)
        {
        if (usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId != 0)
            {
            for (j = 0; j < usbBulkDeviceTbl[i].usbBulkDrvNumLun; j++)
                {
                if (strcmp(driveName,usbBulkDeviceTbl[i].usbBulkDrvName[j]) == 0)
                    {
                    return(usbBulkDriveEmpty(usbBulkDeviceTbl[i].usbBulkDrvXbdNodeId,j));
                    }
                }
            }
        }
    return(TRUE);
    }

/*******************************************************************************
*
* usrUsbBulkDevDown - un-initializes USB BULK Mass storage driver
*
* This function un-initializes the BULK driver and un-registers a BULK
* drive with the USBD. It also un - mount the device if it is connected from
* the file system
*
* RETURNS: OK or ERROR if error un-initializing the mass storage device
*
* ERRNO: none
*/

STATUS usrUsbBulkDevDown (void)
    {

    UINT8   cnt = 0;    /* driver number */
    BOOL    bulkFlag = FALSE;

    /* unregsiter the callback function */

    if (usbBulkDynamicAttachUnregister (bulkAttachCallback, (pVOID) NULL) ==
            ERROR)
        {
        USB1_BLK_ERR("Error un-registering the callback \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

     /*
      * wait for the all the attached devices to be removed before deleting
      * the thread "tBulkClnt" that handled device connects and disconnects
      */

    while (TRUE)
        {
        for (cnt = 0; cnt < BULK_MAX_DEVS; cnt++)
            {
            if (usbBulkDeviceTbl[cnt].usbBulkDrvXbdNodeId != 0)
                bulkFlag = TRUE;
            }

        if (bulkFlag)
            {
            taskDelay (5);
            bulkFlag = FALSE;
            }
        else
            break;
        }

    /* Mark bulk node structure as dead */

    bulkNodeId = 0;

    /* delete the task for insertion and deletion */

    if (bulkTaskId != OS_THREAD_FAILURE
        && bulkTaskId != 0)
        {
        taskDelete (bulkTaskId);
        bulkTaskId = OS_THREAD_FAILURE;
        }

    if (bulkXbdMutex != NULL)
        {
        OSS_MUTEX_DESTROY(bulkXbdMutex);
        bulkXbdMutex = NULL;
        }

    /* Destroy the queue */

    if (usbQueueDestroy (bulkCallbackQueue) == ERROR)
        {
        USB1_BLK_ERR("Error destroying the queue \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* un-intialize the stack */
    if (usbBulkDevShutDown (OK) == ERROR)
        {
        USB1_BLK_ERR("Error un-intializing the driver  \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }
    USB1_BLK_DBG("%s[L%d]: Bulk Class Driver Successfully Un-Initialized \n",
                 __FILE__, __LINE__, 3, 4, 5, 6);
    return OK;
    }


/*******************************************************************************
*
*  usbBulkXbdStrategy - strategy routine for the XBD block device
*
* This routine effects the data transaction. The information in the bio is used
* by the device driver to transfer data. This is an asynchronous function in
* that once the data in the bio is used to setup the data transfer, or queued
* in the FIFO XBD bio queue, the function returns. It releases a semaphore to
* the usbBulkXbdService task to handle the bio.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL int usbBulkXbdStrategy
    (
    XBD *         usbBulkXbdDev, /* pointer to XBD */
    struct bio *  bio            /* pointer to bio */
    )
    {
    pUSB_BULK_DEV_XBD_LUN pUsbBulkDevLun =
                                (pUSB_BULK_DEV_XBD_LUN)usbBulkXbdDev;
    struct bio *          last = bio;
    struct bio *          bioPending;
    int                   status;

    if ((pUsbBulkDevLun->usbBulkXbdRemovable == TRUE) &&
        (usbBulkStatusCheckBypass() == FALSE))
        {
        /* check the status for this lun */
        status = usbBulkXbdTest(pUsbBulkDevLun);

        if (status != OK)
            {
            bio_done(bio,status); /* bio queue has been purged, kill this one*/
            return (status);
            }
        }

    /* Take the mutex semaphore for this XBD interface */
    if (semTake (pUsbBulkDevLun->usbBulkXbdMutex, WAIT_FOREVER) == ERROR)
        {
        while (last != NULL)
            {
            bio_done (last,ENXIO);
            last = last->bio_chain;
            }
        return ERROR;
        }

    /* if the device is going to be removed or unmounted, purge pending bios */
    if (pUsbBulkDevLun->unmounting == TRUE)
        {
        while ((bioPending = usbBulkGetNextBio(pUsbBulkDevLun)) != NULL )
            bio_done(bioPending, ENXIO);        /* purge pending bios */

        bio_done(bio, ENXIO);                   /* the current bio */

        semGive (pUsbBulkDevLun->usbBulkXbdMutex);
        return ERROR;
        }

    /* get the last bio in the bio chain */
    while (last->bio_chain != NULL)
        last = last->bio_chain;

    if (pUsbBulkDevLun->usbBulkXbdQueuet)
        {
        pUsbBulkDevLun->usbBulkXbdQueuet->bio_chain = bio;
        }
    else
        {
        pUsbBulkDevLun->usbBulkXbdQueueh = bio;
        }

    pUsbBulkDevLun->usbBulkXbdQueuet = last;


    /* Release the mutex semaphore */
    semGive (pUsbBulkDevLun->usbBulkXbdMutex);

    /* Tap our service task */

    semGive(pUsbBulkDevLun->usbBulkXbdReady);

    return (OK);
    }

/*******************************************************************************
*
* usbBulkGetNextBio - get the next bio in the bio chain
*
* This routine is called to get the next bio in the bio chain for an XBD device.*
* RETURNS: pointer to the next bio
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL struct bio * usbBulkGetNextBio
    (
    pUSB_BULK_DEV_XBD_LUN usbBulkXbdLun  /* pointer to XBD block device
                                          * wrapper */
    )
    {
    struct bio *  retVal;

    /* take the mutex semaphore for this XBD interface */
    if (semTake (usbBulkXbdLun->usbBulkXbdMutex, WAIT_FOREVER) == ERROR)
        return NULL;

    retVal = usbBulkXbdLun->usbBulkXbdQueueh;

    if (retVal != NULL)
        {
        usbBulkXbdLun->usbBulkXbdQueueh = retVal->bio_chain;
        retVal->bio_chain = NULL;

        if (usbBulkXbdLun->usbBulkXbdQueueh == NULL)
            usbBulkXbdLun->usbBulkXbdQueuet = NULL;
        }

   semGive (usbBulkXbdLun->usbBulkXbdMutex);
   return (retVal);
   }

/***************************************************************************
*
* usbBulkExecBio - execute a bio
*
* This routine excercise the bio for read from or write to the XBD block device.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL void usbBulkExecBio
    (
    pUSB_BULK_DEV_XBD_LUN pUsbBulkXbdLun,  /* pointer to XBD block device wrapper */
    struct bio *          bio       /* pointer to bio */
    )
    {
    XBD *      pusbBulkXbdDev =  (XBD *)pUsbBulkXbdLun;
    int        status = ERROR;
    unsigned   usbBulkBlockSize = pusbBulkXbdDev->xbd_blocksize;
    sector_t   usbBulkXbdBlocks = pusbBulkXbdDev->xbd_nblocks;
    sector_t   nblocks;
    unsigned   size;

    /* Check that all of this transaction fits in the disk */
    size = bio->bio_bcount;
    nblocks = size / usbBulkBlockSize;

    /* If our starting block number is bad, done with error */

    if (usbBulkXbdBlocks <= bio->bio_blkno)
        {
        bio->bio_resid = bio->bio_bcount;
        bio_done (bio, ENOSPC);
        return;
        }

    /* If we overrun the end of the disk, truncate the block number */
    if (usbBulkXbdBlocks < bio->bio_blkno + nblocks)
        {
        nblocks = usbBulkXbdBlocks - bio->bio_blkno;
        }

    /* calculate the real size and residual */
    size = nblocks * usbBulkBlockSize;
    bio->bio_resid = bio->bio_bcount - size;

    /* If we have less than 1 block, set the resid and done it */
    if (nblocks == 0)
        {
        bio_done (bio, 0);
        return;
        }

    /* Now we actually do the read/write peration */
    if (bio->bio_flags & BIO_READ)
        {
        /* call the driver's read routine */
        status =pUsbBulkXbdLun->usbBulkDevRd(pusbBulkXbdDev,
                                             (UINT32) bio->bio_blkno,
                                         (UINT32) nblocks, bio->bio_data);
        if(status != OK)
            {
            /* set the io error number */
            errno = S_ioLib_DEVICE_ERROR;
            }
        }
    else if (bio->bio_flags & BIO_WRITE)
        {
        /* check if the disk is write protected */
        if (pUsbBulkXbdLun->usbBulkRWMode == O_RDONLY)
            {
            /* disk is write protected set errno and status */
            status = EACCES;
            errno = S_ioLib_WRITE_PROTECTED;
            }
        else
            {
            /* disk is not write protected, call the driver's write routine */
            status = pUsbBulkXbdLun->usbBulkDevWrt(pusbBulkXbdDev,
                                                (UINT32) bio->bio_blkno,
                                                (UINT32) nblocks, bio->bio_data);
            if(status != OK)
                {
                /* set the io error number */
                errno = S_ioLib_DEVICE_ERROR;
                }
            }
        }
    else
        {
        status = ESRCH;
        errno = ESRCH;
        }

    if (status == OK)
        bio_done (bio, 0);
    else
        bio_done (bio, errno);
    }

/***************************************************************************
*
* usbBulkXbdService - task level routine to handle read and write operation
*
* This routine waits on a semaphore from strategy routine. It calls
* usbBulkGetNextBio and usbBulkExecBio to excercise all the bios currently
* chained for this device.
*
* Note that this task is spawned when the device is attached to the system and
* deleted after the device is detached and all the resources for this device
* are freed.
*
* RETURN: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL void usbBulkXbdService
    (
    pUSB_BULK_DEV_XBD_LUN pusbBulkXbdLun /* pointer to the XBD block wrapper */
    )
    {
    struct bio *           bio;

    while (TRUE)
        {
        /* Wait semaphore from strategy routine */

        if (semTake(pusbBulkXbdLun->usbBulkXbdReady, WAIT_FOREVER) == ERROR)
             return;

        /*
         * set the bio status flag to TRUE to indicate that bios are being
         * serviced
         */

        pusbBulkXbdLun->bioStatus = TRUE;

        /* get all the bios queued for this device and exercise them */

        while ((bio = usbBulkGetNextBio(pusbBulkXbdLun)) != NULL)
           usbBulkExecBio (pusbBulkXbdLun, bio);

        /*
         * set the bio status flag to FALSE to indicate that all submited bios
         * have been serviced
         */

        pusbBulkXbdLun->bioStatus = FALSE;

        }
    }

/*******************************************************************************
* usbBulkXbdLunDump - dump data to device
*
* This routine effects the data transaction specified in the bio with interrupts
* off. This call is similar to the usbBulkXbdStrategy call, only it executes
* with interrupts off. This is intended to allow a filesystem to dump data to
* the device prior to a system crash.
*
* RETURNS: bio_error
*
* ERRNO: none
*\NOMANUAL
*/

LOCAL int usbBulkXbdLunDump
    (
    XBD *     self,
    sector_t  blkno,
    void *    data,
    size_t    size
    )
    {
    struct bio     bio;

    /* Set up a bio for this transaction */
    bio.bio_dev     = self->xbd_dev.dv_dev;
    bio.bio_blkno   = blkno;
    bio.bio_bcount  = size;
    bio.bio_data    = data;
    bio.bio_error   = 0;
    bio.bio_flags   = BIO_WRITE;
    bio.bio_done    = NULL;
    bio.bio_caller1 = NULL;
    bio.bio_chain   = NULL;

    /* call our own strategy routine on it */

    usbBulkXbdStrategy(self, &bio);

    return (bio.bio_error);
    }


/***************************************************************************
*
* usbBulkXbdCreateSyncHandler - XBD instantiation synchronization handler
*
* RETURNS: none
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL void usbBulkXbdCreateSyncHandler
    (
    UINT16 Category,
    UINT16 Type,
    void * eventData,
    void * userData
    )
    {
    XBD *                  pBulkXbdDev;
    pUSB_BULK_DEV_XBD_LUN  pUsbBulkDevLun;
    devname_t *            devname;

    if ((Category == xbdEventCategory) && (Type == xbdEventInstantiated))
        {
        devname = (devname_t *) eventData;
        pBulkXbdDev = (XBD *) userData;
        pUsbBulkDevLun = (pUSB_BULK_DEV_XBD_LUN)pBulkXbdDev;

        if (strncmp ((char *) devname,
                  (char *) pBulkXbdDev->xbd_dev.dv_xname,
                 sizeof (devname_t)) != OK)
            {
            return;
            }

        erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                              usbBulkXbdCreateSyncHandler, userData);

        semGive (pUsbBulkDevLun->usbBulkXbdSyncId);

        }

    return;
    }

/***************************************************************************
* usbBulkXbdMediaTest - Test for media existence
*
* RETURNS: OK or ERROR
* ERRNO: none
*
*\NOMANUAL
*/
int usbBulkXbdMediaTest
    (
    pUSB_BULK_DEV_XBD_LUN pBulkDevLun
    )
    {
    int status;
    BOOL rcBit;
    XBD * pBulkXbdDev = (XBD *)pBulkDevLun;

    /* call the driver's status routine */
    status =  usbBulkDevStatusChk(pBulkXbdDev);

    rcBit = pBulkDevLun->usbBulkXbdReadyChanged;

    pBulkDevLun->usbBulkXbdReadyChanged = FALSE;

    if (status != OK)
        return (ERROR);

    if (rcBit)
        return (ERROR);

    return (OK);
    }


/***************************************************************************
* usbBulkXbdTest - Test to see if media is present or not.  If a change
* of media has occurred, test XBD's for 0 size and eject the device
* and instantiate the next device accordingly.
*
* /NOMANUAL
*/

int usbBulkXbdTest
    (
    pUSB_BULK_DEV_XBD_LUN pBulkDevLun
    )
    {
    BOOL status;

    status = usbBulkXbdMediaTest(pBulkDevLun);

    if (pBulkDevLun->usbBulkXbdInserted == TRUE)
        {
        /* We are a full-size XBD - if there is no media, then eject */
        if (status == ERROR)
            {
            /* Removal happened */
            usbBulkXbdEject(pBulkDevLun);
            return (S_ioLib_MEDIA_CHANGED);
            }
        }
    else
        {
        /* We are a 0 size XBD */
        if (status == ERROR)
            {
            status = usbBulkXbdMediaTest(pBulkDevLun);
            if (status == OK)
                {
                /* if there IS media then eject */
                usbBulkXbdEject(pBulkDevLun);
                return (S_ioLib_MEDIA_CHANGED);
                }
            }
        }
    return (OK);
    }

/*******************************************************************************
*
* usbBulkXbdEject - eject the device and instantiate the next step
*
* This routine ejects the bulk device and instantiate the next XBD.
*
* RETURNS: OK, or status code describing the failure.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usbBulkXbdEject
    (
    pUSB_BULK_DEV_XBD_LUN pBulkDevLun
    )
    {
    int status;
    struct bio * bio;
    device_t device;
    XBD * pBulkXbdDev = (XBD *)pBulkDevLun;

    /* Take the mutex before checking the unmounting flag */

    if (pBulkDevLun->usbBulkEjectMutex != NULL)
        {
        if (semTake(pBulkDevLun->usbBulkEjectMutex, 
                    USB_MSC_XBD_SYNC_WAIT_TIMEOUT) != OK)
            {
            USB1_BLK_ERR("Timeout to take usbBulkEjectMutex semphone. \n",
                         1, 2, 3, 4, 5, 6);
            return S_ioLib_DEVICE_TIMEOUT;
            }
        }

    /* Don't try further if the device is going to be removed or unmounted */

    if (pBulkDevLun->unmounting == TRUE)
        {
        /* Release the mutex */

        if (pBulkDevLun->usbBulkEjectMutex != NULL)
            {
            semGive(pBulkDevLun->usbBulkEjectMutex);
            }
        return S_ioLib_DISK_NOT_PRESENT;
        }

    /* Announce the ejection */

    erfEventRaise(xbdEventCategory, xbdEventMediaChanged, ERF_ASYNC_PROC,
                  (void *)(ULONG)pBulkXbdDev->xbd_dev.dv_dev, NULL);

    /* Detach the XBD handle */

    xbdDetach(pBulkXbdDev);

    /* Done any pending bio's */

    while ((bio = usbBulkGetNextBio(pBulkDevLun)) != NULL )
        bio_done(bio, ENXIO);

    /*
     * Now we instantiate the next XBD - if we have media, then its a regular
     * XBD, but if we don't, its an xbd of size 0 which fills in for the XBD
     * until media is actually inserted
     */

    if (usbBulkXbdMediaTest(pBulkDevLun) == OK)
        {
        /* Check again to see if the device is online */

        if (pBulkDevLun->unmounting == FALSE)
            {
            /* We have new media to mount */

            status = xbdAttach(pBulkXbdDev, &usbBulkXbdLunFuncs,
                               pBulkXbdDev->xbd_dev.dv_xname,
                               pBulkXbdDev->xbd_blocksize,
                               pBulkXbdDev->xbd_nblocks,
                               &device);

            pBulkDevLun->usbBulkXbdInserted = TRUE;
            }
        else
            {
            status = S_ioLib_DISK_NOT_PRESENT;
            }
        }
    else
        {
        /* Check again to see if the device is online */

        if (pBulkDevLun->unmounting == FALSE)
            {
            /* We are making a 0-block length XBD */

            status = xbdAttach(pBulkXbdDev,&usbBulkXbdLunFuncs,
                                pBulkXbdDev->xbd_dev.dv_xname,
                                512, 0, &device);
            pBulkDevLun->usbBulkXbdInserted = FALSE;
            }
        else
            {
            status = S_ioLib_DISK_NOT_PRESENT;
            }
        }

    /* Announce our new XBD */

    if (status == OK)
        {
        erfEventRaise(xbdEventCategory, xbdEventPrimaryInsert,
                    ERF_ASYNC_PROC, (void *)(ULONG)device, NULL);
        }

    /* Release the mutex */

    if (pBulkDevLun->usbBulkEjectMutex != NULL)
        {
        semGive(pBulkDevLun->usbBulkEjectMutex);
        }

    return (status);
    }

/*************************************************************************
*
* usbBulkStatusCheckBypassSet - set function for the status check bypass flag
*
* This function can be used by user to set the usbBulkStatusCheckBypassFlag
* at runtime.
*
* RETURNS: N/A
*
* ERRNO: none
*/

VOID usbBulkStatusCheckBypassSet
    (
    BOOL value
    )
    {
    usbBulkStatusCheckBypassFlag = value;
    }

/*************************************************************************
*
* usbBulkStatusCheckBypass - returns the status check bypass flag
*
* RETURNS: N/A
*
* ERRNO: none
*/

BOOL usbBulkStatusCheckBypass()
    {
    return usbBulkStatusCheckBypassFlag;
    }

#endif /* __INCusrUsbBulkDevInitc */

