/* fsl_mpc8536_ds.h - Freescale MPC8536DS board header */

/*
 * Copyright (c) 2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,02apr11,y_c  Support VxWorks native bootrom. (WIND00240155)
01a,20oct08,b_m  written.
*/

#ifndef __INCfsl_mpc8536_dsh
#define __INCfsl_mpc8536_dsh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else   /* _ASMLANGUAGE */
typedef volatile UCHAR  VCHAR;
typedef volatile INT32  VINT32;
typedef volatile INT16  VINT16;
typedef volatile INT8   VINT8;
typedef volatile UINT32 VUINT32;
typedef volatile UINT16 VUINT16;
typedef volatile UINT8  VUINT8;
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#endif  /* M8260ABBREVIATIONS */

/* MPC8536E Processor Header */

#include "mpc8536e.h"

/* MPC8536DS Board Specific Definitions */

/* Clock Frequency */

#define OSCILLATOR_FREQ     33333000    /* 33MHz */
#define CCB_FREQ_DEFAULT    500000000   /* 500MHz */

/* Clock Rates */

#define SYS_CLK_RATE_MIN    1           /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000        /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1
#define AUX_CLK_RATE_MAX    50000
#define AUX_CLK_RATE        20000

/* PCI defines */

#define NUM_PCI_SLOTS           1
#define NUM_PCIEX_SLOTS         1
#define PCI_LAT_TIMER           0x40

/* PCI memory size */

#define PCI_MEM_SIZE            0x04000000
#define PCI_MEMIO_SIZE          0x04000000
#define PCI_IO_SIZE             0x04000000

/* PCI address */

#define PCI_LAW_BASE            0x80000000
#define PCI_LAW_SIZE            0x10000000
#define PCI_LAW_WIN_SZ          LAWAR_SIZE_256MB

#define PCI_MEM_ADRS            (PCI_LAW_BASE)
#define PCI_MEMIO_ADRS          (PCI_MEM_ADRS   + PCI_MEM_SIZE)
#define PCI_IO_ADRS             (PCI_MEMIO_ADRS + PCI_MEMIO_SIZE)

/* PCI Express address */

#define PCIEX1_LAW_BASE         0x90000000
#define PCIEX1_LAW_SIZE         0x10000000
#define PCIEX1_LAW_WIN_SZ       LAWAR_SIZE_256MB

#define PCIEX1_MEM_ADRS         (PCIEX1_LAW_BASE)
#define PCIEX1_MEMIO_ADRS       (PCIEX1_MEM_ADRS   + PCI_MEM_SIZE)
#define PCIEX1_IO_ADRS          (PCIEX1_MEMIO_ADRS + PCI_MEMIO_SIZE)

#define PCIEX2_LAW_BASE         0xa0000000
#define PCIEX2_LAW_SIZE         0x10000000
#define PCIEX2_LAW_WIN_SZ       LAWAR_SIZE_256MB

#define PCIEX2_MEM_ADRS         (PCIEX2_LAW_BASE)
#define PCIEX2_MEMIO_ADRS       (PCIEX2_MEM_ADRS   + PCI_MEM_SIZE)
#define PCIEX2_IO_ADRS          (PCIEX2_MEMIO_ADRS + PCI_MEMIO_SIZE)

#define PCIEX3_LAW_BASE         0xb0000000
#define PCIEX3_LAW_SIZE         0x10000000
#define PCIEX3_LAW_WIN_SZ       LAWAR_SIZE_256MB

#define PCIEX3_MEM_ADRS         (PCIEX3_LAW_BASE)
#define PCIEX3_MEMIO_ADRS       (PCIEX3_MEM_ADRS   + PCI_MEM_SIZE)
#define PCIEX3_IO_ADRS          (PCIEX3_MEMIO_ADRS + PCI_MEMIO_SIZE)

#define PCI_MMU_TLB_SZ          _MMU_TLB_SZ_256M

/* CPU address */

#define PCI_MSTR_MEM_BUS            0x00000000
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* PIXIS registers (FPGA) */

#define PIXIS_BASE              0xefdf0000
#define PIXIS_SIZE              0x00008000

#define PIXIS_ID                (PIXIS_BASE + 0x00)
#define PIXIS_VER               (PIXIS_BASE + 0x01)
#define PIXIS_PVER              (PIXIS_BASE + 0x02)
#define PIXIS_CSR               (PIXIS_BASE + 0x03)
#define PIXIS_RST               (PIXIS_BASE + 0x04)
#define PIXIS_RST2              (PIXIS_BASE + 0x05)
#define PIXIS_AUX               (PIXIS_BASE + 0x06)
#define PIXIS_SPD               (PIXIS_BASE + 0x07)
#define PIXIS_AUX2              (PIXIS_BASE + 0x08)
#define PIXIS_CSR2              (PIXIS_BASE + 0x09)
#define PIXIS_WATCH             (PIXIS_BASE + 0x0A)
#define PIXIS_LED               (PIXIS_BASE + 0x0B)
#define PIXIS_PWR               (PIXIS_BASE + 0x0C)

/* reset control register */

#define PIXIS_RST_ALL           (~0x80)
#define PIXIS_RST_PCIE1         (~0x40)
#define PIXIS_RST_PCIE2         (~0x20)
#define PIXIS_RST_PCIE3         (~0x10)
#define PIXIS_RST_USB           (0x08)
#define PIXIS_RST_PHY           (~0x04)
#define PIXIS_RST_LB            (~0x02)
#define PIXIS_RST_GEN           (~0x01)

#define PIXIS_RST2_SGMII        (~0x02)
#define PIXIS_RST2_PCI          (~0x01)

/* SYSCLK frequency options */

#define PIXIS_SYSCLK_33_MHZ     0
#define PIXIS_SYSCLK_40_MHZ     1
#define PIXIS_SYSCLK_50_MHZ     2
#define PIXIS_SYSCLK_66_MHZ     3
#define PIXIS_SYSCLK_83_MHZ     4
#define PIXIS_SYSCLK_10_0MHZ    5
#define PIXIS_SYSCLK_13_3MHZ    6
#define PIXIS_SYSCLK_16_6MHZ    7

#define FREQ_33_MHZ             33333333
#define FREQ_40_MHZ             40000000
#define FREQ_50_MHZ             50000000
#define FREQ_66_MHZ             66666666
#define FREQ_83_MHZ             83333333
#define FREQ_100_MHZ            100000000
#define FREQ_133_MHZ            133333333
#define FREQ_166_MHZ            166666666

/* RTC freq */

#define RTC_FREQ                14318000

/* I2C RTC */

#define I2C_RTC_BUS             1
#define I2C_RTC_ADDR            0x68

/*
 *  Local Access Window Configuration
 *
 *  LAWBAR0: 0x00000000 - 0x1fffffff    DDR                 512M
 *  LAWBAR1: 0xf0000000 - 0xffffffff    PromJet/Flash       256M
 *  LAWBAR2: 0xefdf0000 - 0xefdf7fff    PIXIS                32K
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

/* LBC Clock Configuration */

#define M85XX_LBCR(base)        (CAST(VUINT32 *)((base) + 0x50d0))
#define M85XX_LCRR(base)        (CAST(VUINT32 *)((base) + 0x50d4))

#define M85XX_DCR0(base)        (CAST(VUINT32 *)((base) + 0xe0f1c))
#define M85XX_DCR1(base)        (CAST(VUINT32 *)((base) + 0xe0f20))

#define M85XX_PORPLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0000))

#define M85XX_DDRDLLCR(base)            (CAST(VUINT32 *)((base) + 0xE0E10))

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_mpc8536_dsh */
