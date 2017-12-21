/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,07apr13,d_l  remove default_rom, default_romResident support. (WIND00411417)
01g,29feb12,syt  updated for new SATA driver with INCLUDE_DRV_STORAGE_AHCI. 
01f,06feb12,syt  added SATA support.
01e,20dec11,x_z  added support for QMAN and on-chip MAC and use on-chip MAC as
                 default ethernet port.
01d,24nov11,x_s  add AMP support.
01c,07sep11,x_s  add SMP support.
01b,30jul11,x_z  add PCI Express support and use GEI825xx as default ethernet
                 port.
01a,16jun10,syt  initial creation.
*/

Bsp apm_serengeti {
    NAME        board support package
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    BUILD_SPECS default_romCompress
    REQUIRES    INCLUDE_KERNEL                  \
                INCLUDE_440X5_PARITY_RECOVERY   \
                INCLUDE_TIMER_SYS               \
                DRV_TIMER_PPC440                \
                DRV_INTCTLR_PPC                 \
                DRV_INTCTLR_MPIC                \
                DRV_SIO_NS16550
    FP          hard
}

Component DRV_PCIBUS_APM_PCIEX {
    REQUIRES += INCLUDE_PCI_BUS                 \
                INCLUDE_PCI_BUS_AUTOCONF        \
                INCLUDE_VXB_LEGACY_INTERRUPTS
}

Component INCLUDE_GEI825XX_VXB_END {
    REQUIRES += DRV_PCIBUS_APM_PCIEX    \
                INCLUDE_MII_BUS         \
                INCLUDE_GENERICPHY
}

Component DRV_VXBEND_APM_PPRO_MAC {
    REQUIRES += DRV_APM_PPRO_QMAN       \
                INCLUDE_MII_BUS         \
                INCLUDE_GENERICPHY
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component  INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    DRV_VXBEND_APM_PPRO_MAC
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK
}

/*
 * The following components allow independent assignment of the two physical
 * UART's to any PPC465 core, either both on one core,  one each on two separate
 * cores, or either/both not assigned to any core.  If included, UART0 will
 * always be placed on SIO chan 0;  UART 1 will be placed on SIO chan 1 if UART0
 * is also configured on the same core, else UART 1 will be on SIO chan 0 for
 * that core.
 *
 * Beware: Since separate kernel images are built for each core, nothing can
 * prevent assigning the same UART to multiple cores, which will lead to
 * undefined behavior (each core will try to control the single physical
 * device).
 */

Component INCLUDE_AMP_UART_0 {
    NAME        Add UART0 for an AMP image
    SYNOPSIS    Support for assigning the UART driver
}

Component INCLUDE_AMP_UART_1 {
    NAME        Add UART1 for an AMP image
    SYNOPSIS    Support for assigning the UART driver
}

Component INCLUDE_AMP_PPRO_END_DRIVER {
    NAME        APM PacketPro END Driver Enable for AMP images
    SYNOPSIS    Support for assigning the end driver
}

Folder FOLDER_AMP_DEVICES {
   NAME         vxWorks AMP Devices
   _CHILDREN    FOLDER_BSP_CONFIG
   CHILDREN     INCLUDE_AMP_UART_0 \
                INCLUDE_AMP_UART_1 \
                INCLUDE_AMP_PPRO_END_DRIVER
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
    _CHILDREN   SELECT_AMP_CPU_NUMBER
    CFG_PARAMS  AMP_CPU_NUMBER
    REQUIRES    INCLUDE_AMP_CPU \
                INCLUDE_VXIPI \
                INCLUDE_VXCPULIB \
                INCLUDE_WRLOAD_IMAGE_BUILD
}

Component INCLUDE_AMP_CPU_00  {
    NAME        AMP kernel configured for core 00
    _CHILDREN   SELECT_AMP_CPU_NUMBER
    CFG_PARAMS  AMP_CPU_NUMBER
    REQUIRES    INCLUDE_AMP_CPU \
                INCLUDE_VXIPI \
                INCLUDE_VXCPULIB \
                INCLUDE_WRLOAD \
                INCLUDE_AMP_PPRO_END_DRIVER
}

/* 
 * On APM86290, the SERDES PHY of SATA port 1 is muxed with PCIE0's,
 * to enable PCIE0, parameter SATA_PORT1_ENABLE should be set as
 * FALSE and PCIEX0_PORT0_ENABLE set as TRUE, vice versa.
 */

Parameter SATA_PORT1_ENABLE  {
    NAME         enable APM86290 SATA port1 interface
    _CFG_PARAMS  INCLUDE_DRV_STORAGE_AHCI
    TYPE         BOOL
    DEFAULT      TRUE
}

Parameter PCIEX_PORT0_ENABLE  {
    NAME         enable APM86290 PCIEX port0 interface
    _CFG_PARAMS  DRV_PCIBUS_APM_PCIEX
    TYPE         BOOL
    DEFAULT      FALSE
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
    NAME        vxWorks bootrom load address
    DEFAULT     (INCLUDE_AMP_CPU_01)::(0x22000000) \
                (INCLUDE_AMP_CPU_00)::(0x01000000) \
                (INCLUDE_BOOT_RAM_IMAGE)::(0x01e00000)  \
                (INCLUDE_BOOT_APP)::(0x01800000)        \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_AMP_CPU_01)::(0x21000000) \
                (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_BOOT_RAM_IMAGE)::(0x01a00000)  \
                (INCLUDE_BOOT_APP)::(0x01400000)        \
                0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        Runtime kernel lower memory address
    DEFAULT     (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (INCLUDE_AMP_CPU_00)::(0x00000000) \
                (INCLUDE_BOOT_APP)::(0x00000000)   \
                0x00000000
}

/* For core 2, we must reserve boot memory for ctrl + x command */

Parameter LOCAL_MEM_SIZE {
        NAME    system memory size
        DEFAULT (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (INCLUDE_AMP_CPU_00)::(0x20000000) \
                (INCLUDE_BOOT_APP)::(0x40000000) \
                (0x40000000)
}

Parameter  RAM_LOW_ADRS_CPU0 {
    NAME        Ram low address specifically for core 0
    SYNOPSIS    bootrom uses the address to calcuate the space for core 1 to \
                save/check CPU status, so the address must be identical with \
                bootrom.
    DEFAULT     0x00100000
}

Parameter AMP_CPU_NUMBER {
    NAME        AMP CPU number
    DEFAULT     (INCLUDE_AMP_CPU_01)::(1) \
                (INCLUDE_AMP_CPU_00)::(0) \
                0
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

