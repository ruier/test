/* fsl_rdb8315.h - Freescale MPC8315ERDB board header */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,08dec09,x_z code clean.
01a,05may08,z_l Created.
*/

/*
DESCRIPTION
This file contains I/O addresses and related constants for the
Freescale MPC8315ERDB board.
*/

#ifndef __INCfsl_rdb8315h
#define __INCfsl_rdb8315h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef  _ASMLANGUAGE
#   define CAST(x)
#else /* _ASMLANGUAGE */
    typedef volatile UCHAR  VCHAR;      /* shorthand for volatile UCHAR */
    typedef volatile INT32  VINT32;     /* volatile unsigned word */
    typedef volatile INT16  VINT16;     /* volatile unsigned halfword */
    typedef volatile INT8   VINT8;      /* volatile unsigned byte */
    typedef volatile UINT32 VUINT32;    /* volatile unsigned word */
    typedef volatile UINT16 VUINT16;    /* volatile unsigned halfword */
    typedef volatile UINT8  VUINT8;     /* volatile unsigned byte */
#   define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

/* PVR and SVR */

#define PVR_E300C3                  0x80850000
#define PVR_E300C3_MASK             0xFFFF0000
#define SVR_MPC8315E                0x80B40000
#define SVR_MPC8315                 0x80B50000

/* IMMR Base Address */

#define CCSBAR_INIT                 0xFF400000
#define CCSBAR                      0xE0000000
#define CCSBAR_SIZE                 0x100000 /* 1MB */

/* IMMR Memory Map */

#define SCCFG_REG_BASE              0x00100
#define WDT_REG_BASE                0x00200
#define QUICC_RTC_BASE              0x00300
#define QUICC_PIT_BASE              0x00400
#define QUICC_GTM1_BASE             0x00500
#define QUICC_GTM2_BASE             0x00600
#define SIC_REG_BASE                0x00700
#define ARBITER_REG_BASE            0x00800
#define RESETM_REG_BASE             0x00900
#define CLKM_REG_BASE               0x00A00
#define DDR_REG_BASE                0x02000
#define M8313_I2C1_BASE             0x03000
#define M8313_I2C2_BASE             0x03100 /* reserved */
#define DUART1_REG_BASE             0x04500
#define DUART2_REG_BASE             0x04600
#define LBC_REG_BA                  0x05000
#define PCI_REG_BASE                0x8000
#define DMA_REG_BASE                0x08100
#define PCICFG_REG_BASE             0x08300
#define PCIEX1_REGBASE              0x09000
#define PCIEX2_REGBASE              0x0A000
#define SEC_REG_BASE                0x101B4
#define USBDR_REG_BASE              0x23000
#define SRDSPHY1_BASE               0xE3000

#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <drv/mem/quiccLbc.h>

/* System Configuration Control */

#define SCCFG_REG_BA                (CCSBAR + SCCFG_REG_BASE)

#define M83XX_SGPRL                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x00))
#define M83XX_SGPRH                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x04))
#define M83XX_SPRIDR                (CAST(VUINT32 *)(SCCFG_REG_BA + 0x08))

#define M83XX_SPCR                  (CAST(VUINT32 *)(SCCFG_REG_BA + 0x10 ))
#define M83XX_SPCR_OPT              0x00800000 /* performance enhancement by  */
                                               /* speculative reading enabled */
#define M83XX_SPCR_TBEN             0x00400000 /* time base enabled           */

#define M83XX_SICRL                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x14))
#define M83XX_SICRH                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x18))

#define M83XX_DDRCDR                (CAST(VUINT32 *)(SCCFG_REG_BA + 0x28))

#define M83XX_PECR1                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x40))
#define M83XX_PECR2                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x44))
#define M83XX_PECR2_RC              0x00008000 /* PCI Express root complex  */
#define M83XX_PECR2_LINK_RST        0x80000000 /* Link reset negated        */
#define M83XX_PECR2_CBRST           0x40000000 /* CSB bridge reset negated  */
#define M83XX_PECR2_CSR_RST         0x20000000 /* CSR reset negated         */

/* Watch Dog Registers */

#define WDT_REG_BA                  (CCSBAR + WDT_REG_BASE)

#define M83XX_SWCRR                 (CAST(VUINT32 *)(WDT_REG_BA + 0x04))
#define M83XX_SWCNR                 (CAST(VUINT32 *)(WDT_REG_BA + 0x08))
#define M83XX_SWSRR                 (CAST(VUINT16 *)(WDT_REG_BA + 0x0E))

/* Clock Module */

#define CLKM_REG_BA                 (CCSBAR + CLKM_REG_BASE)

#define M83XX_SPMR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x00))
#define M83XX_SPMR_LBIU_VAL(val)    (val >> 31)
#define M83XX_SPMR_DDR_VAL(val)     ((val >> 30) & 0x1)
#define M83XX_SPMR_SPMF_VAL(val)    ((val >> 24) & 0xf)
#define M83XX_SPMR_CLK_DIV(val)     ((val >> 23) & 0x1)
#define M83XX_SPMR_CLK_PLL(val)     ((val >> 16) & 0x7f)

#define M83XX_OCCR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x04))

#define M83XX_SCCR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x08))
#define M83XX_SCCR_RES_MSK          0xF3F10000 /* reserved field Mask */

/* Arbiter registers */

#define ARBITER_REG_BA              (CCSBAR + ARBITER_REG_BASE)

#define M83XX_ACR                   (CAST(VUINT32 *)(ARBITER_REG_BA + 0x00))
#define M83XX_ATR                   (CAST(VUINT32 *)(ARBITER_REG_BA + 0x04))
#define M83XX_AER                   (CAST(VUINT32 *)(ARBITER_REG_BA + 0x0c))
#define M83XX_AIDR                  (CAST(VUINT32 *)(ARBITER_REG_BA + 0x10))
#define M83XX_AMR                   (CAST(VUINT32 *)(ARBITER_REG_BA + 0x14))
#define M83XX_AEATR                 (CAST(VUINT32 *)(ARBITER_REG_BA + 0x18))
#define M83XX_AEADR                 (CAST(VUINT32 *)(ARBITER_REG_BA + 0x1c))
#define M83XX_AERR                  (CAST(VUINT32 *)(ARBITER_REG_BA + 0x20))

/* Reset Configuration Module */

#define RESETM_REG_BA               (CCSBAR + RESETM_REG_BASE)

#define M83XX_RCWLR                 (CAST(VUINT32 *)(RESETM_REG_BA + 0x00))
#define M83XX_RCWHR                 (CAST(VUINT32 *)(RESETM_REG_BA + 0x04))
#define M83XX_RSR                   (CAST(VUINT32 *)(RESETM_REG_BA + 0x10))
#define M83XX_RMR                   (CAST(VUINT32 *)(RESETM_REG_BA + 0x14))
#define M83XX_RPR                   (CAST(VUINT32 *)(RESETM_REG_BA + 0x18))
#define M83XX_RCR                   (CAST(VUINT32 *)(RESETM_REG_BA + 0x1c))
#define M83XX_RCER                  (CAST(VUINT32 *)(RESETM_REG_BA + 0x20))

/* USB DR (Host) registers */

#define USBDR_REG_BA                (CCSBAR + USBDR_REG_BASE)

/* These are little-endian */

#define M83XX_CAPLENGTH             (CAST(VUINT8  *)(USBDR_REG_BA + 0x100))
#define M83XX_HCIVERSION            (CAST(VUINT16 *)(USBDR_REG_BA + 0x102))
#define M83XX_HCSPARAMS_L           (CAST(VUINT32 *)(USBDR_REG_BA + 0x104))
#define M83XX_HCSPARAMS_H           (CAST(VUINT32 *)(USBDR_REG_BA + 0x108))

#define M83XX_USBCMD                (CAST(VUINT32 *)(USBDR_REG_BA + 0x140))
#define M83XX_USBSTS                (CAST(VUINT32 *)(USBDR_REG_BA + 0x144))
#define M83XX_USBINTR               (CAST(VUINT32 *)(USBDR_REG_BA + 0x148))
#define M83XX_FRINDEX               (CAST(VUINT32 *)(USBDR_REG_BA + 0x14c))
#define M83XX_PERIODICLISTBASE      (CAST(VUINT32 *)(USBDR_REG_BA + 0x154))
#define M83XX_ASYNCLISTADDR         (CAST(VUINT32 *)(USBDR_REG_BA + 0x158))
#define M83XX_ASYNCTTSTS            (CAST(VUINT32 *)(USBDR_REG_BA + 0x15c))
#define M83XX_BURSTSIZE             (CAST(VUINT32 *)(USBDR_REG_BA + 0x160))
#define M83XX_TXTTFILLTUNING_L      (CAST(VUINT32 *)(USBDR_REG_BA + 0x164))
#define M83XX_TXTTFILLTUNING_H      (CAST(VUINT32 *)(USBDR_REG_BA + 0x168))
#define M83XX_CONFIGFLAG            (CAST(VUINT32 *)(USBDR_REG_BA + 0x180))
#define M83XX_PORTSC1               (CAST(VUINT32 *)(USBDR_REG_BA + 0x184))
#define M83XX_PORTSC2               (CAST(VUINT32 *)(USBDR_REG_BA + 0x188))
#define M83XX_USBMODE               (CAST(VUINT32 *)(USBDR_REG_BA + 0x1a8))

/* These are big-endian */

#define M83XX_SNOOP1                (CAST(VUINT32 *)(USBDR_REG_BA + 0x400))
#define M83XX_SNOOP2                (CAST(VUINT32 *)(USBDR_REG_BA + 0x404))
#define M83XX_AGE_CNT_THRESH        (CAST(VUINT32 *)(USBDR_REG_BA + 0x408))
#define M83XX_PRI_CTRL              (CAST(VUINT32 *)(USBDR_REG_BA + 0x40c))
#define M83XX_SI_CTRL               (CAST(VUINT32 *)(USBDR_REG_BA + 0x410))
#define M83XX_CONTROL               (CAST(VUINT32 *)(USBDR_REG_BA + 0x500))

/* Local address windows */

#define M83XX_LBLAWBARn(n)          (CAST(VUINT32 *)(CCSBAR + 0x20 + (n * 0x8)))
#define M83XX_LBLAWARn(n)           (CAST(VUINT32 *)(CCSBAR + 0x24 + (n * 0x8)))
#define M83XX_PCILAWBARn(n)         (CAST(VUINT32 *)(CCSBAR + 0x60 + (n * 0x8)))
#define M83XX_PCILAWARn(n)          (CAST(VUINT32 *)(CCSBAR + 0x64 + (n * 0x8)))
#define M83XX_PCIEXLAWBARn(n)       (CAST(VUINT32 *)(CCSBAR + 0x80 + (n * 0x8)))
#define M83XX_PCIEXLAWARn(n)        (CAST(VUINT32 *)(CCSBAR + 0x84 + (n * 0x8)))
#define M83XX_DDRLAWBARn(n)         (CAST(VUINT32 *)(CCSBAR + 0xA0 + (n * 0x8)))
#define M83XX_DDRLAWARn(n)          (CAST(VUINT32 *)(CCSBAR + 0xA4 + (n * 0x8)))

#define LAWBAR_ADRS_SHIFT           0
#define LAWAR_ENABLE                0x80000000

/* LAWAR SIZE Settings */

#define LAWAR_SIZE_4KB              0x0000000B
#define LAWAR_SIZE_8KB              0x0000000C
#define LAWAR_SIZE_16KB             0x0000000D
#define LAWAR_SIZE_32KB             0x0000000E
#define LAWAR_SIZE_64KB             0x0000000F
#define LAWAR_SIZE_128KB            0x00000010
#define LAWAR_SIZE_256KB            0x00000011
#define LAWAR_SIZE_512KB            0x00000012
#define LAWAR_SIZE_1MB              0x00000013
#define LAWAR_SIZE_2MB              0x00000014
#define LAWAR_SIZE_4MB              0x00000015
#define LAWAR_SIZE_8MB              0x00000016
#define LAWAR_SIZE_16MB             0x00000017
#define LAWAR_SIZE_32MB             0x00000018
#define LAWAR_SIZE_64MB             0x00000019
#define LAWAR_SIZE_128MB            0x0000001A
#define LAWAR_SIZE_256MB            0x0000001B
#define LAWAR_SIZE_512MB            0x0000001C
#define LAWAR_SIZE_1GB              0x0000001D
#define LAWAR_SIZE_2GB              0x0000001E

/* PCI Window SIZE  */

#define PCI_SIZE_4KB                0x0000000B
#define PCI_SIZE_8KB                0x0000000C
#define PCI_SIZE_16KB               0x0000000D
#define PCI_SIZE_32KB               0x0000000E
#define PCI_SIZE_64KB               0x0000000F
#define PCI_SIZE_128KB              0x00000010
#define PCI_SIZE_256KB              0x00000011
#define PCI_SIZE_512KB              0x00000012
#define PCI_SIZE_1MB                0x00000013
#define PCI_SIZE_2MB                0x00000014
#define PCI_SIZE_4MB                0x00000015
#define PCI_SIZE_8MB                0x00000016
#define PCI_SIZE_16MB               0x00000017
#define PCI_SIZE_32MB               0x00000018
#define PCI_SIZE_64MB               0x00000019
#define PCI_SIZE_128MB              0x0000001A
#define PCI_SIZE_256MB              0x0000001B
#define PCI_SIZE_512MB              0x0000001C
#define PCI_SIZE_1GB                0x0000001D
#define PCI_SIZE_2GB                0x0000001E

/* PCI Outbound/Inbound Comparison mask register defines */

#define PCI_WINDOW_ENABLE_BIT       0x80000000
#define PCI_ATTR_BS_BIT             0x40000000
#define PCI_OUT_ATTR_RTT_MEM        0x00040000
#define PCI_OUT_ATTR_RTT_IO         0x00080000
#define PCI_OUT_ATTR_WTT_MEM        0x00004000
#define PCI_OUT_ATTR_WTT_IO         0x00008000
#define PCI_ATTR_WS_4K              0x0000000B
#define PCI_ATTR_WS_8K              0x0000000c
#define PCI_ATTR_WS_16K             0x0000000D
#define PCI_ATTR_WS_32K             0x0000000E
#define PCI_ATTR_WS_64K             0x0000000F
#define PCI_ATTR_WS_128K            0x00000010
#define PCI_ATTR_WS_256K            0x00000011
#define PCI_ATTR_WS_512K            0x00000012
#define PCI_ATTR_WS_1M              0x00000013
#define PCI_ATTR_WS_2M              0x00000014
#define PCI_ATTR_WS_4M              0x00000015
#define PCI_ATTR_WS_8M              0x00000016
#define PCI_ATTR_WS_16M             0x00000017
#define PCI_ATTR_WS_32M             0x00000018
#define PCI_ATTR_WS_64M             0x00000019
#define PCI_ATTR_WS_128M            0x0000001a
#define PCI_ATTR_WS_256M            0x0000001b
#define PCI_ATTR_WS_512M            0x0000001c
#define PCI_ATTR_WS_1G              0x0000001d
#define PCI_ATTR_WS_2G              0x0000001e
#define PCI_ATTR_WS_4G              0x0000001f
#define PCI_IN_ATTR_TGI_LM          0x00f00000
#define PCI_IN_ATTR_TGI_RIO         0x00c00000
#define PCI_IN_ATTR_RTT_RIO_READ    0x00040000

#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0x00007000

#define PCI_SNOOP_ENABLE            0x40000000
#define PCI_PREFETCHABLE            0x20000000

/* Offsets for DDR registers */

#define DDR_REG_BA                 (CCSBAR + DDR_REG_BASE)

#define CS0_BNDS                    0x000
#define CS1_BNDS                    0x008
#define CS2_BNDS                    0x010
#define CS3_BNDS                    0x018
#define CS0_CONFIG                  0x080
#define CS1_CONFIG                  0x084
#define CS2_CONFIG                  0x088
#define CS3_CONFIG                  0x08C

#define TIMING_CFG_0                0x104
#define TIMING_CFG_1                0x108
#define TIMING_CFG_2                0x10C
#define TIMING_CFG_3                0x100

#define DDR_SDRAM_CFG               0x110
#define DDR_SDRAM_CFG_2             0x114
#define DDR_SDRAM_MODE_CFG          0x118
#define DDR_SDRAM_MODE_CFG_2        0x11C
#define DDR_SDRAM_INTERVAL          0x124
#define DDR_SDRAM_CLK_CNTRL         0x130

#define DDR_DATA_ERR_INJECT_HI      0xE00
#define DDR_DATA_ERR_INJECT_LO      0xE04
#define DDR_ECC_ERR_INJECT          0xE08
#define DDR_CAPTURE_DATA_HI         0xE20
#define DDR_CAPTURE_DATA_LO         0xE24
#define DDR_CAPTURE_ECC             0xE28
#define DDR_ERR_DETECT              0xE40
#define DDR_ERR_DISABLE             0xE44
#define DDR_ERR_INT_EN              0xE48
#define DDR_CAPTURE_ATTRIBUTES      0xE4C
#define DDR_CAPTURE_ADDRESS         0xE50
#define DDR_ERR_SBE                 0xE58

/* SerDes PHY (PCI Express/SGMII) */

#define SRDSPHY1_BA(n)              (CCSBAR + SRDSPHY1_BASE + (n * 0x100))

#define SRDSnCR0(n)                 (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x0))
#define SRDSnCR1(n)                 (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x4))
#define SRDSnCR2(n)                 (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x8))
#define SRDSnCR3(n)                 (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0xC))
#define SRDSnCR4(n)                 (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x10))
#define SRDSnRSTCTLn(n)             (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x20))

#define SRDS_RSTCTL_RST             0x80000000

#define SRDS_RSTCTL_CNT1            0x00400000
#define SRDS_RSTCTL_CNT2            0x00040000
#define SRDS_RSTCTL_CNT3            0x00004000
#define SRDS_RSTCTL_CNT4            0x00000500

#define SRDS_CR0_RXEQA              0x10000000
#define SRDS_CR0_RXEQE              0x01000000
#define SRDS_CR0_TXEQA              0x00004000
#define SRDS_CR0_TXEQE              0x00000400
#define SRDS_CR0_IACCA              0x00000020
#define SRDS_CR0_IACCE              0x00000010

#define SRDS_CR2_PCICA              0x00000100

#define SRDS_CR3_SDFMA              0x01000000
#define SRDS_CR3_SDFME              0x00010000

#define SRDS_CR4_PROTA              0x00000100
#define SRDS_CR4_PROTE              0x00000001

/* PCI */

#define PCI_LAWAR_ENABLE            0x80000000
#define PCI_LAWAR_SIZE_256MB        0x0000001B
#define PCI_LAWAR_SIZE_512MB        0x0000001C

#define PCI_ATTR_IO_BIT             0x40000000
#define PCI_IN_ATTR_WS_256M         0x0000001B
#define PCI_OUT_ATTR_WS_128M        0x000F8000
#define PCI_OUT_ATTR_WS_64M         0x000FC000

/* Define clock speed and source */

#define SYS_CLK_FREQ                133000000 /* 133MHz */
#define OSCILLATOR_FREQ             66000000 /* 66MHz  */

/* SCCR */

#define SCCR_TSEC1CM                0x40000000 /* 1:1 */
#define SCCR_TSEC2CM                0x10000000 /* 1:1 */
#define SCCR_ENCCM                  0x02000000 /* 1:2 */
#define SCCR_USBDRCM                0x00400000 /* 1:2 */
#define SCCR_PCIEXP1CM              0x00100000 /* 1:1 */
#define SCCR_PCIEXP2CM              0x00040000 /* 1:1 */
#define SCCR_PCICM_EN               0x00010000
#define SCCR_SATA1CM                0x00001000 /* 1:1 */
#define SCCR_SATA2CM                0x00000400 /* 1:1 */

/* no off-board bus interface    */

#define BUS                         NONE

/* number of SIOs */

#define N_SIO_CHANNELS              2

/* DUART */

#define DUART1_BA                   (CCSBAR + DUART1_REG_BASE)
#define DUART2_BA                   (CCSBAR + DUART2_REG_BASE)
#define DUART_REG_ADDR_INTERVAL     1 /* duart vector register distance */

/* latency timer value, 64 PCI clocks */

#define PCI_LAT_TIMER               0x40
#define PCI_IRQ                     INUM_IRQ0

/* DS1339 RTC addr */

#define I2C_RTC_ADDR                0x68

/* USB */

#define USB_CRTL_UTMI_PHY_EN        0x00000200
#define USB_CRTL_REFSEL             0x00000040 /* 24MHz */
#define USB_CRTL_PHY_CLK_VALID      0x00020000
#define USB_MODE_CM                 0x00000003 /* host mode */
#define USB_BURSTSIZE_TX            0x00001000 /* 16 */
#define USB_BURSTSIZE_RX            0x00000010 /* 16 */
#define USB_SICTRL_ERR_DISABLE      0x00000008
#define USB_PRICTRL_PRILVL0         0x00000001
#define USB_AGE_CNT_THRESH          0x00000040
#define USB_CMD_RC_RUN              0x00000001
#define USB_STS_HCH                 0x00001000

/* interrupt vectors */

#define INUM_PEX1_CNT                1
#define INUM_PEX2_CNT                2
#define INUM_UART1                   9
#define INUM_UART2                  10
#define INUM_SEC                    11
#define INUM_I2C1                   14
#define INUM_I2C2                   15
#define INUM_SPI                    16
#define INUM_IRQ1                   17
#define INUM_IRQ2                   18
#define INUM_IRQ3                   19
#define INUM_IRQ4                   20
#define INUM_IRQ5                   21
#define INUM_IRQ6                   22
#define INUM_IRQ7                   23
#define INUM_TSEC2_ERR              37
#define INUM_TSEC2_RX               36
#define INUM_TSEC2_TX               35
#define INUM_TSEC1_ERR              34
#define INUM_TSEC1_RX               33
#define INUM_TSEC1_TX               32
#define INUM_USB2_DR                38
#define INUM_USB2_MPH               39
#define INUM_IRQ0                   48
#define INUM_RTC_SEC                64
#define INUM_PIT                    65
#define INUM_PCI1                   66
#define INUM_PCI2                   67
#define INUM_RTC_ALR                68
#define INUM_MU                     69
#define INUM_SBA                    70
#define INUM_DMA                    71
#define INUM_GTM4                   72
#define INUM_GTM8                   73
#define INUM_GPIO1                  74
#define INUM_GPIO2                  75
#define INUM_DDR                    76
#define INUM_LBC                    77
#define INUM_GTM2                   78
#define INUM_GTM6                   79
#define INUM_PMC                    80
#define INUM_GTM3                   84
#define INUM_GTM7                   85
#define INUM_GTM1                   90
#define INUM_GTM5                   91

#define INUM_MSIR1                  4
#define INUM_SATA1                  44
#define INUM_SATA2                  45
#define INUM_MSIR0                  67
#define INUM_MSIR2                  81
#define INUM_MSIR3                  82
#define INUM_MSIR4                  86
#define INUM_MSIR5                  87
#define INUM_MSIR6                  88
#define INUM_MSIR7                  89

#ifndef _ASMLANGUAGE

/* Clock configuration */

typedef struct
    {
    UINT32 csb;
    UINT32 lbiu;
    UINT32 ddr;
    UINT32 spmf;
    UINT32 clkDiv;
    UINT32 corePll;
    UINT32 coreFreq;
    } CLK_CONF;

typedef struct
    {
    UINT32 spmf;
    UINT32 corepll;
    UINT32 clkin;
    UINT32 csbfreq;
    UINT32 corefreq;
    } CLK_TABLE;

/* function declarations */

extern void     sysMsDelay(uint32_t);
extern void     sysDelay(void);
extern UINT32   vxHid2Get(void);
extern void     vxHid2Set(UINT32);
extern UINT32   vxSvrGet(void);
#endif /* !_ASMLANGUAGE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_rdb8315h */

