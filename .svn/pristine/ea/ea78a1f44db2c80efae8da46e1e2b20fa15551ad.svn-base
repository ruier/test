/* 40QorIQFman.cdf - Freescale QorIQ Queue Manager driver file */

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
01b,20jan09,wap  Update filenames
01a,02dec08,wap  created
*/

Component DRV_RESOURCE_QORIQFMAN {
	NAME		Freescale QORIQ FMAN resource driver
	SYNOPSIS	Queue Manager driver
	MODULES		vxbQorIQFman.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	fmanRegister();
	PROTOTYPE	void fmanRegister (void);
	REQUIRES	INCLUDE_VXBUS
	INIT_AFTER	DRV_RESOURCE_QORIQQMAN
}
