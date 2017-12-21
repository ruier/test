/* 40vxbXlrCpld.cdf - xlr CPLD CDF file */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,02may08,wap  created
*/

Component	DRV_RESOURCE_XLRCPLD {
	NAME		Raza Micro XLR CPLD driver
	SYNOPSIS	Raza Micro XLR CPLD driver
	MODULES		vxbXlrCpld.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	xlrCpldRegister();
	PROTOTYPE	void xlrCpldRegister(void);
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}
