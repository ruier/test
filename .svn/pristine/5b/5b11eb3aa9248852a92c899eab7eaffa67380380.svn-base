/* 40FslGpio.cdf - Freescale GPIO driver file */

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
01a,21sep09,wap  created
*/

Component DRV_RESOURCE_FSLGPIO {
	NAME		Freescale GPIO resource driver
	SYNOPSIS	GPIO driver
	MODULES		vxbFslGpio.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	fslGpioRegister();
	PROTOTYPE	void fslGpioRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	DRV_RESOURCE_QORIQQMAN
}
