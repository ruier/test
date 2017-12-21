/* 40fecVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,12feb07,wap  written

*/

Component	INCLUDE_FEC_VXB_END {
	NAME		Motorola/Freescale FEC Enhanced Network Driver
	SYNOPSIS	Motorola/Freescale FEC Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/fecVxbEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	fecRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_BCM52XXPHY \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}


