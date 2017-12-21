/* 40feiVxbEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2006-2007,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */
                                                                                
/*
modification history
--------------------
01d,16sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
01c,15jun07,h_k  added dependency to INCLUDE_END.
01b,23jul06,wap  Silence compiler warnings
01a,01jun06,wap  written

*/

Component	INCLUDE_FEI8255X_VXB_END {
	NAME		feiVxbEnd Intel PRO/100 VxBus Enhanced Network Driver
	SYNOPSIS	fei vxBus END driver
	MODULES		fei8255xVxbEnd.o
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/fei8255xVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	feiRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PCI_BUS
}

#ifdef	_WRS_CONFIG_LP64
Component	INCLUDE_FEI8255X_VXB_END {
	REQUIRES	+= INCLUDE_CACHE_DMA32_LIB
}
#endif	/* _WRS_CONFIG_LP64 */
