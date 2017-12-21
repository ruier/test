/* ppc440GX.h - IBM PowerPC 440GX Chip header */

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

/*
 * Copyright (c) 2002-2004, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,16dec08,b_m  modify uic interrupt vector base.
01m,02sep08,x_f  Remove sysDcr.h include
01l,04dec07,x_s  Removed PCIX MACRO definitions. 
01k,04apr07,x_s  Modify EMAC defines to support amcc_taishan board
01j,29sep04,mdo  Documentation fixes for apigen
01i,17dec03,jtp  SPR 92575 I-MMU support: add device addresses & sizes
01h,02sep03,jtp  Make cache config take USER_D_CACHE_ENABLE into account.
01g,05aug03,jtp  Formal code inspection changes: macros for cache, device
		 PLB addresses, timers.
01f,12jun03,pch  log recovered 440x5 Machine Check events
01e,22jul03,jtp  Allow USER_D_CACHE_MODE of CACHE_DISABLED
01d,09jul03,jtp  Support PCI
01c,04apr03,pch  Add PPC440_x5 architecture support & PLB/OPB BESR definitions
01b,28may03,jtp  Merge IBM changes; change EBM to EBM0, PPM to PPM0, add
		 EBM0 bit definitions
01a,02sep02,saw  created from ppc440GP.h vn 01h
*/

/*
This file contains DCR (Device Control Register) addresses and other
specific information for the PPC440GX chip which are outside of the 440
processor core (see ppc440.h for 440 processor core information).
*/

#ifndef INCppc440GXh
#define INCppc440GXh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * The chip has a 32KB I-cache and a 32KB D-cache. All 440
 * processor cores have 32 byte cache lines, and are 64-way set
 * associative.
 */

#define _ICACHE_LINE_NUM_440GX	16                   /* 16 * 32 * 64 = 32KB */
#define _DCACHE_LINE_NUM_440GX	16                   /* 16 * 32 * 64 = 32KB */
#define _DCACHE_NUM_WAYS	64
#define _ICACHE_NUM_WAYS	64
#define _DCACHE_SIZE		(_DCACHE_LINE_NUM_440GX * _CACHE_ALIGN_SIZE \
				 * _DCACHE_NUM_WAYS)
#define _ICACHE_SIZE		(_ICACHE_LINE_NUM_440GX * _CACHE_ALIGN_SIZE \
				 * _ICACHE_NUM_WAYS)


/*
 * Processor Version Register (PVR) values from product data sheet.
 */

#define PVR_440GX_RA   0x51B21850    /* rev A (1.0) IBM25PPC440GX-3CA500Cx */
				     /* rev A (1.0) IBM25PPC440GX-3CA667Cx */

#define PVR_440GX_RB   0x51B21851    /* rev B (2.0) IBM25PPC440GX-3CB500Cx */
				     /* rev B (2.0) IBM25PPC440GX-3CB667Cx */
				     /* rev B (2.0) IBM25PPC440GX-3CB800Cx */
#define PVR_440GX_RF   0x51B21894

/*
 * Valid Memory Access Layer (MAL) channels. On the chip, the EMACs   
 * are the only communications cores that use MAL. They each have 1 TX
 * channel, and 1 RX channel.
 *
 * and
 *
 * MAL channels numbers the EMAC Ethernet controllers are connected to on
 * 440GX. This information is put into the EMAC driver END load string.
 * The 440GX EMACs have only one TX channel
 *
 * This information is customized based on the defined Ethernet group.
 */

/* EMACs 0, 1, 2 and 3 enabled */
#if   (defined(EMAC0_ENABLE) && defined(EMAC1_ENABLE) && defined(EMAC2_ENABLE) && defined(EMAC3_ENABLE) )
   #define MAL_CHN_TX_CPUCORE  0xF0000000    /* emac0,1,2,3 TX chs            */
   #define MAL_CHN_RX_CPUCORE  0xF0000000    /* emac0,1,2,3 RX chs            */
   #define EMAC0_TX_CHN_0_CPUCORE  0
   #define EMAC0_RX_CHN_0_CPUCORE  0
   #define EMAC1_TX_CHN_0_CPUCORE  1
   #define EMAC1_RX_CHN_0_CPUCORE  1
   #define EMAC2_TX_CHN_0_CPUCORE  2
   #define EMAC2_RX_CHN_0_CPUCORE  2
   #define EMAC3_TX_CHN_0_CPUCORE  3
   #define EMAC3_RX_CHN_0_CPUCORE  3
/* EMACs 0 and 2 enabled */
#elif (defined(EMAC0_ENABLE) && !defined(EMAC1_ENABLE) && defined(EMAC2_ENABLE) )
   #define MAL_CHN_TX_CPUCORE  0xA0000000    /* emac0,2 TX chs                */
   #define MAL_CHN_RX_CPUCORE  0xA0000000    /* emac0,2 RX chs                */
   #define EMAC0_TX_CHN_0_CPUCORE  0
   #define EMAC0_RX_CHN_0_CPUCORE  0
   #define EMAC2_TX_CHN_0_CPUCORE  2
   #define EMAC2_RX_CHN_0_CPUCORE  2
/* EMACs 0, 1, and 2 enabled */
#elif (defined(EMAC0_ENABLE) && defined(EMAC1_ENABLE) && defined(EMAC2_ENABLE) )
   #define MAL_CHN_TX_CPUCORE  0xE0000000    /* emac0,1,2 TX chs              */
   #define MAL_CHN_RX_CPUCORE  0xE0000000    /* emac0,1,2 RX chs              */
   #define EMAC0_TX_CHN_0_CPUCORE  0
   #define EMAC0_RX_CHN_0_CPUCORE  0
   #define EMAC1_TX_CHN_0_CPUCORE  1
   #define EMAC1_RX_CHN_0_CPUCORE  1
   #define EMAC2_TX_CHN_0_CPUCORE  2
   #define EMAC2_RX_CHN_0_CPUCORE  2
#elif (defined(EMAC2_ENABLE) && defined(EMAC3_ENABLE) )
   #define MAL_CHN_TX_CPUCORE  0xF0000000    /* emac0,1,2,3 TX chs            */
   #define MAL_CHN_RX_CPUCORE  0xF0000000    /* emac0,1,2,3 RX chs            */
   #define EMAC0_TX_CHN_0_CPUCORE  0
   #define EMAC0_RX_CHN_0_CPUCORE  0
   #define EMAC1_TX_CHN_0_CPUCORE  1
   #define EMAC1_RX_CHN_0_CPUCORE  1
   #define EMAC2_TX_CHN_0_CPUCORE  2
   #define EMAC2_RX_CHN_0_CPUCORE  2
   #define EMAC3_TX_CHN_0_CPUCORE  3
   #define EMAC3_RX_CHN_0_CPUCORE  3
/* EMACs 0 and 1 enabled */
#else
   #define MAL_CHN_TX_CPUCORE  0xC0000000    /* emac0,1 TX ch                 */
   #define MAL_CHN_RX_CPUCORE  0xC0000000    /* emac0,1 RX ch                 */
   #define EMAC0_TX_CHN_0_CPUCORE  0
   #define EMAC0_RX_CHN_0_CPUCORE  0
   #define EMAC1_TX_CHN_0_CPUCORE  1
   #define EMAC1_RX_CHN_0_CPUCORE  1
#endif

/*
 * The MAL core used in 440 chips has different register bit assignments
 * in the MAL_CFG register.  Define MAL_440 so the MAL driver will use the
 * correct assignments.  In 440, all MAL TX channels are configured to have
 * a burst length of 64 words into the TX FIFO, and all MAL RX channels are
 * configured to have a burst length of 64 words from the RX FIFO.
 */
#define MAL_440
#define MAL_TX_BURST_64
#define MAL_RX_BURST_64

/*
 * Base DCR address values for all peripheral cores in the chip 
 */

#define CPR0_DCR_BASE       0x00C       /* Chip Clocking and POR             */
#define SDR0_DCR_BASE       0x00E       /* System DCR Regs                   */
#define SDRAM0_DCR_BASE     0x010       /* SDRAM Controller                  */
#define EBC0_DCR_BASE       0x012       /* External Bus Controller           */
#define EBM0_DCR_BASE       0x014       /* External Bus Master Interface     */
#define PPM0_DCR_BASE       0x016       /* PLB Performance Counter           */
#define SRAM0_DCR_BASE      0x020       /* Internal SRAM Controller          */
#define L2C0_DCR_BASE       0x030       /* L2 Cache Controller               */
#define PLB0_DCR_BASE       0x080       /* PLB Arbiter                       */
#define POB0_DCR_BASE       0x090       /* PLB to OPB Bridge                 */
#define OPB0_DCR_BASE       0x0A0       /* OPB to PLB Bridge                 */
#define CPM0_DCR_BASE       0x0B0       /* Clocking and Power Management     */
#define UIC0_DCR_BASE       0x0C0       /* Universal Interrupt Controller    */
#define UIC1_DCR_BASE       0x0D0       /* Universal Interrupt Controller    */
#define DMA0_DCR_BASE       0x100       /* DMA Controller                    */
#define MAL0_DCR_BASE       0x180       /* Memory Access Layer Core          */
#define UIC2_DCR_BASE       0x210       /* Universal Interrupt Controller    */
#define UICB_DCR_BASE       0x200       /* Universal Interrupt Controller    */


/*
 * Static interrupt vectors/levels.  These also correspond to bit numbers in
 * many of the registers of the Universal Interrupt Controller.
 */

/* handled by UIC0 */
#define INT_VEC_UART0             0
#define INT_VEC_UART1             1
#define INT_VEC_IIC0              2
#define INT_VEC_IIC1              3
#define INT_VEC_PCI_PIM           4
#define INT_VEC_PCI_PCRW          5
#define INT_VEC_PCI_PPM           6
#define INT_VEC_PCI_MSI0          7
#define INT_VEC_PCI_MSI1          8
#define INT_VEC_PCI_MSI2          9
#define INT_VEC_MAL_TXEOB         10
#define INT_VEC_MAL_RXEOB         11
#define INT_VEC_DMA_CH0           12
#define INT_VEC_DMA_CH1           13
#define INT_VEC_DMA_CH2           14
#define INT_VEC_DMA_CH3           15
#define INT_VEC_GPT_CT5           16
#define INT_VEC_GPT_CT6           17
#define INT_VEC_GPT_CT0           18
#define INT_VEC_GPT_CT1           19
#define INT_VEC_GPT_CT2           20
#define INT_VEC_GPT_CT3           21
#define INT_VEC_GPT_CT4           22
#define INT_VEC_EXT_IRQ_0         23
#define INT_VEC_EXT_IRQ_1         24
#define INT_VEC_EXT_IRQ_2         25
#define INT_VEC_EXT_IRQ_3         26
#define INT_VEC_EXT_IRQ_4         27
#define INT_VEC_EXT_IRQ_5         28
#define INT_VEC_EXT_IRQ_6         29
#define INT_VEC_UIC1NC            30
#define INT_VEC_UIC1C             31

/* handled by UIC1 */
#define INT_VEC_UIC1_BASE         0
#define INT_VEC_MAL_SERR          (INT_VEC_UIC1_BASE+0)
#define INT_VEC_MAL_TXDE          (INT_VEC_UIC1_BASE+1)
#define INT_VEC_MAL_RXDE          (INT_VEC_UIC1_BASE+2)
#define INT_VEC_SDRAM_ECC_UE      (INT_VEC_UIC1_BASE+3)
#define INT_VEC_SDRAM_ECC_CE      (INT_VEC_UIC1_BASE+4)
#define INT_VEC_EBCO              (INT_VEC_UIC1_BASE+5)
#define INT_VEC_EBMI              (INT_VEC_UIC1_BASE+6)
#define INT_VEC_OPB               (INT_VEC_UIC1_BASE+7)
#define INT_VEC_PCI_MSI3          (INT_VEC_UIC1_BASE+8)
#define INT_VEC_PCI_MSI4          (INT_VEC_UIC1_BASE+9)
#define INT_VEC_PCI_MSI5          (INT_VEC_UIC1_BASE+10)
#define INT_VEC_PCI_MSI6          (INT_VEC_UIC1_BASE+11)
#define INT_VEC_PCI_MSI7          (INT_VEC_UIC1_BASE+12)
#define INT_VEC_PCI_MSI8          (INT_VEC_UIC1_BASE+13)
#define INT_VEC_PCI_MSI9          (INT_VEC_UIC1_BASE+14)
#define INT_VEC_PCI_MSI10         (INT_VEC_UIC1_BASE+15)
#define INT_VEC_PCI_MSI11         (INT_VEC_UIC1_BASE+16)
#define INT_VEC_PCI_PLBPM         (INT_VEC_UIC1_BASE+17)
#define INT_VEC_EXT_IRQ_7         (INT_VEC_UIC1_BASE+18)
#define INT_VEC_EXT_IRQ_8         (INT_VEC_UIC1_BASE+19)
#define INT_VEC_EXT_IRQ_9         (INT_VEC_UIC1_BASE+20)
#define INT_VEC_EXT_IRQ_10        (INT_VEC_UIC1_BASE+21)
#define INT_VEC_EXT_IRQ_11        (INT_VEC_UIC1_BASE+22)
#define INT_VEC_EXT_IRQ_12        (INT_VEC_UIC1_BASE+23)
#define INT_VEC_EXT_SRE           (INT_VEC_UIC1_BASE+24)
#define INT_VEC_PCI_ASYNC         (INT_VEC_UIC1_BASE+27)
#define INT_VEC_ENET_0            (INT_VEC_UIC1_BASE+28)
#define INT_VEC_ENET_WAKEUP_0     (INT_VEC_UIC1_BASE+29)
#define INT_VEC_ENET_1            (INT_VEC_UIC1_BASE+30)
#define INT_VEC_ENET_WAKEUP_1     (INT_VEC_UIC1_BASE+31)

/* handled by UIC2 */
#define INT_VEC_UIC2_BASE         0
#define INT_VEC_ENET_2            (INT_VEC_UIC2_BASE+0)
#define INT_VEC_ENET_WAKEUP_2     (INT_VEC_UIC2_BASE+1)
#define INT_VEC_ENET_3            (INT_VEC_UIC2_BASE+2)
#define INT_VEC_ENET_WAKEUP_3     (INT_VEC_UIC2_BASE+3)
#define INT_VEC_TAH_0             (INT_VEC_UIC2_BASE+4)
#define INT_VEC_TAH_1             (INT_VEC_UIC2_BASE+5)
#define INT_VEC_IMU_OFQ_FULL      (INT_VEC_UIC2_BASE+6)
#define INT_VEC_IMU_IPQ_WRTN      (INT_VEC_UIC2_BASE+7)
#define INT_VEC_IMU_IRQ_DB        (INT_VEC_UIC2_BASE+8)
#define INT_VEC_IMU_INB_DB        (INT_VEC_UIC2_BASE+9)
#define INT_VEC_IMU_IMR0          (INT_VEC_UIC2_BASE+10)
#define INT_VEC_IMU_IMR1          (INT_VEC_UIC2_BASE+11)
#define INT_VEC_IMU_TO_ERR        (INT_VEC_UIC2_BASE+12)
#define INT_VEC_PCI_MSI12         (INT_VEC_UIC2_BASE+13)
#define INT_VEC_PCI_MSI13         (INT_VEC_UIC2_BASE+14)
#define INT_VEC_PCI_MSI14         (INT_VEC_UIC2_BASE+15)
#define INT_VEC_PCI_MSI15         (INT_VEC_UIC2_BASE+16)
#define INT_VEC_EXT_IRQ_13        (INT_VEC_UIC2_BASE+17)
#define INT_VEC_EXT_IRQ_14        (INT_VEC_UIC2_BASE+18)
#define INT_VEC_EXT_IRQ_15        (INT_VEC_UIC2_BASE+19
#define INT_VEC_EXT_IRQ_16        (INT_VEC_UIC2_BASE+20)
#define INT_VEC_EXT_IRQ_17        (INT_VEC_UIC2_BASE+21)
#define INT_VEC_PCI_VPD           (INT_VEC_UIC2_BASE+22)
#define INT_VEC_L2                (INT_VEC_UIC2_BASE+23)
#define INT_VEC_EMAC2_PCS         (INT_VEC_UIC2_BASE+24)
#define INT_VEC_EMAC3_PCS         (INT_VEC_UIC2_BASE+25)

/* handled by UICB */
#define INT_VEC_UICB_BASE         0
#define INT_VEC_CAS0_NORMAL       (INT_VEC_UICB_BASE+0)
#define INT_VEC_CAS0_CRITICAL     (INT_VEC_UICB_BASE+1)
#define INT_VEC_CAS1_NORMAL       (INT_VEC_UICB_BASE+2)
#define INT_VEC_CAS1_CRITICAL     (INT_VEC_UICB_BASE+3)
#define INT_VEC_CAS2_NORMAL       (INT_VEC_UICB_BASE+4)
#define INT_VEC_CAS3_CRITICAL     (INT_VEC_UICB_BASE+5)

/* handled by UIC0 */
#define INT_LVL_UART0             0
#define INT_LVL_UART1             1
#define INT_LVL_IIC0              2
#define INT_LVL_IIC1              3
#define INT_LVL_PCI_PIM           4
#define INT_LVL_PCI_PCRW          5
#define INT_LVL_PCI_PPM           6
#define INT_LVL_PCI_MSI0          7
#define INT_LVL_PCI_MSI1          8
#define INT_LVL_PCI_MSI2          9
#define INT_LVL_MAL_TXEOB         10
#define INT_LVL_MAL_RXEOB         11
#define INT_LVL_DMA_CH0           12
#define INT_LVL_DMA_CH1           13
#define INT_LVL_DMA_CH2           14
#define INT_LVL_DMA_CH3           15
#define INT_LVL_GPT_CT5           16
#define INT_LVL_GPT_CT6           17
#define INT_LVL_GPT_CT0           18
#define INT_LVL_GPT_CT1           19
#define INT_LVL_GPT_CT2           20
#define INT_LVL_GPT_CT3           21
#define INT_LVL_GPT_CT4           22
#define INT_LVL_EXT_IRQ_0         23
#define INT_LVL_EXT_IRQ_1         24
#define INT_LVL_EXT_IRQ_2         25
#define INT_LVL_EXT_IRQ_3         26
#define INT_LVL_EXT_IRQ_4         27
#define INT_LVL_EXT_IRQ_5         28
#define INT_LVL_EXT_IRQ_6         29
#define INT_LVL_UIC1NC            30
#define INT_LVL_UIC1C             31

/* handled by UIC1 */
#define INT_LVL_UIC1_BASE         0
#define INT_LVL_MAL_SERR          (INT_LVL_UIC1_BASE+0)
#define INT_LVL_MAL_TXDE          (INT_LVL_UIC1_BASE+1)
#define INT_LVL_MAL_RXDE          (INT_LVL_UIC1_BASE+2)
#define INT_LVL_SDRAM_ECC_UE      (INT_LVL_UIC1_BASE+3)
#define INT_LVL_SDRAM_ECC_CE      (INT_LVL_UIC1_BASE+4)
#define INT_LVL_EBCO              (INT_LVL_UIC1_BASE+5)
#define INT_LVL_EBMI              (INT_LVL_UIC1_BASE+6)
#define INT_LVL_OPB               (INT_LVL_UIC1_BASE+7)
#define INT_LVL_PCI_MSI3          (INT_LVL_UIC1_BASE+8)
#define INT_LVL_PCI_MSI4          (INT_LVL_UIC1_BASE+9)
#define INT_LVL_PCI_MSI5          (INT_LVL_UIC1_BASE+10)
#define INT_LVL_PCI_MSI6          (INT_LVL_UIC1_BASE+11)
#define INT_LVL_PCI_MSI7          (INT_LVL_UIC1_BASE+12)
#define INT_LVL_PCI_MSI8          (INT_LVL_UIC1_BASE+13)
#define INT_LVL_PCI_MSI9          (INT_LVL_UIC1_BASE+14)
#define INT_LVL_PCI_MSI10         (INT_LVL_UIC1_BASE+15)
#define INT_LVL_PCI_MSI11         (INT_LVL_UIC1_BASE+16)
#define INT_LVL_PCI_PLBPM         (INT_LVL_UIC1_BASE+17)
#define INT_LVL_EXT_IRQ_7         (INT_LVL_UIC1_BASE+18)
#define INT_LVL_EXT_IRQ_8         (INT_LVL_UIC1_BASE+19)
#define INT_LVL_EXT_IRQ_9         (INT_LVL_UIC1_BASE+20)
#define INT_LVL_EXT_IRQ_10        (INT_LVL_UIC1_BASE+21)
#define INT_LVL_EXT_IRQ_11        (INT_LVL_UIC1_BASE+22)
#define INT_LVL_EXT_IRQ_12        (INT_LVL_UIC1_BASE+23)
#define INT_LVL_EXT_SRE           (INT_LVL_UIC1_BASE+24)
#define INT_LVL_PCI_ASYNC         (INT_LVL_UIC1_BASE+27)
#define INT_LVL_ENET_0            (INT_LVL_UIC1_BASE+28)
#define INT_LVL_ENET_WAKEUP_0     (INT_LVL_UIC1_BASE+29)
#define INT_LVL_ENET_1            (INT_LVL_UIC1_BASE+30)
#define INT_LVL_ENET_WAKEUP_1     (INT_LVL_UIC1_BASE+31)

/* handled by UIC2 */
#define INT_LVL_UIC2_BASE         0
#define INT_LVL_ENET_2            (INT_LVL_UIC2_BASE+0)
#define INT_LVL_ENET_WAKEUP_2     (INT_LVL_UIC2_BASE+1)
#define INT_LVL_ENET_3            (INT_LVL_UIC2_BASE+2)
#define INT_LVL_ENET_WAKEUP_3     (INT_LVL_UIC2_BASE+3)
#define INT_LVL_TAH_0             (INT_LVL_UIC2_BASE+4)
#define INT_LVL_TAH_1             (INT_LVL_UIC2_BASE+5)
#define INT_LVL_IMU_OFQ_FULL      (INT_LVL_UIC2_BASE+6)
#define INT_LVL_IMU_IPQ_WRTN      (INT_LVL_UIC2_BASE+7)
#define INT_LVL_IMU_IRQ_DB        (INT_LVL_UIC2_BASE+8)
#define INT_LVL_IMU_INB_DB        (INT_LVL_UIC2_BASE+9)
#define INT_LVL_IMU_IMR0          (INT_LVL_UIC2_BASE+10)
#define INT_LVL_IMU_IMR1          (INT_LVL_UIC2_BASE+11)
#define INT_LVL_IMU_TO_ERR        (INT_LVL_UIC2_BASE+12)
#define INT_LVL_PCI_MSI12         (INT_LVL_UIC2_BASE+13)
#define INT_LVL_PCI_MSI13         (INT_LVL_UIC2_BASE+14)
#define INT_LVL_PCI_MSI14         (INT_LVL_UIC2_BASE+15)
#define INT_LVL_PCI_MSI15         (INT_LVL_UIC2_BASE+16)
#define INT_LVL_EXT_IRQ_13        (INT_LVL_UIC2_BASE+17)
#define INT_LVL_EXT_IRQ_14        (INT_LVL_UIC2_BASE+18)
#define INT_LVL_EXT_IRQ_15        (INT_LVL_UIC2_BASE+19
#define INT_LVL_EXT_IRQ_16        (INT_LVL_UIC2_BASE+20)
#define INT_LVL_EXT_IRQ_17        (INT_LVL_UIC2_BASE+21)
#define INT_LVL_PCI_VPD           (INT_LVL_UIC2_BASE+22)
#define INT_LVL_L2                (INT_LVL_UIC2_BASE+23)
#define INT_LVL_EMAC2_PCS         (INT_LVL_UIC2_BASE+24)
#define INT_LVL_EMAC3_PCS         (INT_LVL_UIC2_BASE+25)

/* handled by UICB */
#define INT_LVL_UICB_BASE         0
#define INT_LVL_CAS0_NORMAL       (INT_LVL_UICB_BASE+0)
#define INT_LVL_CAS0_CRITICAL     (INT_LVL_UICB_BASE+1)
#define INT_LVL_CAS1_NORMAL       (INT_LVL_UICB_BASE+2)
#define INT_LVL_CAS1_CRITICAL     (INT_LVL_UICB_BASE+3)
#define INT_LVL_CAS2_NORMAL       (INT_LVL_UICB_BASE+4)
#define INT_LVL_CAS3_CRITICAL     (INT_LVL_UICB_BASE+5)

/*
 * -----------  Processor Local Bus Address Assignments -------------
 */

/*
 * SDRAM PLB Addresses
 */

#define SDRAM0_BASE_PLB_UA	0x0
#define SDRAM0_BASE_PLB		0x00000000

/*
 * SRAM0 on the chip itself may be used as RAM, or as L2 cache. 
 */

#define SRAM0_BASE_PLB_UA 	0x0
#define SRAM0_BASE_PLB    	0x80000000
#define SRAM0_SIZE	  	0x00040000

/*
 * I2O Register PLB Addresses
 */

#define I2O_BASE_PLB_UA	  	0x0
#define I2O_BASE_PLB	  	0xffff0000
#define I2O_SIZE	  	0x00010000

/*
 * Peripheral Bus PLB Addresses
 *
 * The Peripheral Bus resides on the PLB bus at 0x1.4000.0000. It is
 * subdivided into onchip and an offchip peripheral address spaces.
 */

#define PERIPH_BASE_PLB_UA	0x1
#define PERIPH_BASE_PLB		0x40000000
#define EXT_PERIPH_BASE_PLB	(PERIPH_BASE_PLB + 0x08000000)
#define PERIPH_SIZE		0x1000

/*
 * Boot Space PLB Addresses
 */

#define BOOT_BASE_PLB_UA	0x1
#define BOOT_BASE_PLB		0xF0000000

/*
 * On-chip Synchronous RAM (SRAM) DCR registers.
 */
/********/
/* SRAM */
/********/

#define SRAM_SB0CR   0x00    /* SRAM Bank 0 Configuration Register        */
#define SRAM_SB1CR   0x01    /* SRAM Bank 1 Configuration Register        */
#define SRAM_SB2CR   0x02    /* SRAM Bank 2 Configuration Register        */
#define SRAM_SB3CR   0x03    /* SRAM Bank 3 Configuration Register        */
#define SRAM_BEAR    0x04    /* SRAM Bus Error Address Register           */
#define SRAM_BESR0   0x05    /* Bus Error Status Register 0               */
#define SRAM_BESR1   0x06    /* Bus Error Status Register 1               */
#define SRAM_PMEG    0x07    /* Power Management                          */
#define SRAM_DPC     0x0A    /* Data Parity Checking Register             */

#define SRAM0_SB0CR   (SRAM0_DCR_BASE + SRAM_SB0CR)
#define SRAM0_SB1CR   (SRAM0_DCR_BASE + SRAM_SB1CR)
#define SRAM0_SB2CR   (SRAM0_DCR_BASE + SRAM_SB2CR)
#define SRAM0_SB3CR   (SRAM0_DCR_BASE + SRAM_SB3CR)
#define SRAM0_BEAR    (SRAM0_DCR_BASE + SRAM_BEAR)
#define SRAM0_BESR0   (SRAM0_DCR_BASE + SRAM_BESR0)
#define SRAM0_BESR1   (SRAM0_DCR_BASE + SRAM_BESR1)
#define SRAM0_PMEG    (SRAM0_DCR_BASE + SRAM_PMEG)
#define SRAM0_DPC     (SRAM0_DCR_BASE + SRAM_DPC)

/*------------- SRAM BESR0 REG defines --------------------------------------*/
#define SRAM_BESR0_ERROR_MASK    0xF7DF7D00
#define SRAM_BESR0_ET0_MASK      0xE0000000
#define SRAM_BESR0_RWS0          0x10000000
#define SRAM_BESR0_FL0           0x08000000
#define SRAM_BESR0_AL0           0x04000000
#define SRAM_BESR0_ET1_MASK      0x03800000
#define SRAM_BESR0_RWS1          0x00400000
#define SRAM_BESR0_FL1           0x00200000
#define SRAM_BESR0_AL1           0x00100000
#define SRAM_BESR0_ET2_MASK      0x000E0000
#define SRAM_BESR0_RWS2          0x00010000
#define SRAM_BESR0_FL2           0x00008000
#define SRAM_BESR0_AL2           0x00004000
#define SRAM_BESR0_ET3_MASK      0x00003800
#define SRAM_BESR0_RWS3          0x00000400
#define SRAM_BESR0_FL3           0x00000200
#define SRAM_BESR0_AL3           0x00000100

/*------------- SRAM BESR1 REG defines --------------------------------------*/
#define SRAM_BESR1_ERROR_MASK    0xF7DF7D00
#define SRAM_BESR1_ET5_MASK      0x03800000
#define SRAM_BESR1_RWS5          0x00400000
#define SRAM_BESR1_FL5           0x00200000
#define SRAM_BESR1_AL5           0x00100000
#define SRAM_BESR1_ET6_MASK      0x000E0000
#define SRAM_BESR1_RWS6          0x00010000
#define SRAM_BESR1_FL6           0x00008000
#define SRAM_BESR1_AL6           0x00004000
#define SRAM_BESR1_ET7_MASK      0x00003800
#define SRAM_BESR1_RWS7          0x00000400
#define SRAM_BESR1_FL7           0x00000200
#define SRAM_BESR1_AL7           0x00000100

/*------------- SRAM DPC REG defines --------------------------------------*/
#define SRAM_DPC_DPC_MASK        0x80000000
#define SRAM_DPC_DPC_DISABLE     0x00000000
#define SRAM_DPC_DPC_ENABLE      0x80000000

/*------------- SRAM PMEG REG defines --------------------------------------*/
#define SRAM_PMEG_MASK           0xFFE00000
#define SRAM_PMEG_PM_EN          0x80000000
#define SRAM_PMEG_PM_CNT_MASK    0x7E000000
#define SRAM_PMEG_PM_DFLT_MASK   0x01E00000

/*------------- SRAM SBXCR REG defines --------------------------------------*/
#define SRAM_SBXCR_MASK          0xFFFFFF8F
#define SRAM_SBXCR_BAS_MASK      0xFFFFF000
#define SRAM_SBXCR_BAS_ENCODE(x) ((x)&0xFFFFF000)
#define SRAM_SBXCR_BAS_DECODE(x) ((x)&0xFFFFF000)
#define SRAM_SBXCR_BS_MASK       0x00000E00
#define   SRAM_SBXCR_BS_4KB      0x00000000
#define   SRAM_SBXCR_BS_8KB      0x00000200
#define   SRAM_SBXCR_BS_64KB     0x00000800
#define SRAM_SBXCR_BU_MASK       0x00000180
#define   SRAM_SBXCR_BU_DIS      0x00000000
#define   SRAM_SBXCR_BU_RO       0x00000080
#define   SRAM_SBXCR_BU_RW       0x00000180
#define SRAM_SBXCR_UA_MASK       0x0000000F
#define SRAM_SBXCR_UA_ENCODE(x) (((x)&0xF)<<0)
#define SRAM_SBXCR_UA_DECODE(x) (((x)>>0)&0xF)

/*
 * External Bus Controller Addresses
 */

#define EBC0_PHYS_ADRS_HIGH	0x1
#define EBC0_PHYS_ADRS_LOW	0x00000000


/*
 * External Bus Master Interface DCR registers
 */

#define EBM0_CFGADDR  (EBM0_DCR_BASE+0x0)  /* EBMI Controller Addr Reg */
#define EBM0_CFGDATA  (EBM0_DCR_BASE+0x1)  /* EBMI Controller Data Reg */

/* Indirect access registers for EBMI -- use EBM0_CFGADDR to access */

#define EBM0_CTL      0x00  /* EBMI Control Register           */
#define EBM0_LCTL     0x01  /* EBMI OPB Latency Count Register */
#define EBM0_BEAR     0x02  /* EBMI Bus Error Address Register */
#define EBM0_BESR     0x03  /* EBMI Bus Error Status  Register */
#define EBM0_BEMR     0x04  /* EBMI Bus Error Mask    Register */
#define EBM0_UAR      0x05  /* EBMI OPB Upper Address Register */
#define EBM0_UAM      0x06  /* EBMI OPB Upper Address Mask     */
#define EBM0_SLPMD    0x07  /* EBMI Sleep Mode        Register */
#define EBM0_FAIR     0x08  /* EBMI Fairness Control  Register */
#define EBM0_CID      0x11  /* EBMI Core ID           Register */

/*
 * L2 Controller DCR registers.
 */

#define L2C0_CFG                       (L2C0_DCR_BASE + 0x0)
#define L2C0_CMD                       (L2C0_DCR_BASE + 0x1)
#define L2C0_ADDR                      (L2C0_DCR_BASE + 0x2)
#define L2C0_DATA                      (L2C0_DCR_BASE + 0x3)
#define L2C0_SR                        (L2C0_DCR_BASE + 0x4)
#define L2C0_REVID                     (L2C0_DCR_BASE + 0x5)
#define L2C0_SNP0                      (L2C0_DCR_BASE + 0x6)
#define L2C0_SNP1                      (L2C0_DCR_BASE + 0x7)

#define L2C_CFG_L2M_MASK               0x80000000
#define L2C_CFG_L2M_DISABLE            0x00000000
#define L2C_CFG_L2M_ENABLE             0x80000000
#define L2C_CFG_ICU_MASK               0x40000000
#define L2C_CFG_ICU_DISABLE            0x00000000
#define L2C_CFG_ICU_ENABLE             0x40000000
#define L2C_CFG_DCU_MASK               0x20000000
#define L2C_CFG_DCU_DISABLE            0x00000000
#define L2C_CFG_DCU_ENABLE             0x20000000
#define L2C_CFG_DCW_MASK               0x1E000000
#define L2C_CFG_DCW_ENABLE0123         0x00000000
#define L2C_CFG_DCW_ENABLE012          0x02000000
#define L2C_CFG_DCW_ENABLE01           0x06000000
#define L2C_CFG_DCW_ENABLE0            0x0E000000
#define L2C_CFG_TPC_MASK               0x01000000
#define L2C_CFG_TPC_DISABLE            0x00000000
#define L2C_CFG_TPC_ENABLE             0x01000000
#define L2C_CFG_CPC_MASK               0x00800000
#define L2C_CFG_CPC_DISABLE            0x00000000
#define L2C_CFG_CPC_ENABLE             0x00800000
#define L2C_CFG_DOA_MASK               0x00400000
#define L2C_CFG_FRAN_MASK              0x00200000
#define L2C_CFG_FRAN_DISABLE           0x00000000
#define L2C_CFG_FRAN_ENABLE            0x00200000
#define L2C_CFG_SS_MASK                0x00180000
#define L2C_CFG_SS_256KB               0x00000000
#define L2C_CFG_CPIM_MASK              0x00040000
#define L2C_CFG_CPIM_ENABLE            0x00000000
#define L2C_CFG_CPIM_DISABLE           0x00040000
#define L2C_CFG_TPIM_MASK              0x00020000
#define L2C_CFG_TPIM_ENABLE            0x00000000
#define L2C_CFG_TPIM_DISABLE           0x00020000
#define L2C_CFG_LIM_MASK               0x00010000
#define L2C_CFG_LIM_ENABLE             0x00000000
#define L2C_CFG_LIM_DISABLE            0x00010000
#define L2C_CFG_PMUX_MASK              0x00007000
#define L2C_CFG_PMUX_DISABLE           0x00000000
#define L2C_CFG_PMIM_MASK              0x00000800
#define L2C_CFG_PMIM_ENABLE            0x00000000
#define L2C_CFG_PMIM_DISABLE           0x00000800
#define L2C_CFG_TPEI_MASK              0x00000400
#define L2C_CFG_TPEI_DISABLE           0x00000000
#define L2C_CFG_TPEI_ENABLE            0x00000400
#define L2C_CFG_CPEI_MASK              0x00000200
#define L2C_CFG_CPEI_DISABLE           0x00000000
#define L2C_CFG_CPEI_ENABLE            0x00000200
#define L2C_CFG_NAM_MASK               0x00000100
#define L2C_CFG_NAM_DISABLE            0x00000000
#define L2C_CFG_NAM_ENABLE             0x00000100
#define L2C_CFG_SMCM_MASK              0x00000080
#define L2C_CFG_SMCM_DISABLE           0x00000000
#define L2C_CFG_SMCM_ENABLE            0x00000080
#define L2C_CFG_MBRM_MASK              0x00000040
#define L2C_CFG_SCRM_MASK              0x00000020
#define L2C_CFG_SWRM_MASK              0x00000010

#define L2C_CMD_CLR_MASK               0x80000000
#define L2C_CMD_CLR_DISABLE            0x00000000
#define L2C_CMD_CLR_ENABLE             0x80000000
#define L2C_CMD_DIAG_MASK              0x40000000
#define L2C_CMD_DIAG_DISABLE           0x00000000
#define L2C_CMD_DIAG_ENABLE            0x40000000
#define L2C_CMD_INV_MASK               0x20000000
#define L2C_CMD_INV_DISABLE            0x00000000
#define L2C_CMD_INV_ENABLE             0x20000000
#define L2C_CMD_CCP_MASK               0x10000000
#define L2C_CMD_CCP_DISABLE            0x00000000
#define L2C_CMD_CCP_ENABLE             0x10000000
#define L2C_CMD_CTE_MASK               0x08000000
#define L2C_CMD_CTE_DISABLE            0x00000000
#define L2C_CMD_CTE_ENABLE             0x08000000
#define L2C_CMD_HCC_MASK               0x00800000
#define L2C_CMD_HCC_DISABLE            0x00000000
#define L2C_CMD_HCC_ENABLE             0x00800000

#define L2C_SR_CC                      0x80000000
#define L2C_SR_CPE                     0x40000000
#define L2C_SR_TPE                     0x20000000
#define L2C_SR_LRU                     0x10000000
#define L2C_SR_PCS                     0x08000000

#define L2C_SNPX_BA_MASK               0xFFFF0000
#define L2C_SNPX_BAH                   0xF0000000
#define L2C_SNPX_BAL                   0x0FFF0000
#define L2C_SNPX_SSR_MASK              0x0000F000
#define L2C_SNPX_SSR_1MB               0x00000000
#define L2C_SNPX_SSR_2MB               0x00001000
#define L2C_SNPX_SSR_4MB               0x00002000
#define L2C_SNPX_SSR_8MB               0x00003000
#define L2C_SNPX_SSR_16MB              0x00004000
#define L2C_SNPX_SSR_32MB              0x00005000
#define L2C_SNPX_SSR_64MB              0x00006000
#define L2C_SNPX_SSR_128MB             0x00007000
#define L2C_SNPX_SSR_256MB             0x00008000
#define L2C_SNPX_SSR_512MB             0x00009000
#define L2C_SNPX_SSR_1GB               0x0000A000
#define L2C_SNPX_SSR_2GB               0x0000B000
#define L2C_SNPX_SSR_4GB               0x0000C000
#define L2C_SNPX_SSR_8GB               0x0000D000
#define L2C_SNPX_SSR_16GB              0x0000E000
#define L2C_SNPX_SSR_32GB              0x0000F000
#define L2C_SNPX_ESR_MASK              0x00000800
#define L2C_SNPX_ESR_DISABLE           0x00000000
#define L2C_SNPX_ESR_ENABLE            0x00000800

/*
 * PLL Voltage Controlled Oscillator (VCO) definitions
 * Maximum and minimum values for correct PLL operation.
 * Maximum and minimum values for correct CPU and Bus operation.
 */
#define VCO_MHZ_MIN     500
#define VCO_MHZ_MAX     1333
#define CPU_HZ_MIN      0
#define CPU_HZ_MAX      666666666
#define PLB_HZ_MIN      0
#define PLB_HZ_MAX      166666666
#define OPB_HZ_MIN      0
#define OPB_HZ_MAX      (PLB_HZ_MAX/2)
#define EXTBUS_HZ_MIN   0
#define EXTBUS_HZ_MAX   (PLB_HZ_MAX/2)
#define PCIX_HZ_MIN     0
#define PCIX_HZ_MAX     133333333
#define SER_HZ_MIN      0
#define SER_HZ_MAX      133333333

/*
 * PLB Bus Error, Arbiter and PLB to OPB Bridge DCR registers.
 */

#define PLB0_REVID      (PLB0_DCR_BASE+0x02)
#define PLB0_ACR        (PLB0_DCR_BASE+0x03)
#define PLB0_BESR       (PLB0_DCR_BASE+0x04)
#define PLB0_BEARL      (PLB0_DCR_BASE+0x06)
#define PLB0_BEARH      (PLB0_DCR_BASE+0x07)

/* Defines for the PLB0_ACR register fields */
#define PLB_ACR_PPM_FIXED       0x00000000
#define PLB_ACR_PPM_FAIR        0x80000000
#define PLB_ACR_PPM_MASK        0x80000000
#define PLB_ACR_1               0x40000000
#define PLB_ACR_2               0x80000000
#define PLB_ACR_3               0xC0000000
#define PLB_ACR_MASK            0xC0000000

/*------------- PLB0 BESR definitions --------------------------------------*/
#define PLB0_BESR_ERROR_MASK	0x88888888
#define PLB0_BESR_CACHE_MASK	0x88800000
#define PLB0_BESR_CACHE_MASK_U	0x8880
#define PLB0_BESR_IO_MASK	0x00088888
#define PLB0_BESR_IO_MASK_U	0x0008
#define PLB0_BESR_IO_MASK_L	    0x8888
#define PLB0_BESR_PTE0		0x80000000	/* master 0:  Icache read */
#define PLB0_BESR_R_W0		0x40000000
#define PLB0_BESR_FLK0		0x20000000
#define PLB0_BESR_ALK0		0x10000000
#define PLB0_BESR_PTE1		0x08000000	/* master 1:  Dcache read */
#define PLB0_BESR_R_W1		0x04000000
#define PLB0_BESR_FLK1		0x02000000
#define PLB0_BESR_ALK1		0x01000000
#define PLB0_BESR_PTE2		0x00800000	/* master 2:  Dcache write */
#define PLB0_BESR_R_W2		0x00400000
#define PLB0_BESR_FLK2		0x00200000
#define PLB0_BESR_ALK2		0x00100000
#define PLB0_BESR_PTE3		0x00080000	/* master 3:  PCIX */
#define PLB0_BESR_R_W3		0x00040000	/* 1 => read, 0 => write */
#define PLB0_BESR_FLK3		0x00020000
#define PLB0_BESR_ALK3		0x00010000
#define PLB0_BESR_PTE4		0x00008000	/* master 4:  I2O */
#define PLB0_BESR_R_W4		0x00004000	/* 1 => read, 0 => write */
#define PLB0_BESR_FLK4		0x00002000
#define PLB0_BESR_ALK4		0x00001000
#define PLB0_BESR_PTE5		0x00000800	/* master 5:  MAL */
#define PLB0_BESR_R_W5		0x00000400	/* 1 => read, 0 => write */
#define PLB0_BESR_FLK5		0x00000200
#define PLB0_BESR_ALK5		0x00000100
#define PLB0_BESR_PTE6		0x00000080	/* master 6:  DMA */
#define PLB0_BESR_R_W6		0x00000040	/* 1 => read, 0 => write */
#define PLB0_BESR_FLK6		0x00000020
#define PLB0_BESR_ALK6		0x00000010
#define PLB0_BESR_PTE7		0x00000008	/* master 7:  OPB to PLB */
#define PLB0_BESR_R_W7		0x00000004	/* 1 => read, 0 => write */
#define PLB0_BESR_FLK7		0x00000002
#define PLB0_BESR_ALK7		0x00000001

/*
 * PLB to OPB Bridge Control DCR Registers (bridge out).
 */
#define POB0_BESR0      (POB0_DCR_BASE+0x0)
#define POB0_BEARL      (POB0_DCR_BASE+0x2)
#define POB0_BEARH      (POB0_DCR_BASE+0x3)
#define POB0_BESR1      (POB0_DCR_BASE+0x4)
#define POB0_CONFIG0    (POB0_DCR_BASE+0x6)
#define POB0_LATENCY0   (POB0_DCR_BASE+0x8)
#define POB0_REV        (POB0_DCR_BASE+0xa)

/*------------- POB0 BESR0 definitions --------------------------------------*/
#define POB0_BESR0_ERROR_MASK	0xC0C0C0C0
#define POB0_BESR0_CACHE_MASK	0xC0C0C000
#define POB0_BESR0_CACHE_MASK_U	0xC0C0
#define POB0_BESR0_CACHE_MASK_L	    0xC000
#define POB0_BESR0_IO_MASK	0x000000C0
#define POB0_BESR0_PTE0_MASK	0xC0000000	/* master 0:  Icache read */
#define POB0_BESR0_R_W0		0x20000000
#define POB0_BESR0_FLK0		0x10000000
#define POB0_BESR0_ALK0		0x08000000
#define POB0_BESR0_WIRQ0	0x04000000
#define POB0_BESR0_PTE1_MASK	0x00C00000	/* master 1:  Dcache read */
#define POB0_BESR0_R_W1		0x00200000
#define POB0_BESR0_FLK1		0x00100000
#define POB0_BESR0_ALK1		0x00080000
#define POB0_BESR0_WIRQ1	0x00040000
#define POB0_BESR0_PTE2_MASK	0x0000C000	/* master 2:  Dcache write */
#define POB0_BESR0_R_W2		0x00002000
#define POB0_BESR0_FLK2		0x00001000
#define POB0_BESR0_ALK2		0x00000800
#define POB0_BESR0_WIRQ2	0x00000400
#define POB0_BESR0_PTE3_MASK	0x000000C0	/* master 3:  PCIX */
#define POB0_BESR0_R_W3		0x00000020	/* 1 => read, 0 => write */
#define POB0_BESR0_FLK3		0x00000010
#define POB0_BESR0_ALK3		0x00000008
#define POB0_BESR0_WIRQ3	0x00000004

/*------------- POB0 BESR1 definitions --------------------------------------*/
#define POB0_BESR1_ERROR_MASK	0xC0C0C0C0
#define POB0_BESR1_IO_MASK	0xC0C0C0C0
#define POB0_BESR1_IO_MASK_U	0xC0C0
#define POB0_BESR1_IO_MASK_L	    0xC0C0
#define POB0_BESR1_PTE4_MASK	0xC0000000	/* master 4:  I2O */
#define POB0_BESR1_R_W4		0x20000000	/* 1 => read, 0 => write */
#define POB0_BESR1_FLK4		0x10000000
#define POB0_BESR1_ALK4		0x08000000
#define POB0_BESR1_WIRQ4	0x04000000
#define POB0_BESR1_PTE5_MASK	0x00C00000	/* master 5:  MAL */
#define POB0_BESR1_R_W5		0x00200000	/* 1 => read, 0 => write */
#define POB0_BESR1_FLK5		0x00100000
#define POB0_BESR1_ALK5		0x00080000
#define POB0_BESR1_WIRQ5	0x00040000
#define POB0_BESR1_PTE6_MASK	0x0000C000	/* master 6:  DMA */
#define POB0_BESR1_R_W6		0x00002000	/* 1 => read, 0 => write */
#define POB0_BESR1_FLK6		0x00001000
#define POB0_BESR1_ALK6		0x00000800
#define POB0_BESR1_WIRQ6	0x00000400
#define POB0_BESR1_PTE7_MASK	0x000000C0	/* master 7:  OPB to PLB */
#define POB0_BESR1_R_W7		0x00000020	/* 1 => read, 0 => write */
#define POB0_BESR1_FLK7		0x00000010
#define POB0_BESR1_ALK7		0x00000008
#define POB0_BESR1_WIRQ7	0x00000004

/*
 * OPB to PLB Bridge Control DCR Registers (bridge in).
 */
#define OPB0_CTRL  (OPB0_DCR_BASE+0x0)
#define OPB0_STAT  (OPB0_DCR_BASE+0x1)
#define OPB0_BEARL (OPB0_DCR_BASE+0x2)
#define OPB0_BEARH (OPB0_DCR_BASE+0x3)
#define OPB0_REVID (OPB0_DCR_BASE+0x4)

#define OPB_PRI_0    0x00000000
#define OPB_PRI_1    0x40000000
#define OPB_PRI_2    0x80000000
#define OPB_PRI_3    0xC0000000
#define OPB_PRI_MASK 0xC0000000

/*
 * IIC Base Address definition and register offsets.
 */
#define IIC0_BASE       (PERIPH_BASE_ADDR + 0x00000400)
#define IIC1_BASE       (PERIPH_BASE_ADDR + 0x00000500)

/* IIC register offsets */
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

/* mdbuf register */
#define IIC_MDBUF_FIFO_SIZE	4	/* Master Data Buffer size */

/* control register */
#define IIC_CNTL_HMT         0x80
#define IIC_CNTL_AMD         0x40
#define IIC_CNTL_TCT_MASK    0x30
#define IIC_CNTL_TCT_1       0x00
#define IIC_CNTL_TCT_2       0x10
#define IIC_CNTL_TCT_3       0x20
#define IIC_CNTL_TCT_4       0x30
#define IIC_CNTL_TCT_SHIFT	0x4	/* xfer count is bits 2:3 of byte */
#define IIC_CNTL_CHT		0x04	/* chain transfers: set for more data */
#define IIC_CNTL_RW		0x02	/* read/write: set to read */
#define IIC_CNTL_PT		0x01	/* pending transfer: set to start */

/* status register */
#define IIC_STS_SCMP		0x08	/* stop complete; set to clear */
#define IIC_STS_ERR		0x04	/* error detected */
#define IIC_STS_IRQA		0x02	/* irq active; set to clear */
#define IIC_STS_PT		0x01	/* pending transfer */

/* mode control register */
#define IIC_MDCNTL_FMDB		0x40	/* flush master data buffer */
#define IIC_MDCNTL_EUBS		0x02	/* exit unknown IIC bus state */
#define IIC_MDCNTL_HSCL		0x01	/* hold IIC serial clock low */

/* extended status register */
#define IIC_EXTSTS_IRQP		0x80	/* irq pending: set to clear */
#define IIC_EXTSTS_IRQD		0x08	/* irq on-deck; set to clear */
#define IIC_EXTSTS_LA 		0x04	/* lost arbitration: set on init */
#define IIC_EXTSTS_ICT 		0x02	/* incomplete xfer: set on init */
#define IIC_EXTSTS_XFRA		0x01	/* xfer aborted: set on init */

/* extended control and slave status */
#define IIC_XTCNTLSS_SRC	0x80	/* slave read complete */
#define IIC_XTCNTLSS_SRS	0x40	/* slave read needs service */
#define IIC_XTCNTLSS_SWC	0x20	/* slave write complete */
#define IIC_XTCNTLSS_SWS	0x10	/* slave write service */

/* XTCNTLSS Register Bit definition */
#define IIC_XTCNTLSS_SRST  0x01
#define IIC_XTCNTLSS_EPI   0x02
#define IIC_XTCNTLSS_SDBF  0x04
#define IIC_XTCNTLSS_SBDD  0x08
#define IIC_XTCNTLSS_SWS   0x10
#define IIC_XTCNTLSS_SWC   0x20
#define IIC_XTCNTLSS_SRS   0x40
#define IIC_XTCNTLSS_SRC   0x80
#define IIC_TIMEOUT 1			/* 1 seconde */

/* MDCNTL Register Bit definition */
#define IIC_MDCNTL_HSCL 0x01
#define IIC_MDCNTL_EUBS 0x02
#define IIC_MDCNTL_EINT 0x04
#define IIC_MDCNTL_ESM  0x08
#define IIC_MDCNTL_FSM  0x10
#define IIC_MDCNTL_EGC  0x20
#define IIC_MDCNTL_FMDB 0x40
#define IIC_MDCNTL_FSDB 0x80

/* CNTL Register Bit definition */
#define IIC_CNTL_PT     0x01
#define IIC_CNTL_READ   0x02
#define IIC_CNTL_CHT    0x04
#define IIC_CNTL_RPST   0x08

/* STS Register Bit definition */
#define IIC_STS_IRQA    0x02
#define IIC_STS_SCMP    0x08
#define IIC_STS_MDBF    0x10
#define IIC_STS_MDBS    0X20
#define IIC_STS_SLPR    0x40
#define IIC_STS_SSS     0x80

/*
 * UART (2 of them) Base Address definitions. Both UARTs are
 * 16550-like.
 */
#define UART0_BASE		(PERIPH_BASE_ADDR + 0x00000200)
#define UART1_BASE		(PERIPH_BASE_ADDR + 0x00000300)
#define UART_REG_ADDR_INTERVAL	1

#define UART_MEMORY_START	(PERIPH_BASE_ADDR + 0x00000000)
#define UART_MEMORY_END		(PERIPH_BASE_ADDR + 0x00000FFF)


/*
 * Outbound PLB_PCIX bridge address map
 * The outbound memory map consists of 5 fixed address areas (relative to the
 * base address of the PLB_PCIX bridge) and 3 programmable areas.
 *   Fixed:
 *      1. PCI I/O outbound window
 *      2. PCI extra I/O outbound window
 *      3. PCI configuration access register pair
 *      4. PCI configuration registers of the PLB_PCIX bridge itself.
 *      5. PCI special cycle outbound window
 *   Programmable:
 *      1. PCI Memory outbound window 0 (POM0)
 *      2. PCI Memory outbound window 0 (POM1)
 *      3. PCI Memory outbound window 0 (POM2)
 */

/*
 *  Base address of the PCI-X core in the chip.
 *  The physical address is 0x2.0000.0000
 */
#define PCIX0_BASE_PLB_UA	0x2
#define PCIX0_BASE      PCIX0_BASE_ADDR

#define PCI_EXTRA_IO_ADDR  0x0ec00000
#define PCI_IO_ADDR  0x08000000
#define PCI_BRIDGE_REG 0x0ec80000
#define PCI_SPECIAL_CYCLE 0x0ed00000

/*
 * PCI I/O outbound window (PLB and the PCI translation).
 */
#define PLB_PCI_IO_REGION_1_OFFSET  0x08000000   /* PLB side */
#define PLB_PCI_IO_REGION_1_START   (PCIX0_BASE + PLB_PCI_IO_REGION_1_OFFSET) 
#define PLB_PCI_IO_REGION_1_SIZE    0x00010000                  /* 64KB     */
#define PCI_IO_REGION_1_START       0x00000000                  /* PCI side */

/*
 * PCI extra I/O outbound window (PLB and the PCI translation).
 */
#define PLB_PCI_IO_REGION_2_OFFSET  0x08800000   /* PLB side */
#define PLB_PCI_IO_REGION_2_START   (PCIX0_BASE + PLB_PCI_IO_REGION_2_OFFSET) 
#define PLB_PCI_IO_REGION_2_SIZE    0x03800000                  /* 56MB     */
#define PCI_IO_REGION_2_START       0x00800000                  /* PCI side */

/*
 * The following register pair is used to generate configuration cycles
 * on the PCI bus.  This region can be mapped with a single 4KB page in the MMU.
 */
#define PCIX0_CFG_PAIR_ADRS         (PCIX0_BASE + 0x0EC00000)
#define PCIX0_CFG_PAIR_DATA         (PCIX0_BASE + 0x0EC00004)
#define PCIX0_CFG_PAIR_END          (PCIX0_BASE + 0x0EC00FFF)

/*
 * Base address where the PCI configuration registers of the PLB_PCIX bridge
 * itself reside.  Individual register address definitions are below.
 * This region can be mapped with a single 4KB page in the MMU.
 */
#define PCIX0_CONFG_BASE              (PCIX0_BASE + 0x0EC80000)
#define PCIX0_CFG_END               (PCIX0_BASE + 0x0EC80FFF)

/*
 * Defines for the standard PCI device configuration header (offsets and bits).
 * Offsets are relative to PCIX0_CFG_BASE.
 */
#define PCIC_VENDID    0x00
#define PCIC_DEVID     0x02
#define PCIC_CMD       0x04
  #define PCIC_SE    0x0100
  #define PCIC_ME    0x0004
  #define PCIC_MA    0x0002
  #define PCIC_IOA   0x0001
#define PCIC_STATUS    0x06
  #define PCIC_C66   0x0020
#define PCIC_REVID     0x08
#define PCIC_CLS       0x09
  #define PCIC_INTCLS  0x09
  #define PCIC_SUBCLS  0x0A
  #define PCIC_BASECLS 0x0B
#define PCIC_CACHEL    0x0C
#define PCIC_LATTIM    0x0D
#define PCIC_HDTYPE    0x0E
#define PCIC_BIST      0x0F
#define PCIC_BAR0      0x10
#define PCIC_BAR1      0x14
#define PCIC_BAR2      0x18
#define PCIC_BAR3      0x1C
#define PCIC_BAR4      0x20
#define PCIC_BAR5      0x24
#define PCIC_CISPTR    0x28
#define PCIC_SBSYSVID  0x2C
#define PCIC_SBSYSID   0x2E
#define PCIC_EROMBA    0x30
#define PCIC_CAP       0x34
#define PCIC_INTLN     0x3C
#define PCIC_INTPN     0x3D
#define PCIC_MINGNT    0x3E
#define PCIC_MAXLTNCY  0x3F
#define   BRDGOPT1_PLREQ      0x00000060
#define     BRDGOPT1_PLREQ_0  0x00000000
#define     BRDGOPT1_PLREQ_1  0x00000020
#define     BRDGOPT1_PLREQ_2  0x00000040
#define     BRDGOPT1_PLREQ_3  0x00000060
#define PCIC_ERREN     0x50
#define PCIC_ERRSTS    0x54
#define PCIC_PLBSEA    0x58
#define PCIC_PLBBEARL  0x5C
#define PCIC_PLBBEARH  0x60
#define PCIL_POM0LAL   0x68
#define PCIL_POM0LAH   0x6C
#define PCIL_POM0MA    0x70
#define PCIL_POM0PCILA 0x74
#define PCIL_POM0PCIHA 0x78
#define PCIL_POM1LAL   0x7C
#define PCIL_POM1LAH   0x80
#define PCIL_POM1MA    0x84
#define PCIL_POM1PCILA 0x88
#define PCIL_POM1PCIHA 0x8C
#define PCIL_POM2MA    0x90
#define PCIL_PIM0SAL   0x98
#define PCIL_PIM0SAH   0xF8
#define PCIL_PIM0LAL   0x9C
#define PCIL_PIM0LAH   0xA0
#define PCIL_PIM1SA    0xA4
#define PCIL_PIM1LAL   0xA8
#define PCIL_PIM1LAH   0xAC
#define PCIL_PIM2SAL   0xB0
#define PCIL_PIM2SAH   0xFC
#define PCIL_PIM2LAL   0xB4
#define PCIL_PIM2LAH   0xB8
#define PCIM_CAPID     0xC0
#define PCIM_NEXTIPTR  0xC1
#define PCIM_MC        0xC2
#define PCIM_MA        0xC4
#define PCIM_MUA       0xC8
#define PCIM_MD        0xCC
#define PCIM_MEOI      0xCE
#define PCIC_CAPID     0xD0
#define PCIC_NEXTIPTR  0xD1
#define PCIC_PMC       0xD2
#define PCIC_PMCSR     0xD4
#define PCIC_PMCSRBSE  0xD6
#define PCIC_DATA      0xD7
#define PCIC_PMCSRR    0xD8
#define PCIB_CAPID     0xDC
#define PCIB_NEXTIPTR  0xDD
#define PCIB_CMD       0xDE
#define PCIB_STS       0xE0
#define   PCIXSTS_BUSNUM     0x0000FF00
#define   PCIXSTS_DEVNUM     0x000000F8
#define PCIB_IDR       0xE4
#define PCIB_CID       0xE8
#define PCIB_RID       0xEC
#define PCI_VPDCAPID   0xF0
#define PCI_VPDNIPTR   0xF1
#define PCI_VPDADR     0xF2
#define PCI_VPDDATA    0xF4
#define PCIS_MSGI      0x100
#define PCIS_MSGIH     0x104
#define PCIS_MSGO      0x108
#define PCIS_MSGOH     0x10C
#define PCIS_IMSI      0x1F8

#define PCIC_BRIDGEOPT1  0x40
#define PCIX0_BRDGOPT1  (PCIX0_CONFG_BASE + PCIC_BRIDGEOPT1)

/*
 * The following register is used to generate outbound special cycles on the
 * PCI bus. This register supports only 4 byte writes. This region can be mapped
 * with a single 4KB page in the MMU.
 */
#define PCIX0_SPECIAL_CYCLE         (PCIX0_BASE + 0x0ED00000)
#define PCIX0_SPECIAL_CYCLE_END     (PCIX0_BASE + 0x0ED00FFF)

/*
 * Bit definitions for use with POM Size Attribute registers (PCIX0_POMxSA)
 */
#define POM_SIZE_MASK      0xFFF00000

#define POM_SIZE_1MB       0xFFF00000
#define POM_SIZE_2MB       0xFFE00000
#define POM_SIZE_4MB       0xFFC00000
#define POM_SIZE_8MB       0xFF800000
#define POM_SIZE_16MB      0xFF000000
#define POM_SIZE_32MB      0xFE000000
#define POM_SIZE_64MB      0xFC000000
#define POM_SIZE_128MB     0xF8000000
#define POM_SIZE_256MB     0xF0000000
#define POM_SIZE_512MB     0xE0000000
#define POM_SIZE_1GB       0xC0000000
#define POM_SIZE_2GB       0x80000000
#define POM_SIZE_4GB       0x00000000
#define POM_ENABLE         0x00000001
#define POM_UNUSED         0x00000000


/*
 * Bit definitions for use with PIM Size Attribute registers (PCIX0_PIMxSA)
 */
#define PIM_SIZE_MASK      0xFFFFFF00

#define PIM_SIZE_4KB       0xFFFFF000
#define PIM_SIZE_8KB       0xFFFFE000
#define PIM_SIZE_16KB      0xFFFFC000
#define PIM_SIZE_32KB      0xFFFF8000
#define PIM_SIZE_64KB      0xFFFF0000
#define PIM_SIZE_128KB     0xFFFE0000
#define PIM_SIZE_256KB     0xFFFC0000
#define PIM_SIZE_512KB     0xFFF80000
#define PIM_SIZE_1MB       0xFFF00000
#define PIM_SIZE_2MB       0xFFE00000
#define PIM_SIZE_4MB       0xFFC00000
#define PIM_SIZE_8MB       0xFF800000
#define PIM_SIZE_16MB      0xFF000000
#define PIM_SIZE_32MB      0xFE000000
#define PIM_SIZE_64MB      0xFC000000
#define PIM_SIZE_128MB     0xF8000000
#define PIM_SIZE_256MB     0xF0000000
#define PIM_SIZE_512MB     0xE0000000
#define PIM_SIZE_1GB       0xC0000000
#define PIM_SIZE_2GB       0x80000000
#define PIM_SIZE_4GB       0x00000000
#define PIM_PREFETCH       0x00000002
#define PIM_ENABLE         0x00000001
#define PIM_UNUSED         0x00000000


/*
 * General Purpose I/O (GPIO)
 */

#define GPIO_BASE       (PERIPH_BASE_ADDR + 0x00000700)

#define GPIO_OR         (GPIO_BASE+0x00)        /* GPIO Output Register       */
#define GPIO_TCR        (GPIO_BASE+0x04)        /* GPIO Three-state Ctrl Reg  */
#define GPIO_ODR        (GPIO_BASE+0x18)        /* GPIO Open Drain Reg        */
#define GPIO_IR         (GPIO_BASE+0x1c)        /* GPIO Input Register        */

/*
 * Base address of Onchip Peripheral Bus (OPB) Arbiter registers in the chip.
 * Memory mapped registers are located at physical address 0x1.4000.0600.
 */

#define OPBA0_BASE      (PERIPH_BASE_ADDR + 0x00000600)

#define OPBA0_PR        (OPBA0_BASE+0x00)       /* OPB Arbiter Priority Reg   */
#define OPBA0_CR        (OPBA0_BASE+0x01)       /* OPB Arbiter Control Reg    */

/*
 * Base address of Ethernet (EMAC) registers in the chip
 */

#define EMAC0_BASE      (PERIPH_BASE_ADDR + 0x00000800)
#define EMAC1_BASE      (PERIPH_BASE_ADDR + 0x00000900)
#define EMAC2_BASE      (PERIPH_BASE_ADDR + 0x00000C00)
#define EMAC3_BASE      (PERIPH_BASE_ADDR + 0x00000E00)

#ifndef ZMII0_BASE
#define ZMII0_BASE      (PERIPH_BASE_ADDR + 0x00000780)
#endif /* ZMII0_BASE */
#ifndef RGMII0_BASE
#define RGMII0_BASE     (PERIPH_BASE_ADDR + 0x00000790)
#endif /* RGMII0_BASE */
#define TAH0_BASE       (PERIPH_BASE_ADDR + 0x00000B00)
#define TAH1_BASE       (PERIPH_BASE_ADDR + 0x00000D00)

/*
 * Base address of General Purpose Timer (GPT0) registers in the chip 
 */

#define GPT0_BASE       (PERIPH_BASE_ADDR + 0x00000900)

/*
 * Timer clock divider ratios (in 2^n)
 */

#define FIT_DIVIDER_TAP_3	25
#define FIT_DIVIDER_TAP_2	21
#define FIT_DIVIDER_TAP_1	17
#define FIT_DIVIDER_TAP_0	13

#define WDT_DIVIDER_TAP_3	33
#define WDT_DIVIDER_TAP_2	29
#define WDT_DIVIDER_TAP_1	25
#define WDT_DIVIDER_TAP_0	21

/*
 * Header files for other cores in the chip 
 */

#include "sysCpcr.h"
#include "uicDcr.h"            /* Universal Interrupt Controller */
#include "malDcr.h"            /* Memory Access Layer DCR        */
#include "sdramDcr.h"          /* SDRAM Controller               */
#include "ebcDcr.h"            /* External Bus Controller        */
#include "dmaDcr.h"            /* DMA Controller                 */
#include "cprDcr.h"            /* Chip Control regs              */
#include "sdrDcr.h"            /* Chip Control regs              */

#ifdef __cplusplus
    }
#endif

/* Validate and instantiate 440GX Machine Check Handler configuration */
#ifdef	INCLUDE_440X5_DCACHE_RECOVERY
# ifndef INCLUDE_440X5_PARITY_RECOVERY
#  define INCLUDE_440X5_PARITY_RECOVERY
# endif
# if (USER_D_CACHE_MODE != CACHE_DISABLED && defined(USER_D_CACHE_ENABLE))
#  undef	USER_D_CACHE_MODE
#  define	USER_D_CACHE_MODE	(CACHE_WRITETHROUGH)
# endif /* USER_D_CACHE_MODE != CACHE_DISABLED) */
#define	_WRS_PPC440X5_CFG_DCR	TRUE
#else	/* INCLUDE_440X5_DCACHE_RECOVERY */
#define	_WRS_PPC440X5_CFG_DCR	FALSE
#endif	/* INCLUDE_440X5_DCACHE_RECOVERY */

#ifdef	INCLUDE_440X5_TLB_RECOVERY_MAX
# if	(VEC_BASE_ADRS == VEC_BASE_ALT_ADRS)
# error "INCLUDE_440X5_TLB_RECOVERY_MAX requires VEC_BASE_ADRS != VEC_BASE_ALT_ADRS"
# endif
# ifndef INCLUDE_440X5_TLB_RECOVERY
# define INCLUDE_440X5_TLB_RECOVERY
# endif
#endif

#ifdef	INCLUDE_440X5_TLB_RECOVERY
# ifndef INCLUDE_440X5_PARITY_RECOVERY
# define INCLUDE_440X5_PARITY_RECOVERY
# endif
# ifndef INCLUDE_MMU_BASIC
# define INCLUDE_MMU_BASIC
# endif
#define	_WRS_PPC440X5_CFG_TLB	TRUE
#else	/* INCLUDE_440X5_TLB_RECOVERY */
#define	_WRS_PPC440X5_CFG_TLB	FALSE
#endif	/* INCLUDE_440X5_TLB_RECOVERY */

#ifdef	INCLUDE_440X5_MCH_LOGGER
# ifndef _PPC_440X5_MCH_LOG_RTN
# define _PPC_440X5_MCH_LOG_RTN	(FUNCPTR)1
# endif	 /* _PPC_440X5_MCH_LOG_RTN */
#else	/* INCLUDE_440X5_MCH_LOGGER */
#undef	_PPC_440X5_MCH_LOG_RTN
#define _PPC_440X5_MCH_LOG_RTN	(FUNCPTR)NULL
#endif	/* INCLUDE_440X5_MCH_LOGGER */

#ifdef	INCLUDE_440X5_PARITY_RECOVERY
# ifndef INCLUDE_EXC_HANDLING
# define INCLUDE_EXC_HANDLING
# endif
# ifndef _ASMLANGUAGE
IMPORT BOOL exc440x5_parity, exc440x5_tlb, exc440x5_dcache;
IMPORT char * excVecBaseAltAdrs;
IMPORT FUNCPTR exc440x5_logger;
# endif
#define	INCLUDE_SYS_HW_INIT_0
#define	SYS_HW_INIT_0()	exc440x5_parity = TRUE, exc440x5_tlb = _WRS_PPC440X5_CFG_TLB, exc440x5_dcache = _WRS_PPC440X5_CFG_DCR, excVecBaseAltAdrs = VEC_BASE_ALT_ADRS, exc440x5_logger = _PPC_440X5_MCH_LOG_RTN
#endif	/* INCLUDE_440X5_PARITY_RECOVERY */

#endif  /* INCppc440GXh */
