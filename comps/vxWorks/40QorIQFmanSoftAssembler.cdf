/* 40QorIQFmanSoftAssembler.cdf - Freescale QorIQ Frame Manager soft parser assembler */

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

Component DRV_RESOURCE_QORIQFMAN_SOFTPARSE_ASSEMBLER {
	NAME		Freescale QORIQ FMAN soft parser assembler
	SYNOPSIS	Freescale QORIQ FMAN soft parser assembler
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	fmanPrSpAsmInit ();
	PROTOTYPE	void fmanPrSpAsmInit (void);
	REQUIRES	DRV_RESOURCE_QORIQFMAN
	INIT_AFTER	DRV_RESOURCE_QORIQFMAN
}
