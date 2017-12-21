/* config.h - Freescale MPC8536DS BSP configuration file */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,19jul12,clx  Update BSP_REV
01h,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01g,10mar11,my_  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
01f,02apr11,y_c  Support VxWorks native bootrom. (WIND00240155)
01e,05jun10,sye  Add support to eSDHC.
01d,12mar10,rec  CPU performance management update
01c,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01b,10feb09,b_m  BSP_REV bump up.
01a,20oct08,b_m  derived from version 01h of fsl_mds8568/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_VER_1_1         1
#define BSP_VER_1_2         1
#define BSP_VERSION         "6.9"
#define BSP_REV             "/2"

#include <configAll.h>
#include "fsl_mpc8536_ds.h"

#define SYS_MODEL           "Freescale MPC8536DS"
#define SYS_MODEL_SEC       "Freescale MPC8536DS - Security Engine"

/* MMU and Cache */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#ifdef  INCLUDE_MMU_BASIC
#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG
#   define INCLUDE_AIM_MMU_PT_PROTECTION
#   define INCLUDE_MMU_OPTIMIZE
#   define INCLUDE_LOCK_TEXT_SECTION
#   undef  INCLUDE_PAGE_SIZE_OPTIMIZATION   /* Not available for 85XX */
#endif  /* INCLUDE_MMU_BASIC */

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
#define TLB_CACHE_MODE           (MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_CACHE_COHERENCY)

#define INCLUDE_BRANCH_PREDICTION

/* L2 Cache Mode */

#define INCLUDE_L2_CACHE
#define REV2_SILICON            /* L2 Cache Rev.2 */

/* L2 SRAM Mode */

#undef  INCLUDE_L2_SRAM
#define L2SRAM_ADDR             0x7FF80000
#define L2SRAM_WINDOW_SIZE      0x80000

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_256KB
#   define L2_SRAM_SIZE         L2SIZ_256KB
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_512KB
#   define L2_SRAM_SIZE         0
#else
#   define L2_CACHE_SIZE        0
#   define L2_SRAM_SIZE         L2SIZ_512KB
#endif

/*
 * Support for SPE 64-bit registers.
 * Note: if using e500v2diab/e500v2gnu toolchain, it cannot be undefined.
 */

#define INCLUDE_SPE

/* vxbus support */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE         50000
#define INCLUDE_INTCTLR_LIB
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX
#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#undef  DRV_PCIBUS_M85XX
#undef  DRV_STORAGE_FSLSATA

#ifdef DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#endif

/* use openPicTimer as the auxClk */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif /* INCLUDE_AUX_CLK */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_SDHC */

/*
 * To help for synchronization, we need implement BSP_BOOT_CACHE_SYNC
 * which is used in romStart()
 */

#define BSP_BOOT_CACHE_SYNC     sysBspBootSync()

/*
 * The constants ROM_TEXT_ADRS, ROM_BASE_ADRS, ROM_SIZE are defined in both config.h
 * and Makefile. All definitions for these constants must be identical.
 */
 
#define ROM_TEXT_ADRS           0xFFF00100
#define ROM_BASE_ADRS           0xFFF00000

#define ROM_SIZE                0x00100000

/* memory */

#define LOCAL_MEM_LOCAL_ADRS    0x00000000
#define LOCAL_MEM_SIZE          0x20000000  /* 512MB */
#define USER_RESERVED_MEM       0

/* flash */

#define FLASH_BASE_ADRS         0xf8000000
#define FLASH_SIZE              0x08000000  /* 128MB */

/* console */

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE   115200

/* network */

#define INCLUDE_END

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define FSL_ENET0           0x00
#define FSL_ENET1           0x04
#define FSL_ENET2           0x9F

#define CUST_ENET3          0x00
#define CUST_ENET4          0xAB
#define CUST_ENET5_0        0xDD
#define CUST_ENET5_1        0xDE

#define MAX_MAC_ADRS        2
#define MAX_MAC_DEVS        1
#define MAC_ADRS_LEN        6

/* default mac address */

#define ENET_DEFAULT0       FSL_ENET0
#define ENET_DEFAULT1       FSL_ENET1
#define ENET_DEFAULT2       FSL_ENET2

/* nvram */

#define FLASH_ADRS          FLASH_BASE_ADRS
#define FLASH_WIDTH         2
#define FLASH_CHIP_WIDTH    2
#define FLASH_SECTOR_SIZE   0x00020000
#define FLASH_SECTOR_MASK   0xfffe0000

#define ROM_CODE_OFFSET     0x100
#define NV_RAM_ADRS         (ROM_TEXT_ADRS - ROM_CODE_OFFSET - FLASH_SECTOR_SIZE)
#define NV_RAM_SIZE         FLASH_SECTOR_SIZE

#define NV_MAC_ADRS_OFFSET  0x200

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT     1           /* 16-bit */
#define S29GL_FLASH_NUM     1           /* only one chip */
#define S29GL_FLASH_WIDTH   2           /* 16-bit */
#define S29GL_SECTOR_SIZE   0x00020000  /* 128K per sector */
#define S29GL_FLASH_SIZE    0x08000000  /* 128MB per chip */
#define S29GL_FLASH_OP_DELAY

/* default bootline */

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.2 e=192.168.0.3 \
u=vxworks pw=vxworks f=0x0 tn=mpc8536ds"

#undef INCLUDE_CPU_LIGHT_PWR_MGR

/* I2C support */

#undef INCLUDE_I2C

/* floating point */

#if (TOOL==e500v2diab) || (TOOL==e500v2gnu)

#   undef INCLUDE_GNU_INTRINSICS

/* These are 60X specific for now */

#   undef INCLUDE_HW_FP

/* e500v2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   undef  INCLUDE_HW_FP
#   define INCLUDE_SW_FP

#endif /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* PRJ_BUILD */
