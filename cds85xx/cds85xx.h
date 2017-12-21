/* cds85xx.h - Freescale MPC85xx CDS board header */

/*
 * Copyright 2002-2006, 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02a,11jan10,pch  Add symbolic definitions for TIMING_CFG_1
01z,08dec09,pch  CQ195058: Add symbolic definitions of some DDR
		 and chip-select fields
01y,25sep08,pmr  add more FIT divider defines
01x,07dec06,y_w  WIND00061260: add timer clock divider ratios define 
01w,22mar05,dtr  Fix typos.
01v,04nov04,jln  add PCI 2 support
01u,10sep04,jln  Mod for cds85xx
01t,28jul04,dtr  Mod for PCI DEVNO with RevA board.
01s,31mar04,dtr  Add a new PCI ID for rev2 silicon.
01r,12feb04,dtr  Add in extra DDR register defines.
01q,04feb04,dtr  Adding in DDRDLL reg define.
01p,08oct03,dtr  Adding in DLL debug registers.
01o,08sep03,dtr  Fixing windows compilation.
01n,22aug03,dtr  Adding General registers.
01m,04aug03,dtr  Changing allocation of LAWBAR/LAWAR registers to accomodate
                 LBC SDRAM.
01l,07jul03,mil  Added offsets for SCC regs.
01k,08jul02,dtr  Adding some PCI defines and macros.
*/

/*
This file contains I/O addresses and related constants for the
FreeScale MPC85xx CDS board. 
*/

#ifndef	INCcds85xxh
#define	INCcds85xxh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR VCHAR;   /* shorthand for volatile UCHAR */
typedef volatile INT32 VINT32; /* volatile unsigned word */
typedef volatile INT16 VINT16; /* volatile unsigned halfword */
typedef volatile INT8 VINT8;   /* volatile unsigned byte */
typedef volatile UINT32 VUINT32; /* volatile unsigned word */
typedef volatile UINT16 VUINT16; /* volatile unsigned halfword */
typedef volatile UINT8 VUINT8;   /* volatile unsigned byte */
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#endif /* M8260ABBREVIATIONS */

/* Base Address of Memory Mapped Registers */
#define CCSBAR  0xFE000000


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

/* PCI defines begin */

#define PCI_CFG_ADR_REG  (CCSBAR + 0x8000)
#define PCI_CFG_DATA_REG (CCSBAR + 0x8004)
#define PCI_AUTO_CONFIG_ADRS  0x4c00

#define PPCACR_PRKM_MASK  0XF0
#define PCI_REQUEST_LEVEL 0x3

#define CLASS_OFFSET      0xB
#define CLASS_WIDTH       0x1
#define BRIDGE_CLASS_TYPE 0x6

#define PCICMD_ADRS       (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL        0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS      (PCI_CFG_BASE + 0x06)  /* PCI status reg */

#define NUM_PCI1_SLOTS          0x4            
#define NUM_PCI2_SLOTS          0x1            

#define PCI_XINT1_LVL           0x0            /* PCI XINT1 routed to IRQ0  */
#define PCI_XINT2_LVL           0x1            /* PCI XINT2 routed to IRQ1 */
#define PCI_XINT3_LVL           0x2            /* PCI XINT3 routed to IRQ2 */
#define PCI_XINT4_LVL           0x3            /* PCI XINT3 routed to IRQ2 */

#define PCI2_XINT1_LVL          0xb            /* PCI2 XINT1 routed to IRQ11  */

#define PCI_SLOT1_DEVNO        0x12            /* PCI SLOT 1 Device no */

#define PCI_LAT_TIMER          0x40            /* latency timer value, 64 PCI clocks */

#define PCI_DEV_ID_85XX  0x000C1057            /* Id for Freescale 8555/8541 PCI 1*/

#define PCI_ARCADIA_BRIDGE_DEV_ID 0x051310E3   /* DEV ID for PCI bridge in Arcadia */

#define PCI1_DEV_ID      0x826010E3
#define PCI2_DEV_ID      0x826110E3
#define PCI3_DEV_ID      0x826210E3
#define PCI_DEV_ID_82XX  0x00031057  /* Id for MPC8266ADS-PCI board - Rev1 */

#define PCI_ID_I82559    0x12298086  /* Id for Intel 82559 */
#define PCI_ID_I82559ER  0x12098086  /* Id for Intel 82559 ER */

#define MPC8266ADS_PCI_IRQ       08

#define PCI_INTA_IRQ     MPC8266ADS_PCI_IRQ
#define PCI_INTB_IRQ     MPC8266ADS_PCI_IRQ
#define PCI_INTC_IRQ     MPC8266ADS_PCI_IRQ
#define PCI_INTD_IRQ     MPC8266ADS_PCI_IRQ

#define DELTA(a,b)                 (abs((int)a - (int)b))

#define BUS	0				/* bus-less board */


#define N_SIO_CHANNELS	 	2		/* No. serial I/O channels */
#define DEC_CLOCK_FREQ	OSCILLATOR_FREQ

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

#define  M85XX_LAWBAR0(base)        (CAST(VUINT32 *)((base) + 0xc08))
#define  M85XX_LAWAR0(base)         (CAST(VUINT32 *)((base) + 0xc10))

/* Used for LBC NVRAM */

#define  M85XX_LAWBAR1(base)        (CAST(VUINT32 *)((base) + 0xc28))
#define  M85XX_LAWAR1(base)         (CAST(VUINT32 *)((base) + 0xc30))

/* Used for LBC SDRAM   */

#define  M85XX_LAWBAR2(base)        (CAST(VUINT32 *)((base) + 0xc48))
#define  M85XX_LAWAR2(base)         (CAST(VUINT32 *)((base) + 0xc50))

/* Used for FLASH */

#define  M85XX_LAWBAR3(base)        (CAST(VUINT32 *)((base) + 0xc68))
#define  M85XX_LAWAR3(base)         (CAST(VUINT32 *)((base) + 0xc70))

/* Used for PCI 1 */

#define  M85XX_LAWBAR4(base)        (CAST(VUINT32 *)((base) + 0xc88))
#define  M85XX_LAWAR4(base)         (CAST(VUINT32 *)((base) + 0xc90))

/* Used for PCI 2*/

#define  M85XX_LAWBAR5(base)        (CAST(VUINT32 *)((base) + 0xcA8))
#define  M85XX_LAWAR5(base)         (CAST(VUINT32 *)((base) + 0xcB0))

/* Not Used */

#define  M85XX_LAWBAR6(base)        (CAST(VUINT32 *)((base) + 0xcc8))    
#define  M85XX_LAWAR6(base)         (CAST(VUINT32 *)((base) + 0xcd0))

/* Not Used */

#define  M85XX_LAWBAR7(base)        (CAST(VUINT32 *)((base) + 0xce8))
#define  M85XX_LAWAR7(base)         (CAST(VUINT32 *)((base) + 0xcf0))

#define  LAWBAR_ADRS_SHIFT     12
#define  LAWAR_ENABLE          0x80000000
#define  LAWAR_TGTIF_PCI       0x00000000
#define  LAWAR_TGTIF_PCI2      0x00100000
#define  LAWAR_TGTIF_LBC       0x00400000
#define  LAWAR_TGTIF_RAPIDIO   0x00c00000
#define  LAWAR_TGTIF_DDRSDRAM  0x00F00000

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

/* Convert number of bits to a LAWAR_SIZE_xxx setting */

#define	 LAWAR_SIZE_SETTING(bits)	((bits) - 1)

/* Local Bus Controller (LBC) Registers */
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


#define  M85XX_BR0(base)         (CAST(VUINT32 *)((base) + 0x5000))
#define  M85XX_OR0(base)         (CAST(VUINT32 *)((base) + 0x5004))
#define  M85XX_BR1(base)         (CAST(VUINT32 *)((base) + 0x5008))
#define  M85XX_OR1(base)         (CAST(VUINT32 *)((base) + 0x500c))
#define  M85XX_BR2(base)         (CAST(VUINT32 *)((base) + 0x5010))
#define  M85XX_OR2(base)         (CAST(VUINT32 *)((base) + 0x5014))
#define  M85XX_BR3(base)         (CAST(VUINT32 *)((base) + 0x5018))
#define  M85XX_OR3(base)         (CAST(VUINT32 *)((base) + 0x501c))
#define  M85XX_BR4(base)         (CAST(VUINT32 *)((base) + 0x5020))
#define  M85XX_OR4(base)         (CAST(VUINT32 *)((base) + 0x5024))
#define  M85XX_BR5(base)         (CAST(VUINT32 *)((base) + 0x5028))
#define  M85XX_OR5(base)         (CAST(VUINT32 *)((base) + 0x502C))
#define  M85XX_BR6(base)         (CAST(VUINT32 *)((base) + 0x5030))
#define  M85XX_OR6(base)         (CAST(VUINT32 *)((base) + 0x5034))
#define  M85XX_BR7(base)         (CAST(VUINT32 *)((base) + 0x5038))
#define  M85XX_OR7(base)         (CAST(VUINT32 *)((base) + 0x503C))

#define  M85XX_MAR(base)         (CAST(VUINT32 *)((base) + 0x5068))
#define  M85XX_MAMR(base)         (CAST(VUINT32 *)((base) + 0x5070))
#define  M85XX_MBMR(base)         (CAST(VUINT32 *)((base) + 0x5074))
#define  M85XX_MCMR(base)         (CAST(VUINT32 *)((base) + 0x5078))
#define  M85XX_MRTPR(base)         (CAST(VUINT32 *)((base) + 0x5084))
#define  MRTPR_PTP_MASK 0xff000000
#define  MRTPR_PTP_WRITE(x)   ( (x << 24) & MRTPR_PTP_MASK)
#define  M85XX_MDR(base)         (CAST(VUINT32 *)((base) + 0x5088))

#define  M85XX_LSDMR(base)         (CAST(VUINT32 *)((base) + 0x5094))
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

#define  M85XX_LURT(base)         (CAST(VUINT32 *)((base) + 0x50A0))
#define  M85XX_LSRT(base)         (CAST(VUINT32 *)((base) + 0x50A4))
#define  M85XX_LTESR(base)         (CAST(VUINT32 *)((base) + 0x50B0))
#define  M85XX_LTEDR(base)         (CAST(VUINT32 *)((base) + 0x50B4))
#define  M85XX_LTEIR(base)         (CAST(VUINT32 *)((base) + 0x50B8))
#define  M85XX_LTEATR(base)         (CAST(VUINT32 *)((base) + 0x50BC))
#define  M85XX_LTEAR(base)         (CAST(VUINT32 *)((base) + 0x50C0))

/* LBC Clock Configuration */
#define  M85XX_LBCR(base)         (CAST(VUINT32 *)((base) + 0x50D0))
#define  M85XX_LCRR(base)         (CAST(VUINT32 *)((base) + 0x50D4))

/*
 * Timer clock divider ratios (in 2^n)
 */

#define FIT_DIVIDER_TAP_29 29
#define FIT_DIVIDER_TAP_28 28
#define FIT_DIVIDER_TAP_27 27
#define FIT_DIVIDER_TAP_26 26
#define FIT_DIVIDER_TAP_25 25
#define FIT_DIVIDER_TAP_24 24
#define FIT_DIVIDER_TAP_23 23
#define FIT_DIVIDER_TAP_22 22
#define FIT_DIVIDER_TAP_21 21
#define FIT_DIVIDER_TAP_20 20
#define FIT_DIVIDER_TAP_19 19
#define FIT_DIVIDER_TAP_18 18
#define FIT_DIVIDER_TAP_17 17
#define FIT_DIVIDER_TAP_16 16
#define FIT_DIVIDER_TAP_15 15
#define FIT_DIVIDER_TAP_14 14
#define FIT_DIVIDER_TAP_13 13
#define FIT_DIVIDER_TAP_12 12
#define FIT_DIVIDER_TAP_11 11
#define FIT_DIVIDER_TAP_10 10

/* ECM Registers */
#define ECM_OFFSET 0x1000
#define ECMBA (CCSBAR | ECM_OFFSET)

/*
 * Largest possible TLB entry.  A case can be made for moving this
 * -- and several other definitions from this file -- to the arch,
 * but it's being added during asynchronous BSP cleanup and we'd 
 * prefer not to require an arch patch.
 */

#define	MMUE500_MAX_STATIC_TLB_SIZE	0x10000000

/*
 * Offsets & fields for DDR registers
 * from Sec 9.4 pp. 378-394 of MPC8555ERM Rev. 2 10/2006
 */
#define	DDR_OFFSET 0x2000
#define	DDRBA      (CCSBAR | DDR_OFFSET)

#define	CS0_BNDS   0x000
#define	CS1_BNDS   0x008
#define	CS2_BNDS   0x010
#define	CS3_BNDS   0x018
#define	ADRS_TO_CS_BNDS_LOW_SHIFT	8
#define	CS_BNDS_LOW_MASK		0x00FF0000
#define	ADRS_TO_CS_BNDS_HIGH_SHIFT	24
#define	CS_BNDS_HIGH_MASK		0x000000FF

#define	CS0_CONFIG 0x080
#define	CS1_CONFIG 0x084
#define	CS2_CONFIG 0x088
#define	CS3_CONFIG 0x08C
#define	CS_ENABLE	0x80000000
#define	CS_AP_ENABLE	0x00800000

#define	CS_ROW_BITS_12	0x00000000
#define	CS_ROW_BITS_13	0x00000100
#define	CS_ROW_BITS_14	0x00000200
#define	CS_ROW_BIT_COUNT(fieldVal)	(((fieldVal) >> 8) + 12)

#define	CS_COL_BITS_8	0x00000000
#define	CS_COL_BITS_9	0x00000001
#define	CS_COL_BITS_10	0x00000002
#define	CS_COL_BITS_11	0x00000003
#define	CS_COL_BIT_COUNT(fieldVal)	((fieldVal) + 8)

#define	TIMING_CFG_1 0x108
#define	DDR_SDRAM_TIMING_CFG_1_PRETOACT_SHIFT	28
#define	DDR_SDRAM_TIMING_CFG_1_ACTTOPRE_SHIFT	24
#define	DDR_SDRAM_TIMING_CFG_1_ACTTORW_SHIFT	20

#define	DDR_SDRAM_TIMING_CFG_1_CASLAT_SHIFT	16
#define		DDR_SDRAM_CASLAT_1_PT_0	    1
#define		DDR_SDRAM_CASLAT_1_PT_5	    2
#define		DDR_SDRAM_CASLAT_2_PT_0	    3
#define		DDR_SDRAM_CASLAT_2_PT_5	    4
#define		DDR_SDRAM_CASLAT_3_PT_0	    5
#define		DDR_SDRAM_CASLAT_3_PT_5	    6
#define		DDR_SDRAM_CASLAT_4_PT_0	    7

#define	DDR_SDRAM_TIMING_CFG_1_REFREC_SHIFT	12
#define	DDR_SDRAM_TIMING_CFG_1_WRREC_SHIFT	8
#define	DDR_SDRAM_TIMING_CFG_1_ACTTOACT_SHIFT	4
#define	DDR_SDRAM_TIMING_CFG_1_WRTORD_SHIFT	0

#define	TIMING_CFG_2 0x10C

#define	DDR_SDRAM_CFG 0x110
#define	DDR_SDRAM_CFG_MEM_ENABLE	0x80000000  /* memory enable */
#define	DDR_SDRAM_CFG_SREN		0x40000000  /* self-refresh enable */
#define	DDR_SDRAM_CFG_ECC_ENABLE	0x20000000  /* ECC enable */
#define	DDR_SDRAM_RD_ENABLE		0x10000000  /* registered DIMM */
#define	DDR_SDRAM_TYPE_DDR		0x02000000  /* always set this */
#define	DDR_SDRAM_DYN_PWR		0x00200000  /* dynamic pwr mgt */
#define	DDR_SDRAM_NCAP			0x00020000  /* non-concur auto prechg */
#define	DDR_SDRAM_2T_ENABLE		0x00008000  /* 2T timing enable */

#define	DDR_SDRAM_MODE_CFG 0x118
#define	DDR_SDRAM_INTERVAL 0x124

#define	DDR_SDRAM_CLK_CNTL 0x130
#define	DDR_SDRAM_CLK_CNTL_SS_ENABLE	0x80000000	/* source synch enab */
#define	DDR_SDRAM_CLK_CNTL_CLK_ADJST_0	0x00000000	/* no delay */
#define	DDR_SDRAM_CLK_CNTL_CLK_ADJST_1	0x01000000	/* 1/4 cycle delay */
#define	DDR_SDRAM_CLK_CNTL_CLK_ADJST_2	0x02000000	/* 1/2 cycle delay */
#define	DDR_SDRAM_CLK_CNTL_CLK_ADJST_3	0x03000000	/* 3/4 cycle delay */
#define	DDR_SDRAM_CLK_CNTL_CLK_ADJST_4	0x04000000	/* 1 cycle delay */

#define DDR_DATA_ERR_INJECT_HI 0xe00
#define DDR_DATA_ERR_INJECT_LO 0xe04
#define DDR_ECC_ERR_INJECT 0xe08
#define DDR_CAPTURE_DATA_HI 0xe20
#define DDR_CAPTURE_DATA_LO 0xe24
#define DDR_CAPTURE_ECC 0xe28

/*
 * With the exception of MME, the bit values
 * defined here apply to all three registers.
 */
#define	DDR_ERR_DETECT 0xe40		/* memory error types detected */
#define	DDR_ERR_DETECT_MME	0x80000000	/* multiple mem error types */
#define	DDR_ERR_DISABLE 0xe44		/* memory error types disabled */
#define	DDR_ERR_INT_EN 0xe48		/* memory error interrupts enabled */
#define	DDR_ERR_TYPE_MBE	0x00000008	/* multi-bit error */
#define	DDR_ERR_TYPE_SBE	0x00000004	/* single-bit error */
#define	DDR_ERR_TYPE_MSE	0x00000001	/* memory select error */

#define	DDR_CAPTURE_ATTRIBUTES 0xe4c
#define	DDR_CAPTURE_ADDRESS 0xe50

#define	DDR_ERR_SBE 0xe58
#define	DDR_ERR_SBE_SBET_SHIFT	16		/* SBE threshold */
#define	DDR_ERR_SBE_SBEC_MASK	0x000000ff	/* SBE count */

/* PIC Base Address */

#define PIC_OFFSET 0x40000
#define PCIBA      (CCSBAR | PIC_OFFSET)

/* 
 * Maximum number of SCC channels to configure as SIOs. Note that this 
 * assumes sequential usage of SCCs.
*/

#define MAX_SCC_SIO_CHANS 2
#define M85XX_CPM_SICR(base)         (CAST(VUINT16 *)((base) + 0x90c00))
#define M85XX_CPM_SIVEC(base)        (CAST(VUINT8  *)((base) + 0x90c04))
#define M85XX_CPM_SIPNR_H(base)      (CAST(VUINT32 *)((base) + 0x90c08))
#define M85XX_CPM_SIPNR_L(base)      (CAST(VUINT32 *)((base) + 0x90c0c))
#define M85XX_CPM_SCPRR_H(base)      (CAST(VUINT32 *)((base) + 0x90c14))
#define M85XX_CPM_SCPRR_L(base)      (CAST(VUINT32 *)((base) + 0x90c18))
#define M85XX_CPM_SIMR_H(base)       (CAST(VUINT32 *)((base) + 0x90c1c))
#define M85XX_CPM_SIMR_L(base)       (CAST(VUINT32 *)((base) + 0x90c20))
#define M85XX_CPM_SIEXR(base)        (CAST(VUINT32 *)((base) + 0x90c24))
#define M85XX_CPM_SCCR(base)         (CAST(VUINT32 *)((base) + 0x90c80))

/* Global Function Registers */
/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */
/* Plat Ratio not working on board need to test!!!!*/
 
#define M85XX_PORPLLSR(base)           (CAST(VUINT32 *)((base) + 0xE0000))
#define M85XX_PORPLLSR_E500_RATIO_MASK 0x003f0000
#define M85XX_PORPLLSR_PLAT_RATIO_MASK 0x0000003e
#define M85XX_PORPLLSR_E500_RATIO(base) ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_RATIO_MASK)>>16)
#define M85XX_PORPLLSR_PLAT_RATIO(base) ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_PLAT_RATIO_MASK)>>1)

#define M85XX_DDRDLLCR(base)           (CAST(VUINT32 *)((base) + 0xE0E10))
#define M85XX_LBCDLLSR(base)           (CAST(VUINT32 *)((base) + 0xE0E20))

#define M85XX_DEVDISR(base)            (CAST(VUINT32 *)((base) + 0xE0070))
#define M85XX_DEVDISR_DDR  0x00010000
#define M85XX_PVR(base)                (CAST(VUINT32 *)((base) + 0xE00A0))
#define M85XX_SVR(base)                (CAST(VUINT32 *)((base) + 0xE00A4))

#define _PPC_BUCSR_FI 0x200            /* Invalidate branch cache */
#define _PPC_BUCSR_E 0x1               /* Enable branch prediction */

/* Port A, B, C and D Defines */

#define PA31    (0x00000001)
#define PA30    (0x00000002)
#define PA29    (0x00000004)
#define PA28    (0x00000008)
#define PA27    (0x00000010)
#define PA26    (0x00000020)
#define PA25    (0x00000040)
#define PA24    (0x00000080)
#define PA23    (0x00000100)
#define PA22    (0x00000200)
#define PA21    (0x00000400)
#define PA20    (0x00000800)
#define PA19    (0x00001000)
#define PA18    (0x00002000)
#define PA17    (0x00004000)
#define PA16    (0x00008000)
#define PA15    (0x00010000)
#define PA14    (0x00020000)
#define PA13    (0x00040000)
#define PA12    (0x00080000)
#define PA11    (0x00100000)
#define PA10    (0x00200000)
#define PA9     (0x00400000)
#define PA8     (0x00800000)
#define PA7     (0x01000000)
#define PA6     (0x02000000)
#define PA5     (0x04000000)
#define PA4     (0x08000000)
#define PA3     (0x10000000)
#define PA2     (0x20000000)
#define PA1     (0x40000000)
#define PA0     (0x80000000)

#define PB31    (0x00000001)
#define PB30    (0x00000002)
#define PB29    (0x00000004)
#define PB28    (0x00000008)
#define PB27    (0x00000010)
#define PB26    (0x00000020)
#define PB25    (0x00000040)
#define PB24    (0x00000080)
#define PB23    (0x00000100)
#define PB22    (0x00000200)
#define PB21    (0x00000400)
#define PB20    (0x00000800)
#define PB19    (0x00001000)
#define PB18    (0x00002000)
#define PB17    (0x00004000)
#define PB16    (0x00008000)
#define PB15    (0x00010000)
#define PB14    (0x00020000)
#define PB13    (0x00040000)
#define PB12    (0x00080000)
#define PB11    (0x00100000)
#define PB10    (0x00200000)
#define PB9     (0x00400000)
#define PB8     (0x00800000)
#define PB7     (0x01000000)
#define PB6     (0x02000000)
#define PB5     (0x04000000)
#define PB4     (0x08000000)

#define PC31    (0x00000001)
#define PC30    (0x00000002)
#define PC29    (0x00000004)
#define PC28    (0x00000008)
#define PC27    (0x00000010)
#define PC26    (0x00000020)
#define PC25    (0x00000040)
#define PC24    (0x00000080)
#define PC23    (0x00000100)
#define PC22    (0x00000200)
#define PC21    (0x00000400)
#define PC20    (0x00000800)
#define PC19    (0x00001000)
#define PC18    (0x00002000)
#define PC17    (0x00004000)
#define PC16    (0x00008000)
#define PC15    (0x00010000)
#define PC14    (0x00020000)
#define PC13    (0x00040000)
#define PC12    (0x00080000)
#define PC11    (0x00100000)
#define PC10    (0x00200000)
#define PC9     (0x00400000)
#define PC8     (0x00800000)
#define PC7     (0x01000000)
#define PC6     (0x02000000)
#define PC5     (0x04000000)
#define PC4     (0x08000000)
#define PC3     (0x10000000)
#define PC2     (0x20000000)
#define PC1     (0x40000000)
#define PC0     (0x80000000)

#define PD31    (0x00000001)
#define PD30    (0x00000002)
#define PD29    (0x00000004)
#define PD28    (0x00000008)
#define PD27    (0x00000010)
#define PD26    (0x00000020)
#define PD25    (0x00000040)
#define PD24    (0x00000080)
#define PD23    (0x00000100)
#define PD22    (0x00000200)
#define PD21    (0x00000400)
#define PD20    (0x00000800)
#define PD19    (0x00001000)
#define PD18    (0x00002000)
#define PD17    (0x00004000)
#define PD16    (0x00008000)
#define PD15    (0x00010000)
#define PD14    (0x00020000)
#define PD13    (0x00040000)
#define PD12    (0x00080000)
#define PD11    (0x00100000)
#define PD10    (0x00200000)
#define PD9     (0x00400000)
#define PD8     (0x00800000)
#define PD7     (0x01000000)
#define PD6     (0x02000000)
#define PD5     (0x04000000)
#define PD4     (0x08000000)

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCcds85xxh */
