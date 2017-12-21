/* mds8360/config.h - Freescale MDS8360 board configuration header */

/*
 * Copyright (c) 2006-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,19jul12,clx  update BSP_REV
01p,17apr10,pch  nvram & flash config adjustments
01o,15apr10,clx  Update BSP version for vxWorks 6.9
01n,21sep09,rgo  Fix build error when MPC8360EA_MDS is undefined(CQ:181688)
01m,18dec08,z_l  Increased rev of BSP.
01l,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01k,29jul08,z_l  Support MDS8360EA.
01j,18mar08,wap  Switch from hEnd qeFcc driver to VxBus UCC driver
01i,23aug07,mmi  remove DEFAULT_POWER_MGT_MODE
01h,12jul07,vik  added macro INCLUDE_VXB_CMDLINE
01g,06jun07,dtr  Support vxBus Interrupt controller drivers and PCI.
01f,07may07,dtr  added INCLUDE_DMA_SYS
01e,19dec06,rlg  updates for NOR
01d,22nov06,d_c  Move MAX_MAC_DEVS and ENET_DEFAULT from sysNet.h to config.h
01c,30aug06,dtr  Increment BSP rev.
01b,29jun06,dtr  update some definitions and include PCI.
01a,18may06,dtr  adapted from ads834x config.h
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
Freescale MDS8360 and MDS8360EA board.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * BSP version/revision identification, should be placed
 * before #include "configAll.h"
 */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */
#define BSP_NAME      mds8360

/* Define Clock Speed and source  */

#define FREQ_33_MHZ   33000000
#define FREQ_40_MHZ   40000000
#define FREQ_66_MHZ   66000000
#define FREQ_100_MHZ  100000000
#define FREQ_266_MHZ  266000000
#define FREQ_333_MHZ  333000000

/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 or the PCI freq of the MDS8360 board.
 * Choose from above list. BSP assumes standalone board ie not in
 * PCI chassis
 */

#define OSCILLATOR_FREQ FREQ_66_MHZ

#define SYS_CLK_FREQ    FREQ_266_MHZ

#define DEC_CLK_TO_INC  4            /* bus clks per increment*/
#define DEC_CLOCK_FREQ  SYS_CLK_FREQ /* Set to system default */
#define TIMESTAMP_HZ    SYS_CLK_FREQ

#define TPR 0x2000
#define LSRT_VALUE 0x3F

#include "configAll.h"

/* define only one of the following CPU*/

#define MPC8360

#define DEFAULT_BOOT_LINE \
"qefcc(0,0)host:target/config/mds8360/vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=mds8360"

/*
 * This macro is optional selected to support MDS8360EA board, if not defined,
 * then the BSP will support MDS8360 board.
 */

#define MPC8360EA_MDS

#ifdef MPC8360EA_MDS
#   define REV1_1_SILICON /* decide whether Errata ENET 15 patch is required */
#endif /* MPC8360EA_MDS */

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif	/* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE
/* Does nothing about copyback vs writethrough in h/w,
 * must use sysPhysMemDesc */
#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE  (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif	/* INCLUDE_CACHE_SUPPORT */

/* Number of TTY definition */

#undef  NUM_TTY
#define NUM_TTY  N_SIO_CHANNELS

#define INCLUDE_VXBUS
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QE
#define DRV_INTCTLR_QUICC
#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_QUICC_ENGINE_UTILS
#define INCLUDE_UCC_VXB_END
#define INCLUDE_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_MV88E1X11PHY
#define DRV_SIO_NS16550
#define DRV_TIMER_DEC_PPC
#define DRV_TIMER_QUICC_PPC
#define INCLUDE_TIMER_SYS
#define INCLUDE_DMA_SYS
#define INCLUDE_VXB_CMDLINE
#ifdef  INCLUDE_USB
#   define INCLUDE_PCI_OLD_CONFIG_ROUTINES
#endif /* INCLUDE_USB */

#define INCLUDE_SIO_UTILS
#define INCLUDE_HEND_PARAM_SYS
#define INCLUDE_PARAM_SYS
#define HWMEM_POOL_SIZE 50000

/* Optional timestamp support */

#define INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK

#undef INCLUDE_DMA

/* clock rates */

#define SYS_CLK_RATE_MIN  1     /* minimum system clock rate */
#define SYS_CLK_RATE_MAX  8000  /* maximum system clock rate */
#define AUX_CLK_RATE_MIN  1     /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX  8000  /* maximum auxiliary clock rate */

/* add on-chip drivers */

#undef INCLUDE_SECURITY_ENGINE  /* only supported if chip has E ie 8360E */

#undef INCLUDE_PIB_SUPPORT      /* include PIB IO board support */

#undef INCLUDE_PCI_BUS          /* include PCI library support */

#undef INCLUDE_TFFS             /* TrueFFS is supported in MDS8360EA board */

#if defined(INCLUDE_TFFS) && defined(MPC8360EA_MDS)
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_TL_FTL
#   define INCLUDE_DOSFS
#endif /* INCLUDE_TFFS & ... */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_SHOW
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_HRFS
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_ERF
#   define INCLUDE_XBD
#   define INCLUDE_XBD_BLKDEV
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_XBD_BLK_DEV
#   define INCLUDE_XBD_PART_LIB
#   define INCLUDE_DISK_UTIL
#endif /* INCLUDE_DOSFS */


/*
 * To enable PCI module, which is not supported in default configuration, the
 * following macros should be defined. See "Known Problems" in target.ref
 * for more details.
 */

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_M83XX
#   define INCLUDE_FEI8255X_VXB_END
#   ifndef INCLUDE_PIB_SUPPORT
#       define INCLUDE_PIB_SUPPORT
#endif /* INCLUDE_PIB_SUPPORT */
#endif /* INCLUDE_PCI_BUS */

#ifdef  INCLUDE_PCI_BUS


/*
CPU Addr    PCI Addr
PCI_LOCAL_MEM_BUS  ------------------------- PCI_MSTR_MEM_BUS
		   -                 -
		   -                 -
PCI_LOCAL_MEM_BUS +------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE ----IMMR          -       PCI_MSTR_MEM_SIZE
		   -                 -
		   -                 -----PIMMR
		   -                 -
		   -                 -
CPU_PCI_MEM_ADRS   --------------------------- PCI_MEM_ADRS
		   -                 -
		   -                 -
CPU_PCI_MEMIO_ADRS -------------------------   PCI_MEMIO_ADRS
		   -                 -
		   -                 -
CPU_PCI_IO_ADRS        ----------------------- PCI_IO_ADRS
		   -                 -
		   -                 -
CPU_PCI_IO_ADRS +  --------------------------- PCI_IO_ADRS +
CPU_PCI_IO_SIZE    -                 -         PCI_IO_SIZE
		   -                 -
		   -                 -
		   -                 -
		   -------------------------- 4GBytes
*/

/* for custom sysPciAutoConfig.c */

#   define PCI1_LAW_BASE       0x80000000       /* PCI LAW window */
#   define PCI1_LAW_SIZE       0x10000000       /* PCI LAW size   */

/* PCI based addresses */

#   define PCI_MEM_ADRS        0x80000000
#   define PCI_MEM_SIZE        0x08000000      /* 16MB */
#   define PCI_MEM_SIZE_MASK   PCI_SIZE_128MB /* This should match PCI_MEM_SIZE */
#   define PCI_MEMIO_ADRS      0x88000000
#   define PCI_MEMIO_SIZE      0x04000000     /* 16MB */
#   define PCI_MEMIO_SIZE_MASK PCI_SIZE_64MB  /* This should match PCI_MEMIO_SIZE */

#   define PCI_IO_ADRS         0x8c000000
#   define PCI_IO_SIZE         0x04000000  /* IO Space is not available */
#   define PCI_IO_SIZE_MASK    PCI_SIZE_64MB

#   ifdef INCLUDE_SECONDARY_PCI

#       define PCI2_LAW_BASE       0x90000000     /* PCI LAW window */
#       define PCI2_LAW_SIZE       0x10000000     /* PCI LAW size   */

#       define PCI2_MEM_SIZE      0x08000000  /* 16MB */
#       define PCI2_MEMIO_SIZE    0x08000000  /* 16MB */
#       define PCI2_MEM_SIZE_MASK PCI_SIZE_128MB /* This should match PCI_MEM_SIZE */
#       define PCI2_MEM_ADRS      (PCI2_LAW_BASE)
#       define PCI2_MEMIO_ADRS    (PCI2_MEM_ADRS + PCI2_MEM_SIZE)
#       define PCI2_IO_ADRS       (PCI2_MEMIO_ADRS + PCI2_MEMIO_SIZE)
#       define PCI2_IO_SIZE       0x10000000   /* IO Space is not available */

#       define CPU_PCI2_MEM_ADRS   0x90000000
#       define CPU_PCI2_MEM_SIZE   PCI_MEM_SIZE
#       define CPU_PCI2_MEMIO_ADRS 0x98000000
#       define CPU_PCI2_MEMIO_SIZE PCI_MEMIO_SIZE
#       define CPU_PCI2_IO_ADRS    0xa0000000
#       define CPU_PCI2_IO_SIZE    PCI_IO_SIZE

#   endif /* INCLUDE_SECONDARY_PCI */

/* CPU based addresses */

#   define CPU_PCI_MEM_ADRS    0x80000000
#   define CPU_PCI_MEM_SIZE    PCI_MEM_SIZE
#   define CPU_PCI_MEMIO_ADRS  0x88000000
#   define CPU_PCI_MEMIO_SIZE  PCI_MEMIO_SIZE
#   define CPU_PCI_IO_ADRS     0x8c000000
#   define CPU_PCI_IO_SIZE     PCI_IO_SIZE

/* CPU from PCI bus */

#   define PCI_MSTR_MEM_BUS  0x00000000
#   define PCI_MSTR_MEM_SIZE PCI_LOCAL_MEM_SIZE
#   define PCI_BRIDGE_PIMMR_BASE_ADRS      0x40000000

/* CPU Address that is visible from PCI */

#   define PCI_LOCAL_MEM_BUS    LOCAL_MEM_LOCAL_ADRS
#   define PCI_LOCAL_MEM_SIZE   LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#   define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

#endif /* INCLUDE_PCI_BUS */

/* add necessary drivers */

#define MAX_MAC_DEVS         2          /* two network devices */

/* default mac address */

#define ENET_DEFAULT0        WR_ENET0
#define ENET_DEFAULT1        WR_ENET1
#define ENET_DEFAULT2        WR_ENET2

#ifdef INCLUDE_UCC_VXB_END
#define ETHERNET_MAC_HANDLER
#   ifndef INCLUDE_END
#       define INCLUDE_END  /* only END-style driver for FCC */
#   endif /* INCLUDE_END */
#endif /* INCLUDE_UCC_VXB_END */

/*
 * Both FCC MAC address and bootline are stored in top of flash
 * If you want to move bootimage here must move elsewhere in flash
 * Requires 1 segment
 */

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
# ifdef MPC8360EA_MDS
#   define FLASH_29GL256N        0x7e
#   define SYS_FLASH_TYPE        FLASH_29GL256N
#   define FLASH_CHIP_WIDTH      2
#   define FLASH_ADRS            0xFE000000
#   define FLASH_SIZE            0x2000000
#   define FLASH_SECTOR_SIZE     0x20000     /* sector size of S29GL256N */
#   define FLASH_WIDTH           2
#   define FLASH_SEGMENT_SIZE    0x20000
#   define FLASH_WIDTH_SPECIAL_2
#   undef FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET        0
# else	/* MPC8360EA_MDS */
#   define SYS_FLASH_TYPE        FLASH_28F640J3A    /* flash device type */
#   define FLASH_WIDTH           2
#   define FLASH_CHIP_WIDTH      1
#   define FLASH_SEGMENT_SIZE    0x10000
#   define FLASH_ADRS            0xfe1e0000
#   define FLASH_SIZE            0x10000
#   define FLASH_SECTOR_SIZE     0x20000     /* sector size: 128KB, N=64 */
#   define FLASH_SIZE_WRITEABLE  0x10000
#   define NV_RAM_SIZE           FLASH_SIZE
#   define FLASH_WIDTH_SPECIAL_2
#   undef FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET        0
# endif /* MPC8360EA_MDS */
#define	INCLUDE_NVRAM
#define	INCLUDE_NVRAM_WRITE
#define	INCLUDE_FLASH_WRITE
#endif /* INCLUDE_FLASH */

#define FLASH_BASE_ADRS 0xfe000000
#ifdef MPC8360EA_MDS
#   define FLASH_WINDOW_SIZE 0x02000000
#else	/* MPC8360EA_MDS */
#   define FLASH_WINDOW_SIZE 0x00200000
#endif /* MPC8360EA_MDS */
/* Memory addresses */

#define LOCAL_MEM_SIZE       0x10000000 /* 256 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS 0x00000000 /* Base of RAM */
#define INCLUDE_LBC_SDRAM
#define INCLUDE_DDR_SDRAM
#define INCLUDE_SECONDARY_DRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE2            0x4000000   /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS2      0x10000000  /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK  0xfc000000
#define LBC_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE2
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS       0xfe000000     /* base address of ROM */
#define ROM_SIZE            0x100000       /* ROM space */
#define ROM_TEXT_ADRS       (ROM_BASE_ADRS + 0x100)
#define ROM_WARM_ADRS       (ROM_TEXT_ADRS + 8)   /* warm reboot entry */

#define USER_RESERVED_MEM   0x00000000    /* user reserved memory size */

#ifdef MPC8360EA_MDS
#   define NV_RAM_SIZE      FLASH_SECTOR_SIZE
#   define NV_RAM_ADRS      ((ROM_TEXT_ADRS - 0x100) + ROM_SIZE)
#endif /* MPC8360EA_MDS */

/* Hard Reset Configuration Words */

/* See MPC8360E hardware specification for supported clock freq */
/* More information in target.ref */


/* spmf 1:4 ie 4*66Mhz == 266Mhz CSB*/

#define HRCW_LOW_BYTE0 0x04

/* corepll ratio 1:2 533Mhz  */

#define HRCW_LOW_BYTE1  0x04
#define HRCW_LOW_BYTE2  0x00
#define HRCW_LOW_BYTE3  (HRCW_LOW_CEPMF_6)  /* QuiccEngine 66 * 6 == 333MHz */
#define HRCW_HIGH_BYTE0 (HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | \
			 HRCW_HIGH_PCI_CLK_OUT) /* PCI HOST/ARB/CLK_OUT */
#define HRCW_HIGH_BYTE1 (ROMLOC_GPCM_16BIT)
#define HRCW_HIGH_BYTE2 0x00
#define HRCW_HIGH_BYTE3 0x00

#include "mds8360.h"  /* include the mds8360 params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* __INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif	/* PRJ_BUILD */
