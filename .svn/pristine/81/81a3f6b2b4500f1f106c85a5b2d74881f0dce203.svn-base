/* usrUsbKbdInit.c - Initialization of the USB Keyboard driver */

/*
 * Copyright (c) 1999-2007, 2009-2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
02j,06may13,s_z  Remove compiler warning (WIND00356717)
02i,12aug11,ghs  Always release event when keyboard buffer not empty
                 (WIND00290770)
02h,08jul11,ghs  Give a delay when shell ready for attached (WIND00286684)
02g,07jul11,ghs  Fix conflict variable define (WIND00286557)
02f,16mar11,j_x  Duplicate include tyLib.h (WIND00256473)
02e,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788)
02d,07jan11,ghs  Clean up compile warnings (WIND00247082)
02c,15oct10,ghs  Support attach to shell before shell initialization
                 (WIND00236632)
02b,02sep10,ghs  Use OS_THREAD_FAILURE to check taskSpawn failure (WIND00229830)
02a,28jul10,ghs  Fix device name display error(WIND00223154)
01z,05jul10,m_y  Add valid check after allocate resource (WIND00183499)
01y,05jul10,ghs  Modify display logs(WIND00216586)
01x,27apr10,j_x  Error debug adaption
01w,18mar10,j_x  Changed for USB debug (WIND00184542)
01v,03mar10,ghs  Fix LP64 adapting(WIND00202078)
01u,13jan10,ghs  vxWorks 6.9 LP64 adapting
01t,03sep09,y_l  Add support for bootAppShell in bootrom (WIND00130193)
01s,02aug07,adh  WIND00034090 fix, usb Keyboard in target shell
01r,07aug06,jrp  Fixed multiple keyboard conflict
01q,19jan06,ami  Warning related Fixes (SPR #116600)
01p,09jan06,ami  WindML requirements incorporated (SPR #116401 Fix)
01o,22dec05,ami  Fix for SPR #112887
01n,26aug05,ami  Select Feature Support Added
01m,10aug05,ami  Un-initialization routien modified and return type added for
                 initialization routine
01l,09apr05,ami  SEL_WAKEUP_LIST member and selWakeupListInit removed. (Fix
                 for exception for multiple connects/disconnects)
01k,15oct04,ami  Apigen Changes
01j,05oct04,pdg  Fix for SPR #94286
01i,03jun04,hch  Fix warning for usbKbdRead and playWithKeyboard
01h,05nov03,cfc  USB2 Development Integration
01g,29oct03,cfc  USB2 merge
01f,23sep03,cfc  Fixed issue of overflowing driver table
01e,06sep03,cfc  Decrement keyboard ctr when detached
01d,30jan02,wef  fixed DevFind routine
01c,08dec01,wef  fixed some warnings
01b,02feb01,wef  Added ios functionality - allows for open, close, read,
                 ioctl, etc access to the printer
01a,23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes the USB keyboard driver.  This assumes the
USB host stack has already been initialized and has a host controller
driver attached.

This configlette demonstrates how a user might integrate a USB class
driver into the vxWorks file system.  usbKbdDevCreate () intalls a USB
keyboard and its associated driver functions into the driver table allowing
the keyboard to be accesed with standard fopen, close, read, write, etc. type
calls.

There is an included test routine playWithKeyboard () that demonstrates
using the ios functionality.

This configlette also contains 2 macros that can be used to attach the target
shell (in normal vxWorks) or bootAppShell (in bootrom) to the usb keyboards.
To attach the target shell (in normal vxWorks) to the usb keyboard, define
the macro INCLUDE_USB_KEYBOARD_SHELL_ATTACH; To attach the bootAppShell (in
bootrom) to the usb keyboard, define the macro INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH.
These 2 macros can not be defined simultaneously.

INCLUDE FILES: iosLib.h, vxWorks.h, iv.h, ioLib.h, tyLib.h, intLib.h,
errnoLib.h, sioLib.h, stdlib.h, stdio.h, logLib.h,
selectLib.h, drv/usb/usbKeyboardLib.h, usb/usbHid.h

*/

#ifndef __INCusrUsbKbdInitc
#define __INCusrUsbKbdInitc

/* includes */

#include <drv/usb/usbKeyboardLib.h>
#include <usb/usbHid.h>
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

#define USB_KBD_MUTEX_TAKE(tmout)                               \
    semTake (usbKbdMutex, (_Vx_ticks_t) (tmout))

#define USB_KBD_MUTEX_GIVE                                      \
    semGive (usbKbdMutex)

#define USB_KBD_LIST_SEM_TAKE(tmout)                            \
    semTake (usbKbdListMutex, (_Vx_ticks_t) (tmout))

#define USB_KBD_LIST_SEM_GIVE                                   \
    semGive (usbKbdListMutex)


#define KBD_NAME_LEN_MAX        100
#define USB_KBD_NAME        "/usbKb/"


#define CTRL_Z                26    /* ASCI code for ^Z */

#define QUEUE_SIZE 8           /* size of circular queue */
/*
 * If you are going to have more than 10 keyboards, increase this number
 */
#ifndef USB_MAX_KEYBOARDS
#define USB_MAX_KEYBOARDS 5
#endif
#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
#define NUM_SHELL_ATTACH_ATTEMPTS 10  /* Number of attempts if shell not here */
#endif /* INCLUDE_USB_KEYBOARD_SHELL_ATTACH */



/*
 * usb keyboard attach to vxworks shell or bootAppShell macro check.
 *
 * INCLUDE_USB_KEYBOARD_SHELL_ATTACH and INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
 * are mutually exclusive
 */

#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH

#ifndef INCLUDE_PC_CONSOLE
#  error INCLUDE_PC_CONSOLE must be defined in USB bootAppShell keyboard\
         attachmode
#endif /* INCLUDE_PC_CONSOLE */

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
#  error INCLUDE_USB_KEYBOARD_SHELL_ATTACH must not be defined in USB\
         bootAppShell attach mode
#endif /* INCLUDE_USB_KEYBOARD_SHELL_ATTACH */

#endif /* INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH */

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH

#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
#   error INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH must not be defined in USB\
          target shell attach mode
#endif /* INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH */

#endif /* INCLUDE_USB_KEYBOARD_SHELL_ATTACH */

/* data types */

/* typedefs */

typedef struct usb_kbd_node
    {
    NODE        node;
    struct usb_kbd_dev    * pUsbKbdDev;
    } USB_KBD_NODE;

typedef struct usb_kbd_dev /* USB_KBD_DEV */
    {
    DEV_HDR                 ioDev;
    SIO_CHAN *              pSioChan;
    UINT16                  numOpen;
    UINT32                  bufSize;
    UCHAR *                 buff;
    USB_KBD_NODE *          pUsbKbdNode;
    SEL_WAKEUP_LIST         selWakeupList;   /*list of tasks pended on select*/
    SEM_ID                  kbdSelectSem;    /* semaphore for select handling */
#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
    TY_DEV *                pTyDev;          /* tty device for usb keyboard */
#endif
    } USB_KBD_DEV, *pUSB_KBD_DEV;

typedef struct kbdQueue /* KBD_QUEUE */
    {
    char            queueData [QUEUE_SIZE];
    INT8            queueFront;
    INT8            queueRear;
    SEM_ID          queueSem;
    } KBD_QUEUE, *pKBD_QUEUE;


/* local variables */
LOCAL SIO_CHAN *pKbdDevArray[USB_MAX_KEYBOARDS];
LOCAL SEM_ID    usbKbdMutex;        /* mutex semaphore */
LOCAL SEM_ID    usbKbdListMutex;    /* mutex semaphore to protect list */
LOCAL LIST      usbKbdList;         /* all USB keyboards in the system */

LOCAL int usbKbdDrvNum = 0;        /* driver number */

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
LOCAL int       usbOrigFd;           /* Original shell fd */
LOCAL int       usbShellFd;          /* fd of usb shell keyboard */
LOCAL char *    usbShellKeyboard=NULL;  /* Name of USB shell kbd */
#else
#ifndef INCLUDE_USBTOOL
LOCAL BOOL enterPressed;
#endif
#endif
LOCAL KBD_QUEUE    kbdQueueData;    /* holds data sent by keyboard */

#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
/* import pc console function for usb keyboard  */
IMPORT TY_DEV * pcConDevBind (int, FUNCPTR, void *);
#endif

/* forward declarations */

LOCAL void  usbKbdDrvAttachCallback (void * arg, SIO_CHAN *pChan,
                    UINT16 attachCode);

LOCAL ssize_t   usbKbdRead (USB_KBD_DEV * pUsbKbdDev, char * buffer,
                    size_t nBytes);

LOCAL ULONG usbKbdClose (USB_KBD_DEV * pUsbKbdDev);

LOCAL ssize_t   usbKbdWrite (USB_KBD_DEV * pUsbKbdDev, char * buffer,
                    size_t nBytes);

LOCAL ULONG usbKbdOpen (USB_KBD_DEV * pUsbKbdDev, char * name,
                    int flags, int mode);

LOCAL int   usbKbdIoctl (USB_KBD_DEV * pUsbKbdDev, int request, void * arg);

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
LOCAL SIO_CHAN *findUsbKeyboard (void);
#endif

LOCAL STATUS    usbKbdDevFind (SIO_CHAN *pChan, USB_KBD_DEV ** ppUsbKbdDev);

LOCAL STATUS    usbKbdDevDelete (USB_KBD_DEV * pUsbKbdDev, char *fileName);

LOCAL STATUS    usbKbdRcvCallback (void * putRxCharArg, char nextChar);

LOCAL VOID    putDataInQueue (char data);
LOCAL char    getDataFromQueue (void);
LOCAL BOOL    isDataQueueEmpty (void);
LOCAL UINT16  queueBytesCount (void);


#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
LOCAL STATUS     usbKbdDevFind (SIO_CHAN *pChan, USB_KBD_DEV ** ppUsbKbdDev);

/*******************************************************************************
*
* redirectShellInput - Redirects the target shell input to the usb keyboard
*
* This routine redirect the input of the target shell to a newly connected
* usb keyboard. This routine run as a task and wait shell start up.
*
* The define of NUM_SHELL_ATTACH_ATTEMPTS can change the retry times of
* attaching to shell.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
LOCAL void redirectShellInput
    (
    USB_KBD_DEV * pDev
    )
    {

    int counter = 0;

    do {
        USB1_KBD_DBG ("usbKbdShellAttachTask(): Attempt %d\n",
                      counter, 2, 3, 4, 5, 6);

        if (NULL != shellFirst ())
            {

            /*
             * When first shell session is not NULL, shell may be still starting,
             * but not ready to use. So we should wait a moment make shell ready
             * to be attached.
             */

            taskDelay(sysClkRateGet());

            /*
             * Since we have only one instance of the shell, if the keyboard has
             * already been attached to shell, we skip it.  We know a keyboard
             * is attached to the shell is usbShellKeyboard has a name in it
             */
            if (usbShellKeyboard == NULL)
                {
                USB1_KBD_INFO("Opening %s for console device \n",
                             pDev->ioDev.name, 2, 3, 4, 5, 6);
                usbShellFd = open(pDev->ioDev.name, O_RDONLY, 0);
                if (usbShellFd <= 0)
                    {
                    USB1_KBD_ERR("Could not open fd for shell redirection \n",
                                 1, 2, 3, 4, 5, 6);
                    return;
                    }

                usbOrigFd = ioGlobalStdGet (0);

                shellTerminate(shellFirst());
                if (OK != shellGenericInit(SHELL_FIRST_CONFIG, SHELL_STACK_SIZE,
                        NULL, NULL, TRUE, SHELL_SECURE, usbShellFd, usbOrigFd,
                        usbOrigFd))
                    {
                    USB1_KBD_ERR("Could not start a new shell \n",
                                 1, 2, 3, 4, 5, 6);
                    return;
                    }
                else
                    {
                    usbShellKeyboard = (char *)malloc(strlen(pDev->ioDev.name)+1);
                    if (usbShellKeyboard == NULL)
                        {
                        USB1_KBD_ERR("memory allocate failed for shell KBD \n",
                                     1, 2, 3, 4, 5, 6);
                        return;
                        }
                    else
                        {
                        strcpy(usbShellKeyboard, pDev->ioDev.name);
                        }
                    }
                return;
                }

            return;
            }

        taskDelay(sysClkRateGet());
        }
    while ((counter++) < NUM_SHELL_ATTACH_ATTEMPTS);

    }
/*******************************************************************************
*
* restoreShellInput - Restores the target shell input to original device
*
* This routine checks to see whether or not the device being removed is
* attached to the shell.  If so, it restores the shells input device to
* the original default.
*
* RETURNS: TRUE.  A FALSE is returned in case of internal error
*
* \NOMANUAL
*
* ERRNO: none
*/
LOCAL BOOL restoreShellInput
    (
    USB_KBD_DEV * pDev
    )
    {

    if (usbShellKeyboard != NULL)
        {
        if (strcmp(pDev->ioDev.name, usbShellKeyboard) == 0)
            {
            /*
             * We are closing the console keyboard
             */
            shellTerminate(shellFirst());

            if (OK != shellGenericInit(SHELL_FIRST_CONFIG, SHELL_STACK_SIZE, NULL,
                        NULL, TRUE, SHELL_SECURE, usbOrigFd, usbOrigFd, usbOrigFd))
                {
                USB1_KBD_ERR("Could not start a new shell \n",
                             1, 2, 3, 4, 5, 6);
                return FALSE;
                }

            if (usbShellKeyboard != NULL)
                {
                free (usbShellKeyboard);
                usbShellKeyboard = NULL;
                }

            }
        }
    return TRUE;
    }
#endif


/*******************************************************************************
*
* usbKbdDevCreate - create a VxWorks device for an USB keyboard
*
* This routine creates a device on a specified serial channel.  Each channel
* to be used should have exactly one device associated with it by calling
* this routine.
*
* For instance, to create the device "/ /0", the proper call would be:
* \cs
*     usbKbdDevCreate ("/usbKb/0", pSioChan);
* \ce
*
* Where pSioChan is the address of the underlying SIO_CHAN serial channel
* descriptor (defined in sioLib.h).
* This routine is typically called by the USB keyboard driver, when it detects
* an insertion of a USB keyboard.
*
* RETURNS: OK, or ERROR if the driver is not installed, or the
* device already exists, or failed to allocate memory.
*
* ERRNO: none
*/

STATUS usbKbdDevCreate
    (
    char        * name,         /* name to use for this device      */
    SIO_CHAN    * pSioChan      /* pointer to core driver structure */
    )
    {
    USB_KBD_NODE    * pUsbKbdNode = NULL;   /* pointer to device node */
    USB_KBD_DEV     * pUsbKbdDev = NULL;     /* pointer to USB device */
    STATUS          status = ERROR;


    if (pSioChan == (SIO_CHAN *) ERROR)
        {
        USB1_KBD_ERR("pSioChan is ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* allocate memory for the device */

    if ((pUsbKbdDev = (USB_KBD_DEV *) calloc (1, sizeof (USB_KBD_DEV))) == NULL)
        {
        USB1_KBD_ERR("calloc returned NULL - out of memory \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    pUsbKbdDev->pSioChan = pSioChan;

    /* allocate memory for this node, and populate it */

    pUsbKbdNode = (USB_KBD_NODE *) calloc (1, sizeof (USB_KBD_NODE));

    if (pUsbKbdNode == NULL)
        {
        USB1_KBD_ERR("calloc returned NULL - out of memory \n",
                     1, 2, 3, 4, 5, 6);
        pUsbKbdDev->pSioChan = NULL;
        free(pUsbKbdDev);
        return (ERROR);
        }

    /* record useful information */

    pUsbKbdNode->pUsbKbdDev = pUsbKbdDev;
    pUsbKbdDev->pUsbKbdNode = pUsbKbdNode;

    /* bind keyboard with pc console in bootrom */
#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
    pUsbKbdDev->pTyDev = pcConDevBind (PC_CONSOLE, NULL, NULL);
#endif

    /* add the device to the I/O system */

    if ((status = iosDevAdd (&pUsbKbdDev->ioDev, name, usbKbdDrvNum)) != OK)
        {
        pUsbKbdNode->pUsbKbdDev = NULL;
        free(pUsbKbdNode);

        pUsbKbdDev->pUsbKbdNode = NULL;
        pUsbKbdDev->pSioChan = NULL;
        free(pUsbKbdDev);

        (void) usbKeyboardSioChanLock (pSioChan);
        USB1_KBD_ERR("Unable to create keyboard device \n",
                     1, 2, 3, 4, 5, 6);
        return (status);
        }

    /* Initialize the list holding the list of tasks pended on the driver. */

    selWakeupListInit(&pUsbKbdDev->selWakeupList);

    /* add the node to the list */

    USB_KBD_LIST_SEM_TAKE (WAIT_FOREVER);

    lstAdd (&usbKbdList, (NODE *) pUsbKbdNode);

    USB_KBD_LIST_SEM_GIVE;

    /* start the device in the only supported mode */

    sioIoctl (pUsbKbdDev->pSioChan, SIO_MODE_SET, (void *) SIO_MODE_INT);

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
    if (usbShellKeyboard == NULL)
        {
        taskSpawn("tShellAttach",
                   100,
                   0,
                   4096,
                   (FUNCPTR)redirectShellInput,
                   (ULONG)pUsbKbdDev,
                   0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
#endif
    return (status);
    }


/*******************************************************************************
*
* usbKbdDevDelete - delete a VxWorks device for an USB keyboard
*
* This routine deletes a device on a specified serial channel.
*
* This routine is typically called by the USB keyboard driver, when it detects
* a removal of a USB keyboard.
*
* RETURNS: OK, or ERROR if the driver is not installed.
*
* ERRNO: none
*
* \NOMANUAL
*/


LOCAL STATUS usbKbdDevDelete
    (
    USB_KBD_DEV *    pUsbKbdDev,        /* keyboard device to read from */
    char *           fileName           /* compatible with DRV_REMOVE_PTR */
    )
    {
    int        status = OK;        /* holder for the return value */

    if (usbKbdDrvNum <= 0)
        {
        errnoSet (S_ioLib_NO_DRIVER);
        return (ERROR);
        }

    /*
     * Terminate and free the select wake-up list. Call selWakeUpAll
     * in case there is data.
     * This should also unblock all tasks waiting on the driver.
     */

    selWakeupAll (&pUsbKbdDev->selWakeupList, SELREAD);

    /* create a semaphore */

    if ((pUsbKbdDev->kbdSelectSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY)) ==
         NULL)
        return ERROR;

    if (selWakeupListLen (&pUsbKbdDev->selWakeupList) > 0)
        {
        /*
         * There are task pending on the list wait of signal to
         * unpend the task before terminating the list
         */

        /* wait on this semphore to be signaled */

        semTake (pUsbKbdDev->kbdSelectSem, WAIT_FOREVER);
        }

    /* delete the semaphore */

    (void) semDelete (pUsbKbdDev->kbdSelectSem);
    pUsbKbdDev->kbdSelectSem = NULL;

    selWakeupListTerm(&pUsbKbdDev->selWakeupList);
#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
    /*
     * If the shell is attached to this keyboard, restore the shell to the
     * default device
     */
    if (strcmp(pUsbKbdDev->ioDev.name,usbShellKeyboard) == 0)
        {
        /*
         * If the shell is attached to this keyboard, restore the shell to the
         * default device
         */
        if (restoreShellInput(pUsbKbdDev) != TRUE)
            {
            USB1_KBD_ERR("Unable to restore shell to default device \n",
                         1, 2, 3, 4, 5, 6);
            }
        }
#endif
    /* remove the device from the I/O system */

    iosDevDelete (&pUsbKbdDev->ioDev);

    /* remove from list */

    USB_KBD_LIST_SEM_TAKE (WAIT_FOREVER);
    lstDelete (&usbKbdList, (NODE *) pUsbKbdDev->pUsbKbdNode);
    USB_KBD_LIST_SEM_GIVE;

    /* free memory for the device */
    free (pUsbKbdDev->pUsbKbdNode);
    pUsbKbdDev->pUsbKbdNode = NULL;
    free (pUsbKbdDev);
    return (status);
    }



/*************************************************************************
*
* usbKbdDevFind - find the USB_KBD_DEV device for an SIO channel
*
* This function finds the USB_KBD_DEV device for the specified SIO Channel
*
* RETURNS: OK, or ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbKbdDevFind
    (
    SIO_CHAN *        pChan,          /* pointer to affected SIO_CHAN */
    USB_KBD_DEV **    ppUsbKbdDev    /* pointer to an USB_KBD_DEV */
    )
    {
    USB_KBD_NODE *      pUsbKbdNode = NULL;     /* pointer to USB device node */
    USB_KBD_DEV *    pTempDev;        /* pointer to an USB_KBD_DEV */

    if (pChan == NULL)
        return (ERROR);

    /* protect it from other module's routines */

    USB_KBD_LIST_SEM_TAKE (WAIT_FOREVER);

    /* loop through all the devices */

    for (pUsbKbdNode = (USB_KBD_NODE *) lstFirst (&usbKbdList);
         pUsbKbdNode != NULL;
         pUsbKbdNode = (USB_KBD_NODE *) lstNext ((NODE *) pUsbKbdNode))
        {
        pTempDev = pUsbKbdNode->pUsbKbdDev;

        /* return as soon as you find it */
        if (pTempDev->pSioChan == pChan)
            {
            *ppUsbKbdDev = (USB_KBD_DEV *) pTempDev;
            USB_KBD_LIST_SEM_GIVE;
            return (OK);
            }
        }

    USB_KBD_LIST_SEM_GIVE;

    return (ERROR);
    }

/*************************************************************************
*
* getKeyboardNum - Gets the next unit number for a keyboard
*
* This routine supports multiple keyboards being connected and
* disconnected.  Previous versions of the software used a simple counter
* and assigned unit numbers based on the counter.  That scheme falls
* apart if the keyboards are connected/disconnected in random order.
*
* This routine simply returns the next available keyboard number
*
* RETURNS: Next available keyboard number.  -1 if no keyboards available.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int getKeyboardNum(SIO_CHAN *pChan)
{
    int index;
    for (index = 0; index < USB_MAX_KEYBOARDS; index++)
        {
        if (pKbdDevArray[index] == 0)
            {
            pKbdDevArray[index] = pChan;
            return (index);
            }
        }
    USB1_KBD_ERR("Out of available keyboards. Check USB_MAX_KEYBOARDS \n",
                 1, 2, 3, 4, 5, 6);
    return(-1);
}

/*************************************************************************
*
* freeKeyboardNum - Frees up the next unit number for a keyboard
*
* This routine supports multiple keyboards being connected and
* disconnected.  Previous versions of the software used a simple counter
* and assigned unit numbers based on the counter.  That scheme falls
* apart if the keyboards are connected/disconnected in random order.
*
* This routine frees up the previously allocated keyboard number
*
* RETURNS: Freed keyboard number.  -1 if not found.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int freeKeyboardNum(SIO_CHAN *pChan)
{
    int index;
    for (index = 0; index < USB_MAX_KEYBOARDS; index++)
        {
        if (pKbdDevArray[index] == pChan)
            {
            pKbdDevArray[index] = NULL;
            return (index);
            }
        }
    USB1_KBD_ERR("Unable to locate keyboard pointed to by channel 0x%X \n",
                 pChan, 2, 3, 4, 5, 6);
    return(-1);
}

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
/*************************************************************************
*
* findUsbKeyboard - Finds an available usb keyboard
*
* This routine searchs for a connected usb keyboard and returns a pointer
* to the SIO_CHAN of that keyboard.
* apart if the keyboards are connected/disconnected in random order.
*
*
* RETURNS: pointer to a SIO_CHAN of the keyboard  NULL if none are  found.
*
* ERRNO: none
*
* \NOMANUAL
*/
LOCAL SIO_CHAN *findUsbKeyboard (void)
    {
    int index;
    for (index = 0; index < USB_MAX_KEYBOARDS; index++)
        {
        if (pKbdDevArray[index] != NULL)
            {
            return (pKbdDevArray[index]);
            }
        }
    return (NULL);
    }
#endif

/*************************************************************************
*
* usbKbdDrvAttachCallback - receives attach callbacks from keyboard SIO driver
*
* This is the callback function which is called whenever the keyboard device
* is connected
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void usbKbdDrvAttachCallback
    (
    void *     arg,          /* caller-defined argument */
    SIO_CHAN * pChan,        /* pointer to affected SIO_CHAN */
    UINT16     attachCode    /* defined as USB_KBD_xxxx */
    )
    {
    int     kbdUnitNum;
    USB_KBD_DEV * pUsbKbdDev=NULL;        /* keyboard device */
    char    kbdName [KBD_NAME_LEN_MAX];    /* keyboard name */

    /*  A keyboard has been attached. */

    if (attachCode == USB_KBD_ATTACH)
        {

        /*  Lock this particular channel for this particular keyboard */

        if (usbKeyboardSioChanLock (pChan) != OK)
            {
            USB1_KBD_ERR("usbKeyboardSioChanLock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }
        else
            {
            /*  plugs the name /usbKb/x into the kbdName variable */
            kbdUnitNum = getKeyboardNum(pChan);
            if (kbdUnitNum >= 0)
                {

                snprintf (kbdName, KBD_NAME_LEN_MAX -1, "%s%d", 
                          USB_KBD_NAME, kbdUnitNum);

                /*
                 *  Create the keyboard device, do all the ios stuff such as
                 *  drvInstall, devAdd...
                 */
                if (usbKbdDevCreate (kbdName, pChan) != OK)
                    {
                    USB1_KBD_ERR("usbKbdDevCreate () returned ERROR \n",
                                 1, 2, 3, 4, 5, 6);
                    return;
                    }

                }
            else
                {
                USB1_KBD_ERR("Excessive keyboards - " \
                             "check USB_MAX_KEYBOARDS \n",
                             1, 2, 3, 4, 5, 6);
                return;
                }

            if (usbKbdDevFind (pChan, &pUsbKbdDev) != OK)
                {
                USB1_KBD_ERR("usbKbdDevFind () returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                return;
                }

            if ((*pChan->pDrvFuncs->callbackInstall)
                    (pChan,
                    SIO_CALLBACK_PUT_RCV_CHAR,
                    (STATUS  (*) (void *, ...)) usbKbdRcvCallback,
                    (void *) pUsbKbdDev)
                    != OK)
                {
                USB1_KBD_ERR("usbKbdRcvCallback () failed to install \n",
                             1, 2, 3, 4, 5, 6);
                return;
                }

            USB1_KBD_INFO("USB keyboard attached as %s%d \n",
                          USB_KBD_NAME, kbdUnitNum, 3, 4, 5, 6);
            }
        }
    else if (attachCode == USB_KBD_REMOVE)
        {
        /* find the related device */

        if (usbKbdDevFind (pChan, &pUsbKbdDev) != OK)
            {
            USB1_KBD_ERR("usbKbdDevFind could not find channel %p \n",
                         pChan, 2, 3, 4, 5, 6);
            return;
            }

        /* delete the device */

        usbKbdDevDelete (pUsbKbdDev, NULL);
        pUsbKbdDev = NULL;

        kbdUnitNum = freeKeyboardNum(pChan);

        if (usbKeyboardSioChanUnlock (pChan) != OK)
            {
            USB1_KBD_ERR("usbKeyboardSioChanUnlock () returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return;
            }

        USB1_KBD_INFO("USB keyboard %s%d removed \n",
                      USB_KBD_NAME, kbdUnitNum, 3, 4, 5, 6);

#ifdef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
        /*
         * The possiblity exists that there are other USB keyboards attached.
         * if so and we have removed the shell kbd, we want to redirect the
         * shell to one of those keyboards
         */
            {
            SIO_CHAN   *pSioChan;

            pSioChan = findUsbKeyboard();
            if (pSioChan != NULL)
                {
                /*
                 * There is still a connected keyboard.  Attach the console to
                 * that keyboard
                 */
                usbKbdDevFind (pSioChan, &pUsbKbdDev);
                taskSpawn("tShellAttach",
                           100,
                           0,
                           4096,
                           (FUNCPTR)redirectShellInput,
                           (ULONG)pUsbKbdDev,
                           0, 0, 0, 0, 0, 0, 0, 0, 0);
                }
            }
#endif

        }

    }

/*******************************************************************************
*
* usbKbdOpen - open a usbKbdDrv serial device.
*
* Increments a counter that holds the number of open paths to device.
*
* RETURNS: the pointer to USB_KBD_DEV
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbKbdOpen
    (
    USB_KBD_DEV    * pUsbKbdDev,       /* keyboard device to read from */
    char     *       name,             /* device name */
    int              flags,            /* flags */
    int              mode              /* mode selected */
    )
    {

    pUsbKbdDev->numOpen++;  /* increment number of open paths */

    sioIoctl (pUsbKbdDev->pSioChan, SIO_OPEN, NULL);

    /* flush the data from the circular buffer */

    return (ULONG)pUsbKbdDev;
    }

/*******************************************************************************
*
* usbKbdDrvUnInit - shuts down an I/O USB keyboard driver
*
* This is supplied to for the user, but it should be noted that iosDrvRemove()
* may cause unpredictable results.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

STATUS usbKbdDrvUnInit (void)
    {

    if (!usbKbdDrvNum)
        return (OK);

    /* unregister */

    if (usbKeyboardDynamicAttachUnRegister (usbKbdDrvAttachCallback,
                     NULL)
                    != OK)
        {
        USB1_KBD_ERR("usbKeyboardDynamicAttachUnRegister () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    /* remove the driver */

    if (iosDrvRemove (usbKbdDrvNum, TRUE) != OK)
        {
        USB1_KBD_ERR("iosDrvRemove () returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    usbKbdDrvNum = 0;

    /* delete the mutex semaphores */

    if (usbKbdMutex != NULL)
        {
        semTake(usbKbdMutex, WAIT_FOREVER);
        if (semDelete (usbKbdMutex) == ERROR)
            {
            USB1_KBD_ERR("semDelete (usbKbdMutex) returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return (ERROR);
            }
        usbKbdMutex = NULL;
        }

    if (usbKbdListMutex != NULL)
        {
        semTake(usbKbdListMutex, WAIT_FOREVER);
        if (semDelete (usbKbdListMutex) == ERROR)
            {
            USB1_KBD_ERR("semDelete (usbKbdListMutex) returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            return (ERROR);
            }
        usbKbdListMutex = NULL;
        }

    /* shutdown */

    if (usbKeyboardDevShutdown () != OK)
        {
        USB1_KBD_ERR("usbKeyboardDynamicAttachUnRegister() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* usbKbdClose - close a usbKbdDrv serial device.
*
* Decrements the counter of open paths to device and alerts the driver
* with an ioctl call when the count reaches zero. This scheme is used to
* implement the HUPCL(hang up on last close).
*
* RETURNS: the pointer to USB_KBD_DEV
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ULONG usbKbdClose
    (
    USB_KBD_DEV    * pUsbKbdDev          /* keyboard device to read from */
    )
    {

    /* if there are no open channels */

    if (pUsbKbdDev->numOpen > 0)
        --pUsbKbdDev->numOpen;

    if (pUsbKbdDev->numOpen == 0)
        {
        sioIoctl (pUsbKbdDev->pSioChan, SIO_HUP, NULL);
        }

    return (ULONG)pUsbKbdDev;
    }

/*******************************************************************************
*
* usbKbdIoctl - issue device control commands
*
* This routine issues device control commands to an USB keyboard device.
*
* RETURNS: depends on the function invoked.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbKbdIoctl
    (
    USB_KBD_DEV   * pUsbKbdDev,     /* keyboard device to read from */
    int             request,        /* request code */
    void          * arg             /* some argument */
    )
    {

    int errNo = OK ;

    switch (request)
        {

        case FIOSELECT:

        /* add the node to the <selWakeupList> list */

        errNo = selNodeAdd (&pUsbKbdDev->selWakeupList, (SEL_WAKEUP_NODE *) arg);

        if (errNo == OK)
            {

            /*
             * If data is available wake up the task that's waiting
             * for it.
             */

            USB_KBD_MUTEX_TAKE(WAIT_FOREVER);

            if ((!isDataQueueEmpty()) &&
                    (selWakeupType ((SEL_WAKEUP_NODE *) arg) == SELREAD ))

                {

                /* data available, make sure task does not pend */

                selWakeup ((SEL_WAKEUP_NODE *) arg);
                }

            USB_KBD_MUTEX_GIVE;

            }
        break;

        case FIOUNSELECT:

        /* delete the node */

        errNo = selNodeDelete (&pUsbKbdDev->selWakeupList,
                           (SEL_WAKEUP_NODE *) arg);

        /*
         * check if the number of  task pending is 0 and
         * the semaphore  <kbdSelectSem> is created if so the
         * device is disconnected. Signal the
         * semaphore and return ERROR
         */

        if ((selWakeupListLen (&pUsbKbdDev->selWakeupList) == 0)
                 && (pUsbKbdDev->kbdSelectSem != NULL))
            {
            semGive (pUsbKbdDev->kbdSelectSem);
            errNo = ERROR;
            }

        break;

        case FIONREAD:
        /* returns the number of bytes available for read in kdbQueue */

        USB_KBD_MUTEX_TAKE(WAIT_FOREVER);
        errNo = queueBytesCount();
        USB_KBD_MUTEX_GIVE;

        break;

        default:
        /* call the appropiate ioctl function */

        errNo = (sioIoctl (pUsbKbdDev->pSioChan, request, arg));
        }

    return errNo;
    }

/*******************************************************************************
*
* usbKbdRead - read from the USB keyboard
*
* This routines is a no-op for a read from the USB keyboard.
*
* RETURNS: OK, always.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbKbdRead
    (
    USB_KBD_DEV   * pUsbKbdDev,             /* keyboard device to read from */
    char *          buffer,                 /* buffer to read data */
    size_t          nBytes                  /* size of data to read */
    )
    {
    int         arg = 0;
    UINT32      bytesAvailable = 0;     /* store the total bytes in queue */
    UINT32      bytesToBeRead  = 0;     /* number of bytes to be read */
    UINT32      i = 0;                  /* counter value */

    /* if the nBytes is not valid return ERROR */

    if ((nBytes == 0) || (buffer == NULL))
        return ERROR;

    /* set the buffer to 0 */
    memset (buffer, 0, nBytes);

    sioIoctl (pUsbKbdDev->pSioChan, SIO_MODE_GET, &arg);

    if (arg == SIO_MODE_POLL)
        {
        return  pUsbKbdDev->pSioChan->pDrvFuncs->pollInput (pUsbKbdDev->pSioChan,
                                                            buffer);
        }

    else if (arg == SIO_MODE_INT)
        {

        USB_KBD_MUTEX_TAKE(WAIT_FOREVER);

        /* determine the number of bytes available in the queue */

        if ((bytesAvailable = (UINT32)queueBytesCount()) == 0)
            {
            /* if hte number if bytes is 0, return ERROR */


            USB_KBD_MUTEX_GIVE;
            *buffer= getDataFromQueue();


            if (*buffer == 0)
                return ERROR;
            else
                return 1;
            }

        /* determine how many bytes can be read from the queue */

        if (nBytes > bytesAvailable )
            bytesToBeRead = bytesAvailable;
        else
            bytesToBeRead = nBytes;

        /* read the number of bytes and store it in buffer */
        USB_KBD_MUTEX_GIVE;
        for (i = 0; i < bytesToBeRead; i++)
            {
            buffer[i] = getDataFromQueue();
            }


        }
    else
        {
        USB1_KBD_ERR("Unsupported Mode \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* return the number of bytes read */
    return bytesToBeRead;

    }

/*******************************************************************************
*
* usbKbdWrite - write to the USB keyboard
*
*
* USB keyboards don't like to be written to.  We don't support this feature.
*
* RETURNS: ENOSYS
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL ssize_t usbKbdWrite
    (
    USB_KBD_DEV   * pUsbKbdDev,     /* keyboard device to read from */
    char          * buffer,         /* buffer of data to write  */
    size_t          nBytes          /* number of bytes in buffer */
    )
    {

    return (ENOSYS);
    }


/*************************************************************************
*
* usbKbdRcvCallback - invoked when reports received from keyboard.
* This callback is registered with the Driver so as to receive a notification
* whenever data is available. If data is available, we are updating the count
* of characters that are in the keyboard buffer and then waking up all tasks
* that are pending on the keyboard for data by calling selWakeupAll.
*
* RETURNS: OK
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS usbKbdRcvCallback
    (
    void *    putRxCharArg, /* argument */
    char      nextChar      /* character read */
    )
    {
    /* Retrieve the USB_KBD_DEV pointer. */

    USB_KBD_DEV    * pUsbKbdDev = ( USB_KBD_DEV *) putRxCharArg;

    USB_KBD_MUTEX_TAKE(WAIT_FOREVER);

     /* use for bootAppShell echo in bootrom mode */
#ifdef INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
    if(pUsbKbdDev->pTyDev != NULL)
        tyIRd (pUsbKbdDev->pTyDev, nextChar);
#endif
    /* put the data in the queue */

    putDataInQueue (nextChar);

    USB_KBD_MUTEX_GIVE;

    /* If we have elements in the Wake up list, wake them all up! */

    if (selWakeupListLen(&pUsbKbdDev->selWakeupList) > 0)
        {
        selWakeupAll (&pUsbKbdDev->selWakeupList, SELREAD);
        }

    return OK;
    }


/*****************************************************************************
*
* usrUsbKbdInit - initialize the USB keyboard driver
*
* This function initializes the USB keyboard driver and registers for attach
* callbacks.
*
* RETURNS: OK or ERROR, on failure
*
* ERRNO: none
*
* \NOMANUAL
*/
STATUS usrUsbKbdInit (void)
    {
    int index;
    /* Check if driver already installed */

    if (usbKbdDrvNum > 0)
        {
        USB1_KBD_ERR("Keyboard driver has already initilaized \n",
                     1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* Create the mutex semaphores */

    usbKbdMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                  SEM_INVERSION_SAFE);

    usbKbdListMutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                  SEM_INVERSION_SAFE);

    if (usbKbdMutex == NULL ||
        usbKbdListMutex == NULL)
        {
        USB1_KBD_ERR("Resource Allocation Failure.\n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }

    /* initialize the linked list */

    lstInit (&usbKbdList);

    /* Install the appropriate functions into the driver table */
    usbKbdDrvNum = iosDrvInstall ((DRV_CREATE_PTR) NULL,
                                  (DRV_REMOVE_PTR) usbKbdDevDelete,
                                  (DRV_OPEN_PTR) usbKbdOpen,
                                  (DRV_CLOSE_PTR) usbKbdClose,
                                  (DRV_READ_PTR) usbKbdRead,
                                  (DRV_WRITE_PTR) usbKbdWrite,
                                  (DRV_IOCTL_PTR) usbKbdIoctl);

    /*  check to see if there was room to install the driver */

    if (usbKbdDrvNum <= 0)
        {
        errnoSet (S_ioLib_NO_DRIVER);
        USB1_KBD_ERR("There is no more room in the driver table \n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }

    /* initialize all the elements in the queue to NULL */

    memset (kbdQueueData.queueData, 0, QUEUE_SIZE);
    kbdQueueData.queueFront = 0;
    kbdQueueData.queueRear = -1;
    kbdQueueData.queueSem  = semBCreate(SEM_Q_FIFO ,SEM_EMPTY);

    if (kbdQueueData.queueSem == NULL)
        {
        USB1_KBD_ERR(" Create kbdQueueData.queueSem fail\n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }

    for (index = 0; index < USB_MAX_KEYBOARDS; index++)
         {
         pKbdDevArray[index] = NULL;
         }

    if (usbKeyboardDevInit () == OK)
        {
        if (usbKeyboardDynamicAttachRegister (usbKbdDrvAttachCallback,
                          (void *) NULL)
                      != OK)
            {
            USB1_KBD_ERR("usbKeyboardDynamicAttachRegister() return ERROR \n",
                         1, 2, 3, 4, 5, 6);
            (void) usbKeyboardDevShutdown();
            goto ERROR_HANDLER;
            }
        }
    else
        {
        USB1_KBD_ERR("usbKeyboardDevInit() returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        goto ERROR_HANDLER;
        }

    USB1_KBD_INFO("USB Keyboard Driver Successfully Initialized \r\n",
                  1, 2, 3, 4, 5, 6);
    return (OK);

ERROR_HANDLER:

    if(usbKbdDrvNum)
        {
        (void) iosDrvRemove (usbKbdDrvNum, 1);
        usbKbdDrvNum = 0;
        }

    if (kbdQueueData.queueSem != NULL)
        {
        (void) semDelete(kbdQueueData.queueSem);
        kbdQueueData.queueSem = NULL;
        }

    if (usbKbdMutex != NULL)
        (void) semDelete(usbKbdMutex);

    if(usbKbdListMutex != NULL)
        (void) semDelete(usbKbdListMutex);

    usbKbdMutex = NULL;
    usbKbdListMutex = NULL;
    return ERROR;
    }

#ifndef INCLUDE_USB_KEYBOARD_SHELL_ATTACH
#ifndef INCLUDE_USBTOOL
/*************************************************************************
*
* enterThread - waits for user to press [enter]
*
* This is a thread which wait for the enter to be pressed for the keyboard
* driver to come out of polling mode
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void enterThread ( void )

    {
    char bfr [256];

    printf ("Press [enter] to terminate polling.\n");
    gets (bfr);
    enterPressed = TRUE;
    }


/*************************************************************************
*
* playWithKeyboard - prints key presses to the terminal
*
* This is a test-application which prints the key pressed to the terminal.
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*
*/

STATUS playWithKeyboard ( void )
    {
    TASK_ID    taskId = OS_THREAD_FAILURE;
    char       inChar;
    int        fd = 0;


    /* Poll for input or until user presses CTRL-Z on USB keyboard or
     * [enter] on main keyboard. */


    if ((fd = open ("/usbKb/0", O_RDONLY, 0)) == ERROR)
        return (ERROR);

    /* Create thread to watch for keypress */

    enterPressed = FALSE;

     if((taskId = taskSpawn ("tEnter",
                             5,          /* priority */
                             0,          /* task options */
                             0x4000,     /* 16k stack space */
                             (FUNCPTR) enterThread,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )) == OS_THREAD_FAILURE)
         {
         USB1_KBD_ERR("TaskSpawn Error...! \n",
                      1, 2, 3, 4, 5, 6);
         close (fd);
         return ERROR;
         }

    printf ("Press CTRL-Z to terminate polling.\n");

    while (!enterPressed)
        {
        if (read (fd, &inChar, 1) == 1)
            {
            printf ("ASCII %3d", inChar);
            if (inChar >= 32)
                printf (" '%c'", inChar);
            printf ("\n");

        if (inChar == CTRL_Z)
            {
            printf ("Stopped by CTRL-Z\n");
            break;
            }
        }

        taskDelay (2);
        }

    taskDelete (taskId);
    close(fd);
    return OK;
    }
#endif
#endif

/*******************************************************************************
*
* putDataInQueue - puts the data in the circular queue
*
* The function puts the data in the circular queue <KBD_QUEUE_DATA>.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL VOID putDataInQueue
    (
    char    data        /* Data to put in the queue */
    )
    {
    INT8         tempRear = kbdQueueData.queueRear;
                            /* temporary pointer to hold the rear pointer */

    /* if end of the queue is reached, start from front */

    if (kbdQueueData.queueRear == QUEUE_SIZE - 1)
        {
        kbdQueueData.queueRear = 0;
        }
    else
        {
        kbdQueueData.queueRear++ ;
        }

    /*
     * if the "front" points where "rear" is pointing and the queue holds a
     * not NULL value; update as QUEUE OVERFLOW condition
     */

    if ((kbdQueueData.queueRear == kbdQueueData.queueFront) &&
        (kbdQueueData.queueData[kbdQueueData.queueFront] != 0))
        {
        /* intialize the rear to initial value */

        kbdQueueData.queueRear = tempRear;

        /* queue has over flowed. Do not accept any more data */
        semGive(kbdQueueData.queueSem);
        return;
        }

    /* put the data in the queue */

    kbdQueueData.queueData [kbdQueueData.queueRear] = data;
    semGive(kbdQueueData.queueSem);
    return;
    }

/*******************************************************************************
*
* getDataFromQueue - gets the data in the circular queue
*
* The function get the data in the circular queue <KBD_QUEUE_DATA>.
*
* RETURNS: data read from keyboard or NULL on no data
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL char getDataFromQueue (void)
    {

    char    inChar;        /* data read */

    /* if the queue is empty; return 0 */

    semTake(kbdQueueData.queueSem, WAIT_FOREVER);

    if (isDataQueueEmpty() == TRUE)
        {
        return 0;
        }

    /* read the data from the queue */

    inChar = kbdQueueData.queueData[kbdQueueData.queueFront];

    /* intialize the memory location read to NULL */

    kbdQueueData.queueData[kbdQueueData.queueFront] = 0;

    /*
     * since both front and rear are at same location, queue is empty now.
     * intialize to initial value
     */

    if (kbdQueueData.queueFront == kbdQueueData.queueRear)
        {
        kbdQueueData.queueFront = 0;
        kbdQueueData.queueRear = -1;
        return inChar;
        }

    /* increment the front */

    if (kbdQueueData.queueFront == QUEUE_SIZE - 1)
        {
        kbdQueueData.queueFront = 0;
        }
    else
        {
        kbdQueueData.queueFront++;
        }
    /* return data read */

    if (isDataQueueEmpty() == FALSE)
        {
        semGive(kbdQueueData.queueSem);
        }

    return inChar;
    }

/*******************************************************************************
* isDataQueueEmpty - checks whether there is data in queue
*
* RETURNS: TRUE if the queue is empty or FALSE if it has data
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL BOOL isDataQueueEmpty (void)
    {
    if (kbdQueueData.queueRear  == -1)
        return TRUE;
    else
        return FALSE;
    }


/*******************************************************************************
* queueBytesCount - gets the number of bytes in the queue
*
* RETURNS: returns the number of bytes in the queue
*
* ERRNO: none
*
*\NOMANUAL
*/

LOCAL UINT16 queueBytesCount (void)
    {

    INT8     front = kbdQueueData.queueFront; /* front of queue */
    UINT16   count = 1; /* counter for number of bytes in the queue */

    /* check if there is any data, if no return 0 */

    if (isDataQueueEmpty())
        return 0;

    while (front != kbdQueueData.queueRear)
        {
        /* increment the counter */

        count++;

        /*
         * if the front is pointing to the last element of the queue and rear
         * is not reached reset the front to the start of the queue
         */

        if (front == (QUEUE_SIZE - 1))
            front = 0;
        else
            front++;
        }
    /* return the number of bytes */

    return count;
    }

#endif /* __INCusrUsbKbdInitc */
