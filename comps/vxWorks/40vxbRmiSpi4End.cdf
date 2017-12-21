/* 40vxbRmiSpi4End.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,27jan10,x_f  written
*/

Component	DRV_VXBEND_XLRSPI4 {
	NAME		Raza Micro SPI-4.2 card (10x1G MAC ports) VxBus Enhanced Network Driver
	SYNOPSIS	Raza Micro SPI-4.2 card (10x1G MAC ports) VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void xlrSpi4Register(void);
	INIT_RTN	xlrSpi4Register();
	REQUIRES	INCLUDE_PLB_BUS    \
				INCLUDE_GENERICPHY \
				INCLUDE_PARAM_SYS  \
				INCLUDE_DUMMY_MDIO
	INIT_AFTER	INCLUDE_PLB_BUS
}

