/* p1021mds.h - Freescale P1021MDS board header */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,05mar12,y_y  add vxBus I2C support.
01c,28may11,y_y  add some I2C devices address.
01b,07apr11,y_y  add NV_RAM_ADRS macro to meet nvRam.(WIND00266085) 
01a,19dec10,y_y  derived from version 01b of fsl_p2020_ds/p2020ds.h
*/

/*
 * This file contains I/O addresses and related constants for the
 * Freescale P1021MDS board.
*/

#ifndef __INCp1021mdsh
#define __INCp1021mdsh

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
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

/* Base Address of Memory Mapped Registers */

#define CCSBAR_INIT                     0xff700000
#define CCSBAR                          0xffe00000
#define CCSBAR_SIZE                     0x00100000
#define BCSR_BASE_ADRS                  0xf8000000
#define MP_MAGIC_RBV                    0xfeedfeed
#define MP_MAGIC_CACHE_FLUSHED          0xD07ED07E

/* SOC PVR and SVR */

#define PPC_PVR_P1021                   0x80212050
#define PPC_PVR_85XX                    0x80200000
#define PPC_PVR_MASK                    0xFFF00000
#define PPC_SVR_P1021_SEC               0x80EC0110
#define PPC_SVR_P1021                   0x80E40110
#define PPC_SVR_P1012_SEC               0x80ED0110
#define PPC_SVR_P1012                   0x80E50110
#define PPC_SVR_MASK                    0xFFF00000

/* Define Clock Speed and source */

#define FREQ_33_MHZ                     33330000
#define FREQ_50_MHZ                     50000000
#define FREQ_66_MHZ                     66670000
#define FREQ_100_MHZ                    100000000
#define FREQ_133_MHZ                    133330000
#define FREQ_266_MHZ                    266660000
#define FREQ_400_MHZ                    400000000
#define DDR_CLK_FREQ                    FREQ_66_MHZ
#define DEFAULT_SYSCLKFREQ              FREQ_400_MHZ

/* Frequency applied to the "RTC" pin */

#define HPCN_RTC_FREQ                   FREQ_66_MHZ

/* OpenPic timer vector definition */

#define OPENPIC_TIMERA0_INT_VEC         0x4C
#define OPENPIC_TIMERA1_INT_VEC         0x4D
#define OPENPIC_TIMERA2_INT_VEC         0x4E
#define OPENPIC_TIMERA3_INT_VEC         0x4F
#define OPENPIC_TIMERB0_INT_VEC         0x50
#define OPENPIC_TIMERB1_INT_VEC         0x51
#define OPENPIC_TIMERB2_INT_VEC         0x52
#define OPENPIC_TIMERB3_INT_VEC         0x53

/* Offsets for DDR registers */

#define CS0_BNDS                        0x000
#define CS0_CONFIG                      0x080
#define CS0_CONFIG_2                    0x0c0
#define TIMING_CFG_3                    0x100
#define TIMING_CFG_0                    0x104
#define TIMING_CFG_1                    0x108
#define TIMING_CFG_2                    0x10c
#define DDR_SDRAM_CFG                   0x110
#define DDR_SDRAM_CFG_2                 0x114
#define DDR_SDRAM_MODE                  0x118
#define DDR_SDRAM_MODE_2                0x11c
#define DDR_SDRAM_INTERVAL              0x124
#define DDR_DATA_INIT                   0x128
#define DDR_SDRAM_CLK_CNTL              0x130
#define TIMING_CFG_4                    0x160
#define TIMING_CFG_5                    0x164
#define DDR_ZQ_CNTL                     0x170
#define DDR_WRLVL_CNTL                  0x174
#define DDR_SR_CNTR                     0x17c
#define DDR_WRLVL_CNTL_2                0x190
#define DDR_WRLVL_CNTL_3                0x194
#define DDR_DDRCDR_1                    0xb28
#define DDR_DDRCDR_2                    0xb2c
#define DDR_PHY_ADDR                    0x0
#define DDR_PHY_SIZE                    0x20000000

/* Offsets for LAW registers */

#define  M85XX_LAWAR4_OFFSET            0xc90
#define  M85XX_LAWAR5_OFFSET            0xcb0
#define  M85XX_LAWAR6_OFFSET            0xcd0

/* Used for PCIEX1_MEM, PCIEX1_MEM_IO, PCIEX1_IO */

#define  M85XX_LAWBAR0(base)            (CAST(VUINT32 *)((base) + 0xc08))
#define  M85XX_LAWAR0(base)             (CAST(VUINT32 *)((base) + 0xc10))

/* Used for PCIEX2_MEM, PCIEX2_MEM_IO, PCIEX2_IO */

#define  M85XX_LAWBAR1(base)            (CAST(VUINT32 *)((base) + 0xc28))
#define  M85XX_LAWAR1(base)             (CAST(VUINT32 *)((base) + 0xc30))

/* Used for NAND Flash interface */

#define  M85XX_LAWBAR4(base)            (CAST(VUINT32 *)((base) + 0xc88))
#define  M85XX_LAWAR4(base)             (CAST(VUINT32 *)((base) + 0xc90))

/* Used for BCSR interface */

#define  M85XX_LAWBAR5(base)            (CAST(VUINT32 *)((base) + 0xca8))
#define  M85XX_LAWAR5(base)             (CAST(VUINT32 *)((base) + 0xcb0))

/* Used for DDR interface */

#define  M85XX_LAWBAR6(base)            (CAST(VUINT32 *)((base) + 0xcc8))
#define  M85XX_LAWAR6(base)             (CAST(VUINT32 *)((base) + 0xcd0))

/* LAWBAR  */

#define LAWBAR_ADRS_SHIFT               12
#define LAWAR_ENABLE                    0x80000000
#define LAWAR_TRGT_ELBC                 0x00000004
#define LAWAR_TRGT_DDR                  0x0000000f
#define LAWAR_TRGT_PCI1                 0x00000002
#define LAWAR_TRGT_PCI2                 0x00000001
#define LAWAR_TGTIF_PCIEX2              0x00100000
#define LAWAR_TGTIF_PCIEX1              0x00200000
#define LAWAR_TRGT_ELBC_SHIFT           0x00000014
#define LAWBAR_MASK                     0x000fffff

/* LAWAR SIZE Settings */

#define LAWAR_SIZE_4KB                  0x0000000B
#define LAWAR_SIZE_8KB                  0x0000000C
#define LAWAR_SIZE_16KB                 0x0000000D
#define LAWAR_SIZE_32KB                 0x0000000E
#define LAWAR_SIZE_64KB                 0x0000000F
#define LAWAR_SIZE_128KB                0x00000010
#define LAWAR_SIZE_256KB                0x00000011
#define LAWAR_SIZE_512KB                0x00000012
#define LAWAR_SIZE_1MB                  0x00000013
#define LAWAR_SIZE_2MB                  0x00000014
#define LAWAR_SIZE_4MB                  0x00000015
#define LAWAR_SIZE_8MB                  0x00000016
#define LAWAR_SIZE_16MB                 0x00000017
#define LAWAR_SIZE_32MB                 0x00000018
#define LAWAR_SIZE_64MB                 0x00000019
#define LAWAR_SIZE_128MB                0x0000001A
#define LAWAR_SIZE_256MB                0x0000001B
#define LAWAR_SIZE_512MB                0x0000001C
#define LAWAR_SIZE_1GB                  0x0000001D
#define LAWAR_SIZE_2GB                  0x0000001E

#define M85XX_BR0(base)                 (CAST(VUINT32 *)((base) + 0x5000))
#define M85XX_OR0(base)                 (CAST(VUINT32 *)((base) + 0x5004))
#define M85XX_BR1(base)                 (CAST(VUINT32 *)((base) + 0x5008))
#define M85XX_OR1(base)                 (CAST(VUINT32 *)((base) + 0x500c))
#define M85XX_BR2(base)                 (CAST(VUINT32 *)((base) + 0x5010))
#define M85XX_OR2(base)                 (CAST(VUINT32 *)((base) + 0x5014))
#define M85XX_BR3(base)                 (CAST(VUINT32 *)((base) + 0x5018))
#define M85XX_OR3(base)                 (CAST(VUINT32 *)((base) + 0x501c))
#define M85XX_BR4(base)                 (CAST(VUINT32 *)((base) + 0x5020))
#define M85XX_OR4(base)                 (CAST(VUINT32 *)((base) + 0x5024))
#define M85XX_BR5(base)                 (CAST(VUINT32 *)((base) + 0x5028))
#define M85XX_OR5(base)                 (CAST(VUINT32 *)((base) + 0x502C))
#define M85XX_BR6(base)                 (CAST(VUINT32 *)((base) + 0x5030))
#define M85XX_OR6(base)                 (CAST(VUINT32 *)((base) + 0x5034))
#define M85XX_BR7(base)                 (CAST(VUINT32 *)((base) + 0x5038))
#define M85XX_OR7(base)                 (CAST(VUINT32 *)((base) + 0x503C))
#define M85XX_MAR(base)                 (CAST(VUINT32 *)((base) + 0x5068))
#define M85XX_MAMR(base)                (CAST(VUINT32 *)((base) + 0x5070))
#define M85XX_MBMR(base)                (CAST(VUINT32 *)((base) + 0x5074))
#define M85XX_MCMR(base)                (CAST(VUINT32 *)((base) + 0x5078))
#define M85XX_MRTPR(base)               (CAST(VUINT32 *)((base) + 0x5084))
#define M85XX_MDR(base)                 (CAST(VUINT32 *)((base) + 0x5088))
#define M85XX_LSDMR(base)               (CAST(VUINT32 *)((base) + 0x5094))
#define M85XX_LBCR(base)                (CAST(VUINT32 *)((base) + 0x50D0))
#define M85XX_LCRR(base)                (CAST(VUINT32 *)((base) + 0x50D4))

/* Enhanced Local Bus Register */

#define ELBC_LSOR(base)                 (CAST(VUINT32 *)((base) | 0x5090))
#define ELBC_LTESR(base)                (CAST(VUINT32 *)((base) | 0x50b0))
#define ELBC_LTEDR(base)                (CAST(VUINT32 *)((base) | 0x50b4))
#define ELBC_LTEIR(base)                (CAST(VUINT32 *)((base) | 0x50b8))
#define ELBC_LTEATR(base)               (CAST(VUINT32 *)((base) | 0x50bc))
#define ELBC_LTEAR(base)                (CAST(VUINT32 *)((base) | 0x50c0))
#define ELBC_FMR(base)                  (CAST(VUINT32 *)((base) | 0x50e0))
#define ELBC_FIR(base)                  (CAST(VUINT32 *)((base) | 0x50e4))
#define ELBC_FCR(base)                  (CAST(VUINT32 *)((base) | 0x50e8))
#define ELBC_FBAR(base)                 (CAST(VUINT32 *)((base) | 0x50ec))
#define ELBC_FPAR(base)                 (CAST(VUINT32 *)((base) | 0x50f0))
#define ELBC_FBCR(base)                 (CAST(VUINT32 *)((base) | 0x50f4))
#define ELBC_BR_MASK                    0xffff8000
#define ELBC_BR_Valid                   0x00000001
#define ELBC_FIR_INIT_VAL               0xc12e0000
#define ELBC_FMR_VAL                    0x0000f022
#define ELBC_FPAR_MASK                  0x00007C00
#define LCRR_DBYP                       0x80000000
#define LCRR_CLKDIV_8                   0x00000004
#define ELBC_BR_PS_8                    (1 << 11)
#define ELBC_BR_PS_16                   (2 << 11)
#define ELBC_BR_DECC_DISABLE            (0 << 9)
#define ELBC_BR_DECC_HW                 (1 << 9)
#define ELBC_BR_WP_ENABLE               (1 << 8)
#define ELBC_BR_MSEL_GPCM               (0 << 5)
#define ELBC_BR_MSEL_FCM                (1 << 5)

#define BPTR_BOOT_PAGE_SHIFT            12
#define CPU1CHECK                       (vxHid1Get() & 0x1)

/* ECM Registers */

#define ECM_OFFSET                      0x1000
#define EEB_PCR(base)                   (CAST(VUINT32 *)((base) + \
                                        ECM_OFFSET + 0x10))
#define EEB_PCR_CORE1_ENABLE            0x02000000
#define EEB_PCR_CORE0_ENABLE            0x01000000

/* CCSR Block Base Address Map */

#define ECM_BASE                        (CCSBAR + 0x1000)
#define DDR_BASE                        (CCSBAR + 0x2000)
#define I2C1_BASE                       (CCSBAR + 0x3000)
#define I2C2_BASE                       (CCSBAR + 0x3100)
#define UART0_BASE                      (CCSBAR + 0x4500)
#define UART1_BASE                      (CCSBAR + 0x4600)
#define eLBC_BASE                       (CCSBAR + 0x5000)
#define eSPI_BASE                       (CCSBAR + 0x7000)
#define PCIE2_BASE                      (CCSBAR + 0x9000)
#define PCIE2_ATMU_BASE                 (CCSBAR + 0x9C00)
#define PCIE1_BASE                      (CCSBAR + 0xA000)
#define PCIE1_ATMU_BASE                 (CCSBAR + 0xAC00)
#define PAR_IO_INTR_BASE                (CCSBAR + 0xF000)
#define L2_SRAM_BASE                    (CCSBAR + 0x20000)
#define DMA_BASE                        (CCSBAR + 0x21100)
#define USB1_BASE                       (CCSBAR + 0x22000)
#define ETSEC1_MDIO_BASE                (CCSBAR + 0x24000)
#define ETSEC2_MDIO_BASE                (CCSBAR + 0x25000)
#define ETSEC3_MDIO_BASE                (CCSBAR + 0x26000)
#define ESDHC_BASE                      (CCSBAR + 0x2E000)
#define SEC_BASE                        (CCSBAR + 0x30000)
#define EPIC_BASE                       (CCSBAR + 0x40000)
#define MSI_BASE                        (CCSBAR + 0x41600)
#define QUICC_ENGINE_BASE               (CCSBAR + 0x80000)
#define ETSEC1_GRP0_BASE                (CCSBAR + 0xB0000)
#define ETSEC2_GRP0_BASE                (CCSBAR + 0xB1000)
#define ETSEC3_GRP0_BASE                (CCSBAR + 0xB2000)
#define GUTIL_BASE                      (CCSBAR + 0xE0000)
#define PM_BASE                         (CCSBAR + 0xE1000)

/* EHCI registers */

#define EHCI_CAPLENGTH(base)            ((base) + 0x00100)
#define EHCI_BURSTSIZE(base)            ((base) + 0x00160)
#define EHCI_TXTTFILLTUNING_L(base)     ((base) + 0x00164)
#define EHCI_TXTTFILLTUNING_H(base)     ((base) + 0x00168)
#define EHCI_CONFIGFLAG(base)           ((base) + 0x00180)
#define EHCI_PORTSC(base)               ((base) + 0x00184)
#define EHCI_USBMODE(base)              ((base) + 0x001a8)
#define EHCI_SNOOP1(base)               ((base) + 0x00400)
#define EHCI_SNOOP2(base)               ((base) + 0x00404)
#define EHCI_AGE_CNT_THRESH(base)       ((base) + 0x00408)
#define EHCI_PRI_CTRL(base)             ((base) + 0x0040c)
#define EHCI_SI_CTRL(base)              ((base) + 0x00410)
#define EHCI_CONTROL(base)              ((base) + 0x00500)
#define EHCI_USBMODE_IDLE               0x00000000
#define EHCI_USBMODE_DEVICE             0x00000002
#define EHCI_USBMODE_HOST               0x00000003
#define EHCI_PORTSC_ULPI                0x80000000
#define EHCI_SNOOP_SIZE_4K              0x0000000B
#define EHCI_SNOOP_SIZE_8K              0x0000000C
#define EHCI_SNOOP_SIZE_16K             0x0000000D
#define EHCI_SNOOP_SIZE_32K             0x0000000E
#define EHCI_SNOOP_SIZE_64K             0x0000000F
#define EHCI_SNOOP_SIZE_128K            0x00000010
#define EHCI_SNOOP_SIZE_256K            0x00000011
#define EHCI_SNOOP_SIZE_512K            0x00000012
#define EHCI_SNOOP_SIZE_1M              0x00000013
#define EHCI_SNOOP_SIZE_2M              0x00000014
#define EHCI_SNOOP_SIZE_4M              0x00000015
#define EHCI_SNOOP_SIZE_8M              0x00000016
#define EHCI_SNOOP_SIZE_16M             0x00000017
#define EHCI_SNOOP_SIZE_32M             0x00000018
#define EHCI_SNOOP_SIZE_64M             0x00000019
#define EHCI_SNOOP_SIZE_128M            0x0000001A
#define EHCI_SNOOP_SIZE_256M            0x0000001B
#define EHCI_SNOOP_SIZE_512M            0x0000001C
#define EHCI_SNOOP_SIZE_1G              0x0000001D
#define EHCI_SNOOP_SIZE_2G              0x0000001E

#ifdef INCLUDE_AMP_CPU
#   define EHCI_SNOOP_SIZE              EHCI_SNOOP_SIZE_256M
#else /* INCLUDE_AMP_CPU */
#   define EHCI_SNOOP_SIZE              EHCI_SNOOP_SIZE_512M
#endif /* INCLUDE_AMP_CPU */

#define EHCI_CONTROL_USB_EN             0x00000004

/* Global Function Registers */

/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */

#define M85XX_PORPLLSR(base)                (CAST(VUINT32 *)((base) + 0xE0000))
#define M85XX_DDRDLLCR(base)                (CAST(VUINT32 *)((base) + 0xE0E10))
#define M85XX_LBCDLLSR(base)                (CAST(VUINT32 *)((base) + 0xE0E20))
#define M85XX_DEVDISR(base)                 (CAST(VUINT32 *)((base) + 0xE0070))
#define M85XX_PORPLLSR_E500_RATIO(base)     ((*M85XX_PORPLLSR(base) & \
                                            M85XX_PORPLLSR_E500_RATIO_MASK)\
                                            >> 16)
#define M85XX_PORPLLSR_E500_1_RATIO(base)   ((*M85XX_PORPLLSR(base) & \
                                            M85XX_PORPLLSR_E500_1_RATIO_MASK)\
                                            >> 24)
#define M85XX_PORPLLSR_DDR_RATIO(base)      ((*M85XX_PORPLLSR(base) & \
                                            M85XX_PORPLLSR_DDR_RATIO_MASK)\
                                            >> 9)
#define M85XX_PORPLLSR_PLAT_RATIO(base)     ((*M85XX_PORPLLSR(base) & \
                                            M85XX_PORPLLSR_PLAT_RATIO_MASK)\
                                            >> 1)
#define M85XX_PORPLLSR_E500_1_RATIO_MASK    0x3f000000
#define M85XX_PORPLLSR_E500_RATIO_MASK      0x003f0000
#define M85XX_PORPLLSR_DDR_RATIO_MASK       0x00003e00
#define M85XX_PORPLLSR_PLAT_RATIO_MASK      0x0000003e
#define M85XX_DEVDISR_TB0                   0x00004000
#define M85XX_DEVDISR_TB1                   0x00001000

/* Processor initialization register */

#define M85XX_EPICPIR(base)             (CAST(VUINT32 *)((base) + 0x41090))

/* Global Utilities Block Register */

#define M85XX_AUTORSTSR(base)           (CAST(VUINT32 *)((base) + 0xe009c))
#define READY_P1                        (0x01 << 14)

#define M85XX_EPICPIR_HRST_CORE0        0x00000001
#define M85XX_EPICPIR_HRST_CORE1        0x00000002

#define M85XX_PVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A0))
#define M85XX_SVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A4))

#define _PPC_BUCSR_FI                   0x200   /* Invalidate branch cache */
#define _PPC_BUCSR_E                    0x1     /* Enable branch prediction */

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

#define EPIC_DMA1_CHAN0_INT_NUM         4
#define EPIC_DMA1_CHAN0_INT_VEC         (EPIC_DMA1_CHAN0_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN1_INT_NUM         5
#define EPIC_DMA1_CHAN1_INT_VEC         (EPIC_DMA1_CHAN1_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN2_INT_NUM         6
#define EPIC_DMA1_CHAN2_INT_VEC         (EPIC_DMA1_CHAN2_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN3_INT_NUM         7
#define EPIC_DMA1_CHAN3_INT_VEC         (EPIC_DMA1_CHAN3_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_USB_INT_NUM                12
#define EPIC_USB_INT_VEC                (EPIC_USB_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_I2C_INT_NUM                27
#define EPIC_I2C_INT_VEC                (EPIC_I2C_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_SPI_INT_NUM                43
#define EPIC_SPI_INT_VEC                (EPIC_SPI_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_SDHC_INT_NUM               56
#define EPIC_SDHC_INT_VEC               (EPIC_SDHC_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)

/* Alternate Function Signal Multiplex Control Register */

#define M85XX_PMUXCR                    *((VUINT32 *)(CCSBAR + 0xE0060))

/* Enable Machine Check pin */

#define HID0_EMCP                       (1<<31)

/* Enable MAS7 Update for 36-bit phys */

#define HID0_ENMAS7                     (1<<7)

/* DUART */

#define DUART_REG_ADDR_INTERVAL         0x1

/* DMA registers base address and offsets */

#define DMA_CHAN_OFFSET                 0x80

/* m85xxFITimer */

#define FIT_CLK_RATE_MIN                (DEFAULT_SYSCLKFREQ /(1 << 21))
#define FIT_CLK_RATE_MAX                (DEFAULT_SYSCLKFREQ /(1 << 10))

/* eSDHC */

#define SD_DATA                         0x80000000
#define SDHC_CD                         0x40000000
#define SDHC_WP                         0x20000000

#define USB_PORT_EN                     0x01000000

/* SPI defines */

#define SPI_CS_CFG_INIT                 0x24171118
#define FLASH_ADRS                      NAND_BUFFER_BASE
#define NV_RAM_ADRS                     FLASH_ADRS

/* NAND Flash */

#define NAND_BUFFER_BASE                0xfff00000
#define NAND_PAGE_SIZE                  512
#define NAND_BLOCK_SIZE                 (16 * 1024)
#define NAND_PAGE_PER_BLOCK             32

/* make sure (NAND_BLOCKCOPY_COUNT)*(16k) > the iamge size !!*/

#define NAND_BLOCKCOPY_COUNT            64     
#define NAND_LOADER_ADRS                0x1ffff000

/* QUICC Engine */

#define QUICC_ENGINE_BA                 0x80000
#define INUM_QE_HIGH                    47+12
#define INUM_QE_LOW                     44+12
#define QE_IO_PORTS                     0xe0100
#define QE_MURAM_BA                     0x10000
#define QE_MURAM_POOL_BA                (QUICC_ENGINE_BASE + QE_MURAM_BA)
#define QE_SDMA_BASE                    (QUICC_ENGINE_BA + 0x4000)
#define QE_SDMR(base)                   (CAST(VUINT32 *)((base) + \
                                        QE_SDMA_BASE + 0x04))

/* QE interrupt vectors */

#define UCC1_BASE                       0x2000
#define UCC5_BASE                       0x2400
#define UCC7_BASE                       0x2600
#define QE_UCC1                         0x1
#define QE_UCC5                         0x5
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
#define QE_CMXUCR2_UCC5_RX_DIS          0x00000000
#define QE_CMXUCR2_UCC5_RX_BRG5         0x00100000
#define QE_CMXUCR2_UCC5_RX_BRG6         0x00200000
#define QE_CMXUCR2_UCC5_RX_BRG7         0x00300000
#define QE_CMXUCR2_UCC5_RX_BRG8         0x00400000
#define QE_CMXUCR2_UCC5_RX_CLK13        0x00500000
#define QE_CMXUCR2_UCC5_RX_CLK14        0x00600000
#define QE_CMXUCR2_UCC5_RX_CLK19        0x00700000
#define QE_CMXUCR2_UCC5_RX_CLK20        0x00800000
#define QE_CMXUCR2_UCC5_RX_CLK15        0x00900000
#define QE_CMXUCR2_UCC5_RX_CLK16        0x00A00000
#define QE_CMXUCR2_UCC5_TX_DIS          0x00000000
#define QE_CMXUCR2_UCC5_TX_BRG5         0x00010000
#define QE_CMXUCR2_UCC5_TX_BRG6         0x00020000
#define QE_CMXUCR2_UCC5_TX_BRG7         0x00030000
#define QE_CMXUCR2_UCC5_TX_BRG8         0x00040000
#define QE_CMXUCR2_UCC5_TX_CLK13        0x00050000
#define QE_CMXUCR2_UCC5_TX_CLK14        0x00060000
#define QE_CMXUCR2_UCC5_TX_CLK19        0x00070000
#define QE_CMXUCR2_UCC5_TX_CLK20        0x00080000
#define QE_CMXUCR2_UCC5_TX_CLK15        0x00090000
#define QE_CMXUCR2_UCC5_TX_CLK16        0x000A0000

/* QE mux UCC clock route registers */

#define QE_MUX                          (QUICC_ENGINE_BA + 0x400)
#define QE_CMXGCR(base)                 (CAST(VUINT32 *)((base) + QE_MUX ))
#define QE_CMXUCR1(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x10))
#define QE_CMXUCR2(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x14))
#define QE_CMXUCR3(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x18))
#define QE_CMXUCR4(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x1C))
#define QE_CMXUPCR(base)                (CAST(VUINT32 *)((base) + QE_MUX + 0x22))
#define MPC85xx_BCSR(reg)               (CAST(VUINT8 *)(CONFIG_SYS_BCSR_BASE + reg))

/* Implement conversion of addresses in the LBC */

#define CONFIG_SYS_LBC_LBCR             0x00080000
#define CONFIG_SYS_BCSR_BASE            0xf8000000
#define M85XX_BPTR(base)                (CAST(VUINT32 *)((base) + 0x20))
#define BPTR_EN                         0x80000000
#define BCSR6                           0x6
#define BCSR7                           0x7
#define BCSR8                           0x8
#define BCSR9                           0x9
#define BCSR10                          0xa
#define BCSR11                          0xb
#define BCSR6_QESPI_IIC2                0x40
#define BCSR6_LBC_UPC                   0x20
#define BCSR6_TDMC_ENET5                0x08
#define BCSR6_TDMD_UART1                0x04
#define BCSR6_TDMB_ENET1                0x01
#define BCSR7_GETH1_EN                  0x80
#define BCSR7_GETH3_EN                  0x40
#define BCSR8_RS232_EN                  0x80
#define BCSR8_SDHC8_ESPI                0x40
#define BCSR8_PCIE_CLK_DIS              0x20
#define BCSR8_BRD_WP                    0x08
#define BCSR9_LED_CFG                   0x80
#define BCSR9_LED1                      0x40
#define BCSR9_LED2                      0x20
#define BCSR9_LED3                      0x10
#define BCSR11_UEM_MARVEL_RESET         0x40
#define BCSR11_ENET_MICRST              0x20
#define MPC85xx_PMUXCR_OFFSET           0x60
#define MPC85xx_PMUXCR_SD_DATA          0x80000000
#define MPC85xx_PMUXCR_SDHC_CD          0x40000000
#define MPC85xx_PMUXCR_SDHC_WP          0x20000000
#define MPC85xx_PMUXCR_QE0              0x00008000
#define MPC85xx_PMUXCR_QE1              0x00004000
#define MPC85xx_PMUXCR_QE2              0x00002000
#define MPC85xx_PMUXCR_QE3              0x00001000
#define MPC85xx_PMUXCR_QE4              0x00000800
#define MPC85xx_PMUXCR_QE5              0x00000400
#define MPC85xx_PMUXCR_QE6              0x00000200
#define MPC85xx_PMUXCR_QE7              0x00000100
#define MPC85xx_PMUXCR_QE8              0x00000080
#define MPC85xx_PMUXCR_QE9              0x00000040
#define MPC85xx_PMUXCR_QE10             0x00000020
#define MPC85xx_PMUXCR_QE11             0x00000010
#define MPC85xx_PMUXCR_QE12             0x00000008
#define QE_CMXGCR_MII_ENET_MNG_MASK     0x00007000
#define QE_CMXGCR_MII_ENET_MNG_SHIFT    12
#define QE_CMXGCR_MEM_MASK              0x00007000

/* QE interrupt source for UCC1,UCC5,UCC7 */

#define QE_IC_INTR_UCC_1                32
#define QE_IC_INTR_UCC_5                40
#define QE_IC_INTR_UCC_7                42

/* QE mux UCC clock route registers */

#define QE_CMXGCR_MEM_MASK              0x00007000

/* 2 UCCs for per CMXUCR */

#define QE_CMXUCR_HIGH_MASK             0xffff0000
#define QE_CMXUCR_LOW_MASK              0x0000ffff
#define QE_CMXUCR_SHIFT                 16      /* shift for high halfword  */
#define QE_CMXUCR_RUCS_SHIFT            4       /* Rx clock source shift    */
#define QE_CMXUCR_TUCS_SHIFT            0       /* Tx clock source shift    */
#define QE_CMXUCR_UCS_MASK              0xf     /* clock source select Mask */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCp1021mdsh */

