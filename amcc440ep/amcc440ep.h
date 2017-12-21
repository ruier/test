
/* amcc440ep.h - AMCC Yosemite 440EP eval board header */

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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* 
 * Copyright 2006, 2008 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

********************************************************************************
\NOMANUAL
*/


/*
modification history
--------------------
01e,20aug08,d_l break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01d,16mar06,mmi Changed PHY0, PH1 ADRS 
01d,16mar06,mmi Changed default first 3 bytes of MAC addr to wind rivers's 
01c,15mar06,mmi Mods to add M command 
01b,09mar06,mmi mods IIC0 offsets, amcc440ep_bcsr_reg fields, file hearder
                file header, copyrights date, reference to name bamboo 	 
01a,01feb06,mmi Created based on bamboo.h version 01f 
*/

/*
This file contains I/O addresses and related constants for the AMCC amcc440ep 
BSP.  amcc440ep  has the PowerPC 440EP embedded controller on it.
*/

#ifndef INCamcc440eph
#define INCamcc440eph

#ifdef __cplusplus
    extern "C" {
#endif

#include "ppc440EP.h"

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


#define BUS   NONE                  /* no off-board bus interface    */
#define N_SIO_CHANNELS  NUM_TTY     /* Number of serial I/O channels */

/*
 * Minimum and maximum system clock rates
 */
#define SYS_CLK_RATE_MIN  3         /* minimum system clock rate     */
#define SYS_CLK_RATE_MAX  5000      /* maximum system clock rate     */

/*
 * system clocks
 */
#define BSP_SYSTEM_CLK             (66666666)

/*
 * The board supplies a 11.0592 MHz clock to the external serial clock
 * pin of the 440EP (UARTSerClk).
 */

/* 11.0592 MHz dedicated clk  */

#define EXT_SER_CLK_FREQ             11059200           

/* External peripherals */

#define EBC0_VIRT_START              0xD0000000
#define EBC0_CPLD_OFF                0x2000  /* CPLD   */

/* Physical addresses */

/* DDR SRAM */
#define DDR_PHY_START_LOW          SDRAM0_PHY_START_LOW
#define DDR_PHY_START_HIGH         SDRAM0_PHY_START_HIGH

/* DDR SRAM (cached) */

#define DDR_CA_PHY_START_LOW       SDRAM0_PHY_START_LOW
#define DDR_CA_PHY_START_HIGH      SDRAM0_PHY_START_HIGH
#define DDR_CA_VIRT_START          0
#define DDR_CA_TLB                 0

/* DDR SRAM (uncached) */

#define DDR_UA_PHY_START_LOW       SDRAM0_PHY_START_LOW
#define DDR_UA_PHY_START_HIGH      SDRAM0_PHY_START_HIGH
#define DDR_UA_VIRT_START          0x40000000
#define DDR_UA_TLB                 1

/* USB OP Arbiter    a.k.a. OPBA1 */

#define OPBA1_VIRT_START    0xC9000000
#define OPBA1_PR_VIRT_ADRS  (OPBA1_VIRT_START + OPBA1_PR_OFF) /* Priority reg */
#define OPBA1_CR_VIRT_ADRS  (OPBA1_VIRT_START + OPBA1_CR_OFF) /* Control reg  */
#define OPBA1_TLB           16

#define OPB4A_BASE              (OPBA1_VIRT_START) 
#define OPB4A_PR                (OPBA1_PR_VIRT_ADRS)
#define OPB4A_CR                (OPBA1_CR_VIRT_ADRS)

/* USB 2.0 Device */

#define USB2_PHY_START_LOW      USB2D0_PHY_START_LOW    
#define USB2_PHY_START_HIGH     USB2D0_PHY_START_HIGH
#define USB20_DEV_BASE          0xC9000100

/* CPLD */

#define CPLD_PHY_START_LOW      (EBC0_PHY_START_ADRS_LOW + 0x2000)
#define CPLD_PHY_START_HIGH     (EBC0_PHY_START_ADRS_HIGH)
#define CPLD_VIRT_START         (EBC0_VIRT_START + EBC0_CPLD_OFF)
#define CPLD_SIZE               (0x1F) 
#define CPLD_SIZE_ALIGNED       0x1000

/* PCI Memory */

#define PCIM0_VIRT_START         0x80000000
#define PCIM0A_PHY_START_LOW     (PCIM0_PHY_START_LOW)
#define PCIM0A_PHY_START_HIGH    (PCIM0_PHY_START_HIGH)
#define PCIM0A_VIRT_START        PCIM0_VIRT_START

/* #define PCI_MEMORY_START         PCIM0A_VIRT_START */

#define PCIM0A_TLB               6
#define PCIM0B_PHY_START_LOW     (PCIM0A_PHY_START_LOW + 0x10000000)
#define PCIM0B_PHY_START_HIGH    (PCIM0A_PHY_START_HIGH)
#define PCIM0B_VIRT_START        (PCIM0A_VIRT_START + 0x10000000)

/* #define PCI_MEMORY2_START         PCIM0B_VIRT_START */

#define PCIM0B_TLB               7
#define PCIM0C_PHY_START_LOW     (PCIM0B_PHY_START_LOW + 0x10000000)
#define PCIM0C_PHY_START_HIGH    (PCIM0B_PHY_START_HIGH) 
#define PCIM0C_VIRT_START        (PCIM0B_VIRT_START + 0x10000000)
#define PCIM0C_TLB               8
#define PCIM0D_PHY_START_LOW     (PCIM0C_PHY_START_LOW + 0x10000000)
#define PCIM0D_PHY_START_HIGH    (PCIM0C_PHY_START_HIGH) 
#define PCIM0D_VIRT_START        (PCIM0C_VIRT_START + 0x10000000)
#define PCIM0D_TLB               9

#define PCI_MEMORY1_MAP_SIZE     0x02000000  /* 32 MB mapped in MMU */
#define PCI_MEMORY2_MAP_SIZE     0x02000000  /* 32 MB mapped in MMU */

/* PCI IO */

#define PCIIO0_VIRT_START                   0xC1000000
#define PCIIO0_TLB                          17
#define PLB_PCI_IO_REGION_1_START_HIGH      0    /* PLB side */
#define PLB_PCI_IO_REGION_1_START_LOW       PCIIO0_VIRT_START                
#define PLB_PCI_IO_REGION_1_END             (PCIIO0_VIRT_START \
                                             + PLB_PCI_IO_REGION_1_SIZE - 1)

#define PCIIO1_VIRT_START                   (PCIIO0_VIRT_START + 0x10000000)
#define PCIIO1_TLB                          18
#define PLB_PCI_IO_REGION_2_START_HIGH      0   /* PLB side */
#define PLB_PCI_IO_REGION_2_START_LOW       (PCIIO0_VIRT_START+0x01800000)
#define PLB_PCI_IO_REGION_2_END             (PLB_PCI_IO_REGION_2_START_LOW \
                                             + (PLB_PCI_IO_REGION_2_SIZE - 1))
/* PCI Extra IO */

#define PCIIO1A_PHY_START_HIGH      PCIIO1_PHY_START_HIGH
#define PCIIO1A_PHY_START_LOW       0xE8800000
#define PCIIO1A_VIRT_START          0xC2800000
#define PCIIO1A_TLB                 18
#define PCIIO1B_PHY_START_HIGH      PCIIO1A_PHY_START_HIGH  
#define PCIIO1B_PHY_START_LOW       (PCIIO1A_PHY_START_LOW  + 0x1000000)
#define PCIIO1B_VIRT_START          (PCIIO1A_VIRT_START + 0x1000000)
#define PCIIO1B_TLB                 19
#define PCIIO1C_PHY_START_HIGH      PCIIO1B_PHY_START_HIGH  
#define PCIIO1C_PHY_START_LOW       (PCIIO1B_PHY_START_LOW  + 0x1000000)
#define PCIIO1C_VIRT_START          (PCIIO1B_VIRT_START + 0x1000000)
#define PCIIO1C_TLB                 20
#define PCIIO1D_PHY_START_HIGH      PCIIO1C_PHY_START_HIGH  
#define PCIIO1D_PHY_START_LOW       (PCIIO1C_PHY_START_LOW + 0x1000000)
#define PCIIO1D_VIRT_START          (PCIIO1C_VIRT_START + 0x1000000)
#define PCIIO1D_TLB                  21

/* PCI Configuration registers */

#define PCIC0_VIRT_START            (PCIIO0_VIRT_START + PCIC0_CFGADDR_OFF)
#define PCIC0_CFGADDR               PCIC0_VIRT_START
#define PCI_CFGADDR                 PCIC0_CFGADDR
#define PCIC0_CFGDATA               (PCIIO0_VIRT_START + PCIC0_CFGDATA_OFF)
#define PCI_CFGDATA                 PCIC0_CFGDATA
#define PP_BRIDGE_CR                (PCIIO0_VIRT_START + PP_BRIDGE_CR_OFF)
#define PCIC0_TLB                   22

/* PCI interrup ack / Special Cycle */

#define PCIC0_IASC0_VIRT_START      (PCIIO0_VIRT_START + PCIC0_IASC0_OFF)
#define PCIC0_IASC0_TLB             23
#define PCIC0_IASC0                 PCIC0_IASC0_VIRT_START

/* PCI Local Conf Register */

#define PCIL0_VIRT_START         0xC8400000
#define PCIL0_TLB                24

/* Boot space EBC */

#define BS_VIRT_START            0xF0000000
#define BS_TLB                   3

/* Boot Space Flash */

#define BSF_PHY_START_HIGH       BS_PHY_START_HIGH
#define BSF_PHY_START_LOW        (BS_PHY_START_LOW + 0xFF00000)
#define BSF_VIRT_START           (BS_VIRT_START + 0xFF00000)
#define BSF_SIZE                 0x100000

/* On-Chip Peripherals off OPB3 */

#define OCP_VIRT_START          0xC0600000         

/* General Purpose Timer 0 registers */

#define GPT0_VIRT_START          (OCP_VIRT_START + OCP_GPT0_OFF)
#define GPT0_BASE                GPT0_VIRT_START

/* UARTs registers */

#define UART0_VIRT_START         (OCP_VIRT_START + OCP_UART0_OFF)
#define UART0_BASE               UART0_VIRT_START
#define UART1_VIRT_START         (OCP_VIRT_START + OCP_UART1_OFF)
#define UART1_BASE               UART1_VIRT_START
#define UART2_VIRT_START         (OCP_VIRT_START + OCP_UART2_OFF)
#define UART2_BASE               UART2_VIRT_START
#define UART3_VIRT_START         (OCP_VIRT_START + OCP_UART3_OFF)
#define UART3_BASE               UART3_VIRT_START

/* IIC registers */

#define IIC0_VIRT_START          (OCP_VIRT_START + OCP_IIC0_OFF)
#define IIC0_BASE                IIC0_VIRT_START
#define IIC1_VIRT_START          (OCP_VIRT_START + OCP_IIC1_OFF)
#define IIC1_BASE                IIC1_VIRT_START
 
/* SPI registers */

#define SPI0_VIRT_START     (OCP_VIRT_START + OCP_SPI0_OFF)
#define SPI0_BASE           SPI0_VIRT_START
#define SPI0_SPMODE         (SPI0_BASE + SPI0_SPMODE_OFF)  /* R/W Loop            */
#define SPI0_RXDATA         (SPI0_BASE + SPI0_RXDATA_OFF)  /* R   Received data   */
#define SPI0_TXDATA         (SPI0_BASE + SPI0_TXDATA_OFF)  /* R/W Transmit data   */
#define SPI0_SPCOM          (SPI0_BASE + SPI0_SPCOM_OFF)   /* R/W STR             */
#define SPI0_STATUS         (SPI0_BASE + SPI0_STATUS_OFF)  /* R   Recd byte ready */
#define SPI0_CDM            (SPI0_BASE + SPI0_CDM_OFF)     /* R/W Clk Div Modulus */


#define OPBA0_VIRT_START    (OCP_VIRT_START + OCP_OPBA0_OFF)
#define OPBA0_PR_VIRT_ADRS  (OPBA0_VIRT_START + OPBA0_PR_OFFSET) /* Priority reg */
#define OPBA0_CR_VIRT_ADRS  (OPBA0_VIRT_START + OPBA0_CR_OFFSET) /* Control reg  */


#define OPB3A_BASE          (OPBA0_VIRT_START)
#define OPB3A_PR            (OPBA0_PR_VIRT_ADRS)    /* OPB3 Arbiter Priority Reg   */
#define OPB3A_CR            (OPBA0_CR_VIRT_ADRS)    /* OPB3 Aribter Control Reg    */


/* GPIO registers */

#define GPIO0_VIRT_START        (OCP_VIRT_START + OCP_GPIO0_OFF) 
#define GPIO0_BASE              GPIO0_VIRT_START
#define GPIO1_VIRT_START        (OCP_VIRT_START + OCP_GPIO1_OFF)
#define GPIO1_BASE              GPIO1_VIRT_START

/* EMAC registers */

#define ZMII0_VIRT_START        (OCP_VIRT_START + OCP_ZMII0_OFF)
#define ZMII0_BASE              ZMII0_VIRT_START
#define EMAC0_VIRT_START        (OCP_VIRT_START + OCP_EMAC0_OFF)
#define EMAC0_BASE              EMAC0_VIRT_START
#define EMAC1_VIRT_START        (OCP_VIRT_START + OCP_EMAC1_OFF)
#define EMAC1_BASE              EMAC1_VIRT_START


/* USB 1.1 Host controller */

#define USBH0_VIRT_START        (OCP_VIRT_START + OCP_USBH0_OFF)
#define USB11_HOST_BASE         USBH0_VIRT_START

/* Define USB 1.1 Host controller registers not compliant with OHCI */

#define USB11_HOST_OPBMC        USB11_HOST_BASE+0x070
#define     OPBMC_MAXACK_MASK   0x0000001F
#define     OPBMC_DISABLE       0x80000000

/* Peripheral Bank initialization values */

/*
 *  Flash EBC init values
 */

/*
 *  EBC Expansion connector init values
 *  The following values are only examples and should be changed for custom purposes
 */

 /* Expansion connector Peripheral Bank Access Parameters register value */

 #define EXP_CON_AP_VALUE ( EBC_BXAP_BME | \
                                (7 << TWT_BITSTOSHIFT) | \
                                EBC_BXAP_CSN_1 | EBC_BXAP_OEN_1 | \
                                EBC_BXAP_WBN_1 | EBC_BXAP_WBF_1 | EBC_BXAP_TH_4 |  \
                                EBC_BXAP_BEM_W | EBC_BXAP_SOR | EBC_BXAP_NO_PAR )

 /* Expansion connector Peripheral Bank Configuration Parameters register values 
  * Bank1 
  */

 #define EXP_CON_CR_VALUE1 ( EXP_CON_PHYS_ADRS_LOW1 | \
                                EBC_BXCR_BS_1MB | EBC_BXCR_BU_RW | EBC_BXCR_BW_8 )
 /* Bank2 */

 #define EXP_CON_CR_VALUE2 ( EXP_CON_PHYS_ADRS_LOW2 | \
                                EBC_BXCR_BS_1MB | EBC_BXCR_BU_RW | EBC_BXCR_BW_8 )

 /* Bank3 */

 #define EXP_CON_CR_VALUE3 ( EXP_CON_PHYS_ADRS_LOW3 | \
                                EBC_BXCR_BS_1MB | EBC_BXCR_BU_RW | EBC_BXCR_BW_8 )


/*
 *    EBC Configuration Register init value
 */

#define EBC_CFG_INIT_VALUE (EBC_CFG_ETC | EBC_CFG_RTC_64 | EBC_CFG_CSTC)

/*
 * Interrupt Vector Prefix Register value. Exception vectors will be
 * located at the start of SDRAM.
 */

#define AMCC440EP_IVPR_VAL     0x00000000

/*
 * Initial values for UIC interrupt controller polarity, triggering, and
 * critical vs. non-critical. See corresponding PPC440EP UM and PPC440EP
 * Reference Board UM Chapters.
 */

/*
 * Interrupt Polarity
 *   UIC0 - All active high except PCI SERR,IRQ0(PHY),IRQ1(expansion slot),IRQ2-5(PCI slots),IRQ6(SMI button)
 *   UIC1 - All active high except IRQ7-9 (Ext DMA ChA),USB Dev, ext PCI PERR
 */

#define UIC0_INTR_POLARITY  0xffbffe03
#define UIC1_INTR_POLARITY  0xffffc6ef

/*
 * Interrupt Triggering
 *   UICO - All are level triggered, except IRQ6  (SMI button)
 *   UIC1 - All are level triggered except USB device
 */

#define UIC0_INTR_TRIGGER   0x00000004
#define UIC1_INTR_TRIGGER   0x00000100

/*
 * Critical Interrupts
 *   UIC0 - All are non-critical, except critical UIC1 ints and SMI pushbutton
 *   UIC1 - All are non-critical
 */
#define UIC0_INTR_CRITICAL  0x00000005
#define UIC1_INTR_CRITICAL  0x00000000

/*
 * All on board interrupts should be programmed active low on the PPC440EP/GR
 * IRQ pins. Following is the processor's external interrupts mapping. For 
 * further information on this, refer to the PPC440EP/GR user Manual. 
 * 
 */

#define BOARD_ETH0_IRQ        INT_VEC_EXT_IRQ_0     /* Ethernet0 */
#define BOARD_ETH1_IRQ        INT_VEC_EXT_IRQ_1     /* Ethernet1 */
#define BOARD_PCI_INTA_IRQ    INT_VEC_EXT_IRQ_2     /* PCI slot  */
#define BOARD_STTM_IRQ        INT_VEC_EXT_IRQ_3     /* Thermal sensor alert */
#define BOARD_GPIO44_IRQ      INT_VEC_EXT_IRQ_4     
#define BOARD_GPIO45_IRQ      INT_VEC_EXT_IRQ_5     
#define BOARD_GPIO46_IRQ      INT_VEC_EXT_IRQ_6     
#define BOARD_GPIO47_IRQ      INT_VEC_EXT_IRQ_7     
#define BOARD_GPIO48_IRQ      INT_VEC_EXT_IRQ_8     

/*
 * BCSR control registers.
 * The amcc440ep board has onboard control registers. These registers are 
 * configured as x8 ports.
 */

#define BCSR_BASE            (unsigned char * ) (CPLD_VIRT_START)
#define BCSR0_BOARD_ID       (unsigned char * ) (CPLD_VIRT_START + 0)
#define BCSR1_CPLD_REV       (unsigned char * ) (CPLD_VIRT_START + 1)
#define BCSR2_USR_DIPSW_LEDS (unsigned char * ) (CPLD_VIRT_START + 2)
#define BCSR3_CFG_DIPSW      (unsigned char * ) (CPLD_VIRT_START + 3)
#define BCSR4_CFG_TMRCLK     (unsigned char * ) (CPLD_VIRT_START + 4)
#define BCSR5_PCI_CNTRL      (unsigned char * ) (CPLD_VIRT_START + 5)
#define BCSR6_BOARD_CNTRL    (unsigned char * ) (CPLD_VIRT_START + 6)
#define BCSR7_MEM_STTM       (unsigned char * ) (CPLD_VIRT_START + 7)
#define BCSR8_ETHER_CNTRL    (unsigned char * ) (CPLD_VIRT_START + 8)
#define BCSR9_USB_CNTRL      (unsigned char * ) (CPLD_VIRT_START + 9)
#define BCSR10_TMR_27_24     (unsigned char * ) (CPLD_VIR_START + A)
#define BCSR11_TMR_23_16     (unsigned char * ) (CPLD_VIR_START + B)
#define BCSR12_TMR_15_8      (unsigned char * ) (CPLD_VIR_START + C)
#define BCSR13_TMR_7_0       (unsigned char * ) (CPLD_VIR_START + D)

/* BCSR0 BOARD revision 1 Id */

#define BCSR0_BOARD_ID_REV_1       (0x09)

/* BCSR2 */

#define BCSR2_LED_0      (0x80)
#define BCSR2_LED_1      (0x40)
#define BCSR2_SWITCH_1   (0x08)
#define BCSR2_SWITCH_2   (0x04)
#define BCSR2_SWITCH_3   (0x02)
#define BCSR2_SWITCH_4   (0x01)

/* BCSR3 */

#define BCSR3_CNTRL_SWITCH_0 (0x08)
#define BCSR3_CNTRL_SWITCH_1 (0x04)
#define BCSR3_CNTRL_SWITCH_2 (0x02)
#define BCSR3_CNTRL_SWITCH_4 (0x01)

/* BCSR4 */

#define BCSR4_TMRCLK_SRC_PERCLK    (0x10)
#define BCSR4_TMRCLK_MULT          (0x0F)

/* BCSR5 */

#define BCSR5_PCI_M66EN            (0x80)

/* BCSR6 */

#define BCSR6_BOARD_RST            (0x80)
#define BCSR6_BOARD_POR            (0x01)

/* BCSR7 */

#define BCSR7_FLASH_OP_BUSY        (0x80)
#define BCSR7_FLASH_OP_DONE        (0x40)
#define BCSR7_FLASH_PRT            (0x20)
#define BCSR7_EEPROM_USR_PRT       (0x10)
#define BCSR7_EEPROM_BOOT_PRT      (0x08)
#define BCSR7_FLASH_WRITE_PRT      (0x04)
#define BCSR7_STTM_FLOAT0          (0x00)
#define BCSR7_STTM_GND             (0x01)
#define BCSR7_STTM_VVD             (0x10)
#define BCSR7_STTM_FLOAT1          (0x11)

/* BCSR6 */

#define BCSR8_PHY0_RST             (0x80)
#define BCSR8_PHY0_EN              (0x40)
#define BCSR8_PHY1_RST             (0x20)
#define BCSR8_PHY1_EN              (0x10)


/* BCSR9 */

#define BCSR9_USB20_RST             (0x80)
#define BCSR9_USB20_VCC              (0x40)
#define BCSR9_USB11_LS             (0x20)
#define BCSR9_USB11_VCC              (0x10)

/* BCSR10 */

#define BCSR10_TMR_START            (0x80)
#define BCSR10_TMR_LATCH            (0x40)
#define BCSR10_TMRCLK_INT           (0x20)
#define BCSR10_TMR_CLR_COUNT        (0x10)
#define BCSR10_TMR_27TO24            (0x07)

/* BCSR11 */

#define BCSR10_TMR_23TO16            (0xFF)

/* BCSR12 */

#define BCSR12_TMR_15TO8            (0xFF)

/* BCSR13 */

#define BCSR13_TMR_7TO0            (0xFF)

/*
 * PCI
 */

#ifdef INCLUDE_PCI

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

/*
 * Routines for reading or writing PCI I/O or Memory space. These routines are
 * written in assembler and use byte swapping load/store instructions.
 */

#    define PCI_IN_BYTE(x)       sysPciInByte (x)
#    define PCI_IN_WORD(x)       sysPciInWord (x)
#    define PCI_IN_LONG(x)       sysPciInLong (x)
#    define PCI_OUT_BYTE(x,y)    sysPciOutByte (x,y)
#    define PCI_OUT_WORD(x,y)    sysPciOutWord (x,y)
#    define PCI_OUT_LONG(x,y)    sysPciOutLong (x,y)

#endif /* INCLUDE_PCI */

/*
 * The Allied Telesyn AT-270XTX card contains the AMD Am79C972 controller
 * (PCnet-FAST+).  Below is the information for the PCI vendor and device
 * IDs for the AMD Am79C97x family and registers where the MAC address can be
 * found after it is loaded from the serial EPROM on the PCI card.
 */
#ifdef INCLUDE_NETWORK
# ifdef INCLUDE_LN97XEND
#   define PCI_VENDOR_ID_AMD     0x1022
#   define PCI_DEVICE_ID_79C97X  0x2000

#   define APROM01               0           /* EEPROM registers */
#   define APROM23               2
#   define APROM45               4
#   define APROM_SIZE            16
# endif /* INCLUDE_LN97XEND */
#endif /* INCLUDE_NETWORK */

/*
 * If PIBS is not used, reset the BOOT_line offset to 0x0. Otherwise set it to 
 * 0x400 AND ensure that usrBootLineInit and sysNvRamSet in ../all/bootConfig.c are
 * NOT hardcoded to '0'. 
 */

#define NV_ENET_OFFSET_0        0x100         /* Offset of EMAC HW adrs   */
                                              /* from the boot offset     */
#define NV_ENET_OFFSET_1        0x106         /* Offset of 2nd EMAC HW adrs */
                                              /* from the boot offset       */

/* default mac address */

#define ENET_DEFAULT0 0x00
#define ENET_DEFAULT1 0xA0
#define ENET_DEFAULT2 0x1E

/*
 * Address of the PHY(s) attached to the MII interface of EMAC(s).
 * There is one PHY (AC104Z) chip on the AMCC 440EP board supporting RMII
 * and SMII. Pay particular attention to the corresponding chapter the 440EP
 * Reference Board Manual with respect to strapping. To use an MII PHY, an
 * external PHY must be attached to the AMCC 440EP dedicated connector. 
 * Only one EMAC can be used if an MII PHY is used.
 */

#define EMAC_PHY0_RMII_ADRS   1      /* Assumes default jumper settings     */
#define EMAC_PHY1_RMII_ADRS   0x3    /* Assumes default jumper settings     */

#define MAC_ADRS_LEN 6
#define ETHERNET_MAC_HANDLER

#define WR_ENET0  	    0x08  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1  	    0x00
#define WR_ENET2  	    0x3E

#define CUST_ENET3_0    0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1    0xA1
#define CUST_ENET3_2    0xA2
#define CUST_ENET4 	0xAA
#define CUST_ENET5      0xA0

/* Define arbitrarily addresses for the 3 Banks dedicated to EBC expansion 
 * connector 
 */

#define EXP_CON_PHYS_ADRS_HIGH1  0x00000000  /* Needed for EBC configuration */
#define EXP_CON_PHYS_ADRS_LOW1   0x90000000
#define EXP_CON_PHYS_ADRS_HIGH2  0x00000000
#define EXP_CON_PHYS_ADRS_LOW2   0x90100000
#define EXP_CON_PHYS_ADRS_HIGH3  0x00000000
#define EXP_CON_PHYS_ADRS_LOW3   0x90200000

#define MAPPED_SDRAM_SIZE  (SDRAM_MAX_SIZE/2) /* TLB's map only 1GB */

/*
 * I2C device information
 * The following four devices are accessible through the I2C bus: a serial 
 * temperature and thermal monitor (STTM), a serial EEPROM (SEP) for 
 * boot configuration with 66 MHZ PCI clock, a serial EEPROM (SEP) with 33OA
 * MHZ PCI clock and a secondary serial EEPROM (SEP).
 * The two boot configuration SEP are identical and mapped to the same address
 * IIC0_EEPROM0_ADRS.
 *
 */

#define IIC0_STTM_ADRS         0x90
#define IIC0_BOOTSTRAP_ADR0    0xA4
#define IIC0_BOOTSTRAP_ADR1    0xA6
#define NV_RAM_SIZE            512
#define IIC_NVRAM_PAGE_BLOCK_SIZE 256
#define IIC0_BOOTLINE0_ADRS    0xA8
#define IIC0_ENET_MAC_ADRS     0xAA

#ifndef _ASMLANGUAGE

IMPORT int sysTimerClkFreq;

/*
 * BCSR onboard registers 
 */

typedef struct amcc440ep_bcsr_reg
    {
    unsigned char boardRevId;
    unsigned char cpldCodeRev;
    unsigned char usrConfig;
    unsigned char sysConfig;
    unsigned char tmrclkCntrl;
    unsigned char pciSts;
    unsigned char resetCtrl;
    unsigned char sttmCtrl;
    unsigned char ethCtrl;
    unsigned char usbCtrl;
    unsigned char tmrCnt27_24;
    unsigned char tmrCnt23_16;
    unsigned char tmrCnt15_8;
    unsigned char tmrCnt7_0;
    } AMCC440EP_BCSR_REG;

/*
 * System information structure.  It is used to get information about the
 * 440EP PLLs and operating frequencies.
 */

typedef struct
    {
    UINT32  pllFwdDivA;
    UINT32  pllFwdDivB;
    UINT32  pllFbkDiv;
    UINT32  pllEfbDiv;
    UINT32  pllPlbDiv;
    UINT32  pllCpuDiv;
    UINT32  pllPciDiv;
    UINT32  pllExtBusDiv;
    UINT32  pllOpbDiv;
    UINT32  pllMalDiv;
    UINT32  freqSys;          /* clock supplied to 440EP sysClk pin */
    UINT32  freqVCOMhz;       /* in MHz                             */
    UINT32  freqProcessor;
    UINT32  freqPLB;
    UINT32  freqMAL;
    UINT32  freqPCIInt;
    UINT32  freqPCIExt;
    UINT32  pciIntArbEn;      /* Internal PCI arbiter mode */
    UINT32  pciClkSync;       /* PCI clock mode        */
    } SYS_INFO;

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif  /* INCamcc440ep*/
