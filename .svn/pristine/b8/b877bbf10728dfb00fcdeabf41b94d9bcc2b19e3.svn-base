/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,07jul09,syt  created
*/

Bsp fsl_p2020_ds_vpx6 {
   
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550     \
                INCLUDE_VXIPI
}
Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_DMA_SYS     \
                INCLUDE_MII_BUS
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}




/*
 * VX_SMP_NUM_CPUS is a SMP parameter only and only available for SMP
 * builds. Due to a limitation of the project tool at the time this
 * parameter is created where the tool can not recognize the ifdef SMP
 * selection, this parameter is set up such that _CFG_PARAMS is not
 * specified here. In the 00vxWorks.cdf file, where the parameter
 * VX_SMP_NUM_CPUS is defined, the _CFG_PARAMS is specified only for
 * VxWorks SMP. Hence the redefining of VX_SMP_NUM_CPUS here should only
 * override the value and not the rest of the properties. And for UP, the
 * parameter is ignored since the parameter is not tied to any component
 * (_CFG_PARAMS is not specified).
 */

Parameter VX_SMP_NUM_CPUS {
    NAME        Number of CPUs available to be enabled for VxWorks SMP
    TYPE        UINT
    DEFAULT     2
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                (INCLUDE_BOOT_APP)::(0x01800000)        \
                0x01000000
}
Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME     MDIO anchor adrs
    DEFAULT  (0x4400 + 0x20000000)
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}
/*-------------------------DataLight-------------------------------*/
/*-------------------------------------------------------------------
	General Settings
-------------------------------------------------------------------*/
Folder FOLDER_FLASHFX
{
	NAME		Datalight FlashFX Pro Flash Media Manager
	SYNOPSIS	FlashFX Pro Components
	_CHILDREN	FOLDER_PERIPHERALS
	CHILDREN	INCLUDE_FLASHFX_DRIVER		\
			INCLUDE_FLASHFX_TOOLS		\
			INCLUDE_FLASHFX_TESTS		\
	           	INCLUDE_FLASHFX_DISK_0		\
			INCLUDE_FLASHFX_DISK_1		\
			INCLUDE_FLASHFX_DISK_2		\
			INCLUDE_FLASHFX_DISK_3
	DEFAULTS	INCLUDE_FLASHFX_DRIVER		\
			INCLUDE_FLASHFX_DISK_0		\
			INCLUDE_FLASHFX_DISK_1		\
			INCLUDE_FLASHFX_TOOLS		\
			INCLUDE_FLASHFX_TESTS
}
/*-------------------------------------------------------------------
	DISK1 Settings
-------------------------------------------------------------------*/
Parameter FFX_DISK_DEVICENUM_1
{
	NAME		Device Number
	SYNOPSIS	The FlashFX device on which DISK1 is to be created (DEV0 to DEVn).
	TYPE		uint
	DEFAULT		1
}
