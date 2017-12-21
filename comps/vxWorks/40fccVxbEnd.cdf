/* 40fccVxbEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2006, 2007, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01d,12mar12,y_c  Move the component INCLUDE_BCM52XXPHY to 20bsp.cdf
                 of the specific bsp. (WIND00333700)
01c,31jan07,wap  Change CPM to VXB_CPM
01b,23jul06,wap  Fix compiler warnings
01a,02jun06,wap  written

*/

Component	INCLUDE_FCC_VXB_END {
	NAME		Freescale FCC ethernet VxBus Enhanced Network Driver
	SYNOPSIS	Freescale FCC ethernet VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/fccVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	fccRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_MII_BUS \
			INCLUDE_MDIO \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_VXB_CPM
	INIT_AFTER	INCLUDE_PLB_BUS
}

