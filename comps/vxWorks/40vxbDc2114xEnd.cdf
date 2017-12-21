/* 40vxbDc2114xEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,10mar08,wap  written
*/

Component	DRV_VXBEND_DC2114X {
	NAME		DEC/Intel 2114x Enhanced Network Driver
	SYNOPSIS	DEC/Intel 2114x Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbDc2114xEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROROTYPE	void dcRegister(void);
	INIT_RTN	dcRegister();
	MODULES		vxbDc2114xEnd.o
	REQUIRES	INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}
