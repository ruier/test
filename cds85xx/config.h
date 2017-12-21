/* config.h - Freescale MPC85xx BSP (8555/8541) configuration file */

/*
 * Copyright (c) 2004-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02p,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01o,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01n,15apr10,clx  Update BSP version for vxWorks 6.9
01m,11jan10,pch  add vxprj support for DDR sizing
01l,08dec09,pch  CQ195058: Clean up DDR sizing
01k,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01j,24aug09,pmr  WIND00178151: remove #undef INCLUDE_PCI_BUS
01i,23feb09,x_z  add M command support and update BSP version.(WIND00046372)
01h,16oct08,j_z  remove RAM_HIGH_ADRS and RAM_LOW_ADRS
01g,23sep08,pmr  correct setting of aux clk limits (WIND00128655)
01f,04aug08,fao  update bsp rev for vxWorks 6.7
01e,17sep07,h_k  removed INCLUDE_PCI_BUS_SHOW. (CQ:105028)
01d,23aug07,pdg  added vxBus m85xxPci support
01c,22aug07,wap  fixed the mottsec device not found error
01b,16aug07,wap  Switch to new VxBus TSEC driver, remove FCC support
                 (cds8541/cds8555 boards have no FCC ports) (WIND00101678)
01a,10jul07,p_g  added macro INCLUDE_VXB_CMDLINE
01i,07may07,???  added INCLUDE_DMA_SYS
01h,23mar07,pmr  fix bootapp support
01g,05mar07,dtr  Undefine PCI_BUS because virtual PCi layer broken.
01f,25jan07,ami  Ported to vxBus
01e,07dec06,y_w  Add SYS_MODEL macro define, modify AUX_CLK_RATE
                 related 3 macros, CQ:WIND00061260
01e,02jan07,rlg  Updates for NOR
01d,26apr05,kab  Doc fixes (SPR 108364)
01c,22apr05,dtr  Set CACHE_SNOOP_ENABLE by default - SPR 107990.
01c,14jun05,pcm  removed INCLUDE_DOSFS
01b,15nov04,jln  Support TFFS, PCI 1/2
01a,10sep04,jln  Modify from ads85xx/config.h/01q
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Base model string. sysModel adds processor details */

#define SYS_MODEL     "Freescale"

/* Define Clock Speed and source  */

#define	FREQ_33_MHZ	 33333333
#define	FREQ_66_MHZ	 66666666
#define	FREQ_100_MHZ	 99999999
#define	FREQ_133_MHZ	133333333
#define	FREQ_266_MHZ	266666666

/* only 33Mhz is verified in CDS REVC board */ 

#define	OSCILLATOR_FREQ	FREQ_33_MHZ
#define DEFAULT_SYSCLKFREQ FREQ_266_MHZ
#undef FORCE_DEFAULT_FREQ /* Use to force freq used with DUART/Timers etc */

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

/* Fix me:
 * Assume Worst case of 333MHz CCB with local bus clk ratio of 4 
 * ie revA board settings  
 */

#define	TPR			0x2000
#define BRGCLK_DIV_FACTOR	DIV_FACT_16

#define M8260_BRGC_DIVISOR	BRGCLK_DIV_FACTOR

#include "configAll.h"

#include "cds85xx.h"


#define DEFAULT_BOOT_LINE \
"mottsec(0,0)host:/wind/river/target/config/cds85xx/vxWorks h=192.168.24.1 e=192.168.24.101 u=demo"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

# define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
# define TLB_CACHE_MODE         VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#define INCLUDE_CACHE_SUPPORT 
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE  (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE (CACHE_LIBRARY_MODE)

#define  INCLUDE_L2_CACHE
#undef   INCLUDE_L2_SRAM

#define INCLUDE_BRANCH_PREDICTION

#if ((defined(INCLUDE_L2_CACHE)) && (defined(INCLUDE_L2_SRAM)))
#define L2_CACHE_SIZE      L2SIZ_128KB
#define L2_SRAM_SIZE       L2SIZ_128KB
#elif ((defined(INCLUDE_L2_CACHE)) && (!defined(INCLUDE_L2_SRAM)))
#define L2_CACHE_SIZE      L2SIZ_256KB
#define L2_SRAM_SIZE       0         /* Not Used */
#else
#define L2_SRAM_SIZE       L2SIZ_256KB
#define L2_CACHE_SIZE      0         /* Not Used */
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

#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#define INCLUDE_PLB_BUS

/* 
 * PCI is not defined by default to reduce footprint.
 * To include, use the project facility, or uncomment the following line.
 *
 * Note: if you #undef this here, it will be impossible to add in the project
 * facility.  Better to not #define or #undef it.
 *
#define INCLUDE_PCI_BUS
 */

#define INCLUDE_VXB_CMDLINE
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define HWMEM_POOL_SIZE 50000

#ifdef INCLUDE_PCI_BUS

#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M85XX
#define DRV_RESOURCE_M85XXCCSR
#define INCLUDE_FEI8255X_VXB_END	/* Intel PRO/100 support */
#define INCLUDE_GENERICPHY		/* generic 10/100/100 PHY support */

#endif /* INCLUDE_PCI_BUS */

/* serial driver */

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/* TSEC is included */

#define INCLUDE_TSEC_VXB_END
#define INCLUDE_END

#if (defined(INCLUDE_TSEC_VXB_END) || defined(INCLUDE_TSEC_VXB_END2))
#define INCLUDE_MII_BUS
#define INCLUDE_BCM54XXPHY
#define INCLUDE_GENERICPHY
#endif /* (defined(INCLUDE_TSEC_VXB_END) || defined(INCLUDE_TSEC_VXB_END2)) */

#endif

#define ETHERNET_MAC_HANDLER

#define ENET_DEFAULT0   0x00
#define ENET_DEFAULT1   0xA0
#define ENET_DEFAULT2   0x1E

/* Customer portion of MAC address */

#define CUST_ENET3      0x11  
#define CUST_ENET4      0x01
#define CUST_ENET5_0    0x10
#define CUST_ENET5_1    0x11

#define MAX_MAC_ADRS    2
#define MAC_ADRS_LEN    6
#define MAX_MAC_DEVS    1 

/* console is always DUART */

#undef INCLUDE_DUART

/* Serial channel and TTY */

#if defined(INCLUDE_DUART) 
#  undef  NUM_TTY
#  define NUM_TTY 1
#endif /* INCLUDE_DUART */

/* CPM is not supported */

#undef INCLUDE_CPM

/* Optional timestamp support */

#define	INCLUDE_TIMESTAMP
#define	INCLUDE_AUX_CLK

/* Clock rates */

#define	SYS_CLK_RATE_MIN	1	/* minimum system clock rate */
#define	SYS_CLK_RATE_MAX	8000	/* maximum system clock rate */

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_29))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_21))

#define AUX_CLK_RATE         (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_18))
                                                   /* apprx   95 Hz */
#define AUX_CLK_RATE_MIN     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_21))
                                                   /* apprx   11 Hz */
#define AUX_CLK_RATE_MAX     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_12))
                                                   /* apprx 6103 Hz */

/* TrueFFS flash support */

#undef INCLUDE_TFFS

#ifdef  INCLUDE_TFFS
#define INCLUDE_SHOW_ROUTINES	/* show routines for system facilities*/
#endif  /* INCLUDE_TFFS */

#ifdef INCLUDE_PCI_BUS
#define EPIC_EX_DFT_POLAR	EPIC_INT_ACT_LOW

/* define this MACRO to enable PCI 2 */

#undef  INCLUDE_CDS85XX_SECONDARY_PCI

/*  cds85xx support dual PCI controllers */

/*
********************************************
   CPU Addr                                         PCI Addr ( PCI1 or PCI2)


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
                         -                        - 
CPU_PCI_MEMIO_ADRS       -------------------------- PCI_MEMIO_ADRS
CPU_PCI_MEMIO_ADRS2      -                        - PCI_MEMIO_ADRS2
                         -                        - 
CPU_PCI_IO_ADRS  (PCI1)  -------------------------- PCI_IO_ADRS
CPU_PCI_IO_ADRS2 (PCI2)  -                        - PCI_IO_ADRS2
                         -                        -
CPU_PCI_IO_ADRS  (PCI1)+ -------------------------- PCI_IO_ADRS +
CPU_PCI_IO_ADRS2 (PCI2)                             PCI_IO_ADRS2
CPU_PCI_IO_SIZE          -                        - PCI_IO_SIZE
                         -                        -
*************************************************/

/* PCI based addresses */

#define PCI_LAW_BASE             0x50000000       /* PCI LAW window */
#define PCI_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCI_MEM_SIZE             0x04000000        /* 64 MB */
#define PCI_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCI_IO_SIZE              0x04000000        /* 64 MB */

#define PCI_MEM_ADRS             (PCI_LAW_BASE)                     
#define PCI_MEMIO_ADRS           (PCI_MEM_ADRS   + PCI_MEM_SIZE)    
#define PCI_IO_ADRS              (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)  

#ifdef INCLUDE_CDS85XX_SECONDARY_PCI

#define PCI2_LAW_BASE            0x60000000       /* PCI LAW window */
#define PCI2_LAW_SIZE            0x10000000       /* PCI LAW size   */

#define PCI_MEM_ADRS2            (PCI2_LAW_BASE)                     
#define PCI_MEMIO_ADRS2          (PCI_MEM_ADRS2   + PCI_MEM_SIZE)    
#define PCI_IO_ADRS2             (PCI_MEMIO_ADRS2 + PCI_MEMIO_SIZE)  

#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

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
 * Set the Carrier Card's SW2[12]=00 for the following configuration,
 * the bootrom would be in the last 1 MB of flash0, and UBOOT would still 
 * be in the flash 1.
 */  

#define FLASH_WINDOW_SIZE               0x00800000 
#define CDS85XX_FLASH_RESERVED_SIZE     0x00100000

#ifdef INCLUDE_TFFS

/* Second flash for TFFS support */

#undef  INCLUDE_CDS85XX_SECOND_TFFS_FLASH
#endif /* INCLUDE_TFFS */

/* LBC CS0 - flash 0 - default for bootrom */ 

#define FLASH_BASE_ADRS                 0xff800000
#define FLASH_ADRS_MASK                 0xff800000

/* LBC CS1 - flash 1 - default for UBOOT */ 

#define FLASH1_BASE_ADRS                0xff000000
#define FLASH1_ADRS_MASK                0xff800000

/* LBC CS2 - SDRAM */

#undef INCLUDE_LBC_SDRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE2            0x4000000      /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS2      0xf8000000     /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK  0xfc000000
#define LBC_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE2

/* LBC CS3 - nvram, cadmus, ATM phy */

#define INCLUDE_LBC_CS3

#ifdef INCLUDE_LBC_CS3
#define INCLUDE_NV_RAM
#define LBC_CS3_LOCAL_ADRS       0xf7000000
#define LBC_CS3_LOCAL_SIZE_MASK  0xff000000
#endif

/* NVRAM configuration */

#ifdef  INCLUDE_NV_RAM
#   define NV_RAM_ADRS          LBC_CS3_LOCAL_ADRS 
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE          (0x2000 - 0x10)  /* 8KB - 16 */
#   define NV_RAM_INTRVL        1
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET       0
#   define NV_MAC_ADRS_OFFSET   0x200
#endif  /* INCLUDE_NV_RAM */

/* Memory addresses */

#define INCLUDE_DDR_SDRAM

/*
 * The cds85xx board has only one DIMM slot,
 * but can support either of two DIMM widths:
 * + An 8-byte wide DIMM provides one physical bank and requires one
 *   chip select.  For this case, set DDR_SDRAM_PHYS_BANK_BITS to 0.
 * + A 16-byte wide DIMM provides two physical banks and requires
 *   two chip selects.  The two banks will have the same dimensions.
 *   The most-significant address line among those assigned to SDRAM
 *   will be used to select between the two banks.  For this case, set
 *   DDR_SDRAM_PHYS_BANK_BITS to 1.
 *
 * The following four settings must match the installed SDRAM DIMM.
 * See cds85xx.h, and Tables 9-27 & 9-28 in the MPC555E Reference
 * Manual, for possible values of ROW_ADRS_BITS and COL_ADRS_BITS.
 *
 * These values can be set via the project configuration mechanism, so
 * there is no need to edit them here if using only project builds.
 */

#define	DDR_SDRAM_ROW_ADRS_BITS		CS_ROW_BITS_13
#define	DDR_SDRAM_COL_ADRS_BITS		CS_COL_BITS_10
#define	DDR_SDRAM_PHYS_BANK_BITS	0
#define	DDR_SDRAM_CASLAT		DDR_SDRAM_CASLAT_2_PT_5

/*
 * The total SDRAM size, and the corresponding address window size setting
 * DDR_SDRAM_LAWAR_SIZE, are calculated from the above DIMM dimensions:
 *
 * b = 3  The three least-significant address bits identify a byte position
 *        within the 8-byte (64-bit) data bus.
 * c      The next COL bits are sent to the DIMM as the column address (MCAS*).
 * lb = 2 The next 2 bits are sent to the DIMM as the logical bank select (MBA).
 * r      The next ROW bits are sent to the DIMM as the row address (MRAS*).
 * pb     In the 16-byte case, the next bit selects between the DIMM's two
 *        physical banks (i.e. between CS0 and CS1).
 *
 * Thus the DIMM accounts for (b + c + lb + r + pb) address lines.
 *
 * For example, the default case has 13 rows (r = 13), 10 columns (c = 10),
 * and is 8 bytes wide (pb = 0).  The DIMM accounts for 28 address lines:
 * b  =  3  (bits 29-31)
 * c  = 10  (bits 19-28)
 * lb =  2  (bits 17-18)
 * r  = 13  (bits  4-16)
 * pb =  0  [no bits used for physical bank selection]
 *      --
 *      28
 * The size of the DIMM is 2^28 bytes (256MB).
 */

#define	DDR_SDRAM_LAWAR_SIZE						\
     LAWAR_SIZE_SETTING(  3			/* byte on bus */	\
			+ CS_COL_BIT_COUNT(DDR_SDRAM_COL_ADRS_BITS)	\
			+ 2		/* logical bank select */	\
			+ CS_ROW_BIT_COUNT(DDR_SDRAM_ROW_ADRS_BITS)	\
			+ DDR_SDRAM_PHYS_BANK_BITS)


#define LOCAL_MEM_LOCAL_ADRS    0x00000000      /* Base of RAM */

/*
 * This definition of LOCAL_MEM_SIZE ensures that it will match
 * DDR_SDRAM_LAWAR_SIZE.
 *
 * The preprocessor must recognize LOCAL_MEM_SIZE as unsigned so that
 * conditionals like
 *
 *   #if (LOCAL_MEM_SIZE > MMUE500_MAX_STATIC_TLB_SIZE)
 *
 * -- used to size sysStaticTlbDesc[] in sysLib.c -- will produce
 * the correct results even when LOCAL_MEM_SIZE is 2GB (0x80000000).
 *
 * We can't use (1UL << _count_) because LOCAL_MEM_SIZE is also used
 * in assembly code and the assembler chokes on a U or UL suffix, so
 * we derive the value from 0x80000000 which is implicitly unsigned.
 * However we can't just use (0x80000000 >> 31) in place of 1UL because
 * the assembler would sign-extend the shift, producing 0xffffffff.
 */

                     /* |---------------matching parentheses----------------| */
                     /*  |-------------------|    |------------------------|  */
                     /*   |-----------|           |                        |  */
#ifdef _ASMLANGUAGE
#define LOCAL_MEM_SIZE	(((~0x80000000) >> 30) << (DDR_SDRAM_LAWAR_SIZE + 1))
#else
#define LOCAL_MEM_SIZE	(1UL << (DDR_SDRAM_LAWAR_SIZE + 1))
#endif

#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE       LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END   (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)
#define	DDR_SDRAM_LOCAL_MAX_VALID  (DDR_SDRAM_LOCAL_ADRS_END - 1)

/*
 * Definitions of DDR_SDRAM_BANK0_MAX -- and DDR_SDRAM_BANK0_END if
 * applicable -- logically belong here, but have to come after the
 * value of DDR_SDRAM_PHYS_BANK_BITS is finalized (which happens near
 * the end of config.h).  See comments there re pseudo-components.
 */

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

#define INCLUDE_CTORS_DTORS

/* Using software FP support. Athough task based 32 bit HW_FP is allowed 
 * no optimised libraries are available. The kernel doesn't execute any hw 
 * fp instructions */

#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP

/*
 * The constants ROM_XXX_ADRS, ROM_SIZE, and RAM_XXX_ADRS are defined
 * in both config.h and Makefile.  All definitions for these constants
 * must be identical.
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

/*
 * Map pseudo-components to parameter values --
 * see comments re SDRAM sizing in 20bsp.cdf.
 */

#undef	DDR_SDRAM_ROW_ADRS_BITS
#undef	DDR_SDRAM_COL_ADRS_BITS
#undef	DDR_SDRAM_PHYS_BANK_BITS
#undef	DDR_SDRAM_CASLAT

# ifdef	INCLUDE_CS_ROW_BITS_12
#define	DDR_SDRAM_ROW_ADRS_BITS		CS_ROW_BITS_12
# endif	/* INCLUDE_CS_ROW_BITS_12 */

# ifdef	INCLUDE_CS_ROW_BITS_13
#define	DDR_SDRAM_ROW_ADRS_BITS		CS_ROW_BITS_13
# endif	/* INCLUDE_CS_ROW_BITS_13 */

# ifdef	INCLUDE_CS_ROW_BITS_14
#define	DDR_SDRAM_ROW_ADRS_BITS		CS_ROW_BITS_14
# endif	/* INCLUDE_CS_ROW_BITS_14 */

# ifdef	INCLUDE_CS_COL_BITS_8
#define	DDR_SDRAM_COL_ADRS_BITS		CS_COL_BITS_8
# endif	/* INCLUDE_CS_COL_BITS_8 */

# ifdef	INCLUDE_CS_COL_BITS_9
#define	DDR_SDRAM_COL_ADRS_BITS		CS_COL_BITS_9
# endif	/* INCLUDE_CS_COL_BITS_9 */

# ifdef	INCLUDE_CS_COL_BITS_10
#define	DDR_SDRAM_COL_ADRS_BITS		CS_COL_BITS_10
# endif	/* INCLUDE_CS_COL_BITS_10 */

# ifdef	INCLUDE_CS_COL_BITS_11
#define	DDR_SDRAM_COL_ADRS_BITS		CS_COL_BITS_11
# endif	/* INCLUDE_CS_COL_BITS_11 */

# ifdef	INCLUDE_CS_PHYS_BANK_BITS_0
#define	DDR_SDRAM_PHYS_BANK_BITS	0
# endif	/* INCLUDE_CS_COL_BITS_0 */

# ifdef	INCLUDE_CS_PHYS_BANK_BITS_1
#define	DDR_SDRAM_PHYS_BANK_BITS	1
# endif	/* INCLUDE_CS_COL_BITS_1 */

# ifdef	INCLUDE_CASLAT_1_PT_0
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_1_PT_0
# endif	/* INCLUDE_CASLAT_1_PT_0 */

# ifdef	INCLUDE_CASLAT_1_PT_5
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_1_PT_5
# endif	/* INCLUDE_CASLAT_1_PT_5 */

# ifdef	INCLUDE_CASLAT_2_PT_0
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_2_PT_0
# endif	/* INCLUDE_CASLAT_2_PT_0 */

# ifdef	INCLUDE_CASLAT_2_PT_5
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_2_PT_5
# endif	/* INCLUDE_CASLAT_2_PT_5 */

# ifdef	INCLUDE_CASLAT_3_PT_0
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_3_PT_0
# endif	/* INCLUDE_CASLAT_3_PT_0 */

# ifdef	INCLUDE_CASLAT_3_PT_5
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_3_PT_5
# endif	/* INCLUDE_CASLAT_3_PT_5 */

# ifdef	INCLUDE_CASLAT_4_PT_0
#define	DDR_SDRAM_CASLAT	DDR_SDRAM_CASLAT_4_PT_0
# endif	/* INCLUDE_CASLAT_4_PT_0 */

#endif /* PRJ_BUILD */

/* Now that we know the setting of DDR_SDRAM_PHYS_BANK_BITS ... */

#if (DDR_SDRAM_PHYS_BANK_BITS == 0)
# define DDR_SDRAM_BANK0_MAX	DDR_SDRAM_LOCAL_MAX_VALID
#else	/* DDR_SDRAM_PHYS_BANK_BITS */
# define DDR_SDRAM_BANK0_END (DDR_SDRAM_LOCAL_ADRS + (DDR_SDRAM_LOCAL_SIZE>>1))
# define DDR_SDRAM_BANK0_MAX (DDR_SDRAM_BANK0_END - 1)
#endif	/* DDR_SDRAM_PHYS_BANK_BITS */
