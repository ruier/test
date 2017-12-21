/* 40vxbSdhcStorage.cdf - SDHC Driver */

/*
 * Copyright (c) 2009, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,08may13,e_d  add Parameter SD_HOSTMONITOR_TASK_PRIORITY to set
                 SDHC monitor task priority. (WIND00372920)
01b,25mar11,my_  change FSL_SDHC to SDHC.
01a,28jul09,b_m  written.
*/

Component DRV_STORAGE_SDHC {
    NAME        Secure Digital Host Controller driver
    SYNOPSIS    Secure Digital Host Controller driver
    MODULES     vxbSdhcStorage.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_ERF \
                INCLUDE_FS_MONITOR \
                INCLUDE_XBD
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    sdhcStorageRegister();
    CONFIGLETTES usrSdhcCfg.c
    PROTOTYPE   void sdhcStorageRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
    CFG_PARAMS  SD_HOSTMONITOR_TASK_PRIORITY
}

Parameter SD_HOSTMONITOR_TASK_PRIORITY {
    NAME            SD host controller manager task priority
    SYNOPSIS        SD host controller task priority (proposed thresholds 50 - 100)
    TYPE            int
    DEFAULT         100
}
