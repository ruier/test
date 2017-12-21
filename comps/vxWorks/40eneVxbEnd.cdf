/* 40eneVxbEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2007, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,14may13,fao  Disable SMP support. (WIND00406517)
01b,28nov07,mdo  CQ: WIND00110055  - fix typos
01a,17jan07,wap  written

*/

#ifndef _WRS_CONFIG_SMP
Component	INCLUDE_NE2000_VXB_END {
	NAME		NE2000 compatible VxBus END Driver
	SYNOPSIS	NE2000 compatible VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/ne2000VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	eneRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}
#endif /* _WRS_CONFIG_SMP */
