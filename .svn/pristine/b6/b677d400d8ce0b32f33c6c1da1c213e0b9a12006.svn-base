/* apm86x90.h - APM86x90 SOC header */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,06feb12,syt  add SDHC, SATA, SPI and I2C support.
01i,16nov11,x_z  changed QMan Mailbox memroy from OCM to MEMC.
01h,22sep11,x_z  added support for QMAN and on-chip MAC;
                 fixed conflict between PCIe address space and OCM space.
01g,07sep11,x_s  add support for SMP.
01f,29aug11,x_s  add support for L2 cache.
01e,30jul11,x_z  add macros for PCI Express memory shutdown operation;
                 adjust PCI Express address space;
                 fix some macros.
01d,22jul11,b_m  add PCI Express defines.
01c,19jul11,x_z  add ethernet support.
01b,18jul11,b_m  add usb related defines.
01a,19jun11,syt  initial creation.
*/

/*
 * This file contains I/O addresses and related constants for the
 * APM86x90 SOC.
*/

#ifndef __INCapm86x90h
#define __INCapm86x90h

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define SYS_MODEL_86290             "APM86290 - PPC465"
#define SYS_MODEL_86190             "APM86190 - PPC465"

/*
 * Interrupt Vector Prefix Register value. Exception vectors will be
 * located at the start of SDRAM.
 */

#define IVPR_INIT_VAL               0x00000000

/* PPC465 L1, L2 Cache Size */

#define L1_CACHE_SIZE               (32 * 1024)
#define L2_CACHE_SIZE               (256 * 1024)
#define L2_CACHE_LINE_SIZE          128

/* Validate and instantiate 465 Machine Check Handler configuration */

#ifdef  INCLUDE_440X5_DCACHE_RECOVERY
#   ifndef INCLUDE_440X5_PARITY_RECOVERY
#       define INCLUDE_440X5_PARITY_RECOVERY
#   endif  /* INCLUDE_440X5_PARITY_RECOVERY */

#   if (USER_D_CACHE_MODE != CACHE_DISABLED && defined(USER_D_CACHE_ENABLE))
#       undef  USER_D_CACHE_MODE
#       define USER_D_CACHE_MODE        CACHE_WRITETHROUGH
#   endif /* USER_D_CACHE_MODE != CACHE_DISABLED) */

#   define _WRS_PPC440X5_CFG_DCR        TRUE
#else /* INCLUDE_440X5_DCACHE_RECOVERY */
#   define _WRS_PPC440X5_CFG_DCR        FALSE
#endif  /* INCLUDE_440X5_DCACHE_RECOVERY */

#ifdef  INCLUDE_440X5_TLB_RECOVERY
#   ifndef INCLUDE_440X5_PARITY_RECOVERY
#       define INCLUDE_440X5_PARITY_RECOVERY
#   endif  /* INCLUDE_440X5_PARITY_RECOVERY */

#   ifndef INCLUDE_MMU_BASIC
#       define INCLUDE_MMU_BASIC
#   endif  /* INCLUDE_MMU_BASIC */

#   define _WRS_PPC440X5_CFG_TLB        TRUE
#else /* INCLUDE_440X5_TLB_RECOVERY */
#   define _WRS_PPC440X5_CFG_TLB        FALSE
#endif  /* INCLUDE_440X5_TLB_RECOVERY */

#ifdef  INCLUDE_440X5_MCH_LOGGER
#   ifndef _PPC_440X5_MCH_LOG_RTN
#       define _PPC_440X5_MCH_LOG_RTN   (FUNCPTR)1
#   endif  /* _PPC_440X5_MCH_LOG_RTN */
#else /* INCLUDE_440X5_MCH_LOGGER */
#   undef  _PPC_440X5_MCH_LOG_RTN
#   define _PPC_440X5_MCH_LOG_RTN       (FUNCPTR)NULL
#endif  /* INCLUDE_440X5_MCH_LOGGER */

/* APM86x90 system map */

/* APM86x90 peripherals map */

#define APM_PERIPHERAL_BASE         0xe0000000

#define APM_PERIPHERAL1_PHY_BASE    0xa0000000
#define APM_PERIPHERAL1_PHY_BASE_HI 0xf

#define APM_PERIPHERAL2_PHY_BASE    0x00400000
#define APM_PERIPHERAL2_PHY_BASE_HI 0xd

/* APM86x90 SPI controller Base address */

#define APM_SPI0_BASE               (APM_PERIPHERAL_BASE + 0x5000)
#define APM_SPI1_BASE               (APM_PERIPHERAL_BASE + 0x6000)

/* APM86x90 I2C controller Base address */

#define APM_I2C0_BASE               (APM_PERIPHERAL_BASE + 0x2000)
#define APM_I2C1_BASE               (APM_PERIPHERAL_BASE + 0x200000 + 0x11000)

/* APM86x90 On-Chip Memory (OCM) map */

#define APM_OCM_BASE                0x90000000
#define APM_OCM_SIZE                0x00008000
#define APM_OCM_BASE_PHY_BASE       0xffff8000
#define APM_OCM_BASE_PHY_BASE_HI    0xe

/* APM86x90 Memory Controller (MEMC) map */

#define APM_MEMC_PHY_BASE           0xffffc000
#define APM_MEMC_PHY_BASE_HI        0x3
#define APM_MEMC_BASE               0xefffc000
#define APM_MEMC_MAP_SIZE           0x4000

/* APM86x90 controll / status registers base address */

#define APM_CSR_BASE                0xdd800000
#define APM_CSR_BASE_HI             0xd
#define APM_CSR_MAP_SIZE            0x100000

/* APM86x90 MPIC & LCD registers base address */

#define APM_MPIC_LCD_CSR_BASE       0xdf400000
#define APM_MPIC_LCD_CSR_BASE_HI    0xd

/* APM86x90 UARTs base address */

#define APM_UART_BASE               APM_PERIPHERAL_BASE

/* MPIC Interrupt Sources define */

#define APM_MPIC_BASE               0xdf400000
#define APM_MPIC_BASE_HI            0xd
#define APM_MPIC_MAP_SIZE           0x40000

/* APM86x90 QMan CSR map */

#define APM_QMAN_CSR_BASE           APM_CSR_BASE

#define APM_QMAN_MEM_SHTDOWN        (APM_QMAN_CSR_BASE + 0x7070)

/* APM86x90 Ethernet & Classifier map */

#define APM_ETH_BASE                (APM_CSR_BASE + 0xa0000)
#define APM_CLE_BASE                (APM_CSR_BASE + 0xe0000)

#define APM_ETH_MEM_SHTDOWN         (APM_ETH_BASE + 0x7070)
#define APM_CLE_MEM_SHTDOWN         (APM_CLE_BASE + 0x7070)

/* APM86x90 QMan HBF map */

#define APM_QMAN_HBF_PHY_BASE       0xdf000000
#define APM_QMAN_HBF_PHY_BASE_HI    0xd
#define APM_QMAN_HBF_BASE           0xdf000000
#define APM_QMAN_HBF_MAP_SIZE       0x100000

/* APM86x90 QMan Mailbox map (can be mappped to OCM, MEMC and DDR RAM) */

#define APM_QMAN_MBOX_OFFSET        0x1000
#define APM_QMAN_MBOX_PHY_BASE      (APM_MEMC_PHY_BASE + APM_QMAN_MBOX_OFFSET)
#define APM_QMAN_MBOX_PHY_BASE_HI   APM_MEMC_PHY_BASE_HI
#define APM_QMAN_MBOX_BASE          (APM_MEMC_BASE + APM_QMAN_MBOX_OFFSET)

/* APM86x90 SATA CSR map */

#define APM_SATA_PCS_BASE           (APM_CSR_BASE + 0x70000)
#define APM_SATA_MEM_SHTDOWN        (APM_SATA_PCS_BASE + 0x7070)
#define APM_SATA_BLOCK_MEM_RDY      (APM_SATA_PCS_BASE + 0x7074)

/* SATA SERDES registers */

#define SATA_PORT_NUM               2

#define APM_AHCI_CAP                (APM_SATA_BASE + 0x0000)
#define APM_AHCI_GHC                (APM_SATA_BASE + 0x0004)
#define APM_AHCI_PI                 (APM_SATA_BASE + 0x000c)
#define APM_AHCI_BISTCR             (APM_SATA_BASE + 0x00a4)
#define APM_AHCI_TESTR              (APM_SATA_BASE + 0x00f4)

#define AHCI_EN                     0x80000000

#define PCSCSR_CTL0                 0x4000
#define PCSCSR_CTL1                 0x4004
#define PCSCSR_CTL2                 0x402c
#define PCSCSR_CTL3                 0x4030
#define PCSCSR_STATUS1              0x4008
#define PCSCSR_REG_ADDR             0x400c
#define PCSCSR_REG_WRDATA           0x4010
#define PCSCSR_REG_CMD              0x4014
#define PCSCSR_REG_CMDDONE          0x4018
#define PCSCSR_REG_RDDATA           0x401c
#define PCSCSR_CLK_CTLR             0x4028
#define SATA_IOCOH_START0           0x4034
#define SATA_IOCOH_START1           0x4038
#define SATA_IOCOH_END0             0x403c
#define SATA_IOCOH_END1             0x4040

#define SDS_PCS_PRBS_TIMER_0_ADDR   0x8004
#define SDS_PCS_SATA_CTRL_0_ADDR    0x80E0
#define SDS_PCS_SATA_RX_LEN_1_ADDR  0x80E7
#define SDS_PCS_SATA_CFG_0_ADDR     0x80EA

#define SDS_PCS_PRBS_TIMER_0_DATA   0x0470
#define SDS_PCS_SATA_RX_LEN_1_DATA  0x2933
#define SDS_PCS_SATA_CFG_0_DATA     0x2FF9

#define LANE_SEL_F                  0xF0000

#define PCSCSR_CTL0_INIT_VAL        0x209fb200
#define PCS_PHY_RST_MASK            0x00000020

#define PCSCSR_CTL1_INIT_VAL        0x00801e33
#define PCSCSR_CTL2_INIT_VAL        0x1a000000

/* Fields mgmt_pcs_reg_rd */

#define MGMT_PCS_RD                 0x00000001
#define MGMT_PCS_RD_DONE            0x00000001

/* Fields mgmt_pcs_reg_wr */

#define MGMT_PCS_WR                 0x00000002
#define MGMT_PCS_WR_DONE            0x00000002

/* Fields sds_pcs_pll_lock */

#define SDS_PCS_PLL_LOCK1_MASK      0x00000400
#define SDS_PCS_CLK_RDY1_MASK       0x00000800

/* APM86x90 systerm controll registers base address */

#define SCU_BASE_ADDR               0xdd8b0000

/* Clocking Register Map */

#define SCU_SOCPLL0                 (SCU_BASE_ADDR + 0x0040)
#define SCU_SOCPLL1                 (SCU_BASE_ADDR + 0x0044)
#define SCU_SOCPLL2                 (SCU_BASE_ADDR + 0x0048)
#define SCU_SOCPLL3                 (SCU_BASE_ADDR + 0x004c)
#define SCU_SOCPLL4                 (SCU_BASE_ADDR + 0x00f0)
#define SCU_SOCPLLADJ0              (SCU_BASE_ADDR + 0x0050)
#define SCU_SOCPLLADJ1              (SCU_BASE_ADDR + 0x0054)
#define SCU_SOCPLLADJ2              (SCU_BASE_ADDR + 0x0058)
#define SCU_SOCPLLADJ3              (SCU_BASE_ADDR + 0x005c)
#define SCU_SOCPLLADJ4              (SCU_BASE_ADDR + 0x00f4)
#define SCU_SOCDIV                  (SCU_BASE_ADDR + 0x00c0)
#define SCU_SOCDIV1                 (SCU_BASE_ADDR + 0x00c4)
#define SCU_SOCDIV2                 (SCU_BASE_ADDR + 0x00c8)
#define SCU_SOCDIV3                 (SCU_BASE_ADDR + 0x00cc)
#define SCU_SOCDIV4                 (SCU_BASE_ADDR + 0x00d0)
#define SCU_SOCDIV5                 (SCU_BASE_ADDR + 0x00d4)
#define SCU_SOCPLLSTAT              (SCU_BASE_ADDR + 0x0060)
#define SCU_PLLDLY                  (SCU_BASE_ADDR + 0x0064)

/* Reset and Initialization Register Map */

#define SCU_SRST                    (SCU_BASE_ADDR + 0x000c)
#define SCU_IPL_DONE                (SCU_BASE_ADDR + 0x0014)
#define SCU_PINSTRAP                (SCU_BASE_ADDR + 0x0020)
#define SCU_NFBOOT0                 (SCU_BASE_ADDR + 0x0024)
#define SCU_NFBOOT1                 (SCU_BASE_ADDR + 0x0028)
#define SCU_EBCBOOT                 (SCU_BASE_ADDR + 0x002c)
#define SCU_PPCBOOT                 (SCU_BASE_ADDR + 0x0030)
#define SCU_PPCBOOTDEV              (SCU_BASE_ADDR + 0x0034)
#define SCU_SRST1                   (SCU_BASE_ADDR + 0x0038)
#define SCU_CSR_SRST                (SCU_BASE_ADDR + 0x00e0)
#define SCU_CSR_SRST1               (SCU_BASE_ADDR + 0x00e4)
#define SCU_SCRST                   (SCU_BASE_ADDR + 0x1420)

/* Soft Reset Registers */

#define SCU_SRST_MPIC               0x10000000
#define SCU_SRST_SATA0              0x08000000
#define SCU_SRST_SATA1              0x04000000
#define SCU_SRST_USB0               0x02000000
#define SCU_SRST_USB1               0x01000000
#define SCU_SRST_USB2               0x00800000
#define SCU_SRST_PCIEX0             0x00400000
#define SCU_SRST_PCIEX1             0x00200000
#define SCU_SRST_PCIEX2             0x00100000
#define SCU_SRST_ENET0              0x00080000
#define SCU_SRST_ENET1              0x00040000
#define SCU_SRST_SPI                0x00010000
#define SCU_SRST_SDIO               0x00008000
#define SCU_SRST_MEMC               0x00004000
#define SCU_SRST_DDR_PHY            0x00001000
#define SCU_SRST_OCM                0x00000800
#define SCU_SRST_QMTM               0x00000200
#define SCU_SRST_QML                0x00000100

#define SCU_SRST1_CLE0              0x00800000
#define SCU_SRST1_CLE1              0x00400000

/* CSR Soft Reset Registers */

#define SCU_CSR_SRST_MPIC           0x10000000
#define SCU_CSR_SRST_SATA0          0x08000000
#define SCU_CSR_SRST_SATA1          0x04000000
#define SCU_CSR_SRST_PCIEX0         0x00400000
#define SCU_CSR_SRST_PCIEX1         0x00200000
#define SCU_CSR_SRST_PCIEX2         0x00100000
#define SCU_CSR_SRST_ENET0          0x00080000
#define SCU_CSR_SRST_ENET1          0x00040000
#define SCU_CSR_SRST_SPI            0x00010000
#define SCU_CSR_SRST_MEMC           0x00004000
#define SCU_CSR_SRST_OCM            0x00000800
#define SCU_CSR_SRST_QMTM           0x00000200
#define SCU_CSR_SRST_QML            0x00000100

#define SCU_CSR_SRST1_CLE           0x00800000

/* Power Management Register Map */

#define SCU_CLKEN                   (SCU_BASE_ADDR + 0x0010)
#define SCU_CLKEN1                  (SCU_BASE_ADDR + 0x003c)
#define SCU_PPC0_RESUME_DS          (SCU_BASE_ADDR + 0x0068)
#define SCU_PPC1_RESUME_DS          (SCU_BASE_ADDR + 0x006c)
#define SCU_SOC_PWR_CTL             (SCU_BASE_ADDR + 0x0070)
#define SCU_SOC_PWR_STAT            (SCU_BASE_ADDR + 0x0074)
#define SCU_PWRGOOD                 (SCU_BASE_ADDR + 0x0078)
#define SCU_PS_CTL                  (SCU_BASE_ADDR + 0x007c)
#define SCU_MRDY                    (SCU_BASE_ADDR + 0x00b0)
#define SCU_SOC_TDR_CTL             (SCU_BASE_ADDR + 0x00b8)

/* Clock Enable Registers */

#define SCU_CLKEN_SATA0             0x08000000
#define SCU_CLKEN_SATA1             0x04000000
#define SCU_CLKEN_USB0              0x02000000
#define SCU_CLKEN_USB1              0x01000000
#define SCU_CLKEN_USB2              0x00800000
#define SCU_CLKEN_PCIEX0            0x00400000
#define SCU_CLKEN_PCIEX1            0x00200000
#define SCU_CLKEN_PCIEX2            0x00100000
#define SCU_CLKEN_ENET0             0x00080000
#define SCU_CLKEN_ENET1             0x00040000
#define SCU_CLKEN_SPI               0x00010000
#define SCU_CLKEN_SDIO              0x00008000
#define SCU_CLKEN_MEMC_AXI          0x00004000
#define SCU_CLKEN_MEMC_PLB          0x00002000
#define SCU_CLKEN_MEMC_DDR          0x00001000
#define SCU_CLKEN_OCM               0x00000800
#define SCU_CLKEN_QMTM              0x00000200
#define SCU_CLKEN_QML               0x00000100

#define SCU_CLKEN1_CLE0_IL          0x00800000
#define SCU_CLKEN1_CLE1_IL          0x00400000

/* EBUS Registers Map */

#define EBUS_REG_BASE               (APM_PERIPHERAL_BASE + 0x140000)

#define NFLASH_SRAM_CTRL1_0         (EBUS_REG_BASE + 0x0d0)
#define NFLASH_SRAM_CTRL1_1         (EBUS_REG_BASE + 0x0d4)
#define NFLASH_SRAM_CTRL1_2         (EBUS_REG_BASE + 0x0d8)
#define NFLASH_SRAM_CTRL1_3         (EBUS_REG_BASE + 0x0dc)
#define NFLASH_SRAM_CTRL2_0         (EBUS_REG_BASE + 0x0e0)
#define NFLASH_SRAM_CTRL2_1         (EBUS_REG_BASE + 0x0e4)
#define NFLASH_SRAM_CTRL2_2         (EBUS_REG_BASE + 0x0e8)
#define NFLASH_SRAM_CTRL2_3         (EBUS_REG_BASE + 0x0ec)
#define NFLASH_SRAM_CFG_0           (EBUS_REG_BASE + 0x0f0)
#define NFLASH_SRAM_CFG_1           (EBUS_REG_BASE + 0x0f4)
#define NFLASH_SRAM_CFG_2           (EBUS_REG_BASE + 0x0f8)
#define NFLASH_SRAM_CFG_3           (EBUS_REG_BASE + 0x0fc)
#define NFLASH_SRAM_BNKCFG_0        (EBUS_REG_BASE + 0x100)
#define NFLASH_SRAM_BNKCFG_1        (EBUS_REG_BASE + 0x104)
#define NFLASH_SRAM_BNKCFG_2        (EBUS_REG_BASE + 0x108)
#define NFLASH_SRAM_BNKCFG_3        (EBUS_REG_BASE + 0x10c)
#define NFLASH_SRAM_BEAR            (EBUS_REG_BASE + 0x110)
#define NFLASH_SRAM_BESR            (EBUS_REG_BASE + 0x114)
#define NFLASH_SRAM_EXTRESET        (EBUS_REG_BASE + 0x118)
#define NFLASH_BOOT_CFG             (EBUS_REG_BASE + 0x11c)
#define NFLASH_SRAM_CS              (EBUS_REG_BASE + 0x120)

/* System Operations Register Map */

#define SCU_JTAG0                   (SCU_BASE_ADDR + 0x0004)
#define SCU_STAT                    (SCU_BASE_ADDR + 0x0018)
#define SCU_SOC_EFUSE               (SCU_BASE_ADDR + 0x001c)
#define SCU_ECID0                   (SCU_BASE_ADDR + 0x0080)
#define SCU_ECID1                   (SCU_BASE_ADDR + 0x0084)
#define SCU_ECID2                   (SCU_BASE_ADDR + 0x0088)
#define SCU_ECID3                   (SCU_BASE_ADDR + 0x008c)
#define SCU_SOC_TS_CTL              (SCU_BASE_ADDR + 0x00b4)
#define SCU_SOC_SMS_CTL             (SCU_BASE_ADDR + 0x00dc)
#define EFUSE_CTL                   (SCU_BASE_ADDR + 0x1400)
#define EFUSE_STAT                  (SCU_BASE_ADDR + 0x1404)
#define EFUSE_RDDATA                (SCU_BASE_ADDR + 0x140c)

/* MEMC Registers Map */

#define MEMC_REG_BASE               0xeffff800

#define MEMC_BID                    (MEMC_REG_BASE + 0x000)
#define MEMRANGE                    (MEMC_REG_BASE + 0x004)
#define MEMQ_ARB                    (MEMC_REG_BASE + 0x008)
#define DDRC0_00                    (MEMC_REG_BASE + 0x010)
#define DDRC0_01                    (MEMC_REG_BASE + 0x014)
#define DDRC0_02                    (MEMC_REG_BASE + 0x018)
#define DDRC0_03                    (MEMC_REG_BASE + 0x01C)
#define DDRC0_04                    (MEMC_REG_BASE + 0x020)
#define DDRC0_05                    (MEMC_REG_BASE + 0x024)
#define DDRC0_06                    (MEMC_REG_BASE + 0x028)
#define DDRC0_07                    (MEMC_REG_BASE + 0x02C)
#define DDRC0_08                    (MEMC_REG_BASE + 0x030)
#define DDRC0_09                    (MEMC_REG_BASE + 0x034)
#define DDRC0_10                    (MEMC_REG_BASE + 0x038)
#define DDRC0_11                    (MEMC_REG_BASE + 0x03C)
#define DDRC0_12                    (MEMC_REG_BASE + 0x040)
#define DDRC0_13                    (MEMC_REG_BASE + 0x044)
#define DDRC0_15                    (MEMC_REG_BASE + 0x04C)
#define DDRC0_16                    (MEMC_REG_BASE + 0x050)
#define DDRC0_17                    (MEMC_REG_BASE + 0x054)
#define DDRC0_18                    (MEMC_REG_BASE + 0x058)
#define DDRC0_18_1                  (MEMC_REG_BASE + 0x05C)
#define DDRC0_19                    (MEMC_REG_BASE + 0x060)
#define DDRC0_21                    (MEMC_REG_BASE + 0x068)
#define DDRC0_22                    (MEMC_REG_BASE + 0x06C)
#define DDRC0_23                    (MEMC_REG_BASE + 0x070)
#define DDRC0_31                    (MEMC_REG_BASE + 0x090)
#define DDRC0_31_1                  (MEMC_REG_BASE + 0x094)
#define DDRC0_32                    (MEMC_REG_BASE + 0x098)
#define DDRC0_33                    (MEMC_REG_BASE + 0x09C)
#define DDRC0_35                    (MEMC_REG_BASE + 0x0A4)
#define PHY_1                       (MEMC_REG_BASE + 0x0B0)
#define PHY_CTRL_SLAVE              (MEMC_REG_BASE + 0x0B4)
#define PHY_RD_DQS_SLAVE_R0_S0      (MEMC_REG_BASE + 0x0B8)
#define PHY_RD_DQS_SLAVE_R1_S0      (MEMC_REG_BASE + 0x0bc)
#define PHY_RD_DQS_SLAVE_R2_S0      (MEMC_REG_BASE + 0x0c0)
#define PHY_RD_DQS_SLAVE_R3_S0      (MEMC_REG_BASE + 0x0c4)
#define PHY_RD_DQS_SLAVE_R0_S1      (MEMC_REG_BASE + 0x0c8)
#define PHY_RD_DQS_SLAVE_R1_S1      (MEMC_REG_BASE + 0x0cc)
#define PHY_RD_DQS_SLAVE_R2_S1      (MEMC_REG_BASE + 0x0d0)
#define PHY_RD_DQS_SLAVE_R3_S1      (MEMC_REG_BASE + 0x0d4)
#define PHY_RD_DQS_SLAVE_R0_S2      (MEMC_REG_BASE + 0x0d8)
#define PHY_RD_DQS_SLAVE_R1_S2      (MEMC_REG_BASE + 0x0dc)
#define PHY_RD_DQS_SLAVE_R2_S2      (MEMC_REG_BASE + 0x0e0)
#define PHY_RD_DQS_SLAVE_R3_S2      (MEMC_REG_BASE + 0x0e4)
#define PHY_RD_DQS_SLAVE_R0_S3      (MEMC_REG_BASE + 0x0e8)
#define PHY_RD_DQS_SLAVE_R1_S3      (MEMC_REG_BASE + 0x0ec)
#define PHY_RD_DQS_SLAVE_R2_S3      (MEMC_REG_BASE + 0x0f0)
#define PHY_RD_DQS_SLAVE_R3_S3      (MEMC_REG_BASE + 0x0f4)
#define PHY_RD_DQS_SLAVE_R0_S4      (MEMC_REG_BASE + 0x0f8)
#define PHY_RD_DQS_SLAVE_R1_S4      (MEMC_REG_BASE + 0x0fc)
#define PHY_RD_DQS_SLAVE_R2_S4      (MEMC_REG_BASE + 0x100)
#define PHY_RD_DQS_SLAVE_R3_S4      (MEMC_REG_BASE + 0x104)
#define PHY_RD_DQS_SLAVE_R0_S5      (MEMC_REG_BASE + 0x108)
#define PHY_RD_DQS_SLAVE_R1_S5      (MEMC_REG_BASE + 0x10c)
#define PHY_RD_DQS_SLAVE_R2_S5      (MEMC_REG_BASE + 0x110)
#define PHY_RD_DQS_SLAVE_R3_S5      (MEMC_REG_BASE + 0x114)
#define PHY_RD_DQS_SLAVE_R0_S6      (MEMC_REG_BASE + 0x118)
#define PHY_RD_DQS_SLAVE_R1_S6      (MEMC_REG_BASE + 0x11c)
#define PHY_RD_DQS_SLAVE_R2_S6      (MEMC_REG_BASE + 0x120)
#define PHY_RD_DQS_SLAVE_R3_S6      (MEMC_REG_BASE + 0x124)
#define PHY_RD_DQS_SLAVE_R0_S7      (MEMC_REG_BASE + 0x128)
#define PHY_RD_DQS_SLAVE_R1_S7      (MEMC_REG_BASE + 0x130)
#define PHY_RD_DQS_SLAVE_R2_S7      (MEMC_REG_BASE + 0x134)
#define PHY_RD_DQS_SLAVE_R3_S7      (MEMC_REG_BASE + 0x138)
#define PHY_RD_DQS_SLAVE_R0_S8      (MEMC_REG_BASE + 0x13c)
#define PHY_RD_DQS_SLAVE_R1_S8      (MEMC_REG_BASE + 0x140)
#define PHY_RD_DQS_SLAVE_R2_S8      (MEMC_REG_BASE + 0x144)
#define PHY_RD_DQS_SLAVE_R3_S8      (MEMC_REG_BASE + 0x148)
#define PHY_WR_DQS_SLAVE_R0_S0      (MEMC_REG_BASE + 0x14c)
#define PHY_WR_DQS_SLAVE_R1_S0      (MEMC_REG_BASE + 0x150)
#define PHY_WR_DQS_SLAVE_R2_S0      (MEMC_REG_BASE + 0x154)
#define PHY_WR_DQS_SLAVE_R3_S0      (MEMC_REG_BASE + 0x158)
#define PHY_WR_DQS_SLAVE_R0_S1      (MEMC_REG_BASE + 0x15c)
#define PHY_WR_DQS_SLAVE_R1_S1      (MEMC_REG_BASE + 0x160)
#define PHY_WR_DQS_SLAVE_R2_S1      (MEMC_REG_BASE + 0x164)
#define PHY_WR_DQS_SLAVE_R3_S1      (MEMC_REG_BASE + 0x168)
#define PHY_WR_DQS_SLAVE_R0_S2      (MEMC_REG_BASE + 0x16c)
#define PHY_WR_DQS_SLAVE_R1_S2      (MEMC_REG_BASE + 0x170)
#define PHY_WR_DQS_SLAVE_R2_S2      (MEMC_REG_BASE + 0x174)
#define PHY_WR_DQS_SLAVE_R3_S2      (MEMC_REG_BASE + 0x178)
#define PHY_WR_DQS_SLAVE_R0_S3      (MEMC_REG_BASE + 0x17c)
#define PHY_WR_DQS_SLAVE_R1_S3      (MEMC_REG_BASE + 0x180)
#define PHY_WR_DQS_SLAVE_R2_S3      (MEMC_REG_BASE + 0x184)
#define PHY_WR_DQS_SLAVE_R3_S3      (MEMC_REG_BASE + 0x188)
#define PHY_WR_DQS_SLAVE_R0_S4      (MEMC_REG_BASE + 0x18c)
#define PHY_WR_DQS_SLAVE_R1_S4      (MEMC_REG_BASE + 0x190)
#define PHY_WR_DQS_SLAVE_R2_S4      (MEMC_REG_BASE + 0x194)
#define PHY_WR_DQS_SLAVE_R3_S4      (MEMC_REG_BASE + 0x198)
#define PHY_WR_DQS_SLAVE_R0_S5      (MEMC_REG_BASE + 0x19c)
#define PHY_WR_DQS_SLAVE_R1_S5      (MEMC_REG_BASE + 0x1a0)
#define PHY_WR_DQS_SLAVE_R2_S5      (MEMC_REG_BASE + 0x1a4)
#define PHY_WR_DQS_SLAVE_R3_S5      (MEMC_REG_BASE + 0x1a8)
#define PHY_WR_DQS_SLAVE_R0_S6      (MEMC_REG_BASE + 0x1ac)
#define PHY_WR_DQS_SLAVE_R1_S6      (MEMC_REG_BASE + 0x1b0)
#define PHY_WR_DQS_SLAVE_R2_S6      (MEMC_REG_BASE + 0x1b4)
#define PHY_WR_DQS_SLAVE_R3_S6      (MEMC_REG_BASE + 0x1b8)
#define PHY_WR_DQS_SLAVE_R0_S7      (MEMC_REG_BASE + 0x1bc)
#define PHY_WR_DQS_SLAVE_R1_S7      (MEMC_REG_BASE + 0x1c0)
#define PHY_WR_DQS_SLAVE_R2_S7      (MEMC_REG_BASE + 0x1c4)
#define PHY_WR_DQS_SLAVE_R3_S7      (MEMC_REG_BASE + 0x1c8)
#define PHY_WR_DQS_SLAVE_R0_S8      (MEMC_REG_BASE + 0x1cc)
#define PHY_WR_DQS_SLAVE_R1_S8      (MEMC_REG_BASE + 0x1d0)
#define PHY_WR_DQS_SLAVE_R2_S8      (MEMC_REG_BASE + 0x1d4)
#define PHY_WR_DQS_SLAVE_R3_S8      (MEMC_REG_BASE + 0x1d8)
#define PHY_2                       (MEMC_REG_BASE + 0x1dc)
#define PHY_3_R0_S0                 (MEMC_REG_BASE + 0x1e0)
#define PHY_3_R1_S0                 (MEMC_REG_BASE + 0x1e4)
#define PHY_3_R2_S0                 (MEMC_REG_BASE + 0x1e8)
#define PHY_3_R3_S0                 (MEMC_REG_BASE + 0x1ec)
#define PHY_3_R0_S1                 (MEMC_REG_BASE + 0x1f0)
#define PHY_3_R1_S1                 (MEMC_REG_BASE + 0x1f4)
#define PHY_3_R2_S1                 (MEMC_REG_BASE + 0x1f8)
#define PHY_3_R3_S1                 (MEMC_REG_BASE + 0x1fc)
#define PHY_3_R0_S2                 (MEMC_REG_BASE + 0x200)
#define PHY_3_R1_S2                 (MEMC_REG_BASE + 0x204)
#define PHY_3_R2_S2                 (MEMC_REG_BASE + 0x208)
#define PHY_3_R3_S2                 (MEMC_REG_BASE + 0x20c)
#define PHY_3_R0_S3                 (MEMC_REG_BASE + 0x210)
#define PHY_3_R1_S3                 (MEMC_REG_BASE + 0x214)
#define PHY_3_R2_S3                 (MEMC_REG_BASE + 0x218)
#define PHY_3_R3_S3                 (MEMC_REG_BASE + 0x21c)
#define PHY_3_R0_S4                 (MEMC_REG_BASE + 0x220)
#define PHY_3_R1_S4                 (MEMC_REG_BASE + 0x224)
#define PHY_3_R2_S4                 (MEMC_REG_BASE + 0x228)
#define PHY_3_R3_S4                 (MEMC_REG_BASE + 0x22c)
#define PHY_3_R0_S5                 (MEMC_REG_BASE + 0x230)
#define PHY_3_R1_S5                 (MEMC_REG_BASE + 0x234)
#define PHY_3_R2_S5                 (MEMC_REG_BASE + 0x238)
#define PHY_3_R3_S5                 (MEMC_REG_BASE + 0x23c)
#define PHY_3_R0_S6                 (MEMC_REG_BASE + 0x240)
#define PHY_3_R1_S6                 (MEMC_REG_BASE + 0x244)
#define PHY_3_R2_S6                 (MEMC_REG_BASE + 0x248)
#define PHY_3_R3_S6                 (MEMC_REG_BASE + 0x24c)
#define PHY_3_R0_S7                 (MEMC_REG_BASE + 0x250)
#define PHY_3_R1_S7                 (MEMC_REG_BASE + 0x254)
#define PHY_3_R2_S7                 (MEMC_REG_BASE + 0x258)
#define PHY_3_R3_S7                 (MEMC_REG_BASE + 0x25c)
#define PHY_3_R0_S8                 (MEMC_REG_BASE + 0x260)
#define PHY_3_R1_S8                 (MEMC_REG_BASE + 0x264)
#define PHY_3_R2_S8                 (MEMC_REG_BASE + 0x268)
#define PHY_3_R3_S8                 (MEMC_REG_BASE + 0x26c)
#define PHY_FIFO_WE_SLAVE_R0_S0     (MEMC_REG_BASE + 0x270)
#define PHY_FIFO_WE_SLAVE_R1_S0     (MEMC_REG_BASE + 0x274)
#define PHY_FIFO_WE_SLAVE_R2_S0     (MEMC_REG_BASE + 0x278)
#define PHY_FIFO_WE_SLAVE_R3_S0     (MEMC_REG_BASE + 0x27c)
#define PHY_FIFO_WE_SLAVE_R0_S1     (MEMC_REG_BASE + 0x280)
#define PHY_FIFO_WE_SLAVE_R1_S1     (MEMC_REG_BASE + 0x284)
#define PHY_FIFO_WE_SLAVE_R2_S1     (MEMC_REG_BASE + 0x288)
#define PHY_FIFO_WE_SLAVE_R3_S1     (MEMC_REG_BASE + 0x28c)
#define PHY_FIFO_WE_SLAVE_R0_S2     (MEMC_REG_BASE + 0x290)
#define PHY_FIFO_WE_SLAVE_R1_S2     (MEMC_REG_BASE + 0x294)
#define PHY_FIFO_WE_SLAVE_R2_S2     (MEMC_REG_BASE + 0x298)
#define PHY_FIFO_WE_SLAVE_R3_S2     (MEMC_REG_BASE + 0x29c)
#define PHY_FIFO_WE_SLAVE_R0_S3     (MEMC_REG_BASE + 0x2a0)
#define PHY_FIFO_WE_SLAVE_R1_S3     (MEMC_REG_BASE + 0x2a4)
#define PHY_FIFO_WE_SLAVE_R2_S3     (MEMC_REG_BASE + 0x2a8)
#define PHY_FIFO_WE_SLAVE_R3_S3     (MEMC_REG_BASE + 0x2ac)
#define PHY_FIFO_WE_SLAVE_R0_S4     (MEMC_REG_BASE + 0x2b0)
#define PHY_FIFO_WE_SLAVE_R1_S4     (MEMC_REG_BASE + 0x2b4)
#define PHY_FIFO_WE_SLAVE_R2_S4     (MEMC_REG_BASE + 0x2b8)
#define PHY_FIFO_WE_SLAVE_R3_S4     (MEMC_REG_BASE + 0x2bc)
#define PHY_FIFO_WE_SLAVE_R0_S5     (MEMC_REG_BASE + 0x2c0)
#define PHY_FIFO_WE_SLAVE_R1_S5     (MEMC_REG_BASE + 0x2c4)
#define PHY_FIFO_WE_SLAVE_R2_S5     (MEMC_REG_BASE + 0x2c8)
#define PHY_FIFO_WE_SLAVE_R3_S5     (MEMC_REG_BASE + 0x2cc)
#define PHY_FIFO_WE_SLAVE_R0_S6     (MEMC_REG_BASE + 0x2d0)
#define PHY_FIFO_WE_SLAVE_R1_S6     (MEMC_REG_BASE + 0x2d4)
#define PHY_FIFO_WE_SLAVE_R2_S6     (MEMC_REG_BASE + 0x2d8)
#define PHY_FIFO_WE_SLAVE_R3_S6     (MEMC_REG_BASE + 0x2dc)
#define PHY_FIFO_WE_SLAVE_R0_S7     (MEMC_REG_BASE + 0x2e0)
#define PHY_FIFO_WE_SLAVE_R1_S7     (MEMC_REG_BASE + 0x2e4)
#define PHY_FIFO_WE_SLAVE_R2_S7     (MEMC_REG_BASE + 0x2e8)
#define PHY_FIFO_WE_SLAVE_R3_S7     (MEMC_REG_BASE + 0x2ec)
#define PHY_FIFO_WE_SLAVE_R0_S8     (MEMC_REG_BASE + 0x2f0)
#define PHY_FIFO_WE_SLAVE_R1_S8     (MEMC_REG_BASE + 0x2f4)
#define PHY_FIFO_WE_SLAVE_R2_S8     (MEMC_REG_BASE + 0x2f8)
#define PHY_FIFO_WE_SLAVE_R3_S8     (MEMC_REG_BASE + 0x2fc)
#define PHY_WR_DATA_SLAVE_R0_S0     (MEMC_REG_BASE + 0x300)
#define PHY_WR_DATA_SLAVE_R1_S0     (MEMC_REG_BASE + 0x304)
#define PHY_WR_DATA_SLAVE_R2_S0     (MEMC_REG_BASE + 0x308)
#define PHY_WR_DATA_SLAVE_R3_S0     (MEMC_REG_BASE + 0x30C)
#define PHY_WR_DATA_SLAVE_R0_S1     (MEMC_REG_BASE + 0x310)
#define PHY_WR_DATA_SLAVE_R1_S1     (MEMC_REG_BASE + 0x314)
#define PHY_WR_DATA_SLAVE_R2_S1     (MEMC_REG_BASE + 0x318)
#define PHY_WR_DATA_SLAVE_R3_S1     (MEMC_REG_BASE + 0x31C)
#define PHY_WR_DATA_SLAVE_R0_S2     (MEMC_REG_BASE + 0x320)
#define PHY_WR_DATA_SLAVE_R1_S2     (MEMC_REG_BASE + 0x324)
#define PHY_WR_DATA_SLAVE_R2_S2     (MEMC_REG_BASE + 0x328)
#define PHY_WR_DATA_SLAVE_R3_S2     (MEMC_REG_BASE + 0x32C)
#define PHY_WR_DATA_SLAVE_R0_S3     (MEMC_REG_BASE + 0x330)
#define PHY_WR_DATA_SLAVE_R1_S3     (MEMC_REG_BASE + 0x334)
#define PHY_WR_DATA_SLAVE_R2_S3     (MEMC_REG_BASE + 0x338)
#define PHY_WR_DATA_SLAVE_R3_S3     (MEMC_REG_BASE + 0x33C)
#define PHY_WR_DATA_SLAVE_R0_S4     (MEMC_REG_BASE + 0x340)
#define PHY_WR_DATA_SLAVE_R1_S4     (MEMC_REG_BASE + 0x344)
#define PHY_WR_DATA_SLAVE_R2_S4     (MEMC_REG_BASE + 0x348)
#define PHY_WR_DATA_SLAVE_R3_S4     (MEMC_REG_BASE + 0x34C)
#define PHY_WR_DATA_SLAVE_R0_S5     (MEMC_REG_BASE + 0x350)
#define PHY_WR_DATA_SLAVE_R1_S5     (MEMC_REG_BASE + 0x354)
#define PHY_WR_DATA_SLAVE_R2_S5     (MEMC_REG_BASE + 0x358)
#define PHY_WR_DATA_SLAVE_R3_S5     (MEMC_REG_BASE + 0x35C)
#define PHY_WR_DATA_SLAVE_R0_S6     (MEMC_REG_BASE + 0x360)
#define PHY_WR_DATA_SLAVE_R1_S6     (MEMC_REG_BASE + 0x364)
#define PHY_WR_DATA_SLAVE_R2_S6     (MEMC_REG_BASE + 0x368)
#define PHY_WR_DATA_SLAVE_R3_S6     (MEMC_REG_BASE + 0x36C)
#define PHY_WR_DATA_SLAVE_R0_S7     (MEMC_REG_BASE + 0x370)
#define PHY_WR_DATA_SLAVE_R1_S7     (MEMC_REG_BASE + 0x374)
#define PHY_WR_DATA_SLAVE_R2_S7     (MEMC_REG_BASE + 0x378)
#define PHY_WR_DATA_SLAVE_R3_S7     (MEMC_REG_BASE + 0x37C)
#define PHY_WR_DATA_SLAVE_R0_S8     (MEMC_REG_BASE + 0x380)
#define PHY_WR_DATA_SLAVE_R1_S8     (MEMC_REG_BASE + 0x384)
#define PHY_WR_DATA_SLAVE_R2_S8     (MEMC_REG_BASE + 0x388)
#define PHY_WR_DATA_SLAVE_R3_S8     (MEMC_REG_BASE + 0x38C)
#define DDRC0_DBG                   (MEMC_REG_BASE + 0x3A8)
#define DDRC0_INTERRUPT             (MEMC_REG_BASE + 0x3F4)
#define PHY_DQ_OFFSET_0             (MEMC_REG_BASE + 0x390)
#define PHY_DQ_OFFSET_1             (MEMC_REG_BASE + 0x394)
#define PHY_DQ_OFFSET_2             (MEMC_REG_BASE + 0x398)
#define DDR_IO_CONTROL              (MEMC_REG_BASE + 0x458)

#define MEM_ECC_BYPASS              (MEMC_REG_BASE + 0x668)
#define MEMQ_MISC                   (MEMC_REG_BASE + 0x4b0)
#define MEM_RAM_SHUTDOWN            (MEMC_REG_BASE + 0x670)

/* AHB Register Map */

#define AHB_BASE                    0xdd860000
#define AHBC_CFG_CTL                (AHB_BASE + 0x2004)

/* USB misc control regsiters */

#define USB0_TUNE                   (AHB_BASE + 0x2018)
#define USB0_RST_CTL                (AHB_BASE + 0x201c)
#define USB0_CLK_CTL                (AHB_BASE + 0x2020)
#define USB1_TUNE                   (AHB_BASE + 0x2024)
#define USB1_RST_CTL                (AHB_BASE + 0x2028)
#define USB1_CLK_CTL                (AHB_BASE + 0x202c)
#define USB1_CFG                    (AHB_BASE + 0x2064)
#define USB2_TUNE                   (AHB_BASE + 0x20ac)
#define USB2_RST_CTL                (AHB_BASE + 0x20b0)
#define USB2_CLK_CTL                (AHB_BASE + 0x20b4)
#define USB2_CFG                    (AHB_BASE + 0x20b8)

#define USB_PORT_RESET              0x00000002
#define USB_POR                     0x00000001
#define USB_CLK_48MHZ               0x0000000a
#define USB_CFG_VALUE               0x00001000
#define USB_TUNE_VALUE              0x00730232

#define USB0_CLK_48MHZ              0x00000002

#define ENABLE_INTF                 (AHB_BASE + 0x2090)

/* UART_MODE_SEL Register */

#define UART_MODE_SEL               (AHB_BASE + 0x20cc)
#define UART_REFCLK_MASK            ~(1 << 5)
#define SYS_REFCLK_MODE             (0 << 5)
#define UART_SCLK_MODE              (1 << 5)

/* PLB5 DCR Registers Map */

#define PRID                        0x24000001
#define PMCR0                       0x24000005
#define PESR                        0x24000008
#define PEARL                       0x24000009
#define PEARH                       0x2400000A
#define PCICR                       0x2400000B

/* APM86x90 UART Base Address */

#define APM_UART0_BASE              (APM_UART_BASE + 0x03)
#define APM_UART1_BASE              (APM_UART_BASE + 0x1000 + 0x03)
#define APM_UART2_BASE              (APM_UART_BASE + 0x7000 + 0x03)
#define APM_UART3_BASE              (APM_UART_BASE + 0x8000 + 0x03)
#define UART_REG_ADDR_INTERVAL      4

/* APM86x90 SATA Base Address */

#define APM_SATA_BASE               (APM_PERIPHERAL_BASE + 0x300000)
#define APM_SATA_PHY_BASE_HI        0x5
#define APM_SATA_PHY_BASE_LOW       0x00000000

/* APM86x90 Interrupt Vectors */

#define SDU_INT                     0
#define LCD_CTL_INT                 1
#define DDR_CTL_INT                 2

#define QM_SAB0_INT                 5
#define SPI_INT                     6
#define DMA_CTL_INT                 7
#define QM_SAB1_INT                 8

#define UART0_INT                   11
#define UART1_INT                   12
#define UART2_INT                   13
#define UART3_INT                   14
#define I2C0_CTL_INT                15
#define I2C1_CTL_INT                16

#define QM_DEQMBOX0_INT             17
#define QM_DEQMBOX1_INT             18
#define QM_DEQMBOX2_INT             19
#define QM_DEQMBOX3_INT             20
#define QM_DEQMBOX4_INT             21
#define QM_DEQMBOX5_INT             22
#define QM_DEQMBOX6_INT             23
#define QM_DEQMBOX7_INT             24
#define QM_DEQMBOX8_INT             25
#define QM_DEQMBOX9_INT             26
#define QM_DEQMBOX10_INT            27
#define QM_DEQMBOX11_INT            28
#define QM_DEQMBOX12_INT            29
#define QM_DEQMBOX13_INT            30
#define QM_DEQMBOX14_INT            31
#define QM_DEQMBOX15_INT            32
#define QM_ERR_INT                  33

#define ETH_CSR_INT                 34
#define ETH_MAC_INT                 35
#define ETH_QMI_INT                 43
#define SATA_INT                    44

#define PCIEX0_INT_A                51
#define PCIEX0_INT_B                52
#define PCIEX0_INT_C                53
#define PCIEX0_INT_D                54
#define PCIEX1_INT_A                55
#define PCIEX1_INT_B                56
#define PCIEX1_INT_C                57
#define PCIEX1_INT_D                58
#define PCIEX2_INT_A                38
#define PCIEX2_INT_B                39
#define PCIEX2_INT_C                40
#define PCIEX2_INT_D                41

#define USB1_OHCI_INT               42
#define USB1_EHCI_INT               95
#define USB2_OHCI_INT               47
#define USB2_EHCI_INT               46

#define SDIO_INT                    89

/* SCU_SOCPLL_X Register Bit Defines */

#define PLL_DOWN                    0x80000000
#define BYPASS_MASK                 0x40000000
#define PLL_BYPASS                  BYPASS_MASK

#define CLKR_SHIFT                  24
#define CLKR_MASK                   0x3f
#define PLL_NR_RATIO(X)             (((X >> CLKR_SHIFT) & CLKR_MASK) + 1)
#define PLL_NR_SET(X)               (X << CLKR_SHIFT)

#define CLKOD_SHIFT                 20
#define CLKOD_MASK                  0x0f
#define PLL_OD_RATIO(X)             (((X >> CLKOD_SHIFT) & CLKOD_MASK) + 1)
#define PLL_OD_SET(X)               (X << CLKOD_SHIFT)

#define CLKF_MASK                   0x1fff
#define PLL_NF_RATIO(X)             ((X & CLKF_MASK) + 1)
#define PLL_NF_SET(X)               (X)
#define PLL_NF_ETH_DEFAULT          0x13

#define PLL_RESET_SHIFT             16
#define PLL_RESET                   (0x01 << PLL_RESET_SHIFT)

/* SCU_SOCDIV bit defines */

#define AXI_DIV_SHIFT               24
#define AXI_DIV_MASK                0x1f
#define AXI_DIV_RATIO(X)            ((X >> AXI_DIV_SHIFT) & AXI_DIV_MASK)

#define AHB_DIV_SHIFT               16
#define AHB_DIV_MASK                0x1f
#define AHB_DIV_RATIO(X)            ((X >> AHB_DIV_SHIFT) & AHB_DIV_MASK)

#define APB_DIV_SHIFT               8
#define APB_DIV_MASK                0x1f
#define APB_DIV_RATIO(X)            ((X >> APB_DIV_SHIFT) & APB_DIV_MASK)

/* SCU_SOCDIV1 bit define */

#define LCD_SER_DIV_SHIFT           24
#define LCD_SER_DIV_MASK            0x1f
#define LCD_SER_DIV_RATIO(X)        ((X >> LCD_SER_DIV_SHIFT) & LCD_SER_DIV_MASK)

#define PCIE_DIV_SHIFT              16
#define PCIE_DIV_MASK               0x1f
#define PCIE_DIV_RATIO(X)           ((X >> PCIE_DIV_SHIFT) & PCIE_DIV_MASK)

#define DDR2X_DIV_SHIFT             8
#define DDR2X_DIV_MASK              0x1f
#define DDR2X_DIV_RATIO(X)          ((X >> DDR2X_DIV_SHIFT) & DDR2X_DIV_MASK)

#define LCD_DIV_SHIFT               0
#define LCD_DIV_MASK                0x1f
#define LCD_DIV_RATIO(X)            ((X >> LCD_DIV_SHIFT) & LCD_DIV_MASK)

/* SCU_SOCDIV2 bit define */

#define CRYPTO_DIV_SHIFT            24
#define CRYPTO_DIV_MASK             0x1f
#define CRYPTO_DIV_RATIO(X)         ((X >> CRYPTO_DIV_SHIFT) & CRYPTO_DIV_MASK)

#define TRCLKIN_DIV_SHIFT           16

/* SCU_SOCDIV3 bit define */

#define IICCLK_MUX_SEL_SHIFT        28
#define IICCLK_APB_CLK              (0x01 << IICCLK_MUX_SEL_SHIFT)

/* SCU_SOCDIV4 bit defines */

#define RGMII1_DIV_SHIFT            16
#define RGMII1_DIV_MASK             0x1ff
#define RGMII1_DIV_RATIO(X)         ((X >> RGMII1_DIV_SHIFT) & RGMII1_DIV_MASK)
#define RGMII1_DIV_SET(X)           ((X & RGMII1_DIV_MASK) << RGMII1_DIV_SHIFT)

#define RGMII0_DIV_MASK             0x1ff
#define RGMII0_DIV_RATIO(X)         (X & RGMII1_DIV_MASK)
#define RGMII0_DIV_SET(X)           (X & RGMII1_DIV_MASK)

#define RGMII_DIV_DEFAULT_10M       0xC8
#define RGMII_DIV_DEFAULT_100M      0x14
#define RGMII_DIV_DEFAULT_1G        0x04

/* SCU_SOCDIV5 bit define */

#define EBUS_DIV_SHIFT              24
#define EBUS_DIV_MASK               0x0ff
#define EBUS_DIV_RATIO(X)           ((X >> EBUS_DIV_SHIFT) & EBUS_DIV_MASK)

#define SDIO_DIV_SHIFT              16
#define SDIO_DIV_MASK               0x0ff
#define SDIO_DIV_RATIO(X)           ((X >> SDIO_DIV_SHIFT) & SDIO_DIV_MASK)

/* SCU_SOCPLLADJ bit define */

#define BWADJ_MASK                  0x0fff
#define PLL2_BWADJ_DEFAULT          0x1f
#define PLL4_BWADJ_DEFAULT          0x13

/* AHP ENABLE_INTF Register Setting */

#define AHB_APB_CLK_RATIO_MASK      0xcfffffff
#define AHB_APB_CLK_RATIO           0x30000000

/* PLB Coherent Interface Configuration Register bit define */

#define DIE_MASK                    0x00000002

/* SCU_MRDY bit seting */

#define MPIC_RDY_MASK               (0x01 << 28)
#define SATA0_RDY_MASK              (0x01 << 27)
#define SATA1_RDY_MASK              (0x01 << 26)
#define PCIEX0_RDY_MASK             (0x01 << 22)
#define PCIEX1_RDY_MASK             (0x01 << 21)
#define PCIEX2_RDY_MASK             (0x01 << 20)
#define ENET_RDY_MASK               (0x01 << 19)
#define MEMC_RDY_MASK               (0x01 << 14)
#define OCM_RDY_MASK                (0x01 << 11)
#define QMTM_RDY_MASK               (0x01 << 9)
#define CLE_RDY_MASK                (0x01 << 7)

#define WRLVL_INIT_RATIO_MASK       0x3fffffff
#define RDDATA_EN_MASK              0x1f

#define DDRC_MODE_MASK              0x03

#define DDRC_RMODE_MASK             (0x03 << 24)
#define DDRC_PHY_RST                (0x01 << 23)
#define DDRC_OTD_MASK               (0x03 << 20)
#define DDRC_ACTIVE_RANK_MASK       (0x03 << 16)

#define DDRC_HIGH_SPEED_MODE        (0x02 << 24)

#define DDRC_OTD_SETTING            (0x00 << 20)
#define DDRC_ACTIVE_ALL_RANK        (0x0f << 16)

#define MEM_RDY_TIMEOUT_COUNT       10000

/* MPIC Register Map */

#define MPIC_BASE                   0xdf400000

/* MPIC Interrupt PRR register */

#define APM_MPIC_PRR                (MPIC_BASE + 0x1070)
#define APM_MPIC_PRR_RST1           0x00000002

/* L2C Configuration Register 0 (L2CR0) bit define */

#define L2CR0_AS_SHIFT              28
#define L2CR0_AS_MASK               (0x03 << L2CR0_AS_SHIFT)
#define L2CR0_AS_256K               (0x01 << L2CR0_AS_SHIFT)
#define L2CR0_TAA_SHIFT             24
#define TAA_MASK                    (~(0x03 << L2CR0_TAA_SHIFT))
#define L2CR0_DAA_SHIFT             20
#define DAA_MASK                    (~(0x03 << L2CR0_DAA_SHIFT))
#define L2CR0_DECC_SHIFT            3
#define DECC_DISABLE                (0x01 << L2CR0_DECC_SHIFT)
#define L2CR0_DECA_SHIFT            2
#define DECA_DISABLE                (0x01 << L2CR0_DECA_SHIFT)
#define L2CR0_TAI_SHIFT             1
#define L2CR0_TAI                   (0x01 << L2CR0_TAI_SHIFT)

/* L2C Configuration Register 1 (L2CR1) bit define */

#define L2CR1_FAMES_SHIFT           31
#define FAMES_MASK                  (~(0x03 << L2CR1_FAMES_SHIFT))
#define FAMES_256K                  (0x02 << L2CR1_FAMES_SHIFT)
#define L2CR1_PUBFAM_SHIFT          30
#define PUBFAM                      (0x01 << L2CR1_PUBFAM_SHIFT)

/* L2C Configuration Register 2 (L2CR2) bit define */

#define L2CR2_SNPME                 0x80000000
#define L2CR2_CSNPKF                0x08000000
#define L2CR2_SNPRQPD               0x00300000
#define L2CR2_L1CE                  0x00008000

#define L2CR2_DEFAULT_VAL           (L2CR2_SNPME | L2CR2_CSNPKF | \
                                     L2CR2_SNPRQPD | L2CR2_L1CE)

/* L2C Configuration Register 3 (L2CR3) bit define */

#define L2CR3_MSTPD                 0x03000000

/* Machine Check Reporting Enable Register (L2MCRER) bit define */

#define L2MCRER_ITSBEN_SHIFT        30
#define L2MCRER_ITDBEN_SHIFT        29
#define L2MCRER_IDSBEN_SHIFT        28
#define L2MCRER_IDDBEN_SHIFT        27
#define L2MCRER_DTSBEN_SHIFT        26
#define L2MCRER_DTDBEN_SHIFT        25
#define L2MCRER_DDSBEN_SHIFT        24
#define L2MCRER_DDDBEN_SHIFT        23
#define L2MCRER_SNPTSBEN_SHIFT      22
#define L2MCRER_SNPTDBEN_SHIFT      21
#define L2MCRER_SNPDSBEN_SHIFT      20
#define L2MCRER_SNPDDBEN_SHIFT      19
#define L2MCRER_FARMEN_SHIFT        18
#define L2MCRER_MSTRQEN_SHIFT       15
#define L2MCRER_MSTRDEN_SHIFT       14
#define L2MCRER_MSTRDPEN_SHIFT      13
#define L2MCRER_SNPAPEN_SHIFT       12
#define L2MCRER_SLVWDEN_SHIFT       10
#define L2MCRER_SLVWDPEN_SHIFT      9
#define L2MCRER_SLVAPEN_SHIFT       8
#define L2MCRER_SLVBEPEN_SHIFT      7
#define L2MCRER_SLVDSBEN_SHIFT      6
#define L2MCRER_SLVDDBEN_SHIFT      5
#define L2MCRER_SLVIAEEN_SHIFT      4
#define L2MCRER_SLVICEEN_SHIFT      3
#define L2MCRER_DCRTOEN_SHIFT       0

#define ITSBEN                      (0x01 << L2MCRER_ITSBEN_SHIFT)
#define ITDBEN                      (0x01 << L2MCRER_ITDBEN_SHIFT)
#define IDSBEN                      (0x01 << L2MCRER_IDSBEN_SHIFT)
#define IDDBEN                      (0x01 << L2MCRER_IDDBEN_SHIFT)
#define DTSBEN                      (0x01 << L2MCRER_DTSBEN_SHIFT)
#define DTDBEN                      (0x01 << L2MCRER_DTDBEN_SHIFT)
#define DDSBEN                      (0x01 << L2MCRER_DDSBEN_SHIFT)
#define DDDBEN                      (0x01 << L2MCRER_DDDBEN_SHIFT)
#define FRMEN                       (0x01 << L2MCRER_FARMEN_SHIFT)

#define L2MCRER_INIT_VALUE          ((ITSBEN | ITDBEN | IDSBEN | IDDBEN | \
                                     DTSBEN | DTDBEN | DDSBEN | DDDBEN) & (~FRMEN))

/* PCI Express */

#define APM_PCIEX0_CSR_BASE         (APM_CSR_BASE + 0x80000)
#define APM_PCIEX1_CSR_BASE         (APM_CSR_BASE + 0x90000)
#define APM_PCIEX2_CSR_BASE         (APM_CSR_BASE + 0xf0000)

#define APM_PCIEX0_CFG_BASE         0xd0000000
#define APM_PCIEX0_CFG_MAP_SIZE     0x100000
#define APM_PCIEX1_CFG_BASE         0xd1000000
#define APM_PCIEX1_CFG_MAP_SIZE     0x100000
#define APM_PCIEX2_CFG_BASE         0xd2000000
#define APM_PCIEX2_CFG_MAP_SIZE     0x100000

#define PCIEX0_MEM_PHY_ADRS_HI      0x6
#define PCIEX0_MEM_ADRS             0xa0000000
#define PCIEX0_MEM_SIZE             0x08000000
#define PCIEX0_MEMIO_ADRS           0xa8000000
#define PCIEX0_MEMIO_SIZE           0x04000000
#define PCIEX0_IO_ADRS              0xac000000
#define PCIEX0_IO_SIZE              0x04000000
#define PCIEX0_MAP_SIZE             0x10000000

#define PCIEX1_MEM_PHY_ADRS_HI      0x7
#define PCIEX1_MEM_ADRS             0xb0000000
#define PCIEX1_MEM_SIZE             0x08000000
#define PCIEX1_MEMIO_ADRS           0xb8000000
#define PCIEX1_MEMIO_SIZE           0x04000000
#define PCIEX1_IO_ADRS              0xbc000000
#define PCIEX1_IO_SIZE              0x04000000
#define PCIEX1_MAP_SIZE             0x10000000

#define PCIEX2_MEM_PHY_ADRS_HI      0xa
#define PCIEX2_MEM_ADRS             0xc0000000
#define PCIEX2_MEM_SIZE             0x08000000
#define PCIEX2_MEMIO_ADRS           0xc8000000
#define PCIEX2_MEMIO_SIZE           0x04000000
#define PCIEX2_IO_ADRS              0xcc000000
#define PCIEX2_IO_SIZE              0x04000000
#define PCIEX2_MAP_SIZE             0x10000000

/* SERDES Registers */

#define APM_PCS0_CSR_BASE           (APM_CSR_BASE + 0x84000)
#define APM_PCS1_CSR_BASE           (APM_CSR_BASE + 0x94000)
#define APM_PCS2_CSR_BASE           (APM_CSR_BASE + 0xf4000)

#define PCS_X1_CTL0(base)           ((base) + 0x0000)
#define PCS_X1_CTL1(base)           ((base) + 0x0004)
#define PCS_X1_STS1(base)           ((base) + 0x0008)

#define PCS_X1_MODE_MASK            0xfffffffe
#define PCS_X1_MODE_PCIE            0x00000001
#define PCS_X1_MODE_SATA            0x00000000

#define PCS_X1_CTL0_VALUE           0x3E4F102B
#define PCS_X1_CTL0_RESET           0x00000020
#define PCS_X1_CTL1_VALUE           0x00812044
#define PCS_X1_STS1_CLK_RDY         0x00000800
#define PCS_X1_STS1_PLL_RDY         0x00000400

#define PCS_X4_CTL0(base)           ((base) + 0x0000)
#define PCS_X4_CTL1(base)           ((base) + 0x0004)
#define PCS_X4_STS0(base)           ((base) + 0x0008)
#define PCS_X4_STS1(base)           ((base) + 0x000c)
#define PCS_X4_REG_ADDR(base)       ((base) + 0x0010)
#define PCS_X4_REG_WR_DATA(base)    ((base) + 0x0014)
#define PCS_X4_REG_CMD(base)        ((base) + 0x0018)
#define PCS_X4_REG_CMD_DONE(base)   ((base) + 0x001c)
#define PCS_X4_REG_RD_DATA(base)    ((base) + 0x0020)

#define PCS_X4_CTL0_RESET           0x00000001
#define PCS_X4_STS1_CLK_RDY         0x00000800
#define PCS_X4_STS1_PLL_RDY         0x00000400

#define PCS_X4_REG_CMD_RD           0x00000001
#define PCS_X4_REG_CMD_WT           0x00000002

#define PCS_X4_REG_CMD_RD_DONE      0x00000001
#define PCS_X4_REG_CMD_WT_DONE      0x00000002

#define PCS_TIMEOUT                 1000
#define PCS_DELAY_IN_US             10
#define PCS_WR_DELAY_IN_US          1000
#define PCS_RST_DELAY_IN_US         1000

/* Misc registers */

#define APM_PCIEX_MEM_SHUTDOWN      0x7070

/* USB */

#define APM_OHCI1_BASE              (APM_PERIPHERAL_BASE + 0x10000)
#define APM_EHCI1_BASE              (APM_PERIPHERAL_BASE + 0x10400)
#define APM_OHCI2_BASE              (APM_PERIPHERAL_BASE + 0x10800)
#define APM_EHCI2_BASE              (APM_PERIPHERAL_BASE + 0x10c00)

/* SDHC */

#define APM_SDHC0_BASE              (APM_PERIPHERAL_BASE + 0x40000)
#define APM_SDHC1_BASE              (APM_PERIPHERAL_BASE + 0x40100)

/* SDHC AHB register offset */

#define SDR_SDIO_AHB2AXI_CFG        0x2010
#define SDHC_TAP_DELAY              0x2030
#define SDHC_CAP_REG_LOW_S1         0x2034
#define SDHC_CAP_REG_HIGH_S1        0x2038
#define SDHC_CAP_REG_LOW_S2         0x203c
#define SDHC_CAP_REG_HIGH_S2        0x2040
#define SDHC_INIT_PRESET            0x2044
#define SDHC_DEFAULT_PRESET         0x2048
#define SDHC_HIGH_SPEED_PRESET      0x204c
#define SDHC_SDR12_PRESET           0x2050
#define SDHC_SDR25_PRESET           0x2054
#define SDHC_DATARAM                0x20a0

#define APM_SDHC_CAP_REG_LO_INIT_VAL    0x01fc1970
#define APM_SDHC_CAP_REG_HI_INIT_VAL    0x000c008f

#ifndef _ASMLANGUAGE

/* System information structure */

typedef struct
    {
    UINT32  sysFreq;
    UINT32  cpuPllFreq;
    UINT32  cpu0ClkFreq;
    UINT32  cpu1ClkFreq;
    UINT32  plbClkFreq;
    UINT32  tracePpcClkFreq;
    UINT32  socPllOutClkFreq;
    UINT32  hbfClkFreq;
    UINT32  ahbClkFreq;
    UINT32  apbClkFreq;
    UINT32  pcieClkFreq;
    UINT32  cryptoClkFreq;
    UINT32  ebusClkFreq;
    UINT32  sdIoClkFreq;
    UINT32  ddrPllOutClkFreq;
    UINT32  ddr2xClkFreq;
    UINT32  ddr1xClkFreq;
    UINT32  ethPllOutFreq;
    UINT32  mac0TxClkFreq;
    UINT32  mac1TxClkFreq;
    UINT32  ptpClkFreq;
    }SYS_INFO;

typedef volatile unsigned int   VUINT32;
#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCamp86x90h */
