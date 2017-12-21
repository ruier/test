/* 40sccVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,31jan07,wap  Change CPM to VXB_CPM
01b,23jul06,wap  Fix compiler warnings
01a,02jun06,wap  written

*/

Component	INCLUDE_SCC_VXB_END {
	NAME		Freescale SCC ethernet VxBus Enhanced Network Driver
	SYNOPSIS	Freescale SCC ethernet VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/sccVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	sccRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_VXB_CPM
	INIT_AFTER	INCLUDE_PLB_BUS
}

