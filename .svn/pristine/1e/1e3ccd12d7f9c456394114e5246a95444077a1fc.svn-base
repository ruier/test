/* 40fecHEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,04jun07,h_k  removed CONFIGLETTES
01c,08aug06,rcs  Added INCLUDE_DMA_SYS to REQUIRES
01b,07feb06,wap  Add INCLUDE_PARAM_SYS as depencency
01a,20jan06,wap  written

*/

Component	INCLUDE_MOT_FEC_HEND {
	NAME		motFecHEnd Hierarchical Enhanced Network Driver
	SYNOPSIS	motFec hEnd
	MODULES		motFecHEnd.o
	PROTOTYPE	void motFecHEndRegister (void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	motFecHEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_BCM52XXPHY \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}


