/* usrUsbTgtMscInit.c - Initialization of the USB Target MSC Function Driver */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,12jul11,s_z  Remove compile warning
01k,16may11,s_z  Remove force format type in usbTgtMscBlockDevCreate(WIND00271227)
01j,12apr11,m_y  correct wrong spell
01i,11apr11,m_y  disable the default ramdisk
01h,22mar11,s_z  Changes for unused routines removed
01g,17mar11,m_y  modify configuration items for usb MSC driver
01f,17mar11,m_y  modify code based on the code review
01e,09mar03,s_z  Code clean up
01d,09mar03,m_y  Add function name definition
01c,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01b,28dec10,x_f  clean up
01a,12oct10,s_z  created
*/

/*
DESCRIPTION

This file includes the definations and the routines to configure the USB 
mass storage emulation function driver.

*/

#ifndef __INCusrUsbTgtMscInitc
#define __INCusrUsbTgtMscInitc

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <blkIo.h>
#include <xbdRamDisk.h>
#include <usb/usbPlatform.h>
#include <usb/usb.h>
#include <usb/usbLib.h>
#include <usb/usbTgt.h>
#include <usb/usbTgtMsc.h>

/* Selection 1*/

#if defined(INCLUDE_USBTGT_MSC_1)

#ifndef USBTGT_MSC_ATTACH_TCD_NAME1
#   define USBTGT_MSC_ATTACH_TCD_NAME1             "/usbTgt"
#endif

#ifndef USBTGT_MSC_ATTACH_TCD_UNIT1
#   define USBTGT_MSC_ATTACH_TCD_UNIT1             0
#endif

#ifndef USBTGT_MSC_NAME1
#   define USBTGT_MSC_NAME1                        "usbTgtMsc"
#endif

#ifndef USBTGT_MSC_UNIT1
#   define USBTGT_MSC_UNIT1                        0
#endif

#ifndef USBTGT_MSC_PROTOCOL1
#   define USBTGT_MSC_PROTOCOL1                    "RBC"
#endif

#ifndef USBTGT_MSC_CONFIG_NUM1
#   define USBTGT_MSC_CONFIG_NUM1                  1
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_NAME1
#   define USBTGT_MSC_ATTACH_DEVICE_NAME1          "/ram0"
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_REMOVABLE1
#   define USBTGT_MSC_ATTACH_DEVICE_REMOVABLE1     "y"
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_READONLY1
#   define USBTGT_MSC_ATTACH_DEVICE_READONLY1      "n"
#endif

#endif

/* Selection 2 */

#if defined(INCLUDE_USBTGT_MSC_2)

#ifndef USBTGT_MSC_ATTACH_TCD_NAME2
#   define USBTGT_MSC_ATTACH_TCD_NAME2             "/usbTgt"
#endif

#ifndef USBTGT_MSC_ATTACH_TCD_UNIT2
#   define USBTGT_MSC_ATTACH_TCD_UNIT2             0
#endif

#ifndef USBTGT_MSC_NAME2
#   define USBTGT_MSC_NAME2                        "usbTgtMsc"
#endif

#ifndef USBTGT_MSC_UNIT2
#   define USBTGT_MSC_UNIT2                        0
#endif

#ifndef USBTGT_MSC_PROTOCOL2
#   define USBTGT_MSC_PROTOCOL2                    "RBC"
#endif

#ifndef USBTGT_MSC_CONFIG_NUM2
#   define USBTGT_MSC_CONFIG_NUM2                  1
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_NAME2
#   define USBTGT_MSC_ATTACH_DEVICE_NAME2          "/ram0"
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_REMOVABLE2
#   define USBTGT_MSC_ATTACH_DEVICE_REMOVABLE2     "y"
#endif

#ifndef USBTGT_MSC_ATTACH_DEVICE_READONLY2
#   define USBTGT_MSC_ATTACH_DEVICE_READONLY2      "n"
#endif

#endif

USR_USBTGT_MSC_CONFIG usrUsbTgtMscConfigTable[] = 
    {
#if defined (INCLUDE_USBTGT_MSC_1)
        {
        USBTGT_MSC_ATTACH_TCD_NAME1,
        USBTGT_MSC_ATTACH_TCD_UNIT1,
        USBTGT_MSC_NAME1,
        USBTGT_MSC_PROTOCOL1,
        USBTGT_MSC_UNIT1,
        USBTGT_MSC_CONFIG_NUM1,
        USBTGT_MSC_ATTACH_DEVICE_NAME1,
        USBTGT_MSC_ATTACH_DEVICE_REMOVABLE1,
        USBTGT_MSC_ATTACH_DEVICE_READONLY1
        },
#endif

#if defined (INCLUDE_USBTGT_MSC_2)
        {
        USBTGT_MSC_ATTACH_TCD_NAME2,
        USBTGT_MSC_ATTACH_TCD_UNIT2,
        USBTGT_MSC_NAME2,
        USBTGT_MSC_PROTOCOL2,
        USBTGT_MSC_UNIT2,
        USBTGT_MSC_CONFIG_NUM2,
        USBTGT_MSC_ATTACH_DEVICE_NAME2,
        USBTGT_MSC_ATTACH_DEVICE_REMOVABLE2,
        USBTGT_MSC_ATTACH_DEVICE_READONLY2
        },

#endif
    };

#define USBTGT_MSC_COUNT NELEMENTS(usrUsbTgtMscConfigTable)
#define USBTGT_MSC_DEFAULT_RAMDISK   FALSE

#if (USBTGT_MSC_DEFAULT_RAMDISK == TRUE)
LOCAL device_t usbTgtMscDefaultRamHandle;

/*******************************************************************************
*
* usbTgtMscBlockDevCreate - create a RAM block device 
*
* This routine creates the RAM block device for usb target mass storage 
* resource.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS usbTgtMscBlockDevCreate
    (
    char * devName,
    UINT32 blockSize,
    UINT32 blockNum
    )
    {
    printf("usbTgtMscBlockDevCreate: Trying to create RAM disk %s\n", devName);

    usbTgtMscDefaultRamHandle = xbdRamDiskDevCreate
            (
            blockSize,              /* number of bytes per block */
            blockNum * blockSize,   /* number of bytes on this device */
            FALSE,
            devName
            );

    if (usbTgtMscDefaultRamHandle == (device_t)0x0)
        {
        printf("usbTgtMscBlockDevCreate:"
               "ramDiskDevCreate returned NULL\n");

        return(ERROR);
        }

    printf("OK. Trying to format RAM disk %s as dosFs\n", devName);

    /* 
     * Format the RAM disk, ignore memory contents
     *
     * Remove the force format type, and let 
     * FS set the format type according to the sector count.
     */

    if (dosFsVolFormat(devName,
                       DOS_OPT_BLANK | DOS_OPT_QUIET,
                       NULL) != OK)
        {
        printf("dosFsVolFormat failed\n");

        return ERROR;
        }

    return (OK);
    }
#endif

/*******************************************************************************
*
* usrUsbTgtMscInit - initialize the global usb target resource
*
* This routine initializes the global usb target resource.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtMscInit (void)
    {
    /* Create the RAM disk for use first */
#if (USBTGT_MSC_DEFAULT_RAMDISK == TRUE)
    usbTgtMscBlockDevCreate("/ram0", 512, 20480);
#endif

    usbTgtMscInit(USBTGT_MSC_COUNT);

    return;
    }

/*******************************************************************************
*
* usrUsbTgtMscUnInit - uninitialize the global usb target resource
*
* This routine uninitializes the global usb target resource.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtMscUnInit (void)
    {
    usbTgtMscExit();
#if (USBTGT_MSC_DEFAULT_RAMDISK == TRUE)
    xbdRamDiskDevDelete(usbTgtMscDefaultRamHandle);
#endif
    return;
    }

#endif /* __INCusrUsbTgtMscInitc */
