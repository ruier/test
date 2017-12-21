/* 40vxbDtsecEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2009-2011 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,01nov11,kab  Add to drv folder
01c,11may11,pch  CQ273410: make component visible
01b,15jul10,wap  Make sure the DTSEC driver shows up in the drivers folder
01a,24jun09,wap  written
*/

Component	DRV_VXBEND_DTSEC {
	NAME		DTSEC VxBus Enhanced Network Driver
	SYNOPSIS	DTSEC VxBus Enhanced Network Driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/vxbDtsecEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbDtsecEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_GENERICPHY \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}
