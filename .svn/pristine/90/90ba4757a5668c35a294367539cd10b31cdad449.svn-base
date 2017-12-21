/* 40vxbEtsecEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007-2008 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01e,26sep08,dlk  Remove unneded 'EXCLUDES' clauses for components in
                 a selection.
01d,22jul08,dlk  Eliminate duplicate INCLUDE_ETSEC_VXB_END2 component.
                 Also removed dependence upon particular PHY drivers.
01c,29apr08,dlk  Added INCLUDE_ETSEC_VXB_END2, SELECT_ETSEC_VXB_END.
01b,22may08,dtr  Remove requires for INCLUDE_BCM54XXPHY INCLUDE_MV88E1X11PHY.
01a,20jun07,wap  written
*/

Selection	SELECT_ETSEC_VXB_END {
	NAME		Select END or END2 version of VxBus Enhanced TSEC driver
	SYNOPSIS	Select END or END2 version of VxBus Enhanced TSEC driver
	COUNT		1-1
	_CHILDREN	FOLDER_DRIVERS
	CHILDREN	INCLUDE_ETSEC_VXB_END \
			INCLUDE_ETSEC_VXB_END2
	DEFAULTS	INCLUDE_ETSEC_VXB_END
}

Component	INCLUDE_ETSEC_VXB_END {
	NAME		Enhanced TSEC VxBus Enhanced Network Driver
	SYNOPSIS	Enhanced TSEC VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbEtsecEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	etsecRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}

Component	INCLUDE_ETSEC_VXB_END2 {
	NAME		Enhanced TSEC VxBus END2 Driver
	SYNOPSIS	Enhanced TSEC VxBus END2 Driver
	HDR_FILES	../src/hwif/h/end2/vxbEtsecEnd2.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	etsecEnd2Register();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS \
			INCLUDE_END2
	INIT_AFTER	INCLUDE_PLB_BUS
}
