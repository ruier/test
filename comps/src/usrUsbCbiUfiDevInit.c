/* usrUsbCbiUfiDevInit.c - USB Mass Storage CBI driver initialization */

/*
 * Copyright (c) 2000-2001, 2003-2008, 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
02g,06may13,s_z  Remove compiler warning (WIND00356717)
02f,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
02e,10nov10,ghs  Change fsmName size to avoid buffer overflow (WIND00240908)
02d,02sep10,ghs  Use OS_THREAD_FAILURE to check taskSpawn failure (WIND00229830)
02c,03aug10,m_y  Add bioBusy flag to indicate the bio is still executed by
                 the usbCbiUfiXbdService (WIND00226144)
02b,30jul10,m_y  Modify to remove compile warning
02a,21jul10,m_y  Move the action of destroy device structure into the
                 ufiClientThread (WIND00183499)
01z,05jul10,m_y  Modify routine usrUsbCbiUfiDevDown to improve usb stack
                 robustness (WIND00183499)
01y,27apr10,j_x  Error debug adaption
01x,18mar10,j_x  Changed for USB debug (WIND00184542)
01w,13jan10,ghs  vxWorks 6.9 LP64 adapting
01v,27sep08,w_x  Never break out tUfiClnt task; Check connected flag to stop
                 further IO when unmounting device (As for WIND00131719)
01u,11sep07,jrp  WIND00088083  - removed VX_UNBREAKABLE
01t,29aug06,dee  WIND00063436 - bio's never properly queued in
                 usbCbiUfiXbdStrategy()
01s,22sep05,hch  Add zero size disk support and XBD instantiation
                 Fix SPR #112282 and SPR #111530
01r,10aug05,hch  Added fsm name mapping to maintain backward compatibility
01q,05aug05,hch  Incoperate review comment for XBD implementation.
01p,02aug05,hch  Use removable file system APIs in usbCbiUfiDevDown()
01o,19jul05,hch  Modified to the XBD model to support the removable file system
01n,26jul05,ami  Un- intitialization function implemented and made
                 intialization routine return a status
01m,05jan05,ami  while (condition) replaced with while (TRUE)
01l,28dec04,mta  SPR104554: Warning Removal
01k,26oct04,hch  SPR #93843
01j,15oct04,ami  Apigen Changes
01i,07oct04,ami  SPR #91732 Fix
01h,03jun04,hch  Added definition for CBI_DRIVE_NAME
                 Fixed warning on iosDevFind
01g,07jan04,cfc  Rename BULK_DEV_UNMARK to UFI_DEV_USE_UNMARK
01f,30oct03,cfc  Add support for multiple CBI devices
01e,27oct03,sm   checkout to create a version and label in
                 usb branch
01d,20dec01,wef  declare usrFdiskPartRead
01c,07dec01,wef  Fixed more warnings.
01b,13nov01,wef  Removed warnings, added CBIO layer for dosFs2
01a,10dec00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB Mass Storage Control / Bulk / Interrupt
driver and places it in the driver table.  On boot, it can be refered to by
the name given specified by CBI_DRIVE_NAME.  This assumes the USB host stack has
already been initialized and has a host controller driver attached.

INCLUDE FILES: usb/usbdLib.h, usb/usbQueueLib.h,drv/usb/usbCbiUfiDevLib.h,
drv/usb/usbCbiUfiDevLibP.h, fsMonitor.h
*/

#ifndef __INCusrUsbCbiUfiDevInitc
#define __INCusrUsbCbiUfiDevInitc

/* includes */

#include <usb/usbdLib.h>
#include <usb/usbOsal.h>
#include <usb/usbQueueLib.h>
#include <drv/usb/usbCbiUfiDevLib.h>
#include <drv/usb/usbCbiUfiDevLibP.h>
#include <fsMonitor.h>

/* defines */

#ifndef UFI_MAX_DEVS
#define UFI_MAX_DEVS             2              /* Max number of UFI devices  */
#endif

#ifndef UFI_MAX_DRV_NAME_SZ
#define UFI_MAX_DRV_NAME_SZ      20             /* Max size of device name */
#endif

#ifndef CBI_DRIVE_NAME
#define CBI_DRIVE_NAME           "/cbid"
#endif

/* locals */

LOCAL QUEUE_HANDLE  cbiCallbackQueue;
LOCAL USB_MESSAGE   cbiDeviceStatus;

LOCAL USBD_NODE_ID  ufiNodeId;  /* Store for nodeId of the UFI device */
LOCAL XBD         * pUfiXbdDev[UFI_MAX_DEVS]; /* Store for drive structure */
LOCAL char          usbUfiDrvName[UFI_MAX_DEVS][UFI_MAX_DRV_NAME_SZ];
LOCAL USBD_NODE_ID  usbUfiDrvBlkNodeId[UFI_MAX_DEVS];
LOCAL TASK_ID       cbiTaskId = OS_THREAD_FAILURE;
LOCAL MUTEX_HANDLE  cbiUfiMutex;

/* Forward Functional Declarations */
LOCAL int usbCbiUfiXbdStrategy(XBD *, struct bio *);
LOCAL int usbCbiUfiXbdDump(XBD *, sector_t, void *, size_t);
LOCAL struct bio * usbCbiUfiGetNextBio(pUSB_CBI_UFI_DEV);
LOCAL void usbCbiUfiExecBio(pUSB_CBI_UFI_DEV, struct bio *);
LOCAL void usbCbiUfiXbdService ( pUSB_CBI_UFI_DEV);
LOCAL void usbCbiUfiXbdCreateSyncHandler (UINT16, UINT16, void *, void *);

LOCAL struct xbd_funcs usbCbiUfiXbdFuncs =
    {
    usbCbiUfiDevIoctl,
    usbCbiUfiXbdStrategy,
    usbCbiUfiXbdDump,
    };


/*************************************************************************
*
* ufiMountDrive - mounts a drive to the DOSFS.
*
* This function mounts the device to the DOSFS File System
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS ufiMountDrive
    (
    USBD_NODE_ID attachCode /* attach code */
    )
    {
    int         cnt = 0;
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev;
    device_t  retVal;

    /* Find the available drive name */

    for(cnt = 0; cnt < UFI_MAX_DEVS; cnt++)
        {
        if(usbUfiDrvBlkNodeId[cnt] == 0)
            /* mark this drive number as used */
            break;
        }

    if (cnt == UFI_MAX_DEVS)
        {
        USB1_CBI_ERR("Exceed the maximum UFI devices \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    usbUfiDrvBlkNodeId[cnt] =  attachCode;

    /* Create the block device with in the driver */

    pUfiXbdDev[cnt] = (XBD *) usbCbiUfiBlkDevCreate (attachCode);

    if (pUfiXbdDev[cnt] == NULL)
        {
        USB1_CBI_ERR("usbCbiUfiBlkDevCreate() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    pUsbCbiUfiDev = (pUSB_CBI_UFI_DEV)pUfiXbdDev[cnt];

    /* register new event handler for base device synchronization */

    if (erfHandlerRegister (xbdEventCategory, xbdEventInstantiated,
                            usbCbiUfiXbdCreateSyncHandler,pUsbCbiUfiDev,
                            0)
                            != OK)
        {
        return (ERROR);
        }

    /* Spawn the service task for this XBD */
    if ((pUsbCbiUfiDev->usbCbiUfiXbdService = taskSpawn("tUsbCbiXbdService",
                                                150,
                                                0,
                                                4096,
                                                (FUNCPTR)usbCbiUfiXbdService,
                                                (ULONG)pUsbCbiUfiDev,
                                                0,0,0,0,0,0,0,0,0))
                                                == OS_THREAD_FAILURE)
        {
        USB1_CBI_ERR("usbCbiUfiXbdService taskSpawn returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                             usbCbiUfiXbdCreateSyncHandler, pUsbCbiUfiDev);
        return ERROR;
        }

    if (pUsbCbiUfiDev->usbCbiXbdInserted == TRUE)
        {
        /* full size disk file system initialization */
        if (xbdAttach(pUfiXbdDev[cnt],
                      &usbCbiUfiXbdFuncs,
                      usbUfiDrvName[cnt],
                      pUfiXbdDev[cnt]->xbd_blocksize,
                      pUfiXbdDev[cnt]->xbd_nblocks,
                      &retVal) == OK)
            {
            erfEventRaise(xbdEventCategory, xbdEventPrimaryInsert,
                          ERF_ASYNC_PROC, (void *)(ULONG)retVal, NULL);
            semTake (pUsbCbiUfiDev->usbCbiXbdSyncId, WAIT_FOREVER);
            }
        else
            {
            USB1_CBI_ERR("xbdAttach returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            taskDelete (pUsbCbiUfiDev->usbCbiUfiXbdService);
            pUsbCbiUfiDev->usbCbiUfiXbdService = OS_THREAD_FAILURE;
            erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                                  usbCbiUfiXbdCreateSyncHandler,pUsbCbiUfiDev);
            return ERROR;
            }
        }
    else
        {
        /* 0 size XBD instantiation */
        if (xbdAttach(pUfiXbdDev[cnt],
                      &usbCbiUfiXbdFuncs,
                      usbUfiDrvName[cnt],
                      512,0, &retVal) == OK)
            {
            erfEventRaise(xbdEventCategory, xbdEventPrimaryInsert,
                          ERF_ASYNC_PROC, (void *)(ULONG)retVal, NULL);
            semTake (pUsbCbiUfiDev->usbCbiXbdSyncId, WAIT_FOREVER);
            }
        else
            {
            USB1_CBI_ERR("xbdAttach returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            taskDelete (pUsbCbiUfiDev->usbCbiUfiXbdService);
            pUsbCbiUfiDev->usbCbiUfiXbdService = OS_THREAD_FAILURE;
            erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                                  usbCbiUfiXbdCreateSyncHandler,pUsbCbiUfiDev);
            return (ERROR);
            }
        }

    return OK;
    }



/*************************************************************************
*
* ufiAttachCallback - user attach callback for USB UFI class driver.
*
* This function is the callback function for USB CBI UFI Class Driver.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID ufiAttachCallback
    (
    pVOID        arg,           /* caller-defined argument */
    USBD_NODE_ID nodeId,        /* pointer to UFI Device */
    UINT16       attachCode     /* attach code */
    )

    {
    usbQueuePut (cbiCallbackQueue,
                 0,                      /* msg */
                 attachCode,             /* wParam */
                 (UINT32) nodeId,        /* lParam */
                 5000);

    }

/***************************************************************************
*
* ufiClientThread - handle control of drives being plugged / unplugged
*
* This function controls what happens when a new drive gets plugged in
* or when an existing drive gets removed.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID ufiClientThread(void)
    {
    int              cnt = 0;
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev;
    struct bio *     bio;
    char             fsmName[UFI_MAX_DRV_NAME_SZ];

    /* support multi Cbi devices */

    for(cnt = 0; cnt < UFI_MAX_DEVS; cnt++ )
        {
        /* zero out */
        memset ((void *)usbUfiDrvName[cnt], 0, UFI_MAX_DRV_NAME_SZ);

        /* Initialize device names */
        snprintf(usbUfiDrvName[cnt], UFI_MAX_DRV_NAME_SZ - 1, 
                 "%s%d", CBI_DRIVE_NAME, cnt);

        /* fsm name mapping */
        snprintf(fsmName, UFI_MAX_DRV_NAME_SZ -1, 
                 "%s:0",usbUfiDrvName[cnt]);
        fsmNameInstall(fsmName,usbUfiDrvName[cnt]);

        usbUfiDrvBlkNodeId[cnt] = 0;    /* Initialize nodId's to zero */
        pUfiXbdDev[cnt] = 0;            /* Init for drive structure */
        }

    while (TRUE)
        {

        usbQueueGet (cbiCallbackQueue, &cbiDeviceStatus, OSS_BLOCK);

        /* Handle one device at a time */
        OSS_MUTEX_TAKE (cbiUfiMutex, OSS_BLOCK);

        /* Store nodeId of UFI device */
        ufiNodeId = (USBD_NODE_ID) cbiDeviceStatus.lParam;

        /* If attached. Only one device is supported at a time */
        if (cbiDeviceStatus.wParam == USB_UFI_ATTACH)
            {
            USB1_CBI_INFO("Found CbiUfi device at node 0x%x \n",
                          ufiNodeId, 2, 3, 4, 5, 6);
            /* Lock the device for protection */
            if (usbCbiUfiDevLock (ufiNodeId) != OK)
                {
                USB1_CBI_ERR("usbCbiUfiDevLock() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                OSS_MUTEX_RELEASE(cbiUfiMutex);
                /* Do not break out this while loop, just continue to work */
                continue;
                }
            /* Mount the drive to the DOS file system */
            if (ufiMountDrive(ufiNodeId) != OK)
                {
                USB1_CBI_ERR("ufiMountDrive() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }
            }
        else
            {
            /* Ufi device removed */
            /* Lookup nodeID to get device count/name */
            for (cnt = 0 ; cnt < UFI_MAX_DEVS ; cnt++)
                {
                if(usbUfiDrvBlkNodeId[cnt] == ufiNodeId)
                    {
                    break;
                    }
                }

            if (cnt == UFI_MAX_DEVS)
                {
                OSS_MUTEX_RELEASE(cbiUfiMutex);
                continue;
                }
            else
                {
                USB1_CBI_INFO("CbiUfi device at node 0x%x detached \n",
                              ufiNodeId, 2, 3, 4, 5, 6);
                pUsbCbiUfiDev = (pUSB_CBI_UFI_DEV)pUfiXbdDev[cnt];

                if (pUsbCbiUfiDev != NULL)
                    {
                    erfEventRaise(xbdEventCategory, xbdEventRemove,
                                  ERF_ASYNC_PROC,
                                  (void *)(ULONG)pUfiXbdDev[cnt]->xbd_dev.dv_dev,
                                   NULL);
                    xbdDetach (pUfiXbdDev[cnt]);

                   /* Execute all the remaining bios and wait for more bios
                    * when dv_dev is not NULLDEV
                    */

                    while (((bio = usbCbiUfiGetNextBio(pUsbCbiUfiDev)) != NULL) ||
                            (pUfiXbdDev[cnt]->xbd_dev.dv_dev != NULLDEV))
                            {
                            if (bio != NULL)
                                bio_done(bio,ENXIO);
                            else
                                {
                                /* reschedule */
                                taskDelay(1);
                                }
                            }

                    /*
                     * Before deleting the XBD service task, wait for all BIO's to
                     * be serviced.
                     * This is important because it may be possible that while we are
                     * deleting the thread some BIOs are submitted but yet to be
                     * serviced.
                     */

                    while (pUsbCbiUfiDev->bioBusy)
                        {
                        taskDelay (2);
                        }

                   /* Now that no more work will be available for this XBD, it
                    * is safe to delete the XBD service task
                    */

                    if (pUsbCbiUfiDev->usbCbiUfiXbdService != OS_THREAD_FAILURE
                        && pUsbCbiUfiDev->usbCbiUfiXbdService != 0)
                        {
                        taskDelete (pUsbCbiUfiDev->usbCbiUfiXbdService);
                        pUsbCbiUfiDev->usbCbiUfiXbdService = OS_THREAD_FAILURE;
                        }

                    /* Mark this XBD structure as NULL */
                    pUfiXbdDev[cnt] = 0;
                    }
                }

           /* Unlock the UFI device structure, so that it
            * gets destroyed
            */
            if (usbCbiUfiDevUnlock ((USBD_NODE_ID)cbiDeviceStatus.lParam)
                != OK)
                {
                USB1_CBI_ERR("usbCbiUfiDevUnlock() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                OSS_MUTEX_RELEASE (cbiUfiMutex);
                /* Do not break out this while loop, just continue to work */
                continue;
                }
            /*
             * reduce the cbi device's lockcount, if the count reach to 0
             * destroy the device's structure
             */

            usbCbiUfiDevDestroyByNode(usbUfiDrvBlkNodeId[cnt]);

            /* Mark this node as dead and unused */
            usbUfiDrvBlkNodeId[cnt] = 0;
            }
        OSS_MUTEX_RELEASE (cbiUfiMutex);
        }
    }


/***************************************************************************
*
* ufiClntRegister - Mass Storage Drive Registration
*
* This function registers a CBI - UFI drive with the USBD.  In addition, it
* also spawns a task to handle plugging / unplugging activity.
*
* RETURNS: OK or ERROR if not able to register
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS ufiClntRegister (void)
    {

    /* This queue is used to pass status parameters to the task spawned below */

    if (usbQueueCreate (128, &cbiCallbackQueue) != OK)
        {
        USB1_CBI_ERR("callback queue creation error \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Spawn a task to manage drive removal and insertion */

    if ((cbiTaskId = taskSpawn ("tUfiClnt",
                                5,
                                0,
                                20000,
                                (FUNCPTR) ufiClientThread,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )) == OS_THREAD_FAILURE)
        {
        USB1_CBI_ERR("TaskSpawn Error...! \n",
                     1, 2, 3, 4, 5, 6);
        cbiTaskId = OS_THREAD_FAILURE;
        (void) usbQueueDestroy (cbiCallbackQueue);
        return ERROR;
        }

    /* Register for dynamic attach callback */

    if (usbCbiUfiDynamicAttachRegister (ufiAttachCallback, (pVOID)NULL) != OK)
        {
        USB1_CBI_ERR("usbCbiUfiDynamicAttachRegister() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        (void) usbQueueDestroy (cbiCallbackQueue);
        taskDelete (cbiTaskId);
        cbiTaskId = OS_THREAD_FAILURE;
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usrUsbCbiUfiDevInit - initializes USB UFI Mass storage driver.
*
* This function initializes the USB UFI Mass storage driver and then
* regsiters the client by calling the routine ufiClntRegister ().
*
* RETURNS: OK or ERROR if error in initializing
*
* ERRNO: none
*/

STATUS usrUsbCbiUfiDevInit (void)
    {

    /* Initialize the UFI class driver */

    if (usbCbiUfiDevInit () != OK)
        {
        USB1_CBI_ERR("usbCbiUfiDevInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Registration routine */

    cbiUfiMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                              SEM_INVERSION_SAFE);

    if (cbiUfiMutex == NULL)
        {
        USB1_CBI_ERR("Error creating the semaphore \n",
                     1, 2, 3, 4, 5, 6);
        /* uninitialize the driver */

        (void) usbCbiUfiDevShutDown (OK);
        return ERROR;
        }

    if (ufiClntRegister () != OK)
        {
        USB1_CBI_ERR("Error Register the client callback \n",
                     1, 2, 3, 4, 5, 6);

        /* destroy the mutex */

        (void) semDelete (cbiUfiMutex);
        cbiUfiMutex = NULL;

        /* uninitialize the driver */

        (void) usbCbiUfiDevShutDown (OK);
        return ERROR;
        }
    USB1_CBI_INFO("USB CBI Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;
    }

/*******************************************************************************
*
* usrUsbCbiUfiDevDown - un-initializes USB UFI Mass storage driver.
*
* This function un-initializes the USB UFI Mass storage driver
*
* RETURNS: OK or ERROR if error in un-initializing
*
* ERRNO: none
*/

STATUS usrUsbCbiUfiDevDown (void)
    {

    UINT8   cnt = 0;    /* driver number */
    struct bio * bio;
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev;

    /* unregsiter the callback function */

    if (usbCbiUfiDynamicAttachUnregister (ufiAttachCallback, (pVOID) NULL) ==
        ERROR)
        {
        USB1_CBI_ERR("Error un-registering the callback \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* do the requried freeups */

    OSS_MUTEX_TAKE (cbiUfiMutex, OSS_BLOCK);

    for(cnt = 0; cnt < UFI_MAX_DEVS; cnt++)
        {
        if (usbUfiDrvBlkNodeId[cnt] != 0)
            {
            if (pUfiXbdDev[cnt] != 0)
                {
                pUsbCbiUfiDev=(pUSB_CBI_UFI_DEV)pUfiXbdDev[cnt];
                erfEventRaise(xbdEventCategory, xbdEventRemove,
                              ERF_ASYNC_PROC,
                              (void *)(ULONG)pUfiXbdDev[cnt]->xbd_dev.dv_dev,
                              NULL);
                xbdDetach (pUfiXbdDev[cnt]);

                /* Execute all the remaining bios and wait for more bios when
                 * dv_dev is not NULLDEV
                 */

                while (((bio = usbCbiUfiGetNextBio(pUsbCbiUfiDev))!=NULL) ||
                        (pUfiXbdDev[cnt]->xbd_dev.dv_dev != NULLDEV))
                    {
                    if (bio != NULL)
                        bio_done(bio,ENXIO);
                    else
                        {
                        /* reschedule */
                        taskDelay(1);
                        }
                    }

                if (pUsbCbiUfiDev->usbCbiXbdInstantiated != TRUE)
                    {
                    /*
                     * Unregister the xbdEventInstantiated event to stop
                     * usb2MscXbdCreateSyncHandler being called after the
                     * device has been removed
                     */

                    erfHandlerUnregister (xbdEventCategory,
                                          xbdEventInstantiated,
                                          usbCbiUfiXbdCreateSyncHandler,
                                          pUsbCbiUfiDev);
                    }

                /* Now that no more work will be available for this XBD, it is
                 * safe to delete the XBD service task
                 */

               if (pUsbCbiUfiDev->usbCbiUfiXbdService != OS_THREAD_FAILURE
                   && pUsbCbiUfiDev->usbCbiUfiXbdService != 0)
                    {
                    taskDelete (pUsbCbiUfiDev->usbCbiUfiXbdService);
                    pUsbCbiUfiDev->usbCbiUfiXbdService = OS_THREAD_FAILURE;
                    }

            /* Mark this XBD structure as NULL */
            pUfiXbdDev[cnt] = NULL;
            }

           /* Unlock the UFI device structure, so that it gets destroyed */

           if (usbCbiUfiDevUnlock ((USBD_NODE_ID)cbiDeviceStatus.lParam)
                != OK)
                {
                USB1_CBI_ERR("usbCbiUfiDevUnlock() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                OSS_MUTEX_RELEASE(cbiUfiMutex);
                }

            /* Mark this node as dead and unused */
            usbUfiDrvBlkNodeId[cnt] = 0;
            }
            memset ((void *)usbUfiDrvName[cnt], 0, UFI_MAX_DRV_NAME_SZ);
        }

    /* delete the task for insertion and deletion */

    if (cbiTaskId != OS_THREAD_FAILURE && cbiTaskId != 0)
        {
        taskDelete (cbiTaskId);
        cbiTaskId = OS_THREAD_FAILURE;
        }

    /* Destroy the queue */

    if (usbQueueDestroy (cbiCallbackQueue) == ERROR)
        {
        USB1_CBI_ERR("Error destroying the queue \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /*
     * we must take the mutex before destroy it
     * destroy the mutex created
     */

    if (semDelete (cbiUfiMutex) == ERROR)
        {
        USB1_CBI_ERR("Error destroying the mutex \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    cbiUfiMutex = NULL;

    /* un-intialize the stack */

    if (usbCbiUfiDevShutDown (OK) == ERROR)
        {
        USB1_CBI_ERR("Error un-intializing the driver \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    USB1_CBI_DBG("%s[L%d]: CBI Class Driver Successfully Un-Initialized \n",
                 __FILE__, __LINE__, 3, 4, 5, 6);
    return OK;
    }

/*******************************************************************************
*
*  usbCbiUfiXbdStrategy - strategy routine for the XBD block device
*
* This routine effects the data transaction. The information in the bio is used
* by the device driver to transfer data. This is an asynchronous function in
* that once the data in the bio is used to setup the data transfer, or queued
* in the FIFO XBD bio queue, the function returns. It releases a semaphore to
* the usbCbiUfiXbdService task to handle the bio.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL int usbCbiUfiXbdStrategy
    (
    XBD *         usbCbiUfiXbdDev, /* pointer to XBD */
    struct bio *  bio              /* pointer to bio */
    )
    {
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev =
                                (pUSB_CBI_UFI_DEV)usbCbiUfiXbdDev;
    struct bio *     last = bio;
    int              status;

    /* Don't try further if the device is not online */

    if (pUsbCbiUfiDev->connected == FALSE)
        {
        /*
         * File system don't care the return value, always wait for
         * the BIO, so we must let the BIO complete.
         */
        bio_done(bio, ENXIO);
        return ERROR;
        }

    if (pUsbCbiUfiDev->usbCbiUfiDevRemovable == TRUE)
        {
        /* check the status for this lun */
        status = usbCbiXbdTest(pUsbCbiUfiDev);

        if (status != OK)
            {
            bio_done(bio, status); /* bio queue has been purged, kill this one*/
            return (status);
            }
        }

    /* Take the mutex semaphore to guard the XBD bio queue */

    if (semTake (pUsbCbiUfiDev->usbCbiUfiXbdMutex, WAIT_FOREVER) == ERROR)
        {
        while (last != NULL)
            {
            bio_done(bio, ENXIO);
            last = last->bio_chain;
            }
        return ERROR;
        }

    /* get the last bio in the bio chain */

    while (last->bio_chain != NULL)
       last = last->bio_chain;

    /* link the currnet bio in the bio queue for this XBD */

    if (pUsbCbiUfiDev->usbCbiUfiXbdQueuet)
        {
        pUsbCbiUfiDev->usbCbiUfiXbdQueuet->bio_chain = bio;
        }
    else
        {
        pUsbCbiUfiDev->usbCbiUfiXbdQueueh = bio;
        }

     pUsbCbiUfiDev->usbCbiUfiXbdQueuet = last;

     /* Tap our service task */
     semGive(pUsbCbiUfiDev->usbCbiUfiReady);

     /* release the mutex */
     semGive (pUsbCbiUfiDev->usbCbiUfiXbdMutex);

     return (OK);
     }

/*******************************************************************************
*
* usbCbiUfiGetNextBio - get the next bio in the bio chain
*
* This routine is called to get the next bio in the bio chain for an XBD device.
*
* RETURNS: pointer to the next bio or NULL
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL struct bio * usbCbiUfiGetNextBio
    (
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev  /* pointer to XBD block device
                                     * wrapper */
    )
    {
    struct bio *  retVal;

    /* Take the mutex semaphore to guard the XBD queue */

    if (semTake (pUsbCbiUfiDev->usbCbiUfiXbdMutex, WAIT_FOREVER) == ERROR)
        return NULL;

    /* get the head of the bio queue for this XBD */

    retVal = pUsbCbiUfiDev->usbCbiUfiXbdQueueh;

    /* update the XBD bio queue */

    if (retVal != NULL)
        {
        pUsbCbiUfiDev->usbCbiUfiXbdQueueh = retVal->bio_chain;

        /* Mark the returned bio as a single bio without chain */

        retVal->bio_chain = NULL;

        if (pUsbCbiUfiDev->usbCbiUfiXbdQueueh == NULL)
            pUsbCbiUfiDev->usbCbiUfiXbdQueuet = NULL;
        }

   /* release mutex semaphore */

   semGive (pUsbCbiUfiDev->usbCbiUfiXbdMutex);

   return (retVal);
   }


/***************************************************************************
*
* usbCbiUfiExecBio - excercise a bio
*
* This routine excercise the bio for read or write to the XBD block device.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL void usbCbiUfiExecBio
    (
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev,  /* pointer to XBD block device wrapper */
    struct bio *     bio             /* pointer to bio */
    )
    {
    int        status = ERROR;
    XBD *      pUsbCbiUfiXbdDev   = (XBD *)pUsbCbiUfiDev;
    unsigned   usbCbiUfiBlockSize = pUsbCbiUfiXbdDev->xbd_blocksize;
    sector_t   usbCbiUfiXbdBlocks = pUsbCbiUfiXbdDev->xbd_nblocks;
    sector_t   nblocks;
    unsigned   size;

    /* Check that all of this transaction fits in the disk */
    size = bio->bio_bcount;

    nblocks = size / usbCbiUfiBlockSize;

    /* If our starting block number is bad, done with error */
    if (usbCbiUfiXbdBlocks <= bio->bio_blkno)
        {
        bio->bio_resid = bio->bio_bcount;
        bio_done (bio, ENOSPC);
        return;
        }

    /* If we overrun the end of the disk, truncate the block number */
    if (usbCbiUfiXbdBlocks < bio->bio_blkno + nblocks)
        {
        nblocks = usbCbiUfiXbdBlocks - bio->bio_blkno;
        }

    /* calculate the real size and residual */
    size = nblocks * usbCbiUfiBlockSize;
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
        status =pUsbCbiUfiDev->usbCbiUfiDevRd(pUsbCbiUfiXbdDev,
                                             (UINT32) bio->bio_blkno,
                                             (UINT32) nblocks, bio->bio_data);
        }
    else if (bio->bio_flags & BIO_WRITE)
        {
        if (pUsbCbiUfiDev->usbCbiUfiRWMode == O_RDONLY)
            {
            status = EACCES;
            errno = S_ioLib_WRITE_PROTECTED;
            }
        else
            {
            status = pUsbCbiUfiDev->usbCbiUfiDevWrt(pUsbCbiUfiXbdDev,
                                            (UINT32)bio->bio_blkno,
                                            (UINT32)nblocks, bio->bio_data);
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
* usbCbiUfiXbdService - task level routine to handle read and write operation
*
* This routine waits on a semaphore from the strategy routine. It calls
*  usbCbiUfiGetNextBio and usbCbiUfiExecBio to excercise all the bios currently
* chained for this device.
*
* Note that this task is spawned when the device is attached to the system and
* deleted after te device is detached and all the resources for this device are
* freed.
*
* RETURN: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL void usbCbiUfiXbdService
    (
    pUSB_CBI_UFI_DEV  pUsbCbiUfiDev /* pointer to the XBD block wrapper */
    )
    {
    struct bio *        bio;

    while (TRUE)
        {

        if (pUsbCbiUfiDev->connected == FALSE)
            {
            break;
            }

        /* take sempahore from strategy routine */

        if (semTake(pUsbCbiUfiDev->usbCbiUfiReady, WAIT_FOREVER) == ERROR)
            return;

        pUsbCbiUfiDev->bioBusy = TRUE;

        /* get all the bios queued for this device and exercise them */

        while ((bio = usbCbiUfiGetNextBio (pUsbCbiUfiDev)) != NULL)
            usbCbiUfiExecBio (pUsbCbiUfiDev, bio);

        pUsbCbiUfiDev->bioBusy = FALSE;
        }
    }

/*******************************************************************************
* usbCbiUfiXbdDump - dump data to device
*
* This routine effects the data transaction specified in the bio with interrupts
* off. This call is similar to the usbCbiUfiXbdStrategy call, only it executed
* with interrupts off. This is intended to allow a filesystem to dump data to
* the device prior to a system crash.
*
* RETURNS: bio_error
*
* ERRNO: none
*\NOMANUAL
*/

LOCAL int usbCbiUfiXbdDump
    (
    XBD *     self,
    sector_t  blkno,
    void *    data,
    size_t    size
    )
    {
    struct bio     bio;

    /* Set up a bio for this transaction */
    bio.bio_dev = self->xbd_dev.dv_dev;
    bio.bio_blkno   = blkno;
    bio.bio_bcount  = size;
    bio.bio_data    = data;
    bio.bio_error   = 0;
    bio.bio_flags   = BIO_WRITE;
    bio.bio_done    = NULL;
    bio.bio_caller1 = NULL;
    bio.bio_chain   = NULL;

    /* call our own strategy routine on it */

    usbCbiUfiXbdStrategy(self, &bio);

    return (bio.bio_error);
    }

/***************************************************************************
*
* usbCbiUfiXbdCreateSyncHandler -
*/

LOCAL void usbCbiUfiXbdCreateSyncHandler
    (
    UINT16 Category,
    UINT16 Type,
    void * eventData,
    void * userData
    )
    {
    XBD *            pUsbCbUfiiXbdDev;
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev;
    devname_t *      devname;

    if ((Category == xbdEventCategory) && (Type == xbdEventInstantiated))
        {
        devname = (devname_t *) eventData;
        pUsbCbUfiiXbdDev = (XBD *) userData;
        pUsbCbiUfiDev = (pUSB_CBI_UFI_DEV) pUsbCbUfiiXbdDev;

        if (strncmp ((char *) devname,
                     (char *) pUsbCbUfiiXbdDev->xbd_dev.dv_xname,
                     sizeof (devname_t)) != OK)
            return;

        erfHandlerUnregister(xbdEventCategory, xbdEventInstantiated,
                             usbCbiUfiXbdCreateSyncHandler, userData);

        semGive (pUsbCbiUfiDev->usbCbiXbdSyncId);
        }

    return;
    }

/***************************************************************************
*
* usbCbiXbdMediaTest - Test for media existence
*
* /NOMANUAL
*/
int usbCbiXbdMediaTest
    (
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev
    )
    {
    int status;
    BOOL rcBit;
    XBD * pUsbCbUfiiXbdDev = (XBD*) pUsbCbiUfiDev;

    /* call the driver's status routine */
    status = usbCbiUfiDevStChk(pUsbCbUfiiXbdDev);

    rcBit = pUsbCbiUfiDev->usbCbiUfiDevReadyChanged;

    pUsbCbiUfiDev->usbCbiUfiDevReadyChanged = FALSE;

    if (status != OK)
        return (ERROR);

    if (rcBit)
        return (ERROR);

    return (OK);
    }

/***************************************************************************
* usbCbiXbdTest - Test to see if media is present or not.  If a change
* of media has occurred, test XBD's for 0 size and eject the device
* and instantiate the next device accordingly.
*
* /NOMANUAL
*/

int usbCbiXbdTest
    (
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev
    )
    {
    BOOL status;

    status = usbCbiXbdMediaTest(pUsbCbiUfiDev);

    if (pUsbCbiUfiDev->usbCbiXbdInserted == TRUE)
        {
        /* We are a full-size XBD - if there is no media, then eject */
        if (status == ERROR)
            {
            /* Removal happened */
            usbCbiXbdEject (pUsbCbiUfiDev);
            return (S_ioLib_MEDIA_CHANGED);
            }
        }
    else
        {
        /* We are a 0 size XBD */
        if (status == ERROR)
            {
            status = usbCbiXbdMediaTest(pUsbCbiUfiDev);
            if (status == OK)
                {
                /* if there IS media then eject */
                usbCbiXbdEject (pUsbCbiUfiDev);
                return (S_ioLib_MEDIA_CHANGED);
                }
            }
       }

    return (OK);
    }

/***************************************************************************
*
* usbCbiXbdEject - Eject the device and instantiate the next step
*
* /NOMANUAL
*/

int usbCbiXbdEject
    (
    pUSB_CBI_UFI_DEV pUsbCbiUfiDev
    )
    {
    int status;
    struct bio * bio;
    device_t device;
    XBD * pUsbCbiUfiXbdDev = (XBD *) pUsbCbiUfiDev;

    /* Don't try further if the device not online */
    if (pUsbCbiUfiDev->connected == FALSE)
        {
        return S_ioLib_DISK_NOT_PRESENT;
        }

    /* Announce the ejection */
    erfEventRaise(xbdEventCategory, xbdEventMediaChanged, ERF_ASYNC_PROC,
                 (void *)(ULONG)pUsbCbiUfiXbdDev->xbd_dev.dv_dev,NULL);

    /* Detach the XBD handle */
    xbdDetach(pUsbCbiUfiXbdDev);

    /* Done any pending bio's */
    while ((bio = usbCbiUfiGetNextBio(pUsbCbiUfiDev)) != NULL )
        bio_done(bio,ENXIO);

    /*
     * Now we instantiate the next XBD - if we have media, then its a regular
     * XBD, but if we don't, its an xbd of size 0 which fills in for the XBD
     * until media is actually inserted
     */

    if (usbCbiXbdMediaTest(pUsbCbiUfiDev) == OK)
        {
        /* Check again to see if the device is online */
        if (pUsbCbiUfiDev->connected == TRUE)
            {

            /* We have new media to mount */
            status = xbdAttach(pUsbCbiUfiXbdDev, &usbCbiUfiXbdFuncs,
                               pUsbCbiUfiXbdDev->xbd_dev.dv_xname,
                               pUsbCbiUfiXbdDev->xbd_blocksize,
                               pUsbCbiUfiXbdDev->xbd_nblocks,
                               &device);
            pUsbCbiUfiDev->usbCbiXbdInserted = TRUE;
            }
        else
            {
            status = S_ioLib_DISK_NOT_PRESENT;
            }
        }
    else
        {
        /* Check again to see if the device is online */
        if (pUsbCbiUfiDev->connected == TRUE)
            {
            /* We are making a 0-block length XBD */
            status = xbdAttach(pUsbCbiUfiXbdDev, &usbCbiUfiXbdFuncs,
                               pUsbCbiUfiXbdDev->xbd_dev.dv_xname,
                               512,0,&device);
            pUsbCbiUfiDev->usbCbiXbdInserted = FALSE;
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

    return (status);
    }

#endif /* __INCusrUsbCbiUfiDevInitc */
