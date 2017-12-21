/* 02shell_commands.cdf - Kernel shell commands components */

/*
 * Copyright (c) 2005-2007, 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01g,25jan12,cww  Fix typo in INCLUDE_MEM_EDR_SHELL_CMD synopsis (WIND00048161)
01f,27sep11,rlp  Added INCLUDE_SPY_SHELL_CMD - CQID: WIND00285819.
01e,01apr11,jmp  Added INCLUDE_TIP_CMD & INCLUDE_WRLOAD_CMD (CQ:WIND00144662).
01d,14sep07,vhe  Improved component description as part of CQ:WIND00102457 fix
01c,18jul07,v_r  Added shell history saving/loading mechanism (req
                 CQ:WIND00005098).
01b,12jan06,zl   added INCLUDE_MAPPED_FILES_SHOW_SHELL_CMD
01a,31jan05,bpn  Written from version 01q,25jan05,bpn of 01shell.cdf.
*/

/*
DESCRIPTION

This file contains the definition for the commands components for the 
shell command interpreter.

The folder FOLDER_SHELL_CMD is defined in 01shell.cdf. 
*/

Component INCLUDE_DISK_UTIL_SHELL_CMD {
	NAME		file system shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to file system.
        PROTOTYPE       IMPORT STATUS usrFsShellCmdInit ();
	MODULES		usrFsShellCmd.o
	INIT_RTN	usrFsShellCmdInit ();
	REQUIRES	INCLUDE_DISK_UTIL INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_DISK_UTIL INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_EDR_SHELL_CMD {
	NAME		ED&R shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to ED&R.
        PROTOTYPE       IMPORT STATUS edrShellCmdInit ();
	MODULES		edrShellCmd.o
	INIT_RTN	edrShellCmdInit ();
	REQUIRES	INCLUDE_EDR_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_EDR_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_TASK_SHELL_CMD {
	NAME		task shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to tasks.
        PROTOTYPE       IMPORT STATUS taskShellCmdInit ();
	MODULES		taskShellCmd.o
	INIT_RTN	taskShellCmdInit ();
	REQUIRES	INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_DEBUG_SHELL_CMD {
	NAME		debug shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to debugging.
        PROTOTYPE       IMPORT STATUS dbgShellCmdInit ();
	MODULES		dbgShellCmd.o
	INIT_RTN	dbgShellCmdInit ();
	REQUIRES	INCLUDE_DEBUG INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_DEBUG INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_SYM_SHELL_CMD {
	NAME		symbol shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to symbol access.
        PROTOTYPE       IMPORT STATUS symShellCmdInit ();
	MODULES		symShellCmd.o
	INIT_RTN	symShellCmdInit ();
	REQUIRES	INCLUDE_SYM_TBL INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_SYM_TBL INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_VM_SHOW_SHELL_CMD {
	NAME		virtual memory show shell commands
	SYNOPSIS	Virtual Memory show commands for the shell command interpreter. 
	MODULES		vmShowShellCmd.o
	PROTOTYPE	STATUS vmShowShellCmdInit (void);
	INIT_RTN	vmShowShellCmdInit ();
	HDR_FILES	vmLib.h
	REQUIRES	INCLUDE_VM_SHOW INCLUDE_SHELL_INTERP_CMD 
	INCLUDE_WHEN	INCLUDE_VM_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_ADR_SPACE_SHELL_CMD {
	NAME		address space shell commands
	SYNOPSIS	Address space commands for the shell command interpreter. 
	MODULES		adrSpaceShellCmd.o
	PROTOTYPE	STATUS adrSpaceShellCmdInit (void);
	INIT_RTN	adrSpaceShellCmdInit ();
	HDR_FILES	adrSpaceLib.h
	REQUIRES	INCLUDE_ADR_SPACE_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_ADR_SPACE_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_SHARED_DATA_SHOW_SHELL_CMD {
	NAME		shared data show shell commands
	SYNOPSIS	Shared data show commands for the shell command interpreter. 
	MODULES		sdShowShellCmd.o
	PROTOTYPE	STATUS sdShowShellCmdInit (void);
	INIT_RTN	sdShowShellCmdInit ();
	HDR_FILES	sdLib.h
	REQUIRES	INCLUDE_SHARED_DATA_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_SHARED_DATA_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_MAPPED_FILES_SHOW_SHELL_CMD {
	NAME		mapped files shell commands
	SYNOPSIS	mapped files commands for the shell command interpreter. 
	MODULES		mmanShowShellCmd.o
	PROTOTYPE	STATUS mmanShowShellCmdInit (void);
	INIT_RTN	mmanShowShellCmdInit ();
	REQUIRES	INCLUDE_MAPPED_FILES_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_MAPPED_FILES_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_MEM_EDR_SHELL_CMD {
	NAME		memory ED&R shell commands
	SYNOPSIS	Memory ED&R commands for the shell command interpreter. 
	MODULES		memEdrShellCmd.o
	PROTOTYPE	STATUS memEdrShellCmdInit (void);
	INIT_RTN	memEdrShellCmdInit ();
	REQUIRES	INCLUDE_MEM_EDR_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_MEM_EDR_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_MEM_EDR_RTP_SHELL_CMD {
	NAME		memory ED&R shell commands related to processes
	SYNOPSIS	Memory ED&R shell commands related to processes for the shell command interpreter.
	MODULES		memEdrRtpShellCmd.o
	PROTOTYPE	STATUS memEdrRtpShellCmdInit (void);
	INIT_RTN	memEdrRtpShellCmdInit ();
	REQUIRES	INCLUDE_MEM_EDR_RTP_SHOW INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_MEM_EDR_RTP_SHOW INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_MODULE_SHELL_CMD {
        NAME            target loader shell command
	SYNOPSIS	Target loader commands for the shell command interpreter.
        PROTOTYPE       extern STATUS moduleShellCmdInit (void);
        INIT_RTN        moduleShellCmdInit();
        MODULES         moduleShellCmd.o
        REQUIRES        INCLUDE_LOADER INCLUDE_SHELL_INTERP_CMD
        INCLUDE_WHEN    INCLUDE_LOADER INCLUDE_SHELL_INTERP_CMD
        _CHILDREN       FOLDER_SHELL_CMD
}

Component INCLUDE_UNLOADER_SHELL_CMD {
        NAME            unloader shell command
	SYNOPSIS	Unloader commands for the shell command interpreter. 
        PROTOTYPE       extern STATUS unloadShellCmdInit (void);
        INIT_RTN        unloadShellCmdInit();
        MODULES         unloadShellCmd.o
        REQUIRES        INCLUDE_UNLOADER INCLUDE_MODULE_SHELL_CMD
        INCLUDE_WHEN    INCLUDE_UNLOADER INCLUDE_SHELL_INTERP_CMD
        _CHILDREN       FOLDER_SHELL_CMD
}

Component INCLUDE_SHL_SHELL_CMD {
	NAME		shared library commands
	SYNOPSIS	List of SHL commands for the shell command interpreter
	MODULES		shlShellCmd.o
	INIT_RTN	shlShellCmdInit ();
	HDR_FILES	shlLib.h
	REQUIRES	INCLUDE_SHL INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_SHL INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_RTP_SHELL_CMD {
	NAME		process shell commands
	SYNOPSIS	List of commands for the shell command interpreter related to processes.
	MODULES		rtpShellCmd.o
	INIT_RTN	rtpShellCmdInit ();
	HDR_FILES	rtpLib.h
	REQUIRES	INCLUDE_RTP INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_RTP INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_RTP_SHOW_SHELL_CMD {
	NAME		process show shell commands
	SYNOPSIS	List of process show commands for the shell command interpreter. 
	MODULES		rtpShowShellCmd.o
	PROTOTYPE	STATUS rtpShowShellCmdInit (void);
	INIT_RTN	rtpShowShellCmdInit ();
	HDR_FILES	rtpLib.h
	REQUIRES	INCLUDE_RTP_SHELL_CMD
	INCLUDE_WHEN	INCLUDE_RTP_SHELL_CMD INCLUDE_RTP_SHOW	
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_HISTORY_FILE_SHELL_CMD {
	NAME            shell history saving/loading commands
        SYNOPSIS        Saving and loading commands for the shell command interpreter history.
        MODULES         histShellCmd.o
        INIT_RTN        histShellCmdInit ();
        HDR_FILES       private/shellHistLibP.h
        REQUIRES        INCLUDE_SHELL_HISTORY_FILE
        INCLUDE_WHEN    INCLUDE_SHELL_INTERP_CMD INCLUDE_SHELL_HISTORY_FILE
        _CHILDREN       FOLDER_SHELL_CMD
}

Component INCLUDE_SPY_SHELL_CMD {
	NAME		Spy CPU activity commands
	SYNOPSIS	Shell commands for spy library
	MODULES		spyShellCmd.o
	INIT_RTN	spyShellCmdInit ();
        HDR_FILES       private/shellCmdP.h
	REQUIRES	INCLUDE_SPY INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_SPY INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_TIP_CMD {
	NAME		Serial line connection commands
	SYNOPSIS	tip shell command line support
	MODULES		tipShellCmd.o
	INIT_RTN	tipShellCmdInit ();
        HDR_FILES       private/shellCmdP.h
	REQUIRES	INCLUDE_TIP INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_TIP INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

Component INCLUDE_WRLOAD_CMD {
	NAME		Wind River multi-cpu boot loader commands
	SYNOPSIS	Commands for loading and starting an image in a multi-cpu system.
	MODULES		wrloadShellCmd.o
	INIT_RTN	wrloadShellCmdInit ();
        HDR_FILES       private/shellCmdP.h
	REQUIRES	INCLUDE_WRLOAD INCLUDE_SHELL_INTERP_CMD
	INCLUDE_WHEN	INCLUDE_WRLOAD INCLUDE_SHELL_INTERP_CMD
	_CHILDREN	FOLDER_SHELL_CMD
}

InitGroup usrShellCmdInit {
	INIT_RTN	usrShellCmdInit ();
	SYNOPSIS	The kernel shell commands initialization sequence
	INIT_ORDER	INCLUDE_TASK_SHELL_CMD \
			INCLUDE_DEBUG_SHELL_CMD \
			INCLUDE_SYM_SHELL_CMD \
			INCLUDE_DISK_UTIL_SHELL_CMD \
			INCLUDE_EDR_SHELL_CMD \
			INCLUDE_VM_SHOW_SHELL_CMD \
			INCLUDE_ADR_SPACE_SHELL_CMD \
			INCLUDE_SHARED_DATA_SHOW_SHELL_CMD \
			INCLUDE_MAPPED_FILES_SHOW_SHELL_CMD \
			INCLUDE_MEM_EDR_SHELL_CMD \
			INCLUDE_MEM_EDR_RTP_SHELL_CMD \
                        INCLUDE_MODULE_SHELL_CMD \
			INCLUDE_UNLOADER_SHELL_CMD \
			INCLUDE_RTP_SHELL_CMD \
			INCLUDE_RTP_SHOW_SHELL_CMD \
			INCLUDE_SHL_SHELL_CMD \
			INCLUDE_HISTORY_FILE_SHELL_CMD \
			INCLUDE_SPY_SHELL_CMD \
			INCLUDE_TIP_CMD	\
			INCLUDE_WRLOAD_CMD
	_INIT_ORDER	usrShellInit
	INIT_AFTER	INCLUDE_SHELL_INTERP_CMD
	INIT_BEFORE	INCLUDE_STARTUP_SCRIPT
}
