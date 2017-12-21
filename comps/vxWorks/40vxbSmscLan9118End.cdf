/* 40vxbSmscLan9118End.cdf - Component configuration file */
                                                                                
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
01a,23may07,wap  written

*/

Component	INCLUDE_SMSCLAN9118_VXB_END {
	NAME		SMSC LAN9118 VxBus Enhanced Network Driver
	SYNOPSIS	SMSC LAN9118 VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void smeRegister(void);
	INIT_RTN	smeRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_MII_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}

