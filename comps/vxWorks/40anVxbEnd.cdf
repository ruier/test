/* 40anVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,28jul06,wap  written

*/

Component	INCLUDE_AN983_VXB_END {
	NAME		Infineon AN983 VxBus Enhanced Network Driver
	SYNOPSIS	Infineon AN983 VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/an983VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	anRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

