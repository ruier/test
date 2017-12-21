/* hpcNet8641.h - Freescale HPC-NET 8641(D) board header */

/*
* Copyright (c) 2006-2008 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/

/*
modification history
--------------------
01o,23oct08,d_l  Add OpenPic timer vector support. (WIND00140273)
01n,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01m,14aug08,dtr  Add boot page translation support.
01l,03oct07,dtr  Update PIXIS settings.
01k,02jul07,pmr  move EPIC FRR and function to driver
01j,12apr07,pch  add PORBMSR, PORDEVSR, and HPCN_RTC_FREQ
01i,19jan07,pch  add EPIC_FRR_ definitions; clean up historical baggage
01h,07dec06,tor  Add EPIC vectors
01g,21nov06,pch  handle SMP warm reboot
01f,08sep06,dtr  Fix core ratio mask.
01e,07aug06,dtr  Renamed file.
01d,14jun06,to   define INCLUDE_MMU as needed.
01c,28apr06,dtr  Add PIXIS defines.
01b,19apr06,dtr  Add MAC handler support.
01a,26jan06,dtr  Created from cds8548/cds8548.h/01c.
*/

/*
This file contains I/O addresses and related constants for the
FreeScale HPCII 8641D board. 
*/

#ifndef	INChpcNet8641
#define	INChpcNet8641

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include <vsbConfig.h>

#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

# ifdef  _ASMLANGUAGE
#define CAST(x)
# else /* _ASMLANGUAGE */
IMPORT UINT32 sysMsscr0Get();
typedef volatile UCHAR VCHAR;   /* shorthand for volatile UCHAR */
typedef volatile INT32 VINT32; /* volatile unsigned word */
typedef volatile INT16 VINT16; /* volatile unsigned halfword */
typedef volatile INT8 VINT8;   /* volatile unsigned byte */
typedef volatile UINT32 VUINT32; /* volatile unsigned word */
typedef volatile UINT16 VUINT16; /* volatile unsigned halfword */
typedef volatile UINT8 VUINT8;   /* volatile unsigned byte */
#define CAST(x) (x)
# endif  /* _ASMLANGUAGE */

#endif /* M8260ABBREVIATIONS */

/*
 * Space for the SMP-reboot detection structure.  This should go in
 * memory with the same volatility as DRAM -- i.e. it will survive
 * a reset via the PIXIS but not a power-cycle.  It would be nice
 * for it to be accessible without setting up the DRAM controller,
 * but absent knowledge of any such space on the board we'll just
 * put it in DRAM (and therefore it can't be tested until after the
 * DRAM controller has been set up).
 */
#define	RBV_ADRS	(RAM_LOW_ADRS - 16)

/*
 * "magic numbers" used to validate the rebootVec.  The definitions
 * are arbitrary, but should not be "common" values like all zeros,
 * all ones, 0xeeeeeeee, 0x55aa55aa, 0xdeadbeef, etc.
 */
#define	_WRS_BOOT_MAGIC_1	0x31415927
#define	_WRS_BOOT_MAGIC_2	0x27182818
#define	_WRS_BOOT_MAGIC_3	0x05772157
/* should not be a possible address for sysInit */
#define MP_MAGIC_RBV       0xfeedfeed
/*
 * Structure written at RBV_ADRS by SMP sysToMonitor when rebooting
 * with multiple processors running, examined by romInit to detect
 * an SMP warm boot.  The assembly version is defined for all builds,
 * because a non-SMP bootrom should work with an SMP vxWorks.
 */
#ifdef	_ASMLANGUAGE
#define	OFF_RBV_STARTTYPE	0
#define	OFF_RBV_MAGIC1		4
#define	OFF_RBV_MAGIC2		8
#define	OFF_RBV_MAGIC3		12
#else	/* _ASMLANGUAGE */
# ifdef	_WRS_CONFIG_SMP
struct rebootVec
    {
    int startType;
    VUINT32 magic1;
    VUINT32 magic2;
    VUINT32 magic3;
    };
# endif	/* _WRS_CONFIG_SMP */
#endif	/* _ASMLANGUAGE */

/* Frequency applied to the MPC8641 "RTC" pin, per manual p. 55: 14.318 MHz */
#define	HPCN_RTC_FREQ	14318000

/* Base Address of Memory Mapped Registers */
#define	CCSBAR_RESET	0xFF700000
#define	CCSBAR		0xF8000000

/* boot page translation register */
#define M86XX_BPTR(base)     (CAST(VUINT32 *)((base) + 0x20))
#define BPTR_EN 0x80000000
#define BPTR_BOOT_PAGE_SHIFT 12
#define BPTR_BOOT_PAGE_MASK 0x00ffff00
#define CORE1T_OFFSET 0x10000000

/* Processor initialization register */

#define M8XXX_EPICPIR(base)		(CAST(VUINT32 *)((base) + 0x41090))

#define M8XXX_EPICPIR_SRST_CORE0	0x00000001
#define M8XXX_EPICPIR_SRST_CORE1	0x00000002

/* Processor reset register */

#define M8XXX_EPICPRR(base)		(CAST(VUINT32 *)((base) + 0x41098))

#define M8XXX_EPICPRR_HRST_CORE0	0x00000001
#define M8XXX_EPICPRR_HRST_CORE1	0x00000002

#define CPU1CHECK  ((sysMsscr0Get() & 0x20) >> 5) 

/* add PCI access macros */
#define PCI_MEMIO2LOCAL(x) \
	 (((UINT32)x  - PCI_MEMIO_ADRS) + CPU_PCI_MEMIO_ADRS) 
    
/* PCI IO memory adrs to CPU (60x bus) adrs */       
#define PCI_IO2LOCAL(x) \
     (((UINT32)x  - PCI_IO_ADRS) + CPU_PCI_IO_ADRS)
	     
#define PCI_MEM2LOCAL(x) \
	 (((UINT32)x  - PCI_MEM_ADRS) + CPU_PCI_MEM_ADRS)

/* 60x bus adrs to PCI (non-prefetchable) memory address */
	      
#define LOCAL2PCI_MEMIO(x) \
     ((int)(x) + PCI_MSTR_MEM_BUS)

#define ETHERNET_MAC_HANDLER

#define WR_ENET0  	    0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1  	    0xA0
#define WR_ENET2  	    0x1E

#define CUST_ENET3_0    0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1    0xA1
#define CUST_ENET3_2    0xA2
#define CUST_ENET3_3    0xA3
#define CUST_ENET4 	0xAA
#define CUST_ENET5      0xA0

#define MAX_MAC_ADRS 4
#define MAC_ADRS_LEN 6
#define NV_MAC_ADRS_OFFSET 0x200

/* OpenPic timer vector support */

#define OPENPIC_TIMERA0_INT_VEC 0x3c
#define OPENPIC_TIMERA1_INT_VEC 0x3d
#define OPENPIC_TIMERA2_INT_VEC 0x3e
#define OPENPIC_TIMERA3_INT_VEC 0x3f
#define OPENPIC_TIMERB0_INT_VEC 0x40
#define OPENPIC_TIMERB1_INT_VEC 0x41
#define OPENPIC_TIMERB2_INT_VEC 0x42
#define OPENPIC_TIMERB3_INT_VEC 0x43

/* PCI defines begin */

#define PCI_AUTO_CONFIG_ADRS  0x4c00

#define PPCACR_PRKM_MASK  0XF0
#define PCI_REQUEST_LEVEL 0x3

#define CLASS_OFFSET      0xB
#define CLASS_WIDTH       0x1
#define BRIDGE_CLASS_TYPE 0x6

#define PCICMD_ADRS       (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL        0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS      (PCI_CFG_BASE + 0x06)  /* PCI status reg */
      
#define NUM_PCIEX_SLOTS          0x1            

#define PCIEX_XINT1_LVL           0x0            /* PCIEX XINT1 routed to IRQ0 */
#define PCIEX_XINT2_LVL           0x1            /* PCIEX XINT2 routed to IRQ1 */
#define PCIEX_XINT3_LVL           0x2            /* PCIEX XINT3 routed to IRQ2 */
#define PCIEX_XINT4_LVL           0x3            /* PCIEX XINT3 routed to IRQ3 */

#define PCIEX2_XINT1_LVL           0x4            /* PCIEX XINT1 routed to IRQ0 */
#define PCIEX2_XINT2_LVL           0x5            /* PCIEX XINT2 routed to IRQ1 */
#define PCIEX2_XINT3_LVL           0x6            /* PCIEX XINT3 routed to IRQ2 */
#define PCIEX2_XINT4_LVL           0x7            /* PCIEX XINT3 routed to IRQ3 */
#define PCI_LAT_TIMER	0x40		/* latency timer value, 64 PCI clocks */

#define PCI_DEV_ID_85XX	0x000C1057	/* Id for Freescale 8555/8541 PCI 1*/

#define PCI_ARCADIA_BRIDGE_DEV_ID 0x051310E3   /* DEV ID for PCI bridge in Arcadia */

#define PCI1_DEV_ID      0x826010E3
#define PCI2_DEV_ID      0x826110E3
#define PCI3_DEV_ID      0x826210E3
#define PCI_DEV_ID_82XX  0x00031057  /* Id for MPC8266ADS-PCI board - Rev1 */

#define PCI_ID_I82559    0x12298086  /* Id for Intel 82559 */
#define PCI_ID_I82559ER  0x12098086  /* Id for Intel 82559 ER */




#define DELTA(a,b)                 (abs((int)a - (int)b))

#define N_SIO_CHANNELS	 	0		/* No. serial I/O channels */

#define M8XXX_I2C1_BASE 0x3000
#define M8XXX_I2C2_BASE 0x3100

/* Local Access Windows Regster Offsets from CCSBAR */
/* LAWBARx 
 * 0-11 Reserved - read 0
 * 12-31 Base address - Most significant 20 bits 
 *
 * LAWARx
 * 0   Enable window
 * 1-7 Reserved
 * 8-11 Target interface - 0000 PCI/PCI-X
 *                       - 0001 -> 0011 Reserved
 *                       - 0100 Local Bus memory controller eg SDRAM/L2SRAM
 *                       - 0101 -> 1011 Reserved
 *                       - 1100 Rapid IO
 *                       - 1101 -> 1110 Reserved
 *                       - 1111 DDR SDRAM
 * 12-25 Reserved - read 0
 * 26-31 Size of Window  - min 001011 -> 4KBytes
 *                       step power of 2
 *                       - max 011110 -> 2 Gbytes
 */

/* Used for DDR SDRAM */

#define  M8XXX_LAWBAR0(base)        (CAST(VUINT32 *)((base) + 0xc08))
#define  M8XXX_LAWAR0(base)         (CAST(VUINT32 *)((base) + 0xc10))

/* Used for LBC NVRAM */

#define  M8XXX_LAWBAR1(base)        (CAST(VUINT32 *)((base) + 0xc28))
#define  M8XXX_LAWAR1(base)         (CAST(VUINT32 *)((base) + 0xc30))

/* Used for LBC SDRAM   */

#define  M8XXX_LAWBAR2(base)        (CAST(VUINT32 *)((base) + 0xc48))
#define  M8XXX_LAWAR2(base)         (CAST(VUINT32 *)((base) + 0xc50))

/* Used for FLASH */

#define  M8XXX_LAWBAR3(base)        (CAST(VUINT32 *)((base) + 0xc68))
#define  M8XXX_LAWAR3(base)         (CAST(VUINT32 *)((base) + 0xc70))

/* Used for PCI 1 */

#define  M8XXX_LAWBAR4(base)        (CAST(VUINT32 *)((base) + 0xc88))
#define  M8XXX_LAWAR4(base)         (CAST(VUINT32 *)((base) + 0xc90))

/* Used for PCI 2*/

#define  M8XXX_LAWBAR5(base)        (CAST(VUINT32 *)((base) + 0xcA8))
#define  M8XXX_LAWAR5(base)         (CAST(VUINT32 *)((base) + 0xcB0))

/* Used for PCI Express */

#define  M8XXX_LAWBAR6(base)        (CAST(VUINT32 *)((base) + 0xcc8))    
#define  M8XXX_LAWAR6(base)         (CAST(VUINT32 *)((base) + 0xcd0))

/* Used for Serial RIO */

#define  M8XXX_LAWBAR7(base)        (CAST(VUINT32 *)((base) + 0xce8))
#define  M8XXX_LAWAR7(base)         (CAST(VUINT32 *)((base) + 0xcf0))

/* Used for ?? */

#define  M8XXX_LAWBAR8(base)        (CAST(VUINT32 *)((base) + 0xd08))
#define  M8XXX_LAWAR8(base)         (CAST(VUINT32 *)((base) + 0xd10))

/* Used for ?? */

#define  M8XXX_LAWBAR9(base)        (CAST(VUINT32 *)((base) + 0xd28))
#define  M8XXX_LAWAR9(base)         (CAST(VUINT32 *)((base) + 0xd30))

#define  LAWBAR_ADRS_SHIFT       12
#define  LAWAR_ENABLE            0x80000000
#define  LAWAR_TGTIF_PCIEX1      0x00000000
#define  LAWAR_TGTIF_PCIEX2      0x00100000
#define  LAWAR_TGTIF_LBC         0x00400000
#define  LAWAR_TGTIF_INTER_DDR   0x00B00000
#define  LAWAR_TGTIF_RAPIDIO     0x00C00000
#define  LAWAR_TGTIF_DDR1        0x00F00000
#define  LAWAR_TGTIF_DDR2        0x01600000

/* LAWAR SIZE Settings */ 

#define  LAWAR_SIZE_4KB     0x0000000B
#define  LAWAR_SIZE_8KB     0x0000000C
#define  LAWAR_SIZE_16KB    0x0000000D
#define  LAWAR_SIZE_32KB    0x0000000E
#define  LAWAR_SIZE_64KB    0x0000000F
#define  LAWAR_SIZE_128KB   0x00000010
#define  LAWAR_SIZE_256KB   0x00000011
#define  LAWAR_SIZE_512KB   0x00000012
#define  LAWAR_SIZE_1MB     0x00000013
#define  LAWAR_SIZE_2MB     0x00000014
#define  LAWAR_SIZE_4MB     0x00000015
#define  LAWAR_SIZE_8MB     0x00000016
#define  LAWAR_SIZE_16MB    0x00000017
#define  LAWAR_SIZE_32MB    0x00000018
#define  LAWAR_SIZE_64MB    0x00000019
#define  LAWAR_SIZE_128MB   0x0000001A
#define  LAWAR_SIZE_256MB   0x0000001B
#define  LAWAR_SIZE_512MB   0x0000001C
#define  LAWAR_SIZE_1GB     0x0000001D
#define  LAWAR_SIZE_2GB     0x0000001E
#define  LAWAR_SIZE_4GB     0x0000001F
#define  LAWAR_SIZE_8GB     0x00000020
#define  LAWAR_SIZE_16GB     0x00000021
#define  LAWAR_SIZE_32GB     0x00000022


/* Local Bus Controller (LBC) Registers - unmentioned bits are reserved */
/* BRx 0-16 Base Address
 *     17-18 Extended Base Address 
 *     19-20 Port Size - 00 reserved
 *                     - 01 8bit
 *                     - 10 16bit
 *                     - 11 32bit
 *     21-22 Data Error Correction
 *                     - 00 reserved
 *                     - 01 Normal parity 
 *                     - 10 RMW parity generation (32-bit)
 *                     - 11 reserved
 *     23    Write Protect
 *     24-26 Machine Select = 000 GPCM
 *                          - 001->010 reserved
 *                          - 011 SDRAM
 *                          - 100->110 UPMA->UPMC
 *                          - 111 reserved
 *     28-29 Atomic Access  - 00 No atomic access
 *                          - 01 Read-after-write
 *                          - 10 Write-after-read
 *                          - 11 reserved
 *     31    Valid     
 *      
 * ORx for SDRAM
 *     0-16  Address mask 
 *     17-18 Extended address mask
 *     19-21 Column address lines - 000->111 7->14
 *     23-25 Number of row address lines - 000->110 9->15
 *                                       - 111 Reserved
 *     26    Page mode select
 *     31    External address latch delay 
 *
 * ORx for GPCM Mode
 *     0-16  Address mask 
 *     17-18 Extended address mask
 *     19    Buffer Control Disable
 *     20    Chip select negation
 *     21-22 Address to chip select setup
 *     23    Extra Address to chip select setup
 *     24-27 Cycle length in Bus clocks - 0000->1111 0->15 wait states
 *     28    External address termination
 *     29    Timing relaxed
 *     30    Extended hold time for read access
 *     31    External address latch delay  
 */


#define  M8XXX_BR0(base)         (CAST(VUINT32 *)((base) + 0x5000))
#define  M8XXX_OR0(base)         (CAST(VUINT32 *)((base) + 0x5004))
#define  M8XXX_BR1(base)         (CAST(VUINT32 *)((base) + 0x5008))
#define  M8XXX_OR1(base)         (CAST(VUINT32 *)((base) + 0x500c))
#define  M8XXX_BR2(base)         (CAST(VUINT32 *)((base) + 0x5010))
#define  M8XXX_OR2(base)         (CAST(VUINT32 *)((base) + 0x5014))
#define  M8XXX_BR3(base)         (CAST(VUINT32 *)((base) + 0x5018))
#define  M8XXX_OR3(base)         (CAST(VUINT32 *)((base) + 0x501c))
#define  M8XXX_BR4(base)         (CAST(VUINT32 *)((base) + 0x5020))
#define  M8XXX_OR4(base)         (CAST(VUINT32 *)((base) + 0x5024))
#define  M8XXX_BR5(base)         (CAST(VUINT32 *)((base) + 0x5028))
#define  M8XXX_OR5(base)         (CAST(VUINT32 *)((base) + 0x502C))
#define  M8XXX_BR6(base)         (CAST(VUINT32 *)((base) + 0x5030))
#define  M8XXX_OR6(base)         (CAST(VUINT32 *)((base) + 0x5034))
#define  M8XXX_BR7(base)         (CAST(VUINT32 *)((base) + 0x5038))
#define  M8XXX_OR7(base)         (CAST(VUINT32 *)((base) + 0x503C))

#define  M8XXX_MAR(base)         (CAST(VUINT32 *)((base) + 0x5068))
#define  M8XXX_MAMR(base)         (CAST(VUINT32 *)((base) + 0x5070))
#define  M8XXX_MBMR(base)         (CAST(VUINT32 *)((base) + 0x5074))
#define  M8XXX_MCMR(base)         (CAST(VUINT32 *)((base) + 0x5078))
#define  M8XXX_MRTPR(base)         (CAST(VUINT32 *)((base) + 0x5084))
#define  MRTPR_PTP_MASK 0xff000000
#define  MRTPR_PTP_WRITE(x)   ( (x << 24) & MRTPR_PTP_MASK)
#define  M8XXX_MDR(base)         (CAST(VUINT32 *)((base) + 0x5088))

#define  M8XXX_LSDMR(base)         (CAST(VUINT32 *)((base) + 0x5094))
#define  LSDMR_RFEN     0x40000000 /* Refresh Enable */

/* LSDMR OP - 000 Normal operation
 *          - 001 Auto Refresh  (Initialization)
 *          - 010 Self Refresh
 *          - 011 Mode Register Write (Initialization)
 *          - 100 Precharge Bank
 *          - 101 Precharge all banks (Initialization)
 *          - 110 Activate Bank
 *          - 111 Read/Write without valid transfer
 */      

#define  LSDMR_OP_MASK  0x38000000
#define  LSDMR_OP_SHIFT(x) ((x << 27) & LSDMR_OP_MASK)

/* Bank Select Multiplexed address line - 000 lines 12:13
 *                                      - 001       13:14
 *                                      - 010       14:15
 *                                      - 011       15:16
 *                                      - 100       16:17
 *                                      - 101       17:18
 *                                      - 110       18:19
 *                                      - 111       19:20
 */

#define  LSDMR_BSMA_MASK 0x00E00000
#define  LSDMR_BSMA_SHIFT(x) ((x << 23) & LSDMR_BSMA_MASK)

/* RFCR Refresh recovery 000 - reserved
 *                       001->110 - 3->8 clocks
 *                       111 - 16 clocks
 */

#define  LSDMR_RFCR_MASK 0x00038000
#define  LSDMR_RFCR_SHIFT(x) ((x << 15) & LSDMR_RFCR_MASK)

/* Incomplete LSDMR definitions */

#define  M8XXX_LURT(base)         (CAST(VUINT32 *)((base) + 0x50A0))
#define  M8XXX_LSRT(base)         (CAST(VUINT32 *)((base) + 0x50A4))
#define  M8XXX_LTESR(base)         (CAST(VUINT32 *)((base) + 0x50B0))
#define  M8XXX_LTEDR(base)         (CAST(VUINT32 *)((base) + 0x50B4))
#define  M8XXX_LTEIR(base)         (CAST(VUINT32 *)((base) + 0x50B8))
#define  M8XXX_LTEATR(base)         (CAST(VUINT32 *)((base) + 0x50BC))
#define  M8XXX_LTEAR(base)         (CAST(VUINT32 *)((base) + 0x50C0))

/* LBC Clock Configuration */
#define  M8XXX_LBCR(base)         (CAST(VUINT32 *)((base) + 0x50D0))
#define  M8XXX_LCRR(base)         (CAST(VUINT32 *)((base) + 0x50D4))

#define  M8XXX_DCR0(base)         (CAST(VUINT32 *)((base) + 0xe0f1c))
#define  M8XXX_DCR1(base)         (CAST(VUINT32 *)((base) + 0xe0f20))

/* MPX Registers */
#define MCM_OFFSET (0x1000)
#define MCM_BA (CCSBAR | MCM_OFFSET)

#define MCM_ABCR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0x0))
#define MCM_DBCR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0x8))
#define MCM_PCR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0x10))
#define MCM_REV1(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xBF8))
#define MCM_REV2(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xBFC))
#define MCM_EDR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xE00))
#define MCM_EER(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xE08))
#define MCM_EATR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xE0C))
#define MCM_ELADR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xE10))
#define MCM_EHADR(base)		(CAST(VUINT32 *)((base) + MCM_OFFSET + 0xE14))

#define MCM_PCR_CORE1_ENABLE 0x02000000
#define MCM_PCR_CORE0_ENABLE 0x01000000

/* Offsets for DDR registers */
#define DDR1_OFFSET 0x2000
#define DDR1BA      (CCSBAR | DDR1_OFFSET)

#define DDR2_OFFSET 0x6000
#define DDR2BA      (CCSBAR | DDR1_OFFSET)

#define CS0_BNDS                  0x000
#define CS1_BNDS                  0x008
#define CS2_BNDS                  0x010
#define CS3_BNDS                  0x018
#define CS0_CONFIG                0x080
#define CS1_CONFIG                0x084
#define CS2_CONFIG                0x088
#define CS3_CONFIG                0x08C
#define TIMING_CFG_3              0x100
#define TIMING_CFG_0              0x104
#define TIMING_CFG_1              0x108
#define TIMING_CFG_2              0x10C
#define DDR_SDRAM_CFG             0x110
#define DDR_SDRAM_CFG_2           0x114
#define DDR_SDRAM_MODE_CFG        0x118
#define DDR_SDRAM_MODE_CFG_2      0x11c
#define DDR_SDRAM_MD_CNTL         0x120
#define DDR_SDRAM_INTERVAL        0x124
#define DDR_DATA_INIT             0x128
#define DDR_SDRAM_CLK_CTRL        0x130

#define DDR_INIT_ADDRESS          0x148
#define DDR_INIT_EXT_ADDRESS      0x14C

#define DDR_DSR1          0xB20
#define DDR_DSR2          0xB24
#define DDR_CDR1          0xB28
#define DDR_CDR2          0xB2C

#define DDR_IP_REV1          0xBF8
#define DDR_IP_REV2          0xBFC

#define	DDR_DATA_ERR_INJECT_HI	0xe00
#define	DDR_DATA_ERR_INJECT_LO	0xe04
#define	DDR_ECC_ERR_INJECT	0xe08
#define	DDR_CAPTURE_DATA_HI	0xe20
#define	DDR_CAPTURE_DATA_LO	0xe24
#define	DDR_CAPTURE_ECC		0xe28
#define	DDR_ERR_DETECT		0xe40
#define	DDR_ERR_DISABLE		0xe44
#define	DDR_ERR_INT_EN		0xe48
#define	DDR_CAPTURE_ATTRIBUTES	0xe4c
#define	DDR_CAPTURE_ADDRESS	0xe50
#define	DDR_CAPTURE_EXT_ADDRESS	0xe54
#define	DDR_ERR_SBE		0xe58

#define	DDR_IO_OVCR	0x000E0F24

/* PIC Base Address */

#define	PIC_OFFSET	0x40000
#define	PCIBA		(CCSBAR | PIC_OFFSET)

/* Global Function Registers */
/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */
/* Plat Ratio not working on board need to test!!!!*/
 
#define	M8XXX_PORPLLSR(base)		(CAST(VUINT32 *)((base) + 0xE0000))
#define	M8XXX_PORPLLSR_E600_RATIO_MASK		0x003f0000
#define	M8XXX_PORPLLSR_PSR_CORE_RATIO_MASK	0x003e0000
#define	M8XXX_PORPLLSR_PLAT_RATIO_MASK		0x0000003e
#define	M8XXX_PORPLLSR_E600_RATIO(base) \
	((*M8XXX_PORPLLSR(base) & M8XXX_PORPLLSR_E600_RATIO_MASK)>>16)
#define	M8XXX_PORPLLSR_PSR_CORE_RATIO(base) \
	((*M8XXX_PORPLLSR(base) & M8XXX_PORPLLSR_PSR_CORE_RATIO_MASK)>>18)
#define	M8XXX_PORPLLSR_PLAT_RATIO(base) \
	((*M8XXX_PORPLLSR(base) & M8XXX_PORPLLSR_PLAT_RATIO_MASK)>>1)

/* PORBMSR reports boot configuration */

#define	M8XXX_PORBMSR(base)		(CAST(VUINT32 *)((base) + 0xE0004))
#define	M8XXX_PORBMSR_BCFG		0x80000000  /* core 0 boot config */
#define	M8XXX_PORBMSR_ROM_LOC		0x0f000000  /* boot ROM location */
#define	M8XXX_PORBMSR_BSCFG		0x00300000  /* boot sequencer config */
#define	M8XXX_PORBMSR_HA		0x00060000  /* Host-Agent */
#define	M8XXX_PORBMSR_ABV		0x00000001  /* Alternate Boot Vector */

/* PORDEVSR reports miscellaneous device configuration */

#define	M8XXX_PORDEVSR(base)		(CAST(VUINT32 *)((base) + 0xE000C))
#define	M8XXX_PORDEVSR_ECW1		0x80000000  /* eTSEC1 width */
#define	M8XXX_PORDEVSR_ECW2		0x40000000  /* eTSEC2 width */
#define	M8XXX_PORDEVSR_ECW3		0x20000000  /* eTSEC3 width */
#define	M8XXX_PORDEVSR_ECW4		0x10000000  /* eTSEC4 width */
#define	M8XXX_PORDEVSR_ECP1		0x0c000000  /* eTSEC1 protocol */
#define	M8XXX_PORDEVSR_ECP2		0x03000000  /* eTSEC2 protocol */
#define	M8XXX_PORDEVSR_ECP3		0x00c00000  /* eTSEC3 protocol */
#define	M8XXX_PORDEVSR_ECP4		0x00300000  /* eTSEC4 protocol */
#define	M8XXX_PORDEVSR_HS_CFG		0x000f0000  /* SERDES port selection */
#define	M8XXX_PORDEVSR_SRIOTS		0x00008000  /* SRIO transport size */
#define	M8XXX_PORDEVSR_PLAT_FREQ	0x00000400  /* platform frequency */
#define	M8XXX_PORDEVSR_RTYPE		0x00000300  /* DRAM type */
#define	M8XXX_PORDEVSR_CORE1TE		0x00000080  /* core 1 offset mode */
#define	M8XXX_PORDEVSR_DEV_ID		0x00000007  /* SRIO device ID */


#define	M8XXX_DDRDLLCR(base)		(CAST(VUINT32 *)((base) + 0xE0E10))
#define	M8XXX_LBCDLLSR(base)		(CAST(VUINT32 *)((base) + 0xE0E20))

#define	M8XXX_DEVDISR(base)		(CAST(VUINT32 *)((base) + 0xE0070))
#define	M8XXX_DEVDISR_DDR			0x00010000
#define	M8641_DEVDISR_TB			0x00002000
#define	M8XXX_PVR(base)			(CAST(VUINT32 *)((base) + 0xE00A0))
#define	M8XXX_SVR(base)			(CAST(VUINT32 *)((base) + 0xE00A4))

#define	_PPC_BUCSR_FI	0x200		/* Invalidate branch cache */
#define	_PPC_BUCSR_E	0x1		/* Enable branch prediction */

#if CPU==PPC603
#   error This BSP does not support PPC603
#else
#   define WRONG_CPU_MSG "PVR shows unsupported CPU type!\n";
#endif

#define CPU_TYPE        ((vxPvrGet() >> 16) & 0xffff)
#define CPU_TYPE_8641   0x8004

/* PIXIS board control registers */

#define PIXIS_BASE 0xf8100000
#define PIXIS_ID(base)   (CAST(VUINT8*)(base + 0x0))
#define PIXIS_VER(base)  (CAST(VUINT8*)(base + 0x1))
#define PIXIS_PVER(base) (CAST(VUINT8*)(base + 0x2))
#define PIXIS_CSR(base)  (CAST(VUINT8*)(base + 0x3))
#define PIXIS_RST(base)  (CAST(VUINT8*)(base + 0x4))
#define PIXIS_PWR(base)  (CAST(VUINT8*)(base + 0x5))
#define PIXIS_OSC(base)  (CAST(VUINT8*)(base + 0x7))

#define PIXIS_OSC_40_MHZ 0x1 /* 40 Mhz */
#define PIXIS_OSC_50_MHZ 0x2 /* 50 Mhz */
#define PIXIS_VCTL(base)  (CAST(VUINT8*)(base + 0x10))
#define PIXIS_VCTL_GO     0x1

#define PIXIS_VSPEED0(base) (CAST(VUINT8*)(base + 0x17))
#define PIXIS_VSPEED1(base) (CAST(VUINT8*)(base + 0x18))

/* C PPC syncronization macro */

#define PPC_EIEIO_SYNC	WRS_ASM (" eieio; sync")	

/* create a single macro INCLUDE_MMU */

#if defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_GLOBAL_MAP)
#define INCLUDE_MMU
#endif

/*
 * sysPhysMemDesc[] dummy entries:
 * these create space for updating sysPhysMemDesc table at a later stage
 * mainly to provide plug and play
 */

#define DUMMY_PHYS_ADDR         -1
#define DUMMY_VIRT_ADDR         -1
#define DUMMY_LENGTH            -1
#define DUMMY_INIT_STATE_MASK   -1
#define DUMMY_INIT_STATE        -1

#define DUMMY_MMU_ENTRY		{ 				\
				    (VIRT_ADDR) DUMMY_VIRT_ADDR,	\
				    (PHYS_ADDR) DUMMY_PHYS_ADDR,	\
				    (UINT32)DUMMY_LENGTH,		\
				    (UINT32)DUMMY_INIT_STATE_MASK,	\
				    (UINT32)DUMMY_INIT_STATE		\
				}

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INChpcNet8641 */
