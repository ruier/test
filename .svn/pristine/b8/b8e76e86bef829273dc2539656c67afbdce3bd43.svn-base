/* config.h - Wind River SBC P5020 BSP configuration file */

/*
 * Copyright (c) 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,07apr13,d_l  uprev BSP_REV to "6.9/3"
01j,22feb13,d_l  uprev BSP_REV to "6.9/2"
01i,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01h,02aug12,c_l  updated BSP_REV. (WIND00360538)
01g,25jun12,y_y  add INCLUDE_DOSFS_CACHE macro. (WIND00356868)
01f,25apr12,y_y  add SATA support and misc code clean up.
01e,09apr12,h_k  updated BSP_REV for 6.9.2 UP4 release. (WIND00341977)
                 removed optional components.
                 removed redundant definitions and undefinitions.
01d,27mar12,y_y  added LED support.
01c,06mar12,y_y  add vxbus I2C support.
01b,27feb12,x_z  replaced legacy driver with vxBus driver for NOR-Flash;
                 added new Flash NVRAM interface.
01a,08nov11,y_y  created from fsl_p5020_ds config.h.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/3"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "wrSbcP5020.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS        0x00100000 - 0x00010000   /* RAM_LOW_ADRS - 64KB of stack */
#define CPUn_SPACE_SIZE             0x00000400                /* must be 2^n */
#define CPUn_SPACE_SIZE_SHIFT       10                        /* used by romInit.s */
#define CPUn_SPACE(n)               ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)      (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n)     (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)      (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)      (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)       (CPUn_SPACE(n) + 16)

/* Common Includes for VXBUS PCI and DTSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define HWMEM_POOL_SIZE             200000

/* PCI BUS */

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_QORIQ_PCIEX
#endif  /* INCLUDE_PCI_BUS */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQFMAN

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

/* TIMER */

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_EARLY_VXB_DELAY

/* DTSEC */

#define INCLUDE_END
#define DRV_VXBEND_DTSEC
#define INCLUDE_P5020_FMAN_UCODE
#define INCLUDE_DMA_SYS
#define DRV_MII_DTSEC_MDIO
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

/*
 * The macro DRV_MII_I2C_MDIO should be defined when the wrSbcP5020 board uses
 * SFP modules when JP18 removed for DTSEC0,1,2,3 ports. These must be managed
 * via I2C interface.
 * The SFP module need the Marvell 88E1111 driver for proper link management.
 */

#ifdef  DRV_MII_I2C_MDIO
#   define INCLUDE_I2C_BUS
#   define DRV_RESOURCE_FSLGPIO
#   define INCLUDE_MV88E1X11PHY
#endif  /* DRV_MII_I2C_MDIO */

/*
 * Consequently, the TGEC driver is not on by default either.
 * When it is enabled, the TGEC driver works in conjunction
 * with the DTEPHY driver for link management with miiBus.
 */

#ifdef DRV_VXBEND_TGEC
#   define DRV_MII_DTEPHY
#   define DRV_MII_TGEC_MDIO
#endif  /* DRV_VXBEND_TGEC */

#define ENET_DEFAULT0               WR_ENET0
#define ENET_DEFAULT1               WR_ENET1
#define ENET_DEFAULT2               WR_ENET2

#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE          (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE         _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE              (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE           CACHE_LIBRARY_MODE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE           CACHE_LIBRARY_MODE

#define INCLUDE_L2_CACHE
#define INCLUDE_L3_CACHE
#define L3_WRITE_THRU_MODE          0   /* 1 => write thru, 0 => write back */

#define INCLUDE_BRANCH_PREDICTION

#ifdef INCLUDE_MMU_BASIC
#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG
#endif  /* INCLUDE_MMU_BASIC */

/* Serial channel */

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/* Clock rates */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif /* INCLUDE_AUX_CLK */

#define SYS_CLK_RATE_MIN            1        /* minimum system clock rate */
#define SYS_CLK_RATE_MAX            8000     /* maximum system clock rate */
#define AUX_CLK_RATE_MIN            1        /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX            50000    /* maximum auxiliary clock rate */
#define WDT_CLK_RATE_MIN            ((DEFAULT_SYSCLKFREQ >> 3) / (1 << 26))
#define WDT_CLK_RATE_MAX            ((DEFAULT_SYSCLKFREQ >> 3) / (1 << 21))

/* I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* DMA support */

#undef DRV_DMA_FSL

/* SATA */

#ifdef DRV_STORAGE_FSLSATA
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_FSLSATA */

/* USB EHCI */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/* USB Target */

#ifdef INCLUDE_FSL_TCD
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif
#   define FSL_USB_DR_DEVICE_IO_BASE      USB2_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      EPIC_USB2_INT_VEC
#   define FSL_USB_DR_DEVICE_HW_INIT      fslUsbDrDeviceHwInit
#   define FSL_USB_DR_DEVICE_BE_REG       FALSE
#   define FSL_USB_DR_DEVICE_BE_DESC      FALSE
#   define FSL_USB_DR_DEVICE_BE_SETUP     TRUE
#   define FSL_USB_DR_DEVICE_SYS_CONF     TRUE
#   define FSL_USB_DR_DEVICE_BUFFER_ES    FALSE
#   define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_UTMI8
#   define INCLUDE_USB_TARG
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT
#endif  /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif  /* INCLUDE_MS_EMULATOR */

/* TrueFFS flash support */

#ifdef INCLUDE_TFFS

#   define INCLUDE_DOSFS
#   define INCLUDE_TFFS_MOUNT
#   define TFFS_FORMAT_PRINT /* print the tffs format process */

/* TrueFFS partition 0 */

/*
 * define to support boot region lock function if supported by corresponding
 * Flash chip
 *
 * Note: sysTffsBootUnLock() must be called before tffsBootImagePut(), and
 * sysTffsBootLock() should be called to re-lock the boot region after
 * tffsBootImagePut().
 */

#   undef  TFFS_BOOT_LOCK

#   define TFFS_PART0_NAME              "RFA0"
#   define TFFS_PART0_FLASH_NAME        "norFlash"
#   define TFFS_PART0_FLASH_UNIT        0

/* main Flash information */

#   define TFFS_PART0_FLASH_BASE_ADRS   FLASH_BASE_ADRS
#   define TFFS_PART0_FLASH_OFFSET      0           /* offset to Flash */
#   define TFFS_PART0_FLASH_SIZE        0x00800000  /* total size */
#   define TFFS_PART0_FLASH_BOOT_SIZE   0x00000000  /* reserved for bootrom */

/* TrueFFS partition 1 */

#   define TFFS_PART1_NAME              "RFA1"
#   define TFFS_PART1_FLASH_NAME        "norFlash"
#   define TFFS_PART1_FLASH_UNIT        1

/* system Flash information */

#   define FLASH2_BASE_ADRS             0xF2000000
#   define SYS_FLASH_SIZE	            0x01000000

#   define TFFS_PART1_FLASH_BASE_ADRS   FLASH2_BASE_ADRS
#   define TFFS_PART1_FLASH_OFFSET      0x00000000  /* offset to Flash */
#   define TFFS_PART1_FLASH_SIZE        0x01000000  /* total size */
#   define TFFS_PART1_FLASH_BOOT_SIZE   0           /* reserved for bootrom */
#endif /* INCLUDE_TFFS */

/*  Define for FS */

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
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_CACHE
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DISK_UTIL
#   ifdef  BOOTAPP
#      define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */
#endif  /* INCLUDE_DOSFS */

/* LBC CS0 - flash 0(16MB) - default for bootrom */

#define FLASH_BASE_ADRS             0xff000000
#define FLASH_ADRS_MASK             0xff000000
#define MAIN_FLASH_SIZE             0x01000000

/* LCD */

#undef INCLUDE_LCD

/* LED */

#define INCLUDE_SYSLED

/* NVRAM configuration */

/* EEPROM is used to save bootline by default */

#define INCLUDE_NV_RAM
#ifdef  INCLUDE_NV_RAM
#   undef  NV_RAM_ADRS
#   define NV_RAM_ADRS              EEPROM_BASE
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE              EEPROM_SIZE
#   define NV_RAM_SIZE_MASK         EEPROM_SIZE_MASK
#   define NV_RAM_INTRVL            1
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0
#   define NV_MAC_ADRS_OFFSET       0x200
#else
#   define NV_RAM_FLASH_NAME        "norFlash"
#   define NV_RAM_FLASH_UNIT        0
#   undef  NV_RAM_ADRS
#   define NV_RAM_ADRS              (ROM_BASE_ADRS - NV_RAM_SIZE)
#   define NV_RAM_OFFSET            (NV_RAM_ADRS - FLASH_BASE_ADRS)
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE              0x00020000 /* Main Flash sector size */
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0
#   define NV_MAC_ADRS_OFFSET       0x200
#endif  /* INCLUDE_NV_RAM */

/* Flash */

#if defined(INCLUDE_TFFS) || !defined(INCLUDE_NV_RAM)
#   define DRV_VXB_NORFLASH
#   define DRV_VXB_NORFLASH_CMDSET_AMD
#   define DRV_VXB_NORFLASH_CMDSET_ITL
#endif /* INCLUDE_TFFS || !INCLUDE_NV_RAM) */

/* Memory addresses */

#define PHYS_MEM_START              0x00000000

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM           0x00000000

#define DDR_SDRAM_LOCAL_END         0x7FFFFFFF
#define PHYS_MEM_SIZE               0x80000000
#define DEFAULT_LOCAL_MEM_SIZE      PHYS_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS        PHYS_MEM_START
#define DDR_SDRAM_LAWAR_SIZE        LAWAR_SIZE_2GB

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 2MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 1MB starting from BMAN_MEM_BASE.
 * Changing the MEM_BASE's will require reflashing the bootrom.
 * This memory is reserved and should not be accessed by software.
 *
 * The QMAN and BMAN are passed the address of their private memory
 * _once_ at hw init. Once passed and initialized, the private memory
 * cannot be relocated. This has the implication that downloadable
 * kernels must know and respect whatever location the bootrom has
 * used to place the private memory. For this reason Wind River chose
 * to hard-code the private memory to a fixed location, as this could
 * be most easily supported in a reference BSP which must implement
 * all possible configurations.
 *
 * vxWorks has multiple methods for allocating reserved memory. For
 * this BSP we have chosen to reserve the memory by hard-coding it
 * at a place immediately before the end of physical memory. Another
 * method would have been to place it below RAM_LOW_ADRS. Users without
 * Wind River's legacy bootrom considerations should feel free to
 * relocate and reserve the private space however best suits their
 * application.
 */

#define QMAN_BMAN_RESERVED_MEM      0x00300000
#define QMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     QMAN_BMAN_RESERVED_MEM)
#define BMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     0x00100000)

/*
 * add CDF_OVERRIDE for remove the following define when prj build, but used
 * for commandline build.
 */

#ifndef CDF_OVERRIDE
#   define DTSEC3_IN_RGMII          TRUE
#   define LOCAL_MEM_SIZE           (PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM)
#   define LOCAL_MEM_LOCAL_ADRS     PHYS_MEM_START
#endif  /* CDF_OVERRIDE */

/*
 * NEW 36-BIT CONFIGURATION MACROS.
 *
 * LOCAL_MEM_ERPN specifies the upper 4 bits of the physical address of RAM
 * used by this image.  It cannot be set to anything other than 0 for CPU0.
 * It should be configured using the project facility or vxprj.
 *
 * DDR_SDRAM_LOCAL_ADRS_H specifies the upper 4 bits of the physical address
 * of the first bank of RAM.  This bank is what is used by CPU0 and any CPUs
 * configured with a LOCAL_MEM_ERPN of 0.
 *
 * DDR_SDRAM_LOCAL_ADRS2_H specifies the upper 4 bits of the physical address
 * of the second bank of RAM.  This bank can be used by any CPU configured
 * with a LOCAL_MEM_ERPN equal to DDR_SDRAM_LOCAL_ADRS2_H.
 *
 */

#ifndef PRJ_BUILD
#   define LOCAL_MEM_ERPN           0
#endif /* PRJ_BUILD */

#define DDR_SDRAM_LOCAL_ADRS_H      0
#define DDR_SDRAM_LOCAL_ADRS2_H     2

#define DDR_SDRAM_LOCAL_ADRS2       DDR_SDRAM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_END2        DDR_SDRAM_LOCAL_END

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS

/*
* Do not clear memory in romStart() since it is not necessary on physical
* target, and slows boot time.
*/

#undef  ROMSTART_BOOT_CLEAR

#define ROM_TEXT_ADRS               0xFFF00100
#define ROM_BASE_ADRS               0xFFF00000
#define ROM_SIZE                    0x00100000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */

