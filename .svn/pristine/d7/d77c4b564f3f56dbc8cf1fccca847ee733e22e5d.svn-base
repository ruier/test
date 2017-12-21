/* ibmEvb440gx.h - amcc_taishan BSP (AMCC EVB440GX 440GX eval board) header */

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
 * Copyright (c) 2002-2004, 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01l,08dec07,x_s  Removed PCI Reading and Writing MACROS
01k,26mar07,x_s  Add LCM define and delete NVRAM define
01j,09jun06,wap  Add definition for SMII PHY reset bit in FPGA_REG2
01i,29sep04,mdo  Documentation fixes for apigen
01h,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
01g,17dec03,jtp  SPR 92409 Rev 3 board & gigabit; SPR 92575 I-MMU support
01f,03sep03,jtp  Correct SML_FLASH_SIZE
01e,05aug03,jtp  Formal code inspection changes. Added macros to better
                 describe board memory map and key TLB mappings.
01d,16jul03,jtp  Support TFFS, add PLB addresses for Flash & SRAM
01c,29may03,pch  remove EVB_IVPR_VAL
01b,28may03,jtp  Merge IBM changes; add FPGA_REGn macros, rename macro to
                 ENET_TBI_COPPER_PHY
01a,08nov02,saw  created based on ebony.h rev 01k
*/

/*
This file contains I/O addresses and related constants for the amcc_taishan
BSP, which supports the IBM 440GX evaluation board.  Many of the
constants in here are dictated by the board design, which are detailed in
the IBM PPC440GX Evaluation Board User's Manual. Others reflect choices
made in the software implementation of a board support package on this
board - in particular, the mapping of the program address space.
*/

#ifndef INCibmEvb440gxh
#define INCibmEvb440gxh

#ifdef __cplusplus
    extern "C" {
#endif

/* generic macros */

#ifndef EIEIO_SYNC
# define EIEIO_SYNC  _WRS_ASM(" mbar; msync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
# define EIEIO  _WRS_ASM(" mbar")
#endif  /* EIEIO */

#ifndef SYNC
# define SYNC  _WRS_ASM(" msync")
#endif  /* SYNC */

#ifndef EIEIO
#define EIEIO __asm__ volatile ("eieio");
#endif

#define BUS		NONE		     /* no off-board bus interface    */
#define N_SIO_CHANNELS	2		     /* Number of serial I/O channels */

/*
 * Minimum and maximum system clock rates
 */
#define SYS_CLK_RATE_MIN	3	     /* minimum system clock rate     */
#define SYS_CLK_RATE_MAX	5000	     /* maximum system clock rate     */


/*
 * Misc clock defines
 *
 * The clock supplied to the sysClk pin of the chip varies depending on
 * board switch settings.  sysInfoGet() determines this clock speed.
 */
#define ONE_MILLION        	1000000
#define ONE_BILLION        	1000000000             
#define PERIOD_33_3MHZ       		30000 

/*
 * The EVB440GX board FPGA supplies a 25MHz clock to the external timer clock
 * pin of the chip (TmrClk).
 */

#define EXT_TIMER_CLK_FREQ   25000000           /* FPGA supplied 25MHz        */  

/*
 * The EVB440GX board supplies a 11.0592 MHz clock to the external serial clock
 * pin of the chip (UARTSerClk).
 */
#define EXT_SER_CLK_FREQ     11059200           /* 11.0592 MHz dedicated clk  */  


/*
 * Initial values for UIC interrupt controller polarity, triggering, and
 * critical vs. non-critical.
 */

/*
 * Interrupt Polarity
 *   UIC0 - All active high except EXT IRQ 0-6, bits 23-29
 *   UIC1 - All active high except EXT IRQ 8-11, bits 19-22
 *   UIC2 - All active high except ?
 *   UICB - All active high
 */
#define UIC0_INTR_POLARITY  0xfffffe03
#define UIC1_INTR_POLARITY  0xffffe1ff
#define UIC2_INTR_POLARITY  0xffffffff
#define UICB_INTR_POLARITY  0xffffffff

/*
 * Interrupt Triggering
 *   UICO - All are level, except PCI MSI 0-2 bits 7-9, EXT IRQ 5 bit 28
 *   UIC1 - All are level, except PCI MSI 3-11 bits 8-16
 *   UIC2 - All are level, except PCI MSI 12-15 bits 13-16,
 *                                PCIV/L2C/ETH2/3P bits 22-25
 *   UICB - All are level
 */
#define UIC0_INTR_TRIGGER   0x01c00008
#define UIC1_INTR_TRIGGER   0x00ff8000
#define UIC2_INTR_TRIGGER   0x000f83c0
#define UICB_INTR_TRIGGER   0x00000000

/*
 * Critical Interrupts
 *   UIC0 - All are non-critical, except SMI pushbutton
 *   UIC1 - All are non-critical
 *   UIC2 - All are non-critical
 *   UICB - All are non-critical, except cascade bits 0,2,4
 */
#define UIC0_INTR_CRITICAL  0x00000008
#define UIC1_INTR_CRITICAL  0x00000000
#define UIC2_INTR_CRITICAL  0x00000000
#define UICB_INTR_CRITICAL  0xA8000000

/*
 * Base cascaded interrupts
 */
#define UICB_INTR_UIC0_CRIT  0x80000000
#define UICB_INTR_UIC0_NCRIT 0x40000000
#define UICB_INTR_UIC1_CRIT  0x20000000
#define UICB_INTR_UIC1_NCRIT 0x10000000
#define UICB_INTR_UIC2_CRIT  0x08000000
#define UICB_INTR_UIC2_NCRIT 0x04000000

/*
 * EVB440GX board external IRQ assignments, or how the UIC external interrupt
 * pins are actually used on the EVB440GX board.
 */
#define INT_VEC_PCI_SLOT0  INT_VEC_EXT_IRQ_0        /* J23 */
#define INT_VEC_PCI_SLOT1  INT_VEC_EXT_IRQ_1        /* J8  */
#define INT_VEC_PCI_SLOT2  INT_VEC_EXT_IRQ_2        /* J10 */
#define INT_VEC_PCI_SLOT3  INT_VEC_EXT_IRQ_3        /* J12 */

#define INT_LVL_PCI_SLOT0  INT_LVL_EXT_IRQ_0        /* J23 */
#define INT_LVL_PCI_SLOT1  INT_LVL_EXT_IRQ_1        /* J8 */
#define INT_LVL_PCI_SLOT2  INT_LVL_EXT_IRQ_2        /* J10 */
#define INT_LVL_PCI_SLOT3  INT_LVL_EXT_IRQ_3        /* J12 */


/*
 * MMU
 */


/*
 * PCI
 */

#ifdef INCLUDE_PCI

/*
 * PCI Interrupts
 *
 * The following establishes the relationship between the PCI device numbers
 * (Bus 0) and the pins on the interrupt controller (UIC) the PCI devices are
 * attached to.  During PCI device configuration, the interrupt vector will
 * be written to the interrupt line config register in each PCI device for
 * later use by device drivers or device driver initialization routines.
 *
 *   IDSEL 17 --> device 1 --> PCI Slot 0   (J23)
 *   IDSEL 18 --> device 2 --> PCI Slot 1   (J8)
 *   IDSEL 19 --> device 3 --> PCI Slot 2   (J10)
 *   IDSEL 20 --> device 4 --> PCI Slot 3   (J12)
 *
 * PCI Device #    UIC interrupt          Comments
 */

#define DEV0_INT   0xFF                /* Device 0 40x PCI bridge - no int    */
#define DEV1_INT   INT_VEC_PCI_SLOT0   /* Device 1 PCI slot 0                 */
#define DEV2_INT   INT_VEC_PCI_SLOT1   /* Device 2 PCI slot 1                 */
#define DEV3_INT   INT_VEC_PCI_SLOT2   /* Device 3 PCI slot 2                 */
#define DEV4_INT   INT_VEC_PCI_SLOT3   /* Device 4 PCI slot 3                 */
#define DEV5_INT   0xFF                /* Device 5 unused                     */
#define DEV6_INT   0xFF                /* Device 6 unused                     */
#define DEV7_INT   0xFF                /* Device 7 unused                     */
#define DEV8_INT   0xFF                /* Device 8 unused                     */
#define DEV9_INT   0xFF                /* Device 9 unused                     */

    /* Translate PCI addresses to virtual addresses (master windows) */

#   define PCI_MEM2LOCAL(x) \
	((void *)((UINT)(x) - PCI_MSTR_MEM_BUS + PCI_MSTR_MEM_LOCAL))

#   define PCI_IO2LOCAL(x) \
	((void *)((UINT)(x) - PCI_MSTR_IO_BUS + PCI_MSTR_IO_LOCAL))

#   define PCI_MEMIO2LOCAL(x) \
	((void *)((UINT)(x) - PCI_MSTR_MEMIO_BUS + PCI_MSTR_MEMIO_LOCAL))

    /* Translate local memory address to PCI address (slave window) */

#   define LOCAL2PCI_MEM(x) \
	((void *)((UINT)(x) - PCI_SLV_MEM_LOCAL + PCI_SLV_MEM_BUS))

#endif /* INCLUDE_PCI */

/*
 * The Allied Telesyn AT-2700TX card contains the AMD Am79C972 controller
 * (PCnet-FAST+).  Below is the information for the PCI vendor and device
 * IDs for the AMD Am79C97x family and registers where the MAC address can be
 * found after it is loaded from the serial EPROM on the PCI card.
 */
#ifdef INCLUDE_END
# ifdef INCLUDE_LN97XEND
#   define PCI_VENDOR_ID_AMD     0x1022
#   define PCI_DEVICE_ID_79C97X  0x2000

#   define APROM01               0           /* EEPROM registers */
#   define APROM23               2
#   define APROM45               4
#   define APROM_SIZE            16
# endif /* INCLUDE_LN97XEND */
#endif /* INCLUDE_END */


/*
 * Program Address at which PPC440GX peripherals are mapped
 */

#define PERIPH_BASE_ADDR	0xE0000000
#define EXT_PERIPH_BASE_ADDR	(PERIPH_BASE_ADDR + 0x08000000)

/*
 * Address at which PCI outbound memory is mapped.
 *
 * The follow region can be used to create 3 separate PCI outbound
 * memory windows with different attributes.  The windows are defined
 * using the POM0-P0M3 registers.
 *
 * To conserve MMU page table space, PCI_MEMORY_MAP_END can be defined
 * to an address less than PCI_MEMORY_END.
 */

#define PCI_MEMORY_START	0x90000000
#define PCI_MEMORY_END		0xAFFFFFFF
#define PCI_MEMORY_MAP_END	0xAFFFFFFF

/*
 * Program Address of the PCIX0 controller register & I/O space
 */

#define PCIX0_BASE_ADDR		0xD0000000

/*
 * Program Address of the Onboard Bootrom & Flash space
 */

#define BOOT_BASE_ADDR		0xF0000000

/*
 * External Bus Controller address space.
 *
 * This is not mapped as a whole; instead, specific devices in it are
 * mapped (see below).
 */

#undef EBC0_MAPPED		/* define to create VM map for EBC0 space */
#define EBC0_BASE_ADRS		0x40000000 /* VADDR of EBC0 mapped area */
#define EBC0_SIZE		0x40000000 /* size of EBC0 mapped area */


/*
 * Dallas Semiconductor DS1743P 8KB NVRAM with Real-Time Clock definitions
 *
 * The first 1KB of the NVRAM is reserved for IBM Eval kit software use.
 * The last three bytes of the EMAC Ethernet hardware address are stored
 * after the boot information (offset 1KB + 256). The first three bytes
 * of this address are assumed to be 0x0004ac.
 * The NVRAM is located at physical address 0x1.4800.0000.
 */

#define SYS_FLASH_TYPE       FLASH_29GL256N
#define FLASH_WIDTH          4
#define FLASH_CHIP_WIDTH     2

#define FLASH_SIZE              0x04000000  /* size of FLASH 64M*/


#define FLASH_ADRS           0xFC000000


#define FLASH_SECTOR_SIZE    0x40000     /* sector size of S29GL128N */

#define NV_RAM_ADRS             0xffe00000
#define NV_RAM_INTRVL           1
#define NV_RAM_SIZE             256*1024 /*(8*1024) - 8 */ /* 8KB - 8 bytes for the RTC    */

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0x0    /* Offset of 1KB                */
#define NV_ENET_OFFSET_0        0x10000         /* Offset of EMAC HW adrs       */
                                              /* from the boot offset         */
#define NV_ENET_OFFSET_1        0x10006         /* Offset of 2nd EMAC HW adrs   */
                                              /* from the boot offset         */
#define NV_ENET_OFFSET_2        0x1000c         /* Offset of 3rd EMAC HW adrs   */
                                              /* from the boot offset         */
#define NV_ENET_OFFSET_3        0x10012         /* Offset of 4th EMAC HW adrs   */
                                              /* from the boot offset         */

/*-----------------------------------------------------------------------
 * Base addresses -- Note these are effective addresses where the
 * actual resources get mapped (not physical addresses)
 *----------------------------------------------------------------------*/

#define CFG_FLASH_BASE	    0xfc000000	    /* start of FLASH		*/

#define CFG_PCI_MEMBASE	    0x80000000	    /* mapped pci memory	*/
#define CFG_PERIPHERAL_BASE 0xe0000000	    /* internal peripherals	*/
#define CFG_ISRAM_BASE	    0xc0000000	    /* internal SRAM		*/
#define CFG_PCI_BASE	    0xd0000000	    /* internal PCI regs	*/

#define CFG_EBC0_FLASH_BASE	CFG_FLASH_BASE
#define CFG_EBC1_FPGA_BASE	(CFG_PERIPHERAL_BASE + 0x01000000)
#define CFG_EBC2_LCM_BASE	(CFG_PERIPHERAL_BASE + 0x02000000)
#define CFG_EBC3_CONN_BASE	(CFG_PERIPHERAL_BASE + 0x08000000)

#define TAISHAN_FPGA_ADDR_PH		0x41000000
#define TAISHAN_LCM_ADDR_PH		0x42000000
#define TAISHAN_CON_ADDR_PH     0x48000000

#define TAISHAN_FPGA_SIZE     0x00100000
#define TAISHAN_LCM_SIZE      0x00100000
#define TAISHAN_CON_SIZE      0x08000000  

#define CFG_GPIO_BASE		(CFG_PERIPHERAL_BASE + 0x00000700)

#define BOOTSTRAP_IIC_ADDR 0x50
#define TEMPCHIP_IIC_ADDR  0x48    

/* default mac address */

#define ENET_DEFAULT0 0x00
#define ENET_DEFAULT1 0xA0
#define ENET_DEFAULT2 0x1E

/*
 * Ethernet PHY support
 */
#include "phyLib.h"

/*
 * TBI interface definition
 *
 * Define this if connecting the TBI interface to PHY using a copper
 * interface.
 * Undef this variable if using special daughter card and enabling serial
 * connection over fiber interface using the EMAC GPCS
 */
#define ENET_TBI_COPPER_PHY

/*
 * Ethernet group definitions.
 *
 * Several configuration combinations are provided for the four on-chip
 * ethernets
 *
 *  Group0: MII
 *  Group1: RMII0,RMII1
 *  Group2: SMII0,SMII1,SMII2,SMII3
 *  Group3: RMII0,RGMII2|RTBI2
 *  Group4: SMII0,SMII1,RGMII2|RTBI2,RGMII3|RTBI3
 *  Group4: SMII0,SMII1,GMII2/TBI2|DISABLE3
 *  Group5: SMII0,SMII1,SMII2,RGMII3|RTBI3,DMA
 *  Group6: SMII0,SMII1,RGMII2|RTBI2
 *
 *  With the exception of group 6, all pins are muxed with the DMA
 *  channels 2 and 3 and CPU trace pins.
 */

/*
 * Define ONE of the following Ethernet Group configurations:
 */
#undef  ENET_GROUP0_E0_MII           /* Group 0 - EMAC0 MII                */
#undef  ENET_GROUP0_E1_MII           /* Group 0 - EMAC1 MII                */
#undef  ENET_GROUP1                  /* Group 1 - RMII                     */
#undef  ENET_GROUP2                  /* Group 2 - SMII                     */
#undef  ENET_GROUP3_E2RGMII          /* Group 3 - EMAC2 RGMII              */
#undef  ENET_GROUP3_E2RTBI           /* Group 3 - EMAC2 RTBI               */
#define ENET_GROUP4_E2RGMII_E3RGMII  /* Group 4 - EMAC2 RGMII, EMAC3 RGMII */
#undef  ENET_GROUP4_E2RTBI_E3RGMII   /* Group 4 - EMAC2 RTBI , EMAC3 RGMII */
#undef  ENET_GROUP4_E2RGMII_E3RTBI   /* Group 4 - EMAC2 RGMII, EMAC3 RTBI  */
#undef  ENET_GROUP4_E2RTBI_E3RTBI    /* Group 4 - EMAC2 RTBI , EMAC3 RTBI  */
#undef  ENET_GROUP4_E2GMII_E3DIS     /* Group 4 - EMAC2 GMII , EMAC3 DIS   */
#undef  ENET_GROUP4_E2TBI_E3DIS      /* Group 4 - EMAC2 TBI  , EMAC3 DIS   */
#undef  ENET_GROUP4_E2DIS_E3GMII     /* Group 4 - EMAC2 DIS  , EMAC3 GMII  */
#undef  ENET_GROUP4_E2DIS_E3TBI      /* Group 4 - EMAC2 DIS  , EMAC3 TBI   */
#undef  ENET_GROUP5_E3RGMII          /* Group 5 - EMAC3 RGMII              */
#undef  ENET_GROUP5_E3RTBI           /* Group 5 - EMAC3 RTBI               */
#undef  ENET_GROUP6_E2RGMII_E3DIS    /* Group 6 - EMAC2 RGMII              */
#undef  ENET_GROUP6_E2RTBI_E3DIS     /* Group 6 - EMAC2 RTBI               */

/* The Ethernet Group and PHY Mode mode params are automatically set based
 * on the Ethernet Group Configuration defined above
 */
#undef ENET_GROUP
#undef EMAC2_PHY_MODE
#undef EMAC3_PHY_MODE
#undef EMAC0_ENABLE
#undef EMAC1_ENABLE
#undef EMAC2_ENABLE
#undef EMAC3_ENABLE
#if   defined(ENET_GROUP0_E0_MII)
   #define ENET_GROUP 0
   #define EMAC2_PHY_MODE PHY_MODE_DISABLED
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
#elif defined(ENET_GROUP0_E1_MII)
   #define ENET_GROUP 0
   #define EMAC2_PHY_MODE PHY_MODE_DISABLED
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC1_ENABLE
#elif defined(ENET_GROUP1)
   #define ENET_GROUP 1
   #define EMAC2_PHY_MODE PHY_MODE_DISABLED
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
#elif defined(ENET_GROUP2)
   #define ENET_GROUP 2
   #define EMAC2_PHY_MODE PHY_MODE_SMII
   #define EMAC3_PHY_MODE PHY_MODE_SMII
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP3_E2RGMII)
   #define ENET_GROUP 3
   #define EMAC2_PHY_MODE PHY_MODE_RGMII
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP3_E2RTBI)
   #define ENET_GROUP 3
   #define EMAC2_PHY_MODE PHY_MODE_RTBI
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP4_E2RGMII_E3RGMII)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_RGMII
   #define EMAC3_PHY_MODE PHY_MODE_RGMII
   #undef  EMAC0_ENABLE
   #undef  EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP4_E2RTBI_E3RGMII)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_RTBI
   #define EMAC3_PHY_MODE PHY_MODE_RGMII
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP4_E2RGMII_E3RTBI)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_RGMII
   #define EMAC3_PHY_MODE PHY_MODE_RTBI
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP4_E2RTBI_E3RTBI)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_RTBI
   #define EMAC3_PHY_MODE PHY_MODE_RTBI
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP4_E2GMII_E3DIS)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_GMII
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP4_E2TBI_E3DIS)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_TBI
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP4_E2DIS_E3GMII)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_DISABLED
   #define EMAC3_PHY_MODE PHY_MODE_GMII
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP4_E2DIS_E3TBI)
   #define ENET_GROUP 4
   #define EMAC2_PHY_MODE PHY_MODE_DISABLED
   #define EMAC3_PHY_MODE PHY_MODE_TBI
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP5_E3RGMII)
   #define ENET_GROUP 5
   #define EMAC2_PHY_MODE PHY_MODE_SMII
   #define EMAC3_PHY_MODE PHY_MODE_RGMII
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP5_E3RTBI)
   #define ENET_GROUP 5
   #define EMAC2_PHY_MODE PHY_MODE_SMII
   #define EMAC3_PHY_MODE PHY_MODE_RTBI
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
   #define EMAC3_ENABLE
#elif defined(ENET_GROUP6_E2RGMII_E3DIS)
   #define ENET_GROUP 6
   #define EMAC2_PHY_MODE PHY_MODE_RGMII
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#elif defined(ENET_GROUP6_E2RTBI_E3DIS)
   #define ENET_GROUP 6
   #define EMAC2_PHY_MODE PHY_MODE_RTBI
   #define EMAC3_PHY_MODE PHY_MODE_DISABLED
   #define EMAC0_ENABLE
   #define EMAC1_ENABLE
   #define EMAC2_ENABLE
#endif

/*
 *
 * TLB Entry Numbers used for specific purposes, and size of TLB-mapped
 * SDRAM; coordinate with the resetEntry and sysStaticTlbDesc[] arrays.
 */

#define TLB_NUM_BOOT_SPACE	3	/* Boot Space mapped via TLB #3 */
#ifdef INCLUDE_440X5_TLB_RECOVERY_MAX
#define TLB_NUM_SDRAM_0		1	/* SDRAM 0x0... mapped via TLB 1 */
#else /* INCLUDE_440X5_TLB_RECOVERY_MAX */
#define TLB_NUM_SDRAM_0		0	/* SDRAM 0x0... mapped via TLB 1 */
#endif /* INCLUDE_440X5_TLB_RECOVERY_MAX */

#define MAX_NUM_DIMMS		2	/* max # of DIMMS on board */ 
#define MAPPED_SDRAM_SIZE	SDRAM_MAX_SIZE	/* TLBs map all 2GB  */

 /*
 * There is 256KB of SRAM on the chip itself. It can be used as
 * conventional SRAM, or as L2 cache.
 *
 * In this BSP, for conventional SRAM use it is statically mapped
 * via TLB to effective address 0xc0000000.
 */

#define SRAM0_BASE_ADDR   0xC0000000

/*
 * There is an I2O messaging register space on the 440GX chip.
 *
 * In this BSP, it is statically mapped via TLB to effective address
 * 0xbffff000, and there are 64KB of registers there.
 */
#define I2O_BASE_ADRS	  0xbfff0000


/*
 * The EVB440GX board has two sockets for 184 pin DDR SDRAM DIMMs.
 * The following are offsets in the SDRAM DIMM's presence-detect serial
 * EEPROM. Values at these offsets are accessed via the I2C
 * interface, and are used to help configure the DDR SDRAM controller
 * correctly for the properties of the DIMM.
 */
#define SDRAM_NUM_ROWS	   3
#define SDRAM_NUM_COLS	   4
#define SDRAM_NUM_BANKS    5
#define SDRAM_WIDTH	   6
#define SDRAM_NUM_INTBANKS 17
#define SDRAM_MOD_DENSITY  31

/*
 * I2C device information
 */

#define IIC0_BOOTPROM0_ADDR	0xA0
#define IIC0_BOOTPROM1_ADDR	0xA8
#define IIC0_SPD_DIMM0_ADRS	0xA6
#define IIC0_SPD_DIMM1_ADRS	0xA4
#define IIC0_CDCV850_ADRS	0xD2

/*
 * CDCV850 CMOS to SSTL differential clock driver chip
 */

#define BYTE0_Y0_Y0N		0x80	/* used for DIMM0 */
#define BYTE0_Y1_Y1N		0x40	/* not used */
#define BYTE0_Y2_Y2N		0x20	/* used for DIMM0 */
#define BYTE0_Y3_Y3N		0x10	/* not used */
#define BYTE0_Y4_Y4N		0x08	/* used for DIMM0 */
#define BYTE0_Y5_Y5N		0x04	/* used for DIMM1 */
#define BYTE0_Y6_Y6N		0x02	/* not used */
#define BYTE0_Y7_Y7N		0x01	/* used for DIMM1 */
#define BYTE1_Y8_Y8N		0x80	/* not used */
#define BYTE1_Y9_Y9N		0x40	/* used for DIMM1 */

/*
 * Support for bus error identification functions, e.g. sysBusErrRegsGet
 */

#define	SYS_BUS_ERR_PLB_ID	1
#define	SYS_BUS_ERR_POB_ID	2

/* summary information for SYS_INFO flags field */

#define SI_PLL_CPU_CLOCK	0x00000001
#define SI_PLL_EBC_CLOCK	0x00000002
#define SI_PLL_LOCAL_A		0x00000004
#define SI_PLL_LOCAL_B		0x00000008
#define SI_ARB_INTERNAL		0x00000010
#define SI_ARB_EXTERNAL		0x00000020

#define INIT_CPR_PLLC		0x4000033c
#define INIT_CPR_PLLD	    0x0c010502	
#define INIT_CPR_PRIMBD		0x01000000
#define INIT_CPR_OPBD		0x02000000
#define INIT_CPR_PERD		0x01000000
#define INIT_CPR_MALD		0x01000000

#define INIT_SDR_EBC		0x20000000	/* 32bit ROM*/
#define INIT_SDR_CP440		0x1eaaea02
#define INIT_SDR_XCR		0xe0880000
#define INIT_SDR_PFC0		0x00303f00
#define INIT_SDR_PFC1		0x01100000
#define INIT_SDR_CUST0		0x00000000
#define INIT_SDR_CUST1		0x00000000
 

#ifndef _ASMLANGUAGE

IMPORT int sysTimerClkFreq;

/*
 * System information structure.  It is used to get information about the
 * PLLs and operating frequencies.
 */

typedef struct
    {
    UINT32  pllFwdDivA;
    UINT32  pllFwdDivB;
    UINT32  pllFbkDiv;
    UINT32  pllPlbDiv;
    UINT32  pllPciDiv;
    UINT32  pllExtBusDiv;
    UINT32  pllOpbDiv;
    UINT32  freqSys;                /* clock supplied to sysClk pin */
    UINT32  freqVCO;
    UINT32  freqProcessor;
    UINT32  freqPLB;
    UINT32  freqOPB;
    UINT32  freqEBC;
    UINT32  freqMAL;
    UINT32  freqTMR;                /* timer clock */
    UINT32  freqPCI;
    UINT32  pciIntArbEn;	    /* Internal PCI arbiter is enabled */
    UINT32  pciClkSync;             /* PCI clock is synchronous        */
    UINT32  flags;		    /* summary information */
    } SYS_INFO;

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif	/* INCibmEvb440gxh */
