/* 20comp_msd.cdf - configuration for MIPC Serial Device */

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
01j,02oct09,jlk  WIND00184907 - MSD config string bad.
01i,07jul09,jlk  Code inspection changes
01h,18oct08,jlk  WIND00136113 - Configure multiple MSDs
01g,08oct08,jlk  WIND00137348: Cleanup.
01f,22aug08,jlk  Added folder information and updated per code review.
01e,08aug08,jlk  Updated as per HLD review.
01d,21jul08,jlk  Cleanup. Renamed to MSD
01c,20jun08,jlk  Renamed to mio
01b,07may08,jlk  added INCLUDE_SEM_COUNTING
01a,17apr08,jlk  written.
*/

Folder FOLDER_MSD {
	NAME		MIPC Serial Device
	SYNOPSIS	Serial device between multiple operating systems.
	_CHILDREN	FOLDER_MIPC
}

Component INCLUDE_MSD {
	NAME		Multi-OS IPC serial device
	SYNOPSIS	MSD library that initializes several devices.
	_INIT_ORDER	usrIosCoreInit
	MODULES		msdLib.o msd_common.o
	HDR_FILES	multios_ipc/msdLib.h
	CONFIGLETTES	usrMsd.c
	INIT_RTN	usrMsdInit (MSD_TX_BUFS, MSD_TX_PRIO, \
			    MSD_TX_OPTS, MSD_MTU, MSD_TTY_BUF_SZ,\
			    MSD_NUM_DEVS, MSD_CFG_STR);
	CFG_PARAMS	MSD_TX_BUFS \
			MSD_TX_PRIO \
			MSD_TX_OPTS \
			MSD_MTU \
			MSD_TTY_BUF_SZ \
			MSD_NUM_DEVS \
			MSD_CFG_STR
	REQUIRES	INCLUDE_MIPC_SM
	INIT_AFTER	INCLUDE_TTY_DEV
	_CHILDREN	FOLDER_MSD
}

Parameter MSD_TX_BUFS {
	NAME		Number of transmission buffers
	SYNOPSIS	Sets the number of transmission buffers of the \
underlying MIPC socket. The value must be a power of two and no greater than 64.
	TYPE		int
	DEFAULT		8
}

Parameter MSD_TX_PRIO {
	NAME		Priority of MSD transmissions
	SYNOPSIS	Sets the priority of transmissions over MSD \
communication channels. 
	TYPE		int
	DEFAULT		50
}

Parameter MSD_TX_OPTS {
	NAME		Transmission options
	SYNOPSIS	For the current release, do not change this \
parameter.
	TYPE		int
	DEFAULT		0
}

Parameter MSD_MTU {
	NAME		Max bytes in data packet
	SYNOPSIS	Determines the maximum size, in bytes, of the data \
that follows the MSD packet header. A value of -1 means use the maximum as \
allowed by the MTU of the MIPC bus.
	TYPE		int
	DEFAULT		-1
}

Parameter MSD_TTY_BUF_SZ {
	NAME		TTY Buffer size
	SYNOPSIS	Size, in bytes, of the TTY receive and transmit \
buffers. The value should be larger than the value of the MSD_MTU (Max \
bytes in data packet) parameter.
	TYPE		int
	DEFAULT		2048
}

Parameter MSD_NUM_DEVS {
	NAME		Number of MSD devices
	SYNOPSIS	Determines the number of MSDs on this core. The \
maximum value is 128.

	TYPE		int
	DEFAULT		1
}

Parameter MSD_CFG_STR {
	NAME		MSD configuration string
	SYNOPSIS	Specifies the MSD devices configure.
	TYPE		char *
	DEFAULT		"#auto=y bus=main"
}



