/* 40vxbIntelTopcliffIoh.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2010, 2011 Wind River Systems, Inc. */
                                                                               
/*
modification history
--------------------
01b,04dec11,wyt  enclose DRV_RESOURCE_INTELTOPCLIFF_IOH with macro
01a,29jul10,wap  written
*/

#ifndef _WRS_CONFIG_LP64
Component	DRV_RESOURCE_INTELTOPCLIFF_IOH {
	NAME		Intel Topcliff I/O hub Driver
	SYNOPSIS	Intel Topcliff I/O hub Driver
	HDR_FILES	../src/hwif/h/resource/vxbIntelTopcliffIoh.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void iohRegister(void);
	INIT_RTN	iohRegister();
	MODULES		vxbIntelTopcliffIoh.o
	REQUIRES	INCLUDE_PCI_BUS
	INIT_AFTER	INCLUDE_PCI_BUS
}
#endif /* !_WRS_CONFIG_LP64 */
