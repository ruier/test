/* 10driver_download.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2005 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,01jul08,tor  library build
01a,06jul05,tor  written

*/

Component	INCLUDE_DOWNLOADABLE_DRIVERS {
	NAME		Downloadable Device Drivers
	SYNOPSIS	Downloadable Device Drivers
	MODULES		drvDownLoad.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	LINK_SYMS	drvUnloadLibInit
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	REQUIRES	INCLUDE_PLB_BUS
	INIT_AFTER	INCLUDE_PLB_BUS
}

