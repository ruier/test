/* 40ppcDecTimer.cdf - Component configuration file */

/* Copyright (c) 2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,13jun07,h_k  changed to library build.
01a,07feb07,dtr  created
*/


/**********************************************************************
*
* Generic PowerPC Decrementer Timer
*
*/

Component DRV_TIMER_DEC_PPC {
	NAME		PPC Dec Timer
	SYNOPSIS	PPC Dec Timer for vxBus
	_CHILDREN	FOLDER_DRIVERS
	MODULES		vxbPpcDecTimer.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_TIMER_SYS
	INIT_RTN	ppcDecTimerDrvRegister();
	PROTOTYPE	void ppcDecTimerDrvRegister (void);
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}
