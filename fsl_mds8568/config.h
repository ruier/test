/* config.h - Freescale MDS8568 BSP configuration file */

/*
 * Copyright (c) 2007-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,19jul12,clx  Update BSP_REV
01t,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01s,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01r,16dec11,d_c  revert version - BSP not released as planned
01q,14dec11,swu  Update BSP version for vxWorks 6.9.2
01p,30aug11,swu  Update BSP version for vxWorks 6.9.1
01o,07apr10,pch  nvram & flash config adjustments
01n,12mar10,rec  CPU performance management update
01m,15apr10,clx  Update BSP version for vxWorks 6.9
01l,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01k,06mar09,syt  increase BSP_REV to "/2".
01j,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01i,24jul08,b_m  switch to use new vxbus etsec and ucc driver.
01h,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
01g,23aug07,mmi  remove DEFAULT_POWER_MGT_MODE
01f,30jul07,b_m  add INCLUDE_VXB_CMDLINE macro.
01e,26jul07,b_m  add m85xxTimer vxbus driver support.
01d,18jul07,b_m  add epic/qe interrupt controller vxbus driver support.
01c,12jul07,b_m  add m85xxPci vxbus driver support.
01b,16may07,b_m  add BOOT_FROM_UBOOT support.
01a,10may07,b_m  derived from version 01g of wrSbc8548/config.h.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/3"

#include <configAll.h>
#include "fsl_mds8568.h"

#define SYS_MODEL           "Freescale MDS"
#define SYS_MODEL_8568E     "Freescale MDS MPC8568E - Security Engine"
#define SYS_MODEL_8568      "Freescale MDS MPC8568"
#define SYS_MODEL_8567E     "Freescale MDS MPC8567E - Security Engine"
#define SYS_MODEL_8567      "Freescale MDS MPC8567"
#define SYS_MODEL_E500      "Freescale e500: Unknown System Version"
#define SYS_MODEL_UNKNOWN   "Freescale Unknown Processor"

/* Clock Frequency */

#define OSCILLATOR_FREQ     66000000    /* 66MHz */
#define DEFAULT_SYSCLKFREQ  396000000   /* 396MHz */

/* Clock Rates */

#define SYS_CLK_RATE_MIN    1           /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000        /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    ((DEFAULT_SYSCLKFREQ / 16) / (1 << FIT_DIVIDER_TAP_21))
#define AUX_CLK_RATE_MAX    ((DEFAULT_SYSCLKFREQ / 16) / (1 << FIT_DIVIDER_TAP_10))
#define AUX_CLK_RATE        ((DEFAULT_SYSCLKFREQ / 16) / (1 << FIT_DIVIDER_TAP_18))

/* MMU and Cache */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

/* L1 Cache Support */

#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE   CACHE_LIBRARY_MODE
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE   CACHE_LIBRARY_MODE

#define CACHE_LIBRARY_MODE       (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE      _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE           MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_CACHE_COHERENCY

#define INCLUDE_BRANCH_PREDICTION

/* L2 Cache Mode */

#define INCLUDE_L2_CACHE

/* L2 SRAM Mode */

#undef   INCLUDE_L2_SRAM

/* for MPC8548 rev.2 silicon, here only for L2 cache on MPC8568 */

#define REV2_SILICON

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_256KB
#   define L2_SRAM_SIZE         L2SIZ_256KB
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_512KB
#   define L2_SRAM_SIZE         0           /* Not Used */
#else	/* INCLUDE_L2_CACHE */
#   define L2_SRAM_SIZE         L2SIZ_512KB
#   define L2_CACHE_SIZE        0           /* Not Used */
#endif	/* INCLUDE_L2_CACHE */

#define L2SRAM_ADDR             0x7FF80000
#define L2SRAM_WINDOW_SIZE      0x80000

/* disable support for SPE 64bit registers */

#define INCLUDE_SPE

/* Serial Line Settings */

#undef  NUM_TTY
#define NUM_TTY                 1
#undef  CONSOLE_TTY
#define CONSOLE_TTY             0
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#define N_SIO_CHANNELS          2

/* PCI Express address */

#define PCI_MEM_SIZE            0x04000000
#define PCI_MEMIO_SIZE          0x04000000
#define PCI_IO_SIZE             0x04000000

#define PCI_LAW_BASE            0x80000000
#define PCI_LAW_SIZE            0x10000000
#define PCI_LAW_WIN_SZ          LAWAR_SIZE_256MB

#define PCI_MEM_ADRS            (PCI_LAW_BASE)
#define PCI_MEMIO_ADRS          (PCI_MEM_ADRS   + PCI_MEM_SIZE)
#define PCI_IO_ADRS             (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)

#define PCIEX_LAW_BASE          0xa0000000
#define PCIEX_LAW_SIZE          0x10000000
#define PCIEX_LAW_WIN_SZ        LAWAR_SIZE_256MB

#define PCIEX_MEM_ADRS          (PCIEX_LAW_BASE)
#define PCIEX_MEMIO_ADRS        (PCIEX_MEM_ADRS   + PCI_MEM_SIZE)
#define PCIEX_IO_ADRS           (PCIEX_MEMIO_ADRS + PCI_MEMIO_SIZE)

#define PCI_MMU_TLB_SZ          _MMU_TLB_SZ_256M

#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* CPU address */

#define PCI_MSTR_MEM_BUS        0x00000000
#define PCI_MSTR_MEM_SIZE       0x40000000

/* default using vxbus drivers */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE         50000
#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_UCC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#undef  DRV_PCIBUS_M85XX

#ifdef INCLUDE_UCC_VXB_END
#   define DRV_INTCTLR_QE
#   define INCLUDE_QUICC_ENGINE_UTILS
#if !defined(INCLUDE_ETSEC_VXB_END) && !defined(INCLUDE_ETSEC_VXB_END2)
#   define INCLUDE_TSEC_MDIO
#endif	/* !INCLUDE_ETSEC_VXB_END && !INCLUDE_ETSEC_VXB_END2 */
#endif	/* INCLUDE_UCC_VXB_END */

#ifdef DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#endif	/* DRV_PCIBUS_M85XX */

/* Local Access Windows Configuration */

/* Memory addresses */

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_LOCAL_ADRS        0x00000000
#define LOCAL_MEM_SIZE              0x20000000  /* 512MB */

#define DDR_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE

/* LBC CS0 - 32MB 16-bit flash */

#define FLASH_BASE_ADRS             0xfe000000
#define FLASH_SIZE                  0x02000000  /* 32MB */

/* LBC CS1 - BCSR */

#define BCSR_BASE_ADRS              0xf8000000
#define BCSR_SIZE                   0x00008000  /* 32K */

/* LBC CS2 - SDRAM */

#define INCLUDE_LBC_SDRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_LOCAL_ADRS2       0xf0000000
#define LOCAL_MEM_SIZE2             0x04000000  /* 64MB */

#define LBC_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE2

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_MMU_OPTIMIZE

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
#   undef INCLUDE_PAGE_SIZE_OPTIMIZATION    /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */

#if (TOOL==e500v2diab) || (TOOL==e500v2gnu)

#   undef INCLUDE_GNU_INTRINSICS

/* These are 60X specific for now */

#   undef INCLUDE_HW_FP

/* e500v2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else	/* TOOL==e500v2... */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   undef  INCLUDE_HW_FP
#   define INCLUDE_SW_FP

#endif /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/* define if U-Boot boots us */

#undef BOOT_FROM_UBOOT

#ifndef BOOT_FROM_UBOOT
#   define ROM_TEXT_ADRS    0xFFF00100
#   define ROM_BASE_ADRS    0xFFF00000
#else	/* BOOT_FROM_UBOOT */
#   define ROM_TEXT_ADRS    0xFFE00100
#   define ROM_BASE_ADRS    0xFFE00000
#endif	/* BOOT_FROM_UBOOT */

#define ROM_SIZE            0x00100000

#define USER_RESERVED_MEM   0

/* NVRAM configuration */

#define	INCLUDE_NVRAM
#define	INCLUDE_NVRAM_WRITE
#define	INCLUDE_FLASH_WRITE

#define	SYS_FLASH_TYPE      FLASH_29GL256N
#define	FLASH_WIDTH         2
#define	FLASH_CHIP_WIDTH    2
#define	FLASH_SECTOR_SIZE   0x20000     /* sector size of S29GL256N */
#define	FLASH_ADRS          0xFE000000

#undef	FLASH_NO_OVERLAY
#undef	FLASH_SIZE_WRITEABLE

#define	NV_RAM_SIZE         FLASH_SECTOR_SIZE

#define ROM_CODE_OFFSET     0x100
#ifdef BOOT_FROM_UBOOT
#   define UBOOT_TEXT_BASE  0xFFF80000
#   define NV_RAM_ADRS      (UBOOT_TEXT_BASE - NV_RAM_SIZE)
#else	/* BOOT_FROM_UBOOT */
#   define NV_RAM_ADRS      ((ROM_TEXT_ADRS - ROM_CODE_OFFSET) - NV_RAM_SIZE)
#endif	/* BOOT_FROM_UBOOT */

#define NV_MAC_ADRS_OFFSET  0x200

#define INCLUDE_END

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define FSL_ENET0           0x00
#define FSL_ENET1           0x04
#define FSL_ENET2           0x9F

#define CUST_ENET3          0x00
#define CUST_ENET4          0x37
#define CUST_ENET5_0        0xEB
#define CUST_ENET5_1        0xEC
#define CUST_ENET5_2        0xED
#define CUST_ENET5_3        0xEE

#define MAX_MAC_ADRS        4
#define MAX_MAC_DEVS        2
#define MAC_ADRS_LEN        6

/* default mac address */

#define ENET_DEFAULT0       FSL_ENET0
#define ENET_DEFAULT1       FSL_ENET1
#define ENET_DEFAULT2       FSL_ENET2

/* default bootline */

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.2 e=192.168.0.3 \
u=vxworks pw=vxworks f=0x0 tn=mds8568"

#undef INCLUDE_CPU_LIGHT_PWR_MGR

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* PRJ_BUILD */
