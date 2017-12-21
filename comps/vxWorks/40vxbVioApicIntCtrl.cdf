/* 40vxbVioApicIntCtrl.cdf - Virtual IO APIC interrupt controller */

/*
 * Copyright (c) 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01n,28apr09,rec  guest Os merge from vxWorks 6.7.0
01a,08May07,mmi  created
*/

Component DRV_INTCTRL_VIOAPIC {
	NAME		Virtual IO APIC Interrupt Controller driver
	SYNOPSIS	Vitual IO APIC Interrupt Controller driver
	MODULES		vxbVioApicIntr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN	FOLDER_DRIVERS
	INIT_RTN	vxbVioApicIntrDrvRegister();
	PROTOTYPE	void vxbVioApicIntrDrvRegister (void);
	INIT_AFTER	INCLUDE_PLB_BUS
	_INIT_ORDER	hardWareInterFaceBusInit
}
