/* 40QorIQBman.cdf - Freescale QorIQ Buffer Manager driver file */

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
01a,20dec09,wap  created
*/

Component DRV_RESOURCE_QORIQBMAN {
	NAME		Freescale QORIQ BMAN resource driver
	SYNOPSIS	Buffer Manager driver
	MODULES		vxbQorIQBman.o \
			vxbQorIQBmanPortal.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	bmanRegister();
	PROTOTYPE	void bmanRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	DRV_RESOURCE_QORIQLAW
}
