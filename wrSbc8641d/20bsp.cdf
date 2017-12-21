/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008, 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,12jul12,l_z  Merge AMP support
01f,13jan11,c_p  added component INCLUDE_GENERICPHY (WIND00244693)
01e,18feb10,mze  mod to MP_OPTIONS
01d,29oct08,dcc  removed AMP from MP_OPTIONS
01c,11sep08,l_z  Fix bootApp build error 
01b,07sep07,jmg  Added VX_SMP_NUM_CPUS and changed Bsp to wrSbc8641d.
01a,06jun07,x_s  initial created.
*/

Bsp wrSbc8641d {
    NAME        Wind River SBC8641D board support package
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

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY    
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
   NAME        Bootrom Copy region
   DEFAULT     (INCLUDE_BOOT_APP)::(0x01000000)   \
               (INCLUDE_AMP_CPU_00)::(0x00e00000) \
               (INCLUDE_AMP_CPU_01)::(0x00e00000) \
               0x00e00000
}

Parameter RAM_LOW_ADRS {
   NAME        Runtime kernel load address
   DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01200000) \
               (INCLUDE_BOOT_APP)::(0x00e00000)       \
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
