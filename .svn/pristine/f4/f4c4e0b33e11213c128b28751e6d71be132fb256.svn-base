/* config.h - Freescale Lite5200B board configuration header */

/*
 * Copyright (c) 2003-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,26apr13,ylu  Add INCLUDE_TL_FTL define default.(WIND00414267) 
01g,15apr13,ylu  Increased BSP version to 6.9/0
01f,25jan10,c_t  Increased BSP version to 2.0/5
01e,21jan10,liu  Increased BSP version to 2.0/4
02d,29jun09,z_l  Increased BSP version to 2.0/3 for VxWorks 6.8
02c,06oct08,b_m  update for new MTD driver defines for TFFS;
                 remove legacy fei/gei driver support.
02b,26sep08,to   remove INCLUDE_SHOW_ROUTINES (WIND00135542)
02a,26aug08,l_g  include DRV_TIMER_M5XXX_GPT if defined INCLUDE_AUX_CLK.(WIND00128798)
                 Remove the definitions of RAM_HIGH_ADRS and RAM_LOW_ADRS.(WIND00128773)
01z,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01y,16apr08,b_m  add vxbus support; code cleanup.
01x,21aug07,b_m  undef INCLUDE_FEI82557_END & INCLUDE_GEI8254X_END by default.
01w,07aug07,agf  move MAX_MAC_DEVS from sysNet.h to config.h; inc BSP_REV
01v,18jan07,b_m  Add SYS_MODEL define; code cleanup.
01u,03feb06,pkr  add support for Lite5200b
01t,03feb06,pkr  adapt to different frequency jumpers
01s,24jan05,pkr  Cleanup: remove CP2 switch and VWARE
01r,24jan05,pkr  Remove split of RAM in two pieces (LOCAL_MEM_ADD_SIZE)
01q,24jan05,pkr  Merge in USB1.1 stack and Datalight FlashFx Support
01p,11aug05,j_b  add wireless LAN support
01o,14jun05,j_b  remove FEI END inclusion in project images (SPR 110023)
01n,14jun05,pcm  removed INCLUDE_DOSFS
01m,07mar05,j_b  define INCLUDE_SHOW_ROUTINES only once
01l,30dec04,k_p  SPR #99747,removed INCLUDE_NETWORK dependencies
01k,30dec04,k_p  moved cache related definitions to under
                 #ifdef INCLUDE_CACHE_SUPPORT ... #endif
01j,29dec04,k_p  removed High version(BOOT_HIGH == 0) and added
                 "BSP specific  Default board configurations" section
01i,22dec04,k_p  bump version and revision to 2.0/10
01h,22dec04,k_p  teamF1,Update for vxWorks 6.0
01g,28may04,bjn  Default cache configuration
01f,17mar04,bjn  Enable snooping
01e,05mar04,bjn  BSP Rev 8.
01d,04feb04,bjn  support for IS_ICECUBE_OLD
01c,12nov03,e_c  added NVRAM support (release 1.2/3).
01b,10nov03,pdr  added USB support for bootrom (release 1.2/2).
01a,17sep03,pdr  written for CP2 board.
*/

/*
This file contains the configuration parameters for the BSP.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VER_1_1     0
#define BSP_VER_1_2     1
#define BSP_VERSION     "6.9"
#define BSP_REV         "/0"

#define SYS_MODEL       "Freescale Lite5200B - MPC5200"

#include "configAll.h"  /* Set the VxWorks default configuration */

/* for romInit.s we don't use DYN_FREQ settings so far */

#ifndef _ASMLANGUAGE
#   define DYN_FREQ
#endif

#include "lite5200b.h"

/* Boot loader configuration */

/* Define if U-Boot boots us, undef if VxWorks Bootrom boots use */

#undef BOOT_FROM_UBOOT

#ifdef BOOT_FROM_UBOOT

/*
 * Option 1: U-Boot initiliaze the memory and jump to VxWorks
 * bootrom at some address in Flash, e.g. FFE00000
 */

#   define JUMP_TO_VX_BOOT      1

/*
 * Option 2: define ROM_TEXT_ADRS somewhere in RAM. Load image there
 * with U-Boot. Start bootInit always with WARM BOOT,
 * i.e.ROM_TEXT_ADRS + 0x8
 */

#   define TFTP_BOOT            2

#   define UBOOT_BOOT_OPTION    JUMP_TO_VX_BOOT

#endif /* BOOT_FROM_UBOOT */

/*
 * BSP specific  Default board configurations
 *
 * If a supported feature is desired,
 *         change to: #define
 * If a feature is not desired or not supported
 *         change to: #undef
 *
 */

#define INCLUDE_CACHE_SUPPORT       /* cache support */
#undef  SNOOP_ENABLE                /* snoop */
#define INCLUDE_MMU_BASIC           /* MMU basic */

#define INCLUDE_BESTCOMM            /* Include BestComm SmartDMA support */
#define INCLUDE_FLASH               /* Enable Flash as nvRam */
#undef  INCLUDE_PCI                 /* PCI support */

/*
 * Cache configuration
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#ifdef INCLUDE_CACHE_SUPPORT

#   define USER_I_CACHE_ENABLE
#   define USER_D_CACHE_ENABLE

#   undef USER_I_CACHE_MODE
#   undef USER_D_CACHE_MODE

#   ifdef SNOOP_ENABLE
#       define USER_I_CACHE_MODE    (CACHE_WRITETHROUGH | CACHE_SNOOP_ENABLE)
#       define USER_D_CACHE_MODE    (CACHE_WRITETHROUGH | CACHE_SNOOP_ENABLE)
#   else
#       define USER_I_CACHE_MODE    (CACHE_COPYBACK)
#       define USER_D_CACHE_MODE    (CACHE_COPYBACK)
#   endif /* SNOOP_ENABLE */

#endif /* INCLUDE_CACHE_SUPPORT */

/* MMU configuration,  create a single macro INCLUDE_MMU */

#ifdef INCLUDE_MMU_BASIC
#   define INCLUDE_MMU
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

/* Memory configuration */

/*
 *  The constants ROM_TEXT_ADRS and ROM_SIZE are defined in
 *  config.h, and Makefile. All definitions for these
 *  constants must be identical.
 */

#define LOCAL_MEM_SIZE              0x10000000  /* 256MB */
#define DRAM_BITS                   0x1B

#define LOCAL_MEM_LOCAL_ADRS        0x00000000  /* Physical start of RAM */

/* user reserved memory */

#define USER_RESERVED_MEM           0x00100000  /* user reserved memory size */

/* definitions for ROM_SIZE, ROM_TEXT_ADRS, ROM_BASE_ADR, ROM_TEXT_ADRS */

#define ROM_BASE_ADRS               0xFE000000  /* Physical start of ROM */
#define ROM_SIZE                    0x02000000  /* 32M bytes of ROM space */

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == JUMP_TO_VX_BOOT)
#       define ROM_TEXT_ADRS        0xFF800100
#   else
#       define ROM_TEXT_ADRS        0x02000000
#       undef  ROMSTART_BOOT_CLEAR  /* for romStart.c */
#       define FORCE_DEFAULT_BOOT_LINE
#   endif
#else
#   define ROM_TEXT_ADRS            0xFFF00100  /* ROM entry address */
#endif  /* BOOT_FROM_UBOOT */

#define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 8) /* warm reboot entry */

/* vxbus support */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE             50000
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC
#define INCLUDE_SIO_UTILS
#define DRV_SIO_M5XXX_PSC

/* include DRV_TIMER_M5XXX_GPT if defined INCLUDE_AUX_CLK */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_M5XXX_GPT
#endif /* INCLUDE_AUX_CLK */

/* use vxbus virtual PCI adaptor driver */

#ifdef INCLUDE_PCI
#   define DRV_PCI_VIRTUAL
#endif /* INCLUDE_PCI */

#undef INCLUDE_MII_BUS
#undef INCLUDE_GENERICPHY

/* Serial port configuration */

#undef  NUM_TTY
#define NUM_TTY                 N_SIO_CHANNELS
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

/* Flash starts such that it ends at 0xFFFFFFFF */

#define FLASH_BASE_ADRS         0xFE000000

/* FLASH configuration */

#ifdef INCLUDE_FLASH

#   define SYS_FLASH_TYPE       FLASH_29GL128N
#   define FLASH_WIDTH          1
#   define FLASH_CHIP_WIDTH     1
#   define FLASH_SECTOR_SIZE    0x20000     /* sector size of S29GL128N */
#   define FLASH_ADRS           0xFF000000  /* upper 16MB */

#   undef  FLASH_NO_OVERLAY
#   undef  FLASH_SIZE_WRITEABLE
#   define SYS_FLASH_DELAY

#   define NV_RAM_SIZE          FLASH_SECTOR_SIZE

#   ifdef BOOT_FROM_UBOOT
#       define NV_RAM_ADRS      (ROM_TEXT_ADRS - NV_RAM_SIZE)
#   else
#       define ROM_CODE_OFFSET  0x100
#       define NV_RAM_ADRS      ((ROM_TEXT_ADRS - ROM_CODE_OFFSET) - NV_RAM_SIZE)
#   endif /* BOOT_FROM_UBOOT */

#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET       0

#   define NV_MAC_ADRS_OFFSET   (BOOT_LINE_SIZE + 1)  /* from NV_BOOT_OFFSET */

#else

#   define NV_RAM_SIZE          NONE

#endif  /* INCLUDE_FLASH */

/* PCI configuration */

#ifdef  INCLUDE_PCI

#   define INCLUDE_PCI_AUTOCONF

#   ifndef PCI_CFG_TYPE
#       ifdef INCLUDE_PCI_AUTOCONF
#           define PCI_CFG_TYPE PCI_CFG_AUTO
#       else
#           define PCI_CFG_TYPE PCI_CFG_FORCE
#       endif /* INCLUDE_PCI_AUTOCONF */
#   endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI */

/* END Network driver configuration */

/* De-select unused advanced network driver support */

#define INCLUDE_END         /* Enhanced Network Driver(see configNet.h) */

#define INCLUDE_FEC_END     /* (END) FEC Ethernet driver */

/* FEC END MAC Address configuration */

#ifdef INCLUDE_FEC_END

#   define ETHERNET_MAC_HANDLER     /* enable 'M' command */
#   undef  ETHERNET_ADR_SET         /* disable 'N' command */

#   define MAC_ADRS_LEN         6   /* 6 bytes in MAC address */
#   define MAX_MAC_ADRS         1
#   define MAX_MAC_DEVS         1

/* Freescale specific portion of MAC (MSB->LSB) */

#   define FS_ENET0             0x00
#   define FS_ENET1             0x04
#   define FS_ENET2             0x9F

/* Device specific portion of MAC adrs */

#   define CUST_ENET3           0x01
#   define CUST_ENET4           0x02
#   define CUST_ENET5           0x03

/* default mac address */

#   define ENET_DEFAULT0        FS_ENET0
#   define ENET_DEFAULT1        FS_ENET1
#   define ENET_DEFAULT2        FS_ENET2

#   define ETHERNET_MAC_ADRS    {FS_ENET0, FS_ENET1, FS_ENET2,  \
                                 CUST_ENET3, CUST_ENET4, CUST_ENET5}

#endif /* INCLUDE_FEC_END */

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
#       define UBOOT_ENV        0xFFF40000  /* u-boot environment address */
#       define UBOOT_ENV_SIZE   0x4000
#       undef  ETHERNET_ADR_SET             /* disable 'N' command */
#       undef  ETHERNET_MAC_HANDLER         /* enable 'M' command */
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

/* add TFFS as boot device */

#undef INCLUDE_TFFS

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT         0           /* 8-bit per flash */
#define S29GL_FLASH_NUM         1           /* only one flash */
#define S29GL_FLASH_WIDTH       1           /* 8-bit per CS */
#define S29GL_SECTOR_SIZE       0x00020000  /* 128K per sector */
#define S29GL_FLASH_SIZE        0x01000000  /* 16MB per chip */
#define S29GL_FLASH_OP_DELAY

#ifdef INCLUDE_TFFS
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_TL_FTL                  /* FTL translation layer */
#endif /* INCLUDE_TFFS */

/* add ATA as boot device */

#undef INCLUDE_ATA

#ifdef INCLUDE_ATA
#   define INCLUDE_ATA_SHOW
#endif

/* add USB as boot device */

#undef INCLUDE_USB
#undef INCLUDE_OHCI

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#endif /* INCLUDE_USB */
#ifdef INCLUDE_OHCI
#   define INCLUDE_OHCI_INIT
#endif /* INCLUDE_OHCI */

#undef INCLUDE_USB_MS_BULKONLY

#ifdef INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#endif /* INCLUDE_USB_MS_BULKONLY */

#ifdef INCLUDE_TFFS
#   ifndef INCLUDE_DOSFS
#       define INCLUDE_DOSFS        /* file system to be used */
#   endif   /* INCLUDE_DOSFS */
#endif  /* INCLUDE_TFFS */

#ifdef INCLUDE_ATA
#   ifndef INCLUDE_DOSFS
#       define INCLUDE_DOSFS        /* file system to be used */
#   endif   /* INCLUDE_DOSFS */
#endif  /* INCLUDE_ATA */

#ifdef INCLUDE_USB_MS_BULKONLY
#   ifndef INCLUDE_DOSFS
#       define INCLUDE_DOSFS        /* file system to be used */
#   endif /* INCLUDE_DOSFS */
#endif  /* INCLUDE_USB_MS_BULKONLY */

/* Use DOS File System */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CACHE
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DISK_UTIL
#endif  /* INCLUDE_DOSFS */

/* CAN 2.0 A/B support */

#undef INCLUDE_CAN

#ifdef INCLUDE_CAN
#   define INCLUDE_MPC5200_WNCAN
#   define INCLUDE_CAN_NETWORK_INIT
#endif  /* INCLUDE_CAN */

#ifdef INCLUDE_MPC5200_WNCAN

/* CAN Controllers number */

#   define NUM_MPC5200_MSCAN_CONTROLLERS    2

/*
 * CAN Controllers clock source
 * 1: use IPB_CLK
 * 0: use SYS_XTAL_IN
 * If you are configuring more than one MPC5200 MSCAN controller,
 * list a clock source for each controller, separated by spaces.
 */

#   define MPC5200_MSCAN_CLOCK_SOURCE       1 1

#endif

/* Debuging configuration */

/*
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. See target.nr for details. This is usually used to debug
 * WR downloaded images with out a bootrom present.
 */

#ifdef  INCLUDE_WDB_COMM_VTMD

/* When using TMD this macro should be undefined */

#define FORCE_DEFAULT_BOOT_LINE

#endif  /* INCLUDE_WDB_COMM_VTMD */

/* Stuff to be excluded if FORCE_DEFAULT_BOOT_LINE defined */

#ifdef  FORCE_DEFAULT_BOOT_LINE
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE      NONE
#endif  /* FORCE_DEFAULT_BOOT_LINE */

/* Boot line configuration */

#define BOOT_DEV_NAME       "fec"
#define HOSTNM              "host"
#define HOSTPATH            "vxWorks"
#define TARGETNM            "lite5200b"
#define HOSTIP              "0.0.0.0"
#define TARGETIP            "0.0.0.0:ffffff00"
#define GATEWYIP            "0.0.0.0"
#define USERNAME            "target"
#define PASSWORD            "vxTarget"

#define DEFAULT_BOOT_LINE   BOOT_DEV_NAME   \
                            "(0,0)" HOSTNM ":" HOSTPATH " " \
                            "e=" TARGETIP " "   \
                            "h=" HOSTIP " "     \
                            "g=" GATEWYIP " "   \
                            "u=" USERNAME " pw=" PASSWORD " "   \
                            "f=0x8 tn=" TARGETNM

#if !defined(PRJ_BUILD)
/* configuration for bootrom */
#define INCLUDE_ERF
#define INCLUDE_DEVICE_MANAGER
#define INCLUDE_XBD
#define INCLUDE_FS_MONITOR
#define INCLUDE_FS_EVENT_UTIL
#endif

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCconfigh */
