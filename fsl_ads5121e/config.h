/* config.h - Freescale ADS5121e BSP configuration file */

/*
 * Copyright (c) 2007-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01v,08may13,d_l  add INCLUDE_I2C when define INCLUDE_RTC. (WIND00415725) 
                 uprev BSP_REV to "6.9/1"
01u,12jul12,x_s  define the macro S29GL_FLASH_OP_DELAY by default. (WIND00360357)
01t,07jun10,pch  nvram & flash config adjustments
01s,21may10,b_m  add dependency for WindML.
01r,04may10,clx  update BSP version for vxWorks 6.9
01q,16apr10,ydh  modify component name(WIND00204841) 
01p,09oct09,x_f  add NAND via datalight support. (WIND00184589)
01o,09sep09,w_x  Update USB target configuration(WIND00180575)
01n,22jun09,b_m  bump up BSP_REV for 6.8.
01m,23apr09,x_f  Update BSP_REV to "/3"
01l,11mar09,w_x  add USB device support.
01k,05mar09,b_m  add FIFOC and AXE support.
01j,06feb09,x_f  add RTC support.
01i,04feb09,w_x  add USB host support.
01h,25dec08,x_f  add bootrom, CAN, PCI and CPLD intCtlr support for
		 PCB rev 4 hardware.
01g,16sep08,b_m  add auxiliary clock support.
01f,25aug08,b_m  support rev 3.x hardware.
01e,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01d,13aug08,x_f  Update BSP_REV to "/1", replace DRV_SIO_M5XXXPSC
		 with DRV_SIO_M5XXX_PSC
01c,07may08,b_m  add macros needed by TFFS MTD driver.
01b,24dec07,b_m  add ppcIntCtrl & IPIC support.
01a,28sep07,b_m  created.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

#include "configAll.h"

#include "fsl_ads5121e.h"

#define SYS_MODEL       "Freescale MPC5121E ADS"

/* define if U-Boot boots us, undef if VxWorks bootrom boots us */

#undef BOOT_FROM_UBOOT

#define INCLUDE_CACHE_SUPPORT       /* cache support */
#define INCLUDE_MMU_BASIC           /* MMU basic */

/*
 * Cache configuration
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#ifdef INCLUDE_CACHE_SUPPORT
#   define USER_I_CACHE_ENABLE
#   define USER_D_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   undef  USER_D_CACHE_MODE
#   define USER_I_CACHE_MODE        (CACHE_COPYBACK)
#   define USER_D_CACHE_MODE        (CACHE_COPYBACK)
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

/* main memory */

#define LOCAL_MEM_LOCAL_ADRS        0x00000000  /* Physical start of RAM */
#define LOCAL_MEM_SIZE              DDR_MEM_SIZE

/* user reserved memory */

#define USER_RESERVED_MEM           0x00000000  /* user reserved memory size */

#define ROM_BASE_ADRS               0xFC000000  /* Physical start of ROM */
#define ROM_SIZE                    0x04000000  /* 64M bytes of ROM space */

#ifdef BOOT_FROM_UBOOT
#   define ROM_TEXT_ADRS            0xFFE00100
#else  /* BOOT_FROM_UBOOT */
#   define ROM_TEXT_ADRS            0xFFF00100
#endif /* BOOT_FROM_UBOOT */

#define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 8) /* warm reboot entry */

/* vxbus support */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_DMA_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE             50000
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC
#define DRV_MF_M512X_FIFOC
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC
#define DRV_SIO_M5XXX_PSC
#define INCLUDE_SIO_UTILS
#define INCLUDE_M512XFEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#ifndef ADS5121E_REV2
#   define DRV_INTCTLR_ADS5121E_CPLD
#   define INCLUDE_VXB_LEGACY_INTERRUPTS
#endif /* ADS5121E_REV2 */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_M5XXX_GPT
#endif	/* INCLUDE_AUX_CLK */

/* AXE support */

#undef DRV_FW_FSL_AXE

/* serial line */

#undef  NUM_TTY
#ifdef INCLUDE_SER_EMULATOR
#define NUM_TTY                 3
#else  /* INCLUDE_SER_EMULATOR */
#define NUM_TTY                 2
#endif /* INCLUDE_SER_EMULATOR */

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#undef  CONSOLE_TTY
#define CONSOLE_TTY             0       /* console channel */

/* PCI support (define only on PCB rev 4.x hardware) */

#undef DRV_PCIBUS_M83XX
#ifdef ADS5121E_REV2
#   undef DRV_PCIBUS_M83XX
#endif /* ADS5121E_REV2 */

#ifdef DRV_PCIBUS_M83XX
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   ifndef DRV_INTCTLR_ADS5121E_CPLD
#       define DRV_INTCTLR_ADS5121E_CPLD
#   endif  /* DRV_INTCTLR_ADS5121E_CPLD */
#endif /* DRV_PCIBUS_M83XX */

/*
 * INCLUDE_NAND_FLASH is defined to support NAND flash chip
 * and datalight tools
 */

#undef INCLUDE_NAND_FLASH

#ifdef INCLUDE_NAND_FLASH
#   define INCLUDE_FLASHFX_DISK_0
#   define FFX_DISK_NAME_0          "/d0"       /* name of device */
#   define FFX_DEFAULT_FS_0         1
#   define FFX_FORMAT_STATE_0       1
#   define FFX_DISK_SIZE_0          0xFFFFFFFF
#   define INCLUDE_FLASHFX_DRIVER
#   define FFX_CONFIG_FLAGS         0
#   define INCLUDE_FLASHFX_TESTS
#   define INCLUDE_FLASHFX_TOOLS
#endif /* INCLUDE_NAND_FLASH */

/* NVRAM configuration */

#define	INCLUDE_NVRAM
#define	INCLUDE_NVRAM_WRITE
#define	INCLUDE_FLASH_WRITE

#define SYS_FLASH_TYPE          FLASH_29GL256N
#define FLASH_ADRS              FLASH_BASE_ADRS
#define FLASH_SIZE              0x04000000

#define FLASH_WIDTH             4
#define FLASH_CHIP_WIDTH        4
#define FLASH_SECTOR_SIZE       0x40000

#undef  FLASH_NO_OVERLAY
#undef  FLASH_SIZE_WRITEABLE

#define NV_RAM_SIZE             FLASH_SECTOR_SIZE

#define ROM_CODE_OFFSET         0x100
#define NV_RAM_ADRS             (ROM_TEXT_ADRS - ROM_CODE_OFFSET - NV_RAM_SIZE)

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0

#define NV_MAC_ADRS_OFFSET      0x200

/* END Network driver configuration */

#define INCLUDE_END             /* Enhanced Network Driver(see configNet.h) */

/* FEC END MAC Address configuration */

#define ETHERNET_MAC_HANDLER

#define MAC_ADRS_LEN            6
#define MAX_MAC_ADRS            1
#define MAX_MAC_DEVS            1

#define FSL_ENET0               0x00
#define FSL_ENET1               0x04
#define FSL_ENET2               0x9F

#define CUST_ENET3              0x01
#define CUST_ENET4              0x02
#define CUST_ENET5              0x03

/* default mac address */

#define ENET_DEFAULT0           FSL_ENET0
#define ENET_DEFAULT1           FSL_ENET1
#define ENET_DEFAULT2           FSL_ENET2

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT         1
#define S29GL_FLASH_NUM         2           /* two chips */
#define S29GL_FLASH_WIDTH       4
#define S29GL_SECTOR_SIZE       0x00020000  /* 128K per sector */
#define S29GL_FLASH_SIZE        0x02000000  /* 32MB per chip */
#define S29GL_FLASH_OP_DELAY    /* delay is required for flash operation */

#undef INCLUDE_TFFS

/* ATA support */

#undef INCLUDE_ATA

/* I2C support */

#undef INCLUDE_I2C
#ifdef INCLUDE_ATA
#define INCLUDE_I2C
#endif	/* INCLUDE_ATA */

/* RTC support */

#undef INCLUDE_RTC
#if defined(INCLUDE_RTC) && !defined(INCLUDE_I2C)
#   define INCLUDE_I2C
#endif	/* INCLUDE_RTC && !INCLUDE_I2C */

/* CAN support */

#undef INCLUDE_CAN

/* WindML support */

#undef INCLUDE_WINDML

#ifdef	INCLUDE_WINDML
#define INCLUDE_I2C
#endif	/* INCLUDE_WINDML */

/* USB host support */

#undef INCLUDE_FSL_USB_HOST

#ifdef INCLUDE_FSL_USB_HOST
#   define INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif /* INCLUDE_FSL_USB_HOST */

/* USB device support */

#undef INCLUDE_FSL_TCD

#ifdef INCLUDE_FSL_TCD
#   define INCLUDE_USB_TARG
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif  /* _ASMLANGUAGE */
#   define FSL_USB_DR_DEVICE_IO_BASE        USB0_BA
#   define FSL_USB_DR_DEVICE_INT_NUM        INUM_USB_UTMI
#   define FSL_USB_DR_DEVICE_HW_INIT        fslUsbDrDeviceHwInit
#   define FSL_USB_DR_DEVICE_BE_REG         TRUE
#   define FSL_USB_DR_DEVICE_BE_DESC        TRUE
#   define FSL_USB_DR_DEVICE_BE_SETUP       FALSE
#   define FSL_USB_DR_DEVICE_SYS_CONF       FALSE
#   define FSL_USB_DR_DEVICE_BUFFER_ES      TRUE
#   define FSL_USB_DR_DEVICE_PHY_MODE       FSL_USB_DR_PHY_UTMI16
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT

#   undef  INCLUDE_USB_CDC_SERIAL
#   ifdef INCLUDE_USB_CDC_SERIAL
#       define INCLUDE_SER_EMULATOR
#       define INCLUDE_SER_EMULATOR_INIT
#       undef  INCLUDE_MS_EMULATOR
#       undef  INCLUDE_MS_EMULATOR_INIT
#   endif /* INCLUDE_USB_CDC_SERIAL */
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif /* INCLUDE_MS_EMULATOR */

#ifdef INCLUDE_NAND_FLASH
#   ifndef INCLUDE_DOSFS
#       define INCLUDE_DOSFS
#   endif /* !INCLUDE_DOSFS */
#endif /* INCLUDE_NAND_FLASH */

/* defines for File System */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_ERF
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_XBD
#   define INCLUDE_XBD_BLK_DEV
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_MAIN
#endif /* INCLUDE_DOSFS */

/* default boot line configuration */

#define DEFAULT_BOOT_LINE   \
    "motfec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 \
    u=vxworks pw=vxworks f=0x0 tn=ads5121e"

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* defined(PRJ_BUILD) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */
