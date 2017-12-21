/* 40xlsPciEx.cdf - Component configuration file */

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
01b,09sep09,pch  cq177176: add INCLUDE_PCI_BUS_AUTOCONF to REQUIRES
01a,19mar09,x_f  created.
*/

/**********************************************************************
*
* Generic XLS PCI-E bus controller
*
*/

Component   DRV_PCIBUS_XLS_PCIEX {
	NAME        XLS PCI-E bus controller Driver
	SYNOPSIS    XLS PCI-E bus controller Driver
	MODULES     vxbXlsPciEx.o
	SOURCE      $(WIND_BASE)/target/src/hwif/busCtlr
	_CHILDREN   FOLDER_DRIVERS
	_INIT_ORDER hardWareInterFaceBusInit
	INIT_RTN    xlsPciExRegister();
	PROTOTYPE   void xlsPciExRegister (void);
	REQUIRES    INCLUDE_VXBUS \
		    INCLUDE_PLB_BUS \
		    INCLUDE_PARAM_SYS \
		    INCLUDE_PCI_BUS_AUTOCONF \
		    INCLUDE_PCI_BUS
	INIT_AFTER  INCLUDE_PCI_BUS
}
