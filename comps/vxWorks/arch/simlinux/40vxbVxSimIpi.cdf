/* 40vxbVxSimIpi.cdf - configuration file for VxSim IPI driver */
                                                                                
/* Copyright (c) 2007,2009 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01c,22apr09,elp  include when required
01b,11sep07,elp  adding PROTOTYPE to fix warning.
01a,10jul07,tor  created
*/

Component	DRV_INTCTLR_VXSIM {
	NAME		VxSim IntCtlr driver
	SYNOPSIS	VxSim IntCtlr driver to generate IPIs
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbVxSimIntCtlrRegister();
	REQUIRES	INCLUDE_PLB_BUS
	_REQUIRES	INCLUDE_VXIPI
	INIT_AFTER	INCLUDE_PLB_BUS
	PROTOTYPE       void vxbVxSimIntCtlrRegister (void);
}

