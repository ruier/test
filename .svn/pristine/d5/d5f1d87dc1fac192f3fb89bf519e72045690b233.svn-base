/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2009, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,01jun10,dhy  Add PCI component for WorkBench. (WIND00143454)
01e,23dec09,y_y  Fix wrSbc8548 VIP image with PROFILE_BOOTAPP boot failed. (WIND00193103)
01d,12may09,fao  Fix PROFILE_BASIC_OS build problems. (WIND00162782)
01c,29sep08,pgh  Fix PROFILE_BOOTAPP build problems.
01b,05sep08,pee  changing FP order so that VSB get e500v2 by default
01a,24may07,b_m  created.
*/

Bsp wrSbc8548 {
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL \
                INCLUDE_VXBUS \
                DRV_TIMER_M85XX \
                INCLUDE_TIMER_SYS \
                DRV_SIO_NS16550 \
                INCLUDE_INTCTLR_LIB \
                DRV_INTCTLR_EPIC \
                DRV_INTCTLR_PPC
    FP          e500v2 soft
}

Component       INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_MII_BUS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                    (INCLUDE_BOOT_APP)::(0x01400000)    \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                    (INCLUDE_BOOT_APP)::(0x01000000)    \
                    0x00100000
}

Component INCLUDE_RAPIDIO_BUS {
       REQUIRES   += INCLUDE_M85XX_RAPIDIO \
                  INCLUDE_M85XX_CPU DRV_RESOURCE_M85XXCCSR \
                  RAPIDIO_BUS_STATIC_TABLE \
                  VXBUS_TABLE_CONFIG
}

Component INCLUDE_SM_COMMON {
       REQUIRES   +=  INCLUDE_M85XX_RIO_SM_CFG INCLUDE_VXBUS_SM_SUPPORT \
                  INCLUDE_VXBUS_SM_SUPPORT INCLUDE_SMEND INCLUDE_SM_NET \
                  INCLUDE_IPATTACH INCLUDE_IFCONFIG \
                  INCLUDE_IPWRAP_IPPROTO
}

Component INCLUDE_PCI_BUS {
       REQUIRES   +=  DRV_PCIBUS_M85XX DRV_RESOURCE_M85XXCCSR \
	              INCLUDE_PCI_BUS_AUTOCONF
}
