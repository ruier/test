/* 40vxbTgecEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2009-2010 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,15jul10,wap  Make sure the DTSEC driver shows up in the drivers folder
01a,02sep09,wap  written
*/

Component	DRV_VXBEND_TGEC {
	NAME		TGEC VxBus Enhanced Network Driver
	SYNOPSIS	Freescale Ten Gigabit Ethernet VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbTgecEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbTgecEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
	REQUIRES	DRV_MII_DTEPHY
}
