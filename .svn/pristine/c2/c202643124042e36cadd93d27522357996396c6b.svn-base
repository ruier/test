/* config.h - Freescale QorIQ P5040 DS BSP configuration file */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,29jul13,d_l  Uprev BSP_REV to "6.9/2"
01c,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01b,12oct12,d_l  Uprev BSP_REV.
01a,04sep12,d_l  adapted from fsl_p5020_ds version 01k.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/2"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "p5040ds.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS    0x00100000 - 0x00010000 /* RAM_LOW_ADRS - 64KB of stack */
#define CPUn_SPACE_SIZE         0x00000400              /* must be 2^n */
#define CPUn_SPACE_SIZE_SHIFT   10                      /* used by romInit.s */
#define CPUn_SPACE(n)           ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)   (CPUn_SPACE(n) + 16)

/*
 * Errata: A-004510 (Transactions can be lost in the CoreNet coherency fabric),
 * This workaround is implemented in bootrom(romInit.s) and vxWorks
 * image(sysLib.c).
 */

#define FSL_ERRATA_CCF_A004510

/*
 * Errata: A-004511 (Transactions may be incorrectly dropped from the wait
 * queue in the CoreNet coherency fabric), This workaround is implemented
 * in bootrom(romInit.s) and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_CCF_A004511

/*
 * Workarounds for Freescale A-004529(Phantom Branch triggered by floating point
 * instruction can cause core to stall until interrupted):
 *
 * (1)Ensure that an interrupt can always be taken when executing floating point
 *    code and provide a source for such an interrupt on a periodic basis. Do
 *    not execute any floating-point instructions when interrupts are blocked.
 * (2)Disable the BTB (BUCSR[BPEN]=0) when MSR[FP] is set to 1 and enable the
 *    BTB when MSR[FP] is set to 0.
 *
 * Note: If you make sure your code meet workaround 1, you can undefine
 *       FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE for performance. Otherwise, you
 *       should define it. But, this will cause performance drop about 50%.
 *       The issue is fixed in P5040 silicon Rev 1.1. So, nothing will be done even
 *       FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE is defined for Rev 1.1 or later.
 */

#undef FREESCALE_ERRATA_A004529_WORKAROUND_ENABLE

/* Common Includes for VXBUS PCI and DTSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS

#define INCLUDE_PLB_BUS

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_QORIQ_PCIEX
#endif /* INCLUDE_PCI_BUS */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQFMAN

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_EARLY_VXB_DELAY 
#undef  DRV_TIMER_OPENPIC
#undef  INCLUDE_TIMESTAMP

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define DRV_VXBEND_DTSEC
#define INCLUDE_DMA_SYS
#define INCLUDE_P5_FMAN_UCODE
#define DRV_MII_DTSEC_MDIO
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define SGMII_RISER_CARD_IN_USE     FALSE

/*
 * add CDF_OVERRIDE for remove the following define when prj build, but used
 * for commandline build.
 */

#ifndef CDF_OVERRIDE
#   define QORIQ_PCIEX1_ENABLE      TRUE
#   define QORIQ_PCIEX2_ENABLE      FALSE
#   define QORIQ_PCIEX3_ENABLE      TRUE
#   define QORIQ_PCIEX1_UNIT_NUM    0
#   define QORIQ_PCIEX2_UNIT_NUM    1
#   define QORIQ_PCIEX3_UNIT_NUM    2
#endif /* CDF_OVERRIDE */

/*
 * The P5040 also has two 10 gigabit ethernet controllers.
 * To use them, XAUI riser cards are needed,
 * which are not included by default with the P5040DS board.
 * Consequently, the TGEC driver is not on by default either.
 *
 * When it is enabled, the TGEC driver works in conjunction
 * with the DTEPHY driver for link management with miiBus.
 * If fiber media is used, TGEC_FIBER should be defined.
 */

#ifdef DRV_VXBEND_TGEC
#   define DRV_MII_DTEPHY
#   undef  TGEC_FIBER
#   define DRV_MII_TGEC_MDIO
#endif  /* DRV_VXBEND_TGEC */

#define ENET_DEFAULT0       WR_ENET0
#define ENET_DEFAULT1       WR_ENET1
#define ENET_DEFAULT2       WR_ENET2

#define HWMEM_POOL_SIZE     200000

/*
 * This is required for SMP mode. When tNet0 is scheduled, we don't
 * have any way of predicting which core it will be running on when
 * it wakes up. But the frame manager is programmed to enqueue frames
 * to a portal belonging to a specific CPU (when a frame queue is
 * created, it's typically targeted at a particular CPU's portal).
 * We need to avoid the case where tNet0 and some other task both
 * try to dequeue from the same portal while running on different
 * cores in an SMP configuration, since then there would be
 * overlapping accesses to the same portal. To do this, we must
 * enforce the rule that if a task wants to dequeue a frame
 * descriptor from a given portal, it has to be running on the
 * CPU corresponding to that portal. To do that, we have to make
 * sure tNet0 always runs on the CPU specified as the target of
 * the DTSEC driver's frame queues, which in an UP and SMP
 * configuration defaults to 0.
 */

#ifdef _WRS_CONFIG_SMP
#   define NET_DAEMONS_CPU_AFFINITY     TRUE
#endif /* WRS_VX_SMP */

#define DEFAULT_BOOT_LINE \
"dtsec(4,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

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
#define L3_WRITE_THRU_MODE  0   /* 1 => write thru, 0 => write back */

#define INCLUDE_BRANCH_PREDICTION

#ifdef INCLUDE_MMU_BASIC
#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG
#endif /* INCLUDE_MMU_BASIC */

/* Serial channel and TTY */

#undef  NUM_TTY
#define NUM_TTY                     4
#undef  CONSOLE_TTY
#define CONSOLE_TTY                 0
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           115200

/* Clock rates */

#ifdef  INCLUDE_AUX_CLK
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
#   define DRV_I2C_RTC
#   define INCLUDE_TIMER_RTC
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* SPI support */

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* DMA support */

#undef DRV_DMA_FSL

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
#endif /* INCLUDE_MS_EMULATOR */

/* SATA */

#ifdef DRV_STORAGE_FSLSATA
#   define INCLUDE_DOSFS
#   define INCLUDE_DOSFS_CACHE
#   undef  DOSFS_DEFAULT_DATA_DIR_CACHE_SIZE
#   undef  DOSFS_DEFAULT_FAT_CACHE_SIZE
#   define DOSFS_DEFAULT_DATA_DIR_CACHE_SIZE           0x8000000
#   define DOSFS_DEFAULT_FAT_CACHE_SIZE                0x1000000
#endif /* DRV_STORAGE_FSLSATA */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

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
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DISK_UTIL
#   ifdef  BOOTAPP
#      define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */
#endif  /* INCLUDE_DOSFS */

#define FLASH_WINDOW_SIZE           0x00800000

/* LBC CS3 - for PIXIS interface (romInit.s) */
#define INCLUDE_LBC_CS3

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS             0xff000000
#define FLASH_ADRS_MASK             0xff000000
#define MAIN_FLASH_SIZE             0x01000000

/* NVRAM configuration */

/* EEPROM is used to save bootline by default */

#define INCLUDE_FLASH
#ifdef INCLUDE_FLASH
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
#endif  /* INCLUDE_FLASH */

/* Flash */

#if defined(INCLUDE_TFFS) || !defined(INCLUDE_NV_RAM)
#   define DRV_VXB_NORFLASH
#   define DRV_VXB_NORFLASH_CMDSET_AMD
#endif /* INCLUDE_TFFS || !INCLUDE_NV_RAM) */

/* Memory addresses */

#define PHYS_MEM_START              0x00000000
#define USER_RESERVED_MEM           0x00000000
#define DDR_SDRAM_LOCAL_END         0x7fffffff
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
 * CDF_OVERRIDE is automatically defined for vxprj builds, ensuring
 * LOCAL_MEM_SIZE will be referenced from the 20bsp.cdf instead.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE           (PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM)
#   define LOCAL_MEM_LOCAL_ADRS     PHYS_MEM_START
#endif /* CDF_OVERRIDE */

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

#undef  ROMSTART_BOOT_CLEAR   /* see romInit.s */

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
