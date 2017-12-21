/* 40vxbPpcIntCtlr.cdf - PowerPC CPU interrupt controller */

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
01d,04jun08,h_k  added vxbPpcIntCtlrIsr.o to MODULES attribute.
01c,15aug07,h_k  converted to library build.
01b,10jan07,pch  update for unified vxbPpcIntCtlr.c
01a,04Dec06,tor  created
*/

Component DRV_INTCTLR_PPC {
	NAME		PowerPC Interrupt Controller driver
	SYNOPSIS	PowerPC Interrupt Controller driver
	MODULES		vxbPpcIntCtlr.o \
			vxbPpcIntCtlrIsr.o
	SOURCE		$(WIND_BASE)/target/src/arch/ppc
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	ppcIntCtlrRegister();
	PROTOTYPE	void ppcIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
