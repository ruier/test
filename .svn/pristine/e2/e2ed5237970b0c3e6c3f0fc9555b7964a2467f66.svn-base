/* 02comp_net_unshow.cdf - AF_LOCAL domain show routines configuration file */

/*
 * Copyright (c) 2004, 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,05mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
01d,20sep04,bwa  removed init group.
01c,20aug04,bwa  removed single-quote in INCLUDE_UN_COMP_SHOW synopsis.
01b,16aug04,bwa  removed references to 'unix'; added PROTOTYPEs.
01a,26jan04,bwa  written
*/

/* 
DESCRIPTION

This file contains the component for the AF_LOCAL domain show routines.

*/

Folder	FOLDER_AF_LOCAL_SHOW {
	NAME		AF_LOCAL address domain show routines
	SYNOPSIS	AF_LOCAL address domain show routines
	CHILDREN	INCLUDE_UN_SHOW \
			INCLUDE_UN_COMP_SHOW
	_CHILDREN	FOLDER_NET_SHOW
}

Component INCLUDE_UN_SHOW {
	NAME		AF_LOCAL domain show routines
	SYNOPSIS	AF_LOCAL address domain and harness
        PROTOTYPE       void unShowInit (void);
	INIT_RTN	unShowInit ();
	MODULES		unShow.o 
	REQUIRES	INCLUDE_UN
	_CHILDREN	FOLDER_AF_LOCAL_SHOW
}

Component INCLUDE_UN_COMP_SHOW {
	NAME		COMP protocol show routine
	SYNOPSIS	show routine for AF_LOCAL domain COMP protocol
        PROTOTYPE       void uncompShowInit (void);
	INIT_RTN	uncompShowInit ();
	MODULES		uncompShow.o
	REQUIRES        INCLUDE_UN_SHOW \
                        INCLUDE_UN_COMP
	_CHILDREN	FOLDER_AF_LOCAL_SHOW
}

