/* 22comp_mipc.cdf - configuration for MSD/MIPC */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,05jul11,zhw  Removed unnecessary MSD_CFG_STR parameter (WIND00279265)
01t,05may10,als  Add missing MIPC shared memory pool size info (WIND00210390)
01s,14sep09,elp  updated to use MSD automatic configuration by default.
01r,24jun09,h_x  Fix WIND00170474
01q,15jun09,ebh  Modify names to remove MCB/MOB references and update for MIPC
                 2.0
01p,20apr09,elp  added mipc2.0 support.
01o,09mar09,dbt  Added MSD default parameters for wrload.
01n,23oct08,ebh  Move MOB Driver definitions out of BSP directory.
                 Remove MOB_WDB completely.
01m,18oct08,jlk  WIND00136113 - Configure multiple MSDs
01l,14oct08,ebh  Simplify selection of MULTIOS components
01k,08oct08,jlk  WIND00137348: Cleanup.
01j,17sep08,elp  fixed CQ:WIND134266.
01i,12sep08,elp  increase default max node and pool size.
01h,05aug08,elp	 written.
*/

Folder FOLDER_MIPC {
	_CHILDREN       FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
	_CHILDREN       FOLDER_NOT_VISIBLE
}

Parameter MIPC_SM_SYSTEM_POOL_BASE {
	SYNOPSIS        On simulator base address is automatically determined
	TYPE		UINT
	DEFAULT		(uint64_t)(ULONG)sysMipcSmAddr
}

/*
 * MIPC_SM_SYSTEM_POOL_BASE, MIPC_SM_SYSTEM_POOL_SIZE, and MIPC_SM_NODE_IRQ 
 * are only used in unsupervised systems and these parameters are ignored in 
 * a supervised system
 */

/* Configure 0.5 MB region, enough for 300+ buffers of default MTU size */

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
	DEFAULT		0x80000
}

/* use IPI level 0 */

Parameter MIPC_SM_NODE_IRQ {
	DEFAULT         0
}

Parameter MSD_NUM_DEVS {
	DEFAULT	(INCLUDE_WRLOAD)::(3) \
		1
}

