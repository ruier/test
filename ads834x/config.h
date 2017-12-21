/* ads834x/config.h - Motorola ADS834x-PCI board configuration header */

/*
 * Copyright (c) 2003, 2005-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02e,19jul12,clx  update BSP_REV
02d,17apr10,pch  nvram & flash config adjustments
02c,15apr10,clx  Update BSP version for vxWorks 6.9
02b,09sep09,s_z  Integrate the USB TCD support by INCLUDE_FSL_TCD(WIND00180575)
02a,17dec08,z_l  Increased rev of BSP.
01z,16dec08,z_l  Support USB host and target. (WIND00118187)
01y,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01x,24jul08,z_l  VxWorks 6.7 update, support MDS8349EA.
01w,14may08,wap  Switch to VxBus TSEC driver
01v,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
01u,21aug07,mmi  remove legacy power management define
01t,14aug07,jmt  Add SYS_MODEL define
01s,20jul07,pdg  added m83xx pci support
01r,13jul07,dtr  Update for vxbus interrupt controller driver.
01q,09jul07,p_g  added macro INCLUDE_VXB_CMDLINE
01p,20jun07,sup  serial driver vxBus compliant
01o,07may07,???  added INCLUDE_DMA_SYS
01n,29jan07,rec  vxWorks 6.5 updates
01m,12oct06,dtr  Mod for rev3 silicon.
01l,10oct06,sup  Changes for supporting vxBus complaint quicc timer driver
01k,30aug06,dtr  Increment rev of BSP.
01j,02may06,dtr  Allow BMS_HIGH option.
01i,13sep05,dtr  Define local USB by default.
01h,07sep05,dtr  Clk freq set for 400Mhz by default for general part use.
01g,15aug05,dtr  Setup CPU specific includes and HRCW settings.
01f,02aug05,dtr  Add support for PCI2 and TSEC MAC in Flash.
01e,16may05,dtr  Use a suggested HRCW clk configurations for all variants.
01d,22apr05,dtr  Clocking the CPU a little too high.
01c,31mar05,dtr  Add in BSP_NAME.
01b,13jun05,pcm  removed INCLUDE_DOSFS
01a,18dec03,dtr  adapted from ads827x config.h
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
Motorola MPC8349E ADS board.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* BSP version/revision identification, should be placed
 * before #include "configAll.h"
 */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */
#define BSP_NAME     ads834x

/* Define Clock Speed and source  */

#define FREQ_33_MHZ      33000000
#define FREQ_40_MHZ      40000000
#define FREQ_66_MHZ      66000000
#define FREQ_100_MHZ     100000000
#define FREQ_266_MHZ     266000000
#define FREQ_333_MHZ     333000000

/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 or the PCI freq of the ADS834x board.
 * Choose from above list. BSP assumes standalone board ie not in
 * PCI chassis
 */

#define OSCILLATOR_FREQ    FREQ_66_MHZ

#undef  REV1_0_SILICON
#define REV1_1_SILICON
#undef  REV2_0_SILICON
#undef  REV3_SILICON

#ifdef REV3_SILICON
#   define REV2_0_SILICON
#endif	/* REV3_SILICON */

/*
 * This macro is optional selected to support MDS8349EA board, if not defined,
 * then the BSP will support ADS board.
 */

#define MPC8349EA_MDS

#ifdef MPC8349EA_MDS
#   undef REV1_0_SILICON
#   undef REV1_1_SILICON
#   undef REV2_0_SILICON
#   define REV3_SILICON

#   ifdef REV3_SILICON
#       define REV2_0_SILICON
#   endif /* REV3_SILICON */
#endif /* MPC8349EA_MDS */

#if defined(REV1_0_SILICON) || defined(REV1_1_SILICON)
#undef ERRATA_LBC2 /* definition to fix errata LBC2 */
#endif	/* REV1_0_SILICON || REV1_1_SILICON */

/*
 * These default values assume CSB is 333Mhz based on HRCW setting in
 * boot image
 */

#ifdef REV2_0_SILICON

/*
 * This bus speed not supported until rev2 silicon so default 266Mhz
 * See also HRCW at end of file.
 */

#   define SYS_CLK_FREQ         FREQ_333_MHZ
#else  /* REV2_0_SILICON */
#   define SYS_CLK_FREQ         FREQ_266_MHZ
#endif /* REV2_0_SILICON */

#define DEC_CLK_TO_INC          4            /* bus clks per increment*/
#define DEC_CLOCK_FREQ          SYS_CLK_FREQ /* Set to system default */
#define TIMESTAMP_HZ            SYS_CLK_FREQ

#define TPR                     0x2000
#define LSRT_VALUE              0x20

#include "configAll.h"


/* define only one of the following CPU*/

#define MPC8349
#undef  MPC8347 /* no secondary PCI controller */
#undef  MPC8343 /* selects 32 bit DDR and lower clock - requires new bootrom*/

#define DEFAULT_BOOT_LINE \
"mottsec(0,0)host:target/config/ads834x/vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=ads834x"

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif  /* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE

/*
 * Does nothing about copyback vs writethrough in h/w,
 * must use sysPhysMemDesc
 */

#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE  (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif  /* INCLUDE_CACHE_SUPPORT */

/* Number of TTY definition */

#undef  NUM_TTY
#define NUM_TTY        N_SIO_CHANNELS    /* defined in ads827x.h */

/* Optional timestamp support */

#define INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PLB_BUS
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC
#define DRV_TIMER_QUICC_PPC
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC
#define INCLUDE_PARAM_SYS
#define DRV_SIO_NS16550       /* serial driver */
#define INCLUDE_SIO_UTILS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000

#define INCLUDE_END
#define INCLUDE_TSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY
#define INCLUDE_GENERICPHY
#define INCLUDE_PARAM_SYS

#undef INCLUDE_DMA

/* clock rates */

#define SYS_CLK_RATE_MIN    1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */

/* add on-chip drivers */

#undef INCLUDE_SECURITY_ENGINE     /* only supported if chip has E ie 8349E */

#undef  INCLUDE_MPC8349_USB         /* For USB host operations */

#ifdef  INCLUDE_MPC8349_USB
#   ifdef INCLUDE_FSL_TCD
#       error ERROR - Cannot define both INCLUDE_MPC8349_USB and INCLUDE_FSL_TCD within this BSP
#   endif /* INCLUDE_FSL_TCD */

#   define INCLUDE_USB

    /* USB 2.0 Driver */

#   define INCLUDE_EHCI

    /* Mass Storage */

#   define INCLUDE_USB_MS_BULKONLY

#   ifdef INCLUDE_EHCI
#       define INCLUDE_MPC834X_USB  /* Initialize onchip EHCI */
#   endif /* INCLUDE_EHCI */

#   ifndef INCLUDE_USBTOOL
#       define INCLUDE_USB_INIT

#       ifdef INCLUDE_EHCI
#           define INCLUDE_EHCI_INIT
#       endif /* INCLUDE_EHCI */

#       ifdef INCLUDE_OHCI
#           define INCLUDE_OHCI_INIT
#       endif /* INCLUDE_OHCI */

#       ifdef INCLUDE_UHCI
#           define INCLUDE_UHCI_INIT
#       endif /* INCLUDE_UHCI */

#       ifdef INCLUDE_USB_KEYBOARD
#           define INCLUDE_USB_KEYBOARD_INIT
#       endif /* INCLUDE_USB_KEYBOARD */

#       ifdef INCLUDE_USB_MOUSE
#           define INCLUDE_USB_MOUSE_INIT
#       endif /* INCLUDE_USB_MOUSE */

#       ifdef INCLUDE_USB_SPEAKER
#           define INCLUDE_USB_SPEAKER_INIT
#       endif /* INCLUDE_USB_SPEAKER */

#       ifdef INCLUDE_USB_MS_BULKONLY
#           define BULK_MAX_DEVS 4
#           define INCLUDE_USB_MS_BULKONLY_INIT
#           define INCLUDE_DOSFS
#       endif /* INCLUDE_USB_MS_BULKONLY */
#   endif /* INCLUDE_USBTOOL */

#endif /* INCLUDE_MPC8349_USB */


/* Defines for USB Device support */

#undef INCLUDE_FSL_TCD

#ifdef INCLUDE_FSL_TCD
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif  /* _ASMLANGUAGE */
#   define FSL_USB_DR_DEVICE_IO_BASE      M834X_DR_USB_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      INUM_USB2_DR
#   define FSL_USB_DR_DEVICE_HW_INIT      fslUsbDrDeviceHwInit
#   define FSL_USB_DR_DEVICE_BE_REG       FALSE
#   define FSL_USB_DR_DEVICE_BE_DESC      FALSE
#   define FSL_USB_DR_DEVICE_BE_SETUP     TRUE
#   define FSL_USB_DR_DEVICE_SYS_CONF     TRUE
#   define FSL_USB_DR_DEVICE_BUFFER_ES    FALSE
#   define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_ULPI
#   define INCLUDE_USB_TARG
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT
#endif /* INCLUDE_FSL_TCD */


#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif /* INCLUDE_MS_EMULATOR */

/* Define for FS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_ERF
#   define INCLUDE_XBD
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_XBD_PART_LIB
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_XBD_BLKDEV
#endif /* INCLUDE_DOSFS */


/*
 * Note proto version of PIB (Platform IO base Board) has some
 * issues.  Remove for default but add if required.
 * Some GEI cards fail to work SPR113700.
 */

#undef INCLUDE_PIB_SUPPORT   /* PIB IO board support */

#undef INCLUDE_PCI            /* include PCI library support */

#undef INCLUDE_TFFS

#ifdef INCLUDE_TFFS
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_TL_FTL
#endif /* INCLUDE_TFFS */

#if defined(INCLUDE_TFFS) || defined(INCLUDE_USB)
#   define INCLUDE_DOSFS
#endif /* INCLUDE_TFFS || INCLUDE_USB */


/* include PCI library support */

#ifdef INCLUDE_PCI
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_M83XX
#   define INCLUDE_FEI8255X_VXB_END
#   ifndef INCLUDE_PIB_SUPPORT
#       define INCLUDE_PIB_SUPPORT
#endif /* INCLUDE_PIB_SUPPORT */
#endif /* INCLUDE_PCI */

#ifdef MPC8349

/* Don't define as default as 64bit PCI1 likely more popular */

#   undef INCLUDE_SECONDARY_PCI   /* only supported for 8349 */

#endif /* MPC8349 */

#ifdef  INCLUDE_PCI
#   ifndef REV1_0_SILICON
#       define INCLUDE_PCI_AUTOCONF
#   else  /* REV1_0_SILICON */
#       undef INCLUDE_PCI_AUTOCONF
#   endif /* REV1_0_SILICON */

/*
 * CPU Addr                                       PCI Addr
 * PCI_LOCAL_MEM_BUS    ------------------------- PCI_MSTR_MEM_BUS
 *                      -                -
 *                      -                -
 * PCI_LOCAL_MEM_BUS +  ------------------------- PCI_MSTR_MEM_BUS +
 * PCI_LOCAL_MEM_SIZE   -                -        PCI_MSTR_MEM_SIZE
 *                      -                -
 * IMMR                 ------------------------- PIMMR
 *                      -                -
 *                      -                -
 * CPU_PCI_MEM_ADRS     ------------------------- PCI_MEM_ADRS
 *                      -                -
 *                      -                -
 * CPU_PCI_MEMIO_ADRS   ------------------------- PCI_MEMIO_ADRS
 *                      -                -
 *                      -                -
 * CPU_PCI_IO_ADRS      ------------------------- PCI_IO_ADRS
 *                      -                -
 *                      -                -
 * CPU_PCI_IO_ADRS +    ------------------------- PCI_IO_ADRS +
 * CPU_PCI_IO_SIZE      -                -        PCI_IO_SIZE
 *                      -                -
 *                      -                -
 *                      ------------------------- 4GBytes
 */

/* for custom sysPciAutoConfig.c */

#   define PCI1_LAW_BASE        0x80000000       /* PCI LAW window */
#   define PCI1_LAW_SIZE        0x10000000       /* PCI LAW size   */

/* PCI based addresses */

#   define PCI_MEM_ADRS         0x80000000
#   define PCI_MEM_SIZE         0x08000000       /* 16MB */
#   define PCI_MEM_SIZE_MASK    PCI_SIZE_128MB
#   define PCI_MEMIO_ADRS       0x88000000
#   define PCI_MEMIO_SIZE       0x04000000       /* 16MB */
#   define PCI_MEMIO_SIZE_MASK  PCI_SIZE_64MB

#   define PCI_IO_ADRS          0x8c000000
#   define PCI_IO_SIZE          0x04000000       /* IO Space is not available */
#   define PCI_IO_SIZE_MASK     PCI_SIZE_64MB

#   ifdef INCLUDE_SECONDARY_PCI

#       define PCI2_LAW_BASE        0x90000000       /* PCI LAW window */
#       define PCI2_LAW_SIZE        0x10000000       /* PCI LAW size   */

#       define PCI2_MEM_SIZE        0x08000000       /* 16MB */
#       define PCI2_MEMIO_SIZE      0x08000000       /* 16MB */
#       define PCI2_MEM_SIZE_MASK   PCI_SIZE_128MB
#       define PCI2_MEM_ADRS        (PCI2_LAW_BASE)
#       define PCI2_MEMIO_ADRS      (PCI2_MEM_ADRS   + PCI2_MEM_SIZE)
#       define PCI2_IO_ADRS         (PCI2_MEMIO_ADRS + PCI2_MEMIO_SIZE)
#       define PCI2_IO_SIZE         0x10000000   /* IO Space is not available */

/* CPU based addresses */

#       define CPU_PCI2_MEM_ADRS    0x90000000
#       define CPU_PCI2_MEM_SIZE    PCI_MEM_SIZE
#       define CPU_PCI2_MEMIO_ADRS  0x98000000
#       define CPU_PCI2_MEMIO_SIZE  PCI_MEMIO_SIZE
#       define CPU_PCI2_IO_ADRS     0xa0000000
#       define CPU_PCI2_IO_SIZE     PCI_IO_SIZE

#   endif /* INCLUDE_SECONDARY_PCI */

/* CPU based addresses */
#   define CPU_PCI_MEM_ADRS        0x80000000
#   define CPU_PCI_MEM_SIZE        PCI_MEM_SIZE
#   define CPU_PCI_MEMIO_ADRS      0x88000000
#   define CPU_PCI_MEMIO_SIZE      PCI_MEMIO_SIZE
#   define CPU_PCI_IO_ADRS         0x8c000000
#   define CPU_PCI_IO_SIZE         PCI_IO_SIZE

/* CPU from PCI bus */

#   define PCI_MSTR_MEM_BUS            0x00000000
#   define PCI_MSTR_MEM_SIZE           PCI_LOCAL_MEM_SIZE
#   define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* CPU Address that is visible from PCI */

#   define PCI_LOCAL_MEM_BUS           LOCAL_MEM_LOCAL_ADRS
#   define PCI_LOCAL_MEM_SIZE          LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#   define PCI_LOCAL_MEM_SIZE_MASK     PCI_SIZE_256MB

#   ifndef PCI_CFG_TYPE
#       ifdef INCLUDE_PCI_AUTOCONF
#           define PCI_CFG_TYPE PCI_CFG_AUTO
#       else  /* INCLUDE_PCI_AUTOCONF */
#           define PCI_CFG_TYPE PCI_CFG_FORCE
#       endif /* INCLUDE_PCI_AUTOCONF */
#   endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI */

/* default mac address */

#define ENET_DEFAULT0 0x00
#define ENET_DEFAULT1 0xA0
#define ENET_DEFAULT2 0x1E

/*
 * Both TSEC MAC address and bootline is stored in top of flash.
 * If you want to move bootimage here must move elsewhere in flash.
 * Requires 1 segment
 */

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#   ifdef MPC8349EA_MDS
#	define FLASH_29GL256N        0x7e
#	define SYS_FLASH_TYPE        FLASH_29GL256N
#	define FLASH_WIDTH           2
#	define FLASH_CHIP_WIDTH      2
#	define FLASH_SEGMENT_SIZE    0x20000
#	define FLASH_ADRS            0xfe000000
#	define FLASH_SECTOR_SIZE     0x20000     /* sector size of S29GL256N */
#	define FLASH_SIZE            0x2000000
#	define FLASH_WIDTH_SPECIAL_2
#	undef FLASH_NO_OVERLAY
#	define NV_RAM_SIZE           FLASH_SECTOR_SIZE
#	define NV_RAM_ADRS           ((ROM_TEXT_ADRS - ROM_CODE_OFFSET) + \
				     0x200000 - NV_RAM_SIZE)
#	undef  NV_BOOT_OFFSET
#	define NV_BOOT_OFFSET        0
#   else /* MPC8349 ADS */
#	define SYS_FLASH_TYPE        FLASH_28F640J3A    /* flash device type */
#	define FLASH_WIDTH           2
#	define FLASH_CHIP_WIDTH      1
#	define FLASH_SEGMENT_SIZE    0x10000
#	define FLASH_ADRS            0xfffe0000
#	define FLASH_SIZE            0x10000
#	define FLASH_SIZE_WRITEABLE  0x10000
#	define NV_RAM_SIZE           FLASH_SIZE
#	define FLASH_WIDTH_SPECIAL_2
#	undef FLASH_NO_OVERLAY
#	undef  NV_BOOT_OFFSET
#	define NV_BOOT_OFFSET        0
#   endif /* MPC8349EA_MDS */
#define	INCLUDE_NVRAM
#define	INCLUDE_NVRAM_WRITE
#define	INCLUDE_FLASH_WRITE
#endif /* INCLUDE_FLASH */

#ifndef MPC8349EA_MDS
#   define FLASH_BASE_ADRS      0xff800000
#   define FLASH_WINDOW_SIZE    0x00400000
#else  /* MPC8349EA_MDS */
#   define FLASH_BASE_ADRS      0xfe000000
#   define FLASH_WINDOW_SIZE    0x02000000
#endif /* MPC8349EA_MDS */

/* Memory addresses */

#define LOCAL_MEM_SIZE          0x10000000  /* 256 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000  /* Base of RAM */
#define INCLUDE_LBC_SDRAM
#define INCLUDE_DDR_SDRAM
#define INCLUDE_SECONDARY_DRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE2            0x04000000   /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS2      0x10000000   /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK  0xfc000000
#define LBC_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE2
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

/*
 * USE_32BIT_DDR enables a switch to 32bit DDR mode but upper address lines
 * must be left unconnected so assume 8343 specific board required to support
 * this mode.
 */

#ifdef MPC8343
#   define USE_32BIT_DDR
#endif /* MPC8343 */

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

/*
 * #define BMS_HIGH_ENABLE  boot from fff00100 - adjust ROM_TEXT_ADRS etc
 * accordingly in Makefile
 */

#ifdef BMS_HIGH_ENABLE /*Note Makefile needs to be modified for this option*/
#   define ROM_BASE_ADRS        0xfff00000      /* base address of ROM */
#else  /* BMS_HIGH_ENABLE */
#   ifndef MPC8349EA_MDS
#      define ROM_BASE_ADRS     0xff800000      /* base address of ROM */
#   else  /* MPC8349EA_MDS */
#      define ROM_BASE_ADRS     0xfe000000      /* base address of ROM */
#   endif /* MPC8349EA_MDS */
#endif /* BMS_HIGH_ENABLE */

#ifdef MPC8349EA_MDS
#   define ROM_CODE_OFFSET  0x100
#   define ROM_TEXT_ADRS    (ROM_BASE_ADRS + ROM_CODE_OFFSET)
#   define ROM_SIZE         0x2000000           /* ROM space */
#else  /* MPC8349EA_MDS */
#   define ROM_TEXT_ADRS    (ROM_BASE_ADRS + 0x100)
#   define ROM_SIZE         0x100000            /* ROM space */
#endif /* MPC8349EA_MDS */

#define ROM_WARM_ADRS       (ROM_TEXT_ADRS + 8) /* warm reboot entry */

#define USER_RESERVED_MEM   0x00000000      /* user reserved memory size */

/* Hard Reset Configuration Words */

/* See MPC8349E hardware specification for supported clock freq */
/* More information in target.ref */

#if (defined(MPC8349) || defined(MPC8347))

/* Only Rev2 silicon will support this clock speed */

#   ifdef REV2_0_SILICON

/* spmf 1:5 ie 5*66Mhz == 333Mhz CSB*/

#       define HRCW_LOW_BYTE0   0x05

/* corepll ratio 1:2 667Mhz  */

#       define HRCW_LOW_BYTE1   0x04

/* Must be cleared*/

#       define HRCW_LOW_BYTE2   0x00

/* Must be cleared*/

#       define HRCW_LOW_BYTE3   0x00
#   else  /* REV2_0_SILICON */

/* spmf 1:5 ie 4*66Mhz == 266Mhz CSB*/

#       define HRCW_LOW_BYTE0   0x04

/* corepll ratio 1:1.5 400Mhz core */

#       define HRCW_LOW_BYTE1   0x03

/* Must be cleared*/

#       define HRCW_LOW_BYTE2   0x00

/* Must be cleared*/

#       define HRCW_LOW_BYTE3   0x00
#   endif /* REV2_0_SILICON */
#else /* defined(MPC8349) || defined(MPC8347) */

/* spmf 1:5 ie 4*66Mhz == 266Mhz CSB*/

#   define HRCW_LOW_BYTE0       0x04

/* corepll ratio 1:1.5 400Mhz core */

#   define HRCW_LOW_BYTE1       0x03

/* Must be cleared*/

#   define HRCW_LOW_BYTE2       0x00

/* Must be cleared*/

#   define HRCW_LOW_BYTE3       0x00

#endif /* (defined(MPC8349) || defined(MPC8347)) */

#ifdef INCLUDE_SECONDARY_PCI

/* Pci host, 2 * 32 bit buses,arbiters disabled */

#   ifdef BMS_HIGH_ENABLE
#       define HRCW_HIGH_BYTE0     ( HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | \
				     HRCW_HIGH_PCI2_ARB | HRCW_HIGH_BMS_HIGH )
#   else  /* BMS_HIGH_ENABLE */
#       define HRCW_HIGH_BYTE0     ( HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | \
				     HRCW_HIGH_PCI2_ARB )
#   endif /* BMS_HIGH_ENABLE */
#else  /* INCLUDE_SECONDARY_PCI */

/* Not using PCI2 so assume 64bit PCI1 */

#   ifdef BMS_HIGH_ENABLE
#       define HRCW_HIGH_BYTE0     ( HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | \
				     HRCW_HIGH_PCI64 | HRCW_HIGH_BMS_HIGH )
#   else  /* BMS_HIGH_ENABLE */
#       define HRCW_HIGH_BYTE0     ( HRCW_HIGH_PCI_HOST | HRCW_HIGH_PCI1_ARB | \
				     HRCW_HIGH_PCI64 )
#   endif /* BMS_HIGH_ENABLE */
#endif /* INCLUDE_SECONDARY_PCI */

/* Rom Location Flash 16 bit. Watch dog disabled  */

#define HRCW_HIGH_BYTE1  ROMLOC_GPCM_16BIT

/* GMII */

#define HRCW_HIGH_BYTE2  HRCW_HIGH_TSEC12M_GMII /* Both TSECs use GMII */

/* Big Endian */

#define HRCW_HIGH_BYTE3  0x00

/* add necessary drivers */

#ifdef INCLUDE_TSEC_VXB_END
#   define ETHERNET_MAC_HANDLER
#   define MAX_MAC_DEVS 2 /* two network devices (tsec) */
#   ifdef BOOTAPP
#       define INCLUDE_BOOT_ETH_MAC_HANDLER
#   endif  /* BOOTAPP */
#endif /* INCLUDE_TSEC_VXB_END */

#ifdef INCLUDE_TSEC_VXB_END2
#   define ETHERNET_MAC_HANDLER
#   define MAX_MAC_DEVS 2 /* two network devices (tsec) */
#   ifdef BOOTAPP
#       define INCLUDE_BOOT_ETH_MAC_HANDLER
#   endif  /* BOOTAPP */
#   ifndef INCLUDE_END2
#      define INCLUDE_END2  /* only END-style driver for FCC */
#   endif /* INCLUDE_END2 */
#endif /* INCLUDE_TSEC_VXB_END2 */

#include "ads834x.h"            /* include the ads834x params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* __INCconfigh */

#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif	/* PRJ_BUILD */
