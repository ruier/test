/* 40vxbIntelTimestamp.cdf - Component Definition file for Intel timestamp */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01mar07,pdg  written

*/
Component	DRV_TIMER_IA_TIMESTAMP {
	NAME		Intel timestamp Driver
	SYNOPSIS	Intel timestamp Driver
	MODULES		vxbIntelTimestamp.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
        PROTOTYPE       void vxbIaTimestampDrvRegister (void);
	INIT_RTN	vxbIaTimestampDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}
