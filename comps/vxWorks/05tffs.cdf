/* 05tffs.cdf - True Flash File System component description */

/*
 * Copyright (c) 1999, 2001-2003, 2005, 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,16jan13,x_z  added INCLUDE_TFFS_STUB_VXBFLASH.
01o,28jun12,zly  Fix WIND00341863: adding INCLUDE_TFFS_START_SECTOR_ZERO to 
                 config if compatible with old format.
01n,11aug11,zly  fix WIND00292424:update module list for INCLUDE_TFFS
01m,10jun10,jxu  Removed tffs component from LP64 build.
01l,16jul09,wqi  changed INCLUDE_MTD_WAMDMTD to INCLUDE_MTD_WAMD. (WIND00076463)
01k,13sep05,pcm  changed INCLUDE_TFFS_DOSFS to INCLUDE_TFFS_MOUNT
01j,21jun05,pcm  removed INCLUDE_DOSFS dependency
01i,22oct03,dat  Move folder components here.
01h,20nov03,kk   updated INCLUDE_XXX_SHOW components to depend on
                 INCLUDE_SHOW_ROUTINES 
01g,30jan03,tor  Mount TFFS filesystem at boot time (spr #69254)
01f,26feb02,yp   adding INCLUDE_MTD_CFIAMD component
01e,05dec01,nrv  add INCLUDE_TFFS_SHOW component
01d,13nov01,nrv  re-written to configure MTDs and TL through project
01c,22apr99,pr   added dependency on DOSFS
01b,20apr99,pr   removed reference to ftllite.h msyscard.h ssfdc.h (SPR 26910)
01a,03feb99,yp   written
*/

/*
This file contains descriptions for the TrueFFS Flash file system
components. This file describes the "generic" component which is available
to all CPU architectures and BSPs.  MTD and "Socket" configuration must
be done in sysTffs.c.
*/

#ifndef _WRS_CONFIG_LP64

/* TFFS is attached in the FOLDER_PERIPHERALS folder */

Folder  FOLDER_TFFS {
        NAME          TrueFFS
        SYNOPSIS      TFFS components
	_CHILDREN     FOLDER_PERIPHERALS
        CHILDREN      INCLUDE_TFFS         \
		      INCLUDE_TFFS_MOUNT   \
                      INCLUDE_TFFS_SHOW    \
                      INCLUDE_TFFS_START_SECTOR_ZERO \
                      FOLDER_TFFS_DRIVERS  \
                      FOLDER_TFFS_TL
}


Folder  FOLDER_TFFS_DRIVERS {
        NAME                  TrueFFS drivers
        CHILDREN              INCLUDE_MTD_AMD \
                              INCLUDE_MTD_I28F008 \
                              INCLUDE_MTD_I28F008BAJA \
                              INCLUDE_MTD_I28F016 \
                              INCLUDE_MTD_WAMD \
                              INCLUDE_MTD_CFIAMD \
                              INCLUDE_MTD_CFISCS
}


Folder  FOLDER_TFFS_TL {
        NAME             TrueFFS translation layer
        CHILDREN         INCLUDE_TL_FTL \
                         INCLUDE_TL_SSFDC
}

Component INCLUDE_TFFS {
	NAME		TrueFFS Flash File System
	SYNOPSIS	Allows a DOS file system to placed in Flash memory
	INIT_RTN	tffsDrv ();
	CONFIGLETTES	usrTffs.c
        BSP_STUBS       sysTffs.c   
        MODULES         backgrnd.o \
                        fatlite.o \
                        flbase.o \
                        flflash.o \
                        flsocket.o \
                        fltl.o \
                        tffsDrv.o \
                        tffsLib.o
	HDR_FILES	tffs/backgrnd.h tffs/dosformt.h tffs/fatlite.h \
			tffs/flbase.h tffs/flbuffer.h tffs/flcustom.h  \
			tffs/flflash.h tffs/flsocket.h tffs/flsysfun.h \
			tffs/flsystem.h tffs/fltl.h tffs/pcic.h \
			tffs/reedsol.h tffs/stdcomp.h tffs/tffsDrv.h
	_INIT_ORDER	usrIosExtraInit
}

Component INCLUDE_TFFS_START_SECTOR_ZERO {
	NAME                 Starting sector of File System
        SYNOPSIS             Compatible with old format. \
                             Include it make flFileSysSectorStart=0 for dosfs, \
                             otherwise flFileSysSectorStart=1 for reliance
        REQUIRES             INCLUDE_TFFS
}

/*
 * NOTE: Show routines can be connected in the show routines folder too.
 * The system allows components to exist in more than one folder, but
 * I'm not sure if that was a design feature, or just luck. 
 */

Component INCLUDE_TFFS_SHOW {
        NAME                 TrueFFS Show Routines
        SYNOPSIS             TrueFFS show routines: tffsShow and tffsShowAll
	INCLUDE_WHEN	INCLUDE_TFFS \
			INCLUDE_SHOW_ROUTINES
}

Component INCLUDE_MTD_AMD {
        NAME            amdmtd
        SYNOPSIS        AMD, Fujitsu: 29F0{40,80,16} 8-bit devices
        MODULES         amdmtd.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_CFIAMD {
        NAME            cfiamdmtd
        SYNOPSIS        CFI driver for AMD Flash Part
        MODULES         cfiamd.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_I28F008 {
        NAME            i28f008
        SYNOPSIS        Intel 28f008 devices
        MODULES         i28f008.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_I28F008BAJA {
        NAME            i28f008Baja
        SYNOPSIS        Intel 28f008 on the Heurikon Baja 4000
        MODULES         i28f008Baja.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_I28F016 {
        NAME            i28f016
        SYNOPSIS        Intel 28f016 devices
        MODULES         i28f016.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_WAMD {
        NAME            wamdmtd
        SYNOPSIS        AMD, Fujitsu 29F0{40,80,16} 16-bit devices
        MODULES         wamdmtd.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_MTD_CFISCS {
        NAME            cfiscs
        SYNOPSIS        CFI/SCS devices
        MODULES         cfiscs.o
        HDR_FILES       tffs/flflash.h tffs/backgrnd.h
        REQUIRES        INCLUDE_TFFS \
                        INCLUDE_TL_FTL
}

Component INCLUDE_TL_FTL {
        NAME            ftllite
        SYNOPSIS        NOR Logic Translation Layer
        MODULES         ftllite.o
        HDR_FILES       tffs/fltl.h
        REQUIRES        INCLUDE_TFFS
}


Component INCLUDE_TL_SSFDC {
        NAME            ssfdc
        SYNOPSIS        Translation Layer Solid State Floppy Disk Card
        MODULES         ssfdc.o
        HDR_FILES       tffs/fltl.h
        REQUIRES        INCLUDE_TFFS
}

Component INCLUDE_TFFS_MOUNT {
	NAME		Mounted TFFS Filesystem
	SYNOPSIS	DosFS filesystem mounted on TFFS at boot time
	CFG_PARAMS	TFFS_DRIVE_NUMBER TFFS_REMOVABLE TFFS_MOUNT_POINT
        REQUIRES        INCLUDE_TFFS             \
                        INCLUDE_FS_MONITOR
	_INIT_ORDER	usrIosExtraInit
	INIT_RTN	usrTffsConfig (TFFS_DRIVE_NUMBER, TFFS_REMOVABLE, TFFS_MOUNT_POINT );
}

Parameter TFFS_DRIVE_NUMBER {
	NAME		TFFS Drive Number
	TYPE		int
	DEFAULT		0
}

Parameter TFFS_REMOVABLE {
	NAME		Removable flash device
	TYPE		bool
	DEFAULT		FALSE
}

Parameter TFFS_MOUNT_POINT {
	NAME		TFFS Mount point
	TYPE		string
	DEFAULT		"/tffs0"
}

Component INCLUDE_TFFS_STUB_VXBFLASH {
        NAME            TFFS stub to vxBus Flash interface
        SYNOPSIS        TFFS stub to vxBus Flash interface
        _CHILDREN       FOLDER_NOT_VISIBLE
}
#endif /*#ifndef _WRS_CONFIG_LP64*/
