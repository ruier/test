/* ppc440EP.h - IBM PowerPC 440EP Chip header */

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
 * Copyright (c) 2006, 2008 Wind River Systems, Inc.
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
01b,02sep08,x_f  Remove sysDcr.h include
01a,10mar06,mmi  Created based on bamboo pc440EP.h 01h
*/

/*
This file contains DCR (Device Control Register) addresses and other
chip specific information for the IBM PowerPC 440EP outside of the 440
processor core (see ppc440.h for 440 processor core information).
*/

#ifndef INCppc440EPh
#define INCppc440EPh

#define PPC440EP

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * 440EP System Memory Address Map
 * The 440EP addresses are 36 bits,
 * hence the need for a _HIGH physical
 * address component.
 */ 

/* DDR SDRAM Controller 0 */
#define SDRAM0_PHY_START_HIGH   0x0   
#define SDRAM0_PHY_START_LOW    0x0

/* OPB Arbiter to 128-bit PLB */
#define OPBA1_PHY_START_HIGH    0x0   
#define OPBA1_PHY_START_LOW     0x50000000
#define OPBA1_SIZE              64              /* 0x000 to 0x03F */

/* USB2.0 Device Controller 0 */
#define USB2D0_PHY_START_HIGH   0x0   
#define USB2D0_PHY_START_LOW    0x50000100
#define USB2D0_SIZE             128             /* 0x100 to 0x17F */

/* External Bus Controller 0 */
#define EBC0_PHY_START_HIGH     0x0   
#define EBC0_PHY_START_LOW      0x80000000

/* PCI Memory                 */
#define PCIM0_PHY_START_HIGH    0x0   
#define PCIM0_PHY_START_LOW     0xA0000000

/* PCI I/O 0                  */
#define PCIIO0_PHY_START_HIGH   0x0   
#define PCIIO0_PHY_START_LOW    0xE8000000

/* PCI I/O 1                  */
#define PCIIO1_PHY_START_HIGH   0x0   
#define PCIIO1_PHY_START_LOW    0xE8800000

/* PCI Configuration Register */
#define PCIC0_PHY_START_HIGH    0x0   
#define PCIC0_PHY_START_LOW     0xEEC00000

/* PCI Inter.Ack / Spec.Cycle */
#define PCIC0_IASC0_PHY_START_HIGH 0x0   
#define PCIC0_IASC0_PHY_START_LOW  0xEED00000
#define PCIC0_IASC0_MAP_SIZE       0x1000   

/* PCI Local Conf. Reg. */
#define PCIL0_PHY_START_HIGH    0x0   
#define PCIL0_PHY_START_LOW     0xEF400000
/* #define PCIL0_MAP_SIZE              0x1000 */

/* On-chip Peripherals  (on On-Chip Peripheral Bus 1 (OPB1)) */
#define OCP_PHY_START_HIGH      0x0   
#define OCP_PHY_START_LOW       0xEF600000 
#define OCP_SIZE                4224 
#define OCP_MAP_SIZE            0x4000 
#ifndef INCLUDE_USB             /* reduce size if USB not required */
# undef  OCP_SIZE
# define OCP_SIZE               4096
# undef  OCP_MAP_SIZE
# define OCP_MAP_SIZE           0x1000 
#endif /* INCLUDE_USB */

/* BOOT SPACE */
#define EBC_PHY_START_HIGH      0x0   
#define EBC_PHY_START_LOW       0xF0000000 
#define BS_PHY_START_HIGH       EBC_PHY_START_HIGH /* BOOT SPACE */
#define BS_PHY_START_LOW        EBC_PHY_START_LOW     

/* EBC BANK 0 */
#define EBCB0_PHY_START_HIGH    0x0   
#define EBCB0_PHY_START_LOW     0xFFE00000 

/* General Use      */
#define GEN_PHY_START_HIGH      0x1   
#define GEN_PHY_START_LOW       0x00000000 

/*
 * 440EP On-Chip Peripherals  
 * (Offsets from OCP_PHY_START)
 */
#define OCP_GPT0_OFF   0x0000  /* general purpose timers */
#define OCP_UART0_OFF  0x0300  /* UART-0 */
#define OCP_UART1_OFF  0x0400  /* UART-1 */
#define OCP_UART2_OFF  0x0500  /* UART-2 */
#define OCP_UART3_OFF  0x0600  /* UART-3 */
#define OCP_IIC0_OFF   0x0700  /* Inter Intergrated Circuit Bus 0  */
#define OCP_IIC1_OFF   0x0800  /* Inter Intergrated Circuit Bus 1  */
#define OCP_SPI0_OFF   0x0900  /* Serial Port Interface 0          */
#define OCP_OPBA0_OFF  0x0A00  /* OPB Arbiter to 64-bit PLB 0      */
#define OCP_GPIO0_OFF  0x0B00  /* General Purpose I/O 0            */
#define OCP_GPIO1_OFF  0x0C00  /* General Purpose I/O 1            */
#define OCP_ZMII0_OFF  0x0D00  /* Ethernet to PHY Bridge 0         */
#define OCP_EMAC0_OFF  0x0E00  /* Ethernet Media Access Controller 0 */
#define OCP_EMAC1_OFF  0x0F00  /* Ethernet Media Access Controller 1 */
#define OCP_USBH0_OFF  0x1000  /* USB 1.1 Host Controller 0 */

/*
 * The 440EP chip has a 32KB I-cache and a 32KB D-cache. All 440
 * processor cores have 32 byte cache lines, and are 64-way set
 * associative.
 */

#define _ICACHE_LINE_NUM_440EP  16      /* 16 * 32 * 64 = 32KB */
#define _DCACHE_LINE_NUM_440EP  16      /* 16 * 32 * 64 = 32KB */
#define _DCACHE_NUM_WAYS        64
#define _ICACHE_NUM_WAYS        64
#define _DCACHE_SIZE    (_DCACHE_LINE_NUM_440EP * _CACHE_ALIGN_SIZE \
                             * _DCACHE_NUM_WAYS)
#define _ICACHE_SIZE    (_ICACHE_LINE_NUM_440EP * _CACHE_ALIGN_SIZE \
                             * _ICACHE_NUM_WAYS)
#define _NUM_DCACHE_LINES (_DCACHE_SIZE/_CACHE_ALIGN_SIZE)

/* Processor Version Register (PVR) value for 440EP */

#define PVR_440EP_RA   0x42221850    /* rev A (1.0) JTAG_ID: 0x049950a1 */
#define PVR_440EP_RB   0x422218D3    /* rev B (2.0) JTAG_ID: 0x2a950049 */
#define PVR_440EP_RC   0x422218D3    /* rev C (2.1) JTAG_ID: 0x2A950049 */

/*
 * Valid Memory Access Layer (MAL) channels. On the 440EP, EMAC0 and 1
 * are the only communications cores that use MAL. They each have 2 TX
 * channels, and 1 RX channel.
 */

#ifdef USE_BOTH_EMACS
#define MAL_CHN_TX_CPUCORE  0xf0000000    /* emac0 TX ch 0/1, emac1 TX ch 0/1 */
#define MAL_CHN_RX_CPUCORE  0xc0000000    /* emac0 RX ch 0, emac1 ch 0        */
#else /* !USE_BOTH_EMACS */
#define MAL_CHN_TX_CPUCORE  0xc0000000    /* emac0 TX ch 0/1                  */
#define MAL_CHN_RX_CPUCORE  0x80000000    /* emac0 RX ch 0                    */
#endif /* USE_BOTH_EMACS */

/*
 * MAL channels numbers the EMAC Ethernet controllers are connected to on
 * 440EP. This information is put into the EMAC driver END load string.
 */

#define EMAC0_TX_CHN_0_CPUCORE  0
#define EMAC0_TX_CHN_1_CPUCORE  1
#define EMAC0_RX_CHN_0_CPUCORE  0

#ifdef USE_BOTH_EMACS
#  define EMAC1_TX_CHN_0_CPUCORE        2
#  define EMAC1_TX_CHN_1_CPUCORE        3
#  define EMAC1_RX_CHN_0_CPUCORE        1
#endif /* USE_BOTH_EMACS */

/* Base DCR address values for all peripheral cores in the 440EP */

#define CPR0_DCR_BASE       0x00C       /* Clocking-Power On-Reset Config     */
#define SDR0_DCR_BASE       0x00E       /* System DCR                         */
#define SDRAM0_DCR_BASE     0x010       /* SDRAM Controller                   */
#define EBC0_DCR_BASE       0x012       /* External Bus Controller            */
#define PPM0_DCR_BASE       0x016       /* PLB Performance Counter            */
#define P4P3B_DCR_BASE      0x020       /* PLB4 to PLB3 Bridge Out            */
#define P3P4B_DCR_BASE      0x030       /* PLB3 to PLB4 Bridge In             */
#define PLB3_DCR_BASE       0x070       /* PLB3 Arbiter                       */
#ifdef PPC440EP_REVA
#define PLB4_DCR_BASE       0x080       /* PLB4 Arbiter                       */
#else /* PPC440EP 2.0 */
#define PLB0_DCR_BASE       0x080       /* PLB4 Crossbar Arbiter              */
#endif /* PPC440EP 1.0 */
#define P3OB_DCR_BASE       0x090       /* PLB3 to OPB Bridge                 */
#define OP3B_DCR_BASE       0x0A8       /* OPB to PLB3 Bridge                 */
#define CPM0_DCR_BASE       0x0B0       /* Chip Power Management              */
#define UIC0_DCR_BASE       0x0C0       /* Universal Interrupt Controller 0   */
#define UIC1_DCR_BASE       0x0D0       /* Universal Interrupt Controller 1   */
#define DMA0_DCR_BASE       0x100       /* DMA to PLB3 Controller             */
#define MAL0_DCR_BASE       0x180       /* Memory Access Layer Core           */
#define P4OB_DCR_BASE       0x200       /* PLB4 to OPB Bridge                 */
#define DMA1_DCR_BASE       0x210       /* DMA to PLB4 Controller             */

/*
 * Clocking/Power On Reset (CPR) register definitions.
 * Two DCRs are used (address/data) to
 * indirectly access the config registers of this function.
 */

#define CPR0_CFGADDR  (CPR0_DCR_BASE+0x0) /* CPR configuration address*/
#define CPR0_CFGDATA  (CPR0_DCR_BASE+0x1) /* CPR configuration data   */

/*
 * Values for CPR0_CFGADDR register - indirect addressing of these regs
 * and associated mask and bits definitions
 */

#define CPR0_CLKUPD   0x0020     /* Clocking Update Register   */
#define   CPR0_CLKUPD_BSY_MASK         0x80000000
#define   CPR0_CLKUPD_BSY_COMPLETED    0x00000000
#define   CPR0_CLKUPD_BSY_BUSY         0x80000000
#define   CPR0_CLKUPD_CUI_MASK         0x80000000
#define   CPR0_CLKUPD_CUI_DISABLE      0x00000000
#define   CPR0_CLKUPD_CUI_ENABLE       0x80000000
#define   CPR0_CLKUPD_CUD_MASK         0x40000000
#define   CPR0_CLKUPD_CUD_DISABLE      0x00000000
#define   CPR0_CLKUPD_CUD_ENABLE       0x40000000
#define CPR0_PLLC     0x0040     /* PLL Control                */
#define   CPR0_PLLC_RST_MASK           0x80000000
#define   CPR0_PLLC_RST_PLLLOCKED      0x00000000
#define   CPR0_PLLC_RST_PLLRESET       0x80000000
#define   CPR0_PLLC_ENG_MASK           0x40000000
#define   CPR0_PLLC_ENG_DISABLE        0x00000000
#define   CPR0_PLLC_ENG_ENABLE         0x40000000
#define   CPR0_PLLC_SRC_MASK           0x20000000
#define   CPR0_PLLC_SRC_PLLOUTA        0x00000000
#define   CPR0_PLLC_SRC_PLLOUTB        0x20000000
#define   CPR0_PLLC_SEL_MASK           0x07000000
#define   CPR0_PLLC_SEL_PLLOUT         0x00000000
#define   CPR0_PLLC_SEL_CPU            0x01000000
#define   CPR0_PLLC_SEL_EBC            0x05000000
#define   CPR0_PLLC_TUNE_MASK          0x000003FF
#define CPR0_PLLD     0x0060     /* PLL Divider                */
#define   CPR0_PLLD_FBDV_MASK          0x1F000000
#define   CPR0_PLLD_FBDV_ENCODE(n)     ((((unsigned long)(n))&0x1F)<<24)
#define   CPR0_PLLD_FBDV_DECODE(n)     ((((((unsigned long)(n))>>24)-1)&0x1F)+1)
#define   CPR0_PLLD_FWDVA_MASK         0x000F0000
#define   CPR0_PLLD_FWDVA_ENCODE(n)    ((((unsigned long)(n))&0x0F)<<16)
#define   CPR0_PLLD_FWDVA_DECODE(n)    ((((((unsigned long)(n))>>16)-1)&0x0F)+1)
#define   CPR0_FWDVA_9R                0x00090000   /* Reserved */
#define   CPR0_FWDVA_11R               0x000B0000   /* Reserved */
#define   CPR0_FWDVA_13R               0x000D0000   /* Reserved */
#define   CPR0_FWDVA_15R               0x000F0000   /* Reserved */
#define   CPR0_PLLD_FWDVB_MASK         0x00000700
#define   CPR0_PLLD_FWDVB_ENCODE(n)    ((((unsigned long)(n))&0x07)<<8)
#define   CPR0_PLLD_FWDVB_DECODE(n)    ((((((unsigned long)(n))>>8)-1)&0x07)+1)
#define   CPR0_PLLD_LFBDV_MASK         0x0000003F
#define   CPR0_PLLD_LFBDV_ENCODE(n)    ((((unsigned long)(n))&0x3F)<<0)
#define   CPR0_PLLD_LFBDV_DECODE(n)    ((((((unsigned long)(n))>>0)-1)&0x3F)+1)
#define CPR0_PRIMAD   0x0080     /* Primary A Divider          */
#define   CPR0_PRIMAD_PRADV0_MASK      0x07000000
#define   CPR0_PRIMAD_PRADV0_ENCODE(n) ((((unsigned long)(n))&0x07)<<24)
#define   CPR0_PRIMAD_PRADV0_DECODE(n) ((((((unsigned long)(n))>>24)-1)&0x07)+1)
#define CPR0_PRIMBD   0x00A0     /* Primary Divider B          */
#define   CPR0_PRIMBD_PRBDV0_MASK      0x07000000
#define   CPR0_PRIMBD_PRBDV0_ENCODE(n) ((((unsigned long)(n))&0x07)<<24)
#define   CPR0_PRIMBD_PRBDV0_DECODE(n) ((((((unsigned long)(n))>>24)-1)&0x07)+1)
#define CPR0_OPBD     0x00C0     /* OPB Clock Divider          */
#define   CPR0_OPBD_OPBDV0_MASK        0x03000000
#define   CPR0_OPBD_OPBDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_OPBD_OPBDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)
#define CPR0_PERD     0x00E0     /* Peripheral Clock Divider   */
#define   CPR0_PERD_PERDV0_MASK        0x03000000
#define   CPR0_PERD_PERDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_PERD_PERDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)
#define CPR0_MALD     0x0100     /* MAL Clock Divider          */
#define   CPR0_MALD_MALDV0_MASK        0x03000000
#define   CPR0_MALD_MALDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_MALD_MALDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)
#define CPR0_SPCID    0x0120     /* Synch PCI Clock Divider    */
#define   CPR0_SPCID_SPCIDV0_MASK        0x03000000
#define   CPR0_SPCID_SPCIDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_SPCID_SPCIDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)
#define CPR0_ICFG     0x0140     /* Initial Configuration      */
#define   CPR0_ICFG_RLI_MASK           0x80000000
#define   CPR0_ICFG_RLI_RESETCPR       0x00000000
#define   CPR0_ICFG_RLI_PRESERVECPR    0x80000000


/*
 * System DCR (SDR) register definitions.
 * Two DCRs are used (address/data) to
 * indirectly access the config registers of this function.
 */
/* #define SDR0_CFGADDR  (SDR0_DCR_BASE+0x0) */ /* SDRO Controller Addr Reg - defined in sdrDcr.h */
/* #define SDR0_CFGDATA  (SDR0_DCR_BASE+0x1) */ /* SDRO Controller Data Reg - defined in sdrDcr.h */

/* Indirect access registers for SDR0 -- use SDR0_CFGADDR to access */

/* #define SDR0_SDSTP0            0x0020 */  /*  Serial Device Strap Reg 0 - defined in sdrDcr.h    */
/* #define SDR0_SDSTP1            0x0021 */  /*  Serial Device Strap Reg 1 - defined in sdrDcr.h    */
#define SDR0_PSTRP0               0x0040     /*  Pin Straps                                         */
#define   SDR0_PSTRP0_PSTP_MASK   0xE0000000
#define   SDR0_PSTRP0_PSTP_DSS0   0x00000000 /* Default strap setting 0 */
#define   SDR0_PSTRP0_PSTP_DSS1   0x20000000 /* Default strap setting 1 */
#define   SDR0_PSTRP0_PSTP_DSS2   0x40000000 /* Default strap setting 2 */
#define   SDR0_PSTRP0_PSTP_DSS3   0x60000000 /* Default strap setting 3 */
#define   SDR0_PSTRP0_PSTP_SPROM1 0xA0000000 /* Boot from SPROM 1 */
#define   SDR0_PSTRP0_PSTP_SPROM0 0xE0000000 /* Boot from SPROM 0 */
#define SDR0_SDCS0                0x0060 */  /*  Serial Device Controller Settings                */
/* #define SDR0_ECID0             0x0080 */  /*  Electronic Chip ID 0 - defined in sdrDcr.h       */
/* #define SDR0_ECID1             0x0081 */  /*  Electronic Chip ID 1 - defined in sdrDcr.h       */
/* #define SDR0_ECID2             0x0082 */  /*  Electronic Chip ID 2 - defined in sdrDcr.h       */
#define SDR0_ECID3                0x0083     /*  Electronic Chip ID 3                             */
/* #define SDR0_JTAG              0x00C0 */  /*  JTAG ID - defined in sdrDcr.h                    */
#define SDR0_DDRDL0               0x00E0     /*  DDR Delay Line                                   */
#define SDR0_EBC0                 0x0100     /*  EBC                                              */
/* #define SDR0_UART0             0x0120 */  /*  UART0 - defined in sdrDcr.h                      */
/* #define SDR0_UART1             0x0121 */  /*  UART1 - defined in sdrDcr.h                      */
#define SDR0_UART2                0x0122     /*  UART2                                            */
#define SDR0_UART3                0x0123     /*  UART3                                            */
#define   SDR0_UARTX_UXICS_MASK        0xF0000000
/* #define   SDR0_UARTX_UXICS_PLB      0x20000000 */ /* defined in sdrDcr.h			  */
#define   SDR0_UARTX_UXEC_MASK         0x00800000
/* #define   SDR0_UARTX_UXEC_INT       0x00000000 */ /* defined in sdrDcr.h                       */
/* #define   SDR0_UARTX_UXEC_EXT       0x00800000 */ /* defined in sdrDcr.h                       */
#define   SDR0_UARTX_UXDTE_MASK        0x00400000
#define   SDR0_UARTX_UXDTE_DISABLE     0x00000000
#define   SDR0_UARTX_UXDTE_ENABLE      0x00400000
#define   SDR0_UARTX_UXDRE_MASK        0x00200000
#define   SDR0_UARTX_UXDRE_DISABLE     0x00000000
#define   SDR0_UARTX_UXDRE_ENABLE      0x00200000
#define   SDR0_UARTX_UXDC_MASK         0x00100000
#define   SDR0_UARTX_UXDC_NOTCLEARED   0x00000000
#define   SDR0_UARTX_UXDC_CLEARED      0x00100000
#define   SDR0_UARTX_UXDIV_MASK        0x000000FF
/* #define   SDR0_UARTX_UXDIV_ENCODE(n)((((unsigned long)(n))&0xFF)<<0) */ /* defined in sdrDcr.h */ 
#define   SDR0_UARTX_UXDIV_DECODE(n)   ((((((unsigned long)(n))>>0)-1)&0xFF)+1)
#define	  SDR0_CPU440		       0x0180		/*  440CPU                                */
#define   SDR0_CPU440_EARV_MASK        0x30000000
#define   SDR0_CPU440_EARV_EBC         0x00000000
#define   SDR0_CPU440_EARV_PCI         0x10000000
#define   SDR0_CPU440_EARV_NDFC        0x20000000
#define   SDR0_CPU440_DRU_MASK         0x0C000000
#define   SDR0_CPU440_DRU_0            0x00000000
#define   SDR0_CPU440_DRU_1            0x04000000
#define   SDR0_CPU440_DRU_2            0x08000000
#define   SDR0_CPU440_DRU_3            0x0C000000
#define   SDR0_CPU440_DRT_MASK         0x03000000
#define   SDR0_CPU440_DRT_0            0x00000000
#define   SDR0_CPU440_DRT_1            0x01000000
#define   SDR0_CPU440_DRT_2            0x02000000
#define   SDR0_CPU440_DRT_3            0x03000000
#define   SDR0_CPU440_DRNC_MASK        0x00C00000
#define   SDR0_CPU440_DRNC_0           0x00000000
#define   SDR0_CPU440_DRNC_1           0x00400000
#define   SDR0_CPU440_DRNC_2           0x00800000
#define   SDR0_CPU440_DRNC_3           0x00C00000
#define   SDR0_CPU440_DRLC_MASK        0x00030000
#define   SDR0_CPU440_DRLC_0           0x00000000
#define   SDR0_CPU440_DRLC_1           0x00010000
#define   SDR0_CPU440_DRLC_2           0x00020000
#define   SDR0_CPU440_DRLC_3           0x00030000
#define   SDR0_CPU440_DWF_MASK         0x0000C000
#define   SDR0_CPU440_DWF_0            0x00000000
#define   SDR0_CPU440_DWF_1            0x00004000
#define   SDR0_CPU440_DWF_2            0x00008000
#define   SDR0_CPU440_DWF_3            0x0000C000
#define   SDR0_CPU440_DWS_MASK         0x00003000
#define   SDR0_CPU440_DWS_0            0x00000000
#define   SDR0_CPU440_DWS_1            0x00001000
#define   SDR0_CPU440_DWS_2            0x00002000
#define   SDR0_CPU440_DWS_3            0x00003000
#define   SDR0_CPU440_DWU_MASK         0x00000C00
#define   SDR0_CPU440_DWU_0            0x00000000
#define   SDR0_CPU440_DWU_1            0x00000400
#define   SDR0_CPU440_DWU_2            0x00000800
#define   SDR0_CPU440_DWU_3            0x00000C00
#define   SDR0_CPU440_IRF_MASK         0x00000300
#define   SDR0_CPU440_IRF_0            0x00000000
#define   SDR0_CPU440_IRF_1            0x00000100
#define   SDR0_CPU440_IRF_2            0x00000200
#define   SDR0_CPU440_IRF_3            0x00000300
#define   SDR0_CPU440_IRT_MASK         0x000000C0
#define   SDR0_CPU440_IRT_0            0x00000000
#define   SDR0_CPU440_IRT_1            0x00000040
#define   SDR0_CPU440_IRT_2            0x00000080
#define   SDR0_CPU440_IRT_3            0x000000C0
#define   SDR0_CPU440_IRS_MASK         0x00000030
#define   SDR0_CPU440_IRS_0            0x00000000
#define   SDR0_CPU440_IRS_1            0x00000010
#define   SDR0_CPU440_IRS_2            0x00000020
#define   SDR0_CPU440_IRS_3            0x00000030
#define   SDR0_CPU440_NTO1_MASK        0x00000002
#define   SDR0_CPU440_NTO1_NTOP        0x00000000
#define   SDR0_CPU440_NTO1_NTO1        0x00000002

#define SDR0_SRST0                0x0200   /*  Individual core reset control 0                    */
#define SDR0_SRST1                0x0201   /*  Individual core reset control 1                    */
#define SDR0_SLPIPE0              0x0220   /*  Individual control for disabling address pipelining
                                               at up to 1024 PLB slaves                           */
#define SDR0_AMP0                 0x0240   /*  Override PLB4 priority for master(s).
                                               Each register provides control for up to 8 masters */
#define SDR0_AMP1                 0x0241   /*  Override PLB3 priority for master(s).
                                               Each register provides control for up to 8 masters */
#define SDR0_AMPX_APM0_0               0x00000000
#define SDR0_AMPX_APM0_1               0x40000000
#define SDR0_AMPX_APM0_2               0x80000000
#define SDR0_AMPX_APM0_3               0xC0000000
#define SDR0_AMPX_APM0_MASK            0xC0000000
#define SDR0_AMPX_APM1_0               0x00000000
#define SDR0_AMPX_APM1_1               0x10000000
#define SDR0_AMPX_APM1_2               0x20000000
#define SDR0_AMPX_APM1_3               0x30000000
#define SDR0_AMPX_APM1_MASK            0x30000000
#define SDR0_AMPX_APM2_0               0x00000000
#define SDR0_AMPX_APM2_1               0x04000000
#define SDR0_AMPX_APM2_2               0x08000000
#define SDR0_AMPX_APM2_3               0x0C000000
#define SDR0_AMPX_APM2_MASK            0x0C000000
#define SDR0_AMPX_APM3_0               0x00000000
#define SDR0_AMPX_APM3_1               0x01000000
#define SDR0_AMPX_APM3_2               0x02000000
#define SDR0_AMPX_APM3_3               0x03000000
#define SDR0_AMPX_APM3_MASK            0x03000000
#define SDR0_AMPX_APM5_0               0x00000000
#define SDR0_AMPX_APM5_1               0x00100000
#define SDR0_AMPX_APM5_2               0x00200000
#define SDR0_AMPX_APM5_3               0x00300000
#define SDR0_AMPX_APM5_MASK            0x00300000
#define SDR0_AMPX_APM6_0               0x00000000
#define SDR0_AMPX_APM6_1               0x00040000
#define SDR0_AMPX_APM6_2               0x00080000
#define SDR0_AMPX_APM6_3               0x000C0000
#define SDR0_AMPX_APM6_MASK            0x000C0000
#define SDR0_AMPX_APM7_0               0x00000000
#define SDR0_AMPX_APM7_1               0x00010000
#define SDR0_AMPX_APM7_2               0x00020000
#define SDR0_AMPX_APM7_3               0x00030000
#define SDR0_AMPX_APM7_MASK            0x00030000
#define SDR0_AMPX_M0PS_MASTER          0x00000000
#define SDR0_AMPX_M0PS_ALT             0x00008000
#define SDR0_AMPX_M0PS_MASK            0x00008000
#define SDR0_AMPX_M1PS_MASTER          0x00000000
#define SDR0_AMPX_M1PS_ALT             0x00004000
#define SDR0_AMPX_M1PS_MASK            0x00004000
#define SDR0_AMPX_M2PS_MASTER          0x00000000
#define SDR0_AMPX_M2PS_ALT             0x00002000
#define SDR0_AMPX_M2PS_MASK            0x00002000
#define SDR0_AMPX_M3PS_MASTER          0x00000000
#define SDR0_AMPX_M3PS_ALT             0x00001000
#define SDR0_AMPX_M3PS_MASK            0x00001000
#define SDR0_AMPX_M5PS_MASTER          0x00000000
#define SDR0_AMPX_M5PS_ALT             0x00000400
#define SDR0_AMPX_M5PS_MASK            0x00000400
#define SDR0_AMPX_M6PS_MASTER          0x00000000
#define SDR0_AMPX_M6PS_ALT             0x00000200
#define SDR0_AMPX_M6PS_MASK            0x00000200
#define SDR0_AMPX_M7PS_MASTER          0x00000000
#define SDR0_AMPX_M7PS_ALT             0x00000100
#define SDR0_AMPX_M7PS_MASK            0x00000100

/* #define SDR0_MIRQ0             0x0260 */ /*  PLB3 Master Int Reg 0 - defined in sdrDcr.h        */
/* #define SDR0_MALTBL            0x0280 */ /*  MAL Tx Burst Length  - defined in sdrDcr.h         */
/* #define SDR0_MALRBL            0x02A0 */ /*  MAL Rx Burst Length  - defined in sdrDcr.h         */
/* #define SDR0_MALTBS            0x02C0 */ /*  MAL Tx Bus Size  - defined in sdrDcr.h             */
/* #define SDR0_MALRBS            0x02E0 */ /*  MAL Rx Bus Size  - defined in sdrDcr.h             */
#define   SDR0_PCI0               0x0300      /* PCI                            */
#define   SDR0_PCI0_PAE_MASK      0x80000000
#define   SDR0_PCI0_PAE_DISABLE   0x00000000  /* PCI internal Arbiter Disable   */
#define   SDR0_PCI0_PAE_ENABLE    0x80000000  /* PCI internal Arbiter Enable    */
#define   SDR0_PCI0_PHCE_MASK     0x40000000
#define   SDR0_PCI0_PHCE_DISABLE  0x00000000  /* PCI Host Configuration Disable */
#define   SDR0_PCI0_PHCE_ENABLE   0x40000000  /* PCI Host Configuration Enable  */
#define   SDR0_PCI0_PAME_MASK     0x20000000
#define   SDR0_PCI0_PAME_DISABLE  0x00000000  /* PCI Asynchronous Mode Disable  */
#define   SDR0_PCI0_PAME_ENABLE   0x20000000  /* PCI Asynchronous Mode Enable   */

#define   SDR0_USB0               0x0320      /*  USB					*/
#define   SDR0_USB0_USB2D_DEVSEL  0x00000002  /* selection: USB2 or USB11		*/
                                              /* 0 = USB2.0 Device Interface enabled	*/
                                              /* 1 = USB1.1 Device Interface enabled 	*/
#define   SDR0_USB0_LEEN          0x00000001  /* 1= Little Endian Enabled 		*/

#define   SDR0_NDFC0                0x0340   /*  NDFC                                               */
/* #define SDR0_CUST0               0x4000 */ /*  Reserved for Customer Use - defined in sdrDcr.h   */
#define   SDR0_CUST0_MUX_EMAC_NDFC_MASK       0xC0000000  /* MUX EMAC/NDFC/GPIO Mask    */
#define   SDR0_CUST0_SEL_EMAC                 0x40000000  /* Select EMAC for MUX EMAC/NDFC/GPIO */
#define   SDR0_CUST0_SEL_NDFC                 0x80000000  /* Select NDFC for MUX EMAC/NDFC/GPIO */
#define   SDR0_CUST0_SEL_GPIO                 0xC0000000  /* Select GPIO for MUX EMAC/NDFC/GPIO */
#define   SDR0_CUST0_NDFC_ENABLE_MASK         0x20000000  /* NDFC Enable Mask    */
#define   SDR0_CUST0_NDFC_ENABLE              0x20000000  /* NDFC Enable  */
#define   SDR0_CUST0_NDFC_DISABLE             0x00000000  /* NDFC Disable  */
#define   SDR0_CUST0_NDFC_BOOT_WIDTH_MASK     0x10000000  /* NDFC Boot width Mask    */
#define   SDR0_CUST0_NDFC_BOOT_WIDTH_16       0x10000000  /* 16-bit NDFC Boot width */
#define   SDR0_CUST0_NDFC_BOOT_WIDTH_8        0x00000000  /* 8-bit NDFC Boot width */
#define   SDR0_CUST0_NDFC_BOOT_PAGE_MASK      0x0F000000  /* NDFC Boot Page Mask    */
#define   SDR0_CUST0_NDFC_BOOT_ADD_CYCLE_MASK 0x00C00000  /* NDFC Boot Addr cycle Mask   */
#define   SDR0_CUST0_NDFC_AUTO_READ_ENA_MASK  0x00200000  /* NDFC Auto read enable Mask  */
#define   SDR0_CUST0_NDFC_AUTO_READ_ENABLE    0x00200000  /* NDFC Auto read enable */
#define   SDR0_CUST0_NDFC_AUTO_READ_DISABLE   0x00000000  /* NDFC Auto read disable */
#define   SDR0_CUST0_NDFC_READY_BUSY_MASK     0x00100000  /* NDFC Ready/Busy Mask  */
#define   SDR0_CUST0_NDFC_BUSY                0x00100000  /* NDFC Busy  */
#define   SDR0_CUST0_NDFC_READY               0x00000000  /* NDFC Ready  */
#define   SDR0_CUST0_DEVICE_RST_CNT_MASK      0x000FFFF0  /* NDFC Device Reset Counter Mask  */
#define   SDR0_CUST0_CHIP_SELECT_GATING_MASK  0x0000000F  /* EBC Chip select Gating  */
#define   SDR0_CUST0_CHIP_SELECT_GATING_CS0   0x00000008  /* EBC CS0 Chip select Gating */
#define   SDR0_CUST0_CHIP_SELECT_GATING_CS1   0x00000004  /* EBC CS1 Chip select Gating */
#define   SDR0_CUST0_CHIP_SELECT_GATING_CS2   0x00000002  /* EBC CS2 Chip select Gating */
#define   SDR0_CUST0_CHIP_SELECT_GATING_CS3   0x00000001  /* EBC CS3 Chip select Gating */
#define SDR0_STRP2             0x4001    /*  Read Only version of CUST0                           */
/* #define SDR0_CUST1          0x4002 */ /*  Reserved for Customer Use - defined in sdrDcr.h      */
#define SDR0_STRP3             0x4003    /*  Read Only version of CUST1                           */
/* #define SDR0_PFC0           0x4100 */ /*  Pin Function Control: Selects which multiplexed      */
                                         /*  functions appear on chip pins - defined in sdrDcr.h  */
/* #define SDR0_PFC1           0x4101 */ /*  Pin Function Control: Selects which multiplexed      */
                                         /*  functions appear on chip pins - defined in sdrDcr.h  */
#define   SDR0_PFC1_U1ME_MASK     0x02000000
#define   SDR0_PFC1_U1ME_CTS      0x02000000 /* UART1 in CTS/RTS mode */
#define   SDR0_PFC1_U1ME_DSR      0x00000000 /* UART1 in DSR/DTR mode */
#define   SDR0_PFC1_U0ME_MASK     0x00080000
#define   SDR0_PFC1_U0ME_CTS      0x00080000 /* UART0 in CTS/RTS mode */
#define   SDR0_PFC1_U0ME_DSR      0x00000000 /* UART0 in DSR/DTR mode */
#define   SDR0_PFC1_U0IM_MASK     0x00040000
#define   SDR0_PFC1_U0IM_4PINS    0x00040000 /* UART 0 in 4 pins mode */
#define   SDR0_PFC1_U0IM_8PINS    0x00000000 /* UART 0 in 8 pins mode */
#define   SDR0_PFC1_SIS_MASK      0x00020000
#define   SDR0_PFC1_SIS_IIC1      0x00020000 /* IIC1 interface selected */
#define   SDR0_PFC1_SIS_SCP       0x00000000 /* SCP interface selected */
#define   SDR0_PFC1_UES_MASK      0x00010000
#define   SDR0_PFC1_UES_EBCHR     0x00010000 /* EBC Hold Req selected */
#define   SDR0_PFC1_UES_USBRA     0x00000000 /* USB Rx Active selected */
#define   SDR0_PFC1_DIS_MASK      0x00008000
#define   SDR0_PFC1_DIS_IRQ5      0x00008000 /* Ext IRQ 5 Selected */
#define   SDR0_PFC1_DIS_DMAR1     0x00000000 /* DMA Req(1) Selected */
#define   SDR0_PFC1_ERE_MASK      0x00004000
#define   SDR0_PFC1_ERE_GPIO27    0x00004000 /* GPIO(27) Selected */
#define   SDR0_PFC1_ERE_EXTREN    0x00000000 /* External Request Enable Selected */
#define   SDR0_PFC1_PME_MASK      0x00001000
#define   SDR0_PFC1_PMPLB4_ENABLE 0x00001000 /* PLB4 Performance Monitor Enable */
#define   SDR0_PFC1_PMPLB3_ENABLE 0x00000000 /* PLB3 Performance Monitor Enable */
#define   SDR0_PFC1_GFGGI_MASK    0x0000000F
/* #define   SDR0_PLBTR           0x4200  */ /*  PLB Trace Muxing Select - defined in sdrDcr.h         */
/* #define   SDR0_MFR             0x4300  */ /*  Miscellaneous Function Register - defined in sdrDcr.h */
#define   SDR0_MFR_ENET0_CLK_SEL      0x08000000  /* Ethernet 0 Clock Select */
#define   SDR0_MFR_ENET0_INT_CLK      0x08000000  /* Select Internal Reference Clock/4 for internal */
#define   SDR0_MFR_ENET0_EXT_CLK      0x00000000  /* Select External Clock */
#define   SDR0_MFR_ENET1_CLK_SEL      0x04000000  /* Ethernet 1 Clock Select */
#define   SDR0_MFR_ENET1_INT_CLK      0x04000000  /* Select Internal Reference Clock/4 for internal */
#define   SDR0_MFR_ENET1_EXT_CLK      0x00000000  /* Select External Clock */
#define   SDR0_MFR_ZMII_MODE_MASK     0x03000000  /* ZMII mode Bits 0:1 */
#define   SDR0_MFR_ZMII_MODE_MII      0x00000000  /* MII mode Selected */
#define   SDR0_MFR_ZMII_MODE_SMII     0x01000000  /* SMII mode selected */
#define   SDR0_MFR_ZMII_MODE_RMII10M  0x02000000  /* RMII 10 Mbps selected */
#define   SDR0_MFR_ZMII_MODE_RMII100M 0x03000000  /* RMII 100 Mbps selected */
#define   SDR0_MFR_ERRATA3_0_ENABLE   0x00800000  /* ERRATA3(0) enable */
#define   SDR0_MFR_ERRATA3_1_ENABLE   0x00400000  /* ERRATA3(1) enable */
#define   SDR0_MFR_PKT_REJECT_0_ENA   0x00200000  /* Packet reject(0) enable */
#define   SDR0_MFR_PKT_REJECT_1_ENA   0x00100000  /* Packet reject(1) enable */
#define   SDR0_MFR_PKT_REJECT_POL     0x00080000  /* Packet reject polarity */
#define   SDR0_MFR_PKT_RJCT_POL_NEG   0x00080000  /* Packet reject polarity = Negative */
#define   SDR0_MFR_PKT_RJCT_POL_POS   0x00000000  /* Packet reject polarity = Positive */
#define SDR0_HSF                  0x4400   /*  DDR Hardware Self Refresh                          */

#ifdef PPC440EP_REVB  /* PPC440EP 2.0 */

#define SDR0_EMAC0_RXSTATR          0x4301   /*  EMAC0 RX Status Register                           */
#define SDR0_EMAC0_TXSTATR          0x4302   /*  EMAC0 TX Status Register                           */
#define SDR0_EMAC1_RXSTATR          0x4303   /*  EMAC1 RX Status Register                           */
#define SDR0_EMAC1_TXSTATR          0x4304   /*  EMAC1 TX Status Register                           */
#define SDR0_EMAC0_PKT_REJCNT       0x4305   /*  EMAC0 Packet Reject Counter Register               */
#define SDR0_EMAC1_PKT_REJCNT       0x4306   /*  EMAC1 Packet Reject Counter Register               */

#endif /* PPC440EP 2.0 */

/*
 * CPM register definitions.
 * Each is a separate DCR register.
 */

#define CPM0_SR     (CPM0_DCR_BASE+0x0)  /* clock/power management status */
#define CPM0_ER     (CPM0_DCR_BASE+0x1)  /* clock/power management enable */
#define CPM0_FR     (CPM0_DCR_BASE+0x2)  /* clock/power management force  */

/*
 * Support for bus error identification functions, e.g. sysBusErrRegsGet
 */
#ifdef PPC440EP_REVA  /* PPC440EP 1.0 */

#define SYS_BUS_ERR_PLB4_ID    1
#define SYS_BUS_ERR_P4OB_ID    2
#define SYS_BUS_ERR_P4P3B_ID   3
#define SYS_BUS_ERR_PLB3_ID    4
#define SYS_BUS_ERR_P3OB_ID    5

#else /* PPC440EP 2.0 */

#define SYS_BUS_ERR_PLB0L_ID   1
#define SYS_BUS_ERR_PLB1L_ID   2
#define SYS_BUS_ERR_P4OB_ID    3
#define SYS_BUS_ERR_P4P3B_ID   4
#define SYS_BUS_ERR_PLB3_ID    5
#define SYS_BUS_ERR_P3OB_ID    6

#endif /* PPC440EP 1.0 */
/*
 * PLB Bus Error, Arbiter and PLB to OPB Bridge DCR registers.
 */

/* PLB3 arbiter Regs */
#define PLB3_REVID      (PLB3_DCR_BASE+0x02)  /* R PLB3 Arbiter Revision ID */
#define PLB3_BESR       (PLB3_DCR_BASE+0x04)  /* R/Clear PLB3 Bus Error Status Register */
#define PLB3_BEAR       (PLB3_DCR_BASE+0x06)  /* R/W PLB3 Bus Error Address Register */
#define PLB3_ACR        (PLB3_DCR_BASE+0x07)  /* R/W PLB3 Arbiter Control Register */

/*------------- PLB3 BESR bits definitions --------------------------------------*/
#define PLB3_BESR_ERROR_MASK  0x88888888
#define PLB3_BESR_P4P3B_MASK  0x80000000
#define PLB3_BESR_P4P3B_MASK_U  0x8000
#define PLB3_BESR_IO_MASK 0x08888888
#define PLB3_BESR_IO_MASK_U 0x0888
#define PLB3_BESR_IO_MASK_L     0x8888
#define PLB3_BESR_PTE0    0x80000000  /* master 0:  PLB4 to PLB3 bridge */
#define PLB3_BESR_R_W0    0x40000000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK0    0x20000000
#define PLB3_BESR_ALK0    0x10000000
#define PLB3_BESR_PTE1    0x08000000  /* master 1:  MAL */
#define PLB3_BESR_R_W1    0x04000000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK1    0x02000000
#define PLB3_BESR_ALK1    0x01000000
#define PLB3_BESR_PTE2    0x00800000  /* master 2:  PCI bridge */
#define PLB3_BESR_R_W2    0x00400000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK2    0x00200000
#define PLB3_BESR_ALK2    0x00100000
#define PLB3_BESR_PTE3    0x00080000  /* master 3:  DMA0 Controller */
#define PLB3_BESR_R_W3    0x00040000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK3    0x00020000
#define PLB3_BESR_ALK3    0x00010000
#define PLB3_BESR_PTE4    0x00008000  /* master 4:  OPB to PLB3 bridge */
#define PLB3_BESR_R_W4    0x00004000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK4    0x00002000
#define PLB3_BESR_ALK4    0x00001000
#define PLB3_BESR_PTE5    0x00008000  /* master 5:  EBC */
#define PLB3_BESR_R_W5    0x00004000  /* 1 => read, 0 => write */
#define PLB3_BESR_FLK5    0x00002000
#define PLB3_BESR_ALK5    0x00001000

#ifdef PPC440EP_REVA  /* PPC440EP 1.0 */

/* PLB4 arbiter Regs */
#define PLB4_REVID      (PLB4_DCR_BASE+0x02)  /* R PLB4 Arbiter Revision ID */
#define PLB4_ACR        (PLB4_DCR_BASE+0x03)  /* R/W PLB4 Arbiter Control Register */
#define PLB4_BESR       (PLB4_DCR_BASE+0x04)  /* R/Clear PLB4 Bus Error Status Register */
#define PLB4_BEARL      (PLB4_DCR_BASE+0x06)  /* R/W PLB4 Bus Error Address Register Low */
#define PLB4_BEARH      (PLB4_DCR_BASE+0x07)  /* R/W PLB4 Bus Error Address Register High */

/*------------- PLB4 BESR bits definitions --------------------------------------*/
#define PLB4_BESR_ERROR_MASK  0x88888888
#define PLB4_BESR_CACHE_MASK  0x88800000
#define PLB4_BESR_CACHE_MASK_U  0x8880
#define PLB4_BESR_IO_MASK 0x00088888
#define PLB4_BESR_IO_MASK_U 0x0008
#define PLB4_BESR_IO_MASK_L     0x8888
#define PLB4_BESR_PTE0    0x80000000  /* master 0:  Icache read */
#define PLB4_BESR_R_W0    0x40000000
#define PLB4_BESR_FLK0    0x20000000
#define PLB4_BESR_ALK0    0x10000000
#define PLB4_BESR_PTE1    0x08000000  /* master 1:  Dcache read */
#define PLB4_BESR_R_W1    0x04000000
#define PLB4_BESR_FLK1    0x02000000
#define PLB4_BESR_ALK1    0x01000000
#define PLB4_BESR_PTE2    0x00800000  /* master 2:  Dcache write */
#define PLB4_BESR_R_W2    0x00400000
#define PLB4_BESR_FLK2    0x00200000
#define PLB4_BESR_ALK2    0x00100000
#define PLB4_BESR_PTE3    0x00080000  /* master 3:  DMA1 controller */
#define PLB4_BESR_R_W3    0x00040000  /* 1 => read, 0 => write */
#define PLB4_BESR_FLK3    0x00020000
#define PLB4_BESR_ALK3    0x00010000
#define PLB4_BESR_PTE4    0x00008000  /* master 4:  PLB3 to PLB4 bridge */
#define PLB4_BESR_R_W4    0x00004000  /* 1 => read, 0 => write */
#define PLB4_BESR_FLK4    0x00002000
#define PLB4_BESR_ALK4    0x00001000

/* Defines for the PLBx_ACR register fields */
#define PLB_ACR_PPM_FIXED       0x00000000
#define PLB_ACR_PPM_FAIR        0x80000000
#define PLB_ACR_PPM_MASK        0x80000000

#else /* PPC440EP 2.0 */
#define PLB0_REVID      (PLB0_DCR_BASE+0x00)
#define PLB0_ACR        (PLB0_DCR_BASE+0x01)
#define PLB0_BESRL      (PLB0_DCR_BASE+0x02)
#define PLB0_BESRH      (PLB0_DCR_BASE+0x03)
#define PLB0_BEARL      (PLB0_DCR_BASE+0x04)
#define PLB0_BEARH      (PLB0_DCR_BASE+0x05)
#define PLB0_CCR        (PLB0_DCR_BASE+0x08)
#define PLB1_ACR        (PLB0_DCR_BASE+0x09)
#define PLB1_BESRL      (PLB0_DCR_BASE+0x0A)
#define PLB1_BESRH      (PLB0_DCR_BASE+0x0B)
#define PLB1_BEARL      (PLB0_DCR_BASE+0x0C)
#define PLB1_BEARH      (PLB0_DCR_BASE+0x0D)

/*------------- PLB0 BESRL definitions --------------------------------------*/
#define PLB_BESRL_ERROR_MASK    0x88888888
#define PLB_BESRL_CACHE_MASK    0x88800000
#define PLB_BESRL_CACHE_MASK_U  0x8880
#define PLB_BESRL_IO_MASK       0x00088888
#define PLB_BESRL_IO_MASK_U     0x0008
#define PLB_BESRL_IO_MASK_L     0x8888
#define PLB_BESRL_PTE0    0x80000000  /* master 0:  Icache read */
#define PLB_BESRL_R_W0    0x40000000
#define PLB_BESRL_FLK0    0x20000000
#define PLB_BESRL_ALK0    0x10000000
#define PLB_BESRL_PTE1    0x08000000  /* master 1:  Dcache read */
#define PLB_BESRL_R_W1    0x04000000
#define PLB_BESRL_FLK1    0x02000000
#define PLB_BESRL_ALK1    0x01000000
#define PLB_BESRL_PTE2    0x00800000  /* master 2:  Dcache write */
#define PLB_BESRL_R_W2    0x00400000
#define PLB_BESRL_FLK2    0x00200000
#define PLB_BESRL_ALK2    0x00100000
#define PLB_BESRL_PTE3    0x00080000  /* master 3:  DMA1 controller */
#define PLB_BESRL_R_W3    0x00040000  /* 1 => read, 0 => write */
#define PLB_BESRL_FLK3    0x00020000
#define PLB_BESRL_ALK3    0x00010000
#define PLB_BESRL_PTE4    0x00008000  /* master 4:  PLB3 to PLB4 bridge */
#define PLB_BESRL_R_W4    0x00004000  /* 1 => read, 0 => write */
#define PLB_BESRL_FLK4    0x00002000
#define PLB_BESRL_ALK4    0x00001000
#define PLB_BESRL_PTE5    0x00000800
#define PLB_BESRL_R_W5    0x00000400
#define PLB_BESRL_FLK5    0x00000200
#define PLB_BESRL_ALK5    0x00000100
#define PLB_BESRL_PTE6    0x00000080
#define PLB_BESRL_R_W6    0x00000040
#define PLB_BESRL_FLK6    0x00000020
#define PLB_BESRL_ALK6    0x00000010
#define PLB_BESRL_PTE7    0x00000008
#define PLB_BESRL_R_W7    0x00000004
#define PLB_BESRL_FLK7    0x00000002
#define PLB_BESRL_ALK7    0x00000001


/* Defines for the PLB0_ACR register fields */
#define   PLB_ACR_PPM_MASK             0xF0000000
#define   PLB_ACR_PPM_FIXED            0x00000000
#define   PLB_ACR_PPM_FAIR             0xD0000000
#define   PLB_ACR_HBU_MASK             0x08000000
#define   PLB_ACR_HBU_DISABLED         0x00000000
#define   PLB_ACR_HBU_ENABLED          0x08000000
#define   PLB_ACR_RDP_MASK             0x06000000
#define   PLB_ACR_RDP_DISABLED         0x00000000
#define   PLB_ACR_RDP_2DEEP            0x02000000
#define   PLB_ACR_RDP_3DEEP            0x04000000
#define   PLB_ACR_RDP_4DEEP            0x06000000
#define   PLB_ACR_WRP_MASK             0x01000000
#define   PLB_ACR_WRP_DISABLED         0x00000000
#define   PLB_ACR_WRP_2DEEP            0x01000000

#endif /* PPC440EP 1.0 */

/*
 * PLB4 to PLB3 Bridge Control DCR Registers (bridge out).
 */

#define P4P3_ESR0       (P4P3B_DCR_BASE+0x0) /* Read/Clear PLB to PLB Bridge Error Status 0 */
#define P4P3_ESR0_SET   (P4P3B_DCR_BASE+0x1) /* Set PLB to PLB Bridge Error Status 0 */
#define P4P3_EADR       (P4P3B_DCR_BASE+0x2) /* Read/Clear PLB to PLB Bridge Error Address */
#define P4P3_EUADR      (P4P3B_DCR_BASE+0x3) /* Read/Clear PLB to PLB Bridge Error Upper Address */
#define P4P3_ESR1       (P4P3B_DCR_BASE+0x4) /* Read/Clear PLB to PLB Bridge Error Status 1 */
#define P4P3_ESR1_SET   (P4P3B_DCR_BASE+0x5) /* Set PLB to PLB Bridge Error Status 1 */
#define P4P3_CONFG      (P4P3B_DCR_BASE+0x6) /* Read/Clear/Set PLB to PLB Bridge Configuration */
#define P4P3_CONFG_PRI_MASK    0x00C00000
#define P4P3_CONFG_PRI_0       0x00000000
#define P4P3_CONFG_PRI_1       0x00400000
#define P4P3_CONFG_PRI_2       0x00800000
#define P4P3_CONFG_PRI_3       0x00C00000
#define P4P3_PIC        (P4P3B_DCR_BASE+0x7) /* Read/Clear/Set PLB to PLB Bridge Priority */
#define P4P3_PEIR       (P4P3B_DCR_BASE+0x8) /* Read/Clear/Set PLB to PLB Bridge Parity Error Interrupt */
#define P4P3_REV        (P4P3B_DCR_BASE+0xA) /* Read/Clear PLB to PLB Bridge Revision ID */

/*------------- PLB4 to PLB3 Bridge ESR0 bits definitions --------------------------------------*/
#define P4P3_ESR0_ERROR_MASK    0xC0C0C0C0
#define P4P3_ESR0_CACHE_MASK    0xC0C0C000
#define P4P3_ESR0_CACHE_MASK_U  0xC0C0
#define P4P3_ESR0_CACHE_MASK_L  0xC000
#define P4P3_ESR0_IO_MASK       0x000000C0
#define P4P3_ESR0_IO_MASK_L       0x00C0
#define P4P3_ESR0_SSET0_MASK    0xC0000000  /* master 0:  Icache read */
#define P4P3_ESR0_R_W0          0x20000000
#define P4P3_ESR0_FLK0          0x10000000
#define P4P3_ESR0_ALK0          0x08000000
#define P4P3_ESR0_WIRQ0         0x04000000
#define P4P3_ESR0_PAR0_MASK     0x03000000
#define P4P3_ESR0_SSET1_MASK    0x00C00000  /* master 1:  Dcache read */
#define P4P3_ESR0_R_W1          0x00200000
#define P4P3_ESR0_FLK1          0x00100000
#define P4P3_ESR0_ALK1          0x00080000
#define P4P3_ESR0_WIRQ1         0x00040000
#define P4P3_ESR0_PAR1_MASK     0x00030000
#define P4P3_ESR0_SSET2_MASK    0x0000C000  /* master 2:  Dcache write */
#define P4P3_ESR0_R_W2          0x00002000
#define P4P3_ESR0_FLK2          0x00001000
#define P4P3_ESR0_ALK2          0x00000800
#define P4P3_ESR0_WIRQ2         0x00000400
#define P4P3_ESR0_PAR2_MASK     0x00000300
#define P4P3_ESR0_SSET3_MASK    0x000000C0  /* master 3:  DMA1 controller */
#define P4P3_ESR0_R_W3          0x00000020  /* 1 => read, 0 => write */
#define P4P3_ESR0_FLK3          0x00000010
#define P4P3_ESR0_ALK3          0x00000008
#define P4P3_ESR0_WIRQ3         0x00000004
#define P4P3_ESR0_PAR3_MASK     0x00000003

/*------------- PLB4 to PLB3 Bridge ESR1 bits definitions --------------------------------------*/
#define P4P3_ESR1_ERROR_MASK    0xC0C0C0C0
#define P4P3_ESR1_IO_MASK       0xC0000000
#define P4P3_ESR1_SSET4_MASK    0xC0000000  /* master 4:  PLB3 to PLB4 bridge */
#define P4P3_ESR1_R_W4          0x20000000  /* 1 => read, 0 => write */
#define P4P3_ESR1_FLK4          0x10000000
#define P4P3_ESR1_ALK4          0x08000000
#define P4P3_ESR1_WIRQ4         0x04000000
#define P4P3_ESR1_PAR4_MASK     0x03000000
#define P4P3_ESR1_SSET5_MASK    0x00C00000  /* master 5:  not used */
#define P4P3_ESR1_R_W5          0x00200000
#define P4P3_ESR1_FLK5          0x00100000
#define P4P3_ESR1_ALK5          0x00080000
#define P4P3_ESR1_WIRQ5         0x00040000
#define P4P3_ESR1_PAR5_MASK     0x00030000
#define P4P3_ESR1_SSET6_MASK    0x0000C000  /* master 6:  not used */
#define P4P3_ESR1_R_W6          0x00002000
#define P4P3_ESR1_FLK6          0x00001000
#define P4P3_ESR1_ALK6          0x00000800
#define P4P3_ESR1_WIRQ6         0x00000400
#define P4P3_ESR1_PAR6_MASK     0x00000300
#define P4P3_ESR1_SSET7_MASK    0x000000C0  /* master 7:  not used */
#define P4P3_ESR1_R_W7          0x00000020
#define P4P3_ESR1_FLK7          0x00000010
#define P4P3_ESR1_ALK7          0x00000008
#define P4P3_ESR1_WIRQ7         0x00000004
#define P4P3_ESR1_PAR7_MASK     0x00000003

/*
 * PLB3 to PLB4 Bridge Control DCR Registers (bridge in).
 */

#define P3P4_ESR0       (P3P4B_DCR_BASE+0x0) /* Read/Clear PLB to PLB Bridge Error Status 0 */
#define P3P4_ESR0_SET   (P3P4B_DCR_BASE+0x1) /* Set PLB to PLB Bridge Error Status 0 */
#define P3P4_EADR       (P3P4B_DCR_BASE+0x2) /* Read/Clear PLB to PLB Bridge Error Address */
#define P3P4_EUADR      (P3P4B_DCR_BASE+0x3) /* Read/Clear PLB to PLB Bridge Error Upper Address */
#define P3P4_ESR1       (P3P4B_DCR_BASE+0x4) /* Read/Clear PLB to PLB Bridge Error Status 1 */
#define P3P4_ESR1_SET   (P3P4B_DCR_BASE+0x5) /* Set PLB to PLB Bridge Error Status 1 */
#define P3P4_CONFG      (P3P4B_DCR_BASE+0x6) /* Read/Clear/Set PLB to PLB Bridge Configuration */
#define P3P4_CONFG_PRI_MASK    0x00C00000
#define P3P4_CONFG_PRI_0       0x00000000
#define P3P4_CONFG_PRI_1       0x00400000
#define P3P4_CONFG_PRI_2       0x00800000
#define P3P4_CONFG_PRI_3       0x00C00000
#define P3P4_PIC        (P3P4B_DCR_BASE+0x7) /* Read/Clear/Set PLB to PLB Bridge Priority */
#define P3P4_PEIR       (P3P4B_DCR_BASE+0x8) /* Read/Clear/Set PLB to PLB Bridge Parity Error Interrupt */
#define P3P4_REV        (P3P4B_DCR_BASE+0xA) /* Read/Clear PLB to PLB Bridge Revision ID */


/*
 * PLB4 to OPB Bridge Control DCR Registers (bridge out).
 */
#define P4OB_BESR0      (P4OB_DCR_BASE+0x0)
#define P4OB_BEARL      (P4OB_DCR_BASE+0x2)
#define P4OB_BEARH      (P4OB_DCR_BASE+0x3)
#define P4OB_BESR1      (P4OB_DCR_BASE+0x4)
#define P4OB_CONFIG0    (P4OB_DCR_BASE+0x6)
#define P4OB_LATENCY0   (P4OB_DCR_BASE+0x8)
#define P4OB_REV        (P4OB_DCR_BASE+0xa)

/*------------- P4OB BESR0 bits definitions --------------------------------------*/
#define P4OB_BESR0_ERROR_MASK 0xC0C0C0C0
#define P4OB_BESR0_CACHE_MASK 0xC0C0C000
#define P4OB_BESR0_CACHE_MASK_U 0xC0C0
#define P4OB_BESR0_CACHE_MASK_L     0xC000
#define P4OB_BESR0_IO_MASK  0x000000C0
#define P4OB_BESR0_IO_MASK_L  0x00C0
#define P4OB_BESR0_PTE0_MASK  0xC0000000  /* master 0:  Icache read */
#define P4OB_BESR0_R_W0   0x20000000
#define P4OB_BESR0_FLK0   0x10000000
#define P4OB_BESR0_ALK0   0x08000000
#define P4OB_BESR0_WIRQ0  0x04000000
#define P4OB_BESR0_PTE1_MASK  0x00C00000  /* master 1:  Dcache read */
#define P4OB_BESR0_R_W1   0x00200000
#define P4OB_BESR0_FLK1   0x00100000
#define P4OB_BESR0_ALK1   0x00080000
#define P4OB_BESR0_WIRQ1  0x00040000
#define P4OB_BESR0_PTE2_MASK  0x0000C000  /* master 2:  Dcache write */
#define P4OB_BESR0_R_W2   0x00002000
#define P4OB_BESR0_FLK2   0x00001000
#define P4OB_BESR0_ALK2   0x00000800
#define P4OB_BESR0_WIRQ2  0x00000400
#define P4OB_BESR0_PTE3_MASK  0x000000C0  /* master 3:  DMA1 controller */
#define P4OB_BESR0_R_W3   0x00000020  /* 1 => read, 0 => write */
#define P4OB_BESR0_FLK3   0x00000010
#define P4OB_BESR0_ALK3   0x00000008
#define P4OB_BESR0_WIRQ3  0x00000004

/*------------- P4OB BESR1 bits definitions --------------------------------------*/
#define P4OB_BESR1_ERROR_MASK 0xC0C0C0C0
#define P4OB_BESR1_IO_MASK  0xC0000000
#define P4OB_BESR1_IO_MASK_U  0xC000
#define P4OB_BESR1_PTE4_MASK  0xC0000000  /* master 4:  PLB3 to PLB4 bridge */
#define P4OB_BESR1_R_W4   0x20000000  /* 1 => read, 0 => write */
#define P4OB_BESR1_FLK4   0x10000000
#define P4OB_BESR1_ALK4   0x08000000
#define P4OB_BESR1_WIRQ4  0x04000000
#define P4OB_BESR1_PTE5_MASK  0x00C00000  /* master 5:  Not used */
#define P4OB_BESR1_R_W5   0x00200000  /* 1 => read, 0 => write */
#define P4OB_BESR1_FLK5   0x00100000
#define P4OB_BESR1_ALK5   0x00080000
#define P4OB_BESR1_WIRQ5  0x00040000
#define P4OB_BESR1_PTE6_MASK  0x0000C000  /* master 6:  Not used */
#define P4OB_BESR1_R_W6   0x00002000  /* 1 => read, 0 => write */
#define P4OB_BESR1_FLK6   0x00001000
#define P4OB_BESR1_ALK6   0x00000800
#define P4OB_BESR1_WIRQ6  0x00000400
#define P4OB_BESR1_PTE7_MASK  0x000000C0  /* master 7:  Not used */
#define P4OB_BESR1_R_W7   0x00000020  /* 1 => read, 0 => write */
#define P4OB_BESR1_FLK7   0x00000010
#define P4OB_BESR1_ALK7   0x00000008
#define P4OB_BESR1_WIRQ7  0x00000004

/*
 * PLB3 to OPB Bridge Control DCR Registers (bridge out).
 */
#define P3OB_BESR0      (P3OB_DCR_BASE+0x0)
#define P3OB_BEAR       (P3OB_DCR_BASE+0x2)
#define P3OB_REV        (P3OB_DCR_BASE+0x3)
#define P3OB_BESR1      (P3OB_DCR_BASE+0x4)

/*------------- P3OB BESR0 definitions --------------------------------------*/
#define P3OB_BESR0_ERROR_MASK 0xC6318000
#define P3OB_BESR0_P4P3B_MASK 0xC0000000
#define P3OB_BESR0_P4P3B_MASK_U 0xC000
#define P3OB_BESR0_P4P3B_MASK_L 0x0000
#define P3OB_BESR0_IO_MASK  0x06318000
#define P3OB_BESR0_IO_MASK_U  0x0631
#define P3OB_BESR0_IO_MASK_L  0x8000
#define P3OB_BESR0_PTE0_MASK  0xC0000000  /* master 0:  PLB4 to PLB3 bridge */
#define P3OB_BESR0_R_W0   0x20000000
#define P3OB_BESR0_FLK0   0x10000000
#define P3OB_BESR0_ALK0   0x08000000
#define P3OB_BESR0_PTE1_MASK  0x06000000  /* master 1:  MAL */
#define P3OB_BESR0_R_W1   0x01000000
#define P3OB_BESR0_FLK1   0x00800000
#define P3OB_BESR0_ALK1   0x00400000
#define P3OB_BESR0_PTE2_MASK  0x00300000  /* master 2:  PCI bridge */
#define P3OB_BESR0_R_W2   0x00080000
#define P3OB_BESR0_FLK2   0x00040000
#define P3OB_BESR0_ALK2   0x00020000
#define P3OB_BESR0_PTE3_MASK  0x00018000  /* master 3:  DMA0 Controller */
#define P3OB_BESR0_R_W3   0x00004000  /* 1 => read, 0 => write */
#define P3OB_BESR0_FLK3   0x00002000
#define P3OB_BESR0_ALK3   0x00001000

/*------------- P3OB BESR1 definitions --------------------------------------*/
#define P3OB_BESR1_ERROR_MASK 0xC6318000
#define P3OB_BESR1_IO_MASK  0xC6318000
#define P3OB_BESR1_PTE4_MASK  0xC0000000  /* master 4:  OPB to PLB3 bridge */
#define P3OB_BESR1_R_W4   0x20000000
#define P3OB_BESR1_FLK4   0x10000000
#define P3OB_BESR1_ALK4   0x08000000
#define P3OB_BESR1_PTE5_MASK  0x06000000  /* master 5:  EBC Master */
#define P3OB_BESR1_R_W5   0x01000000
#define P3OB_BESR1_FLK5   0x00800000
#define P3OB_BESR1_ALK5   0x00400000
#define P3OB_BESR1_PTE6_MASK  0x00300000  /* master 6:  Not used */
#define P3OB_BESR1_R_W6   0x00080000
#define P3OB_BESR1_FLK6   0x00040000
#define P3OB_BESR1_ALK6   0x00020000
#define P3OB_BESR1_PTE7_MASK  0x00018000  /* master 7:  not used */
#define P3OB_BESR1_R_W7   0x00004000
#define P3OB_BESR1_FLK7   0x00002000
#define P3OB_BESR1_ALK7   0x00001000

/*
 * OPB to PLB3 Bridge Control DCR Registers (bridge in).
 */
#define OPB3_CTRL  (OP3B_DCR_BASE+0x0)
#define OPB3_STAT  (OP3B_DCR_BASE+0x1)
#define OPB3_REVID (OP3B_DCR_BASE+0x2)

#define OPB_PRI_0    0x00000000
#define OPB_PRI_1    0x40000000
#define OPB_PRI_2    0x80000000
#define OPB_PRI_3    0xC0000000
#define OPB_PRI_MASK 0xC0000000

/********
 * IIC
 ********/

/* IIC register offsets */
#define    IIC_MDBUF         0x00 /* Master Data Buffer */
#define    IIC_SDBUF         0x02 /* Slave Data Buffer */
#define    IIC_LMADR         0x04 /* Lo Master Address Register */
#define    IIC_HMADR         0x05 /* Hi Master Address Register */
#define    IIC_CNTL          0x06 /* Control Register */
#define    IIC_MDCNTL        0x07 /* Mode Control Register */
#define    IIC_STS           0x08 /* Status Register */
#define    IIC_EXTSTS        0x09 /* Extended Status Register */
#define    IIC_LSADR         0x0A /* Lo Slave Address Register */
#define    IIC_HSADR         0x0B /* Hi Slave Address Register */
#define    IIC_CLKDIV        0x0C /* Clock Divide Register */
#define    IIC_INTRMSK       0x0D /* Interrupt Enable Register */
#define    IIC_INTRENA       0x0D /* Interrupt Enable Register */
#define    IIC_XFRCNT        0x0E /* Xfer Count Register */
#define    IIC_XTCNTLSS      0x0F /* Extended Control and Slave Status Register */
#define    IIC_DIRECTCNTL    0x10 /* Direct Control Register */
#define    IIC_INTERRUPT     0x11 /* Interrupt Register */

/* mdbuf register */
#define IIC_MDBUF_FIFO_SIZE     4 /* Master Data Buffer size */

/* control register */
#define IIC_CNTL_HMT         0x80 /* Halt master transfer */
#define IIC_CNTL_AMD         0x40 /* 10/7 bit addressing 1: 10 bits 0: 7 bits */
#define IIC_CNTL_TCT_MASK    0x30 /*  Count bits mask */
#define IIC_CNTL_TCT_1       0x00
#define IIC_CNTL_TCT_2       0x10
#define IIC_CNTL_TCT_3       0x20
#define IIC_CNTL_TCT_4       0x30
#define IIC_CNTL_TCT_SHIFT   0x4   /* xfer count is bits 2:3 of byte */
#define IIC_CNTL_RPST        0x08  /* Repeat Start */
#define IIC_CNTL_CHT         0x04  /* chain transfers: set for more data */
#define IIC_CNTL_RW          0x02  /* read/write: set to read */
#define IIC_CNTL_PT          0x01  /* pending transfer: set to start */

/* mode control register */
#define IIC_MDCNTL_FSDB     0x80  /* flush slave data buffer */
#define IIC_MDCNTL_FMDB     0x40  /* flush master data buffer */
#define IIC_MDCNTL_EGC      0x20  /* enable general call */
#define IIC_MDCNTL_FSM      0x10  /* Fast/Standard Mode */
#define IIC_MDCNTL_ESM      0x08  /* Enable Slave Mode */
#define IIC_MDCNTL_EINT     0x04  /* Enable Interrupt */
#define IIC_MDCNTL_EUBS     0x02  /* exit unknown IIC bus state */
#define IIC_MDCNTL_HSCL     0x01  /* hold IIC serial clock low */

/* status register */
#define IIC_STS_SSET        0x80  /* Slave status set */
#define IIC_STS_SLR         0x40  /* Sleep request */
#define IIC_STS_MDH         0x20  /* Master data buffer has data */
#define IIC_STS_MDF         0x10  /* Master data buffer full */
#define IIC_STS_SCMP        0x08  /* Halted or stopped; set to clear */
#define IIC_STS_ERR         0x04  /* error detected */
#define IIC_STS_IRQA        0x02  /* irq active; set to clear */
#define IIC_STS_PT          0x01  /* pending transfer */

/* extended status register */
#define IIC_EXTSTS_IRQP       0x80  /* Illegal start or stop in a slave transfer */
#define IIC_EXTSTS_BCS_MASK 0x70  /* Bus Control State mask */
#define IIC_IN_ERROR1_STATE   0     /* I2C ctlr should never be in this state */
#define IIC_SLAVE_SELECTED    0x10
#define IIC_SLAVE_TRANSFER    0x20
#define MASTER_XFER_STATE     0x30
#define IIC_BUS_FREE          0x40   /* bcs = 100  */
#define IIC_BUS_BUSY          0x50
#define IIC_STATE_UNKNOWN     0x60   /* When found in this state IIC ctrl can be restarted */
#define IIC_IN_ERROR2_STATE   0x70   /* IIC ctlr should never be in this state */
#define IIC_EXTSTS_IRQD       0x08  /* Illegal start or stop in a master transfer */
#define IIC_EXTSTS_LA         0x04  /* lost arbitration: set on init */
#define IIC_EXTSTS_ERR_MASK   0x03
#define IIC_EXTSTS_ICT        0x02  /* incomplete xfer: set on init */
#define IIC_EXTSTS_XFRA       0x01  /* Address non-acknowledged: set on init */

/* interrupt enable register */
#define IIC_INTRENA_IRQSC     0x80  /* Enable IRQ on slave read complete */
#define IIC_INTRENA_IRQSR     0x40  /* Enable IRQ on slave read needs service */
#define IIC_INTRENA_IRQSW     0x20  /* Enable IRQ on slave write complete */
#define IIC_INTRENA_IRQSVW    0x10  /* Enable IRQ on slave write needs service */
#define IIC_INTRENA_IRQH      0x08  /* Enable IRQ on halt executed */
#define IIC_INTRENA_IRQI      0x04  /* Enable IRQ on incomplete transfer */
#define IIC_INTRENA_IRQA      0x02  /* Enable IRQ on address non-acknowledged */
#define IIC_INTRENA_IRQM      0x01  /* Enable IRQ on requested master transfer complete */

/* Xfer Count register */
#define IIC_XFRCNT_STC_MASK   0x70  /* Slave Transfer Count MAsk */
#define IIC_XFRCNT_MTC_MASK   0x07  /* Master Transfer Count MAsk */

/* extended control and slave status */
#define IIC_XTCNTLSS_SRC     0x80  /* slave read complete */
#define IIC_XTCNTLSS_SRS     0x40  /* slave read needs service */
#define IIC_XTCNTLSS_SWC     0x20  /* slave write complete */
#define IIC_XTCNTLSS_SWS     0x10  /* slave write needs service */
#define IIC_XTCNTLSS_SDBD    0x08  /* Slave data buffer has data */
#define IIC_XTCNTLSS_SDBF    0x04  /* Slave data buffer full */
#define IIC_XTCNTLSS_SRST    0x01  /* Soft reset */

/* Direct control register */
#define IIC_DIRECTCNTL_CSDA  0x08  /* directly control IIC_SDA_Out */
#define IIC_DIRECTCNTL_CSDL  0x04  /* directly control IIC_SCL_Out */
#define IIC_DIRECTCNTL_SDA   0x02  /* directly control IIC_SDA_In */
#define IIC_DIRECTCNTL_CSL   0x01  /* directly control IIC_SCL_In */

/* Interrupt register */
#define IIC_INTERRUPT_IRQP  0x02  /* IRQ pending */
#define IIC_INTERRUPT_IRQD  0x01  /* IRQ on-deck */


#define SPI0_SPMODE_OFF     0x00   /* R/W Loop                  */
#define SPI0_RXDATA_OFF     0x01   /* R   Received data         */
#define SPI0_TXDATA_OFF     0x02   /* R/W Transmit data         */
#define SPI0_SPCOM_OFF      0x03   /* R/W STR                   */
#define SPI0_STATUS_OFF     0x04   /* R   Received byte ready   */
#define SPI0_CDM_OFF        0x06   /* R/W Clock Divisor Modulus */


/* 440EP UARTs Base Address definitions.  */
#define UART0_PHY_START (OCP_PHY_START + OCP_UART0_OFF)
#define UART1_PHY_START (OCP_PHY_START + OCP_UART1_OFF)
#define UART2_PHY_START (OCP_PHY_START + OCP_UART2_OFF)
#define UART3_PHY_START (OCP_PHY_START + OCP_UART3_OFF)

#define UART_REG_ADDR_INTERVAL   1


/*
 * PCI address map
 */

/*
 *  Base address of the PCI core in the 440EP.
 *  The physical address is 0x0.E8000000
 * 
 * There are two PCI I/O areas:
 *    0x0.E8000000  to  0x0.E800FFFF    (64k)
 *    0x0.E8800000  to  0x0.EBFFFFFF    (56M)
 */

/*
 * PCI memory space from the PLB.
 * PLB to PCI address translation is controlled by the 3 pairs of PTM registers.
 */

/* PCI memory1 with prefetch */
#define PCI_MEMORY_PHY_START_HIGH   PCIM0_PHY_START_HIGH
#define PCI_MEMORY_PHY_START_LOW    PCIM0_PHY_START_LOW

/* PCI memory2 without prefetch */
#define PCI_MEMORY2_PHY_START_HIGH   0
#define PCI_MEMORY2_PHY_START_LOW    0xB0000000

/* PCI memory3 with prefetch */
#define PCI_MEMORY3_PHY_START_HIGH   0
#define PCI_MEMORY3_PHY_START_LOW    0xC0000000

/* PCI memory4 with prefetch */
#define PCI_MEMORY4_PHY_START_HIGH   0
#define PCI_MEMORY4_PHY_START_LOW    0xD0000000

/*
 * PCI I/O space from the PLB and the PCI translation.
 */

/* 64k PCI I/O space */
#define PLB_PCI_IO_REGION_1_SIZE            0x00010000          
#define PLB_PCI_IO_REGION_1_MAP_START_LOW   PCIIO0_PHY_START_LOW
#define PLB_PCI_IO_REGION_1_MAP_START_HIGH  0
#define PLB_PCI_IO_REGION_1_MAP_SIZE        PLB_PCI_IO_REGION_1_SIZE

/* 56M PCI I/O space */
#define PLB_PCI_IO_REGION_2_SIZE            0x03800000 
#define PLB_PCI_IO_REGION_2_MAP_START_LOW   (PCIIO0_PHY_START_LOW + 0x08800000)
#define PLB_PCI_IO_REGION_2_MAP_START_HIGH  0
#define PLB_PCI_IO_REGION_2_MAP_SIZE        0x00100000     /* 1 MB mapped in MMU */
                                                           /* increase if needed */


#define PCI_IO_REGION_1_START       0x00000000     /* PCI side */
#define PCI_IO_REGION_1_END         (PCI_IO_REGION_1_START+(PCI_IO_REGION_1_SIZE-1))
#define PCI_IO_REGION_1_SIZE        0x00010000               /* 64K */
#define PCI_IO_REGION_2_START       0x00800000
#define PCI_IO_REGION_2_END         (PCI_IO_REGION_2_START+(PCI_IO_REGION_2_SIZE-1))
#define PCI_IO_REGION_2_SIZE        0x03800000               /* 56MB */


/* PCI config Address and data registers.  */
#define PCIC0_OFF                   0x06C00000     
#define PCIC0_CFGADDR_PHY_HIGH      0
#define PCIC0_CFGADDR_PHY_LOW       (PCIIO0_PHY_START_LOW + PCIC0_OFF) 
#define PCIC0_CFGADDR_OFF           0x5c00000
#define PCIC0_CFGDATA_OFF           (PCIC0_CFGADDR_OFF + 4)
#define PCI_CFG_MAP_SIZE            0x1000    

/* PCI Interrupt Ack/Special Cycle Register */
#define PCIC0_IASC0_OFF             0x06d00000
#define PCIC0_IASC0_PHY_HIGH        0
#define PCIC0_IASC0_PHY_LOW         (PCIIO0_PHY_START_LOW + PCIC0_IASC0_OFF)
#define PCI_INTERRUPT_ACK_MAP_SIZE  0x1000 

/* PP Bridge Local Config registers space */
#define PP_BRIDGE_CR_OFF            0x07400000
#define PP_BRIDGE_CR_PHY_HIGH       0
#define PP_BRIDGE_CR_PHY_LOW        (PCIIO0_PHY_START_LOW + PP_BRIDGE_CR_OFF) 
#define PP_BRIDGE_CR_MAP_SIZE       0x1000 


/*
 * Register addresses for the 3 sets of PCI Master Mode (PMM)
 * local configuration registers.  These registers control how local memory
 * addresses are translated to PCI memory addresses when the chip is a
 * PCI bus master (initiator).  The contents of the PMMxLA register must be
 * set to an address that is within the PCI memory range
 * (PCI_MEMORY_START - PCI_MEMORY_END).
 */
#define PCIL_PMM0LA        (PP_BRIDGE_CR+0x00)       /* Local Address                  */
#define PCIL_PMM0MA        (PP_BRIDGE_CR+0x04)       /* Mask/Attribute register        */
#define PCIL_PMM0PCILA     (PP_BRIDGE_CR+0x08)       /* Local address is translated to */
#define PCIL_PMM0PCIHA     (PP_BRIDGE_CR+0x0C)       /* this 64 bit PCI address        */

#define PCIL_PMM1LA        (PP_BRIDGE_CR+0x10)       /* Local Address                  */
#define PCIL_PMM1MA        (PP_BRIDGE_CR+0x14)       /* Mask/Attribute register        */
#define PCIL_PMM1PCILA     (PP_BRIDGE_CR+0x18)       /* Local address is translated to */
#define PCIL_PMM1PCIHA     (PP_BRIDGE_CR+0x1C)       /* this 64 bit PCI address        */

#define PCIL_PMM2LA        (PP_BRIDGE_CR+0x20)       /* Local Address                  */
#define PCIL_PMM2MA        (PP_BRIDGE_CR+0x24)       /* Mask/Attribute register        */
#define PCIL_PMM2PCILA     (PP_BRIDGE_CR+0x28)       /* Local address is translated to */
#define PCIL_PMM2PCIHA     (PP_BRIDGE_CR+0x2C)       /* this 64 bit PCI address        */

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
 * Register addresses for the 2 sets of PCI Target Mode (PTM)
 * local configuration registers.  These registers control how PCI memory
 * addresses are translated to Local memory addresses when the chip is a
 * PCI bus target.  PTMxLA must be set to an address that is within the
 * Local SDRAM Memory or ROM regions (0x00000000 - 7FFFFFFF, or
 * F0000000 - FFFFFFFF).
 */
#define PCIL_PTM1MS        (PP_BRIDGE_CR+0x30)       /* Memory Size/Attribute register */
#define PCIL_PTM1LA        (PP_BRIDGE_CR+0x34)       /* Local Address                  */

#define PCIL_PTM2MS        (PP_BRIDGE_CR+0x38)       /* Memory Size/Attribute register */
#define PCIL_PTM2LA        (PP_BRIDGE_CR+0x3C)       /* Local Address                  */

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
 * Chip-specific PCI configuration register offsets.  These are found
 * immediately following the architected 64 byte PCI configuration header.
 */
#define PCI_CFG_ICS          0x44       /* PCI Interrupt Control/Status       */
#define PCI_CFG_ERREN        0x48       /* Error Enable                       */
#define PCI_CFG_ERRSTS       0x49       /* Error Status                       */
#define PCI_CFG_BRDGOPT1     0x4A       /* Bridge Options 1                   */                    

/* Value to access indirectly this Reg from PCI_CFGADDR (Little endian) */
#define PCI_CFG_BOPT1PK      0x4A000080 /* Bridge Options 1 access by packing EN,BN,FN,RN */
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

/* Needed bits definition for PCI_CFG_BRDGOPT1 register (Litle endian) */
#define   BRDGOPT1_PLREQ      0x6000
#define     BRDGOPT1_PLREQ_0  0x0000
#define     BRDGOPT1_PLREQ_1  0x2000
#define     BRDGOPT1_PLREQ_2  0x4000
#define     BRDGOPT1_PLREQ_3  0x6000



/* Arbiters have a Priority register and a Control register. */

#define OPBA_PR_OFF            (0x00)      /* Priority register */
#define OPBA_CR_OFF            (0x01)      /* Control register  */

/*
 * Base Address of OnChip Peripheral Bus Arbiters.
 * Arbiter 0 is connnected to PLB3.
 * Arbiter 1 is connnected to PLB4.
 */
#define OPBA0_BASE_PHY_HIGH     OCP_PHY_START_HIGH
#define OPBA0_BASE_PHY_LOW      OCP_PHY_START_LOW
#define OPBA0_PR_PHY_ADRS       (OPBA0_BASE_PHY_LOW + OPBA_PR_OFF) 
#define OPBA0_CR_PHY_ADRS       (OPBA0_BASE_PHY_LOW + OPBA_CR_OFF)
#define OPBA1_PR_PHY_ADRS      (OPBA1_PHY_START + OPBA_PR_OFF) 
#define OPBA1_CR_PHY_ADRS      (OPBA1_PHY_START + OPBA_CR_OFF)

/*
 * On-Chip Peripheral Bus 0 access
 * The OPB0 is connected to PLB4. 
 * Access to the OPB0 peripherals * is controlled by the arbiter at 0x0.50000000.
 * The first (and only) peripheral is USB2.0 and is located at 0x0.50000100.
 */
#define OPB0_PHY_START_HIGH    OPBA1_PHY_START_HIGH
#define OPB0_PHY_START_LOW     OPBA1_PHY_START_LOW 




/*
 * Timer clock divider ratios (in 2^n)
 */
#define FIT_DIVIDER_TAP_3 25
#define FIT_DIVIDER_TAP_2 21
#define FIT_DIVIDER_TAP_1 17
#define FIT_DIVIDER_TAP_0 13

#define WDT_DIVIDER_TAP_3 33
#define WDT_DIVIDER_TAP_2 29
#define WDT_DIVIDER_TAP_1 25
#define WDT_DIVIDER_TAP_0 21


/*
 * Physical address of Nand Flash Controller on EBC
 */
#define NDFC_PHYS_ADRS_HIGH   0x0
#define NDFC_PHYS_ADRS_LOW    0x90000000


/* 
 * 440EP External Bus Controller 0 (used for MMU)
 */
#define EBC0_PHY_START_ADRS_HIGH     0x0
#define EBC0_PHY_START_ADRS_LOW      0x80000000


/* Internal peripherals connected to OPB4 */ 
 #define INTERN_OPB4_PERIPHERAL_START_HIGH  0
 #define INTERN_OPB4_PERIPHERAL_START_LOW   OPB4A_BASE       
 #define INTERN_OPB4_PERIPHERAL_PHY_HIGH    0
 #define INTERN_OPB4_PERIPHERAL_PHY_LOW     OPB4A_PHY_START_LOW        
 #define INTERN_OPB4_PERIPHERAL_SIZE        0x1000 


/*
 * Static interrupt vectors/levels.  These also correspond to bit numbers in
 * many of the registers of the Universal Interrupt Controller.
 */

/* interrupt vectors handled by UIC0 */

#define INT_VEC_UIC0_BASE         0
#define INT_VEC_UART0             0
#define INT_VEC_UART1             1
#define INT_VEC_IIC0              2
#define INT_VEC_UART2             3
#define INT_VEC_UART3             4
#define INT_VEC_PCI               5
#define INT_VEC_PCI_PWR           6
#define INT_VEC_IIC1              7
#define INT_VEC_SCP               8
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

/* interrupt vectors handled by UIC1 */

#define INT_VEC_UIC1_BASE         32
#define INT_VEC_MAL_SERR          (INT_VEC_UIC1_BASE+0)
#define INT_VEC_MAL_TXDE          (INT_VEC_UIC1_BASE+1)
#define INT_VEC_MAL_RXDE          (INT_VEC_UIC1_BASE+2)
#define INT_VEC_SDRAM_ECC_UE      (INT_VEC_UIC1_BASE+3)
#define INT_VEC_SDRAM_ECC_CE      (INT_VEC_UIC1_BASE+4)
#define INT_VEC_EBCO              (INT_VEC_UIC1_BASE+5)
#define INT_VEC_NDFC              (INT_VEC_UIC1_BASE+6)
#define INT_VEC_OPB               (INT_VEC_UIC1_BASE+7)
#define INT_VEC_USB1_1_HOST_INT1  (INT_VEC_UIC1_BASE+8)
#define INT_VEC_USB1_1_HOST_INT2  (INT_VEC_UIC1_BASE+9)
#define INT_VEC_PLB3_4_MIRQ0      (INT_VEC_UIC1_BASE+10)
#define INT_VEC_PLB3_4_MIRQ1      (INT_VEC_UIC1_BASE+11)
#define INT_VEC_PLB3_4_MIRQ2      (INT_VEC_UIC1_BASE+12)
#define INT_VEC_PLB3_4_MIRQ3      (INT_VEC_UIC1_BASE+13)
#define INT_VEC_PLB3_4_MIRQ4      (INT_VEC_UIC1_BASE+14)
#define INT_VEC_PLB3_4_MIRQ5      (INT_VEC_UIC1_BASE+15)
#define INT_VEC_DMA1_CH0          (INT_VEC_UIC1_BASE+16)
#define INT_VEC_DMA1_CH1          (INT_VEC_UIC1_BASE+17)
#define INT_VEC_EXT_IRQ_7         (INT_VEC_UIC1_BASE+18)
#define INT_VEC_EXT_IRQ_8         (INT_VEC_UIC1_BASE+19)
#define INT_VEC_EXT_IRQ_9         (INT_VEC_UIC1_BASE+20)
#define INT_VEC_DMA1_CH2          (INT_VEC_UIC1_BASE+21)
#define INT_VEC_DMA1_CH3          (INT_VEC_UIC1_BASE+22)
#define INT_VEC_USB2_DEV_INT      (INT_VEC_UIC1_BASE+23)
#define INT_VEC_EXT_SRE           (INT_VEC_UIC1_BASE+24)
#define INT_VEC_GPT_DCT           (INT_VEC_UIC1_BASE+25)
#define INT_VEC_PPM               (INT_VEC_UIC1_BASE+26)
#define INT_VEC_PCI_PERR          (INT_VEC_UIC1_BASE+27)
#define INT_VEC_ENET_0            (INT_VEC_UIC1_BASE+28)
#define INT_VEC_ENET_WAKEUP_0     (INT_VEC_UIC1_BASE+29)
#define INT_VEC_ENET_1            (INT_VEC_UIC1_BASE+30)
#define INT_VEC_ENET_WAKEUP_1     (INT_VEC_UIC1_BASE+31)

/* interrupt levels handled by UIC0 */

#define INT_LVL_UART0             0
#define INT_LVL_UART1             1
#define INT_LVL_IIC0              2
#define INT_LVL_UART2             3
#define INT_LVL_UART3             4
#define INT_LVL_PCI               5
#define INT_LVL_PCI_PWR           6
#define INT_LVL_IIC1              7
#define INT_LVL_SCP               8
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

/* interrupt levels handled by UIC1 */

#define INT_LVL_UIC1_BASE         32
#define INT_LVL_MAL_SERR          (INT_LVL_UIC1_BASE+0)
#define INT_LVL_MAL_TXDE          (INT_LVL_UIC1_BASE+1)
#define INT_LVL_MAL_RXDE          (INT_LVL_UIC1_BASE+2)
#define INT_LVL_SDRAM_ECC_UE      (INT_LVL_UIC1_BASE+3)
#define INT_LVL_SDRAM_ECC_CE      (INT_LVL_UIC1_BASE+4)
#define INT_LVL_EBCO              (INT_LVL_UIC1_BASE+5)
#define INT_LVL_NDFC              (INT_LVL_UIC1_BASE+6)
#define INT_LVL_OPB               (INT_LVL_UIC1_BASE+7)
#define INT_LVL_USB1_1_HOST_INT1  (INT_LVL_UIC1_BASE+8)
#define INT_LVL_USB1_1_HOST_INT2  (INT_LVL_UIC1_BASE+9)
#define INT_LVL_PLB3_4_MIRQ0      (INT_LVL_UIC1_BASE+10)
#define INT_LVL_PLB3_4_MIRQ1      (INT_LVL_UIC1_BASE+11)
#define INT_LVL_PLB3_4_MIRQ2      (INT_LVL_UIC1_BASE+12)
#define INT_LVL_PLB3_4_MIRQ3      (INT_LVL_UIC1_BASE+13)
#define INT_LVL_PLB3_4_MIRQ4      (INT_LVL_UIC1_BASE+14)
#define INT_LVL_PLB3_4_MIRQ5      (INT_LVL_UIC1_BASE+15)
#define INT_LVL_DMA1_CH0          (INT_LVL_UIC1_BASE+16)
#define INT_LVL_DMA1_CH1          (INT_LVL_UIC1_BASE+17)
#define INT_LVL_EXT_IRQ_7         (INT_LVL_UIC1_BASE+18)
#define INT_LVL_EXT_IRQ_8         (INT_LVL_UIC1_BASE+19)
#define INT_LVL_EXT_IRQ_9         (INT_LVL_UIC1_BASE+20)
#define INT_LVL_DMA1_CH2          (INT_LVL_UIC1_BASE+21)
#define INT_LVL_DMA1_CH3          (INT_LVL_UIC1_BASE+22)
#define INT_LVL_USB2_DEV_INT      (INT_LVL_UIC1_BASE+23)
#define INT_LVL_EXT_SRE           (INT_LVL_UIC1_BASE+24)
#define INT_LVL_GPT_DCT           (INT_LVL_UIC1_BASE+25)
#define INT_LVL_PPM               (INT_LVL_UIC1_BASE+26)
#define INT_LVL_PCI_PERR          (INT_LVL_UIC1_BASE+27)
#define INT_LVL_ENET_0            (INT_LVL_UIC1_BASE+28)
#define INT_LVL_ENET_WAKEUP_0     (INT_LVL_UIC1_BASE+29)
#define INT_LVL_ENET_1            (INT_LVL_UIC1_BASE+30)
#define INT_LVL_ENET_WAKEUP_1     (INT_LVL_UIC1_BASE+31)


/*
 * PLL Voltage Controlled Oscillator (VCO) definitions
 * Maximum and minimum values for correct PLL operation.
 * Maximum and minimum values for correct CPU and Bus operation.
 */
#define VCO_MHZ_MIN     600
#define VCO_MHZ_MAX     1333
#define CPU_HZ_MIN      0
#define CPU_HZ_MAX      533333333
#define PLB_HZ_MIN      0
#define PLB_HZ_MAX      133333333
#define OPB_HZ_MIN      0
#define OPB_HZ_MAX      (PLB_HZ_MAX/2)
#define EXTBUS_HZ_MIN   0
#define EXTBUS_HZ_MAX   (PLB_HZ_MAX/2)
#define PCI_HZ_MIN      0
#define PCI_HZ_MAX      66
#define SER_HZ_MIN      0
#define SER_HZ_MAX      66

/* Header files for other cores in the 440EP */

#include "uicDcr.h"            /* Universal Interrupt Controller */
#include "malDcr.h"            /* Memory Access Layer DCR        */
#include "sdramDcr.h"          /* SDRAM Controller               */
#include "sdrDcr.h"            /* DCR reg definitions            */
#include "ebcDcr.h"            /* External Bus Controller        */
#include "dmaDcr.h"            /* DMA Controller                 */
#include "gpio.h"              /* GPIO                           */

#ifdef __cplusplus
    }
#endif

/* Validate and instantiate 440EP Machine Check Handler configuration */
#ifdef  INCLUDE_440X5_DCACHE_RECOVERY
# ifndef INCLUDE_440X5_PARITY_RECOVERY
#  define INCLUDE_440X5_PARITY_RECOVERY
# endif
# if (USER_D_CACHE_MODE != CACHE_DISABLED && defined(USER_D_CACHE_ENABLE))
#  undef  USER_D_CACHE_MODE
#  define USER_D_CACHE_MODE (CACHE_WRITETHROUGH)
# endif /* USER_D_CACHE_MODE != CACHE_DISABLED) */
#define _WRS_PPC440X5_CFG_DCR TRUE
#else /* INCLUDE_440X5_DCACHE_RECOVERY */
#define _WRS_PPC440X5_CFG_DCR FALSE
#endif  /* INCLUDE_440X5_DCACHE_RECOVERY */

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
# if  (VEC_BASE_ADRS == VEC_BASE_ALT_ADRS)
# error "INCLUDE_440X5_TLB_RECOVERY_MAX requires VEC_BASE_ADRS != VEC_BASE_ALT_ADRS"
# endif
# ifndef INCLUDE_440X5_TLB_RECOVERY
# define INCLUDE_440X5_TLB_RECOVERY
# endif
#endif

#ifdef  INCLUDE_440X5_TLB_RECOVERY
# ifndef INCLUDE_440X5_PARITY_RECOVERY
# define INCLUDE_440X5_PARITY_RECOVERY
# endif
# ifndef INCLUDE_MMU_BASIC
# define INCLUDE_MMU_BASIC
# endif
#define _WRS_PPC440X5_CFG_TLB TRUE
#else /* INCLUDE_440X5_TLB_RECOVERY */
#define _WRS_PPC440X5_CFG_TLB FALSE
#endif  /* INCLUDE_440X5_TLB_RECOVERY */

#ifdef  INCLUDE_440X5_MCH_LOGGER
# ifndef _PPC_440X5_MCH_LOG_RTN
# define _PPC_440X5_MCH_LOG_RTN (FUNCPTR)1
# endif  /* _PPC_440X5_MCH_LOG_RTN */
#else /* INCLUDE_440X5_MCH_LOGGER */
#undef  _PPC_440X5_MCH_LOG_RTN
#define _PPC_440X5_MCH_LOG_RTN  (FUNCPTR)NULL
#endif  /* INCLUDE_440X5_MCH_LOGGER */

#ifdef  INCLUDE_440X5_PARITY_RECOVERY
# ifndef INCLUDE_EXC_HANDLING
# define INCLUDE_EXC_HANDLING
# endif
# ifndef _ASMLANGUAGE
IMPORT BOOL exc440x5_parity, exc440x5_tlb, exc440x5_dcache;
IMPORT char * excVecBaseAltAdrs;
IMPORT FUNCPTR exc440x5_logger;
# endif
#define INCLUDE_SYS_HW_INIT_0
#define SYS_HW_INIT_0() exc440x5_parity = TRUE, exc440x5_tlb = _WRS_PPC440X5_CFG_TLB, exc440x5_dcache = _WRS_PPC440X5_CFG_DCR, excVecBaseAltAdrs = VEC_BASE_ALT_ADRS, exc440x5_logger = _PPC_440X5_MCH_LOG_RTN
#endif  /* INCLUDE_440X5_PARITY_RECOVERY */

#endif  /* INCppc440EPh */
