/* 40vxbAt91Mci.cdf - AT91 MCI Driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,04jan12,e_d  written.
*/

Component DRV_STORAGE_AT91_MCI {
    NAME        AT91 MCI host controller driver
    SYNOPSIS    AT91 MCI host controller driver
    MODULES     vxbAt91Mci.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_ERF \
                INCLUDE_FS_MONITOR \
                INCLUDE_XBD
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    at91MciRegister();
    PROTOTYPE   void at91MciRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}