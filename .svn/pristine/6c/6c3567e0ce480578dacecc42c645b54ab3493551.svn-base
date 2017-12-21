/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,23mar12,d_l  update for AMP support on VxWorks 6.9.2.1
                 add SPI, I2C, NorFlash components.
01a,17aug10,e_d  created
*/

Bsp fsl_p1022_ds {
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550     \
                DRV_VXB_NORFLASH_CMDSET_AMD
}

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_TSEC_MDIO   \
                INCLUDE_GENERICPHY  \
                DRV_I2CBUS_FSL      \
                DRV_I2C_GENERIC_DEV
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
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
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                    (INCLUDE_BOOT_APP)::(0x01800000)        \
                    (INCLUDE_AMP_CPU_01)::(0x42000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x01000000)      \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                    (INCLUDE_BOOT_APP)::(0x01000000)        \
                    (INCLUDE_AMP_CPU_01)::(0x41000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00100000)      \
                    0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME            Runtime kernel lower memory address
    DEFAULT         (INCLUDE_AMP_CPU_01)::(0x40000000)      \
                    (INCLUDE_AMP_CPU_00)::(0x00000000)      \
                    0x00000000
}

Parameter LOCAL_MEM_SIZE {
    NAME            system memory size
    DEFAULT         (INCLUDE_AMP_CPU_01)::(0x37000000-SPI_BOOT_RESMEM) \
                    (INCLUDE_AMP_CPU_00)::(0x40000000)      \
                    (0x80000000-SPI_BOOT_RESMEM)
}

Parameter RAM_LOW_ADRS_CPU0 {
    NAME            Ram low address specifically for cpu0
    DEFAULT         0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME            MDIO anchor adrs
    _CFG_PARAMS     INCLUDE_AMP
    DEFAULT         (0x77000000)
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

/* shared memory components */

Component INCLUDE_VIRTUAL_SM {
    NAME            virtual SM region
    _CHILDREN       FOLDER_BSP_CONFIG
    CFG_PARAMS      VIRTUAL_SM_BASE \
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

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

/* Freescale eSPI Controller Driver */

Component INCLUDE_FSL_SPI
{
    NAME            SPI Bus Controller Driver
    SYNOPSIS        Freescale eSPI Controller Driver
    _CHILDREN       FOLDER_DRIVERS
}

Component DRV_SPI_S25FLXX
{
    NAME            Spansion SPI Flash Driver
    SYNOPSIS        Spansion S25FLXX serial flash driver
    REQUIRES        INCLUDE_FSL_SPI
    _CHILDREN       FOLDER_DRIVERS
}
