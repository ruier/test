/* 40m83xxPciEx.cdf - Component configuration file */

/*
 * Copyright (c) 2007-2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,13mar12,y_c  add dependency to component DRV_PCIBUS_M83XX_EX. (WIND00252646)
01d,23oct08,syt  convert to library build (WIND00131698)
01c,19oct08,jrp  WIND00136163
01b,07sep07,dtr  added Pci Express driver.
01a,07feb07,rcs  created
*/

/**********************************************************************
*
* Generic PowerPC 83xx PCI Express bus controller
*
*/

Component DRV_PCIBUS_M83XX_EX {
	NAME		PowerPC 83xx PCI Express host controller driver
	SYNOPSIS	PowerPC 83xx PCI Express host controller driver
	MODULES		m83xxPciEx.o
	SOURCE		$(WIND_BASE)/target/src/hwif/busCtlr
	REQUIRES	INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_PCI_BUS \
                INCLUDE_PCI_BUS_AUTOCONF
	INIT_RTN	m83xxPciExRegister();
    PROTOTYPE   void m83xxPciExRegister(void);
    INIT_AFTER  INCLUDE_PCI_BUS
    _INIT_ORDER hardWareInterFaceBusInit
    _CHILDREN   FOLDER_DRIVERS
}

