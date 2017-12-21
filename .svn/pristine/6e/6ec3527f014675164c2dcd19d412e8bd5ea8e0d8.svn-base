/* enetPhy.h - Ethernet PHY header */

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

/* Copyright 2001-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,02nov04,dr   Additional documentation changes for apigen
01f,07sep04,mdo  Documentation fixes for apigen
01e,07jul03,tr   modify to support ALTIMA AC104Z Phy (regs definition)
01d,12jul02,pch  SPR 77083: fix typo in name PHY_PHYIDR1
01c,12jun02,pch  SPR 74987: C++
01b,19nov01,pch  cleanup
01a,03nov99,mcg  created
*/

/*
This file contains information about Ethernet Physical Layer Devices (PHYs).
The file was create for the National Semiconductor DP83843 PHYTER device.  It may
require some modification if another PHY device is used.
See also IEEE Std 802.3, 1998 Edition, section 22.2.4.
*/

#ifndef INCenetPhyh
#define INCenetPhyh

#ifdef __cplusplus
    extern "C" {
#endif

/* Interface properties */

#define PHY_100MBPS      100
#define PHY_10MBPS       10
#define PHY_HALF_DUPLEX  22
#define PHY_FULL_DUPLEX  44


/* PHY register offsets */
/* Common to all standard PHYs (MII specification) */

#define PHY_BMCR         0x00        /* Basic Mode Control Reg */
#define PHY_BMSR         0x01        /* Basic Mode Status Reg */
#define PHY_PHYIDR1      0x02        /* PHY identifier Reg 1 */
#define PHY_PHYIDR2      0x03        /* PHY identifier Reg 2 */
#define PHY_ANAR         0x04        /* Auto-neg Advertisement Reg */
#define PHY_ANLPAR       0x05        /* Auto-neg Link Partner Ability Reg */
#define PHY_ANER         0x06        /* Auto-neg Expansion Reg */
#define PHY_ANNPTR       0x07        /* Auto-neg Next Page TX */

/* Registers specific to AC104Z */
#define PHY_POLINTLR     0x10        /* Polarity and Interrupt Level Reg  */
#define PHY_INTCNTSTR    0x11        /* Interrupt Control/Status Reg */
#define PHY_TESTR        0x13        /* Test Reg */
#define PHY_CABMEASR     0x14        /* Cable Measurement Reg */
#define PHY_RCVERCNTR    0x15        /* Receive Error Count Reg */
#define PHY_PWRMANR      0x16        /* Power Management Reg */
#define PHY_OPMODR       0x17        /* Operation Mode Reg */
#define PHY_GLOBAL1R     0x1C        /* Offset of first global Reg */
#define PHY_GLOBAL2R     0x1D        /* Offset of second global Reg */
#define PHY_GLOBAL3R     0x1E        /* Offset of third global Reg */

/* PHY BMCR */

#define PHY_BMCR_RESET          0x8000
#define PHY_BMCR_LOOP           0x4000
#define PHY_BMCR_100MB          0x2000
#define PHY_BMCR_AUTON          0x1000
#define PHY_BMCR_POWD           0x0800
#define PHY_BMCR_ISO            0x0400
#define PHY_BMCR_RST_NEG        0x0200
#define PHY_BMCR_DPLX           0x0100
#define PHY_BMCR_COL_TST        0x0080

/* PHY BMSR */

#define PHY_BMSR_100T4          0x8000
#define PHY_BMSR_100TXF         0x4000
#define PHY_BMSR_100TXH         0x2000
#define PHY_BMSR_10TF           0x1000
#define PHY_BMSR_10TH           0x0800
#define PHY_BMSR_PRE_SUP        0x0040
#define PHY_BMSR_AUTN_COMP      0x0020
#define PHY_BMSR_RF             0x0010
#define PHY_BMSR_AUTN_ABLE      0x0008
#define PHY_BMSR_LS             0x0004
#define PHY_BMSR_JD             0x0002
#define PHY_BMSR_EXT            0x0001

/*PHY ANLPAR */

#define PHY_ANLPAR_NP           0x8000
#define PHY_ANLPAR_ACK          0x4000
#define PHY_ANLPAR_RF           0x2000
#define PHY_ANLPAR_T4           0x0200
#define PHY_ANLPAR_TXFD         0x0100
#define PHY_ANLPAR_TX           0x0080
#define PHY_ANLPAR_10FD         0x0040
#define PHY_ANLPAR_10           0x0020
#define PHY_ANLPAR_100          0x0380

/* PHY 10BTSCR */

#define PHY_10BTSCR_HBDS        0x0080

#ifdef __cplusplus
    }
#endif

#endif  /* INCenetPhyh */
