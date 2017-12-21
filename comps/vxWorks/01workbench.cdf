/* 01workbench.cdf - Workbench tools runtime support */

/*
 * Copyright (c) 2007,2009-2011 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01g,21feb11,vhe  Add INCLUDE_MODULE_MANAGER, needed by ST (WIND00255740)
01f,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01e,20jan10,rlp  Added some WDB dependencies in order to load DKM for WB.
01d,07oct09,rlp  Modified INCLUDE_GNU_INTRINSICS dependency (CQ:WIND00183905).
01c,11sep09,flu  For defect 165733, add  INCLUDE_GNU_INTRINSICS compenent
01b,09oct07,mze  fix for WIND00104829 SMP flag now _WRS_VX_SMP
01a,26sep07,dbt  written (CQ:WIND00105203)

DESCRIPTION

This file does contain the description of the component that describes
the runtime facilities required by the various Workbench tools. This 
component is automatically included whenever WDB task mode support is
included.
*/

Component INCLUDE_WB_RUNTIME_SUPPORT {
	NAME		WB runtime support
	SYNOPSIS	Workbench Runtime Support
	_CHILDREN	FOLDER_NOT_VISIBLE
	_REQUIRES	INCLUDE_WDB_TASK
	REQUIRES	INCLUDE_WDB_TASK \
			INCLUDE_MODULE_MANAGER \
			INCLUDE_HASH
}

#ifdef	_WRS_CONFIG_SMP
Component INCLUDE_WB_RUNTIME_SUPPORT {
	REQUIRES	+= INCLUDE_TASK_TOOLS_UTIL
}
#endif	/* _WRS_CONFIG_SMP */

#ifdef	_WRS_CONFIG_LP64
Component INCLUDE_WB_RUNTIME_SUPPORT {
	REQUIRES	+= INCLUDE_WDB_TSFS
}
#endif	/* _WRS_CONFIG_LP64 */
