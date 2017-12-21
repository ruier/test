/* 40rtgVxbEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2006,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */
                                                                                
/*
modification history
--------------------
01c,21sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
01b,23jul06,wap  Fix compiler warnings
01a,02jun06,wap  written

*/

Component	INCLUDE_RTL8169_VXB_END {
	NAME		RealTek "Cplus" VxBus Enhanced Network Driver
	SYNOPSIS	RealTek 8139C+/8168/8169/8110/8101E VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	HDR_FILES	../src/hwif/h/end/rtl8169VxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	rtgRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_RTL8169PHY \
			INCLUDE_MV88E1X11PHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

#ifdef	_WRS_CONFIG_LP64
Component	INCLUDE_RTL8169_VXB_END {
	REQUIRES	+= INCLUDE_CACHE_DMA32_LIB
}
#endif	/* _WRS_CONFIG_LP64 */
