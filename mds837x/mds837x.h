/* mds837x.h - Motorola MPC837x MDS board header */

/*
* Copyright (c) 2005, 2008-2009 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01d,18dec09,x_z  code clean.
01c,18aug08,x_s  moved DUART macros from sysDuart.h.
01b,20feb08,x_s  update for vxbus interrupt controller driver.
01a,10jan05,dtr  adapted from ads834x.h/01d
*/

/*
This file contains I/O addresses and related constants for the
Freescale MPC837xE MDS board.
*/

#ifndef INCmds837xh
#define INCmds837xh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _ASMLANGUAGE

#   define CAST(x)

#else /* _ASMLANGUAGE */

typedef volatile UCHAR      VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile INT32      VINT32;     /* volatile unsigned word       */
typedef volatile INT16      VINT16;     /* volatile unsigned halfword   */
typedef volatile INT8       VINT8;      /* volatile unsigned byte       */
typedef volatile UINT32     VUINT32;    /* volatile unsigned word       */
typedef volatile UINT16     VUINT16;    /* volatile unsigned halfword   */
typedef volatile UINT8      VUINT8;     /* volatile unsigned byte       */

#   define CAST(x) (x)

#endif  /* _ASMLANGUAGE */

/* system & processor version */

#define PVR_E300C4                  0x80860000
#define PVR_E300C4_MASK             0xFFFF0000
#define SVR_MPC8379E                0x80C20000
#define SVR_MPC8379                 0x80C30000
#define SVR_MPC8378E                0x80C40000
#define SVR_MPC8378                 0x80C50000
#define SVR_MPC8377E                0x80C60000
#define SVR_MPC8377                 0x80C70000

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
#define I2C1_BASE                   0x03000
#define I2C2_BASE                   0x03100
#define DUART1_REG_BASE             0x04500
#define DUART2_REG_BASE             0x04600
#define LBC_REG_BA                  0x05000
#define PCI_REG_BASE                0x08000
#define DMA_REG_BASE                0x08100
#define PCICFG_REG_BASE             0x08300
#define PCIEX1_REG_BASE             0x09000
#define PCIEX2_REG_BASE             0x0A000
#define SEC_REG_BASE                0x101B4
#define SATA1_REG_BASE              0x18000
#define SATA2_REG_BASE              0x19000
#define SATA3_REG_BASE              0x1A000
#define SATA4_REG_BASE              0x1B000
#define USBDR_REG_BASE              0x23000
#define ETSEC1_REG_BASE             0x24000
#define ETSEC2_REG_BASE             0x25000
#define SRDSPHY1_BASE               0xE3000
#define SRDSPHY2_BASE               0xE3100

/*
 * Define the register definition and interrupt numbers based on
 * cpu varient
 */

#define PQ2PRO

#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <drv/mem/quiccLbc.h>

/* System Configuration Control */

#define SCCFG_REG_BA                (CCSBAR + SCCFG_REG_BASE)

#define M83XX_SGPRL                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x00))
#define M83XX_SGPRH                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x04))
#define M83XX_SPRIDR                (CAST(VUINT32 *)(SCCFG_REG_BA + 0x08))

#define M83XX_SPCR                  (CAST(VUINT32 *)(SCCFG_REG_BA + 0x10))
#define M83XX_SPCR_OPT              0x00800000 /* performance enhancement by  */
                                               /* speculative reading enabled */
#define M83XX_SPCR_TBEN             0x00400000 /* time base enabled           */

#define M83XX_SICRL                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x14))
#define M83XX_SICRL_USB_A           0xC0000000 /* GPIO/USB A            */
#define M83XX_SICRL_USB_A_USB       0x40000000 /* USB A                 */
#define M83XX_SICRL_USB_B           0x30000000 /* GPIO/USB B/SD         */
#define M83XX_SICRL_USB_B_USB       0x10000000 /* USB B                 */
#define M83XX_SICRL_USB_B_SD        0x20000000 /* SD                    */
#define M83XX_SICRL_UART            0x0C000000 /* UART/MSRCID/LSRCID    */
#define M83XX_SICRL_UART_MSRCID     0x01000000 /* MSRCID                */
#define M83XX_SICRL_UART_LSRCID     0x02000000 /* LSRCID                */

#define M83XX_SICRH                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x18))

#define M83XX_DDRCDR                (CAST(VUINT32 *)(SCCFG_REG_BA + 0x28))
#define M83XX_OBIR                  (CAST(VUINT32 *)(SCCFG_REG_BA + 0x30))
#define M83XX_OBIR_RESERVED         0x30000000
#define M83XX_OBIR_TSEC1OBI         0x03000000 /* TSEC1 output buffer         */
                                               /* impedance: 0 - 3.3 V 45 ohm */
#define M83XX_OBIR_TSEC1OBI_1       0x01000000 /* 1 -  2.5 V 45 ohm           */
#define M83XX_OBIR_TSEC2OBI         0x00300000 /* TSEC1 output buffer         */
                                               /* impedance: 0 - 3.3 V 45 ohm */
#define M83XX_OBIR_TSEC2OBI_1       0x00100000 /* 1 -  2.5 V 45 ohm           */
#define M83XX_OBIR_LOBI             0x00030000 /* Local bus output buffer     */
                                               /* impedance: 0 - 3.3 V 45 ohm */
                                               /* 1 -  2.5 V 45 ohm           */
                                               /* 2 -  1.8 V 40 ohm           */
#define M83XX_PECR1                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x40))
#define M83XX_PECR2                 (CAST(VUINT32 *)(SCCFG_REG_BA + 0x44))
#define M83XX_PECR_RC               0x00008000 /* PCI Express root complex  */
#define M83XX_PECR_LINK_RST         0x80000000 /* Link reset negated        */
#define M83XX_PECR_CBRST            0x40000000 /* CSB bridge reset negated  */
#define M83XX_PECR_CSR_RST          0x20000000 /* CSR reset negated         */

/* Watch Dog Registers */

#define WDT_REG_BA                  (CCSBAR + WDT_REG_BASE)

#define M83XX_SWCRR                 (CAST(VUINT32 *)(WDT_REG_BA + 0x04))
#define M83XX_SWCNR                 (CAST(VUINT32 *)(WDT_REG_BA + 0x08))
#define M83XX_SWSRR                 (CAST(VUINT16 *)(WDT_REG_BA + 0x0E))

/* GTM1 Registes */

#define QUICC_GTM1_BA               (CCSBAR + QUICC_GTM1_BASE)
#define QUICC_GTM1_GTCFR1           QUICC_GTM1_BA
#define QUICC_GTM1_GTCFR2           (QUICC_GTM1_BA + 0x4)
#define QUICC_GTM1_GTMDR1           (QUICC_GTM1_BA + 0x10)
#define QUICC_GTM1_GTMDR2           (QUICC_GTM1_BA + 0x12)
#define QUICC_GTM1_GTMDR3           (QUICC_GTM1_BA + 0x20)
#define QUICC_GTM1_GTMDR4           (QUICC_GTM1_BA + 0x22)

/* Clock Module */

#define CLKM_REG_BA                 (CCSBAR + CLKM_REG_BASE)

#define M83XX_SPMR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x00))
#define M83XX_SPMR_LBIU_VAL(val)    (val >> 31)
#define M83XX_SPMR_DDR_VAL(val)     ((val >> 30) & 0x1)
#define M83XX_SPMR_SPMF_VAL(val)    ((val >> 24) & 0xf)
#define M83XX_SPMR_CLK_DIV(val)     ((val >> 23) & 0x1)
#define M83XX_SPMR_CLK_PLL(val)     ((val >> 16) & 0x7f)

#define M83XX_OCCR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x04))
#define M83XX_OCCR_PCICOE_ALL       0xF8000000

#define M83XX_SCCR                  (CAST(VUINT32 *)(CLKM_REG_BA + 0x08))
#define M83XX_SCCR_TSEC1CM          0xC0000000 /* TSEC1 clock mode          */
#define M83XX_SCCR_TSEC1CM_DIS      0x00000000 /* TSEC1 clock is disabled   */
#define M83XX_SCCR_TSEC1CM_1        0x40000000 /* clock/csb_clk = 1:1       */
#define M83XX_SCCR_TSEC1CM_2        0x80000000 /* clock/csb_clk = 1:2       */
#define M83XX_SCCR_TSEC1CM_3        0xC0000000 /* clock/csb_clk = 1:3       */
                     
#define M83XX_SCCR_TSEC2CM          0x30000000 /* TSEC2 clock mode          */
#define M83XX_SCCR_TSEC2CM_DIS      0x00000000 /* TSEC2 clock is disabled   */
#define M83XX_SCCR_TSEC2CM_1        0x10000000 /* clock/csb_clk = 1:1       */
#define M83XX_SCCR_TSEC2CM_2        0x20000000 /* clock/csb_clk = 1:2       */
#define M83XX_SCCR_TSEC2CM_3        0x30000000 /* clock/csb_clk = 1:3       */
                
#define M83XX_SCCR_USBDRCM          0x00c00000 /* USB DR clock mode         */
#define M83XX_SCCR_USBDRCM_DIS      0x00000000 /* USB DR clock is disabled  */
#define M83XX_SCCR_USBDRCM_1        0x00400000 /* clock/csb_clk = 1:1       */
#define M83XX_SCCR_USBDRCM_2        0x00800000 /* clock/csb_clk = 1:2       */
#define M83XX_SCCR_USBDRCM_3        0x00c00000 /* clock/csb_clk = 1:3       */

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

/* Local Bus Controller */

/* BRn Field Descriptions */

#define M83XX_BR_BA_MASK            0xFFFF8000 /* Base address mask          */

#define M83XX_BR_PS                 0x00001800
#define M83XX_BR_PS_8               0x00000800 /* Port Size 8 bit            */
#define M83XX_BR_PS_16              0x00001000 /* Port Size 16 bit           */
#define M83XX_BR_PS_32              0x00001800 /* Port Size 32 bit           */

#define M83XX_BR_DECC               0x00000600 /* data error checking        */
#define M83XX_BR_DECC_OFF           0x00000000
#define M83XX_BR_DECC_CHK           0x00000200
#define M83XX_BR_DECC_CHK_GEN       0x00000400

#define M83XX_BR_WP                 0x00000100 /* Write protect              */

#define M83XX_BR_MSEL               0x000000E0
#define M83XX_BR_MS_GPCM            0x00000000 /* GPCM                       */
#define M83XX_BR_MS_FCM             0x00000020 /* FCM                        */
#define M83XX_BR_MS_UPMA            0x00000080 /* UPMA                       */
#define M83XX_BR_MS_UPMB            0x000000A0 /* UPMB                       */
#define M83XX_BR_MS_UPMC            0x000000C0 /* UPMC                       */

#define M83XX_BR_V                  0x00000001 /* Valid bit                  */

/* ORn Field Descriptions - GPCM Mode */

#define M83XX_OR_GPCM_BCTLD         0x00001000 /* Buffer control disable     */
#define M83XX_OR_GPCM_CSNT          0x00000800 /* CS negation time           */

#define M83XX_OR_GPCM_ACS           0x00000600 /* Address to CS setup        */
#define M83XX_OR_GPCM_ACS_DIV2      0x00000600
#define M83XX_OR_GPCM_ACS_DIV4      0x00000400

#define M83XX_OR_GPCM_XACS          0x00000100 /* Extra Address to CS setup  */

#define M83XX_OR_GPCM_SCY           0x000000F0 /* Cycle length in bus clocks */
#define M83XX_OR_GPCM_SCY_SHIFT     4           /* 1 ~ 15 is allowed         */

#define M83XX_OR_GPCM_SETA          0x00000008 /* External address suspend   */
#define M83XX_OR_GPCM_TRLX          0x00000004 /* Timing relaxed             */
#define M83XX_OR_GPCM_EHTR          0x00000002 /* Extended hold time on read */
#define M83XX_OR_GPCM_EAD           0x00000001 /* External addr latch delay  */

#define M83XX_OR_AM_32KB            0xFFFF8000
#define M83XX_OR_AM_64KB            0xFFFF0000
#define M83XX_OR_AM_128KB           0xFFFE0000
#define M83XX_OR_AM_256KB           0xFFFC0000
#define M83XX_OR_AM_512KB           0xFFF80000
#define M83XX_OR_AM_1MB             0xFFF00000
#define M83XX_OR_AM_2MB             0xFFE00000
#define M83XX_OR_AM_4MB             0xFFC00000
#define M83XX_OR_AM_8MB             0xFF800000
#define M83XX_OR_AM_16MB            0xFF000000
#define M83XX_OR_AM_32MB            0xFE000000
#define M83XX_OR_AM_64MB            0xFC000000
#define M83XX_OR_AM_128MB           0xF8000000
#define M83XX_OR_AM_256MB           0xF0000000
#define M83XX_OR_AM_512MB           0xE0000000
#define M83XX_OR_AM_1GB             0xC0000000
#define M83XX_OR_AM_2GB             0x80000000
#define M83XX_OR_AM_4GB             0x00000000

/* LCRR Field Descriptions */

#define M83XX_LCRR_PBYP             0x80000000 /* PLL bypass                */

#define M83XX_LCRR_EADC             0x00030000 /* External address delay    */
                                               /* cycles of LCLK            */
#define M83XX_LCRR_EADC_4           0x00000000
#define M83XX_LCRR_EADC_1           0x00010000
#define M83XX_LCRR_EADC_2           0x00020000
#define M83XX_LCRR_EADC_3           0x00030000

#define M83XX_LCRR_CLKDIV           0x0000001F /* System clock divider      */
#define M83XX_LCRR_CLKDIV_2         0x00000002
#define M83XX_LCRR_CLKDIV_4         0x00000004
#define M83XX_LCRR_CLKDIV_8         0x00000008

/* SATA registers */

#define SATA1_REG_BA                (CCSBAR + SATA1_REG_BASE)
#define SATA2_REG_BA                (CCSBAR + SATA2_REG_BASE)
#define SATA3_REG_BA                (CCSBAR + SATA3_REG_BASE)
#define SATA4_REG_BA                (CCSBAR + SATA4_REG_BASE)

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
#define M83XX_TXTTFILLTUNING        (CAST(VUINT32 *)(USBDR_REG_BA + 0x164))
#define M83XX_CONFIGFLAG            (CAST(VUINT32 *)(USBDR_REG_BA + 0x180))
#define M83XX_PORTSC                (CAST(VUINT32 *)(USBDR_REG_BA + 0x184))
#define M83XX_PORTSC_PTS            0xC0000000 /* Port transceiver select */
#define M83XX_PORTSC_PTS_ULPI       0x80000000 /* ULPI parallel interface */
#define M83XX_PORTSC_PTS_SERIAL     0xC0000000 /* FS/LS serial interface  */

#define M83XX_OTGSC                 (CAST(VUINT32 *)(USBDR_REG_BA + 0x1a4))
#define M83XX_USBMODE               (CAST(VUINT32 *)(USBDR_REG_BA + 0x1a8))
#define M83XX_USBMODE_CM            0x3 /* Controller mode */
#define M83XX_USBMODE_CM_OTG        0
#define M83XX_USBMODE_CM_DEVICE     2
#define M83XX_USBMODE_CM_HOST       3

/* These are big-endian */

#define M83XX_SNOOP1                (CAST(VUINT32 *)(USBDR_REG_BA + 0x400))
#define M83XX_SNOOP2                (CAST(VUINT32 *)(USBDR_REG_BA + 0x404))
#define M83XX_SNOOP_START_MASK      0xFFFFF000
#define M83XX_SNOOP_SIZE_2GB        0x1E
#define M83XX_AGE_CNT_THRESH        (CAST(VUINT32 *)(USBDR_REG_BA + 0x408))
#define M83XX_PRI_CTRL              (CAST(VUINT32 *)(USBDR_REG_BA + 0x40c))
#define M83XX_SI_CTRL               (CAST(VUINT32 *)(USBDR_REG_BA + 0x410))
#define M83XX_SI_CTRL_ERR_OFF       0x10 /* err_disable     */
#define M83XX_SI_CTRL_RD_FETCH_64   0x00 /* 64-byte fetch   */
#define M83XX_SI_CTRL_RD_FETCH_32   0x01 /* 32-byte fetch   */
#define M83XX_CONTROL               (CAST(VUINT32 *)(USBDR_REG_BA + 0x500))
#define M83XX_CONTROL_USB_EN        0x04 /* USB interface enable    */
#define M83XX_CONTROL_ULPI_INT_EN   0x01 /* ULPI low power wakeup   */
                                         /* interrupt enable        */

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

/* PCI LAWAR SIZE Settings */

#define PCI_LAWAR_ENABLE            0x80000000

#define PCI_LAWAR_SIZE_4KB          0x0000000B
#define PCI_LAWAR_SIZE_8KB          0x0000000C
#define PCI_LAWAR_SIZE_16KB         0x0000000D
#define PCI_LAWAR_SIZE_32KB         0x0000000E
#define PCI_LAWAR_SIZE_64KB         0x0000000F
#define PCI_LAWAR_SIZE_128KB        0x00000010
#define PCI_LAWAR_SIZE_256KB        0x00000011
#define PCI_LAWAR_SIZE_512KB        0x00000012
#define PCI_LAWAR_SIZE_1MB          0x00000013
#define PCI_LAWAR_SIZE_2MB          0x00000014
#define PCI_LAWAR_SIZE_4MB          0x00000015
#define PCI_LAWAR_SIZE_8MB          0x00000016
#define PCI_LAWAR_SIZE_16MB         0x00000017
#define PCI_LAWAR_SIZE_32MB         0x00000018
#define PCI_LAWAR_SIZE_64MB         0x00000019
#define PCI_LAWAR_SIZE_128MB        0x0000001A
#define PCI_LAWAR_SIZE_256MB        0x0000001B
#define PCI_LAWAR_SIZE_512MB        0x0000001C
#define PCI_LAWAR_SIZE_1GB          0x0000001D
#define PCI_LAWAR_SIZE_2GB          0x0000001E

/* Outbound/Inbound Comparison mask register defines */

#define PCI_ATTR_IO_BIT             0x40000000
#define PCI_OUT_ATTR_IO_BIT         0x40000000
#define PCI_OUT_ATTR_SE_BIT         0x20000000
#define PCI_OUT_ATTR_WS_4K          0x000FFFFF
#define PCI_OUT_ATTR_WS_8K          0x000FFFFE
#define PCI_OUT_ATTR_WS_16K         0x000FFFFC
#define PCI_OUT_ATTR_WS_32K         0x000FFFF8
#define PCI_OUT_ATTR_WS_64K         0x000FFFF0
#define PCI_OUT_ATTR_WS_128K        0x000FFFE0
#define PCI_OUT_ATTR_WS_256K        0x000FFFC0
#define PCI_OUT_ATTR_WS_512K        0x000FFF80
#define PCI_OUT_ATTR_WS_1M          0x000FFF00
#define PCI_OUT_ATTR_WS_2M          0x000FFE00
#define PCI_OUT_ATTR_WS_4M          0x000FFC00
#define PCI_OUT_ATTR_WS_8M          0x000FF800
#define PCI_OUT_ATTR_WS_16M         0x000FF000
#define PCI_OUT_ATTR_WS_32M         0x000FE000
#define PCI_OUT_ATTR_WS_64M         0x000FC000
#define PCI_OUT_ATTR_WS_128M        0x000F8000
#define PCI_OUT_ATTR_WS_256M        0x000F0000
#define PCI_OUT_ATTR_WS_512M        0x000E0000
#define PCI_OUT_ATTR_WS_1G          0x000C0000
#define PCI_OUT_ATTR_WS_2G          0x00080000
#define PCI_OUT_ATTR_WS_4G          0x00000000
#define PCI_IN_ATTR_WS_4K           0x0000000B
#define PCI_IN_ATTR_WS_8K           0x0000000c
#define PCI_IN_ATTR_WS_16K          0x0000000D
#define PCI_IN_ATTR_WS_32K          0x0000000E
#define PCI_IN_ATTR_WS_64K          0x0000000F
#define PCI_IN_ATTR_WS_128K         0x00000010
#define PCI_IN_ATTR_WS_256K         0x00000011
#define PCI_IN_ATTR_WS_512K         0x00000012
#define PCI_IN_ATTR_WS_1M           0x00000013
#define PCI_IN_ATTR_WS_2M           0x00000014
#define PCI_IN_ATTR_WS_4M           0x00000015
#define PCI_IN_ATTR_WS_8M           0x00000016
#define PCI_IN_ATTR_WS_16M          0x00000017
#define PCI_IN_ATTR_WS_32M          0x00000018
#define PCI_IN_ATTR_WS_64M          0x00000019
#define PCI_IN_ATTR_WS_128M         0x0000001A
#define PCI_IN_ATTR_WS_256M         0x0000001B
#define PCI_IN_ATTR_WS_512M         0x0000001C
#define PCI_IN_ATTR_WS_1G           0x0000001D
#define PCI_IN_ATTR_WS_2G           0x0000001E
#define PCI_IN_ATTR_WS_4G           0x0000001F

#define PCI_WINDOW_ENABLE_BIT                               0X80000000
#define PCI_IN_ATTR_TGI_LM                                  0X00F00000
#define PCI_IN_ATTR_TGI_RIO                                 0X00C00000
#define PCI_IN_ATTR_RTT_RIO_READ                            0X00040000
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0X00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0X00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0X00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0X00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0X00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0X00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0X00007000
#define PCI_SNOOP_ENABLE                                    0X40000000
#define PCI_PREFETCHABLE                                    0X20000000

/* Offsets for DDR registers */

#define DDR_REG_BA                 (CCSBAR + DDR_REG_BASE)

/* Chip Select Memory Bounds Register */

#define CS0_BNDS                    0x000
#define CS1_BNDS                    0x008
#define CS2_BNDS                    0x010
#define CS3_BNDS                    0x018

#define CS_BNDS_MASK                0xff
#define CS_BNDS_SHIFT               24
#define CS_BNDS_SA_SHIFT            16

/* Chip Select Configuration Register */

#define CS0_CONFIG                  0x080
#define CS1_CONFIG                  0x084
#define CS2_CONFIG                  0x088
#define CS3_CONFIG                  0x08C

#define CS_CONFIG_EN                0x80000000 /* CS enable                   */
#define CS_CONFIG_AP_EN             0x00800000 /* auto-precharge enable       */

#define CS_CONFIG_ODT_RD_CFG        0x00700000
#define CS_CONFIG_ODT_RD_CFG_DIS    0x00000000 /* Never assert for reads      */
#define CS_CONFIG_ODT_RD_CFG_1      0x00100000 /* reads to CSn only           */
#define CS_CONFIG_ODT_RD_CFG_2      0x00200000 /* reads to other chip         */
                                               /* selects only                */
#define CS_CONFIG_ODT_RD_CFG_3      0x00300000 /* reads to other DIMM         */
                                               /* modules only                */
#define CS_CONFIG_ODT_RD_CFG_4      0x00400000 /* Assert ODT for all reads    */

#define CS_CONFIG_ODT_WR_CFG        0x00070000
#define CS_CONFIG_ODT_WR_CFG_DIS    0x00000000 /* Never assert for writes     */
#define CS_CONFIG_ODT_WR_CFG_1      0x00010000 /* writes to CSn only          */
#define CS_CONFIG_ODT_WR_CFG_2      0x00020000 /* writes to other chip        */
                                               /* selects only                */
#define CS_CONFIG_ODT_WR_CFG_3      0x00030000 /* writes to other DIMM        */
                                               /* modules only                */
#define CS_CONFIG_ODT_WR_CFG_4      0x00040000 /* Assert ODT for all writes   */

#define CS_CONFIG_BANK_BITS         0x0000C000 /* Number of bank bits         */
#define CS_CONFIG_BANK_BITS_2       0x00000000
#define CS_CONFIG_BANK_BITS_3       0x00004000
#define CS_CONFIG_ROW_BIT           0x00000700 /* Number of row bits          */
#define CS_CONFIG_ROW_BIT_12        0x00000000
#define CS_CONFIG_ROW_BIT_13        0x00000100
#define CS_CONFIG_ROW_BIT_14        0x00000200
#define CS_CONFIG_ROW_BIT_15        0x00000300
#define CS_CONFIG_COL_BIT           0x00000007 /* Number of column bits       */
#define CS_CONFIG_COL_BIT_8         0x00000000
#define CS_CONFIG_COL_BIT_9         0x00000001
#define CS_CONFIG_COL_BIT_10        0x00000002
#define CS_CONFIG_COL_BIT_11        0x00000003

/* DDR SDRAM Timing Configuration Register */

#define TIMING_CFG_3                0x100
#define TIMING_CFG_0                0x104
#define TIMING_CFG_1                0x108
#define TIMING_CFG_2                0x10C

#define TIMING_CFG_3_EXT_REFREC         0x00070000 /* Extended refresh        */
#define TIMING_CFG_3_EXT_REFREC_SHIFT   16         /* recovery time:          */
                                                   /* 0 ~ 7 - (n * 16) clocks */

#define TIMING_CFG_0_RWT            0xC0000000 /* Read-to-write turnaround    */
#define TIMING_CFG_0_RWT_SHIFT      30         /* 0 ~ 3 is allowed            */
#define TIMING_CFG_0_WRT            0x30000000 /* Write-to-read turnaround    */
#define TIMING_CFG_0_WRT_SHIFT      28         /* 0 ~ 3 is allowed            */
#define TIMING_CFG_0_RRT            0x0C000000 /* Read-to-read turnaround     */
#define TIMING_CFG_0_RRT_SHIFT      26         /* 0 ~ 3 is allowed            */
#define TIMING_CFG_0_WWT            0x03000000 /* Write-to-write turnaround   */
#define TIMING_CFG_0_WWT_SHIFT      24         /* 0 ~ 3 is allowed            */

#define TIMING_CFG_0_ACT_PD_EXIT        0x00700000 /* Active powerdown exit   */
#define TIMING_CFG_0_ACT_PD_EXIT_SHIFT  20         /* 1 ~ 7 is allowed        */
#define TIMING_CFG_0_PRE_PD_EXIT        0x00070000 /* Precharge powerdown     */
#define TIMING_CFG_0_PRE_PD_EXIT_SHIFT  16         /* exit: 1 ~ 7 is allowed  */
#define TIMING_CFG_0_ODT_PD_EXIT        0x00000F00 /* ODT powerdown exit      */
#define TIMING_CFG_0_ODT_PD_EXIT_SHIFT  8          /* 0 ~ 15 is allowed       */
#define TIMING_CFG_0_MRS_CYC            0x0000000F /* Mode register set       */
                                                   /* cycle: 1 ~ 7 is allowed */

#define TIMING_CFG_1_PRETOACT       0x70000000 /* Precharge-to-activate       */
#define TIMING_CFG_1_PRETOACT_SHIFT 28         /* interval: 1 ~ 7 is allowed  */
#define TIMING_CFG_1_ACTTOPRE       0x0F000000 /* Activate to precharge       */
#define TIMING_CFG_1_ACTTOPRE_SHIFT 24         /* interval:                   */
                                               /* 0 ~ 3  - 16 ~ 19 clocks     */
                                               /* 4 ~ 15 - 4 ~ 15 clocks      */
#define TIMING_CFG_1_ACTTORW        0x00700000 /* Activate to read/write      */
#define TIMING_CFG_1_ACTTORW_SHIFT  20         /* interval: 1 ~ 7 is allowed  */
#define TIMING_CFG_1_CASLAT         0x00070000 /* MCAS latency from READ      */
#define TIMING_CFG_1_CASLAT_SHIFT   16         /* command: 1 ~ 15 -           */
                                               /* (1 + ( n - 1) * 0.5) clocks */
#define TIMING_CFG_1_REFREC         0x0000F000 /* Refresh recovery time       */
#define TIMING_CFG_1_REFREC_SHIFT   12         /* 0 ~ 15 - 8 ~ 23 clocks      */
#define TIMING_CFG_1_WRREC          0x00000700 /* Last data to precharge      */
#define TIMING_CFG_1_WRREC_SHIFT    8          /* minimum interval: 1 ~ 7 is  */
                                               /* allowed                     */
#define TIMING_CFG_1_ACTTOACT       0x00000070 /* Activate-to-activate        */
#define TIMING_CFG_1_ACTTOACT_SHIFT 4          /* interval: 1 ~ 7 is allowed  */
#define TIMING_CFG_1_WRTORD         0x00000007 /* Last write data pair to     */
                                               /* read command issue interval:*/
                                               /* 1 ~ 7 is allowed            */

#define TIMING_CFG_2_ADD_LAT        0x70000000 /* Additive latency: 0 ~ 5 is  */
#define TIMING_CFG_2_ADD_LAT_SHIFT  28         /* allowed                     */
#define TIMING_CFG_2_CPO            0x0F800000 /* MCAS-to-preamble override:  */
#define TIMING_CFG_2_CPO_SHIFT      23         /* 0      - READ_LAT + 1       */
                                               /* 1      - Reserved           */
                                               /* 2 ~ 21 - READ_LAT + (n-2)/4 */

#define TIMING_CFG_2_WR_LAT_DELAY        0x00380000 /* Write latency:         */
#define TIMING_CFG_2_WR_LAT_DELAY_SHIFT  19         /* 1 ~ 7 is allowed       */
#define TIMING_CFG_2_RD_TO_PRE           0x0000E000 /* Read to precharge:     */
#define TIMING_CFG_2_RD_TO_PRE_SHIFT     13         /* 1 ~ 4 is allowed       */
#define TIMING_CFG_2_WR_DATA_DELAY       0x00001C00 /* Write command to write */
#define TIMING_CFG_2_WR_DATA_DELAY_SHIFT 10         /* data strobe timing     */
                                                    /* adjustment: 0 ~ 6 -    */
                                                    /* n/4 clock delay        */
#define TIMING_CFG_2_CKE_PLS        0x000001C0 /* Minimum CKE pulse width:    */
#define TIMING_CFG_2_CKE_PLS_SHIFT  6          /* 1 ~ 4 is allowed            */
#define TIMING_CFG_2_FOUR_ACT       0x0000003F /* Window for four activates:  */
                                               /* 1 ~ 20 is allowed           */

/* DDR SDRAM Control Configuration register */

#define DDR_SDRAM_CFG               0x110
#define DDR_SDRAM_CFG_2             0x114

#define SDRAM_CFG_MEM_EN            0x80000000 /* SDRAM interface logic enable*/
#define SDRAM_CFG_SREN              0x40000000 /* Self refresh enable         */
#define SDRAM_CFG_ECC_EN            0x20000000 /* ECC enable                  */
#define SDRAM_CFG_RD_EN             0x10000000 /* Registered DIMM enable      */
#define SDRAM_CFG_SDRAM_TYPE        0x07000000 /* Type of SDRAM device        */
#define SDRAM_CFG_SDRAM_TYPE_DDR1   0x02000000 /* DDR1 SDRAM                  */
#define SDRAM_CFG_SDRAM_TYPE_DDR2   0x03000000 /* DDR2 SDRAM                  */
#define SDRAM_CFG_DYN_PWR           0x00200000 /* Dynamic power management    */
#define SDRAM_CFG_32_BE             0x00080000 /* 32-bit bus enable           */
#define SDRAM_CFG_8_BE              0x00040000 /* 8-beat burst enable         */
#define SDRAM_CFG_NCAP              0x00020000 /* Non-concurrent              */
                                               /* auto-precharge              */
#define SDRAM_CFG_2T_EN             0x00008000 /* Enable 2T timing            */
#define SDRAM_CFG_BI                0x00000001 /* Bypass initialization       */

#define SDRAM_CFG_2_NUM_PR          0x0000F000 /* Number of posted refreshes: */
#define SDRAM_CFG_2_NUM_PR_SHIFT    12         /* 1 ~ 8 is allowed            */

/* DDR SDRAM Mode Configuration register */

#define DDR_SDRAM_MODE_CFG          0x118
#define DDR_SDRAM_MODE_CFG_2        0x11C

/*
 * SDRAM mode and Extended SDRAM mode will be loaded into the corresponding DDR
 * SDRAM register. Those vlaues are specified by the DDR SDRAM manufacturer.
 */

#define DDR_SDRAM_MODE_ESD          0xFFFF0000 /* Extended SDRAM mode         */
#define DDR_SDRAM_MODE_ESD_SHIFT    16
#define DDR_SDRAM_MODE_SD           0x0000FFFF /* SDRAM mode                  */
#define DDR_SDRAM_MODE_ESD2         0xFFFF0000 /* Extended SDRAM mode 2       */
#define DDR_SDRAM_MODE_ESD2_SHIFT   16
#define DDR_SDRAM_MODE_ESD3         0x0000FFFF /* Extended SDRAM mode 3       */

/* DDR SDRAM Interval Configuration register */

#define DDR_SDRAM_INTERVAL          0x124

#define SDRAM_INTERVAL_REFINT       0xFFFF0000 /* Refresh interval            */
#define SDRAM_INTERVAL_REFINT_SHIFT 16
#define SDRAM_INTERVAL_BSTOPRE      0x00003FFF /* Precharge interval          */

/* DDR SDRAM Clock Control Register */

#define DDR_SDRAM_CLK_CNTRL                 0x130

#define DDR_SDRAM_CLK_CNTL_CLK_ADJUST       0x07000000 /* Clock adjust:       */
#define DDR_SDRAM_CLK_CNTL_CLK_ADJUST_SHIFT 24         /* 0 ~ 4 - Clock is    */
                                                       /* launched (n/4)      */
                                                       /* applied cycle after */
                                                       /* address/command     */

/* other registers */

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

/* SerDes PHY (PCI Express/SGMII/SATA) */

#define SRDSPHY1_BA(n)              (CCSBAR + SRDSPHY1_BASE + (n * 0x100))

#define SRDSn_CR0(n)                (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x0))
#define SRDSn_CR1(n)                (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x4))
#define SRDSn_CR2(n)                (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x8))
#define SRDSn_CR3(n)                (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0xC))
#define SRDSn_CR4(n)                (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x10))
#define SRDSn_RSTCTL(n)             (CAST(VUINT32 *)(SRDSPHY1_BA(n) + 0x20))

#define SRDSn_PROT_SATA             1 /* 2 SATA interface    */
#define SRDSn_PROT_PEX_X1           2 /* 2 PCIe x1 interface */
#define SRDSn_PROT_PEX_X2           3 /* 1 PCIe x2 interface */
#define SRDSn_RST_TIMEOUT           0x100000

#define SRDSn_CR0_DPP               0x00008800 /* Diff pk-pk swing          */

#define SRDSn_CR1_PLLBW             0x00000040 /* PLL bandwidth             */

#define SRDSn_CR2_SEIC_MASK         0x00001C1C /* Receiver electrical idle  */
                                               /* detection control         */
#define SRDSn_CR2_SEIC_SATA         0x00001414 /* default for SATA          */
#define SRDSn_CR2_SEIC_PEX          0x00001010 /* default for PCIe          */
#define SRDSn_CR2_SEIC_SGMII        0x00000101 /* default for SGMII         */

#define SRDSn_CR3_KFR_SATA          0x10100000 /* gain Kfr in CDR for SATA  */
#define SRDSn_CR3_KPH_SATA          0x04040000 /* gain Kph in CDR for SATA  */
#define SRDSn_CR3_SDFM_SATA_PEX     0x01010000 /* bandwidth of the digital  */
                                               /* filter for SATA and PCIe  */
#define SRDSn_CR3_SDTXL_SATA        0x00000505 /* transmitter amplitude     */
                                               /* levels for SATA           */

#define SRDSn_CR4_RFCKS_100MHz      0x00000000 /* 100 MHz reference clock   */
#define SRDSn_CR4_RFCKS_125MHz      0x10000000 /* 125 MHz reference clock   */
#define SRDSn_CR4_RFCKS_150MHz      0x30000000 /* 150 MHz reference clock   */
#define SRDSn_CR4_PROT_SATA         0x00000808 /* SATA protocol             */
#define SRDSn_CR4_PROT_PEX          0x00000101 /* PCIe protocol             */
#define SRDSn_CR4_PROT_SGMII        0x00000505 /* SGMII protocol            */
#define SRDSn_CR4_PLANE_X2          0x01000000 /* 1 PCIe x2 interface       */

#define SRDSn_RSTCTL_RST            0x80000000 /* SerDes soft reset         */
#define SRDSn_RSTCTL_RDONE          0x40000000 /* SerDes reset done         */
#define SRDSn_RSTCTL_SATA_RESET     0xF        /* reset Rx and Tx of SATA   */

/* PCI defines */

#define NUM_PCI_SLOTS               0x4 /* 3 PCI slots: 0 to 2 */
#define NUM_PCIEX1_SLOTS            0x1
#define NUM_PCIEX2_SLOTS            0x1

#define PCI_XINT1_LVL               0x0 /* PCI XINT1 routed to IRQ0 */
#define PCI_XINT2_LVL               0x1 /* PCI XINT2 routed to IRQ1 */
#define PCI_XINT3_LVL               0x2 /* PCI XINT3 routed to IRQ2 */
#define PCI_XINT4_LVL               0x3 /* PCI XINT3 routed to IRQ3 */

#define PCIEX1_XINT1_LVL            0x1 /* PCIEX1 XINT1 routed to IRQ1 */
#define PCIEX1_XINT2_LVL            0x1 /* PCIEX1 XINT2 routed to IRQ1 */
#define PCIEX1_XINT3_LVL            0x1 /* PCIEX1 XINT3 routed to IRQ1 */
#define PCIEX1_XINT4_LVL            0x1 /* PCIEX1 XINT3 routed to IRQ1 */
#define PCIEX2_XINT1_LVL            0x2 /* PCIEX2 XINT1 routed to IRQ2 */
#define PCIEX2_XINT2_LVL            0x2 /* PCIEX2 XINT2 routed to IRQ2 */
#define PCIEX2_XINT3_LVL            0x2 /* PCIEX2 XINT3 routed to IRQ2 */
#define PCIEX2_XINT4_LVL            0x2 /* PCIEX2 XINT3 routed to IRQ2 */

#define PCI_SLOT1_DEVNO             0x10 /* PCI SLOT 1 Device no */

#define PCI_LAT_TIMER               0x40 /* latency timer value */
                                         /* 64 PCI clocks       */

/* DUART */

#define DUART1_REG_BA               (CCSBAR + DUART1_REG_BASE)
#define DUART2_REG_BA               (CCSBAR + DUART2_REG_BASE)
#define DUART_REG_ADDR_INTERVAL     1 /* duart vector register distance */

/* ETSEC */

#define ETSEC1_REG_BA               (CCSBAR + ETSEC1_REG_BASE)
#define ETSEC2_REG_BA               (CCSBAR + ETSEC2_REG_BASE)

/* interrupt controller  */

#define SIC_REG_BA                  (CCSBAR + SIC_REG_BASE)

/* interrupt vectors */

#define INUM_PEX1_CNT                1
#define INUM_PEX2_CNT                2
#define INUM_MSIR1                   4
#define INUM_UART1                   9
#define INUM_UART2                  10
#define INUM_SEC                    11
#define INUM_ETSEC1_1588_TIMER      12
#define INUM_ETSEC2_1588_TIMER      13
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
#define INUM_TSEC1_TX               32
#define INUM_TSEC1_RX               33
#define INUM_TSEC1_ERR              34
#define INUM_TSEC2_TX               35
#define INUM_TSEC2_RX               36
#define INUM_TSEC2_ERR              37
#define INUM_USB_DR                 38
#define INUM_ESDHC                  42
#define INUM_SATA1                  44
#define INUM_SATA2                  45
#define INUM_SATA3                  46
#define INUM_SATA4                  47
#define INUM_IRQ0                   48
#define INUM_RTC_SEC                64
#define INUM_PIT                    65
#define INUM_PCI                    66
#define INUM_MSIR0                  67
#define INUM_RTC_ALR                68
#define INUM_MU                     69
#define INUM_SBA                    70
#define INUM_DMA                    71
#define INUM_GTM4                   72
#define INUM_GTM8                   73
#define INUM_CPIO1                  74
#define INUM_GPIO2                  75
#define INUM_DDR                    76
#define INUM_LBC                    77
#define INUM_GTM2                   78
#define INUM_GTM6                   79
#define INUM_PMC                    80
#define INUM_MSIR2                  81
#define INUM_MSIR3                  82
#define INUM_GTM3                   84
#define INUM_GTM7                   85
#define INUM_MSIR4                  86
#define INUM_MSIR5                  87
#define INUM_MSIR6                  88
#define INUM_MSIR7                  89
#define INUM_GTM1                   90
#define INUM_GTM5                   91

#define INUM_MAX                    128
#define IVEC_MAX                    128

/* BCSR */

#define BCSR0                       BCSR_BASE_ADRS          /* Register 0 */
#define BCSR1                       (BCSR_BASE_ADRS + 0x01) /* Register 1 */
#define BCSR2                       (BCSR_BASE_ADRS + 0x02) /* Register 2 */
#define BCSR3                       (BCSR_BASE_ADRS + 0x03) /* Register 3 */
#define BCSR4                       (BCSR_BASE_ADRS + 0x04) /* Register 4 */
#define BCSR5                       (BCSR_BASE_ADRS + 0x05) /* Register 5 */
#define BCSR6                       (BCSR_BASE_ADRS + 0x06) /* Register 6 */
#define BCSR7                       (BCSR_BASE_ADRS + 0x07) /* Register 7 */
#define BCSR8                       (BCSR_BASE_ADRS + 0x08) /* Register 8 */
#define BCSR9                       (BCSR_BASE_ADRS + 0x09) /* Register 9 */
#define BCSR10                      (BCSR_BASE_ADRS + 0x0a) /* Register 10 */
#define BCSR11                      (BCSR_BASE_ADRS + 0x0b) /* Register 11 */

#ifndef _ASMLANGUAGE

typedef union ads_bcsr
    {
    struct
        {
        /* 0 */

        UINT8  lbcm:1;
        UINT8  ddrcm:1;
        UINT8  :2;
        UINT8  spmf:4;

        /* 1 */

        UINT8  :1;
        UINT8  corepll:7;

        /* 2 */

        UINT8  :8;

        /* 3 */

        UINT8  :8;

        /* 4 */

        UINT8  pciHost:1;
        UINT8  :1;
        UINT8  pciArb:1;
        UINT8  :1;
        UINT8  coreDis:1;
        UINT8  bms:1;
        UINT8  bootSeq:2;

        /* 5 */

        UINT8  swen:1;
        UINT8  romLoc:3;
        UINT8  rlext:2;
        UINT8  :2;

        /* 6 */

        UINT8  tsec1m:2;
        UINT8  tsec2m:2;
        UINT8  :4;

        /* 7 */

        UINT8  :4;
        UINT8  tle:1;
        UINT8  :2;
        UINT8  ldp:1;

        /* 8 */

        UINT8 gethrst:1;
        UINT8 geth1_en:1;
        UINT8 rsvd:1;
        UINT8 geth2_en:1;
        UINT8 rs232en:1;
        UINT8 bootwp:1;
        UINT8 signal0:1;
        UINT8 signal1:1;

        /* 9 */

        UINT8 cfgClkinDiv:1;
        UINT8 cfgRs:4;
        UINT8 flashPrt:1;
        UINT8 jtagSel:1;
        UINT8 cfgLbmux:1;

        /* 10 */

        UINT8 :8;

        /* 11 */

        UINT8 :8;

        /* 12 */

        UINT8 usbUlpi:1;
        UINT8 spiSd:1;
        UINT8 fcfg:1;
        UINT8 cnflock:1;
        UINT8 usbSerSd:1;
        UINT8 irq4wp:1;
        UINT8 usbMode:1;
        UINT8 :1;

        /* 13 */

        UINT8 ledEn:1;
        UINT8 :4;
        UINT8 pcieEnable:1;
        UINT8 :2;

        /* 14 */

        UINT8 irq0Tri:1;
        UINT8 irq1Tri:1;
        UINT8 irq2Tri:1;
        UINT8 irq3Tri:1;
        UINT8 irq4Tri:1;
        UINT8 irq5Tri:1;
        UINT8 irq6Tri:1;
        UINT8 irq7Tri:1;

        } field;

    UINT32 bcsr[15];
    } BCSR_REG;

#endif  /* _ASMLANGUAGE */

/* Define Clock Speed and source  */

#define FREQ_400_MHZ                396000000

/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 or the PCI freq of the MDS837x board.
 * Choose from above list. BSP assumes standalone board ie not in
 * PCI chassis
 */

#define OSCILLATOR_FREQ             66000000 /* 66MHZ */

/*
 * These default values assume CSB is 333Mhz based on HRCW setting in
 *  boot image
 */

#define SYS_CLK_FREQ                396000000 /* 396MHz */

/* no off-board bus interface */

#define BUS                         NONE

/* number of SIOs */

#define N_SIO_CHANNELS              2

/* Hard Reset Configuration Words */

/* Low Word */

/* Byte 0 */

#define HRCW_LOW_LBIU_DIV2          0x80
#define HRCW_LOW_DDR_DIV2           0x40
#define HRCW_LOW_SPMF               0x0f

/* Byte 1 */

#define HRCW_LOW_CORE_PLL           0xFF

/* Byte 2 and 3 must be cleared */

/* High Word */

/* Byte 0 */

#define HRCW_HIGH_PCI_HOST          0x80

#define HRCW_HIGH_PCI_ARB           0x20

#define HRCW_HIGH_CORE_DIS          0x08
#define HRCW_HIGH_BMS_HIGH          0x04
#define HRCW_HIGH_BOOT_SEQ_I2C      0x01
#define HRCW_HIGH_BOOT_SEQ_EXT_I2C  0x02

/* Byte 1 */

#define HRCW_HIGH_SWEN              0x80
#define HRCW_HIGH_ROMLOC            0x70
#define ROMLOC_DDR                  0x00
#define ROMLOC_PCI1                 0x10
#define ROMLOC_PCI2                 0x20
#define ROMLOC_GPCM_8BIT            0x50
#define ROMLOC_GPCM_16BIT           0x60
#define ROMLOC_GPCM_32BIT           0x70

/* Byte 2 */

#define HRCW_HIGH_TSEC1M_RGMII      0x60
#define HRCW_HIGH_TSEC2M_RGMII      0x0C

#define HRCW_HIGH_TSEC12M_RGMII     0x6C

/* Byte 3 */

#define HRCW_HIGH_TLE               0x40
#define HRCW_HIGH_LDP               0x02

#ifndef _ASMLANGUAGE

/* function declarations */

extern void     sysMsDelay (uint32_t);
extern void     sysDelay (void);
extern UINT32   vxHid2Get (void);
extern void     vxHid2Set (UINT32);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INCads837xh */
