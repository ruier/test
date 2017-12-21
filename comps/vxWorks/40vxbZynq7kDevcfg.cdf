/* 40vxbZynq7kDevcfg.cdf - zynq7k Devcfg CDF file */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,29may13,y_c  created
*/

Component	DRV_RESOURCE_ZYNQ7K_DEVC {
	NAME		Xilinx DevCfg driver
	SYNOPSIS	Xilinx DevCfg driver
	MODULES		vxbZynqDevCfg.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	zynq7kDevCfgRegister();
	PROTOTYPE	void zynq7kDevCfgRegister(void);
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}
