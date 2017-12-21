/* 40elPciVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,28jul06,wap  written

*/

Component	INCLUDE_TC3C905_VXB_END {
	NAME		3Com 3c905/B/C VxBus END Driver
	SYNOPSIS	3Com 3c905/B/C VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/tc3c905VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	elPciRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

