/* config.h - Freescale MDS8569 BSP configuration file */

/*
 * Copyright (c) 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,09may13,d_l  remove DRV_PCIBUS_M85XX to reduce default footprint.
                 (WIND00416916)
                 Update BSP_REV to "6.9/3"
01g,19jul12,clx  Update BSP_REV
01f,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01e,07jul11,y_c  Support VxWorks native bootrom. (WIND00240156)
01d,07apr10,pch  nvram & flash config adjustments
01c,12mar10,rec  CPU performance management update
01b,15oct09,x_z  add support for QE UART and update BSP_REV
01a,10jul09,x_z  derived from version 01j of fsl_mmds8568/config.h.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_VER_1_1                 1
#define BSP_VER_1_2                 1
#define BSP_VERSION                 "6.9"
#define BSP_REV                     "/3"

#include <configAll.h>
#include "fsl_mds8569.h"

#define SYS_MODEL                   "Freescale MDS MPC8569"
#define SYS_MODEL_8569              "Freescale MDS MPC8569"
#define SYS_MODEL_8569E             "Freescale MDS MPC8569E - Security Engine"

/* Clock Frequency */

#define OSCILLATOR_FREQ             66666666    /* 66.67MHz */
#define DEFAULT_SYSCLKFREQ          533333328   /* 533MHz */

/* Clock Rates */

#define SYS_CLK_RATE_MIN            1           /* minimum system clock rate */
#define SYS_CLK_RATE_MAX            8000        /* maximum system clock rate */
#define AUX_CLK_RATE_MIN            ((DEFAULT_SYSCLKFREQ / 16) / \
				    (1 << FIT_DIVIDER_TAP_21))
#define AUX_CLK_RATE_MAX            ((DEFAULT_SYSCLKFREQ / 16) / \
				    (1 << FIT_DIVIDER_TAP_10))
#define AUX_CLK_RATE                ((DEFAULT_SYSCLKFREQ / 16) / \
				    (1 << FIT_DIVIDER_TAP_18))

/* MMU and Cache */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_MMU_OPTIMIZE

#   define INCLUDE_AIM_MMU_SHOW

#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM      0x40
#   define AIM_MMU_INIT_PT_INCR     0x20
#   define AIM_MMU_INIT_RT_NUM      0x10
#   define AIM_MMU_INIT_RT_INCR     0x10

/* This write protects the page tables */

#   define INCLUDE_AIM_MMU_PT_PROTECTION

/*
 * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can
 * possibly be already used in sysStaticTlbDesc. Also the larger the
 * page size that can be used the less CAM entries required so aligning
 * RAM_LOW_ADRS on a large page size boundary is usually necessary.
 * Locking the text section should increase performance because no
 * Instruction TLB misses are taken within the text section.
 */

#   define INCLUDE_LOCK_TEXT_SECTION
#   undef  INCLUDE_PAGE_SIZE_OPTIMIZATION    /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */

/* L1 Cache Support */

#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE           CACHE_LIBRARY_MODE
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE           CACHE_LIBRARY_MODE

#define CACHE_LIBRARY_MODE       (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE      _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE           (MMU_ATTR_CACHE_DEFAULT | \
				     MMU_ATTR_CACHE_COHERENCY)


/* L2 Cache Mode */

#define INCLUDE_L2_CACHE

/* L2 SRAM Mode */

#undef INCLUDE_L2_SRAM

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE            L2SIZ_256KB
#   define L2_SRAM_SIZE             L2SIZ_256KB
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE            L2SIZ_512KB
#   define L2_SRAM_SIZE             0           /* Not Used */
#else	/* INCLUDE_L2_CACHE */
#   define L2_SRAM_SIZE             L2SIZ_512KB
#   define L2_CACHE_SIZE            0           /* Not Used */
#endif /* defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM) */

#define L2SRAM_ADDR                 0xE0400000
#define L2SRAM_WINDOW_SIZE          0x80000

#define INCLUDE_BRANCH_PREDICTION

/* float point */

#if (TOOL==e500v2diab) || (TOOL==e500v2gnu)

#   undef INCLUDE_GNU_INTRINSICS

/* These are 60X specific for now */

#   undef INCLUDE_HW_FP

/* e500v2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   undef  INCLUDE_HW_FP
#   define INCLUDE_SW_FP

#endif /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/* enable support for SPE 64bit registers */

#define INCLUDE_SPE

/* vxBus  */

/* common */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE             100000

/* interrupt controller */

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC

/* timer */

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_SDHC */

/* SIO */

/* QE UART is mutually exclusive with DUART */

#define DRV_SIO_NS16550
#undef  DRV_SIO_UCC

#ifdef DRV_SIO_UCC
#   define INCLUDE_ISR_DEFER
#endif /* DRV_SIO_UCC */

#define INCLUDE_SIO_UTILS

/* Serial Line Settings */

#undef  NUM_TTY

#ifdef DRV_SIO_UCC
#   define NUM_TTY                  1   /* 1 UCC SIO     */
#endif /* DRV_SIO_UCC */

#ifdef DRV_SIO_NS16550
#   define NUM_TTY                  2   /* 2 NS16550 SIO */
#endif /* DRV_SIO_NS16550 */

#undef  CONSOLE_TTY
#define CONSOLE_TTY                 1
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           9600

/* QUICC Engine */

#define DRV_INTCTLR_QE
#define INCLUDE_QUICC_ENGINE_UTILS

/* ethernet */

#define INCLUDE_UCC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY

/* PCIE */

#ifdef DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#endif	/* DRV_PCIBUS_M85XX */

/* PCI Express Outbound Windows (mapped 1-1) */

#define PCIEX_LAW_BASE              0xa0000000
#define PCIEX_LAW_SIZE              0x10000000
#define PCIEX_LAW_WIN_SZ            LAWAR_SIZE_256MB

/* window 0 */

#define PCIEX_MEM_ADRS              PCIEX_LAW_BASE
#define PCIEX_MEM_SIZE              0x04000000

/* window 1 */

#define PCIEX_MEMIO_ADRS            (PCIEX_MEM_ADRS + PCIEX_MEM_SIZE)
#define PCIEX_MEMIO_SIZE            0x04000000

/* window 2 */

#define PCIEX_IO_ADRS               (PCIEX_MEMIO_ADRS + PCIEX_MEMIO_SIZE)
#define PCIEX_IO_SIZE               0x04000000

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

/* PCI Express Inbound Windows */

/* window 0 or window 1 */

#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000  /* PCI Express address */

/* window 2 */

#define PCI_MSTR_MEM_BUS            0x00000000
#define PCI_MSTR_MEM_SIZE           0x40000000

/* Local Access Windows Configuration */

/* Memory addresses */

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_LOCAL_ADRS        0x00000000
#define LOCAL_MEM_SIZE              0x40000000  /* 1GB */
#define USER_RESERVED_MEM           0

/* LBC CS0 - 32MB 8-bit flash */

#define FLASH_BASE_ADRS             0xfe000000
#define FLASH_BASE_ADRS_TLB         0xfc000000  /* used for TLB entry */
#define FLASH_BASE_ADRS_LAW         0x000fe000  /* used for TLB LAW */
#define FLASH_SIZE                  0x02000000  /* 32MB */

/* LBC CS1 - BCSR */

#define BCSR_BASE_ADRS              0xf8000000
#define BCSR_BASE_ADRS_LAW          0x000f8000  /* used for TLB LAW */
#define BCSR_SIZE                   0x00008000  /* 32K */

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and
 * Makefile.
 * All definitions for these constants must be identical.
 */

/* Boot ROM address space. */

#define ROM_BASE_ADRS               0xFFF00000
#define ROM_TEXT_ADRS               0xFFF00100
#define ROM_WARM_ADRS               0xFFF00104
#define ROM_SIZE                    0x00100000

/* FLASH configuration */

#define	FLASH_ADRS                  FLASH_BASE_ADRS
#define	FLASH_WIDTH                 1
#define	FLASH_CHIP_WIDTH            1
#define	FLASH_SECTOR_SIZE           0x20000
#define	FLASH_SECTOR_MASK           0xFFFE0000
#define	INCLUDE_FLASH_WRITE

/* TFFS configuration */

#define	FLASH_TFFS_ADRS             FLASH_BASE_ADRS
#define	FLASH_TFFS_SIZE             0x01000000  /* 16MB */
#define	FLASH_WINDOW_SIZE           0x02000000

/* NVRAM configuration */

#define	INCLUDE_NVRAM
#define	INCLUDE_NVRAM_WRITE
#define	NV_RAM_SIZE                 FLASH_SECTOR_SIZE
#define	NV_RAM_ADRS                 (ROM_BASE_ADRS - NV_RAM_SIZE)
#define	NV_MAC_ADRS_OFFSET          0x200

/* network */

#define INCLUDE_END

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define FSL_ENET0                   0x00
#define FSL_ENET1                   0x04
#define FSL_ENET2                   0x9F

#define CUST_ENET3                  0x00
#define CUST_ENET4                  0x50
#define CUST_ENET5_0                0x3D
#define CUST_ENET5_1                0x3E
#define CUST_ENET5_2                0x3F
#define CUST_ENET5_3                0x40

#define MAX_MAC_ADRS                4
#define MAX_MAC_DEVS                1
#define MAC_ADRS_LEN                6

/* default mac address */

#define ENET_DEFAULT0               FSL_ENET0
#define ENET_DEFAULT1               FSL_ENET1
#define ENET_DEFAULT2               FSL_ENET2

/* default bootline */

#define DEFAULT_BOOT_LINE	"qefcc(0,0)host:vxWorks h=192.168.0.2 " \
				"e=192.168.0.3 u=target pw=vxTarget " \
				"f=0x0 tn=mds8569"

/* misc */

#undef INCLUDE_I2C
#undef INCLUDE_RTC

#undef INCLUDE_CPU_LIGHT_PWR_MGR

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* PRJ_BUILD */
