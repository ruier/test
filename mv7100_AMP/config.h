/* config.h - mv7100 configuration header file */

/*
 * Copyright (c) 2008-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01r,01jul13,d_l  undefine NV_BOOT_OFFSET for project build. (WIND00423673)
01q,26jun13,d_l  update BSP_REV to "6.9/2"
01p,02feb12,mpc  update BSP version for vxWorks 6.9.2
01o,15apr10,clx  update BSP version for vxWorks 6.9
01n,20dec09,e_d  add sysAmpCpuStatus check function
01m,14dec09,e_d  undefine INCLUDE_PCI_BUS. (WIND00194065) 
01l,17dec09,e_d  removed INCLUDE_TIMESTAMP and M command support
01k,20nov09,e_d  modified usb and tffs define
01j,16nov09,e_d  update bsp version
01i,11nov09,e_d  add macro INCLUDE_SMFORVME
01h,07feb09,e_d  add M command support
01g,05feb09,e_d  add include <vsbConfig.h>
01f,04feb09,e_d  change _WRS_VX_SMP to _WRS_CONFIG_SMP
01e,14nov08,y_w  added USB related macros
01d,28sep08,y_w  modified for TFFS support and code clean up
01c,10sep08,y_w  removed RAM_HIGH_ADRS and RAM_LOW_ADRS
01b,08apr08,mdg  removed light power management from AMP mode.
01a,17mar08,mdg  written from hpcNet8641 and mv3100 bsps.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif /* _ASMLANGUAGE */

#include <vsbConfig.h>

#define CPU1_FUNC_START_ADR  (RAM_LOW_ADRS - 0x4000)
#define CPU1_STACK_START_ADR (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR  (CPU1_STACK_START_ADR + 4)

#ifdef _WRS_VX_AMP
#define AMP_CPU_STATUS_BLOCK (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU0_STATUS      (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU1_STATUS      (CPU1_FUNC_START_ADR + 0x40)

#ifndef _ASMLANGUAGE
typedef	enum ampCpuState {
ampCpuStateUnknown = 0,
ampCpuStateReboot = 1,
ampCpuStateBooting = 2,
ampCpuStateRunning = 3,
ampCpuStateError = -1
} AMP_CPU_STATE;
#endif /* _ASMLANGUAGE */

#endif /* _WRS_VX_AMP */

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */

#define SYS_MODEL       "Motorola"
#include "configAll.h"          /* Set the VxWorks default configuration */

#define INCLUDE_END

/* possibly this is not supported check your DDR has extra chip to support ECC */

#undef  INCLUDE_DDR_ECC

#define DEFAULT_SYSCLKFREQ FREQ_400_MHZ

#undef  FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=128.224.162.146 e=128.224.163.118 u=target pw=password f=0x0"

/* User include/exclude section */

#define INCLUDE_SYSCLK              /* PPC decrementer system clock */
#define INCLUDE_CACHE_SUPPORT       /* cache support */
#define INCLUDE_MMU_BASIC           /* basic MMU support        */
#define INCLUDE_CACHE_L2
#define  INCLUDE_ALTIVEC

/* Common Includes for VXBUS RIO and ETSEC requires vxWorks 6.3 */

#define INCLUDE_VXBUS

#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define HWMEM_POOL_SIZE 100000

/*
 * the file vxbUsrCmdLine.c file gets included in usrConfig.h only if
 * INCLUDE_VXB_CMDLINE macro is defined. This included vxBus componets
 * for command line build
 */

#define INCLUDE_VXB_CMDLINE

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#define DRV_TIMER_DEC_PPC
#define DRV_TIMER_OPENPIC
#define INCLUDE_TIMER_SYS

#ifdef _WRS_VX_AMP
#   define INCLUDE_AMP_CPU_00
#endif /* _WRS_VX_AMP */

/* other useful includes */

#undef  INCLUDE_ISR_OBJECTS
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB
#undef  INCLUDE_PCI_BUS

/* TBD MDG */

#undef  INCLUDE_ADT7461         /* ADT7461 Temp Sensor Support */
#undef  INCLUDE_RTC             /* Maxim DS1375 RTC with Alarm Support */
#undef  INCLUDE_WDOG_TIMER      /* Watchdog timer support */
#undef  INCLUDE_USB             /* Wind River USB support */
#undef  INCLUDE_TFFS            /* True Flash File system support */
#undef  INCLUDE_BOOT_IMAGE_PROGRAM

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
#   define INCLUDE_IPWRAP_ROUTECMD /* programming bootrom needs to add route */
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */

/*
 * IMPORTANT INSTRUCTIONS
 *
 * INCLUDE_AMP support requires low memory offset mode enabled and to
 * build for vxWorks images.
 *
 * #define INCLUDE_AMP
 * In order to establish a network interface in the second core you need
 * a shared memory END driver. To include use:
 *
 * #define INCLUDE_SM_COMMON
 * Only include for the vxWorks image as hardware TAS is used which requires
 * MMU enabled.
 */

/*
 * The macro _WRS_VX_AMP is defined in Makefile for AMP bsp,
 * so we can't define this macro in UP or SMP bsp.
 */

#ifdef _WRS_VX_AMP
#   define INCLUDE_AMP
#endif  /* _WRS_VX_AMP */

/*
 * Include Shared Memory
 *
 * Note: If building bootrom for shared memory master, shared memory, and
 * therefore INCLUDE_SM_COMMON should not be defined.  It would only be
 * defined for bootroms of shared memory clients.
 */

#undef INCLUDE_SM_COMMON

/*
 * When using AMP, the TSEC_MDIO driver is required if we want to
 * use the on-board ETSEC ethernet ports on both cores. This is because
 * there is only one MDIO port, and it has to be shared between the cores.
 * A TSEC_MDIO instance on one core will cooperate with its twin on the
 * other code to insure that MDIO accesses will not overlap.
 */

#ifdef _WRS_VX_AMP
#   define INCLUDE_TSEC_MDIO
#   define TM_ANCHOR_ADRS (0x4400 + 0x10000000)

/* These constants specify which core each TSEC is assigned to */

#   define TSEC0_CORE 0
#   define TSEC1_CORE 1
#   define TSEC2_CORE 0
#   define TSEC3_CORE 1
#endif /* _WRS_VX_AMP */

/* If TIPC required then define here so the required components are included */

#undef  INCLUDE_TIPC

/*
 * Need LINKHDR extension to support FCB in ETSEC to
 * reduce FCB insertion requiring it's own BD
 */

#undef  MAX_LINKHDR_CFG
#define MAX_LINKHDR_CFG 32

#define INCLUDE_DUART

/*
 * Cache configuration
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#ifdef INCLUDE_CACHE_SUPPORT
#   define USER_I_CACHE_ENABLE
#   define USER_D_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   undef  USER_D_CACHE_MODE
#   define USER_I_CACHE_MODE        (CACHE_COPYBACK)
#   define USER_D_CACHE_MODE        (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

/* create a single macro INCLUDE_MMU */

#if defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_GLOBAL_MAP)
#   define INCLUDE_MMU
#endif /* INCLUDE_MMU_BASIC */

#ifdef INCLUDE_CACHE_L2

#   define L2CACHE_MODE_WRITETHROUGH   0
#   define L2CACHE_MODE_COPYBACK       1

/* Set the default L2 cache mode */

#   define L2CACHE_MODE        L2CACHE_MODE_COPYBACK
#   define L2CACHE_SIZE        L2CACHE_SIZE_1M      /* 1MB  in sysL2Cache.h */

#endif /* INCLUDE_CACHE_L2  */

/*
 * Because the usb device and vme bus is depended on
 * the PCI bus. So If used the usb and vme device, we
 * must define INCLUDE_PCI_BUS first.
 */

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define INCLUDE_PCI_OLD_CONFIG_ROUTINES
#   define DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#endif /* INCLUDE_PCI_BUS */
/*
 * PCI and PCI Auto-Configuration Support
 *
 * PCI_MSTR_IO_SIZE, PCI_MSTR_MEMIO_SIZE and PCI_MSTR_MEM_SIZE
 * control the sizes of the available PCI address spaces. The windows
 * defined by these parameters must be large enough to accommodate all
 * of the PCI memory and I/O space requests found during PCI autoconfiguration.
 * If they are not, some devices will not be autoconfigured.
 *
 * NOTE: Since PCI auto-configuration is performed by the bootroms,
 * changing any of these values requires the creation of new bootroms.
 *
 * CPU Space                                   PCI (|) or VME (}) Space
 * (default values)
 *
 * LOCAL_MEM_LOCAL_ADRS =        -----------------  0x00000000
 *              (0x00000000)    |      DRAM       |
 *                              :                 :
 *                              :                 :
 * PCI0_MSTR_MEMIO_LOCAL =       ----------------- PCI0_MSTR_MEMIO_BUS =
 *              (0x80000000)    |                 |  PCI0_MSTR_MEMIO_LOCAL =
 *                              | NonPrefetch PCI |  (0xa0000000)
 *                              | Memory space    |
 *                              |     128MB       |
 *                              | (0x08000000)    |
 *                              |                 |
 * PCI0_MSTR_MEM_LOCAL =         ----------------- PCI0_MSTR_MEM_BUS =
 * PCI0_MSTR_MEMIO_LOCAL +      |                 |  PCI0_MSTR_MEM_LOCAL
 * PCI0_MSTR_MEMIO_SIZE =       | Prefetch PCI    |  (0xa0800000)
 *              (0x88000000)    | Memory space    |
 *                              |    128MB        |
 *                              | (0x08000000)    |
 * VME_A32_MSTR_LOCAL =          -----------------  VME_A32_MSTR_BUS
 *              (0x90000000)    |                 } (0x08000000)
 *                              | VME A32 space   } .
 *                              |    256MB        } . Tempe outbound window 1
 *                              | (0x10000000)    } .
 *              (0x97ffffff)    |                 } (0x0fffffff)
 *                              |.................}
 *                              | Unused A32      }
 *              (0x9fff0000)    |.................} (0xfb000000)
 *                              |                 } .
 *                              |                 } . Tempe outbound window 0
 *                              |                 } .
 *              (0x9fffffff)    |                 } (0xfb00ffff)
 * VME_A24_MSTR_LOCAL =          -----------------
 * VME_A32_MSTR_LOCAL +         |                 } (0x00000000)
 * VME_A32_MSTR_SIZE =          | VME A24 space   } .
 *              (0xa0000000)    |    16MB         } . Tempe outbound window 2
 *                              | (0x01000000)    } .
 *              (0xa0ffffff)    |                 } (0x00ffffff)
 * VME_A16_MSTR_LOCAL =          -----------------
 * VME_A24_MSTR_LOCAL +         |                 } (0x00000000)
 * VME_A24_MSTR_SIZE =          | VME A16 space   } .
 *              (0xa1000000)    |    64KB         } . Tempe outbound window 3
 *                              | (0x00010000)    } .
 *              (0xa10fffff)    |                 } (0x0000ffff)
 *                               -----------------
 *                              :                 :
 *                              :  Not used       :
 *                              :  239 MB         :
 *                              : (0x0ef00000)    :
 *                              : Available for   :
 *                              : expansion of    :
 *                              : VME space       :
 *                               -----------------
 *              (0xb0000000)    :                 :
 *                              : Not used,       :
 *                              :  256 MB         :
 *                              : Available for   :
 *                              : expansion of    :
 *                              : PCI0 spaces     :
 *                              :                 :
 *                              :                 :
 * PCI1_MSTR_MEMIO_LOCAL =       ----------------- PCI1_MSTR_MEMIO_BUS =
 *              (0xC0000000)    |                 |  PCI1_MSTR_MEMIO_LOCAL =
 *                              | NonPrefetch PCI |  (0xC0000000)
 *                              | Memory space    |
 *                              |     128MB       |
 *                              | (0x08000000)    |
 *                              |                 |
 * PCI1_MSTR_MEM_LOCAL =         ----------------- PCI1_MSTR_MEM_BUS =
 * PCI1_MSTR_MEMIO_LOCAL +      |                 |  PCI1_MSTR_MEM_LOCAL
 * PCI1_MSTR_MEMIO_SIZE =       | Prefetch PCI    |  (0xC8000000)
 *              (0xC8000000)    | Memory space    |
 *                              |    128MB        |
 *                              | (0x08000000)    |
 *              (0xd0000000)     -----------------
 *                              :                 :
 *                              : Not used,       :
 *                              :    256 MB       :
 *                              : Available for   :
 *                              : expansion of    :
 *                              : PCI1 spaces     :
 *                              :                 :
 *              (0xe0000000)     -----------------
 *                              :                 :
 *                              : Not used,       :
 *                              :    256 MB       :
 *                              :                 :
 * ISA0_MSTR_IO_LOCAL =          ----------------- ISA0_MSTR_IO_BUS =
 * PCI0_MSTR_MEM_LOCAL +        |                 |   (0xF0000000)
 * PCI0_MSTR_MEM_SIZE           |  I/O space      |
 *              (0xf0000000)    |    64KB         |
 *                              | (0x00010000)    |
 * PCI0_MSTR_IO_LOCAL =          ----------------- PCI0_MSTR_IO_BUS =
 * ISA0_MSTR_IO_LOCAL +         |                 |   (0xF0010000)
 * ISA0_MSTR_IO_SIZE =          |   I/O space     |
 *              (0xf0010000)    |   8MB - 64KB    |
 *                              | (0x007f0000)    |
 *              (0xf0800000)     ----------------- (0x01000000)
 * ISA1_MSTR_IO_LOCAL =          ----------------- ISA1_MSTR_IO_BUS =
 * PCI1_MSTR_MEM_LOCAL +        |                 |   (0xF0800000)
 * PCI1_MSTR_MEM_SIZE           |  I/O space      |
 *              (0xf0800000)    |    64KB         |
 *                              | (0x00010000)    |
 * PCI1_MSTR_IO_LOCAL =          ----------------- PCI1_MSTR_IO_BUS =
 * ISA1_MSTR_IO_LOCAL +         |                 |   (0xF0810000)
 * ISA1_MSTR_IO_SIZE =          |   I/O space     |
 *              (0xf0810000)    |   8MB - 64KB    |
 *                              | (0x007f0000)    |
 * CCSBAR =                      -----------------
 *              (0xf1000000)    | MPC8641         :
 *                              | internal regs.  :
 *                              |     1M          :
 *                              | (0x00100000)    :
 *              (0xf1100000)     -----------------
 *                              :     Unused      :
 * LOCAL_BOARD_ACCESS_ADRS       -----------------
 *              (0xf2000000)    :   MVME7100      :
 *                              : Control/Status  :
 *                              :   Registers     :
 *                              :   Quad UARTs    :
 *                              :  Tick Timers    :
 *                              :  (0x00040000)   :
 *              (0xf2040000)     -----------------
 *                              :     Unused      :
 *                              :                 :
 * LOCAL_BOARD_ACCESS_ADRS       -----------------
 *              (0xf2400000)    :   MVME7100      :
 *                              :     MRAM        :
 *                              :  (0x00080000)   :
 *              (0xf2480000)     -----------------
 *                              :     Unused      :
 *                              :                 :
 *                              :                 :
 * Flash Bank  =                 -----------------
 *              (0xf8000000)    |   Flash Bank    :
 *                              |     128MB       :
 *                              |                 :
 *                              |                 :
 *                              |                 :
 *              (0xffffffff)     -----------------
 *
 * Notes on inbound translations from VME bus:
 *
 * Tempe inbound window 0 in support of shared-memory:
 *
 * On the processor node compiled with SM_OFF_BOARD set to FALSE, an
 * inbound window is opened using Tempe's inbound window 0 to map the
 * shared-memory anchor and shared-memory pool.
 *
 * The inbound access is shown here (output from sysTempeInWinShow(0)):
 *
 * Inbound window 0: ENABLED,  Size = 00000000_00040000
 *
 * raw: 00000000 08000000 00000000 0803fff0 ffffffff f8000000 80000faf
 * VME Base:  00000000_08000000    PCI Base:  00000000_00000000
 * VME Limit: 00000000_0803ffff    PCI Limit: 00000000_0003ffff
 *   Attributes:
 *      Read-ahead threshold - when FIFO competely empty
 *      Virtual FIFO size    - 64 bytes
 *      2eSST Mode           - 160 MB/s
 *      Cycle response       - 2eSSTB (Two Edge Source Synchronous Broadcast)
 *                             2eSST (Two Edge Source Synchronous nonBroadcast)
 *                             2eVME (Two Edge VMEbus)
 *                             MBLT (Multiplexed Block Transfer)
 *                             BLT (Block Transfer)
 *      Address space        - A32
 *      VME AM response      - Supervisor, nonSupervisor, Program, Data
 *
 * Here's a recap:
 *
 * If this is not the anchor node (SM_OFF_BOARD == TRUE)
 * then
 *     No A32 inbound mappings are programmed for access to DRAM.
 *     Note that all nodes must have inbound mapping to reach
 *     their mailbox's so shared memory doorbells can
 *     be hit by any other node.  This is taken care of by the
 *     RAI register programming on Universe equipped boards and
 *     the CRG programming by Tempe equipped boards.
 * else
 *     (this is the anchor node ...)
 *     A single inbound mapping through Tempe window 0 is configured,
 *     mapping DRAM address 0x00000000 through 0x0003ffff to VME
 *     bus address 0x08000000 through 0x0803ffff.
 *
 */

/* VME support */

#define VME_A64_MSTR_SIZE       (0x00000000)      /* 0 MB */
#define VME_A32_MSTR_SIZE       (0x10000000)      /* 256 MB */
#define VME_A24_MSTR_SIZE       (0x01000000)      /* 16 MB */
#define VME_A16_MSTR_SIZE       (0x00010000)      /* 64 KB */

/* PCI and PCI Auto-Configuration Support */

#define PCI0_MSTR_MEMIO_SIZE    (0x08000000)      /* 128 MB */
#define PCI0_MSTR_MEM_SIZE      (0x08000000)      /* 128 MB */
#define ISA0_MSTR_IO_SIZE       (0x00010000)      /* 64 kbytes */
#define PCI0_MSTR_IO_SIZE       (0x007f0000)      /* 8 MB - 64 KB */
#define PCI1_MSTR_MEMIO_SIZE    (0x08000000)      /* 128 MB */
#define PCI1_MSTR_MEM_SIZE      (0x08000000)      /* 128 MB */
#define ISA1_MSTR_IO_SIZE       (0x00010000)      /* 64 kbytes */
#define PCI1_MSTR_IO_SIZE       (0x007f0000)      /* 8 MB - 64 KB */

/* memory configuration */

#define LOCAL_MEM_LOCAL_ADRS       0x0
#define INCLUDE_DDR_SDRAM                         /* TBD DDR1/2or interleaved */
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)
#define USER_RESERVED_MEM          0x0

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE
 * are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS           0xfff00000             /* base address of ROM */
#define ROM_TEXT_ADRS           (ROM_BASE_ADRS+0x0100) /* with PC & SP */
#define ROM_WARM_ADRS           (ROM_TEXT_ADRS+0x000C) /* warm reboot entry */
#define ROM_SIZE                0x00100000             /* 512KB ROM space */

/* Serial port configuration */

#define N_DUART_CHANNELS        1                 /* 1 internal serial ports  */
#define N_DUART_SIO_CHANNELS    N_DUART_CHANNELS  /* 1 serial I/O chnl */
#define DUART_BAUD              9600              /* default baud rate */
#define DUART_REG_ADDR_INTERVAL 1                 /* duart vector reg distance */

#define N_QUART_CHANNELS        4                 /* 4 external serial ports  */
#define QUART_REG_ADDR_INTERVAL 1                 /* uart vector reg distance */

/* Quad UART Support */

#define INCLUDE_QUAD_UART

#undef NUM_TTY

#ifdef INCLUDE_QUAD_UART
#   define NUM_TTY (N_DUART_CHANNELS + N_QUART_CHANNELS)
#else  /* INCLUDE_QUAD_UART */
#   define NUM_TTY (N_DUART_CHANNELS)
#endif /* INCLUDE_QUAD_UART */

/*
 * Each core may have a different console serial port.  They may also be
 * set to NONE.  For single core operation, CORE0_CONSOLE_TTY controls
 * the console port number
 */

#  undef  CONSOLE_TTY
#  define CORE0_CONSOLE_TTY 0
#  define CORE1_CONSOLE_TTY 1
#  define CONSOLE_TTY (sysSerialConsoleTty())

/* This value MUST match the 8641 MPX bus speed */

#define DEC_CLOCK_FREQ      (sysClkFreqGet())      /* 400MHz default */

/* TFFS Support */

#ifdef INCLUDE_TFFS
#   define INCLUDE_DOSFS            /* dosFs file system */
#   define INCLUDE_SHOW_ROUTINES    /* show routines for system facilities*/
#   define INCLUDE_FLASH

#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_FS_EVENT_UTIL    /* File system event utility */
#   define INCLUDE_FS_MONITOR       /* File system monitor */
#   define INCLUDE_ERF              /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER   /* Device Manager */
#   define INCLUDE_XBD              /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV      /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#endif  /* INCLUDE_TFFS */

/* USB Host Support */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_OHCI
#   define INCLUDE_OHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_FS_EVENT_UTIL    /* File system event utility */
#   define INCLUDE_FS_MONITOR       /* File system monitor */
#   define INCLUDE_ERF              /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER   /* Device Manager */
#   define INCLUDE_XBD              /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV      /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_CHKDSK     /* file system integrity checking */
#   define INCLUDE_DOSFS_CACHE
#endif /* INCLUDE_USB */

/*
 * Define the priority list for the Tempe interrupt support.  List items
 * correspond to bit postions in the interrupt enable register.  The
 * first item in the list is the first one scanned when a Tempe interrupt
 * occurs.
 */

#define TEMPE_INT_PRIORITIES \
        TEMPE_INT_NUM_IRQ1_BIT,  TEMPE_INT_NUM_IRQ2_BIT, \
        TEMPE_INT_NUM_IRQ3_BIT,  TEMPE_INT_NUM_IRQ4_BIT, \
        TEMPE_INT_NUM_IRQ5_BIT,  TEMPE_INT_NUM_IRQ6_BIT, \
        TEMPE_INT_NUM_IRQ7_BIT,  TEMPE_INT_NUM_ACFL_BIT, \
        TEMPE_INT_NUM_SYSFL_BIT, TEMPE_INT_NUM_IACK_BIT, \
        TEMPE_INT_NUM_VIEEN_BIT, TEMPE_INT_NUM_VERR_BIT, \
        TEMPE_INT_NUM_PERR_BIT,  TEMPE_INT_NUM_MB0_BIT,  \
        TEMPE_INT_NUM_MB1_BIT,   TEMPE_INT_NUM_MB2_BIT,  \
        TEMPE_INT_NUM_MB3_BIT,   TEMPE_INT_NUM_LM0_BIT,  \
        TEMPE_INT_NUM_LM1_BIT,   TEMPE_INT_NUM_LM2_BIT,  \
        TEMPE_INT_NUM_LM3_BIT,   TEMPE_INT_NUM_DMA0_BIT, \
        TEMPE_INT_NUM_DMA1_BIT,  0xff    /* end of list */

/* Define if shared memory is off board.  If FALSE, then we are the master */

#define CPU_NUM_OF_SM_LOCATION  0   /* the cpu number that shared memory locates in */

/* Network driver configuration */

#if defined(INCLUDE_SM_COMMON)
#   define INCLUDE_SM_NET
#   define INCLUDE_SM_SEQ_ADDR
#   define INCLUDE_SM_OBJ
#   define INCLUDE_SM_NET_SHOW
#   define INCLUDE_NET_DRV
#   define INCLUDE_NET_LIB
#   define INCLUDE_SMEND

#   ifdef  STANDALONE
#       define STANDALONE_NET
#   endif  /* STANDALONE */

#   undef  PCI_DMA_SNOOP_BUS_0_ON
#   undef  IPFORWARDING_CFG
#   define IPFORWARDING_CFG  TRUE /* ipForwardCfg is TRUE or FALSE depending */
                                  /* on whether the stack was built as a host */
                                  /* or a router. Replace ipForwardCfg with */
                                  /* TRUE or FALSE to change the default */
                                  /* configuration. */

#   undef  WDB_COMM_TYPE
#   define WDB_COMM_TYPE    WDB_COMM_END /* WDB_COMM_NETWORK */

#   if defined(PRJ_BUILD)
#       undef INCLUDE_WDB_COMM_END
#       undef INCLUDE_WDB_SYS
#       define INCLUDE_WDB_COMM_NETWORK
#   endif /* PRJ_BUILD */
#endif    /* INCLUDE_SM_COMMON */

/*
 * Default is not to include shared memory in bootrom for dual core
 * The values used below are solely for the dual core shared memory
 * communication with AMP mode.
 */

/*
 * If we want to use the smNet between the board , we should
 * define the macro INCLUDE_SMFORVME . Others if we use the smNet
 * between cores, we should undefine this macro.
 */

#ifdef INCLUDE_SMFORVME
#   define SM_OFF_BOARD FALSE
#else /* INCLUDE_SMFORVME */
#   define SM_OFF_BOARD TRUE
#endif /* INCLUDE_SMFORVME */

#undef  SM_TAS_TYPE
#define SM_TAS_TYPE SM_TAS_HARD

#undef SM_BASE_ADRS
#ifdef INCLUDE_SMFORVME
#   define SM_BASE_ADRS  ((sysProcNumGet() == CPU_NUM_OF_SM_LOCATION) ? \
           ((char*)LOCAL_MEM_LOCAL_ADRS) : ((char*)VME_A32_MSTR_LOCAL))
#else
#   define SM_BASE_ADRS  0x10000000
#endif /* INCLUDE_AMP */

#undef  SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS ((char*) (SM_BASE_ADRS + SM_ANCHOR_OFFSET))
#define SM_MEM_ADRS    ((char*) (SM_BASE_ADRS + 0x0010000))

#define SM_MEM_SIZE         0x00070000
#define SM_OBJ_MEM_ADRS     (SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */

#ifdef _WRS_CONFIG_SMP
#   define SM_OBJ_MEM_SIZE     0  /* VxMP not supported on SMP */
#else
#   define SM_OBJ_MEM_SIZE     0x00040000
#endif

#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS            SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00020000
#define SM_TIPC_SIZE            SM_TIPC_SIZE_DEFAULT /* SM TIPC pool size  */

#ifdef INCLUDE_AMP
#   define SM_SLOCK_MEM_ADRS   (0xe0000 + 0x10000000)
#   define SM_SLOCK_MEM_SIZE   0x00010000
#   define SM_SLOCK_BUS        (SM_SLOCK_MEM_ADRS + 0x0)
#   define SM_SLOCK_I2C        (SM_SLOCK_MEM_ADRS + 0x4)
#   define SM_FLASH_SIZE       (SM_SLOCK_MEM_ADRS + 0x8)
#   define SM_TEMPE_ADRS       (SM_SLOCK_MEM_ADRS + 0xc)
#   define SM_MEM_END          (SM_SLOCK_MEM_ADRS + SM_SLOCK_MEM_SIZE - 1)
#else
#   define SM_MEM_END          (SM_TIPC_ADRS + SM_TIPC_SIZE - 1)
#endif /* INCLUDE_AMP */

/*
 * When using AMP, the TSEC_MDIO driver is required if we want to
 * use the on-board ETSEC ethernet ports on both cores. This is because
 * there is only one MDIO port, and it has to be shared between the cores.
 * A TSEC_MDIO instance on one core will cooperate with its twin on the
 * other code to insure that MDIO accesses will not overlap.
 */

#ifdef _WRS_VX_AMP
#define INCLUDE_TSEC_MDIO
#define TM_ANCHOR_ADRS (0x4400 + 0x10000000)
#endif

#ifdef INCLUDE_SMFORVME
#   define SM_INT_TYPE        SM_INT_MAILBOX_1  /* or SM_INT_NONE */

#   define SM_INT_ARG1        VME_AM_EXT_SUP_DATA

#   ifdef _WRS_CONFIG_SMP
#      define SM_INT_ARG2     ((char *) (VME_CRG_MSTR_BUS + VME_MBOX0_OFFSET + \
                              (sysProcNumGet() * VME_CRG_SLV_SIZE)))
#   else
#      define SM_INT_ARG2     ((char *) (VME_CRG_MSTR_BUS + VME_MBOX0_OFFSET + \
                              ((sysProcNumGet()-(CPU1CHECK)) * VME_CRG_SLV_SIZE)))
#   endif /* _WRS_CONFIG_SMP */

#   define SIG1_INTR_SET      0x80  /* Shared mem Mailbox interrupt "write" value */
#   define SIG2_INTR_SET      0x81  /* Shared mem Mailbox interrupt "write" value */

#   ifdef _WRS_CONFIG_SMP
#      define SM_INT_ARG3     SIG1_INTR_SET
#   else
#      define SM_INT_ARG3     ((CPU1CHECK == 0) ? SIG1_INTR_SET : SIG2_INTR_SET)
#   endif /* _WRS_CONFIG_SMP */
#else /* INCLUDE_SMFORVME */
#   define SM_INT_TYPE        SM_INT_BUS          /* or SM_INT_NONE */
#   define SM_INT_ARG1        (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#   define SM_INT_ARG2        (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#   define SM_INT_ARG3        1
#endif /* INCLUDE_SMFORVME */

/*
 * There are two methods of "hardware assist" available for
 * implementing the sysBusTas() function.  They are "VME bus locking"
 * and "VME bus RMW".  Which of these two methods is used is goverened
 * by the defined/undefined state of ANY_BRDS_IN_CHASSIS_NOT_RMW.
 * Having this symbol "#undef"ed will cause "VME bus RMW" to be used.
 * This is the required setting for Rev 1 of the Tempe (Tsi148 PCI-VME)
 * Chip.  When Rev 2 of the Tempe chip is available, then you will have
 * the option of "#define"ing ANY_BRDS_IN_CHASSIS_NOT_RMW to force the
 * use of VME bus locking to assist sysBusTas().
 */

#undef ANY_BRDS_IN_CHASSIS_NOT_RMW      /* required for Rev 1 of Tsi148 */

#undef VME_TAS_HW_ASSIST

#ifdef ANY_BRDS_IN_CHASSIS_NOT_RMW
#   define VME_TAS_HW_ASSIST    VME_BUS_LOCK
#else
#   define VME_TAS_HW_ASSIST    VME_BUS_RMW
#endif /* ANY_BRDS_IN_CHASSIS_NOT_RMW */

#if defined(_WRS_VX_AMP) || defined(_WRS_CONFIG_SMP)

/*
 * If we are in AMP, light power managment must be removed, as having
 * one core go to sleep (in idle loop) on the 8641D causes issues with the
 * PCIE Snooping and the other core, so that shared memory will not work
 * on the shared memory master in AMP mode.
 */

#   undef INCLUDE_CPU_LIGHT_PWR_MGR
#endif /* INCLUDE_AMP */

#include "mv7100.h"             /* Verify user options, specify i/o addr etc */

#ifdef CDF_OVERRIDE
#   undef NV_BOOT_OFFSET
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif /* define(PRJ_BUILD) */

#ifdef INCLUDE_AMP
#   ifdef _WRS_CONFIG_SMP
#       error AMP and SMP do not mix. Undefine INCLUDE_AMP or remove VXBUILD=SMP
#   endif /* _WRS_CONFIG_SMP */
#endif    /* INCLUDE_AMP */

