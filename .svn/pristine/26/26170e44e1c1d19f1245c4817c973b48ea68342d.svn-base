/* 40vxbIbmZmii.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,01feb01,rcs  written

*/

Component	INCLUDE_VXB_IBM_ZMII {
	NAME		IBM ZMII bridge driver
	SYNOPSIS	IBM ZMII bridge driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/mii/vxbIbmZmii.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	zmiiRegister();
        INIT_AFTER      INCLUDE_PLB_BUS
	REQUIRES	INCLUDE_PLB_BUS
}

