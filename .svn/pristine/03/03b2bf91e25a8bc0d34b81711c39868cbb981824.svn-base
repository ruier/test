/* 40vxbI8259IntCtlr.cdf - I8259 interrupt controller */

/*
* Copyright (c) 2007 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/

/*
modification history
--------------------
01a,07Jun07,ami  created
*/

Component DRV_INTCTLR_I8259 {
	NAME		Intel 8259A Interrupt Controller driver
	SYNOPSIS	Intel 8259A Interrupt Controller driver
	MODULES         vxbI8259IntCtlr.o
	PROTOTYPE       IMPORT void vxbI8259IntCtlrRegister (void);
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbI8259IntCtlrRegister();
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
