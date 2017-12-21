/* usrUsbSpkrInit.c - Initialization of the USB Speaker driver */

/*
 * Copyright (c) 2001, 2002, 2004-2007, 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01z,06may13,s_z  Remove compiler warning (WIND00356717)
01y,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01x,07jan11,ghs  Clean up compile warnings (WIND00247082)
01w,28jul10,ghs  Fix device name display error(WIND00223154)
01v,05jul10,m_y  Set all the resources to NULL after release (WIND00183499)
01u,05jul10,ghs  Modify display logs(WIND00216586)
01t,27apr10,j_x  Error debug adaption
01s,26mar10,ghs  Fix logic error and memory leak(WIND00204310, WIND00204305)
01r,18mar10,j_x  Changed for USB debug (WIND00184542)
01q,03mar10,ghs  Fix LP64 adapting(WIND00202082)
01p,13jan10,ghs  vxWorks 6.9 LP64 adapting
01o,10apr07,sup  Change the intialization function to return a status
01n,05feb07,jrp  USB headset integration
01m,22dec05,ami  Mutex Definition usbSpkrMutex removed. Never used in the file
                 (SPR #112279 Fix)
01l,10aug05,ami  Deinitialization call modified, Removed unnessary mutexes
01k,23jan06,jrp  Modified to support productization
01j,06oct06,j_l  Modify create, delete, init, and uninit routines
01i,14sep06,j_l  usbSpkrClose() shuts down audio stream
01h,08sep06,j_l  Modify for Nortel's headset driver
01g,11apr05,hch  Remove select initialization to solve exception on base6
01f,28dec04,mta  SPR104699:Warning Removal
01e,15oct04,ami  Apigen Changes
01d,30jan02,wef  fixed DevFind routine
01c,07dec01,wef  fixed some warnings
01b,02feb01,wef  Added ios functionality - allows for open, close, read,
                 ioctl, etc access to the printer
01a,01feb01,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB speaker driver.  This assumes the
USB host stack has already been initialized and has a host controller
driver attached.

This configlette demonstrates how a user might integrate a USB class
driver into the vxWorks file system.  usbSpkrDevCreate () intalls a USB
speaker and its associated driver functions into the driver table allowing
the speaker to be accesed with standard fopen, close, read, write, etc. type
calls.  The decision to keep this functionality out of the driver itself
was made for backwards compatability reasons.

INCLUDE FILES: iosLib.h, vxWorks.h, iv.h, ioLib.h, tyLib.h, intLib.h,
errnoLib.h, sioLib.h, stdlib.h, stdio.h, logLib.h,
selectLib.h, drv/usb/usbSpeakerLib.h, usb/usbAudio.h,
usb/tools/wavFormat.h

*/

#ifndef __INCusrUsbSpkrInitc
#define __INCusrUsbSpkrInitc

/* includes */

#include <drv/usb/usbSpeakerLib.h>
#include <usb/tools/wavFormat.h>        /* Microsoft .wav file format */
#include <usb/usbAudio.h>
#include <vxWorks.h>
#include <iv.h>
#include <ioLib.h>
#include <iosLib.h>
#include <tyLib.h>
#include <intLib.h>
#include <errnoLib.h>
#include <sioLib.h>
#include <stdlib.h>
#include <stdio.h>
#include <logLib.h>
#include <selectLib.h>


/* defines */

#define TX_BUF_NUM    0x10000

#define USB_SPKR_LIST_SEM_TAKE(tmout)        \
    semTake (usbSpkrListMutex, (_Vx_ticks_t) (tmout))

#define USB_SPKR_LIST_SEM_GIVE            \
    semGive (usbSpkrListMutex)


#define SPKR_NAME_LEN_MAX       100
#define USB_SPKR_NAME           "/usbSp/"

/* data types */

/* typedefs */

typedef struct usb_spkr_node
    {
    NODE                    node;
    struct usb_spkr_dev *   pUsbSpkrDev;
    } USB_SPKR_NODE;

typedef struct usb_spkr_dev /* USB_SPKR_DEV */
    {
    DEV_HDR         ioDev;
    SEQ_DEV         *pSeqDev;
    UINT16          numOpen;
    UINT32          bufSize;
    UCHAR           *buff;
    SEL_WAKEUP_LIST selList;
    USB_SPKR_NODE   * pUsbSpkrNode;
    } USB_SPKR_DEV;


/* local variables */

LOCAL SEM_ID    usbSpkrListMutex;   /* mutex semaphore to protect list */
LOCAL UINT32    spkrCount = 0;
LOCAL LIST      usbSpkrList;        /* all USB speakers in the system */

LOCAL int usbSpkrDrvNum = 0;        /* driver number */
LOCAL BOOL spkrDrvNames[SPKR_MAX_DEVS];


/* forward declarations */

LOCAL void  usbSpkrDrvAttachCallback (void * arg, SEQ_DEV *pSeqDev,
                                      UINT16 attachCode);

LOCAL ssize_t   usbSpkrRead (USB_SPKR_DEV * pUsbSpkrDev,UCHAR *buffer,
                             size_t nBytes);

LOCAL ULONG usbSpkrClose (USB_SPKR_DEV * pUsbSpkrDev);

LOCAL ssize_t   usbSpkrWrite (USB_SPKR_DEV * pUsbSpkrDev, UCHAR * buffer,
                              size_t nBytes);

LOCAL ULONG usbSpkrOpen (USB_SPKR_DEV * pUsbSpkrDev, char * name,
                         int flags, int mode);

LOCAL int   usbSpkrIoctl (USB_SPKR_DEV * pUsbSpkrDev, int request, void * arg);

LOCAL STATUS    usbSpkrDevFind (SEQ_DEV * pSeqDev, USB_SPKR_DEV ** ppUsbSpkrDev);

LOCAL STATUS    usbSpkrDevDelete (USB_SPKR_DEV * pUsbSpkrDev, char * filename);


/*******************************************************************************
*
* usbSpkrDevCreate - create a VxWorks device for an USB speaker
*
* This routine creates a device on a specified serial channel.  Each channel
* to be used should have exactly one device associated with it by calling
* this routine.
*
* For instance, to create the device "/ /0", the proper call would be:
* .CS
*     usbSpkrDevCreate ("/usbSp/0", pSeqDev);
* .CE
* Where pSeqDev is the address of the underlying SEQ_DEV serial channel
* descriptor (defined in sioLib.h).
* This routine is typically called by the USB speaker driver, when it detects
* an insertion of a USB speaker.
*
* RETURNS: OK, or ERROR if the driver is not installed, or the
* device already exists, or failed to allocate memory.
*
* ERRNO: none
*/

STATUS usbSpkrDevCreate
    (
    char    * name,         /* name to use for this device      */
    SEQ_DEV * pSeqDev       /* pointer to core driver structure */
    )
    {
    USB_SPKR_NODE       * pUsbSpkrNode = NULL;      /* pointer to device node */
    USB_SPKR_DEV        * pUsbSpkrDev = NULL;       /* pointer to USB device */

    if (pSeqDev == (SEQ_DEV *) ERROR)
        {
        USB1_SPK_ERR("pSeqDev is ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* allocate memory for the device */

    if ((pUsbSpkrDev = (USB_SPKR_DEV *)
        calloc (1, sizeof (USB_SPKR_DEV))) == NULL)
        {
        USB1_SPK_ERR("memory allocate failed for pUsbSpkrDev \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    pUsbSpkrDev->pSeqDev = pSeqDev;

    /* allocate memory for this node, and populate it */

    pUsbSpkrNode = (USB_SPKR_NODE *) calloc (1, sizeof (USB_SPKR_NODE));

    if (pUsbSpkrNode == NULL)
        {
        USB1_SPK_ERR("memory allocate failed for pUsbSpkrNode \n",
                     1, 2, 3, 4, 5, 6);
        pUsbSpkrDev->pSeqDev = NULL;
        free(pUsbSpkrDev);
        return (ERROR);
        }

    /* record useful information */

    pUsbSpkrNode->pUsbSpkrDev = pUsbSpkrDev;
    pUsbSpkrDev->pUsbSpkrNode = pUsbSpkrNode;

    /* add the node to the list */

    USB_SPKR_LIST_SEM_TAKE (WAIT_FOREVER);

    lstAdd (&usbSpkrList, (NODE *) pUsbSpkrNode);

    USB_SPKR_LIST_SEM_GIVE;

    /* allow for select support */

    selWakeupListInit (&pUsbSpkrDev->selList);

    /* add the device to the I/O system */

    return (iosDevAdd (&pUsbSpkrDev->ioDev, name, usbSpkrDrvNum));
    }




/*******************************************************************************
*
* usbSpkrDevDelete - delete a VxWorks device for an USB speaker
*
* This routine deletes a device on a specified serial channel.
*
* This routine is typically called by the USB speaker driver, when it detects
* a removal of a USB speaker.
*
* RETURNS: OK, or ERROR if the driver is not installed.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbSpkrDevDelete
    (
    USB_SPKR_DEV    * pUsbSpkrDev,      /* speaker device to read from */
    char            * filename          /* compatible with DRV_REMOVE_PTR */
    )
    {
    int     status = OK;                /* holder for the return value */

    if (usbSpkrDrvNum <= 0)
        {
        USB1_SPK_ERR("invalid parameter of usbSpkrDrvNum \n",
                     1, 2, 3, 4, 5, 6);
        errnoSet (S_ioLib_NO_DRIVER);
        return (ERROR);
        }

    /* remove the device from the I/O system */

    iosDevDelete (&pUsbSpkrDev->ioDev);

    /* remove from list */

    USB_SPKR_LIST_SEM_TAKE (WAIT_FOREVER);
    lstDelete (&usbSpkrList, (NODE *) pUsbSpkrDev->pUsbSpkrNode);
    USB_SPKR_LIST_SEM_GIVE;

    /* delete  wakes up all tasks pended. */

    selWakeupListTerm (&pUsbSpkrDev->selList);

    /* free memory for the device */

    free (pUsbSpkrDev->pUsbSpkrNode);
    pUsbSpkrDev->pUsbSpkrNode = NULL;
    free (pUsbSpkrDev);

    return (status);
    }


/*************************************************************************
*
* usbSpkrDevFind - find the USB_SPKR_DEV device for an SIO channel
*
* This function finds the USB_SPKR_DEV device for an SIO channel
*
* RETURNS: OK, or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbSpkrDevFind
    (
    SEQ_DEV *        pSeqDev,        /* pointer to affected SEQ_DEV */
    USB_SPKR_DEV **  ppUsbSpkrDev    /* pointer to an USB_SPKR_DEV */
    )
    {
    USB_SPKR_NODE *      pUsbSpkrNode = NULL;     /* pointer to USB device node */
    USB_SPKR_DEV *    pTempDev;        /* pointer to an USB_SPKR_DEV */

    if (pSeqDev == NULL)
        {
        USB1_SPK_ERR("invalid parameter of pSeqDev \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }
    /* protect it from other module's routines */

    USB_SPKR_LIST_SEM_TAKE (WAIT_FOREVER);

    /* loop through all the devices */

    for (pUsbSpkrNode = (USB_SPKR_NODE *) lstFirst (&usbSpkrList);
         pUsbSpkrNode != NULL;
         pUsbSpkrNode = (USB_SPKR_NODE *) lstNext ((NODE *) pUsbSpkrNode))
        {
        pTempDev = pUsbSpkrNode->pUsbSpkrDev;

        /* return as soon as you find it */
        if (pTempDev->pSeqDev == pSeqDev)
            {
            *ppUsbSpkrDev = (USB_SPKR_DEV *) pTempDev;
            USB_SPKR_LIST_SEM_GIVE;
            return (OK);
            }
        }
    USB1_SPK_ERR("USB_SPKR_DEV device not found \n",
                 1, 2, 3, 4, 5, 6);

    USB_SPKR_LIST_SEM_GIVE;

    return (ERROR);
    }

/*************************************************************************
*
* usbSpkrDrvAttachCallback - receives attach callbacks from speaker SIO driver
*
* This function is called whenever the speaker device is connected or
* removed.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void usbSpkrDrvAttachCallback
    (
    void    * arg,            /* caller-defined argument */
    SEQ_DEV * pSeqDev,        /* pointer to affected SEQ_DEV */
    UINT16    attachCode      /* defined as USB_SPKR_xxxx */
    )
    {
    USB_SPKR_DEV * pUsbSpkrDev = NULL;        /* speaker device */
    char    spkrName [SPKR_NAME_LEN_MAX];    /* speaker name */
    unsigned int i = 0;
    const char * pName;

    if (attachCode == USB_SPKR_ATTACH)
        {
        if (usbSpeakerSeqDevLock (pSeqDev) != OK)
            {
            USB1_SPK_ERR("usbSpeakerSeqDevLock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        else
            {
            for (i = 0; i < SPKR_MAX_DEVS; i++)
                {
                if (spkrDrvNames[i] == FALSE)
                    {
                    break;
                    }
                }

            if (i == SPKR_MAX_DEVS)
                {
                USB1_SPK_ERR("Could not install speaker, "
                             "because reached maximum \n",
                             1, 2, 3, 4, 5, 6);
                if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
                    {
                    USB1_SPK_ERR("usbSpeakerSeqDevUnlock () "
                                 "returned ERROR \n",
                                 1, 2, 3, 4, 5, 6);
                    }
                return;
                }

            spkrDrvNames[i] = TRUE;
            sprintf (spkrName, "%s%d", USB_SPKR_NAME, i);

            if (usbSpkrDevCreate (spkrName, pSeqDev) != OK)
                {
                spkrDrvNames[i] = FALSE;
                USB1_SPK_ERR("usbSpkrDevCreate() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
                    {
                    USB1_SPK_ERR("usbSpeakerSeqDevUnlock () "
                                 "returned ERROR \n",
                                 1, 2, 3, 4, 5, 6);
                    }
                return;
                }

            if (usbSpkrDevFind (pSeqDev, &pUsbSpkrDev) != OK)
                {
                USB1_SPK_ERR("usbSpkrDevFind() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                spkrDrvNames[i] = FALSE;
                if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
                    {
                    USB1_SPK_ERR("usbSpeakerSeqDevUnlock () "
                                 "returned ERROR \n",
                                 1, 2, 3, 4, 5, 6);
                    }
                return;
                }

            /* now we can increment the counter */
            spkrCount++;

            USB1_SPK_INFO("USB speaker attached as %s%d\n",
                          USB_SPKR_NAME, i, 3, 4, 5, 6);

            }
        }
    else if (attachCode == USB_SPKR_REMOVE)
        {
        /* find the related device */
        if (usbSpkrDevFind (pSeqDev, &pUsbSpkrDev) != OK)
            {
            USB1_SPK_ERR("usbSpkrDevFind could not find channel 0x%d \n",
                         pSeqDev, 2, 3, 4, 5, 6);
            return;
            }

        /* delete the device */

        pName = ((USB_SPKR_DEV *)pUsbSpkrDev)->ioDev.name;
        pName = pName + strlen(USB_SPKR_NAME);
        (void) sscanf(pName,"%d",&i);
        if (i < SPKR_MAX_DEVS)
            spkrDrvNames[i] = FALSE;
        else
            {
            USB1_SPK_ERR("Received strange device name : %s \n",
                         ((USB_SPKR_DEV *)pUsbSpkrDev)->ioDev.name,
                         2, 3, 4, 5, 6);
            }

        if (OK != usbSpkrDevDelete (pUsbSpkrDev, NULL))
            USB1_SPK_ERR("failed to remove device from ioslib \n",
                         1, 2, 3, 4, 5, 6);
        else
            {
            USB1_SPK_INFO("USB speaker %s%d removed \n",
                          USB_SPKR_NAME, i, 3, 4, 5, 6);
            pUsbSpkrDev = NULL;
            }

        /* now we can decrement the counter */
        spkrCount--;

        if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
            {
            USB1_SPK_ERR("usbSpeakerSeqDevUnlock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        }
    return;
    }

/*******************************************************************************
*
* usbSpkrOpen - open a usbSpkrDrv serial device.
*
* Increments a counter that holds the number of open paths to device.
*
* RETURNS: pointer to USB_SPKR_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbSpkrOpen
    (
    USB_SPKR_DEV    *pUsbSpkrDev,   /* speaker device to read from */
    char            *name,          /* device name */
    int             flags,          /* flags */
    int             mode            /* mode selected */
    )
    {

    pUsbSpkrDev->numOpen++;  /* increment number of open paths */

    return ((ULONG) pUsbSpkrDev);
    }

/*******************************************************************************
*
* usbSpkrDrvUnInit - shuts down an I/O USB speaker driver
*
* This is supplied to for the user, but it should be noted that iosDrvRemove()
* may cause unpredictable results.
*
* RETURNS: OK or ERROR, if not able to shutdown
*
* ERRNO: none
*/

STATUS usbSpkrDrvUnInit (void)
    {
    int        status = OK;        /* holder for the return value */

    if (!usbSpkrDrvNum)
        return status;

    /* remove the driver */

    if (usbSpkrDrvNum > 0)
        {
        if (iosDrvRemove (usbSpkrDrvNum, TRUE) != OK)
            {
            USB1_SPK_ERR("iosDrvRemove () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            status = ERROR;
            }
        }

    usbSpkrDrvNum = 0;

    /* delete the mutex semaphores */

    if (usbSpkrListMutex != NULL)
        {
        if (((STATUS) semDelete (usbSpkrListMutex)) == ERROR)
            {
            USB1_SPK_ERR("semDelete (usbSpkrListMutex) returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            status = ERROR;
            }
        usbSpkrListMutex = NULL;
        }

    /* unregister */

    if (usbSpeakerDynamicAttachUnRegister (usbSpkrDrvAttachCallback,
        (pVOID) NULL) != OK)
        {
        USB1_SPK_ERR("usbSpeakerDynamicAttachUnRegister () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        status = ERROR;
        }

    /* shutdown */

    if (usbSpeakerDevShutdown () != OK)
        {
        USB1_SPK_ERR("usbSpeakerDevShutdown () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        status = ERROR;
        }

    return status;
    }

/*******************************************************************************
*
* usbSpkrClose - close a usbSpkrDrv serial device.
*
* Decrements the counter of open paths to device.
*
* RETURNS: pointer to USB_SPKR_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbSpkrClose
    (
    USB_SPKR_DEV    * pUsbSpkrDev          /* speaker device to read from */
    )
    {

    (*pUsbSpkrDev->pSeqDev->sd_ioctl) (pUsbSpkrDev->pSeqDev,
                                       USB_SPKR_IOCTL_CLOSE_AUDIO_STREAM,
                                       1);
    if (pUsbSpkrDev->numOpen > 0)
        --pUsbSpkrDev->numOpen;

    return (ULONG)(pUsbSpkrDev);
    }

/*******************************************************************************
*
* usbSpkrIoctl - issue device control commands
*
* This routine issues device control commands to an USB speaker device.
*
* RETURNS: depends on the function invoked.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbSpkrIoctl
    (
    USB_SPKR_DEV    * pUsbSpkrDev,    /* speaker device to read from */
    int        request,        /* request code */
    void *    arg            /* some argument */
    )
    {

    return ((*pUsbSpkrDev->pSeqDev->sd_ioctl) (pUsbSpkrDev->pSeqDev,
                           request,
                           arg));
    }

/*******************************************************************************
*
* usbSpkrRead - read from the USB speaker
*
* This routines is a no-op for a read from the USB speaker.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbSpkrRead
    (
    USB_SPKR_DEV    * pUsbSpkrDev,           /* speaker device to read from */
    UCHAR           * pBuffer,
    size_t            nBytes
    )
    {

    return ((*pUsbSpkrDev->pSeqDev->sd_seqRd) (pUsbSpkrDev->pSeqDev,
                                               nBytes,
                                               pBuffer,
                                               FALSE));

    }

/*******************************************************************************
*
* usbSpkrWrite - write to the USB speaker
*
*
* USB mice don't like to be written to.  The don't support this feature.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

LOCAL ssize_t usbSpkrWrite
    (
    USB_SPKR_DEV    * pUsbSpkrDev,  /* speaker device to read from */
    UCHAR           * pBuffer,      /* buffer of data to write  */
    size_t            nBytes        /* number of bytes in buffer */
    )
    {

    return ((*pUsbSpkrDev->pSeqDev->sd_seqWrt) (pUsbSpkrDev->pSeqDev,
                                                nBytes,
                                                pBuffer,
                                                FALSE));
    }


/*****************************************************************************
*
* usrUsbSpkrInit - initialize the USB speaker driver
*
* This function initializes the USB speaker driver and registers for attach
* callbacks.
*
* RETURNS: N/A
*
* ERRNO: none
*/

STATUS usrUsbSpkrInit (void)
    {

    UINT8  i = 0;

    /* Check if driver already installed */

    if (usbSpkrDrvNum > 0)
        {
        USB1_SPK_ERR("Speaker driver already initilaized \n",
                     1, 2, 3, 4, 5, 6);
        return OK;
        }

    for (i = 0; i < SPKR_MAX_DEVS; i++)
        spkrDrvNames[i] = FALSE;

    if (usbSpeakerDevInit () == OK)
        {
        usbSpkrListMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                                       SEM_INVERSION_SAFE);

        if (usbSpkrListMutex == NULL)
            {
            USB1_SPK_ERR("Mutex creation failed \n",
                         1, 2, 3, 4, 5, 6);
            /* un - intialize the speaker class driver */

            (void) usbSpeakerDevShutdown ();

            return ERROR;
            }
        /* initialize the linked list */

        lstInit (&usbSpkrList);

        /* install the driver */

        usbSpkrDrvNum = iosDrvInstall ((DRV_CREATE_PTR) NULL,
                                       (DRV_REMOVE_PTR) usbSpkrDevDelete,
                                       (DRV_OPEN_PTR) usbSpkrOpen,
                                       (DRV_CLOSE_PTR) usbSpkrClose,
                                       (DRV_READ_PTR) usbSpkrRead,
                                       (DRV_WRITE_PTR) usbSpkrWrite,
                                       (DRV_IOCTL_PTR) usbSpkrIoctl);

        if (usbSpkrDrvNum <= 0)
            {
            errnoSet (S_ioLib_NO_DRIVER);
            USB1_SPK_ERR("There is no more room in the driver table \n",
                         1, 2, 3, 4, 5, 6);
            /* uninitialize the list */

            lstFree (&usbSpkrList);

            /* delete the mutex created */

            (void) semDelete (usbSpkrListMutex);
            usbSpkrListMutex = NULL;

            /* un - intialize the speaker class driver */

            (void) usbSpeakerDevShutdown ();

            return (ERROR);
            }

        if (usbSpeakerDynamicAttachRegister (usbSpkrDrvAttachCallback,
                                             (void *) NULL)
                                          != OK)
            {
            USB1_SPK_ERR("usbSpeakerDynamicAttachRegister() failed \n",
                         1, 2, 3, 4, 5, 6);
            /* remove the driver */

            (void) iosDrvRemove (usbSpkrDrvNum, TRUE);
            usbSpkrDrvNum = 0;

            /* uninitialize the list */

            lstFree (&usbSpkrList);

            /* delete the mutex created */

            (void) semDelete (usbSpkrListMutex);
            usbSpkrListMutex = NULL;

            /* un - intialize the speaker class driver */

            (void) usbSpeakerDevShutdown ();
            return ERROR;
            }
        }
    else
        {
        USB1_SPK_ERR("usbSpeakerDevInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    USB1_SPK_INFO("USB Speaker Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;
    }

#endif /* __INCusrUsbSpkrInitc */
