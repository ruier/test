/* 40vxbAmbaTimer.cdf - Component configuration file */

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
01a,06jul07,to   written
*/

Component	DRV_TIMER_AMBA {
	NAME		AMBA timer Driver
	SYNOPSIS	AMBA timer Driver
	MODULES		vxbAmbaTimer.o
	PROTOTYPE	IMPORT void vxbAmbaTimerDrvRegister(void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbAmbaTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_TIMER_SYS
}

