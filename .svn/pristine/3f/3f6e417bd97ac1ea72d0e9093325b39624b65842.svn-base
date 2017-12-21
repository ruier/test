/* usrUsbMseInit.c - Initialization of the USB Mouse driver */

/*
 * Copyright (c) 1999-2005, 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01w,06may13,s_z  Remove compiler warning (WIND00356717)
01v,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01u,07jan11,ghs  Clean up compile warnings (WIND00247082)
01t,28jul10,ghs  Fix device name display error(WIND00223154)
01s,05jul10,m_y  Set all the resources to NULL after release (WIND00183499)
01r,05jul10,ghs  Modify display logs(WIND00216586)
01q,04jun10,s_z  Fix multiple mouses plugging issue (WIND00214977)
01p,27apr10,j_x  Error debug adaption
01o,18mar10,j_x  Changed for USB debug (WIND00184542)
01n,13jan10,ghs  vxWorks 6.9 LP64 adapting
01m,26aug05,ami  Select Feature Support Added
01l,10aug05,ami  Deinitialization routine modified
01k,09apr05,ami  SEL_WAKEUP_LIST member and selWakeupListInit removed. (Fix
                 for exception for multiple connects/disconnects).
01j,25oct04,hch  Fixed SPR #98731 and SPR #101127
01i,15oct04,ami  Apigen Changes
01h,25sep03,cfc  fix mouse name when removed
01g,23sep03,cfc  Fixed issue of overflowing driver table
01f,04sep03,cfc  Decrement mouse counter when unplugged
01e,30jan02,wef  fixed DevFind routine
01d,08dec01,wef  fixing warnings
01c,07dec01,wef  fixed some warnings
01b,02feb01,wef  Added ios functionality - allows for open, close, read,
                 ioctl, etc access to the printer
01a,23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB mouse driver.  This assumes the
USB host stack has already been initialized and has a host controller
driver attached.

This configlette demonstrates how a user might integrate a USB class
driver into the vxWorks file system.  usbMseDevCreate () intalls a USB
mouse and its associated driver functions into the driver table allowing
the mouse to be accesed with standard fopen, close, read, write, etc. type
calls.

INCLUDE FILES: errnoLib.h, drv/usb/usbMouseLib.h

*/

#ifndef __INCusrUsbMseInitc
#define __INCusrUsbMseInitc

/* includes */

#include "drv/usb/usbMouseLib.h"
#include "usb/usbHid.h"
#include "vxWorks.h"
#include "iv.h"
#include "ioLib.h"
#include "iosLib.h"
#include "tyLib.h"
#include "intLib.h"
#include "errnoLib.h"
#include "sioLib.h"
#include "stdlib.h"
#include "stdio.h"
#include "logLib.h"
#include "selectLib.h"


/* defines */

#define TX_BUF_NUM                      0x10000

#define USB_MSE_MUTEX_TAKE(tmout)       \
    semTake (usbMseMutex, (_Vx_ticks_t) (tmout))

#define USB_MSE_MUTEX_GIVE              \
    semGive (usbMseMutex)

#define USB_MSE_LIST_SEM_TAKE(tmout)    \
    semTake (usbMseListMutex, (_Vx_ticks_t) (tmout))

#define USB_MSE_LIST_SEM_GIVE           \
    semGive (usbMseListMutex)


#define MSE_NAME_LEN_MAX        100

#define USB_MSE_NAME            "/usbMo/"

#ifndef USB_MAX_MOUSE_SUPPORTED
#define USB_MAX_MOUSE_SUPPORTED 8
#endif

/* data types */

/* typedefs */

typedef struct usb_mse_node
    {
    NODE                node;
    struct usb_mse_dev  * pUsbMseDev;
    } USB_MSE_NODE;

typedef struct usb_mse_dev /* USB_MSE_DEV */
    {
    DEV_HDR             ioDev;
    SIO_CHAN          * pSioChan;
    UINT16              numOpen;
    UINT32              bufSize;
    UCHAR             * buff;
    USB_MSE_NODE      * pUsbMseNode;
    SEL_WAKEUP_LIST     selWakeupList;  /* list of tasks pended on select. */
    BOOL                mseDataFlag;    /* Notifier when data arrives. */
    SEM_ID              mseSelectSem;
    } USB_MSE_DEV, *pUSB_MSE_DEV;


/* local variables */

LOCAL SEM_ID    usbMseMutex;        /* mutex semaphore */
LOCAL SEM_ID    usbMseListMutex;    /* mutex semaphore to protect list */
LOCAL UINT32    mseCount = 0;
LOCAL LIST      usbMseList;         /* all USB mouses in the system */

LOCAL SIO_CHAN * pMseDevArray[USB_MAX_MOUSE_SUPPORTED];

LOCAL HID_MSE_BOOT_REPORT * pReportlocal;

LOCAL int usbMseDrvNum = 0;         /* driver number */

/* forward declarations */

LOCAL void  usbMseDrvAttachCallback (void * arg, SIO_CHAN *pChan,
                                     UINT16 attachCode);

LOCAL STATUS  usbMseRptCallback (void *putReportArg,
                                   pHID_MSE_BOOT_REPORT pReport);


LOCAL ssize_t  usbMseRead (USB_MSE_DEV * pUsbMseDev,UCHAR *buffer,
                           size_t nBytes);

LOCAL ULONG  usbMseClose (USB_MSE_DEV * pUsbMseDev);

LOCAL ssize_t  usbMseWrite (USB_MSE_DEV * pUsbMseDev, UCHAR * buffer,
                            size_t nBytes);

LOCAL ULONG  usbMseOpen (USB_MSE_DEV * pUsbMseDev, char * name,
                         int flags, int mode);

LOCAL int   usbMseIoctl (USB_MSE_DEV * pUsbMseDev, int request, void * arg);

LOCAL STATUS    usbMseDevFind (SIO_CHAN *pChan, USB_MSE_DEV ** ppUsbMseDev);

LOCAL STATUS    usbMseDevDelete (USB_MSE_DEV * pUsbMseDev, char * filename);


/*************************************************************************
*
* usbMseGetDeviceIndex - gets one free devcie index for a mouse
*
* This routine supports multiple mouse being connected and
* disconnected.  Previous versions of the software used a simple counter
* and assigned unit numbers based on the counter.  That scheme falls
* apart if the mouse are connected/disconnected in random order.
*
* This routine simply returns the next available mouse number
*
* RETURNS: Next available device index number.  -1 if no index available.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int usbMseGetDeviceIndex
    (
    SIO_CHAN *pChan
    )
    {
    int index;

    for (index = 0; index < USB_MAX_MOUSE_SUPPORTED; index++)
        {
        if (pMseDevArray[index] == 0)
            {
            pMseDevArray[index] = pChan;
            return (index);
            }
        }

    printf("\n Out of max supported mouse.  Check USB_MAX_MOUSE_SUPPORTED");

    return(-1);
    }

/*************************************************************************
*
* usbMseFreeDeviceIndex - frees up the next unit number for a mouse
*
* This routine supports multiple mouses being connected and
* disconnected.  Previous versions of the software used a simple counter
* and assigned unit numbers based on the counter.  That scheme falls
* apart if the mouses are connected/disconnected in random order.
*
* This routine frees up the previously allocated mouse number
*
* RETURNS: Freed mouse number.  -1 if not found.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int usbMseFreeDeviceIndex
    (
    SIO_CHAN *pChan
    )
    {
    int index;

    for (index = 0; index < USB_MAX_MOUSE_SUPPORTED; index++)
        {
        if (pMseDevArray[index] == pChan)
            {
            pMseDevArray[index] = NULL;
            return (index);
            }
        }

    printf("\n Unable to locate mouse pointed to by channel %p.",pChan);

    return(-1);
    }


/*******************************************************************************
*
* usbMseDevCreate - create a VxWorks device for an USB mouse
*
* This routine creates a device on a specified serial channel.  Each channel
* to be used should have exactly one device associated with it by calling
* this routine.
*
* For instance, to create the device "/ /0", the proper call would be:
* \cs
*     usbMseDevCreate ("/usbMo/0", pSioChan);
* \ce
* Where pSioChan is the address of the underlying SIO_CHAN serial channel
* descriptor (defined in sioLib.h).
* This routine is typically called by the USB mouse driver, when it detects
* an insertion of a USB mouse.
*
* RETURNS: OK, or ERROR if the driver is not installed, or the
* device already exists, or failed to allocate memory.
*
* ERRNO: none
*/

LOCAL STATUS usbMseDevCreate
    (
    char        * name,         /* name to use for this device      */
    SIO_CHAN    * pSioChan      /* pointer to core driver structure */
    )
    {
    USB_MSE_NODE    * pUsbMseNode = NULL;   /* pointer to device node */
    USB_MSE_DEV     * pUsbMseDev = NULL;     /* pointer to USB device */
    STATUS          status = ERROR;

    if (pSioChan == (SIO_CHAN *) ERROR)
        {
        return (ERROR);
        }

    /* allocate memory for the device */

    if ((pUsbMseDev = (USB_MSE_DEV *) calloc (1, sizeof (USB_MSE_DEV))) == NULL)
        return (ERROR);

    pUsbMseDev->pSioChan = pSioChan;

    /* allocate memory for this node, and populate it */

    pUsbMseNode = (USB_MSE_NODE *) calloc (1, sizeof (USB_MSE_NODE));

    if (NULL == pUsbMseNode)
        {
        pUsbMseDev->pSioChan = NULL;
        free(pUsbMseDev);
        return ERROR;
        }

    /* record useful information */

    pUsbMseNode->pUsbMseDev = pUsbMseDev;
    pUsbMseDev->pUsbMseNode = pUsbMseNode;

    status = iosDevAdd (&pUsbMseDev->ioDev, name, usbMseDrvNum);

    /* Initialize the list holding the list of tasks pended on the driver. */

    selWakeupListInit(&pUsbMseDev->selWakeupList);

    /* add the node to the list */

    USB_MSE_LIST_SEM_TAKE (WAIT_FOREVER);

    lstAdd (&usbMseList, (NODE *) pUsbMseNode);

    USB_MSE_LIST_SEM_GIVE;

    /* start the device in the only supported mode */

    sioIoctl (pUsbMseDev->pSioChan, SIO_MODE_SET, (void *) SIO_MODE_INT);

    /* add the device to the I/O system */

    return (status);
    }


/*******************************************************************************
*
* usbMseDevDelete - delete a VxWorks device for an USB mouse
*
* This routine deletes a device on a specified serial channel.
*
* This routine is typically called by the USB mouse driver, when it detects
* a removal of a USB mouse.
*
* RETURNS: OK, or ERROR if the driver is not installed.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbMseDevDelete
    (
    USB_MSE_DEV * pUsbMseDev,       /* mouse device to read from */
    char        * fileName          /* compatible with DRV_REMOVE_PTR */
    )
    {
    int         status = OK;        /* holder for the return value */

    if (usbMseDrvNum <= 0)
        {
        errnoSet (S_ioLib_NO_DRIVER);
        return (ERROR);
        }

    /*
     * Terminate and free the select wake-up list. Call selWakeUpAll
     * in case there is data. This should also unblock all tasks waiting
     * on the driver
     */

    selWakeupAll (&pUsbMseDev->selWakeupList, SELREAD);

    /* create a semaphore */

    if ((pUsbMseDev->mseSelectSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY)) ==
         NULL)
        return ERROR;

    if (selWakeupListLen (&pUsbMseDev->selWakeupList) > 0)
        {
        /*
         * There are task pending on the list wait of signal to
         * unpend the task before terminating the list
         */

        /* wait on this semphore to be signaled */

        semTake (pUsbMseDev->mseSelectSem, WAIT_FOREVER);
        }

    /* delete the semaphore */

    (void) semDelete (pUsbMseDev->mseSelectSem);
    pUsbMseDev->mseSelectSem = NULL;

    selWakeupListTerm(&pUsbMseDev->selWakeupList);

    /* remove the device from the I/O system */

    status = iosDevDelete (&pUsbMseDev->ioDev);

    /* remove from list */

    USB_MSE_LIST_SEM_TAKE (WAIT_FOREVER);
    lstDelete (&usbMseList, (NODE *) pUsbMseDev->pUsbMseNode);
    USB_MSE_LIST_SEM_GIVE;

    /* free memory for the device */

    free (pUsbMseDev->pUsbMseNode);
    pUsbMseDev->pUsbMseNode = NULL;
    free (pUsbMseDev);

    return (status);
    }


/*************************************************************************
*
* usbMseDevFind - find the USB_MSE_DEV device for an SIO channel
*
* This function  finds the USB_MSE_DEV device for an SIO channel
*
* RETURNS: OK, or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbMseDevFind
    (
    SIO_CHAN *      pChan,          /* pointer to affected SIO_CHAN */
    USB_MSE_DEV **  ppUsbMseDev     /* pointer to an USB_MSE_DEV */
    )
    {
    USB_MSE_NODE *      pUsbMseNode = NULL;     /* pointer to USB device node */
    USB_MSE_DEV *       pTempDev;               /* pointer to an USB_MSE_DEV */

    if (pChan == NULL)
        return (ERROR);

    /* protect it from other module's routines */

    USB_MSE_LIST_SEM_TAKE (WAIT_FOREVER);

    /* loop through all the devices */

    for (pUsbMseNode = (USB_MSE_NODE *) lstFirst (&usbMseList);
         pUsbMseNode != NULL;
         pUsbMseNode = (USB_MSE_NODE *) lstNext ((NODE *) pUsbMseNode))
        {
        pTempDev = pUsbMseNode->pUsbMseDev;

        /* return as soon as you find it */
        if (pTempDev->pSioChan == pChan)
            {
            *ppUsbMseDev = (USB_MSE_DEV *) pTempDev;
            USB_MSE_LIST_SEM_GIVE;
            return (OK);
            }
        }

    USB_MSE_LIST_SEM_GIVE;

    return (ERROR);
    }

/*************************************************************************
*
* usbMseDrvAttachCallback - receives attach callbacks from mouse SIO driver
*
* This callback function is called everytime a mouse is connected or
* disconnected
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void usbMseDrvAttachCallback
    (
    void *     arg,          /* caller-defined argument */
    SIO_CHAN * pChan,        /* pointer to affected SIO_CHAN */
    UINT16     attachCode    /* defined as USB_KBD_xxxx */
    )
    {
    USB_MSE_DEV * pUsbMseDev = NULL;    /* mouse device */
    char mseName [MSE_NAME_LEN_MAX];    /* mouse name */
    int     mseIndex = 0;

    if (attachCode == USB_MSE_ATTACH)
        {
        if (usbMouseSioChanLock (pChan) != OK)
            {
            USB1_MSE_ERR("usbMouseSioChanLock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            }
        else
            {
            mseIndex = usbMseGetDeviceIndex(pChan);

            if (mseIndex >= 0)
                {
                snprintf (mseName, MSE_NAME_LEN_MAX - 1, 
                          "%s%d", USB_MSE_NAME, mseIndex);
                if (usbMseDevCreate (mseName, pChan) != OK)
                    {
                    USB1_MSE_ERR("usbMseDevCreate () returned ERROR \n",
                                 1, 2, 3, 4, 5, 6);

                    return;
                    }
                }
            else
                {
                USB1_MSE_ERR("Excessive mouse - check "
                             "USB_MAX_MOUSE_SUPPORTED = %d\n",
                             USB_MAX_MOUSE_SUPPORTED, 2, 3, 4, 5, 6);

                return;
                }
            /* now we can increment the counter */
            mseCount++;

            if (usbMseDevFind (pChan, &pUsbMseDev) != OK)
                {
                USB1_MSE_ERR("usbMseDevFind () returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                return;
                }

            if ((*pChan->pDrvFuncs->callbackInstall)
                    (pChan,
                    SIO_CALLBACK_PUT_MOUSE_REPORT,
                    (STATUS  (*) (void *, ...)) usbMseRptCallback,
                    (void *) pUsbMseDev)
                    != OK)
                {
                USB1_MSE_ERR("usbMseRptCallback () failed to install \n",
                             1, 2, 3, 4, 5, 6);
                return;
                }
            USB1_MSE_INFO("USB mouse attached as %s%d\n",
                          USB_MSE_NAME, mseIndex, 3, 4, 5, 6);
            }
        }
    else if (attachCode == USB_MSE_REMOVE)
        {
        /* find the related device */

        if (usbMseDevFind (pChan, &pUsbMseDev) != OK)
            {
            USB1_MSE_ERR("usbMseDevFind could not find channel %p \n",
                         pChan, 2, 3, 4, 5, 6);
            return;
            }

        /* delete the device */

        usbMseDevDelete (pUsbMseDev, NULL);
        pUsbMseDev = NULL;
        mseIndex = usbMseFreeDeviceIndex(pChan);

        if (usbMouseSioChanUnlock (pChan) != OK)
            {
            USB1_MSE_ERR("usbMouseSioChanUnlock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        USB1_MSE_INFO("USB mouse %s%d removed \n",
                      USB_MSE_NAME, mseIndex, 3, 4, 5, 6);
        /* now we can decrement the counter */

        mseCount--;
        }
    }

/*******************************************************************************
*
* usbMseOpen - open a usbMseDrv serial device.
*
* Increments a counter that holds the number of open paths to device.
*
* RETURNS: pointer to USB_MSE_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbMseOpen
    (
    USB_MSE_DEV * pUsbMseDev,   /* mouse device to read from */
    char        * name,         /* device name */
    int           flags,        /* flags */
    int           mode          /* mode selected */
    )
    {

    pUsbMseDev->numOpen++;  /* increment number of open paths */

    sioIoctl (pUsbMseDev->pSioChan, SIO_OPEN, NULL);

    /* reset the flag */

    pUsbMseDev->mseDataFlag = FALSE;

    return ((ULONG) pUsbMseDev);
    }

/*******************************************************************************
*
* usbMseDrvUnInit - shuts down an I/O USB mouse driver
*
* This is supplied to for the user, but it should be noted that iosDrvRemove()
* may cause unpredictable results.
*
* RETURNS: OK or ERRNO, if not able to shutdown the USB Mouse Driver
*
* ERRNO: none
*/

STATUS usbMseDrvUnInit (void)
    {
    if (!usbMseDrvNum)
        return (OK);


    /* unregister */

    if (usbMouseDynamicAttachUnRegister (usbMseDrvAttachCallback,
                                         NULL)
                                        != OK)
        {
        USB1_MSE_ERR("usbMouseDynamicAttachUnRegister () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* remove the driver */

    if (iosDrvRemove (usbMseDrvNum, TRUE) != OK)
        {
        USB1_MSE_ERR("iosDrvRemove () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* HELP */
    usbMseDrvNum = 0;

    /* delete the mutex semaphores */

    if (usbMseMutex != NULL)
        {
        semTake(usbMseMutex, WAIT_FOREVER);
        if (semDelete (usbMseMutex) == ERROR)
            {
            USB1_MSE_ERR("semDelete (usbMseMutex) returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return (ERROR);
            }
        usbMseMutex = NULL;
        }

    if (usbMseListMutex != NULL)
        {
        semTake(usbMseListMutex, WAIT_FOREVER);
        if (semDelete (usbMseListMutex) == ERROR)
            {
            USB1_MSE_ERR("semDelete (usbMseListMutex) returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return (ERROR);
            }
        usbMseListMutex = NULL;
        }

    /* shutdown */

    if (usbMouseDevShutdown () != OK)
        {
        USB1_MSE_ERR("usbMouseDevShutDown () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* usbMseClose - close a usbMseDrv serial device.
*
* Decrements the counter of open paths to device and alerts the driver
* with an ioctl call when the count reaches zero. This scheme is used to
* implement the HUPCL(hang up on last close).
*
* RETURNS: pointer to USB_MSE_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbMseClose
    (
    USB_MSE_DEV * pUsbMseDev          /* mouse device to read from */
    )
    {

    if (pUsbMseDev->numOpen > 0)
        --pUsbMseDev->numOpen;

    /* if there are no open channels */

    if (0 == pUsbMseDev->numOpen)
        {
        sioIoctl (pUsbMseDev->pSioChan, SIO_HUP, NULL);
        }

    return ((ULONG) pUsbMseDev);
    }

/*******************************************************************************
*
* usbMseIoctl - issue device control commands
*
* This routine issues device control commands to an USB mouse device.
*
* RETURNS: depends on the function invoked.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbMseIoctl
    (
    USB_MSE_DEV *   pUsbMseDev, /* mouse device to read from */
    int             request,    /* request code */
    void *          arg         /* some argument */
    )
    {

    int errNo = OK ;

    switch (request)
        {

        case FIOSELECT:

            errNo = selNodeAdd (&pUsbMseDev->selWakeupList,
                                (SEL_WAKEUP_NODE *) arg) ;

            if (errNo == OK)
                {

                /* If data is available wake up the task that's waiting for it. */

                if (pUsbMseDev->mseDataFlag &&
                   (selWakeupType ((SEL_WAKEUP_NODE *) arg) == SELREAD ))
                    {

                    /* data available, make sure task does not pend */

                    selWakeup ((SEL_WAKEUP_NODE *) arg);
                    }
                }

            return errNo;

        case FIOUNSELECT:

            /* delete the node */

            errNo = selNodeDelete (&pUsbMseDev->selWakeupList,
                                   (SEL_WAKEUP_NODE *) arg);

             /*
              * check if the number of  task pending is 0 and
              * the semaphore  <kbdSelectSem> is created if so the
              * device is disconnected. Signal the
              * semaphore and return ERROR
              */

             if ((selWakeupListLen (&pUsbMseDev->selWakeupList) == 0)
                 && (pUsbMseDev->mseSelectSem != NULL))
                 {
                 semGive (pUsbMseDev->mseSelectSem);
                 return ERROR;
                 }

             else
                 return  errNo;

        default:
            return (sioIoctl (pUsbMseDev->pSioChan, request, arg));

        }

    }

/*******************************************************************************
*
* usbMseRead - read from the USB mouse
*
* This routines is a no-op for a read from the USB mouse.
*
* RETURNS: size of HID_MSE_BOOT_REPORT
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbMseRead
    (
    USB_MSE_DEV * pUsbMseDev,           /* mouse device to read from */
    UCHAR       * buffer,
    size_t        nBytes
    )
    {

    USB_MSE_MUTEX_TAKE(WAIT_FOREVER);

    memcpy(buffer, pReportlocal, nBytes);

    pUsbMseDev->mseDataFlag = FALSE;

    /* This shows the mouse coordinate */

    USB1_MSE_DBG("%s[L%d]: x:%d  y:%d  button:%d \n",
                 __FILE__, __LINE__,
                 pReportlocal->xDisplacement,
                 pReportlocal->xDisplacement,
                 pReportlocal->buttonState, 6);

    USB_MSE_MUTEX_GIVE;

    return (sizeof(HID_MSE_BOOT_REPORT));
    }

/*******************************************************************************
*
* usbMseWrite - write to the USB mouse
*
* USB mice don't like to be written to.  The don't support this feature.
*
* RETURNS: ENOSYS
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbMseWrite
    (
    USB_MSE_DEV * pUsbMseDev,   /* mouse device to read from */
    UCHAR       * buffer,       /* buffer of data to write  */
    size_t        nBytes        /* number of bytes in buffer */
    )
    {

    return (ENOSYS);
    }

/*************************************************************************
*
* usbMseRptCallback - invoked when reports received from mouse
*
* The mouse returns 3 bytes.  The first two describe its x, y displacement
* from its previous position.  The third describes the button state
* of its 3 buttons.
*
* In the third byte the first 3 bits represent the state of the buttons as
* follows:  bit 1 = the middle button, bit 2 = the left button and bit 3 =
* the right button.
*
* These three bytes are containied in a HID_MSE_BOOT_REPORT structure.
* this fucntion copys these 3 bytes into a local copy that can be accessed
* with the mouse read routine.
*
*
* RETURNS: OK
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbMseRptCallback
    (
    void *               putReportArg,
    pHID_MSE_BOOT_REPORT pReport
    )

    {

    pUSB_MSE_DEV pUsbMseDev = (pUSB_MSE_DEV)putReportArg;

    pReportlocal = pReport;


    /* If we have elements in the Wake up list, wake them all up! */

    if (selWakeupListLen(&pUsbMseDev->selWakeupList) > 0)
        {
        selWakeupAll (&pUsbMseDev->selWakeupList, SELREAD);
        }

    /* set the flagto indicate that there is data in the mouse */

    USB_MSE_MUTEX_TAKE (WAIT_FOREVER);
    pUsbMseDev->mseDataFlag = TRUE;
    USB_MSE_MUTEX_GIVE;

    return OK;
    }

/*****************************************************************************
*
* usrUsbMseInit - initialize the USB mouse driver
*
* This function initializes the USB mouse driver and registers for attach
* callbacks.
*
* RETURNS: OK or Error , if not able to initialize
*
* ERRNO: none
*/

STATUS usrUsbMseInit (void)
    {
    UINT8 index;

    /* Check if driver already installed */

    if (usbMseDrvNum > 0)
        {
        USB1_MSE_ERR("Mouse driver has already initilaized \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    usbMseMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                              SEM_INVERSION_SAFE);

    usbMseListMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                                  SEM_INVERSION_SAFE);
    if (usbMseMutex == NULL ||
        usbMseListMutex == NULL)
        {
        USB1_MSE_ERR("Resource Allocation Failure \n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }


    lstInit (&usbMseList);

    usbMseDrvNum = iosDrvInstall ((DRV_CREATE_PTR) NULL,
                                  (DRV_REMOVE_PTR) usbMseDevDelete,
                                  (DRV_OPEN_PTR) usbMseOpen,
                                  (DRV_CLOSE_PTR) usbMseClose,
                                  (DRV_READ_PTR) usbMseRead,
                                  (DRV_WRITE_PTR) usbMseWrite,
                                  (DRV_IOCTL_PTR) usbMseIoctl);

    if (usbMseDrvNum <= 0)
        {
        USB1_MSE_ERR("problem installing mouse driver \n",
                     1, 2, 3, 4, 5, 6);
        errnoSet (S_ioLib_NO_DRIVER);
        goto ERROR_HANDLER;
        }

    for (index = 0; index < USB_MAX_MOUSE_SUPPORTED; index++)
         {
         pMseDevArray[index] = NULL;
         }

    if (usbMouseDevInit () == OK)
        {
        pReportlocal = OSS_MALLOC (sizeof (HID_MSE_BOOT_REPORT));

        if (pReportlocal == NULL)
            {
            USB1_MSE_ERR("could not allocate memory \n",
                         1, 2, 3, 4, 5, 6);
            (void) usbMouseDevShutdown();
            goto ERROR_HANDLER;
            }

        if (usbMouseDynamicAttachRegister (usbMseDrvAttachCallback,
                                           (void *) NULL)
                                          != OK)
            {
            USB1_MSE_ERR("usbMouseDynamicAttachRegister() return ERROR \n",
                         1, 2, 3, 4, 5, 6);
            (void) usbMouseDevShutdown();
            OSS_FREE(pReportlocal);
            pReportlocal = NULL;
            goto ERROR_HANDLER;
            }
        }
    else
        {
        USB1_MSE_ERR("usbMouseDevInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }

    USB1_MSE_INFO("USB Mouse Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;

ERROR_HANDLER:
    if(usbMseDrvNum)
       {
       (void) iosDrvRemove (usbMseDrvNum, 1);
       usbMseDrvNum = 0;
       }

    if (usbMseMutex != NULL)
       (void) semDelete(usbMseMutex);

    if(usbMseListMutex != NULL)
       (void) semDelete(usbMseListMutex);

    usbMseMutex = NULL;
    usbMseListMutex = NULL;
    return ERROR;

    }

#endif /* __INCusrUsbMseInitc */
