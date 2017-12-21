/* 10vrfs.cdf - Kernel component definitions for VRFS */

/*
 * Copyright (c) 2005-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,01aug07,act  io footprint work (from pcm)
01a,21dec05,rfr  written
*/

Component INCLUDE_VRFS
	{
	NAME		Virtual Root File System
	SYNOPSIS	File System which exports the Core I/O device table
	MODULES		vrfsLib.o
	INIT_RTN	usrVrfsInit ();
        CONFIGLETTES    usrVrfs.c
	REQUIRES	INCLUDE_IO_FILE_SYSTEM
	}

