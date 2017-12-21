/* 40xlrxxPci.cdf - Component configuration file */

/*
 * Copyright (c) 2007, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,07jan11,l_z  add INCLUDE_PCI_BUS_AUTOCONF (WIND00249606)
01a,19nov07,l_z  created.
*/

Component   INCLUDE_XLR_PCI {
    NAME        XLR PCI bus controller Driver
    SYNOPSIS    XLR PCI bus controller Driver
    MODULES     vxbXlrxxPci.o
    SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    xlrPciRegister();
    PROTOTYPE   void xlrPciRegister (void);
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
    INIT_AFTER  INCLUDE_PCI_BUS
}

