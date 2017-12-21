/* 01wdbproxy.cdf - WDB Agent Proxy components */

/*
 * Copyright (c) 2005, 2007-2009, 2011, 2012 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01j,16feb12,elp  added serial backend support.
01i,23may11,rlp  Added TCP support.
01j,13may11,rlp  Used IPCOM secure key database to store the private key
                 (WIND00271834).
01i,15apr11,rlp  Added INCLUDE_WDB_PROXY_SSL component to the proxy folder
                 (CQ:WIND00268084).
01h,25mar11,rlp  Added SSL support.
01g,23mar11,rlp  Added a usrWdbProxy configlette.
01f,16mar11,jmp  Performed some component naming/synopsis updates.
01e,03jan11,rlp  Added INCLUDE_IPNET dependency (CQ:WIND00241952).
01d,23apr09,rlp  Removed private task option value (CQ:WIND00163553).
01c,23jul08,dbt  Added MIPC support. Added more proxy parameters.
01b,06dec07,rlp  Removed tornado references (CQ:WIND00112534).
01a,01jun05,dbt  written

DESCRIPTION

This file contains descriptions for all of the WDB Agent Proxy components.
*/

Folder FOLDER_WDB_PROXY {
	NAME		WDB Agent Proxy components
	SYNOPSIS	software agent for Workbench tools connections
	_CHILDREN	FOLDER_TOOLS
	CHILDREN	INCLUDE_WDB_PROXY		\
			INCLUDE_WDB_PROXY_SSL		\
			SELECT_WDB_PROXY_BACKEND	\
	DEFAULTS	INCLUDE_WDB_PROXY		\
			SELECT_WDB_PROXY_BACKEND
	HELP		WDB
}

InitGroup usrWdbProxyInit {
	INIT_RTN	usrWdbProxyInit ();
	SYNOPSIS	WDB Agent Proxy
	_INIT_ORDER	usrToolsInit
	INIT_ORDER	INCLUDE_WDB_PROXY_UDP		\
			INCLUDE_WDB_PROXY_TCP		\
			INCLUDE_WDB_PROXY_TIPC		\
			INCLUDE_WDB_PROXY_MIPC		\
			INCLUDE_WDB_PROXY_SERIAL	\
			INCLUDE_WDB_PROXY
}

Component INCLUDE_WDB_PROXY {
	NAME		WDB Agent Proxy
	SYNOPSIS	Proxy to communicate with agents
	MODULES		wrProxyLib.o
	HDR_FILES	wrProxyLib.h
        CONFIGLETTES    usrWdbProxy.c
	INIT_RTN	usrWdbProxyCfgInit (WDB_PROXY_PORT, WDB_PROXY_INIT_STRING, WDB_PROXY_HAS_CONTROL_PORT, WDB_PROXY_TASK_PRIORITY, WDB_PROXY_TASK_STACK_SIZE, WDB_PROXY_TASK_OPTIONS);
	REQUIRES	SELECT_WDB_PROXY_BACKEND \
			INCLUDE_IPNET
	CFG_PARAMS	WDB_PROXY_PORT WDB_PROXY_TASK_PRIORITY \
			WDB_PROXY_TASK_STACK_SIZE WDB_PROXY_TASK_OPTIONS \
			WDB_PROXY_HAS_CONTROL_PORT WDB_PROXY_INIT_STRING
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_SSL {
	NAME		SSL support for WDB Agent Proxy
	SYNOPSIS	Proxy to communicate with agents through SSL
	MODULES		wrProxySslLib.o
	HDR_FILES	wrProxyLib.h
	REQUIRES	INCLUDE_WDB_PROXY \
			INCLUDE_IPSSL \
			SELECT_IPCOM_FS
	CFG_PARAMS	WDB_PROXY_SSL_REPOSITORY
}

Selection SELECT_WDB_PROXY_BACKEND {
	NAME		WDB Agent Proxy backend
	SYNOPSIS	Select the WDB Proxy backend to use
	COUNT		1-
	CHILDREN	INCLUDE_WDB_PROXY_TIPC		\
			INCLUDE_WDB_PROXY_MIPC		\
			INCLUDE_WDB_PROXY_TCP		\
			INCLUDE_WDB_PROXY_SERIAL	\
			INCLUDE_WDB_PROXY_UDP		
	DEFAULTS	INCLUDE_WDB_PROXY_UDP
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_UDP {
	NAME		UDP/IP WDB Agent Proxy backend
	SYNOPSIS	UDP/IP WDB Agent Proxy backend
	HDR_FILES	wrProxyLib.h
	INIT_RTN	wrProxyUdpSockRegister ();
	MACRO_NEST	INCLUDE_WDB_PROXY
	REQUIRES	INCLUDE_WDB_PROXY
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_TCP {
	NAME		TCP/IP WDB Agent Proxy backend
	SYNOPSIS	TCP/IP WDB Agent Proxy backend
	HDR_FILES	wrProxyLib.h
	INIT_RTN	wrProxyTcpSockRegister ();
	MACRO_NEST	INCLUDE_WDB_PROXY
	REQUIRES	INCLUDE_WDB_PROXY
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_TIPC {
	NAME		TIPC WDB Agent Proxy backend
	SYNOPSIS	TIPC WDB Agent Proxy backend
	HDR_FILES	wrProxyLib.h
	INIT_RTN	wrProxyTipcSockRegister ();
	MACRO_NEST	INCLUDE_WDB_PROXY
	REQUIRES	INCLUDE_TIPC INCLUDE_WDB_PROXY
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_MIPC {
	NAME		MIPC WDB Agent Proxy backend
	SYNOPSIS	MIPC WDB Agent Proxy backend
	HDR_FILES	wrProxyLib.h
	MODULES		wrProxyMipcSockLib.o
	INIT_RTN	wrProxyMipcSockRegister ();
	MACRO_NEST	INCLUDE_WDB_PROXY
	REQUIRES	INCLUDE_MIPC_SM INCLUDE_WDB_PROXY 
	HELP		tgtsvr WDB
}

Component INCLUDE_WDB_PROXY_SERIAL {
	NAME		Serial WDB Agent Proxy backend
	SYNOPSIS	Serial WDB Agent Proxy backend
	HDR_FILES	wrProxyLib.h
	MODULES		wrProxySerialLib.o
	INIT_RTN	wrProxySerialRegister ();
	MACRO_NEST	INCLUDE_WDB_PROXY
	REQUIRES	INCLUDE_WDB_PROXY 
	HELP		tgtsvr WDB
}

Parameter WDB_PROXY_PORT {
	NAME		WDB Agent Proxy TCP/IP port
	SYNOPSIS	TCP/IP port used for WDB Agent Proxy connection
	TYPE		ushort
	DEFAULT		0x4444
}


Parameter WDB_PROXY_TASK_PRIORITY {
	NAME		WDB Agent Proxy task priority
	SYNOPSIS	WDB Agent Proxy task priority
	TYPE		uint
	DEFAULT		51
}

Parameter WDB_PROXY_TASK_STACK_SIZE {
	NAME		WDB Agent Proxy task stack size
	SYNOPSIS	WDB Agent Proxy task stack size
	TYPE		uint
	DEFAULT		10000
}

Parameter WDB_PROXY_TASK_OPTIONS {
        NAME            WDB Agent Proxy task options
        SYNOPSIS	WDB Agent Proxy task options
        DEFAULT         VX_UNBREAKABLE
        TYPE            int
}

Parameter WDB_PROXY_HAS_CONTROL_PORT {
	NAME		WDB Agent Proxy control port support
	SYNOPSIS	If TRUE enable WDB Agent Proxy control port
	TYPE		BOOL
	DEFAULT		TRUE
}

Parameter WDB_PROXY_INIT_STRING {
	NAME		WDB Agent Proxy init string
	SYNOPSIS	String-encoded list of wdb proxy commands to run at startup. Several commands can be specified separated with '|' character.
    	TYPE            string
	DEFAULT		""
}

Parameter WDB_PROXY_SSL_REPOSITORY {
	NAME		Repository for SSL authentication files
	SYNOPSIS	This is the repository for certificates used during the authentication process of SSL proxy connection
    	TYPE            string
	DEFAULT		NULL
}
