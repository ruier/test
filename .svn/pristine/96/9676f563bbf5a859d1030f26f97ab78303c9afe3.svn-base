/* 40vxbAltSocGen5DwMshc.cdf - Altera Mshc Driver */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,24may13,sye  written.
*/

Component DRV_STORAGE_ALT_SOC_GEN5_DW_MSHC {
    NAME        Altera SoC DesignWare MSHC driver
    SYNOPSIS    Altera SoC DesignWare Mobile Storage Host controller driver
    MODULES     vxbAltSocGen5DwMshc.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_ERF \
                INCLUDE_FS_MONITOR \
                INCLUDE_XBD
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    altSocGen5DwMshcRegister();
    PROTOTYPE   void altSocGen5DwMshcRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
