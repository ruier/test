/* config.h - IBM750CL Evaluation Board BSP configuration header file */

/*
 * Copyright (c) 2005-2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright (c) 2005-2006 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01k,15apr10,clx  update BSP version for vxWorks 6.9
01j,16oct08,y_w  fix the BootApp VTS test failure. (WIND00134573)
01i,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01h,03jul08,y_w  add VxBus support and update revision for VxWorks 6.7
01g,08nov07,y_w  added complete support for booting from big flash.
                 undef INCLUDE_SECONDARY_GIGE to increase the booting speed.
01f,05aug07,y_w  revision update for VxWorks 6.5
01e,20jul07,y_w  add 'M' command for changing the MAC address in the bootrom
01d,10jul07,y_w  add option to support TFFS to the "Big Flash"
01c,10jan07,y_w  modified based on Holly (IBM750CL) Evaluation Board
01b,13jul06,avb  modified based on HPC-II BSP
01a,21jul05,avb  created based on templatePpc BSP
*/

/*
DESCRIPTION
This file contains the configuration parameters for the IBM750CL EVB BSP.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/0"        /* 0 for first revision */

#include <configAll.h>      /* Set the VxWorks default configuration */

#define BOOT_DEV_NAME "tsiG0"
#define DEFAULT_BOOT_LINE \
        "tsiG(0,0)host:vxWorks e=192.168.118.60 h=192.168.118.24 g=192.168.118.1 \
        f=0x0 tn=IBM_750CL"

#define SYS_MODEL   "IBM750 Eval. Board "

/* vxbus support */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE     50000
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC

/* 
 * If load vxWorks image from PIBS2 with "bootfile" command, 
 * LOAD_VXWORKS_FROM_PIBS2 should be define.
 */

#undef LOAD_VXWORKS_FROM_PIBS2

#ifdef LOAD_VXWORKS_FROM_PIBS2
#   undef BOOT_LINE_OFFSET
#   define BOOT_LINE_OFFSET NV_RAM_ADRS
#endif /* LOAD_VXWORKS_FROM_PIBS2 */

/*
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. This is usually used to debug downloaded images without
 * a bootrom present.
 */

#undef  FORCE_DEFAULT_BOOT_LINE

/* User include/exclude section */

#define INCLUDE_I2C                 /* i2c.c */
#define INCLUDE_NVRAM               /* NVRAM driver byteNvRam.c  */
#define INCLUDE_RTC                 /* RTC driver */
#define INCLUDE_FLASH               /* flashMem.c            */
#undef  INCLUDE_TFFS                /* Support for TrueFFS */
#define INCLUDE_SYS_COPY_TO_FLASH   /* sysCopyToFlash() - sysLib.c  */ 
#define INCLUDE_AUX_CLK             /* Support Aux Clock */

#define INCLUDE_CACHE_SUPPORT       /* cacheLib support         */
#define INCLUDE_CACHE_L2            /* include L2 cache support     */
#define INCLUDE_PCI                 /* pciConfigLib/pciIntLib   */
#define INCLUDE_PCI_AUTOCONF
#define INCLUDE_SERIAL
#define INCLUDE_END
#define INCLUDE_NETWORK             /* include networking support */
#define INCLUDE_TSI108_DMA          /* Include Tsi108 DMA support library */

#define INCLUDE_MMU_BASIC           /* bundled MMU support */
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE
#define SNOOP_ENABLE                /* enable snooping */

/* Serial port configuration */

#undef  NUM_TTY
#define NUM_TTY                 N_SIO_CHANNELS  /* defined in template.h */

#undef  CONSOLE_TTY 
#define CONSOLE_TTY             0
#undef  WDB_TTY_CHANNEL 
#define WDB_TTY_CHANNEL         1

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

/* Memory configuration */

#define LOCAL_MEM_LOCAL_ADRS    0x00000000  /* Physical start of RAM */
#undef  LOCAL_MEM_AUTOSIZE                  /* run-time memory sizing */
#define LOCAL_MEM_SIZE          0x20000000  /* 512M of SDRAM memory */
#define USER_RESERVED_MEM       0x02000000  /* 32MB for user */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

/* The macro BOOT_FROM_BIGFLASH is defined in Makefile */

#ifdef BOOT_FROM_BIGFLASH
#   define ROM_BASE_ADRS       0xfe000000      /* base address of ROM */
#else
#   define ROM_BASE_ADRS       0xfff00000      /* base address of ROM */
#endif /* BOOT_FROM_BIGFLASH */

#define ROM_TEXT_ADRS       (ROM_BASE_ADRS+0x0100)  /* with PC & SP */
#define ROM_TEXT2_ADRS      (FLASH_ADRS+0x0100)
#define ROM_SIZE            0x00100000              /* 1MB ROM space */
#define ROM_WARM_ADRS       (ROM_TEXT_ADRS+0x0008)  /* warm reboot entry */

/*
 * Cache configuration
 *
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#ifdef  INCLUDE_CACHE_SUPPORT

#   define USER_I_CACHE_ENABLE              /* Enable INSTRUCTION CACHE */
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE CACHE_COPYBACK /* select COPYBACK or WRITETHROUGH */

#   define USER_D_CACHE_ENABLE              /* Enable DATA CACHE */
#   undef  USER_D_CACHE_MODE

#   ifdef SNOOP_ENABLE
#       define USER_D_CACHE_MODE (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   else
#       define USER_D_CACHE_MODE (CACHE_COPYBACK)
#   endif /* SNOOP_ENABLE */

#else  /* INCLUDE_CACHE_SUPPORT */

#   undef  USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE   CACHE_DISABLED
#   undef  USER_D_CACHE_ENABLE
#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE   CACHE_DISABLED
#   undef  USER_B_CACHE_ENABLE

#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_CACHE_L2

#define L2CACHE_MODE_WRITETHROUGH   0
#define L2CACHE_MODE_COPYBACK       1

/* Set the default L2 cache mode */

#define L2CACHE_MODE        L2CACHE_MODE_COPYBACK

#define L2CACHE_SIZE        L2CACHE_SIZE_1M      /* 1MB  in sysL2Cache.h */

#endif /* INCLUDE_CACHE_L2  */

/* Network driver configuration */

#ifdef  INCLUDE_NETWORK
#   define INCLUDE_END          /* Enhanced Network Driver (see configNet.h) */
#   define INCLUDE_TSI108_END   /* include tsi108End driver */
#   define INCLUDE_PRIMARY_GIGE
#   undef  INCLUDE_SECONDARY_GIGE   /* include init of secondary tsi108End */
#endif /* INCLUDE_NETWORK */

#ifdef INCLUDE_PCI

    /* Intel 8254x support */

#   undef INCLUDE_GEI8254X_END

#   ifdef INCLUDE_GEI8254X_END
#       define INCLUDE_PRIMARY_GEI_END
#       undef INCLUDE_SECONDARY_GEI_END
#       ifndef INCLUDE_END
#           define INCLUDE_END
#       endif /* INCLUDE_END */
#   endif /* INCLUDE_GEI8254X_END */

#endif /* INCLUDE_PCI */

#if defined(INCLUDE_FLASH) && defined(INCLUDE_TFFS)
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_FS_EVENT_UTIL   /* File system event utility */
#   define INCLUDE_FS_MONITOR      /* File system monitor */
#   define INCLUDE_ERF             /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER  /* Device Manager */
#   define INCLUDE_XBD             /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV     /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#endif /* INCLUDE_FLASH && INCLUDE_TFFS */

/* MAC Address configuration */

#define MAC_ADRS_LEN    6   /* 6 bytes in MAC address */
#define MAX_MAC_ADRS    2   /* number of MAC addresses to save in NVRAM */

/* enable 'M' command in bootrom */

#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS 1  /* one network device */

#ifdef BOOTAPP
#   define INCLUDE_BOOT_ETH_MAC_HANDLER
#endif /* BOOTAPP */

/*
 * The Ethernet hardware address is of the form
 *
 *       00:0d:60:nn:nn:nn
 *
 * where the first three bytes are defined below, and last three bytes are
 * user specified.
 */

#define WR_ENET0        0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1        0x0d
#define WR_ENET2        0x60

#define CUST_ENET3      0x11  /* Customer portion of MAC address */
#define CUST_ENET4      0x12
#define CUST_ENET5_0    0x00
#define CUST_ENET5_1    0x00

/* default mac address */

#define ENET_DEFAULT0   WR_ENET0
#define ENET_DEFAULT1   WR_ENET1
#define ENET_DEFAULT2   WR_ENET2

#include "ppc750evb.h"  /* Verify user options, specify i/o addr etc */

#ifdef __cplusplus
}
#endif

#endif  /* __INCconfigh */
#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

