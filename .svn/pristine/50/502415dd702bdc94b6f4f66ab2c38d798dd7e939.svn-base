/* usrUsbPrnInit.c - Initialization of the USB Printer driver */

/*
 * Copyright (c) 2000, 2001, 2004-2008, 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01y,06may13,s_z  Remove compiler warning (WIND00356717)
01x,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01w,07jan11,ghs  Clean up compile warnings (WIND00247082)
01v,05jul10,m_y  Add valid check after allocate resource (WIND00183499)
01u,05jul10,ghs  Modify display logs(WIND00216586)
01t,22jun10,m_y  Changed to remove build warning (WIND00217394)
01s,27apr10,j_x  Error debug adaption
01r,18mar10,j_x  Changed for USB debug (WIND00184542)
01q,03mar10,ghs  Fix LP64 adapting(WIND00202075)
01p,13jan10,ghs  vxWorks 6.9 LP64 adapting
01o,14mar08,j_x  Removed linked list usbPrnList (WIND00115332)
01n,02aug07,adh  WIND00097541 fix, usbPrnWrite, counter variable too small
01m,01mar06,ami  Fix for Printer Hang when out of Paper (SPR #111242)
01l,22sep05,ami  write call made blocking (SPR #106633) & select library call
                 removed
01k,11aug05,ami  Initialization routine made to return a status
01j,09apr05,ami  selWakeupListTerm added to terminate a select( ) wake-up
                 list
01i,28dec04,mta  SPR104701,SPR104702:Warning Removal
01h,15oct04,ami  Apigen Changes
01g,07oct04,pdg  Updation of review comments on the fix for SPR #91160
01f,05oct04,pdg  Fix for SPR #91160
01e,11aug04,hch  Decreased the printer count when printer is removed
01d,08dec01,wef  fixing warnings
01c,07dec01,wef  fixed some warnings.
01b,02feb01,wef  Added ios functionality - allows for open, close, read,
                 ioctl, etc access to the printer.
01a,23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB printer driver.  This assumes the
USB host stack has already been initialized and has a host controller
driver attached.

This configlette demonstrates how a user might integrate a USB class
driver into the vxWorks file system.  usbPrnDevCreate () intalls a USB
printer and its associated driver functions into the driver table allowing
the printer to be accesed with standard fopen, close, read, write, etc. type
calls.  The decision to keep this functionality out of the driver itself
was made for backwards compatability reasons.

INCLUDE FILES: iosLib.h, vxWorks.h, iv.h, ioLib.h, tyLib.h, intLib.h,
               errnoLib.h, sioLib.h, stdlib.h, stdio.h, logLib.h,
               selectLib.h, drv/usb/usbPrinterLib.h, usb/usbPrinter.h

*/

#ifndef __INCusrUsbPrnInitc
#define __INCusrUsbPrnInitc

/* includes */

#include "drv/usb/usbPrinterLib.h"
#include "usb/usbPrinter.h"
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

#define TX_BUF_NUM                  0x10000

#define USB_PRN_MUTEX_TAKE(tmout)       \
    semTake (usbPrnMutex, (_Vx_ticks_t) (tmout))

#define USB_PRN_MUTEX_GIVE              \
    semGive (usbPrnMutex)

#define PRN_NAME_LEN_MAX            20
#define USB_PRN_NAME                "/usbPr/"


#ifndef PRN_MAX_DEVS

#define PRN_MAX_DEVS                2

#endif

#define WRITE_TIMEOUT_SEC           60


#define PRINT_SIZE                  4096

/* data types */

/* typedefs */

typedef struct usb_prn_node
    {
    NODE                node;
    struct usb_prn_dev  * pUsbPrnDev;
    } USB_PRN_NODE;

typedef struct usb_prn_dev /* USB_PRN_DEV */
    {
    DEV_HDR             ioDev;
    SIO_CHAN    *       pSioChan;
    UINT16              numOpen;
    UINT32              bufSize;
    UCHAR       *       buff;
    SEM_ID              usbPrnDevSem;
    STATUS              bufResult;
    BOOL                usbPrnDevFree;
    BOOL                usbPrnDevStable;
    } USB_PRN_DEV;


/* local variables */


LOCAL USB_PRN_DEV   usbPrnDeviceTbl[PRN_MAX_DEVS];
LOCAL SEM_ID        usbPrnMutex;            /* mutex semaphore */

LOCAL int usbPrnDrvNum = 0;                 /* driver number */

/* forward declarations */

LOCAL void  usbPrnDrvAttachCallback (void * arg, SIO_CHAN *pChan,
                                     UINT16 attachCode);

LOCAL ssize_t usbPrnRead (USB_PRN_DEV * pUsbPrnDev,
                          char * buffer, size_t maxBytes);

LOCAL ULONG usbPrnClose (USB_PRN_DEV * pUsbPrnDev);

LOCAL ssize_t   usbPrnWrite (USB_PRN_DEV * pUsbPrnDev, UCHAR * buffer,
                             size_t nBytes);

LOCAL ULONG usbPrnOpen (USB_PRN_DEV * pUsbPrnDev, char * name,
                        int flags, int mode);

LOCAL STATUS    usbPrnTxCallback (void *, char *, STATUS);

LOCAL int       usbPrnIoctl (USB_PRN_DEV * pUsbPrnDev, int request, void * arg);

LOCAL STATUS    usbPrnDevDelete (USB_PRN_DEV * pUsbPrnDev, char * filename);


/*******************************************************************************
*
* usbPrnDevCreate - create a VxWorks device for an USB printer
*
* This routine creates a device on a specified serial channel.  Each channel
* to be used should have exactly one device associated with it by calling
* this routine.
*
* For instance, to create the device "/ /0", the proper call would be:
* .CS
*     usbPrnDevCreate ("/usbPr/0", pSioChan);
* .CE
* Where pSioChan is the address of the underlying SIO_CHAN serial channel
* descriptor (defined in sioLib.h).
* This routine is typically called by the USB printer driver, when it detects
* an insertion of a USB printer.
*
* RETURNS: OK, or ERROR if the driver is not installed, or the
* device already exists, or failed to allocate memory.
*
* ERRNO: none
*/

LOCAL STATUS usbPrnDevCreate
    (
    char        * name,         /* name to use for this device      */
    USB_PRN_DEV * pUsbPrnDev    /* pointer to core driver structure */
    )
    {

    pUsbPrnDev->usbPrnDevSem = semBCreate (SEM_Q_FIFO ,SEM_EMPTY);

    if (pUsbPrnDev->usbPrnDevSem == NULL)
        {
        /* return ERROR */

        return ERROR;
        }

    /*
     * start the device in the only supported mode. The device is being
     * set to interrupt mode
     */
    /* start the device in the only supported mode */

    sioIoctl (pUsbPrnDev->pSioChan, SIO_MODE_SET, (void *) SIO_MODE_INT);

    /* add the device to the I/O system */

    return (iosDevAdd (&pUsbPrnDev->ioDev, name, usbPrnDrvNum));
    }




/*******************************************************************************
*
* usbPrnDevDelete - delete a VxWorks device for an USB printer
*
* This routine deletes a device on a specified serial channel.
*
* This routine is typically called by the USB printer driver, when it detects
* a removal of a USB printer.
*
* RETURNS: OK, or ERROR if the driver is not installed.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbPrnDevDelete
    (
    USB_PRN_DEV * pUsbPrnDev,       /* printer device to read from */
    char        * filename          /* compatible with DRV_REMOVE_PTR */
    )
    {
    int     status = OK;            /* holder for the return value */

    if (usbPrnDrvNum <= 0)
        {
        errnoSet (S_ioLib_NO_DRIVER);
        return (ERROR);
        }

    /* remove the device from the I/O system */

    iosDevDelete (&pUsbPrnDev->ioDev);

    /* delete  wakes up all tasks pended. */

    if (pUsbPrnDev->usbPrnDevSem != NULL)
        {
        if (semDelete (pUsbPrnDev->usbPrnDevSem) == ERROR)
            return ERROR;
        pUsbPrnDev->usbPrnDevSem = NULL;
        }

    return (status);
    }



/*************************************************************************
*
* usbPrnDrvAttachCallback - receives attach callbacks from printer SIO driver
*
* This function is called when the printer is attached or removed
*
* RETURNS: N/A
*
* ERRNO: none
*/

LOCAL void usbPrnDrvAttachCallback
    (
    void *    arg,              /* caller-defined argument */
    SIO_CHAN *pChan,            /* pointer to affected SIO_CHAN */
    UINT16    attachCode        /* defined as USB_KBD_xxxx */
    )
    {
    char    prnName [PRN_NAME_LEN_MAX]; /* printer name */
    UINT32  index = 0;      /* Index into the printer name table */

    /* Search in the table whether any index is available */

    if (pChan == NULL)
        {
        USB1_PRN_ERR("invalid parameter of pChan \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }
    /*  A printer has been attached. */

    if (attachCode == USB_PRN_ATTACH)
        {

        for (index = 0; index < PRN_MAX_DEVS; index++)
            {
            if (usbPrnDeviceTbl[index].usbPrnDevFree)
                break;
            }

        /* If all the indices are occupied, then this printer cannot be used */

        if (index == PRN_MAX_DEVS)
            {
            USB1_PRN_ERR("Printer cannot be accomodated \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }

        /*  Lock this particular channel for this particular printer */

        if (usbPrinterSioChanLock (pChan) != OK)
            {
            USB1_PRN_ERR("usbPrinterSioChanLock() return error \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        else
            {
            /*  plugs the name /usbPr/x into the prnName variable */

            snprintf (prnName,PRN_NAME_LEN_MAX - 1, "%s%d", USB_PRN_NAME, index);

            /*  Create the printer device, do all the ios stuff (drvInstall,
             *  devAdd...)
             */

            usbPrnDeviceTbl[index].pSioChan = pChan;
            if (usbPrnDevCreate (prnName, &usbPrnDeviceTbl[index]) != OK)
                {
                USB1_PRN_ERR("usbPrnDevCreate() return error \n",
                             1, 2, 3, 4, 5, 6);
                (void) usbPrinterSioChanUnlock (pChan);
                return;
                }

            /* Ensure that the table element is made not available */

            usbPrnDeviceTbl[index].usbPrnDevFree = FALSE;


            /*  Install a callback to spew characters to the printer
             *  usbPrnTxCallback () handles sending chunks when the printer
             *  is ready for them
             */

            if ((*pChan->pDrvFuncs->callbackInstall) (pChan,
                                SIO_CALLBACK_GET_TX_CHAR,
                                (STATUS  (*) (void *, ...)) usbPrnTxCallback,
                                &usbPrnDeviceTbl[index])
                                != OK)
                {
                USB1_PRN_ERR("usbPrnTxCallback () failed to install \n",
                             1, 2, 3, 4, 5, 6);
                usbPrnDeviceTbl[index].usbPrnDevFree = TRUE;

                /* delete the device */

                usbPrnDevDelete (&usbPrnDeviceTbl[index], NULL);
                (void) usbPrinterSioChanUnlock (pChan);

                return;
                }
            USB1_PRN_INFO("USB printer attached as %s%d\n",
                          USB_PRN_NAME, index, 3, 4, 5, 6);
            usbPrnDeviceTbl[index].usbPrnDevStable = TRUE;
            }
        }

    /* A printer has been detached */

    else if (attachCode == USB_PRN_REMOVE)
        {

        /* Search for the device element in the table */

        for (index = 0; index < PRN_MAX_DEVS; index++)
            {
            if ((usbPrnDeviceTbl[index].pSioChan != NULL)
                && (usbPrnDeviceTbl[index].pSioChan == pChan))
                break;
            }

        /*
         * If the device is not located, then there is an error
         * Note: Do not unlock the channel as the device removal has an error.
         */

        if (index == PRN_MAX_DEVS)
            {
            USB1_PRN_ERR("Device is not located \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }

        usbPrnDeviceTbl[index].usbPrnDevStable = FALSE;
        /* delete the device */
        USB_PRN_MUTEX_TAKE (WAIT_FOREVER);
        usbPrnDevDelete (&usbPrnDeviceTbl[index], NULL);
        USB_PRN_MUTEX_GIVE;
        /* Unlock this channel since the device that was plugged into it
         * was removed
         */
        if (usbPrinterSioChanUnlock (pChan) != OK)
            {
            USB1_PRN_ERR("usbPrinterSioChanUnlock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }

        USB1_PRN_INFO("USB printer %s%d removed \n",
                      USB_PRN_NAME, index, 3, 4, 5, 6);
        /* Ensure that the table element is made available */

        usbPrnDeviceTbl[index].usbPrnDevFree = TRUE;

        }
    }

/*******************************************************************************
*
* usbPrnOpen - open a usbPrnDrv serial device.
*
* Increments a counter that holds the number of open paths to device.
*
* RETURNS: pointer to USB_PRN_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbPrnOpen
    (
    USB_PRN_DEV * pUsbPrnDev,   /* printer device to read from */
    char        * name,         /* device name */
    int           flags,        /* flags */
    int           mode          /* mode selected */
    )
    {
    USB_PRN_MUTEX_TAKE (WAIT_FOREVER);

    if (pUsbPrnDev->usbPrnDevStable == FALSE)
        {
        USB_PRN_MUTEX_GIVE;
        return (ULONG)ERROR;
        }

    pUsbPrnDev->numOpen++;  /* increment number of open paths */

    sioIoctl (pUsbPrnDev->pSioChan, SIO_OPEN, NULL);
    USB_PRN_MUTEX_GIVE;
    return ((ULONG) pUsbPrnDev);
    }

/*******************************************************************************
*
* usbPrnDrvUnInit - shuts down an I/O USB printer driver
*
* This is supplied to for the user, but it should be noted that iosDrvRemove()
* may cause unpredictable results.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

STATUS usbPrnDrvUnInit (void)
    {
    if (!usbPrnDrvNum)
        return (OK);

    /* unregister */

    if (usbPrinterDynamicAttachUnRegister (usbPrnDrvAttachCallback,
                                           NULL)
                                         != OK)
        {
        USB1_PRN_ERR("usbPrinterDynamicAttachUnRegister() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }
    /* remove the driver */

    if (iosDrvRemove (usbPrnDrvNum, TRUE) != OK)
        {
        USB1_PRN_ERR("iosDrvRemove() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* HELP */
    usbPrnDrvNum = 0;

    /* delete the mutex semaphores */

    if (usbPrnMutex != NULL)
        {
        semTake(usbPrnMutex, WAIT_FOREVER);
        if (semDelete (usbPrnMutex) == ERROR)
            {
            USB1_PRN_ERR("semDelete() returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return (ERROR);
            }
        usbPrnMutex = NULL;
        }

    /* shutdown */

    if (usbPrinterDevShutdown () != OK)
        {
        USB1_PRN_ERR("usbPrinterDevShutdown() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* usbPrnClose - close a usbPrnDrv serial device.
*
* Decrements the counter of open paths to device and alerts the driver
* with an ioctl call when the count reaches zero. This scheme is used to
* implement the HUPCL(hang up on last close).
*
* RETURNS: pointer to USB_PRN_DEV structure
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbPrnClose
    (
    USB_PRN_DEV * pUsbPrnDev          /* printer device to read from */
    )
    {

    /* if there are no open channels */
    USB_PRN_MUTEX_TAKE (WAIT_FOREVER);
    if (pUsbPrnDev->usbPrnDevStable == FALSE)
        {
        USB_PRN_MUTEX_GIVE;
        return (ULONG)ERROR;
        }
    if (pUsbPrnDev->numOpen > 0)
        --pUsbPrnDev->numOpen;

    if (0 == pUsbPrnDev->numOpen)
        {
        sioIoctl (pUsbPrnDev->pSioChan, SIO_HUP, NULL);
        }

    USB_PRN_MUTEX_GIVE;
    return ((ULONG) pUsbPrnDev);
    }

/*******************************************************************************
*
* usbPrnIoctl - issue device control commands
*
* This routine issues device control commands to an USB printer device.
*
* RETURNS: depends on the function invoked.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbPrnIoctl
    (
    USB_PRN_DEV * pUsbPrnDev,   /* printer device to read from */
    int           request,      /* request code */
    void        * arg           /* some argument */
    )
    {
    int ret;
    USB_PRN_MUTEX_TAKE (WAIT_FOREVER);
    if (pUsbPrnDev->usbPrnDevStable == FALSE)
        {
        USB_PRN_MUTEX_GIVE;
        return ERROR;
        }
    ret = sioIoctl (pUsbPrnDev->pSioChan, request, arg);
    USB_PRN_MUTEX_GIVE;
    return ret;
    }

/*******************************************************************************
*
* usbPrnRead - read from the USB printer
*
* This routines is a no-op for a read from the USB printer.
*
* RETURNS: OK, always.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbPrnRead
    (
    USB_PRN_DEV * pUsbPrnDev,            /* printer device to read from */
    char *        buffer,                /* compatible with DRV_READ_PTR */
    size_t        maxBytes
    )
    {
    return OK;
    }


/*******************************************************************************
*
* usbPrnWrite - write to the USB printer
*
* This routine writes <nBytes> bytes of data from the buffer pointed to by
* <buff> to the USB printer described by <pUsbPrnDev>.
*
* RETURNS: number of bytes written or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbPrnWrite
    (
    USB_PRN_DEV * pUsbPrnDev,           /* printer device to read from */
    UCHAR       * buffer,               /* buffer of data to write  */
    size_t        nBytes                /* number of bytes in buffer */
    )
    {
    UINT32          i = 0;                /* counter */
    UINT32          bytesWritten = nBytes;


    /* protect the critical region */

    USB_PRN_MUTEX_TAKE (WAIT_FOREVER);
    if (pUsbPrnDev->usbPrnDevStable == FALSE)
        {
        USB_PRN_MUTEX_GIVE;
        return ERROR;
        }

    /*
     * The printer driver can issue a request for 4096 bytes of data only,
     * so issue a write command comamnd in chunks of 4096 bytes till all
     * data is written
     */

     while (i < bytesWritten)
        {
        /* store buffer information */


        pUsbPrnDev->buff = buffer + i;

        if (nBytes >= PRINT_SIZE)
            pUsbPrnDev->bufSize = PRINT_SIZE;
        else
            pUsbPrnDev->bufSize = nBytes;

        if ((*(pUsbPrnDev->pSioChan->pDrvFuncs->txStartup)) (pUsbPrnDev->pSioChan)
            != OK)
            {
            /* end of the critical region */

            USB_PRN_MUTEX_GIVE;

            return ERROR;
            }

        /*
         * wait for semaphore to be release. This will make write blocking so
         * that the next request is not made before the first request is through
         */

        if (semTake (pUsbPrnDev->usbPrnDevSem, WRITE_TIMEOUT_SEC * sysClkRateGet()) == ERROR)
            {
            /* Cancel the pending transfers */

            sioIoctl (pUsbPrnDev->pSioChan, SIO_USB_PRN_CANCEL, NULL);

            /* end of the critical region */

            USB_PRN_MUTEX_GIVE;

            /* no response, Time out failed, return ERROR */

            return ERROR;
            }

        /* check for the status of the write operation */

        if (pUsbPrnDev->bufResult == ERROR)
            {
            /* end of the critical region */

            USB_PRN_MUTEX_GIVE;

            return ERROR;
            }

        i += PRINT_SIZE;
        nBytes -= PRINT_SIZE;
        }
    /* end of the critical region */

    USB_PRN_MUTEX_GIVE;

    /*
     * return the number of bytes written by the printer device
     */

    return (bytesWritten);
    }


/*************************************************************************
*
* usbPrnTxCallback - feeds characters to USB printer SIO driver
*
* This function feeds characters to USB printer SIO driver to print
*
* RETURNS: OK
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbPrnTxCallback
    (
    void * callbackParam,
    char * txChar,
    STATUS irpResult
    )
    {
    USB_PRN_DEV * pDev = (USB_PRN_DEV *) callbackParam; /* printer device */

    if (irpResult == ERROR)
        {

        pDev->bufResult = ERROR;

        /* give the semaphore pending for write operation */

        semGive (pDev->usbPrnDevSem);
        return ERROR;
        }

    if (!(pDev->bufSize))
        {
        /* This condition is hit when all the data in the buffer is written
         * to the driver buffer.
         * Release the semaphore
         */

       pDev->bufResult = OK;

        /* give the semaphore */

        semGive (pDev->usbPrnDevSem);
        return (ERROR);
        }

    pDev->bufSize--;

    *txChar = *pDev->buff++;

    return OK;
    }


/*****************************************************************************
*
* usrUsbPrnInit - initialize the USB printer driver
*
* This function initializes the USB printer driver and registers for attach
* callbacks.
*
* RETURNS: OK or ERROR if not able to initialize
*
* ERRNO: none
*/

STATUS usrUsbPrnInit (void)
    {

    UINT32  index = 0;  /* Index into the printer identifier table */

    /* Check if driver already installed */

    if (usbPrnDrvNum > 0)
        {
        USB1_PRN_ERR("Printer driver has already initilaized \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Create the mutex semaphores */

    usbPrnMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                              SEM_INVERSION_SAFE);
    if (NULL == usbPrnMutex)
        {
        USB1_PRN_ERR("Create usbPrnMutex failed \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    usbPrnDrvNum = iosDrvInstall ((DRV_CREATE_PTR) NULL,
                                  (DRV_REMOVE_PTR) usbPrnDevDelete,
                                  (DRV_OPEN_PTR) usbPrnOpen,
                                  (DRV_CLOSE_PTR) usbPrnClose,
                                  (DRV_READ_PTR) usbPrnRead,
                                  (DRV_WRITE_PTR) usbPrnWrite,
                                  (DRV_IOCTL_PTR) usbPrnIoctl);


    if (usbPrnDrvNum <= 0)
        {
        USB1_PRN_ERR("iosDrvInstall failed \n",
                     1, 2, 3, 4, 5, 6);
        (void) semDelete(usbPrnMutex);
        usbPrnMutex = NULL;
        errnoSet (S_ioLib_NO_DRIVER);
        return ERROR;
        }

    /* Initialize the printer identifier table */

    for (index = 0; index < PRN_MAX_DEVS; index++)
        {
        usbPrnDeviceTbl[index].usbPrnDevFree = TRUE;
        usbPrnDeviceTbl[index].usbPrnDevStable = FALSE;
        }

    /* Initialize USB printer SIO driver */

    if (usbPrinterDevInit () == OK)
        {
        /* Register our device for attach callbacks.  */

        if (usbPrinterDynamicAttachRegister (usbPrnDrvAttachCallback,
                                             (void *) NULL)
                                                 != OK)
            {
            USB1_PRN_ERR("usbPrinterDynamicAttachRegister() return ERROR \n",
                         1, 2, 3, 4, 5, 6);
            (void) usbPrinterDevShutdown();
            (void) iosDrvRemove(usbPrnDrvNum, TRUE);
            usbPrnDrvNum = 0;
            (void) semDelete(usbPrnMutex);
            usbPrnMutex = NULL;
            return ERROR;
            }
        }
    else
        {
        USB1_PRN_ERR("usbPrinterDevInit() return ERROR \n",
                     1, 2, 3, 4, 5, 6);
        (void) iosDrvRemove(usbPrnDrvNum, TRUE);
        usbPrnDrvNum = 0;
        (void) semDelete(usbPrnMutex);
        usbPrnMutex = NULL;
        return ERROR;
        }

    USB1_PRN_INFO("USB Printer Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return OK;
    }


/*************************************************************************
*
* usbPrnWrTest - test the USB printer write function
*
* This is given as a sample application to print a file using the i/o
* system calls to access the printer device.  It has been assumed that the
* file that it sent to the printer has been "canned" for the particular
* printer being used.  Refer to the USB Developer's Kit manual for further
* details.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: none
*/

STATUS usbPrnWrTest
    (
    char    * deviceName,       /* Printer device name */
    char    * fileName          /* file to print */
    )
    {
    int     testFd;
    UCHAR   * tempBuff = NULL;
    FILE    * testFile;
    long    fTellSize = 0;
    UINT32  fsize;

    if ((testFile = fopen (fileName, "rb")) == NULL)
        {
        printf ("fopen() failed open the input file\n");
        return (ERROR);
        }

    fseek (testFile, 0, SEEK_END);
    fTellSize = ftell (testFile);
    fseek (testFile, 0, SEEK_SET);

    fsize = (fTellSize > 0) ? fTellSize : 0;

    printf ("usbPrnTxCallback() file size =0x%x \n", fsize);


    if ((tempBuff = calloc (1, fsize)) == NULL)
        {
        fclose (testFile);
        return (ERROR);
        }

    if (fread (tempBuff, 1, fsize, testFile) == 0)
        {
        free(tempBuff);
        fclose (testFile);
        return (ERROR);
        }

    if ((testFd = open (deviceName, O_RDWR, 0)) == ERROR)
        {
        free(tempBuff);
        fclose (testFile);
        return (ERROR);
        }

    if (write (testFd, (char *) tempBuff, fsize) == ERROR)
        {
        close (testFd);
        free(tempBuff);
        fclose(testFile);
        return (ERROR);
        }
    close (testFd);
    fclose (testFile);
    free (tempBuff);

    return (OK);
    }

#endif /* __INCusrUsbPrnInitc */
