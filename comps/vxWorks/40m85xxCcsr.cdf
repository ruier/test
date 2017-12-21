/* 40m85xxCcsr.cdf - 85xx CPU CCSR resource configuration file */

/*
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,26jul07,h_k  changed to library build
01a,23aug06,pmr  created
*/

Component DRV_RESOURCE_M85XXCCSR {
	NAME		CCSR resource driver
	SYNOPSIS	Local Access Window allocation
	MODULES		m85xxCCSR.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	m85xxCCSRRegister();
	PROTOTYPE	void m85xxCCSRRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	INCLUDE_PLB_BUS
}
