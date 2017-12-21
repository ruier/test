/* 20comp_tipc.cdf - TIPC Component configuration file */

/*
 * Copyright (c) 2004-2008, 2010, Wind River Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
/*
modification history
--------------------
02i,11may10,h_x  Port the patches to VxWorks from Linux. 
02h,30jul09,h_x  Fix WIND00174718
02k,02jun09,h_w  Add InitGroup usrTipcInit (Defect WIND00162024) 
02j,08oct08,als  Update to TIPC 1.7.6 (Defect WIND00127984)
02i,07sep07,mze  remove REQUIRES INCLUDE_TIPC_SHOW for TIPC_TS and TIPC_IS
                 Components WIND00103862
02h,20aug07,mze  moved BUILD_TIPC_SRC into a new cdf that is only installed if source is
02g,30jul07,mze  fixed WIND00099891 put INCLUDE_TIPC_HEND_INIT in advanced folder  
02f,26jul07,mze  changes for SOURCE_ONLY builds
02e,17jul07,mze  fixed list in SOURCE_ONLY and added USE_LIBTIPC
02d,28jun07,mze  added SOURCE_ONLY keyword
02c,08may07,mze  added INCLUDE_TIPC_DEFINES
02b,12apr07,ebh  Fixed typo
02a,08feb07,mze  changes to the wording in the INCLUDE_CONFIG_TIPC... components
01z,12dec06,mze  mods to new components after testing
01y,02dec06,mze  made INCLUDE_WVTIPC dependant on LINK_SYMS wvTipcInit
01x,01dec06,mze  added new folders and components for footprint reduction
01w,11sep06,mze  added new select to bring in INCLUDE_NET_POOL when we use
                 the IP stack  WIND00064552
01v,16aug06,mze  added more info to synopsis for INCLUDE_WVTIPC
01u,28jul06,mze  move INCLUDE_WVTIPC into this file
01t,21jun06,mze  move tipcConfigShow into Tipc folder
01s,23apr06,ebh  fix typos in customer viewable text
01r,13mar06,ebh  Added TIPC_SOCK_RXQ_LIMIT parameter for sockets
01q,06mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
                 add SELECT_TIPC_INIT and make REQUIRES changes to get defaults
                 to work, fixed text in some SYNOPSIS fields to better reflect
                 component requirements/use
01p,21feb06,ebh  Revert maximum cluster size back to 65536 after problems found
01o,20feb06,mze  removing 32byte pools, increase max TIPC cluster size to 66076
01n,20jan06,mze  changed driver.o to core.o tipcShow.o to tipcConfig.o
01m,11nov05,mze  Moved INCLUDE_TIPC_HEND_INIT component to its own cdf
		     SPR114816
01l,21sep05,dsk  Added INCLUDE_TIPC_HEND_INIT component and its parameter
01k,15sep05,ebh  Update TIPC components names for clarity and eliminate
                 an incorrect EXCLUDE specification for TIPC
01j,20aug05,ebh  Fix prototype for usrTipcSocket()
01i,20jul05,dsk  mods according to code inspection
01h,20jul05,dsk  added TIPC socket backend and TIPC mempool components
01g,14apr05,bwa  moved SM media type child to its component (SPR107526).
01f,14apr05,als  added licensing info
01e,18mar05,bwa  added INCLUDE_TIPC_MEDIA_SM component reference.
01d,16feb05,als  Added support for multiple media types
01c,07sep04,als  Incorporated code inspection feedback
01b,04aug04,als  added configuration components, missing REQUIRES, etc.
01a,09jul04,p_r  written.
*/

/*
DESCRIPTION

This file contains the basic components for TIPC, the Transparent Inter-Process
Communication protocol.  TIPC components lie in the Network Protocol Components
folder.

Including the main INCLUDE_TIPC component allows applications to use the
default TIPC configuration, which provides intra-node communication only.
This configuration can be modified by including a static configuration string
and/or a dynamic configuration string hook routine, using the
INCLUDE_TIPC_CONFIG_STR component and SELECT_TIPC_CONFIG_HOOK selection items.
These items control the value of TIPC_CONFIG_STR and TIPC_CONFIG_HOOK,
respectively, which are passed to TIPC when it is initialized.

Including the INCLUDE_TIPC_SHOW component brings in the optional TIPC
configuration and display routines.

*/

Folder	FOLDER_TIPC {
	NAME		TIPC components
	SYNOPSIS	Components for the Transparent Inter-Process Communication protocol
	CHILDREN	INCLUDE_TIPC \
                        SELECT_TIPC_INIT \
                        SELECT_TIPC_MEDIA_TYPES \
                        SELECT_TIPC_STACK \
                        FOLDER_TIPC_ADVANCED \
			INCLUDE_TIPC_SHOW
	_CHILDREN	FOLDER_OS
	DEFAULTS 	INCLUDE_TIPC
}

Folder FOLDER_TIPC_ADVANCED {
	NAME		TIPC advanced configuration
	SYNOPSIS	Advanced components/options for TIPC
        CHILDREN        INCLUDE_CONFIG_TIPC_SOCKET_API \
                        FOLDER_TIPC_FOOTPRINT \
                        INCLUDE_TIPC_MEMPOOL \
                        INCLUDE_TIPC_HEND_INIT
        DEFAULTS	INCLUDE_TIPC_MEMPOOL \
                        INCLUDE_CONFIG_TIPC_SOCKET_API
}

InitGroup usrTipcInit {
    PROTOTYPE   void usrTipcInit (void)
    INIT_RTN    usrTipcInit ();
    SYNOPSIS    Tipc initialization
    INIT_ORDER  \
                INCLUDE_TIPC_DEFINES \
                INCLUDE_CONFIG_TIPC_SOCKET_API \
                INCLUDE_TIPC_MEMPOOL \
                INCLUDE_TIPC
}

Component INCLUDE_TIPC {
	NAME		TIPC
	SYNOPSIS	Transparent Inter-Process Communication protocol
	HDR_FILES       tipc/tipc.h
        LINK_SYMS       tipc_core_start
	CONFIGLETTES	usrTipc.c
	INIT_RTN	tipc_init (TIPC_CONFIG_STR, TIPC_CONFIG_HOOK);
        REQUIRES	INCLUDE_NETWORK \
			SELECT_TIPC_MEDIA_TYPES \
			SELECT_TIPC_INIT \
			SELECT_TIPC_BUILD \
			INCLUDE_TIPC_MEMPOOL \
			SELECT_TIPC_STACK   \
                        INCLUDE_TIPC_DEFINES
}

Folder FOLDER_TIPC_FOOTPRINT {
	NAME		TIPC footprint reduction
	SYNOPSIS	Components that will reduce the footprint for TIPC. Note \
                        these components require TIPC to be built from source.
        CHILDREN        INCLUDE_TIPC_NODEBUG \
                        INCLUDE_TIPC_NOSYS_MSGS \
                        INCLUDE_TIPC_NOCFG_SERVICE \
                        INCLUDE_TIPC_NOSOCKET
}

Selection SELECT_TIPC_BUILD {
	NAME		TIPC library selection
	SYNOPSIS	Dynamic configuration source for TIPC
	COUNT		1-1
	CHILDREN	INCLUDE_USE_LIBTIPC
        _CHILDREN	FOLDER_TIPC_ADVANCED
	DEFAULTS	INCLUDE_USE_LIBTIPC
}

Component INCLUDE_USE_LIBTIPC {
	NAME		Build TIPC from object library
	SYNOPSIS	Selecting this component forces the use of the standard \
                        libtipc.a. This is the default.
        ARCHIVE         libtipc.a
        REQUIRES	INCLUDE_TIPC
}

Component INCLUDE_TIPC_NODEBUG {
	NAME		No TIPC debug
	SYNOPSIS	Debug code is not included if this option is selected.
        REQUIRES	INCLUDE_TIPC INCLUDE_BUILD_TIPC_SRC
}

Component INCLUDE_TIPC_NOSYS_MSGS {
	NAME		No TIPC system messages
	SYNOPSIS	System message code is not included if this option is selected.
        REQUIRES	INCLUDE_TIPC INCLUDE_BUILD_TIPC_SRC
}

Component INCLUDE_TIPC_NOCFG_SERVICE {
	NAME		No TIPC configuration service
	SYNOPSIS	Configuration code is not included if this option is selected.
        REQUIRES	INCLUDE_TIPC INCLUDE_BUILD_TIPC_SRC
}

Component INCLUDE_TIPC_NOSOCKET {
	NAME		No TIPC socket API
	SYNOPSIS	TIPC socket code is not included if this option is selected. 
        EXCLUDES        INCLUDE_CONFIG_TIPC_SOCKET_API
        REQUIRES	INCLUDE_TIPC INCLUDE_BUILD_TIPC_SRC
}

Selection SELECT_TIPC_INIT {
	NAME		TIPC initialization
	SYNOPSIS	TIPC initialization methods
	COUNT		0-
	CHILDREN	SELECT_TIPC_CONFIG_HOOK \
			INCLUDE_TIPC_CONFIG_STR
	_CHILDREN	FOLDER_TIPC
	DEFAULTS	SELECT_TIPC_CONFIG_HOOK
}

Component INCLUDE_TIPC_CONFIG_STR {
	NAME		TIPC static configuration
	SYNOPSIS	Static configuration string for TIPC
	CFG_PARAMS	TIPC_CONFIG_STR
	REQUIRES	INCLUDE_TIPC
}

Parameter TIPC_CONFIG_STR {
	NAME		Static Configuration
	SYNOPSIS	Configuration info (enclosed in double quotes) or NULL. \
	Example: "netid=1900;a=1.1.123;be=eth:fei0;be=sm:sm0;log=1024"
	TYPE    	string
	DEFAULT		NULL
}

Selection SELECT_TIPC_CONFIG_HOOK {
	NAME		TIPC dynamic configuration
	SYNOPSIS	Dynamic configuration source for TIPC
	COUNT		0-1
	CHILDREN	INCLUDE_TIPC_CONFIG_HOOK_BOOT	\
			INCLUDE_TIPC_CONFIG_HOOK_USER
	DEFAULTS	INCLUDE_TIPC_CONFIG_HOOK_BOOT
}

Component INCLUDE_TIPC_CONFIG_HOOK_BOOT {
	NAME		Bootline configuration
	SYNOPSIS	TIPC uses bootline "other" parameter. \
	Example: netid=1900;a=1.1.123;be=eth:fei0;be=sm:sm0;log=1024
	REQUIRES	INCLUDE_TIPC
}

Component INCLUDE_TIPC_CONFIG_HOOK_USER {
	NAME		User configuration
	SYNOPSIS	TIPC invokes user-defined tipcConfigInfoGet() routine. \
	EXAMPLE DEFINE : STATUS tipcConfigInfoGet(char * buffer,  UINT bufferlen)
	REQUIRES	INCLUDE_TIPC
}

Selection SELECT_TIPC_MEDIA_TYPES {
	NAME		TIPC media types
	SYNOPSIS	Media types that can be used by TIPC
	COUNT		0-
	CHILDREN	INCLUDE_TIPC_MEDIA_ETH
	DEFAULTS	INCLUDE_TIPC_MEDIA_ETH
}

Component INCLUDE_TIPC_MEDIA_ETH {
	NAME		Ethernet
	SYNOPSIS	Allows TIPC to communicate over Ethernet
	REQUIRES	INCLUDE_TIPC
	_CHILDREN	SELECT_TIPC_MEDIA_TYPES
}

Component INCLUDE_TIPC_SHOW {
	NAME		TIPC configuration and display routines
	SYNOPSIS	tipcConfig, tipcSysPoolShow, tipcDataPoolShow
	LINK_SYMS	tipcConfig
        EXCLUDES        INCLUDE_TIPC_NOCFG_SERVICE \
                        INCLUDE_TIPC_NOSOCKET
        REQUIRES	INCLUDE_TIPC
        _CHILDREN	FOLDER_TIPC
}

Component INCLUDE_CONFIG_TIPC_SOCKET_API {
	NAME		TIPC socket API
	SYNOPSIS	TIPC socket configuration parameters.
	PROTOTYPE	void usrTipcSocket (void);
	CONFIGLETTES	tipc/usrTipcSocket.c
	INIT_RTN	usrTipcSocket();
        INCLUDE_WHEN    INCLUDE_TIPC
        EXCLUDES        INCLUDE_TIPC_NOSOCKET
        REQUIRES	INCLUDE_TIPC \
			INCLUDE_SOCKLIB \
			INCLUDE_TIPC_MEMPOOL
	CFG_PARAMS	TIPC_NUM_SOCKETS \
			TIPC_SOCK_RXQ_LIMIT
}

Parameter TIPC_NUM_SOCKETS {
	NAME		Number of TIPC sockets
	SYNOPSIS	Number of MBLKS, CBLKS, and clusters in System Pool. Default: 200
	DEFAULT		200
	TYPE		uint
}

Parameter TIPC_SOCK_RXQ_LIMIT {
	NAME		Socket receive queue threshold
	SYNOPSIS	Number of queued messages on a TIPC socket before \
	congestion occurs. Default: 2500
	DEFAULT		2500
	TYPE		uint
}

Component INCLUDE_TIPC_DEFINES {
	NAME		TIPC system defines
	SYNOPSIS	TIPC system configuration parameters. \
	(Note: These default settings may be overridden during system initialization \
	by means of a static or dynamic configuration string.)
	PROTOTYPE	void usrTipcDefsInit (void);
	CONFIGLETTES	usrTipc.c
	INIT_RTN	usrTipcDefsInit ();
	CFG_PARAMS	TIPC_DEF_NET_ID        \
    			TIPC_DEF_MAX_PORTS        \
    			TIPC_DEF_MAX_NODES        \
    			TIPC_DEF_MAX_CLUSTERS        \
    			TIPC_DEF_MAX_ZONES        \
    			TIPC_DEF_MAX_REMOTES        \
    			TIPC_DEF_MAX_PUBS        \
    			TIPC_DEF_MAX_SUBS        \
    			TIPC_DEF_LOG_SIZE        \
    			TIPC_DEF_REMOTE_MGT
	INCLUDE_WHEN    INCLUDE_TIPC
        REQUIRES        INCLUDE_TIPC
	_CHILDREN   	FOLDER_TIPC_ADVANCED
}

Parameter TIPC_DEF_NET_ID {
	NAME		Network ID
	SYNOPSIS	Specifies which TIPC network this node belongs to; \
	can range from 1 to 9999. Default value: 4711
	DEFAULT		4711
	TYPE		uint
}

Parameter TIPC_DEF_MAX_PORTS {
	NAME		Max Ports
	SYNOPSIS	Specifies how many ports can be supported by this node; \
	can range from 127 to 65535 ports. Default value: 8191
	DEFAULT		8191
	TYPE		uint
}

Parameter TIPC_DEF_MAX_NODES {
	NAME		Max Nodes
	SYNOPSIS	Specifies how many nodes can be supported in own TIPC cluster; \
	can range from 8 to 4095 nodes. Default value: 255
	DEFAULT		255
	TYPE		uint
}

Parameter TIPC_DEF_MAX_CLUSTERS {
	NAME		Max Clusters
	SYNOPSIS	Specifies how many clusters can be supported in own TIPC zone; \
	can range from 1 to 4095 clusters. Default value: 8
	DEFAULT		8
	TYPE		uint
}

Parameter TIPC_DEF_MAX_ZONES {
	NAME		Max Zones
	SYNOPSIS	Specifies how many zones can be supported in own TIPC network; \
	can range from 1 to 255 zones. Default value: 4
	DEFAULT		4
	TYPE		uint
}

Parameter TIPC_DEF_MAX_REMOTES {
	NAME		Max Remotes
	SYNOPSIS	Specifies how many nodes in other clusters this node can establish links to; \
	can range from 0 to 255 nodes. Default value: 8
	DEFAULT		8
	TYPE		uint
}

Parameter TIPC_DEF_MAX_PUBS {
	NAME		Max Publications
	SYNOPSIS	Specifies how many concurrent name publications this node can issue; \
	can range from 1 to 65535 publications. Default value: 10000
	DEFAULT		10000
	TYPE		uint
}

Parameter TIPC_DEF_MAX_SUBS {
	NAME		Max Subscriptions
	SYNOPSIS	Specifies how many concurrent name subscriptions this node can handle; \
	can range from 1 to 65535 subscriptions. Default value: 2000
	DEFAULT		2000
	TYPE		uint
}

Parameter TIPC_DEF_LOG_SIZE {
	NAME		Log Buffer Size
	SYNOPSIS	Size (in bytes) of TIPC's log buffer, which records significant events for later review; \
	can range from 0 to 32768 bytes. Default value: 0
	DEFAULT		0
	TYPE		uint
}

Parameter TIPC_DEF_REMOTE_MGT {
	NAME		Remote Management
	SYNOPSIS	Specifies if this node can be remotely managed from another node in the TIPC network; \
	can be set to 1 (enabled) or 0 (disabled). Default value: 1
	DEFAULT		1
	TYPE		uint
}

Component INCLUDE_TIPC_MEMPOOL {
	NAME		TIPC memory pool
	SYNOPSIS	Number of data clusters used in messaging
	PROTOTYPE	void usrTipcPoolConfig (void);
	CONFIGLETTES	tipc/usrTipcPool.c
	INIT_RTN	usrTipcPoolConfig ();
	CFG_PARAMS	TIPC_DATA_00064        \
    			TIPC_DATA_00128        \
    			TIPC_DATA_00256        \
    			TIPC_DATA_00512        \
    			TIPC_DATA_01024        \
    			TIPC_DATA_02048        \
    			TIPC_DATA_04096        \
    			TIPC_DATA_08192        \
    			TIPC_DATA_16384        \
    			TIPC_DATA_32768        \
    			TIPC_DATA_65536
	REQUIRES	INCLUDE_NETBUFLIB   \
    			INCLUDE_NETBUFPOOL  \
    			INCLUDE_TIPC
	_CHILDREN   	FOLDER_TIPC_ADVANCED
}

Parameter TIPC_DATA_00064 {
	NAME		Number of    64 byte clusters
	SYNOPSIS	Used for messages of up to 64 bytes including headers. Default value: 120
	DEFAULT		120
	TYPE		uint
}

Parameter TIPC_DATA_00128 {
	NAME		Number of   128 byte clusters
	SYNOPSIS	Used for messages of up to 128 bytes including headers. Default value: 200
	DEFAULT		200
	TYPE		uint
}

Parameter TIPC_DATA_00256 {
	NAME		Number of   256 byte clusters
	SYNOPSIS	Used for messages of up to 256 bytes including headers. Default value: 40
	DEFAULT		40
	TYPE		uint
}

Parameter TIPC_DATA_00512 {
	NAME		Number of   512 byte clusters
	SYNOPSIS	Used for messages of up to 512 bytes including headers. Default value: 40
	DEFAULT		40
	TYPE		uint
}

Parameter TIPC_DATA_01024 {
	NAME		Number of  1024 byte clusters
	SYNOPSIS	Used for messages of up to 1024 bytes including headers. Default value: 50
	DEFAULT		50
	TYPE		uint
}

Parameter TIPC_DATA_02048 {
	NAME		Number of  2048 byte clusters
	SYNOPSIS	Used for messages of up to 2048 bytes including headers. Default value: 20
	DEFAULT		20
	TYPE		uint
}

Parameter TIPC_DATA_04096 {
	NAME		Number of  4096 byte clusters
	SYNOPSIS	Used for messages of up to 4096 bytes including headers. Default value: 2
	DEFAULT		2
	TYPE		uint
}

Parameter TIPC_DATA_08192 {
	NAME		Number of  8192 byte clusters
	SYNOPSIS	Used for messages of up to 8192 bytes including headers. Default value: 0
	DEFAULT		0
	TYPE		uint
}

Parameter TIPC_DATA_16384 {
	NAME		Number of 16384 byte clusters
	SYNOPSIS	Used for messages of up to 16384 bytes including headers. Default value: 0
	DEFAULT		0
	TYPE		uint
}

Parameter TIPC_DATA_32768 {
	NAME		Number of 32768 byte clusters
	SYNOPSIS	Used for messages of up to 32768 bytes including headers. Default value: 0
	DEFAULT		0
	TYPE		uint
}

Parameter TIPC_DATA_65536 {
	NAME		Number of 65536 byte clusters
	SYNOPSIS	Used for messages of up to 65536 bytes including headers. Default value: 0
	DEFAULT		0
	TYPE		uint
}

Selection SELECT_TIPC_STACK {
	NAME		TIPC stack support
	SYNOPSIS	Select one of INCLUDE_TIPC_IP or INCLUDE_TIPC_ONLY
	COUNT		1-1
	CHILDREN	INCLUDE_TIPC_IP	\
			INCLUDE_TIPC_ONLY
	DEFAULTS	INCLUDE_TIPC_IP
}

Component INCLUDE_TIPC_IP {
	NAME		TIPC and IP network stacks present
	SYNOPSIS	Required component if the IP stack is part of the project.
	REQUIRES	INCLUDE_NET_POOL
}

Component INCLUDE_TIPC_ONLY {
	NAME		TIPC network stack only
	SYNOPSIS	Required component if the IP stack is not included. Remove \
			FOLDER_NETWORK first and then add this component.
	REQUIRES	INCLUDE_TIPC \
			INCLUDE_END             \
			INCLUDE_MUX             \
			INCLUDE_NETBUFPOOL      \
			INCLUDE_LINKBUFPOOL
	EXCLUDES	INCLUDE_NET_BOOT
}

Component INCLUDE_WVTIPC {
        NAME            TIPC instrumentation
        SYNOPSIS        System Viewer instrumentation for TIPC. 
        REQUIRES	INCLUDE_WINDVIEW \
                        INCLUDE_TIPC \
                        INCLUDE_BUILD_TIPC_SRC
        INIT_RTN        wvTipcInit ();
        PROTOTYPE       void wvTipcInit (void);
        _CHILDREN   	FOLDER_WINDVIEW
}

Folder	FOLDER_TIPC_DEMOS {
	NAME		TIPC kernel demos
	SYNOPSIS	Contains TIPC VIP demos that can be included as components
	CHILDREN	INCLUDE_TIPC_TS \
                        INCLUDE_TIPC_IS
	_CHILDREN	FOLDER_TIPC_ADVANCED
	DEFAULTS 	INCLUDE_TIPC_TS
}

Component INCLUDE_TIPC_TS {
	NAME		TIPC test suite demo
	SYNOPSIS	Add the TIPC test suite to the kernel image
        SOURCE_ONLY     {$(TGT_DIR)/src/demo/tipcTestSuite {tipc_ts_client.o tipc_ts_server.o \
                        tipc_ts_client_vxworks.o tipc_ts_server_vxworks.o tipc_ts_common.o } }
        REQUIRES	INCLUDE_TIPC \
                        INCLUDE_SHELL
}

Component INCLUDE_TIPC_IS {
	NAME		TIPC inventory simulation demo
	SYNOPSIS	Add the TIPC inventory simulation to the kernel image
        SOURCE_ONLY     {$(TGT_DIR)/src/demo/tipcInventorySim {tipcInventorySim_VxWorks.o } }
        REQUIRES	INCLUDE_TIPC \
                        INCLUDE_SHELL
}

