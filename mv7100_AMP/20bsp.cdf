/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008, 2009, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,29mar12,c_l  Fix NV_BOOT_OFFSET for AMP. (WIND00328678)
01b,20feb09,e_d  Add support for wrload
01a,31jan08,mdg  created
*/

Bsp mv7100_AMP {
    NAME        VME MV7100 board support package
    REQUIRES    INCLUDE_KERNEL \
                DRV_SIO_NS16550 \
                INCLUDE_AMP_CPU
    CPU         PPC32
    ENDIAN      big
    FP          hard
} 

/* 
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
 
Component    INCLUDE_BOOT_NET_DEVICES {
             REQUIRES   INCLUDE_ETSEC_VXB_END
} 
 
/*
 * Required component for AMP.
 */
 
Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER
    _CHILDREN   FOLDER_BSP_CONFIG
}

/*
 * Override the default (empty) component definitions with custom options
 */
 
Component INCLUDE_AMP_CPU_01 {
    NAME        AMP kernel configured for core 01
}

Component INCLUDE_AMP_CPU_00 { 
    NAME        AMP kernel configured for core 00
}

/*
 * Maxim DS1621 Temp Sensor Support
 *
 * This allows the support for the ADT7461 Temperature sensor
 * to be enabled/disabled.
 *
 * define/undef INCLUDE_ADT7461
 */

Component INCLUDE_ADT7461 {
    NAME      adt7461 temperature sensor support
    SYNOPSIS  enable support for the adt7461 temp sensor
    _CHILDREN FOLDER_BSP_CONFIG
}

/* 
 * Maxim DS1375 Real-Time Clock 
 *
 * Allows the real-time clock support to be enabled/disabled.
 *
 * define/undef INCLUDE_RTC
 */

Component INCLUDE_RTC {
    NAME      maxim ds1375 real-time clock
    SYNOPSIS  enables real-time clock of maxim ds1375
    _CHILDREN FOLDER_BSP_CONFIG
}

/*
 * MVME7100 Watchdog Timer Support
 *
 * This allows the mvme7100's watchdog timer support to be enabled/disabled.
 *
 * define/undef INCLUDE_WDOG_TIMER 
 */

Component INCLUDE_WDOG_TIMER {
    NAME      mvme7100 watchdog timer support
    SYNOPSIS  enable support for the mvme7100s watchdog timer
    _CHILDREN FOLDER_BSP_CONFIG
}

Parameter NV_BOOT_OFFSET {
    NAME        NVRAM boot offset
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x0) \
                (INCLUDE_AMP_CPU_01)::(0x100) \
                (0x0)
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (0x00e00000)
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    NAME        system memory start address
    DEFAULT     (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME        system memory size
    DEFAULT     (0x10000000)

}

Parameter  RAM_LOW_ADRS_CPU0 {
    NAME        Ram low address specifically for cpu0
    DEFAULT     0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME        MDIO anchor adrs
    DEFAULT     (0x4400 + 0x10000000)
}

Component INCLUDE_PCI_BUS {
    REQUIRES    INCLUDE_PCI_BUS_AUTOCONF \
                DRV_PCIBUS_M85XX        \
                INCLUDE_PCI_OLD_CONFIG_ROUTINES \
                DRV_RESOURCE_M85XXCCSR
}
