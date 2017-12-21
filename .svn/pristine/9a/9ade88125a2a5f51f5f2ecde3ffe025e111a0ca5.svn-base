/* 01bundle_shell.cdf - VxWorks kernel shell component bundles */

/*
 * Copyright (c) 2003-2004, 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01e,18jul07,v_r  Added the persistent shell history mechanism (req
                 CQ:WIND00005098).
01d,10may06,tfo  remove FOLDER_BUNDLES
01c,06apr04,bpn  Added INCLUDE_SHELL_EMACS_MODE and INCLUDE_SHELL_VI_MODE
                 components.
01b,11feb04,bpn  Add these components to FOLDER_BUNDLES.
01a,20nov03,fle  written
*/

Bundle BUNDLE_STANDALONE_SHELL {
    NAME	standalone kernel shell
    SYNOPSIS	Kernel shell tool with standalone symbol table
    HELP	shell windsh tgtsvr
    COMPONENTS	INCLUDE_STANDALONE_SYM_TBL \
		INCLUDE_SHELL \
		INCLUDE_SHELL_EMACS_MODE \
		INCLUDE_SHELL_VI_MODE \
		INCLUDE_SHELL_HISTORY_FILE \
		INCLUDE_LOADER \
		INCLUDE_DISK_UTIL \
		INCLUDE_SHOW_ROUTINES \
		INCLUDE_STAT_SYM_TBL \
		INCLUDE_DEBUG \
		INCLUDE_UNLOADER \
		INCLUDE_MEM_SHOW \
		INCLUDE_SYM_TBL_SHOW \
		INCLUDE_CPLUS 
}

Bundle BUNDLE_NET_SHELL {
    NAME	network kernel shell
    SYNOPSIS	Kernel shell tool with networking symbol table
    HELP	shell windsh tgtsvr
    COMPONENTS	INCLUDE_NET_SYM_TBL \
		INCLUDE_SHELL \
		INCLUDE_SHELL_EMACS_MODE \
		INCLUDE_SHELL_VI_MODE \
		INCLUDE_SHELL_HISTORY_FILE \
		INCLUDE_LOADER \
		INCLUDE_DISK_UTIL \
		INCLUDE_SHOW_ROUTINES \
		INCLUDE_STAT_SYM_TBL \
		INCLUDE_DEBUG \
		INCLUDE_UNLOADER \
		INCLUDE_MEM_SHOW \
		INCLUDE_SYM_TBL_SHOW \
		INCLUDE_CPLUS
}
