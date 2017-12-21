/* 40vxbIntCtlrLib.cdf - VxBus interrupt controller library cfg */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,22oct08,h_k  added INCLUDE_INTCTLR_SHOW component.
01b,31may07,h_k  changed to library build.
01a,04Dec06,tor  created 
*/

Component INCLUDE_INTCTLR_LIB {
    NAME            VxBus Interrupt Controller Library
    SYNOPSIS        VxBus Interrupt Controller Library
    MODULES         vxbIntCtlrLib.o
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_HWIF
}

Component INCLUDE_INTCTLR_SHOW {
	NAME		VxBus Interrupt Controller show routines
	SYNOPSIS	VxBus Interrupt Controller show routines
	MODULES		vxbIntCtlrShow.o
	REQUIRES	INCLUDE_INTCTLR_LIB
	LINK_SYMS	intCtlrHwConfShow
	_CHILDREN	FOLDER_HWIF
}
