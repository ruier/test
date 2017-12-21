/* config.h - Freescale QorIQ T4240 QDS BSP configuration file */

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
02j,27jun13,j_z  uprev BSP_REV to "6.9/1".
02i,09apr13,x_s  undefined L2&L3 cache for BOOTAPP by default.
02h,17mar13,syt  added definition ROM_TLB_SZ.
02g,11mar13,syt  added errata DDR A004390 and removed macro DRV_SPD_FSL.
02f,01mar13,h_k  apply workaround for errata CPU A004806 only in SMT.
02e,08feb13,syt  added DDR DIMM SPD support.
02d,06feb13,wyt  removed Simics definition.
02c,06feb13,x_s  excluded L2&L3 cache definition for non-primary AMP.
02b,05feb13,wyt  added support for SATA.
02a,27jan13,y_y  added eSPI controller and SPI FLASH TFFS support. 
01z,22jan13,h_k  fixed harmless linker warning at bootrom_uncmp build.
                 (CQ:WIND00400206)
01y,22jan13,x_s  removed the NVRAM offset definitions replaced by configAll.h.
                 added definition ROM_WARM_ADRS.
01x,21jan13,x_z  added INCLUDE_NVRAM_FLASH for NVRAM;
                 added INCLUDE_TFFS_STUB_VXBFLASH for new TFFS stub.
01w,17jan13,h_k  added FSL_ERRATA_CPU_A004792 and 4809.
01v,17jan13,wap  Add support for alternate SERDES1/SERDES2 layout
01u,16jan13,zl   added CPUn_PHYS_ID, CPUn_SPACE_START_ADDR
01t,16jan13,wap  Add support for Vitesse VSC3316 crossbar switch chips
01s,10jan13,wyt  added macro definition INCLUDE_USR_MACADDR
01r,06jan13,x_s  added macro definition INCLUDE_USB_PCI_STUB_CUSTOM for USB.
01q,07dec12,h_k  corrected comments for 40bit phys mem support.
                 added FSL_ERRATA_DDR_A004934.
                 enabled branch prediction by default.
01p,04dec12,h_k  added FSL_ERRATA_CPU_A004779, 4857, 5553.
                 changed the default to actual HW.
01o,30nov12,x_s  added component INCLUDE_CORE_E6500 by default.
01n,21nov12,h_k  enabled L2 and L3 caches for actual HW.
                 disabled INCLUDE_CPU_LIGHT_PWR_MGR.
                 added errata workarounds for CPU_A004786 and CPU_A004806.
01m,12nov12,syt  used the new vxbus mode driver to support NVRAM.
01l,08nov12,h_k  removed L2 and L3 cache and cache parity error recovery
                 components from the default configuration.
                 re-enabled SIMICS macro.
01k,01nov12,syt  added I2C support.
01j,07nov12,syt  added TureFFS support.
01i,30oct12,h_k  added INCLUDE_DDR_ECC. updated SDRAM physical addresses.
                 added missing DRV_MII_QORIQMEMAC_MDIO to non-SIMICS.
                 disabled L2 and L3 caches.
01h,19oct12,syt  added DMA support.
01g,15oct12,syt  added SDHC support.
01f,15oct12,wap  Unbreak physical board configuration -- SIMICS and UBOOT
                 macros _must_ be set before any header files are included
01e,09oct12,syt  code clean up.
01d,19sep12,h_k  added UBOOT config.
                 removed INCLUDE_TIMER_STUB and INCLUDE_VXBUS_SHOW.
                 changed the default memac unit number to 4 to match to the
                 actual hardware.
01c,14sep12,h_k  removed TGEC from a comment.
01b,15aug12,wap  Update for new MEMAC driver
01a,07may12,h_k  adapted from fsl_p5020_ds version 01o for t4240 Simics.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/1"

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include <arch/ppc/mmuE500Lib.h>
#include <vsbConfig.h>
#include <configAll.h>

#define INCLUDE_DDR_ECC

/*
 * Apply the workaround for errata CPU A004779:
 * Misaligned bank collisions in the L1 Data Cache may cause a hang.
 *
 * Workaround: Follow all of these steps.
 * 1. Set bit 32 of SPR978 to 1.
 * 2. Set bits 33:39 of SPR978 to 0b0000011.
 * 3. Set bits 41:47 of SPR978 to 0b0000011.
 */

# define FSL_ERRATA_CPU_A004779

/*
 * Apply the workaround for errata CPU A004786:
 * Speculative lbarx, lharx, lwarx, ldarx are not supported on e6500 Rev1.
 *
 * Workaround:
 * Set bit 32 of SPR631 to 1.
 */

# define FSL_ERRATA_CPU_A004786

/*
 * Apply the workaround for errata CPU A004792:
 * Lightweight memory barriers including lightweight sync, eieio, and elemental
 * barriers may not perform the intended synchronization, resulting in
 * issues.
 *
 * Workaround:
 * Set bit 49 of SPR 976 to 1.
 */

# define FSL_ERRATA_CPU_A004792

/*
 * Apply the workaround for errata CPU A004809:
 * Instruction Line Fill Buffer entries that are canceled because of a change
 * in flow can be overwritten with a new demand fetch. If instruction fetch is
 * aliasing one EA to multiple RAs, the new fetch may incorrectly complete from
 * the old context.
 *
 * Workaround:
 * 1. Set bit 39 of SPR 976 to 1. This will disable instruction prefetch.
 * 2. Set bit 48 of SPR 976 to 1. This will prevent new fetches from
 *    overwriting a canceled nstruction Line Fill Buffer entry until
 *    the previous entry has been loaded into the Instruction Cache.
 */

# define FSL_ERRATA_CPU_A004809

/*
 * Apply the workaround for errata DDR A004390:
 * DDR violates tXPR for higher density DRAMs on the Rev 1.x.
 *
 * Workaround:
 * The following should be written before setting DDR_SDRAM_CFG[MEM_EN] and
 * after writing all other DDR configuration registers.
 * 1. Write DDR_SDRAM_MD_CNTL[CKE_CNTL] to a value of 2'b10.
 * 2. Wait at least tXPR for the memory used. Set DDR_SDRAM_CFG[MEM_EN].
 * 3. DDR_SDRAM_MD_CNTL[CKE_CNTL] may be cleared to allow hardware to
 *    automatically control the MCKE pins.
 */

# define FSL_ERRATA_DDR_A004390

/*
 * Apply the workaround for errata DDR A004934:
 * DDR controller may calibrate to non-optimal operating point.
 *
 * Workaround:
 * Before enabling the DDR controller, set the DDR memory mapped register
 * space at offset 0xF70 to a value of 0x30003000.
 */

#  define FSL_ERRATA_DDR_A004934

#include "t4240qds.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software. The space 
 * "grows down" from the base address.
 *
 * The base address (0x00100000) is used here as an absolute address 
 * for all OS instances - SMP, AMP and non-AMP. It has the same value as 
 * the bootup CPU's RAM_LOW_ADRS.
 *  
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS    (0x00100000 - 0x00010000)
#define CPUn_SPACE_SIZE_SHIFT   10			     /* 1k per CPU */
#define CPUn_SPACE_SIZE         (1 << CPUn_SPACE_SIZE_SHIFT) /* must be 2^n */
#define CPUn_SPACE(n)           (CPUn_SPACE_BASE_ADRS - \
				 ((n) << CPUn_SPACE_SIZE_SHIFT))
#define CPUn_SPACE_COUNT	MAX_CPUS
#define CPUn_SPACE_TOTAL_SIZE	(CPUn_SPACE_COUNT * CPUn_SPACE_SIZE)
#define CPUn_SPACE_START_ADDR	(CPUn_SPACE_BASE_ADRS - CPUn_SPACE_TOTAL_SIZE)

#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)   (CPUn_SPACE(n) + 16)
#define CPUn_PHYS_ID(n)		(CPUn_SPACE(n) + 20)


#define INCLUDE_CORE_E6500

/* VxBus */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_DMA_SYS
#define INCLUDE_PLB_BUS

#ifdef	_WRS_CONFIG_SMP
# define HWMEM_POOL_SIZE	200000
#else
# define HWMEM_POOL_SIZE	100000
#endif	/* _WRS_CONFIG_SMP */

/* Intterrupt controller */

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

/* Timer */

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX

/* SOC Resource */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQFMAN

/* SIO */

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/* PCI bus */

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_QORIQ_PCIEX
#endif /* INCLUDE_PCI_BUS */

/* SATA */

#undef  DRV_STORAGE_FSLSATA

/* eSDHC */

#undef  DRV_STORAGE_SDHC

/* USB */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_USB_MS_CBI
#   define INCLUDE_USB_MS_CBI_INIT
#   define INCLUDE_USB_PCI_STUB_CUSTOM
#endif  /* INCLUDE_USB */

/* DMA */

#undef  DRV_DMA_FSL

/* End */

/*
 * The default SERDES configuration matches the rcw_1_28_6_12_1666MHz.bin
 * configuration included with the Freescale QorIQ SDK version 1.3.1.
 */

#define SERDES1_XAUI
#define SERDES2_SGMII

#define DRV_VXBEND_QORIQMEMAC
#define DRV_MII_QORIQMEMAC_MDIO
#define INCLUDE_T4240_FMAN_UCODE

/*
 * The SGMII and XAUI memac interfaces require the Vitesse VSC3316
 * crossbar switch support.
 */

#ifdef DRV_VXBEND_QORIQMEMAC
#define INCLUDE_I2C_BUS
#define DRV_I2C_VSC3316
#endif

#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define DRV_MII_DTEPHY
#define INCLUDE_END

#if defined(DRV_VXBEND_QORIQMEMAC) || defined(INCLUDE_BOOT_ETH_MAC_HANDLER)
#   define INCLUDE_USR_MACADDR
#endif /* DRV_VXBEND_QORIQMEMAC || INCLUDE_BOOT_ETH_MAC_HANDLER */

#define MAX_MAC_DEVS                1   /* one network device type (memac) */

/* Default mac address */

#define ENET_DEFAULT0               WR_ENET0
#define ENET_DEFAULT1               WR_ENET1
#define ENET_DEFAULT2               WR_ENET2

/* I2C */

#ifdef  INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_GENERIC_DEV
#endif /* INCLUDE_I2C_BUS */

/* I2C RTC device */

#ifdef DRV_I2C_RTC
#   define  INCLUDE_TIMER_RTC
#endif /* DRV_I2C_RTC */

/* I2C EEPROM device */

#ifdef DRV_I2C_EEPROM
#   define  INCLUDE_EEPROMDRV
#   define  INCLUDE_TIMER_STUB
#endif /* DRV_I2C_EEPROM */

#define DEFAULT_BOOT_LINE \
"memac(4,0)host:vxWorks h=10.10.0.1 e=10.10.0.2 u=vxworks pw=vxworks f=0x0"

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

#ifndef INCLUDE_BOOT_APP
#   if !defined(INCLUDE_AMP) || (defined(INCLUDE_AMP) && defined(INCLUDE_AMP_CPU_00))
#      define INCLUDE_L2_CACHE
#      define INCLUDE_L3_CACHE
#   endif /* !INCLUDE_AMP || (INCLUDE_AMP && INCLUDE_AMP_CPU_00)*/
#endif /* !INCLUDE_BOOT_APP */

#if defined(INCLUDE_L3_CACHE) && !defined(INCLUDE_L2_CACHE)

/* L2 cache must be enabled if L3 cache is enabled */

#   define INCLUDE_L2_CACHE
#endif  /* INCLUDE_L3_CACHE && !INCLUDE_L2_CACHE */

#define L3_WRITE_THRU_MODE  0   /* 1 => write thru, 0 => write back */

/* Power Management has not been tested. */

#undef	INCLUDE_CPU_LIGHT_PWR_MGR

/* enable branch prediction */

#define	INCLUDE_BRANCH_PREDICTION

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

/* Flash */

/* IFC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS             0xff000000
#define FLASH_SIZE                  0x01000000

/* NVRAM to Flash mapping */

#define INCLUDE_NVRAM_FLASH
#ifdef INCLUDE_NVRAM_FLASH
#   define NV_RAM_FLASH_NAME        "norFlash"
#   define NV_RAM_FLASH_UNIT        0
#   undef  NV_RAM_ADRS
#   define NV_RAM_ADRS              (ROM_BASE_ADRS - NV_RAM_SIZE)
#   define NV_RAM_OFFSET            (NV_RAM_ADRS - FLASH_BASE_ADRS)
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE              0x00020000 /* Flash sector size */
#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              0
#endif  /* INCLUDE_NVRAM_FLASH */

/* SPI support */

#ifdef DRV_SPIFLASH_SP25
#   define DRV_ESPI_FSL
#   define INCLUDE_SPI_BUS
#endif  /* DRV_SPI_SPS25 */

/* TrueFFS */

#ifdef INCLUDE_TFFS

/* TFFS stub to vxBus Flash interface */

#   define INCLUDE_TFFS_STUB_VXBFLASH

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

#   define TFFS_PART0_NAME          "RFA0"
#   define TFFS_PART0_FLASH_NAME    "norFlash"
#   define TFFS_PART0_FLASH_UNIT    0

/* main Flash information */

#   define TFFS_PART0_FLASH_BASE_ADRS   FLASH_BASE_ADRS
#   define TFFS_PART0_FLASH_OFFSET      0x00000000  /* offset to Flash */
#   define TFFS_PART0_FLASH_SIZE        0x00800000  /* total size */
#   define TFFS_PART0_FLASH_BOOT_SIZE   0x00000000  /* reserved for bootrom */

/* TrueFFS partition 1 */

#   ifdef DRV_SPIFLASH_SP25
#      define TFFS_PART1_NAME              "RFA1"
#      define TFFS_PART1_FLASH_NAME        "spiFlash_sp25probe"
#      define TFFS_PART1_FLASH_UNIT        0
#      define TFFS_PART1_FLASH_BASE_ADRS   0
#      define TFFS_PART1_FLASH_OFFSET      0
#      define TFFS_PART1_FLASH_BOOT_SIZE   0
#      define TFFS_PART1_FLASH_SIZE        0x80000   /* total size (512KB) */
#   endif /* DRV_SPIFLASH_SP25 */
#endif /* INCLUDE_TFFS */

#if defined(INCLUDE_NVRAM_FLASH) || defined(INCLUDE_TFFS_STUB_VXBFLASH)
#   define DRV_VXB_NORFLASH
#   define DRV_VXB_NORFLASH_CMDSET_AMD
#endif /* INCLUDE_NVRAM_FLASH || INCLUDE_TFFS_STUB_VXBFLASH */

/* Memory addresses */

#define PHYS_MEM_START              0x00000000
#define PHYS_MEM_SIZE               0x80000000

#define DDR_SDRAM_LAWAR_SIZE        LAWAR_SIZE_2GB
#define DDR_SDRAM_LOCAL_ADRS        PHYS_MEM_START
#define DDR_SDRAM_LOCAL_END         (PHYS_MEM_START + 0x7fffffff)
#define DDR_SDRAM_LOCAL_ADRS2       0x00000000
#define DDR_SDRAM_LOCAL_END2        0x7fffffff
#define DDR_SDRAM_LOCAL_ADRS3       0x00000000
#define DDR_SDRAM_LOCAL_END3        0x7fffffff

/*
 * 40-BIT CONFIGURATION MACROS
 *
 * LOCAL_MEM_ERPN specifies the upper 8 bits of the physical address of RAM
 * used by this image.  It cannot be set to anything other than 0 for CPU0.
 * It should be configured using the project facility or vxprj.
 *
 * DDR_SDRAM_LOCAL_ADRS_H specifies the upper 8 bits of the physical address
 * of the first bank of RAM.  This bank is what is used by CPU0 and any CPUs
 * configured with a LOCAL_MEM_ERPN of 0.
 *
 * DDR_SDRAM_LOCAL_ADRS2_H specifies the upper 8 bits of the physical address
 * of the second bank of RAM.  This bank can be used by any CPU configured
 * with a LOCAL_MEM_ERPN equal to DDR_SDRAM_LOCAL_ADRS2_H.
 *
 * DDR_SDRAM_LOCAL_ADRS3_H specifies the upper 8 bits of the physical address
 * of the second bank of RAM.  This bank can be used by any CPU configured
 * with a LOCAL_MEM_ERPN equal to DDR_SDRAM_LOCAL_ADRS3_H.
 */

#define _LOCAL_MEM_ERPN             0x00

#define DDR_SDRAM_LOCAL_ADRS_H      0x00
#define DDR_SDRAM_LOCAL_ADRS2_H     0x01
#define DDR_SDRAM_LOCAL_ADRS3_H     0x02

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM           0x00000000

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 8MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 4MB starting from BMAN_MEM_BASE.
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

#define QMAN_MEM_SIZE               0x00800000
#define BMAN_MEM_SIZE               0x00400000
#define QMAN_BMAN_RESERVED_MEM      (QMAN_MEM_SIZE + BMAN_MEM_SIZE)
#define QMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     QMAN_BMAN_RESERVED_MEM)
#define BMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     BMAN_MEM_SIZE)

/*
 * CDF_OVERRIDE is automatically defined for vxprj builds, ensuring
 * LOCAL_MEM_SIZE will be referenced from the 20bsp.cdf instead.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE           (PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM)
#   define LOCAL_MEM_LOCAL_ADRS     PHYS_MEM_START
#endif /* CDF_OVERRIDE */

/*
 * If upper memory half is in 40-bit address range, redefine the lower
 * 32-bits to match the lower half.  For example,
 * 2 GB at 0x0000000000 - 0x007fffffff
 * 2 GB at 0x0200000000 - 0x027fffffff
 *    (rather than 2 GB at 0x0280000000 - 0x02ffffffff)
 *
 * This example is the default configuration.
 */

#if DDR_SDRAM_LOCAL_ADRS2_H
#   undef  DDR_SDRAM_LOCAL_ADRS2
#   define DDR_SDRAM_LOCAL_ADRS2    DDR_SDRAM_LOCAL_ADRS
#   undef  DDR_SDRAM_LOCAL_END2
#   define DDR_SDRAM_LOCAL_END2     DDR_SDRAM_LOCAL_END
#endif /* DDR_SDRAM_LOCAL_ADRS2_H */

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS

/*
* Do not clear memory in romStart() since it is not necessary on physical
* target, and slows boot time. But when runs on Simics module, not clear
* memory will lead image loading failed, it could be caused by DDR device
* not be simulated perfectly on Simics module. So macro ROMSTART_BOOT_CLEAR
* should be defined on Simics T4240QDS module.
*/

#undef  ROMSTART_BOOT_CLEAR   /* see romInit.s */

#define ROM_TEXT_ADRS               0xFFF00100
#define ROM_BASE_ADRS               0xFFF00000
#define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 0x0004)
#define ROM_SIZE                    0x00100000

/* 
 * WARNING: The bootrom base address (ROM_BASE_ADRS) must be modulo
 * RAM_TLB_SZ size. 
 */

#if (ROM_SIZE == 0x00100000)
#   define ROM_TLB_SZ                _MMU_TLB_SZ_1M
#elif (ROM_SIZE == 0x00200000)
#   define ROM_TLB_SZ                _MMU_TLB_SZ_2M
#elif (ROM_SIZE == 0x00400000)
#   define ROM_TLB_SZ                _MMU_TLB_SZ_4M
#elif (ROM_SIZE == 0x00800000)
#   define ROM_TLB_SZ                _MMU_TLB_SZ_8M
#elif (ROM_SIZE == 0x01000000)
#   define ROM_TLB_SZ                _MMU_TLB_SZ_16M
#else
#error "Currently ROM_SIZE only supports 1, 2, 4, 8, or 16MB sizes."
#endif

/* Altivec coprocessor support */

#define INCLUDE_ALTIVEC

#ifdef __cplusplus
    }
#endif

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
# include "prjParams.h"

# ifdef	LOCAL_MEM_ERPN
/*
 * Workaround to set the default LOCAL_MEM_ERPN defined in 20bsp.cdf per AMP
 * configuration at VIP creation.
 */

#  undef _LOCAL_MEM_ERPN
#  define _LOCAL_MEM_ERPN	LOCAL_MEM_ERPN
# endif	/* LOCAL_MEM_ERPN */
#endif /* PRJ_BUILD */

# ifdef	INCLUDE_SMT
/*
 * Apply the workaround for errata CPU A004806:
 * Load data returned from the system may be observed in a different order by
 * threads on the same core.
 *
 * Workaround:
 * Set bit 34 of SPR631 to 1.
 */

#  define FSL_ERRATA_CPU_A004806
# endif	/* INCLUDE_SMT */

#if	defined(INCLUDE_L2_CACHE) && \
	(defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP))
/*
 * Apply the workaround for errata CPU A004857:
 * Shared L2 cache may drop a reload to the core when certain snoops hit on
 * a victim castout.
 *
 * Workaround:
 * Set bits 40:41 of L2 cache register offset 0xf04 to 0b01. This workaround
 * option may have a performance impact.
 */

# define FSL_ERRATA_CPU_A004857

/*
 * Apply the workaround for errata CPU A005553:
 * A non-stcx store from one core on a cluster to the same coherency granule
 * used as a reservation by a different core on the same cluster may not kill
 * the reservation.
 *
 * Workaround:
 * 1. Set the L2 cache register offset 0xf04 bit 54 to 1'b1 to disable CoreNet
 *    reordering from different sources.
 * 2. Set the L2 cache register offset 0xf00 bit 41 to 1'b1 to disable
 *    the cluster-level store merging.
 */

# define FSL_ERRATA_CPU_A005553
#endif

