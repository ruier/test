/* config.h - HPC-NET 8641 configuration header file */

/*
 * Copyright (c) 2006-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
03f,27jun12,l_z  Merge AMP support
03e,15apr10,clx  Update BSP version for vxWorks 6.9
03d,09feb10,liu  Increased BSP version to 2.0/5. 
03c,25sep09,pmr  remove GEI driver by default
03b,06may09,rgo  Nullify past value of SM_BASE_ADRS(WIND00145934)
03a,17mar09,rgo  Define shared memory with macro(wind00145934)
02z,05jan09,b_m  update BSP_REV.
02y,24oct08,ebh  Adjust SM area to match MOB_PLB_1 space after the latter
                 changed.
02x,20oct08,d_l  fix auxclk bspvts issue.(WIND00136349)
02w,08oct08,ebh  Modify area of SM to match MOB_PLB_1 space
02v,04sep08,dtr  Remove AMP_CORE1, single AMP BSP - CPUs configured in
                 project.
02u,02sep08,j_z  Remove RAM_LOW_ADRS and RAM_HIGH_ADRS
02t,01sep08,ebh  Add in include of intCtlr/vxbEpicIntCtlr.h
02s,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
02r,27aug08,dtr  Remove MIPC inclusion, User will use BUNDLE_AMP_PRI.
02q,25aug08,dtr  Allow for AMP bootimage build.
02p,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
02o,13aug08,dtr  Configure for MIPC/WRLOAD in AMP mode.
02n,16jul08,l_z  updated BSP_REV to "/3" for VxWorks 6.7
02m,10jun10,syt  delete the macro USER_RESERVED_MEM duplicated defination
02l,26oct07,to   added CPU1_INIT_START_ADR (WIND00107927)
02k,03oct07,dtr  Define 166Mhz.
02j,03oct07,mze  fix for 106388 can not have BSP specify INCLUDE_SM_COMMON for
                 AMP case as a default
02i,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
02h,28aug07,jmg  Moved VX_SMP_NUM_CPUS to cdf files.
02g,23aug07,mmi  remove INCLUDE_SMP_DEMO due to SDA error
02f,20aug07,dtr  Remove BSD ad NET LIB includes.
02e,18aug07,mmi  add INCLUDE_SMP_DEMO
02d,27jul07,dtr  Increase HWMEM_POOL_SIZE for GEI.
02c,20jul07,mze  removed ifdef INCLUDE_TIPC for demos 
02b,16jul07,ami  added macro INCLUDE_VXB_CMDLINE
02a,13jul07,ebh  Add support for _WRS_VX_AMP
01z,02jul07,dtr  Introduction of dynamic interrupt controller driver.
01y,14jun07,pmr  EPIC changes
01x,31may07,h_k  added INCLUDE_DMA_SYS.
01w,25may07,dtr  CHange DUAL_CORE definition to INCLUDE_AMP for clarity.
01v,11may07,tor  increase hwMemPool size
01u,16apr07,pch  add definition to include vxbOpenPicTimer
		 move inclusion of vxbEpicIntCtlr.h to hwconf.c
01t,23mar07,pmr  fix bootapp support
01s,13mar07,kk   reinstate inclusion of vxbEpicIntCtlr.h
01r,12mar07,kk   temporarily remove include of vxbEpicIntCtlr.h
01q,20feb07,dtr  Add info for TIPC. 
01p,07feb07,dtr  Support PCI Express.
01o,11jan07,pch  update for unified vxbPpcIntCtlr.c
01n,08sep06,dtr  Update clocking info.
01m,15nov06,tor  Convert to VxBus interrupt controller
01l,15nov06,cjj  Rev'ed up BSP_REV for SMP EAR
01k,31oct06,lei  added VX_SMP_NUM_CPUS
01j,27oct06,to   enable MMU by default, no more special settings for SIMICS
01i,08sep06,dtr  Update clocking info.
01h,07aug06,dtr  Change header include name.
01g,14jun06,to   moved INCLUDE_MMU define to hpc2-8641.h
01f,12jun06,kk   remerged to ITER9_FRZ33, modified for SMP support.
01e,06jun06,kk   rolled back merge from FRZ33.
01d,30may06,dtr  Use generic phy.
01c,28apr06,dtr  Changing default bootline device name to motetsec.
01b,04apr06,mig  added SMP support
01a,12feb06,dtr  written from sp7447 bsp/02c.
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

#define CPU1_FUNC_START_ADR  (RAM_LOW_ADRS - 0x4000)
#define CPU1_STACK_START_ADR (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR  (CPU1_STACK_START_ADR + 4)


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

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"

/* Define Clock Speed and source  */

#define	FREQ_33_MHZ	 33330000
#define	FREQ_40_MHZ	 40000000
#define	FREQ_50_MHZ	 50000000
#define	FREQ_66_MHZ	 66670000
#define	FREQ_83_MHZ	 83330000
#define	FREQ_100_MHZ	100000000
#define	FREQ_133_MHZ	133330000
#define	FREQ_166_MHZ	166660000
#define	FREQ_266_MHZ	266660000
#define	FREQ_200_MHZ	200000000
#define	FREQ_333_MHZ	333330000
#define	FREQ_533_MHZ	533330000
#define	FREQ_400_MHZ	400000000

/* Modify accordingly with your board setting. Value is default for 
 * ArgoNavis Rev2 board. 
 */
#define	OSCILLATOR_FREQ	FREQ_50_MHZ

#include <vsbConfig.h>
#include "configAll.h"          /* Set the VxWorks default configuration */

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

/* Subtract two from these and you get multiplier for 50Mhz. Used for DDR */ 
#define PLAT_RATIO_533_MHZ 14 
/* 400 Mhz is the only tested platform ratio for DDR */
#define PLAT_RATIO_400_MHZ 10
#define PLAT_RATIO_333_MHZ 8

/* possibly this is not supported check your DDR has extra chip to support ECC */
#undef INCLUDE_DDR_ECC

#define DEFAULT_SYSCLKFREQ FREQ_400_MHZ

#undef FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 b=10.0.0.1 u=vxworks pw=vxworks f=0x0"

/* memory configuration */

#ifndef CDF_OVERRIDE
#   undef LOCAL_MEM_SIZE
#   define LOCAL_MEM_SIZE          0x20000000
#   define LOCAL_MEM_LOCAL_ADRS    0x00000000
#endif /* CDF_OVERRIDE */

/* User include/exclude section */

#define INCLUDE_SYSCLK			/* PPC decrementer system clock */
#define INCLUDE_MMU_BASIC		/* basic MMU support 		*/
#undef  INCLUDE_TIMESTAMP		/* PPC decrementer as timestamp */
#undef  INCLUDE_INSTRUMENTATION		/* windview optional product 	*/
#define INCLUDE_CACHE_L2
#define INCLUDE_ALTIVEC

    /* Common Includes for VXBUS RIO and ETSEC requires vxWorks 6.3 */

#define INCLUDE_VXBUS

/* 
 * RAPIDIO supports only point to point shared memory support
 * This includes TIPC and SM objects. Untested for this BSP 
 */

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


#define	INCLUDE_HWMEM_ALLOC
#define	INCLUDE_PARAM_SYS
#define	INCLUDE_PLB_BUS
#define	INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define	INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

#define	HWMEM_POOL_SIZE 100000

/* 
 * the file vxbUsrCmdLine.c file gets included in usrConfig.h only if 
 * INCLUDE_VXB_CMDLINE macro is defined. This included vxBus componets
 * for command line build
 */
#define INCLUDE_VXB_CMDLINE

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define	INCLUDE_INTCTLR_LIB
#define	INCLUDE_INTCTLR_DYNAMIC_LIB
#define	DRV_INTCTLR_PPC
#define	DRV_INTCTLR_EPIC

/*
 * PCI bus support, off by default
 * Can be supported on one core only per bus controller use sysDeviceFilter
 * for that.
 *#define INCLUDE_PCI_BUS
 */

#ifdef INCLUDE_PCI_BUS

#   define DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#   define INCLUDE_PCI_BUS_AUTOCONF

/* Intel PRO/1000 ethernet support */

#   define INCLUDE_GEI825XX_VXB_END
#   define INCLUDE_GEITBIPHY

#endif /* INCLUDE_PCI_BUS */

#define DRV_TIMER_DEC_PPC

#define	DRV_TIMER_OPENPIC
#define INCLUDE_TIMER_SYS

#define MAX_MAC_DEVS	4		/* two network devices (mottsec) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

/* If TIPC required then define here so the required components are included 
#define INCLUDE_TIPC
*/
/* Need LINKHDR extension to support FCB in ETSEC to
 * reduce FCB insertion requiring it's own BD */
#undef MAX_LINKHDR_CFG
#define MAX_LINKHDR_CFG	32

#define INCLUDE_DUART

#ifdef INCLUDE_CACHE_L2

#define L2CACHE_MODE_WRITETHROUGH	0
#define L2CACHE_MODE_COPYBACK		1

/* Set the default L2 cache mode */
#define L2CACHE_MODE	    L2CACHE_MODE_COPYBACK

#define L2CACHE_SIZE	    L2CACHE_SIZE_1M      /* 1MB  in sysL2Cache.h */

#endif /* INCLUDE_CACHE_L2  */

/* BSP timer constants */

#define SYS_CLK_RATE_MIN        3      /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        5000   /* maximum system clock rate */
#define AUX_CLK_RATE_MIN        1      /* minimum aux clock rate */
#define AUX_CLK_RATE_MAX        50000  /* maximum aux clock rate */



#define INCLUDE_DDR_SDRAM   /* TBD DDR1/2or interleaved */
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


#ifdef INCLUDE_PCI_BUS

#undef EPIC_EX_DFT_POLAR
#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW

#undef EPIC_EX_DFT_SENSE
#define EPIC_EX_DFT_SENSE	EPIC_SENSE_LVL

/* define this MACRO to enable PCI Express */

/* Support dual PCI Express controllers */

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

#endif /* INCLUDE_PCI_BUS */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE
 * are defined in config.h and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS           0xfff00000      /* base address of ROM */
#define ROM_TEXT_ADRS           (ROM_BASE_ADRS+0x0100) /* with PC & SP */
#define ROM_WARM_ADRS           (ROM_TEXT_ADRS+0x000C) /* warm reboot entry */
#define ROM_SIZE                0x00100000      /* 512KB ROM space */

#define FLASH_BASE_ADRS         0xff800000
#define FLASH_TOTAL_SIZE        0x00800000

#define INCLUDE_FLASH

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

#define NV_RAM_ADRS             0
#define FLASH_SEGMENT_SIZE    0x10000
#define FLASH_ADRS            0xffef0000
#define FLASH_SIZE            0x10000
#define FLASH_SIZE_WRITEABLE	FLASH_SIZE	/* write all of flash */
#define NV_RAM_SIZE           FLASH_SIZE
#undef FLASH_NO_OVERLAY
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET        0

#else /* INCLUDE_FLASH */
#   define NV_RAM_SIZE 0
#   define NV_RAM_ADRS 0
#endif

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
#if defined(INCLUDE_SM_COMMON)
#   define INCLUDE_SMEND
#   define INCLUDE_SM_NET
#   define INCLUDE_NET_DRV
#   define INCLUDE_SM_SEQ_ADDR
#ifdef INCLUDE_AMP
/* Auto start second CPU to connect SM END */
#   define INCLUDE_AMP_SM_END_AUTO 
#endif /* INCLUDE_AMP */
#   undef  WDB_COMM_TYPE
#   define WDB_COMM_TYPE WDB_COMM_NETWORK
#if defined(PRJ_BUILD)
#   undef INCLUDE_WDB_COMM_END
#   undef INCLUDE_WDB_SYS
#   define INCLUDE_WDB_COMM_NETWORK
#endif

#endif /* defined(INCLUDE_SM_NET) */

#   undef IPFORWARDING_CFG
#   define IPFORWARDING_CFG TRUE

#define SM_OFF_BOARD    TRUE

#undef SM_TAS_TYPE

/* Default is not to include shared memory in bootrom for dual core
 * The values used below are solely for the dual core shared memory 
 * communication with AMP mode. */
#define SM_TAS_TYPE SM_TAS_HARD

/* Shared Memory Base Address */
#undef  SM_BASE_ADRS
#define SM_BASE_ADRS		(0x10000000)   

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
 *   SM_MEM_ADRS used  to be 0x10010000
 *   SM_MEM_SIZE used  to be 0x00070000  Note that this plus the SM_OBJ_MEM_SIZE
 *   SM_OBJ_MEM_SIZE was     0x00040000   cannot be larger than the size of
 *                                       MOB_PLB_SM_POOL_SIZE_1 (0x50000)
 * The TIPC space follows as well, and 0x10000 is set aside for that as well.
 * All of these values can be adjusted to suite individual needs. */
				/* Start of actual SM region */
#define SM_MEM_ADRS		(SM_BASE_ADRS + 0x10000 + 0x00080000)	
#define SM_MEM_SIZE		0x00028000
#define SM_OBJ_MEM_ADRS		(SM_MEM_ADRS + SM_MEM_SIZE)/* SM Objects pool */
#define SM_OBJ_MEM_SIZE		0x00030000

#define SM_INT_ARG1   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG2   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG3   1

#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS            SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00018000	/* was 0x00020000 */
#define SM_TIPC_SIZE            SM_TIPC_SIZE_DEFAULT /* SM TIPC pool size  */

#include "hpcNet8641.h"        /* Verify user options, specify i/o addr etc */


#ifdef __cplusplus
}
#endif

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

