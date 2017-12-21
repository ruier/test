/* 40vxbPpc4xxPciEx.cdf - PowerPC 4xx PCI Express Host Controller Driver */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10jul08,b_m  written.
*/

Component DRV_PCIBUS_PPC4XX_PCIEX {
    NAME        PowerPC 4xx PCI Express Host Controller Driver
    SYNOPSIS    PowerPC 4xx PCI Express Host Controller Driver
    MODULES     vxbPpc4xxPciEx.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    ppc4xxPciExRegister();
    PROTOTYPE   void ppc4xxPciExRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
