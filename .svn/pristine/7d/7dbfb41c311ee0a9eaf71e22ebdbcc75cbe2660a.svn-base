/* 40vxbVirtualPciAdaptor.cdf - support VxBus PCI when Firmware configs it */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,21aug08,wap  fix module name
01a,20jun08,tor created
*/



Component   DRV_PCI_VIRTUAL {
    NAME        Virtual VxBus PCI bus controller
    SYNOPSIS    Support VxBus drivers for PCI devices when BSP/FirmWare configures the PCi bus
    MODULES     vxbLegacyPciAdaptor.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbVirtualPciRegister();
    PROTOTYPE   void vxbVirtualPciRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
		INCLUDE_PARAM_SYS
    INIT_AFTER  INCLUDE_PLB_BUS
    EXCLUDES	INCLUDE_PCI_BUS

}
