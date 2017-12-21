/* 20comp_mipc.cdf - MIPC-SM configuration */

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
02j,21dec11,swg  Fix module names and dependencies (WIND00193971)
02i,09sep10,jlk  Cleanup: Remove obsolete MIPC_SM_SEMTASK_PRIORITY parameter
02h,30jun10,swg  Fix WIND00217941
02g,06apr10,als  Initial changes in support of 64-bit memory addressing
02f,10jan10,h_w  Add AF_MIPC support
02e,02nov09,ebh  Increase default for MIPC_SM_BUFFERS to 64
02d,29oct09,als  Fix minor API issues (WIND00186193)
02c,24sep09,als  Rename mipc_sm_api_events files to mipc_sm_event_pool
02b,23sep09,als  Remove MIPC sanity test component
02a,03sep09,als  Remove references to more obsolete files
01z,02sep09,als  Continue flow control rework
01y,27aug09,h_x  Fix WIND00176456 again
01x,25aug09,jlk  Increase default buffers to 32 
01w,10aug09,als  Stop building obsolete memory map file
01v,27jul09,als  Event queue rework
01u,02jul09,als  Changes to reflect updated file names
01t,25jun09,dcc  added mipc_demo_init prototype
01s,25jun09,ebh  Remove reference to mipc_sm_api_defs.o
01r,24jun09,h_x  Fix WIND00170474
01q,10jun09,als  Rework system configuration/initialization as per HLD and CI
01p,01jun09,als  Revise configuration based on MIPC 2.0 HLD feedback
01o,29may09,dcc  added MIPC_DEMO component
01n,25may09,ebh  Code Inspection Modifications
01m,14may09,h_w  Change MIPC_SM_BUSES configuration string format
01l,15apr09,ebh  Add synopsis field to MIPC_TEST_SANITY
01k,27mar09,ebh  Add new MIPC_SHOW component
01j,24feb09,ebh  Add in QOS capability for ISR-deferred mode buses
01i,18feb09,ebh  Collapse 3 packet pools into a single pool per bus.
                 Remove MIPC_SM_NODE_ID and only allow automatic assignment
01h,17feb09,ebh  Fix MIPC_SM_BUSES default
01g,14jan09,ebh  Updates for MIPC 2.0
01f,25nov08,apa  Initial MIPC 2.0 version.
01e,29oct08,ebh  Add in extra MIPC sockets for MSD and WDB, if included
01d,14oct08,ebh  Simplify selection of MIPC components
01c,18sep08,jlk  Updated as per code review
01b,04sep08,jlk  Removed SMP limitation
01a,05feb08,bwa  written.
*/

Folder FOLDER_MIPC {
	NAME		Multi-OS IPC
	SYNOPSIS	Main API for data exchange between multiple OSes.
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_MIPC_UNSUPPORTED {
	NAME		This BSP does not have MIPC Multi-OS support
	SYNOPSIS	Multi-OS support is only available for some BSPs.
	_CHILDREN	FOLDER_MULTIOS
}

Component INCLUDE_MIPC_SHOW {
	NAME		MIPC Show routines
	SYNOPSIS	Addition of mipcHelp, mipcShow, mipcShowBus, mipcShowNode, and mipcShowPort.
	MODULES		mipc_sm_show.o
	_CHILDREN	FOLDER_MIPC
	REQUIRES	INCLUDE_MIPC_SM
	LINK_SYMS	mipcShow
}

Component INCLUDE_MIPC_SM {
	NAME		MIPC over Shared Memory
	SYNOPSIS	Multi-core IPC over shared memory.
	MODULES		\
			mipc_sm_api.o \
			mipc_sm_bus.o \
			mipc_sm_event_pool.o \
			mipc_sm_event_queue.o \
			mipc_sm_mem_region.o \
			mipc_sm_node.o \
			mipc_sm_packet_pool.o \
			mipc_sm_port.o \
			mipc_sm_socket.o \
			mipc_sm_system.o \
			mipc_sm_vxworks.o
	HDR_FILES	multios_ipc/mipc.h
	_CHILDREN	FOLDER_MIPC
#ifndef _WRS_CONFIG_WRHV_GUEST
	CFG_PARAMS	\
			MIPC_SM_BUS_LIST \
			MIPC_SM_BUFFERS \
			MIPC_SM_MTU \
			MIPC_SM_NODES \
			MIPC_SM_PORTS \
			MIPC_SM_EVENTS \
			MIPC_SM_QOS \
			MIPC_SM_SOCKETS \
			MIPC_SM_INIT_MODE \
			MIPC_SM_HEARTBEAT_PERIOD \
			MIPC_SM_RXTASK_STACK \
			MIPC_SM_SYSTEM_POOL_BASE \
			MIPC_SM_SYSTEM_POOL_SIZE \
			MIPC_SM_NODE_IRQ
#else
    CFG_PARAMS  \
            MIPC_SM_BUS_LIST \
            MIPC_SM_BUFFERS \
            MIPC_SM_MTU \
            MIPC_SM_NODES \
            MIPC_SM_PORTS \
            MIPC_SM_EVENTS \
            MIPC_SM_QOS \
            MIPC_SM_SOCKETS \
            MIPC_SM_INIT_MODE \
            MIPC_SM_HEARTBEAT_PERIOD \
            MIPC_SM_RXTASK_STACK
#endif /*_WRS_CONFIG_WRHV_GUEST*/
	CONFIGLETTES	usrMipcSm.c
	INIT_RTN	usrMipcSmInit();
}

Parameter MIPC_SM_BUS_LIST {
	NAME		List of available MIPC buses
	SYNOPSIS	Specifies the MIPC buses known to this node.  Syntax: #<busname>=<busid>[,<busparm>=<integer>,<busparm>=<integer>,...
	TYPE		STRING
	DEFAULT		"#main=0"
}

Parameter MIPC_SM_BUFFERS {
	NAME		Maximum buffers per bus
	SYNOPSIS	Specifies the maximum number of buffers this node can have active on a bus simultaneously, unless overridden by the bus configuration string.
	TYPE		UINT
	DEFAULT		64
}

Parameter MIPC_SM_MTU {
	NAME		Maximum buffer size per bus
	SYNOPSIS	Specifies the maximum size buffer this node can transmit on a bus, unless overridden by the bus configuration string.
	TYPE		UINT
	DEFAULT		1520
}

Parameter MIPC_SM_NODES {
	NAME		Maximum nodes per bus
	SYNOPSIS	Specifies the maximum number of nodes a bus can support, unless overridden by the bus configuration string.
	TYPE		UINT
	DEFAULT		2
}

Parameter MIPC_SM_PORTS {
	NAME		Maximum ports per bus
	SYNOPSIS	Specifies the maximum number of ports this node has on a bus, unless overridden by the bus configuration string.
	TYPE		UINT
	DEFAULT		32
}

Parameter MIPC_SM_EVENTS {
	NAME		Event pool size per bus
	SYNOPSIS	Specifies the size of the event pool this node has on a bus, unless overridden by the bus configuration string.  MIPC uses events from this pool for connection set up and termination, and to send express messages.
	TYPE		INT
	DEFAULT		32
}

Parameter MIPC_SM_QOS {
	NAME		Quality of service used by buses
	SYNOPSIS	Specifies the processing mode a bus uses to handle incoming events. (QOS>0: detected by ISR, but deferred to task priority QOS; QOS=0: processed by ISR; QOS=-1: application polls for bus activity.)
	TYPE		INT
	DEFAULT		41
}

Parameter MIPC_SM_SOCKETS {
	NAME		Maximum number of sockets
	SYNOPSIS	Specifies the maximum number of MIPC sockets that can exist simultaneously on this node. (-1: support a socket on every port the node has on each bus it uses)
	TYPE		INT
	DEFAULT		-1
}

Parameter MIPC_SM_INIT_MODE {
	NAME		Shared memory initialization mode
	SYNOPSIS	Specifies how this node initializes shared memory during node start up. (0: wait for peer to initialize; 1: initialize if first up; 2: always initialize)
	TYPE		UINT
	DEFAULT		1
}

Parameter MIPC_SM_HEARTBEAT_PERIOD {
	NAME		Heartbeat rate
	SYNOPSIS	Specifies how often (in milliseconds) the node updates its heartbeat indicator, indicating that the node is still able to use the bus.
	TYPE		UINT
	DEFAULT		50
}

Parameter MIPC_SM_RXTASK_STACK {
	NAME		Receive task stack size
	SYNOPSIS	Specifies stack size of task created for each bus running in ISR-deferred mode.
	TYPE		UINT
	DEFAULT		4000
}

#ifndef _WRS_CONFIG_WRHV_GUEST
Parameter MIPC_SM_SYSTEM_POOL_BASE {
	NAME		Shared memory base address
	SYNOPSIS	Specifies base address of the MIPC shared memory region.  (-1: use the address defined by the BSP)
	TYPE		LONG
	DEFAULT		-1
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
	NAME		Shared memory size
	SYNOPSIS	Specifies the size of the MIPC shared memory region.  (-1: use the size defined by the BSP)
	TYPE		INT
	DEFAULT		-1
}

Parameter MIPC_SM_NODE_IRQ {
	NAME		Interrupt request line
	SYNOPSIS	Interrupt request line.
	TYPE		INT
	DEFAULT		-1
}
#endif /*_WRS_CONFIG_WRHV_GUEST*/

Component INCLUDE_MIPC_DEMO {
	NAME		MIPC demo
	SYNOPSIS	MIPC demo application.
	MODULES		mipcdemo.o
	LINK_SYMS	mipcdemo
	_CHILDREN	FOLDER_MIPC
}

Folder FOLDER_AF_MIPC {
	NAME		AF_MIPC socket API
	SYNOPSIS	API for AF_MIPC sockets.
	_CHILDREN	FOLDER_MIPC
}

Component INCLUDE_AF_MIPC_SOCKETS {
	NAME		AF_MIPC
	SYNOPSIS	Add support for AF_MIPC sockets.
	MODULES		mipc_socket.o
	HDR_FILES	multios_ipc/mipc.h
	_CHILDREN	FOLDER_AF_MIPC
	CFG_PARAMS	AF_MIPC_NUM_SOCKETS
	CONFIGLETTES	usrMipcSocket.c
	INIT_RTN	usrMipcSocket();
	REQUIRES	INCLUDE_MIPC_SM \
			INCLUDE_SOCKLIB
	INIT_AFTER	INCLUDE_SOCKLIB	
}

Parameter AF_MIPC_NUM_SOCKETS {
	NAME		Maximum AF_MIPC sockets
	SYNOPSIS	Specifies the maximum number of AF_MIPC sockets that can exist simultaneously on this node. (-1: can have as many AF_MIPC sockets as MIPC sockets)
	TYPE		INT
	DEFAULT		-1
}

Component INCLUDE_AF_MIPC_DEMO {
	NAME		AF_MIPC demo
	SYNOPSIS	Kernel-space demo for AF_MIPC sockets.
	MODULES		afmipcdemo.o
	LINK_SYMS	afmipcdemo
	_CHILDREN	FOLDER_AF_MIPC
	REQUIRES	INCLUDE_AF_MIPC_SOCKETS
}
