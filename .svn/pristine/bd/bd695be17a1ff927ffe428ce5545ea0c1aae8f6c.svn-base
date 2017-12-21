/* 02folder_dsi.cdf - Root folder for DSI components */

/*
 * Copyright (c) 2004-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,03mar10,zhw  move FOLDER_DSI into FOLDER_OS (WIND00195159)
01k,06mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
01j,18feb06,mze  combined 02folder_dsi.cdf 02comp_dsi_pool.cdf 
                          02folder_dsi_backend.cdf 02comp_dsi_socket.cdf
                          into this file for ease of maintainence
01i,17feb06,mze  added fix for SELECT_DSI_INIT and reordered the socket pools
                 so that they are in numerical order based on the parameter name
01h,03jan05,bwa  removed 'Net' part of routine names (SPR#103232);
                 removed dsiSockLib.h reference (SPR#105213).
01g,04nov04,p_r  fixed NAME field (SPR 103651). 
01f,20sep04,bwa  now part of usrDsiInit init group.            
01e,20sep04,bwa  removed init group.
01d,18aug04,bwa  dsi socket parent is now FOLDER_DSI_BACKEND.
01c,16aug04,bwa  increased default number of sockets/data clusters.
01b,16aug04,bwa  parent folder is now FOLDER_DSI_BACKEND.
01a,16aug04,bwa  written.
*/


        


Folder FOLDER_DSI {
	NAME		Distributed Systems Infrastructure
	SYNOPSIS	Distributed Systems Infrastructure Components
	CHILDREN	FOLDER_AF_LOCAL     \
                        FOLDER_DSI_BACKEND  \
                        FOLDER_SAL     
        _CHILDREN       FOLDER_OS
        DEFAULTS        INCLUDE_DSI_POOL 
}

Folder FOLDER_DSI_BACKEND {
	NAME		DSI Backend
	SYNOPSIS	Socket Backend for the Distributed Systems Infrastructure
	CHILDREN	INCLUDE_DSI_POOL   \
			INCLUDE_DSI_SOCKET
        DEFAULTS        INCLUDE_DSI_POOL 
}
        

Component INCLUDE_DSI_SOCKET {
	NAME		DSI Backend support and initialization
	SYNOPSIS	Installs and initializes the DSI backend
	PROTOTYPE	STATUS usrDsiSocket (void);
	CONFIGLETTES	dsi/usrDsiSocket.c
	INIT_RTN	usrDsiSocket();
	MODULES		dsiSockLib.o
	REQUIRES	INCLUDE_SOCKLIB \
			INCLUDE_DSI_POOL \
                        SELECT_DSI_INIT
}
        
Component INCLUDE_DSI_POOL {
	NAME		DSI Memory pools configuration
	SYNOPSIS	Number of DSI sockets, amount of data clusters
	PROTOTYPE	void usrDsiPoolConfig (void);
	CONFIGLETTES	dsi/usrDsiPool.c
	INIT_RTN	usrDsiPoolConfig ();
	CFG_PARAMS	DSI_NUM_SOCKETS     \
                        DSI_DATA_32         \
                        DSI_DATA_64         \
                        DSI_DATA_128        \
                        DSI_DATA_256        \
                        DSI_DATA_512        \
                        DSI_DATA_1K         \
                        DSI_DATA_2K         \
                        DSI_DATA_4K         \
                        DSI_DATA_8K         \
                        DSI_DATA_16K        \
                        DSI_DATA_32K        \
			DSI_DATA_64K
	REQUIRES	INCLUDE_NETBUFLIB   \
                        INCLUDE_NETBUFPOOL  \
                        INCLUDE_DSI_SOCKET  
}

Parameter DSI_NUM_SOCKETS {
	NAME		Number of available sockets in DSI backend
	SYNOPSIS	Number of MBLKS, CBLKS & clusters in System Pool. Default value: 200
        DEFAULT         200
	TYPE		uint
}

Parameter DSI_DATA_32 {
	NAME		Number of    32 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 32 byte clusters in DSI Data Pool. Default value: 50
        DEFAULT         50
	TYPE		uint
}

Parameter DSI_DATA_64 {
	NAME		Number of    64 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 64 byte clusters in DSI Data Pool. Default value: 100
        DEFAULT         100
	TYPE		uint
}

Parameter DSI_DATA_128 {
	NAME		Number of   128 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 128 byte clusters in DSI Data Pool. Default value: 200
        DEFAULT         200
	TYPE		uint
}

Parameter DSI_DATA_256 {
	NAME		Number of   256 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 256 byte clusters in DSI Data Pool. Default value: 40
        DEFAULT         40
	TYPE		uint
}

Parameter DSI_DATA_512 {
	NAME		Number of   512 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 512 byte clusters in DSI Data Pool. Default value: 40
        DEFAULT         40
	TYPE		uint
}

Parameter DSI_DATA_1K {
	NAME		Number of  1024 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 1024 byte clusters in DSI Data Pool. Default value: 10
        DEFAULT         10
	TYPE		uint
}

Parameter DSI_DATA_2K {
	NAME		Number of  2048 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 2048 byte clusters in DSI Data Pool. Default value: 10
        DEFAULT         10
	TYPE		uint
}

Parameter DSI_DATA_4K {
	NAME		Number of  4096 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 4096 byte clusters in DSI Data Pool. Default value: 10
        DEFAULT         10
	TYPE		uint
}

Parameter DSI_DATA_8K {
	NAME		Number of  8192 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 8192 byte clusters in DSI Data Pool. Default value: 10
        DEFAULT         10
	TYPE		uint
}

Parameter DSI_DATA_16K {
	NAME		Number of 16384 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 16384 byte clusters in DSI Data Pool. Default value: 4
        DEFAULT         4
	TYPE		uint
}

Parameter DSI_DATA_32K {
	NAME		Number of 32768 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 32768 byte clusters in DSI Data Pool. Default value: 0
        DEFAULT         0
	TYPE		uint
}

Parameter DSI_DATA_64K {
	NAME		Number of 65536 byte clusters in DSI Data Pool
	SYNOPSIS	Number of 65536 byte clusters in DSI Data Pool. Default value: 0
        DEFAULT         0
	TYPE		uint
}

