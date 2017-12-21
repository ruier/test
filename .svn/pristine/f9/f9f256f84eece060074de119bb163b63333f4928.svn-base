/* fsl_mds8568.h - Freescale MDS8568 board header */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,23sep09,x_z  remove QE_MURAM_POOL1_SIZE and QE_MURAM_POOL2_SIZE.
01f,15jul09,b_m  add QE MURAM defines.
01e,12aug08,b_m  remove useless PCI macros.
01d,18jul07,b_m  add PCIe and QE interrupt vectors defines.
01c,28may07,b_m  add QE macros.
01b,21may07,b_m  add BCSR registers.
01a,10may07,b_m  modified from version 01b of wrSbc8548/wrSbc8548.h.
*/

#ifndef __INCfsl_mds8568h
#define __INCfsl_mds8568h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#   define CAST(x)
#else   /* _ASMLANGUAGE */
typedef volatile UCHAR  VCHAR;
typedef volatile INT32  VINT32;
typedef volatile INT16  VINT16;
typedef volatile INT8   VINT8;
typedef volatile UINT32 VUINT32;
typedef volatile UINT16 VUINT16;
typedef volatile UINT8  VUINT8;
#   define CAST(x)      (x)
#endif  /* _ASMLANGUAGE */

#endif

/* Base Address of Memory Mapped Registers */

#define CCSBAR                  0xe0000000

/* Timer clock divider ratios (in 2^n) */

#define FIT_DIVIDER_TAP_29      29
#define FIT_DIVIDER_TAP_28      28
#define FIT_DIVIDER_TAP_27      27
#define FIT_DIVIDER_TAP_26      26
#define FIT_DIVIDER_TAP_25      25
#define FIT_DIVIDER_TAP_24      24
#define FIT_DIVIDER_TAP_23      23
#define FIT_DIVIDER_TAP_22      22
#define FIT_DIVIDER_TAP_21      21
#define FIT_DIVIDER_TAP_20      20
#define FIT_DIVIDER_TAP_19      19
#define FIT_DIVIDER_TAP_18      18
#define FIT_DIVIDER_TAP_17      17
#define FIT_DIVIDER_TAP_16      16
#define FIT_DIVIDER_TAP_15      15
#define FIT_DIVIDER_TAP_14      14
#define FIT_DIVIDER_TAP_13      13
#define FIT_DIVIDER_TAP_12      12
#define FIT_DIVIDER_TAP_11      11
#define FIT_DIVIDER_TAP_10      10

/* PCI defines */

#define NUM_PCIEX_SLOTS         1

#define PCIEX_XINT1_LVL         0       /* PCIEX XINT1 routed to IRQ0 */
#define PCIEX_XINT2_LVL         1       /* PCIEX XINT2 routed to IRQ1 */
#define PCIEX_XINT3_LVL         2       /* PCIEX XINT3 routed to IRQ2 */
#define PCIEX_XINT4_LVL         3       /* PCIEX XINT3 routed to IRQ3 */

#define PCI_LAT_TIMER           0x40    /* latency timer value, 64 PCI clocks */

/* I2C defines */

#define M85XX_I2C1_BASE         0x3000
#define M85XX_I2C2_BASE         0x3100

/*
 *  Local Access Window Configuration
 *
 *  LAWBAR0: 0x00000000 - 0x7fffffff    DDR                 2G
 *  LAWBAR1: 0xf0000000 - 0xffffffff    SDRAM/BCSR/Flash    256M
 *  LAWBAR2: 0x80000000 - 0x9fffffff    PCI                 512M
 *  LAWBAR3: 0xa0000000 - 0xbfffffff    PCIe                512M
 */

#define M85XX_LAWBAR0(base)     (CAST(VUINT32 *)((base) + 0xc08))
#define M85XX_LAWAR0(base)      (CAST(VUINT32 *)((base) + 0xc10))

#define M85XX_LAWBAR1(base)     (CAST(VUINT32 *)((base) + 0xc28))
#define M85XX_LAWAR1(base)      (CAST(VUINT32 *)((base) + 0xc30))

#define M85XX_LAWBAR2(base)     (CAST(VUINT32 *)((base) + 0xc48))
#define M85XX_LAWAR2(base)      (CAST(VUINT32 *)((base) + 0xc50))

#define M85XX_LAWBAR3(base)     (CAST(VUINT32 *)((base) + 0xc68))
#define M85XX_LAWAR3(base)      (CAST(VUINT32 *)((base) + 0xc70))

#define M85XX_LAWBAR4(base)     (CAST(VUINT32 *)((base) + 0xc88))
#define M85XX_LAWAR4(base)      (CAST(VUINT32 *)((base) + 0xc90))

#define M85XX_LAWBAR5(base)     (CAST(VUINT32 *)((base) + 0xca8))
#define M85XX_LAWAR5(base)      (CAST(VUINT32 *)((base) + 0xcb0))

#define M85XX_LAWBAR6(base)     (CAST(VUINT32 *)((base) + 0xcc8))
#define M85XX_LAWAR6(base)      (CAST(VUINT32 *)((base) + 0xcd0))

#define M85XX_LAWBAR7(base)     (CAST(VUINT32 *)((base) + 0xce8))
#define M85XX_LAWAR7(base)      (CAST(VUINT32 *)((base) + 0xcf0))

#define LAWBAR_ADRS_SHIFT       12
#define LAWAR_ENABLE            0x80000000

/* LAWAR target interface */

#define LAWAR_TGTIF_PCI         0x00000000
#define LAWAR_TGTIF_PCI2        0x00100000
#define LAWAR_TGTIF_PCIEX       0x00200000
#define LAWAR_TGTIF_LBC         0x00400000
#define LAWAR_TGTIF_RAPIDIO     0x00C00000
#define LAWAR_TGTIF_DDRSDRAM    0x00F00000

/* LAWAR SIZE Settings */

#define LAWAR_SIZE_4KB          0x0000000B
#define LAWAR_SIZE_8KB          0x0000000C
#define LAWAR_SIZE_16KB         0x0000000D
#define LAWAR_SIZE_32KB         0x0000000E
#define LAWAR_SIZE_64KB         0x0000000F
#define LAWAR_SIZE_128KB        0x00000010
#define LAWAR_SIZE_256KB        0x00000011
#define LAWAR_SIZE_512KB        0x00000012
#define LAWAR_SIZE_1MB          0x00000013
#define LAWAR_SIZE_2MB          0x00000014
#define LAWAR_SIZE_4MB          0x00000015
#define LAWAR_SIZE_8MB          0x00000016
#define LAWAR_SIZE_16MB         0x00000017
#define LAWAR_SIZE_32MB         0x00000018
#define LAWAR_SIZE_64MB         0x00000019
#define LAWAR_SIZE_128MB        0x0000001A
#define LAWAR_SIZE_256MB        0x0000001B
#define LAWAR_SIZE_512MB        0x0000001C
#define LAWAR_SIZE_1GB          0x0000001D
#define LAWAR_SIZE_2GB          0x0000001E

/* Local Bus Controller (LBC) Registers */

#define M85XX_BR0(base)         (CAST(VUINT32 *)((base) + 0x5000))
#define M85XX_OR0(base)         (CAST(VUINT32 *)((base) + 0x5004))

#define M85XX_BR1(base)         (CAST(VUINT32 *)((base) + 0x5008))
#define M85XX_OR1(base)         (CAST(VUINT32 *)((base) + 0x500c))

#define M85XX_BR2(base)         (CAST(VUINT32 *)((base) + 0x5010))
#define M85XX_OR2(base)         (CAST(VUINT32 *)((base) + 0x5014))

#define M85XX_BR3(base)         (CAST(VUINT32 *)((base) + 0x5018))
#define M85XX_OR3(base)         (CAST(VUINT32 *)((base) + 0x501c))

#define M85XX_BR4(base)         (CAST(VUINT32 *)((base) + 0x5020))
#define M85XX_OR4(base)         (CAST(VUINT32 *)((base) + 0x5024))

#define M85XX_BR5(base)         (CAST(VUINT32 *)((base) + 0x5028))
#define M85XX_OR5(base)         (CAST(VUINT32 *)((base) + 0x502c))

#define M85XX_BR6(base)         (CAST(VUINT32 *)((base) + 0x5030))
#define M85XX_OR6(base)         (CAST(VUINT32 *)((base) + 0x5034))

#define M85XX_BR7(base)         (CAST(VUINT32 *)((base) + 0x5038))
#define M85XX_OR7(base)         (CAST(VUINT32 *)((base) + 0x503c))

#define M85XX_MAR(base)         (CAST(VUINT32 *)((base) + 0x5068))
#define M85XX_MAMR(base)        (CAST(VUINT32 *)((base) + 0x5070))
#define M85XX_MBMR(base)        (CAST(VUINT32 *)((base) + 0x5074))
#define M85XX_MCMR(base)        (CAST(VUINT32 *)((base) + 0x5078))
#define M85XX_MRTPR(base)       (CAST(VUINT32 *)((base) + 0x5084))
#define MRTPR_PTP_MASK          0xff000000
#define MRTPR_PTP_WRITE(x)      ((x << 24) & MRTPR_PTP_MASK)
#define M85XX_MDR(base)         (CAST(VUINT32 *)((base) + 0x5088))
#define M85XX_LSDMR(base)       (CAST(VUINT32 *)((base) + 0x5094))
#define LSDMR_RFEN              0x40000000  /* Refresh Enable */

/*
 * LSDMR OP - 000 Normal operation
 *          - 001 Auto Refresh  (Initialization)
 *          - 010 Self Refresh
 *          - 011 Mode Register Write (Initialization)
 *          - 100 Precharge Bank
 *          - 101 Precharge all banks (Initialization)
 *          - 110 Activate Bank
 *          - 111 Read/Write without valid transfer
 */

#define LSDMR_OP_MASK           0x38000000
#define LSDMR_OP_SHIFT(x)       ((x << 27) & LSDMR_OP_MASK)

/*
 * Bank Select Multiplexed address line - 000 lines 12:13
 *                                      - 001       13:14
 *                                      - 010       14:15
 *                                      - 011       15:16
 *                                      - 100       16:17
 *                                      - 101       17:18
 *                                      - 110       18:19
 *                                      - 111       19:20
 */

#define LSDMR_BSMA_MASK         0x00E00000
#define LSDMR_BSMA_SHIFT(x)     ((x << 23) & LSDMR_BSMA_MASK)

/*
 * RFCR Refresh recovery 000 - reserved
 *                       001->110 - 3->8 clocks
 *                       111 - 16 clocks
 */

#define LSDMR_RFCR_MASK         0x00038000
#define LSDMR_RFCR_SHIFT(x)     ((x << 15) & LSDMR_RFCR_MASK)

/* Incomplete LSDMR definitions */

#define M85XX_LURT(base)        (CAST(VUINT32 *)((base) + 0x50a0))
#define M85XX_LSRT(base)        (CAST(VUINT32 *)((base) + 0x50a4))
#define M85XX_LTESR(base)       (CAST(VUINT32 *)((base) + 0x50b0))
#define M85XX_LTEDR(base)       (CAST(VUINT32 *)((base) + 0x50b4))
#define M85XX_LTEIR(base)       (CAST(VUINT32 *)((base) + 0x50b8))
#define M85XX_LTEATR(base)      (CAST(VUINT32 *)((base) + 0x50bc))
#define M85XX_LTEAR(base)       (CAST(VUINT32 *)((base) + 0x50c0))

/* LBC Clock Configuration */

#define M85XX_LBCR(base)        (CAST(VUINT32 *)((base) + 0x50d0))
#define M85XX_LCRR(base)        (CAST(VUINT32 *)((base) + 0x50d4))

#define M85XX_DCR0(base)        (CAST(VUINT32 *)((base) + 0xe0f1c))
#define M85XX_DCR1(base)        (CAST(VUINT32 *)((base) + 0xe0f20))

/* ECM Registers */

#define ECM_OFFSET              0x1000
#define ECMBA                   (CCSBAR | ECM_OFFSET)

/* Offsets for DDR registers */

#define DDR_OFFSET              0x2000
#define DDRBA                   (CCSBAR | DDR_OFFSET)

#define CS0_BNDS                    0x000
#define CS1_BNDS                    0x008
#define CS2_BNDS                    0x010
#define CS3_BNDS                    0x018
#define CS0_CONFIG                  0x080
#define CS1_CONFIG                  0x084
#define CS2_CONFIG                  0x088
#define CS3_CONFIG                  0x08c
#define EXTENDED_REF_REC            0x100
#define TIMING_CFG_0                0x104
#define TIMING_CFG_1                0x108
#define TIMING_CFG_2                0x10c
#define DDR_SDRAM_CFG               0x110
#define DDR_SDRAM_CFG_2             0x114
#define DDR_SDRAM_MODE_CFG          0x118
#define DDR_SDRAM_MODE_CFG_2        0x11c
#define DDR_SDRAM_MD_CNTL           0x120
#define DDR_SDRAM_INTERVAL          0x124
#define DDR_DATA_INIT               0x128
#define DDR_SDRAM_CLK_CTRL          0x130

#define DDR_DATA_ERR_INJECT_HI      0xe00
#define DDR_DATA_ERR_INJECT_LO      0xe04
#define DDR_ECC_ERR_INJECT          0xe08
#define DDR_CAPTURE_DATA_HI         0xe20
#define DDR_CAPTURE_DATA_LO         0xe24
#define DDR_CAPTURE_ECC             0xe28
#define DDR_ERR_DETECT              0xe40
#define DDR_ERR_DISABLE             0xe44
#define DDR_ERR_INT_EN              0xe48
#define DDR_CAPTURE_ATTRIBUTES      0xe4c
#define DDR_CAPTURE_ADDRESS         0xe50
#define DDR_ERR_SBE                 0xe58

#define DDR_IO_OVCR                 0x90000000

/* LBC SDRAM Configuration */

/* SDRAM Machine Mode Register */

#define M85XX_LSDMR_COMMON          0x4063FF03
#define M85XX_LSDMR_OP_NORMAL       0x00000000
#define M85XX_LSDMR_OP_AUTOREF      0x08000000
#define M85XX_LSDMR_OP_SELFREF      0x10000000
#define M85XX_LSDMR_OP_MRWRITE      0x18000000
#define M85XX_LSDMR_OP_PREBANK      0x20000000
#define M85XX_LSDMR_OP_PREALL       0x28000000
#define M85XX_LSDMR_OP_ACTBANK      0x30000000
#define M85XX_LSDMR_OP_RW           0x38000000

/* This value is the 60x bus-assigned SDRAM Refresh Timer PSRT setting */

#define LSRT_VALUE                  0x20

/*
 * This value is the setting for the MPTPR[PTP] Refresh timer prescaler.
 * The value is dependent on the OSCILLATOR_FREQ value.  For other values
 * a conditionally compiled term must be created here for that OSCILLATOR_FREQ
 * value.
 *
 * BRGCLK_DIV_FACTOR
 * Baud Rate Generator division factor
 *              - 0 for division by 1
 *              - 1 for division by 16
 */

#define DIV_FACT_1                  0
#define DIV_FACT_16                 1

/* Assume 533MHz CCB with local bus clk ratio of 8 */

#define TPR                         0x0000

/* Global Function Registers */
/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */
/* Plat Ratio not working on board need to test!!!!*/

#define M85XX_PORPLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0000))
#define M85XX_PORPLLSR_E500_RATIO_MASK  0x003f0000
#define M85XX_PORPLLSR_PLAT_RATIO_MASK  0x0000003e
#define M85XX_PORPLLSR_E500_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_RATIO_MASK) >> 16)
#define M85XX_PORPLLSR_PLAT_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_PLAT_RATIO_MASK) >> 1)

#define M85XX_PORDEVSR(base)           (CAST(VUINT32 *)((base) + 0xE000C))
#define M85XX_PORDEVSR_PCI_MODE_MASK    0x00800000
#define M85XX_PORDEVSR_PCI_MODE(base)   \
        ((*M85XX_PORDEVSR(base) & M85XX_PORDEVSR_PCI_MODE_MASK) >> 23)
#define PORDEVSR_PCIX_MODE              0
#define PORDEVSR_PCI_MODE               1

#define M85XX_DDRDLLCR(base)            (CAST(VUINT32 *)((base) + 0xE0E10))
#define M85XX_LBCDLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0E20))
#define M85XX_PMUXCR(base)              (CAST(VUINT32 *)((base) + 0xE0060))
#define M85XX_DEVDISR(base)             (CAST(VUINT32 *)((base) + 0xE0070))
#define M85XX_DEVDISR_DDR               0x00010000
#define M85XX_PVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A0))
#define M85XX_SVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A4))

#define _PPC_BUCSR_FI                   0x200   /* Invalidate branch cache */
#define _PPC_BUCSR_E                    0x1     /* Enable branch prediction */

/* QUICC Engine */

#define QUICC_ENGINE_BA         0x80000
#define QE_MURAM_BA             0x10000
#define QE_MURAM_SIZE           0x10000
#define INUM_QE_HIGH            42
#define INUM_QE_LOW             43
#define QE_IO_PORTS             0xe0100

/* QE interrupt vectors */

#define QE_INUM_ERROR           0
#define QE_INUM_SPI2            1
#define QE_INUM_SPI1            2
#define QE_INUM_RTT             3
#define QE_INUM_SDMA            10
#define QE_INUM_USB             11
#define QE_INUM_TIMER1          12
#define QE_INUM_TIMER2          13
#define QE_INUM_TIMER3          14
#define QE_INUM_TIMER4          15
#define QE_INUM_VT              20
#define QE_INUM_EXT1            25
#define QE_INUM_EXT2            26
#define QE_INUM_EXT3            27
#define QE_INUM_EXT4            28
#define QE_INUM_UCC1            32
#define QE_INUM_UCC2            33
#define QE_INUM_UCC3            34
#define QE_INUM_UCC4            35
#define QE_INUM_MCC1            36
#define QE_INUM_UCC5            40
#define QE_INUM_UCC6            41
#define QE_INUM_UCC7            42
#define QE_INUM_UCC8            43

#define INUM_MAX                128
#define IVEC_MAX                128

/* QE mux UCC clock route registers */

#define QE_MUX                  (QUICC_ENGINE_BA + 0x400)

#define QE_CMXUCR1(base)        (CAST(VUINT32 *)((base) + QE_MUX + 0x10))
#define QE_CMXUCR2(base)        (CAST(VUINT32 *)((base) + QE_MUX + 0x14))
#define QE_CMXUCR3(base)        (CAST(VUINT32 *)((base) + QE_MUX + 0x18))
#define QE_CMXUCR4(base)        (CAST(VUINT32 *)((base) + QE_MUX + 0x1C))
#define QE_CMXUPCR(base)        (CAST(VUINT32 *)((base) + QE_MUX + 0x20))

/* QE SDMA registers */

#define QE_SDMA                 (QUICC_ENGINE_BA + 0x4000)

#define QE_SDSR(base)           (CAST(VUINT32 *)((base) + QE_SDMA + 0x00))
#define QE_SDMR(base)           (CAST(VUINT32 *)((base) + QE_SDMA + 0x04))

/* needed to enable timer base */

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

/* BCSR defines */

#define BCSR0                   (volatile unsigned char *)(BCSR_BASE_ADRS + 0)
#define BCSR1                   (volatile unsigned char *)(BCSR_BASE_ADRS + 1)
#define BCSR2                   (volatile unsigned char *)(BCSR_BASE_ADRS + 2)
#define BCSR3                   (volatile unsigned char *)(BCSR_BASE_ADRS + 3)
#define BCSR4                   (volatile unsigned char *)(BCSR_BASE_ADRS + 4)
#define BCSR5                   (volatile unsigned char *)(BCSR_BASE_ADRS + 5)
#define BCSR6                   (volatile unsigned char *)(BCSR_BASE_ADRS + 6)
#define BCSR7                   (volatile unsigned char *)(BCSR_BASE_ADRS + 7)
#define BCSR8                   (volatile unsigned char *)(BCSR_BASE_ADRS + 8)
#define BCSR9                   (volatile unsigned char *)(BCSR_BASE_ADRS + 9)
#define BCSR10                  (volatile unsigned char *)(BCSR_BASE_ADRS + 10)
#define BCSR14                  (volatile unsigned char *)(BCSR_BASE_ADRS + 14)

#define BCSR_LED_RED            1
#define BCSR_LED_YELLOW         2
#define BCSR_LED_GREEN          4


/* PCI Express Host Controller */

/* Outbound/Inbound Comparison mask register defines */

#define PCI_WINDOW_ENABLE_BIT   0x80000000
#define PCI_ATTR_BS_BIT         0x40000000
#define PCI_OUT_ATTR_RTT_MEM    0x00040000
#define PCI_OUT_ATTR_RTT_IO     0x00080000
#define PCI_OUT_ATTR_WTT_MEM    0x00004000
#define PCI_OUT_ATTR_WTT_IO     0x00008000
#define PCI_ATTR_WS_4K          0x0000000B
#define PCI_ATTR_WS_8K          0x0000000C
#define PCI_ATTR_WS_16K         0x0000000D
#define PCI_ATTR_WS_32K         0x0000000E
#define PCI_ATTR_WS_64K         0x0000000F
#define PCI_ATTR_WS_128K        0x00000010
#define PCI_ATTR_WS_256K        0x00000011
#define PCI_ATTR_WS_512K        0x00000012
#define PCI_ATTR_WS_1M          0x00000013
#define PCI_ATTR_WS_2M          0x00000014
#define PCI_ATTR_WS_4M          0x00000015
#define PCI_ATTR_WS_8M          0x00000016
#define PCI_ATTR_WS_16M         0x00000017
#define PCI_ATTR_WS_32M         0x00000018
#define PCI_ATTR_WS_64M         0x00000019
#define PCI_ATTR_WS_128M        0x0000001A
#define PCI_ATTR_WS_256M        0x0000001B
#define PCI_ATTR_WS_512M        0x0000001C
#define PCI_ATTR_WS_1G          0x0000001D
#define PCI_ATTR_WS_2G          0x0000001E
#define PCI_ATTR_WS_4G          0x0000001F

#define PCI_IN_ATTR_TGI_LM                                  0x00f00000
#define PCI_IN_ATTR_TGI_RIO                                 0x00c00000
#define PCI_IN_ATTR_RTT_RIO_READ                            0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0x00007000

#define PCI_SNOOP_ENABLE        0x40000000
#define PCI_PREFETCHABLE        0x20000000

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_mds8568h */
