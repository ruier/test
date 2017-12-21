/* config.h - Freescale P2020 BSP configuration file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01b,19aug09,syt  BSP_REV bump up.
01a,17apr09,syt  derived from version 01q of ads8572/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VER_1_1             1
#define BSP_VER_1_2             1
#define BSP_VERSION             "R20150928"   /*bootrom 使用的版本信息*/
#define BSP_REV                 "/1"

#define SOFTWARE_VER  "R20150928-1300"   /*VX BSP使用的版本信息*/
#define SOFTWARE_VEN  "Copyright (C), 2001--, 上海柏飞电子科技有限公司"		

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

#include "p2020.h"



/*
 * Must be the same as RAM_LOW_ADRS for core0
 * This used for reboot on core1
 */

#define RAM_LOW_ADRS_CPU0       0x00100000

/*
 * Here we need a region of memory that is safe to use
 * We assume that 0x10000 is enough space for intial vxworks stack
 * before is runns the first task.F
 * Assumes also that RAM_LOW_ADRS is at least 0x20000.
 */

#define CPU1_FUNC_START_ADR     (RAM_LOW_ADRS_CPU0 - 0x10000)
#define CPU1_STACK_START_ADR    (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR     (CPU1_STACK_START_ADR + 4)

#ifdef _WRS_CONFIG_SMP

/* # of CPUs configured to vxWorks SMP systems */

#   undef  VX_SMP_NUM_CPUS
#   define VX_SMP_NUM_CPUS      2
#   define INCLUDE_SMP_DEMO
#endif /* _WRS_CONFIG_SMP */

/* Common Includes for VXBUS PCI and ETSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

#define HWMEM_POOL_SIZE         100000

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC

#undef  DRV_TIMER_OPENPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB
  
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#ifdef INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M85XX
#define DRV_RESOURCE_M85XXCCSR
#define INCLUDE_GEI825XX_VXB_END
#endif  /* INCLUDE_PCI_BUS */


#define INCLUDE_END
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_MV88E1X11PHY

#define MAX_MAC_DEVS            3

/* default mac address */

#define ENET_DEFAULT0           WR_ENET0
#define ENET_DEFAULT1           WR_ENET1
#define ENET_DEFAULT2           WR_ENET2

#undef  INCLUDE_FSL_SPI

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#define WDT_RATE_MIN            (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX            (sysTimerClkFreq / (1 << 21))

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.3 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

/* e500_L1_Catch_partity_recovery */

#undef  E500_L1_PARITY_RECOVERY

#ifdef  E500_L1_PARITY_RECOVERY
/*
 * *** NOTE FOR PROJECT FACILITY USERS ***
 * Needs to use WRITETHROUGH, building with Project Facility must also
 * change USER_D_CACHE_MODE and USER_I_CACHE_MODE in Project Facility.
 */

# define CACHE_LIBRARY_MODE     CACHE_WRITETHROUGH
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_W
# define TLB_CACHE_MODE         VM_STATE_CACHEABLE_WRITETHROUGH

#else /* E500_L1_PARITY_RECOVERY */

# define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M

# define TLB_CACHE_MODE         VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
#endif /* E500_L1_PARITY_RECOVERY */

/*
 * If E500_L1_PARITY_RECOVERY is not defined, use local BSP handler.
 * Works for L1 instr cache but not data cache.  Writethrough not needed.
 */

#ifdef E500_L1_PARITY_RECOVERY
#   if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
#       define INCLUDE_L1_IPARITY_HDLR          /* VxWorks 6.x */
#   endif  /* INCLUDE_CACHE_SUPPORT && USER_I_CACHE_ENABLE */
#endif  /* E500_L1_PARITY_RECOVERY */



#define INCLUDE_CACHE_SUPPORT
#define INCLUDE_L2_CACHE

#undef  INCLUDE_L2_SRAM

#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_BRANCH_PREDICTION


/*注意是两路SRIO*/
#define RAPIDIO1_ADRS     0xa0000000
#define RAPIDIO1_SIZE      0x10000000   /*256MB*/

#define RAPIDIO2_ADRS     0xb0000000
#define RAPIDIO2_SIZE      0x10000000   /*256MB*/

#define RAPIDIO1_MAIN_ADRS  0xc0000000 
#define RAPIDIO1_MAIN_SIZE   0x10000000  /*256MB*/

#define RAPIDIO2_MAIN_ADRS  0xd0000000 
#define RAPIDIO2_MAIN_SIZE   0x10000000  /*256MB*/


#define FPGA_K7_ADRS	0x90000000   /*PPC-V5-K7*/
#define FPGA_K7_SIZE      0x4000000   /*64MB*/

#define  V5_INTERRAM_ADRS	0x95000000 /*PPC-V5-CAN0/CAN1*/
#define  V5_INTERRAM_SIZE   0x200000   /*2MB, 前1MB给CAN，后1MB用于配制V5*/


/*反映CAN中断类型, 宏定义名字源自.1项目,此处先不做修改*/
#define	EXTERNAL_INT4_SOURCE	 (V5_INTERRAM_ADRS + 0x50)	


#define  RTC_TIMER_BASE_ADRS	(V5_INTERRAM_ADRS + 0x100000) /*FPGA  V5模拟的外部时钟REG基地址*/
#define  RTC_WRITE_DATE0		0x20	
#define  RTC_WRITE_DATE1		0x22
#define  RTC_WRITE_TIME0		0x24
#define  RTC_WRITE_TIME1		0x26
#define  RTC_READ_DATE0		0x28
#define  RTC_READ_DATE1		0x2a
#define  RTC_READ_TIME0		0x2c
#define  RTC_READ_TIME1		0x2e

/*
XXXms timer int reg 
*/
#define TIMER_INT_VECT_NUM     0

/*中断类型REG, 指明当前来的是哪个中断*/
#define TIMER_INT_VECT_ADRS  (V5_INTERRAM_ADRS + 0x100000 + 0x90)

/*
*对P2020.WISH和P2020项目bit0均表示timer中断
*对P2020.1项目, bit2表示timer中断
*/
#define TIMER_INT_FLAG      0x1


/*清中断REG*/
#define TIMER_INT_CLEAR_ADRS  (V5_INTERRAM_ADRS + 0x100000 + 0x92)


/* 对P2020.WISH项目:
 * 中断掩码REG, 用于控制 TIMER中断和429中断开关, 其中bit0表示timer中断
 * bit1表示429中断. 上电默认为bit[0:1]=11, 即两bit均为1，中断关闭.
 * 对P2020项目: 
 * 只有最低1位 timer中断
 */
#define TIMER_INT_MASK_ADRS  (V5_INTERRAM_ADRS + 0x100000 + 0x94)

/*定义timer中断的触发周期*/
#define TIMER_CYCLE_CFG_ADRS  (V5_INTERRAM_ADRS + 0x100000 + 0x96)


/*aucu20141104
  bit0 复位使能REG, 0表示使能复位, 1表示释放复位
*/
#define  FPGA_EXT_RESETN_REG	      (V5_INTERRAM_ADRS + 0x100000 + 0x100)

/*bit0 复位内部,外部选择REG, 1表示内部复位(PPC写bit1控制复位),0表示外部复位(通过MLVDS传信号复位)	  
  bit1 复位使能REG, 0表示使能复位, 1表示释放复位
  bit2 读取MLVDS信号(只读)it， 0表示复位有效, 1表示复位释放*/
#define  SYS_RESET_MODE_REG	      (V5_INTERRAM_ADRS + 0x100000 + 0x60)



/*********这部分REG为VPX4-SP2020.WISH中复位功能使用的REG*******/
#define  SYS_RESET_MODE2_REG	      (V5_INTERRAM_ADRS + 0x100000 + 0x62)
#define RESET_MODE_MLVDS    0
#define RESET_MODE_FPGAVIO 1
#define RESET_MODE_REG        2
/************************************************/








/* L2 SRAM Mode */
#define L2SRAM_ADDR             0xEFF80000
#define L2SRAM_WINDOW_SIZE      0x80000

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_256KB
#   define L2_SRAM_SIZE         L2SIZ_256KB
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_512KB
#   define L2_SRAM_SIZE         0
#else
#   define L2_CACHE_SIZE        0
#   define L2_SRAM_SIZE         L2SIZ_512KB
#endif  /* INCLUDE_L2_CACHE && INCLUDE_L2_SRAM */

/* Disable Support for SPE 64bit registers */

#define INCLUDE_SPE

/* add on-chip drivers */

#undef  INCLUDE_SECURITY_ENGINE     /* only supported if chip has E ie p2020E */

/* Use openPicTimer as the auxClk */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif /* INCLUDE_AUX_CLK */

/* I2C support */

#undef  INCLUDE_I2C

/* LBC CS0 - flash 0 - default for bootrom */
/*该FLASH每片上由两个独立的128MB芯片组成, 故虽然是单片FLASH,但在擦写时,要以128MB区分*/
#define FLASH_BASE_ADRS   0xF0000000
#define FLASH_SIZE              0x10000000    /*256MB*/

/*FLASH 低128MB 用于存储系统IMG, 高128MB用于TFFS*/
#define FLASH_BOOT_BASE_ADRS   0xf8000000
#define FLASH_BOOT_SIZE              0x08000000    /*128MB*/

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
/* NVRam */
#define FLASH_ADRS           FLASH_BOOT_BASE_ADRS
#define FLASH_WIDTH          2
#define FLASH_CHIP_WIDTH     2
#define FLASH_SECTOR_SIZE    0x00020000
#define FLASH_SECTOR_MASK    0xfffe0000
#define NV_RAM_ADRS          (ROM_BASE_ADRS - FLASH_SECTOR_SIZE)
#define NV_RAM_SIZE          FLASH_SECTOR_SIZE
#define NV_MAC_ADRS_OFFSET   0x200

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT      1           /* 16-bit */
#define S29GL_FLASH_NUM      1           /* only one chip */
#define S29GL_FLASH_WIDTH    2           /* 16-bit */
#define S29GL_SECTOR_SIZE    0x00020000  /* 128K per sector */
#define S29GL_FLASH_SIZE     0x04000000  /* only 64MB used for TureFFS */
#define S29GL_FLASH_OP_DELAY

#else  /* INCLUDE_FLASH */
#define NV_RAM_SIZE          0
#endif  /* INCLUDE_FLASH */


/* TrueFFS flash support */
#define INCLUDE_TFFS
#define INCLUDE_DOSFS 


#ifndef COMPILE_BOOTROM
#define INCLUDE_STARTUP_SCRIPT    /*用于自动执行bootrom下配置选择中startup项*/

#define  VXB_M8641_DMA

#define INCLUDE_IPFTPS

#define INCLUDE_WB_RUNTIME_SUPPORT
#define INCLUDE_WDB
#define INCLUDE_WDB_COMM_END
#endif


#ifdef INCLUDE_IPFTPS
#define FTPS_INSTALL_CALLBACK_HOOK TRUE
#define FTPS_AUTH_CALLBACK_HOOK    sysFtpAuth
#define FTPS_INITIAL_DIR           "/tffs0"       /*此处修改为根目录*/
#define FTPS_ROOT_DIR              "/"
#endif



#ifdef INCLUDE_TFFS
#define INCLUDE_TFFS_DOSFS
#define FLASH_TFFS_ADRS          0xf0000000  /*FLASH头部的128MB用于TFFS*/
#define FLASH_TFFS_SIZE		   0x08000000	/*TFFS Flash memory size 128MB*/
#define INCLUDE_TFFS_SHOW

#define INCLUDE_TFFS_MOUNT  /*自动挂载TFFS 文件系统*/
    
#undef INCLUDE_FS_EVENT_UTIL  
#undef INCLUDE_FS_MONITOR   
#undef INCLUDE_ERF            
#undef INCLUDE_DEVICE_MANAGER 
#define INCLUDE_XBD        

#endif  /* INCLUDE_TFFS */

#if !defined(PRJ_BUILD)
/* configuration for bootrom */
#   define INCLUDE_ERF
#   define INCLUDE_XBD
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_DEVICE_MANAGER
#endif

/*  Define for FS */
#ifdef INCLUDE_DOSFS
#define INCLUDE_DOSFS_MAIN 
#define  INCLUDE_DOSFS_CACHE
#define INCLUDE_DOSFS_FAT
#define INCLUDE_DOSFS_DIR_VFAT
#define INCLUDE_DOSFS_DIR_FIXED  
#define INCLUDE_DOSFS_FMT      
#define  INCLUDE_DISK_UTIL

#ifndef COMPILE_BOOTROM
#define INCLUDE_DOSFS_CHKDSK

/*add dosFs disk check and repair when dosFS mount,  add by dongyang140528*/
#define DOSFS_DEFAULT_CREATE_OPTIONS     DOS_CHK_VERB_SILENT | DOS_CHK_REPAIR /*0xff02*/
#endif

#endif  /* INCLUDE_DOSFS */



#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_SHOW
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM  0x40
#   define AIM_MMU_INIT_PT_INCR 0x20
#   define AIM_MMU_INIT_RT_NUM  0x10
#   define AIM_MMU_INIT_RT_INCR 0x10

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

#   undef INCLUDE_HW_FP

/* ev2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   undef  INCLUDE_HW_FP
#   define INCLUDE_SW_FP

#endif  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

#define ROM_TEXT_ADRS           0xFFF00100
#define ROM_BASE_ADRS           0xFFF00000
#define ROM_SIZE                0x00100000

#define INCLUDE_DDR_SDRAM
#define LOCAL_MEM_LOCAL_ADRS    0x00000000
#define LOCAL_MEM_SIZE          PHY_MEM_SIZE
#define USER_RESERVED_MEM       0x00000000


#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE



#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS           LOCAL_MEM_LOCAL_ADRS

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */

