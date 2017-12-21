/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,07nov12,syt  added DRV_VXB_NORFLASH_CMDSET_AMD as ads8572 default
                 included components. (WIND00386971)
01o,11jul12,l_z  Merge AMP support
01n,24feb12,d_l  fix RAM_HIGH_ADRS and RAM_LOW_ADRS. (WIND00332559)
01m,18feb10,mze  mod to MP_OPTION
01l,23oct08,pch  CQ135295: fix RAM_{HIGH,LOW}_ADRS for bootapps
01k,05sep08,pee  changing FP order so that VSB get e500v2 by default
01j,01jul08,dtr  Add dependencies for PCI_BUS.
01i,30apr08,dtr  Remove boot MAC handler..
01h,22apr08,dtr  Remove INCLUDE_TSEC_MDIO driver.
01g,11apr08,dtr  Add BOOT_APP build options.
01f,17jan08,dtr  Remove e500v2 option .
01e,19dec07,dtr  Support for AMP.
01d,28nov07,dtr  Add e500v2 FP type. 
01c,20sep07,dtr  Fix for 8572.
01b,19jul07,ebh  Add AMP advertising option
01a,19jul07,pee  created
*/

Bsp ads8572 {
    CPU         PPC32
    MP_OPTIONS  SMP
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_EPIC      \
                DRV_INTCTLR_PPC       \
                DRV_TIMER_M85XX       \
                DRV_SIO_NS16550       \
                INCLUDE_KERNEL        \
                DRV_VXB_NORFLASH_CMDSET_AMD
}

Component       INCLUDE_ETSEC_VXB_END {
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
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                (INCLUDE_BOOT_APP)::(0x01E00000)   \
                (INCLUDE_AMP_CPU_00)::(0x01000000) \
                (INCLUDE_AMP_CPU_01)::(0x22000000) \
                (0x01000000)
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01000000) \
                (INCLUDE_BOOT_APP)::(0x01800000)   \
                (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_AMP_CPU_01)::(0x21000000) \
                (0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        system memory start address
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00000000) \
                (INCLUDE_AMP_CPU_01)::(0x20000000) \
                (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME         system memory size
    DEFAULT      (INCLUDE_AMP_CPU_00)::(0x20000000) \
                 (INCLUDE_AMP_CPU_01)::(0x20000000) \
                 (0x40000000)
}

Parameter  RAM_LOW_ADRS_CPU0 {
    NAME         Ram low address specifically for cpu0
    DEFAULT      0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME         MDIO anchor adrs
    _CFG_PARAMS  INCLUDE_AMP_CPU
    DEFAULT      (0x4400 + 0x20000000)
}

Component INCLUDE_PCI_BUS {
    REQUIRES     INCLUDE_PCI_BUS_AUTOCONF \
                 DRV_PCIBUS_M85XX
}
