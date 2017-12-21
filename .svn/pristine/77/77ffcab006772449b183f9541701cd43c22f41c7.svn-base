/* 40vxbIntDynaCtlrLib.cdf - VxBus interrupt controller library cfg */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29jun07,dtr  created 
*/

Component INCLUDE_INTCTLR_DYNAMIC_LIB {
	NAME            VxBus Dynamic Interrupt Controller Library
	SYNOPSIS        VxBus Dynamic Interrupt Controller Library
	HDR_FILES	../src/hwif/intCtlr/vxbIntDynaCtlrLib.h
	_INIT_ORDER	hardWareInterFaceBusInit
	MODULES         vxbIntDynaCtlrLib.o
	INIT_RTN	vxbIntDynaCtlrInit();
	REQUIRES        INCLUDE_VXBUS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_HWIF
}
