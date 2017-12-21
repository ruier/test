/* 02mem_edr.cdf - Memory ED&R component description file */

/*
 * Copyright (c) 2004, 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */


/*
modification history
--------------------
01g,27sep11,pcs  Removed INIT_ORDER information for componentd INCLUDE_MEM_EDR
                 and INCLUDE_MEM_EDR2, since now they are explictly
                 initialized in the Initgroup usrRoot.
01f,11aug09,cww  Delayed memEdr initialization
01e,10nov04,zl   remove INCLUDE_WHEN for INCLUDE_MEM_EDR_RTP_SHOW.
01d,05aug04,tam  fixed compilation warning: SPR 100114
01c,29apr04,zl   added INCLUDE_MEM_EDR_RTP_SHOW
01b,18mar04,zl   added INCLUDE_EDR_ERRLOG as required component;
                 added INCLUDE_MEM_EDR_RTC component.
01a,26jan04,zl   written.
*/

/*
DESCRIPTION
This file contains descriptions for the memory error detection and
reporting components.
*/


Component INCLUDE_MEM_EDR {
	NAME		Memory Error Detection and Reporting
	SYNOPSIS	Memory Error Detection and Reporting
	_CHILDREN	FOLDER_TOOLS
	MODULES		memEdrLib.o
	CONFIGLETTES	usrMemEdr.c
	INIT_RTN	memEdrInit ();
	CFG_PARAMS	MEDR_EXTENDED_ENABLE \
			MEDR_FILL_FREE_ENABLE \
			MEDR_FREE_QUEUE_LEN \
			MEDR_BLOCK_GUARD_ENABLE \
			MEDR_POOL_SIZE
	REQUIRES	INCLUDE_EDR_ERRLOG \
			INCLUDE_MEM_EDR2 \
			INCLUDE_MEM_MGR_FULL
}

Component INCLUDE_MEM_EDR2 {
	NAME		Memory Error Detection and Reporting
	SYNOPSIS	Memory Error Detection and Reporting
	_CHILDREN	FOLDER_NOT_VISIBLE
	MODULES		memEdrLib.o
	CONFIGLETTES	usrMemEdr.c
	INIT_RTN	memEdrInit2();
}

Component INCLUDE_MEM_EDR_RTC {
	NAME		Compiler Assisted Run-Time Checker
	SYNOPSIS	Compiler Assisted Run-Time Checker
	_CHILDREN	FOLDER_TOOLS
	MODULES		memEdrRtc.o
	REQUIRES	INCLUDE_MEM_EDR
}

Component INCLUDE_MEM_EDR_SHOW {
        NAME            Memory ED&R Show Routines
        SYNOPSIS        Memory ED&R Show Routines
        MODULES         memEdrShow.o
	INIT_RTN	memEdrShowInit();
	_CHILDREN	FOLDER_SHOW_ROUTINES
	REQUIRES	INCLUDE_MEM_EDR
	_INIT_ORDER	usrShowInit
}

Component INCLUDE_MEM_EDR_RTP_SHOW {
        NAME            Memory ED&R Show Routines for RTPs
        SYNOPSIS        Memory ED&R Show Routines for RTPs
        MODULES         memEdrRtpShow.o
	INIT_RTN	memEdrRtpShowInit();
	PROTOTYPE	STATUS memEdrRtpShowInit (void);
	_CHILDREN	FOLDER_SHOW_ROUTINES
	REQUIRES	INCLUDE_RTP
	_INIT_ORDER	usrShowInit
}

Parameter MEDR_EXTENDED_ENABLE
	{
	NAME		Enable extended memory block info (trace)
	SYNOPSIS	Enable extended memory block info (trace)
        TYPE 		BOOL
        DEFAULT 	FALSE
	}

Parameter MEDR_FILL_FREE_ENABLE
	{
	NAME		Enable filling freed blocks with pattern
	SYNOPSIS	Enable filling freed blocks with pattern
        TYPE 		BOOL
        DEFAULT 	FALSE
	}

Parameter MEDR_FREE_QUEUE_LEN
	{
	NAME		Length of the free block queue
	SYNOPSIS	Length of the free block queue
        TYPE 		uint
        DEFAULT 	64
	}

Parameter MEDR_BLOCK_GUARD_ENABLE
	{
	NAME		Enable allocated block guard zone
	SYNOPSIS	Enable allocated block guard zone
        TYPE 		BOOL
        DEFAULT 	FALSE
	}

Parameter MEDR_POOL_SIZE
	{
	NAME		Size of the memory reserved for database
	SYNOPSIS	Size of the memory reserved for database
        TYPE 		uint
        DEFAULT 	0x100000
	}
