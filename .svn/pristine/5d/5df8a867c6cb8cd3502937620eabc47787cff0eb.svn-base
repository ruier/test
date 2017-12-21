/* 40vxbXlpPciEx.cdf - Component configuration file */

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
01a,21mar11,x_f  created.
*/

Component   DRV_PCIBUS_XLP_PCIEX {
    NAME        NetLogic XLP PCIEX Bridge Device Driver
    SYNOPSIS    NetLogic XLP PCIEX Bridge Device Driver
    MODULES     vxbXlpPciEx.o
    SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    xlpPciExRegister();
    PROTOTYPE   void xlpPciExRegister (void);
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PCI_BUS
    INIT_AFTER  INCLUDE_PCI_BUS
}

