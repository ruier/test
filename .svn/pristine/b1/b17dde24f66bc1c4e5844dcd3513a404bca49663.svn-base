/* 40vxbArmSP804Timer.cdf - Component configuration file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,27jun11,ggm   written
*/

Component	DRV_TIMER_ARM_SP804 {
	NAME		ARM SP804 Timer Driver
	SYNOPSIS	ARM SP804 Timer Driver
	MODULES		vxbArmSP804Timer.o
	PROTOTYPE	IMPORT void vxbArmSP804TimerDrvRegister(void);
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbArmSP804TimerDrvRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_TIMER_SYS
}

