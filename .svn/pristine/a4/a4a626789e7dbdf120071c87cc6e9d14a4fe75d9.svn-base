/* 40vxbApmPciEx.cdf - AppliedMicro PCI Express Host Controller Driver */

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
01a,12jul11,b_m  written.
*/

Component DRV_PCIBUS_APM_PCIEX {
    NAME        AppliedMicro PCI Express Host Controller Driver
    SYNOPSIS    AppliedMicro PCI Express Host Controller Driver
    MODULES     vxbApmPciEx.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    apmPciExRegister();
    PROTOTYPE   void apmPciExRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
