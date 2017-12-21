/* 40sbeVxbEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01c,21may08,jmt  Add requirement for INCLUDE_END component to
                 INCLUDE_SBE_VXB_END
01b,22feb07,jmt  Fix compiler warning due to lack of prototype
01a,08jul06,wap  written

*/

Component	INCLUDE_SBE_VXB_END {
	NAME		Broadcom SB1 VxBus Enhanced Network Driver
	SYNOPSIS	Broadcom SB1 VxBus END driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void sbeRegister(void);
	INIT_RTN	sbeRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_BCM54XXPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
                        INCLUDE_END
	INIT_AFTER	INCLUDE_PLB_BUS
}

