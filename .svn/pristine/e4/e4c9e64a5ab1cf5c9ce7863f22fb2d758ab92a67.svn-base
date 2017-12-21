/* 00devPwrMgmt.cdf - Device Power management component description file */

/*
 * Copyright 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */
	
/*
modification history
--------------------
01i,07sep11,jjk  WIND00296077 - Update Polling Interval Synopsis
01h,02sep11,jjk  WIND00296077 - Added support for device timeout polling
01g,24aug11,rbc  WIND00295402 - Fix build error for guest OS with
                 INCLUDE_PWR_POLICY_MGR
                 WIND00297789 - Fix build error for guest OS with
                 INCLUDE_DEV_PWR_MGMT
01f,08aug11,rbc  WIND00290401 - Add INCLUDE_ERF to
                 INCLUDE_DEV_PWR_MGMT component
01e,01aug11,jlv  Run DPM as a standalone task
01d,28jul11,rbc  WIND00238999 - Add VSB _WRS_CONFIG_PWR_MGMT
01c,20jul11,jlv  WIND00255352, add Power Policy Manager for I86
01b,07jul11,rdl  change initialization mechanism
01a,03jun11,rec  Created

DESCRIPTION

This file contains the device power Management components.
*/

#ifdef _WRS_CONFIG_PWR_MGMT

#ifndef _WRS_CONFIG_WRHV_GUEST
Component INCLUDE_DEV_PWR_MGMT {
    NAME        Device Power Management
    SYNOPSIS    Device Power Management Support
    INIT_RTN    pwrDeviceLibStart (TRUE, DEV_PWR_POLLING_INTERVAL);  /* device power management */
    HDR_FILES   hwif/pwr/pwrDeviceLib.h  
    REQUIRES    INCLUDE_ERF \
                INCLUDE_PWR_LIB_FUNCS \
                INCLUDE_VXBUS
    CFG_PARAMS  DEV_PWR_POLLING_INTERVAL
    _CHILDREN   FOLDER_PWR_MGMT 
}

Parameter DEV_PWR_POLLING_INTERVAL  {
    NAME        Power Device Timeout Polling Interval
    SYNOPSIS    This is the periodic polling interval used by pwrDeviceMonitor \
                Values in the range of 1-minute to several hours are expected. \
                A value of 0 will disable periodic polling. \
                If disabled, the device power manager will check the device database \
                using an interval that is obtained by searching for the device with \
                the shortest timeout period (i.e., if the device with the shortest \
                timeout is configured to time-out after an hour, the database would \
                be checked once each hour). \
                At worst case, a device timeout will be detected after a period of \
                inactivity as long as twice the poll interval. \
                UNITS: ticks RANGE 0..0xffffffff
    TYPE        UINT32
    DEFAULT     0
}

#else /* _WRS_CONFIG_WRHV_GUEST */

Component INCLUDE_DEV_PWR_MGMT {
    NAME        Device Power Management
    SYNOPSIS    Device Power Management Support 
    STATUS_DETAILS this component is disabled because option  \
                _WRS_CONFIG_WRHV_GUEST is selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

#endif /* _WRS_CONFIG_WRHV_GUEST */

#else  /* _WRS_CONFIG_PWR_MGMT */

Component INCLUDE_DEV_PWR_MGMT {
    NAME        Device Power Management
    SYNOPSIS    Device Power Management Support 
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

#endif /* _WRS_CONFIG_PWR_MGMT */
