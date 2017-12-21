/* 11vxblegacyint.cdf - VxBus legacy interrupt Component configuration file */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01a,30oct07,tor  created
*/

Component	INCLUDE_VXB_LEGACY_INTERRUPTS {
	NAME		VxBus Legacy Interrupt Support
	SYNOPSIS	Support for intConnect() when VxBus interrupt controllers used
	INIT_RTN	vxbLegacyIntInit();
	HDR_FILES	hwif/util/vxbLegacyIntLib.h
	_INIT_ORDER	vxbDevInit
	INIT_BEFORE	INCLUDE_VXBUS_DEV_INIT	
	REQUIRES	INCLUDE_INTCTLR_LIB
	_CHILDREN	FOLDER_HWIF
}
