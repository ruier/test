/* 20comp_dshm_bsp.cdf - configuration for dSHM system */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,29aug07,mze  re-added SOURCE_ONLY option to fix GNU compiler issue
01h,21aug07,mze  removed SOURCE_ONLY option
01g,10aug07,mze  added INCLUDE_DSHM_SVC_TIPC INCLUDE_DSHM_SVC_TIPC_PRIM,INCLUDE_DSHM_SVC_TIPC_SEC
01f,10aug07,mze  added FOLDER_DSHM
01e,02jul07,bwa  added bsp-specific value for DSHM TIPC parameters
01d,27jun07,bwa  added POOL_BASE parameter.
01c,23jun07,bwa  updated to split of debug and core components.
01b,21jun07,bwa  changed _FOLDER to _CHILDREN
01a,15may07,bwa  written.
*/

Folder FOLDER_DSHM {
    NAME	DSHM
    SYNOPSIS	Distributed Shared Memory Infrastructure.	\
		Includes Distributed messaging and Distributed Shared Memory.
    _CHILDREN	FOLDER_MEMORY
    CHILDREN	FOLDER_DSHM_CORE \
                FOLDER_DSHM_DEV_AID \
		FOLDER_DSHM_MSG \
		FOLDER_DSHM_DRIVER \
		FOLDER_DSHM_UTIL \
		FOLDER_DSHM_SVC
    DEFAULT	FOLDER_DSHM_CORE \
		FOLDER_DSHM_MSG \
		FOLDER_DSHM_UTIL \
		FOLDER_DSHM_SVC
}

Component INCLUDE_DSHM {
    NAME        DSHM BSP support enabling
    SYNOPSIS    This BSP supports DSHM
    MODULES     dshm.o
    HDR_FILES   dshm/dshm.h
    LINK_SYMS   dshmInit
}

Component INCLUDE_DSHM_BUS_CTLR_8641 {
    NAME        hpcNet8641 - DSHM virtual bus controller for hpcNet8641
    SYNOPSIS    Instance of the virtual bus controller on parent bus
    MODULES     dshmBusCtlr8641.o
    PROTOTYPE   void dshmBusCtlr8641Register (void);
    INIT_RTN    dshmBusCtlr8641Register ();
    REQUIRES    INCLUDE_DSHM_BUS \
                INCLUDE_DSHM_MSG_SM \
                INCLUDE_DSHM \
                INCLUDE_PLB_BUS \
                INCLUDE_VXBUS \
                VXBUS_TABLE_CONFIG
    _CHILDREN   FOLDER_DSHM_DRIVER
    CFG_PARAMS  DSHM_SM_POOL_BASE
}

Parameter DSHM_SM_POOL_BASE {
    NAME        Base address of shared memory pool used by DSHM
    SYNOPSIS    The base address on the hpcNet8641 is at 0x20010000. The \
shared memory extends up to 0x20100000. Both SM and DSHM must share that \
space if included. Care must be taken so that it is not overflown, since this \
will corrupt kernel data and/or text.
    DEFAULT     0x20010000
}

Component INCLUDE_DSHM_VX8641 {
    NAME        hpcNet8641 - Peer driver
    SYNOPSIS    Peer-to-peer driver for hpcNet8641
    MODULES     dshmPeerVx8641.o
    PROTOTYPE   void dshmPeerVx8641Load (void);
    INIT_RTN    dshmPeerVx8641Load ();
    REQUIRES    INCLUDE_DSHM_MSG_SM \
                INCLUDE_DSHM \
                INCLUDE_DSHM_VXPLB \
                INCLUDE_PLB_BUS \
                INCLUDE_VXBUS
    _CHILDREN   FOLDER_DSHM_DRIVER
}

InitGroup usrDshmInit {
    PROTOTYPE   void usrDshmInit (void)
    INIT_RTN    usrDshmInit ();
    SYNOPSIS    First pass initialization
    INIT_ORDER  \
                INCLUDE_DSHM \
                INCLUDE_DSHM_DEBUG \
                INCLUDE_DSHM_MUX \
                INCLUDE_DSHM_ADAPT \
                INCLUDE_DSHM_BUS \
                INCLUDE_DSHM_BUS_CTLR_8641 \
                INCLUDE_DSHM_VX8641
}

Component INCLUDE_DSHM_SVC_TIPC {
    NAME	TIPC Bearer DSHM service
    REQUIRES    INCLUDE_TIPC \
                INCLUDE_DSHM_RW_LOCK \
                INCLUDE_DSHM_OWRING \
                INCLUDE_DSHM_BUS_CTLR_8641 \
                INCLUDE_DSHM_VX8641
    SOURCE_ONLY     {$(TGT_DIR)/src/tipc {tipc_dshm_media.o}}
    _CHILDREN	FOLDER_NOT_VISIBLE
}

/* note INCLUDE_DSHM_SVC_TIPC_PRIM and INCLUDE_DSHM_SVC_TIPC_SEC should be 
   moved to a common BSP  cdf file                                        */

Component INCLUDE_DSHM_SVC_TIPC_PRIM {
    NAME	DSHM primary interface
    SYNOPSIS	Allows TIPC to communicate over DSHM primary interface
    HDR_FILES	dshm/dshm.h dshm/service/dshmSvcTipc.h
    INIT_RTN	dshm_mediaInit (DSHM_SVC_TIPC_PRIM_HW, \
				DSHM_SVC_TIPC, \
				DSHM_SVC_TIPC_PRIM_SZ_REQ_SM_POOL, \
				DSHM_SVC_TIPC_PRIM_SZ_MIN_SM_POOL, \
				DSHM_SVC_TIPC_PRIM_SZ_SM_BUFFER, \
				DSHM_SVC_TIPC_PRIM_LINK_WINDOW, \
				DSHM_SVC_TIPC_PRIM_N_BCAST_ENTRIES, \
                                DSHM_SVC_TIPC_PRIM_MAX_BUF_ALLOC \
				);
    _INIT_ORDER	usrNetworkInit
    INIT_BEFORE	INCLUDE_TIPC
    _CHILDREN 	SELECT_TIPC_MEDIA_TYPES
    REQUIRES	INCLUDE_DSHM \
		INCLUDE_DSHM_SVC_TIPC
    CFG_PARAMS	\
		DSHM_SVC_TIPC_PRIM_HW \
		DSHM_SVC_TIPC_PRIM_N_BCAST_ENTRIES \
		DSHM_SVC_TIPC_PRIM_LINK_WINDOW \
		DSHM_SVC_TIPC_PRIM_SZ_SM_BUFFER \
		DSHM_SVC_TIPC_PRIM_SZ_MIN_SM_POOL \
		DSHM_SVC_TIPC_PRIM_SZ_REQ_SM_POOL \
                DSHM_SVC_TIPC_PRIM_MAX_BUF_ALLOC
}

/* BSP-specific values for TIPC DSHM parameters */
Parameter DSHM_SVC_TIPC_PRIM_N_BCAST_ENTRIES {
        NAME            Broadcast buffers
        SYNOPSIS        Number of concurrent broadcast buffers. \
                        Enter '0' to use replicast.
        TYPE            UINT
        DEFAULT         0
}

/* common values for TIPC DSHM parameters */
Parameter DSHM_SVC_TIPC_PRIM_LINK_WINDOW {
	NAME		TIPC link window (0 for default)
	SYNOPSIS	Number of packets sent before waiting for ack
	TYPE		UINT
	DEFAULT		0
}

Parameter DSHM_SVC_TIPC_PRIM_HW {
        NAME	        Hardware bus
        SYNOPSIS	Literal name
        DEFAULT	        "plb"
}

Parameter DSHM_SVC_TIPC_PRIM_SZ_REQ_SM_POOL {
	NAME		SM buffer pool size
	SYNOPSIS	requested size of SM buffer pool
	TYPE		UINT
	DEFAULT		0x35000
}

Parameter DSHM_SVC_TIPC_PRIM_SZ_MIN_SM_POOL {
	NAME		SM buffer pool size minimum
	SYNOPSIS	minimal acceptable size of SM buffer pool
	TYPE		UINT
	DEFAULT		0x35000
}

Parameter DSHM_SVC_TIPC_PRIM_SZ_SM_BUFFER {
	NAME		SM buffer size
	SYNOPSIS	size of buffers in SM
	TYPE		UINT
	DEFAULT		0x800
}

Parameter DSHM_SVC_TIPC_PRIM_MAX_BUF_ALLOC {
        NAME            Maximum buffers allocated per-peer
        SYNOPSIS        For both incoming and outgoing traffic
        TYPE            UINT
        DEFAULT         32
}
    

