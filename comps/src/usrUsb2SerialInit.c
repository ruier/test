/* usrUsn2SerialInit.c - Generation 2 USB-to-Serial class driver config file */

/*
 * Copyright (c) 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,16apr13,ghs  Add USB PL2303 serial device support (WIND00412983)
01b,07jan11,ghs  Clean up compile warnings (WIND00247082)
01a,12apr10,ghs  written

*/
#ifndef __INCusrUsb2SerialInitc
#define __INCusrUsb2SerialInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <usb2/usb2Serial.h>

/* defines */

/* Default device name */
#ifndef USB_GEN2_SERIAL_NAME
#define USB_GEN2_SERIAL_NAME                        "/usb2ttyS/"
#endif

/* default baud rate */
#ifndef USB_GEN2_SERIAL_DEFAULT_BAUDRATE
#define USB_GEN2_SERIAL_DEFAULT_BAUDRATE            9600
#endif

/* default ios buffer size */
#ifndef USB_GEN2_SERIAL_IOS_RING_BUFFER_SIZE
#define USB_GEN2_SERIAL_IOS_RING_BUFFER_SIZE        8192
#endif

/* device time out in ms */
#ifndef USB_GEN2_SERIAL_TIME_OUT
#define USB_GEN2_SERIAL_TIME_OUT                    1000
#endif

#ifndef USB_GEN2_SERIAL_COMMON_TASK_PRIORITY
#define USB_GEN2_SERIAL_COMMON_TASK_PRIORITY        100
#endif

#ifndef USB_GEN2_SER_FTDI_POLL_INTERVAL
#define USB_GEN2_SER_FTDI_POLL_INTERVAL             0
#endif

#ifndef USB_GEN2_SER_WRSCDC_POLL_INTERVAL
#define USB_GEN2_SER_WRSCDC_POLL_INTERVAL           0
#endif

#ifndef USB_GEN2_SER_PL2303_POLL_INTERVAL
#define USB_GEN2_SER_PL2303_POLL_INTERVAL           0
#endif

#ifdef INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
IMPORT USB_PC_CON_SIO_CHAN *    pUsbPcConSioChan[];

IMPORT char                     usbPcConBindName[];

IMPORT INT32                    usbPcConDefaultBaudRate;

IMPORT INT32                    usbPcConMax;
#endif

INT32   usb2SerialDefaultBaudRate = USB_GEN2_SERIAL_DEFAULT_BAUDRATE;

size_t  usb2SerialRingBfrSize = USB_GEN2_SERIAL_IOS_RING_BUFFER_SIZE;

INT32   usb2SerialTimeOut = USB_GEN2_SERIAL_TIME_OUT;

INT32   usb2SerialCommonTaskPriority = USB_GEN2_SERIAL_COMMON_TASK_PRIORITY;

#ifdef INCLUDE_USB_GEN2_SER_FTDI232
UINT32  usb2SerFtdiPollInterval = USB_GEN2_SER_FTDI_POLL_INTERVAL;
#endif

#ifdef INCLUDE_USB_GEN2_SER_WRS_CDC
UINT32  usb2SerWrsPollInterval = USB_GEN2_SER_WRSCDC_POLL_INTERVAL;
#endif

#ifdef INCLUDE_USB_GEN2_SER_PL2303
UINT32  usb2SerPl2303PollInterval = USB_GEN2_SER_PL2303_POLL_INTERVAL;
#endif

/*******************************************************************************
*
* usrUsb2SerialInit - initializes the GEN2 USB-to-Serial class driver
*
* This routine is initializing the GEN2 USB-to-Serial class driver.
* If called automatically by the project facility
*
* RETURNS: If the USB-to-Serial driver could be initialized or not
*
* ERRNO: N/A
*/

STATUS usrUsb2SerialInit
    (
    void
    )
    {
#ifdef INCLUDE_USB_GEN2_SER_PL2303
    usb2SerialPL2303Init(NULL);
#endif

#ifdef INCLUDE_USB_GEN2_SER_FTDI232
    usb2SerialFtdi232Init(NULL);
#endif

#ifdef INCLUDE_USB_GEN2_SER_WRS_CDC
    usb2SerialWrsInit(NULL);
#endif
    return usb2SerialInit((INT8*)(USB_GEN2_SERIAL_NAME));
    }

/*******************************************************************************
*
* usrUsb2SerialAttachPcCon - attach a specific device to usb pc console
*
* This routine is used for attaching a specific device to usb pc console.
* The device name is configured in config files, if the class driver detect a
* USB-to-Serial adapter device have the same name as configured name, this
* device will be attached to pc console
*
* Since this routine are used in USB-to-Serial class driver, and depend on
* config file, so it is located in this config file.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID usrUsb2SerialAttachPcCon
    (
    USB2_SERIAL_DEVICE *    pDevice
    )
    {
#ifdef INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
    int devId;

    for (devId = 0; devId < usbPcConMax; devId++)
        {
        if (pUsbPcConSioChan[devId] != NULL
            && strcmp(pDevice->deviceHeader.name,
                pUsbPcConSioChan[devId]->bindDevName) == 0)
            {
            pDevice->pUsbPcConSioChan = pUsbPcConSioChan[devId];

            printf("Attach %s to Serial Channel %d\n",
                   pDevice->deviceHeader.name,
                   pDevice->pUsbPcConSioChan->channelNo);

            semTake(pDevice->pUsbPcConSioChan->hFdMutex, WAIT_FOREVER);

            pDevice->pUsbPcConSioChan->fd = open(pDevice->deviceHeader.name,
                                                 O_RDWR, 0);

            ioctl(pDevice->pUsbPcConSioChan->fd, SIO_BAUD_SET,
                  pDevice->pUsbPcConSioChan->baudRate);

            write(pDevice->pUsbPcConSioChan->fd,
                  pDevice->pUsbPcConSioChan->dummyBfr,
                  pDevice->pUsbPcConSioChan->dummyBfrDataSize);

            semGive(pDevice->pUsbPcConSioChan->hFdMutex);

            pDevice->pUsbPcConSioChan->dummyBfrDataSize = 0;
            }
        }
#endif
    }

/*******************************************************************************
*
* usrUsb2SerialDetachPcCon - detach a specific device from usb pc console
*
* This routine is used for detaching a specific device from usb pc console.
* The device name is configured in config files, if the class driver detect a
* USB-to-Serial adapter device have the same name as configured name, this
* device will be detached
*
* Since this routine are used in USB-to-Serial class driver, and depend on
* config file, so it is located in this config file.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

VOID usrUsb2SerialDetachPcCon
    (
    USB2_SERIAL_DEVICE *pDevice
    )
    {
#ifdef INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
    if (pDevice->pUsbPcConSioChan != NULL)
        {
        int fd;

        semGive(pDevice->pUsbPcConSioChan->hFdMutex);

        fd = pDevice->pUsbPcConSioChan->fd;

        pDevice->pUsbPcConSioChan->fd = NONE;

        close(fd);

        semGive(pDevice->pUsbPcConSioChan->hFdMutex);


        pDevice->pUsbPcConSioChan = NULL;
        }
#endif
    }

#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2SerialInitc */

