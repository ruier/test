/* 40vxbRmiXgmacEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,09may08,wap  written

*/

Component	DRV_VXBEND_XLRXGMAC {
	NAME		Raza Micro XGMAC XGMII VxBus Enhanced Network Driver
	SYNOPSIS	Raza Micro XGMAC XGMII VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void xlrXgmacRegister(void);
	INIT_RTN	xlrXgmacRegister();
	MODULES		xlrRmiXgmacEnd.o
	REQUIRES	INCLUDE_PLB_BUS \
			DRV_MII_DTEPHY \
			DRV_RESOURCE_XLRCPLD
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}

