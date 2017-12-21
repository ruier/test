/* 40ykVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,28jul06,wap  written

*/

Component	INCLUDE_MVYUKON_VXB_END {
	NAME		Marvell Yukon VxBus Enhanced Network Driver
	SYNOPSIS	Marvell Yukon VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/mvYukonVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	ykRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_MV88E1X11PHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

