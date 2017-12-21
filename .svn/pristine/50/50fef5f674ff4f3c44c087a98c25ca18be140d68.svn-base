/* 30vxbEndQctrl.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01a,10oct07,dlk  written

*/

Component	INCLUDE_VXBUS_END_QCTRL {
	NAME		Utility to set a VxBus END's job queue
	SYNOPSIS	Utility to set a VxBus END's job queue
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	hwif/util/vxbEndQctrl.h
	_INIT_ORDER	usrRoot
	INIT_RTN	vxbEndQctrlInit ();
	REQUIRES	INCLUDE_PARAM_SYS
	INIT_AFTER	usrKernelExtraInit
}
