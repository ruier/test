/* 40etsecHEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2005-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,04jun07,h_k  removed CONFIGLETTES
01d,08aug06,rcs  Removed INCLUDE_VSC82XXPHY from REQUIRES
01c,08aug06,rcs  Added INCLUDE_DMA_SYS to REQUIRES 
01b,07feb06,wap  Replace INCLUDE_HEND_PARAM_SYS with INCLUDE_PARAM_SYS
01a,28jul05,dtr  written
*/

Component	INCLUDE_MOT_ETSEC_HEND {
	NAME		motEtsecHEnd Hierarchical Enhanced Network Driver
	SYNOPSIS	motEtsec hEnd
	MODULES		etsecHEnd.o
	PROTOTYPE	void motEtsecHEndRegister (void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	motEtsecHEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PLB_BUS
}

