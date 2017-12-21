/* 40vxbUccEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,10mar08,wap  written
*/

Component	INCLUDE_UCC_VXB_END {
	NAME		UCC VxBus Enhanced Network Driver
	SYNOPSIS	UCC VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbUccEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	uccRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}
