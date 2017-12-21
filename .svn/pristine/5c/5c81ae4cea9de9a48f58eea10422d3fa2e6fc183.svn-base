/* 40geiHEnd.cdf - Component configuration file */

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
01d,31may07,h_k  changed to library build.
01c,08aug06,rcs  Added INCLUDE_DMA_SYS to REQUIRES
01b,21feb06,pmr  SPR 117805.
01a,23sep05,agf  copyright and mod history added; parameters added
*/

Component	INCLUDE_GEI_HEND {
	NAME		geiHEnd Hierarchical Enhanced Network Driver
	SYNOPSIS	gei hEnd
	MODULES		geiHEnd.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	geiHEndRegister();
	PROTOTYPE	void geiHEndRegister (void);
	REQUIRES	INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
	CFG_PARAMS	GEI_OFFSET_VALUE \
                        GEI_JUMBO_MTU_VALUE
}

Parameter GEI_OFFSET_VALUE {
	NAME		Network to Host alignment byte offset
	SYNOPSIS	archs which require 4-byte alignment for Integer, use 2
	TYPE		int
	DEFAULT		0
}

Parameter GEI_JUMBO_MTU_VALUE {
	NAME		Jumbo frame MTU size
	SYNOPSIS	max num of bytes in a jumbo MTU; driver will default to 9000 if 0 is specified
	TYPE		int
	DEFAULT		0
}

