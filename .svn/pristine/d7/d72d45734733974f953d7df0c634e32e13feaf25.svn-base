/* 40lnPciVxbEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */
                                                                                
/*
modification history
--------------------
01d,15jun07,h_k  added dependency to INCLUDE_END.
01c,23jul06,wap  Fix compiler warnings
01b,14jul06,wap  Fix component name
01a,32jun06,wap  written

*/

Component	INCLUDE_AM79C97X_VXB_END {
	NAME		AMD Am79C97x PCnet/PCI VxBus Enhanced Network Driver
	SYNOPSIS	AMD Am79C97x PCnet/PCI VxBus END driver
	MODULES		am79c97xVxbEnd.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/end/am79c97xVxbEnd.h
	INIT_RTN	lnPciRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PCI_BUS
}

