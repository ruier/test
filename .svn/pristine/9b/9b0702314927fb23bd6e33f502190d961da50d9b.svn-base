/* vxbSmcFdc37x.cdf - SMsC FDC37x Ultra I/O Configuration Driver for VxBus */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,01may07,h_k  written
*/

/**********************************************************************
*
* SMsC FDC37x Ultra I/O controller
*
*/

Component DRV_SUPERIO_SMCFDC37X {
	NAME		SMsC FDC37x driver
	SYNOPSIS	SMsC FDC37x Ultra IO controller driver
	MODULES		vxbSmcFdc37x.o
	INIT_RTN	vxbSmcFdc37xRegister();
	PROTOTYPE	void vxbSmcFdc37xRegister (void);
        _INIT_ORDER     hardWareInterFaceBusInit
	INIT_AFTER	INCLUDE_PLB_BUS
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS
	_CHILDREN	FOLDER_DRIVERS
	_DEFAULTS	+= FOLDER_DRIVERS
}
