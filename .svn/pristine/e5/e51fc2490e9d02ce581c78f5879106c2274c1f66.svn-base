/* 20vxWorks.cdf - VxWorks components */

/*
 * Copyright (c) 1993, 2003, 2005-2006, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
modification history
--------------------
01g,09jun09,cww  Made coprocLib header private
01f,24apr06,ajc	 INCLUDE_ANSI_STRLEN & INCLUDE_ANSI_STRCAT for exception
		 handling (SPR 119277).
01e,17mar05,asa  added fppArchShow.o as the MODULE for INCLUDE_HW_FP_SHOW
                 updated copyright to 2005
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

Component INCLUDE_EXC_HANDLING {
	REQUIRES	+= INCLUDE_ANSI_STRCAT \
			INCLUDE_ANSI_STRLEN
}

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
