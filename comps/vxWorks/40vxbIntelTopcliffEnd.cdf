/* 40vxbIntelTopcliffEnd.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2010, 2011 Wind River Systems, Inc. */
                                                                               
/*
modification history
--------------------
01b,04dec11,wyt  enclose DRV_VXBEND_INTELTOPCLIFF with macro
01a,29jul10,wap  written
*/

#ifndef _WRS_CONFIG_LP64
Component	DRV_VXBEND_INTELTOPCLIFF {
	NAME		Intel Topcliff Enhanced Network Driver
	SYNOPSIS	Intel Topcliff Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbIntelTopcliffEnd.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE	void vxbIntelTopcliffEndRegister(void);
	INIT_RTN	vxbIntelTopcliffEndRegister();
	MODULES		vxbIntelTopcliffEnd.o
	REQUIRES	INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS \
			DRV_RESOURCE_INTELTOPCLIFF_IOH
	INIT_AFTER	INCLUDE_PCI_BUS
}
#endif /* !_WRS_CONFIG_LP64 */
