/* usrUsbTgtSerInit.c - Initialization of the CDC Serial Emulator function driver */

/*
 * Copyright (c) 2009 - 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01l,06may13,s_z  Remove compile warning (WIND00356717)
01k,12jul11,s_z  Remove compile warning
01j,22mar11,s_z  Changes for unused routines removed
01i,22mar11,ghs  Fix compile error for command line build(WIND00262317)
01h,09mar11,ghs  Change target function name
01g,07mar11,ghs  Fix code review issues
01f,26jan11,ghs  Merge and port to new target platform,
                 base on usrUsbTargSerInit.c
01e,13jun10,ghs  Save console tty number
01d,13jan10,ghs  vxWorks 6.9 LP64 adapting
01c,15sep09,w_x  Added INCLUDE_AT91_USB_TARG support (WIND00182308)
01b,09sep09,w_x  Added various BSP config options for FSL TCD (WIND00180575)
01a,05mar09,w_x  Created.
*/

/*
DESCRIPTION

This files contains the initialization function for the Ser function
driver on the peripheral.

INCLUDES: vxWorks.h, stdio.h, usb/usbPlatform.h, usb/usbCdc.h, usb/usb.h,
          usb/usbLib.h, usb/usbTgtSer.h
*/

/* includes */

#ifndef __INCusrUsbTgtSerInitc
#define __INCusrUsbTgtSerInitc

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>                     /* memset(), memcpy() */
#include <usb/usbPlatform.h>            /* Basic definitions  */
#include <usb/usbCdc.h>
#include <usb/usb.h>
#include <usb/usbLib.h>
#include <usb/usbTgtSer.h>
#include <tyLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defins */

/*
 * Max serial devices should not larger than 1
 * This is left for extend multi-serial ports
 */

#ifndef USBTGT_SER_MAX_COUNT
#define USBTGT_SER_MAX_COUNT                    1
#elif ((USBTGT_SER_MAX_COUNT) > 1)
#define USBTGT_SER_MAX_COUNT                    1
#elif ((USBTGT_SER_MAX_COUNT) < 1)
#define USBTGT_SER_MAX_COUNT                    1
#endif

/* Default device name for vxBus */
#ifndef USBTGT_SER_NAME
#define USBTGT_SER_NAME                         "/usbTgtSer/"
#endif

/* default baud rate */
#ifndef USBTGT_SER_DEFAULT_BAUDRATE
#define USBTGT_SER_DEFAULT_BAUDRATE             9600
#endif

/* default ios buffer size */
#ifndef USBTGT_SER_DUMMY_BUFFER_SIZE
#define USBTGT_SER_DUMMY_BUFFER_SIZE            4096
#endif

/* enable dummy buffer */
#ifndef USBTGT_SER_DUMMY_BUFFER_ENABLE
#define USBTGT_SER_DUMMY_BUFFER_ENABLE          TRUE
#endif

/* enable display redirect message */
#ifndef USBTGT_SER_REDIRECT_MSG_ENABLE
#define USBTGT_SER_REDIRECT_MSG_ENABLE          TRUE
#endif

/* default tcd bulk buffer size */
#ifndef USBTGT_SER_BULK_BUFFER_SIZE
#define USBTGT_SER_BULK_BUFFER_SIZE             512
#endif

/* transfer task priority */
#ifndef USBTGT_SER_TX_TASK_PRIORITY
#define USBTGT_SER_TX_TASK_PRIORITY             100
#endif

#ifndef USBTGT_SER_BINDING_TO_CONFIG_NUM
#define USBTGT_SER_BINDING_TO_CONFIG_NUM        1
#endif

#ifndef USBTGT_SER_BINDING_TO_TARGET_UNIT
#define USBTGT_SER_BINDING_TO_TARGET_UNIT       0
#endif

#ifndef USBTGT_SER_BINDING_TO_TARGET_NAME
#define USBTGT_SER_BINDING_TO_TARGET_NAME       "/usbTgt"
#endif

#ifndef USBTGT_SER_FUNC_NAME
#define USBTGT_SER_FUNC_NAME                    "usbTgtSer"
#endif

/* globals */

/* dynamic configuration settings */

int usbTgtSerSysConNum          = CONSOLE_TTY;

int usbTgtSerMaxCount           = USBTGT_SER_MAX_COUNT;

int usbTgtSerDummyBfrSize       = USBTGT_SER_DUMMY_BUFFER_SIZE;

int usbTgtSerDefualtBaudrate    = USBTGT_SER_DEFAULT_BAUDRATE;

int usbTgtSerTxPriority         = USBTGT_SER_TX_TASK_PRIORITY;

BOOL usbTgtSerVxbInited         = FALSE;

BOOL usbTgtSerReInitFlag        = FALSE;

BOOL usbTgtSerDummyBufferEnable = USBTGT_SER_DUMMY_BUFFER_ENABLE;

BOOL usbTgtSerRedirectMsgEnable = USBTGT_SER_REDIRECT_MSG_ENABLE;

char * usbTgtSerVxbDevName      = USBTGT_SER_NAME;

char * usbTgtSerFuncName   = USBTGT_SER_FUNC_NAME;
char * usbTgtSerTargetName = USBTGT_SER_BINDING_TO_TARGET_NAME;
UINT8  usbTgtSerTargetUnit = USBTGT_SER_BINDING_TO_TARGET_UNIT;
UINT8  usbTgtSerConfigNum  = USBTGT_SER_BINDING_TO_CONFIG_NUM;

/* Sio Chan for global */

USBTGT_ACM_SIO_CHAN * pUsbTgtSerSioChan[USBTGT_SER_MAX_COUNT];

IMPORT device_method_t usbTgtSerSioDrvMethods[];

IMPORT struct vxbBusPresent * pPlbBus;

IMPORT int consoleFd;

/* locals */

/*
 * The drvBusFuncs structure provides a set of entry points into the
 * driver that are called during various phases of the
 * boot process. Drivers can choose to implement 1 or
 * more of these entry point, according to the needs of
 * the driver during its initialization phases.
 */

LOCAL struct drvBusFuncs usbTgtSerVxbFuncs =
    {
    usbTgtSerInstInit,     /* devInstanceInit */
    usbTgtSerInstInit2,    /* devInstanceInit2 */
    usbTgtSerInstConnect   /* devConnect */
    };

LOCAL struct vxbDevRegInfo usbTgtSerVxbRegistration[USBTGT_SER_MAX_COUNT];

LOCAL struct vxbDev * pUsbTgtSerVxbDevList[USBTGT_SER_MAX_COUNT];

/*******************************************************************************
*
* usrUsbTgtSerVxbAnnounce - announce the dummy usb serial device to vxBus
*
* This routine announces the dummy usb serial device to vxBus, alloc a device
* struct and announce this device to vxBus. after this, the dummy device will
* be added to system SIO channel list and can be read or write.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID usrUsbTgtSerVxbAnnounce
    (
    int     devId,
    char *  devName
    )
    {
    struct vxbDev * pDev;

    pDev = vxbDevStructAlloc(WAIT_FOREVER);

    if (pDev == NULL)
        return;

    pDev->pParentBus = pPlbBus;

    pDev->pMethods = &usbTgtSerSioDrvMethods[0];

    /* the device name is same as driver name register in vxBus */

    pDev->pName = devName;

    pDev->busID = VXB_BUSID_PLB;

    /* announce it */

    if (vxbDeviceAnnounce(pDev) == OK)
        pUsbTgtSerVxbDevList[devId] = pDev;

    }

/*******************************************************************************
*
* usrUsbTgtSerVxbRegister - register the dummy usb serial device driver to vxBus
*
* This routine registers the dummy usb serial device driver to vxBus. This driver
* is a dummy PLB device driver, driver name come from user configure. After
* register the driver immediately, the routine call usrUsbTgtSerVxbAnnounce() to
* announce this device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtSerVxbRegister
    (
    void
    )
    {
    int     devId;
    char    devName[MAX_DRV_NAME_LEN + 1] = {0};

    if (usbTgtSerVxbInited == TRUE)
        return;

    usbTgtSerVxbInited = TRUE;

    for (devId = 0; devId < USBTGT_SER_MAX_COUNT; devId++)
        {
        usbTgtSerVxbRegistration[devId].busID = VXB_BUSID_PLB;
        usbTgtSerVxbRegistration[devId].devID = VXB_DEVID_DEVICE;
        usbTgtSerVxbRegistration[devId].vxbVersion = USB_VXB_VERSIONID;
        usbTgtSerVxbRegistration[devId].pDrvBusFuncs = &usbTgtSerVxbFuncs;

        strncpy(devName, usbTgtSerVxbDevName, MAX_DRV_NAME_LEN - 1);

        devName[strlen(devName)] = devId + '0';

        strncpy(usbTgtSerVxbRegistration[devId].drvName, devName, MAX_DRV_NAME_LEN);

        vxbDevRegister(&usbTgtSerVxbRegistration[devId]);

        usrUsbTgtSerVxbAnnounce(devId, devName);
        }
    }

/*******************************************************************************
*
* usrUsbTgtSerInit - initialize USB CDC serial emulator driver
*
* This routine initializes the peripheral stack and attaches it with
* the included Target Controller with USB CDC serial emulator functionality.
*
* This routine also check if target serial is restarting, if this driver quit by
* calling usrUsbTgtSerUnInit, then some static data still exist. At this time,
* these data does not recreated.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtSerInit
    (
    void
    )
    {
    /* if it is inited recreate "/tyCo/" devices */

    if (usbTgtSerReInitFlag == TRUE)
        {
        int index;
        char tyName[20];
        char tyNum[2] = {0};
        unsigned int ix;
        USBTGT_ACM_SIO_CHAN * pChan;

        for (index = 0; index < USBTGT_SER_MAX_COUNT; index++)
            {
            if ((pChan = pUsbTgtSerSioChan[index]) != NULL)
                {

                /* recreate tty device */

                if (pChan->ttyNum >= 0
                    && pChan->ttyNum < NUM_TTY
                    && NUM_TTY > 0
                    && pChan->ttyNum != usbTgtSerSysConNum)
                    {
                    pChan->onClosing = FALSE;
                    ix = pChan->ttyNum;
                    strncpy (tyName, "/tyCo/", 19);
                    tyNum[0] = ix + '0';
                    strncat (tyName, tyNum, 19 - sizeof (tyName) );
                    ttyDevCreate (tyName, sysSerialChanGet(ix), 512, 512);
                    }
                }
            }
        }

    /* call real init routine */

    usbTgtSerInit(USBTGT_SER_MAX_COUNT);

    return;
    }

/*******************************************************************************
*
* usrUsbTgtSerUnInit - uninitialize USB CDC serial emulator driver
*
* This routine uninitializes the peripheral stack. Detach and remove register
* function in target management layer.
*
* This routine set usbTgtSerReInitFlag to be true, force stack to recreate tty
* device at next time.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtSerUnInit
    (
    void
    )
    {
    usbTgtSerReInitFlag = TRUE;

    usbTgtSerUnInit(USBTGT_SER_MAX_COUNT);

    return;
    }

/*******************************************************************************
*
* usbTgtSerTtyRemove - delete tty devices for USB CDC serial emulator driver
*
* This routine deletes tty devices for USB CDC serial emulator driver. It can
* be called when target stack quit.
*
* NOTE: if a channel is console or WBD channel, this routine will be failed,
*       because console or WBD channel are static and can not be removed
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbTgtSerTtyRemove
    (
    int index
    )
    {
    USBTGT_ACM_SIO_CHAN * pChan;
    TY_DEV_ID pTyDev;

    pChan = pUsbTgtSerSioChan[index];

    if (pChan == NULL)
        return;

    if (pChan->pUsbTgtSerDev != NULL)
        {
        USBTGT_SER_ERR("Should uninit target driver first before "
                       "remove tty device\n", 1, 2, 3, 4, 5, 6);

        return;
        }

    if (usbTgtSerSysConNum == pChan->channelNo)
        {
        USBTGT_SER_ERR("Console tty device can not be removed\n",
                       1, 2, 3, 4, 5, 6);

        return;
        }

#if (defined(INCLUDE_WDB) && defined(INCLUDE_WDB_COMM_SERIAL))
    if (index == WDB_TTY_CHANNEL)
        {
        USBTGT_SER_DBG("wdb channel can not be removed\n",
                       1, 2, 3, 4, 5, 6);

        return;
        }
#endif

    /* terminate transfer task normally */

    pChan->onClosing = TRUE;

    pTyDev = pChan->putRcvArg;

    (void) sscanf(pTyDev->devHdr.name + strlen("/tyCo/"), "%d", &pChan->ttyNum);

    /* can not remove serial device really, only remove tty device */

    if (iosDevDelete(&pTyDev->devHdr) == ERROR)
        {
        USBTGT_SER_ERR("IOS device delete failed\n", 1, 2, 3, 4, 5, 6);

        return;
        }

    tyDevTerminate(pTyDev);
    }

#endif /* __INCusrUsbTgtSerInitc */

