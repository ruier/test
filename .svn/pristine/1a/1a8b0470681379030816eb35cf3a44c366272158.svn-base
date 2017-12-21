/* 40vxbMyriLanaiEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2009-2010 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,15jul10,wap  Make sure the DTSEC driver shows up in the drivers folder
01b,15jul10,wap  Make sure the Myricom driver shows up in the drivers folder
01a,17mar09,wap  written

*/

Component	DRV_VXBEND_MYRILANAI {
	NAME		Myricom Lanai 10GbE VxBus Enhanced Network Driver
	SYNOPSIS	Myricom Lanai 10GbE VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbMyriLanaiEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	myeRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PCI_BUS
}
