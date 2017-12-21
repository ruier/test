/* 40feiVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,23jul06,wap  Fix compiler warnings
01b,02jun06,wap  Add dependency on RTL8201 PHY driver
01a,01jun06,wap  written

*/

Component	INCLUDE_RTL8139_VXB_END {
	NAME		RealTek 8139/8100 VxBus Enhanced Network Driver
	SYNOPSIS	RealTek 8139/8100 VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/rtl8139VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	rtlRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_RTL8201PHY \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

