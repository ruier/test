/* 40cpm.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,31jan07,wap  Change CPM to VXB_CPM
01b,23jul06,wap  Fix compiler warnings
01a,02jun06,wap  written

*/

Component	INCLUDE_VXB_CPM {
	NAME		Freescale CPM VxBus driver
	SYNOPSIS	Freescale CPM VxBus driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/resource/cpm.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	cpmRegister();
	REQUIRES	INCLUDE_PLB_BUS
	INIT_AFTER	INCLUDE_PLB_BUS
}

