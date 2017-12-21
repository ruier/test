/* 40vxbLoApicTimer.cdf - Component Definition file for loApic timer */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01mar07,pdg  written

*/
Component	DRV_TIMER_LOAPIC {
	NAME		loApic timer Driver
	SYNOPSIS	loApic timer Driver
	MODULES		vxbLoApicTimer.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
        PROTOTYPE       void vxbLoApicTimerDrvRegister (void);
	INIT_RTN	vxbLoApicTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}
