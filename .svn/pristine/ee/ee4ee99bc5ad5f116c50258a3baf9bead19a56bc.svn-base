/* 40tsecVxbEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007-2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01b,06may08,dlk  Added END2 version of driver.
01a,06feb07,wap  written
*/

Selection	SELECT_TSEC_VXB_END {
	NAME		Select END or END2 version of VxBus TSEC driver
	SYNOPSIS	Select END or END2 version of VxBus TSEC driver
	COUNT		1-1
	_CHILDREN	FOLDER_DRIVERS
	CHILDREN	INCLUDE_TSEC_VXB_END \
			INCLUDE_TSEC_VXB_END2
	DEFAULTS	INCLUDE_TSEC_VXB_END
}

Component	INCLUDE_TSEC_VXB_END {
	NAME		TSEC VxBus Enhanced Network Driver
	SYNOPSIS	TSEC VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/tsecVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	tsecRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_BCM54XXPHY \
			INCLUDE_MV88E1X11PHY \
                        INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}

Component	INCLUDE_TSEC_VXB_END2 {
	NAME		TSEC VxBus END2 Driver
	SYNOPSIS	TSEC VxBus END2 Driver
	HDR_FILES	../src/hwif/h/end2/vxbTsecEnd2.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	tsecEnd2Register();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_BCM54XXPHY \
			INCLUDE_MV88E1X11PHY \
                        INCLUDE_DMA_SYS \
			INCLUDE_END2
	INIT_AFTER	INCLUDE_PLB_BUS
}
