/* 40vxbVbIntCtlr.cdf - Virtual board interrupt controller */

/*
* Copyright (c) 2008 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/

/*
modification history
--------------------
01a,16oct08,dtr  created
*/

Component DRV_INTCTLR_VB {
	NAME		Virtual Board Interrupt Controller driver
	SYNOPSIS	Virtual Board Interrupt Controller driver
	MODULES		vxbVbIntCtlr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN	FOLDER_DRIVERS
	INIT_RTN	vxbVbIntCtlrRegister();
	PROTOTYPE	void vxbVbIntCtlrRegister (void);
	INIT_AFTER	INCLUDE_PLB_BUS
	_INIT_ORDER	hardWareInterFaceBusInit
}
