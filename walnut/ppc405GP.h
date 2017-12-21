/* ppc405GP.h - IBM PowerPC 405GP Chip header */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01m,03sep04,mdo  Documentation fixes for apigen
01l,23oct03,pch  SPR 80911: get PPC4xx cache dimensions from PVR
01k,20aug02,pch  405GP / 405GPr interoperability
01j,18jun02,mcg  add support for 405GPr new registers and bits
01i,12jun02,pch  SPR 74987: C++
01h,09jan02,pch  Change MAL, ENET, and EBC references to MAL0, ENET0, EBC0 for
                 consistency with ebony
01g,19nov01,pch  cleanup
01f,06dec00,s_m  added Rev E PVR
01e,26may00,mcg  moved DMA register information to its own header file
01d,15may00,mcg  register name updates to match 405GP User Manual
01c,18apr00,mcg  fix PCI_IO_REGION_X definitions
01b,15jan00,mcg  move MAL, UIC, SDRAM controller info to their own .h files
01a,09sep99,mcg  created
*/

/*
This file contains DCR (Device Control Register) addresses and other chip
specific information for the IBM PowerPC 405GP outside of the 405 processor
core (see ppc405.h for 405 processor core information).
*/

#ifndef INCppc405GPh
#define INCppc405GPh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * The 405GP chip uses the 405B3 processor core which has a 16KB I-cache and a
 * 8KB D-cache.  All 405 processor cores have 32 byte cache lines, and are
 * 2-way set associative.  The names are misleading:  these are actually the
 * number of congruent classes, not the number of lines in the cache.
 */

#define _ICACHE_LINE_NUM_405GP 256                     /* 256 * 32 * 2 = 16KB */
#define _DCACHE_LINE_NUM_405GP 128                     /* 128 * 32 * 2 =  8KB */

/*
 * The 405GPr chip uses the 405D4 processor core which has a 16KB I-cache and a
 * 16KB D-cache.  All 405 processor cores have 32 byte cache lines, and are
 * 2-way set associative.  The names are misleading:  these are actually the
 * number of congruent classes, not the number of lines in the cache.
 */

#define _ICACHE_LINE_NUM_405GPR 256                    /* 256 * 32 * 2 = 16KB */
#define _DCACHE_LINE_NUM_405GPR 256                    /* 256 * 32 * 2 = 16KB */


/*
 * Processor Version Register (PVR) values for 405GP and 405GPr
 */

/*#define PVR_405GP_RA  0x40110000    405GP Revision A (not supported) */
#define PVR_405GP_RB    0x40110040        /* 405GP Revision B                 */
#define PVR_405GP_RC    0x40110082        /* 405GP Revision C                 */
#define PVR_405GP_RD    0x401100C4        /* 405GP Revision D                 */
#define PVR_405GP_RE    0x40110145        /* 405GP Revision E                 */
#define PVR_405GPR_RB   0x50910951        /* 405GPr Revision B (1.1)          */

#define PVR_405GP_HI    0x4011		/* generic 405GP recognition */

/*
 * Reduce the number of MAL channel DCR access functions because a MAL with
 * only 8 channels is used in the 405GP.
 */

#define MAL_CHANNELS_8

/*
 * Valid Memory Access Layer (MAL) channels.  On the 405GP, EMAC is the only
 * communications core that uses MAL.  It has 2 TX channels, and 1 TX channel.
 */

#define MAL_CHN_TX_CPUCORE	0xC0000000	/* TX channels 0 and 1       */
#define MAL_CHN_RX_CPUCORE	0x80000000	/* RX channel  0             */

/*
 * MAL channels numbers the EMAC Ethernet controller is connected to on 405GP.
 * This information is put into the EMAC driver END load string.
 */

#define EMAC0_TX_CHN_0_CPUCORE	0
#define EMAC0_TX_CHN_1_CPUCORE	1
#define EMAC0_RX_CHN_0_CPUCORE	0

/*
 * Base DCR address values for all peripheral cores in the 405GP
 */

#define SDRAM_DCR_BASE        0x010       /* SDRAM Controller                 */
#define EBC0_DCR_BASE         0x012       /* External Bus Controller          */
#define UIC_DCR_BASE          0x0C0       /* Universal Interrupt Controller   */
#define DMA_DCR_BASE          0x100       /* DMA Controller                   */
#define CLKPWRCH_DCR_BASE     0x0B0       /* Clock/Power/Chip Control         */
#define MAL0_DCR_BASE         0x180       /* Memory Access Layer Core         */
#define DECOMP_DCR_BASE       0x014       /* Code Decompression Core          */
#define OCM_DCR_BASE          0x018       /* On-chip Memory Controller        */

/*
 * Static interrupt vectors/levels.  These also correspond to bit numbers in
 * many of the registers of the Universal Interrupt Controller.
 */
#define INT_VEC_UART0             0
#define INT_VEC_UART1             1
#define INT_VEC_IIC               2
#define INT_VEC_EXT_MASTER        3
#define INT_VEC_PCI               4
#define INT_VEC_DMA_CH0           5
#define INT_VEC_DMA_CH1           6
#define INT_VEC_DMA_CH2           7
#define INT_VEC_DMA_CH3           8
#define INT_VEC_ENET_WAKEUP       9
#define INT_VEC_MAL_SERR          10
#define INT_VEC_MAL_TXEOB         11
#define INT_VEC_MAL_RXEOB         12
#define INT_VEC_MAL_TXDE          13
#define INT_VEC_MAL_RXDE          14
#define INT_VEC_ENET_0            15
#define INT_VEC_PCI_SERR          16
#define INT_VEC_ECC_ERROR         17
#define INT_VEC_PCI_POWER         18
#define INT_VEC_EXT_IRQ_7         19                /* 405GPr only */
#define INT_VEC_EXT_IRQ_8         20                /* 405GPr only */
#define INT_VEC_EXT_IRQ_9         21                /* 405GPr only */
#define INT_VEC_EXT_IRQ_10        22                /* 405GPr only */
#define INT_VEC_EXT_IRQ_11        23                /* 405GPr only */
#define INT_VEC_EXT_IRQ_12        24                /* 405GPr only */
#define INT_VEC_EXT_IRQ_0         25
#define INT_VEC_EXT_IRQ_1         26
#define INT_VEC_EXT_IRQ_2         27
#define INT_VEC_EXT_IRQ_3         28
#define INT_VEC_EXT_IRQ_4         29
#define INT_VEC_EXT_IRQ_5         30
#define INT_VEC_EXT_IRQ_6         31

#define INT_LVL_UART0             0
#define INT_LVL_UART1             1
#define INT_LVL_IIC               2
#define INT_LVL_EXT_MASTER        3
#define INT_LVL_PCI               4
#define INT_LVL_DMA_CH0           5
#define INT_LVL_DMA_CH1           6
#define INT_LVL_DMA_CH2           7
#define INT_LVL_DMA_CH3           8
#define INT_LVL_ENET_WAKEUP       9
#define INT_LVL_MAL_SERR          10
#define INT_LVL_MAL_TXEOB         11
#define INT_LVL_MAL_RXEOB         12
#define INT_LVL_MAL_TXDE          13
#define INT_LVL_MAL_RXDE          14
#define INT_LVL_ENET_0            15
#define INT_LVL_PCI_SERR          16
#define INT_LVL_ECC_ERROR         17
#define INT_LVL_PCI_POWER         18
#define INT_LVL_EXT_IRQ_7         19                /* 405GPr only */
#define INT_LVL_EXT_IRQ_8         20                /* 405GPr only */
#define INT_LVL_EXT_IRQ_9         21                /* 405GPr only */
#define INT_LVL_EXT_IRQ_10        22                /* 405GPr only */
#define INT_LVL_EXT_IRQ_11        23                /* 405GPr only */
#define INT_LVL_EXT_IRQ_12        24                /* 405GPr only */
#define INT_LVL_EXT_IRQ_0         25
#define INT_LVL_EXT_IRQ_1         26
#define INT_LVL_EXT_IRQ_2         27
#define INT_LVL_EXT_IRQ_3         28
#define INT_LVL_EXT_IRQ_4         29
#define INT_LVL_EXT_IRQ_5         30
#define INT_LVL_EXT_IRQ_6         31

/*
 * Clock, power management, chip control and strapping register definitions.
 * Each is a separate DCR register.
 */
#define CPC0_PLLMR  (CLKPWRCH_DCR_BASE+0x0)  /* PLL Mode Register             */
#define CPC0_CR0    (CLKPWRCH_DCR_BASE+0x1)  /* Chip control 0 Register       */
#define CPC0_CR1    (CLKPWRCH_DCR_BASE+0x2)  /* Chip control 1 Register       */
#define CPC0_PSR    (CLKPWRCH_DCR_BASE+0x4)  /* Pin Strapping Register        */
#define CPC0_SR     (CLKPWRCH_DCR_BASE+0x8)  /* clock/power management status */
#define CPC0_ER     (CLKPWRCH_DCR_BASE+0x9)  /* clock/power management enable */
#define CPC0_FR     (CLKPWRCH_DCR_BASE+0xa)  /* clock/power management force  */
#define CPC0_ECID0       0xA8                /* 405GPr only chip id upper     */
#define CPC0_ECID1       0xA9                /* 405GPr only chip id lower     */
#define CPC0_ECR         0xAA                /* 405GPr only edge conditioner  */
#define CPC0_EIRR   (CLKPWRCH_DCR_BASE+0x6)  /* 405GPr ext interrupt routing  */

/* Bit definitions */
#define PLLMR_FWD_DIV_MASK      0xE0000000     /* Forward Divisor */
#define PLLMR_FWD_DIV_BIT_SHIFT     29
#define PLLMR_FWD_DIV_BYPASS    0xE0000000
#define PLLMR_FWD_DIV_3         0xA0000000
#define PLLMR_FWD_DIV_4         0x80000000
#define PLLMR_FWD_DIV_6         0x40000000

#define PLLMR_FB_DIV_MASK       0x1E000000     /* Feedback Divisor */
#define PLLMR_FB_DIV_BIT_SHIFT      25
#define PLLMR_FB_DIV_1          0x02000000
#define PLLMR_FB_DIV_2          0x04000000
#define PLLMR_FB_DIV_3          0x06000000
#define PLLMR_FB_DIV_4          0x08000000

#define PLLMR_TUNING_MASK       0x01F80000

#define PLLMR_CPU_TO_PLB_MASK   0x00060000     /* CPU:PLB Frequency Divisor */
#define PLLMR_CPU_TO_PLB_BIT_SHIFT  17
#define PLLMR_CPU_PLB_DIV_1     0x00000000
#define PLLMR_CPU_PLB_DIV_2     0x00020000
#define PLLMR_CPU_PLB_DIV_3     0x00040000
#define PLLMR_CPU_PLB_DIV_4     0x00060000

#define PLLMR_OPB_TO_PLB_MASK   0x00018000     /* OPB:PLB Frequency Divisor */
#define PLLMR_OPB_TO_PLB_BIT_SHIFT  15
#define PLLMR_OPB_PLB_DIV_1     0x00000000
#define PLLMR_OPB_PLB_DIV_2     0x00008000
#define PLLMR_OPB_PLB_DIV_3     0x00010000
#define PLLMR_OPB_PLB_DIV_4     0x00018000

#define PLLMR_PCI_TO_PLB_MASK   0x00006000     /* PCI:PLB Frequency Divisor */
#define PLLMR_PCI_TO_PLB_BIT_SHIFT  13
#define PLLMR_PCI_PLB_DIV_1     0x00000000
#define PLLMR_PCI_PLB_DIV_2     0x00002000
#define PLLMR_PCI_PLB_DIV_3     0x00004000
#define PLLMR_PCI_PLB_DIV_4     0x00006000

#define PLLMR_EXB_TO_PLB_MASK   0x00001800     /* External Bus:PLB Divisor  */
#define PLLMR_EXB_TO_PLB_BIT_SHIFT  11
#define PLLMR_EXB_PLB_DIV_2     0x00000000
#define PLLMR_EXB_PLB_DIV_3     0x00000800
#define PLLMR_EXB_PLB_DIV_4     0x00001000
#define PLLMR_EXB_PLB_DIV_5     0x00001800

#define PLLMR_TUNING_MASK_UPPER 0x00000780     /* 405GPr only               */

#define PLLMR_BYPASS_MODE       0x00000040     /* 405GPr only               */

#define PLLMR_FWD_DIVB_MASK     0x00000007     /* Forward Divisor           */
#define PLLMR_FWD_DIVB_BIT_SHIFT     0
#define PLLMR_FWD_DIVB_8        0x00000000     /* 405GPr only               */
#define PLLMR_FWD_DIVB_7        0x00000001
#define PLLMR_FWD_DIVB_6        0x00000002
#define PLLMR_FWD_DIVB_5        0x00000003
#define PLLMR_FWD_DIVB_4        0x00000004
#define PLLMR_FWD_DIVB_3        0x00000005
#define PLLMR_FWD_DIVB_2        0x00000006
#define PLLMR_FWD_DIVB_1        0x00000007

/*
 * PLL Voltage Controlled Oscillator (VCO) definitions
 * Maximum and minimum values (in MHz) for correct PLL operation.
 *    405GP    400 <= VCO <= 800
 *    405GPr   400 <= VCO <= 1333
 */
#define VCO_MIN_GP	400
#define VCO_MAX_GP	800
#define VCO_MIN_GPR	400
#define VCO_MAX_GPR	1333

/* Chip Control Register 0 bits */
#define CR0_PLL_MODE_REG_EN     0x80000000
#define CR0_GPIO_10_EN          0x04000000
#define CR0_GPIO_11_EN          0x02000000
#define CR0_GPIO_12_EN          0x01000000
#define CR0_GPIO_13_EN          0x00800000
#define CR0_GPIO_14_EN          0x00400000
#define CR0_GPIO_15_EN          0x00200000
#define CR0_GPIO_16_EN          0x00100000
#define CR0_GPIO_17_EN          0x00080000
#define CR0_GPIO_18_EN          0x00040000
#define CR0_GPIO_19_EN          0x00020000
#define CR0_GPIO_20_EN          0x00010000
#define CR0_GPIO_21_EN          0x00008000
#define CR0_GPIO_22_EN          0x00004000
#define CR0_GPIO_23_EN          0x00002000
#define CR0_UART1_CTS_EN        0x00001000
#define CR0_UART1_DTR_EN        0x00000800
#define CR0_UART0_EXT_CLK       0x00000080
#define CR0_UART1_EXT_CLK       0x00000040
#define CR0_UART_DIV_MASK       0x0000003E


#define CR1_DET_MULT_EN         0x80000000
#define CR1_EXT_TIMER_EN        0x00800000


#define PSR_PLL_FWD_MASK        0xC0000000
#define PSR_PLL_FDBACK_MASK     0x30000000
#define PSR_PLL_TUNING_MASK     0x0E000000
#define PSR_PLB_CPU_MASK        0x01800000
#define PSR_OPB_PLB_MASK        0x00600000
#define PSR_PCI_PLB_MASK        0x00180000
#define PSR_EB_PLB_MASK         0x00060000
#define PSR_ROM_WIDTH_MASK      0x00018000
#define PSR_ROM_LOC             0x00004000
#define PSR_PCI_ASYNC_EN        0x00001000
#define PSR_PERCLK_SYNC         0x00000800     /* 405GPr only     */
#define PSR_PCI_ARBIT_EN        0x00000400
#define PSR_PLL_FWDA_BIT2       0x00000200     /* 405GPr only     */
#define PSR_PLL_FWDB_BIT2       0x00000100     /* 405GPr only     */
#define PSR_PLL_FDBACK_BIT2     0x00000080     /* 405GPr only     */
#define PSR_PLL_FDBACK_BIT3     0x00000040     /* 405GPr only     */
#define PSR_NEW_MODE            0x00000020     /* 405GPr only     */
#define PSR_FLIP_CIRC_DISABLE   0x00000010     /* 405GPr only     */

#define ECR_EMAC_RX_MASK        0xE0000000     /* 405GPr only     */
#define ECR_EMAC_RX_BYP         0x00000000     /* 405GPr only     */
#define ECR_EMAC_RX_1           0x20000000     /* 405GPr only     */
#define ECR_EMAC_RX_2           0x40000000     /* 405GPr only     */
#define ECR_EMAC_RX_3           0x60000000     /* 405GPr only     */
#define ECR_EMAC_RX_4           0x80000000     /* 405GPr only     */
#define ECR_EMAC_RX_6           0xA0000000     /* 405GPr only     */
#define ECR_EMAC_RX_8           0xC0000000     /* 405GPr only     */
#define ECR_EMAC_RX_10          0xE0000000     /* 405GPr only     */
#define ECR_EMAC_TX_MASK        0x00E00000     /* 405GPr only     */
#define ECR_EMAC_TX_BYP         0x00000000     /* 405GPr only     */
#define ECR_EMAC_TX_1           0x00200000     /* 405GPr only     */
#define ECR_EMAC_TX_2           0x00400000     /* 405GPr only     */
#define ECR_EMAC_TX_3           0x00600000     /* 405GPr only     */
#define ECR_EMAC_TX_4           0x00800000     /* 405GPr only     */
#define ECR_EMAC_TX_6           0x00A00000     /* 405GPr only     */
#define ECR_EMAC_TX_8           0x00C00000     /* 405GPr only     */
#define ECR_EMAC_TX_10          0x00E00000     /* 405GPr only     */
#define ECR_UIC_MASK            0x0000E000     /* 405GPr only     */
#define ECR_UIC_BYP             0x00000000     /* 405GPr only     */
#define ECR_UIC_1               0x00002000     /* 405GPr only     */
#define ECR_UIC_2               0x00004000     /* 405GPr only     */
#define ECR_UIC_3               0x00006000     /* 405GPr only     */
#define ECR_UIC_4               0x00008000     /* 405GPr only     */
#define ECR_UIC_6               0x0000A000     /* 405GPr only     */
#define ECR_UIC_8               0x0000C000     /* 405GPr only     */
#define ECR_UIC_10              0x0000E000     /* 405GPr only     */


/*
 * On-chip memory controller configuration register definitions.
 */

#define OCM_ISARC  (OCM_DCR_BASE+0x00)    /* Instruction-side Addr range cmp  */
#define OCM_ISCNTL (OCM_DCR_BASE+0x01)    /* Instruction-side control reg     */
#define OCM_DSARC  (OCM_DCR_BASE+0x02)    /* Data-side Addr range cmp         */
#define OCM_DSCNTL (OCM_DCR_BASE+0x03)    /* Data-side control reg            */

/* OCM register bits */

#define OCM_DSAR    0xFC000000            /* OCM D-side address mask          */
#define OCM_DSEN    0x80000000            /* OCM D-side enable                */

#define OCM_ISAR    0xFC000000            /* OCM I-side address mask          */
#define OCM_ISEN    0x80000000            /* OCM I-side enable                */
#define OCM_ISTCM   0x40000000            /* OCM I-side 2 cycle mode          */


/*
 * CodePack Code Decompression core register definitions. Two DCRs are used
 * (address/data) to indirectly access the config registers of this controller.
 */
#define DCP_CFGADDR (DECOMP_DCR_BASE+0x0)  /* Decompression cntrl addr reg    */
#define DCP_CFGDATA (DECOMP_DCR_BASE+0x1)  /* Decompression cntrl data reg    */

  /* values for DCP_CFGADDR register - indirect addressing of these regs */
  #define DCP_ITOR0   0x00    /* index table origin register 0        */
  #define DCP_ITOR1   0x01    /* index table origin register 1        */
  #define DCP_ITOR2   0x02    /* index table origin register 2        */
  #define DCP_ITOR3   0x03    /* index table origin register 3        */
  #define DCP_ADDR0   0x04    /* address decode definition register 0 */
  #define DCP_ADDR1   0x05    /* address decode definition register 1 */
  #define DCP_CFG     0x40    /* decompression core config register   */
  #define DCP_ID      0x41    /* decompression core ID     register   */
  #define DCP_VER     0x42    /* decompression core version # reg     */
  #define DCP_PLBBEAR 0x50    /* bus error addr reg (PLB addr)        */
  #define DCP_MEMBEAR 0x51    /* bus error addr reg (DCP to EBIU addr)*/
  #define DCP_ESR     0x52    /* bus error status reg 0  (R/clear)    */
  /* There are 0x400 of the following registers, from 0x400 to 0x7ff  */
  /* Only the first and last ones are defined here.                   */
  #define DCP_RAM0   0x400    /* SRAM/ROM read/write (first location) */
  #define DCP_RAM3FF 0x7FF    /* SRAM/ROM read/write (last location)  */

/*
 * PLB Bus Error, Arbiter and PLB to OPB Bridge DCR registers.
 */
#define PLB_BESR      0x84    /* PLB Error Status Reg                 */
#define PLB_BEAR      0x86    /* PLB Error Address Reg                */
#define PLB_ACR       0x87    /* PLB Arbiter Control Reg              */
#define POB_BESR0     0xa0    /* PLB to OPB Bridge Error Status Reg 0 */
#define POB_BEAR      0xa2    /* PLB to OPB Bridge Error Address Reg  */
#define POB_BESR1     0xa4    /* PLB to OPB Bridge Error Status Reg 1 */


/*
 * OPB Bus Arbiter registers (memory mapped)
 */
#define OPBA_PR       0xEF600600     /* OPB Arbiter Priority Register */
#define OPBA_CR       0xEF600601     /* OPB Arbiter Control Register  */


/*
 * 405GP IIC Base Address definition and register offsets.
 */
#define IIC0_BASE       0xEF600500
#define    IIC_MDBUF         0x00
#define    IIC_SDBUF         0x02
#define    IIC_LMADR         0x04
#define    IIC_HMADR         0x05
#define    IIC_CNTL          0x06
#define    IIC_MDCNTL        0x07
#define    IIC_STS           0x08
#define    IIC_EXTSTS        0x09
#define    IIC_LSADR         0x0A
#define    IIC_HSADR         0x0B
#define    IIC_CLKDIV        0x0C
#define    IIC_INTRMSK       0x0D
#define    IIC_XFRCNT        0x0E
#define    IIC_XTCNTLSS      0x0F
#define    IIC_DIRECTCNTL    0x10

/*
 * 405GP UART (2 of them) Base Address definitions.  Both UARTs are 16550-like.
 */
#define UART0_BASE      0xEF600300
#define UART1_BASE      0xEF600400
#define UART_REG_ADDR_INTERVAL   1

#define UART_MEMORY_START   0xEF600000
#define UART_MEMORY_END     0xEF600FFF


/*
 * 405GP PCI address map
 */

/*
 * PCI memory space from the PLB.
 * PLB to PCI address translation is controlled by the 3 pairs of PTM registers.
 */

#define PCI_MEMORY_START            0x80000000
#define PCI_MEMORY_END              0xE7FFFFFF

/*
 * For the MMU to conserve page table space, we map only 64 MB of the PCI
 * memory space (needed by VGA card).
 */
#define PCI_MEMORY_MAP_END      0x83FFFFFF  /* 64 MB mapped in MMU */


/*
 * PCI I/O space from the PLB and the PCI translation.
 */

#define PLB_PCI_IO_REGION_1_START   0xE8000000     /* PLB side */
#define PLB_PCI_IO_REGION_1_END     0xE800FFFF
#define PLB_PCI_IO_REGION_1_SIZE    0x00010000

#define PLB_PCI_IO_REGION_2_START   0xE8800000
#define PLB_PCI_IO_REGION_2_END     0xEBFFFFFF
#define PLB_PCI_IO_REGION_2_SIZE    0x03800000
#define PLB_PCI_IO_REGION_2_MAP_END 0xE88FFFFF     /* 1 MB mapped in MMU */
                           						   /* increase if needed */


#define PCI_IO_REGION_1_START       0x00000000     /* PCI side */
#define PCI_IO_REGION_1_END         0x0000FFFF
#define PCI_IO_REGION_2_START       0x00800000
#define PCI_IO_REGION_2_END         0x03FFFFFF

#define PCI_INTERRUPT_ACK           0xEED00000     /* read */
#define PCI_SPECIAL_CYCLE           0xEED00000     /* write */
#define PCI_INTERRUPT_ACK_END       0xEED00FFF     /* 1 page in MMU */


/*
 * Register pair used to generate configuration cycles on the PCI bus
 * and access the 405GP's own PCI configuration registers.
 */
#define PCI_CFGADDR        0xEEC00000
#define PCI_CFGDATA        0xEEC00004
#define PCI_CFGEND         0xEEC00FFF  /* 1 page in MMU */

/*
 * PP Bridge Local Config registers space
 */
#define PP_BRIDGE_CR        0xEF400000
#define PP_BRIDGE_CR_END    0xEF400FFF  /* 1 page in MMU */


/*
 * Register addresses for the 3 sets of 405GP PCI Master Mode (PMM)
 * local configuration registers.  These registers control how local memory
 * addresses are translated to PCI memory addresses when the 405GP is a
 * PCI bus master (initiator).  The contents of the PMMxLA register must be
 * set to an address that is within the 405GP PCI memory range
 * (PCI_MEMORY_START - PCI_MEMORY_END).
 */
#define PCIL_PMM0LA        0xEF400000       /* Local Address                  */
#define PCIL_PMM0MA        0xEF400004       /* Mask/Attribute register        */
#define PCIL_PMM0PCILA     0xEF400008       /* Local address is translated to */
#define PCIL_PMM0PCIHA     0xEF40000C       /* this 64 bit PCI address        */

#define PCIL_PMM1LA        0xEF400010       /* Local Address                  */
#define PCIL_PMM1MA        0xEF400014       /* Mask/Attribute register        */
#define PCIL_PMM1PCILA     0xEF400018       /* Local address is translated to */
#define PCIL_PMM1PCIHA     0xEF40001C       /* this 64 bit PCI address        */

#define PCIL_PMM2LA        0xEF400020       /* Local Address                  */
#define PCIL_PMM2MA        0xEF400024       /* Mask/Attribute register        */
#define PCIL_PMM2PCILA     0xEF400028       /* Local address is translated to */
#define PCIL_PMM2PCIHA     0xEF40002C       /* this 64 bit PCI address        */

/*
 * Bit definitions for PMM Mask Attribute registers
 */
#define PMM_MASK           0xFFFFF000
#define PMM_MASK_4KB       0xFFFFF000
#define PMM_MASK_8KB       0xFFFFE000
#define PMM_MASK_16KB      0xFFFFC000
#define PMM_MASK_32KB      0xFFFF8000
#define PMM_MASK_64KB      0xFFFF0000
#define PMM_MASK_128KB     0xFFFE0000
#define PMM_MASK_256KB     0xFFFC0000
#define PMM_MASK_512KB     0xFFF80000
#define PMM_MASK_1MB       0xFFF00000
#define PMM_MASK_2MB       0xFFE00000
#define PMM_MASK_4MB       0xFFC00000
#define PMM_MASK_8MB       0xFF800000
#define PMM_MASK_16MB      0xFF000000
#define PMM_MASK_32MB      0xFE000000
#define PMM_MASK_64MB      0xFC000000
#define PMM_MASK_128MB     0xF8000000
#define PMM_MASK_256MB     0xF0000000
#define PMM_MASK_512MB     0xE0000000
#define PMM_PREFETCH       0x00000002
#define PMM_ENABLE         0x00000001

#define PMM_UNUSED         0x00000000

/*
 * Register addresses for the 2 sets of 405GP PCI Target Mode (PTM)
 * local configuration registers.  These registers control how PCI memory
 * addresses are translated to Local memory addresses when the 405GP is a
 * PCI bus target.  PTMxLA must be set to an address that is within the
 * 405GP Local SDRAM Memory or ROM regions (0x00000000 - 7FFFFFFF, or
 * F0000000 - FFFFFFFF).
 */
#define PCIL_PTM1MS        0xEF400030       /* Memory Size/Attribute register */
#define PCIL_PTM1LA        0xEF400034       /* Local Address                  */

#define PCIL_PTM2MS        0xEF400038       /* Memory Size/Attribute register */
#define PCIL_PTM2LA        0xEF40003C       /* Local Address                  */

/*
 * Bit definitions for PTM Memory Size/Attribute registers
 */
#define PTM_SIZE           0xFFFFF000
#define PTM_SIZE_4KB       0xFFFFF000
#define PTM_SIZE_8KB       0xFFFFE000
#define PTM_SIZE_16KB      0xFFFFC000
#define PTM_SIZE_32KB      0xFFFF8000
#define PTM_SIZE_64KB      0xFFFF0000
#define PTM_SIZE_128KB     0xFFFE0000
#define PTM_SIZE_256KB     0xFFFC0000
#define PTM_SIZE_512KB     0xFFF80000
#define PTM_SIZE_1MB       0xFFF00000
#define PTM_SIZE_2MB       0xFFE00000
#define PTM_SIZE_4MB       0xFFC00000
#define PTM_SIZE_8MB       0xFF800000
#define PTM_SIZE_16MB      0xFF000000
#define PTM_SIZE_32MB      0xFE000000
#define PTM_SIZE_64MB      0xFC000000
#define PTM_SIZE_128MB     0xF8000000
#define PTM_SIZE_256MB     0xF0000000
#define PTM_SIZE_512MB     0xE0000000
#define PTM_SIZE_1GB       0xC0000000
#define PTM_SIZE_2GB       0x80000000
#define PTM_ENABLE         0x00000001

#define PTM_UNUSED         0x00000000

/*
 * Bus/Device/Function used to access the PCI configuration registers
 * that belong to the host bridge itself (Bus = Device = Function = 0).
 */
#define PCI_HOST_BUS        0
#define PCI_HOST_DEVICE     0
#define PCI_HOST_FUNCTION   0

/*
 * 405GP-specific PCI configuration register offsets.  These are found
 * immediately following the architecture's 64 byte PCI configuration header.
 */
#define PCI_CFG_ICS          0x44       /* PCI Interrupt Control/Status       */
#define PCI_CFG_ERREN        0x48       /* Error Enable                       */
#define PCI_CFG_ERRSTS       0x49       /* Error Status                       */
#define PCI_CFG_BRDGOPT1     0x4A       /* Bridge Options 1                   */
#define PCI_CFG_PLBBESR0     0x4C       /* PLB Slave Error Syndrome 0         */
#define PCI_CFG_PLBBESR1     0x50       /* PLB Slave Error Syndrome 1         */
#define PCI_CFG_PLBBEAR      0x54       /* PLB Slave Error Address            */
#define PCI_CFG_CAPID        0x58       /* Capability Identifier              */
#define PCI_CFG_NEXTIPTR     0x59       /* Next Item Pointer                  */
#define PCI_CFG_PMC          0x5A       /* Power Management Capabilities      */
#define PCI_CFG_PMCSR        0x5C       /* Power Management Control Status    */
#define PCI_CFG_PMCSRBSE     0x5E       /* PMCSR PCI to PCI bridge exten      */
#define PCI_CFG_DATA         0x5F       /* Data                               */
#define PCI_CFG_BRDGOPT2     0x60       /* Bridge Options 2                   */
#define PCI_CFG_PMSCRR       0x64       /* Power Management State Change Req. */


/*
 * General Purpose I/O (GPIO)
 */

#define GPIO_BASE          0xEF600700

#define GPIO_OR      (GPIO_BASE+0x00)   /* GPIO Output Register               */
#define GPIO_TCR     (GPIO_BASE+0x04)   /* GPIO Three-state Control Reg       */
#define GPIO_ODR     (GPIO_BASE+0x18)   /* GPIO Open Drain Reg                */
#define GPIO_IR      (GPIO_BASE+0x1c)   /* GPIO Input Register                */

/*
 * Base address of Ethernet (EMAC) registers in the 405GP
 */

#define EMAC0_BASE	0xEF600800

/*
 * Flash / Boot ROM area
 */
#define FLASH_START 0xFFF80000
#define FLASH_END   0xFFFFFFFF


/*
 * Header files for other cores in the 405GP
 */

#include "405gpDcr.h"
#include "uicDcr.h"            /* Universal Interrupt Controller */
#include "malDcr.h"            /* Memory Access Layer DCR        */
#include "sdramDcr.h"          /* SDRAM Controller               */
#include "ebcDcr.h"            /* External Bus Controller        */
#include "dmaDcr.h"            /* DMA Controller                 */

#ifdef __cplusplus
    }
#endif

#endif  /* INCppc405GPh */
