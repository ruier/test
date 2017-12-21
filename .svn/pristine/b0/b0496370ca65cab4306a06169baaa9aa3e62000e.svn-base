/* 40vxbRmiGmacEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,10oct07,wap  written

*/

Component	INCLUDE_XLR_GMAC_VXB_END {
	NAME		Raza Micro GMAC RGMII VxBus Enhanced Network Driver
	SYNOPSIS	Raza Micro GMAC RGMII VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void xlrGmacRegister(void);
	INIT_RTN	xlrGmacRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}

