/* 40OpenPicTimer.cdf - Component configuration file */

/* Copyright (c) 2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,18apr07,pch  created
*/

Component	DRV_TIMER_OPENPIC {
	NAME		OpenPIC Timer Driver
	SYNOPSIS	OpenPIC Timer Driver
	MODULES		vxbOpenPicTimer.o
	PROTOTYPE	IMPORT void openPicTimerDrvRegister(void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	openPicTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_TIMER_SYS
}

