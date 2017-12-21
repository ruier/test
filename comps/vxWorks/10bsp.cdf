/* 10bsp.cdf - secondary BSP components */

/*
 * Copyright (c) 1998, 2001-2005, 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,12nov10,mze  _WRS_VX_SMP is now obsolete replaced with _WRS_CONFIG_SMP
01o,29jul09,e_d  removed the name detection for ATA device.(WIND00139792)
01n,04oct07,mze  fix for 104829
01m,02aug07,h_k  made SCSI components invisible in SMP
01l,13oct05,dee  SPR# 113433, better error handling for non-existent ATA
                 drives.  Remove USE_XBD_BLOCK_WRAPPER parameter.
01k,29aug05,dee  add parameter USE_XBD_BLOCK_WRAPPER for usrAta.c
01j,16jun05,pcm  added more dependencies to INCLUDE_FD
01i,09jun05,pcm  removed CBIO and Dos FS dependencies from INCLUDE_ATA
01h,07jun05,pcm  renamed DOSFS_NAMES_xxx to FS_NAMES_xxx
01g,01oct04,fle  Removed // style comments
01f,23oct03,dat  move all TFFS to 05tffs.cdf
01e,20nov03,kk   updated INCLUDE_XXX_SHOW components to depend on 
		 INCLUDE_SHOW_ROUTINES
01d,30jan03,tor  Mount TFFS & SCSI filesystems at boot time (spr #69254)
01c,09jul02,jkf  SPR#78374, assume only one dos volume on ctrl=0 drive=0,
                 ATA_DEV_NAMES_0 is DOSFS_NAMES_ATA_PRIMARY_MASTER, etc
01b,09nov01,jac  Added ATA_DEV_NAMES_2 and ATA_DEV_NAMES_3 to define devices
                 on secondary ATA controller.  Added support for defining
                 cdromFs as file system for an ATA target device.
01a,14oct98,lrn  written
*/

/*
This file contains additions and modifications for BSP components.

XXX - to do
*/

/* Floppy Disk Section */

Parameter FD_DEV_NAME {
	NAME		Floppy disk logical device name
	TYPE		string
	DEFAULT		"/fd"	/* 0 or 1 will be appended */
}

Parameter FD_0_TYPE {
	NAME		Floppy Disk 0 drive type 
	SYNOPSIS	Use 0 for 3.5 and 1 for 5.25 inch drives
	TYPE		int
	DEFAULT		0
}
Parameter FD_1_TYPE {
	NAME		Floppy Disk 1 drive type 
	SYNOPSIS	Use 0 for 3.5 and 1 for 5.25 inch drives,\
			Use NONE of none installed.
	TYPE		int
	DEFAULT		NONE
}
	
Component INCLUDE_FD {
	NAME		floppy drive (NEC 765)
	SYNOPSIS	NEC 765 floppy drive component
	MODULES		nec765Fd.o
	CONFIGLETTES	usrFd.c
	CFG_PARAMS	FD_INT_VEC  \
                        FD_INT_LVL  \
			FD_DEV_NAME \
			FD_0_TYPE   \
                        FD_1_TYPE
	HDR_FILES	drv/fdisk/nec765Fd.h
	INIT_RTN	fdDrv (FD_INT_VEC, FD_INT_LVL);              \
			usrFdConfig(0, FD_0_TYPE, FD_DEV_NAME "0");  \
			usrFdConfig(1, FD_1_TYPE, FD_DEV_NAME "1");
        REQUIRES        INCLUDE_FS_MONITOR
}

/* ATA Hard drive component */

Parameter FS_NAMES_ATA_PRIMARY_MASTER {
        NAME            ATA Controller 0, Hard disk 0 logical names
        SYNOPSIS        Comma separated list for each partition: "/ata0a,/ata0b"
        TYPE            string
        DEFAULT         "/ata0a"
}
Parameter FS_NAMES_ATA_PRIMARY_SLAVE {
        NAME            ATA Controller 0, Hard disk 1 logical names
        SYNOPSIS        Comma separated list for each partition: "/ata1a,/ata1b"
        TYPE            string
        DEFAULT         ""
}
Parameter FS_NAMES_ATA_SECONDARY_MASTER {
        NAME            ATA Controller 1, Hard disk 0 logical names
        SYNOPSIS        Comma separated list for each partition: "/ata2a,/ata2b"
        TYPE            string
        DEFAULT         ""
}
Parameter FS_NAMES_ATA_SECONDARY_SLAVE {
        NAME            ATA Controller 1, Hard disk 1 logical names
        SYNOPSIS        Comma separated list for each partition: "/ata3a,/ata3b"
        TYPE            string
        DEFAULT         ""
}


Component INCLUDE_ATA {
	NAME		ATA hard drive
	SYNOPSIS	ATA hard drive component
	MODULES		ataDrv.o
	CONFIGLETTES	usrAta.c
	CFG_PARAMS	FS_NAMES_ATA_SECONDARY_SLAVE    \
                        FS_NAMES_ATA_SECONDARY_MASTER   \
                        FS_NAMES_ATA_PRIMARY_SLAVE      \
                        FS_NAMES_ATA_PRIMARY_MASTER 
	HDR_FILES	drv/hdisk/ataDrv.h
	INIT_RTN	usrAtaInit ();\
            usrAtaConfig (0, 0, FS_NAMES_ATA_PRIMARY_MASTER); \
            usrAtaConfig (0, 1, FS_NAMES_ATA_PRIMARY_SLAVE);  \
            usrAtaConfig (1, 0, FS_NAMES_ATA_SECONDARY_MASTER); \
            usrAtaConfig (1, 1, FS_NAMES_ATA_SECONDARY_SLAVE);
        REQUIRES        INCLUDE_FS_MONITOR
}

Component INCLUDE_ATA_SHOW {
	NAME            ATA hard drive information display/show 
	MODULES		ataShow.o
	INIT_RTN	ataShowInit();
	REQUIRES	INCLUDE_ATA
	_CHILDREN	FOLDER_HD
	INCLUDE_WHEN	INCLUDE_ATA \
			INCLUDE_SHOW_ROUTINES
}

#ifndef	_WRS_CONFIG_SMP
/* SCSI hard disk component */

Folder	FOLDER_SCSI {
	NAME		scsi
	SYNOPSIS	SCSI components
	CHILDREN	INCLUDE_SCSI		\
			INCLUDE_SCSI_DOSFS	\
			SELECT_SCSI_VERSION
}

Component INCLUDE_SCSI {
	NAME		scsi
	SYNOPSIS	SCSI components
	CONFIGLETTES	usrScsi.c
	MODULES		scsiLib.o
	INIT_RTN	usrScsiConfig ();
	HDR_FILES	sysLib.h stdio.h scsiLib.h
        REQUIRES        INCLUDE_FS_MONITOR
}

Component INCLUDE_SCSI_DOSFS {
	NAME		scsi FileSystem
	SYNOPSIS	Filesystem on SCSI magnetic drive
	CONFIGLETTES	usrScsiFs.c
	INIT_RTN	usrScsiFsConfig (SCSI_FS_DEV_BUS_ID,        \
                                         SCSI_FS_DEV_LUN,           \
                                         SCSI_FS_DEV_REQ_SENSE_LEN, \
                                         SCSI_FS_DEV_TYPE,          \
                                         SCSI_FS_DEV_REMOVABLE,     \
                                         SCSI_FS_DEV_NUM_BLOCKS,    \
                                         SCSI_FS_DEV_BLK_SIZE,      \
                                         SCSI_FS_OFFSET,            \
                                         SCSI_FS_MOUNT_POINT);
	REQUIRES	INCLUDE_SCSI         \
			INCLUDE_DOSFS
	CFG_PARAMS	SCSI_FS_DEV_BUS_ID        \
			SCSI_FS_DEV_LUN           \
			SCSI_FS_DEV_REQ_SENSE_LEN \
			SCSI_FS_DEV_TYPE          \
			SCSI_FS_DEV_REMOVABLE     \
			SCSI_FS_DEV_NUM_BLOCKS    \
			SCSI_FS_DEV_BLK_SIZE      \
			SCSI_FS_OFFSET            \
			SCSI_FS_MOUNT_POINT
}

Parameter SCSI_FS_DEV_BUS_ID {
	NAME		SCSI FileSystem Device Bus ID
	SYNOPSIS	SCSI FileSystem Device Bus ID (0 - 7)
	TYPE		int
	DEFAULT		-1
}

Parameter SCSI_FS_DEV_LUN {
	NAME		SCSI FileSystem Device Logical Unit Number
	SYNOPSIS	SCSI FileSystem Device Logical Unit Number
	TYPE		int
	DEFAULT		0
}

Parameter SCSI_FS_DEV_REQ_SENSE_LEN {
	NAME		SCSI FileSystem Device REQUEST SENSE length
	SYNOPSIS	SCSI FileSystem Device REQUEST SENSE length
	TYPE		int
	DEFAULT		0
}

Parameter SCSI_FS_DEV_TYPE {
	NAME		SCSI FileSystem Device Type
	SYNOPSIS	SCSI FileSystem Device Type
	TYPE		int
	DEFAULT		-1
}

Parameter SCSI_FS_DEV_REMOVABLE {
	NAME		SCSI FileSystem Device Removable flag
	SYNOPSIS	SCSI FileSystem Device Removable flag
	TYPE		int
	DEFAULT		0
}

Parameter SCSI_FS_DEV_NUM_BLOCKS {
	NAME		SCSI FileSystem Device Number of Blocks
	SYNOPSIS	SCSI FileSystem Device Number of Blocks
	TYPE		int
	DEFAULT		0
}

Parameter SCSI_FS_DEV_BLK_SIZE {
	NAME		SCSI FileSystem Device Block Size
	SYNOPSIS	SCSI FileSystem Device Block Size
	TYPE		int
	DEFAULT		512
}

Parameter SCSI_FS_OFFSET {
	NAME		SCSI FileSystem Offset on device
	SYNOPSIS	SCSI FileSystem Offset on device
	TYPE		int
	DEFAULT		0
}

Parameter SCSI_FS_MOUNT_POINT {
	NAME		SCSI FileSystem mount point
	SYNOPSIS	SCSI FileSystem mount point
	TYPE		string
	DEFAULT		"/scsi"
}
#endif	/* !_WRS_CONFIG_SMP */
