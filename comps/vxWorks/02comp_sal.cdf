/* 02comp_sal.cdf - SAL Component configuration file */

/*
 * Copyright (c) 2003-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,05mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
01o,10may05,ebh  allow user-defined distributed SNS server TIPC type +
                 instance
		 delete synopsis fields that duplicate name fields
01n,07apr05,ebh  Adding support for distributed sal (INCLUDE_SNS_MP)
01m,01mar05,kkz  Removing single quote
01l,21feb05,ebh  update manually (zipply merge was not running)
01k,04jan05,bwa  added SNS_LISTEN_BACKLOG parameter (SPR #102955).
01j,11nov04,job  Changed initialization order of SAL component
01i,04nov04,p_r  Usability issue (SPR #103651)
01h,16sep04,ymz  Added SNS_PRIORITY and SNS_STACK_SIZE to INCLUDE_SNS.
01g,13sep04,bwa  enhanced layout in IDE.
01f,01sep04,ymz  added INCLUDE_SNS_COMMON, INCLUDE_SAL_COMMON 
                 and INCLUDE_SNS_RTP_HOOK.
01e,25may04,ymz  updated due to SAL file merging
01d,24apr04,p_r  fixed dsi pathnames
01c,24dec04,ymz  added INCLUDE_SNS_SHOW init routine
01b,08dec03,p_r  added COMP dependency 
01a,17nov03,p_r  written.
*/

/* 
DESCRIPTION

This file contains all components for the Socket Application Library and
the Socket Name Service.
There are three main components independent from each other and a show routine
for the Socket Name Service.
The three main components are:
- Socket Application Library Client
- Socket Application Library Server
- Socket Name Service Server

All three components need to be present in a system in order for SAL to work
properly. They do not need to be in the kernel. This module describes the
configuration of SAL for the kernel. Normally they will reside on different 
memory spaces, possibly on different instances of the system. Therefore there 
can be different scenarios for the configuration of SAL:

1. All three components reside in the same kernel. 
2. The SNS is in the kernel and the Client and Server are in different memory 
space.
3. The SNS, the client and server are all in user space, but in different memory
spaces.
4. The SNS is in user space and either the client or the server are in the kernel.
5. The SNS is in user space and the client and the server are in the same user 
space different from SNS. 

There can be several instances of clients in one node, and also different 
instances of servers libraries. 
There MUST be one and only one instance of the SNS in the same node. If it 
resides in user space, it has to be in a separate RTP.
Each node that participates in using distributed named sockets must have
an SNS server running on that node (either kernel or user space).

*/

Folder	FOLDER_SAL {
	NAME		Socket Application Library components
	SYNOPSIS	Client, Server, and Show routines for the Socket Application Library
	CHILDREN	SELECT_SNS_SERVER     \
                        INCLUDE_SAL_CLIENT    \
                        INCLUDE_SAL_SERVER    \
                        INCLUDE_SNS_SHOW    
	_CHILDREN	FOLDER_DSI
	DEFAULTS	INCLUDE_SAL_CLIENT    \
                        INCLUDE_SAL_SERVER    \
                        SELECT_SNS_SERVER
}

Component INCLUDE_SAL_CLIENT {
	NAME		Socket Application Library (Client)
	SYNOPSIS	Client portion of the Socket Application Library
	MODULES		salClient.o
	LINK_SYMS	salOpen 
	REQUIRES        INCLUDE_SAL_COMMON \
	                SELECT_SNS_SERVER
}

Component INCLUDE_SAL_SERVER {
	NAME		Socket Application Library (Server)
	SYNOPSIS	Server portion of the Socket Application Library
	MODULES		salServer.o salSns.o 
	LINK_SYMS	salCreate
	REQUIRES        INCLUDE_SAL_COMMON \
	                SELECT_SNS_SERVER
}

Component INCLUDE_SAL_COMMON {
	NAME		Socket Application Common Library
	MODULES		salCommon.o
	REQUIRES        INCLUDE_UN_COMP
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Selection	SELECT_SNS_SERVER {
	NAME		Socket Name Service server (Kernel or RTP)
	SYNOPSIS	Location of SNS server: kernel or RTP
	COUNT		1-1
	CHILDREN	INCLUDE_SNS	\
                        INCLUDE_SNS_RTP \
                        INCLUDE_SNS_MP  \
                        INCLUDE_SNS_MP_RTP
	DEFAULTS	INCLUDE_SNS
}

Component INCLUDE_SNS {
	NAME		Socket Name Service in Kernel
	SYNOPSIS	The Socket Name Service server resides in Kernel space, AF_LOCAL support only
	CONFIGLETTES    dsi/usrSalSnsInit.c
	INIT_RTN	salSnsTaskInit ();
        CFG_PARAMS      SNS_PRIORITY   \
                        SNS_STACK_SIZE \
                        SNS_LISTEN_BACKLOG
	EXCLUDES        INCLUDE_SNS_RTP \
                        INCLUDE_SNS_MP \
                        INCLUDE_SNS_MP_RTP
	REQUIRES        INCLUDE_SAL_SERVER \
			INCLUDE_SNS_COMMON \
			INCLUDE_UN_COMP
}

Component INCLUDE_SNS_MP {
	NAME		Distributed Socket Name Service in Kernel
	SYNOPSIS	The Distributed Socket Name Service server resides in Kernel space, includes AF_LOCAL and AF_TIPC support
	CONFIGLETTES    dsi/usrSalSnsInit.c
	INIT_RTN	salSnsTaskInit ();
        CFG_PARAMS      SNS_PRIORITY   \
                        SNS_STACK_SIZE \
                        SNS_LISTEN_BACKLOG \
			SNS_DISTRIBUTED_SERVER_TYPE \
			SNS_DISTRIBUTED_SERVER_INSTANCE
	EXCLUDES        INCLUDE_SNS \
                        INCLUDE_SNS_RTP \
                        INCLUDE_SNS_MP_RTP
	REQUIRES        INCLUDE_SAL_SERVER \
			INCLUDE_SNS_COMMON \
			INCLUDE_UN_COMP \
			INCLUDE_TIPC
}

Parameter SNS_PRIORITY
        {
        NAME            Socket Name Service task priority
        SYNOPSIS        Recommended priority value: 50
        TYPE            int
        DEFAULT         50
        }

Parameter SNS_STACK_SIZE
        {
        NAME            Socket Name Service task stack size
        SYNOPSIS        Recommended stack size: 20000
        TYPE            int
        DEFAULT         20000
        }

Parameter SNS_LISTEN_BACKLOG
        {
        NAME            Socket Name Service listen sockets backlog size
        SYNOPSIS        Increase if connections get refused.  Default:  5
        TYPE            int
        DEFAULT         5
        }

Parameter SNS_DISTRIBUTED_SERVER_TYPE
        {
        NAME            Distributed Socket Name Service TIPC Type
        SYNOPSIS        Forms TIPC type,instance of SNS server.  Default Type:  70
        TYPE            int
        DEFAULT         70
        }

Parameter SNS_DISTRIBUTED_SERVER_INSTANCE
        {
        NAME            Distributed Socket Name Service TIPC Instance
        SYNOPSIS        Forms TIPC type,instance of SNS server.  Default Instance:  1
        TYPE            int
        DEFAULT         1
        }

Component INCLUDE_SNS_SHOW {
	NAME		Socket Name Service Show Routine
	SYNOPSIS	Show routine for the Socket Name Service
	MODULES		snsShow.o 
	LINK_SYMS	snsShow
	REQUIRES        INCLUDE_SAL_COMMON \
                        INCLUDE_SNS_COMMON
}

Component INCLUDE_SNS_COMMON {
	NAME		Socket Name Service Common Library
	MODULES		snsCommon.o
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_SNS_RTP_HOOK {
	NAME		Socket Name Service RTP hook
	MODULES		snsRtpHook.o
	REQUIRES        INCLUDE_SAL_COMMON
	_CHILDREN	FOLDER_NOT_VISIBLE
}

InitGroup usrSalInit {
	SYNOPSIS	SNS daemon init group
	INIT_RTN	usrSalInit ();
        INIT_ORDER      INCLUDE_SNS     \
                        INCLUDE_SNS_RTP \
                        INCLUDE_SNS_MP \
                        INCLUDE_SNS_MP_RTP
        _INIT_ORDER     usrToolsInit
	INIT_BEFORE     usrShellInit
}
