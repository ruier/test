/* 40vxbIbmEmacEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01feb01,rcs  written

*/

Component	INCLUDE_EMAC_VXB_END {
	NAME		vxbIbmEmacEnd VxBus Enhanced Network Driver
	SYNOPSIS	emac vxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/vxbIbmEmacEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbEmacRegister();
        INIT_AFTER      INCLUDE_VXB_IBM_MAL 
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_VXB_IBM_MAL \
			INCLUDE_VXB_IBM_ZMII \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
}

