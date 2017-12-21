/* 40vxbPpc440gxPci.cdf - Component configuration file */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,01may07,rcs  written

*/

Component       INCLUDE_PPC440GX_PCI {
        NAME            vxbPpc440gxPci PCI bus support for vxBus
        SYNOPSIS        PCI Bus support
        MODULES         vxbPpc440gxPci.o 
        INIT_RTN        vxbPpc440gxPciRegister();
        PROTOTYPE       void vxbPpc440gxPciRegister (void);
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_PCI_BUS
        INIT_AFTER      INCLUDE_PCI_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_BUSES
}

