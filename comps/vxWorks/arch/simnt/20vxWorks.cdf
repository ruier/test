/* 20vxWorks.cdf - VxWorks components */

/*
 * Copyright (c) 1993, 2003, 2005, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
modification history
--------------------
01f,09jun09,cww  Made coprocLib header private
01e,25jan05,bpn  Added fppArchShow.o module to INCLUDE_HW_FP_SHOW.
01d,03sep03,dbt  Added _INIT_ORDER directory to INCLUDE_COPROCESSOR_SHOW
		 component.
01c,07jul93,pch  Revise for PPC32
01b,06may03,pes  Limit redefinitions to processors which support the
		 coprocessor abstraction
01a,16apr03,pes  written

DESCRIPTION

This file contains updated descriptions for some vxWorks components
defined in 00vxWorks.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

Component INCLUDE_COPROCESSOR_SHOW {
	NAME		coprocessor show routine
	SYNOPSIS	task coprocessor registers
	MODULES		coprocShow.o
	INIT_RTN	coprocShowInit ();
	HDR_FILES	private/coprocLibP.h
	REQUIRES	INCLUDE_COPROCESSOR
	_CHILDREN	FOLDER_SHOW_ROUTINES
	_INIT_ORDER	usrShowInit
}

Component INCLUDE_HW_FP_SHOW {
	NAME		floating point show routine
	SYNOPSIS	task floating point registers
	MODULES		fppArchShow.o
	INIT_RTN	
	HDR_FILES	
	REQUIRES	INCLUDE_COPROCESSOR_SHOW
}
