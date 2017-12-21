/* 40nseVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,28jul06,wap  written

*/

Component	INCLUDE_NS8381X_VXB_END {
	NAME		National Semiconductor DP83815/6 VxBus END Driver
	SYNOPSIS	National Semiconductor DP83815/6 VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/ns8381xVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	nseRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

