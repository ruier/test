/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,13nov12,j_z  add SRIO share memory componet INCLUDE_SM_COMMON 
01h,05mar12,y_y  remove I2C related component for vxBus I2C support.
01g,28feb12,y_y  correct the wrong RAM_LOW_ADRS(INCLUDE_BOOT_RAM_IMAGE) from
                 0x101000000 to 0x10100000. (WIND00335183) 
01f,31jan12,agf  CQ329621: Remove SYSTEM_GLOBALS, superceded by
                 RESERVED_ATOMICS
01e,31jan12,y_y  changed the default RAM_LOW_ADRS of INCLUDE_BOOT_RAM_IMAGE from 
                 0x02000000 to 0x101000000 and added TM_ANCHOR_ADRS component.
01d,19oct11,y_y  added tffs and dtsec3 component.
01b,27sep11,ers  Replaced DYNAMIC_WRLOAD with generic wrload. (WIND00253685)
01c,19aug11,y_y  added I2C releated component.
01b,19may11,y_y  added spi flash component.
01a,19mar11,syt  created from fsl_p4080_ds 20bsp.cdf version 01q
*/

Bsp fsl_p5020_ds {
    CPU     PPC32
    MP_OPTIONS  UP SMP
    ENDIAN      big
    FP          hard soft
    REQUIRES    INCLUDE_KERNEL  \
                DRV_INTCTLR_PPC \
                DRV_INTCTLR_EPIC \
                DRV_TIMER_M85XX \
                DRV_SIO_NS16550
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
    DEFAULT         2
}

/* AMP components */

Bundle BUNDLE_AMP_PRI {
    COMPONENTS  +=  INCLUDE_AMP
}

Bundle BUNDLE_AMP_SEC {
    COMPONENTS  +=  INCLUDE_AMP
}

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

/*
 * Override the default (empty) component definitions with custom options
 */

Component INCLUDE_AMP_CPU_01  {
    CFG_PARAMS      AMP_CPU_NUMBER
}

Component INCLUDE_AMP_CPU_00  {
    CFG_PARAMS      AMP_CPU_NUMBER
}

Parameter RAM_HIGH_ADRS {
    NAME            vxWorks bootrom load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x11000000)  \
                    (INCLUDE_BOOT_APP)::(0x02000000)        \
                    (INCLUDE_AMP_CPU_01)::(0x42000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x01000000)      \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x10100000)  \
                    (INCLUDE_BOOT_APP)::(0x01000000)        \
                    (INCLUDE_AMP_CPU_01)::(0x41000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00100000)      \
                    0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME            Runtime kernel lower memory address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x10000000)  \
                    (INCLUDE_BOOT_APP)::(0x00000000)        \
                    (INCLUDE_AMP_CPU_01)::(0x40000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00000000)      \
                    0x00000000
}

Parameter LOCAL_MEM_SIZE {
    NAME            system memory size
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                    (INCLUDE_BOOT_APP)::(QMAN_MEM_BASE)     \
                    (INCLUDE_AMP_CPU_01)::(0x37000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x40000000)      \
                    QMAN_MEM_BASE
}

Parameter AMP_CPU_NUMBER {
    NAME            AMP CPU number
    DEFAULT         (INCLUDE_AMP_CPU_01)::(1) \
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
    DEFAULT         (INCLUDE_AMP_CPU_01)::(11) \
                    (INCLUDE_AMP_CPU_00)::(1) \
                    0
}

/* driver components */

Parameter TM_ANCHOR_ADRS {
    NAME            MDIO anchor adrs
    _CFG_PARAMS     DRV_MII_DTSEC_MDIO
    DEFAULT         (0x4400 + 0x77000000)
}

Component DRV_VXBEND_DTSEC {
    REQUIRES +=     DRV_MII_DTSEC_MDIO  \
                    DRV_RESOURCE_QORIQBMAN  \
                    DRV_RESOURCE_QORIQFMAN  \
                    DRV_RESOURCE_QORIQQMAN  \
                    DRV_RESOURCE_QORIQLAW
    CFG_PARAMS      DTSEC3_IN_RGMII
}

/*
 * Set DTSEC3 in RGMII mode as default. 
 * SerDes lane H(RCW[159])   Media Mode   DTSEC3_IN_RGMII
 *   powered off   (1)         RGMII        TRUE            
 *   powered on    (0)         SGMII        FALSE           
 */

Parameter DTSEC3_IN_RGMII  {
    NAME            DTSEC3 Media Mode
    SYNOPSIS        DTSEC3 on the P5020DS target is configurable based on  \
                    SerDes lane setting. Set to TRUE in RGMII, FALSE in SGMII.
    TYPE            BOOL
    DEFAULT         TRUE
}

Component INCLUDE_PCI_BUS {
    REQUIRES        INCLUDE_PCI_BUS_AUTOCONF \
                    DRV_PCIBUS_QORIQ_PCIEX
}

Component DRV_PCIBUS_QORIQ_PCIEX {
    REQUIRES        DRV_RESOURCE_QORIQLAW
    CFG_PARAMS      QORIQ_PCIEX1_ENABLE   \
                    QORIQ_PCIEX2_ENABLE   \
                    QORIQ_PCIEX3_ENABLE   \
                    QORIQ_PCIEX4_ENABLE   \
                    QORIQ_PCIEX1_UNIT_NUM \
                    QORIQ_PCIEX2_UNIT_NUM \
                    QORIQ_PCIEX3_UNIT_NUM \
                    QORIQ_PCIEX4_UNIT_NUM
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

Parameter QORIQ_PCIEX4_ENABLE  {
    NAME            Enable PCIEX4 interface
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

Parameter QORIQ_PCIEX4_UNIT_NUM {
    NAME            Unit number of PCIEX4
    DEFAULT         3
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

Component INCLUDE_FSL_SPI
{
    NAME            SPI Bus Controller Driver
    SYNOPSIS        Freescale eSPI Controller Driver
    _CHILDREN       FOLDER_DRIVERS
}

/* RapidIO share memory net common components */

Component INCLUDE_SM_COMMON {
    REQUIRES     += DRV_SRIO_FSL \
                    DRV_SRIO_CPU_EP \
                    INCLUDE_PLB_BUS \
                    INCLUDE_RAPIDIO_BUS \
                    DRV_RESOURCE_QORIQRMAN \
                    INCLUDE_MSG_NET \
                    INCLUDE_SM_NET \
                    INCLUDE_SM_OBJ \
                    INCLUDE_VXBUS_SM_SUPPORT
}

