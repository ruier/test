/* 40vxbIbmRgmii.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01feb01,rcs  written

*/

Component	INCLUDE_VXB_IBM_RGMII {
	NAME		IBM RGMII bridge driver
	SYNOPSIS	IBM RGMII bridge driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/mii/vxbIbmRgmii.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	rgmiiRegister();
        INIT_AFTER      INCLUDE_PLB_BUS
	REQUIRES	INCLUDE_PLB_BUS
}

