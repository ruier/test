/* 40vxbEpicIntCtlr.cdf - EPIC interrupt controller */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01c,02jun08,h_k  split vxbEpicIntCtlr.c.
01b,18jun07,pmr  convert to standalone build
01a,04Dec06,tor  created
*/

Component DRV_INTCTLR_EPIC {
	NAME		EPIC Interrupt Controller driver
	SYNOPSIS	EPIC Interrupt Controller driver
	MODULES		vxbEpicIntCtlr.o \
			vxbEpicIntCtlrIsr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbEpicIntCtlrRegister();
	PROTOTYPE	void vxbEpicIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
