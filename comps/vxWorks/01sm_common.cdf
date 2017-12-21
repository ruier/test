/* 01sm_common.cdf - Common shared memory components and parameters */

/* 
 * Copyright (c) 1998-2002,2005,2007-2009,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,17oct12,whu  fixed defect WIND00381666. Added SM_MASTER to smLibInit.
01i,24sep09,h_x  Remove INCLUDE_MOB_PLB_1
01h,04may09,rgo  Define SM_BASE_ADRS as configurable parameter(wind145934)
01g,02sep08,ebh  Renamed MOB_PLB_2 to MOB_PLB_1
01f,29aug08,ebh  Add in EXCLUDES for MOB_PLB_2 when SM_COMMON included
01e,17apr07,bwa  added SM_AND_DSHM.
01d,17apr05,bwa  made DEFAULT_CPUS_MAX the SM_CPUS_MAX default (SPR107525).
01c,07feb05,zl   initialize before PROTECT_VEC_TABLE.
01b,11mar02,mas  added default values to SM params (SPR 73371)
01a,12nov01,mas  written based on 00vxWorks.cdf: 02v,05nov01,gls
*/

/*
DESCRIPTION
  This file contains the description for the VxWorks shared memory component.
*/


Component INCLUDE_SM_COMMON {
	NAME		basic shared memory support and parameters
	SYNOPSIS	Common modules needed by all shared memory subsystems.
	MODULES		smLib.o \
			smUtilLib.o
	INIT_RTN	smLibInit (NULL, SM_OBJ_MEM_SIZE, SM_MEM_SIZE, SM_MASTER);
	_INIT_ORDER	usrKernelExtraInit
	INIT_BEFORE	INCLUDE_PROTECT_VEC_TABLE
	_CHILDREN	FOLDER_MEMORY
	REQUIRES	INCLUDE_MEMORY_CONFIG \
                        INCLUDE_SM_AND_DSHM \
                        INCLUDE_SM_AND_MIPC

	CFG_PARAMS	SM_MASTER \
			SM_CPUS_MAX \
			SM_OFF_BOARD \
			SM_BASE_ADRS \
			SM_ANCHOR_OFFSET \
			SM_ANCHOR_ADRS \
			SM_INT_TYPE \
			SM_INT_ARG1 \
			SM_INT_ARG2 \
			SM_INT_ARG3 \
			SM_MAX_WAIT \
			SM_MEM_ADRS \
			SM_MEM_SIZE \
			SM_TAS_TYPE \
			SM_OBJ_MEM_SIZE
	HDR_FILES	smLib.h \
			smUtilLib.h
}


Parameter SM_MEM_ADRS {
	NAME		shared memory address, NONE = allocate local memory
	DEFAULT		NONE
}

Parameter SM_MEM_SIZE {
	NAME		shared memory size
	TYPE		uint
	DEFAULT		0
}

Parameter SM_INT_TYPE {
	NAME		shared memory interrupt type
	TYPE		uint
	DEFAULT		SM_INT_NONE
}

Parameter SM_INT_ARG1 {
	NAME		shared memory interrupt type - argument 1
	TYPE		uint
	DEFAULT		0
}

Parameter SM_INT_ARG2 {
	NAME		shared memory interrupt type - argument 2
	TYPE		uint
	DEFAULT		0
}

Parameter SM_INT_ARG3 {
	NAME		shared memory interrupt type - argument 3
	TYPE		uint
	DEFAULT		0
}

Parameter SM_OFF_BOARD {
	NAME		is the shared memory off board?
	TYPE		bool
	DEFAULT		TRUE
}

Parameter SM_MASTER {
	NAME            shared memory master CPU number
	TYPE            uint
	DEFAULT         0
}

Parameter SM_OBJ_MEM_SIZE {
	NAME		shared memory object pool size
	TYPE		uint
	DEFAULT		0
}

Parameter SM_TAS_TYPE {
	NAME		shared memory test-and-set type
	DEFAULT		SM_TAS_HARD
}

Parameter SM_MAX_WAIT {
	NAME		max period in ticks to wait for master to boot
	TYPE		uint
	DEFAULT		3000
}

Parameter SM_CPUS_MAX {
	NAME		max # of cpus for shared network
	TYPE		uint
	DEFAULT		DEFAULT_CPUS_MAX
}

Parameter SM_BASE_ADRS {
    NAME            Shared Memory Base Address
    SYNOPSIS        Shared memory base address
    DEFAULT         LOCAL_MEM_LOCAL_ADRS 
}

Parameter SM_ANCHOR_ADRS {
	NAME		shared memory anchor address
	DEFAULT		((char *) ( SM_BASE_ADRS + SM_ANCHOR_OFFSET ))
}

Parameter SM_ANCHOR_OFFSET {
	NAME		shared memory anchor offset from start of phys memory
	TYPE		uint
	DEFAULT		0x600
}

Component INCLUDE_SM_AND_DSHM {
	NAME		Check for inclusion of both SM and DSHM
	CONFIGLETTES    usrSmAndDshm.c
	_CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_SM_AND_MIPC {
	NAME		Check for inclusion of both SM and MIPC
	CONFIGLETTES    usrSmAndMipc.c
	_CHILDREN       FOLDER_NOT_VISIBLE
}
