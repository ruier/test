/* phyLib.h - MII/physical network configuration header */

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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2003-2004, 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,05dec07,x_s  Changed to use MACRO VXB_EMAC_DRV_CTRL
01j,19mar07,x_s  Fix support for boards with ET1011C PHY chips
01f,08jun06,wap  Fix support for boards with BCM5428 PHY chips
01e,29sep04,mdo  Documentation fixes for apigen
01d,06aug03,jtp  Formal code inspection change: memory macro additions.
01c,17jul03,jln  rename ibmEmacEnd.h to emacEnd.h
01b,11jun03,jln  add function declaration 
01a,02jan03,saw  created.
*/

/*
This file contains PHY specific definitions 
*/


#ifndef __INCphyLibh
#define __INCphyLibh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/* includes */

#include "end.h"

/* globals */
IMPORT UINT sysPlbInLong(ULONG address);
IMPORT void sysPlbOutLong(ULONG address, UINT data);

/*
 * Address of the PHY(s) attached to the EMAC interfaces.
 * There are multiple PHY chips on the EVB440GX board - LXT9762HC SMII PHY,
 * Am79C875 RMII PHY, and CIS 8201 Gigabit PHY.  Also, the GPCS logic in the 
 * EMAC core can be configured to act as a PHY in TBI mode.
 * 
 * Note the SMII addresses are swapped between PHY2 and PHY3 on the board
 *
 */

#define PHY0_MII_ADDR           0x0008  /* Set switches on PHY to match */

#define PHY0_SMII_ADDR          0x0001
#define PHY1_SMII_ADDR          0x0002
#define PHY2_SMII_ADDR          0x0004  /* Note these are swapped */
#define PHY3_SMII_ADDR          0x0003

#define PHY0_RMII_ADDR          0x0008  /* Assumes default jumper settings */
#define PHY1_RMII_ADDR          0x0009

#define PHY2_RGMII_ADDR         0x0001
#define PHY2_GPCS_ADDR          0x0011  /* addr to use for GPCS core in EMAC */
#define PHY3_RGMII_ADDR         0x0003
#define PHY3_GPCS_ADDR          0x0019  /* addr to use for GPCS core in EMAC */

#define PHY_ADDR_INVALID        0x0000

#define PHY_INTEL_LXT9762_ID1	0x0013

/*
 * PHY bridge base addresses
 */
#ifndef	ZMII0_BASE
#define ZMII0_BASE            (PERIPH_BASE_ADDR + 0x00000780)
#endif	/* ZMII0_BASE */
#ifndef	RGMII0_BASE
#define RGMII0_BASE           (PERIPH_BASE_ADDR + 0x00000790)
#endif	/* RGMII0_BASE */

/*
 * Access macros 
 */
#define ZMII_REG_WRITE(addr,value)	sysPlbOutLong((addr),(value))
#define ZMII_REG_READ(addr)	      sysPlbInLong((addr))
#define RGMII_REG_WRITE(addr,value)	sysPlbOutLong((addr),(value))
#define RGMII_REG_READ(addr)	      sysPlbInLong((addr))

/*-----------------------------------------------------------------------------+
| ZMII PHY bridge register defines
+-----------------------------------------------------------------------------*/
#define ZMII0_FER              (ZMII0_BASE+0x0)
#define ZMII0_SSR              (ZMII0_BASE+0x4)
#define ZMII0_SMIISR           (ZMII0_BASE+0x8)

#define ZMII_FER_MDI0         0x80000000
#define ZMII_FER_SMII0        0x40000000
#define ZMII_FER_RMII0        0x20000000
#define ZMII_FER_MII0         0x10000000
#define ZMII_FER_EMAC0_MASK   0xF0000000
#define ZMII_FER_MDI1         0x08000000
#define ZMII_FER_SMII1        0x04000000
#define ZMII_FER_RMII1        0x02000000
#define ZMII_FER_MII1         0x01000000
#define ZMII_FER_EMAC1_MASK   0x0F000000
#define ZMII_FER_MDI2         0x00800000
#define ZMII_FER_SMII2        0x00400000
#define ZMII_FER_RMII2        0x00200000
#define ZMII_FER_MII2         0x00100000
#define ZMII_FER_EMAC2_MASK   0x00F00000
#define ZMII_FER_MDI3         0x00080000
#define ZMII_FER_SMII3        0x00040000
#define ZMII_FER_RMII3        0x00020000
#define ZMII_FER_MII3         0x00010000
#define ZMII_FER_EMAC3_MASK   0x000F0000

#define ZMII_SSR_SCI0         0x40000000
#define ZMII_SSR_FSS0         0x20000000
#define ZMII_SSR_SP0          0x10000000
#define ZMII_SSR_SP0_10Mbps   0x00000000
#define ZMII_SSR_SP0_100Mbps  0x10000000
#define ZMII_SSR_EMAC0_MASK   0x70000000
#define ZMII_SSR_SCI1         0x04000000
#define ZMII_SSR_FSS1         0x02000000
#define ZMII_SSR_SP1          0x01000000
#define ZMII_SSR_SP1_10Mbps   0x00000000
#define ZMII_SSR_SP1_100Mbps  0x01000000
#define ZMII_SSR_EMAC1_MASK   0x07000000
#define ZMII_SSR_SCI2         0x00400000
#define ZMII_SSR_FSS2         0x00200000
#define ZMII_SSR_SP2          0x00100000
#define ZMII_SSR_SP2_10Mbps   0x00000000
#define ZMII_SSR_SP2_100Mbps  0x00100000
#define ZMII_SSR_EMAC2_MASK   0x00700000
#define ZMII_SSR_SCI3         0x00040000
#define ZMII_SSR_FSS3         0x00020000
#define ZMII_SSR_SP3          0x00010000
#define ZMII_SSR_SP3_10Mbps   0x00000000
#define ZMII_SSR_SP3_100Mbps  0x00010000
#define ZMII_SSR_EMAC3_MASK   0x00070000

/*-----------------------------------------------------------------------------+
| RGMII PHY bridge register defines
+-----------------------------------------------------------------------------*/
#define RGMII0_FER            (RGMII0_BASE+ 0x00)
#define RGMII0_SSR            (RGMII0_BASE+ 0x04)

#define RGMII_FER_RGMII3_MASK  0x00000070
#define RGMII_FER_RGMII3       0x00000070
#define RGMII_FER_RGMII3_DISAB 0x00000000
#define RGMII_FER_RGMII3_RTBI  0x00000040
#define RGMII_FER_RGMII3_RGMII 0x00000050
#define RGMII_FER_RGMII3_TBI   0x00000060
#define RGMII_FER_RGMII3_GMII  0x00000070
#define RGMII_FER_RGMII2_MASK  0x00000007
#define RGMII_FER_RGMII2       0x00000007
#define RGMII_FER_RGMII2_DISAB 0x00000000
#define RGMII_FER_RGMII2_RTBI  0x00000004
#define RGMII_FER_RGMII2_RGMII 0x00000005
#define RGMII_FER_RGMII2_TBI   0x00000006
#define RGMII_FER_RGMII2_GMII  0x00000007

#define RGMII_SSR_SP3          0x00000700
#define RGMII_SSR_SP3_10       0x00000000
#define RGMII_SSR_SP3_100      0x00000200
#define RGMII_SSR_SP3_1000     0x00000400
#define RGMII_SSR_SP2          0x00000007
#define RGMII_SSR_SP2_10       0x00000000
#define RGMII_SSR_SP2_100      0x00000002
#define RGMII_SSR_SP2_1000     0x00000004

/*-----------------------------------------------------------------------------+
| PHY defines
+-----------------------------------------------------------------------------*/

/* General definitions */

#define PHY_HALF_DUPLEX 	22
#define PHY_FULL_DUPLEX 	44

#define PHY_INVALID             0x00    /* phy type */
#define PHY_MII                 0x01
#define PHY_SMII                0x02
#define PHY_RMII                0x03
#define PHY_GIG                 0x04

#define PHY_NUM_SMII_REGS       0x31
#define PHY_NUM_RMII_REGS       0x25
#define PHY_NUM_MII_REGS        0x26
#define PHY_NUM_GIG_REGS        0x20

/* General PHY reg definitions */
#define PHY_BMCR                0x00
#define    PHY_BMCR_RESET       0x8000
#define    PHY_BMCR_LOOP        0x4000
#define    PHY_BMCR_100MB       0x2000
#define    PHY_BMCR_AUTON       0x1000
#define    PHY_BMCR_POWD        0x0800
#define    PHY_BMCR_ISO         0x0400
#define    PHY_BMCR_RST_NEG     0x0200
#define    PHY_BMCR_DPLX        0x0100
#define    PHY_BMCR_COL_TST     0x0080
#define    PHY_BMCR_GIG         0x0040
#define PHY_BMSR                0x01
#define    PHY_BMSR_100T4       0x8000
#define    PHY_BMSR_100TXF      0x4000
#define    PHY_BMSR_100TXH      0x2000
#define    PHY_BMSR_10TF        0x1000
#define    PHY_BMSR_10TH        0x0800
#define    PHY_BMSR_100BT2F     0x0400
#define    PHY_BMSR_100BT2H     0x0200
#define    PHY_MBSR_EXTSTS      0x0100
#define    PHY_BMSR_PRE_SUP     0x0040
#define    PHY_BMSR_AUTN_COMP   0x0020
#define    PHY_BMSR_RF          0x0010
#define    PHY_BMSR_AUTN_ABLE   0x0008
#define    PHY_BMSR_LS          0x0004
#define    PHY_BMSR_JD          0x0002
#define    PHY_BMSR_EXT         0x0001
#define PHY_PHY1DR1             0x02
#define PHY_PHYIDR2             0x03
#define PHY_ANAR                0x04
#define PHY_ANLPAR              0x05
#define    PHY_ANLPAR_NP        0x8000
#define    PHY_ANLPAR_ACK       0x4000
#define    PHY_ANLPAR_RF        0x2000
#define    PHY_ANLPAR_T4        0x0200
#define    PHY_ANLPAR_TXFD      0x0100
#define    PHY_ANLPAR_TX        0x0080
#define    PHY_ANLPAR_10FD      0x0040
#define    PHY_ANLPAR_10        0x0020
#define    PHY_ANLPAR_100       0x0380
#define PHY_ANER                0x06
#define PHY_ANNPTR              0x07
#define PHY_ANLPNPRR            0x08

#define PHY_1000BTSR            0x0A
/* MII PHY reg definitions */
#define MII_PHYSTS              0x10
#define    MII_PHYSTS_REL       0x8000
#define    MII_PHYSTS_CIML      0x4000
#define    MII_PHYSTS_FCSL      0x2000
#define    MII_PHYSTS_DR        0x0800
#define    MII_PHYSTS_PR        0x0400
#define    MII_PHYSTS_ANE       0x0200
#define    MII_PHYSTS_MIP       0x0100
#define    MII_PHYSTS_RF        0x0080
#define    MII_PHYSTS_JD        0x0040
#define    MII_PHYSTS_ANC       0x0020
#define    MII_PHYSTS_RS        0x0010
#define    MII_PHYSTS_LBKSTS    0x0008
#define    MII_PHYSTS_DS        0x0004
#define    MII_PHYSTS_SS        0x0002
#define    MII_PHYSTS_LS        0x0001
#define MII_MIPSCR              0x11
#define MII_MIPGSR              0x12
#define MII_DCR                 0x13
#define MII_FCSCR               0x14
#define MII_RECR                0x15
#define MII_PCSR                0x16
#define MII_LBR                 0x17
#define MII_10BTSCR             0x18
#define MII_PHYCTRL             0x19

/* RMII PHY reg definitions */
#define RMII_MISCFEATURE1       0x10
#define RMII_INTCS              0x11
#define RMII_DIAG               0x12
#define    RMII_DIAG_DPLX       0x0800
#define    RMII_DIAG_SPEED      0x0400
#define    RMII_DIAG_RXPASS     0x0200
#define    RMII_DIAG_RXLOCK     0x0100
#define RMII_TEST               0x13
#define RMII_MISCFEATURE2       0x14
#define RMII_RECR               0x15
#define RMII_MODECTRL           0x18

/* Intel SMII PHY reg definitions */
#define SMII_PCR                0x10
#define SMII_QSR                0x11
#define    SMII_QSR_SPEED       0x4000
#define    SMII_QSR_TXSTS       0x2000
#define    SMII_QSR_RXSTS       0x1000
#define    SMII_QSR_COLSTS      0x0800
#define    SMII_QSR_LINK        0x0400
#define    SMII_QSR_DPLX        0x0200
#define    SMII_QSR_AUTON       0x0100
#define    SMII_QSR_AUTONC      0x0080
#define    SMII_QSR_POL         0x0020
#define    SMII_QSR_PAUSE       0x0010
#define    SMII_QSR_ERROR       0x0008
#define SMII_IER                0x12
#define SMII_ISR                0x13
#define SMII_LEDCFG             0x14
#define SMII_TCR1               0x1C
#define SMII_TCR2               0x1E

/* Broadcom SMII PHY definitions */
#define SMII_ACSR		0x18

#define SMII_ACSR_JABBER_DIS     0x8000  /* jabber disable */
#define SMII_ACSR_FLINK          0x4000  /* force 10baseT link pass */
#define SMII_ACSR_HSQ            0x0080  /* SQ high */
#define SMII_ACSR_LSQ            0x0040  /* SQ low */
#define SMII_ACSR_ER1            0x0020  /* edge rate 1 */
#define SMII_ACSR_ER0            0x0010  /* edge rate 0 */
#define SMII_ACSR_ANEG           0x0008  /* auto-negotiation activated */
#define SMII_ACSR_F100           0x0004  /* force 100base */
#define SMII_ACSR_SPEED          0x0002  /* 1 = 100, 0 = 10 */
#define SMII_ACSR_FDX            0x0001  /* full-duplex */

/* RGMII PHY reg definitions */
#define    GIG_EXT_CTRL1           0x16
#define    GIG_EXT_CTRL1_MODE       0x0007
#define    GIG_EXT_CTRL1_MODE_GMII  0x0000
#define    GIG_EXT_CTRL1_MODE_RGMII 0x0006
#define    GIG_EXT_CTRL1_MODE_TBI   0x2000
#define    GIG_EXT_CTRL1_MODE_RTBI  0x3000

#define    PHY_CONT_REG             0x17
#define    PHY_RGMII_DELAY_VALUE    0x40

#define    LED_CONTROL_REG2         0x1C
#define    LED_REG2_DEFAU_LVALUE    0x74f0
#define    GIG_CTRL_STAT_FDX        0x0020
#define    GIG_CTRL_STAT_SPD        0x0018
#define    GIG_CTRL_STAT_SPD_10     0x0000
#define    GIG_CTRL_STAT_SPD_100    0x0008
#define    GIG_CTRL_STAT_SPD_1000   0x0010

/* GPCS PHY reg definitions */
#define GPCS_CR                 0x00
#define    GPCS_CR_ANE          0x1000
#define    GPCS_CR_PD           0x0800
#define    GPCS_CR_ISOL         0x0400
#define    GPCS_CR_RAN          0x0200
#define GPCS_SR                 0x01
#define    GPCS_SR_PD           0x0800

/* PHY_1000BTSR */
#define PHY_1000BTSR_MSCF       0x8000
#define PHY_1000BTSR_MSCR       0x4000
#define PHY_1000BTSR_LRS        0x2000
#define PHY_1000BTSR_RRS        0x1000
#define PHY_1000BTSR_1000FD     0x0800
#define PHY_1000BTSR_1000HD     0x0400

/* mode supported in EMAC */

#define PHY_MODE_DISABLED       0x00
#define PHY_MODE_MII            0x01
#define PHY_MODE_SMII           0x02
#define PHY_MODE_RMII           0x03
#define PHY_MODE_GMII           0x04
#define PHY_MODE_RGMII          0x05
#define PHY_MODE_TBI            0x06
#define PHY_MODE_RTBI           0x07
/* EMAC speed */

#define EMAC_SPEED_10Mbps        10
#define EMAC_SPEED_100Mbps       100
#define EMAC_SPEED_1000Mbps      1000
/*
 * EMAC MII station management unit (STA Control) register bit definitions
 */
#define EMAC_STACR_OC           0x00008000     /* Operation complete          */
#define EMAC_STACR_PHYE         0x00004000     /* PHY error                   */
#define EMAC_STACR_WRITE        0x00002000     /* STA command WRITE           */
#define EMAC_STACR_READ         0x00001000     /* STA command READ            */
#define EMAC_MR1_OBCI           0x00000038     /* OPB bus clock indication   */
#define EMAC_MR1_OBCI_50MHZ     0x00000000     /* OPB bus clock freq 50MHz    */
#define EMAC_MR1_OBCI_66MHZ     0x00000008     /* OPB bus clock freq 66MHz    */
#define EMAC_MR1_OBCI_83MHZ     0x00000010     /* OPB bus clock freq 83MHz    */
#define EMAC_MR1_OBCI_100MHZ    0x00000018     /* OPB bus clock freq 100MHz   */
#define EMAC_MR1_OBCI_G100MHZ   0x00000020     /* OPB bus clock freq 100MHz   */
#define EMAC_STACR		0x005C	/* STA control register */
#define EMAC_MR1		0x0004	/* Mode register 1 */

STATUS phyDump(int emacNumber);
void   phyDumpZmii(void);
void   phyDumpRgmii(void);
UINT32 phyGetSpeed(int emacNumber);
UINT32 phyGetDuplex(int emacNumber);
STATUS phySetInterface(int emacNumber, int emacBaseAddr);
UINT32 phyGetAddr(int emacNumber);
UINT32 phyGetType(int emacNumber);
UINT32 phyGetMode(int emacNumber);
void   phyGetModeDesc(int emacNumber, char * phyDesc);
STATUS phyAutoNegWait(int emacNumber, UINT ticks);
#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCphyLibh */
