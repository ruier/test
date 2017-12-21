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
01d,29jul13,d_l  remove default_rom/default_romResident support. (WIND00427646)
01c,12oct12,d_l  update memory space for AMP cores. (WIND00344670)
01b,27sep12,d_l  add Nor flash component for bootapp project.(WIND00379585)
01a,05sep12,d_l  created from fsl_p5020_ds version 01g.
*/

Bsp fsl_p5040_ds {
    CPU     PPC32
    MP_OPTIONS  UP SMP
    ENDIAN      big
    FP          hard soft
    BUILD_SPECS default_romCompress
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_PPC     \
                DRV_INTCTLR_EPIC    \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550     \
                DRV_VXB_NORFLASH_CMDSET_AMD
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    DRV_VXBEND_DTSEC
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
	INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

/* Local component dependencies, to match config.h */

Component INCLUDE_TFFS {
	REQUIRES +=	INCLUDE_TL_FTL INCLUDE_TFFS_MOUNT
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
    NAME            Number of CPUs available to be enabled for VxWorks SMP
    TYPE            UINT
    DEFAULT         4
}

/* AMP components */

Component INCLUDE_AMP {
    NAME            Base AMP Support
    REQUIRES        INCLUDE_AMP_CPU \
                    INCLUDE_VXIPI   \
                    INCLUDE_VXCPULIB
    _CHILDREN       FOLDER_BSP_CONFIG
}

Component INCLUDE_AMP_CPU {
    NAME            AMP CPU Support
    SYNOPSIS        Per-cpu configuration for AMP
    REQUIRES        SELECT_AMP_CPU_NUMBER
    _CHILDREN       FOLDER_BSP_CONFIG
}

/* Override the default (empty) component definitions with custom options */

Component INCLUDE_AMP_CPU_03  {
    NAME            AMP kernel configured for core 03
    REQUIRES        INCLUDE_AMP_CPU
    _CHILDREN       SELECT_AMP_CPU_NUMBER
    CFG_PARAMS      AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_02  {
    NAME            AMP kernel configured for core 02
    REQUIRES        INCLUDE_AMP_CPU
    _CHILDREN       SELECT_AMP_CPU_NUMBER
    CFG_PARAMS      AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_01  {
    NAME            AMP kernel configured for core 01
    REQUIRES        INCLUDE_AMP_CPU
    _CHILDREN       SELECT_AMP_CPU_NUMBER
    CFG_PARAMS      AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_00  {
    NAME            AMP kernel configured for core 00
    REQUIRES        INCLUDE_AMP_CPU
    _CHILDREN       SELECT_AMP_CPU_NUMBER
    CFG_PARAMS      AMP_CPU_NUMBER
}

Parameter RAM_HIGH_ADRS {
    NAME            vxWorks bootrom load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x11000000)  \
                    (INCLUDE_BOOT_APP)::(0x02000000)        \
                    (INCLUDE_AMP_CPU_03)::(0x61000000)      \
                    (INCLUDE_AMP_CPU_02)::(0x51000000)      \
                    (INCLUDE_AMP_CPU_01)::(0x41000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x01000000)      \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x10100000)  \
                    (INCLUDE_BOOT_APP)::(0x01000000)        \
                    (INCLUDE_AMP_CPU_03)::(0x60100000)      \
                    (INCLUDE_AMP_CPU_02)::(0x50100000)      \
                    (INCLUDE_AMP_CPU_01)::(0x40100000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00100000)      \
                    0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME            Runtime kernel lower memory address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x10000000)  \
                    (INCLUDE_BOOT_APP)::(0x00000000)        \
                    (INCLUDE_AMP_CPU_03)::(0x60000000)      \
                    (INCLUDE_AMP_CPU_02)::(0x50000000)      \
                    (INCLUDE_AMP_CPU_01)::(0x40000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00000000)      \
                    0x00000000
}

Parameter LOCAL_MEM_SIZE {
    NAME            system memory size
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                    (INCLUDE_BOOT_APP)::(QMAN_MEM_BASE)     \
                    (INCLUDE_AMP_CPU_03)::(0x10000000)      \
                    (INCLUDE_AMP_CPU_02)::(0x10000000)      \
                    (INCLUDE_AMP_CPU_01)::(0x10000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x40000000)      \
                    QMAN_MEM_BASE
}

Parameter AMP_CPU_NUMBER {
    NAME            AMP CPU number
    DEFAULT         (INCLUDE_AMP_CPU_03)::(3) \
                    (INCLUDE_AMP_CPU_02)::(2) \
                    (INCLUDE_AMP_CPU_01)::(1) \
                    (INCLUDE_AMP_CPU_00)::(0) \
                    0
}

Parameter LOCAL_MEM_ERPN {
    NAME            Runtime kernel physical address, upper 4 bits
    DEFAULT         0x0
    _CFG_PARAMS     INCLUDE_MEMORY_CONFIG
}

Component INCLUDE_LOGICAL_PARTITION {
    NAME            Logical partition support
    CFG_PARAMS      LOGICAL_PARTITION_ID
    INCLUDE_WHEN    INCLUDE_AMP
    _CHILDREN       FOLDER_BSP_CONFIG
}

Parameter LOGICAL_PARTITION_ID {
    NAME            Logical partition ID for this OS instance
    DEFAULT         (INCLUDE_AMP_CPU_03)::(13) \
                    (INCLUDE_AMP_CPU_02)::(12) \
                    (INCLUDE_AMP_CPU_01)::(11) \
                    (INCLUDE_AMP_CPU_00)::(1) \
                    0
}

/* driver components */

Parameter TM_ANCHOR_ADRS {
    NAME            MDIO anchor adrs
    _CFG_PARAMS     INCLUDE_AMP
    DEFAULT         (0x4400 + 0x77000000)
}

Parameter SGMII_RISER_CARD_IN_USE {
    NAME            SGMII Riser Card In Use flag
    SYNOPSIS        a flag represent if SGMII Riser Card is used 
    TYPE            BOOL
    DEFAULT         FALSE
}

Component DRV_VXBEND_DTSEC {
    REQUIRES +=     DRV_MII_DTSEC_MDIO  \
                    DRV_RESOURCE_QORIQBMAN  \
                    DRV_RESOURCE_QORIQFMAN  \
                    DRV_RESOURCE_QORIQQMAN  \
                    DRV_RESOURCE_QORIQLAW
    CFG_PARAMS      SGMII_RISER_CARD_IN_USE
}

omponent INCLUDE_PCI_BUS {
    REQUIRES        INCLUDE_PCI_BUS_AUTOCONF \
                    DRV_PCIBUS_QORIQ_PCIEX
}

Component DRV_PCIBUS_QORIQ_PCIEX {
    REQUIRES        DRV_RESOURCE_QORIQLAW
    CFG_PARAMS      QORIQ_PCIEX1_ENABLE   \
                    QORIQ_PCIEX2_ENABLE   \
                    QORIQ_PCIEX3_ENABLE   \
                    QORIQ_PCIEX1_UNIT_NUM \
                    QORIQ_PCIEX2_UNIT_NUM \
                    QORIQ_PCIEX3_UNIT_NUM
}

Parameter QORIQ_PCIEX1_ENABLE  {
    NAME            Enable PCIEX1 interface
    TYPE            BOOL
    DEFAULT         TRUE
}

Parameter QORIQ_PCIEX2_ENABLE {
    NAME            Enable PCIEX2 interface
    TYPE            BOOL
    DEFAULT         TRUE
}

Parameter QORIQ_PCIEX3_ENABLE  {
    NAME            Enable PCIEX3 interface
    TYPE            BOOL
    DEFAULT         TRUE
}

Parameter QORIQ_PCIEX1_UNIT_NUM {
    NAME            Unit number of PCIEX1
    DEFAULT         0
}

Parameter QORIQ_PCIEX2_UNIT_NUM {
    NAME            Unit number of PCIEX2
    DEFAULT         1
}

Parameter QORIQ_PCIEX3_UNIT_NUM {
    NAME            Unit number of PCIEX3
    DEFAULT         2
}

/* shared memory components */

Component INCLUDE_VIRTUAL_SM {
    NAME            virtual SM region
    _CHILDREN       FOLDER_BSP_CONFIG
    CFG_PARAMS      VIRTUAL_SM_BASE     \
                    VIRTUAL_SM_BASE_PHYS \
                    VIRTUAL_SM_SIZE
}

Parameter VIRTUAL_SM_BASE {
    NAME            shared memory virtual address
    DEFAULT         0x78000000
}

Parameter VIRTUAL_SM_BASE_PHYS {
    NAME            shared memory physical address
    DEFAULT         0x78000000
}

Parameter VIRTUAL_SM_SIZE {
    NAME            shared memory size
    DEFAULT         0x01000000
}

/* Cache error handling components */

Component INCLUDE_CACHE_ERROR_HANDLER {
    NAME            L1/L2/L3 cache error handler
    _CHILDREN       FOLDER_BSP_CONFIG
}

Component INCLUDE_L1_CACHE_ERROR_RECOVERY {
    NAME            L1 cache error recovery
    REQUIRES        INCLUDE_CACHE_ERROR_HANDLER
    INCLUDE_WHEN    INCLUDE_CACHE_ERROR_HANDLER
    CFG_PARAMS      L1_CACHE_ERROR_INJECTION
    _CHILDREN       FOLDER_BSP_CONFIG
}

Parameter L1_CACHE_ERROR_INJECTION {
    NAME            include L1 cache error injection code
    TYPE            exists
    DEFAULT         FALSE
}

Component INCLUDE_L2_CACHE_ERROR_RECOVERY {
    NAME            L2 cache error recovery
    REQUIRES        INCLUDE_CACHE_ERROR_HANDLER
    CFG_PARAMS      L2_CACHE_ERROR_INJECTION \
                    L2_CACHE_ERROR_INT_ENABLE \
                    L2_CACHE_ERROR_L2CTHRESH
    _CHILDREN       FOLDER_BSP_CONFIG
}

Parameter L2_CACHE_ERROR_INJECTION {
    NAME            include L2 cache error injection code
    TYPE            exists
    DEFAULT         FALSE
}

Parameter L2_CACHE_ERROR_INT_ENABLE {
    NAME            L2 cache error interrupt enable setting
    DEFAULT         0x9e
}

Parameter L2_CACHE_ERROR_L2CTHRESH {
    NAME            L2 cache error threshold setting (L2ERRCTL)
    DEFAULT         0x0
}

Component INCLUDE_L3_CACHE_ERROR_RECOVERY {
    NAME            L3 cache error recovery
    REQUIRES        INCLUDE_CACHE_ERROR_HANDLER
    CFG_PARAMS      L3_CACHE_ERROR_INJECTION \
                    L3_CACHE_ERROR_INT_ENABLE \
                    L3_CACHE_ERROR_CPCTHRESH
    _CHILDREN       FOLDER_BSP_CONFIG
}

Parameter L3_CACHE_ERROR_INJECTION {
    NAME            include L3 cache error injection code
    TYPE            exists
    DEFAULT         FALSE
}

Parameter L3_CACHE_ERROR_INT_ENABLE {
    NAME            L3 cache error interrupt enable setting
    DEFAULT         0xec
}

Parameter L3_CACHE_ERROR_CPCTHRESH {
    NAME            L3 cache error threshold setting (CPCERRCTL)
    DEFAULT         0x0
}

/* Freescale eSPI Controller Driver */

Component INCLUDE_FSL_SPI {
    NAME            SPI Bus Controller Driver
    SYNOPSIS        Freescale eSPI Controller Driver
    _CHILDREN       FOLDER_DRIVERS
}

Component DRV_SPI_S25FLXX {
    NAME            Spansion SPI Flash Driver
    SYNOPSIS        Spansion S25FLXX serial flash driver
    REQUIRES        INCLUDE_FSL_SPI
    _CHILDREN       FOLDER_DRIVERS
}

