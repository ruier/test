/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2010-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,10jan13,y_y  add vxBus SPI flash support.
01f,08mar12,d_l  Added P1020RDB_PC_REVB and DRV_VXB_NORFLASH_CMDSET_AMD
                 to default settings.
01e,28apr12,d_l  Added SPI components.
01d,10apr12,x_z  Added support for P1020RDB-PC Rev B board with DDR3. 
01c,31aug11,swu  Add parameters ETSECx_CORE_NUM for etsec.
                 (WIND00260801)
01b,21jul11,swu  Support for AMP
01a,22mar10,syt  derived from version 01a of fsl_p2020_rdb/20bsp.cdf
*/

Bsp fsl_p1020_rdb {
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550     \
                DRV_VXB_NORFLASH_CMDSET_AMD
}

/* Support for P1020RDB-PC Rev B board */

Component P1020RDB_PC_REVB  {
    NAME        P1020RDB-PC Rev B board support
    SYNOPSIS    support for P1020RDB-PC Rev B board
    _CHILDREN   FOLDER_BSP_CONFIG
}

Profile PROFILE_BOOTAPP {
    COMPONENTS += P1020RDB_PC_REVB 
}
/* Override the default (empty) component definitions with custom options */

/* Support for P1020RDB-PC Rev B board */

Component P1020RDB_PC_REVB  {
    NAME        P1020RDB-PC Rev B board support
    SYNOPSIS    support for P1020RDB-PC Rev B board
    _CHILDREN   FOLDER_BSP_CONFIG
}

/* Override the default (empty) component definitions with custom options */

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_QCOM_AR8021PHY \
                INCLUDE_DUMMY_MDIO  \
                INCLUDE_TSEC_MDIO   \
                INCLUDE_VSC82XXPHY  \
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

/* required components for AMP */

Bundle BUNDLE_AMP_PRI {
    COMPONENTS  += INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
    COMPONENTS  += INCLUDE_AMP
}

Component INCLUDE_AMP {
    NAME        Base AMP Support
    REQUIRES    INCLUDE_AMP_CPU \
                INCLUDE_VXIPI \
                INCLUDE_TIMER_SYS \
                INCLUDE_VXCPULIB
    _CHILDREN   FOLDER_BSP_CONFIG
}

Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER
    _CHILDREN   FOLDER_BSP_CONFIG
}

/* Override the default (empty) component definitions with custom options */

Component INCLUDE_AMP_CPU_01  {
    NAME        AMP kernel configured for core 01
}

Component INCLUDE_AMP_CPU_00  {
    NAME        AMP kernel configured for core 00
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
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                (INCLUDE_BOOT_APP)::(0x01400000)        \
                (INCLUDE_AMP_CPU_00)::(0x01000000) \
                (INCLUDE_AMP_CPU_01) && (P1020RDB_PC_REVB)::(0x22000000) \
                (INCLUDE_AMP_CPU_01)::(0x12000000) \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01400000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_AMP_CPU_01) && (P1020RDB_PC_REVB)::(0x21000000) \
                (INCLUDE_AMP_CPU_01)::(0x11000000) \
                0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        system memory start address
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00000000) \
                (INCLUDE_AMP_CPU_01) && (P1020RDB_PC_REVB)::(0x20000000) \
                (INCLUDE_AMP_CPU_01)::(0x10000000) \
                (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME        system memory size
    DEFAULT     (INCLUDE_AMP_CPU_00) && (P1020RDB_PC_REVB)::(0x20000000) \
                (INCLUDE_AMP_CPU_01) && (P1020RDB_PC_REVB)::(0x20000000) \
                (INCLUDE_AMP_CPU_00)::(0x10000000) \
                (INCLUDE_AMP_CPU_01)::(0x10000000) \
                (P1020RDB_PC_REVB)::(0x40000000) \
                (0x20000000)
}

Parameter RAM_LOW_ADRS_CPU0 {
    NAME        Ram low address specifically for cpu0
    DEFAULT     0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME        MDIO anchor adrs
    _CFG_PARAMS INCLUDE_AMP
    DEFAULT     (P1020RDB_PC_REVB)::(0x4400 + 0x20000000) \
                (0x4400 + 0x10000000)
}

/* driver parameters for eTSEC */

Parameter ETSEC0_CORE_NUM {
    NAME        eTSEC0 CPU assignment
    _CFG_PARAMS	INCLUDE_ETSEC_VXB_END
    DEFAULT     0
}

Parameter ETSEC1_CORE_NUM {
    NAME        eTSEC1 CPU assignment
    _CFG_PARAMS	INCLUDE_ETSEC_VXB_END
    DEFAULT     1
}

Parameter ETSEC2_CORE_NUM {
    NAME        eTSEC2 CPU assignment
    _CFG_PARAMS	INCLUDE_ETSEC_VXB_END
    DEFAULT     1
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

/* Freescale eSPI Controller Driver */

Component INCLUDE_FSL_SPI {
    NAME        SPI Bus Controller Driver
    SYNOPSIS    Freescale eSPI Controller Driver
    _CHILDREN   FOLDER_DRIVERS
}

/* Spansion SPI flash Legacy driver */

Component DRV_SPI_S25FLXX {
    NAME        Spansion SPI Flash Legacy Driver
    SYNOPSIS    Spansion S25FLXX serial flash Legacy driver
    REQUIRES    INCLUDE_FSL_SPI
    _CHILDREN   FOLDER_DRIVERS
}

/* Spansion SPI flash vxBus driver */

Component DRV_SPIFLASH_SP25 {
    NAME        Spansion SPI Flash vxBus Driver
    SYNOPSIS    Spansion S25FLXX serial flash vxBus driver
    REQUIRES    DRV_ESPI_FSL
    _CHILDREN   FOLDER_DRIVERS
}

