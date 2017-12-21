/* 40vxbViaRhineEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,10mar08,wap  written
*/

Component	DRV_VXBEND_VIARHINE {
	NAME		VIA Rhine Enhanced Network Driver
	SYNOPSIS	VIA Rhine Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbViaRhineEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vrRegister();
	MODULES		vxbViaRhineEnd.o
	REQUIRES	INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}
