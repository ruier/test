/* mpc8308rdb.h - Freescale MPC830x mitx board header */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,30jun11,e_d  initial creation.
*/

/*
 * This file contains I/O addresses and related constants for the
 * Freescale MPC830x mitx board.
 */

#ifndef __INCmpc830xmitxh
#define __INCmpc830xmitxh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <drv/mem/quiccLbc.h>

#ifdef  _ASMLANGUAGE
#   define  CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR  VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile INT32  VINT32;     /* volatile unsigned word */
typedef volatile INT16  VINT16;     /* volatile unsigned halfword */
typedef volatile INT8   VINT8;      /* volatile unsigned byte */
typedef volatile UINT32 VUINT32;    /* volatile unsigned word */
typedef volatile UINT16 VUINT16;    /* volatile unsigned halfword */
typedef volatile UINT8  VUINT8;     /* volatile unsigned byte */
#   define  CAST(x) (x)
extern void     sysMsDelay (uint32_t);
extern void     sysDelay (void);
extern uint32_t sysAbs (int);
extern uint32_t sysDecGet (void);
extern UINT32   vxHid2Get (void);
extern void     vxHid2Set (UINT32);
extern UINT32   vxSvrGet (void);

#endif  /* _ASMLANGUAGE */

/* IMMR Base Address */

#define IMMR_INIT               0xff400000
#define IMMR_BASE               0xe0000000
#define IMMR_SIZE               0x00200000
#define CCSBAR                  IMMR_BASE

/* IMMR Memory Map */

#define SCCFG_REG_BA            (IMMR_BASE + 0x00100)
#define RTC_BASE                (IMMR_BASE + 0x00300)
#define GTM_BASE                (IMMR_BASE + 0x00500)
#define IPIC_BASE               (IMMR_BASE + 0x00700)
#define ACR_BASE                (IMMR_BASE + 0x00800)
#define RST_CFG_BASE            (IMMR_BASE + 0x00900)
#define CLKM_REG_BASE           (IMMR_BASE + 0x00A00)
#define GPIO1_BASE              (IMMR_BASE + 0x00C00)
#define GPIO2_BASE              (IMMR_BASE + 0x00D00)
#define DDR_REG_BASE            (IMMR_BASE + 0x02000)
#define UART0_BASE              (IMMR_BASE + 0x04500)
#define UART1_BASE              (IMMR_BASE + 0x04600)
#define ELBC_BASE               (IMMR_BASE + 0x05000)
#define PCI_BASE                (IMMR_BASE + 0x08000)
#define USB_DR_BASE             (IMMR_BASE + 0x23000)
#define ETSEC1_BASE             (IMMR_BASE + 0x24000)
#define ETSEC2_BASE             (IMMR_BASE + 0x25000)
#define DMAC_BASE               (IMMR_BASE + 0x2c000)
#define SDHC_BASE               (IMMR_BASE + 0x2e000)
#define SRDS_BASE               (IMMR_BASE + 0xE3000)
#define QUICC_ENGINE_BASE       (IMMR_BASE + 0x100000)

/* Reset Configuration Register */

#define RCWLR                   (RST_CFG_BASE + 0x00)
#define RCWHR                   (RST_CFG_BASE + 0x04)
#define RSR                     (RST_CFG_BASE + 0x10)
#define RMR                     (RST_CFG_BASE + 0x14)
#define RPR                     (RST_CFG_BASE + 0x18)
#define RCR                     (RST_CFG_BASE + 0x1c)
#define RCER                    (RST_CFG_BASE + 0x20)

#define SYS_PLL_RATIO_SHIFT     24
#define SYS_PLL_RATIO_MASK      (0x0f << SYS_PLL_RATIO_SHIFT)
#define SYS_PLL_RATIO           ((*(CAST(VUINT32 *)RCWLR) & \
                                  SYS_PLL_RATIO_MASK) >> SYS_PLL_RATIO_SHIFT)

/* Clock Source Speed */

#define FREQ_33_MHZ             33333333
#define SYS_CLK_IN_FREQ         FREQ_33_MHZ
#define SYS_CLK_FREQ            (SYS_CLK_IN_FREQ * 4)

/* SYS_CLK and AUX_CLK rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN        1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX        8000    /* maximum auxiliary clock rate */

/* Network device */

#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS            3       /* three network devices */

/* WR specific portion of MAC (MSB->LSB) */

#define WR_ENET0                0x00
#define WR_ENET1                0xA0
#define WR_ENET2                0x1E

/* Customer portion of MAC address */

#define CUST_ENET3_0            0xA0
#define CUST_ENET3_1            0xA1
#define CUST_ENET3_2            0xA2
#define CUST_ENET3_3            0xA3
#define CUST_ENET4              0xAA
#define CUST_ENET5              0xA0

#define MAX_MAC_ADRS            3
#define MAC_ADRS_LEN            6

/* default mac address */

#define ENET_DEFAULT0           WR_ENET0
#define ENET_DEFAULT1           WR_ENET1
#define ENET_DEFAULT2           WR_ENET2

/* IPIC Interrupt Vectors */

#define INUM_ERR                0
#define INUM_DMAC               3
#define INUM_MSIR1              4
#define INUM_UART1              9
#define INUM_I2C1               14
#define INUM_I2C2               15
#define INUM_SPI                16
#define INUM_IRQ1               17
#define INUM_IRQ2               18
#define INUM_IRQ3               19
#define INUM_QE_HIGH            32
#define INUM_QE_LOW             33
#define INUM_USB_DR             38
#define INUM_SDHC               42
#define INUM_IRQ0               48
#define INUM_RTC_SEC            64
#define INUM_PIT                65
#define INUM_PCI                66
#define INUM_RTC_ALR            68
#define INUM_SBA                70
#define INMU_DMAC2              71
#define INUM_GTM4               72
#define INUM_QE_PORT            74
#define INUM_GPIO               75
#define INUM_DDR                76
#define INUM_LBC                77
#define INUM_GTM2               78
#define INUM_PMC                80
#define INUM_GTM3               84
#define INUM_GTM1               90
#define INUM_DMAC_ERR           94

/* QUICC Engine */

#define QUICC_ENGINE_BA                 0x100000
#define QE_MURAM_BA                     0x10000
#define QE_MURAM_POOL_BA                (QUICC_ENGINE_BASE + QE_MURAM_BA)
#define QE_SDMA_BASE                    (QUICC_ENGINE_BA + 0x4000)
#define QE_SDMR(base)                   (CAST(VUINT32 *)((base) + QE_SDMA_BASE + 0x04))

/* QE interrupt vectors */

#define UCC1_BASE                       0x2000
#define UCC2_BASE                       0x3000
#define UCC3_BASE                       0x2200
#define QE_UCC1                         0x1
#define INUM_MAX                        128
#define IVEC_MAX                        128

#define QE_SDEBCR_BA_MASK               0x01FFFFFF
#define QE_SDMR_CEN_SHIFT               13
#define QE_SDMR_GLB_1_MSK               0x80000000

/* CMXUCR registers */

#define QE_CMXUCR1_UCC1_RX_DIS          0x00000000
#define QE_CMXUCR1_UCC1_RX_BRG1         0x00100000
#define QE_CMXUCR1_UCC1_RX_BRG2         0x00200000
#define QE_CMXUCR1_UCC1_RX_BRG7         0x00300000
#define QE_CMXUCR1_UCC1_RX_BRG8         0x00400000
#define QE_CMXUCR1_UCC1_RX_CLK9         0x00500000
#define QE_CMXUCR1_UCC1_RX_CLK10        0x00600000
#define QE_CMXUCR1_UCC1_RX_CLK11        0x00700000
#define QE_CMXUCR1_UCC1_RX_CLK12        0x00800000
#define QE_CMXUCR1_UCC1_RX_CLK15        0x00900000
#define QE_CMXUCR1_UCC1_RX_CLK16        0x00A00000
#define QE_CMXUCR1_UCC1_TX_DIS          0x00000000
#define QE_CMXUCR1_UCC1_TX_BRG1         0x00010000
#define QE_CMXUCR1_UCC1_TX_BRG2         0x00020000
#define QE_CMXUCR1_UCC1_TX_BRG7         0x00030000
#define QE_CMXUCR1_UCC1_TX_BRG8         0x00040000
#define QE_CMXUCR1_UCC1_TX_CLK9         0x00050000
#define QE_CMXUCR1_UCC1_TX_CLK10        0x00060000
#define QE_CMXUCR1_UCC1_TX_CLK11        0x00070000
#define QE_CMXUCR1_UCC1_TX_CLK12        0x00080000
#define QE_CMXUCR1_UCC1_TX_CLK15        0x00090000
#define QE_CMXUCR1_UCC1_TX_CLK16        0x000A0000

/* CPM mux FCC clock route register */

#define QE_CMXGCR_MII_ENET_MNG_MASK     0x00007000
#define QE_CMXGCR_MII_ENET_MNG_SHIFT    12
#define QE_CMXGCR_MEM_MASK              0x00007000

/* QE mux UCC clock route registers */

#define QE_MUX                          (QUICC_ENGINE_BA + 0x400)
#define QE_CMXGCR(base)                 (CAST(VUINT32 *)((base) + QE_MUX ))
#define QE_CMXUCR1(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x10))
#define QE_CMXUCR2(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x14))
#define QE_CMXUCR3(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x18))
#define QE_CMXUCR4(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x1C))
#define QE_CMXUPCR(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x22))

/* DUART */

#define DUART_REG_ADDR_INTERVAL         1

/* NAND Flash */

#define NAND_BUFFER_SIZE                0x8000
#define NAND_PAGE_SIZE                  2048
#define NAND_PAGE_PER_BLOCK             64
#ifdef BOOT_FROM_NAND
#   define NAND_BANK                    0
#else /* BOOT_FROM_NAND */
#   define NAND_BANK                    1
#endif /* BOOT_FROM_NAND */

/* I2C Controller Memory Map */

#define M8313_I2C1_BASE                 0x03000
#define M8313_I2C2_BASE                 0x03100

#define I2C1ADR                         0x03000
#define I2C1FDR                         0x03004
#define I2C1ICR                         0x03008
#define I2C1ISR                         0x0300c
#define I2C1IDR                         0x03010
#define I2C1DFSRR                       0x03014

#define I2C2ADR                         0x03100
#define I2C2FDR                         0x03104
#define I2C2ICR                         0x03108
#define I2C2ISR                         0x0310c
#define I2C2IDR                         0x03110
#define I2C2DFSRR                       0x03114

/* 24LC256 EEPROM addr */

#define I2C_EEPROM_ADDR                 0x50

/* PCA 9536 I2C addr */

#define I2C_PCA9536_ADDR                0x41
#define I2C_PCA9536_DREG                0x01
#define I2C_PCA9536_DREG_INIT           0x04


/* processor and version */

#define PVR_E300C3                      0x80850000
#define SVR_MPC8306                     0x81100211
#define SVR_MPC8306S                    0x81100000
#define SVR_MPC8309                     0x81100011

/* System Configuration Register Memory Map */

#define M83XX_SGPRL                     (CAST(VUINT32 *)(SCCFG_REG_BA + 0x00))
#define M83XX_SGPRH                     (CAST(VUINT32 *)(SCCFG_REG_BA + 0x04))
#define M83XX_SPRIDR                    (CAST(VUINT32 *)(SCCFG_REG_BA + 0x08))
#define M83XX_SPCR                      (CAST(VUINT32 *)(SCCFG_REG_BA + 0x10))
#define M83XX_SICRL                     (CAST(VUINT32 *)(SCCFG_REG_BA + 0x14))
#define M83XX_SICRH                     (CAST(VUINT32 *)(SCCFG_REG_BA + 0x18))
#define M83XX_PECR1                     (CAST(VUINT32 *)(SCCFG_REG_BA + 0x40))
#define M83XX_SDHCCR                    (CAST(VUINT32 *)(SCCFG_REG_BA + 0x44))

#define M83XX_SPCR_INIT                 0x00c00000
#define M83XX_SDHCCR_INIT               0x02000000

/* System arbiter register */

#define M83XX_ACR                       (CAST(VUINT32 *)(ACR_BASE + 0x0))
#define M83XX_ACR_INIT                  0x00030300

/* eSDHC bit define */

#define SNOOP_ENABLE                    0x02000000
#define M83XX_SPCR_TBEN                 0x00400000
#define USB_PIN_MASK_SHIFT              18
#define USB_PIN_MASK                    (0x03 << USB_PIN_MASK_SHIFT)

/* DDR Memory Controller Memory Map */

#define CS0_BNDS                        0x000
#define CS1_BNDS                        0x008
#define CS2_BNDS                        0x010
#define CS3_BNDS                        0x018
#define CS0_CONFIG                      0x080
#define CS1_CONFIG                      0x084
#define CS2_CONFIG                      0x088
#define CS3_CONFIG                      0x08C

#define TIMING_CFG_0                    0x104
#define TIMING_CFG_1                    0x108
#define TIMING_CFG_2                    0x10C
#define TIMING_CFG_3                    0x100

#define DDR_SDRAM_CFG                   0x110
#define DDR_SDRAM_CFG_2                 0x114
#define DDR_SDRAM_MODE_CFG              0x118
#define DDR_SDRAM_MODE_CFG_2            0x11C
#define DDR_SDRAM_INTERVAL              0x124
#define DDR_SDRAM_INIVALUE              0x128
#define DDR_SDRAM_CLK_CNTRL             0x130

#define DDR_DATA_ERR_INJECT_HI          0xe00
#define DDR_DATA_ERR_INJECT_LO          0xe04
#define DDR_ECC_ERR_INJECT              0xe08
#define DDR_CAPTURE_DATA_HI             0xe20
#define DDR_CAPTURE_DATA_LO             0xe24
#define DDR_CAPTURE_ECC                 0xe28
#define DDR_ERR_DETECT                  0xe40
#define DDR_ERR_DISABLE                 0xe44
#define DDR_ERR_INT_EN                  0xe48
#define DDR_CAPTURE_ATTRIBUTES          0xe4c
#define DDR_CAPTURE_ADDRESS             0xe50
#define DDR_ERR_SBE                     0xe58

/* Clock Configuration Registers Memory Map */

#define M83XX_SPMR                     (CAST(VUINT32 *)(CLKM_REG_BASE + 0x00))
#define M83XX_OCCR                     (CAST(VUINT32 *)(CLKM_REG_BASE + 0x04))
#define M83XX_SCCR                     (CAST(VUINT32 *)(CLKM_REG_BASE + 0x08))

#define M83XX_SPMR_SPMF_VAL(x)         ((x >> 24) & 0xf)
#define M83XX_SPMR_CLK_DIV(x)          ((x >> 23) & 0x1)
#define M83XX_SPMR_CLK_PLL(x)          ((x >> 16) & 0x7f)

#define SDHC_CLK_RATIO_SHIFT           26
#define SDHC_CLK_RATIO_MASK            (0x03 << SDHC_CLK_RATIO_SHIFT)
#define SDHC_CLK_RATIO                 ((*M83XX_SCCR & SDHC_CLK_RATIO_MASK) >> \
                                        SDHC_CLK_RATIO_SHIFT)
#define USB_CLK_RATIO_SHIFT            22
#define USB_CLK_RATIO_MASK             (0x03 << USB_CLK_RATIO_SHIFT)
#define USB_CLK_RATIO                  ((*M83XX_SCCR & USB_CLK_RATIO_MASK) >>   \
                                        USB_CLK_RATIO_SHIFT)
#define PCIEX_CLK_RATIO_SHIFT          20
#define PCIEX_CLK_MASK                 (0x03 << PCIEX_CLK_RATIO_SHIFT)
#define PCIEX_CLK_ENABLE               (0x01 << PCIEX_CLK_RATIO_SHIFT)

/* USB Interface Memory Map */

/* Little-endian access registers */

#define M83XX_CAPLENGTH                (CAST(VUINT8  *)(USB_DR_BASE + 0x100))
#define M83XX_HCIVERSION               (CAST(VUINT16 *)(USB_DR_BASE + 0x102))
#define M83XX_HCSPARAMS_L              (CAST(VUINT32 *)(USB_DR_BASE + 0x104))
#define M83XX_HCSPARAMS_H              (CAST(VUINT32 *)(USB_DR_BASE + 0x108))

#define M83XX_USBCMD                   (CAST(VUINT32 *)(USB_DR_BASE + 0x140))
#define M83XX_USBSTS                   (CAST(VUINT32 *)(USB_DR_BASE + 0x144))
#define M83XX_USBINTR                  (CAST(VUINT32 *)(USB_DR_BASE + 0x148))
#define M83XX_FRINDEX                  (CAST(VUINT32 *)(USB_DR_BASE + 0x14c))
#define M83XX_PERIODICLISTBASE         (CAST(VUINT32 *)(USB_DR_BASE + 0x154))
#define M83XX_ASYNCLISTADDR            (CAST(VUINT32 *)(USB_DR_BASE + 0x158))
#define M83XX_ASYNCTTSTS               (CAST(VUINT32 *)(USB_DR_BASE + 0x15c))
#define M83XX_BURSTSIZE                (CAST(VUINT32 *)(USB_DR_BASE + 0x160))
#define M83XX_TXTTFILLTUNING_L         (CAST(VUINT32 *)(USB_DR_BASE + 0x164))
#define M83XX_TXTTFILLTUNING_H         (CAST(VUINT32 *)(USB_DR_BASE + 0x168))
#define M83XX_CONFIGFLAG               (CAST(VUINT32 *)(USB_DR_BASE + 0x180))
#define M83XX_PORTSC1                  (CAST(VUINT32 *)(USB_DR_BASE + 0x184))
#define M83XX_PORTSC2                  (CAST(VUINT32 *)(USB_DR_BASE + 0x188))
#define M83XX_USBMODE                  (CAST(VUINT32 *)(USB_DR_BASE + 0x1a8))

/* Big-endian access registers */

#define M83XX_SNOOP1                   (CAST(VUINT32 *)(USB_DR_BASE + 0x400))
#define M83XX_SNOOP2                   (CAST(VUINT32 *)(USB_DR_BASE + 0x404))
#define M83XX_AGE_CNT_THRESH           (CAST(VUINT32 *)(USB_DR_BASE + 0x408))
#define M83XX_PRI_CTRL                 (CAST(VUINT32 *)(USB_DR_BASE + 0x40c))
#define M83XX_SI_CTRL                  (CAST(VUINT32 *)(USB_DR_BASE + 0x410))
#define M83XX_CONTROL                  (CAST(VUINT32 *)(USB_DR_BASE + 0x500))

/* CONTROL Register bits */

#define ULPI_INT_EN                    (1 << 0)
#define WU_INT_EN                      (1 << 1)
#define USB_EN                         (1 << 2)
#define LSF_EN                         (1 << 3)
#define KEEP_OTG_ON                    (1 << 4)
#define OTG_PORT                       (1 << 5)
#define REFSEL_12MHZ                   (0 << 6)
#define REFSEL_16MHZ                   (1 << 6)
#define REFSEL_48MHZ                   (2 << 6)
#define PLL_RESET                      (1 << 8)
#define UTMI_PHY_EN                    (1 << 9)
#define PHY_CLK_SEL_UTMI               (0 << 10)
#define PHY_CLK_SEL_ULPI               (1 << 10)
#define CLKIN_SEL_USB_CLK              (0 << 11)
#define CLKIN_SEL_USB_CLK2             (1 << 11)
#define CLKIN_SEL_SYS_CLK              (2 << 11)
#define CLKIN_SEL_SYS_CLK2             (3 << 11)
#define RESERVED_18                    (0 << 13)
#define RESERVED_17                    (0 << 14)
#define RESERVED_16                    (0 << 15)
#define WU_INT                         (1 << 16)
#define PHY_CLK_VALID                  (1 << 17)

/* CMD Register bits */

#define USB_RUN                        (1 << 0)

/* Local Access Register Memory Map */

#define LBLAWBARn(n)                   (CAST(VUINT32 *)(IMMR_BASE + 0x20 + n*0x8))
#define LBLAWARn(n)                    (CAST(VUINT32 *)(IMMR_BASE + 0x24 + n*0x8))
#define M83XX_PCILAWBARn(base,n)       (CAST(VUINT32 *)((base) + 0x60 + (n*0x8)))
#define M83XX_PCILAWARn(base,n)        (CAST(VUINT32 *)((base) + 0x64 + (n*0x8)))
#define DDRLAWBARn(n)                  (CAST(VUINT32 *)(IMMR_BASE + 0xA0 + n*0x8))
#define DDRLAWARn(n)                   (CAST(VUINT32 *)(IMMR_BASE + 0xA4 + n*0x8))
#define DDRCDR                         (CAST(VUINT32 *)(IMMR_BASE + 0x128))

#define LAWBAR_ADRS_SHIFT              0
#define LAWAR_ENABLE                   0x80000000

/* LAWAR SIZE Settings */

#define LAWAR_SIZE_4KB                 0x0000000B
#define LAWAR_SIZE_8KB                 0x0000000C
#define LAWAR_SIZE_16KB                0x0000000D
#define LAWAR_SIZE_32KB                0x0000000E
#define LAWAR_SIZE_64KB                0x0000000F
#define LAWAR_SIZE_128KB               0x00000010
#define LAWAR_SIZE_256KB               0x00000011
#define LAWAR_SIZE_512KB               0x00000012
#define LAWAR_SIZE_1MB                 0x00000013
#define LAWAR_SIZE_2MB                 0x00000014
#define LAWAR_SIZE_4MB                 0x00000015
#define LAWAR_SIZE_8MB                 0x00000016
#define LAWAR_SIZE_16MB                0x00000017
#define LAWAR_SIZE_32MB                0x00000018
#define LAWAR_SIZE_64MB                0x00000019
#define LAWAR_SIZE_128MB               0x0000001A
#define LAWAR_SIZE_256MB               0x0000001B
#define LAWAR_SIZE_512MB               0x0000001C
#define LAWAR_SIZE_1GB                 0x0000001D
#define LAWAR_SIZE_2GB                 0x0000001E

/* Nand controller register define */

#define SPECIAL_OP_INIT_REG            (CCSBAR + 0x5090)
#define TRANSFER_ERROR_ATT_REG         (CCSBAR + 0x50BC)
#define TRANSFER_ERROR_STU_REG         (CCSBAR + 0x50B0)
#define FCM_DATA_REG                   (CCSBAR + 0x5088)
#define FLASH_MODE_REG                 (CCSBAR + 0x50E0)
#define FLASH_INSTRUCTION_REG          (CCSBAR + 0x50E4)
#define FLASH_COMMAND_REG              (CCSBAR + 0x50E8)
#define FLASH_BLOCKADDR_REG            (CCSBAR + 0x50EC)
#define FLASH_PAGEADDR_REG             (CCSBAR + 0x50F0)
#define FLASH_BYTECOUNT_REG            (CCSBAR + 0x50F4)

#define FCM_SMALLBUF_SIZE              1024
#define FCM_LARBUF_SIZE                4096
#define FCM_SMALLBUF_MASK              0x7
#define FCM_LARBUF_MASK                0x1

/* PCI Express Local Access Window Attributes Register bit */

#define PCI_LAWAR_ENABLE               0x80000000
#define PCI_LAWAR_SIZE_256MB           0x0000001B
#define PCI_LAWAR_SIZE_512MB           0x0000001C

/*
 * Enhanced Local Bus Controller Registers - unmentioned bits are reserved
 *
 * BRx 0-16 Base Address
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
 *
 */

#define ELBC_BR0                (ELBC_BASE + 0x00)
#define ELBC_OR0                (ELBC_BASE + 0x04)
#define ELBC_BR1                (ELBC_BASE + 0x08)
#define ELBC_OR1                (ELBC_BASE + 0x0c)
#define ELBC_BR2                (ELBC_BASE + 0x10)
#define ELBC_OR2                (ELBC_BASE + 0x14)
#define ELBC_BR3                (ELBC_BASE + 0x18)
#define ELBC_OR3                (ELBC_BASE + 0x1c)
#define ELBC_MAR                (ELBC_BASE + 0x68)
#define ELBC_MAMR               (ELBC_BASE + 0x70)
#define ELBC_MBMR               (ELBC_BASE + 0x74)
#define ELBC_MCMR               (ELBC_BASE + 0x78)
#define ELBC_MRTPR              (ELBC_BASE + 0x84)
#define ELBC_MDR                (ELBC_BASE + 0x88)
#define ELBC_LBCR               (ELBC_BASE + 0xD0)
#define ELBC_LCRR               (ELBC_BASE + 0xD4)

/* Option Registers Bit */

#define OR_SIZE_4G              0x00000000
#define OR_SIZE_2G              0x80000000
#define OR_SIZE_1G              0xc0000000
#define OR_SIZE_512M            0xe0000000
#define OR_SIZE_256M            0xf0000000
#define OR_SIZE_128M            0xf8000000
#define OR_SIZE_64M             0xfc000000
#define OR_SIZE_32M             0xfe000000
#define OR_SIZE_16M             0xff000000
#define OR_SIZE_8M              0xff800000
#define OR_SIZE_4M              0xffc00000
#define OR_SIZE_2M              0xffe00000
#define OR_SIZE_1M              0xfff00000
#define OR_SIZE_512K            0xfff80000
#define OR_SIZE_256K            0xfffc0000
#define OR_SIZE_128K            0xfffe0000
#define OR_SIZE_64K             0xffff0000
#define OR_SIZE_32K             0xffff8000

/* Clock Ratio Register Bit */

#define CLKDIV_MASK             0x1f

/* PCI Inbound Windows (PCI/PCIe based addresses) */

#define PCI_MSTR_MEM_BUS        0x00000000
#define PCI_MSTR_MEM_SIZE       LOCAL_MEM_SIZE
#define PCI_PIMMR_BASE_ADRS     0x40000000  /* fixed 1GB space */

/* PCIe Local Access Windows Configuration */

#define PCI_LAW_BASE            0x80000000
#define PCI_LAW_SIZE            0x10000000  /* 512MB */

/* PCI based addresses */

#define PCI_MEM_ADRS            0x80000000
#define PCI_MEM_SIZE            0x04000000  /* 64MB  */
#define PCI_MEMIO_ADRS          0x84000000
#define PCI_MEMIO_SIZE          0x04000000  /* 64MB  */
#define PCI_IO_ADRS             0x88000000
#define PCI_IO_SIZE             0x04000000  /* 64MB  */

/* PCI General control register */

#define PCI_GCR_OFFSET          0x8520
#define PCI_GCR_RESET_BIT       0x00000001

/* latency timer value, 64 PCI clocks */

#define PCI_LAT_TIMER           0x80

/* QE interrupt source for UCC1,UCC2,UCC3 */

#define QE_IC_INTR_UCC_1        32
#define QE_IC_INTR_UCC_2        33
#define QE_IC_INTR_UCC_3        34

/* QE mux UCC clock route registers */

#define QE_CMXGCR_MEM_MASK      0x00007000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCmpc830xmitxh */
