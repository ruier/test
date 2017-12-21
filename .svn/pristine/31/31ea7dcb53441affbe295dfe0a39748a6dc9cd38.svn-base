/* 40vxbMc146818Rtc.cdf - Component Definition file for MC146818 RTC */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01mar07,pdg  written

*/

Component	DRV_TIMER_MC146818 {
	NAME		Mc146818 RTC Driver
	SYNOPSIS	Mc146818 RTC Driver
	MODULES		vxbMc146818Rtc.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
        PROTOTYPE       void vxbMc146818RtcDrvRegister (void);
	INIT_RTN	vxbMc146818RtcDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}
