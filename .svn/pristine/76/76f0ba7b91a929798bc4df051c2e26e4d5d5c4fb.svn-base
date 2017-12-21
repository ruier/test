/* fsl_mds8569.h - Freescale MDS8569 board header */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,07jul11,y_c  Support VxWorks native bootrom and SD/MMC card. (WIND00240156)
01c,15oct09,x_z  add support for UCC SIO and code clean
01b,10sep09,x_z  remove M85XX_PVR, M85XX_SVR and clean macros about MURAM pool
01a,10jul09,x_z  derived from version 01e of fsl_mmds8568/fsl_mds8568.h.
*/

#ifndef __INCfsl_mds8569h
#define __INCfsl_mds8569h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef M8260ABBREVIATIONS
    #define M8260ABBREVIATIONS

#   ifdef  _ASMLANGUAGE

#       define CAST(x)

#   else   /* _ASMLANGUAGE */

typedef volatile UCHAR  VCHAR;
typedef volatile INT32  VINT32;
typedef volatile INT16  VINT16;
typedef volatile INT8   VINT8;
typedef volatile UINT32 VUINT32;
typedef volatile UINT16 VUINT16;
typedef volatile UINT8  VUINT8;

#       define CAST(x)      (x)

#   endif  /* _ASMLANGUAGE */

#endif  /* M8260ABBREVIATIONS */

/* System Version Register (SVR) value */

#define SVR_MPC8569_MASK        0xffff0000
#define SVR_MPC8569             0x80800000  /* without security engine  */
#define SVR_MPC8569E            0x80880000  /* with security engine     */

/* Base Address of Memory Mapped Registers */

#define CCSBAR_RESET            0xff700000
#define CCSBAR                  0xe0000000
#define CCSR_SIZE               0x00100000

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

#define PCIEX_BASE_ADRS         0xa000

#define NUM_PCIEX_SLOTS         1
#define NUM_PCIEX_SUB_BUS       16

#define PCIEX_XINT1_LVL         0       /* PCIEX XINT1 routed to IRQ1 */
#define PCIEX_XINT2_LVL         1       /* PCIEX XINT2 routed to IRQ2 */
#define PCIEX_XINT3_LVL         2       /* PCIEX XINT3 routed to IRQ3 */
#define PCIEX_XINT4_LVL         3       /* PCIEX XINT3 routed to IRQ4 */

#define PCI_LAT_TIMER           0x40    /* latency timer value, 64 PCI clocks */

/* DUART defines */

#define M85XX_UART0_BASE        0x4500
#define M85XX_UART1_BASE        0x4600

/* I2C defines */

#define M85XX_I2C1_BASE         0x3000
#define M85XX_I2C2_BASE         0x3100

/*
 *  Local Access Window Configuration
 *
 *  LAWBAR0: 0x00000000 - 0x3fffffff    DDR                 1G
 *  LAWBAR1: 0xf8000000 - 0xf8007fff    BCSR                32K
 *  LAWBAR2: 0xfe000000 - 0xffffffff    Nor Flash           32M
 *  LAWBAR3: 0xa0000000 - 0xafffffff    PCIe                256M
 */

#define M85XX_LAW_NUM           10

#define M85XX_LAWBARn(base, n)  (CAST(VUINT32 *)((base) + 0xc08 + n * 0x20))
#define M85XX_LAWARn(base, n)   (CAST(VUINT32 *)((base) + 0xc10 + n * 0x20))

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

#define M85XX_CS_NUM            8

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

#define M85XX_BRn(base, n)      (CAST(VUINT32 *)((base) + 0x5000 + n * 0x8))
#define M85XX_ORn(base, n)      (CAST(VUINT32 *)((base) + 0x5004 + n * 0x8))

/* LBC Clock Configuration */

#define M85XX_LBCR(base)        (CAST(VUINT32 *)((base) + 0x50d0))
#define M85XX_LCRR(base)        (CAST(VUINT32 *)((base) + 0x50d4))

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
#define TIMING_CFG_4                0x160
#define TIMING_CFG_5                0x164
#define DDR_DDR_ZQ_CNTL             0x170
#define DDR_DDR_WRLVL_CNTL          0x174

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

#define M85XX_LCRR_CLKDIV_MASK  0x0000001f

/* Global Function Registers */
/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */
/* Plat Ratio not working on board need to test!!!!*/

#define M85XX_PORPLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0000))
#define M85XX_PORPLLSR_QE_RATIO_MASK    0x3e000000
#define M85XX_PORPLLSR_E500_RATIO_MASK  0x003f0000
#define M85XX_PORPLLSR_DDR_RATIO_MASK   0x00003e00
#define M85XX_PORPLLSR_PLAT_RATIO_MASK  0x0000003e
#define M85XX_PORPLLSR_QE_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_QE_RATIO_MASK) >> 25)
#define M85XX_PORPLLSR_E500_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_RATIO_MASK) >> 16)
#define M85XX_PORPLLSR_DDR_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_DDR_RATIO_MASK) >> 9)
#define M85XX_PORPLLSR_PLAT_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_PLAT_RATIO_MASK) >> 1)

#define M85XX_PORDEVSR(base)           (CAST(VUINT32 *)((base) + 0xE000C))
#define M85XX_PORDEVSR_PCI_MODE_MASK    0x00800000
#define M85XX_PORDEVSR_PCI_MODE(base)   \
        ((*M85XX_PORDEVSR(base) & M85XX_PORDEVSR_PCI_MODE_MASK) >> 23)
#define PORDEVSR_PCIX_MODE              0
#define PORDEVSR_PCI_MODE               1

#define M85XX_LBCDLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0E20))
#define M85XX_PMUXCR(base)              (CAST(VUINT32 *)((base) + 0xE0060))
#define M85XX_DEVDISR(base)             (CAST(VUINT32 *)((base) + 0xE0070))
#define M85XX_DEVDISR_DDR               0x00010000

#define _PPC_BUCSR_FI                   0x200   /* Invalidate branch cache */
#define _PPC_BUCSR_E                    0x1     /* Enable branch prediction */

/* QUICC Engine */

#define QUICC_ENGINE_BA                 (CCSBAR + 0x80000)
#define UCC1_BA                         (QUICC_ENGINE_BA + 0x02000)
#define UCC2_BA                         (QUICC_ENGINE_BA + 0x03000)
#define UCC3_BA                         (QUICC_ENGINE_BA + 0x02200)
#define UCC4_BA                         (QUICC_ENGINE_BA + 0x03200)
#define UCC5_BA                         (QUICC_ENGINE_BA + 0x02400)
#define UCC6_BA                         (QUICC_ENGINE_BA + 0x03400)
#define UCC7_BA                         (QUICC_ENGINE_BA + 0x02600)
#define UCC8_BA                         (QUICC_ENGINE_BA + 0x03600)
#define QE_MURAM_BA                     (QUICC_ENGINE_BA + 0x10000)
#define QE_MURAM_SIZE                   0x20000
#define INUM_QE_HIGH                    42
#define INUM_QE_LOW                     43

/* QE Pin Muxing */

#define QE_IO_PORTS                     (CCSBAR  + 0xe0100)

/* QE interrupt vectors */

#define QE_INUM_ERROR                   0
#define QE_INUM_SPI2                    1
#define QE_INUM_SPI1                    2
#define QE_INUM_RTT                     3
#define QE_INUM_SDMA                    10
#define QE_INUM_USB                     11
#define QE_INUM_TIMER1                  12
#define QE_INUM_TIMER2                  13
#define QE_INUM_TIMER3                  14
#define QE_INUM_TIMER4                  15
#define QE_INUM_VT                      20
#define QE_INUM_EXT1                    25
#define QE_INUM_EXT2                    26
#define QE_INUM_EXT3                    27
#define QE_INUM_EXT4                    28
#define QE_INUM_UCC1                    32
#define QE_INUM_UCC2                    33
#define QE_INUM_UCC3                    34
#define QE_INUM_UCC4                    35
#define QE_INUM_MCC1                    36
#define QE_INUM_UCC5                    40
#define QE_INUM_UCC6                    41
#define QE_INUM_UCC7                    42
#define QE_INUM_UCC8                    43

#define INUM_MAX                        64
#define IVEC_MAX                        64

/* CMXUCR registers */

#define QE_CMXUCR1_UCC1_RX_DIS          0x00000000
#define QE_CMXUCR1_UCC1_TX_CLK12        0x00080000
#define QE_CMXUCR1_UCC2_RX_DIS          0x00000000
#define QE_CMXUCR1_UCC2_TX_CLK17        0x00070000
#define QE_CMXUCR1_UCC3_RX_DIS          0x00000000
#define QE_CMXUCR1_UCC3_TX_CLK12        0x00000008
#define QE_CMXUCR1_UCC4_RX_DIS          0x00000000
#define QE_CMXUCR1_UCC4_TX_CLK17        0x00000007

/* QE mux UCC clock route registers */

#define QE_MUX                          (QUICC_ENGINE_BA + 0x400)

#define QE_CMXGCR                       (CAST(VUINT32 *) QE_MUX)
#define QE_CMXGCR_MEM_MASK              0x00007000

#define QE_CMXUCR1                      (CAST(VUINT32 *) (QE_MUX + 0x10))
#define QE_CMXUCR2                      (CAST(VUINT32 *) (QE_MUX + 0x14))
#define QE_CMXUCR3                      (CAST(VUINT32 *) (QE_MUX + 0x18))
#define QE_CMXUCR4                      (CAST(VUINT32 *) (QE_MUX + 0x1c))
#define QE_CMXUPCR                      (CAST(VUINT32 *) (QE_MUX + 0x20))

/* 2 UCCs for per CMXUCR */

#define QE_CMXUCR_HIGH_MASK             0xffff0000
#define QE_CMXUCR_LOW_MASK              0x0000ffff
#define QE_CMXUCR_SHIFT                 16      /* shift for high halfword  */
#define QE_CMXUCR_RUCS_SHIFT            4       /* Rx clock source shift    */
#define QE_CMXUCR_TUCS_SHIFT            0       /* Tx clock source shift    */
#define QE_CMXUCR_UCS_MASK              0xf     /* clock source select Mask */

/* needed to enable timer base */

#define HID0_MCP                        0x80000000
#define HID0_SEL_TBCLK                  0x00002000
#define HID1_ABE                        0x00001000
#define HID1_ASTME                      0x00002000
#define HID1_RXFE                       0x00020000

/* L2 Control Register initate value */

#define L2CTL_INIT_VAL                  0x20000000

/* BCSR defines */

#define BCSR0                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 0))
#define BCSR1                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 1))
#define BCSR2                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 2))
#define BCSR3                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 3))
#define BCSR4                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 4))
#define BCSR5                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 5))
#define BCSR6                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 6))
#define BCSR7                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 7))
#define BCSR8                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 8))
#define BCSR9                           (CAST(VUINT8 *)(BCSR_BASE_ADRS + 9))
#define BCSR10                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 10))
#define BCSR11                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 11))
#define BCSR12                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 12))
#define BCSR13                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 13))
#define BCSR14                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 14))
#define BCSR15                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 15))
#define BCSR16                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 16))
#define BCSR17                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 17))
#define BCSR18                          (CAST(VUINT8 *)(BCSR_BASE_ADRS + 18))

#define BCSR6_SD_CARD_1BIT_EN           0x08
#define BCSR6_SD_CARD_4BIT_EN           0x04

#define BCSR7_UCC1_GETH_EN              0x80
#define BCSR7_UCC1_RGMII_EN             0x40
#define BCSR7_GETHRST_UCC_1_2           0x04
#define BCSR7_BRD_WRT_PROTECT           0x02

#define BCSR8_UCC2_GETH_EN              0x80
#define BCSR8_UCC2_RGMII_EN             0x40
#define BCSR8_UEM_MARVEL_RESET          0x02

#define BCSR9_UCC3_GETH_EN              0x80
#define BCSR9_UCC3_RGMII_EN             0x40
#define BCSR9_UCC_RMII_EN               0x08
#define BCSR9_UCC_SMII_EN               0x04
#define BCSR9_UCC6_RESV                 0x02
#define BCSR9_UCC3_UEM_MICREL           0x01

#define BCSR10_UCC4_GETH_EN             0x80
#define BCSR10_UCC4_RGMII_EN            0x40
#define BCSR10_UCC_RMII_EN              0x10
#define BCSR10_UCC_SMII_EN              0x08
#define BCSR10_UCC3_UEM_MICREL          0x04

#define BCSR11_LED_RED                  0x01
#define BCSR11_LED_YELLOW               0x02
#define BCSR11_LED_GREEN                0x04

#define BCSR15_QE_UART_EN               0x01

#define BCSR17_FLASH_WP                 0x01

/* Global Utilities */

#define GU_BASE                         (CCSBAR + 0xe0000)

#define PORPLLSR                        (GU_BASE + 0x00000)
#define GENCFGR                         (GU_BASE + 0x00030)
#define PLPPAR1                         (GU_BASE + 0x00040)
#define PLPDIR1                         (GU_BASE + 0x00048)

/* GENCFGR defines */

#define SDHC_WP_INV                     0x20000000

/* PMUXCR defines */

#define SDHC_DATA                       0x80000000
#define SDHC_CD                         0x40000000
#define SDHC_WP                         0x20000000

/* EPIC defines */

#define EPIC_EX_IRQ_NUM                 12
#define EPIC_IN_IRQ_NUM                 64
#define EPIC_GT_IRQ_NUM                 4
#define EPIC_MSG_IRQ_NUM                8
#define EPIC_SMSG_IRQ_NUM               8
#define EPIC_IPI_IRQ_NUM                4

#define EPIC_EX_REG_BASE                0x50000
#define EPIC_IN_REG_BASE                0x50200
#define EPIC_GT_A_REG_BASE              0x41120
#define EPIC_GT_B_REG_BASE              0x42120
#define EPIC_MSG_REG_BASE               0x51600
#define EPIC_SMSG_REG_BASE              0x51c00
#define EPIC_IPI_REG_BASE               0x410a0

#define EPIC_EX_REG_INTERVAL            0x20
#define EPIC_IN_REG_INTERVAL            0x20
#define EPIC_GT_REG_INTERVAL            0x40
#define EPIC_MSG_REG_INTERVAL           0x20
#define EPIC_SMSG_REG_INTERVAL          0x20
#define EPIC_IPI_REG_INTERVAL           0x10

/* interrupt vectors */

#define INUM_EX_IRQ                     0
#define INUM_IN_IRQ                     (INUM_EX_IRQ + EPIC_EX_IRQ_NUM)
#define INUM_ESDHC                      (INUM_IN_IRQ + 56)

/* eSDHC */

#define SDHC_BASE                       (CCSBAR + 0x2E000)

/* PCI Express Host Controller */

/* Outbound/Inbound Comparison mask register defines */

#define PCI_WINDOW_ENABLE_BIT                               0x80000000
#define PCI_ATTR_BS_BIT                                     0x40000000
#define PCI_OUT_ATTR_RTT_MEM                                0x00040000
#define PCI_OUT_ATTR_RTT_IO                                 0x00080000
#define PCI_OUT_ATTR_WTT_MEM                                0x00004000
#define PCI_OUT_ATTR_WTT_IO                                 0x00008000
#define PCI_ATTR_WS_4K                                      0x0000000B
#define PCI_ATTR_WS_8K                                      0x0000000C
#define PCI_ATTR_WS_16K                                     0x0000000D
#define PCI_ATTR_WS_32K                                     0x0000000E
#define PCI_ATTR_WS_64K                                     0x0000000F
#define PCI_ATTR_WS_128K                                    0x00000010
#define PCI_ATTR_WS_256K                                    0x00000011
#define PCI_ATTR_WS_512K                                    0x00000012
#define PCI_ATTR_WS_1M                                      0x00000013
#define PCI_ATTR_WS_2M                                      0x00000014
#define PCI_ATTR_WS_4M                                      0x00000015
#define PCI_ATTR_WS_8M                                      0x00000016
#define PCI_ATTR_WS_16M                                     0x00000017
#define PCI_ATTR_WS_32M                                     0x00000018
#define PCI_ATTR_WS_64M                                     0x00000019
#define PCI_ATTR_WS_128M                                    0x0000001A
#define PCI_ATTR_WS_256M                                    0x0000001B
#define PCI_ATTR_WS_512M                                    0x0000001C
#define PCI_ATTR_WS_1G                                      0x0000001D
#define PCI_ATTR_WS_2G                                      0x0000001E
#define PCI_ATTR_WS_4G                                      0x0000001F

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

#define PCI_SNOOP_ENABLE                                    0x40000000
#define PCI_PREFETCHABLE                                    0x20000000

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_mds8569h */
