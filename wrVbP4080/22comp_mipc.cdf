/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22dec09,to   created from rvb8572/22comp_mipc.cdf, v01d
*/

Folder FOLDER_MIPC {
	_CHILDREN	FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter MIPC_SM_SYSTEM_POOL_BASE {
    SYNOPSIS    For Wind River HyperVisor, this value must be equal to the \
lowest shared memory virtual address defined in the config_pentium/vxworks.xml \
file, that is intended to be used by the wrhv_0 bus.
    DEFAULT     0xFD000000
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x80000
}

Parameter MIPC_SM_NODE_IRQ {
    Synopsis	Ignored by hypervisor guests. Present for completeness sake. \
Must not equal -1.
    DEFAULT     0
}

