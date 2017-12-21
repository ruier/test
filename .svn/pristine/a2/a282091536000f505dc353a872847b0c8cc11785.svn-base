/* config.h - LSI ACP34XX Board Configuration Header */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,30nov12,j_z  add SRIO support.
01r,03aug12,x_s  Update BSP revision to support LSI ACP34XX JUNCTION board.
01q,16may12,x_s  Update BSP revision and code clean.
01p,04oct11,d_c  Defect WIND00309565 Fix WIND00309570
                 Enable cache snoop mode in cache mode masks.
		 Disable MMU optimization by default.
		 Update BSP revision.
01o,24aug11,x_z  updated BSP revision.
01n,09may11,x_s  removed the dependency between AXIS timer and INCLUDE_TIMESTAMP.
01m,05may11,x_z  updated BSP revision.
01l,25mar11,x_z  added AXIS timer support.
01k,08mar11,x_z  replaced INCLUDE_PCI with DRV_PCIBUS_LSI_PCIEX.
01j,01mar11,x_z  enabled page table optimization;
                 removed USB macros;
                 fixed NV_RAM_ADRS.
01i,23feb11,x_z  fixed sysModel issue and removed unused ENET_DEFAULT.
01h,23jan11,x_z  fixed NVRAM macros.
01g,22jan11,x_s  added PCIe support.
01f,22dec10,x_s  added I2C support.
01e,21dec10,x_s  added SPI support.
01d,20dec10,x_z  added HRFS macros.
01c,16dec10,x_z  added support for hard float poin.
01b,26oct10,x_z  added support for uAMP and USB.
01a,27may10,x_z  written.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef  __cplusplus
    extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/5"

/* Base model string. sysModelGet adds processor details */

#define SYS_MODEL_ACP3448       "LSI PPC476 ELPASO - ACP3448"
#define SYS_MODEL_ACP3423       "LSI PPC476 JUNCTION - ACP3423"
#define SYS_MODEL               "LSI PPC476"

#include <configAll.h>

/* Default boot line */

#define DEFAULT_BOOT_LINE   "lsiMac(0,0)host:vxWorks.st h=192.168.0.100 \
                            e=192.168.0.101:ffffff00 g=192.168.0.1 u=target \
                            pw=vxTarget f=0x0 tn=ACP34xx"

/* Using hard FP support by default */

#define INCLUDE_HW_FP
#undef  INCLUDE_SW_FP

/*
 * HW Debugger Support
 *
 * When CLEAR_DEBUG_REGISTERS is defined, the bootrom and kernel clear
 * the onchip debugging registers.  This is ideal for a delivered
 * application as it ensures that those chip resources are inert.
 *
 * When it is not defined, the bootrom and kernel do not initialize the
 * onchip debug control registers. This is the most appropriate setting
 * when using hardware assisted debugging.
 */

#undef CLEAR_DEBUG_REGISTERS

/*
 * Memory configuration.  1 GB
 *
 * NOTE: bootrom is loaded from NAND Flash to memory by LSI starge 2 U-Boot,
 * we must reserve enough space for it.
 */

/* unnecessary  for project build */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE          (DRAM_SIZE - ROM_SIZE)
#   define LOCAL_MEM_LOCAL_ADRS    DRAM_BASE_ADRS    /* fixed at zero */
#endif /* CDF_OVERRIDE */

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS

#define USER_RESERVED_MEM           0

/* memory configuration for AMP */

/* unnecessary  for project build */

#ifndef CDF_OVERRIDE
#   define RAM_LOW_ADRS_CPU0        0x00100000
#endif /* CDF_OVERRIDE */

#define CPUn_SPACE(n)               ((RAM_LOW_ADRS_CPU0 - (0x10000 * n)))
#define CPU_SPACE_SIZE_SHIFT        16  /* 0x10000 = 1 << 16*/
#define CPU_FUNC_START_ADR_OFFSET   0
#define CPU_STACK_START_ADR_OFFSET  4
#define CPU_INIT_START_ADR_OFFSET   8
#define CPU_INIT_STATUS_ADR_OFFSET  12

#define CPUn_FUNC_START_ADR(n)      (CPUn_SPACE(n) + CPU_FUNC_START_ADR_OFFSET)
#define CPUn_STACK_START_ADR(n)     (CPUn_SPACE(n) + CPU_STACK_START_ADR_OFFSET)
#define CPUn_INIT_START_ADR(n)      (CPUn_SPACE(n) + CPU_INIT_START_ADR_OFFSET)
#define CPUn_INIT_STATUS_ADR(n)     (CPUn_SPACE(n) + CPU_INIT_STATUS_ADR_OFFSET)

#undef  USER_I_CACHE_MODE
#undef  USER_D_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_COPYBACK  | CACHE_SNOOP_ENABLE)
#define USER_D_CACHE_MODE       (CACHE_COPYBACK  | CACHE_SNOOP_ENABLE)
    
/*
 * MMU support.
 *
 * On PowerPC Book E processors, MMU translation cannot be deactivated.
 * The VxWorks bootrom establishes several TLB entries mapping chunks
 * (see bootEntry in romInit.s). These entries statically map the entire
 * effective address space to physical addresses, allowing the boot
 * process to load a kernel.
 *
 * This process is repeated for the vxWorks kernel through the
 * sysStaticTlbDesc[] array (see sysLib.c), and for mappings beyond the
 * reach of vmLib (outside the lowest 4GB of physical address space).
 *
 * Finer-grained storage attributes (caching, guarded,
 * execute/read/write) and mapping (4KB chunks) are managed by the vmLib
 * and vmBaseLib API.  These MMU libraries accomplish this through use
 * and reuse of the remaining TLB entries.
 *
 * For more details, see sysPhysMemDesc[] in sysLib.c.
 */

#define INCLUDE_MMU_BASIC
#define USER_D_MMU_ENABLE
#define USER_I_MMU_ENABLE

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG

#   ifndef  BOOTAPP
#       undef  INCLUDE_MMU_OPTIMIZE /* unsupported if using RTPs */

    /*
     * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can
     * possibly be already used in sysStaticTlbDesc. Also the larger the
     * page size that can be used the less CAM entries required so aligning
     * RAM_LOW_ADRS on a large page size boundary is usually necessary.
     * Locking the text section should increase performance because no
     * Instruction TLB misses are taken within the text section.
     */

#       undef  INCLUDE_LOCK_TEXT_SECTION   /* unsupported */

    /*
     * INCLUDE_PAGE_SIZE_OPTIMIZATION - This routines calls one time
     * vmPageOptimize for the entire kernel mapped address space.
     * It will use where applicable large MMU TLB page sizes to map memory.
     * This routine should reduce TLB misses. Any modification of the kernel
     * MMU state will unoptimize the related MMU pages.
     */

#       undef  INCLUDE_PAGE_SIZE_OPTIMIZATION /* unsupported if using RTPs */
#   endif /* BOOTAPP */

    /* This write protects the page tables */

#   undef INCLUDE_AIM_MMU_PT_PROTECTION /* unsupported now */

#   define  INCLUDE_AIM_MMU_MEM_POOL_CONFIG

#endif /* INCLUDE_MMU_BASIC */

/* timers */

/*  System Timer */

#define SYS_CLK_RATE_MIN        3       /* minimum system clock rate     */
#define SYS_CLK_RATE_MAX        5000    /* maximum system clock rate     */

/* Auxiliary Timer */

#define AUX_CLK_RATE_MIN        1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX        8000    /* maximum auxiliary clock rate */

/* Watchdog Timer */

#define WDT_RATE_MIN            1       /* minimum watchdog timer rate */
#define WDT_RATE_MAX            8000    /* maximum watchdog timer rate */

/* serial line */

#undef  NUM_TTY
#define NUM_TTY                 N_SIO_CHANNELS
#undef  CONSOLE_TTY
#define CONSOLE_TTY             0       /* 0 = UART1, 1 = UART2 */
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       9600

/* default mac address */

#define ENET_DEFAULT0           0x00
#define ENET_DEFAULT1           0x02
#define ENET_DEFAULT2           0x2d

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define CUST_ENET3              0x84
#define CUST_ENET4              0x11
#define CUST_ENET5              0x9D

#define MAX_MAC_ADRS            1
#define MAX_MAC_DEVS            1
#define MAC_ADRS_LEN            6

/* vxBus */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_DMA_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE         50000

/* SIO */

#define DRV_SIO_PRIMECELL
#define INCLUDE_SIO_UTILS

/* intterrupt controller */

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_MPIC

/* timer */

#define DRV_TIMER_PPC440
#define INCLUDE_TIMER_SYS

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_LSI
#endif /* INCLUDE_AUX_CLK */

/* FEMAC */

#define INCLUDE_LSI_MAC_VXB_END
#define INCLUDE_LSI_MDIO
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

/* NAND Flash */

#define SYS_NAND_BOARD_DELAY
#undef  SYS_NAND_DBG

/* SPI & SPI FLASH */

#undef INCLUDE_LSI_SPI

/* I2C & EEPROM */

#undef INCLUDE_I2C

#ifdef INCLUDE_I2C
#   define  INCLUDE_AT24C_MEM
#endif /* INCLUDE_I2C */

/* PCIe */

/* PCIE configuration*/

#define PCIE0_CFG_ADRS           0x80000000
#define PCIE0_CFG_SIZE           0x08000000    /* 128M size */
#define PCIE0_MEM_ADRS           0x88000000
#define PCIE0_MEM_SIZE           0x08000000    /* 128M size */
#define PCIE0_MEMIO_ADRS         0x90000000
#define PCIE0_MEMIO_SIZE         0x08000000    /* 128M size */
#define PCIE0_IO_ADRS            0x98000000
#define PCIE0_IO_SIZE            0x00010000    /* 64K size */
#define PCIE0_MEM_BASE_ADRS      0x40000000    /* 1G size */

#define PCIE_BASE_ADRS_PHYS_LO   0x40000000
#define PCIE_BASE_ADRS_PHYS_HI   0x020
#define PCIE_SIZE                0x40000000    /* 1G size */

#ifdef INCLUDE_PCI_BUS
#   define DRV_PCIBUS_LSI_PCIEX
#   define INCLUDE_PCI_BUS_AUTOCONF
#endif /* INCLUDE_PCI_BUS */

#ifdef DRV_SRIO_LSI
#   define LSI_RIO_MEM_ADRS      0xC0000000
#   define LSI_RIO_MEM_SIZE      0x10000000    /* 256M size */
#endif /* DRV_SRIO_LSI */

/* Base address for exception vector handlers */

#define VEC_BASE                ((char *) 0)

#include "lsi_acp34xx.h" /* board header file */

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and
 * Makefile.
 * All definitions for these constants must be identical.
 */

/*
 * boot loader address space
 *
 * bootrom can be only saved in NAND, and ACP34XX can't boot from NAND, so
 * bootrom must be saved in DRAM for cold/warm reset.
 */

#define ROM_BASE_ADRS           (DRAM_BASE_ADRS + DRAM_SIZE - ROM_SIZE)
#define ROM_TEXT_ADRS           ROM_BASE_ADRS

#define ROM_WARM_ADRS           (ROM_TEXT_ADRS + 0x0004)
#define ROM_SIZE                0x100000

/*
 * NVRAM can be NAND FLASH only, so must be multiple of sysNandBlockSize in
 * sysNandFlash.c or NAND_BLOCK_SIZE_MAX
 */

#define NV_RAM_ADRS             (0x38000000 & (~(NAND_BLOCK_SIZE_MAX - 1)))
#define NV_RAM_SIZE             NAND_BLOCK_SIZE_MAX

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0
#define NV_MAC_ADRS_OFFSET      (NV_BOOT_OFFSET + BOOT_LINE_SIZE + 1)

#ifdef __cplusplus
    }
#endif

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif
