/* 40vxbComcertoPci.cdf - Mindspeed Comcerto 100 PCI Host Controller Driver */

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
01a,22aug08,b_m  written.
*/

Component DRV_PCIBUS_COMCERTO {
    NAME        Mindspeed Comcerto 100 PCI Host Controller Driver
    SYNOPSIS    Mindspeed Comcerto 100 PCI Host Controller Driver
    MODULES     vxbComcertoPci.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF \
                INCLUDE_VXB_LEGACY_INTERRUPTS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    comcertoPciRegister();
    PROTOTYPE   void comcertoPciRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PCI_BUS
}
