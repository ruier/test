/* 40tsecHEnd.cdf - Component configuration file */

/* 
 * Copyright (c) 2005-2007 Wind River Systems, Inc. 
 * 
 * The right to copy, distribute, modify, or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
01e,04jun07,h_k  removed CONFIGLETTES
01d,06mar07,b_m  add INCLUDE_END to REQUIRES (WIND00071500)
01c,08aug06,rcs  Added INCLUDE_DMA_SYS to REQUIRES
01b,23sep05,agf  add parameters
01a,06jul05,tor  written

*/

Component	INCLUDE_MOT_TSEC_HEND {
	NAME		motTsecHEnd Hierarchical Enhanced Network Driver
	SYNOPSIS	motTsec hEnd
	MODULES		motTsecHEnd.o
	PROTOTYPE	void motTsecHEndRegister (void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	motTsecHEndRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_BCM54XXPHY \
			INCLUDE_MV88E1X11PHY \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
	CFG_PARAMS	MOT_TSEC_USR_FLAGS
}

Parameter MOT_TSEC_USR_FLAGS {
	NAME		User flags to specify operation of interface
	SYNOPSIS	enable stats and put into GMII mode 
	TYPE		int
	DEFAULT		MOT_TSEC_USR_MODE_GMII | MOT_TSEC_USR_STAT_ENABLE
}


