/* config.h - Freescale MPC8572 BSP configuration file */

/*
 * Copyright (c) 2007-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02c,07mar13,d_l  add A-005125 erratum. (WIND00403379).
                 Uprev BSP_REV to "6.9/4"
02b,16nov12,d_l  change NV_RAM_SIZE to 0x10000. (WIND00381792)
02a,07nov12,syt  cancel conditional compilation for INCLUDE_FLASH. (WIND00386971)
01z,04jul12,mpc  Add support for TrueFFS. (WIND00202696)
01y,27jun12,l_z  Merge AMP support
01x,27feb12,d_l  Uprev BSP_REV.
01w,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01v,11sep11,ers  Rolled back sysPhysMemDesc RAM settings. (WIND00253685)
01u,10sep10,agf  add warning about setting L1 recovery with SMP
01w,15apr10,clx  Update BSP version for vxWorks 6.9
01v,19may10,syt  Modified PCI_MSTR_MEM_BUS and PCI_MSTR_MEM_SIZE defines.
                 (WIND00210150)
01u,09feb10,liu  Increased BSP version to 2.0/4. 
01t,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01s,26jun09,h_w  add SM_BASE_ADRS definition
01r,24jun09,wqi  remove INCLUDE_SMP_DEMO. (WIND00160267)
01q,05jan09,b_m  update BSP_REV.
01p,24oct08,ebh  Adjust SM area to match MOB_PLB_1 space after the latter
                 changed.
01o,23oct08,d_l  Add auxclk rate for OpenPic timer. (WIND00140273)
01w,03sep08,ebh  Modify area of SM to match MOB_PLB_1 space
01v,03sep08,ebh  Update SM memory regions for the ads8572 target variants
01u,27aug08,dtr  Remove AMP_CORE1 control for core specific driver allocation.
01t,01sep08,ebh  Add in include of intCtlr/vxbEpicIntCtlr.h
01s,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01r,27aug08,dtr  Remove MIPC inclusion, User will use BUNDLE_AMP_PRI.
01q,25aug08,dtr  Fix AMP bootrom build.
01p,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01o,30jul08,dtr  Just remove SIO driver and sio util for wrload image.
01n,26jun08,dtr  Modify AMP BSP configuration to default to using wrload.
01m,06feb08,dtr  Add Epic header file. Remove M85XXCCSR driver from AMP_CORE1 
                 build to fix PCI on core0. 
01l,31jan08,dtr  Remove openPic Timer and dynamic interrupt lib for second
                 core with AMP.
01k,30jan08,dtr  Add vxIpiLib.h if AMP.
01j,28jan08,dtr  Undefine DDR_ECC by default.
01i,09jan08,dtr  Fix some shared memory init parameters.
01h,07jan08,dtr  Mod to SM ANCHOR.
01g,19dec07,dtr  Allow exception vector table to move with
                 LOCAL_MEM_BASE_ADDR.
01f,12dec07,dtr  Change standard BAUD_RATE to 115200 to match u-boot.
01e,28nov07,dtr  Remove default freq.
01d,14nov07,dtr  Add back GNU_INTRINSICS.
01c,26sep07,dtr  Include SPE by default and Temp remove GNU Intrinsics.
01b,20sep07,dtr  Add PCI by default.
01a,04jan07,dtr  Modify from ads8544/config.h/01i
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/4"        /* 0 for first revision */
#include <vsbConfig.h>

#include <configAll.h>

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "ads8572.h"

/* Must be the same as RAM_LOW_ADRS for core0
 * This used for reboot on core1
 */

#define RAM_LOW_ADRS_CPU0 0x00100000

/* Here we need a region of memory that is safe to use 
 * We assume that 0x10000 is enough space for intial vxworks stack
 * before is runns the first task.
 * Assumes also that RAM_LOW_ADRS is at least 0x20000.
 */
#define CPU1_FUNC_START_ADR  (RAM_LOW_ADRS_CPU0 - 0x10000)
#define CPU1_STACK_START_ADR (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR  (CPU1_STACK_START_ADR + 4)

#define AMP_CPU_STATUS_BLOCK (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU0_STATUS      (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU1_STATUS      (CPU1_FUNC_START_ADR + 0x40)

#define DEVICE_CORE0         0
#define DEVICE_CORE1         1

#ifndef _ASMLANGUAGE
typedef enum ampCpuState {
    ampCpuStateUnknown = 0,
    ampCpuStateReboot  = 1,
    ampCpuStateBooting = 2,
    ampCpuStateRunning = 3,
    ampCpuStateError  = -1
} AMP_CPU_STATE;
#endif  /* _ASMLANGUAGE */

/* Define Clock Speed and source  */

#define	FREQ_33_MHZ	 33333300
#define	FREQ_40_MHZ	 40000000
#define	FREQ_50_MHZ	 50000000
#define	FREQ_66_MHZ	 66666600
#define FREQ_83_MHZ      83333300
#define	FREQ_100_MHZ	100000000
#define	FREQ_133_MHZ	133333300
#define	FREQ_266_MHZ	266666000
#define	FREQ_400_MHZ	400000000
#define	FREQ_333_MHZ	333333000
#define	FREQ_533_MHZ	533333000
#define	FREQ_600_MHZ	600000000

#define	OSCILLATOR_FREQ	FREQ_66_MHZ

#define PLAT_RATIO_800_MHZ 20
#define PLAT_RATIO_533_MHZ 16
#define PLAT_RATIO_400_MHZ 12
#define PLAT_RATIO_333_MHZ 8 

/* Frequency applied to the  "RTC" pin */
#define	HPCN_RTC_FREQ	14318000

/* possibly this is not supported check your DDR undefined for safety */
#undef INCLUDE_DDR_ECC
#define DEFAULT_SYSCLKFREQ FREQ_533_MHZ

#undef FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

#undef CONSOLE_BAUD_RATE 
#define CONSOLE_BAUD_RATE 115200

/* Common Includes for VXBUS PCI and ETSEC */

#define INCLUDE_VXBUS

#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS

#define INCLUDE_PLB_BUS


#ifdef INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M85XX
#endif

#define DRV_RESOURCE_M85XXCCSR

#define	INCLUDE_INTCTLR_LIB
#define	DRV_INTCTLR_PPC
#define	DRV_INTCTLR_EPIC

#define	DRV_TIMER_OPENPIC
#define	INCLUDE_INTCTLR_DYNAMIC_LIB

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS


#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#undef  NUM_TTY
#define NUM_TTY 1

#define	INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS

#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

#define HWMEM_POOL_SIZE 100000


#define MAX_MAC_DEVS 	4 		/* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << 21))

#define DEFAULT_BOOT_LINE \
"motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/*
 * Errata: A-005125 (In a very rare condition, a system hang is possible when the 
 * e500 core initiates a guarded load to PCI/PCIe/sRIO while the PCI/PCIe/sRIO 
 * performs a coherent write to memory.)
 */

#define FSL_ERRATA_CPU_A005125

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

/* WARNING: L1 parity recovery sets the cache mode to write-through, which is
 *          incompatible with SMP. Do not define for SMP kernels.
 */

#undef E500_L1_PARITY_RECOVERY

#ifdef E500_L1_PARITY_RECOVERY
  /* *** NOTE FOR PROJECT FACILITY USERS ***
   * Needs to use WRITETHROUGH, building with Project Facility must also
   * change USER_D_CACHE_MODE and USER_I_CACHE_MODE in Project Facility.
   */

# define CACHE_LIBRARY_MODE     CACHE_WRITETHROUGH
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_W
# define TLB_CACHE_MODE         VM_STATE_CACHEABLE_WRITETHROUGH

#ifdef _WRS_CONFIG_SMP
#error "E500_L1_PARITY_RECOVERY should not be set for SMP builds"
#endif /* _WRS_CONFIG_SMP */

#else  /* E500_L1_PARITY_RECOVERY */

# define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
# define TLB_CACHE_MODE         VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#endif

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_LIBRARY_MODE)
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_LIBRARY_MODE)

#define  INCLUDE_L2_CACHE
#undef  INCLUDE_L2_SRAM

  /* If E500_L1_PARITY_RECOVERY is not efined, use local BSP handler.
   * Works for L1 instr cache but not data cache.  Writethrough not needed.
   */  
#ifdef E500_L1_PARITY_RECOVERY

# if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
#   if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
#     define INCLUDE_L1_IPARITY_HDLR              /* VxWorks 6.x */
#   else  /* _WRS_VXWORKS_MAJOR */
#     define INCLUDE_L1_IPARITY_HDLR_INBSP        /* VxWorks 5.5.x */
#   endif /* _WRS_VXWORKS_MAJOR */
# endif  /* INCLUDE_CACHE_SUPPORT && USER_I_CACHE_ENABLE */

#endif  /* E500_L1_PARITY_RECOVERY */

#define INCLUDE_BRANCH_PREDICTION

/* These are maximum values for the 8572 processor */
#define L2_CACHE_SIZE      L2SIZ_256KB
#define L2_SRAM_SIZE       L2SIZ_256KB

#define L2SRAM_ADDR        0x7FF80000
#define L2SRAM_WINDOW_SIZE 0x40000 

#ifdef INCLUDE_NFS
/* Default NFS parameters - constants may be changed here, variables
 * may be changed in usrConfig.c at the point where NFS is included.
 */
#define NFS_USER_ID             2001            /* dummy nfs user id */
#define NFS_GROUP_ID            100             /* dummy nfs user group id */
#define NFS_MAXPATH             255             /* max. file path length */
#endif /* INCLUDE_NFS */

/* Disable Support for SPE 64bit registers */

#define  INCLUDE_SPE


/* Clock rates */

#define INCLUDE_AUX_CLK

#define	SYS_CLK_RATE_MIN	1	/* minimum system clock rate */
#define	SYS_CLK_RATE_MAX	8000	/* maximum system clock rate */
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


/* Although the support for point to point has been added it is untested 
   with Serial RIO. SImilar was tested on parallel RIO */

#ifdef INCLUDE_RAPIDIO_BUS
/* RapidIO information */
#define RAPIDIO_REG_BASE 0xc0000
#define RAPIDIO_BASE   (CCSBAR + RAPIDIO_REG_BASE)
#define RAPIDIO_ADRS   0xc0000000
#define RAPIDIO_SIZE   0x10000000
#define RAPIDIO_BUS_ADRS 0x80000000
#define RAPIDIO_BUS_SIZE 0x10000000
#endif

#undef  EPIC_EX_DFT_POLAR
#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW
#define EPIC_EX_DFT_SENSE	EPIC_SENSE_LVL

#ifdef  INCLUDE_PCI_BUS





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
CPU_PCI_MEM_ADRS (PCIEX1) -------------------------- PCI_MEM_ADRS
CPU_PCI_MEM_ADRS2(PCIEX2) -                        - PCI_MEM_ADRS2
CPU_PCI_MEM_ADRS3(PCIEX3) -------------------------- PCI_MEM_ADRS3
                         -                        - 
CPU_PCI_MEMIO_ADRS       -------------------------- PCI_MEMIO_ADRS
CPU_PCI_MEMIO_ADRS2      -                        - PCI_MEMIO_ADRS2
CPU_PCI_MEMIO_ADRS3      -                        - PCI_MEMIO_ADRS3
                         -                        - 
CPU_PCI_IO_ADRS (PCIEX1) -------------------------- PCI_IO_ADRS
CPU_PCI_IO_ADRS2(PCIEX2) -                        - PCI_IO_ADRS2
CPU_PCI_IO_ADRS3(PCIEX3) -                        - PCI_IO_ADRS3
                         -                        -
CPU_PCI_IO_ADRS (PCIEX1)+-------------------------- PCI_IO_ADRS +
CPU_PCI_IO_ADRS2(PCIEX2)+                           PCI_IO_ADRS2 +
CPU_PCI_IO_ADRS2(PCIEX3)                            PCI_IO_ADRS3
CPU_PCI_IO_SIZE          -                        - PCI_IO_SIZE
                         -                        -
*/

/* PCI based addresses */

#define PCIEX1_LAW_BASE             0x50000000       /* PCI LAW window */
#define PCIEX1_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX1_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX1_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX1_IO_SIZE              0x04000000        /* 64 MB */

#define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)                     
#define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)    
#define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)  


#define PCIEX2_LAW_BASE            0x60000000       /* PCI LAW window */
#define PCIEX2_LAW_SIZE            0x10000000       /* PCI LAW size   */
#define PCIEX2_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000        /* 64 MB */

#define PCIEX2_MEM_ADRS            (PCIEX2_LAW_BASE)                     
#define PCIEX2_MEMIO_ADRS          (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)    
#define PCIEX2_IO_ADRS             (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)  

#define PCIEX3_LAW_BASE             0x80000000       /* PCI LAW window */
#define PCIEX3_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX3_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX3_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX3_IO_SIZE              0x04000000        /* 64 MB */

#define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)                     
#define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)    
#define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)  

#define PCI_LAW_BASE            0x90000000       /* PCI LAW window */
#define PCI_LAW_SIZE            0x10000000       /* PCI LAW size   */
#define PCI_MEM_SIZE             0x04000000        /* 64 MB */
#define PCI_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCI_IO_SIZE              0x04000000        /* 64 MB */

#define PCI_MEM_ADRS            (PCI_LAW_BASE)                     
#define PCI_MEMIO_ADRS          (PCI_MEM_ADRS   + PCI_MEM_SIZE)    
#define PCI_IO_ADRS             (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)  



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

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS          LOCAL_MEM_LOCAL_ADRS
#define PCI_MSTR_MEM_SIZE         LOCAL_MEM_SIZE

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS         PCI_MSTR_MEM_BUS
#define PCI_LOCAL_MEM_SIZE        PCI_MSTR_MEM_SIZE
#define PCI_LOCAL_MEM_SIZE_MASK   PCI_ATTR_WS_1G

#endif /* INCLUDE_PCI_BUS */


#define FLASH_WINDOW_SIZE               0x00800000 

/* Flash Driver pulled from cds85xx BSP */
#define CDS85XX_FLASH_RESERVED_SIZE     0x00110000



/* LBC CS0 - flash 0 - default for bootrom */ 

#define FLASH_BASE_ADRS                 0xf8000000
#define MAIN_FLASH_SIZE                 0x08000000
#define FLASH_SIZE_PER_BANK             (MAIN_FLASH_SIZE/2)


/* LBC CS3 - nvram, cadmus, ATM phy */
#define INCLUDE_LBC_CS3
#ifdef INCLUDE_LBC_CS3
#define LBC_CS3_LOCAL_ADRS       PIXIS_BASE
#define LBC_CS3_LOCAL_SIZE_MASK  0xfff00000
#define LBC_CS3_SIZE             0x00100000
#endif

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH

#define NV_RAM_FLASH_NAME        "norFlash"
#define NV_RAM_FLASH_UNIT        0

#define FLASH_SIZE               MAIN_FLASH_SIZE
#define NV_RAM_SIZE              0x10000

#undef  NV_RAM_ADRS
#define NV_RAM_ADRS              (ROM_BASE_ADRS - NV_RAM_SIZE)
#define NV_RAM_OFFSET            (NV_RAM_ADRS - FLASH_BASE_ADRS)

#undef  FLASH_NO_OVERLAY
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET        0

#else /* INCLUDE_FLASH */

#define NV_RAM_SIZE 0

#endif

/* TrueFFS */

#ifdef INCLUDE_TFFS

#   define INCLUDE_DOSFS
#   define INCLUDE_TFFS_MOUNT
#   define TFFS_FORMAT_PRINT /* print the tffs format process */

/* TrueFFS partition 0 */

/*
 * define to support boot region lock function if supported by corresponding
 * Flash chip
 *
 * Note: sysTffsBootUnLock() must be called before tffsBootImagePut(), and
 * sysTffsBootLock() should be called to re-lock the boot region after
 * tffsBootImagePut().
 */

#   undef  TFFS_BOOT_LOCK

#   define TFFS_PART0_NAME              "RFA0"
#   define TFFS_PART0_FLASH_NAME        "norFlash"
#   define TFFS_PART0_FLASH_UNIT        0

/* main Flash information */

#   define TFFS_PART0_FLASH_BASE_ADRS   FLASH_BASE_ADRS
#   define TFFS_PART0_FLASH_OFFSET      FLASH_SIZE_PER_BANK       /* offset to Flash */
#   define TFFS_PART0_FLASH_SIZE        (FLASH_SIZE_PER_BANK/2)   /* total size */
#   define TFFS_PART0_FLASH_BOOT_SIZE   0                         /* reserved for bootrom */
#endif /* INCLUDE_TFFS */

/* Flash */

#if defined(INCLUDE_FLASH) || defined(INCLUDE_TFFS)
#   define DRV_VXB_NORFLASH
#   define DRV_VXB_NORFLASH_CMDSET_AMD
#endif /* INCLUDE_FLASH || INCLUDE_TFFS */

#ifdef INCLUDE_SM_COMMON
#define INCLUDE_SMEND
#define INCLUDE_SM_NET
#define INCLUDE_NET_DRV
#define INCLUDE_SM_SEQ_ADDR

#undef IPFORWARDING_CFG
#define IPFORWARDING_CFG 1
#endif /* defined(INCLUDE_SM_COMMON) */


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

/* Shared Memory Base Address */
#undef  SM_BASE_ADRS
#define SM_BASE_ADRS        (0x20000000)

#undef SM_ANCHOR_OFFSET
#define SM_ANCHOR_OFFSET 0x4100
#undef SM_ANCHOR_ADRS
#define SM_ANCHOR_ADRS   SM_ANCHOR_OFFSET
/* Defining the SM area to correspond to the area used by the MCB MOB_1 so that
 * both legacy SM and MIPC can co-exist.  Note that if SM is included, then
 * MOB_1 is not permitted in the image.
 *   SM_MEM_ADRS used to be 0x00010000
 *   SM_MEM_SIZE used to be 0x00070000 - note that this plus the SM_OBJ_MEM_SIZE
 *   SM_OBJ_MEM_SIZE was    0x00040000   cannot be larger than the size of
 *                                       MOB_PLB_SM_POOL_SIZE_1 (0x60000)
 * The TIPC space follows as well, and 0x10000 is set aside for that as well.
 * All of these values can be adjusted to suite individual needs. */
#undef SM_MEM_ADRS
#define SM_MEM_ADRS		(SM_BASE_ADRS + 0x10000 + 0x00078000)
#define SM_MEM_SIZE		0x00028000

#define SM_OBJ_MEM_ADRS		(SM_MEM_ADRS+SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE		0x00038000

#define SM_INT_ARG1   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG2   (EPIC_VEC_IPI_IRQ0) /*EPIC_VEC_IPI_IRQ0*/
#define SM_INT_ARG3   1

#define SM_TIPC_ADRS_DEFAULT    ((int) SM_OBJ_MEM_ADRS + SM_OBJ_MEM_SIZE)
#define SM_TIPC_ADRS            SM_TIPC_ADRS_DEFAULT
#define SM_TIPC_SIZE_DEFAULT    0x00018000
#define SM_TIPC_SIZE            SM_TIPC_SIZE_DEFAULT /* SM TIPC pool size  */

#define INCLUDE_SM_SEQ_ADD
#undef SM_TAS_TYPE

/* Default is not to include shared memory in bootrom for dual core
 * SM_TAS_HARD will break if you do so as it requires data cache enabled.
 */
#define SM_TAS_TYPE SM_TAS_HARD
#define SM_INT_TYPE SM_INT_BUS  /* or SM_INT_NONE */


/* Memory addresses */

#define INCLUDE_DDR_SDRAM

#define PHYS_MEM_START          0x00000000
#define PHYS_MEM_SIZE           0x40000000

#define AMP_CORE1_OFFSET        0x20000000

#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_ADRS2      LOCAL_MEM_LOCAL_ADRS + 0x20000000


#ifdef INCLUDE_MMU_BASIC

#define INCLUDE_AIM_MMU_CONFIG
#define INCLUDE_AIM_MMU_MEM_POOL_CONFIG


/* This write protects the page tables */
#define INCLUDE_AIM_MMU_PT_PROTECTION

/* 
 * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can 
 * possibly be already used in sysStaticTlbDesc. Also the larger the 
 * page size that can be used the less CAM entries required so aligning 
 * RAM_LOW_ADRS on a large page size boundary is usually necessary.
 * Locking the text section should increase performance because no 
 * Instruction TLB misses are taken within the text section.
 */

#define INCLUDE_LOCK_TEXT_SECTION     
#undef INCLUDE_PAGE_SIZE_OPTIMIZATION /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CTORS_DTORS

#if ((TOOL==e500v2diab) || (TOOL==e500v2gnu))
    /* These are 60X specific for now */
#undef INCLUDE_HW_FP
    /* ev2diab is hard float so undefine soft float */
#undef INCLUDE_SW_FP

#else

/* Using software FP support. Athough task based 32 bit HW_FP is allowed 
 * no optimised libraries are available. The kernel doesn't execute any hw 
 * fp instructions */

#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP
#endif

#define ROM_TEXT_ADRS           0xFFF00100
#define ROM_BASE_ADRS           0xFFF00000
#define ROM_SIZE                0x00100000

#define PHYS_MEM_START          0x00000000

#   define PHYS_MEM_SIZE        0x40000000

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS           LOCAL_MEM_LOCAL_ADRS

#define AMP_CORE1_OFFSET        0x20000000
#define AMP_CORE1_TEXT_ADRS     0xFFF00100

/*
 * LOCAL_MEM_SIZE only can be defined in config.h when command line build.
 * So we add CDF_OVERRIDE for remove this define when prj build.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE       PHYS_MEM_SIZE 
#   define LOCAL_MEM_LOCAL_ADRS 0x00000000
#endif /* CDF_OVERRIDE */

#define USER_RESERVED_MEM       0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
