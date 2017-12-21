/* 40vxbLsiPciEx.cdf - LSI PCI Express Host Controller Driver */

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
01a,10jan11,x_s  written.
*/

Component DRV_PCIBUS_LSI_PCIEX {
    NAME        LSI PCI Express Host Controller Driver
    SYNOPSIS    LSI PCI Express Host Controller Driver
    MODULES     vxbLsiPciEx.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    lsiPciExRegister();
    PROTOTYPE   void lsiPciExRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
