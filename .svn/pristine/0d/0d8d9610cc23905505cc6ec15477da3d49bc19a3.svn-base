/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2004,2005,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,20jan10,dbt  Added VXSIM_USER_SM_SIZE parameter.
01b,22apr05,dr   SPR108100 - adding aimFppLib.o and fppArchLib.o to
		 INCLUDE_HW_FP
01a,08nov04,jmp  written from 20vxWorks.cdf version 01e (SPR #103353).

DESCRIPTION
This file contains updated descriptions for some vxWorks components
defined in 00bsp.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

Component INCLUDE_COPROCESSOR {
	NAME		coprocessor
	SYNOPSIS	generalized coprocessor support
	MODULES		coprocLib.o
	INIT_RTN	coprocLibInit();
	CONFIGLETTES	usrCoprocs.c
	_CHILDREN	FOLDER_PERIPHERALS
	REQUIRES	INCLUDE_KERNEL
	_INIT_ORDER	usrIosCoreInit
}

Component INCLUDE_HW_FP {
	NAME		hardware fpp support
	SYNOPSIS	Modified definitions for generalized coprocessor
	MODULES		fppArchLib.o aimFppLib.o
	INIT_RTN	
	HDR_FILES	
	REQUIRES	INCLUDE_COPROCESSOR
}

/*
 * Simulators do no support AIM, so remove AIM from MMU Folder.
 */

Folder FOLDER_MMU {
    CHILDREN -= FOLDER_AIM_CONFIG
}

Component INCLUDE_VXSIM_USER_SM {
	NAME		VxSim user shared memory
	SYNOPSIS	Add VxSim user reserved shared memory support
	CFG_PARAMS	VXSIM_USER_SM_SIZE
	_CHILDREN	FOLDER_MEMORY
}

Parameter VXSIM_USER_SM_SIZE {
	NAME		VxSim user shared memory size
	SYNOPSIS	Size in bytes of the VxSim user shared memory
	TYPE		uint
	DEFAULT		0x2000
}
