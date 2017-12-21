/* 20comp_dshm.cdf - configuration for DSHM system */

/*
 * Copyright (c) 2006-2007,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01t,05nov07,ebh  Typo fix for WIND00110058
01s,04oct07,mze  fix for WIND00104829
01r,11sep07,mze  fix missing \ in Parameter DSHM_BUS_PLB_NODE
01q,24aug07,bwa  renamed dshmMux.h to dshmMuxLib.h
01p,14aug07,mze  moved INCLUDE_DSHM_SVC_TIPC_PRIM  and 
                 INCLUDE_DSHM_SVC_TIPC_SEC to 20comp_dshm_bsp.cdf files
01o,10aug07,mze  moved FOLDER_DSHM to 20comp_dshm_bsp.cdf files
01n,30jul07,mze  changed dshmMux.o to dshmMuxLib.o and added INCLUDE_TIPC to INCLUDE_DSHM_SVC_TIPC
01m,26jul07,mze  fix SOURCE_ONLY list
01l,12jul07,mze  added SMP flag and SOURCE_ONLY flag
01k,21jun07,bwa  fixed little issues.
01j,15jun07,bwa  removed generic multi-core components.
01i,18apr07,bwa  added SM_AND_DSHM.
01h,11apr07,bwa  moved from under FOLDER_BUS to under FOLDER_MEMORY.
01g,22mar07,bwa  adjusted TIPC bearer service parameters.
01f,14feb07,bwa  added object file for PLB peers.
01e,08feb07,bwa  added object files for DSHM and PLB bus controllers.
01d,02feb07,bwa  removed useless LibInit() references.
01c,21dec06,bwa  cleanup.
01b,08sep06,bwa  initialization sequence shuffle; added hash, pow2.
01a,05jul06,bwa  written.
*/
#ifndef _WRS_CONFIG_SMP
/* overall */


Folder FOLDER_DSHM_CORE {
    NAME	Core support
    SYNOPSIS	These must be included for DSHM to work
    CHILDREN	INCLUDE_DSHM \
		INCLUDE_DSHM_MUX \
		INCLUDE_DSHM_ADAPT
    DEFAULT	INCLUDE_DSHM \
		INCLUDE_DSHM_MUX \
		INCLUDE_DSHM_ADAPT
}

/* fake component to grey out all of DSHM on BSPs that don't support it */
Component INCLUDE_DSHM {
    NAME	DSHM Core support
    SYNOPSIS	DSHM IS NOT SUPPORTED BY THIS BSP
    MODULES	dshmInvalidBsp.o
    HDR_FILES	dshm/dshm.h
    LINK_SYMS	dshmInit
}

Folder FOLDER_DSHM_DEV_AID {
    NAME	Service and hardware interface development aid
    CHILDREN	INCLUDE_DSHM_DEBUG
    DEFAULT	INCLUDE_DSHM_DEBUG
}

Component INCLUDE_DSHM_DEBUG {
    NAME	Debugging aid
    SYNOPSIS	This module brings in the DSHM debugging aid. \
                It can be used to debug either custom services by inserting \
                debug statements in them or the DSHM system when adding new \
                hardware interfaces. If the latter, the DSHM library must be \
                recompiled with debugging on. Be warned that if debugging \
                statements are used but this component is not included, \
                linking errors are possible.
    MODULES	dshmDebug.o
    HDR_FILES	dshm/debug/dshmDebug.h
    INIT_RTN	dshmDebugInit(DSHM_INIT_DBG_LVL);
    CFG_PARAMS	DSHM_INIT_DBG_LVL
}

Parameter DSHM_INIT_DBG_LVL {
    NAME	Initial Debug Level
    SYNOPSIS	Modify by setting dshmDebugLevel to values defined in \
		dshm/debug/dshmDebug.h
    TYPE	UINT
    DEFAULT	DSHMDBG_OFF
}

Component INCLUDE_DSHM_MUX {
    NAME	DSHM MUX
    SYNOPSIS	Provides multiplexing and demultiplexing between hardware \
		buses, services, and nodes
    MODULES	dshmMuxLib.o
    HDR_FILES	dshm/dshmMuxLib.h
    INIT_RTN	dshmMuxLibInit (DSHM_MAX_HW, DSHM_MAX_SERVICES);
    CFG_PARAMS	DSHM_MAX_HW \
		DSHM_MAX_SERVICES
    REQUIRES    INCLUDE_DSHM \
                INCLUDE_SM_AND_DSHM
}

Parameter DSHM_MAX_HW {
    NAME	Maximum instances of hw buses in DSHM system
    TYPE	UINT
    DEFAULT	1
}

Parameter DSHM_MAX_SERVICES {
    NAME	Maximum number of services per bus
    SYNOPSIS	Must be at least one higher than the highest service number \
                used. \
		Node manager service always occupies one slot.
    TYPE	UINT
    DEFAULT	2
}

Component INCLUDE_DSHM_ADAPT {
    NAME	VxWorks adaptation layer
    SYNOPSIS	OS-dependent implementation of some core functionalities.
    MODULES	dshmAdapt.o
    PROTOTYPE	void dshmAdaptInit (void);
    INIT_RTN	dshmAdaptInit ();
    REQUIRES    INCLUDE_DSHM
}

Folder FOLDER_DSHM_SVC {
    NAME	Services
    SYNOPSIS	Middleware using the DSHM API
    CHILDREN	\
		INCLUDE_DSHM_SVC_MNG 
    DEFAULT	INCLUDE_DSHM_SVC_MNG
}

/* messaging */
Folder FOLDER_DSHM_MSG {
    NAME	Distributed Multi-Processing Messaging
    SYNOPSIS	Provides messaging over specialized types of hardware that \
		support it. If no such hardware is available, uses messaging \
		over a distributed shared memory interface.
    CHILDREN	SELECT_DSHM_MSG_HW
    DEFAULT	SELECT_DSHM_MSG_HW
}
 
Selection SELECT_DSHM_MSG_HW {
    NAME	Messaging type
    SYNOPSIS	Depends on the type of bus the BSP supports
    COUNT	1-
    DEFAULTS	INCLUDE_DSHM_MSG_SM
    CHILDREN	INCLUDE_DSHM_MSG_SM
}

Component INCLUDE_DSHM_SVC_MNG {
    NAME	Node manager
    SYNOPSIS	Takes care of nodes shuffle in the system.
    MODULES	dshmSvcMng.o
    REQUIRES	INCLUDE_DSHM_MUX \
                INCLUDE_DSHM
    LINK_SYMS	dshmSvcMngInstall
}

Component INCLUDE_DSHM_MSG_SM {
    NAME	Shared-memory messaging
    SYNOPSIS	No special HW support, messaging goes through shared memory
    MODULES	dshmSm.o
    REQUIRES	INCLUDE_DSHM_NOWRING \
                INCLUDE_DSHM
    LINK_SYMS	dshmSmInit
}

/* drivers
 */
 
Folder FOLDER_DSHM_DRIVER {
    NAME	Peer-to-peer drivers
    SYNOPSIS	Include drivers for all the different type of boards \
		participating in the DSHM system.
    CHILDREN	INCLUDE_DSHM_BUS \
		INCLUDE_DSHM_BUS_PLB
    DEFAULTS	INCLUDE_DSHM_CORE
}

Component INCLUDE_DSHM_BUS {
    NAME	DSHM virtual bus
    SYNOPSIS	This virtual bus sits on real bus controllers in the VxBus \
		system. DSHM peer devices sit on this virtual bus.
    MODULES	dshmBus.o \
                dshmBusCtlrMethods.o
    PROTOTYPE	void dshmBusRegister (void);
    INIT_RTN	dshmBusRegister ();
    LINK_SYMS	dshmBusCtlrMethodId_desc
    REQUIRES	INCLUDE_DSHM_MUX \
                INCLUDE_DSHM
}

Component INCLUDE_DSHM_VXPLB {
    NAME        PLB - Base VxWorks Peer driver
    SYNOPSIS    Base driver that provide common functionalities for peer \
                drivers on PLB.
    MODULES     dshmPeerVxPlb.o
    LINK_SYMS   dshmPeerVxPlbRx
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_NOT_VISIBLE
}

Component INCLUDE_DSHM_BUS_PLB {
    NAME	DSHM virtual bus on PLB
    SYNOPSIS	For multi-core boards and simulators
    REQUIRES	INCLUDE_DSHM
    MODULES	dshmBusCtlrPlb.o
    LINK_SYMS	dshmBusCtlrMethods
    CFG_PARAMS	\
		DSHM_BUS_PLB_SM_POOL \
		DSHM_BUS_PLB_NODE_DYNAMIC \
		DSHM_BUS_PLB_NODE \
		DSHM_BUS_PLB_RMW \
		DSHM_BUS_PLB_NRETRIES \
		DSHM_BUS_PLB_POOLSIZE \
		DSHM_BUS_PLB_MAXNODES \
		DSHM_BUS_PLB_NENTRIES \
		DSHM_BUS_PLB_ENTRY_SIZE
    REQUIRES    INCLUDE_VXBUS
}

Parameter DSHM_BUS_PLB_NODE {
	NAME		Address of local node
	SYNOPSIS	0 to N, -1 for allocation at runtime \
			change Dynamic address of local node to FALSE
	TYPE		INT
	DEFAULT		-1
}

Parameter DSHM_BUS_PLB_RMW {
	NAME		Read-Modify-Write routine
	TYPE		FUNCPTR
	DEFAULT		vxTas
}

Parameter DSHM_BUS_PLB_NRETRIES {
	NAME		Number of retries when trying to send a message
	TYPE		UINT
	DEFAULT		20
}

Parameter DSHM_BUS_PLB_POOLSIZE {
	NAME		Shared memory pool size for this node
	TYPE		UINT
	DEFAULT		0x40000
}

Parameter DSHM_BUS_PLB_MAXNODES {
	NAME		Maximum number of nodes in the system
	SYNOPSIS	MUST be a power-of-two
	TYPE		UINT
	DEFAULT		2
}

Parameter DSHM_BUS_PLB_NENTRIES {
	NAME		Number of entries in the SM messaging ring
        SYNOPSIS        This should be greater than the total number of \
messages that can be in transit from one peer to another by all services \
to prevent a failure to send
	TYPE		UINT
	DEFAULT		64
}

Parameter DSHM_BUS_PLB_ENTRY_SIZE {
	NAME		Size of one entry in the SM messaging ring
        SYNOPSIS        Should be cache-aligned sizes (multiple of 32 bytes)
	TYPE		UINT
	DEFAULT		64
}
 
/* core utilities
 */

Folder FOLDER_DSHM_UTIL {
    NAME	Utilities
    SYNOPSIS	Rings, locks, etc.
    CHILDREN	INCLUDE_DSHM_MEM \
		INCLUDE_DSHM_RW_LOCK \
		INCLUDE_DSHM_OWRING
}

Component INCLUDE_DSHM_MEM {
    NAME	Memory allocator
    SYNOPSIS	Allocator/deallocator for local dual-ported RAM
    MODULES	dshmMem.o
    LINK_SYMS	dshmMemSetup
    REQUIRES	INCLUDE_DSHM
}

Component INCLUDE_DSHM_RW_LOCK {
    NAME	Read-write locks
    SYNOPSIS	Read-write locks in shared memory, used for SM messaging
    MODULES	dshmRwLock.o
    LINK_SYMS	dshmRwLockInit
    REQUIRES	INCLUDE_DSHM
}
 
Component INCLUDE_DSHM_OWRING {
    NAME	Overwritable rings
    SYNOPSIS	Used for DSHM broadcasting
    MODULES	dshmOwRing.o
    LINK_SYMS	dshmOwRingCreate
    REQUIRES	INCLUDE_DSHM
}
 
Component INCLUDE_DSHM_NOWRING {
    NAME	Non-overwritable rings
    SYNOPSIS	Used for SM messaging
    MODULES	dshmNowRing.o
    REQUIRES	INCLUDE_DSHM_SPINLOCK \
                INCLUDE_DSHM
    LINK_SYMS	dshmNowRingInit
    _CHILDREN   FOLDER_NOT_VISIBLE
}
 
Component INCLUDE_DSHM_SPINLOCK {
    NAME	Spinlocks
    SYNOPSIS	DSHM spinlocks
    MODULES	dshmSpinLock.o
    LINK_SYMS	dshmSpinLockInit
    REQUIRES	INCLUDE_DSHM
    _CHILDREN   FOLDER_NOT_VISIBLE
}
 
Component INCLUDE_DSHM_POW2 {
    NAME	Power-of-two utility routines
    SYNOPSIS	Verify if parameters are power-of-two, find nearest power-of-two
    MODULES	dshmPow2.o
    LINK_SYMS	dshmPow2Verify
    REQUIRES	INCLUDE_DSHM
    _CHILDREN   FOLDER_NOT_VISIBLE
}

/* This is a template: this init should really be in the 20comp_dshm_bsp.cdf
 * file that reside in BSPs that provide support for DSHM.
 * 
 * Driver initialization should take place after the INCLUDE_DSHM_BUS
 * initialization at the end and should be added there.
 */
InitGroup usrDshmInit {
    PROTOTYPE   void usrDshmInit (void)
    INIT_RTN    usrDshmInit ();
    SYNOPSIS    First pass initialization
    INIT_ORDER  \
                INCLUDE_DSHM \
                INCLUDE_DSHM_MUX \
                INCLUDE_DSHM_ADAPT \
                INCLUDE_DSHM_BUS
}
#if 0   /* this has not been tested */
Component INCLUDE_DSHM_SVC_TIPC_SEC {
    NAME	DSHM secondary interface
    SYNOPSIS	Allows TIPC to communicate over DSHM secondary interface
    HDR_FILES	dshm/dshm.h dshm/service/dshmSvcTipc.h
    INIT_RTN	dshm_mediaInit (DSHM_SVC_TIPC_SEC_HW, \
				DSHM_SVC_TIPC_SEC, \
				DSHM_SVC_TIPC_SEC_SZ_REQ_SM_POOL, \
				DSHM_SVC_TIPC_SEC_SZ_MIN_SM_POOL, \
				DSHM_SVC_TIPC_SEC_SZ_SM_BUFFER, \
				DSHM_SVC_TIPC_SEC_LINK_WINDOW, \
				DSHM_SVC_TIPC_SEC_N_BCAST_ENTRIES, \
				0 \
				);
    _INIT_ORDER	usrNetworkInit
    INIT_BEFORE	INCLUDE_TIPC
    _CHILDREN 	SELECT_TIPC_MEDIA_TYPES
    REQUIRES	INCLUDE_DSHM \
		INCLUDE_DSHM_SVC_TIPC
    CFG_PARAMS	\
		DSHM_SVC_TIPC_SEC_HW \
		DSHM_SVC_TIPC_SEC_N_BCAST_ENTRIES \
		DSHM_SVC_TIPC_SEC_LINK_WINDOW \
		DSHM_SVC_TIPC_SEC_SZ_SM_BUFFER \
		DSHM_SVC_TIPC_SEC_SZ_MIN_SM_POOL \
		DSHM_SVC_TIPC_SEC_SZ_REQ_SM_POOL
}

Parameter DSHM_SVC_TIPC_SEC_N_BCAST_ENTRIES {
	NAME		Broadcast buffers
	SYNOPSIS	Number of concurrent broadcast buffers. \
			Enter '0' to use replicast.
	TYPE		UINT
	DEFAULT		8
}
Parameter DSHM_SVC_TIPC_SEC_LINK_WINDOW {
	NAME		TIPC link window (0 for default)
	SYNOPSIS	Number of packets sent before waiting for ack
	TYPE		UINT
	DEFAULT		0
}

Parameter DSHM_SVC_TIPC_SEC_HW {
    NAME	Hardware bus
    SYNOPSIS	Literal name
    DEFAULT	"plb"
}

Parameter DSHM_SVC_TIPC_SEC_SZ_REQ_SM_POOL {
	NAME		requested size of SM buffer pool
	TYPE		UINT
	DEFAULT		0x35000
}

Parameter DSHM_SVC_TIPC_SEC_SZ_MIN_SM_POOL {
	NAME		minimal acceptable size of SM buffer pool
	TYPE		UINT
	DEFAULT		0x35000
}

Parameter DSHM_SVC_TIPC_SEC_SZ_SM_BUFFER {
	NAME		size of buffers in SM
	TYPE		UINT
	DEFAULT		0x800
}

#endif /* 0 */
#endif

