/* 20comp_multios.cdf - configuration for Multi-OS systems */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,01jun09,dcc  added MIPC_DEMO to INIT_ORDER
01l,17feb09,ebh  Add initialization of Multi-OS back into file
01k,14jan09,ebh  Updates for MIPC 2.0 (based on Andre's changes)
01j,31oct08,ebh  Remove MOB_PLB_WDB reference
01i,24oct08,ebh  Fix naming of MOB for PPC
01h,04sep08,jlk  Removed SMP limitation
01g,02sep08,ebh  Renamed MOB_PLB_2 to MOB_PLB_1
01f,12aug08,bwa  added multiple instances of a bus.
01e,05aug08,elp  add simulator support.
01d,08may08,agf  add cavium CN38XX support
01c,14apr08,dtr  Add 8572 MCB platform driver.
01b,25mar08,bwa  merged.
01a,24feb08,bwa  written.
*/

Folder FOLDER_MULTIOS {
	NAME		Multi-OS
	SYNOPSIS	Support for system with multiple operating systems
	_CHILDREN	FOLDER_ROOT
}

InitGroup usrMultiosInit {
	PROTOTYPE	void usrMultiosInit (void)
	INIT_RTN	usrMultiosInit ();
	SYNOPSIS	Multi-OS support
	INIT_ORDER	\
			INCLUDE_MIPC_SM \
			INCLUDE_MIPC_DEMO
	/* end of init modules */
}


