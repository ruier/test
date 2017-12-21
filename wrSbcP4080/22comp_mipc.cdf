/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

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
01r,05jul11,zhw  Removed unnecessary MSD_CFG_STR parameter (WIND00279265)
01q,12feb10,pmr  virtual address for MIPC SM
01p,13nov09,pmr  BSP name change
01o,11sep09,h_w  MIPC 2.0 support, cleanup obsolete items.
01n,28jan09,pmr  PLB_1 max nodes
01m,03dec08,pmr  default MSD parameters
01l,20oct08,ebh  Move MOB Driver definitions out of BSP file.
                 Remove MOB_WDB completely.
01k,18oct08,jlk  WIND00136113 - Configure multiple MSDs
01j,14oct08,ebh  Simplify selection of MULTIOS components
01i,08oct08,jlk  WIND00137348: Cleanup.
01h,02oct08,kab  Correct MSD defaults per CPU
01g,30sep08,dtr  Code review fixes and tidyup
01f,15sep08,apa  Fix for WIND00134266
01e,03sep08,ebh  Modify area of MOB_PLB_1 to increase it (used by default as
                 SM space as well)
01d,03sep08,ebh  Put MOB_WDB in FOLDER_NOT_VISIBLE
01c,02sep08,ebh  renamed MOB_PLB_2 to MOB_PLB_1
01b,01sep08,ebh  renamed and moved all MSD/MOB items from 20bsp.cdf
01a,18aug08,ebh  written.
*/

Folder FOLDER_MIPC {
    _CHILDREN   FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
    _CHILDREN   FOLDER_NOT_VISIBLE
}

Parameter MIPC_SM_SYSTEM_POOL_BASE {
    NAME        Base address of shared memory pool used by MCB (virtual)
    SYNOPSIS	This is the virtual address for MIPC SM.  Configure the \
		physical address (MIPC_SM_SYSTEM_POOL_BASE_PHYS) instead.
    DEFAULT     (INCLUDE_VIRTUAL_SM)::((VIRTUAL_SM_BASE - VIRTUAL_SM_BASE_PHYS + MIPC_SM_SYSTEM_POOL_BASE_PHYS)) \
		(MIPC_SM_SYSTEM_POOL_BASE_PHYS)
}

/*
 * MIPC_SM_SYSTEM_POOL_BASE, MIPC_SM_SYSTEM_POOL_SIZE, and MIPC_SM_NODE_IRQ 
 * are only used in unsupervised systems and these parameters are ignored in 
 * a supervised system
 */

Parameter MIPC_SM_SYSTEM_POOL_BASE_PHYS {
    NAME        Base address of shared memory pool used by MCB
    SYNOPSIS    The base address on the fsl_p4080_ds is at 0x20010000. The \
shared memory extends up to 0x20100000. MCB, SM and DSHM must share that \
space if included. Care must be taken so that it is not overflown, since this \
will corrupt kernel data and/or text.
    _CFG_PARAMS	INCLUDE_MIPC_SM
    DEFAULT     0x20010000
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x80000
}

Parameter MIPC_SM_NODE_IRQ {
	NAME		Interrupt request line
	SYNOPSIS	Interrupt request line.
	TYPE		INT
	DEFAULT		3
}

Parameter MIPC_SM_NODES {
    NAME        Maximum nodes per bus
    SYNOPSIS    Specifies the maximum number of nodes a bus can support, unless overridden by the bus configuration string.
    TYPE        UINT
    DEFAULT     8
}

Parameter MSD_NUM_DEVS {
	DEFAULT (INCLUDE_AMP_CPU_00)::(7) \
		(1)
}

