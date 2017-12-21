/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,20may13,wyt  add support for P1023RDB
01c,31oct12,syt  corrected BSP's FP option to soft (WIND00379626)
01b,22aug12,syt  added support for P1021RDB
01a,22may12,syt  derived from version 01f of fsl_p1020_rdb/20bsp.cdf
*/

Bsp fsl_p10xx_rdb {
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550     \
                INCLUDE_I2C_BUS     \
                DRV_VXB_NORFLASH_CMDSET_AMD
}

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_QCOM_AR8021PHY  \
                INCLUDE_TSEC_MDIO   \
                INCLUDE_VSC82XXPHY  \
                INCLUDE_GENERICPHY  \
                INCLUDE_DUMMY_MDIO  \
                INCLUDE_DMA_SYS     \
                INCLUDE_MII_BUS
}

Component INCLUDE_UCC_VXB_END {
    REQUIRES += INCLUDE_QUICC_ENGINE_UTILS  \
                DRV_INTCTLR_QE      \
                INCLUDE_UCC_MDIO    \
                INCLUDE_DMA_SYS     \
                INCLUDE_MII_BUS     \
                INCLUDE_GENERICPHY
}

Component INCLUDE_I2C_BUS {
    REQUIRES += DRV_I2CBUS_FSL  \
                DRV_I2C_GENERIC_DEV
}

Component DRV_SIO_UCC {
    REQUIRES += INCLUDE_QUICC_ENGINE_UTILS  \
                DRV_INTCTLR_QE
}

Component DRV_VXBEND_DTSEC {
    REQUIRES +=   DRV_MII_DTSEC_MDIO    \
                  DRV_RESOURCE_QORIQBMAN    \
                  DRV_RESOURCE_QORIQFMAN    \
                  DRV_RESOURCE_QORIQQMAN    \
                  INCLUDE_QCOM_AR8021PHY
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END  \
                DRV_VXBEND_DTSEC
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

/* required components for AMP */

Bundle BUNDLE_AMP_PRI {
    COMPONENTS  +=  INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
    COMPONENTS  +=  INCLUDE_AMP
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

Component INCLUDE_P1023RDB  {
    NAME        P1023 Board Component
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
                (INCLUDE_AMP_CPU_01 & INCLUDE_P1023RDB)::(0x12000000) \
                (INCLUDE_AMP_CPU_01)::(0x22000000) \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01400000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_AMP_CPU_01 & INCLUDE_P1023RDB)::(0x11000000) \
                (INCLUDE_AMP_CPU_01)::(0x21000000) \
                0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        system memory start address
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00000000) \
                (INCLUDE_AMP_CPU_01 & INCLUDE_P1023RDB)::(0x10000000) \
                (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME        system memory size
    DEFAULT     (INCLUDE_AMP_CPU_00 & INCLUDE_P1023RDB)::(0x10000000) \
                (INCLUDE_AMP_CPU_00)::(0x20000000) \
                (INCLUDE_AMP_CPU_01 & INCLUDE_P1023RDB)::(0x10000000) \
                (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (0x20000000)
}

Parameter PHYS_MEM_SIZE_P1023 {
    NAME        P1023 board memory size
    _CFG_PARAMS INCLUDE_AMP
    DEFAULT    (INCLUDE_AMP_CPU_00)::(0x20000000) \
               (INCLUDE_AMP_CPU_01)::(0x10000000) \
               (0x20000000)
}

Parameter PHYS_MEM_SIZE_DEFAULT {
    NAME        Other P10xx boards physical size
    _CFG_PARAMS INCLUDE_AMP
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x40000000) \
                (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (0x40000000)
}

Parameter TM_ANCHOR_ADRS {
    NAME        MDIO anchor adrs
    _CFG_PARAMS INCLUDE_AMP
    DEFAULT     (INCLUDE_P1023RDB)::((0x4400 + 0x10000000)) \
                (0x4400 + 0x20000000)
}

/* driver parameters for eTSEC */

Parameter ETSEC0_CORE_NUM {
    NAME        eTSEC0 CPU assignment
    _CFG_PARAMS INCLUDE_ETSEC_VXB_END
    DEFAULT     0
}

Parameter ETSEC1_CORE_NUM {
    NAME        eTSEC1 CPU assignment
    _CFG_PARAMS INCLUDE_ETSEC_VXB_END
    DEFAULT     1
}

Parameter ETSEC2_CORE_NUM {
    NAME        eTSEC2 CPU assignment
    _CFG_PARAMS INCLUDE_ETSEC_VXB_END
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

/* Spansion SPI flash driver */

Component DRV_SPI_S25FLXX {
    NAME        Spansion SPI Flash Driver
    SYNOPSIS    Spansion S25FLXX serial flash driver
    REQUIRES    INCLUDE_FSL_SPI
    _CHILDREN   FOLDER_DRIVERS
}

