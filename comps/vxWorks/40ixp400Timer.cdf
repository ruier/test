/* 40ixp400Timer.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006-2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,15sep06,pmr  written

*/

Component	DRV_TIMER_IXP400 {
	NAME		IXP4xx timer Driver
	SYNOPSIS	IXP4xx timer Driver
	MODULES		vxbIxp400Timer.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	ixp400TimerDrvRegister();
	PROTOTYPE	void ixp400TimerDrvRegister (void);
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_TIMER_SYS
}

