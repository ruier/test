/* 02comp_un.cdf - AF_LOCAL domain Component configuration file */

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
01g,06mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
01f,04jan05,bwa  removed 'Net' from routine names (SPR#103232);
                 removed unCompLib.h reference (SPR#105213).
01e,20sep04,bwa  removed init group.
01d,16aug04,bwa  parent folder is now FOLDER_DSI.
01c,24apr04,p_r  fixed pathnames
01b,07apr04,bwa  changed 'netinet' directory reference to 'netun;
		 remove 'uncomp_subr.o' module reference;
                 added DSI references.
01a,24nov03,bwa  written
*/

/* 
DESCRIPTION

This file contains the component for the AF_LOCAL domain. This allows
the use of sockets on a local node without pulling in the whole network stack.

For this component to be of any use, at least one protocol for AF_LOCAL must
be included in the system as well (eg. COMP for SOCK_SEQPACKET).

*/

Folder	FOLDER_AF_LOCAL {
	NAME		AF_LOCAL address domain
	SYNOPSIS	Support for the AF_LOCAL address domain
	CHILDREN	INCLUDE_UN_COMP
	_CHILDREN	FOLDER_DSI
}

Component INCLUDE_UN {
	NAME		AF_LOCAL domain
	SYNOPSIS	AF_LOCAL address domain and harness
	CONFIGLETTES	dsi/usrUn.c
	PROTOTYPE	void usrUnInit (void);
	INIT_RTN	usrUnInit ();
	MODULES		unLib.o 
	REQUIRES	INCLUDE_DSI_SOCKET  \
                        INCLUDE_DSI_POOL
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_UN_COMP {
	NAME		COMP protocol
	SYNOPSIS	SOCK_SEQPACKET-based protocol for AF_LOCAL domain
	CONFIGLETTES	dsi/usrUnComp.c
	PROTOTYPE	void usrUnCompInit (void);
	INIT_RTN	usrUnCompInit ();
	MODULES		unCompLib.o	\
			uncomp_usrreq.o
	REQUIRES        INCLUDE_UN
}
