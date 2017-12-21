/* 40vxbPpc4xxPci.cdf - PowerPC 4xx PCI Host Controller Driver */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,25jul08,x_f  written
*/

Component       DRV_PCIBUS_PPC4XX_PCI {
        NAME            PPC4xx PCI Host Controller Driver 
        SYNOPSIS        PPC4xx PCI Host Controller Driver
        MODULES         vxbPpc4xxPci.o 
        INIT_RTN        vxbPpc4xxPciRegister();
        PROTOTYPE       void ppc4xxPciRegister (void);
        REQUIRES        INCLUDE_VXBUS     \
                        INCLUDE_PLB_BUS   \
                        INCLUDE_PCI_BUS   \
                        INCLUDE_PCI_BUS_AUTOCONF
        INIT_AFTER      INCLUDE_PCI_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_BUSES
}
