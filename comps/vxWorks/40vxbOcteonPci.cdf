/* 40vxbOcteonPci.cdf - Component configuration file for Octeon PCI drivers */
  
/*
 * Copyright (c) 2010-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,24jan11,d_c  WIND00249323 - bring up PCIe on 63xx
01b,07jan11,l_z  fix broken cdf after INCLUDE_PCI_BUS_AUTOCONF (WIND00249600)
01a,11oct10,d_c  written

DESCRIPTION

This file contains component definitions for Cavium Octeon PCI and
PCI-E drivers.

*/

Component DRV_PCIBUS_OCTEON {
    NAME            octPci Vxbus PCI driver
    SYNOPSIS        octPci Vxbus PCI driver
    _CHILDREN       FOLDER_DRIVERS
    MODULES         vxbOcteonPci.o
    _INIT_ORDER     hardWareInterFaceBusInit
    PROTOTYPE       void octPciRegister (void);
    INIT_RTN        octPciRegister();
    REQUIRES        INCLUDE_VXBUS            \
                    INCLUDE_PLB_BUS          \
                    INCLUDE_PCI_BUS          \
                    INCLUDE_PCI_BUS_AUTOCONF 
    INIT_AFTER      INCLUDE_PCI_BUS
}

Component DRV_PCIBUS_OCTEON_PCIEX {
    NAME        Cavium 56XX PCI Express Host Controller Driver
    SYNOPSIS    Cavium 56XX PCI Express Host Controller Driver
    MODULES     vxbOcteonPciEx.o
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    octPciExRegister();
    PROTOTYPE   void octPciExRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}	

