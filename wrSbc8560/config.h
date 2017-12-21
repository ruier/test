/* config.h - wrSbc85xx configuration header file */

/*
 * Copyright (c) 2002-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02y,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
02x,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
02w,24jun10,pmr  clean up exceptin extended vector support
02w,15apr10,clx  Update BSP version for vxWorks 6.9
02v,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
02u,24oct08,j_z  remove RAM_LOW_ADRS, RAM_HIGH_ADRS
02t,26aug08,d_l  correct the oscillator frequency.(WIND00127676)
02s,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
02r,23jul08,l_z  Support END2
02q,26may08,syt  modify BSP_REV form "/6" to "/7"
02p,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
02o,21aug07,mmi  remove legacy power management define
02o,17aug07,pmr  remove non-default components; fix cache error recovery
02n,12aug07,wap  Correct PCI_MSTR_MEM_BUS and PCI_BRIDGE_PIMMR_BASE_ADRS
                 (WIND00099736)
02m,30jul07,mil  Excluded PCI to workaround boot problem on some boards.
02l,27jul07,vik  Fix for WIND00099581, added macro INCLUDE_VXB_CMDLINE
02k,31may07,h_k  added INCLUDE_SIO_UTILS.
02j,23mar07,pmr  fix bootapp support
02i,05mar07,dtr  Use new PCi bbus controller driver.
02h,06feb07,d_z  Increase BSP revision to 2.0/6.
02g,01feb07,wap  Convert to VxBus TSEC driver
02f,31jan07,wap  VxBus CPM component is now named INCLUDE_VXB_CPM
02e,29aug06,wap  Add VxBus FCC driver support
02d,29nov06,rec	 NOR update - change RAM_HIGH_ADRS, don't lock bootrom text
02c,24oct06,jmt	 Add SYS_MODEL define for bspvts
02b,13sep06,tfo	 Remove WLAN support for Atheros AR521X, not supported
02a,09sep06,jmt	 Fix problem with INCLUDE_AR521X_END undefining INCLUDE_PCI
01z,30aug06,tfo	 WLAN support for Atheros AR521X
01y,28Jul06,tor	 incorporate PCI support, VXBUS mods
01x,06feb06,wap	 Add INCLUDE_PARAM_SYS
01w,15sep05,to	 revision update for VxWorks 6.2.
01v,06apr05,dtr	 Add in SNOOP for cache.
01v,13jun05,pcm	 removed INCLUDE_DOSFS
01u,27aug04,mil	 Modified TFFS macros.
01t,27sep04,mil	 Added conditional for L1 parity error handler (SPR 102069).
01s,30sep04,dtr	 Add in AIM MMU configuration.
01r,30jul04,md	 use default PM_RESERVED_MEM 
01r,14sep04,j_b	 remove INCLUDE_SHOW_ROUTINES (SPR 101418)
01q,10sep04,mil	 Make RAM_LOW_ADRS and RAM_HIGH_ADRS conditional.
01p,10jun04,mil	 Added L1 errata workaround.
01o,08jun04,mdo	 Change MMU configuration - dependency is elsewhere.
01n,26may04,mil	 Fixed L2 not being enabled by default.
01m,10may04,mil	 Repositioned defines of cache macros.
01l,05may04,mil	 Removed TSEC cache undef, added cache settings, N boot cmd.
01k,02feb04,mil	 Added config for L2 cache and L2 SRAM.
01j,23oct03,mil	 Cleanup.
01i,20oct03,mil	 Changed TFFS to optional, removed CTORS_DTORS.
01h,17oct03,mil	 Fixed RAM_HIGH_ADRS value.
01g,12sep03,mil	 Updated settings for cache, clock and interrupt.
01f,29jul03,dtr	 Added snoop and disable L2 because of dcbi/dcbst issue.
01e,22jul03,mil	 Added flash parameters.
01d,07jul03,mil	 Added CPM and SCC.
01c,25jun03,mil	 Added prjParams.h for project build.
01b,09oct02,dtr	 Added more defines for more complete base for BSP.
01a,03oct02,dtr	 Created.
*/


#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */


#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Base model string. sysModel adds processor details */
#define SYS_MODEL	"MPC85"

/*
 * ECC support
 * ECC is off by default, because the memory module included with
 * the wrSbc8560 board doesn't support it. If we enable it here
 * without replacing the module, the bootrom will hang.
 
#define INCLUDE_ECC

 */

#include "configAll.h"
#include "wrSbc85xx.h"

#define INCLUDE_VXBUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_PLB_BUS
#define INCLUDE_VXB_CMDLINE

/*
 * FCC ethernet support
 * This is only available on the 8560 board with CPM expansion card.
 */

#ifdef INCLUDE_CPM
#define INCLUDE_VXB_CPM
#define INCLUDE_FCC_VXB_END
#define INCLUDE_BCM52XXPHY
#define INCLUDE_MDIO
#endif /* INCLUDE_CPM */

/*
 * TSEC ethernet support
 */

#define INCLUDE_TSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_BCM54XXPHY
#define INCLUDE_GENERICPHY
#define INCLUDE_END

/*
 * PCI support
 * This is off by default. Uncomment the following #define to enable it.
 * With newer boards that have no additional PCI slots available, leaving
 * PCI support enabled here will cause the CPU to hang.

#define INCLUDE_PCI_BUS

 */

#ifdef INCLUDE_PCI_BUS

#define INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M85XX

/*
 * If PCI support and END support are available, enable the
 * Intel PRO/1000 ethernet driver too.
 */
#if defined(INCLUDE_END) || defined(INCLUDE_END2)
#define INCLUDE_GEI825XX_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_GEITBIPHY
#endif /* defined(INCLUDE_END) || defined(INCLUDE_END2) */

#endif /* INCLUDE_PCI_BUS */

#define DRV_RESOURCE_M85XXCCSR
#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/* Boot device and file location */

#define DEFAULT_BOOT_LINE \
"mottsec(0,0)host:/target/config/wrSbc85xx/vxWorks h=90.0.0.3 e=90.0.0.50 u=vxworks"
#define ALT_BOOT_LINE \
"motfcc(0,0)host:/target/config/wrSbc85xx/vxWorks h=90.0.0.3 e=90.0.0.50 u=vxworks"
#undef	USE_ALT_BOOTLINE	/* define when no bootrom or override NVRAM */


/* Clock speed and source  */

#define FREQ_33_MHZ	 33000000
#define FREQ_66_MHZ	 66000000
#define FREQ_99_MHZ	 99000000
#define FREQ_132_MHZ	132000000
#define FREQ_264_MHZ	264000000
#define FREQ_USR_MHZ	196800000	/* expr result CCB @ 200 MHz */

/*
 * Note: OSCILLATOR_FREQ is set to 66Mhz by default based on the
 * assumption that the wrSbc85xx board will be supplied with a 66Mhz
 * clock by the PCI backplane. If a 32-bit/33Mhz device is plugged
 * into one of the other slots in the backplane, the bus speed will
 * be reduced to 33Mhz. In this case, either the CCB clock PLL ratio
 * switch settings on the board will have to be changed, or OSCILLATOR_FREQ
 * must be changed to 33Mhz.
 */

#define OSCILLATOR_FREQ		FREQ_66_MHZ	/* oscillator installed */
/* make sure PLAT_FREQ_DEFAULT matches CCB CLK rate per DIP switches */
#define PLAT_FREQ_DEFAULT	(OSCILLATOR_FREQ * 3)  /* 3:1 CCB ratio */


/* MMU and cache */

#define CACHE_LIBRARY_MODE	(CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE	_MMU_TLB_ATTR_M
#define TLB_CACHE_MODE		(VM_STATE_CACHEABLE | \
				VM_STATE_MEM_COHERENCY)


#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#ifdef CACHE_LIBRARY_MODE
# define ICACHE_MODE	CACHE_LIBRARY_MODE
# define DCACHE_MODE	CACHE_LIBRARY_MODE
#else  /* CACHE_LIBRARY_MODE */
# define ICACHE_MODE	CACHE_COPYBACK
# define DCACHE_MODE	CACHE_COPYBACK
#endif	/* CACHE_LIBRARY_MODE */

#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef	USER_D_CACHE_MODE
#define USER_D_CACHE_MODE	(DCACHE_MODE)
#define USER_I_CACHE_ENABLE
#undef	USER_I_CACHE_MODE
#define USER_I_CACHE_MODE	(ICACHE_MODE)

#define INCLUDE_L2_CACHE
#define INCLUDE_L2_SRAM

#if defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_CACHE_SUPPORT)
# warning "Excluding L2 cache because INCLUDE_CACHE_SUPPORT is not defined"
# undef INCLUDE_L2_CACHE	/* L2 requires L1 */
#endif

#define L2SRAM_ADDR		0x7FFC0000
#define L2SRAM_WINDOW_SIZE	0x40000 

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM))
# define L2_CACHE_SIZE		L2SIZ_128KB
# define L2_SRAM_SIZE		L2SIZ_128KB
#elif (defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM))
# define L2_CACHE_SIZE		L2SIZ_256KB
# define L2_SRAM_SIZE		0
#else
# define L2_SRAM_SIZE		L2SIZ_256KB
# define L2_CACHE_SIZE		0
#endif

#define INCLUDE_BRANCH_PREDICTION


/* Serial channel and TTY
 *
 * INCLUDE_CPM should be defined for a MPC8560-based board, or undefined
 * otherwise.  It should be found in EXTRA_DEFINE in the Makefile for a
 * wrSbc8560 board, where the on-board DUART device will be used.  On a
 * wrSbc8540 board, the serial is the on-chip DUART device.  Both the
 * on-board or on-chip devices share the same physical com port.
 *
 * If a mezzanine card with a second serial port is installed on the
 * wrSbc8560, define INCLUDE_MEZZ_COM2 to avoid no output from both
 * COM1 and COM2.
 */

#undef INCLUDE_MEZZ_COM2

#ifdef INCLUDE_SCC_SERIAL
#  define NUM_TTY_BSP 2	       
#else 
#  define NUM_TTY_BSP 0
#endif	/* INCLUDE_SCC_SERIAL */

#if ! (defined(INCLUDE_SCC_SERIAL) || defined(DRV_SIO_NS16550))
#  undef  INCLUDE_TTY_DEV
#warning no console
#endif

#ifdef DRV_SIO_NS16550
#define NUM_TTY_VXBUS	2
#else
#define NUM_TTY_VXBUS	0
#endif

#undef	NUM_TTY
#define NUM_TTY (NUM_TTY_BSP + NUM_TTY_VXBUS)

/* EPIC configuration
 *
 * Define INCLUDE_EPIC_CRT_INTR to include EPIC critical handler
 */

#undef	INCLUDE_EPIC_CRT_INTR


/* Optional features support */

#define INCLUDE_SPE			/* signal processing engine */
#define INCLUDE_AUX_CLK			/* aux clock */
#undef	INCLUDE_TIMESTAMP		/* optional timestamp */
#define INCLUDE_USR_FLASH		/* maps usr (SIMM) flash */
#define INCLUDE_LOCAL_SDRAM		/* maps LBC SDRAM */


/* MAC Address configuration */

#define MAC_ADRS_LEN 6
#define MAX_MAC_ADRS 4
#define MAX_MAC_DEVS 2
#define ETHERNET_MAC_HANDLER

#define WR_ENET0		0x00	/* WR portion of MAC (MSB->LSB) */
#define WR_ENET1		0xA0
#define WR_ENET2		0x1E

#define CUST_ENET3_0		0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1		0xA1
#define CUST_ENET3_2		0xA2
#define CUST_ENET3_3		0xA3
#define CUST_ENET4		0xAA
#define CUST_ENET5		0xA0

#define WR_MAC0			0x00	/* WR portion of MAC (MSB->LSB) */
#define WR_MAC1			0xa0
#define WR_MAC2			0x1e

#define USR_MAC3a		0xb4	/* user portion of MAC address */
#define USR_MAC3b		0xb5	/* user portion of MAC address */
#define USR_MAC3c		0xb6	/* user portion of MAC address */
#define USR_MAC3d		0xb7	/* user portion of MAC address */
#define USR_MAC4		0x2e
#define USR_MAC5		0x3f

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

/* Remove unsupported drivers */

#undef INCLUDE_SM_NET
#undef INCLUDE_SM_SEQ_ADD


/* TFFS configuration
 *
 * Define INCLUDE_TFFS here if TrueFFS is needed.  Optionally, TFFS
 * can be used to program the bootrom when INCLUDE_TFFS_BOOTROM_OVERWRITE 
 * is defined.	Use caution to avoid erasing the bootrom accidentally.

#define  INCLUDE_TFFS

 */

#undef	INCLUDE_TFFS_BOOTROM_OVERWRITE	/* define only when flashing bootrom */

/* PCI configuration
 *
 */

#define INCLUDE_PCI_AUTOCONF

/*
 *  CPU Addr					     PCI Addr
 *  PCI_LOCAL_MEM_BUS	  -------------------------- PCI_MSTR_MEM_BUS
 *			  -			   - 
 *			  -			   -
 *  PCI_LOCAL_MEM_BUS +	  -------------------------- PCI_MSTR_MEM_BUS +
 *  PCI_LOCAL_MEM_SIZE	  -			   - PCI_MSTR_MEM_SIZE
 *			  -			   - 
 *			  -			   ----- PCI Bridge   
 *			  -			   -	 configuration regs
 *			  -			   - 
 *  CPU_PCI_MEM_ADRS	  -------------------------- PCI_MEM_ADRS
 *			  -			   - 
 *			  -			   - 
 *  CPU_PCI_MEMIO_ADRS	  -------------------------- PCI_MEMIO_ADRS
 *			  -			   - 
 *			  -			   - 
 *  CPU_PCI_IO_ADRS	  -------------------------- PCI_IO_ADRS
 *			  -			   -
 *			  -			   -
 *  CPU_PCI_IO_ADRS +	  -------------------------- PCI_IO_ADRS +
 *  CPU_PCI_IO_SIZE	  -			   - PCI_IO_SIZE
 *			  -			   -
 *			  -			   -
 *			  ----PCI Bridge	   -
 *			  -------------------------- 4GBytes 
 */

/* For custom sysPciAutoConfig.c */

/* PCI based addresses */

#define PCI_MEM_ADRS		0x90000000 
#define PCI_MEM_SIZE		0x01000000	/* 16MB */
#define PCI_MEMIO_ADRS		0xa0000000
#define PCI_MEMIO_SIZE		0x01000000	/* 16MB */
#define PCI_IO_ADRS		0xb0000000 
#define PCI_IO_SIZE		0x01000000	/* 16MB */

#define PCI_MEM_SIZE_MASK	PCI_ATTR_WS_16M
#define PCI_MEMIO_SIZE_MASK	PCI_ATTR_WS_16M
#define PCI_IO_SIZE_MASK	PCI_ATTR_WS_16M

/* CPU based addresses */

#define CPU_PCI_MEM_ADRS	0x90000000
#define CPU_PCI_MEM_SIZE	PCI_MEM_SIZE 
#define CPU_PCI_MEMIO_ADRS	0xa0000000
#define CPU_PCI_MEMIO_SIZE	PCI_MEMIO_SIZE 
#define CPU_PCI_IO_ADRS		0xb0000000
#define CPU_PCI_IO_SIZE		PCI_IO_SIZE

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS	0x00000000	/* align on 512 MB */
#define PCI_MSTR_MEM_SIZE	LOCAL_MEM_SIZE
#define PCI_BRIDGE_PIMMR_BASE_ADRS	0x50000000

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS	0x00000000
#define PCI_LOCAL_MEM_SIZE	PCI_MSTR_MEM_SIZE
#define PCI_LOCAL_MEM_SIZE_MASK PCI_ATTR_WS_512M

#ifndef PCI_CFG_TYPE
#   ifdef INCLUDE_PCI_AUTOCONF
#	define PCI_CFG_TYPE PCI_CFG_AUTO
#   else
#	define PCI_CFG_TYPE PCI_CFG_FORCE
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif	/* PCI_CFG_TYPE */

/* Saving boot parameters */

/* NVRAM configuration */

#define INCLUDE_NV_RAM
#ifdef	INCLUDE_NV_RAM
#   define NV_RAM_ADRS		0x28b00000
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE		0x2000 /* 8KB */
#   define NV_RAM_INTRVL	1
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET	0
#   define NV_MAC_ADRS_OFFSET	0x200

#   define NV_RAM_WR_ENBL	sysNvRamWrEnbl()
#   define NV_RAM_WR_DSBL	sysNvRamWrDsbl()
#   define NV_RAM_READ		sysNvRamRead
#   define NV_RAM_WRITE		sysNvRamWrite
#endif	/* INCLUDE_NV_RAM */
 
/* Using flash to store boot params is not supported */

#undef INCLUDE_FLASH
#ifdef INCLUDE_FLASH
#   define SYS_FLASH_TYPE	FLASH_28F640J3A	   /* flash device type */
#   define FLASH_WIDTH		2
#   define FLASH_CHIP_WIDTH	2
#   define FLASH_WIDTH_SPECIAL_2
#   define FLASH_SEGMENT_SIZE	0x20000

#   define FLASH_ADRS		(0xff800000 + 0x700000 - FLASH_SEGMENT_SIZE)
#   define FLASH_SIZE		FLASH_SEGMENT_SIZE
#   define FLASH_SIZE_WRITEABLE FLASH_SEGMENT_SIZE
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE		FLASH_SEGMENT_SIZE
#   undef  FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET	FLASH_SEGMENT_SIZE - 0x200
#endif	/* INCLUDE_FLASH */

#ifndef NV_RAM_SIZE
#   define NV_RAM_SIZE		NONE
#endif


/* Memory addresses */
 
#define LOCAL_MEM_SIZE		0x20000000	/* 512 MB memory available */
#define LOCAL_MEM_LOCAL_ADRS	0x00000000	/* Base of RAM */

 
/* Toolchain related defines */

#undef	INCLUDE_HW_FP		/* disable hardware fp support */
#define INCLUDE_SW_FP		/* enable software fp support */

#ifdef INCLUDE_MMU_BASIC

#define INCLUDE_AIM_MMU_CONFIG

#ifndef BOOTAPP
#define INCLUDE_MMU_OPTIMIZE 
#endif

#define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/* 
 * The following parameters are to configure initial memory usage for 
 * page table and region tables and in event of running out the increment 
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks). 
 */
#define AIM_MMU_INIT_PT_NUM 0x40
#define AIM_MMU_INIT_PT_INCR 0x20
#define AIM_MMU_INIT_RT_NUM 0x10
#define AIM_MMU_INIT_RT_INCR 0x10

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

#ifndef BOOTAPP
#define INCLUDE_LOCK_TEXT_SECTION
#endif	  

#undef INCLUDE_PAGE_SIZE_OPTIMIZATION /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */

/*
 * The constants ROM_SIZE and RAM_XXX_ADRS are defined
 * in both config.h and Makefile.  All definitions for these constants
 * must be identical.
 */
 
#define ROM_BASE_ADRS		0xFFF00000
#define ROM_TEXT_ADRS		0xFFF00100
#define ROM_SIZE		0x00100000

#define USER_RESERVED_MEM	0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif	/* INCconfigh */

#if defined(PRJ_BUILD)
#    include "prjParams.h"
#endif /* PRJ_BUILD */
