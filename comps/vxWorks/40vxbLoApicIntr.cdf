/* 40vxbLoApicIntr.cdf - Local APIC interrupt controller */

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
01b,19jun07,h_k  changed to library build.
01a,08May07,scm  created
*/

Component DRV_INTCTLR_LOAPIC {
	NAME		Local APIC Interrupt Controller driver
	SYNOPSIS	Local APIC Interrupt Controller driver
	MODULES		vxbLoApicIntr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbLoApicIntrDrvRegister();
	PROTOTYPE	void vxbLoApicIntrDrvRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
