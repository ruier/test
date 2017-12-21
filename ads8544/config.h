/* config.h - Freescale MPC8544 BSP configuration file */

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
01v,19jul12,clx  Update BSP_REV
01u,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01t,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01s,15apr10,clx  Update BSP version for vxWorks 6.9
01r,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01q,05jan09,b_m  update BSP_REV.
01p,23oct08,d_l  Add auxclk rate for OpenPic timer. (WIND00140273)
01o,25aug08,dtr  Add support for hardware float build.
01n,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01m,08aug08,x_f  Updated BSP_REV to "/2", update NUM_TTY to 2
01l,22may08,x_f  remove RAM_LOW_ADRS and RAM_HIGH_ADRS define 
01k,28apr08,x_f  Changed timestamp clock to M85XX timer instead of openPIC 
                 timer.
01j,09apr08,x_f  Fixed wrong value of AUX_CLK_RATE_MAX, FREQ_66_MHZ, 
                 FREQ_100_MHZ.
01j,09apr08,x_f  Fixed wrong AUX_CLK_RATE_MAX, FREQ_66_MHZ, FREQ_100_MHZ value.
01i,27aug07,dtr  Revert NUM_TTY to 1. WIND00102728 fix bootup.
01h,23aug07,dtr  Use dynamic interrupt controller lib and latest vxb etsec.
01g,21aug07,mmi  remove legacy power management define
01f,12jul07,vik  added macro INCLUDE_VXB_CMDLINE for dynamic cmdLine
01e,07may07,???  added INCLUDE_DMA_SYS
01d,23mar07,pmr  fix bootApp support
01c,22feb07,dtr  Add AUX clk include.
01b,22feb07,dtr  Add support for vxBus sio.
01a,04jan07,dtr  Modify from cds8548/config.h/01i
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */

/* Define Clock Speed and source  */

#define FREQ_33_MHZ     33333333
#define FREQ_66_MHZ     66666667
#define FREQ_100_MHZ    100000000
#define FREQ_133_MHZ    133333333
#define FREQ_266_MHZ    266666666
#define FREQ_400_MHZ    400000000
#define FREQ_333_MHZ    333333333
#define FREQ_533_MHZ    533333333

#define OSCILLATOR_FREQ FREQ_66_MHZ

#define PLAT_RATIO_533_MHZ 16
#define PLAT_RATIO_400_MHZ 12
#define PLAT_RATIO_333_MHZ 8

/* Frequency applied to the  "RTC" pin */

#define HPCN_RTC_FREQ   14318000

/* possibly this is not supported check your DDR undefined for safety */

#define INCLUDE_DDR_ECC
#define DEFAULT_SYSCLKFREQ FREQ_400_MHZ

#undef  FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

#include <configAll.h>

#include "ads8544.h"

/* Common Includes for VXBUS PCI and ETSEC */

#define INCLUDE_VXBUS
#ifdef  INCLUDE_VXBUS
#   define INCLUDE_VXB_CMDLINE
#   define INCLUDE_VXBUS_INIT
#   define INCLUDE_HWMEM_ALLOC
#   define INCLUDE_VXBUS_ACTIVATE
#   define INCLUDE_PARAM_SYS
#   define INCLUDE_PLB_BUS

#   undef  INCLUDE_PCI_BUS
#   ifdef  INCLUDE_PCI_BUS
#       define  INCLUDE_PCI_BUS_AUTOCONF
#       define  DRV_PCIBUS_M85XX
#   endif  /* INCLUDE_PCI_BUS */

#   define DRV_RESOURCE_M85XXCCSR

#   define INCLUDE_INTCTLR_LIB
#   define DRV_INTCTLR_PPC
#   define DRV_INTCTLR_EPIC
#   define INCLUDE_INTCTLR_DYNAMIC_LIB

#   define DRV_TIMER_M85XX
#   define INCLUDE_TIMER_SYS

#   define DRV_SIO_NS16550
#   define INCLUDE_SIO_UTILS

#   define INCLUDE_ETSEC_VXB_END
#   define INCLUDE_DMA_SYS

#   define INCLUDE_MII_BUS
#   define INCLUDE_GENERICPHY

#   define HWMEM_POOL_SIZE 100000
#endif /* INCLUDE_VXBUS */

/* other useful includes */

#undef INCLUDE_IFMEDIA
#undef INCLUDE_VXBUS_SHOW
#undef INCLUDE_IFCONFIG
#undef INCLUDE_NET_IF_SHOW
#undef INCLUE_SHOW_ROUTINES
#undef INCLUDE_ISR_OBJECTS
#undef DRV_TIMER_OPENPIC

#define MAX_MAC_DEVS    2           /* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0   WR_ENET0
#define ENET_DEFAULT1   WR_ENET1
#define ENET_DEFAULT2   WR_ENET2

/* 
 * Need LINKHDR extension to support FCB in ETSEC to 
 * reduce FCB insertion requiring it's own BD 
 */

#undef  MAX_LINKHDR_CFG
#define MAX_LINKHDR_CFG       32

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << 21))

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks h=1.1.0.1 e=1.1.0.2 u=vxworks pw=vxworks f=0x0"

#define INCLUDE_END

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE         VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#define INCLUDE_CACHE_SUPPORT 
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE
#undef  INCLUDE_L2_SRAM

#define INCLUDE_BRANCH_PREDICTION

/* These are maximum values for the 8544 processor */

#define L2_CACHE_SIZE       L2SIZ_256KB
#define L2_SRAM_SIZE        L2SIZ_256KB

#define L2SRAM_ADDR         0x7FF80000
#define L2SRAM_WINDOW_SIZE  0x40000 

#ifdef INCLUDE_NFS

/*
 * Default NFS parameters - constants may be changed here, variables
 * may be changed in usrConfig.c at the point where NFS is included.
 */

#   define NFS_USER_ID      2001            /* dummy nfs user id */
#   define NFS_GROUP_ID     100             /* dummy nfs user group id */
#   define NFS_MAXPATH      255             /* max. file path length */
#endif /* INCLUDE_NFS */

/* Disable Support for SPE 64bit registers */

#define INCLUDE_SPE

/* Serial channel and TTY */

#undef  NUM_TTY
#define NUM_TTY 2

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE   115200

#define INCLUDE_AUX_CLK

/* Clock rates */

#define SYS_CLK_RATE_MIN       1        /* minimum system clock rate */
#define SYS_CLK_RATE_MAX       8000     /* maximum system clock rate */
#ifdef INCLUDE_AUX_CLK
#   if defined(DRV_TIMER_OPENPIC)
#      define AUX_CLK_RATE_MIN    1
#      define AUX_CLK_RATE_MAX    50000
#   elif defined(DRV_TIMER_M85XX)
#      define AUX_CLK_RATE_MIN    (FREQ_400_MHZ / (1 << 24))
#      define AUX_CLK_RATE_MAX    (FREQ_400_MHZ / (1 << 13))
#   else
#      error "At least one of DRV_TIMER_M85XX or DRV_TIMER_OPENPIC must be defined!"
#   endif /* DRV_TIMER_OPENPIC */
#endif /* INCLUDE_AUX_CLK */

/* TrueFFS flash support */

#ifdef  INCLUDE_TFFS
#   define INCLUDE_SHOW_ROUTINES     /* show routines for system facilities*/
#   define INCLUDE_FS_EVENT_UTIL     /* File system event utility */
#   define INCLUDE_FS_MONITOR        /* File system monitor */
#   define INCLUDE_ERF               /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER    /* Device Manager */
#   define INCLUDE_XBD               /* Extended block device */
#   define INCLUDE_XBD_BLK_DEV       /* XBD-block device wrapper */

#   define INCLUDE_DOS_FS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#endif  /* INCLUDE_TFFS */

/*
 * Although the support for point to point has been added it is untested 
 * with Serial RIO. SImilar was tested on parallel RIO 
 */

#ifdef INCLUDE_RAPIDIO_BUS

/* RapidIO information */

#   define RAPIDIO_REG_BASE     0xc0000
#   define RAPIDIO_BASE         (CCSBAR + RAPIDIO_REG_BASE)
#   define RAPIDIO_ADRS         0xc0000000
#   define RAPIDIO_SIZE         0x10000000
#   define RAPIDIO_BUS_ADRS     0x80000000
#   define RAPIDIO_BUS_SIZE     0x10000000
#endif /* INCLUDE_RAPIDIO_BUS */

#define EPIC_EX_DFT_POLAR       EPIC_INT_ACT_HIGH
#define EPIC_EX_DFT_SENSE       EPIC_SENSE_LVL

#ifdef  INCLUDE_PCI_BUS

/*  cds85xx support dual PCI controllers */

/*
 * CPU Addr                           PCI Addr ( PCI1 or PCI2)
 * PCI_LOCAL_MEM_BUS         ------------------- PCI_MSTR_MEM_BUS
 *                           -                 - 
 *                           -                 -
 * PCI_LOCAL_MEM_BUS +       ------------------- PCI_MSTR_MEM_BUS +
 * PCI_LOCAL_MEM_SIZE        -                 - PCI_MSTR_MEM_SIZE
 *                           -                 - 
 *                           -                 ----- PCI Bridge (for PCI1 only)
 *                           -                 -     configuration regs
 *                           -                 - 
 * CPU_PCI_MEM_ADRS (PCIEX1) ------------------- PCI_MEM_ADRS
 * CPU_PCI_MEM_ADRS2(PCIEX2) -                 - PCI_MEM_ADRS2
 * CPU_PCI_MEM_ADRS3(PCIEX3) ------------------- PCI_MEM_ADRS3
 *                           -                 - 
 * CPU_PCI_MEMIO_ADRS        ------------------- PCI_MEMIO_ADRS
 * CPU_PCI_MEMIO_ADRS2       -                 - PCI_MEMIO_ADRS2
 * CPU_PCI_MEMIO_ADRS3       -                 - PCI_MEMIO_ADRS3
 *                           -                 - 
 * CPU_PCI_IO_ADRS (PCIEX1)  ------------------- PCI_IO_ADRS
 * CPU_PCI_IO_ADRS2(PCIEX2)  -                 - PCI_IO_ADRS2
 * CPU_PCI_IO_ADRS3(PCIEX3)  -                 - PCI_IO_ADRS3
 *                           -                 -
 * CPU_PCI_IO_ADRS (PCIEX1)  +-------------------PCI_IO_ADRS +
 * CPU_PCI_IO_ADRS2(PCIEX2)  +                   PCI_IO_ADRS2 +
 * CPU_PCI_IO_ADRS2(PCIEX3)                      PCI_IO_ADRS3
 * CPU_PCI_IO_SIZE           -                 - PCI_IO_SIZE
 *                           -                 -
 */

/* PCI based addresses */

#   define PCIEX1_LAW_BASE             0x50000000        /* PCI LAW window */
#   define PCIEX1_LAW_SIZE             0x10000000        /* PCI LAW size   */

#   define PCIEX1_MEM_SIZE             0x04000000        /* 64 MB */
#   define PCIEX1_MEMIO_SIZE           0x04000000        /* 64 MB */
#   define PCIEX1_IO_SIZE              0x04000000        /* 64 MB */

#   define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)
#   define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)
#   define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)

#   define PCIEX2_LAW_BASE             0x60000000        /* PCI LAW window */
#   define PCIEX2_LAW_SIZE             0x10000000        /* PCI LAW size   */
#   define PCIEX2_MEM_SIZE             0x04000000        /* 64 MB */
#   define PCIEX2_MEMIO_SIZE           0x04000000        /* 64 MB */
#   define PCIEX2_IO_SIZE              0x04000000        /* 64 MB */

#   define PCIEX2_MEM_ADRS             (PCIEX2_LAW_BASE)
#   define PCIEX2_MEMIO_ADRS           (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)
#   define PCIEX2_IO_ADRS              (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)

#   define PCIEX3_LAW_BASE             0x80000000        /* PCI LAW window */
#   define PCIEX3_LAW_SIZE             0x10000000        /* PCI LAW size   */

#   define PCIEX3_MEM_SIZE             0x04000000        /* 64 MB */
#   define PCIEX3_MEMIO_SIZE           0x04000000        /* 64 MB */
#   define PCIEX3_IO_SIZE              0x04000000        /* 64 MB */

#   define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)
#   define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)
#   define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)

#   define PCI_LAW_BASE                0x90000000        /* PCI LAW window */
#   define PCI_LAW_SIZE                0x10000000        /* PCI LAW size   */
#   define PCI_MEM_SIZE                0x04000000        /* 64 MB */
#   define PCI_MEMIO_SIZE              0x04000000        /* 64 MB */
#   define PCI_IO_SIZE                 0x04000000        /* 64 MB */

#   define PCI_MEM_ADRS                (PCI_LAW_BASE)
#   define PCI_MEMIO_ADRS              (PCI_MEM_ADRS   + PCI_MEM_SIZE)
#   define PCI_IO_ADRS                 (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)

#   if (PCI_LAW_SIZE == 0x10000000)
#       define  PCI_LAW_WIN_SZ         LAWAR_SIZE_256MB
#   endif

#   if (PCI2_LAW_SIZE == 0x10000000)
#       define  PCI2_LAW_WIN_SZ        LAWAR_SIZE_256MB
#   endif

#   if (PCIEX_LAW_SIZE == 0x10000000)
#       define  PCIEX_LAW_WIN_SZ       LAWAR_SIZE_256MB
#   endif

#   define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

#   if (PCI_MEM_SIZE == 0x04000000)
#       define PCI_MEM_SIZE_MASK       PCI_ATTR_WS_64M
#   endif 

#   if (PCI_MEMIO_SIZE == 0x04000000)
#       define PCI_MEMIO_SIZE_MASK     PCI_ATTR_WS_64M
#   endif

#   if (PCI_IO_SIZE == 0x04000000)
#       define PCI_IO_SIZE_MASK        PCI_ATTR_WS_64M
#   endif

#   define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* just 1:1 mapping */ 

/* PCI 1 */

#   define CPU_PCI_MEM_ADRS            PCI_MEM_ADRS
#   define CPU_PCI_MEMIO_ADRS          PCI_MEMIO_ADRS
#   define CPU_PCI_IO_ADRS             PCI_IO_ADRS

/* CPU from PCI bus */

#   define PCI_MSTR_MEM_BUS            0x00000000
#   define PCI_MSTR_MEM_SIZE           0x40000000 /* 1G */

/* CPU Address that is visible from PCI */

#   define PCI_LOCAL_MEM_BUS           0x00000000
#   define PCI_LOCAL_MEM_SIZE          PCI_MSTR_MEM_SIZE
#   define PCI_LOCAL_MEM_SIZE_MASK     PCI_ATTR_WS_1G

#endif /* INCLUDE_PCI_BUS */

#define FLASH_WINDOW_SIZE              0x00800000

/* Flash Driver pulled from cds85xx BSP */

#define CDS85XX_FLASH_RESERVED_SIZE    0x00110000

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS                0xff800000
#define FLASH_ADRS_MASK                0xff800000
#define MAIN_FLASH_SIZE                0x00800000

/* LBC CS2 - SDRAM */

/* LBC CS3 - nvram, cadmus, ATM phy */

#define INCLUDE_LBC_CS3
#ifdef  INCLUDE_LBC_CS3
#   define LBC_CS3_LOCAL_ADRS          0xf8100000
#   define LBC_CS3_LOCAL_SIZE_MASK     0xfff00000
#   define LBC_CS3_SIZE                0x00100000
#endif  /* INCLUDE_LBC_CS3 */

#define INCLUDE_FLASH
#ifdef  INCLUDE_FLASH

/*
 * 29LV640D flash is not supported in flashMem.c directly but is similar
 * in procedures with 29LV160B
 */

#   define FLASH_29LV640D              FLASH_29LV160B

#   undef  SYS_FLASH_TYPE
#   define SYS_FLASH_TYPE              FLASH_29LV640D
#   undef  FLASH_WIDTH
#   define FLASH_WIDTH                 2
#   undef  FLASH_CHIP_WIDTH
#   define FLASH_CHIP_WIDTH            2

#   define NV_RAM_ADRS                 0x00000000
#   define FLASH_SEGMENT_SIZE          0x00010000
#   define FLASH_ADRS                  0xffef0000
#   define FLASH_SIZE                  0x00010000
#   define FLASH_SIZE_WRITEABLE FLASH_SIZE  /* write all of flash */
#   define NV_RAM_SIZE                 FLASH_SIZE
#   undef FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET              0
#endif /* INCLUDE_FLASH */

/* Memory addresses */

#define INCLUDE_DDR_SDRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE        0x20000000 /* 256/512 Mbyte memory available */  
#define LOCAL_MEM_LOCAL_ADRS  0x00000000 /* Base of RAM */

#define DDR_SDRAM_LOCAL_ADRS      LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE      LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END  (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

#ifdef INCLUDE_MMU_BASIC
#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for 
 * page table and region tables and in event of running out the increment 
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM    0x40
#   define AIM_MMU_INIT_PT_INCR   0x20
#   define AIM_MMU_INIT_RT_NUM    0x10
#   define AIM_MMU_INIT_RT_INCR   0x10

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
#   undef INCLUDE_PAGE_SIZE_OPTIMIZATION /* Not available for 85XX */
#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CTORS_DTORS

#if ((TOOL==e500v2diab) || (TOOL==e500v2gnu))
    /* These are 60X specific for now */
#undef INCLUDE_HW_FP
    /* ev2diab is hard float so undefine soft float */
#undef INCLUDE_SW_FP

#else

/* 
 * Using software FP support. Athough task based 32 bit HW_FP is allowed 
 * no optimised libraries are available. The kernel doesn't execute any hw 
 * fp instructions 
 */

#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP
#endif

/*
 * The constants ROM_XXX_ADRS, ROM_SIZE, and RAM_XXX_ADRS are defined
 * in both config.h and Makefile.  All definitions for these constants
 * must be identical.
 */

#define ROM_TEXT_ADRS 0xFFF00100
#define ROM_BASE_ADRS 0xFFF00000
#define ROM_SIZE      0x00100000

#define USER_RESERVED_MEM       0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif /* PRJ_BUILD */
