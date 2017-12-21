/* 20bsp.cdf - LSI ACP34XX BSP component description file */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,03aug12,x_s  changed the default VX_SMP_NUM_CPUS to 2.
01j,31aug11,x_z  fixed INCLUDE_AMP_UART_1 component issue. (WIND00297401) 
01i,09may11,x_s  added AXIS timer as a component which would be included when 
                 AuxClk used.
01h,14apr11,x_z  added AXIS timer support.
01g,08apr11,x_s  added dependency for component INCLUDE_PCI_BUS.
01f,05mar11,x_s  added SMP support.
01e,25feb11,x_z  removed INCLUDE_AMP_UART_0 for INCLUDE_AMP_CPU_00.
01d,18jan11,x_z  removed unnecessary comment.
01c,16dec10,x_z  added support for hard float point and fix LOCAL_MEM_SIZE.
01b,26oct10,x_z  added support for AMP.
01a,26jan10,x_s  written.
*/

Bsp lsi_acp34xx_elpaso {
    NAME            Board support package
    CPU             PPC32
    MP_OPTIONS      SMP    
    FP              hard
    REQUIRES        INCLUDE_KERNEL      \
                    DRV_INTCTLR_PPC     \
                    DRV_INTCTLR_MPIC    \
                    DRV_TIMER_PPC440    \
                    DRV_SIO_PRIMECELL
}

Component INCLUDE_LSI_MAC_VXB_END {
    REQUIRES += INCLUDE_LSI_MDIO        \
                INCLUDE_GENERICPHY      \
                INCLUDE_MII_BUS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component  INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_LSI_MAC_VXB_END
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK
}

/* 
 * Required LSI PCIe controller driver and INCLUDE_PCI_BUS_AUTOCONF for 
 * component INCLUDE_PCI_BUS 
 */

Component INCLUDE_PCI_BUS {
    REQUIRES    DRV_PCIBUS_LSI_PCIEX    \
                INCLUDE_PCI_BUS_AUTOCONF
}

/*
 * The following components allow independent assignment of the two physical
 * UART's to any PPC476 core, either both on one core,  one each on two separate
 * cores, or either/both not assigned to any core.  If included, UART0 will
 * always be placed on SIO chan 0;  UART 1 will be placed on SIO chan 1 if UART0
 * is also configured on the same core, else UART 1 will be on SIO chan 0 for
 * that core.
 *
 * Beware: Since separate kernel images are built for each core, nothing can
 * prevent assigning the same UART to multiple cores, which will lead to
 * undefined behavior (each core will try to control the single physical
 * device).
 *
 * NOTE: INCLUDE_AMP_UART_1 and INCLUDE_AMP_AXIS_TIMER_UART1 are
 * mutually-exclusive. In other words, if one of them is included, another one
 * that has been include will be removed automatically.
 */

Component INCLUDE_AMP_UART_0 {
    NAME        Add UART0 for an AMP image
    SYNOPSIS    Support for assigning the UART driver
}

Component INCLUDE_AMP_UART_1 {
    NAME        Add UART1 for an AMP image
    SYNOPSIS    Support for assigning the UART driver   \
                mutually-exclusive with INCLUDE_AMP_AXIS_TIMER_UART1
    REQUIRES    SELECT_AMP_UART1_TIMER_MODE
}

Component INCLUDE_AMP_LSI_END_DRIVER {
    NAME        LSI END Driver Enable for AMP images
    SYNOPSIS    Support for assigning the end driver
}

/*
 * The following components allow independent assignment of the 4 AXIS timers to
 * any PPC476 core, either all on one core, one each on 4 separate cores, or
 * either/both not assigned to any core.
 *
 * NOTE: INCLUDE_AMP_UART_1 and INCLUDE_AMP_AXIS_TIMER_UART1 are
 * mutually-exclusive. In other words, if one of them is included, another one
 * that has been include will be removed automatically.
 *
 * Beware: Since separate kernel images are built for each core, nothing can
 * prevent assigning the same timer to multiple cores, which will lead to
 * undefined behavior (each core will try to control the single physical
 * device).
 */

Component INCLUDE_AMP_AXIS_TIMER5 {
    NAME        Add AXIS timer 5 for an AMP image
    SYNOPSIS    Support for assigning the AXIS timer driver
}

Component INCLUDE_AMP_AXIS_TIMER6 {
    NAME        Add AXIS timer 6 for an AMP image
    SYNOPSIS    Support for assigning the AXIS timer driver
}

Component INCLUDE_AMP_AXIS_TIMER7 {
    NAME        Add AXIS timer 7 for an AMP image
    SYNOPSIS    Support for assigning the AXIS timer driver
}

Component INCLUDE_AMP_AXIS_TIMER_UART1 {
    NAME        Add AXIS UART1 timer for an AMP image
    SYNOPSIS    Support for assigning the AXIS timer driver
    REQUIRES    SELECT_AMP_UART1_TIMER_MODE
}

Selection SELECT_AMP_UART1_TIMER_MODE {
    NAME        Select UART1 timer mode
    SYNOPSIS    Method for selecting UART1 timer mode   \
                mutually-exclusive with INCLUDE_AMP_UART_1
    COUNT       1-1
    CHILDREN    INCLUDE_AMP_UART_1 \
                INCLUDE_AMP_AXIS_TIMER_UART1
    DEFAULTS    INCLUDE_AMP_AXIS_TIMER_UART1
}

Folder FOLDER_AMP_DEVICES {
   NAME         vxWorks AMP Devices
   _CHILDREN    FOLDER_BSP_CONFIG
   CHILDREN     INCLUDE_AMP_UART_0 \
                INCLUDE_AMP_LSI_END_DRIVER \
                INCLUDE_AMP_AXIS_TIMER5 \
                INCLUDE_AMP_AXIS_TIMER6 \
                INCLUDE_AMP_AXIS_TIMER7 \
                SELECT_AMP_UART1_TIMER_MODE
}

Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER
    _CHILDREN   FOLDER_BSP_CONFIG
}

/* Override the default (empty) component definitions with custom options */

Component INCLUDE_AMP_CPU_03  {
    NAME        AMP kernel configured for core 03
    _CHILDREN   SELECT_AMP_CPU_NUMBER
    CFG_PARAMS  AMP_CPU_NUMBER
    REQUIRES    INCLUDE_AMP_CPU \
                INCLUDE_VXIPI \
                INCLUDE_VXCPULIB \
                INCLUDE_WRLOAD_IMAGE_BUILD
}

Component INCLUDE_AMP_CPU_02  {
    NAME        AMP kernel configured for core 02
    _CHILDREN   SELECT_AMP_CPU_NUMBER
    CFG_PARAMS  AMP_CPU_NUMBER
    REQUIRES    INCLUDE_AMP_CPU \
                INCLUDE_VXIPI \
                INCLUDE_VXCPULIB \
                INCLUDE_WRLOAD_IMAGE_BUILD
}

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
                INCLUDE_AMP_LSI_END_DRIVER
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
        NAME    Bootrom Copy region
        DEFAULT (INCLUDE_AMP_CPU_03)::(0x32000000) \
                (INCLUDE_AMP_CPU_02)::(0x21000000) \
                (INCLUDE_AMP_CPU_01)::(0x11000000) \
                (INCLUDE_AMP_CPU_00)::(0x01000000) \
                (INCLUDE_BOOT_RAM_IMAGE)::(0x01e00000) \
                (INCLUDE_BOOT_APP)::(0x01800000) \
                (0x01000000)
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_AMP_CPU_03)::(0x31000000) \
                (INCLUDE_AMP_CPU_02)::(0x20100000) \
                (INCLUDE_AMP_CPU_01)::(0x10100000) \
                (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_BOOT_RAM_IMAGE)::(0x01a00000) \
                (INCLUDE_BOOT_APP)::(0x01400000) \
                (0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
        NAME     system memory start address
        DEFAULT (INCLUDE_AMP_CPU_03)::(0x30000000) \
                (INCLUDE_AMP_CPU_02)::(0x20000000) \
                (INCLUDE_AMP_CPU_01)::(0x10000000) \
                (INCLUDE_AMP_CPU_00)::(0x00000000) \
                (INCLUDE_BOOT_APP)::(0x00000000) \
                (0x00000000)
}

/* For core 4, we must reserve boot memory for ctrl + x command */

Parameter LOCAL_MEM_SIZE {
        NAME    system memory size
        DEFAULT (INCLUDE_AMP_CPU_03)::(0x10000000 - ROM_SIZE) \
                (INCLUDE_AMP_CPU_02)::(0x10000000) \
                (INCLUDE_AMP_CPU_01)::(0x10000000) \
                (INCLUDE_AMP_CPU_00)::(0x10000000) \
                (INCLUDE_BOOT_APP)::(0x40000000 - ROM_SIZE) \
                (0x40000000 - ROM_SIZE)
}

Parameter  RAM_LOW_ADRS_CPU0 {
    NAME        Ram low address specifically for core 0
    SYNOPSIS    bootrom uses the address to calcuate the space for core 1~3 to \
                save/check CPU status, so the address must be identical with \
                bootrom.
    DEFAULT     0x00100000
}

Parameter AMP_CPU_NUMBER {
    NAME        AMP CPU number
    DEFAULT     (INCLUDE_AMP_CPU_03)::(3) \
                (INCLUDE_AMP_CPU_02)::(2) \
                (INCLUDE_AMP_CPU_01)::(1) \
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

/* use AXIS timer as the auxClk */

Component DRV_TIMER_LSI {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

