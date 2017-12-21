/* 40qefccHEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2005-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01d,04jun07,h_k  removed CONFIGLETTES
01c,08aug06,rcs  Added INCLUDE_DMA_SYS to REQUIRES
01b,23sep05,agf  add parameters
01a,06jul05,tor  written
*/

Component	INCLUDE_QE_FCC_HEND {
	NAME		qeFccHEnd Hierarchical Enhanced Network Driver
	SYNOPSIS	qeFcc hEnd
	MODULES		qeFccHEnd.o
	PROTOTYPE	void qeFccHEndRegister (void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	qeFccHEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS \
			INCLUDE_QUICC_ENGINE_UTILS
	INIT_AFTER	INCLUDE_PLB_BUS
	CFG_PARAMS	QE_FCC_USR_FLAGS
}

Parameter QE_FCC_USR_FLAGS {
	NAME		User flags to specify operation of interface
	SYNOPSIS	enable stats and put into GMII mode 
	TYPE		int
	DEFAULT		QE_FCC_USR_MODE_GMII | QE_FCC_USR_STAT_ENABLE
}


