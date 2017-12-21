/* config.h - Freescale MPC8360 RDK board configuration header */

/*
 * Copyright (c) 2007-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,19jul12,clx  update BSP_REV
01g,15apr10,clx  update BSP version for vxWorks 6.9
01f,15jul09,b_m  bump up BSP_REV to "/4".
01e,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01d,24jul08,y_w  Updated BSP_REV to "/3" to support VxWorks 6.7
01c,04jun08,y_w  Updated BSP_REV to "/2" to add support for SEC
01b,26dec07,z_l  Updated to vxWorks6.6
01a,15aug07,z_l  Created from mds8360/config.h
*/

/*
DESCRIPTION
This file contains the configuration parameters for the Freescale MPC8360
RDK board.
*/

#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 * BSP version/revision identification, should be placed
 * before #include "configAll.h"
 */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Define Clock Speed and source  */

#define FREQ_33_MHZ     33000000
#define FREQ_40_MHZ     40000000
#define FREQ_66_MHZ     66000000
#define FREQ_100_MHZ    100000000
#define FREQ_266_MHZ    266000000
#define FREQ_333_MHZ    333000000

/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 or the PCI freq of the ADS834x board.  
 * Choose from above list. BSP assumes standalone board ie not in 
 * PCI chassis 
 */

#define OSCILLATOR_FREQ     FREQ_66_MHZ
#define SYS_CLK_FREQ        FREQ_266_MHZ

#define DEC_CLK_TO_INC      4            /* # bus clks per increment*/ 
#define DEC_CLOCK_FREQ      SYS_CLK_FREQ /* Set to system default */
#define TIMESTAMP_HZ        SYS_CLK_FREQ

#define TPR 0x2000
#define LSRT_VALUE 0x3F

#include "configAll.h"

#include "fsl_mpc8360_rdk.h"    /* include the fsl_mpc8360_rdk params */

#define REV_2_SILICON

/* define only one of the following CPU*/

#define MPC8360

#define DEFAULT_BOOT_LINE \
"qefcc(0,0)host:vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=MPC8360-RDK"

#define INCLUDE_MMU_BASIC

#ifdef INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE
#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE  (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif /* INCLUDE_CACHE_SUPPORT */

/* Number of TTY definition */

#undef  NUM_TTY
#define NUM_TTY     N_SIO_CHANNELS

#define INCLUDE_VXBUS

#ifdef  INCLUDE_VXBUS
#   define INCLUDE_INTCTLR_LIB
#   define DRV_INTCTLR_PPC
#   define DRV_INTCTLR_QE
#   define DRV_INTCTLR_QUICC
#   define INCLUDE_VXBUS_INIT
#   define INCLUDE_HWMEM_ALLOC
#   define INCLUDE_VXBUS_ACTIVATE
#   define INCLUDE_PARAM_SYS
#   define INCLUDE_PLB_BUS
#   define INCLUDE_QUICC_ENGINE_UTILS
#   define INCLUDE_UCC_VXB_END

/*
 * Because of the limit of CPU's multi-user RAM size, BSP can not support all of
 * the 4 network ports at the same time. The default network ports we use are 
 * ETH1 & 2, so this macro is undefined to disable ETH3 & 4 by default. Users
 * can define this macro to use ETH3 & 4, but note ETH1 & 2 will be disabled.
 */

#   undef  ETH_OPTION_SUPPORT

#   define INCLUDE_MII_BUS
#   ifndef ETH_OPTION_SUPPORT
#       define INCLUDE_BCM54XXPHY
#   else  /* ETH_OPTION_SUPPORT */
#       define INCLUDE_GENERICPHY
#   endif /* ETH_OPTION_SUPPORT */

#   define DRV_SIO_NS16550
#   define INCLUDE_SIO_UTILS
#   define INCLUDE_HEND_PARAM_SYS
#   define HWMEM_POOL_SIZE 50000
#   define DRV_TIMER_DEC_PPC
#   define DRV_TIMER_QUICC_PPC
#   define INCLUDE_TIMER_SYS
#   define INCLUDE_DMA_SYS

#   undef  INCLUDE_PCI_BUS
#   undef  INCLUDE_PCI_BUS_AUTOCONF

#endif /* INCLUDE_VXBUS */

#define INCLUDE_VXB_CMDLINE

/* Optional aux clock and timestamp support */

#define INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK

#undef  INCLUDE_DMA

/* clock rates */

#define SYS_CLK_RATE_MIN    1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */

#ifdef INCLUDE_PCI_BUS

/*
 * CPU Addr                                      PCI Addr
 * PCI_LOCAL_MEM_BUS   ------------------------- PCI_MSTR_MEM_BUS
 *                     -               -
 *                     -               -
 * PCI_LOCAL_MEM_BUS + ------------------------- PCI_MSTR_MEM_BUS +
 * PCI_LOCAL_MEM_SIZE  ----IMMR                - PCI_MSTR_MEM_SIZE
 *                     -               -
 *                     -               -----PIMMR
 *                     -               -
 *                     -               -
 * CPU_PCI_MEM_ADRS    ------------------------- PCI_MEM_ADRS
 *                     -               -
 *                     -               -
 * CPU_PCI_MEMIO_ADRS  ------------------------- PCI_MEMIO_ADRS
 *                     -               -
 *                     -               -
 * CPU_PCI_IO_ADRS     ------------------------- PCI_IO_ADRS
 *                     -               -
 *                     -               -
 * CPU_PCI_IO_ADRS +   ------------------------- PCI_IO_ADRS +
 * CPU_PCI_IO_SIZE     -               -         PCI_IO_SIZE
 *                     -               -
 *                     -               -
 *                     -               -
 *                     -------------------------- 4GBytes
 */

/* Intel PRO/100+ and PRO/1000 support */

#define DRV_PCIBUS_M83XX

#undef  INCLUDE_GEI825XX_VXB_END
#define INCLUDE_FEI8255X_VXB_END

#if (defined(INCLUDE_GEI825XX_VXB_END) || defined(INCLUDE_FEI8255X_VXB_END))
#   ifndef INCLUDE_GENERICPHY
#       define INCLUDE_GENERICPHY
#   endif
#endif /* INCLUDE_FEI8255X_VXB_END */

/* for custom sysPciAutoConfig.c */

#define PCI1_LAW_BASE       0x80000000      /* PCI LAW window */
#define PCI1_LAW_SIZE       0x10000000      /* PCI LAW size   */

/* PCI based addresses */

#define PCI_MEM_ADRS        0x80000000
#define PCI_MEM_SIZE        0x08000000      /*16MB */
#define PCI_MEM_SIZE_MASK   PCI_SIZE_128MB  /* This should match PCI_MEM_SIZE */
#define PCI_MEMIO_ADRS      0x88000000
#define PCI_MEMIO_SIZE      0x04000000      /*16MB */
#define PCI_MEMIO_SIZE_MASK PCI_SIZE_64MB   /* This should match PCI_MEMIO_SIZE */

#define PCI_IO_ADRS         0x8c000000
#define PCI_IO_SIZE         0x04000000      /* IO Space is not available */
#define PCI_IO_SIZE_MASK    PCI_SIZE_64MB

#ifdef INCLUDE_SECONDARY_PCI

#   define PCI2_LAW_BASE        0x90000000      /* PCI LAW window */
#   define PCI2_LAW_SIZE        0x10000000      /* PCI LAW size   */

#   define PCI2_MEM_SIZE        0x08000000      /*16MB */
#   define PCI2_MEMIO_SIZE      0x08000000      /*16MB */
#   define PCI2_MEM_SIZE_MASK   PCI_SIZE_128MB  /* This should match PCI_MEM_SIZE */
#   define PCI2_MEM_ADRS        (PCI2_LAW_BASE)
#   define PCI2_MEMIO_ADRS      (PCI2_MEM_ADRS   + PCI2_MEM_SIZE)    
#   define PCI2_IO_ADRS         (PCI2_MEMIO_ADRS + PCI2_MEMIO_SIZE)  
#   define PCI2_IO_SIZE         0x10000000      /* IO Space is not available */

/* CPU based addresses */

#   define CPU_PCI2_MEM_ADRS    0x90000000
#   define CPU_PCI2_MEM_SIZE    PCI_MEM_SIZE
#   define CPU_PCI2_MEMIO_ADRS  0x98000000
#   define CPU_PCI2_MEMIO_SIZE  PCI_MEMIO_SIZE
#   define CPU_PCI2_IO_ADRS     0xa0000000
#   define CPU_PCI2_IO_SIZE     PCI_IO_SIZE

#endif /* INCLUDE_SECONDARY_PCI */

/* CPU based addresses */

#define CPU_PCI_MEM_ADRS        0x80000000
#define CPU_PCI_MEM_SIZE        PCI_MEM_SIZE
#define CPU_PCI_MEMIO_ADRS      0x88000000
#define CPU_PCI_MEMIO_SIZE      PCI_MEMIO_SIZE
#define CPU_PCI_IO_ADRS         0x8c000000
#define CPU_PCI_IO_SIZE         PCI_IO_SIZE

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS            0x00000000
#define PCI_MSTR_MEM_SIZE           PCI_LOCAL_MEM_SIZE
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS       LOCAL_MEM_LOCAL_ADRS
#define PCI_LOCAL_MEM_SIZE      LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

#endif /* INCLUDE_PCI_BUS */

/* add necessary drivers */

#ifdef INCLUDE_UCC_VXB_END
#   define ETHERNET_MAC_HANDLER
#   ifndef INCLUDE_END
#       define INCLUDE_END  /* only END-style driver for FCC */
#   endif /* INCLUDE_END */
#endif /* INCLUDE_UCC_VXB_END */

#define MAX_MAC_DEVS         2          /* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0 0x00
#define ENET_DEFAULT1 0xA0
#define ENET_DEFAULT2 0x1E

/* 
 * Both FCC MAC address and bootline is stored in top of flash
 * If you want to move bootimage here must move elsewhere in flash
 * Requires 1 segment 
 */

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#   define SYS_FLASH_TYPE        FLASH_28F640J3A    /* flash device type */
#   define FLASH_WIDTH           2                  /* 16-bit width */
#   define FLASH_CHIP_WIDTH      1
#   define FLASH_SEGMENT_SIZE    0x20000
#   define FLASH_ADRS            0xff900000
#   define FLASH_SIZE            0x20000
#   define FLASH_SIZE_WRITEABLE  0x20000
#   define NV_RAM_SIZE           0x20000            /* 128KB */
#   define FLASH_WIDTH_SPECIAL_2
#   undef  FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET        0                  /* 0xff900000 */
#   define FLASH_LOCKED
#   define INCLUDE_BOOT_IMAGE_PROGRAM /* FLASH Boot Bank Programming Support */
#endif /* INCLUDE_FLASH */

#define FLASH_BASE_ADRS         0xfe000000
#define FLASH_WINDOW_SIZE       0x00200000

#define LOCAL_MEM_SIZE          0x10000000  /* 256 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000  /* Base of RAM */
#define INCLUDE_DDR_SDRAM

#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS           0xff800000  /* base address of ROM */
#define ROM_SIZE                0x00800000  /* ROM space */

#undef  BOOT_FROM_UBOOT

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

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == JUMP_TO_VX_BOOT)
#       define ROM_TEXT_ADRS        0xFFB80100
#   else
#       define ROM_TEXT_ADRS        0x02000000
#       undef  ROMSTART_BOOT_CLEAR  /* for romStart.c */
#       define FORCE_DEFAULT_BOOT_LINE
#   endif
#else  /* BOOT_FROM_UBOOT */
#   define ROM_TEXT_ADRS    (ROM_BASE_ADRS + 0x100)  /* ROM entry address */
#endif /* BOOT_FROM_UBOOT */

#define ROM_WARM_ADRS   (ROM_TEXT_ADRS + 8) /* warm reboot entry */

#define USER_RESERVED_MEM    0x00000000 /* user reserved memory size */

/* Hard Reset Configuration Words */

/* See MPC8349E hardware specification for supported clock freq */
/* More information in target.ref */

/* spmf 1:4 ie 4*66Mhz == 266Mhz CSB*/

#define HRCW_LOW_BYTE0  0x08

/* corepll ratio 1:2 533Mhz  */

#define HRCW_LOW_BYTE1  0x04 
#define HRCW_LOW_BYTE2  0x00
#define HRCW_LOW_BYTE3  (HRCW_LOW_CEPMF_6<<1)  /* QuiccEngine 66 * 6 == 333MHz */

#define HRCW_HIGH_BYTE0 (HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB)
#define HRCW_HIGH_BYTE1 ROMLOC_GPCM_16BIT
#define HRCW_HIGH_BYTE2 0x00
#define HRCW_HIGH_BYTE3 0x04

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif

