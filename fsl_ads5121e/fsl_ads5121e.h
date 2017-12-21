/* fsl_ads5121e.h - Freescale ADS5121e board header */

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
01i,05aug09,e_d  add PATA_IOBA0 and PATA_IOBA1 define. (WIND00171937)
01h,22jun09,b_m  enable Elpida DDR for rev 4.x by default;
                 add AXE encoder decoder task defines.
01g,09mar09,b_m  add GPIO/AXE/SRAM/NAND defines.
01f,23feb09,x_f  add Elpida DDR SDRAM configuration defines.
01e,25dec08,x_f  add CAN, PCI and CPLD defines.
01d,25aug08,b_m  support rev 3.x hardware.
01c,24dec07,b_m  add IPIC defines.
01b,14nov07,b_m  add DMA/I2C/PATA/USB defines.
01a,30sep07,b_m  created.
*/

#ifndef __INCfsl_ads5121eh
#define __INCfsl_ads5121eh

#ifdef __cplusplus
extern "C" {
#endif

/* define to run on ads5121e rev.2 board */

#undef  ADS5121E_REV2

/*
 * There are two kinds of DDR SDRAM chips in PCB rev 4.x board. Define
 * INCLUDE_DDR_MICRON for Micron chip, undef INCLUDE_DDR_MICRON for Elpida
 * chip. Please choose the right one according to your board.
 */

#undef  INCLUDE_DDR_MICRON
#ifndef INCLUDE_DDR_MICRON
#   define INCLUDE_DDR_ELPIDA
#endif /* INCLUDE_DDR_MICRON */

#define MPC5121E_SILICON_REV1_5 0x0010
#define MPC5121E_SILICON_REV2   0x2010

/* Internal Memory Map Base Address */

#define IMMRBAR                 0x80000000
#define IMMRBAR_RESET_VALUE     0xFF400000

#define MBAR                    311     /* Memory Base Address Register */

#define HID0_CONFIG             0x80000000
#define HID2_CONFIG             0x00040000

/* Clock Parameters */

#define SYS_CLK_RATE_MIN        10      /* min system clock rate    */
#define SYS_CLK_RATE_MAX        5000    /* max system clock rate    */
#define AUX_CLK_RATE_MIN        30      /* min auxiliary clock rate */
#define AUX_CLK_RATE_MAX        10000   /* max auxiliary clock rate */

/* Input Clock Freq */

#ifdef ADS5121E_REV2
#   define SYS_OSC_FREQ         66000000L   /* 66 MHz */
#else
#   define SYS_OSC_FREQ         33333333L   /* 33 MHz */
#endif

/* PCI configurations */

#define M83XX_PCILAWBARn(base, n)   (CAST(VUINT32 *)((base) + 0x60 + (n * 0x8)))
#define M83XX_PCILAWARn(base, n)    (CAST(VUINT32 *)((base) + 0x64 + (n * 0x8)))
#define PCICFG_REG_BA           0x08300

/* configuration space reg and int ack */

#define PCI_REG_BASE            0x8000

#define PCI_LAWAR_ENABLE        0x80000000
#define PCI_ATTR_IO_BIT         0x40000000

#define PCI_IN_ATTR_WS_256M     0x0000001b
#define PCI_IN_ATTR_WS_512M     0x0000001c

#define PCI_OUT_ATTR_WS_128M    0x000F8000
#define PCI_OUT_ATTR_WS_64M     0x000FC000

#define PCI_LAWAR_SIZE_256MB    0x0000001B

#define LAWBAR_ADRS_SHIFT       0
#define LAWAR_ENABLE            0x80000000

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

/* PCI Window SIZE  */

#define PCI_SIZE_4KB            0x0000000B
#define PCI_SIZE_8KB            0x0000000C
#define PCI_SIZE_16KB           0x0000000D
#define PCI_SIZE_32KB           0x0000000E
#define PCI_SIZE_64KB           0x0000000F
#define PCI_SIZE_128KB          0x00000010
#define PCI_SIZE_256KB          0x00000011
#define PCI_SIZE_512KB          0x00000012
#define PCI_SIZE_1MB            0x00000013
#define PCI_SIZE_2MB            0x00000014
#define PCI_SIZE_4MB            0x00000015
#define PCI_SIZE_8MB            0x00000016
#define PCI_SIZE_16MB           0x00000017
#define PCI_SIZE_32MB           0x00000018
#define PCI_SIZE_64MB           0x00000019
#define PCI_SIZE_128MB          0x0000001A
#define PCI_SIZE_256MB          0x0000001B
#define PCI_SIZE_512MB          0x0000001C
#define PCI_SIZE_1GB            0x0000001D
#define PCI_SIZE_2GB            0x0000001E

/* PCI latency timer */

#define PCI_LAT_TIMER           0x40

/* PCI slots */

#define NUM_PCI_SLOTS           3

#define PCI_BRIDGE_DEVNO        20
#define PCI_SLOT1_DEVNO         21
#define PCI_SLOT2_DEVNO         22
#define PCI_SLOT3_DEVNO         23

/* CPLD interrupts */

#define RTC_INT                 0
#define PCI_INT                 1
#define TEMP_MON_INT            2
#define FEC_PHY_INTN            3
#define TOUCH_SCR_IRQN          4
#define TOUCH_SCR_BUSYN         5
#define SD_CD                   6
#define SW1_HIBERNATE           7

/* PCI interrupts */

#define PCI_INT_VEC_OFF         8

#define PCI_INTA_SLOT1          (PCI_INT_VEC_OFF + 0)
#define PCI_INTB_SLOT1          (PCI_INT_VEC_OFF + 1)
#define PCI_INTC_SLOT1          (PCI_INT_VEC_OFF + 2)
#define PCI_INTD_SLOT1          (PCI_INT_VEC_OFF + 3)
#define PCI_INTA_SLOT2          (PCI_INT_VEC_OFF + 4)
#define PCI_INTB_SLOT2          (PCI_INT_VEC_OFF + 5)
#define PCI_INTA_SLOT3          (PCI_INT_VEC_OFF + 6)
#define PCI_INTB_SLOT3          (PCI_INT_VEC_OFF + 7)

/* PCI addresses */

#define CPU_PCI_MEM_ADRS        PCI_MEM_ADRS
#define CPU_PCI_MEMIO_ADRS      PCI_MEMIO_ADRS
#define CPU_PCI_IO_ADRS         PCI_IO_ADRS

#define CPU_PCI_MEM_SIZE        PCI_MEM_SIZE
#define CPU_PCI_MEMIO_SIZE      PCI_MEMIO_SIZE
#define CPU_PCI_IO_SIZE         PCI_IO_SIZE

/* PCI base addresses */

#define PCI_MEM_ADRS            0x90000000
#define PCI_MEM_SIZE            0x08000000      /* 128MB */

/* This should match PCI_MEM_SIZE */

#define PCI_MEMIO_ADRS          0x98000000
#define PCI_MEMIO_SIZE          0x04000000      /* 64MB */

/* This should match PCI_MEMIO_SIZE */

#define PCI_IO_ADRS             0x9c000000
#define PCI_IO_SIZE             0x04000000
#define CFG_PCI_IO_BASE         0x00000000
#define CFG_PCI_IO_PHYS         0x9c000000
#define CFG_PCI_IO_SIZE         0x04000000      /* 64M */

/* CPU addresses from PCI bus */

#define PCI_MSTR_MEM_BUS        0x00000000
#define PCI_MSTR_MEM_SIZE       PCI_LOCAL_MEM_SIZE

#define PCI_BRIDGE_PIMMR_BASE_ADRS   0x50000000

/* CPU address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS       LOCAL_MEM_LOCAL_ADRS
#define PCI_LOCAL_MEM_SIZE      LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

/* LPC global enable CS bit */

#define CS_GLOBAL_ENABLE        0x01000000

/* Base Addresses */

#define FLASH_BASE_ADRS         0xFC000000

/* CS0 Flash */

#define CS0_START_ADRS          0xFC000000
#define CS0_STOP_ADRS           0xFFFFFFFF
#define CS0_CONFIG              0x05059310

/* CS2 CPLD */

#define CS2_START_ADRS          0x82000000
#define CS2_STOP_ADRS           0x8200FFFF
#define CS2_CONFIG              0x05059010

/* SOC Module Registers */

/* XLBMEN module */

#define XLBMEN_OFF              0x00000
#define XLBMEN_BA               (IMMRBAR + XLBMEN_OFF)

#define XLBMEN_LPBAW            (XLBMEN_BA + 0x20)
#define XLBMEN_LPCSAW0          (XLBMEN_BA + 0x24)
#define XLBMEN_LPCSAW1          (XLBMEN_BA + 0x28)
#define XLBMEN_LPCSAW2          (XLBMEN_BA + 0x2C)
#define XLBMEN_LPCSAW3          (XLBMEN_BA + 0x30)
#define XLBMEN_LPCSAW4          (XLBMEN_BA + 0x34)
#define XLBMEN_LPCSAW5          (XLBMEN_BA + 0x38)
#define XLBMEN_LPCSAW6          (XLBMEN_BA + 0x3C)
#define XLBMEN_LPCSAW7          (XLBMEN_BA + 0x40)
#define XLBMEN_PCILAWBAR0       (XLBMEN_BA + 0x60)
#define XLBMEN_PCILAWAR0        (XLBMEN_BA + 0x64)
#define XLBMEN_PCILAWBAR1       (XLBMEN_BA + 0x68)
#define XLBMEN_PCILAWAR1        (XLBMEN_BA + 0x6C)
#define XLBMEN_PCILAWBAR2       (XLBMEN_BA + 0x70)
#define XLBMEN_PCILAWAR2        (XLBMEN_BA + 0x74)
#define XLBMEN_DDRLAWBAR        (XLBMEN_BA + 0xA0)
#define XLBMEN_DDRLAWAR         (XLBMEN_BA + 0xA4)
#define XLBMEN_MBXBAR           (XLBMEN_BA + 0xC0)
#define XLBMEN_SRAMBAR          (XLBMEN_BA + 0xC4)
#define XLBMEN_NFCBAR           (XLBMEN_BA + 0xC8)
#define XLBMEN_SPCR             (XLBMEN_BA + 0x104)

/* LAWAR settings */

#define LAWAR_ENABLE            0x80000000

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

/* general purpose timer */

#define GPT_OFF                 0x00B00
#define GPT_BA(n)               (IMMRBAR + GPT_OFF + (n) * 0x10)

/* IPIC */

#define IPIC_OFF                0x00C00
#define IPIC_BA                 (IMMRBAR + IPIC_OFF)

/* interrupt vectors */

#define INUM_NONE               0
#define INUM_PCI                1
#define INUM_PCI_DMA            2
#define INUM_PCI_MU             3
#define INUM_FEC                4
#define INUM_PATA               5
#define INUM_NFC                6
#define INUM_LPC                7
#define INUM_SDHC               8
#define INUM_I2C1               9
#define INUM_I2C2               10
#define INUM_I2C3               11
#define INUM_MSCAN1             12
#define INUM_MSCAN2             13
#define INUM_BDLC               14
#define INUM_GPT0               15
#define INUM_GPT1               16
#define INUM_IRQ1               17
#define INUM_PSC4               32
#define INUM_PSC5               33
#define INUM_PSC6               34
#define INUM_PSC7               35
#define INUM_PSC8               36
#define INUM_PSC9               37
#define INUM_PSC10              38
#define INUM_PSC11              39
#define INUM_FIFOC              40
#define INUM_SPDIF              41
#define INUM_AXE                42
#define INUM_USB_ULPI           43
#define INUM_USB_UTMI           44
#define INUM_SATA               45
#define INUM_IRQ0               48
#define INUM_DIU                64
#define INUM_DMA2               65
#define INUM_MBX                66
#define INUM_PSC0               68
#define INUM_PSC1               69
#define INUM_PSC2               70
#define INUM_PSC3               71
#define INUM_GPT2               72
#define INUM_GPT3               73
#define INUM_GPT4               74
#define INUM_GPT5               75
#define INUM_GPT6               76
#define INUM_GPT7               77
#define INUM_GPIO               78
#define INUM_RTC_SEC            79
#define INUM_RTC_ALARM          80
#define INUM_DDR                81
#define INUM_SBA                82
#define INUM_PMC                83
#define INUM_USB_ULPI_WKUP      84
#define INUM_USB_UTMI_WKUP      85
#define INUM_SATA_CMD           86
#define INUM_TEMP_15C           87
#define INUM_IIM                88
#define INUM_PRIOMON            89

/* CSB arbiter & bus monitor */

#define ARBITER_OFF             0x00D00
#define ARBITER_BA              (IMMRBAR + ARBITER_OFF)

#define ARBITER_ACR             (ARBITER_BA + 0x00)
#define ARBITER_ATR             (ARBITER_BA + 0x04)
#define ARBITER_ATER            (ARBITER_BA + 0x08)
#define ARBITER_AER             (ARBITER_BA + 0x0C)
#define ARBITER_AIDR            (ARBITER_BA + 0x10)
#define ARBITER_AMR             (ARBITER_BA + 0x14)
#define ARBITER_AEATR           (ARBITER_BA + 0x18)
#define ARBITER_AEADR           (ARBITER_BA + 0x1C)
#define ARBITER_AERR            (ARBITER_BA + 0x20)

/* reset module */

#define RESET_OFF               0x00E00
#define RESET_BA                (IMMRBAR + RESET_OFF)

#define RESET_RCWLR             (RESET_BA + 0x00)
#define RESET_RCWHR             (RESET_BA + 0x04)
#define RESET_RSR               (RESET_BA + 0x10)
#define RESET_RMR               (RESET_BA + 0x14)
#define RESET_RPR               (RESET_BA + 0x18)
#define RESET_RCR               (RESET_BA + 0x1C)
#define RESET_RCER              (RESET_BA + 0x20)

/* clock module */

#define CLOCK_OFF               0x00F00
#define CLOCK_BA                (IMMRBAR + CLOCK_OFF)

#define CLOCK_SPMR              (CLOCK_BA + 0x00)
#define CLOCK_SCCR1             (CLOCK_BA + 0x04)
#define CLOCK_SCCR2             (CLOCK_BA + 0x08)
#define CLOCK_SCFR1             (CLOCK_BA + 0x0C)
#define CLOCK_SCFR2             (CLOCK_BA + 0x10)
#define CLOCK_BCR               (CLOCK_BA + 0x18)
#define CLOCK_PSCCCR(n)         (CLOCK_BA + 0x1C + (n) * 4)
#define CLOCK_SPCCR             (CLOCK_BA + 0x4C)
#define CLOCK_CCCR              (CLOCK_BA + 0x50)
#define CLOCK_DCCR              (CLOCK_BA + 0x54)

#define SCFR1_PCI_DIV           0x6
#define SCFR1_PCI_DIV_MASK      0x00700000
#define SCFR1_PCI_DIV_SHIFT     20

/* MSCAN Clock Control Register (only rev2 silicon) */

#define CLOCK_M1CCR             (CLOCK_BA + 0x58)
#define CLOCK_M2CCR             (CLOCK_BA + 0x5c)
#define CLOCK_M3CCR             (CLOCK_BA + 0x60)
#define CLOCK_M4CCR             (CLOCK_BA + 0x64)

/* System Clock Control Register 1 */

#define CLOCK_CFG_EN            0x80000000
#define CLOCK_LPC_EN            0x40000000
#define CLOCK_NFC_EN            0x20000000
#define CLOCK_PATA_EN           0x10000000
#define CLOCK_PSC0_EN           0x08000000
#define CLOCK_PSC1_EN           0x04000000
#define CLOCK_PSC2_EN           0x02000000
#define CLOCK_PSC3_EN           0x01000000
#define CLOCK_PSC4_EN           0x00800000
#define CLOCK_PSC5_EN           0x00400000
#define CLOCK_PSC6_EN           0x00200000
#define CLOCK_PSC7_EN           0x00100000
#define CLOCK_PSC8_EN           0x00080000
#define CLOCK_PSC9_EN           0x00040000
#define CLOCK_PSC10_EN          0x00020000
#define CLOCK_PSC11_EN          0x00010000
#define CLOCK_PSC_FIFO_EN       0x00008000
#define CLOCK_SATA_EN           0x00004000
#define CLOCK_FEC_EN            0x00002000
#define CLOCK_TPR_EN            0x00001000
#define CLOCK_PCI_EN            0x00000800
#define CLOCK_DDR_EN            0x00000400

/* System Clock Control Register 2 */

#define CLOCK_DIU_EN            0x80000000
#define CLOCK_AXE_EN            0x40000000
#define CLOCK_MEM_EN            0x20000000
#define CLOCK_USB1_EN           0x10000000
#define CLOCK_USB2_EN           0x08000000
#define CLOCK_I2C_EN            0x04000000
#define CLOCK_BDLC_EN           0x02000000
#define CLOCK_SDHC_EN           0x01000000
#define CLOCK_SPDIF_EN          0x00800000
#define CLOCK_MBX_BUS_EN        0x00400000
#define CLOCK_MBX_EN            0x00200000
#define CLOCK_MBX_3D_EN         0x00100000
#define CLOCK_IIM_EN            0x00080000

/* GPIO */

#define GPIO_OFF                0x01100
#define GPIO_BA                 (IMMRBAR + GPIO_OFF)

#define GPIO_DIR                (GPIO_BA + 0x00)
#define GPIO_ODR                (GPIO_BA + 0x04)
#define GPIO_DAT                (GPIO_BA + 0x08)
#define GPIO_IER                (GPIO_BA + 0x0c)
#define GPIO_IMR                (GPIO_BA + 0x10)
#define GPIO_ICR1               (GPIO_BA + 0x14)
#define GPIO_ICR2               (GPIO_BA + 0x18)

/* I2C */

#define I2C_OFF(n)              (0x01700 + (n) * 0x20)
#define I2C_BA(n)               (IMMRBAR + I2C_OFF(n))

#define I2C_ADDR(n)             (I2C_BA(n) + 0x00)
#define I2C_FREQ(n)             (I2C_BA(n) + 0x04)
#define I2C_CTRL(n)             (I2C_BA(n) + 0x08)
#define I2C_STATUS(n)           (I2C_BA(n) + 0x0c)
#define I2C_DATA(n)             (I2C_BA(n) + 0x10)

#define I2C_INT_CTRL            (I2C_BA(0) + 0x60)
#define I2C_FILTER              (I2C_BA(0) + 0x64)

/* AXE */

#define AXE_OFF                 0x02000
#define AXE_BA                  (IMMRBAR + AXE_OFF)

#define AXE_RSVD_DRAM_BASE      0x00100000
#define AXE_RSVD_DRAM_SIZE      0x00100000

/* AXE task defines */

#define AXE_TASK_ENC_MP3        0   /* MP3 encode */
#define AXE_TASK_ENC_MP2        1   /* MP2 encode */
#define AXE_TASK_ENC_OV         2   /* OGG encode */
#define AXE_TASK_ENC_NUM        3

#define AXE_TASK_DEC_MP3        0   /* MP3 decode */
#define AXE_TASK_DEC_MP2        1   /* MP2 decode */
#define AXE_TASK_DEC_AAC        2   /* AAC decode */
#define AXE_TASK_DEC_WMA        3   /* WMA decode */
#define AXE_TASK_DEC_OV         4   /* OGG decode */
#define AXE_TASK_DEC_NUM        5

/* FEC */

#define FEC_OFF                 0x02800
#define FEC_BA                  (IMMRBAR + FEC_OFF)

/* USB */

#define USB0_OFF                0x04000
#define USB0_BA                 (IMMRBAR + USB0_OFF)

#define USB0_MODE               (USB0_BA + 0x1A8)
#define USB0_GEN_CTRL           (USB0_BA + 0x200)
#define USB0_OCPHY_CTRL         (USB0_BA + 0x204)

/* USB controller mode */

#define USB_MODE_IDLE           0x00000000
#define USB_MODE_DEVICE         0x00000002
#define USB_MODE_HOST           0x00000003

/* USB general control */

#define USB0_SEL_ULPI           0x00000010
#define USB0_SEL_UTMI           0x00000000
#define USB0_PPP                0x00000008
#define USB0_PFP                0x00000004

/* USB on-chip PHY control */

#define USB_PHY_EN              0x00000010
#define USB_PHY_CLK_EN          0x00000001

/* PCI host controller */

#define PCICFG_OFF              0x08300
#define PCICFG_BA               (IMMRBAR + PCICFG_OFF)

#define PCI_CFG_ADR_REG         (PCICFG_BA + 0x00)
#define PCI_CFG_DATA_REG        (PCICFG_BA + 0x04)
#define PCI_INT_ACK             (PCICFG_BA + 0x08)

/* multi-port DRAM controller */

#define MDDRC_OFF               0x09000
#define MDDRC_BA                (IMMRBAR + MDDRC_OFF)

#define MDDRC_DDR_SYS_CFG       (MDDRC_BA + 0x00)
#define MDDRC_DDR_TIME_CFG0     (MDDRC_BA + 0x04)
#define MDDRC_DDR_TIME_CFG1     (MDDRC_BA + 0x08)
#define MDDRC_DDR_TIME_CFG2     (MDDRC_BA + 0x0C)
#define MDDRC_DDR_CMD           (MDDRC_BA + 0x10)
#define MDDRC_DDR_COMPACT_CMD   (MDDRC_BA + 0x14)
#define MDDRC_SELF_REFRESH_CMD0 (MDDRC_BA + 0x18)
#define MDDRC_SELF_REFRESH_CMD1 (MDDRC_BA + 0x1C)
#define MDDRC_SELF_REFRESH_CMD2 (MDDRC_BA + 0x20)
#define MDDRC_SELF_REFRESH_CMD3 (MDDRC_BA + 0x24)
#define MDDRC_SELF_REFRESH_CMD4 (MDDRC_BA + 0x28)
#define MDDRC_SELF_REFRESH_CMD5 (MDDRC_BA + 0x2C)
#define MDDRC_SELF_REFRESH_CMD6 (MDDRC_BA + 0x30)
#define MDDRC_SELF_REFRESH_CMD7 (MDDRC_BA + 0x34)
#define MDDRC_DQS_CFG_OFF_CNT   (MDDRC_BA + 0x38)
#define MDDRC_DQS_CFG_OFF_TIME  (MDDRC_BA + 0x3C)
#define MDDRC_DQS_DELAY_STATUS  (MDDRC_BA + 0x40)

/* multi-port DRAM priority manager */

#define MDDRC_PRIOMAN_CFG1      (MDDRC_BA + 0x80)
#define MDDRC_PRIOMAN_CFG2      (MDDRC_BA + 0x84)
#define MDDRC_HIPRIO_CFG        (MDDRC_BA + 0x88)
#define MDDRC_LUT0_MAIN_UPPER   (MDDRC_BA + 0x8C)
#define MDDRC_LUT1_MAIN_UPPER   (MDDRC_BA + 0x90)
#define MDDRC_LUT2_MAIN_UPPER   (MDDRC_BA + 0x94)
#define MDDRC_LUT3_MAIN_UPPER   (MDDRC_BA + 0x98)
#define MDDRC_LUT4_MAIN_UPPER   (MDDRC_BA + 0x9C)
#define MDDRC_LUT0_MAIN_LOWER   (MDDRC_BA + 0xA0)
#define MDDRC_LUT1_MAIN_LOWER   (MDDRC_BA + 0xA4)
#define MDDRC_LUT2_MAIN_LOWER   (MDDRC_BA + 0xA8)
#define MDDRC_LUT3_MAIN_LOWER   (MDDRC_BA + 0xAC)
#define MDDRC_LUT4_MAIN_LOWER   (MDDRC_BA + 0xB0)
#define MDDRC_LUT0_ALT_UPPER    (MDDRC_BA + 0xB4)
#define MDDRC_LUT1_ALT_UPPER    (MDDRC_BA + 0xB8)
#define MDDRC_LUT2_ALT_UPPER    (MDDRC_BA + 0xBC)
#define MDDRC_LUT3_ALT_UPPER    (MDDRC_BA + 0xC0)
#define MDDRC_LUT4_ALT_UPPER    (MDDRC_BA + 0xC4)
#define MDDRC_LUT0_ALT_LOWER    (MDDRC_BA + 0xC8)
#define MDDRC_LUT1_ALT_LOWER    (MDDRC_BA + 0xCC)
#define MDDRC_LUT2_ALT_LOWER    (MDDRC_BA + 0xD0)
#define MDDRC_LUT3_ALT_LOWER    (MDDRC_BA + 0xD4)
#define MDDRC_LUT4_ALT_LOWER    (MDDRC_BA + 0xD8)

#ifdef ADS5121E_REV2

#   define DDR_MEM_SIZE         0x10000000      /* 256 MB */
#   define DDR_LAWAR_SIZE       LAWAR_SIZE_256MB

#   define DDR_SYS_CFG_VALUE    0xF8604A00
#   define DDR_SYS_CFG_RUN      0xE8604A00
#   define DDR_TIME_CFG1_VALUE  0x54EC1168
#   define DDR_TIME_CFG2_VALUE  0x35210864

#else /* ADS5121E_REV2 */

#   define DDR_MEM_SIZE         0x20000000      /* 512 MB */
#   define DDR_LAWAR_SIZE       LAWAR_SIZE_512MB
#   ifdef INCLUDE_DDR_MICRON
#       define DDR_SYS_CFG_VALUE    0xFA804A00
#       define DDR_SYS_CFG_RUN      0xEA804A00
#       define DDR_TIME_CFG1_VALUE  0x68EC1168
#       define DDR_TIME_CFG2_VALUE  0x34310864
#   else  /* INCLUDE_DDR_MICRON */
#       define DDR_SYS_CFG_VALUE    0xFA802B40
#       define DDR_SYS_CFG_RUN      0xEA802B40
#       define DDR_TIME_CFG1_VALUE  0x690E1189
#       define DDR_TIME_CFG2_VALUE  0x35410864
#   endif /* INCLUDE_DDR_MICRON */
#endif /* ADS5121E_REV2 */

#define DDR_SYS_CFG_EN          0xF0000000
#define DDR_TIME_CFG0_VALUE     0x00003D2E
#define DDR_TIME_CFG0_RUN       0x06183D2E

#define PRIOMAN_CFG1_VALUE      0x00077777
#define PRIOMAN_CFG2_VALUE      0x00000000
#define HIPRIO_CFG_VALUE        0x00000001

#define LUT0_MAIN_UPPER_VALUE   0xFFEEDDCC
#define LUT0_MAIN_LOWER_VALUE   0xBBAAAAAA
#define LUT1_MAIN_UPPER_VALUE   0x66666666
#define LUT1_MAIN_LOWER_VALUE   0x55555555
#define LUT2_MAIN_UPPER_VALUE   0x44444444
#define LUT2_MAIN_LOWER_VALUE   0x44444444
#define LUT3_MAIN_UPPER_VALUE   0x55555555
#define LUT3_MAIN_LOWER_VALUE   0x55555558
#define LUT4_MAIN_UPPER_VALUE   0x11111111
#define LUT4_MAIN_LOWER_VALUE   0x11111122

#define LUT0_ALT_UPPER_VALUE    0xAAAAAAAA
#define LUT0_ALT_LOWER_VALUE    0xAAAAAAAA
#define LUT1_ALT_UPPER_VALUE    0x66666666
#define LUT1_ALT_LOWER_VALUE    0x66666666
#define LUT2_ALT_UPPER_VALUE    0x11111111
#define LUT2_ALT_LOWER_VALUE    0x11111111
#define LUT3_ALT_UPPER_VALUE    0x11111111
#define LUT3_ALT_LOWER_VALUE    0x11111111
#define LUT4_ALT_UPPER_VALUE    0x11111111
#define LUT4_ALT_LOWER_VALUE    0x11111111

#define DDR_CMD_NOP             0x01380000
#define DDR_CMD_PRECHG_ALL      0x01100400
#define DDR_CMD_REFRESH         0x01080000
#ifdef INCLUDE_DDR_MICRON
#   define DDR_CMD_INIT_DEV_OP  0x01000432
#else  /* INCLUDE_DDR_MICRON */
#   define DDR_CMD_INIT_DEV_OP  0x01000842
#endif /* INCLUDE_DDR_MICRON */

#define DDR_CMD_EXMODE2         0x01020000
#define DDR_CMD_EXMODE3         0x01030000
#define DDR_CMD_EN_DLL          0x01010000
#define DDR_CMD_OCD_DEFAULT     0x01010780
#define DDR_CMD_OCD_EXIT        0x01010400
#define DDR_CMD_RES_DLL         0x01000932

/* IO control */

#define IOCTL_OFF               0x0A000
#define IOCTL_BA                (IMMRBAR + IOCTL_OFF)

#define IOCTL_MEM               (IOCTL_BA + 0x00)

#define IOCTL_PATA_CE1          (IOCTL_BA + 0xB8)
#define IOCTL_PATA_CE2          (IOCTL_BA + 0xBC)
#define IOCTL_PATA_ISOLATE      (IOCTL_BA + 0xC0)
#define IOCTL_PATA_IOR          (IOCTL_BA + 0xC4)
#define IOCTL_PATA_IOW          (IOCTL_BA + 0xC8)
#define IOCTL_PATA_IOCHRDY      (IOCTL_BA + 0xCC)
#define IOCTL_PATA_INTRQ        (IOCTL_BA + 0xD0)
#define IOCTL_PATA_DRQ          (IOCTL_BA + 0xD4)
#define IOCTL_PATA_DACK         (IOCTL_BA + 0xD8)

#define IOCTL_PCI_AD(n)         (IOCTL_BA + 0x170 - (n) * 4)
#define IOCTL_PCI_CBE0          (IOCTL_BA + 0x174)
#define IOCTL_PCI_CBE1          (IOCTL_BA + 0x178)
#define IOCTL_PCI_CBE2          (IOCTL_BA + 0x17C)
#define IOCTL_PCI_CBE3          (IOCTL_BA + 0x180)
#define IOCTL_PCI_GRANT2        (IOCTL_BA + 0x184)
#define IOCTL_PCI_REQ2          (IOCTL_BA + 0x188)
#define IOCTL_PCI_GRANT1        (IOCTL_BA + 0x18C)
#define IOCTL_PCI_REQ1          (IOCTL_BA + 0x190)
#define IOCTL_PCI_GRANT0        (IOCTL_BA + 0x194)
#define IOCTL_PCI_REQ0          (IOCTL_BA + 0x198)
#define IOCTL_PCI_INTA          (IOCTL_BA + 0x19C)
#define IOCTL_PCI_CLK           (IOCTL_BA + 0x1A0)
#define IOCTL_PCI_RST           (IOCTL_BA + 0x1A4)
#define IOCTL_PCI_FRAME         (IOCTL_BA + 0x1A8)
#define IOCTL_PCI_IDSEL         (IOCTL_BA + 0x1AC)
#define IOCTL_PCI_DEVSEL        (IOCTL_BA + 0x1B0)

#define IOCTL_SPDIF_TXCLK       (IOCTL_BA + 0x1CC)
#define IOCTL_SPDIF_TX          (IOCTL_BA + 0x1D0)
#define IOCTL_SPDIF_RX          (IOCTL_BA + 0x1D4)

#define IOCTL_CAN1_TX           (IOCTL_BA + 0x1F8)
#define IOCTL_CAN2_TX           (IOCTL_BA + 0x1FC)

#define IOCTL_PSC(no, pin)  \
        (IOCTL_BA + 0x20C + (no * 0x14) + (pin * 4))

#define IOCTL_FUNMUX_MASK       0x00000180
#define IOCTL_DS_MASK           0x00000003

#define IOCTL_FUNCMUX_FEC       0x00000080
#define IOCTL_DS_CLASS1         0x00000000
#define IOCTL_DS_CLASS2         0x00000001
#define IOCTL_DS_CLASS3         0x00000002
#define IOCTL_DS_CLASS4         0x00000003

/* LPC module */

#define LPC_OFF                 0x10000
#define LPC_BA                  (IMMRBAR + LPC_OFF)

#define LPC_CSCFGR0             (LPC_BA + 0x00)
#define LPC_CSCFGR1             (LPC_BA + 0x04)
#define LPC_CSCFGR2             (LPC_BA + 0x08)
#define LPC_CSCFGR3             (LPC_BA + 0x0C)
#define LPC_CSCFGR4             (LPC_BA + 0x10)
#define LPC_CSCFGR5             (LPC_BA + 0x14)
#define LPC_CSCFGR6             (LPC_BA + 0x18)
#define LPC_CSCFGR7             (LPC_BA + 0x1C)
#define LPC_CSCTLR              (LPC_BA + 0x20)
#define LPC_CSSR                (LPC_BA + 0x24)
#define LPC_CSBCR               (LPC_BA + 0x28)
#define LPC_CSDCR               (LPC_BA + 0x2C)
#define LPC_CSHCR               (LPC_BA + 0x30)
#define LPC_SCLPCPSR            (LPC_BA + 0x100)
#define LPC_SCLPCSAR            (LPC_BA + 0x104)
#define LPC_SCLPCCR             (LPC_BA + 0x108)
#define LPC_SCLPCER             (LPC_BA + 0x10C)
#define LPC_SCLPCNAR            (LPC_BA + 0x110)
#define LPC_SCLPCSR             (LPC_BA + 0x114)
#define LPC_SCLPCBDR            (LPC_BA + 0x118)
#define LPC_EMBSCR              (LPC_BA + 0x11C)
#define LPC_EMBPCR              (LPC_BA + 0x120)
#define LPC_FIFODWR             (LPC_BA + 0x140)
#define LPC_FIFOSR              (LPC_BA + 0x144)
#define LPC_FIFOCR              (LPC_BA + 0x148)
#define LPC_FIFOAR              (LPC_BA + 0x14C)

#define CS_CTRL_ME              0x01000000  /* CS Master Enable bit */
#define CS_CTRL_IE              0x08000000  /* CS Interrupt Enable bit */

/* PATA */

#define PATA_OFF                0x10200
#define PATA_BA                 (IMMRBAR + PATA_OFF)
#define PATA_IOBA0              (PATA_BA + 0xA0)
#define PATA_IOBA1              (PATA_BA + 0xD8)

#define PATA_TIME1              (PATA_BA + 0x00)
#define PATA_TIME2              (PATA_BA + 0x04)
#define PATA_TIME3              (PATA_BA + 0x08)
#define PATA_TIME4              (PATA_BA + 0x0C)
#define PATA_TIME5              (PATA_BA + 0x10)
#define PATA_TIME6              (PATA_BA + 0x14)
#define PATA_FIFO_DATA32        (PATA_BA + 0x18)
#define PATA_FIFO_DATA16        (PATA_BA + 0x1C)
#define PATA_FIFO_FILL          (PATA_BA + 0x20)
#define PATA_CTRL               (PATA_BA + 0x24)
#define PATA_INT_PEND           (PATA_BA + 0x28)
#define PATA_INT_EN             (PATA_BA + 0x2C)
#define PATA_INT_CLR            (PATA_BA + 0x30)
#define PATA_FIFO_ALARM         (PATA_BA + 0x34)

/* PATA driver */

#define SYS_ATA_MAX_CTRLS       1
#undef  USE_ATA_BLOCK_WRAPPER

/*
 * The ATA_DEVx_STATE determines whether the ATA driver should probe
 * for a device.
 *
 * ATA_DEV_PRESENT     = probe for the device;
 * ATA_DEV_NOT_PRESENT = don't probe for the device.
 *
 * To probe for all devices connect to both buses,
 * change all ATA_DEVx_STATE values to DEV_PRESENT.
 *
 *  ATA_DEV0_STATE = cntlr 0 / device 0
 *  ATA_DEV1_STATE = cntlr 0 / device 1
 */

#define ATA_DEV_PRESENT         1   /* drive does exist */
#define ATA_DEV_NOT_PRESENT     0   /* drive does not exist */

#define ATA_DEV0_STATE          ATA_DEV_PRESENT
#define ATA_DEV1_STATE          ATA_DEV_PRESENT

/*
 * Below we use the pre-processor to determine the number of ATA devices
 * to probe on each controller.  This is setup by the user above.
 * If the ATA_DEVx_STATE is ATA_DEV_PRESENT then the drive is probed.
 */

#if ((ATA_DEV0_STATE == ATA_DEV_PRESENT) && (ATA_DEV1_STATE == ATA_DEV_PRESENT))
#   define ATA_CTRL0_DRIVES 2
#elif ((ATA_DEV0_STATE == ATA_DEV_PRESENT) && (ATA_DEV1_STATE == ATA_DEV_NOT_PRESENT))
#   define ATA_CTRL0_DRIVES 1
#elif ((ATA_DEV0_STATE == ATA_DEV_NOT_PRESENT) && (ATA_DEV1_STATE == ATA_DEV_NOT_PRESENT))
#   define ATA_CTRL0_DRIVES 0
#endif

/* PSC module */

#define PSC_OFF                 0x11000
#define PSC_BA(n)               (IMMRBAR + PSC_OFF + (n) * 0x100)

/* FIFOC */

#define FIFOC_BA                PSC_BA(0)

/* UART0 @ PSC3 */

#define UART0_BA                PSC_BA(3)

/* UART1 @ PSC4 */

#define UART1_BA                PSC_BA(4)

/* AUDIO @ PCS5 */

#define AUDIO_BA                PSC_BA(5)

/* DMA */

#define DMA_OFF                 0x14000
#define DMA_BA                  (IMMRBAR + DMA_OFF)

/* board configuration CPLD register defines */

#define CPLD_BASE_ADRS          0x82000000
#define CPLD_SIZE               0x00100000

#define CPLD_BOARD_ID_UPPER     (CPLD_BASE_ADRS + 0x00)
#define CPLD_BOARD_ID_LOWER     (CPLD_BASE_ADRS + 0x01)
#define CPLD_CPLD_REV           (CPLD_BASE_ADRS + 0x02)
#define CPLD_RST_CONF_MISC      (CPLD_BASE_ADRS + 0x03)
#define CPLD_RST_CONF_SW6       (CPLD_BASE_ADRS + 0x04)
#define CPLD_RST_CONF_SW5       (CPLD_BASE_ADRS + 0x05)
#define CPLD_RST_CONF_SW4       (CPLD_BASE_ADRS + 0x06)
#define CPLD_RST_CONF_SW3       (CPLD_BASE_ADRS + 0x07)
#define CPLD_NOR_FLASH_CTL      (CPLD_BASE_ADRS + 0x08)
#define CPLD_NAND_FLASH_CTL     (CPLD_BASE_ADRS + 0x09)
#define CPLD_PCI_INT_MASK       (CPLD_BASE_ADRS + 0x0A)
#define CPLD_PCI_INT_STATUS     (CPLD_BASE_ADRS + 0x0B)
#define CPLD_IRQ_ROUTE          (CPLD_BASE_ADRS + 0x0C)
#define CPLD_DEV_INT_MASK       (CPLD_BASE_ADRS + 0x0D)
#define CPLD_DEV_INT_STATUS     (CPLD_BASE_ADRS + 0x0E)
#define CPLD_DEV_MISC_CTL       (CPLD_BASE_ADRS + 0x0F)
#define CPLD_DEV_VIDEO_CTL      (CPLD_BASE_ADRS + 0x10)

/* NOR Flash Write Protect Control */

#define CPLD_FLASH_BOOT_WP_FULL_EN          0x20
#define CPLD_FLASH_BOOT_WP_FULL_DIS         0x00
#define CPLD_FLASH_BOOT_WP_SECTOR_EN        0x00
#define CPLD_FLASH_BOOT_WP_SECTOR_DIS       0x10
#define CPLD_FLASH_BACKUP_WP_FULL_EN        0x80
#define CPLD_FLASH_BACKUP_WP_FULL_DIS       0x00
#define CPLD_FLASH_BACKUP_WP_SECTOR_EN      0x00
#define CPLD_FLASH_BACKUP_WP_SECTOR_DIS     0x40
#define CPLD_FLASH_BOOT_RESET               0x01
#define CPLD_FLASH_BACKUP_RESET             0x02

/* SRAM */

#define SRAM_BASE_ADRS          0x30000000
#define SRAM_SIZE               0x00020000

/* NAND */

#define NAND_BASE_ADRS          0x40000000
#define NAND_SIZE               0x00100000

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_ads5121eh */
