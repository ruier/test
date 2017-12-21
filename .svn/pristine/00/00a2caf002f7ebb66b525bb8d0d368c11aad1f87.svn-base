/* 02core_dump.cdf - core dump component description file */

/*
 * Copyright (c) 2005-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01x,07jan11,jmp  Added dependency between INCLUDE_CORE_DUMP_RTP_FS and
                 INCLUDE_CORE_DUMP_RTP (CQ:WIND00244682).
01w,07dec10,jmp  Added dependency between INCLUDE_CORE_DUMP_RTP and
                 INCLUDE_RTP (CQ:WIND00244682).
01v,15jul09,jmp  Added local or remote file system support for RTP Core Dump
		 storage (INCLUDE_CORE_DUMP_RTP_FS).
01u,03jun09,jmp  Decouple Core Dump facility from WR Diagnostics (REQ
                 CQ:WIND00158693).
01t,16jul08,jmp  Allow to select different compression methods for Kernel
		 and RTP Core Dump.
		 CDF cleanup: created sections for Kernel, RTP and common
		 Core Dump components and parameters.
01s,19may08,jmp  Adding missing init routine for INCLUDE_CORE_DUMP_RTP.
01r,14apr08,jmp  Reworked core dump components init,
		 Added RTP Core Dump component (REQ CQ:WIND00005075).
01q,21feb08,jmp  updated CORE_DUMP_MAX_HOOKS parameter description (REQ
                 CQ:WIND00105547).
01p,16nov07,jmp  fixed spelling error in NAME of CORE_DUMP_CKSUM_ENABLE
                 parameter (CQ:WIND00108358).
01o,31aug07,jmp  made INCLUDE_CORE_DUMP_MEM_POOL and INCLUDE_CORE_DUMP_UTIL
                 components invisible (CQ:WIND00103252).
01n,15oct06,jmp  added INCLUDE_CORE_DUMP_MEM_POOL component and parameters.
01m,05oct06,jmp  fixed dependencies to prevent CORE_DUMP_SHOW from beeing
                 included when CORE_DUMP_STREAM_DEV is included
                 (CQ:WIND00067876).
01l,20apr06,jmp  added INCLUDE_CORE_DUMP_UTIL component.
		 added INCLUDE_CORE_DUMP_STREAM_DEV component.
01k,28feb06,jmp  removed reference to SELECT_CORE_DUMP_COMPRESS_METHOD (SPR
                 #118174).
01j,24jan06,jmp  removed core dump parent folder definition (The core dump
		 parent folder is now define by Wind River Diagnostic cdf).
		 moved CORE_DUMP_SHOW to FOLDER_CORE_DUMP.
01i,02dec05,jmp  added core dump compression selection method.
		 added CORE_DUMP_CKSUM_ENABLE parameter.
01h,24nov05,jmp  added CORE_DUMP_SKIP_USER_RESERVED_MEM parameter.
01g,07oct05,jmp  fixed naming of Raw device component (SPR #113441).
01f,07sep05,jmp  Added CORE_DUMP_STACK_SIZE parameter.
	    dbt  Added CORE_DUMP_SKIP_TEXT_SECTION parameter.
01e,05aug05,jmp  added KERNEL_APPL_CORE_DUMP_ENABLE.
01d,04aug05,jmp  removed CORE_DUMP_NUM_MAX parameter.
01c,03aug05,dbt  Added CORE_DUMP_MAX_HOOKS parameter.
01b,29jul05,jmp  added memory filtering facility.
		 removed CORE_DUMP_DEVICE_BLK_SIZE and CORE_DUMP_CACHE_SIZE
		 parameters.
01a,18jul05,jmp  written.
*/

/*
DESCRIPTION
This file contains descriptions for the core dump components.
*/

/*******************************************************************************
*
* Core Dump components Folder
*
*/

Folder FOLDER_CORE_DUMP {
	NAME		Core Dump components
	SYNOPSIS	The VxWorks Core Dump facility
	CHILDREN	FOLDER_CORE_DUMP_KERNEL			\
			FOLDER_CORE_DUMP_RTP
	DEFAULTS	FOLDER_CORE_DUMP_KERNEL
	_CHILDREN	FOLDER_TOOLS
}

/*******************************************************************************
*
* Kernel Core Dump components
*
*/

Folder FOLDER_CORE_DUMP_KERNEL {
	NAME		Kernel Core Dump components
	SYNOPSIS	VxWorks Kernel Core Dump facility
	CHILDREN	INCLUDE_CORE_DUMP			\
			SELECT_CORE_DUMP_STORAGE_METHOD		\
			SELECT_CORE_DUMP_COMPRESS_ALGORITHM	\
			INCLUDE_CORE_DUMP_SHOW			\
			INCLUDE_CORE_DUMP_MEM_FILTER
	DEFAULTS	INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_MEM_FILTER
}

Component INCLUDE_CORE_DUMP {
	NAME		Kernel Core Dump
	SYNOPSIS	Support for Kernel Core Dump
	MODULES		coreDumpLib.o
	CONFIGLETTES	usrCoreDump.c
	INIT_RTN	usrCoreDumpInit ();
	CFG_PARAMS	FATAL_SYSTEM_CORE_DUMP_ENABLE		\
			KERNEL_APPL_CORE_DUMP_ENABLE		\
			CORE_DUMP_MAX_HOOKS			\
			CORE_DUMP_SKIP_TEXT_SECTION		\
			CORE_DUMP_SKIP_USER_RESERVED_MEM	\
			CORE_DUMP_STACK_SIZE			\
			CORE_DUMP_CKSUM_ENABLE
	REQUIRES	SELECT_CORE_DUMP_STORAGE_METHOD		\
			INCLUDE_CORE_DUMP_MEM_FILTER		\
			INCLUDE_CORE_DUMP_MEM_POOL		\
			INCLUDE_OFFSET_SYMBOLS
}

Selection SELECT_CORE_DUMP_COMPRESS_ALGORITHM {
	NAME		Kernel Core Dump Compression Method
	SYNOPSIS	Select the Kernel Core Dump compression library
	COUNT		1-1
	CHILDREN	INCLUDE_CORE_DUMP_COMPRESS		\
			INCLUDE_CORE_DUMP_COMPRESS_RLE
	DEFAULTS	INCLUDE_CORE_DUMP_COMPRESS_RLE
}

Component INCLUDE_CORE_DUMP_COMPRESS {
	NAME		Kernel Core Dump ZLib Compression
	SYNOPSIS	Use ZLib for Kernel Core Dump Compression
	CFG_PARAMS	CORE_DUMP_COMPRESSION_LEVEL
	REQUIRES	INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_COMMON_COMPRESS_ZLIB
}

Component INCLUDE_CORE_DUMP_COMPRESS_RLE {
	NAME		Kernel Core Dump RLE Compression
	SYNOPSIS	Use RLE for Kernel Core Dump Compression
	REQUIRES	INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_COMMON_COMPRESS_RLE
}

Component INCLUDE_CORE_DUMP_SHOW {
	NAME		Kernel Core Dump Show routines
	SYNOPSIS	Show routines for Kernel Core Dump facility
	MODULES		coreDumpShow.o
	REQUIRES	INCLUDE_CORE_DUMP_UTIL
	INCLUDE_WHEN	INCLUDE_CORE_DUMP			\
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_CORE_DUMP_MEM {
	NAME		Kernel Core Dump Memory storage device support
	SYNOPSIS	Save Kernel Core Dump to persistent memory
	MODULES		coreDumpMemDevLib.o
	CFG_PARAMS	CORE_DUMP_REGION_SIZE
	REQUIRES	INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_UTIL			\
			INCLUDE_EDR_PM
}

Component INCLUDE_CORE_DUMP_MEM_FILTER {
	NAME		Kernel Core Dump memory region filtering
	SYNOPSIS	Provide the ability to exclude memory region from Kernel Core Dump
	MODULES		coreDumpMemFilterLib.o
	CFG_PARAMS	CORE_DUMP_MEM_FILTER_MAX
	REQUIRES	INCLUDE_CORE_DUMP
}

Component INCLUDE_CORE_DUMP_UTIL {
	NAME		Kernel Core Dump device utilities
	SYNOPSIS	Provide utilities for Kernel Core Dump device management
	MODULES		coreDumpUtilLib.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_CORE_DUMP_RAW_DEV {
	NAME		Kernel Core Dump Generic raw device support (DEPRECATED)
	SYNOPSIS	Configure Kernel Core Dump raw device storage (DEPRECATED: Please use INCLUDE_CORE_DUMP_USER_DEV instead)
	REQUIRES	INCLUDE_CORE_DUMP			\
			INCLUDE_CORE_DUMP_UTIL
}

Selection SELECT_CORE_DUMP_STORAGE_METHOD {
	NAME		Kernel Core Dump Storage Device
	SYNOPSIS	Select how/where to store the Kernel core dumps
	COUNT		1-1
	CHILDREN	INCLUDE_CORE_DUMP_MEM			\
			INCLUDE_CORE_DUMP_RAW_DEV
	DEFAULTS	INCLUDE_CORE_DUMP_MEM
}

/*******************************************************************************
*
* Kernel Core Dump parameters
*
*/

Parameter CORE_DUMP_REGION_SIZE {
        NAME            Kernel Core Dump persistent memory region size
        SYNOPSIS        Size of Persistent Memory to reserve from PM_RESERVED_MEM (By default use remaining free persistent memory)
        TYPE            uint
        DEFAULT         pmFreeSpace(EDR_PM_ARENA)
}

Parameter CORE_DUMP_COMPRESSION_LEVEL {
	NAME		ZLib Kernel Core Dump compression level
	SYNOPSIS	Adjust compression level from 1 (BEST_SPEED) to 9 (BEST_COMPRESSION)
	TYPE		uint
	DEFAULT		9
}

Parameter CORE_DUMP_MEM_FILTER_MAX {
	NAME		Maximum number of kernel memory region filters
	SYNOPSIS	Define the number of kernel memory region filters that can installed
	TYPE		uint
	DEFAULT		10
}

Parameter CORE_DUMP_MAX_HOOKS {
        NAME            Maximum number of Kernel Core Dump hooks
        SYNOPSIS        Define the maximum number of Kernel Core Dump (Creation and Progress hooks)
        TYPE            uint
        DEFAULT         10
}

Parameter FATAL_SYSTEM_CORE_DUMP_ENABLE {
        NAME            Enable Kernel Core Dump generation for fatal system exceptions
        SYNOPSIS        Generate Kernel Core Dump for fatal system exceptions
        TYPE            BOOL
        DEFAULT         TRUE
}

Parameter KERNEL_APPL_CORE_DUMP_ENABLE {
        NAME            Enable Kernel Core Dump generation for kernel application
        SYNOPSIS        Generate Kernel Core Dump for non-fatal system exceptions
        TYPE            BOOL
        DEFAULT         FALSE
}

Parameter CORE_DUMP_SKIP_TEXT_SECTION {
        NAME            Remove VxWorks text section from Kernel Core Dump
        SYNOPSIS        Remove VxWorks text section from Kernel Core Dump
        TYPE            BOOL
        DEFAULT         TRUE
}

Parameter CORE_DUMP_SKIP_USER_RESERVED_MEM {
        NAME            Remove User Reserved Memory from Kernel Core Dump
        SYNOPSIS        Exclude USER_RESERVED_MEM area from Kernel Core Dump
        TYPE            BOOL
        DEFAULT         FALSE
}

Parameter CORE_DUMP_STACK_SIZE {
        NAME            Kernel Core Dump stack size
        SYNOPSIS        Define the size of stack to use for Kernel Core Dump generation (0 means no stack switch).
        TYPE            uint
        DEFAULT         8192
}

Parameter CORE_DUMP_CKSUM_ENABLE {
        NAME            Enable Kernel Core Dump checksumming facility
        SYNOPSIS        Compute Kernel Core Dump checksum during generation
        TYPE            BOOL
        DEFAULT         FALSE
}

/*******************************************************************************
*
* RTP Core Dump components
*
*/

Folder FOLDER_CORE_DUMP_RTP {
	NAME		RTP Core Dump components
	SYNOPSIS	VxWorks RTP Core Dump facility
	CHILDREN	INCLUDE_CORE_DUMP_RTP			\
			SELECT_CORE_DUMP_RTP_STORAGE_METHOD	\
			SELECT_CORE_DUMP_RTP_COMPRESS_ALGORITHM
	DEFAULTS	INCLUDE_CORE_DUMP_RTP
}

Component INCLUDE_CORE_DUMP_RTP {
	NAME		RTP Core Dump
	SYNOPSIS	Support for RTP Core Dump
	MODULES		coreDumpRtpLib.o
	CONFIGLETTES	usrCoreDump.c
	INIT_RTN	usrCoreDumpInit ();
	_INIT_ORDER     usrRoot
	REQUIRES	INCLUDE_RTP				\
			SELECT_CORE_DUMP_RTP_STORAGE_METHOD	\
			INCLUDE_CORE_DUMP_MEM_POOL
}

Selection SELECT_CORE_DUMP_RTP_COMPRESS_ALGORITHM {
	NAME		RTP Core Dump Compression Method
	SYNOPSIS	Select the RTP Core Dump compression library
	COUNT		1-1
	CHILDREN	INCLUDE_CORE_DUMP_RTP_COMPRESS_ZLIB	\
			INCLUDE_CORE_DUMP_RTP_COMPRESS_RLE
	DEFAULTS	INCLUDE_CORE_DUMP_RTP_COMPRESS_RLE
}

Component INCLUDE_CORE_DUMP_RTP_COMPRESS_ZLIB {
	NAME		RTP Core Dump ZLib Compression
	SYNOPSIS	Use ZLib for RTP Core Dump Compression
	CFG_PARAMS	CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL
	REQUIRES	INCLUDE_CORE_DUMP_RTP			\
			INCLUDE_CORE_DUMP_COMMON_COMPRESS_ZLIB
}

Component INCLUDE_CORE_DUMP_RTP_COMPRESS_RLE {
	NAME		RTP Core Dump RLE Compression
	SYNOPSIS	Use RLE for RTP Core Dump Compression
	REQUIRES	INCLUDE_CORE_DUMP_RTP			\
			INCLUDE_CORE_DUMP_COMMON_COMPRESS_RLE
}

Selection SELECT_CORE_DUMP_RTP_STORAGE_METHOD {
	NAME		RTP Core Dump Storage Device
	SYNOPSIS	Select how/where to store the RTP core dumps
	CHILDREN	INCLUDE_CORE_DUMP_RTP_FS
	DEFAULTS	INCLUDE_CORE_DUMP_RTP_FS
	COUNT		1-1
}

Component INCLUDE_CORE_DUMP_RTP_FS {
	NAME		RTP Core Dump file system storage support
	SYNOPSIS	File system storage for RTP Core Dump files.
	CFG_PARAMS	CORE_DUMP_RTP_FS_PATH
	REQUIRES	INCLUDE_CORE_DUMP_RTP
	INIT_AFTER	INCLUDE_CORE_DUMP_RTP
	_INIT_ORDER	usrRoot
}

/*******************************************************************************
*
* RTP Core Dump parameters
*
*/

Parameter CORE_DUMP_RTP_ZLIB_COMPRESSION_LEVEL {
	NAME		ZLib RTP Core Dump compression level
	SYNOPSIS	Adjust compression level from 1 (BEST_SPEED) to 9 (BEST_COMPRESSION)
	TYPE		uint
	DEFAULT		9
}

Parameter CORE_DUMP_RTP_FS_PATH {
	NAME		RTP Core Dump Storage Path
	SYNOPSIS	Path to store RTP Core Dump files
	TYPE		string
	DEFAULT		"/tgtsvr"
}

/*******************************************************************************
*
* Core Dump Common components (Shared between Kernel and RTP Core Dump)
*
*/

Component INCLUDE_CORE_DUMP_COMMON_COMPRESS_ZLIB {
	NAME		ZLIB Common Core Dump Compression
	SYNOPSIS	Common ZLIB Compression module
	MODULES		coreDumpZLib.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_CORE_DUMP_COMMON_COMPRESS_RLE {
	NAME		RLE Common Core Dump Compression
	SYNOPSIS	Common RLE Compression module
	MODULES		coreDumpRleLib.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_CORE_DUMP_MEM_POOL {
	NAME		Core Dump memory pool
	SYNOPSIS	Provide a memory pool for core dump libraries
	MODULES		coreDumpMemPoolLib.o
	CFG_PARAMS	CORE_DUMP_MEM_POOL_PROTECTION_ENABLE	\
			CORE_DUMP_MEM_POOL_PAGE_SIZE
	_CHILDREN	FOLDER_NOT_VISIBLE
}

/*******************************************************************************
*
* Core Dump Common parameters (Shared between Kernel and RTP Core Dump)
*
*/

Parameter CORE_DUMP_MEM_POOL_PROTECTION_ENABLE {
        NAME            Enable core dump memory pool protection
        SYNOPSIS        Protect core dump pool if virtual memory is enabled
        TYPE            BOOL
        DEFAULT         TRUE
}

Parameter CORE_DUMP_MEM_POOL_PAGE_SIZE {
        NAME            Size of core dump memory pool pages
        SYNOPSIS        Set the size of the core dump memory pool pages
        TYPE            uint
        DEFAULT         0x8000
}
