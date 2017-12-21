/* 40ynVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,04jan07,wap  Add dependency on mv88E1113 driver
01b,22aug06,wap  Correct typos
01a,28jul06,wap  written

*/

Component	INCLUDE_MVYUKONII_VXB_END {
	NAME		Marvell Yukon II VxBus Enhanced Network Driver
	SYNOPSIS	Marvell Yukon II VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/mvYukonIIVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	ynRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_MV88E1X11PHY \
			INCLUDE_MV88E1113PHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

