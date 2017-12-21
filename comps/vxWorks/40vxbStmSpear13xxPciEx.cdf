/* 40vxbStmSpear13xxPciEx.cdf - STM SPEAr 13xx PCI Express Host Controller Driver */

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
01a,04mar11,clx  written
*/

Component DRV_PCIBUS_SPEAR13XX_EX {
    NAME        STM SPEAr 13xx PCI Express Host Controller Driver
    SYNOPSIS    STM SPEAr 13xx PCI Express Host Controller Driver
    MODULES     vxbStmSpear13xxPciEx.o
    SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    spear13xxPcieRegister();
    PROTOTYPE   void spear13xxPcieRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
