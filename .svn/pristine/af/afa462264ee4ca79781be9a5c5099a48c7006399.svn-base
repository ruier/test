/* 40QorIQPciEx.cdf - Freescale QorIQ PCI Express Host Controller Driver */

/*
 * Copyright (c) 2009, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,22mar12,sye  add INCLUDE_PCI_BUS_AUTOCONF as required component. 
                 (WIND00290683)
01a,25sep09,wap  written.
*/

Component DRV_PCIBUS_QORIQ_PCIEX {
    NAME        Freescale QorIQ PCI Express Host Controller Driver
    SYNOPSIS    Freescale QorIQ PCI Express Host Controller Driver
    MODULES     vxbQorIQPciEx.o
    REQUIRES    INCLUDE_VXBUS     \
                INCLUDE_PLB_BUS   \
                INCLUDE_PCI_BUS   \
                INCLUDE_PCI_BUS_AUTOCONF
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbQorIQPexRegister();
    PROTOTYPE   void vxbQorIQPexRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
