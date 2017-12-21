/* cprDcr.h - IBM Chip Clocking and POR DCR definitions */

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

   COPYRIGHT   I B M   CORPORATION 2002
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,18jan06,pch  fix typo in CPR0_ICFG_GTCL defn
01c,02nov04,dr   Fixed CPR0_CLKUPD_BSY_BUSY, document changes for apigen
01c,29sep04,mdo  Documentation fixes for apigen
01b,15aug03,jtp  Formal code inspection changes
01a,02dec02,saw  created
*/

/*
This file contains the definitions of the Chip Clocking and POR DCR registers
*/

#ifndef INCcprDcrh
#define INCcprDcrh

/*
 * Chip Clocking and POR (CPR0) DCR Registers.
 */

#define CPR0_CFGADDR  (CPR0_DCR_BASE+0x0)  /* CPRO Controller Addr Reg        */
#define CPR0_CFGDATA  (CPR0_DCR_BASE+0x1)  /* CPRO Controller Data Reg        */

/* Indirect access registers for CPR0 -- use CPR0_CFGADDR to access */

#define CPR0_CLKUPD   0x0020  /* Clocking Update                              */
#define CPR0_PLLC     0x0040  /* PLL Control                                  */
#define CPR0_PLLD     0x0060  /* PLL Divider                                  */
#define CPR0_PRIMAD   0x0080  /* Primary A Divider                            */
#define CPR0_PRIMBD   0x00A0  /* Primary B Divider                            */
#define CPR0_OPBD     0x00C0  /* OPB Clock Divider                            */
#define CPR0_PERD     0x00E0  /* Peripheral Clock Divider                      */
#define CPR0_MALD     0x0100  /* MAL Clock Divider                            */
#define CPR0_ICFG     0x0140  /* Initial Configuration                        */

/* Bit Definitions */

/* Bits for Clocking Update Register (CPR0_CLKUPD) */
#define CPR0_CLKUPD_BSY      0x80000000   /* Clocking update status   (Read)  */
#define  CPR0_CLKUPD_BSY_COMP 0x00000000  /* Clocking subsystem is stable     */
#define  CPR0_CLKUPD_BSY_BUSY 0x80000000  /* Clocking subsystem is busy       */
#define CPR0_CLKUPD_CUI      0x80000000   /* Clocking Update Immediate(Write) */
#define  CPR0_CLKUPD_CUI_DIS 0x00000000   /* Clocking update imm disabled     */
#define  CPR0_CLKUPD_CUI_EN  0x80000000   /* Clocking update imm disabled     */
#define CPR0_CLKUPD_CUD      0x40000000   /* Clocking Update Delay    (Write) */
#define  CPR0_CLKUPD_CUD_DIS 0x00000000   /* Clocking update delay disabled   */
#define  CPR0_CLKUPD_CUD_EN  0x40000000   /* Clocking update delay enabled    */

/* Bits for Initial Configuration Register (CPR0_ICFG) */
#define CPR0_ICFG_RLI        0x80000000   /* Reload Inhibit        Mask       */
#define  CPR0_ICFG_RLI_RST   0x00000000   /* Reset CPR regs from cfg src      */
#define  CPR0_ICFG_RLI_IGN   0x80000000   /* Ignore cfg src and preserve vals */
#define CPR0_ICFG_UDCD       0x00000004   /* UART0 DCD pin strap status       */
#define CPR0_ICFG_UDSR       0x00000002   /* UART0 DSR pin strap status       */
#define CPR0_ICFG_GTCL       0x00000001   /* GMC1TxCtl pin strap status       */

/* Bits for MAL Clock Divider Register (CPR0_MALD) */
#define CPR0_MALD_MALD       0x03000000   /* MAL Clock Divisor     Mask       */
#define  CPR0_MALD_MALDV0_4  0x00000000   /* MAL Clock Divisor 0 = 4          */
#define  CPR0_MALD_MALDV0_1  0x01000000   /* MAL Clock Divisor 0 = 1          */
#define  CPR0_MALD_MALDV0_2  0x02000000   /* MAL Clock Divisor 0 = 2          */
#define  CPR0_MALD_MALDV0_3  0x03000000   /* MAL Clock Divisor 0 = 3          */
#define   CPR0_MALD_MALDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_MALD_MALDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)

/* Bits for OPB Clock Divider Register (CPR0_OPBD) */
#define CPR0_OPBD_OPBD       0x03000000   /* OPB Clock Divisor     Mask       */
#define  CPR0_OPBD_OPBDV0_4  0x00000000   /* OPB Clock Divisor 0 = 4          */
#define  CPR0_OPBD_OPBDV0_1  0x01000000   /* OPB Clock Divisor 0 = 1          */
#define  CPR0_OPBD_OPBDV0_2  0x02000000   /* OPB Clock Divisor 0 = 2          */
#define  CPR0_OPBD_OPBDV0_3  0x03000000   /* OPB Clock Divisor 0 = 3          */
#define   CPR0_OPBD_OPBDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_OPBD_OPBDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)

/* Bits for Peripheral Clock Divider Register (CPR0_PERD) */
#define CPR0_PERD_PERD       0x03000000   /* PER Clock Divisor     Mask       */
#define  CPR0_PERD_PERDV0_4  0x00000000   /* PER Clock Divisor 0 = 4          */
#define  CPR0_PERD_PERDV0_1  0x01000000   /* PER Clock Divisor 0 = 1          */
#define  CPR0_PERD_PERDV0_2  0x02000000   /* PER Clock Divisor 0 = 2          */
#define  CPR0_PERD_PERDV0_3  0x03000000   /* PER Clock Divisor 0 = 3          */
#define   CPR0_PERD_PERDV0_ENCODE(n)   ((((unsigned long)(n))&0x03)<<24)
#define   CPR0_PERD_PERDV0_DECODE(n)   ((((((unsigned long)(n))>>24)-1)&0x03)+1)

/* Bits for PLL Control Register (CPR0_PLLC) */
#define CPR0_PLLC_RST        0x80000000   /* Reset Mask                       */
#define  CPR0_PLLC_RST_LOCK  0x00000000   /* PLL is allowed to lock           */
#define  CPR0_PLLC_RST_RST   0x80000000   /* PLL is forced into reset         */
#define CPR0_PLLC_ENG        0x40000000   /* Engage Mask                      */
#define  CPR0_PLLC_ENG_SYS   0x00000000   /* SysClk is source for PLL         */
#define  CPR0_PLLC_ENG_VCO   0x40000000   /* VCO is source for PLL            */
#define   CPR0_PLLC_ENG_ENCODE(n)      ((((unsigned long)(n))&0x01)<<30)
#define   CPR0_PLLC_ENG_DECODE(n)      ((((unsigned long)(n))>>30)&0x01)
#define CPR0_PLLC_SRC        0x20000000   /* PLL Feedback Source Mask         */
#define  CPR0_PLLC_SRC_PLLA  0x00000000   /* Fbk Src = PLLOUTA                */
#define  CPR0_PLLC_SRC_PLLB  0x20000000   /* Fbk Src = PLLOUTB                */
#define   CPR0_PLLC_SRC_ENCODE(n)      ((((unsigned long)(n))&0x01)<<29)
#define   CPR0_PLLC_SRC_DECODE(n)      ((((unsigned long)(n))>>29)&0x01)
#define CPR0_PLLC_SEL        0x07000000   /* Feedback Selection Mask          */
#define  CPR0_PLLC_SEL_PLL   0x00000000   /* Fbk Sel = PLL Output             */
#define  CPR0_PLLC_SEL_CPU   0x01000000   /* Fbk Sel = CPU                    */
#define  CPR0_PLLC_SEL_EBC   0x05000000   /* Fbk Sel = EBC                    */
#define   CPR0_PLLC_SEL_ENCODE(n)      ((((unsigned long)(n))&0x07)<<24)
#define   CPR0_PLLC_SEL_DECODE(n)      ((((unsigned long)(n))>>24)&0x07)
#define CPR0_PLLC_TUNE       0x000003FF   /* Tune Bits Mask                   */
#define   CPR0_PLLC_TUNE_ENCODE(n)     ((((unsigned long)(n))&0x3FF)<<0)
#define   CPR0_PLLC_TUNE_DECODE(n)     ((((unsigned long)(n))>>0)&0x3FF)

/* Bits for PLL Divider Register (CPR0_PLLD) */
#define CPR0_PLLD_FBDV       0x1F000000   /* Feedback Divisor      Mask       */
#define  CPR0_PLLD_FBDV_32   0x00000000   /* Feedback Divisor = 32            */
#define  CPR0_PLLD_FBDV_1    0x01000000   /* Feedback Divisor = 1             */
#define  CPR0_PLLD_FBDV_2    0x02000000   /* Feedback Divisor = 2             */
#define  CPR0_PLLD_FBDV_3    0x03000000   /* Feedback Divisor = 3             */
#define  CPR0_PLLD_FBDV_4    0x04000000   /* Feedback Divisor = 4             */
/* ... */
#define  CPR0_PLLD_FBDV_31   0x0F000000   /* Feedback Divisor = 31            */
#define   CPR0_PLLD_FBDV_ENCODE(n)     ((((unsigned long)(n))&0x1F)<<24)
#define   CPR0_PLLD_FBDV_DECODE(n)     ((((((unsigned long)(n))>>24)-1)&0x1F)+1)
#define CPR0_PLLD_FWDVA      0x000F0000   /* Forward Divisor A     Mask       */
#define  CPR0_PLLD_FWDVA_16  0x00000000   /* Forward Divisor A = 16           */
#define  CPR0_PLLD_FWDVA_1   0x00010000   /* Forward Divisor A = 1            */
#define  CPR0_PLLD_FWDVA_2   0x00020000   /* Forward Divisor A = 2            */
#define  CPR0_PLLD_FWDVA_3   0x00030000   /* Forward Divisor A = 3            */
#define  CPR0_PLLD_FWDVA_4   0x00040000   /* Forward Divisor A = 4            */
#define  CPR0_PLLD_FWDVA_5   0x00050000   /* Forward Divisor A = 5            */
#define  CPR0_PLLD_FWDVA_6   0x00060000   /* Forward Divisor A = 6            */
#define  CPR0_PLLD_FWDVA_7   0x00070000   /* Forward Divisor A = 7            */
#define  CPR0_PLLD_FWDVA_8   0x00080000   /* Forward Divisor A = 8            */
#define  CPR0_PLLD_FWDVA_10  0x000A0000   /* Forward Divisor A = 10           */
#define  CPR0_PLLD_FWDVA_12  0x000C0000   /* Forward Divisor A = 12           */
#define  CPR0_PLLD_FWDVA_14  0x000D0000   /* Forward Divisor A = 14           */
#define   CPR0_PLLD_FWDVA_ENCODE(n)    ((((unsigned long)(n))&0x0F)<<16)
#define   CPR0_PLLD_FWDVA_DECODE(n)    ((((((unsigned long)(n))>>16)-1)&0x0F)+1)
#define CPR0_PLLD_FWDVB      0x00000700   /* Forward Divisor B     Mask       */
#define  CPR0_PLLD_FWDVB_8   0x00000000   /* Forward Divisor B = 8            */
#define  CPR0_PLLD_FWDVB_1   0x00000100   /* Forward Divisor B = 1            */
#define  CPR0_PLLD_FWDVB_2   0x00000200   /* Forward Divisor B = 2            */
#define  CPR0_PLLD_FWDVB_3   0x00000300   /* Forward Divisor B = 3            */
#define  CPR0_PLLD_FWDVB_4   0x00000400   /* Forward Divisor B = 4            */
#define  CPR0_PLLD_FWDVB_5   0x00000500   /* Forward Divisor B = 5            */
#define  CPR0_PLLD_FWDVB_6   0x00000600   /* Forward Divisor B = 6            */
#define  CPR0_PLLD_FWDVB_7   0x00000700   /* Forward Divisor B = 7            */
#define   CPR0_PLLD_FWDVB_ENCODE(n)    ((((unsigned long)(n))&0x07)<<8)
#define   CPR0_PLLD_FWDVB_DECODE(n)    ((((((unsigned long)(n))>>8)-1)&0x07)+1)
#define CPR0_PLLD_LFBDV      0x0000003F   /* Local Feedback Divisor  Mask     */
#define   CPR0_PLLD_LFBDV_ENCODE(n)    ((((unsigned long)(n))&0x3F)<<0)
#define   CPR0_PLLD_LFBDV_DECODE(n)    ((((((unsigned long)(n))>>0)-1)&0x3F)+1)

/* Bits for Primary A Divider Register (CPR0_PRIMAD) */
#define CPR0_PRIMAD_PRIMAD   0x07000000   /* Primary A Divisor 0   Mask       */
#define  CPR0_PRIMAD_PRIMAD_8 0x00000000  /* Primary A Divisor 0 = 8          */
#define  CPR0_PRIMAD_PRIMAD_1 0x01000000  /* Primary A Divisor 0 = 1          */
#define  CPR0_PRIMAD_PRIMAD_2 0x02000000  /* Primary A Divisor 0 = 2          */
#define  CPR0_PRIMAD_PRIMAD_3 0x03000000  /* Primary A Divisor 0 = 3          */
#define  CPR0_PRIMAD_PRIMAD_4 0x04000000  /* Primary A Divisor 0 = 4          */
#define  CPR0_PRIMAD_PRIMAD_5 0x05000000  /* Primary A Divisor 0 = 5          */
#define  CPR0_PRIMAD_PRIMAD_6 0x06000000  /* Primary A Divisor 0 = 6          */
#define  CPR0_PRIMAD_PRIMAD_7 0x07000000  /* Primary A Divisor 0 = 7          */
#define   CPR0_PRIMAD_PRADV0_ENCODE(n) ((((unsigned long)(n))&0x07)<<24)
#define   CPR0_PRIMAD_PRADV0_DECODE(n) ((((((unsigned long)(n))>>24)-1)&0x07)+1)
 
/* Bits for Primary B Divider Register (CPR0_PRIMBD) */
#define CPR0_PRIMBD_PRIMBD   0x07000000   /* Primary B Divisor 0   Mask       */
#define  CPR0_PRIMBD_PRIMBD_8 0x00000000  /* Primary B Divisor 0 = 8          */
#define  CPR0_PRIMBD_PRIMBD_1 0x01000000  /* Primary B Divisor 0 = 1          */
#define  CPR0_PRIMBD_PRIMBD_2 0x02000000  /* Primary B Divisor 0 = 2          */
#define  CPR0_PRIMBD_PRIMBD_3 0x03000000  /* Primary B Divisor 0 = 3          */
#define  CPR0_PRIMBD_PRIMBD_4 0x04000000  /* Primary B Divisor 0 = 4          */
#define  CPR0_PRIMBD_PRIMBD_5 0x05000000  /* Primary B Divisor 0 = 5          */
#define  CPR0_PRIMBD_PRIMBD_6 0x06000000  /* Primary B Divisor 0 = 6          */
#define  CPR0_PRIMBD_PRIMBD_7 0x07000000  /* Primary B Divisor 0 = 7          */
#define   CPR0_PRIMBD_PRBDV0_MASK      0x07000000
#define   CPR0_PRIMBD_PRBDV0_ENCODE(n) ((((unsigned long)(n))&0x07)<<24)
#define   CPR0_PRIMBD_PRBDV0_DECODE(n) ((((((unsigned long)(n))>>24)-1)&0x07)+1)

#endif  /* INCcprDcrh  */
