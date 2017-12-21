/* 40armAxiPci.cdf - Component configuration file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,07oct09,fao  created
*/

Component DRV_PCIBUS_ARMAXI {
    NAME            Generic ARM AXI2PCI bus controller driver
    SYNOPSIS        Generic ARM AXI2PCI bus controller driver
    _CHILDREN       FOLDER_DRIVERS
    MODULES         vxbArmAxiPci.o
    SOURCE          $(WIND_BASE)/target/src/hwif/busCtlr
    REQUIRES        INCLUDE_VXBUS   \
                    INCLUDE_PLB_BUS \
                    INCLUDE_PCI_BUS
    INIT_RTN        vxbArmAxiPciRegister();
    PROTOTYPE       void vxbArmAxiPciRegister (void);
    INIT_AFTER      INCLUDE_PCI_BUS
    _INIT_ORDER     hardWareInterFaceBusInit
    _CHILDREN       FOLDER_DRIVERS
    _DEFAULTS    += FOLDER_DRIVERS
}
