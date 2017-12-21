/* 40vxbOmap35xxMmchs.cdf - TI MMCHS Driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,02jun10,hcl  written.
*/

Component DRV_STORAGE_OMAP35XX_MMCHS {
    NAME        Omap35xx MMC/SD/SDIO host controller driver
    SYNOPSIS    Omap35xx MMC/SD/SDIO host controller driver
    MODULES     vxbOmap35xxMmchs.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_ERF \
                INCLUDE_FS_MONITOR \
                INCLUDE_XBD \
                DRV_DMA_OMAP35XX \
                INCLUDE_VXB_LEGACY_INTERRUPTS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    omap35xxMmchsRegister();
    PROTOTYPE   void omap35xxMmchsRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}