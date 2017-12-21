/* mpc8536e.h - Freescale MPC8536E header file */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,02apr11,y_c  Support VxWorks native bootrom. (WIND00240155)
01d,02jul10,b_m  add SDHC_WP polarity configuration register.
01c,05jun10,sye  Add support to eSDHC.
01b,10feb09,b_m  update openPicTimer support.
01a,20oct08,b_m  written.
*/

#ifndef __INCmpc8536eh
#define __INCmpc8536eh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* e500 core registers */

/* HID defines */

#define HID0_MCP                    0x80000000
#define HID1_ABE                    0x00001000
#define HID1_ASTME                  0x00002000
#define HID1_RXFE                   0x00020000

/* CCSR */

#define CCSBAR_RESET                0xff700000
#define CCSR_BASE                   0xe0000000
#define CCSR_SIZE                   0x00100000

#define CCSBAR                      CCSR_BASE

/* Local Access Window Configuration */

#define LAWBAR0                     (CCSR_BASE + 0x00c08)
#define LAWAR0                      (CCSR_BASE + 0x00c10)
#define LAWBAR1                     (CCSR_BASE + 0x00c28)
#define LAWAR1                      (CCSR_BASE + 0x00c30)
#define LAWBAR2                     (CCSR_BASE + 0x00c48)
#define LAWAR2                      (CCSR_BASE + 0x00c50)
#define LAWBAR3                     (CCSR_BASE + 0x00c68)
#define LAWAR3                      (CCSR_BASE + 0x00c70)
#define LAWBAR4                     (CCSR_BASE + 0x00c88)
#define LAWAR4                      (CCSR_BASE + 0x00c90)
#define LAWBAR5                     (CCSR_BASE + 0x00ca8)
#define LAWAR5                      (CCSR_BASE + 0x00cb0)
#define LAWBAR6                     (CCSR_BASE + 0x00cc8)
#define LAWAR6                      (CCSR_BASE + 0x00cd0)
#define LAWBAR7                     (CCSR_BASE + 0x00ce8)
#define LAWAR7                      (CCSR_BASE + 0x00cf0)
#define LAWBAR8                     (CCSR_BASE + 0x00d08)
#define LAWAR8                      (CCSR_BASE + 0x00d10)
#define LAWBAR9                     (CCSR_BASE + 0x00d28)
#define LAWAR9                      (CCSR_BASE + 0x00d30)
#define LAWBAR10                    (CCSR_BASE + 0x00d48)
#define LAWAR10                     (CCSR_BASE + 0x00d50)
#define LAWBAR11                    (CCSR_BASE + 0x00d68)
#define LAWAR11                     (CCSR_BASE + 0x00d70)

/* LAWBAR defines */

#define LAWBAR_ADRS_SHIFT           12

/* LAWAR defines */

#define LAWAR_ENABLE                0x80000000

/* LAWAR Target Interface */

#define LAWAR_TGTIF_PCI             0x00000000
#define LAWAR_TGTIF_PCIEX2          0x00100000
#define LAWAR_TGTIF_PCIEX1          0x00200000
#define LAWAR_TGTIF_PCIEX3          0x00300000
#define LAWAR_TGTIF_LBC             0x00400000
#define LAWAR_TGTIF_DDRSDRAM        0x00F00000

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
#define LAWAR_SIZE_4GB              0x0000001F
#define LAWAR_SIZE_8GB              0x00000020
#define LAWAR_SIZE_16GB             0x00000021
#define LAWAR_SIZE_32GB             0x00000022

/* I2C */

#define I2C0_BASE                   (CCSR_BASE + 0x03000)
#define I2C1_BASE                   (CCSR_BASE + 0x03100)

/* I2C defines */

#define M85XX_I2C1_BASE             0x3000
#define M85XX_I2C2_BASE             0x3100

/* DUART */

#define UART0_BASE                  (CCSR_BASE + 0x04500)
#define UART1_BASE                  (CCSR_BASE + 0x04600)

/* PCI */

#define PCI_BASE                    (CCSR_BASE + 0x08000)

/* PCIE */

#define PCIEX2_BASE                 (CCSR_BASE + 0x09000)
#define PCIEX1_BASE                 (CCSR_BASE + 0x0a000)
#define PCIEX3_BASE                 (CCSR_BASE + 0x0b000)

/* GPIO */

#define GPIO_BASE                   (CCSR_BASE + 0x0f000)

/* SATA */

#define SATA0_BASE                  (CCSR_BASE + 0x18000)
#define SATA1_BASE                  (CCSR_BASE + 0x19000)

/* USB */

#define USB1_BASE                   (CCSR_BASE + 0x22000)
#define USB2_BASE                   (CCSR_BASE + 0x23000)

/* EHCI registers */

#define EHCI_CAPLENGTH(base)        ((base) + 0x00100)

#define EHCI_BURSTSIZE(base)        ((base) + 0x00160)
#define EHCI_TXTTFILLTUNING_L(base) ((base) + 0x00164)
#define EHCI_TXTTFILLTUNING_H(base) ((base) + 0x00168)
#define EHCI_CONFIGFLAG(base)       ((base) + 0x00180)
#define EHCI_PORTSC(base)           ((base) + 0x00184)
#define EHCI_USBMODE(base)          ((base) + 0x001a8)

#define EHCI_SNOOP1(base)           ((base) + 0x00400)
#define EHCI_SNOOP2(base)           ((base) + 0x00404)
#define EHCI_AGE_CNT_THRESH(base)   ((base) + 0x00408)
#define EHCI_PRI_CTRL(base)         ((base) + 0x0040c)
#define EHCI_SI_CTRL(base)          ((base) + 0x00410)
#define EHCI_CONTROL(base)          ((base) + 0x00500)

#define EHCI_USBMODE_IDLE           0x00000000
#define EHCI_USBMODE_DEVICE         0x00000002
#define EHCI_USBMODE_HOST           0x00000003

#define EHCI_PORTSC_ULPI            0x80000000

#define EHCI_SNOOP_SIZE_4K          0x0000000B
#define EHCI_SNOOP_SIZE_8K          0x0000000C
#define EHCI_SNOOP_SIZE_16K         0x0000000D
#define EHCI_SNOOP_SIZE_32K         0x0000000E
#define EHCI_SNOOP_SIZE_64K         0x0000000F
#define EHCI_SNOOP_SIZE_128K        0x00000010
#define EHCI_SNOOP_SIZE_256K        0x00000011
#define EHCI_SNOOP_SIZE_512K        0x00000012
#define EHCI_SNOOP_SIZE_1M          0x00000013
#define EHCI_SNOOP_SIZE_2M          0x00000014
#define EHCI_SNOOP_SIZE_4M          0x00000015
#define EHCI_SNOOP_SIZE_8M          0x00000016
#define EHCI_SNOOP_SIZE_16M         0x00000017
#define EHCI_SNOOP_SIZE_32M         0x00000018
#define EHCI_SNOOP_SIZE_64M         0x00000019
#define EHCI_SNOOP_SIZE_128M        0x0000001A
#define EHCI_SNOOP_SIZE_256M        0x0000001B
#define EHCI_SNOOP_SIZE_512M        0x0000001C
#define EHCI_SNOOP_SIZE_1G          0x0000001D
#define EHCI_SNOOP_SIZE_2G          0x0000001E

#define EHCI_CONTROL_USB_EN         0x00000004

/* ETSEC */

#define ETSEC1_BASE                 (CCSR_BASE + 0x24000)
#define ETSEC3_BASE                 (CCSR_BASE + 0x26000)

/* Global Utilities */

#define GU_BASE                     (CCSR_BASE + 0xe0000)

#define PORPLLSR                    (GU_BASE + 0x00000)
#define GENCFGR                     (GU_BASE + 0x00030)
#define PMUXCR                      (GU_BASE + 0x00060)
#define DEVDISR                     (GU_BASE + 0x00070)
#define M85XX_PVR                   (GU_BASE + 0x000a0)
#define M85XX_SVR                   (GU_BASE + 0x000a4)

/* GENCFGR defines */

#define SDHC_WP_INV                 0x20000000

/* PMUXCR defines */

#define SDHC_DATA                   0x80000000
#define SDHC_CD                     0x40000000
#define SDHC_WP                     0x20000000

/* SVR */

#define SVR_MPC8536E                0x803f0090
#define SVR_MPC8536                 0x80370090

/* SERDES */

#define SERDES1_BASE                (CCSR_BASE + 0xe3000)
#define SERDES2_BASE                (CCSR_BASE + 0xe3100)

/* PORPLLSR defines */

#define PORPLLSR_E500_RATIO_MASK    0x003f0000
#define PORPLLSR_PLAT_RATIO_MASK    0x0000003e

/* PCI Express Host Controller */

/* ATMU register defines */

#define PCI_WINDOW_ENABLE_BIT       0x80000000
#define PCI_SNOOP_ENABLE            0x40000000
#define PCI_PREFETCHABLE            0x20000000

/* outbound window attributes */

#define PCI_OUT_ATTR_RTT_MEM        0x00040000
#define PCI_OUT_ATTR_RTT_IO         0x00080000
#define PCI_OUT_ATTR_WTT_MEM        0x00004000
#define PCI_OUT_ATTR_WTT_IO         0x00008000

#define PCI_ATTR_WS_4K              0x0000000B
#define PCI_ATTR_WS_8K              0x0000000C
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
#define PCI_ATTR_WS_128M            0x0000001A
#define PCI_ATTR_WS_256M            0x0000001B
#define PCI_ATTR_WS_512M            0x0000001C
#define PCI_ATTR_WS_1G              0x0000001D
#define PCI_ATTR_WS_2G              0x0000001E
#define PCI_ATTR_WS_4G              0x0000001F
#define PCI_ATTR_WS_8G              0x00000020
#define PCI_ATTR_WS_16G             0x00000021
#define PCI_ATTR_WS_32G             0x00000022
#define PCI_ATTR_WS_64G             0x00000023

/* inbound window attributes */

#define PCI_IN_ATTR_TGI_LM                                  0x00f00000
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0x00007000

/* PCI Express Interrupts */

#define PCIEX1_XINT1_LVL            0
#define PCIEX1_XINT2_LVL            1
#define PCIEX1_XINT3_LVL            2
#define PCIEX1_XINT4_LVL            3

#define PCIEX2_XINT1_LVL            4
#define PCIEX2_XINT2_LVL            5
#define PCIEX2_XINT3_LVL            6
#define PCIEX2_XINT4_LVL            7

#define PCIEX3_XINT1_LVL            8
#define PCIEX3_XINT2_LVL            9
#define PCIEX3_XINT3_LVL            10
#define PCIEX3_XINT4_LVL            11

/* PCI Interrupts */

#define PCI_XINT1_LVL               1
#define PCI_XINT2_LVL               2
#define PCI_XINT3_LVL               3
#define PCI_XINT4_LVL               4

/* EPIC defines */

#define EPIC_EX_IRQ_NUM             12
#define EPIC_IN_IRQ_NUM             64
#define EPIC_GT_IRQ_NUM             4
#define EPIC_MSG_IRQ_NUM            8
#define EPIC_SMSG_IRQ_NUM           8
#define EPIC_IPI_IRQ_NUM            4

#define EPIC_EX_REG_BASE            0x50000
#define EPIC_IN_REG_BASE            0x50200
#define EPIC_GT_A_REG_BASE          0x41120
#define EPIC_GT_B_REG_BASE          0x42120
#define EPIC_MSG_REG_BASE           0x51600
#define EPIC_SMSG_REG_BASE          0x51c00
#define EPIC_IPI_REG_BASE           0x410a0

#define EPIC_EX_REG_INTERVAL        0x20
#define EPIC_IN_REG_INTERVAL        0x20
#define EPIC_GT_REG_INTERVAL        0x40
#define EPIC_MSG_REG_INTERVAL       0x20
#define EPIC_SMSG_REG_INTERVAL      0x20
#define EPIC_IPI_REG_INTERVAL       0x10

/* interrupt vectors */

#define INUM_EX_IRQ                 0

#define INUM_IN_IRQ                 (INUM_EX_IRQ + EPIC_EX_IRQ_NUM)
#define INUM_L2CACHE                (INUM_IN_IRQ + 0)
#define INUM_ECM                    (INUM_IN_IRQ + 1)
#define INUM_DDR                    (INUM_IN_IRQ + 2)
#define INUM_ELBC                   (INUM_IN_IRQ + 3)
#define INUM_DMA1                   (INUM_IN_IRQ + 4)
#define INUM_DMA2                   (INUM_IN_IRQ + 5)
#define INUM_DMA3                   (INUM_IN_IRQ + 6)
#define INUM_DMA4                   (INUM_IN_IRQ + 7)
#define INUM_PCI                    (INUM_IN_IRQ + 8)
#define INUM_PCIE2                  (INUM_IN_IRQ + 9)
#define INUM_PCIE1                  (INUM_IN_IRQ + 10)
#define INUM_PCIE3                  (INUM_IN_IRQ + 11)
#define INUM_USB1                   (INUM_IN_IRQ + 12)
#define INUM_ETSEC1_TX              (INUM_IN_IRQ + 13)
#define INUM_ETSEC1_RX              (INUM_IN_IRQ + 14)
#define INUM_ETSEC3_TX              (INUM_IN_IRQ + 15)
#define INUM_ETSEC3_RX              (INUM_IN_IRQ + 16)
#define INUM_ETSEC3_ERR             (INUM_IN_IRQ + 17)
#define INUM_ETSEC1_ERR             (INUM_IN_IRQ + 18)
#define INUM_SATA2                  (INUM_IN_IRQ + 25)
#define INUM_DUART                  (INUM_IN_IRQ + 26)
#define INUM_I2C                    (INUM_IN_IRQ + 27)
#define INUM_PERFMON                (INUM_IN_IRQ + 28)
#define INUM_SEC1                   (INUM_IN_IRQ + 29)
#define INUM_USB2                   (INUM_IN_IRQ + 30)
#define INUM_GPIO                   (INUM_IN_IRQ + 31)
#define INUM_SEC2                   (INUM_IN_IRQ + 42)
#define INUM_ESPI                   (INUM_IN_IRQ + 43)
#define INUM_USB3                   (INUM_IN_IRQ + 44)
#define INUM_ETSEC1_1588            (INUM_IN_IRQ + 52)
#define INUM_ETSEC3_1588            (INUM_IN_IRQ + 54)
#define INUM_ESDHC                  (INUM_IN_IRQ + 56)
#define INUM_SATA1                  (INUM_IN_IRQ + 58)

#define INUM_GT_IRQ                 (INUM_IN_IRQ + EPIC_IN_IRQ_NUM)
#define INUM_GT_TIMER_A0            (INUM_GT_IRQ + 0)
#define INUM_GT_TIMER_A1            (INUM_GT_IRQ + 1)
#define INUM_GT_TIMER_A2            (INUM_GT_IRQ + 2)
#define INUM_GT_TIMER_A3            (INUM_GT_IRQ + 3)
#define INUM_GT_TIMER_B0            (INUM_GT_IRQ + 4)
#define INUM_GT_TIMER_B1            (INUM_GT_IRQ + 5)
#define INUM_GT_TIMER_B2            (INUM_GT_IRQ + 6)
#define INUM_GT_TIMER_B3            (INUM_GT_IRQ + 7)

/* OpenPIC Timer */

#define EPIC_TIMER_A_BASE           (CCSR_BASE + 0x0410f0)
#define EPIC_TIMER_B_BASE           (CCSR_BASE + 0x0420f0)

/* eSDHC */

#define SDHC_BASE                   (CCSBAR + 0x2E000)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCmpc8536eh */
