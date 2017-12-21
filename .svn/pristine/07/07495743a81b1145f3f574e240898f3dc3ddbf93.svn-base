/* config.h - Motorola MPC85XX ADS (ads8540/ads8560) configuration file */

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
02s,24oct12,d_l  Update BSP_REV. (WIND00366162)
02r,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
02q,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
02p,15apr10,clx  Update BSP version for vxWorks 6.9
02o,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
02n,23oct08,j_z  remove RAM_LOW_ADRS and RAM_HIGH_ADRS.
02m,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
02l,04aug08,fao  update bsp rev for vxWorks 6.7 and add END2 support
02k,26jun08,wap  Add IO16 PCI definitions
02j,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
02i,21aug07,mmi  remove legacy power management define
02h,13aug07,wap  Convert to VxBus (WIND00100585)
02g,08aug07,agf  inc rev for vxW6.6
02f,05mar07,rlg  updates for NOR conversion
02e,31jan07,wap  Undo temporary INCLUDE_CMP change
02d,25jan07,d_c  Change INCLUDE_CPM to INCLUDE_CPM so not to conflict
                 with INCLUDE_CPM component
02c,11dec06,y_w  Add AUX_CLK_RATE related 3 macros, CQ:#63379,#63385
02b,28may06,jmt  Add SYS_MODEL define
02a,05apr05,dtr  Add SNOOP in as FCC now supports this mode.
02a,13jun05,pcm  removed INCLUDE_DOSFS
01z,22mar05,dtr  Include secondary TSEC.
01y,13oct04,dtr  Set ADS_BOARD_REVA by default and make 256MB default memory
                 size.
01x,27sep04,dtr  SPR 99491
01w,14sep04,j_b  remove INCLUDE_SHOW_ROUTINES (SPR 101418)
01v,10sep04,mil  Make RAM_LOW_ADRS and RAM_HIGH_ADRS conditional.
01u,16aug04,dtr  Merge from T2.2.1.
01u,30jul04,md   Use default PM_RESERVED_MEM 
01t,05aug04,dtr  Add support for RevA board.
01s,24jun04,mdo  SPR #98438
01r,10jun04,dtr  Add define for L1 instruction cache errata.
01q,09jun04,mil  Converted to VxWorks 6.0.
01p,08jun04,mdo  Change MMU configuration - dependency is elsewhere.
01o,01apr04,dtr  Adding support for L1 parity and parity recovery mechanism.
01n,19jan04,dtr  Enable D_CACHE,L2 CACHE and FCC(8560 only) by default.
01m,28oct03,dtr  Support only 32MB for now.
01l,21oct03,dtr  Trim config.h for final release.
01k,02oct03,dtr  Add back in SPE support.
01j,12sep03,dtr  Change some clock defines.
01i,20aug03,dtr  Adding support for TSEC.
01h,14aug03,dtr  Remove undefs for FP and windview.
01g,04aug03,dtr  Modify configuration elements for LBC SDRAM and DDR SDRAM.
01f,29jul03,dtr  Added snoop and disable L2 because of dcbi/dcbst issue.
01e,22jul03,mil  Added flash parameters.
01d,07jul03,mil  Added CPM and SCC.
01c,25jun03,mil  Added prjParams.h for project build.
01b,09oct02,dtr  Added more defines for more complete base for BSP.
01a,03oct02,dtr  Created.
*/
#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/2"        /* 0 for first revision */

/* Base model string. sysModel adds processor details */
#define SYS_MODEL     "Motorola"

/* Define Clock Speed and source  */

#define	FREQ_33_MHZ	 33333333
#define	FREQ_66_MHZ	 66666666
#define	FREQ_100_MHZ	 99999999
#define	FREQ_133_MHZ	133333333
#define	FREQ_266_MHZ	266666666

/* Build for Revision A of ADS board by default */
#define ADS_BOARD_REVA

/*
 * The following is configured by the board but 33MHZ is the only supported
 * freq for RevA board. BSP defaults to Pilot Rev.
 */
#ifdef ADS_BOARD_REVA
#define OSCILLATOR_FREQ	FREQ_33_MHZ
#else
#define	OSCILLATOR_FREQ	FREQ_66_MHZ
#endif

/* This value is the 60x bus-assigned SDRAM Refresh Timer PSRT setting */
#define	LSRT_VALUE	0x20

/*
 * This value is the setting for the MPTPR[PTP] Refresh timer prescaler.
 * The value is dependent on the OSCILLATOR_FREQ value.  For other values
 * a conditionally compiled term must be created here for that OSCILLATOR_FREQ
 * value.
 *
 * BRGCLK_DIV_FACTOR
 * Baud Rate Generator division factor - 0 for division by 1
 *					 1 for division by 16
 */

#define	DIV_FACT_1	0
#define	DIV_FACT_16	1

/* Assume Worst case of 333MHz CCB with local bus clk ratio of 4
 * ie revA board settings */
#define	TPR			0x2000
#define BRGCLK_DIV_FACTOR	DIV_FACT_16

#define M8260_BRGC_DIVISOR	BRGCLK_DIV_FACTOR

#include "configAll.h"

#include "ads85xx.h"

/* VxBus support */

#define INCLUDE_VXBUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_PLB_BUS
#define INCLUDE_VXB_CMDLINE

/*
 * PCI support
 *
 * Off by default.

#define INCLUDE_PCI_BUS

 */

#ifdef INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M85XX
#define DRV_RESOURCE_M85XXCCSR
#endif

#define DEFAULT_BOOT_LINE \
"mottsec(0,0)host:/target/config/ads85xx/vxWorks h=90.0.0.3 e=90.0.0.50 u=vxworks"

#define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE         (VM_STATE_CACHEABLE | \
				VM_STATE_MEM_COHERENCY)
#define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define INCLUDE_CACHE_SUPPORT
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_LIBRARY_MODE)

#define  INCLUDE_L2_CACHE
#undef  INCLUDE_L2_SRAM

#define INCLUDE_BRANCH_PREDICTION

#if ((defined(INCLUDE_L2_CACHE)) && (defined(INCLUDE_L2_SRAM)))
#define L2_CACHE_SIZE      L2SIZ_128KB
#define L2_SRAM_SIZE       L2SIZ_128KB
#elif ((defined(INCLUDE_L2_CACHE)) && (!defined(INCLUDE_L2_SRAM)))
#define L2_CACHE_SIZE      L2SIZ_256KB
#define L2_SRAM_SIZE       L2SIZ_128KB /* Not Used */
#else
#define L2_SRAM_SIZE       L2SIZ_256KB
#define L2_CACHE_SIZE      L2SIZ_128KB /* Not Used */
#endif

#define L2SRAM_ADDR        0x7FFC0000
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
#define INCLUDE_SPE

/*
 * PCI ethernet support
 * Any of the VxBus PCI ethernet drivers below will work on this BSP.
 * Only the FEI driver is on by default.

#undef  INCLUDE_AM79C97X_VXB_END
#undef  INCLUDE_AN983_VXB_END
#undef  INCLUDE_GEI825XX_VXB_END
#undef  INCLUDE_MVYUKONII_VXB_END
#undef  INCLUDE_MVYUKON_VXB_END
#undef  INCLUDE_NS8381X_VXB_END
#undef  INCLUDE_RTL8139_VXB_END
#undef  INCLUDE_RTL8169_VXB_END
#undef  INCLUDE_TC3C905_VXB_END

 */
                                                                         
/*
 * PHY and MII bus support
 * These are used in conjunction with the PCI ethernet drivers above.
 * the GEITBI driver is only needed when using a PRO/1000 card with
 * the GEI driver which supports fiber media instead of copper.
 * The RealTek PHY drivers are used with the RealTek 8139 and 8169
 * devices, respectively.
 *

#undef  INCLUDE_GEITBIPHY
#undef  INCLUDE_RTL8201PHY
#undef  INCLUDE_RTL8169PHY

 */

/*
 * If PCI support is enabled, include the FEI and GEI drivers
 * by default.
 */

#ifdef INCLUDE_PCI_BUS
#define  INCLUDE_FEI8255X_VXB_END	/* Intel PRO/100 support */
#define  INCLUDE_GEI825XX_VXB_END	/* Intel PRO/1000 support */
#define  INCLUDE_GENERICPHY		/* generic 10/100/100 PHY support */
#define  INCLUDE_GEITBIPHY		/* Intel PRO/1000 fiber support */
#endif /* INCLUDE_PCI_BUS */

/*
 * TSEC ethernet support
 *
 * The ADS8560 provides both copper and fiber connections for the two
 * TSEC ports, via a Marvell 88E1011S PHY. The PHY is dual mode, and
 * defaults to copper operation. Fiber mode can be enabled by defining
 * INCLUDE_TSEC_FIBER. This will cause the MV88E1X11PHY driver to
 * program the PHY for GMII to fiber mode and select the fiber port
 * in favor of the copper one. When configured for fiber mode, the
 * copper port is disabled, and vice-versa.
 *
 * Note that the Freescale documentation for the ADS8560 board implies
 * that it's also possible to select fiber mode by altering the switch
 * settings to set the TSEC ports for TBI mode. In theory this should
 * also work, but has not been tested. The method used to select fiber
 * mode here does not require changing any switch or jumper settings.

#define INCLUDE_TSEC_FIBER_PORT0
#define INCLUDE_TSEC_FIBER_PORT1

 */

#define INCLUDE_TSEC_VXB_END
#define INCLUDE_END                  /* only END-style driver for FCC/TSEC */
#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY

/* INCLUDE_CPM will decides if the serial console will be using
 * the DUART on the 8540, or the SCC on the 8560.  Both should
 * output to the same port.
 */

#ifdef INCLUDE_CPM
#define INCLUDE_SCC_SERIAL

/*
 * FCC ethernet support (only available if CPM support is also enabled)
 *
 * Note: the Davicom DM9161 PHY driver is required for this board,
 * in order to get 100Mbps mode to work correctly. The PHY seems to
 * default to having the 'scrambler bypass' bit set in its PHY specific
 * control register, and this must be turned off in order to exchange
 * data in 100Mbps mode.
 *
 * The FCC driver depends on the following:
 *
 * - VxBus CPM driver (manages access to CPCR register)
 * - MDIO driver (manages access to PHY MDIO pins)
 * - DM9161PHY driver (support for Davicom DM9161 PHY)
 *
 * Note that the Davicom PHY driver is required for the ADS8560
 * board. For some reason, the PHY comes out of reset with the
 * 'scrambler bypass' bit set in its vendor specific config register,
 * which disables 10/100 mode. The DM9161PHY driver clears this bit
 * to work around this problem.
 */

#define INCLUDE_FCC_VXB_END
#define INCLUDE_VXB_CPM
#define INCLUDE_DM9161PHY
#define INCLUDE_MDIO

#else
#define INCLUDE_DUART
#endif

/* Serial channel and TTY */
#undef	NUM_TTY
#if (defined(INCLUDE_DUART) || defined(INCLUDE_SCC_SERIAL))
#  define NUM_TTY 2
#else
#  define NUM_TTY 0
#  undef  INCLUDE_TTY_DEV
#endif  /* INCLUDE_DUART || INCLUDE_SCC_SERIAL */

/* Optional timestamp support */

#define	INCLUDE_TIMESTAMP
#define	INCLUDE_AUX_CLK

#define EPIC_EX_DFT_SENSE	EPIC_SENSE_EDG
#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW
#define EPIC_IN_DFT_POLAR	EPIC_INT_ACT_HIGH

/* clock rates */

#define	SYS_CLK_RATE_MIN	1	/* minimum system clock rate */
#define	SYS_CLK_RATE_MAX	8000	/* maximum system clock rate */

#define WDT_RATE_MIN        (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_29))
#define WDT_RATE_MAX        (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_21))

#define SYS_CLK_FREQ		OSCILLATOR_FREQ * 10  /*refer to sysClkFreqGet()*/ 
#define AUX_CLK_RATE        (( SYS_CLK_FREQ >> 3) / (1 << FIT_DIVIDER_TAP_18 ))
                                                          /* apprx   158 Hz */ 
#define AUX_CLK_RATE_MIN    (( SYS_CLK_FREQ >> 3) / (1 << FIT_DIVIDER_TAP_21 ))
                                                          /* apprx   19  Hz */ 
#define AUX_CLK_RATE_MAX    (( SYS_CLK_FREQ >> 3) / (1 << FIT_DIVIDER_TAP_12 )) 
                                                          /* apprx 10172 Hz */ 

/* add on-chip drivers */
#ifdef INCLUDE_PCI_BUS

/*
CPU Addr                                         PCI Addr
PCI_LOCAL_MEM_BUS     -------------------------- PCI_MSTR_MEM_BUS
                      -                        -
                      -                        -
PCI_LOCAL_MEM_BUS +   -------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE    -                        - PCI_MSTR_MEM_SIZE
                      -                        -
                      -                        ----- PCI Bridge
                      -                        -     configuration regs
                      -                        -
CPU_PCI_MEM_ADRS      -------------------------- PCI_MEM_ADRS
                      -                        -
                      -                        -
CPU_PCI_MEMIO_ADRS    -------------------------- PCI_MEMIO_ADRS
                      -                        -
                      -                        -
CPU_PCI_IO_ADRS       -------------------------- PCI_IO_ADRS
                      -                        -
                      -                        -
CPU_PCI_IO_ADRS +     -------------------------- PCI_IO_ADRS +
CPU_PCI_IO_SIZE       -                        - PCI_IO_SIZE
                      -                        -
                      -                        -
                      ----PCI Bridge           -
                      -------------------------- 4GBytes
*/



/* for custom sysPciAutoConfig.c */

/* PCI based addresses */
#define PCI_MEM_ADRS         0x80000000
#define PCI_MEM_SIZE         0x01000000   /* 16MB */
#define PCI_MEMIO_ADRS       0x90000000
#define PCI_MEMIO_SIZE       0x01000000   /* 16MB */
#define PCI_IO_ADRS          0xa0000000
#define PCI_IO_SIZE          0x01000000   /* 16MB */
#define PCI_IO16_ADRS          0xa0800000
#define PCI_IO16_SIZE          0x00800000   /* 16MB */

#define PCI_MEM_SIZE_MASK  PCI_ATTR_WS_16M
#define PCI_MEMIO_SIZE_MASK PCI_ATTR_WS_16M

/* CPU based addresses */
#define CPU_PCI_MEM_ADRS          0x80000000
#define CPU_PCI_MEM_SIZE          PCI_MEM_SIZE
#define CPU_PCI_MEMIO_ADRS        0x90000000
#define CPU_PCI_MEMIO_SIZE        PCI_MEMIO_SIZE
#define CPU_PCI_IO_ADRS           0xa0000000
#define CPU_PCI_IO_SIZE           PCI_IO_SIZE

/* CPU from PCI bus */
#define PCI_MSTR_MEM_BUS           0x00000000
#define PCI_MSTR_MEM_SIZE          0x40000000 /* 1G */
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x50000000

/* CPU Address that is visible from PCI */
#define PCI_LOCAL_MEM_BUS         0x00000000
#define PCI_LOCAL_MEM_SIZE        PCI_MSTR_MEM_SIZE
#define PCI_LOCAL_MEM_SIZE_MASK   PCI_ATTR_WS_1G

#ifndef PCI_CFG_TYPE
#   ifdef INCLUDE_PCI_AUTOCONF
#      define PCI_CFG_TYPE PCI_CFG_AUTO
#   else
#      define PCI_CFG_TYPE PCI_CFG_FORCE
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI_BUS */

#if (defined(INCLUDE_END) || defined(INCLUDE_END2))

/* MAC Address configuration */

#define MAC_ADRS_LEN 6
#define MAX_MAC_ADRS 4
#define MAX_MAC_DEVS 2
#define ETHERNET_MAC_HANDLER
#define INCLUDE_MAC_COMPAT	/* Compatibility with older bootroms. */

#define FS_ENET0		0x00  /* freescale specific portion of MAC */
#define FS_ENET1		0x04
#define FS_ENET2		0x9F

#define WR_ENET0		FS_ENET0
#define WR_ENET1		FS_ENET1
#define WR_ENET2		FS_ENET2

#define CUST_ENET3_0            0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1            0xA1
#define CUST_ENET3_2            0xA2
#define CUST_ENET3_3            0xA3
#define CUST_ENET4              0xAA
#define CUST_ENET5              0xA0

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

#endif /* (defined(INCLUDE_END) || defined(INCLUDE_END2)) */

/* remove unnecessary drivers */

#undef INCLUDE_SM_NET
#undef INCLUDE_SM_SEQ_ADD

/* flash for boot params */
#define INCLUDE_FLASH
#ifdef  INCLUDE_FLASH
#  define SYS_FLASH_TYPE        FLASH_28F640J3A    /* flash device type */
#ifdef ADS_PROTOTYPE
#  define FLASH_WIDTH           2
#else /* ADS_PROTOTYPE */
#  define FLASH_WIDTH           4
#endif /* ADS_PROTOTYPE */
#  define FLASH_CHIP_WIDTH      2
#  define FLASH_WIDTH_SPECIAL_2
#  define FLASH_SEGMENT_SIZE    0x20000

#  define FLASH_ADRS            (0xff800000 + 0x700000 - FLASH_SEGMENT_SIZE)
#  define FLASH_SIZE            FLASH_SEGMENT_SIZE
#  define FLASH_SIZE_WRITEABLE  FLASH_SEGMENT_SIZE
#  define NV_RAM_SIZE           FLASH_SEGMENT_SIZE
#  undef  FLASH_NO_OVERLAY
#  undef  NV_BOOT_OFFSET
#  define NV_BOOT_OFFSET        FLASH_SEGMENT_SIZE - 0x200
#  define NV_MAC_ADRS_OFFSET	(-0x200)
#else  /* INCLUDE_FLASH */
#  define NV_RAM_SIZE           NONE
#endif  /* INCLUDE_FLASH */

#ifdef ADS_PROTOTYPE
#ifdef VISION_VXWORKS
#define FLASH_BASE_ADRS                 0x3f800000
#else
#define FLASH_BASE_ADRS                 0xff800000
#endif
#define FLASH_ADRS_MASK                 0xff800000
#define FLASH_WINDOW_SIZE               0x00800000
#else /* ADS_PROTOTYPE */
#ifdef VISION_VXWORKS
#define FLASH_BASE_ADRS                 0x3f000000
#else
#define FLASH_BASE_ADRS                 0xff000000
#endif
#define FLASH_ADRS_MASK                 0xff000000
#define FLASH_WINDOW_SIZE               0x01000000
#endif /* ADS_PROTOTYPE */

/* Memory addresses */

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */
#define LOCAL_MEM_SIZE          0x10000000 /* 256 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000 /* Base of RAM */

#define INCLUDE_LBC_SDRAM
#define INCLUDE_DDR_SDRAM
#define INCLUDE_SECONDARY_DRAM
/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */
#define LOCAL_MEM_SIZE2          0x4000000      /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS2    0x10000000      /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK  0xfc000000
#define LBC_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE2
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

#define INCLUDE_CTORS_DTORS

/* Using software FP support.  Although task based 32 bit HW_FP is allowed,
 * no optimized libraries are available. The kernel doesn't execute any hw
 * fp instructions */
#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP

#ifndef BOOTAPP
#ifdef INCLUDE_MMU_BASIC

#define INCLUDE_AIM_MMU_CONFIG
#define INCLUDE_MMU_OPTIMIZE 

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

#define INCLUDE_LOCK_TEXT_SECTION     
#undef INCLUDE_PAGE_SIZE_OPTIMIZATION /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */
#endif /* BOOTAPP */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define ROM_TEXT_ADRS 0xFFF00100

#define ROM_BASE_ADRS 0xFFF00000
#define ROM_SIZE      0x00100000

#define USER_RESERVED_MEM 0x000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
