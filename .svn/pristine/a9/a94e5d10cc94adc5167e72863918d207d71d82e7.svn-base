/* 40vxbI8253Timer.cdf - Component Definition file for i8253 timer */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01mar07,pdg  written

*/

Component	DRV_TIMER_I8253 {
	NAME		i8253 timer Driver
	SYNOPSIS	i8253 timer Driver
	MODULES		vxbI8253Timer.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
        PROTOTYPE       void vxbI8253TimerDrvRegister (void);
	INIT_RTN	vxbI8253TimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}
