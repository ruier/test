/* 10vxIpiLib.cdf - Generic Inter Processor Interrupt utility for SMP/AMP */

/*********************************************************************
*
* Copyright (C) 2007,2010  Wind River Systems, Inc. All rights are reserved.
* 
* The right to copy, distribute, modify, or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,20oct10,elp  fixed init order.
01a,19Jun07,tor  created
*/

Component INCLUDE_VXIPI {
        NAME            vxIpiLib
        SYNOPSIS        Inter Processor Interrupts for SMP and AMP
	PROTOTYPE	STATUS vxIpiLibInit (void);
        INIT_RTN        vxIpiLibInit ();
        MODULES         vxIpiLib.o
        INCLUDE_WHEN    INCLUDE_CPC
        REQUIRES        INCLUDE_VXBUS INCLUDE_INTCTLR_LIB
        _INIT_ORDER     usrRoot
	INIT_BEFORE	INCLUDE_CPC
	INIT_AFTER	INCLUDE_SYSHW_INIT2
	_CHILDREN	FOLDER_UTILITIES
}
