/* 40m85xxPci.cdf - Component configuration file */

/*
 * Copyright (c) 2007, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01c,01jun10,dhy  add m85xxPCI require components. (WIND00143454)
01b,26jul07,h_k  changed to library build.
01a,07feb07,dtr  created
*/

/**********************************************************************
*
* Generic PowerPC 85xx PCI bus controller
*
*/


Component	DRV_PCIBUS_M85XX {
	NAME		Freescale MPC85xx PCI/PCIe Host Controller Driver
	SYNOPSIS	M85xx PCI bus controller Driver
	MODULES		m85xxPci.o
	SOURCE		$(WIND_BASE)/target/src/hwif/busCtlr
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	m85xxPciRegister();
	PROTOTYPE	void m85xxPciRegister (void);
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_PCI_BUS \
			INCLUDE_PCI_BUS_AUTOCONF \
			DRV_RESOURCE_M85XXCCSR
	INIT_AFTER	INCLUDE_PCI_BUS
}

