/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,11apr11,swu  fix build error with INCLUDE_USB_TARG. (WIND00255092)
01e,18feb10,mze  mod to MP_OPTIONS
01d,17dec09,e_d  remove SIO and Network port assignment
01c,14dec09,e_d  removed AMP from MP_OPTIONS 
01b,28dec08,e_d  Add support for bootapp
01a,31jan08,mdg  created
*/

Bsp mv7100 {
    NAME        VME MV7100 board support package
    REQUIRES    INCLUDE_KERNEL        \
                INCLUDE_GENERICPHY    \
                DRV_TIMER_DEC_PPC     \
                DRV_SIO_NS16550       \
                DRV_INTCTLR_EPIC      \
                DRV_INTCTLR_PPC
    CPU         PPC32
    MP_OPTIONS  UP SMP
    ENDIAN      big
    FP          hard
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

Component     INCLUDE_BOOT_NET_DEVICES {
    REQUIRES INCLUDE_ETSEC_VXB_END
} 
 
Parameter VX_SMP_NUM_CPUS {
    NAME      Number of CPUs available to be enabled for VxWorks SMP
    TYPE      UINT
    DEFAULT		2
}


/*
 * Flash Boot Image Programming
 *
 * This allows the boot image programming support to be enabled/disabled.
 *
 * define/undef INCLUDE_BOOT_IMAGE_PROGRAM
 */

Component INCLUDE_BOOT_IMAGE_PROGRAM {
    NAME      flash boot image programming support
    SYNOPSIS  Flash Boot Image Programming Support
    _CHILDREN FOLDER_BSP_CONFIG
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


/* Memory definitions */ 
 
Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                (INCLUDE_BOOT_APP)::(0x01500000)        \
                0x00E00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01500000) \
                (INCLUDE_BOOT_APP)::(0x01200000)       \
                0x00100000
}

Component INCLUDE_PCI_BUS {
    REQUIRES    INCLUDE_PCI_BUS_AUTOCONF \
                DRV_PCIBUS_M85XX        \
                INCLUDE_PCI_OLD_CONFIG_ROUTINES \
                DRV_RESOURCE_M85XXCCSR
}

Component INCLUDE_USB_TARG {
    REQUIRES    += INCLUDE_PCI_BUS
}
