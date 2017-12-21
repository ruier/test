/* 40MipsR4KTimer.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2007-2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */
                                                                                
/*
modification history
--------------------
01e,19jul10,rlg  removal of vxbCn3xxxTimer.c Configlette
01e,28jun10,pgh  Allow cn3xxx timer to work with octeon II.
01d,19oct08,jrp  WIND00136163
01c,23apr08,h_k  converted to source build. (CQ:105277)
01b,29Sep07,l_z  Added xlr timer
01a,06Jul07,jmt  Collapsed MIPS timer defs into one file
                 Added Cavium CN3XXX timer
*/

Component	DRV_TIMER_MIPSR4K {
	NAME		mips R4K timer Driver
	SYNOPSIS	mips R4K timer Driver
	MODULES		vxbMipsR4KTimer.o
	SOURCE		../../../src/hwif/timer
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void vxbR4KTimerDrvRegister (void);
	INIT_RTN	vxbR4KTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}

Component	DRV_TIMER_SB1 {
	NAME		sb1 timer Driver
	SYNOPSIS	sb1 timer Driver
	MODULES		vxbSb1Timer.o
	SOURCE		../../../src/hwif/timer
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void vxbSb1TimerDrvRegister (void);
	INIT_RTN	vxbSb1TimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}

Component	DRV_TIMER_CN3XXX {
	NAME		CN3xxx timer Driver
	SYNOPSIS	Cavium MIPS CN3xxx timer Driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void vxbCn3xxxTimerDrvRegister (void);
	INIT_RTN	vxbCn3xxxTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}

Component	INCLUDE_XLR_TIMER {
	NAME		xlr timer Driver
	SYNOPSIS	xlr MIPS timer Driver
	MODULES		vxbXlrTimer.o
	SOURCE		../../../src/hwif/timer
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void vxbXlrTimerDrvRegister (void);
	INIT_RTN	vxbXlrTimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_TIMER_SYS
}

