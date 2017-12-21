/* 40vxbStmSpear13xxPciExV2.cdf - STM SPEAr 13xx PCI Express Host Controller Version 2 Driver */

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
01a,16oct12,clx  written
*/

Component DRV_PCIBUS_SPEAR13XX_EX_V2 {
    NAME        STM SPEAr 13xx PCI Express Host Controller Version 2 Driver
    SYNOPSIS    STM SPEAr 13xx PCI Express Host Controller Version 2 Driver
    MODULES     vxbStmSpear13xxPciExV2.o
    SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    spear13xxPcieV2Register();
    PROTOTYPE   void spear13xxPcieV2Register(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
