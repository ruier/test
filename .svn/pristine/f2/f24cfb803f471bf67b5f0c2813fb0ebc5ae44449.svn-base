/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,12Dec10,y_y  written.
*/

Folder FOLDER_MIPC {
    _CHILDREN   FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
    _CHILDREN   FOLDER_NOT_VISIBLE
}

Parameter MIPC_SM_SYSTEM_POOL_BASE {
    SYNOPSIS    The base address on the fsl_p1021_mds is at 0x10010000. \
                The shared memory extends up to 0x10100000. MIPC, SM, and DSHM \
                must share that space if included. Care must be taken so that \
                it is not overflown, since this will corrupt kernel data \
                and/or text.
    DEFAULT     (0x10010000)
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x78000
}

Parameter MSD_CFG_STR {
    DEFAULT (INCLUDE_AMP_CPU_00)::("#dev=/ttyMsd0 node=1 instance=0 console=n bus=main") \
            (INCLUDE_AMP_CPU_01)::("#dev=/ttyMsd0 node=0 instance=0 console=y bus=main") \
            "#dev=/ttyMsd0 node=0 instance=0 console=n bus=main"
}

Parameter MIPC_SM_NODE_IRQ {
    DEFAULT     0
}
