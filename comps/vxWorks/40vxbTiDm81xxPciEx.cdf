/* 40vxbTiDm81xxPciEx.cdf - TI DM81xx PCI Express Host Controller Driver */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,08oct11,clx  created
*/

Component DRV_PCIBUS_DM81XX_EX {
    NAME        TI DM81xx PCI Express Host Controller Driver
    SYNOPSIS    TI DM81xx PCI Express Host Controller Driver
    MODULES     vxbTiDm81xxPciEx.o
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbTi81xxPcieRegister();
    PROTOTYPE   void vxbTi81xxPcieRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
