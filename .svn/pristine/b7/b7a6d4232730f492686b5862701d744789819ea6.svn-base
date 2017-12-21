/* 40QorIQLaw.cdf - 85xx CPU CCSR resource configuration file */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,02dec08,wap  created
*/

Component DRV_RESOURCE_QORIQLAW {
	NAME		Freescale QORIQ LAW resource driver
	SYNOPSIS	Local Access Window driver
	MODULES		vxbQorIQLaw.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	lawRegister();
	PROTOTYPE	void lawRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	INCLUDE_PLB_BUS
}
