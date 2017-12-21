/* 40vxbXlpIoMgmt.cdf - Component configuration file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,18aug11,x_f  add INCLUDE_PCI_OLD_CONFIG_ROUTINES for 
                 PROFILE_BOOTAPP(WIND00295102)
01a,19mar11,x_f  created.
*/

Component   DRV_PCIBUS_XLP_IO_MGMT {
    NAME        NetLogic XLP IO Management Driver
    SYNOPSIS    NetLogic XLP IO Management Driver
    MODULES     vxbXlpIoMgmt.o
    SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    xlpIoMgmtRegister();
    PROTOTYPE   void xlpIoMgmtRegister (void);
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF \
                INCLUDE_PCI_OLD_CONFIG_ROUTINES
    INIT_AFTER  INCLUDE_PCI_BUS
}

