/* 40vxbQorIQMemacEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2012 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,13aug12,wap  written
*/

Component	DRV_VXBEND_QORIQMEMAC {
	NAME		MEMAC VxBus Enhanced Network Driver
	SYNOPSIS	Freescale Multirate Ethernet Media Access Controller VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbQorIQMemacEnd.h
	MODULES		vxbQorIQMemacEnd.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbMemacEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}
