/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2003-2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,12mar12,y_c  Add INCLUDE_BCM52XXPHY definition when INCLUDE_FCC_VXB_END
                 is defined. (WIND00333700)
01m,17oct08,jmt  Update components for bootapp support
01l,23jun08,d_c  Auto-exclude tests for unsupported components
01k,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01m,03apr07,h_k  fixed Component definitions which was changed to Selection by
                 mistake.
                 removed INCLUDE_FEI82557END which is not used anywhere.
01l,19jan07,wyc  add SYNOPSIS
01k,19jan07,wap  Remove MOTFCCEND and MOTSCCEND components
01j,13dec06,rec  vxWorks 6.5 changes
01i,24oct06,jmt  Add INCLUDE_BOOT_ETH_MAC_HANDLER
01h,06sep06,jmt  Add bootapp support
01g,16jul06,pee  added CPU to Bsp
01f,08feb06,agf  SPR 114494: add parameter for no. of FCC units
01e,02feb06,agf  add definition of Bsp to cdf
01d,28sep05,mil  Combined 00bsp.cdf and 20bsp.cdf.
01c,02sep05,mil  Added components to support kernel source build.
01b,10nov04,j_b  project support for boot Flash and TFFS flash selection (SPRs
                 103800,103790)
01a,23sep04,rec  written
*/

/*
DESCRIPTION
This file contains descriptions for this BSP and its components.
*/

/* Describe the BSP to the project tools */
Bsp     wrSbcPowerQuiccII {
        NAME		board support package
        REQUIRES	INCLUDE_KERNEL
        CPU		PPC32
        FP		hard
}


/* Definitions for the power management modes supported by the BSP. */

Selection SELECT_POWER_MGMT {
        NAME		Power Management Operating Mode
        COUNT		1-1
        CHILDREN	INCLUDE_CPU_SLEEP_MODE_OFF	 \
        INCLUDE_CPU_SLEEP_MODE_SHORT \
			INCLUDE_CPU_SLEEP_MODE_LONG
        DEFAULTS	INCLUDE_CPU_SLEEP_MODE_LONG
}

/* Memory definitions */

Parameter LOCAL_MEM_LOCAL_ADRS {
	NAME		Physical memory base address
	DEFAULT		0x00000000
}

Parameter RAM_HIGH_ADRS {
	NAME		Bootrom Copy region
	DEFAULT		(INCLUDE_BOOT_APP)::(0x01F00000) \
			0x01C00000
}

Parameter RAM_LOW_ADRS {
	NAME		Runtime kernel load address
	DEFAULT		(INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
			(INCLUDE_BOOT_APP)::(0x01C00000) \
			0x00100000
}

/* Definitions for the boot flash selections supported by the BSP. */

Selection SELECT_BOOT_FLASH {
        NAME		Boot Flash Jumper Selection for wrSbcPowerQuiccII
        SYNOPSIS        Selection of flash device for target boot-up
        COUNT		1-1
        _CHILDREN	FOLDER_BSP_CONFIG
        CHILDREN	INCLUDE_ON_BOARD_BOOT_FLASH \
	                INCLUDE_SIMM_BOOT_FLASH
        DEFAULTS	INCLUDE_ON_BOARD_BOOT_FLASH
}

Component INCLUDE_ON_BOARD_BOOT_FLASH {
        NAME             On Board Boot Flash (JP2: 1-3 & 2-4)
        SYNOPSIS         Boot from on-board flash (JP2: 1-3 & 2-4) 
}

Component INCLUDE_SIMM_BOOT_FLASH {
        NAME             SIMM Boot Flash (JP2: 1-2 & 3-4)
        SYNOPSIS         Boot from SIMM flash (JP2: 1-2 & 3-4)
        REQUIRES	 SELECT_BOOT_FLASH
}

/* Definitions for the TFFS flash selections supported by the BSP. */

Selection SELECT_TFFS_FLASH {
        NAME		TFFS Flash for wrSbcPowerQuiccII
        SYNOPSIS        Selection of flash device for TFFS
        COUNT		1-1
        _CHILDREN	FOLDER_BSP_CONFIG
        CHILDREN	INCLUDE_ON_BOARD_TFFS_FLASH	 \
                        INCLUDE_SIMM_TFFS_FLASH
        DEFAULTS	INCLUDE_SIMM_TFFS_FLASH
}

Component INCLUDE_ON_BOARD_TFFS_FLASH {
        NAME            On Board TFFS Flash
        SYNOPSIS        Place TFFS storage on the on-board flash device
        LAYER           1
        _CHILDREN       SELECT_TFFS_FLASH
        REQUIRES	SELECT_TFFS_FLASH \
                        INCLUDE_TL_FTL
}

Component INCLUDE_SIMM_TFFS_FLASH {
        NAME		SIMM TFFS Flash
        SYNOPSIS        Place TFFS storage on the SIMM flash device
        REQUIRES	SELECT_TFFS_FLASH \
        INCLUDE_TL_FTL
}

/* Components to support kernel source build */

Component INCLUDE_CACHE_BSP {
        NAME            BSP cache CPU varient select
        LAYER           1
        REQUIRES        INCLUDE_CACHE_ARCH_PPC60X
        INCLUDE_WHEN    INCLUDE_CACHE_SUPPORT
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_MMU_BSP {
        NAME            BSP MMU CPU varient select
        LAYER           1
        REQUIRES        INCLUDE_MMU_ARCH_PPC60X
        INCLUDE_WHEN    INCLUDE_MMU_GLOBAL_MAP
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_PCI {
        NAME            BSP PCI support
        SYNOPSIS        BSP PCI Support
        LAYER           3
        _CHILDREN       FOLDER_BSP_CONFIG
        REQUIRES        INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_PCI_AUTOCONF {
        NAME            BSP PCI auto config support
        SYNOPSIS        BSP PCI Auto Config Support
        LAYER           2
        _CHILDREN       FOLDER_BSP_CONFIG
        REQUIRES        INCLUDE_PCI
        INCLUDE_WHEN    INCLUDE_PCI
}

Component M48T59_RTC_SRAM {
        NAME            On board real time clock and NVRAM support
        SYNOPSIS        Real_Time Clock and NVRAM Support
        _CHILDREN       FOLDER_BSP_CONFIG
        LAYER           2
}


Component INCLUDE_SIO_POLL {
        /* always include when project is created, can be unselected. */
        INCLUDE_WHEN    += INCLUDE_KERNEL
}
/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
    	REQUIRES	INCLUDE_FCC_VXB_END \
			INCLUDE_SCC_VXB_END
}

Component       INCLUDE_FCC_VXB_END {
    	REQUIRES	INCLUDE_BCM52XXPHY
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */
Component	INCLUDE_BOOT_ETH_MAC_HANDLER {
	INCLUDE_WHEN 	INCLUDE_BOOT_NETWORK 
}

/*
 * Force unsupported components to be unavailable.
 *
 * The following component definition(s) forces the named component(s)
 * to be "unavailable" as far as the project build facility (vxprj) is
 * concerned. The required component (COMPONENT_NOT_SUPPORTED) should
 * never be defined, and hence the named component(s) will never be
 * available. This little trick is used by the BSPVTS build scripts
 * (suiteBuild, bspBuildTest.tcl) to automatically exclude test modules
 * that are not applicable to a BSP because the BSP does not support a
 * given component and/or test module. If and when support is added,
 * the following definition(s) should be removed.
 */

Component               INCLUDE_SIO_UTILS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_TIMER_SYS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
