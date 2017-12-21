/* 40quiccEngine.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,26jul07,h_k  changed to library build
01a,16may06,tor  written
*/

Component	INCLUDE_QUICC_ENGINE_UTILS {
	NAME		quicc engine utilities
	SYNOPSIS	quicc engine utility methods
	MODULES		quiccEngineUtils.o
	SOURCE		$(WIND_BASE)/target/src/hwif/resource
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	quiccEngineRegister();
	PROTOTYPE	void quiccEngineRegister (void);
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS 
	INIT_AFTER	INCLUDE_PLB_BUS

}

