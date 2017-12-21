/* 40vxbIbmTah.cdf - Component configuration file */

/* Copyright (c) 2001,2008 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,12aug08,mdo  WIND00130408
01a,01feb01,rcs  written

*/

Component	INCLUDE_VXB_IBM_TAH {
	NAME		IBM TCP Acceleration driver
	SYNOPSIS	IBM TCP Acceleration driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/resource/vxbIbmTah.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	tahRegister();
        INIT_AFTER      INCLUDE_PLB_BUS
	REQUIRES	INCLUDE_PLB_BUS
}

