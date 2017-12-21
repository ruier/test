/* usrUsb2MscInit.c - USB Mass Storage Class Driver User Configuration Module */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,16Jul13,wyy  Disable USB2_MSC_TEST_CODE_ENABLE to decrease footprint
                 (WIND00425875)
01o,06may13,s_z  Remove compiler warning (WIND00356717)
01n,25apr13,wyy  Clean diab compiler warning
01m,02aug12,ljg  Correct commandTimeout (WIND00366753)
01l,19jul12,w_x  Add performance testing routines (WIND00188662)
01k,06jul11,ghs  USB sticks coverage expansion (WIND00183500)
01j,17mar11,j_x  Add multiple inclusion guard symbol (WIND00247788)
01i,07jan11,ghs  Clean up compile warnings (WIND00247082)
01h,08dec10,ghs  Initialize USB GEN2 helper before MSC driver initialize
                 (WIND00245236)
01g,24jun10,m_y  Release all the resources allocated before in error handler
                 (WIND00183499)
01f,06jun10,ghs  Reduce some default display logs(WIND00210988)
01e,30mar10,w_x  Back port GEN2 class drivers for old VxWorks (WIND00192167)
01d,13jan10,ghs  vxWorks 6.9 LP64 adapting
01c,27aug09,w_x  Unify log mechanism and variable naming with some compiler
                 warning removal in USB GEN2 class drivers (WIND00178936)
01b,24apr09,w_x  Coding convention changes and more configuration options
01a,29oct08,w_x  Written
*/

#ifndef __INCusrUsb2MscInitc
#define __INCusrUsb2MscInitc

#include <usb2/usb2Msc.h>

/* disable all extra functions for boot app */

#ifdef BOOTAPP
#define USB2_MSC_DYNAMIC_CONFIG         FALSE
#define USB2_MSC_DIRECT_ACCESS_ENABLE   FALSE
#define USB2_MSC_COMPLIANCE_ENABLE      FALSE
#define USB2_MSC_CBI_TRANSPORT_ENABLE   FALSE
#define USB2_MSC_TEST_CODE_ENABLE       FALSE
#endif

/*
 * dynamic configuration support, can enable/disable some modules in MSC driver
 * dynamically. Enable this flag may cause image built larger.
 */
#ifndef USB2_MSC_DYNAMIC_CONFIG
#define USB2_MSC_DYNAMIC_CONFIG         FALSE
#endif

/*
 * vxWorks 6.3 and above changed to use XBD scheme,
 * and fsMontitor is also used by the filesystem.
 */

#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
#include <drv/xbd/bio.h>
#include <drv/manager/device.h>
#include <fsMonitor.h>
#endif

/* Max number of USB storage devices */
#ifndef USB2_MSC_MAX_DEVICES
#define USB2_MSC_MAX_DEVICES                10
#endif

/* Max number of LUNs per device*/
#ifndef USB2_MSC_MAX_DRIVES
#define USB2_MSC_MAX_DRIVES                 16
#endif

/* Max size of device name */
#ifndef USB2_MSC_MAX_NAME_SZ
#define USB2_MSC_MAX_NAME_SZ                50
#endif

/* Device name base */
#ifndef USB2_MSC_DRIVE_NAME
#define USB2_MSC_DRIVE_NAME                 "/bd"
#endif

/* XBD service task base name */
#ifndef USB2_MSC_SERVICE_TASK_NAME
#define USB2_MSC_SERVICE_TASK_NAME          "tUsb2Msc"
#endif

/* XBD service task priority */
#ifndef USB2_MSC_SERVICE_TASK_PRIORITY
#define USB2_MSC_SERVICE_TASK_PRIORITY      150
#endif

/* XBD service task options */
#ifndef USB2_MSC_SERVICE_TASK_OPTIONS
#define USB2_MSC_SERVICE_TASK_OPTIONS       0
#endif

/* XBD service task stack size */
#ifndef USB2_MSC_SERVICE_TASK_STACK_SIZE
#define USB2_MSC_SERVICE_TASK_STACK_SIZE    4096
#endif

/* Direct access base name */
#ifndef USB2_MSC_DIRECT_ACCESS_BASE_NAME
#define USB2_MSC_DIRECT_ACCESS_BASE_NAME    "/usb2Msc"
#endif

/*
 * Test log base name; if it is "stdout", will
 * redirect to stdout; if other string, will use it
 * as base name of the log file
 */

#ifndef USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME
#define USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME "stdout"
#endif

/*
 * Filesystem interface base mode, currently only
 * USB2_MSC_FS_INTERFACE_XBD and USB2_MSC_FS_INTERFACE_BLK_DEV
 */
#ifndef USB2_MSC_FS_INTERFACE_MODE
/*
 * vxWorks 6.3 and above changed to use XBD scheme
 */

#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
#define USB2_MSC_FS_INTERFACE_MODE USB2_MSC_FS_INTERFACE_XBD
#else
#define USB2_MSC_FS_INTERFACE_MODE USB2_MSC_FS_INTERFACE_BLK_DEV
#endif
#endif

/* Flag for direct access module */
#ifndef USB2_MSC_DIRECT_ACCESS_ENABLE
#define USB2_MSC_DIRECT_ACCESS_ENABLE           FALSE
#endif

/* Flag for compliance test module */
#ifndef USB2_MSC_COMPLIANCE_ENABLE
#define USB2_MSC_COMPLIANCE_ENABLE              FALSE
#endif

/* Flag for hotplug check */
#ifndef USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE
#define USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE    TRUE
#endif

/* Flag for status check */
#ifndef USB2_MSC_STATUS_CHECK_ENABLE
#define USB2_MSC_STATUS_CHECK_ENABLE            TRUE
#endif

/* Flag for test code */
#ifndef USB2_MSC_TEST_CODE_ENABLE
#define USB2_MSC_TEST_CODE_ENABLE               FALSE
#endif

/* Flag for CB[I] transport support */
#ifndef USB2_MSC_CBI_TRANSPORT_ENABLE
#define USB2_MSC_CBI_TRANSPORT_ENABLE           TRUE
#endif

/* Medium hotplug check interval, used by "XBD" interface module */
#ifndef USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL
#define USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL  2000 /* in ms */
#endif

/* Timeout for device reset */
#ifndef USB2_MSC_RESET_TIMEOUT
#define USB2_MSC_RESET_TIMEOUT                  5000 /* in ms */
#endif

/* Timeout for command transfer, such as CBW */
#ifndef USB2_MSC_COMMAND_TIMEOUT
#define USB2_MSC_COMMAND_TIMEOUT                5000 /* in ms */
#endif

/* Timeout for command transfer, such as CSW */
#ifndef USB2_MSC_STATUS_TIMEOUT
#define USB2_MSC_STATUS_TIMEOUT                 5000 /* in ms */
#endif

#ifndef USB2_MSC_MAX_TRANSFER_BLOCKS
#define USB2_MSC_MAX_TRANSFER_BLOCKS            64   /* in blocks */
#endif /* USB2_MSC_MAX_TRANSFER_BLOCKS */

/*
 * The dosFs has a defect fix that introduced new API,
 * we need to watch out for cases where that fix is not applied!
 * Default to be FALSE, TRUE if that fix is applied!
 * This has to be configured by the customer at compile time,
 * where he knows exactly whether he actually included that fix!
 * This may only apply to VxWorks 5.x.
 */

#ifndef USB2_MSC_DOSFS_FIX_WIND00173601
#define USB2_MSC_DOSFS_FIX_WIND00173601         FALSE
#endif

#define USB2_MSC_QUIRKY_DEV_SUPPORT_NONE        0
#define USB2_MSC_QUIRKY_DEV_SUPPORT_RELAXED     1
#define USB2_MSC_QUIRKY_DEV_SUPPORT_ALL         2

#ifndef USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL
#define USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL       USB2_MSC_QUIRKY_DEV_SUPPORT_RELAXED
#endif

/* default quirky flags */
#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
#if (USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL == USB2_MSC_QUIRKY_DEV_SUPPORT_NONE)
#define USB2_MSC_QUIRKY_FLAG                    USB2_MSC_DEV_FLAG_STRICT
#elif (USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL == USB2_MSC_QUIRKY_DEV_SUPPORT_ALL)
#define USB2_MSC_QUIRKY_FLAG                    USB2_MSC_DEV_FLAG_ALL
#else
#define USB2_MSC_QUIRKY_FLAG                    USB2_MSC_DEV_FLAG_RELAXED
#endif 
#endif

/* Configurable options */

typedef struct usrUsb2MscConfig
    {
    UINT8 maxDevices;
    UINT8 maxDrives;
    int filesystemInterfaceMode;
    char complianceLogFileBaseName[USB2_MSC_MAX_NAME_SZ];
    BOOL directAccessEnable;
    BOOL complianceEnable;
    BOOL mediumHotplugCheckEnable;
    int  mediumHotplugCheckInterval;
    BOOL statusCheckEnable;
    int resetTimeout;
    int commandTimeout;
    int statusTimeout;
    int maxBlocksPerTransfer;
    int svrTaskPriority;
    int svrTaskOptions;
    int svrTaskStackSize;
    BOOL CBITransportEnable;
    }USR_USB2_MSC_CONFIG;

/* Multiple mass storage device support type */

typedef struct usrUsb2MscDevice
    {
    char              drvName[USB2_MSC_MAX_DRIVES][USB2_MSC_MAX_NAME_SZ];
    char              svrName[USB2_MSC_MAX_DRIVES][USB2_MSC_MAX_NAME_SZ];
    UINT32            hDevice;
    /* vxWorks 5.x till vxWorks 6.2 still use CBIO interface */

#if (_WRS_VXWORKS_MAJOR == 5) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR <= 2))
    CBIO_DEV_ID       drvCbio[USB2_MSC_MAX_DRIVES];
    CBIO_DEV_ID       drvMasterCbio[USB2_MSC_MAX_DRIVES];
#endif
    } USR_USB2_MSC_DEVICE;

/* globals */

IMPORT STATUS usrUsb2HelperInit();

/* global quirky flag */
IMPORT UINT32 uUsb2MscQuirkyFlag;

IMPORT device_t devGetByName(char *deviceName);

/* User provided config options */

LOCAL USR_USB2_MSC_CONFIG usrUsb2MscConfiguration =
    {
     USB2_MSC_MAX_DEVICES,                      /* maxDevices                 */
     USB2_MSC_MAX_DRIVES,                       /* maxDrives                  */
     USB2_MSC_FS_INTERFACE_MODE,                /* filesystemInterfaceMode    */
     USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME,    /* complianceLogFileName      */
     USB2_MSC_DIRECT_ACCESS_ENABLE,             /* directAccessEnable         */
     USB2_MSC_COMPLIANCE_ENABLE,                /* complianceEnable           */
     USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE,      /* mediumHotplugCheckEnable   */
     USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL,    /* mediumHotplugCheckInterval */
     USB2_MSC_STATUS_CHECK_ENABLE,              /* statusCheckEnable          */
     USB2_MSC_RESET_TIMEOUT,                    /* resetTimeout               */
     USB2_MSC_COMMAND_TIMEOUT,                  /* commandTimeout             */
     USB2_MSC_STATUS_TIMEOUT,                   /* statusTimeout              */
     USB2_MSC_MAX_TRANSFER_BLOCKS,              /* maxBlocksPerTransfer       */
     USB2_MSC_SERVICE_TASK_PRIORITY,            /* svrTaskPriority            */
     USB2_MSC_SERVICE_TASK_OPTIONS,             /* svrTaskOptions             */
     USB2_MSC_SERVICE_TASK_STACK_SIZE,          /* svrTaskStackSize           */
     USB2_MSC_CBI_TRANSPORT_ENABLE              /* CBITransportEnable         */
    };

LOCAL USR_USB2_MSC_DEVICE  usrUsb2MscDeviceTbl[USB2_MSC_MAX_DEVICES];

/* User configurable quirky device table */

LOCAL USB2_MSC_QUIRKY_INFO usrUsb2MscQuirkyDevices[] =
    {
    /*
     * EAGET CM981 reports VID/PID/REV all as zero,
     * but it works fine without any flags; This entry
     * is just to log this device as an demonstration for
     * quirky device entry usage.
     */

    USB2_MSC_QUIRKY(0x0000,0x0000,0x0000,0x0000,
        "EAGET CM981/E981HL 4GB",
        USB2_MSC_SUBCLASS_RESERVED,
        USB2_MSC_PROTOCOL_RESERVED,
        NULL,NULL,NULL,
        0
        )
    };

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT

/*******************************************************************************
*
* usrUsb2MscQuirkyInit - Init quirky device list
*
* This routine call usb2MscQuirkyAdd() to add all quirky devices defined by user
* to driver quirky device list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void usrUsb2MscQuirkyInit
    (
    void
    )
    {
    usb2MscQuirkyAdd(usrUsb2MscQuirkyDevices,
                     NELEMENTS(usrUsb2MscQuirkyDevices));
    }

/*******************************************************************************
*
* usrUsb2MscQuirkyUnInit - Clear quirky device list
*
* This routine call usb2MscQuirkyDel() to remove all quirky devices defined by
* user from driver quirky device list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void usrUsb2MscQuirkyUnInit
    (
    void
    )
    {
    int i;

    for (i = 0; i < NELEMENTS(usrUsb2MscQuirkyDevices); i++)
        {
        usb2MscQuirkyDel(&usrUsb2MscQuirkyDevices[i]);
        }
    }
#endif

/*******************************************************************************
*
* usrUsb2MscFilesystemInterfaceGet - get user selected filesystem interface
*
* This routine is to get user selected filesystem interface.
*
* RETURNS: Identifier of the filesystem interface
*
* ERRNO: N/A
*
* \NOMANUAL
*/

UINT8 usrUsb2MscFilesystemInterfaceGet
    (
    void
    )
    {
    if (usrUsb2MscConfiguration.filesystemInterfaceMode
        == USB2_MSC_FS_INTERFACE_XBD)
        {
        return USB2_MSC_FS_INTERFACE_XBD;
        }
    else if (usrUsb2MscConfiguration.filesystemInterfaceMode
        == USB2_MSC_FS_INTERFACE_BLK_DEV)
        {
        return USB2_MSC_FS_INTERFACE_BLK_DEV;
        }
    else
        {
        return USB2_MSC_FS_INTERFACE_DEFAULT;
        }
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessEnabled - get enable flag for direct access module
*
* This routine is to get user configured flag to enable or disable
* direct access module.
*
* RETURNS: Boolean flag to enable or disable direct access module
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscDirectAccessEnabled
    (
    void
    )
    {
    return usrUsb2MscConfiguration.directAccessEnable;
    }

/*******************************************************************************
*
* usrUsb2MscComplianceEnabled - get enable flag for compliance test module
*
* This routine is to get user configured flag to enable or disable
* compliance test module.
*
* RETURNS: Boolean flag to enable or disable compliance test module
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscComplianceEnabled
    (
    void
    )
    {
    return usrUsb2MscConfiguration.complianceEnable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckEabled - get enable flag for medium hotplug check
*
* This routine is to get user configured flag to enable or disable
* medium hotplug check.
*
* RETURNS: Boolean flag to enable or disable medium hotplug check
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscMediumHotplugCheckEabled
    (
    void
    )
    {
    return usrUsb2MscConfiguration.mediumHotplugCheckEnable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckEabled - get enable flag for status check
*
* This routine is to get user configured flag to enable or disable
* status check.
*
* RETURNS: Boolean flag to enable or disable status check
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscStatusCheckEnabled
    (
    void
    )
    {
    return usrUsb2MscConfiguration.statusCheckEnable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckIntervalGet - get interval for medium hotplug check
*
* This routine is to get user configured interval in ms for
* medium hotplug check.
*
* RETURNS: Interval value in ms for medium hotplug check
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscMediumHotplugCheckIntervalGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.mediumHotplugCheckInterval;
    }

/*******************************************************************************
*
* usrUsb2MscResetTimeoutGet - get reset timeout value (in ms)
*
* This routine is to get reset timeout value (in ms)
*
* RETURNS: reset timeout value (in ms)
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscResetTimeoutGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.resetTimeout;
    }

/*******************************************************************************
*
* usrUsb2MscCommandTimeoutGet - get command timeout value (in ms)
*
* This routine is to get command timeout value (in ms)
*
* RETURNS: command timeout value (in ms)
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscCommandTimeoutGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.commandTimeout;
    }

/*******************************************************************************
*
* usrUsb2MscStatusTimeoutGet - get status timeout value (in ms)
*
* This routine is to get status timeout value (in ms)
*
* RETURNS: status timeout value (in ms)
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscStatusTimeoutGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.statusTimeout;
    }

/*******************************************************************************
*
* usrUsb2MscMaxBlocksPerTransferGet - get max blocks per transfer
*
* This routine is to get max blocks per transfer.
*
* RETURNS: status timeout value (in ms)
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscMaxBlocksPerTransferGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.maxBlocksPerTransfer;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskPriorityGet - get XBD service task priority
*
* This routine is to get XBD service task priority
*
* RETURNS: task priority of XBD service task
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscServiceTaskPriorityGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.svrTaskPriority;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskOptionsGet - get XBD service task options
*
* This routine is to get XBD service task options
*
* RETURNS: task options of XBD service task
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscServiceTaskOptionsGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.svrTaskOptions;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskStackSizeGet - get XBD service task stack size
*
* This routine is to get XBD service task stack size.
*
* RETURNS: task stack size of XBD service task
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscServiceTaskStackSizeGet
    (
    void
    )
    {
    return usrUsb2MscConfiguration.svrTaskStackSize;
    }

/*******************************************************************************
*
* usrUsb2MscCBItransportEnabled - get enable flag for CBI transfer support
*
* This routine is to get user configured flag to enable or disable
* CBI transfer support.
*
* RETURNS: Boolean flag of enable or disable status check
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscCBItransportEnabled
    (
    void
    )
    {
    return usrUsb2MscConfiguration.CBITransportEnable;
    }

#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
/*******************************************************************************
*
* usrUsb2MscFilesystemInterfaceSet - set user selected filesystem interface
*
* This routine is to set user selected filesystem interface.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscFilesystemInterfaceSet
    (
    int mode
    )
    {
    /* Right now we only support "XBD" and "BLK_DEV" */

    if (mode == USB2_MSC_FS_INTERFACE_XBD
        || mode == USB2_MSC_FS_INTERFACE_BLK_DEV)
        {
        usrUsb2MscConfiguration.filesystemInterfaceMode = mode;
        }
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessEnabledSet - set enable flag for direct access module
*
* This routine is to set user configured flag to enable or disable
* direct access module.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscDirectAccessEnabledSet
    (
    BOOL enable
    )
    {
    /* Don't allow other values */
    if ((enable == TRUE) || (enable == FALSE))
        usrUsb2MscConfiguration.directAccessEnable = enable;
    }

/*******************************************************************************
*
* usrUsb2MscComplianceEnabledSet - set enable flag for compliance test module
*
* This routine is to set user configured flag to enable or disable
* compliance test module.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscComplianceEnabledSet
    (
    BOOL enable
    )
    {
    /* Don't allow other values */
    if ((enable == TRUE) || (enable == FALSE))
        usrUsb2MscConfiguration.complianceEnable = enable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckEabledSet - set enable flag for medium hotplug check
*
* This routine is to set user configured flag to enable or disable
* medium hotplug check.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscMediumHotplugCheckEabledSet
    (
    BOOL enable
    )
    {
    /* Don't allow other values */
    if ((enable == TRUE) || (enable == FALSE))
        usrUsb2MscConfiguration.mediumHotplugCheckEnable = enable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckEabledSet - set enable flag for status check
*
* This routine is to set user configured flag to enable or disable
* status check.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscStatusCheckEnabledSet
    (
    BOOL enable
    )
    {
    /* Don't allow other values */
    if ((enable == TRUE) || (enable == FALSE))
        usrUsb2MscConfiguration.statusCheckEnable = enable;
    }

/*******************************************************************************
*
* usrUsb2MscMediumHotplugCheckIntervalSet - set interval for medium hotplug check
*
* This routine is to set user configured interval in ms for
* medium hotplug check.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscMediumHotplugCheckIntervalSet
    (
    int interval
    )
    {
    /* Only allow positive value */
    if (interval > 0)
        usrUsb2MscConfiguration.mediumHotplugCheckInterval = interval;
    }

/*******************************************************************************
*
* usrUsb2MscResetTimeoutSet - set reset timeout value (in ms)
*
* This routine is to set reset timeout value (in ms)
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscResetTimeoutSet
    (
    int timeout
    )
    {
    usrUsb2MscConfiguration.resetTimeout = timeout;
    }

/*******************************************************************************
*
* usrUsb2MscResetTimeoutSet - set command timeout value (in ms)
*
* This routine is to set command timeout value (in ms)
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscCommandTimeoutSet
    (
    int timeout
    )
    {
    usrUsb2MscConfiguration.commandTimeout = timeout;
    }

/*******************************************************************************
*
* usrUsb2MscStatusTimeoutSet - set status timeout value (in ms)
*
* This routine is to set status timeout value (in ms)
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscStatusTimeoutSet
    (
    int timeout
    )
    {
    usrUsb2MscConfiguration.statusTimeout = timeout;
    }

/*******************************************************************************
*
* usrUsb2MscMaxBlocksPerTransferSet - set max blocks per transfer
*
* This routine is to set max blocks per transfer.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscMaxBlocksPerTransferSet
    (
    int maxBlocks
    )
    {
    usrUsb2MscConfiguration.maxBlocksPerTransfer = maxBlocks;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskPrioritySet - set XBD service task priority
*
* This routine is to set XBD service task priority.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscServiceTaskPrioritySet
    (
    int priority
    )
    {
    usrUsb2MscConfiguration.svrTaskPriority = priority;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskOptionsSet - set XBD service task options
*
* This routine is to set XBD service task options.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscServiceTaskOptionsSet
    (
    int options
    )
    {
    usrUsb2MscConfiguration.svrTaskOptions = options;
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskStackSizeSet - set XBD service task stack size
*
* This routine is to set XBD service task stack size.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscServiceTaskStackSizeSet
    (
    int stackSize
    )
    {
    usrUsb2MscConfiguration.svrTaskStackSize = stackSize;
    }

/*******************************************************************************
*
* usrUsb2MscCBItransportEnabledSet - set enable flag for CBI transfer support
*
* This routine is to set user configured flag to enable or disable
* CBI transfer support.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscCBItransportEnabledSet
    (
    int stackSize
    )
    {
    usrUsb2MscConfiguration.CBITransportEnable = stackSize;
    }

#endif

/*******************************************************************************
*
* usrUsb2MscNameInstall - install naming mechanism for the MSC driver
*
* This routine is to install the naming mechanism for the MSC driver.
*
* RETURNS: Alays OK.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscNameInstall
    (
    void
    )
    {
    int dev = 0;
    int lun = 0;
    USR_USB2_MSC_DEVICE *pUsrMscDevice;
    char fsmName[USB2_MSC_MAX_NAME_SZ];

    for (dev = 0; dev < USB2_MSC_MAX_DEVICES; dev++ )
        {
        pUsrMscDevice = &usrUsb2MscDeviceTbl[dev];

        /* Clear the structure */

        memset(pUsrMscDevice, 0, sizeof (USR_USB2_MSC_DEVICE));

        for (lun = 0; lun < USB2_MSC_MAX_DRIVES; lun++ )
            {
            /* Set the drive name for this lun on the device */

            snprintf(pUsrMscDevice->drvName[lun],
                     USB2_MSC_MAX_NAME_SZ - 1,
                     "%s%d",
                     USB2_MSC_DRIVE_NAME,
                     lun + (dev * USB2_MSC_MAX_DRIVES));

            snprintf(pUsrMscDevice->svrName[lun],
                     USB2_MSC_MAX_NAME_SZ - 1,
                     "%s%d",
                     USB2_MSC_SERVICE_TASK_NAME,
                     lun + (dev * USB2_MSC_MAX_DRIVES));
/*
 * vxWorks 6.3 and above changed to use XBD scheme,
 * and fsMontitor is also used by the filesystem.
 */

#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 3))

            /* fsm name mapping for the device */

            /* Name mapping for device with partition table */

            snprintf(fsmName, USB2_MSC_MAX_NAME_SZ - 1,
                     "%s:1", pUsrMscDevice->drvName[lun]);
            fsmNameInstall(fsmName, pUsrMscDevice->drvName[lun]);

            /* Name mapping for device without partition table */

            snprintf(fsmName, USB2_MSC_MAX_NAME_SZ - 1,
                     "%s:0", pUsrMscDevice->drvName[lun]);
            fsmNameInstall(fsmName, pUsrMscDevice->drvName[lun]);
#endif
            }
        }
    return OK;
    }

/*******************************************************************************
*
* usrUsb2MscNameUninstall - uninstall naming mechanism for the MSC driver
*
* This routine is to uninstall the naming mechanism for the MSC driver.
*
* RETURNS: Alays OK.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscNameUninstall
    (
    void
    )
    {
    int dev = 0;
    int lun = 0;
    USR_USB2_MSC_DEVICE *pUsrMscDevice;
    char fsmName[USB2_MSC_MAX_NAME_SZ];

    for (dev = 0; dev < USB2_MSC_MAX_DEVICES; dev++ )
        {
        pUsrMscDevice = &usrUsb2MscDeviceTbl[dev];
        for (lun = 0; lun < USB2_MSC_MAX_DRIVES; lun++ )
            {
/*
 * vxWorks 6.3 and above changed to use XBD scheme,
 * and fsMontitor is also used by the filesystem.
 */

#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 3))

            /* fsm name unmapping for the device */

            /* Name unmapping for device with partition table */

            snprintf(fsmName, USB2_MSC_MAX_NAME_SZ - 1,
                     "%s:1", pUsrMscDevice->drvName[lun]);
            fsmNameUninstall(fsmName);

            /* Name unmapping for device without partition table */

            snprintf(fsmName, USB2_MSC_MAX_NAME_SZ - 1,
                     "%s:0", pUsrMscDevice->drvName[lun]);
            fsmNameUninstall(fsmName);
#endif
            }
        }
    return OK;
    }

/*******************************************************************************
*
* usrUsb2MscReserveDevice - reserve the naming entry for the device
*
* This routine is to reserve the naming entry for the device;
* the pointer to the entry is saved on to the 'pUsrContext' pointer;
* The driver code will use the returned pointer as handle to latter calls
* to use the naming entry. This is to ensure driver transparency for
* user provided naming policy.
*
* RETURNS: OK if succseeful, or ERROR if not
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscReserveDevice
    (
    UINT32 hDevice,
    ULONG *pUsrContext
    )
    {
    int dev = 0;
    USR_USB2_MSC_DEVICE *pUsrMscDevice;

    for (dev = 0; dev < USB2_MSC_MAX_DEVICES; dev++ )
        {
        pUsrMscDevice = &usrUsb2MscDeviceTbl[dev];
        if (pUsrMscDevice->hDevice == 0)
            {
            pUsrMscDevice->hDevice = hDevice;
            *pUsrContext = (ULONG)pUsrMscDevice;
            return OK;
            }
        }
    return ERROR;
    }

/*******************************************************************************
*
* usrUsb2MscUnreserveDevice - release the naming entry for the device
*
* This routine is to release the naming entry for the device;
*
* RETURNS: OK if succseeful, or ERROR if not
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscUnreserveDevice
    (
    UINT32 hDevice
    )
    {
    int dev = 0;
    USR_USB2_MSC_DEVICE *pUsrMscDevice;

    for (dev = 0; dev < USB2_MSC_MAX_DEVICES; dev++ )
        {
        pUsrMscDevice = &usrUsb2MscDeviceTbl[dev];
        if (pUsrMscDevice->hDevice == hDevice)
            {
            pUsrMscDevice->hDevice = 0;
            return OK;
            }
        }
    return ERROR;
    }

/*******************************************************************************
*
* usrUsb2MscDrvNameGet - get the drive name the device
*
* This routine is to get the drive name for the LUN of the device.
*
* RETURNS: Pointer to the drive name
*
* ERRNO: N/A
*
* \NOMANUAL
*/

char * usrUsb2MscDrvNameGet
    (
    ULONG hUsrContext,
    UINT8 lun
    )
    {
    USR_USB2_MSC_DEVICE *pUsrMscDevice = (USR_USB2_MSC_DEVICE *)hUsrContext;

    /* The caller ensures this entry is not NULL */

    return pUsrMscDevice->drvName[lun];
    }

/*******************************************************************************
*
* usrUsb2MscServiceTaskNameGet - get XBD service task name for the device
*
* This routine is to get XBD service task name for the device.
*
* RETURNS: Pointer to the XBD service task name
*
* ERRNO: N/A
*
* \NOMANUAL
*/

char * usrUsb2MscServiceTaskNameGet
    (
    ULONG hUsrContext,
    UINT8 lun
    )
    {
    USR_USB2_MSC_DEVICE *pUsrMscDevice = (USR_USB2_MSC_DEVICE *)hUsrContext;

    /* The caller ensures this entry is not NULL */

    return pUsrMscDevice->svrName[lun];
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTestLogFileOpen - get compliance test log file
*
* This routine is to get compliance test log file. A pointer to FILE
* is returned. If the user fills "stdout" in the configuration facility,
* then we will map the file to stdout; If the user fills other string,
* we will use the string as a base to generate a random file name starting
* with that base; then open that file and return the FILE pointer.
*
* RETURNS: Pointer to the test log file
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FILE * usrUsb2MscComplianceTestLogFileOpen
    (
    UINT32 hDevice
    )
    {
    FILE *  pFile = NULL;
    char    logFileName[USB2_MSC_MAX_NAME_SZ];

    /*
     * If user filled "stdout" or does not select the configuration,
     * default to be stdio;
     */

    if (!strcmp(usrUsb2MscConfiguration.complianceLogFileBaseName,
        "stdout"))
        {
        /*
         * Set buffering for stdout to "no buffering" so screen
         * updates will be timely.
         */

        setvbuf (stdout, NULL, _IONBF, 0);

        return stdout;
        }
    else
        {
        snprintf(logFileName, USB2_MSC_MAX_NAME_SZ - 1, "%s%d%d.txt",
            usrUsb2MscConfiguration.complianceLogFileBaseName,
            hDevice,
            rand());

        printf(" Try to open test log file %s\n",logFileName);

        /* Open the file for logging */

        pFile = fopen(logFileName, "w+");

        /* Check if we opened the file */

        if (!pFile)
            {
            printf("Failed to open log file %s\n", logFileName);
            }
        else
            {
            printf("Successed to open log file %s\n", logFileName);
            }

        return pFile;
        }
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTestLogFileClose - close compliance test log file
*
* This routine is to close compliance test log file. If the user has
* selected "stdout" in the configuration facility, then we do not close it;
* Otherwise we close the FILE pointer.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscComplianceTestLogFileClose
    (
    FILE * file
    )
    {
    if (file != stdout)
        {
        fclose(file);
        }
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTestLogFileBaseNameSet - set base name of compliance
* test log file
*
* This routine is to set compliance test log file base name.
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscComplianceTestLogFileBaseNameSet
    (
    const char *name
    )
    {
    if (strlen(name) <= USB2_MSC_MAX_NAME_SZ)
        strncpy(usrUsb2MscConfiguration.complianceLogFileBaseName, 
                name, USB2_MSC_MAX_NAME_SZ - 1);
    }

/*******************************************************************************
*
* usrUsb2MscDefaultMediumChangeCallback - defaulut medium change callback
*
* This routine is a defalut medium change callback handler; It stands
* for demonstration purpose, it does nothing but just to return TRUE to
* allow normal driver medium change hotplug handling. The end user can
* override this routine to do what he wants to do with the device, and
* finally return a BOOL value to tell the driver if he wants to use the
* normal driver hotplug mechanism to run. The end user may also use the
* direct accress module ioctl code to overide a per device medium change
* callback handler. See the MSC class driver direct access module for
* more details.
*
* RETURNS: TRUE allow driver normal medium hotplug handling, FALSE otherwise
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usrUsb2MscDefaultMediumChangeCallback
    (
    UINT32 hDevice,         /* Device handle for the device      */
    UINT8 lun,              /* LUN of the medium change          */
    BOOL  mediumInsert      /* Is this a medium insert or remove */
    )
    {
    /*
     * The following code is used to demostrate a user usage of this media
     * change callback
     */
#if 0
    if (mediumInsert)
        printf("NOTIFY: hDevice 0x%X lun %d medium changed (mediumInsert = 1)\n",
                hDevice, lun);
    else
        printf("NOTIFY: hDevice 0x%X lun %d medium changed (mediumInsert = 0)\n",
                hDevice, lun);
    /*
     * Default to do nothing but just return TRUE to
     * allow normal driver medium change hotplug handling.
     */
#endif
    return TRUE;
    }

/*
 * The CBIO model code below will need to access USR_USB2_MSC_DEVICE
 * structure to allow dcache resize mechanism, and USR_USB2_MSC_DEVICE
 * is intended to be only visiable to this module, thus we put the
 * CBIO mount/unmount code here.
 */

#if (_WRS_VXWORKS_MAJOR == 5) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR <= 2))

/*******************************************************************************
*
* usrUsb2MscCbioDevMount - mount the device based on CBIO model
*
* This routine is to mount the device based on CBIO model. The dcache
* resize mechanism requires us to put the drvCbio and drvMasterCbio
* into a static array, so we choose to put this code along with the
* static naming structure array.
*
* RETURNS: OK or ERROR if failed to mount the device.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscCbioDevMount
    (
    ULONG         hUsrContext,
    UINT8         lun,
    BLK_DEV *     pBlkDev,       /* pointer to block device */
    const char *  pName          /* pointer to device name  */
    )
    {
    USR_USB2_MSC_DEVICE *pUsrMscDevice = (USR_USB2_MSC_DEVICE *)hUsrContext;
    int memSize = min( memFindMax()/16, 188 * 1024 ) ;
    STATUS status = ERROR;

    /* Optional dcache */

    if (pUsrMscDevice->drvCbio[lun] == NULL &&
        pUsrMscDevice->drvMasterCbio[lun] == NULL)
        {
        /* Store the new CBIO */

        pUsrMscDevice->drvCbio[lun] = dcacheDevCreate (
                                (CBIO_DEV_ID)pBlkDev,
                                0, memSize, "usb2MscCache");

        if (NULL == pUsrMscDevice->drvCbio[lun])
            {
            /* insufficient memory, will avoid the cache */

            printf ("WARNING: Failed to create disk cache\n");

            return ERROR;
            }

        /* Store BLK dev pointer */

        pUsrMscDevice->drvMasterCbio[lun] = dpartDevCreate (
                                pUsrMscDevice->drvCbio[lun],
                                1, usrFdiskPartRead);

        if (NULL == pUsrMscDevice->drvMasterCbio[lun])
            {
            printf ("Error creating partition manager\n");

#if (USB2_MSC_DOSFS_FIX_WIND00173601 == TRUE)
            dcacheDevDestroy(pUsrMscDevice->drvCbio[lun]);

            pUsrMscDevice->drvCbio[lun] = NULL;
#endif
            return ERROR;
            }
        }
    else
        {
        if (cbioIoctl (pUsrMscDevice->drvCbio[lun],
            (int) CBIO_RESET,
            (addr_t)pBlkDev) != OK)
            {
            printErr("Error substituting block driver, errno=#0x%x\n", errno);
            }

        /* Resize cache and RESET device cache */

        dcacheDevMemResize (pUsrMscDevice->drvCbio[lun], memSize);
        }

    /* Create the dosFs */

    status = dosFsDevCreate (pName,
                        dpartPartGet(pUsrMscDevice->drvMasterCbio[lun], 0),
                        0x20,
                       (u_int) NONE);

    /* Mount the drive to DOSFS */

    if (status != OK)
        {
        printf ("Error creating dosFs device\n");

#if (USB2_MSC_DOSFS_FIX_WIND00173601 == TRUE)

        dpartDevDestroy(pUsrMscDevice->drvMasterCbio[lun]);

        dcacheDevDestroy(pUsrMscDevice->drvCbio[lun]);

        pUsrMscDevice->drvCbio[lun] = NULL;

        pUsrMscDevice->drvMasterCbio[lun] = NULL;

#endif /* USB2_MSC_DOSFS_FIX_WIND00173601 */
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usrUsb2MscCbioDevUnmount - unmount the device based on CBIO model
*
* This routine is to unmount the device based on CBIO model.
*
* RETURNS: OK or ERROR if failed to unmount the device.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscCbioDevUnmount
    (
    ULONG         hUsrContext,
    UINT8         lun,
    const char *  pName          /* pointer to device name  */
    )
    {
    USR_USB2_MSC_DEVICE * pUsrMscDevice = (USR_USB2_MSC_DEVICE *)hUsrContext;
    DEV_HDR *hdr;
    char    *pDummy;

    if (!pUsrMscDevice)
        return ERROR;

    /* Unmount file system */

    dosFsVolUnmount(pUsrMscDevice->drvName[lun]);

#if (USB2_MSC_DOSFS_FIX_WIND00173601 == TRUE)

    /* Remove the file system device */

    dosFsDevDestroy(pUsrMscDevice->drvName[lun]);

    /* Free the partition device */

    dpartDevDestroy(pUsrMscDevice->drvMasterCbio[lun]);

    /* Free the disk cache CBIO device */

    dcacheDevDestroy(pUsrMscDevice->drvCbio[lun]);

    pUsrMscDevice->drvCbio[lun] = NULL;

    pUsrMscDevice->drvMasterCbio[lun] = NULL;
#else
    /* Remove the dosFs handles if the device is not being used */

    if ((hdr = iosDevFind (pName, (char **) &pDummy)) != NULL)
        {
        printf("iosDevDelete %s\n",pName);

        iosDevDelete (hdr);
        }

#endif /* USB2_MSC_DOSFS_FIX_WIND00173601 */

    printf("Device %s removed and uninstalled from FS\n",
           pUsrMscDevice->drvName[lun]);

    return OK;
    }
#endif

/*******************************************************************************
*
* usrUsb2MscDeviceMount - mounting select routine
*
* This routine selects a mounting function and call it. This selection depends 
* on user configuration and compile parameter.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller.
*
* RETURNS: OK or ERROR if failed to mount the device.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDeviceMount
    (
    USB2_MSC_LUN_DEVICE * pMscLunDevice
    )
    {
    if (pMscLunDevice == NULL)
        return ERROR;

#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    if (usrUsb2MscFilesystemInterfaceGet() ==
        USB2_MSC_FS_INTERFACE_XBD)
        {
        /*
        * vxWorks 6.3 and above changed to use XBD scheme
        */

#if (_WRS_VXWORKS_MAJOR > 6) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
        return usb2MscXbdMount(pMscLunDevice);
#else
        return ERROR;
#endif
        }
    else
        {
        return usb2MscBlkDevMount(pMscLunDevice);
        }
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_FS_INTERFACE_MODE == USB2_MSC_FS_INTERFACE_XBD)

#if (_WRS_VXWORKS_MAJOR > 6) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
    return usb2MscXbdMount(pMscLunDevice);
#else
    return ERROR;
#endif
#else
    return usb2MscBlkDevMount(pMscLunDevice);
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscDeviceUnmount - unmounting select routine
*
* This routine selects a unmounting function and call it. This selection depends
* on user configuration and compile parameter.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller.
*
* RETURNS: OK or ERROR if failed to unmount the device.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDeviceUnmount
    (
    USB2_MSC_LUN_DEVICE * pMscLunDevice
    )
    {
    if (pMscLunDevice == NULL)
        return ERROR;

#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    if (usrUsb2MscFilesystemInterfaceGet() == USB2_MSC_FS_INTERFACE_XBD)
        {
        /*
         * vxWorks 6.3 and above changed to use XBD scheme
         */

#if (_WRS_VXWORKS_MAJOR > 6) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
        return usb2MscXbdUnmount(pMscLunDevice);
#else
        return ERROR;
#endif
        }
    else
        {
        return usb2MscBlkDevUnmount(pMscLunDevice);
        }

#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_FS_INTERFACE_MODE == USB2_MSC_FS_INTERFACE_XBD)

#if (_WRS_VXWORKS_MAJOR > 6) || \
    ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR > 2))
    return usb2MscXbdUnmount(pMscLunDevice);
#else
    return ERROR;
#endif
#else
    return usb2MscBlkDevUnmount(pMscLunDevice);
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessInit - initialize the direct access module
*
* This routine calls real initialization function of direct access module when
* user add direct access module or allow to support direct access module.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* direct access module is not used.
*
* RETURNS: OK or ERROR if initialize failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDirectAccessInit
    (
    void
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscDirectAccessInit();
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_DIRECT_ACCESS_ENABLE == TRUE)
    return usb2MscDirectAccessInit();
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessUnInit - uninitialize the direct access module
*
* This routine calls real uninitialization function of direct access module when
* user add direct access module or allow to support direct access module.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* direct access module is not used.
*
* RETURNS: OK or ERROR if uninitialize failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDirectAccessUnInit
    (
    void
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscDirectAccessUnInit();
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_DIRECT_ACCESS_ENABLE == TRUE)
    return usb2MscDirectAccessUnInit();
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessDeviceAdd - add support of direct access to device
*
* This routine calls real function of adding support of direct access when 
* device attached and user allow to support direct access module.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* direct access module is not used.
*
* RETURNS: OK or ERROR if operation failed
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDirectAccessDeviceAdd
    (
    USB2_MSC_DEVICE *               pMscDevice
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscDirectAccessDeviceAdd(pMscDevice);
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_DIRECT_ACCESS_ENABLE == TRUE)
    return usb2MscDirectAccessDeviceAdd(pMscDevice);
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscDirectAccessDeviceRemove - remove support of direct access to device
*
* This routine calls real function of removing support of direct access when
* device attached and user allow to support direct access module.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* direct access module is not used.
*
* RETURNS: OK or ERROR if operation failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDirectAccessDeviceRemove
    (
    USB2_MSC_DEVICE *               pMscDevice
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscDirectAccessDeviceRemove(pMscDevice);
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_DIRECT_ACCESS_ENABLE == TRUE)
    return usb2MscDirectAccessDeviceRemove(pMscDevice);
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTestDeviceAdd - add a device LUN for test
*
* This routine calls real adding function when a user request a device for
* complaince test.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* compliance test module is not used.
*
* RETURNS: OK or ERROR if operation failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscComplianceTestDeviceAdd
    (
    USB2_MSC_DEVICE *       pMscDevice,
    UINT8                   lun
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscComplianceTestDeviceAdd(pMscDevice, lun);
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_COMPLIANCE_ENABLE == TRUE)
    return usb2MscComplianceTestDeviceAdd(pMscDevice, lun);
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTestDeviceRemove - remove a device LUN from test
*
* This routine calls real removing function when a user request remove a device
* for complaince test.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* compliance test module is not used.
*
* RETURNS: OK or ERROR if operation failed.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscComplianceTestDeviceRemove
    (
    USB2_MSC_DEVICE *       pMscDevice,
    UINT8                   lun
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscComplianceTestDeviceRemove(pMscDevice, lun);
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_COMPLIANCE_ENABLE == TRUE)
    return usb2MscComplianceTestDeviceRemove(pMscDevice, lun);
#else
    return ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscCBITransport - perform the CB[I] transport protocol
*
* This routine calls real transport function for a CB[I] transport request.
*
* Set USB2_MSC_DYNAMIC_CONFIG to be FALSE can make object file size smaller if
* CB[I] transport module is not used.
*
* RETURNS: USB2_MSC_TRANSPORT_STATUS of the transport result.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

USB2_MSC_TRANSPORT_STATUS usrUsb2MscCBITransport
    (
    USB2_MSC_DEVICE *               pMscDevice,
    USB2_MSC_SCSI_COMMAND_INFO *    pCmdInfo,
    USB2_MSC_DATA_TRANSFER_INFO *   pDataInfo
    )
    {
#if (USB2_MSC_DYNAMIC_CONFIG == TRUE)
    return usb2MscCBITransport(pMscDevice, pCmdInfo, pDataInfo);
#else /* USB2_MSC_DYNAMIC_CONFIG == FALSE */
#if (USB2_MSC_CBI_TRANSPORT_ENABLE == TRUE)
    return usb2MscCBITransport(pMscDevice, pCmdInfo, pDataInfo);
#else
    return USB2_MSC_TRANSPORT_ERROR;
#endif
#endif
    }

/*******************************************************************************
*
* usrUsb2MscInit - initialize the MSC class driver
*
* This routine is to initialize the MSC class driver.
*
* RETURNS: OK or ERROR if initialize the MSC class driver.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscInit
    (
    void
    )
    {
    STATUS status = ERROR;

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
    uUsb2MscQuirkyFlag = USB2_MSC_QUIRKY_FLAG;
#endif

    usrUsb2HelperInit();

    status = usrUsb2MscNameInstall();

    if (status == OK)
        {
        status = usb2MscDrvInit(USB2_MSC_DIRECT_ACCESS_BASE_NAME);
        }

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
    usrUsb2MscQuirkyInit();
#endif

    return status;
    }

/*******************************************************************************
*
* usrUsb2MscInit - uninitialize the MSC class driver
*
* This routine is to uninitialize the MSC class driver.
*
* RETURNS: OK or ERROR if uninitialize the MSC class driver.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int usrUsb2MscUninit
    (
    void
    )
    {
    STATUS status = ERROR;

    status = usb2MscDrvUnInit();

    if (status == OK)
        {
        status = usrUsb2MscNameUninstall();
        }

#ifdef USB2_MSC_QUIRKY_DEV_SUPPORT
    usrUsb2MscQuirkyUnInit();
#endif

    return status;
    }

#if (USB2_MSC_TEST_CODE_ENABLE == TRUE)

/*******************************************************************************
*
* usrUsb2MscDumpQuirkyDevices - dump quirky device info list
*
* This routine is to dump quirky device info list
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usrUsb2MscDumpQuirkyDevices
    (
    void
    )
    {
    USB2_MSC_QUIRKY_INFO *pQuirky;
    int count = 0;
    for (count = 0; count < NELEMENTS(usrUsb2MscQuirkyDevices); count++)
        {
        pQuirky = &usrUsb2MscQuirkyDevices[count];
        printf("Quirky info : \n");
        printf("vendorId = 0x%X "       \
               "productId = 0x%X "      \
               "bcdDeviceLow = 0x%X "   \
               "bcdDeviceHigh = 0x%X "  \
               "deviceName = %s "       \
               "subclass = 0x%X "       \
               "protocol = 0x%X "       \
               "setupRoutine = %p "     \
               "mountRoutine = %p "     \
               "unmountRoutine = %p "   \
               "flags = 0x%X \n",
               pQuirky->devId.uVendorID,
               pQuirky->devId.uProductID,
               pQuirky->devId.uDriverBCDLow,
               pQuirky->devId.uDriverBCDHigh,
               pQuirky->deviceName,
               pQuirky->forceSubclass,
               pQuirky->forceProtocol,
               pQuirky->funcs.setup,
               pQuirky->funcs.mount,
               pQuirky->funcs.unmount,
               pQuirky->quirkyFlag
               );
        }
    return;
    }

/*******************************************************************************
*
* userUsb2MscPrintBuffer - print a buffer as hex code
*
* This routine is to print a buffer as hex code
*
* RETURNS: NONE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void userUsb2MscPrintBuffer
    (
    UINT8 *pBuffer,
    int    size
    )
    {
    int col = 0;
    int row = 0;
    int index = 0;
    int width = 0;
    #define WIDTH_PER_LINE 32

    if (size <= 0) return;

    row = size / WIDTH_PER_LINE + 1;

    printf("\n");

    for (index = 0; index < row; index++)
        {
        if (size <= 0) break;

        width = ((size % WIDTH_PER_LINE) == 0)?
            WIDTH_PER_LINE : (size % WIDTH_PER_LINE);

        size -= width;

        for (col = 0; col < width; col++)
            printf(" %2x", pBuffer[col + (32 * index)]);

        printf("\n");
        }

    printf("\n");

    return;
    }

/*******************************************************************************
*
* usrUsb2MscComplianceTest - compliance test code entry point
*
* This routine is compliance test code entry point; Call it like
*
* usrUsb2MscComplianceTest("/usb2Msc0", 0)
*
* Where 'pDevieName' is the device direct access name, and 'lun' is the
* Logic Unit Number of the device that you want to test.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscComplianceTest
    (
    const char *pDevieName,
    UINT8       lun
    )
    {
    int fd = -1;

    fd = open(pDevieName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pDevieName, fd);

    if (fd < 0)
        return ERROR;

    /* Start compliance testing for LUN 0 */

    ioctl(fd, USB2_MSC_DA_START_COMPLIANCE_TEST, lun);

    close(fd);

    printf("\r\nDone.\r\n");

    return OK;
    }

#define MEMM_ALLOC_ALIGN_SIZE (0x100000)

IMPORT PART_ID memSysPartId;

unsigned long sysMemFreeSizeGet( void )
    {
    MEM_PART_STATS	partStats;
    STATUS stat;
        
    stat = memPartInfoGet (memSysPartId, &partStats);

    if (stat != OK)
        return -1UL;

    return partStats.numBytesFree;
    }

void printPerf
    (
    unsigned long size, 
    _Vx_ticks_t ticks
    )
    {
    float time = (float)ticks/sysClkRateGet();
    float perf = ((float)(size/0x100000)) / time;
    
    printf("####Total %d bytes in %.3f seconds => %.3f MB/s\n", 
        size, time, perf);
    }

/*******************************************************************************
*
* usrUsb2MscDumpSectorTest - sector dump test code entry point
*
* This routine is sector dump test code entry point; Call it like
*
* usrUsb2MscDumpSectorTest("/usb2Msc0", 0, 1, 2)
*
* Where 'pDevieName' is the device direct access name, and 'lun' is the
* Logic Unit Number of the device that you want to test. 'sectorStart'
* specifies the first sector to read and 'sectorEnd' specifies the end
* last to read.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscDumpSectorTest
    (
    const char *    pDevieName,
    UINT8           lun,
    UINT32          sectorStart,
    UINT32          sectorEnd
    )
    {
    int fd = -1;
    int tryCount = 0 ;
    int ret = -1;
    UINT32 sectorCount = 0;
    UINT8 * pWorkSpace;
    unsigned sectorSize = 0;

    if (sectorEnd < sectorStart)
        {
        printf("sectorEnd smaller than sectorStart, you set them wrong!\n");

        return ERROR;
        }

    sectorCount = sectorEnd - sectorStart + 1;

    fd = open(pDevieName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pDevieName, fd);

    if (fd < 0)
        return ERROR;

    ret = ioctl(fd, USB2_MSC_DA_GET_SECTOR_SIZE, &sectorSize);

    if ((ret != OK) || (sectorSize == 0))
        {
        printf("Wrong getting sector size\r\n");

        close(fd);

        return ERROR;
        }

    pWorkSpace = (UINT8 *)malloc(sectorSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        close(fd);

        return ERROR;
        }

    printf("Read %s lun %d for %d sectors of sector size %d :[s:%d->e:%d]\n",
           pDevieName, lun, sectorCount, sectorSize, sectorStart, sectorEnd);

    for (tryCount = 0; tryCount < sectorCount; tryCount++)
        {
        ret = ioctl(fd, USB2_MSC_DA_SET_READ_OFFSET, sectorStart);

        ret = read(fd, (char*)pWorkSpace, sectorSize);

        printf("Read sector %d returned %d bytes\n", sectorStart, ret);

        if (ret > 0)
            {
            userUsb2MscPrintBuffer(pWorkSpace, ret);
            }

        sectorStart++; /* Go to the next sector */
        }

    free(pWorkSpace);

    close(fd);

    printf("\r\nDone.\r\n");

    return OK;
    }


/*******************************************************************************
*
* usrUsb2PerfDirectReadTest - sector read performance test code entry point
*
* This routine is sector read performance test code entry point; Call it like
*
* usrUsb2PerfDirectReadTest("/usb2Msc0")
*
* or
*
* sp usrUsb2PerfDirectReadTest, "/usb2Msc0"
*
* Where 'pDevieName' is the device direct access name. 
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfDirectReadTest
    (
    const char *    pDevieName
    )
    {
    int fd = -1;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize;
    _Vx_ticks_t start;
    _Vx_ticks_t end;

    uXferSize = sysMemFreeSizeGet()/2;

    if (uXferSize < MEMM_ALLOC_ALIGN_SIZE)
        return ERROR;
    
    if (uXferSize % MEMM_ALLOC_ALIGN_SIZE)
        uXferSize -= uXferSize % MEMM_ALLOC_ALIGN_SIZE;
    
    fd = open(pDevieName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pDevieName, fd);

    if (fd < 0)
        return ERROR;

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        close(fd);

        return ERROR;
        }
    
    printf("\r\nRead %d bytes.\r\n", uXferSize);
    
    ret = ioctl(fd, USB2_MSC_DA_SET_READ_OFFSET, 0);
    start = tickGet();
    printf("Start tick %d\n", start);
    ret = read(fd, (char*)pWorkSpace, uXferSize);
    end = tickGet();
    printf("End tick %d total %d (%d seconds for %d bytes)\n", 
        end, (end - start), ((end - start)/sysClkRateGet()), uXferSize);
        
    free(pWorkSpace);

    close(fd);

    printPerf(uXferSize, (end - start));

    printf("\r\nDone.\r\n");

    return OK;
    }

/*******************************************************************************
*
* usrUsb2PerfFileReadTest - file read performance test code entry point
*
* This routine is file read performance test code entry point; Call it like
*
* usrUsb2PerfFileReadTest("/bd0/file")
*
* or
*
* sp usrUsb2PerfFileReadTest, "/bd0/file"
*
* Where 'pFileName' is the file name to perform read.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfFileReadTest
    (
    const char *    pFileName
    )
    {
    int fd = -1;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize;
    _Vx_ticks_t start;
    _Vx_ticks_t end;

    uXferSize = sysMemFreeSizeGet()/2;

    if (uXferSize < MEMM_ALLOC_ALIGN_SIZE)
        return ERROR;
    
    if (uXferSize % MEMM_ALLOC_ALIGN_SIZE)
        uXferSize -= uXferSize % MEMM_ALLOC_ALIGN_SIZE;

    fd = open(pFileName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pFileName, fd);

    if (fd < 0)
        return ERROR;

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        close(fd);

        return ERROR;
        }
    
    printf("\r\nRead %d bytes.\r\n", uXferSize);
    
    start = tickGet();
    printf("Start tick %d\n", start);
    ret = read(fd, (char*)pWorkSpace, uXferSize);
    end = tickGet();
    printf("End tick %d total %d (%d seconds for %d bytes)\n", 
        end, (end - start), ((end - start)/sysClkRateGet()), uXferSize);
        
    free(pWorkSpace);

    close(fd);

    printPerf(uXferSize, (end - start));

    printf("\r\nDone.\r\n");

    return OK;
    }

/*******************************************************************************
*
* usrUsb2PerfDirectWriteTest - sector write performance test code entry point
*
* This routine is sector write performance test code entry point; Call it like
*
* usrUsb2PerfDirectWriteTest("/usb2Msc0")
*
* or
*
* sp usrUsb2PerfDirectWriteTest, "/usb2Msc0"
*
* Where 'pDevieName' is the device direct access name. 
*
* NOTE: This test is destructive becasue it will write arbitrary data to the
* disk sectors. Please make sure the disk for this testing do not contain 
* important data. You have been warmed! 
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfDirectWriteTest
    (
    const char *    pDevieName,
    UINT8           lun
    )
    {
    int fd = -1;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize;
    _Vx_ticks_t start;
    _Vx_ticks_t end;
    
    uXferSize = sysMemFreeSizeGet()/2;

    if (uXferSize < MEMM_ALLOC_ALIGN_SIZE)
        return ERROR;
    
    if (uXferSize % MEMM_ALLOC_ALIGN_SIZE)
        uXferSize -= uXferSize % MEMM_ALLOC_ALIGN_SIZE;

    fd = open(pDevieName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pDevieName, fd);

    if (fd < 0)
        return ERROR;

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        close(fd);

        return ERROR;
        }
    
    printf("\r\nWritinging...\r\n");
    
    ret = ioctl(fd, USB2_MSC_DA_SET_WRITE_OFFSET, 0);
    start = tickGet();
    
     /* 
      * No need initialize the pWorkSpace space, 
      * since it is only for testing to save time 
      */ 
    
    ret = write(fd, (char*)pWorkSpace, uXferSize);
    if (ret <= 0)
        {
        printf("write 1 failed\n");
        }
    end = tickGet();
    printf("Write Start tick %d\n", start);
    printf("Write End tick %d total %d (%d seconds for %d bytes)\n", 
        end, (end - start), ((end - start)/sysClkRateGet()), ret);

    free(pWorkSpace);

    close(fd);

    printPerf(uXferSize, (end - start));

    printf("\r\nDone.\r\n");

    return OK;
    }

/*******************************************************************************
*
* usrUsb2PerfFileWriteTest - file write performance test code entry point
*
* This routine is file write performance test code entry point; Call it like
*
* usrUsb2PerfFileWriteTest("/bd0/file")
*
* or
*
* sp usrUsb2PerfFileWriteTest, "/bd0/file"
*
* Where 'pFileName' is the file name to write.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfFileWriteTest
    (
    const char *    pFileName
    )
    {
    int fd = -1;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize;
    _Vx_ticks_t start;
    _Vx_ticks_t end;

    uXferSize = sysMemFreeSizeGet()/2;

    if (uXferSize < MEMM_ALLOC_ALIGN_SIZE)
        return ERROR;
    
    if (uXferSize % MEMM_ALLOC_ALIGN_SIZE)
        uXferSize -= uXferSize % MEMM_ALLOC_ALIGN_SIZE;

    fd = open(pFileName, O_RDWR | O_CREAT, 0);

    printf("Open %s returned fd 0x%x\n", pFileName, fd);

    if (fd < 0)
        return ERROR;

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        close(fd);

        return ERROR;
        }
        
    printf("\r\nWriting...\r\n");
    start = tickGet();
    
    /* 
     * No need initialize the pWorkSpace space, 
     * since it is only for testing to save time
     */

    ret = write(fd, (char*)pWorkSpace, uXferSize);
    if (ret <= 0)
        {
        printf("write 1 failed\n");
        }
    close(fd);
    end = tickGet();
    printf("Start tick %d\n", start);
    printf("End tick %d total %d (%d seconds for %d bytes)\n", 
        end, (end - start), ((end - start)/sysClkRateGet()), ret);
        
    free(pWorkSpace);

    printPerf(uXferSize, (end - start));

    printf("\r\nDone.\r\n");

    return OK;
    }

LOCAL void usrUsb2PerfXbdTestBioDone
    (
    struct bio * pBio
    )
    {
    semGive ((SEM_ID) pBio->bio_caller1);
    }

/*******************************************************************************
*
* usrUsb2PerfXbdWriteTest - xbd write performance test code entry point
*
* This routine is xbd write performance test code entry point; Call it like
*
* usrUsb2PerfXbdWriteTest("/bd0")
*
* or
*
* sp usrUsb2PerfXbdWriteTest, "/bd0"
*
* Where 'pDevieName' is the device name to write.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfXbdWriteTest
    (
    char *    pDevieName
    )
    {
    device_t xbdDevice;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize = (0x100000);
    _Vx_ticks_t start;
    _Vx_ticks_t end;
    _Vx_ticks_t total = 0;
    struct bio   bio;
    unsigned           	blkSize;
    int                 error;

    xbdDevice = devGetByName(pDevieName);
    if (xbdDevice == NULLDEV)
        return ERROR;
    
    printf("xbdDevice %d\n", xbdDevice);
    
    error = xbdBlockSize (xbdDevice, &blkSize);
    if (error != OK)
        return (error);

    bzero ((char *) &bio, sizeof (struct bio));

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        return ERROR;
        }

    bio.bio_caller1 = (void *) semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (bio.bio_caller1 == NULL)
    	{
    	bio_free (bio.bio_data);

        free (pWorkSpace);

    	return (ERROR);
    	}

    bio.bio_dev    = xbdDevice;
    bio.bio_blkno  = 0;
    bio.bio_bcount = uXferSize;
    bio.bio_error  = OK;
    bio.bio_flags  = BIO_WRITE;
    bio.bio_done   = (void *) usrUsb2PerfXbdTestBioDone;
    bio.bio_data = pWorkSpace;

    printf("\r\nWriting...\r\n");

    for (tryCount = 0; tryCount < 600; tryCount++)
        {
        start = tickGet();
        #if 1
        xbdIoctl (xbdDevice, XBD_TEST, 0);
        #endif
        error = xbdStrategy (xbdDevice, &bio);
        ret = semTake ((SEM_ID) bio.bio_caller1, WAIT_FOREVER);
        if ((error != 0) || (ret != 0))
            {
            printf("write 1 failed\n");
            }
        
        end = tickGet();

        total += (end - start);
        bio.bio_blkno  += (uXferSize / blkSize);
        }
    printf("total %d (%d seconds for %d bytes)\n", 
        total, (total/sysClkRateGet()), (uXferSize * 600));
        
    free(pWorkSpace);

    printPerf((uXferSize * 600), total);

    printf("\r\nDone.\r\n");

    return OK;
    }

/*******************************************************************************
*
* usrUsb2PerfXbdWriteTest - xbd write performance test code entry point
*
* This routine is xbd write performance test code entry point; Call it like
*
* usrUsb2PerfXbdReadTest("/bd0")
*
* or
*
* sp usrUsb2PerfXbdReadTest, "/bd0"
*
* Where 'pDevieName' is the device name to write.
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2PerfXbdReadTest
    (
    char *    pDevieName
    )
    {
    device_t xbdDevice;
    int tryCount = 0 ;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT32 uXferSize = (0x100000);
    _Vx_ticks_t start;
    _Vx_ticks_t end;
    _Vx_ticks_t total = 0;
    struct bio   bio;
    unsigned           	blkSize;
    int                 error;

    xbdDevice = devGetByName(pDevieName);
    if (xbdDevice == NULLDEV)
        return ERROR;
    
    printf("xbdDevice %d\n", xbdDevice);
    
    error = xbdBlockSize (xbdDevice, &blkSize);
    if (error != OK)
        return (error);

    bzero ((char *) &bio, sizeof (struct bio));

    pWorkSpace = (UINT8 *)malloc(uXferSize);

    if (!pWorkSpace)
        {
        printf("Failed to malloc buffer\n");

        return ERROR;
        }

    bio.bio_caller1 = (void *) semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (bio.bio_caller1 == NULL)
    	{
    	bio_free (bio.bio_data);
        
        free (pWorkSpace);

        return (ERROR);
    	}

    bio.bio_dev    = xbdDevice;
    bio.bio_blkno  = 0;
    bio.bio_bcount = uXferSize;
    bio.bio_error  = OK;
    bio.bio_flags  = BIO_READ;
    bio.bio_done   = (void *) usrUsb2PerfXbdTestBioDone;
    bio.bio_data = pWorkSpace;

    printf("\r\nReading...\r\n");

    for (tryCount = 0; tryCount < 600; tryCount++)
        {
        start = tickGet();
        
        xbdIoctl (xbdDevice, XBD_TEST, 0);
        error = xbdStrategy (xbdDevice, &bio);
        ret = semTake ((SEM_ID) bio.bio_caller1, WAIT_FOREVER);
        if ((error != 0) || (ret != 0))
            {
            printf("write 1 failed\n");
            }
        
        end = tickGet();

        total += (end - start);
        bio.bio_blkno  += (uXferSize / blkSize);
        }
    printf("total %d (%d seconds for %d bytes)\n", 
        total, (total/sysClkRateGet()), (uXferSize * 600));
        
    free(pWorkSpace);

    printPerf((uXferSize * 600), total);

    printf("\r\nDone.\r\n");

    return OK;
    }

/*******************************************************************************
*
* usrUsb2MscScsiPassThroughTest - SCSI pass through test code entry point
*
* This routine is SCSI pass through test code entry point; Call it like
*
* usrUsb2MscScsiPassThroughTest("/usb2Msc0")
*
* RETURNS: OK, or ERROR when anything fails.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS usrUsb2MscScsiPassThroughTest
    (
    const char *pDevieName
    )
    {
    int fd = -1;
    int ret = -1;
    UINT8 * pWorkSpace;
    UINT8 * pWorkSpace2;
    USB2_MSC_SCSI_PASS_THROUGH passThrough;

    pWorkSpace = (UINT8 *)malloc(512);

    if (!pWorkSpace)
        {
        printf("failed to malloc buffer\n");
        return ERROR;
        }

    pWorkSpace2 =(UINT8 *)malloc(512);

    if (!pWorkSpace2)
        {
        free(pWorkSpace);

        printf("failed to malloc buffer\n");

        return ERROR;
        }

    fd = open(pDevieName, O_RDWR, 0);

    printf("Open %s returned fd 0x%x\n", pDevieName, fd);

    if (fd < 0)
        {
        /* Release the resources alloced before */

        free(pWorkSpace);
        free(pWorkSpace2);

        return ERROR;
        }

    /* For a cmd to be passed through for demonstration */

    passThrough.uCdbLength = 6;
    passThrough.CDB[0] = USB2_MSC_SCSI_OPCODE_INQUIRY;
    passThrough.CDB[4] = USB2_MSC_SCSI_STD_INQUIRY_LEN;

    passThrough.bDataDirection = USB2_MSC_DATA_DIR_IN;
    passThrough.uDataTransferLength = USB2_MSC_SCSI_STD_INQUIRY_LEN;
    passThrough.pDataBuffer = pWorkSpace;

    passThrough.pSensDataBuffer = pWorkSpace2;
    passThrough.uSenseDataLength = 0;
    passThrough.timeout = USB2_MSC_SCSI_COMMAND_TIMEOUT;

    /* Issue scsi pass through */

    ret = ioctl(fd, USB2_MSC_DA_SCSI_PASS_THROUGH, &passThrough);

    if (ret == OK)
        {
        printf("Pass through OK\n");
        if (passThrough.uDataTransferLength)
            {
            printf("Pass through data:\n ");
            userUsb2MscPrintBuffer(passThrough.pDataBuffer,
                passThrough.uDataTransferLength);
            }
        }
    else
        {
        printf("Pass through NG\n");
        if (passThrough.uSenseDataLength)
            {
            printf("Sense data:\n ");
            userUsb2MscPrintBuffer(passThrough.pSensDataBuffer,
            passThrough.uSenseDataLength);
            }
        }

    free(pWorkSpace);

    free(pWorkSpace2);

    close(fd);

    printf("\r\nDone.\r\n");

    /* Things are done .... */

    return OK;
    }
#endif /* USB2_MSC_TEST_CODE_ENABLE */


#endif /* __INCusrUsb2MscInitc */


