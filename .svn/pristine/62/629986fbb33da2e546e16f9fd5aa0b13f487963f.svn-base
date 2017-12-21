/* 40QorIQFmanSoftParse.cdf - Freescale QorIQ Frame Manager soft parse module */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,25feb10,wap  created
*/

Component DRV_RESOURCE_QORIQFMAN_SOFTPARSE {
	NAME		Freescale QORIQ FMAN soft parser resource driver
	SYNOPSIS	Freescale QORIQ FMAN soft parser resource driver
	_CHILDREN	FOLDER_DRIVERS
	CONFIGLETTES	usrFmanSoftParse.c
	INIT_RTN	usrFmanSoftParseInit ();
	REQUIRES	DRV_RESOURCE_QORIQFMAN
	_INIT_ORDER	usrRoot
	INIT_AFTER	INCLUDE_USER_APPL
}
