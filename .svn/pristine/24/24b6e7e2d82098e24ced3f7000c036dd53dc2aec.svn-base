/* config.h - Freescale MDS837x-PCI board configuration header */

/*
 * Copyright (c) 2003, 2007-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,19jul12,clx  update BSP_REV
01m,18jan12,x_s  correct SYS_MODEL vlaue. (WIND00282918)
01l,15apr10,clx  update BSP version for vxWorks 6.9
01k,03mar10,x_z  adjust PCIe memory map to fit new PCIe driver.
01j,10sep09,x_z  code clean.
01i,10sep09,x_z  update BSP_REV to "/6".
01h,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01g,12aug08,x_s  remove RAM_HIGH_ADRS and RAM_LOW_ADRS
01f,08jul08,x_s  update BSP_REV to "/5"
01e,27feb08,x_s  update BSP_REV to "/4"
01d,15feb08,x_s  update BSP_REV to "/3"
01c,23jan08,x_s  update BSP_REV to "/2"
01b,20oct07,x_s  modified for mds837x.
01a,18dec03,dtr  adapted from ads834x config.h/01k
*/

/*
This file contains the configuration parameters for the
Freescale MPC837xE MDS board.
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

/* Base model string. sysModel adds processor details */

#define SYS_MODEL       "Freescale MPC837"
#define SYS_MODEL_8377  "Freescale MPC8377 MDS"
#define SYS_MODEL_8378  "Freescale MPC8378 MDS"
#define SYS_MODEL_8379  "Freescale MPC8379 MDS"
#define SYS_MODEL_8377E "Freescale MPC8377E MDS"
#define SYS_MODEL_8378E "Freescale MPC8378E MDS"
#define SYS_MODEL_8379E "Freescale MPC8379E MDS"

/* define if U-Boot boots us, undef if VxWorks bootrom boots us */

#undef BOOT_FROM_UBOOT

#include "configAll.h"

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks tn=mds837x"

/* MMU configuration */

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE
#   define USER_I_CACHE_ENABLE

/*
 * Does nothing about copyback vs writethrough in h/w, must use sysPhysMemDesc.
 */

#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE  CACHE_COPYBACK | CACHE_SNOOP_ENABLE

#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif /* INCLUDE_CACHE_SUPPORT */

/* vxBus drivers */

/* common */

#define INCLUDE_VXBUS

#define INCLUDE_VXB_CMDLINE
#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define HWMEM_POOL_SIZE     200000

/* interrupt controller */

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC

/* timer */

#define DRV_TIMER_DEC_PPC
#define DRV_TIMER_QUICC_PPC
#define INCLUDE_TIMER_SYS

/* SIO */

#define INCLUDE_SIO_UTILS
#define DRV_SIO_NS16550

/* ethernet */

#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY
#define INCLUDE_GENERICPHY

/* USB */

#ifdef INCLUDE_EHCI
#   define INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI_BUS
#   define INCLUDE_EHCI_INIT

#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT

#   define INCLUDE_DOSFS
#endif /* INCLUDE_EHCI */

/* PCI/PCIe */

/*
 * DRV_PCIBUS_M83XX is not switched on by default as may hang board unless
 * connected to PIB or plugged into base board with correct arbitration.
 */

#if defined(DRV_PCIBUS_M83XX) || defined(DRV_PCIBUS_M83XX_EX)

#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF

/* PCI/PCIe Inbound Windows (PCI/PCIe based addresses) */

#   define PCI_MSTR_MEM_BUS             0x00000000
#   define PCI_MSTR_MEM_SIZE            LOCAL_MEM_SIZE

#   define PCI_BRIDGE_PIMMR_BASE_ADRS   0x40000000      /* fixed 1MB space */

/* PCI configuration */

#   ifdef DRV_PCIBUS_M83XX

/* PCI Local Access Windows Configuration */

#       define PCI_LAW_BASE             0x80000000
#       define PCI_LAW_SIZE             0x20000000      /* 512MB */

/* Outbound Windows */

/* PCI based addresses */

#       define PCI_MEM_ADRS             0x80000000
#       define PCI_MEM_SIZE             0x08000000      /* 128MB */

#       define PCI_MEMIO_ADRS           0x88000000
#       define PCI_MEMIO_SIZE           0x08000000      /* 128MB */

#       define PCI_IO_ADRS              0x90000000
#       define PCI_IO_SIZE              0x10000000      /* 256MB */

/* CPU based addresses */

#       define CPU_PCI_MEM_ADRS         0x80000000
#       define CPU_PCI_MEM_SIZE         PCI_MEM_SIZE

#       define CPU_PCI_MEMIO_ADRS       0x88000000
#       define CPU_PCI_MEMIO_SIZE       PCI_MEMIO_SIZE

#       define CPU_PCI_IO_ADRS          0x90000000
#       define CPU_PCI_IO_SIZE          PCI_IO_SIZE
#   endif /* DRV_PCIBUS_M83XX */

/* PCIe configuration */

#   ifdef DRV_PCIBUS_M83XX_EX

/* PCI Local Access Windows Configuration */

#       define PCIEX1_LAW_BASE          0xA0000000
#       define PCIEX1_LAW_SIZE          0x20000000      /* 512MB */

#       define PCIEX2_LAW_BASE          0xC0000000
#       define PCIEX2_LAW_SIZE          0x20000000      /* 512MB */

/* Outbound Windows */

/* CPU based addresses */

/*
 * Outbound Window 0 - Config Window
 *
 * Only bit 5-7 is used to map bus number, so 8 buses is supported at most. The
 * Config Window must be adjusted for more buses. (For example, 0xA0000000 -
 * 0xB0000000 for 16 buses at most.)
 */

#       define CPU_PCIEX1_CFG_BASE      0xA0000000
#       define CPU_PCIEX1_CFG_SIZE      0x08000000      /* 128MB    */
#       define PCIEX1_MAX_BUS           8
#       define CPU_PCIEX2_CFG_BASE      0xC0000000
#       define CPU_PCIEX2_CFG_SIZE      0x08000000      /* 128MB    */
#       define PCIEX2_MAX_BUS           8

/* Outbound Window 1 */

#       define CPU_PCIEX1_MEM_ADRS      0xA8000000
#       define CPU_PCIEX1_MEM_SIZE      0x08000000      /* 128MB  */
#       define CPU_PCIEX2_MEM_ADRS      0xC8000000
#       define CPU_PCIEX2_MEM_SIZE      0x08000000      /* 128MB  */

/* Outbound Window 2 */

#       define CPU_PCIEX1_MEMIO_ADRS    0xB0000000
#       define CPU_PCIEX1_MEMIO_SIZE    0x08000000      /* 128MB  */
#       define CPU_PCIEX2_MEMIO_ADRS    0xD0000000
#       define CPU_PCIEX2_MEMIO_SIZE    0x08000000      /* 128MB  */

/* Outbound Window 3 */

#       define CPU_PCIEX1_IO_ADRS       0xB8000000
#       define CPU_PCIEX1_IO_SIZE       0x08000000      /* 128MB  */
#       define CPU_PCIEX2_IO_ADRS       0xD8000000
#       define CPU_PCIEX2_IO_SIZE       0x08000000      /* 128MB  */

/* PCI based addresses */

/* Outbound Window 1 */

#       define PCIEX1_MEM_ADRS          CPU_PCIEX1_MEM_ADRS
#       define PCIEX1_MEM_SIZE          CPU_PCIEX1_MEM_SIZE
#       define PCIEX2_MEM_ADRS          CPU_PCIEX2_MEM_ADRS
#       define PCIEX2_MEM_SIZE          CPU_PCIEX2_MEM_SIZE

/* Outbound Window 2 */

#       define PCIEX1_MEMIO_ADRS        CPU_PCIEX1_MEMIO_ADRS
#       define PCIEX1_MEMIO_SIZE        CPU_PCIEX1_MEMIO_SIZE
#       define PCIEX2_MEMIO_ADRS        CPU_PCIEX2_MEMIO_ADRS
#       define PCIEX2_MEMIO_SIZE        CPU_PCIEX2_MEMIO_SIZE

/* Outbound Window 3 */

#       define PCIEX1_IO_ADRS           CPU_PCIEX1_IO_ADRS
#       define PCIEX1_IO_SIZE           CPU_PCIEX1_IO_SIZE
#       define PCIEX2_IO_ADRS           CPU_PCIEX2_IO_ADRS
#       define PCIEX2_IO_SIZE           CPU_PCIEX2_IO_SIZE

#   endif /* DRV_PCIBUS_M83XX_EX */

#endif /* defined(DRV_PCIBUS_M83XX) || defined(DRV_PCIBUS_M83XX_EX) */

/* TFFS and SATA */

#if defined(INCLUDE_TFFS) || defined(DRV_STORAGE_FSLSATA)
#   define INCLUDE_DOSFS
#endif /* defined(INCLUDE_TFFS) || defined(DRV_STORAGE_FSLSATA) */

/* DOSFS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_SHOW
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
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

/* Local Access Windows Configuration */

/* Memory addresses */

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE                  0x20000000 /* 512MB */
#define LOCAL_MEM_LOCAL_ADRS            0
#define USER_RESERVED_MEM               0

/* LBC CS0 - 8MB 16-bit flash */

#define FLASH_BASE_ADRS                 0xFE000000
#define FLASH_SIZE                      0x02000000 /* 32MB */

/* LBC CS1 - 32KB 8-bit BCSR */

#define BCSR_BASE_ADRS                  0xF8000000
#define BCSRS_SIZE                      0x00008000

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and
 * Makefile.
 * All definitions for these constants must be identical.
 */

/*
 * boot from fe000100 by default, or boot from fff00100 if define, but
 * ROM_TEXT_ADRS and other related macros need be adjusted accordingly in
 * Makefile and SW5[7] need be changed to OFF.
 */

#undef BMS_HIGH_ENABLE

/* Boot ROM address space. */

#ifdef BMS_HIGH_ENABLE
#   define ROM_BASE_ADRS                0xfff00000
#else
#   define ROM_BASE_ADRS                FLASH_BASE_ADRS
#endif /* BMS_HIGH_ENABLE */

#ifdef BOOT_FROM_UBOOT
#   define UBOOT_SIZE                   0x100000
#   define ROM_TEXT_ADRS                (ROM_BASE_ADRS + UBOOT_SIZE+ 0x100)
#else
#   define ROM_TEXT_ADRS                (ROM_BASE_ADRS + 0x100)
#endif /* BOOT_FROM_UBOOT */

#define ROM_SIZE                        0x100000 /* 1MB */
#define ROM_WARM_ADRS                   (ROM_TEXT_ADRS + 8)

/* NVRAM configuration */

#define SYS_FLASH_TYPE                  FLASH_29GL256N
#define FLASH_WIDTH                     2
#define FLASH_CHIP_WIDTH                2
#define FLASH_SECTOR_SIZE               0x20000
#define FLASH_ADRS                      FLASH_BASE_ADRS

#undef  FLASH_NO_OVERLAY
#undef  FLASH_SIZE_WRITEABLE

#define NV_RAM_SIZE                     FLASH_SECTOR_SIZE

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET                  0
#define NV_MAC_ADRS_OFFSET              0x200

#ifdef BMS_HIGH_ENABLE
#  define NV_RAM_ADRS                   (FLASH_ADRS + S29GL_FLASH_SIZE)
#else
#  define NV_RAM_ADRS                   (FLASH_ADRS + S29GL_RESERVED_SIZE - \
                                         NV_RAM_SIZE)
#endif /* BMS_HIGH_ENABLE */

/* TFFS configuration */

#define S29GL_FLASH_BASE                FLASH_BASE_ADRS

#ifdef BMS_HIGH_ENABLE
#   define S29GL_RESERVED_SIZE          0
#   define S29GL_FLASH_SIZE             (FLASH_SIZE / 2)
#else
#   define S29GL_RESERVED_SIZE          (FLASH_SIZE / 2)
#   define S29GL_FLASH_SIZE             FLASH_SIZE
#endif /* BMS_HIGH_ENABLE */

/* serial line */

#undef  CONSOLE_TTY
#define CONSOLE_TTY                     0 /* 0 = UART1, 1 = UART2 */

#ifdef BOOT_FROM_UBOOT
#   undef  CONSOLE_BAUD_RATE
#   define CONSOLE_BAUD_RATE            115200
#endif /* BOOT_FROM_UBOOT */

/* clock rates */

#define SYS_CLK_RATE_MIN                1    /* minimum system clock rate    */
#define SYS_CLK_RATE_MAX                8000 /* maximum system clock rate    */
#define AUX_CLK_RATE_MIN                1    /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX                8000 /* maximum auxiliary clock rate */

/* default mac address */

#define FSL_ENET0                       0x00
#define FSL_ENET1                       0x04
#define FSL_ENET2                       0x9F

#define CUST_ENET3_0                    0xA0
#define CUST_ENET3_1                    0xA1
#define CUST_ENET4                      0xAA
#define CUST_ENET5                      0xA0

#define ENET_DEFAULT0                   FSL_ENET0
#define ENET_DEFAULT1                   FSL_ENET1
#define ENET_DEFAULT2                   FSL_ENET2

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define MAX_MAC_DEVS                    1 /* one network devices */
#define MAX_MAC_ADRS                    2
#define MAC_ADRS_LEN                    6

/* add on-chip drivers */

/*
 * PIB IO board support
 *
 * Note proto version of PIB (Platform IO base Board) has some
 * issues.  Remove for default but add if required.
 * Some GEI cards fail to work SPR113700.
 */

#undef INCLUDE_PIB_SUPPORT

/* I2C and RTC */

#undef INCLUDE_I2C
#undef INCLUDE_RTC

/*
 * Default power management mode - selected via vxPowerModeSet() in
 * sysHwInit().
 */

#define DEFAULT_POWER_MGT_MODE  VX_POWER_MODE_DISABLE

/*
 * Hard Reset Configuration Words
 *
 * DIP switches is the source by default. the following can be ignored.
 */

/* See MPC8379E hardware specification for supported clock freq */

/*
 * spmf CSB
 * LBCM = 1:1 DDRCM 1:1 with CSB
 * SVCOD = Divisor 2
 * SPMF = 6 * CLKIN (66Mhz)
 */

#define HRCW_LOW_BYTE0  0x26

/* corepll ratio 3/2 * platform = 3/2*6*66 == 594Mhz */

#define HRCW_LOW_BYTE1  0x03

/* Must be cleared */

#define HRCW_LOW_BYTE2  0x00

/* Must be cleared */

#define HRCW_LOW_BYTE3  0x00

/* HRCW_HIGH_BMS_HIGH should be set if BMS_HIGH_ENABLE is defined */

#define HRCW_HIGH_BYTE0	 HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI_ARB /*| HRCW_HIGH_BMS_HIGH*/

#define HRCW_HIGH_BYTE1  ROMLOC_GPCM_16BIT

/* RGMII */

#define HRCW_HIGH_BYTE2  HRCW_HIGH_TSEC12M_RGMII

/* Big Endian */

#define HRCW_HIGH_BYTE3  HRCW_HIGH_LDP

#include "mds837x.h"            /* include the mds837x params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif

