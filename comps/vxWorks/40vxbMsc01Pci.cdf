/* 40vxbMsc01Pci.cdf - PCI bridge on SOC-it system controller */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01a,05Jul06,pdg  created
*/

Component	INCLUDE_MSC01_PCI {
	NAME		msc01 PCI controller driver
	SYNOPSIS	SOC-it system PCI controller driver
	_CHILDREN	FOLDER_DRIVERS
	MODULES		vxbMsc01Pci.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS
	INIT_RTN	vxbMsc01PciRegister();
	PROTOTYPE	void vxbMsc01PciRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}