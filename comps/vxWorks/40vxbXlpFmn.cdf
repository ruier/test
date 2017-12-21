/* 40vxbXlpFmn.cdf - Component configuration file */

/*
 * Copyright 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01a,14may11,x_f  written
*/

Component	DRV_RESOURCE_FMN_XLP {
	NAME		NetLogic Micro XLP FMN Driver
	SYNOPSIS	NetLogic Micro XLP FMN driver
	_CHILDREN	FOLDER_DRIVERS
	MODULES		vxbXlpFmn.o
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void fmnRegister(void);
	INIT_RTN	fmnRegister();
	REQUIRES	INCLUDE_PLB_BUS    \
				INCLUDE_PCI_BUS
				INCLUDE_PARAM_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

