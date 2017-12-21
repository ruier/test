/* 01vxmp.cdf - Configuration file for VxMP */

/*
 * Copyright (c) 2001-2002,2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01g,04nov10,swg  removed inclusion of funcBindP.h which is now deprecated
                 (to fix WIND00237118).
01f,27jun08,cwu  delete _INIT_ORDER, INIT_AFTER for 111681
01e,04oct07,mze  fix for 104829
01d,20jun06,mze  disabled for SMP until safe
01c,09jun06,bwa  added configuration parameters for VxMP task-level interrupt
                 handling (WIND00039499, WIND00045175).
01b,11mar02,mas  added default for SM_OBJ_MEM_ADRS (SPR 73371)
01a,12nov01,mas  written based on 00vxWorks.cdf: 02v,05nov01,gls


DESCRIPTION
  This file contains the description for VxMP.
*/

#ifndef _WRS_CONFIG_SMP
Component INCLUDE_SM_OBJ {
	NAME		shared memory objects
	CONFIGLETTES	usrSmObj.c
	MODULES		smObjLib.o
	INIT_RTN	usrSmObjInit (BOOT_LINE_ADRS);
	_CHILDREN	FOLDER_KERNEL
	REQUIRES	INCLUDE_SM_COMMON
	CFG_PARAMS	SM_OBJ_MEM_ADRS		\
			SM_OBJ_MAX_TASK		\
			SM_OBJ_MAX_SEM		\
			SM_OBJ_MAX_MSG_Q	\
			SM_OBJ_MAX_MEM_PART	\
			SM_OBJ_MAX_NAME		\
			SM_OBJ_MAX_TRIES        \
			SM_OBJ_TASK_PRIORITY    \
			SM_OBJ_TASK_OPTIONS
	HDR_FILES	sysLib.h stdio.h stdlib.h \
			smLib.h smObjLib.h smPktLib.h cacheLib.h \
			vxLib.h string.h usrConfig.h
}


Parameter SM_OBJ_MEM_ADRS {
	NAME		shared memory object base address, NONE = allocate
	DEFAULT		NONE
}

Parameter SM_OBJ_MAX_TASK {
	NAME		max # of tasks using smObj
	TYPE		uint
	DEFAULT		40
}

Parameter SM_OBJ_MAX_SEM {
	NAME		max # of shared semaphores
	TYPE		uint
	DEFAULT		60
}

Parameter SM_OBJ_MAX_MSG_Q {
	NAME		max # of shared message queues
	TYPE		uint
	DEFAULT		10
}

Parameter SM_OBJ_MAX_MEM_PART {
	NAME		max # of shared memory partitions
	TYPE		uint
	DEFAULT		4
}

Parameter SM_OBJ_MAX_NAME {
	NAME		max # of shared objects names
	TYPE		uint
	DEFAULT		100
}

Parameter SM_OBJ_MAX_TRIES {
	NAME		max # of tries to obtain lock
	TYPE		uint
	DEFAULT		5000
}

Parameter SM_OBJ_TASK_PRIORITY {
	NAME		tSmObjTask task priority
	SYNOPSIS	tSmObjTask must be of higher priority than the \
			application tasks that use VxMP.
	TYPE		uint
	DEFAULT		40
}

Parameter SM_OBJ_TASK_OPTIONS {
	NAME		tSmObjTask task options
	SYNOPSIS	Should not have to change.
	TYPE		uint
	DEFAULT		0
}

#endif
