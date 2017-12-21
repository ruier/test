/* usrUsn2SerialPcConInit.c - Generation 2 USB-to-Serial PC console config file */

/*
 * Copyright (c) 2010-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,01aug11,ghs  Fix issue found during code review (WIND00250716)
01b,07jan11,ghs  Clean up compile warnings (WIND00247082)
01a,02may10,ghs  written

*/
#ifndef __INCusrUsb2SerialPcConInitc
#define __INCusrUsb2SerialPcConInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <usb2/usb2Serial.h>

/* defines */

/* default device name */

#ifndef USB_GEN2_SERIAL_CONSOLE_DEVICE_NAME
#define USB_GEN2_SERIAL_CONSOLE_DEVICE_NAME             "/usbPcCon/"
#endif

/* default attached device name */

#ifndef USB_GEN2_SERIAL_PCCON_ATTACH_DEVICE_NAME
#define USB_GEN2_SERIAL_PCCON_ATTACH_DEVICE_NAME        "/usb2ttyS/"
#endif

/* default baud rate*/

#ifndef USB_GEN2_SERIAL_PCCON_DEFAULT_BAUDRATE
#define USB_GEN2_SERIAL_PCCON_DEFAULT_BAUDRATE          9600
#endif

/* default dummy buffer size */

#ifndef USB_GEN2_SERIAL_PCCON_DUMMY_BUFFER_SIZE
#define USB_GEN2_SERIAL_PCCON_DUMMY_BUFFER_SIZE         4096
#endif

/* default io buffer size */

#ifndef USB_GEN2_SERIAL_PCCON_OUT_BUFFER_SIZE
#define USB_GEN2_SERIAL_PCCON_OUT_BUFFER_SIZE           1024
#endif

/* console transfer task priority */

#ifndef USB_GEN2_SERIAL_PCCON_TASK_PRIORITY
#define USB_GEN2_SERIAL_PCCON_TASK_PRIORITY             100
#endif

/* max pc console device support */

#ifndef USB_GEN2_SER_MAX_CONSOLE
/* do not larger than 10 */
#define USB_GEN2_SER_MAX_CONSOLE                        1
#endif

USB_PC_CON_SIO_CHAN * pUsbPcConSioChan[USB_GEN2_SER_MAX_CONSOLE];

char usbPcConBindName[MAX_DRV_NAME_LEN] =
    {
    USB_GEN2_SERIAL_PCCON_ATTACH_DEVICE_NAME
    };

char usbPcConDevName[MAX_DRV_NAME_LEN] =
    {
    USB_GEN2_SERIAL_CONSOLE_DEVICE_NAME
    };

INT32 usbPcConDefaultBaudRate = USB_GEN2_SERIAL_PCCON_DEFAULT_BAUDRATE;

INT32 usbPcConDummyBfrSize = USB_GEN2_SERIAL_PCCON_DUMMY_BUFFER_SIZE;

INT32 usbPcConOutBfrSize = USB_GEN2_SERIAL_PCCON_OUT_BUFFER_SIZE;

INT32 usbPcConTaskPriority = USB_GEN2_SERIAL_PCCON_TASK_PRIORITY;

INT32 usbPcConMax = USB_GEN2_SER_MAX_CONSOLE;

INT32 usbPcConSysConTtyNum = CONSOLE_TTY;

IMPORT VOID usbPcConVxbInstInit(struct vxbDev *);

IMPORT VOID usbPcConVxbInstInit2(struct vxbDev *);

IMPORT VOID usbPcConVxbInstConnect(struct vxbDev *);

IMPORT device_method_t usbPcConSioDrvMethods[];

IMPORT struct vxbBusPresent * pPlbBus;

LOCAL struct drvBusFuncs usbPcConVxbFuncs =
    {
    usbPcConVxbInstInit,     /* devInstanceInit */
    usbPcConVxbInstInit2,    /* devInstanceInit2 */
    usbPcConVxbInstConnect   /* devConnect */
    };

/* vxBus registration for pc console driver */

LOCAL struct vxbDevRegInfo usbPcConVxbRegistration[USB_GEN2_SER_MAX_CONSOLE];

LOCAL BOOL usbPcConDevInited = FALSE;

/*******************************************************************************
*
* usbPcConVxbAnnounce - announce the dummy usb serial device to vxBus
*
* This routine announce the dummy usb serial device to vxBus, alloc a device
* struct and announce this device to vxBus. after this, the dummy device will
* be added to system SIO channel list and can be read or write.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID usbPcConVxbAnnounce
    (
    int     usbConDevId,
    char *  devName
    )
    {
    struct vxbDev * pDev;

    pDev = vxbDevStructAlloc(WAIT_FOREVER);

    pDev->pParentBus = pPlbBus;

    pDev->pMethods = &usbPcConSioDrvMethods[0];

    /* the device name is same as driver name register in vxBus */

    pDev->pName = devName;

    pDev->busID = VXB_BUSID_PLB;

    /* announce it */

    vxbDeviceAnnounce(pDev);
    }

/*******************************************************************************
*
* usbPcConVxbRegister - register the dummy usb serial device driver to vxBus
*
* This routine register the dummy usb serial device driver to vxBus. This driver
* is a dummy PLB device driver, driver name come from user configure. After
* register the driver immediately, the routine call usbPcConVxbAnnounce() to
* announce this device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID usbPcConVxbRegister
    (
    void
    )
    {
    int     devId;
    char    devName[MAX_DRV_NAME_LEN + 1] = {0};

    if (usbPcConDevInited == TRUE)
        return;

    usbPcConDevInited = TRUE;

    for (devId = 0; devId < usbPcConMax; devId++)
        {
        usbPcConVxbRegistration[devId].devID = VXB_DEVID_DEVICE;
        usbPcConVxbRegistration[devId].busID = VXB_BUSID_PLB;
        usbPcConVxbRegistration[devId].vxbVersion = USB_VXB_VERSIONID;
        usbPcConVxbRegistration[devId].pDrvBusFuncs = &usbPcConVxbFuncs;

        strcpy(devName, usbPcConDevName);

        devName[MAX_DRV_NAME_LEN - 1] = '\0';

        devName[strlen(devName)] = devId + '0';

        strcpy(usbPcConVxbRegistration[devId].drvName, devName);

        vxbDevRegister(&usbPcConVxbRegistration[devId]);

        usbPcConVxbAnnounce(devId, usbPcConVxbRegistration[devId].drvName);
        }
    }

/*******************************************************************************
*
* getUsbPcConSioChanNo - get the serial channel number of specific device
*
* This routine return the serial channel number of specific device. Device ID is
* from 0 to usbPcConMax. This routine can be used in usrConfig.c or prjConfig.c,
* use it to redefine NUM_TTY can get max USB pc console number.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

int getUsbPcConSioChanNo
    (
    int usbConDevId
    )
    {
    return pUsbPcConSioChan[usbConDevId]->channelNo;
    }
#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2SerialPcConInitc */

