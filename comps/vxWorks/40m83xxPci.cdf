/* 40m83xxPci.cdf - Component configuration file */

/*
 * Copyright (c) 2007-2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01d,13mar12,y_c  add dependency to component DRV_PCIBUS_M83XX. (WIND00252646)	 
01c,31oct08,h_k  removed dependency to the component does not exist.
01b,26jul07,h_k  changed to library build.
01a,07feb07,rcs  created
*/

/**********************************************************************
*
* Generic PowerPC 83xx PCI bus controller
*
*/

Component DRV_PCIBUS_M83XX {
	NAME		m83xxPci
	SYNOPSIS	Generic PowerPC 83xx PCI bus controller for VxBus
	_CHILDREN	FOLDER_DRIVERS
	MODULES		m83xxPci.o
	SOURCE		$(WIND_BASE)/target/src/hwif/busCtlr
	REQUIRES	INCLUDE_VXBUS \
			    INCLUDE_PLB_BUS \
			    INCLUDE_PCI_BUS \
			    INCLUDE_PCI_BUS_AUTOCONF
	INIT_RTN	m83xxPciRegister();
	PROTOTYPE	void m83xxPciRegister (void);
        INIT_AFTER      INCLUDE_PCI_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}
