/* 40nicVxbEnd.cdf - Component configuration file */

/*
 * Copyrigh (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,15jun07,h_k  added dependency to INCLUDE_END.
01a,05dec07,wap  written
*/

Component	INCLUDE_NS83902_VXB_END {
	NAME		National Semiconductor DP83902 VxBus END Driver
	SYNOPSIS	National Semiconductor DP83902 VxBus END driver
	MODULES		ns83902VxbEnd.o
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/ns83902VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	nicRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}

