/* config.h - Wind River SBC8641 configuration header file */

/*
 * Copyright (c) 2007-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,26jun13,d_l  Update BSP_REV to "6.9/1"
01w,15apr10,clx  Update BSP version for vxWorks 6.9
01v,09feb10,liu  Increased BSP version to 2.0/4. 
01u,20aug09,e_d  fix compile error when add SM component. (WIND00163180)
01t,26jun09,h_w  add SM_BASE_ADRS definition
01s,05jan09,b_m  update BSP_REV.
01r,04nov08,ebh  Update SM regions to account for MIPC
01q,20oct08,d_l  fix auxclk bspvts issue.(WIND00136349)
01p,04sep08,dtr  New AMP configuration, moving to one AMP BSP.
01o,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01n,27aug08,dtr  Remove MIPC inclusion, User will use BUNDLE_AMP_PRI.
01m,25aug08,dtr  Fix AMP bootrom build.
01l,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01k,11aug08,dtr  Add wrload support.
01j,08jul08,l_z  updated BSP_REV to "/2" for VxWorks 6.7
01i,25oct07,to   added CPU1_INIT_START_ADR (WIND00107937)
01h,03oct07,mze  fix for 106388 can not have BSP specify INCLUDE_SM_COMMON for
                 AMP case as a default
01g,20sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
01f,18sep07,mze  do not include INCLUDE_TIPC_MEDIA_SM if SMP build
01e,07sep07,jmg  Moved VX_SMP_NUM_CPUS to the cdf files.
01d,04sep07,wap  Add support for SMP (WIND00103522)
01c,30aug07,wap  Switch to VxBus ETSEC and PCI drivers (WIND00103172)
01b,30jul07,vik  added macro INCLUDE_VXB_CMDLINE to use vxbUSrCmdLine.c
01a,21jun07,x_s  initial creation.
*/

/*
This file contains the configuration parameters for the wrSbc8641d BSP.
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Note: SMP support is provided with this BSP for experimental
 * purposes only. While it should have the same functionality
 * as the hpcNet8641 BSP, it hasn't yet been fully validated.
 */

#define CPU1_FUNC_START_ADR  (RAM_LOW_ADRS - 0x4000)
#define CPU1_STACK_START_ADR (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR  (CPU1_STACK_START_ADR + 4)
#define CORE1T_OFFSET 0x10000000

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
#define BSP_REV            "/1"        /* 0 for first revision */

/* Define Clock Speed and source  */

#define	FREQ_33_MHZ	33333333
#define	FREQ_40_MHZ	40000000
#define	FREQ_66_MHZ	66666666
#define	FREQ_100_MHZ	100000000
#define	FREQ_133_MHZ	133333333
#define	FREQ_266_MHZ	266666666
#define	FREQ_200_MHZ	200000000
#define	FREQ_333_MHZ	333333333
#define	FREQ_400_MHZ	400000000
#define	FREQ_533_MHZ	533333333


#define	OSCILLATOR_FREQ	FREQ_100_MHZ

#include <vsbConfig.h>
#include "configAll.h"          /* Set the VxWorks default configuration */

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif /* _ASMLANGUAGE */

#include "wrSbc8641.h"        /* Verify user options, specify i/o addr etc */

#define INCLUDE_END

/* not tested */
#if (OSCILLATOR_FREQ == FREQ_33_MHZ)
#define PLAT_RATIO_533_MHZ 16
#define PLAT_RATIO_400_MHZ 12
#define PLAT_RATIO_333_MHZ 8
#endif
#if (OSCILLATOR_FREQ == FREQ_40_MHZ)
/* Approx */
#define PLAT_RATIO_533_MHZ 14
#define PLAT_RATIO_400_MHZ 10
#define PLAT_RATIO_333_MHZ 8
#endif
#if (OSCILLATOR_FREQ == FREQ_66_MHZ)
#define PLAT_RATIO_533_MHZ 8
#define PLAT_RATIO_400_MHZ 6
#define PLAT_RATIO_333_MHZ 4
#endif
#if (OSCILLATOR_FREQ == FREQ_100_MHZ) /* Default for SBC8641D */
#define PLAT_RATIO_533_MHZ 5
#define PLAT_RATIO_400_MHZ 4
#define PLAT_RATIO_333_MHZ 3
#endif

/* possibly this is not supported check your DDR has extra chip to support ECC */
#undef INCLUDE_DDR_ECC

#define DEFAULT_SYSCLKFREQ FREQ_400_MHZ

#undef FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

#ifdef AMP_CORE1
#define DEFAULT_BOOT_LINE \
"motetsec(2,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 b=10.0.0.1 u=vxworks pw=vxworks f=0x0"
#else
#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 b=10.0.0.1 u=vxworks pw=vxworks f=0x0"
#endif
/* User include/exclude section */

#define INCLUDE_SYSCLK			/* PPC decrementer system clock */
#define INCLUDE_MMU_BASIC		/* basic MMU support 		*/
#define INCLUDE_MMU
#undef  INCLUDE_TIMESTAMP		/* PPC decrementer as timestamp */
#undef  INCLUDE_INSTRUMENTATION		/* windview optional product 	*/
#undef  INCLUDE_AUX_CLK
#undef  INCLUDE_AUXCLK
#define INCLUDE_CACHE_L2
#define INCLUDE_ALTIVEC

#define MAX_MAC_DEVS         1          /* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

    /* Common Includes for VXBUS RIO and ETSEC requires vxWorks 6.3 */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
    /* RAPIDIO supports only point to point shared memory support
      This includes TIPC and SM objects */
#undef INCLUDE_RAPIDIO_BUS

#ifdef INCLUDE_RAPIDIO_BUS
/* #define INCLUDE_SM_COMMON only valid for non SMP */
#define INCLUDE_M85XX_RAPIDIO
#define INCLUDE_M85XX_CPU
#define INCLUDE_M85XX_LAWBAR
#define INCLUDE_M85XX_RIO_SM_CFG
#define INCLUDE_VXBUS_SM_SUPPORT
#define RAPIDIO_BUS_STATIC_TABLE
#define VXBUS_TABLE_CONFIG
#endif

#ifdef INCLUDE_VXBUS
#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

/* ETSEC ethernet support */

#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

#define INCLUDE_PARAM_SYS
#define HWMEM_POOL_SIZE 100000

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define INCLUDE_INTCTLR_LIB
#define INCLUDE_INTCTLR_DYNAMIC_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#define DRV_TIMER_DEC_PPC


#define INCLUDE_TIMER_SYS
#define DRV_TIMER_OPENPIC

/*
 * PCI bus support, off by default
 * Can be supported on one core only per bus controller use sysDeviceFilter 
 * for that.
 *#define INCLUDE_PCI_BUS
 */

#ifdef INCLUDE_PCI_BUS

#define DRV_PCIBUS_M85XX
#define DRV_RESOURCE_M85XXCCSR
#define INCLUDE_PCI_BUS_AUTOCONF

/* Intel PRO/1000 ethernet support */

#define INCLUDE_GEI825XX_VXB_END
#define INCLUDE_GEITBIPHY

#endif /* INCLUDE_PCI_BUS */

#endif /* INCLUDE_VXBUS */

/*
 * IMPORTANT INSTRUCTIONS
 *
 * LEGACY_AMP
 *
 * For INCLUDE_AMP support, low memory offset mode must be enabled on
 * the second core. On the Wind River SBC8641D board, low memory offset
 * mode is enabled when switch SW4:5 is in the on position.
 *
 * #define INCLUDE_AMP
 * In order to establish a network interface in the second core you need
 * a shared memory END driver. To include it, use:
 *
 * #define INCLUDE_SM_COMMON
 * Only include for the vxWorks image as hardware TAS is used which requires
 * MMU enabled.
 * Only enable INCLUDE_SM_COMMON for BSP AMP build where DSHM is not required.
 * If both DSHM and SM are required then the memory allocation of SM and 
 * DSHM must be modified
 * to fit in the available space
 * 
 * AMP
 * 
 * AMP support is included by default in the AMP specific BSP and MIPC
 * is included to support redirecting console output from core1 to core0
 * See documentation on wrload and MIPC for further details.
 */

#ifdef _WRS_VX_AMP
#ifndef _ASMLANGUAGE
#include <vxIpiLib.h>
#endif

#define INCLUDE_VXIPI
/* To support Leagcy AMP you should define here
 * #define LEGACY_AMP or uncomment in the Makefile 
 */
#ifdef LEGACY_AMP
#define INCLUDE_AMP
#endif

/*#define INCLUDE_SM_COMMON */
#endif  /* _WRS_VX_AMP */

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


#ifdef INCLUDE_PCI_BUS

#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW

/* define this MACRO to enable PCI Express */

/*  cds85xx support dual PCI controllers */

/*

CPU Addr                                  PCI Addr ( PCI1 or PCI2)
PCI_LOCAL_MEM_BUS        -------------------------- PCI_MSTR_MEM_BUS
                         -                        - 
                         -                        -
PCI_LOCAL_MEM_BUS +      -------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE       -                        - PCI_MSTR_MEM_SIZE
                         -                        - 
                         -                        ----- PCI Bridge (for PCI1 only)
                         -                        -     configuration regs
                         -                        - 
CPU_PCI_MEM_ADRS  (PCI1) -------------------------- PCI_MEM_ADRS
CPU_PCI_MEM_ADRS2 (PCI2) -                        - PCI_MEM_ADRS2
CPU_PCI_MEM_ADRS3(PCIEX) -------------------------- PCI_MEM_ADRS3
                         -                        - 
CPU_PCI_MEMIO_ADRS       -------------------------- PCI_MEMIO_ADRS
CPU_PCI_MEMIO_ADRS2      -                        - PCI_MEMIO_ADRS2
CPU_PCI_MEMIO_ADRS3      -                        - PCI_MEMIO_ADRS3
                         -                        - 
CPU_PCI_IO_ADRS  (PCI1)  -------------------------- PCI_IO_ADRS
CPU_PCI_IO_ADRS2 (PCI2)  -                        - PCI_IO_ADRS2
CPU_PCI_IO_ADRS3 (PCIEX) -                        - PCI_IO_ADRS3
                         -                        -
CPU_PCI_IO_ADRS  (PCI1)+ -------------------------- PCI_IO_ADRS +
CPU_PCI_IO_ADRS2 (PCI2)+                            PCI_IO_ADRS2 +
CPU_PCI_IO_ADRS2 (PCI3)                             PCI_IO_ADRS3
CPU_PCI_IO_SIZE          -                        - PCI_IO_SIZE
                         -                        -
*/

/* PCI based addresses */

#define PCI_LAW_BASE             0x50000000       /* PCI LAW window */
#define PCI_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCI_MEM_SIZE             0x04000000        /* 64 MB */
#define PCI_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCI_IO_SIZE              0x04000000        /* 64 MB */

#define PCI_MEM_ADRS             (PCI_LAW_BASE)                     
#define PCI_MEMIO_ADRS           (PCI_MEM_ADRS   + PCI_MEM_SIZE)    
#define PCI_IO_ADRS              (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)  

#define PCI2_LAW_BASE            0x60000000       /* PCI LAW window */
#define PCI2_LAW_SIZE            0x10000000       /* PCI LAW size   */

#define PCI_MEM_ADRS2            (PCI2_LAW_BASE)                     
#define PCI_MEMIO_ADRS2          (PCI_MEM_ADRS2   + PCI_MEM_SIZE)    
#define PCI_IO_ADRS2             (PCI_MEMIO_ADRS2 + PCI_MEMIO_SIZE)  

#if (PCI_LAW_SIZE == 0x10000000)
#define  PCI_LAW_WIN_SZ          LAWAR_SIZE_256MB
#endif

#if (PCI2_LAW_SIZE == 0x10000000)
#define  PCI2_LAW_WIN_SZ         LAWAR_SIZE_256MB
#endif

#if (PCIEX_LAW_SIZE == 0x10000000)
#define  PCIEX_LAW_WIN_SZ         LAWAR_SIZE_256MB
#endif

#define PCI_MMU_TLB_SZ           _MMU_TLB_SZ_256M

#if (PCI_MEM_SIZE == 0x04000000)
#define PCI_MEM_SIZE_MASK        PCI_ATTR_WS_64M
#endif 

#if (PCI_MEMIO_SIZE == 0x04000000)
#define PCI_MEMIO_SIZE_MASK      PCI_ATTR_WS_64M
#endif

#if (PCI_IO_SIZE == 0x04000000)
#define PCI_IO_SIZE_MASK         PCI_ATTR_WS_64M
#endif

#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* just 1:1 mapping */ 

/* PCI 1 */

#define CPU_PCI_MEM_ADRS          PCI_MEM_ADRS
#define CPU_PCI_MEMIO_ADRS        PCI_MEMIO_ADRS
#define CPU_PCI_IO_ADRS           PCI_IO_ADRS

/* PCI 2 */

#define CPU_PCI_MEM_ADRS2         PCI_MEM_ADRS2
#define CPU_PCI_MEMIO_ADRS2       PCI_MEMIO_ADRS2
#define CPU_PCI_IO_ADRS2          PCI_IO_ADRS2

/* PCI Express */

#define CPU_PCI_MEM_ADRS3         PCI_MEM_ADRS3
#define CPU_PCI_MEMIO_ADRS3       PCI_MEMIO_ADRS3
#define CPU_PCI_IO_ADRS3          PCI_IO_ADRS3

#define CPU_PCI_MEM_SIZE          PCI_MEM_SIZE 
#define CPU_PCI_MEMIO_SIZE        PCI_MEMIO_SIZE 
#define CPU_PCI_IO_SIZE           PCI_IO_SIZE

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS           0x00000000
#define PCI_MSTR_MEM_SIZE          0x40000000 /* 1G */

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS         0x00000000
#define PCI_LOCAL_MEM_SIZE        PCI_MSTR_MEM_SIZE
#define PCI_LOCAL_MEM_SIZE_MASK   PCI_ATTR_WS_1G

#define INCLUDE_PCI_AUTOCONF

#ifndef PCI_CFG_TYPE
#   ifdef INCLUDE_PCI_AUTOCONF
#      define PCI_CFG_TYPE PCI_CFG_AUTO
#   else
#      define PCI_CFG_TYPE PCI_CFG_FORCE
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI_BUS */


/* other useful includes *
#define INCLUDE_IFMEDIA
#define INCLUDE_VXBUS_SHOW
#define INCLUDE_IFCONFIG
#define INCLUDE_NET_IF_SHOW
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_ISR_OBJECTS
*/

/* Need LINKHDR extension to support FCB in ETSEC to
 * reduce FCB insertion requiring it's own BD */
#undef MAX_LINKHDR_CFG
#define MAX_LINKHDR_CFG       32

#define INCLUDE_DUART
#undef INCLUDE_LB_SDRAM

#ifdef INCLUDE_CACHE_L2

#define L2CACHE_MODE_WRITETHROUGH	0
#define L2CACHE_MODE_COPYBACK	        1

/* Set the default L2 cache mode */
#define L2CACHE_MODE	    L2CACHE_MODE_COPYBACK

#define L2CACHE_SIZE	    L2CACHE_SIZE_1M      /* 1MB  in sysL2Cache.h */

#endif /* INCLUDE_CACHE_L2  */

/* BSP timer constants */

#define SYS_CLK_RATE_MIN        3      /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        5000   /* maximum system clock rate */
#define AUX_CLK_RATE_MIN        1      /* minimum aux clock rate */
#define AUX_CLK_RATE_MAX        50000  /* maximum aux clock rate */

/* memory configuration */
#ifdef _WRS_VX_AMP
#define LOCAL_MEM_SIZE      0x10000000
#else
#define LOCAL_MEM_SIZE      0x20000000
#endif

#ifndef _WRS_VX_AMP
#define LOCAL_MEM_LOCAL_ADRS 0x00000000
#endif

#define INCLUDE_DDR_SDRAM   /* TBD DDR1/2or interleaved */
#define INCLUDE_DDR_SDRAM2   /* TBD DDR1/2or interleaved */
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)
#define USER_RESERVED_MEM       0x0

#undef  LOCAL_MEM_AUTOSIZE                      /* run-time memory sizing */
/* Although the support for point to point SRIO has been added it is untested
   with this board */

#ifdef INCLUDE_RAPIDIO_BUS
/* RapidIO information */
#define RAPIDIO_REG_BASE 0xc0000
#define RAPIDIO_BASE   (CCSBAR + RAPIDIO_REG_BASE)
#define RAPIDIO_ADRS   0xc0000000
#define RAPIDIO_SIZE   0x10000000
#define RAPIDIO_BUS_ADRS 0x80000000
#define RAPIDIO_BUS_SIZE 0x10000000
#endif

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, RAM_HIGH_ADRS, and RAM_LOW_ADRS
 * are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS           0xfff00000      /* base address of ROM */
#define ROM_TEXT_ADRS           (ROM_BASE_ADRS+0x0100) /* with PC & SP */
#define ROM_WARM_ADRS           (ROM_TEXT_ADRS+0x000C) /* warm reboot entry */
#define ROM_SIZE                0x00100000      /* 512KB ROM space */

#define FLASH_BASE_ADRS         0xff000000
#define FLASH_TOTAL_SIZE        0x01000000

#define LOGICPD_FLASH_BANK0_SIZE 0x800000

#define USER_RESERVED_MEM       0x0

#undef INCLUDE_FLASH
#ifdef INCLUDE_FLASH

/*
 * 29LV640D flash is not supported in flashMem.c directly but is similar
 * in procedures with 29LV160B
 */

#define FLASH_29LV640D	FLASH_29LV160B

#undef  SYS_FLASH_TYPE
#define SYS_FLASH_TYPE		FLASH_29LV640D
#undef  FLASH_WIDTH
#define FLASH_WIDTH		2
#undef  FLASH_CHIP_WIDTH
#define FLASH_CHIP_WIDTH	2
#define FLASH_SEGMENT_SIZE    0x10000
#define FLASH_ADRS            0xffef0000
#define FLASH_SIZE            0x10000
#define FLASH_SIZE_WRITEABLE	FLASH_SIZE	/* write all of flash */
#define NV_RAM_SIZE           FLASH_SIZE
#undef FLASH_NO_OVERLAY
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET        0

#endif /* INCLUDE_FLASH */

#define INCLUDE_NV_RAM

#ifdef  INCLUDE_NV_RAM
#   define NV_RAM_ADRS          0xF0000000
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE          (0x2000 - 0x10)  /* 8KB - 16 */
#   define NV_RAM_INTRVL        1
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET       0
#   define NV_MAC_ADRS_OFFSET   0x200
#else
#   define NV_RAM_SIZE 0
#   define NV_RAM_ADRS 0
#endif  /* INCLUDE_NV_RAM */


/* Serial port configuration */

#ifdef INCLUDE_DUART
#   define INCLUDE_SERIAL
#   define NUM_DUART_TTY    2
#else
#   define NUM_DUART_TTY    0
#endif /* INCLUDE_DUART */

#undef  NUM_TTY
#define NUM_TTY        NUM_DUART_TTY  /* sio channel defined in sp.h */

/*
 * Cache configuration
 *
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

/* instruction cache */
#define INCLUDE_CACHE_SUPPORT
#define	USER_I_CACHE_ENABLE		/* Enable INSTRUCTION CACHE */
#undef	USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_COPYBACK) /* select COPYBACK or WRITETHROUGH */

/* data cache */

#define USER_D_CACHE_ENABLE		 /* Enable DATA CACHE */
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)  /* select COPYBACK or DISABLED */


/* This value MUST match the 8641 MPX bus speed */

#define DEC_CLOCK_FREQ          DEFAULT_SYSCLKFREQ      /* 400MHz default */


/* Network driver configuration */

/* Shared memory is set up for INCLUDE_AMP (LEGACY_AMP) not RAPIDIO */

#ifdef INCLUDE_SM_COMMON
#define INCLUDE_SMEND
#define INCLUDE_SM_NET
#define INCLUDE_SM_NET_SHOW
#define INCLUDE_NET_DRV
#define INCLUDE_SM_SEQ_ADDR
#undef  WDB_COMM_TYPE
#define WDB_COMM_TYPE WDB_COMM_NETWORK
#if defined(PRJ_BUILD)
#undef INCLUDE_WDB_COMM_END
#undef INCLUDE_WDB_SYS
#define INCLUDE_WDB_COMM_NETWORK
#endif
#undef IPFORWARDING_CFG
#define IPFORWARDING_CFG 1
#endif /* defined(INCLUDE_SM_NET) */

#ifndef PRJ_BUILD
    #ifdef INCLUDE_TIPC
        #ifndef _WRS_CONFIG_SMP
            #define INCLUDE_TIPC_MEDIA_SM           /* shared memory media */
        #endif
        #define INCLUDE_TIPC_CONFIG_STR         /* use static config string */
        #define INCLUDE_TIPC_CONFIG_HOOK_BOOT   /* call boot params config routine */
        #define INCLUDE_TIPC_SHOW               /* include TIPC show routines */
        #define INCLUDE_TIPC_MEMPOOL            /* include TIPC memory buffers */
        #define INCLUDE_TIPC_SOCKET             /* include TIPC socket API */
    #endif
#endif
/*
 * Changing SM_OFF_BOARD to TRUE also requires changing
 * SM_ANCHOR_ADRS and SM_MEM_ADRS appropriately.
 */

#define SM_OFF_BOARD    TRUE

#define INCLUDE_SM_SEQ_ADD
#undef SM_TAS_TYPE

/* Default is not to include shared memory in bootrom for dual core
 * SM_TAS_HARD will break if you do so as it requires data cache enabled.
 */
#define SM_TAS_TYPE SM_TAS_HARD

/* Shared Memory Base Address */
#undef  SM_BASE_ADRS
#define SM_BASE_ADRS        (0x10000000)

#ifndef _ASMLANGUAGE

#undef  SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS  ((CPU1CHECK == 0) ? \
        ((char*) (SM_BASE_ADRS + SM_ANCHOR_OFFSET)) : \
        ((char*) (SM_BASE_ADRS + SM_ANCHOR_OFFSET)))

#endif

#define SM_INT_TYPE		SM_INT_BUS  /* or SM_INT_NONE */

/* Defining the SM area to correspond to the area used by the MCB MOB_1 so that
 * both legacy SM and MIPC can co-exist.  Note that if SM is included, then
 * MOB_1 is not permitted in the image.
 *   SM_MEM_ADRS used to be 0x10010000
 *   SM_MEM_SIZE used to be 0x00070000 - note that this plus the SM_OBJ_MEM_SIZE
 *   SM_OBJ_MEM_SIZE was    0x00040000   cannot be larger than the size of
 *                                       MOB_PLB_SM_POOL_SIZE_1 (0x70000)
 * The TIPC space follows as well, and 0x10000 is set aside for that as well.
 * All of these values can be adjusted to suite individual needs. */

#define SM_MEM_ADRS		(SM_BASE_ADRS + 0x10000 + 0x80000)	/* Start of actual SM region */
#define SM_MEM_SIZE		0x00028000
#define SM_OBJ_MEM_ADRS		(SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE		0x00030000

#define SM_INT_ARG1   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG2   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG3   1


#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS            SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00018000	/* was 0x00020000 */
#define SM_TIPC_SIZE            SM_TIPC_SIZE_DEFAULT /* SM TIPC pool size  */

#define INCLUDE_LCD



#ifdef __cplusplus
}
#endif

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

#ifdef _WRS_VX_AMP
#if defined(_WRS_CONFIG_SMP)
#error AMP and SMP do not mix. Undefine _WRS_VX_AMP or remove VXBUILD=SMP
#endif /* _WRS_CONFIG_SMP */
#endif /* _WRS_VX_AMP */
