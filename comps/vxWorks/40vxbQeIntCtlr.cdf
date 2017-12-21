/* 40vxbQeIntCtlr.cdf - QE interrupt controller */

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
01a,04jun07,dtr  created
*/

Component DRV_INTCTLR_QE {
	NAME		Quicc Engine Interrupt Controller driver
	SYNOPSIS	Quicc Engine Interrupt Controller driver
	MODULES		vxbQeIntCtlr.o
	INIT_RTN	vxbQeIntCtlrRegister();
	PROTOTYPE	void vxbQeIntCtlrRegister(void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS


}
