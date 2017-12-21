/* lsi_acp34xx.h - LSI ACP34XX ELPASO Board header */

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
01o,30nov12,j_z  add SRIO address.
01n,03aug12,x_s  added support for LSI ACP34XX JUNCTION target board.
01m,15may12,x_s  added support for silicon Rev 2.1.
01l,30mar12,x_z  added macro for L2 Cache DCR base address; 
                 replaced vxPirSave() with sysHwInit0() for SYS_HW_INIT_0.
                 (WIND00315263)
01k,08mar11,x_z  added macros for System Clock Control registers and clocks.
01j,23feb11,x_z  added NAND_BLOCK_SIZE_MAX.
01i,22jan11,x_s  added PCIe support.
01h,10jan11,x_z  Added additional PLB6 register definitions.
01g,23dec10,x_z  Moved L2 Cache defines to ARCH.
01f,22dec10,x_s  added I2C support.
01e,21dec10,x_s  added SPI support.
01d,18nov10,x_z  Added support for read-only PIR register and PLB6 configuration.
01c,09nov10,x_z  modified to support EP501G1 controller and USB for X1V2 board.
01b,26oct10,x_z  added support for uAMP.
01a,27may10,x_z  written.
*/

#ifndef __INClsi_acp34xxh
#define __INClsi_acp34xxh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * Memory Address Map
 *
 * PPC476 implements a uniform 32-bit effective address (EA) space. Effective
 * addresses are expanded into virtual addresses and then translated to 42-bit
 * (4TB) real addresses by the memory management unit.
 */

/* DRAM (must 1-1 map) */

#define DRAM_BASE_ADRS              0x00000000
#define DRAM_BASE_ADRS_PHYS_LO      DRAM_BASE_ADRS
#define DRAM_BASE_ADRS_PHYS_HI      0x000
#define DRAM_SIZE                   0x40000000

/* Peripherals Register */

#define PER_BASE_ADRS               0xF0800000
#define PER_BASE_ADRS_PHYS_LO       0x00400000
#define PER_BASE_ADRS_PHYS_HI       0x020
#define PER_SIZE                    0x00200000
#define PER_SIZE_TLB                0x00100000 /* used for TLB */

#define GPIO0_REG_BASE              0
#define GPIO1_REG_BASE              0x1000
#define SSP_REG_BASE                0x2000 /* Serial SPI */
#define I2C_REG_BASE                0x3000
#define UART0_REG_BASE              0x4000
#define UART1_REG_BASE              0x5000
#define SSP_TIMER_REG_BASE          0x8000 /* SPI Clock */
#define I2C_TIMER_REG_BASE          0x8020
#define UART0_TIMER_REG_BASE        0x8040
#define UART1_TIMER_REG_BASE        0x8060 /* General Purpose if UART 1 is */
                                           /* unused */
#define WDT_TIMER_REG_BASE          0x8080 /* System Watchdog */
#define GP5_TIMER_REG_BASE          0x80A0 /* General Purpose */
#define GP6_TIMER_REG_BASE          0x80C0
#define GP7_TIMER_REG_BASE          0x80E0
#define MDIO_REG_BASE               0x9000
#define APB2RC_REG_BASE             0xA000
#define GPREG_REG_BASE              0xC000
#define SRIO_REG_BASE               0x20000
#define PL301_REG_BASE              0x21000 /* PL301 Lab Module */
#define NAND_REG_BASE               0x40000
#define TRNG_REG_BASE               0x60000
#define FEMAC_REG_BASE              0x80000
#define USB_REG_BASE                0xA0000
#define PCIE_CFG_REG_BASE           0xC0000
#define GPDMA_REG_BASE              0xE0000
#define NCA_REG_BASE                0x120000
#define PKA_REG_BASE                0x140000

/* Local CPU Memory (unmapped) */

#define LCM_BASE_ADRS               0xF0A00000
#define LCM_BASE_ADRS_PHYS_LO       0x00000000
#define LCM_BASE_ADRS_PHYS_HI       0x010
#define LCM_SIZE                    0x00100000

/* BOOT ROM (may be unmapped) */

#define BOOTROM_BASE_ADRS           0xF0B00000
#define BOOTROM_BASE_ADRS_PHYS_LO   0x00000000
#define BOOTROM_BASE_ADRS_PHYS_HI   0x021
#define BOOTROM_SIZE                0x00100000

/* Registers Map */

/* APB-1 Peripherals */

/* SSP Controller */

#define SSP_BA            (PER_BASE_ADRS + SSP_REG_BASE)

#define SSP_SSPCR0        (SSP_BA + 0x000) /* Control register 0 */
#define SSP_SSPCR1        (SSP_BA + 0x004) /* Control register 1 */
#define SSP_SSPDR         (SSP_BA + 0x008) /* Receive/Transmit FIFO data register */
#define SSP_SSPSR         (SSP_BA + 0x00c) /* Status register */
#define SSP_SSPCPSR       (SSP_BA + 0x010) /* Clock prescale register */
#define SSP_SSPIMSC       (SSP_BA + 0x014) /* Interrupt mask set and clear register */
#define SSP_SSPRIS        (SSP_BA + 0x018) /* Raw interrupt status register */
#define SSP_SSPMIS        (SSP_BA + 0x01c) /* Masked interrupt status register */
#define SSP_SSPICR        (SSP_BA + 0x020) /* Interrupt clear register */
#define SSP_SSPDMACR      (SSP_BA + 0x024) /* DMA control register */
#define SSP_SSPCSR        (SSP_BA + 0x030) /* Chip select register */

/* bit definitions for SSPCR0 */

#define SSP_SSPCR0_SCR_VALUE      0x3100 /* Serial clock rate */
#define SSP_SSPCR0_FPF_MOTO       0x00 /* Motorola SPI frame format */
#define SSP_SSPCR0_FPF_TI         0x10 /* TI synchronous serial frame format */
#define SSP_SSPCR0_FPF_MICROWIRE  0x20 /* National Microwire frame format */
#define SSP_SSPCR0_FPF_RESERVE    0x30 /* Reserved, undefined operation */
#define SSP_SSPCR0_DSS(x)         (x - 1) /* Data size select */
#define SSP_SSPCR0_DATA_MASK_8    0xFF /* 8 bit data size mask */

/* bit definitions for SSPCR1 */

#define SSP_SSPCR1_SSE_DISABLE    0x0    /* SSP operation disabled */
#define SSP_SSPCR1_SSE_ENABLE     0x2    /* SSP operation enabled */

/* bit definitions for SSPCPSR */

#define SSP_SSPCPSR_CPSDVSR       0x2    /* From 2 to 254, by default 2 */

/* bit definitions for SSPCSR */

#define SSP_SSPCSR_CHIP_SEL_MASK  0x1F   /* Chip select mask for SSP controller */

/* bit definitions for SSPSR */

#define SSP_SSPSR_REFIFO_FULL       0x8    /* Receive FIFO is full */
#define SSP_SSPSR_REFIFO_NOT_EMPTY  0x4    /* Receive FIFO is not empty */

/* I2C Controller */

#define I2C_CTRL_FREQ           100000 /* 100K Hz(only supported) */

#define I2C_BA                  (PER_BASE_ADRS + I2C_REG_BASE)

/* Master Mode Register */

#define I2C_TX_CFG              (I2C_BA + 0x00) /* Transmit Config */
#define I2C_RX_CFG              (I2C_BA + 0x04) /* Recieve Config */
#define I2C_TX_STATUS           (I2C_BA + 0x08) /* Transmit Status */
#define I2C_RX_STATUS           (I2C_BA + 0x0C) /* Recieve Status */
#define I2C_INT_EN              (I2C_BA + 0x10) /* Interrupt Enable */
#define I2C_INT_CLR             (I2C_BA + 0x14) /* Interrupt Clear */
#define I2C_INT_STATUS          (I2C_BA + 0x18) /* Interrupt status */
#define I2C_CLK_CFG             (I2C_BA + 0x1C) /* Clock Config */
#define I2C_START_CFG           (I2C_BA + 0x20) /* Start Setup/Hold Config */
#define I2C_STOP_CFG            (I2C_BA + 0x24) /* Stop Setup/Hold Config */
#define I2C_DATA_CFG            (I2C_BA + 0x28) /* Data Setup/Hold Config */
#define I2C_BP_MODE             (I2C_BA + 0x2C) /* Bypass Mode */
#define I2C_SLAVE_ADRS          (I2C_BA + 0x30) /* Slave Address */
#define I2C_TX_DATA0            (I2C_BA + 0x34) /* Transmit Data 0 */
#define I2C_TX_DATA1            (I2C_BA + 0x38) /* Transmit Data 1 */
#define I2C_RX_DATA0            (I2C_BA + 0x3C) /* Recieve Data 0 */
#define I2C_RX_DATA1            (I2C_BA + 0x40) /* Recieve Data 1 */

#define I2C_CLK_CFG_SCL_LOW          (10 * 40)
#define I2C_CLK_CFG_SCL_HIGH         (10 * 20)
#define I2C_START_CFG_SETUP_PERIOD   0
#define I2C_START_CFG_HOLD_PERIOD    128
#define I2C_STOP_CFG_SETUP_PERIOD    128
#define I2C_STOP_CFG_HOLD_PERIOD     0
#define I2C_DATA_CFG_SETUP_PERIOD    20
#define I2C_DATA_CFG_HOLD_PERIOD     20

#define I2C_CLK_CFG_VALUE          (I2C_CLK_CFG_SCL_LOW | \
                                    (I2C_CLK_CFG_SCL_HIGH << 16 ))

#define I2C_START_CFG_VALUE        (I2C_START_CFG_SETUP_PERIOD | \
                                    (I2C_START_CFG_HOLD_PERIOD << 16 ))

#define I2C_STOP_CFG_VALUE         (I2C_STOP_CFG_SETUP_PERIOD | \
                                    (I2C_STOP_CFG_HOLD_PERIOD << 16 ))

#define I2C_DATA_CFG_VALUE         (I2C_DATA_CFG_SETUP_PERIOD | \
                                    (I2C_DATA_CFG_HOLD_PERIOD << 16 ))

/* bit definitions transmit config register */

#define I2C_TX_CFG_READY           0x00000001         /* transmit ready */
#define I2C_TX_CFG_MASTER_MODE     0x00000100         /* master mode */
#define I2C_TX_CFG_TRANSCMD_READ   0x00000200         /* transmit command */
#define I2C_TX_CFG_ZEROALL         0x00000400         /* zero all conrol & data */
#define I2C_TX_CFG_STOP_CLOCK      0x20000000         /* stop i2c clock */


/* bit definitions receive config register */

#define I2C_RX_CFG_READY           0x00000001         /* receive ready */

/* bit definitions for done bit of Tx & Rx Status Register */

#define I2C_TX_RX_DONE          0x1

/* bit definitions for Recieve Data */

#define I2C_RX_DATA_8BIT_MASK   0xFF

/* bit definitions for Interrupt Enable Register */

#define I2C_INT_TX_FIN_EN       0x1
#define I2C_INT_TX_ERR_EN       0x2
#define I2C_INT_RX_FIN_EN       0x4
#define I2C_INT_RX_ERR_EN       0x8

/* bit definitions for Interrupt status Register */

#define I2C_INT_TX_ERR          0x1
#define I2C_INT_RX_FIN          0x2
#define I2C_INT_TX_FIN          0x4
#define I2C_INT_RX_ERR          0x8

/* UART Controller */

#define UART0_BA                    (PER_BASE_ADRS + UART0_REG_BASE)
#define UART1_BA                    (PER_BASE_ADRS + UART1_REG_BASE)

/* Timer */

#define SSP_TIMER_BA                (PER_BASE_ADRS + SSP_TIMER_REG_BASE)
#define I2C_TIMER_BA                (PER_BASE_ADRS + I2C_TIMER_REG_BASE)
#define UART0_TIMER_BA              (PER_BASE_ADRS + UART0_TIMER_REG_BASE)
#define UART1_TIMER_BA              (PER_BASE_ADRS + UART1_TIMER_REG_BASE)
#define WDT_TIMER_BA                (PER_BASE_ADRS + WDT_TIMER_REG_BASE)
#define GP5_TIMER_BA                (PER_BASE_ADRS + GP5_TIMER_REG_BASE)
#define GP6_TIMER_BA                (PER_BASE_ADRS + GP6_TIMER_REG_BASE)
#define GP7_TIMER_BA                (PER_BASE_ADRS + GP7_TIMER_REG_BASE)

/* Register Offset */

#define AXIS_TIMER_LOAD_VAL         0x00 /* Load Value */
#define AXIS_TIMER_VAL              0x04 /* Value */
#define AXIS_TIMER_CTRL             0x08 /* Control */
#define AXIS_TIMER_INT_CLR          0x0C /* Interrupt Clear */
#define AXIS_TIMER_RAW_INT_STATUS   0x10 /* Raw Interrupt Status */
#define AXIS_TIMER_INT_STATUS       0x14 /* Interrupt Status */
#define AXIS_TIMER_BGLOAD_VAL       0x18 /* Background Load Value */

/* bit definitions for Control Register */

#define AXIS_TIMER_EN               0x80 /* Enable */
#define AXIS_TIMER_MODE_PERIOD      0x40 /* Periodic Mode */
#define AXIS_TIMER_INT_EN           0x20 /* Interrupt Enable */
#define AXIS_TIMER_MODE_OUTPUT      0x10 /* Output Mode */
#define AXIS_TIMER_MODE_32BIT       0x02 /* 32 Bit Counter Mode */
#define AXIS_TIMER_MODE_ONE_SHOT    0x01 /* One-Shot Mode */

/* bit definitions for all Interrupt Registers */

#define AXIS_TIMER_INT              0x01

/* MDIO Controller */

#define MDIO_BA                     (PER_BASE_ADRS + MDIO_REG_BASE)

/* MDIO clock configuration */

#define MDIO_CLK_PERD               0x2C /* MDIO clock period */
#define MDIO_CLK_OFFSET             0x10 /* MDIO input sampled point */

/* General Purpose Regisetr Module */

#define GPREG_BA                    (PER_BASE_ADRS + GPREG_REG_BASE)

/* Register Offset */

/*
 * USB, MAC and GP-DMA) have 32 bits of address bus, but ACP34XX memory map uses
 * 38-bit addressing. The block provides the additional 6 bits of address.
 */

#define GPREG_GPDMA_EXT_ADRS        (GPREG_BA + 0x00) /* GPDMA */
#define GPREG_FEMAC_EXT_ADRS        (GPREG_BA + 0x04) /* FEMAC */
#define GPREG_USB_EXT_ADRS          (GPREG_BA + 0x08) /* USB */

#define GPREG_AXIS_STATUS           (GPREG_BA + 0x0C) /* AXIS STATUS */

#define GPREG_PHY_CTRL0             (GPREG_BA + 0x14) /* PHY CONTROL */

/* bit definitions for GPREG_PHY_CTRL0 Register */

#define GPREG_PHY_CTRL0_X1          0x0001
#define GPREG_PHY_CTRL0_X2          0x1001
#define GPREG_PHY_CTRL0_X3          0x0201
#define GPREG_PHY_CTRL0_X4          0x0301

/* bit definitions for AXIS STATUS Register */

#define GPREG_AXIS_NAND_ECC_BUSY    0x10000000 /* NAND Controller ECC Busy */
                                               /* Status */
#define GPREG_AXIS_GPDMA3_INT       0x00080000 /* GPDMA channel 3 interrupt */
#define GPREG_AXIS_GPDMA2_INT       0x00040000 /* GPDMA channel 2 interrupt */
#define GPREG_AXIS_GPDMA1_INT       0x00020000 /* GPDMA channel 1 interrupt */
#define GPREG_AXIS_GPDMA0_INT       0x00010000 /* GPDMA channel 0 interrupt */

#define GPREG_AXIS_FEMAC_DMA_INT    0x00004000 /* FEMAC DMA interrupt */
#define GPREG_AXIS_FEMAC_RXINT      0x00002000 /* FEMAC RX interrupt */

#define GPREG_AXIS_USB_INT          0x00001000 /* USB interrupt */
#define GPREG_AXIS_SRIO_INT         0x00000800 /* SRIO interrupt */
#define GPREG_AXIS_PEI_INT          0x00000400 /* PEI interrupt */

#define GPREG_AXIS_TIMERC_INT       0x00000200 /* timerc combined interrupt */
#define GPREG_AXIS_TIMER7_INT       0x00000100 /* TIMER 7 interrupt */
#define GPREG_AXIS_TIMER6_INT       0x00000080 /* TIMER 6 interrupt */
#define GPREG_AXIS_TIMER5_INT       0x00000040 /* TIMER 5 interrupt */
#define GPREG_AXIS_TIMER3_INT       0x00000020 /* TIMER 3 interrupt */

#define GPREG_AXIS_UART1_INT        0x00000010 /* UART 1 interrupt */
#define GPREG_AXIS_UART0_INT        0x00000008 /* UART 0 interrupt */

#define GPREG_AXIS_I2C_INT          0x00000004 /* I2C interrupt */

/* bit definitions for AXIS Additional Address Register */

#define GPREG_EXT_ADRS_MASK         0x3F
#define GPREG_EXT_ADRS_WR_SHIFT     8
#define GPREG_EXT_ADRS_RD_SHIFT     0

/* PCIE */

#define PCIE0_BA                    (PER_BASE_ADRS + PCIE_CFG_REG_BASE)
#define PCIE1_BA                    (PCIE0_BA + 0x8000)
#define PCIE2_BA                    (PCIE1_BA + 0x8000)

/* SRIO */

#define SRIO_PER_BA                 (PER_BASE_ADRS + SRIO_REG_BASE)
#define SRIO_STD_BA                 (PER_BASE_ADRS + SRIO_REG_BASE + 0x800)

#define SRIO_BASE_ADRS_PHYS_LO      0x80000000
#define SRIO_BASE_ADRS_PHYS_HI      0x020

/* APB-2 Peripherals */

/* AXI Peripherals */

/* NAND Flash controller */

#define NAND_BA                     (PER_BASE_ADRS + NAND_REG_BASE)

#define NAND_TYPE_EP501G1           0
#define NAND_TYPE_EP501G3           1

/* Register Offset */

#define NAND_DATA                   (NAND_BA + 0x0000) /* Data */
#define NAND_CMD                    (NAND_BA + 0x8000) /* Command */
#define NAND_INDEX                  (NAND_BA + 0x8004) /* Index */
#define NAND_STATUS0                (NAND_BA + 0x8008) /* Status 0 */
#define NAND_STATUS1                (NAND_BA + 0x800C) /* Status 1 */
#define NAND_ID0                    (NAND_BA + 0x8010) /* ID 0 */
#define NAND_ID1                    (NAND_BA + 0x8014) /* ID 1 */
#define NAND_ID2                    (NAND_BA + 0x8018) /* ID 2 */
#define NAND_ID3                    (NAND_BA + 0x801C) /* ID 3 */
#define NAND_ID4                    (NAND_BA + 0x8020) /* ID 4 */
#define NAND_ID5                    (NAND_BA + 0x8024) /* ID 5 */
#define NAND_ID6                    (NAND_BA + 0x8028) /* ID 6 */
#define NAND_ID7                    (NAND_BA + 0x802C) /* ID 7 */
#define NAND_INT_EN                 (NAND_BA + 0x8030) /* Interrupt Enable */
#define NAND_INT_STATUS             (NAND_BA + 0x8034) /* Interrupt status */
#define NAND_INT                    (NAND_BA + 0x8038) /* Interrupt */
#define NAND_ECC_ADRS_LOG           (NAND_BA + 0x803C) /* ECC address log */
#define NAND_ECC_VAL                (NAND_BA + 0x8040) /* ECC value storage */
#define NAND_ECC_INJECT             (NAND_BA + 0x8044) /* ECC injection */
#define NAND_EXT_INDEX              (NAND_BA + 0x8048) /* Extended Index */
#define NAND_TIMING0                (NAND_BA + 0x804C) /* Timing 0 */
#define NAND_TIMING1                (NAND_BA + 0x8050) /* Timing 1 */
#define NAND_CFG                    (NAND_BA + 0x8054) /* Configuration */
#define NAND_PECC                   (NAND_BA + 0x8058) /* Page-wise ECC */
#define NAND_TIMING2                (NAND_BA + 0x805C) /* Timing 2 */

/* bit definitions for Command Register */

#define NAND_CMD_RD0                0x00
#define NAND_CMD_PAGEPROG_CONFIRM   0x10
#define NAND_CMD_ECC_STA_RD         0x23
#define NAND_CMD_OOB_RD             0x50
#define NAND_CMD_ERASE              0x60
#define NAND_CMD_STA                0x70
#define NAND_CMD_PAGEPROG           0x80
#define NAND_CMD_ID_RD              0x90
#define NAND_CMD_ERASE_CONFIRM      0xD0
#define NAND_CMD_PAGE_OPEN          0xE0
#define NAND_CMD_CE_DIS             0xEE
#define NAND_CMD_CE_DIS_EP501G3     0xFE
#define NAND_CMD_RST                0xFF

/* bit definitions for Index Register */

#define NAND_INDEX_CALC_512         0x7fffff
#define NAND_INDEX_CALC_2048        0x0fffff
#define NAND_INDEX_CALC_4096        0x07ffff
#define NAND_INDEX_CALC(n)          (((n) < 2048) ? NAND_INDEX_CALC_512 : \
                                    (NAND_INDEX_CALC_2048 / ((n) / 2048)))

/* bit definitions for Extended Index Register */

#define NAND_EXT_INDEX_CALC_512     0
#define NAND_EXT_INDEX_CALC_2048    (0xFFFFFFFF - NAND_INDEX_CALC_2048)
#define NAND_EXT_INDEX_CALC_4096    (0xFFFFFFFF - NAND_INDEX_CALC_4096)
#define NAND_EXT_INDEX_CALC(n)      (((n) < 2048) ? 0 : \
                                    (0xFFFFFFFF - NAND_INDEX_CALC(n)))

/* bit definitions for Status Register 0 and 1 */

#define NAND_STA_CHIP_MASK          0xff
#define NAND_STA_CHIP1_SHIFT        8
#define NAND_STA_CHIP2_SHIFT        16
#define NAND_STA_CHIP3_SHIFT        24

#define NAND_STA_FAIL               0x01
#define NAND_STA_FAIL_N1            0x02
#define NAND_STA_TRUE_READY         0x20
#define NAND_STA_READY              0x40
#define NAND_STA_WP                 0x80

/* bit definitions for Nand ID Register 0 and 1 */

#define NAND_ID_MCOC_MASK           0xff /* Maker Code Of Chip */
#define NAND_ID_MCOC1_SHIFT         8
#define NAND_ID_MCOC2_SHIFT         16
#define NAND_ID_MCOC3_SHIFT         24

/* bit definitions for Nand ID Register 2 and 3 */

#define NAND_ID_DCOC_MASK           0xff /* Device Code Of Chip */
#define NAND_ID_DCOC1_SHIFT         8
#define NAND_ID_DCOC2_SHIFT         16
#define NAND_ID_DCOC3_SHIFT         24

/* bit definitions for Nand ID Register 4 and 5 */

#define NAND_ID_TBICOC_MASK         0xff /* Third Byte ID Of Chip */
#define NAND_ID_TBICOC1_SHIFT       8
#define NAND_ID_TBICOC2_SHIFT       16
#define NAND_ID_TBICOC3_SHIFT       24

#define NAND_ID_CHIP_NUM_MASK       0x03 /* Internal Chip Number: */
#define NAND_ID_CHIP_NUM(n)         (1 << (n)) /* 0 - 1 chip included */
                                                   /* 1 - 2 chips included */
                                                   /* 2 - 4 chips included */
                                                   /* 3 - 8 chips included */

#define NAND_ID_CELL_TYPE_MASK      0x0C /* Cell Type (Level): */
#define NAND_ID_CELL_LVL(n)         (2 << ((n) >> 2)) /* 0 - 2 Level */
                                                           /* 1 - 4 Level */
                                                           /* 2 - 8 Level */
                                                           /* 3 - 16 Level */

#define NAND_ID_NUM_SPP_MASK        0x30 /* Number of Simultaneously */
                                             /* Programmed Pages */
#define NAND_ID_NUM_SPP(n)          (1 << ((n) >> 4)) /* 0 - 1 Page */
                                                           /* 1 - 2 Pages */
                                                           /* 2 - 4 Pages */
                                                           /* 3 - 8 Pages */

#define NAND_ID_INTERLEAVE          0x40 /* Interleave Program Support */
                                             /* Between multiple chips */
#define NAND_ID_CACHE_PROG          0x80 /* Cache Program Support */

/* bit definitions for Nand ID Register 6 and 7 */

#define NAND_ID_MPCOC_MASK          0xff /* Multi Plane Code Of Chip */
#define NAND_ID_MPCOC1_SHIFT        8
#define NAND_ID_MPCOC2_SHIFT        16
#define NAND_ID_MPCOC3_SHIFT        24

#define NAND_ID_PAGE_SIZE_MASK      0x3 /* Page Size (Without Spare Area) */
#define NAND_ID_PAGE_SIZE(n)        ((1 << 10) << (n))

#define NAND_ID_OOB_SIZE_MASK       0x4 /* Spare Area Size per 512 Bytes */
#define NAND_ID_OOB_SIZE(n, page)   ((8 << ((n) >> 2)) * (page >> 9))

#define NAND_ID_BLOCK_SIZE_MASK     0x30 /* Block Size Without Spare Area */
#define NAND_ID_BLOCK_SIZE(n)       ((1 << 16) << ((n) >> 4))
#define NAND_BLOCK_SIZE_MAX         (1 << 18) /* 256K Bytes */

#define NAND_ID_BUS_WIDTH_16        0x40 /* Bus Width */

/* bit definitions for Interrupt Enable Register */

#define NAND_INT_UCEE           0x1 /* ECC un-correction Interrupt Enable */
#define NAND_INT_CEE            0x2 /* ECC correction Interrupt Enable */
#define NAND_INT_ECCD           0x4 /* ECC disable */

/* bit definitions for Interrupt status Register */

#define NAND_INT_STATUS_UCEED   0x01 /* Un-Correctable ECC Detected */
#define NAND_INT_STATUS_CEED    0x02 /* Correctable ECC Detected */
#define NAND_INT_STATUS_RDY     0x20

/* bit definitions for ECC Value Storage Register */

#define NAND_ECC_VAL_MASK       0xFF

/* bit definitions for ECC Value Injection Register */

#define NAND_ECC_INJE           0x100 /* ECC Injection Enable */

/* bit definitions for Timing configuration Register 0 */

#define NAND_TIMING0_TS_MASK    0xF /* setup time for CLE, CE# and ALE to */
                                        /* command */
#define NAND_TIMING0_TWP_MASK   0xF /* WE#(write) pulse width */
#define NAND_TIMING0_TWP_SHIFT  8
#define NAND_TIMING0_TRP_MASK   0xF /* RE#(read) pulse width */
#define NAND_TIMING0_TRP_SHIFT  16
#define NAND_TIMING0_TH_MASK    0xF /* CLE, CE# and ALE hold time */
#define NAND_TIMING0_TH_SHIFT   24  /* relative to WE# rising edge */

/* bit definitions for Timing configuration Register 1 */

#define NAND_TIMING1_TWH_MASK   0xF /* WE#(write) pulse high time */
#define NAND_TIMING1_TRH_MASK   0xF /* RE#(read) pulse width */
#define NAND_TIMING1_TRH_SHIFT  8
#define NAND_TIMING1_TRR_MASK   0xF /* RY/BY#(busy) rising edge to read */
#define NAND_TIMING1_TRR_SHIFT  8   /* pulse falling edge */
#define NAND_TIMING1_TWB_MASK   0xF /* WE#(write) rising edge to RY/BY# */
#define NAND_TIMING1_TWB_SHIFT  8   /* (busy) falling edge */

/* bit definitions for Timing configuration Register 2 */

#define NAND_TIMING2_CCS_MASK   0xFF /* WE#(write) pulse high time */
#define NAND_TIMING2_WHR_MASK   0xFF /* RE#(read) pulse width */
#define NAND_TIMING2_WHR_SHIFT  8
#define NAND_TIMING2_RHW_MASK   0xFF /* RY/BY#(busy) rising edge to read */
#define NAND_TIMING2_RHW_SHIFT  16   /* pulse falling edge */

/* bit definitions for Configuration Register */

/* calcuate for NAND parameters */

#define NAND_CFG_SIZE_SHIFT     24  /* 128M bits */
#define NAND_CFG_LP_CALC_BASE   512 /* 512 bytes */

#define NAND_CFG_SIZE_64MB      0x000 /* 64M bits */
#define NAND_CFG_SIZE_128MB     0x001 /* 128M bits */
#define NAND_CFG_SIZE_256MB     0x002 /* 256M bits */
#define NAND_CFG_SIZE_512MB     0x003 /* 512M bits */
#define NAND_CFG_SIZE_1GB       0x004 /* 1G bits */
#define NAND_CFG_SIZE_2GB       0x005 /* 2G bits */
#define NAND_CFG_SIZE_4GB       0x006 /* 4G bits */
#define NAND_CFG_SIZE_8GB       0x007 /* 8G bits */

#define NAND_CFG_16BIT          0x008 /* 16 bits width */

#define NAND_CFG_NUM_1          0x000 /* number of FLASH in paralled for */
#define NAND_CFG_NUM_2          0x010 /* combined data width */
#define NAND_CFG_NUM_4          0x020

#define NAND_CFG_BANK_NUM_1     0x000 /* number of active banks(CE#) in */
#define NAND_CFG_BANK_NUM_2     0x040 /* cascade */
#define NAND_CFG_BANK_NUM_4     0x080

#define NAND_CFG_PAGE_MODE_MASK     0x03
#define NAND_CFG_PAGE_MODE_SHIFT    8
#define NAND_CFG_PAGE_MODE_512      0x00 /* page size == 512 bytes */
#define NAND_CFG_PAGE_MODE_2048     0x01 /* page size == 2048 bytes */
#define NAND_CFG_PAGE_MODE_4096     0x02 /* page size == 4096 bytes */
#define NAND_CFG_PAGE_MODE(n)       ((((n) < 2048) ? NAND_CFG_PAGE_MODE_512 : \
                                    ((n) / 2048)) << NAND_CFG_PAGE_MODE_SHIFT)

#define NAND_CFG_RAW_MODE       0x1000 /* 3 cycle row address */
#define NAND_CFG_RAW_MODE_EP501G3   0x2000 /* 3 cycle row address */

#define NAND_CFG_LP             0x0800 /* lagre page mode */
#define NAND_CFG_WP_DIS         0x0400 /* disable write protect */

/* bit definitions for Page-wise ECC Register */

#define NAND_PECC_ERR_BIT_MASK  0x0fff  /* error bit location */
#define NAND_PECC_ERR_SINGLE    0x1000  /* single error(correctable) */
#define NAND_PECC_ERR_MULTI     0x2000  /* uncorrectable multi-bit error */

/* NAND Flash Info */

#define NAND_CHIP_SIZE          0x40000000  /* 8G Bits */
#define NAND_CHIP_SHIFT         30

#define NAND_TIMING0_VAL        \
    ((5 & NAND_TIMING0_TS_MASK) | \
     ((12 & NAND_TIMING0_TWP_MASK) << NAND_TIMING0_TWP_SHIFT) | \
     ((12 & NAND_TIMING0_TRP_MASK) << NAND_TIMING0_TRP_SHIFT) | \
     ((8 & NAND_TIMING0_TH_MASK) << NAND_TIMING0_TH_MASK))

#define NAND_TIMING1_VAL        \
    ((3 & NAND_TIMING1_TWH_MASK) | \
     ((3 & NAND_TIMING1_TRH_MASK) << NAND_TIMING1_TRH_SHIFT) | \
     ((7 & NAND_TIMING1_TRR_MASK) << NAND_TIMING1_TRR_SHIFT) | \
     ((30 & NAND_TIMING1_TWB_MASK) << NAND_TIMING1_TWB_SHIFT))

#define NAND_TIMING2_VAL        \
    ((40 & NAND_TIMING2_CCS_MASK) | \
     ((16 & NAND_TIMING2_WHR_MASK) << NAND_TIMING2_WHR_SHIFT) | \
     ((20 & NAND_TIMING2_RHW_MASK) << NAND_TIMING2_RHW_SHIFT))

/* FEMAC Controller */

#define FEMAC_BA                (PER_BASE_ADRS + FEMAC_REG_BASE)

/* USB DR (Host) registers */

#define USBDR_REG_BA            (PER_BASE_ADRS + USB_REG_BASE)

/* These are little-endian */

#define USBDR_HWTXBUF           (USBDR_REG_BA + 0x10)

#define USBDR_CAPLENGTH         (USBDR_REG_BA + 0x100)
#define USBDR_HCIVERSION        (USBDR_REG_BA + 0x102)
#define USBDR_HCSPARAMS_L       (USBDR_REG_BA + 0x104)
#define USBDR_HCSPARAMS_H       (USBDR_REG_BA + 0x108)

#define USBDR_USBCMD            (USBDR_REG_BA + 0x140)
#define USBDR_USBSTS            (USBDR_REG_BA + 0x144)
#define USBDR_USBINTR           (USBDR_REG_BA + 0x148)
#define USBDR_FRINDEX           (USBDR_REG_BA + 0x14c)
#define USBDR_PERIODICLISTBASE  (USBDR_REG_BA + 0x154)
#define USBDR_ASYNCLISTADDR     (USBDR_REG_BA + 0x158)
#define USBDR_ASYNCTTSTS        (USBDR_REG_BA + 0x15c)
#define USBDR_BURSTSIZE         (USBDR_REG_BA + 0x160)
#define USBDR_TXTTFILLTUNING    (USBDR_REG_BA + 0x164)
#define USBDR_CONFIGFLAG        (USBDR_REG_BA + 0x180)
#define USBDR_PORTSC            (USBDR_REG_BA + 0x184)
#define USBDR_PORTSC_PTS        0xC0000000 /* Port transceiver select */
#define USBDR_PORTSC_PTS_ULPI   0x80000000 /* ULPI parallel interface */
#define USBDR_PORTSC_PTS_SERIAL 0xC0000000 /* FS/LS serial interface  */

#define USBDR_OTGSC             (USBDR_REG_BA + 0x1a4)
#define USBDR_USBMODE           (USBDR_REG_BA + 0x1a8)

/* USBDR_USBMODE registers bit definitions */

#define USBDR_USBMODE_CM        0x3 /* Controller mode */
#define USBDR_USBMODE_CM_OTG    0
#define USBDR_USBMODE_CM_DEVICE 2
#define USBDR_USBMODE_CM_HOST   3

/* USBDR_BURSTSIZE registers bit definitions */

#define USBDR_BURSTSIZE_TX_MASK     0x0000FF00 /* TX Burst Length */
#define USBDR_BURSTSIZE_TX_DEF      0x00004000
#define USBDR_BURSTSIZE_RX_MASK     0x000000FF /* RX Burst Length */

/* DCR registers */

/* L2 Cache controller */

#define L2CACHE_BAn(cpuId)          (0x300 + 0x100 * (cpuId)) 

/* System Clock Control  */

#define SYSCLK_MCGC                 0xD00 /* Main Clock Generator Control */
#define SYSCLK_MCGS                 0xD01 /* Main Clock Generator Status */

/* SYSCLK_MCGC register bit definitions */

#define SYSCLK_MCGC_PPC_CLK_MASK        0x3 /* PPC clock select: */
#define SYSCLK_MCGC_PPC_CLK_SHIFT       30  /* 3 is illegal */
#define SYSCLK_MCGC_PPC_CLK(x)          (((x) >> SYSCLK_MCGC_PPC_CLK_SHIFT) & \
                                        SYSCLK_MCGC_PPC_CLK_MASK)
#define SYSCLK_MCGC_PPC_CLK_REF0        0   /* ppc_clk = clk_ref0 */
#define SYSCLK_MCGC_PPC_CLK_PLLA        1   /* ppc_clk = clk_plla_ppc */
#define SYSCLK_MCGC_PPC_CLK_PLLA_D2     2   /* ppc_clk = clk_plla_ppc */


#define SYSCLK_MCGC_PER_CLK_REF0        0x08000000 /* per_clk = clk_ref0 */
#define SYSCLK_MCGC_PER_CLK_PLLB        0x00000000 /* per_clk = clk_pllb_ppc */

/* SYSCLK_MCGS register bit definitions */

#define SYSCLK_MCGS_PPC_PLL_LOCK        0x80000000 /* PPC PLL Lock status: */
                                                   /* 0: unclocked 1: locked */

/* System Reset Control */

#define SYSRST_MAIN_RST_CON         0xE00 /* Reset Configuration Register */

#define SYSRST_MAIN_RST_STA         0xE01 /* Main Reset Generator Status */

#define SYSRST_MAIN_RST_CTL         0xE02 /* Module Reset Control Register */ 

#define SYSRST_MAIN_RST_MIS         0xE03 /* Miscellaneous Control Register */ 

/* SYSRST_MAIN_RST_CON register bit definitions */

#define SYSRST_MAIN_RST_CON_PCI_SEL 0x4000     /* PCI/SRIO Select */

#define SYSRST_MAIN_RST_CON_PCI_MOD 0x3000     /* PCI/SRIO Mode */

#define SYSRST_MAIN_RST_CON_PCI_MOD_SHIFT  12  /* shift */

/* SYSRST_MAIN_RST_MIS register bit definitions */

#define SYSRST_MAIN_RST_MIS_PHY     0x00800000  /* Reset PHY */

/* SYSRST_MAIN_RST_STA register bit definitions */

#define SYSRST_MAIN_RST_STA_CORE_CPU_MASK   0xF0000000 /* Core CPU Reset */
#define SYSRST_MAIN_RST_STA_CORE_CPU(n)     (1 << (28 + (n)))
#define SYSRST_MAIN_RST_STA_CORE_REG_MASK   0x0F000000 /* Core CPU MRG Reset */
#define SYSRST_MAIN_RST_STA_CORE_REG(n)     (1 << (24 + (n)))
#define SYSRST_MAIN_RST_STA_CHIP_CPU_MASK   0x00F00000 /* Chip CPU Reset */
#define SYSRST_MAIN_RST_STA_CHIP_CPU(n)     (1 << (20 + (n)))
#define SYSRST_MAIN_RST_STA_CHIP_REG        0x00080000 /* Chip CPU MRG Reset */
#define SYSRST_MAIN_RST_STA_SYS_PIN         0x00040000 /* System Pin Reset */
#define SYSRST_MAIN_RST_STA_SYS_WDT         0x00020000 /* System Watchdog */
                                                       /* Timer Reset */
#define SYSRST_MAIN_RST_STA_SYS_CPU_MASK    0x0001E000 /* System CPU Reset */
#define SYSRST_MAIN_RST_STA_SYS_CPU(n)     (1 << (13 + (n)))

/* PLB6 */

#define PLB6BC_PAAM_REG_ADDR           0x103  /* PAAM Window Extension */
#define PLB6BC_HCPP_REG_ADDR           0x10D  /* Hang Count Pulse Period */
#define PLB6BC_BCHD_REG_ADDR           0x10E  /* Hang Detection Status  */
#define PLB6BC_BCQD_REG_ADDR           0x10F  /* Quiesce Status */
#define PLB6BC_BCSHD_REG_ADDR          0x110  /* System Hang Detection Status */

/* PLB6BC_PAAM_REG_ADDR register bit definitions */

#define PLB6BC_PAAM_MASK               0x3 /* PAAM Window Extension */
#define PLB6BC_PAAM_SHIFT              30  /* valid settings 1 to 3 */
#define PLB6BC_PAAM_REG_VALUE          (1 << PLB6BC_PAAM_SHIFT)

/* default value used to clear status */

#define PLB6BC_BCHD_REG_VALUE          0xFFFFFFFF
#define PLB6BC_BCQD_REG_VALUE          0xFFFFFFFF
#define PLB6BC_BCSHD_REG_VALUE         0xFFFFFFFF

#define PLB6BC_HCPP_REG_VALUE          0x1000 /* Minimum valid value is 0x400 */

/* MPIC */

#define MPIC0_DCR_BA        0xFFC00000 /* used for core */
#define MPIC1_DCR_BA        0xEFC00000 /* used for AXIS */

/* interrupt vectors */

#define INUM_NULL           0 /* Avoid MPIC power-up timing issue */
#define INUM_I2C            21
#define INUM_UART0          22
#define INUM_UART1          23
#define INUM_TIMER3         24 /* General Purpose Timer */
#define INUM_TIMER5         25
#define INUM_TIMER6         26
#define INUM_TIMER7         27
#define INUM_TIMERC         28 /* timerc combined interrupt */
#define INUM_PEI0           29 /* PCIe controller 0 */
#define INUM_SRIO           30
#define INUM_USB            31
#define INUM_FEMAC          32
#define INUM_FEMAC_DMA      33
#define INUM_NAND           34
#define INUM_GPDMA0         35
#define INUM_GPDMA1         36
#define INUM_GPDMA2         37
#define INUM_GPDMA3         38
#define INUM_PEI1           72 /* PCIe controller 1 */
#define INUM_PEI2           73 /* PCIe controller 2 */
#define INUM_PEI_DOORBELL   74 /* PCIe controller 0 End Point Door Bell */

/* PHY address */

#define FEMAC_PHY_ADDR          0x1E

/* no off-board bus interface    */

#define BUS                                 NONE

/* Number of serial I/O channels */

#define N_SIO_CHANNELS                      2

/* system clocks */

#define BSP_CLK_REF0_FREQ                   125000000 /* 125M Hz */
#define BSP_PPC_PLL_OUTA_FREQ               1600000000 /* 1.6G Hz*/
#define BSP_PPC_PLL_OUTB_FREQ               200000000 /* 200M Hz*/

/*
 * Interrupt Vector Prefix Register value. Exception vectors will be
 * located at the start of SDRAM.
 */

#define ACP34XX_IVPR_VAL                  0x00000000

/* EEPROM(128 bytes only) I2C address */

#define AT24C_I2C_ADRS                      0x54
#define AT24C_MEM_SIZE                      128 /* 128 bytes */
#define AT24C_PAGE_SIZE                     8

/*
 * PVR:
 * For PPC476, PVR value should be 0xZZZ52RRZ.
 * Where Z represents digits chosen by SOC owner and all Z digits are
 * application specific.
 * The 52 represents a ppc476 and RR digits represent core revision levels,
 * and all four (52RR) of these digits are assigned by IBM.
 */

#define ACP3448_V1_PVR                  0x11A52040
#define ACP3448_V2_PVR                  0x11A52080
#define ACP3448_V2_1_PVR                0x11A520C0
#define ACP3423_V2_PVR                  0x11B22080 

#define ACP34XX_PVR_SOC_MASK            0xFFF00000

/* core number */

#define ACP344X_MAX_CPUS                4
#define ACP342X_MAX_CPUS                2

/* magic number for cpu loop of MP */

#define MP_MAGIC_RBV                    0xfeedfeed

/*
 * size of sysBootStub () in sysALib.s, must be updated if sysBootStub() is
 * modified.
 */

#define ACP34XX_BOOT_STUB_SIZE          20

/* MP status structer */

#ifndef _ASMLANGUAGE

typedef enum mpCpuState {
    mpCpuStateUnknown = 0,
    mpCpuStateReboot  = 1,
    mpCpuStateSleep   = 2,
    mpCpuStateBooting = 3,
    mpCpuStateReady   = 4,
    mpCpuStateRunning = 5,
    mpCpuStateError   = 6
} MP_CPU_STATE;

#else

#define ACP34XX_MP_CPU_STATUS_UNKNOWN   0
#define ACP34XX_MP_CPU_STATUS_REBOOT    1
#define ACP34XX_MP_CPU_STATUS_SLEEP     2
#define ACP34XX_MP_CPU_STATUS_BOOTING   3
#define ACP34XX_MP_CPU_STATUS_READY     4
#define ACP34XX_MP_CPU_STATUS_RUNNING   5
#define ACP34XX_MP_CPU_STATUS_ERROR     6

#endif  /* _ASMLANGUAGE */

/* support for read-only PIR register */

#define  INCLUDE_SYS_HW_INIT_0

#ifndef _ASMLANGUAGE
IMPORT void sysHwInit0 (void);
#endif

#define SYS_HW_INIT_0   sysHwInit0

#ifdef  __cplusplus
    }
#endif

#endif  /* __INClsi_acp34xxh */
