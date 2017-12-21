/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,12jul12,l_z  Merge AMP support
01j,29nov10,swu  Increase BOOTAPP space size.(WIND00192157)
01i,18feb10,mze  mod to MP_OPTION
01h,05may09,rgo  Move the SM_BASE_ADRS definition to SM common cdf(wind145934)
01g,17mar09,rgo  Define configurable shared memory base with macro(wind00145934)
01f,29oct08,dcc  removed AMP from MP_OPTIONS
01e,01sep08,j_z  Added REQUIRES of Bsp hpcNet8641(WIND00107919).
01d,26aug08,pgh  Fix BootApp builds
01c,11sep07,jmg  Added VX_SMP_NUM_CPUS and fix typo in Bsp.
01b,19jul07,ebh  Add AMP advertising option.
01a,19jul07,pee  created
*/

Bsp hpcNet8641 {
    NAME        Freescale 8641D board support package
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          hard
    REQUIRES    INCLUDE_KERNEL    \
                DRV_TIMER_DEC_PPC \
                DRV_SIO_NS16550   \
                DRV_INTCTLR_EPIC  \
                DRV_INTCTLR_PPC
}


Component       INCLUDE_ETSEC_VXB_END {
    REQUIRES +=     INCLUDE_GENERICPHY  
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/* Required component for AMP */

Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER \
                INCLUDE_TSEC_MDIO
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

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
    NAME       Bootrom Copy region
    DEFAULT    (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
               (INCLUDE_BOOT_APP)::(0x01400000)        \
               (INCLUDE_AMP_CPU_00)::(0x00e00000) \
               (INCLUDE_AMP_CPU_01)::(0x00e00000) \
               0x00e00000
}

Parameter RAM_LOW_ADRS {
    NAME       Runtime kernel load address
    DEFAULT    (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
               (INCLUDE_BOOT_APP)::(0x0e00000)         \
               (INCLUDE_AMP_CPU_00)::(0x00100000) \
               (INCLUDE_AMP_CPU_01)::(0x00100000) \
               0x00100000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME       system memory start address
    DEFAULT    (INCLUDE_AMP_CPU_00)::(0x00000000) \
               (INCLUDE_AMP_CPU_01)::(0x00000000) \
               (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME       system memory size
    DEFAULT    (INCLUDE_AMP_CPU_00)::(0x10000000) \
               (INCLUDE_AMP_CPU_01)::(0x10000000) \
               (0x20000000)
}

Parameter TM_ANCHOR_ADRS {
    NAME        MDIO anchor adrs
   _CFG_PARAMS  INCLUDE_AMP_CPU
    DEFAULT     (0x4400 + 0x10000000)
}

Component INCLUDE_PCI_BUS {
    REQUIRES    INCLUDE_PCI_BUS_AUTOCONF \
                DRV_PCIBUS_M85XX
}
