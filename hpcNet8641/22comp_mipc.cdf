/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2008-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,05jul11,zhw  Removed unnecessary MSD_CFG_STR parameter (WIND00279265)
01p,24jun09,h_x  Fix WIND00170474
01o,12jun09,h_w  Fix for WIND00169598
01n,17apr09,h_w  Remove MCB folder; add MIPC_SM_SYSTEM_POOL_BASE and
                 MIPC_SM_SYSTEM_POOL_SIZE definition.
01m,18mar09,rgo  Define shared memory with macro(wind00145934)
01l,20oct08,ebh  Move MOB Driver definitions out of BSP file.
                 Remove MOB_WDB completely.
01k,18oct08,jlk  WIND00136113 - Configure multiple MSDs
01j,14oct08,ebh  Simplify selection of MULTIOS components
01i,08oct08,jlk  WIND00137348: Cleanup.
01h,02oct08,kab  Correct MSD defaults per CPU
01d,15sep08,apa  Fix for WIND00134266
01c,03sep08,ebh  Put MOB_WDB in FOLDER_NOT_VISIBLE
01b,02sep08,ebh  Renamed MOB_PLB_2 to MOB_PLB_1.
01a,01sep08,ebh  written.
*/

Folder FOLDER_MIPC {
	_CHILDREN	FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
	_CHILDREN	FOLDER_NOT_VISIBLE
}

/*
 * MIPC_SM_SYSTEM_POOL_BASE, MIPC_SM_SYSTEM_POOL_SIZE, and MIPC_SM_NODE_IRQ 
 * are only used in unsupervised systems and these parameters are ignored in 
 * a supervised system
 */
Parameter MIPC_SM_SYSTEM_POOL_BASE {
	SYNOPSIS    The base address on the hpcNet8641 is at 0x10010000. The \
shared memory extends up to 0x10100000. MIPC, SM, and DSHM must share that \
space if included. Care must be taken so that it is not overflown, since this \
will corrupt kernel data and/or text.
	DEFAULT		(SM_BASE_ADRS + 0x10000)
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
	DEFAULT		0x80000
}

Parameter MIPC_SM_NODE_IRQ {
	DEFAULT		0
}
