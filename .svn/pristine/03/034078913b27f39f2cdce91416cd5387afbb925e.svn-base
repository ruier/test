/* 40QorIQQman.cdf - Freescale QorIQ Queue Manager driver file */

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
01b,20jan09,wap  Update filenames
01a,02dec08,wap  created
*/

Component DRV_RESOURCE_QORIQQMAN {
	NAME		Freescale QORIQ QMAN resource driver
	SYNOPSIS	Queue Manager driver
	MODULES		vxbQorIQQman.o \
			vxbQorIQQmanPortal.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	qmanRegister();
	PROTOTYPE	void qmanRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	DRV_RESOURCE_QORIQLAW
}
