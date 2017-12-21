/* sdrDcr.h - IBM System DCR Register definitions */

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

/* Copyright 2003-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,25nov04,cjj  conditionally define SDR0_UARTX_UXDIV_DECODE 
01c,29sep04,mdo  Documentation fixes for apigen
01b,12aug03,jtp  Formal code inspection changes. SDR0_MFR_{PCM,ECS} bits.
01a,01may02,saw  created
*/

/*
This file contains the definitions of the System DCR registers
*/

#ifndef INCsdrDcrh
#define INCsdrDcrh

/*
 * System DCR Registers (SDR0)
 */

#define SDR0_CFGADDR  (SDR0_DCR_BASE+0x0)  /* SDRO Controller Addr Reg        */
#define SDR0_CFGDATA  (SDR0_DCR_BASE+0x1)  /* SDRO Controller Data Reg        */

/* Indirect access registers for SDR0 -- use SDR0_CFGADDR to access */

#define SDR0_SDSTP0    0x0020  /* Serial Device Strap Reg 0                   */
#define SDR0_SDSTP1    0x0021  /* Serial Device Strap Reg 1                   */
#define SDR0_PINSTP    0x0040  /* Pin Strapping Reg                           */
#define SDR0_SDCS      0x0060  /* Serial Device Controller Setting Reg        */
#define SDR0_ECID0     0x0080  /* Electronic Chip ID Reg 0                    */
#define SDR0_ECID1     0x0081  /* Electronic Chip ID Reg 1                    */
#define SDR0_ECID2     0x0082  /* Electronic Chip ID Reg 2                    */
#define SDR0_JTAG      0x00C0  /* JTAG ID Reg                                 */
#define SDR0_DDRDL     0x00E0  /* DDR Delay Line Reg                          */
#define SDR0_EBC       0x0100  /* EBC Config Reg                              */
#define SDR0_UART0     0x0120  /* UART Config Reg 0                           */
#define SDR0_UART1     0x0121  /* UART Config Reg 1                           */
#define SDR0_CP440     0x0180  /* CPU Control Reg                             */
#define SDR0_XCR       0x01C0  /* PCIX Control Reg                            */
#define SDR0_XPLLC     0x01C1  /* PCIX PLL Control Reg                        */
#define SDR0_XPLLD     0x01C2  /* PCIX PLL Divisor Reg                        */
#define SDR0_SRST      0x0200  /* Soft Reset Reg                              */
#define SDR0_SLPIPE    0x0220  /* PLB Slave Address Pipeline Reg              */
#define SDR0_AMP       0x0240  /* PLB Alternate Master Priority Reg           */
#define SDR0_MIRQ0     0x0260  /* PLB Master Interrupt Request Reg 0          */
#define SDR0_MIRQ1     0x0261  /* PLB Master Interrupt Request Reg 1          */
#define SDR0_MALTBL    0x0280  /* MAL Tx Burst Length Reg                     */
#define SDR0_MALRBL    0x02A0  /* MAL Rx Burst Length Reg                     */
#define SDR0_MALTBS    0x02C0  /* MAL Tx Bus Size Reg                         */
#define SDR0_MALRBS    0x02E0  /* MAL Rx Bus Size Reg                         */
#define SDR0_CUST0     0x4000  /* Customer Config Reg 0                       */
#define SDR0_SDSTP2    0x4001  /* Serial Device Strap Reg 2                   */
#define SDR0_CUST1     0x4002  /* Customer Config Reg 1                       */
#define SDR0_SDSTP3    0x4003  /* Serial Device Strap Reg 3                   */
#define SDR0_PFC0      0x4100  /* Pin Function Control Reg 0                  */
#define SDR0_PFC1      0x4101  /* Pin Function Control Reg 1                  */
#define SDR0_PLBTR     0x4200  /* PLB Trace Reg                               */
#define SDR0_MFR       0x4300  /* Misc Function Reg                           */

/* Bit Definitions */

/* Bits for Alternate PLB Master Priority Register (SDR0_AMP) */
#define SDR0_AMP_AICURP      0xC0000000   /* Alt ICU Read  Prio          Mask */
#define SDR0_AMP_AICURP_0    0x00000000   
#define SDR0_AMP_AICURP_1    0x40000000
#define SDR0_AMP_AICURP_2    0x80000000
#define SDR0_AMP_AICURP_3    0xC0000000
#define SDR0_AMP_ADCURP      0x30000000   /* Alt DCU Read  Prio          Mask */
#define SDR0_AMP_ADCURP_0    0x00000000   
#define SDR0_AMP_ADCURP_1    0x10000000
#define SDR0_AMP_ADCURP_2    0x20000000
#define SDR0_AMP_ADCURP_3    0x30000000
#define SDR0_AMP_ADCUWP      0x0C000000   /* Alt DCU Write Prio          Mask */
#define SDR0_AMP_ADCUWP_0    0x00000000   
#define SDR0_AMP_ADCUWP_1    0x04000000
#define SDR0_AMP_ADCUWP_2    0x08000000
#define SDR0_AMP_ADCUWP_3    0x0C000000
#define SDR0_AMP_APCIXP      0x03000000   /* Alt PCIX      Prio          Mask */
#define SDR0_AMP_APCIXP_0    0x00000000   
#define SDR0_AMP_APCIXP_1    0x01000000
#define SDR0_AMP_APCIXP_2    0x02000000
#define SDR0_AMP_APCIXP_3    0x03000000
#define SDR0_AMP_AIMUP       0x00C00000   /* Alt IMU       Prio          Mask */
#define SDR0_AMP_AIMUP_0     0x00000000   
#define SDR0_AMP_AIMUP_1     0x00400000
#define SDR0_AMP_AIMUP_2     0x00800000
#define SDR0_AMP_AIMUP_3     0x00C00000
#define SDR0_AMP_AMALP       0x00300000   /* Alt MAL       Prio          Mask */
#define SDR0_AMP_AMALP_0     0x00000000   
#define SDR0_AMP_AMALP_1     0x00100000
#define SDR0_AMP_AMALP_2     0x00200000
#define SDR0_AMP_AMALP_3     0x00300000
#define SDR0_AMP_ADMAP       0x000C0000   /* Alt DMA       Prio          Mask */
#define SDR0_AMP_ADMAP_0     0x00000000   
#define SDR0_AMP_ADMAP_1     0x00040000
#define SDR0_AMP_ADMAP_2     0x00080000
#define SDR0_AMP_ADMAP_3     0x000C0000
#define SDR0_AMP_AOPBP       0x00030000   /* Alt OPB-PLB   Prio          Mask */
#define SDR0_AMP_AOPBP_0     0x00000000   
#define SDR0_AMP_AOPBP_1     0x00010000
#define SDR0_AMP_AOPBP_2     0x00020000
#define SDR0_AMP_AOPBP_3     0x00030000
#define SDR0_AMP_ICURP       0x00008000   /* ICU Read      Prio          Mask */
#define  SDR0_AMP_ICURP_O    0x00000000   /* ICU Read controls own prio       */
#define  SDR0_AMP_ICURP_A    0x00008000   /* ICU Read uses alt prio           */
#define SDR0_AMP_DCURP       0x00004000   /* DCU Read      Prio          Mask */
#define  SDR0_AMP_DCURP_O    0x00000000   /* DCU Read controls own prio       */
#define  SDR0_AMP_DCURP_A    0x00004000   /* DCU Read uses alt prio           */
#define SDR0_AMP_DCUWP       0x00002000   /* DCU Write     Prio          Mask */
#define  SDR0_AMP_DCUWP_O    0x00000000   /* DCU Write controls own prio      */
#define  SDR0_AMP_DCUWP_A    0x00002000   /* DCU Write uses alt prio          */
#define SDR0_AMP_PCIXP       0x00001000   /* PCIX          Prio          Mask */
#define  SDR0_AMP_PCIXP_O    0x00000000   /* PCIX controls own prio           */
#define  SDR0_AMP_PCIXP_A    0x00001000   /* PCIX uses alt prio               */
#define SDR0_AMP_IMUP        0x00000800   /* IMU           Prio          Mask */
#define  SDR0_AMP_IMUP_O     0x00000000   /* IMU  controls own prio           */
#define  SDR0_AMP_IMUP_A     0x00000800   /* IMU  uses alt prio               */
#define SDR0_AMP_MALP        0x00000400   /* MAL           Prio          Mask */
#define  SDR0_AMP_MALP_O     0x00000000   /* MAL  controls own prio           */
#define  SDR0_AMP_MALP_A     0x00000400   /* MAL  uses alt prio               */
#define SDR0_AMP_DMAP        0x00000200   /* DMA           Prio          Mask */
#define  SDR0_AMP_DMAP_O     0x00000000   /* DMA  controls own prio           */
#define  SDR0_AMP_DMAP_A     0x00000200   /* DMA  uses alt prio               */
#define SDR0_AMP_OPBP        0x00000100   /* OPB-PLB bridge Prio         Mask */
#define  SDR0_AMP_OPBP_O     0x00000000   /* Bridge controls own prio         */
#define  SDR0_AMP_OPBP_A     0x00000100   /* Bridge uses alt prio             */

/* Bits for PPC440 CPU Register (SDR0_CP440) */
#define SDR0_CP440_EARV      0x30000000   /* ERPN Address Reset Vector   Mask */
#define  SDR0_CP440_EARV_EBC 0x10000000   /* ERPN Boot from EBC               */
#define  SDR0_CP440_EARV_PCI 0x20000000   /* ERPN Boot from PCI               */
#define   SDR0_CPU440_EARV_ENCODE(n)   ((((unsigned long)(n))&0x03)<<28)
#define   SDR0_CPU440_EARV_DECODE(n)   ((((unsigned long)(n))>>28)&0x03)
#define SDR0_CP440_DRU       0x0C000000   /* DcuRdUrgent                 Mask */
#define SDR0_CP440_DRU_0      0x00000000 
#define SDR0_CP440_DRU_1      0x04000000
#define SDR0_CP440_DRU_2      0x08000000
#define SDR0_CP440_DRU_3      0x0C000000
#define SDR0_CP440_DRT       0x03000000   /* DcuRdTouch                  Mask */
#define SDR0_CP440_DRT_0      0x00000000 
#define SDR0_CP440_DRT_1      0x01000000
#define SDR0_CP440_DRT_2      0x02000000
#define SDR0_CP440_DRT_3      0x03000000
#define SDR0_CP440_DRNC      0x00C00000   /* DcuRdNonCache               Mask */
#define SDR0_CP440_DRNC_0     0x00000000 
#define SDR0_CP440_DRNC_1     0x00400000
#define SDR0_CP440_DRNC_2     0x00800000
#define SDR0_CP440_DRNC_3     0x00C00000
#define SDR0_CP440_DRLC      0x00300000   /* DcuRdLdCache                Mask */
#define SDR0_CP440_DRLC_0     0x00000000 
#define SDR0_CP440_DRLC_1     0x00100000
#define SDR0_CP440_DRLC_2     0x00200000
#define SDR0_CP440_DRLC_3     0x00300000
#define SDR0_CP440_DWF       0x000C0000   /* DcuWrFlush                  Mask */
#define SDR0_CP440_DWF_0      0x00000000 
#define SDR0_CP440_DWF_1      0x00040000
#define SDR0_CP440_DWF_2      0x00080000
#define SDR0_CP440_DWF_3      0x000C0000
#define SDR0_CP440_DWS       0x00030000   /* DcuWrStore                  Mask */
#define SDR0_CP440_DWS_0      0x00000000 
#define SDR0_CP440_DWS_1      0x00010000
#define SDR0_CP440_DWS_2      0x00020000
#define SDR0_CP440_DWS_3      0x00030000
#define SDR0_CP440_DWU       0x0000C000   /* DcuWrUrgent                 Mask */
#define SDR0_CP440_DWU_0      0x00000000 
#define SDR0_CP440_DWU_1      0x00004000
#define SDR0_CP440_DWU_2      0x00008000
#define SDR0_CP440_DWU_3      0x0000C000
#define SDR0_CP440_IRF       0x00003000   /* IcuRdFetch                  Mask */
#define SDR0_CP440_IRF_0      0x00000000 
#define SDR0_CP440_IRF_1      0x00001000
#define SDR0_CP440_IRF_2      0x00002000
#define SDR0_CP440_IRF_3      0x00003000
#define SDR0_CP440_IRT       0x00000C00   /* IcuRdTouch                  Mask */
#define SDR0_CP440_IRT_0      0x00000000 
#define SDR0_CP440_IRT_1      0x00000400
#define SDR0_CP440_IRT_2      0x00000800
#define SDR0_CP440_IRT_3      0x00000C00
#define SDR0_CP440_IRS       0x00000300   /* IcuRdSpec                   Mask */
#define SDR0_CP440_IRS_0      0x00000000 
#define SDR0_CP440_IRS_1      0x00000100
#define SDR0_CP440_IRS_2      0x00000200
#define SDR0_CP440_IRS_3      0x00000300
#define SDR0_CP440_Nto1      0x00000002   /* PLB/CPU N to 1 clk ratio    Mask */
#define  SDR0_CP440_Nto1_NP  0x00000000   /* N:P CPU:PLB ratio (P > 1)        */
#define  SDR0_CP440_Nto1_N1  0x00000002   /* N:1 CPU:PLB ratio                */
#define  SDR0_CPU440_Nto1_ENCODE(n)   ((((unsigned long)(n))&0x01)<<1)
#define  SDR0_CPU440_Nto1_DECODE(n)   ((((unsigned long)(n))>>1)&0x01)
#define SDR0_CP4400_CETE      0x00000001   /* External Timer Enable       Mask */
#define  SDR0_CP440_CETE_DIS  0x0000000   /* External Timer disabled          */
#define  SDR0_CP440_CETE_EN   0x0000001   /* External Timer enabled           */

/* Bits for DDR Delay Line Register (SDR0_DDRDL) */
#define SDR0_DDRDL_TUNE      0x0000000F   /* Tune Bits                   Mask */

/* Bits for EBC Register (SDR0_EBC) */
#define SDR0_EBC_RW          0x30000000   /* ROM Width                   Mask */
#define   SDR0_EBC_RW_8      0x00000000   /* ROM Width = 8  bit               */
#define   SDR0_EBC_RW_16     0x10000000   /* ROM Width = 16 bit               */
#define   SDR0_EBC_RW_32     0x20000000   /* ROM Width = 32 bit               */
#define   SDR0_EBC_RW_ENCODE(n)        ((((unsigned long)(n))&0x03)<<28)
#define   SDR0_EBC_RW_DECODE(n)        ((((unsigned long)(n))>>28)&0x03)

/* Bits for JTAG ID Register (SDR0_JTAG) */
#define SDR0_JTAG_VERS       0xF0000000   /* Revision/EC                 Mask */
#define SDR0_JTAG_LOC        0x0F000000   /* Dev Location                Mask */
#define SDR0_JTAG_PART       0x00FFF000   /* Dev ID/Part No              Mask */
#define SDR0_JTAG_MANF       0x00000FFF   /* Manufacturing ID            Mask */

/* Bits for MAL Receive Burst Length Register (SDR0_MALRBL) */
#define SDR0_MALRBL_RBL0     0x70000000   /* Receive Burst Length 0      Mask */
#define SDR0_MALRBL_RBL1     0x07000000   /* Receive Burst Length 1      Mask */
#define SDR0_MALRBL_RBL2     0x00700000   /* Receive Burst Length 2      Mask */
#define SDR0_MALRBL_RBL3     0x00070000   /* Receive Burst Length 3      Mask */

/* Bits for MAL Receive Bus Size Register (SDR0_MALRBS) */
#define SDR0_MALRBS_RBS      0xF0000000   /* MAL Receive Bus Size        Mask */

/* Bits for MAL Transmit Burst Length Register (SDR0_MALTBL) */
#define SDR0_MALTBL_TBL0     0x70000000   /* Transfer Burst Length 0     Mask */
#define SDR0_MALTBL_TBL1     0x07000000   /* Transfer Burst Length 1     Mask */
#define SDR0_MALTBL_TBL2     0x00700000   /* Transfer Burst Length 2     Mask */
#define SDR0_MALTBL_TBL3     0x00070000   /* Transfer Burst Length 3     Mask */

/* Bits for MAL Transmit Bus Size Register (SDR0_MALTBS) */
#define SDR0_MALTBS_TBS      0xF0000000   /* MAL Transfer Bus Size       Mask */

/* Bits for Misc Function Register (SDR0_MFR) */
#define SDR0_MFR_TAH0        0x80000000   /* TAH 0 Enable                Mask */
#define   SDR0_MFR_TAH0_EN   0x00000000   /* TAH 0 Enabled                    */
#define   SDR0_MFR_TAH0_DIS  0x80000000   /* TAH 0 Disabled                   */
#define SDR0_MFR_TAH1        0x40000000   /* TAH 1 Enable                Mask */
#define   SDR0_MFR_TAH1_EN   0x00000000   /* TAH 1 Enabled                    */
#define   SDR0_MFR_TAH1_DIS  0x40000000   /* TAH 1 Disabled                   */
#define SDR0_MFR_PCM         0x10000000   /* PPC440GP Compatibility Mode Mask */
#define   SDR0_MFR_PCM_GX    0x00000000   /* PPC440GX Mode Enabled            */
#define   SDR0_MFR_PCM_GP    0x10000000   /* PPC440GP Mode Enabled            */
#define SDR0_MFR_ECS         0x08000000   /* Ethernet Clock Select       Mask */
#define   SDR0_MFR_ECS_EXT   0x00000000   /* External Clock                   */
#define   SDR0_MFR_ECS_INT   0x08000000   /* internal Clock                   */
#define SDR0_MFR_T0TxFl      0x00080000   /* Force par err TAH0 Tx 0:63  Mask */
#define   SDR0_MFR_T0TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_T0TxFl_P  0x00080000   /* Force parity error               */
#define SDR0_MFR_T0TxFh      0x00040000   /* Force par err TAH0 Tx 64:127Mask */
#define   SDR0_MFR_T0TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_T0TxFh_P  0x00040000   /* Force parity error               */
#define SDR0_MFR_T1TxFl      0x00020000   /* Force par err TAH1 Tx 0:63  Mask */
#define   SDR0_MFR_T1TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_T1TxFl_P  0x00020000   /* Force parity error               */
#define SDR0_MFR_T1TxFh      0x00010000   /* Force par err TAH1 Tx 64:127Mask */
#define   SDR0_MFR_T1TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_T1TxFh_P  0x00010000   /* Force parity error               */
#define SDR0_MFR_E0TxFl      0x00008000   /* Force par err EMAC0Tx 0:63  Mask */
#define   SDR0_MFR_E0TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E0TxFl_P  0x00008000   /* Force parity error               */
#define SDR0_MFR_E0TxFh      0x00004000   /* Force par err EMAC0Tx 64:127Mask */
#define   SDR0_MFR_E0TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E0TxFh_P  0x00004000   /* Force parity error               */
#define SDR0_MFR_E0RxFl      0x00002000   /* Force par err EMAC0Rx 0:63  Mask */
#define   SDR0_MFR_E0RxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E0RxFl_P  0x00002000   /* Force parity error               */
#define SDR0_MFR_E0RxFh      0x00001000   /* Force par err EMAC0Rx 64:127Mask */
#define   SDR0_MFR_E0RxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E0RxFh_P  0x00001000   /* Force parity error               */
#define SDR0_MFR_E1TxFl      0x00000800   /* Force par err EMAC1Tx 0:63  Mask */
#define   SDR0_MFR_E1TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E1TxFl_P  0x00000800   /* Force parity error               */
#define SDR0_MFR_E1TxFh      0x00000400   /* Force par err EMAC1Tx 64:127Mask */
#define   SDR0_MFR_E1TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E1TxFh_P  0x00000400   /* Force parity error               */
#define SDR0_MFR_E1RxFl      0x00000200   /* Force par err EMAC1Rx 0:63  Mask */
#define   SDR0_MFR_E1RxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E1RxFl_P  0x00000200   /* Force parity error               */
#define SDR0_MFR_E1RxFh      0x00000100   /* Force par err EMAC1Rx 64:127Mask */
#define   SDR0_MFR_E1RxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E1RxFh_P  0x00000100   /* Force parity error               */
#define SDR0_MFR_E2TxFl      0x00000080   /* Force par err EMAC2Tx 0:63  Mask */
#define   SDR0_MFR_E2TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E2TxFl_P  0x00000080   /* Force parity error               */
#define SDR0_MFR_E2TxFh      0x00000040   /* Force par err EMAC2Tx 64:127Mask */
#define   SDR0_MFR_E2TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E2TxFh_P  0x00000040   /* Force parity error               */
#define SDR0_MFR_E2RxFl      0x00000020   /* Force par err EMAC2Rx 0:63  Mask */
#define   SDR0_MFR_E2RxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E2RxFl_P  0x00000020   /* Force parity error               */
#define SDR0_MFR_E2RxFh      0x00000010   /* Force par err EMAC2Rx 64:127Mask */
#define   SDR0_MFR_E2RxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E2RxFh_P  0x00000010   /* Force parity error               */
#define SDR0_MFR_E3TxFl      0x00000008   /* Force par err EMAC3Tx 0:63  Mask */
#define   SDR0_MFR_E3TxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E3TxFl_P  0x00000008   /* Force parity error               */
#define SDR0_MFR_E3TxFh      0x00000004   /* Force par err EMAC3Tx 64:127Mask */
#define   SDR0_MFR_E3TxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E3TxFh_P  0x00000004   /* Force parity error               */
#define SDR0_MFR_E3RxFl      0x00000002   /* Force par err EMAC3Rx 0:63  Mask */
#define   SDR0_MFR_E3RxFl_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E3RxFl_P  0x00000002   /* Force parity error               */
#define SDR0_MFR_E3RxFh      0x00000001   /* Force par err EMAC3Rx 64:127Mask */
#define   SDR0_MFR_E3RxFh_NP 0x00000000   /* No forced parity error           */
#define   SDR0_MFR_E3RxFh_P  0x00000001   /* Force parity error               */

/* Bits for Master Interrupt Request Register 0 (SDR0_MIRQ0) */
#define SDR0_MIRQ0_ICURIRQ   0x78000000   /* ICU Read  Interrupt Request Mask */
#define SDR0_MIRQ0_DCURIRQ   0x03C00000   /* DCU Read  Interrupt Request Mask */
#define SDR0_MIRQ0_DCUWIRQ   0x001E0000   /* DCU Write Interrupt Request Mask */
#define SDR0_MIRQ0_PCIXIRQ   0x0000F000   /* PCIX      Interrupt Request Mask */
#define SDR0_MIRQ0_IMUIRQ    0x00000780   /* IMU       Interrupt Request Mask */
#define SDR0_MIRQ0_MALIRQ    0x0000003C   /* MAL       Interrupt Request Mask */

/* Bits for Master Interrupt Request Register 1 (SDR0_MIRQ1) */
#define SDR0_MIRQ1_DMAIRQ    0x78000000   /* DMA       Interrupt Request Mask */
#define SDR0_MIRQ1_OPBIRQ    0x03C00000   /* OBP-PLB   Interrupt Request Mask */

/* Bits for Pin Function Control Register 0 (SDR0_PFC0) */
#define SDR0_PFC0_G0E        0x80000000   /* GPIO 0  Enable              Mask */
#define  SDR0_PFC0_G0E_INT   0x00000000   /* Enable IRQ00 as INT              */
#define  SDR0_PFC0_G0E_GPIO  0x80000000   /* Enable IRQ00 as GPIO             */
#define SDR0_PFC0_G1E        0x40000000   /* GPIO 1  Enable              Mask */
#define  SDR0_PFC0_G1E_INT   0x00000000   /* Enable IRQ01 as INT              */
#define  SDR0_PFC0_G1E_GPIO  0x40000000   /* Enable IRQ01 as GPIO             */
#define SDR0_PFC0_G2E        0x20000000   /* GPIO 2  Enable              Mask */
#define  SDR0_PFC0_G2E_INT   0x00000000   /* Enable IRQ02 as INT              */
#define  SDR0_PFC0_G2E_GPIO  0x20000000   /* Enable IRQ02 as GPIO             */
#define SDR0_PFC0_G3E        0x10000000   /* GPIO 3  Enable              Mask */
#define  SDR0_PFC0_G3E_INT   0x00000000   /* Enable IRQ03 as INT              */
#define  SDR0_PFC0_G3E_GPIO  0x10000000   /* Enable IRQ03 as GPIO             */
#define SDR0_PFC0_G4E        0x08000000   /* GPIO 4  Enable              Mask */
#define  SDR0_PFC0_G4E_INT   0x00000000   /* Enable IRQ04 as INT              */
#define  SDR0_PFC0_G4E_GPIO  0x08000000   /* Enable IRQ04 as GPIO             */
#define SDR0_PFC0_G5E        0x04000000   /* GPIO 5  Enable              Mask */
#define  SDR0_PFC0_G5E_INT   0x00000000   /* Enable IRQ05 as INT              */
#define  SDR0_PFC0_G5E_GPIO  0x04000000   /* Enable IRQ05 as GPIO             */
#define SDR0_PFC0_G6E        0x02000000   /* GPIO 6  Enable              Mask */
#define  SDR0_PFC0_G6E_INT   0x00000000   /* Enable IRQ06 as INT              */
#define  SDR0_PFC0_G6E_GPIO  0x02000000   /* Enable IRQ06 as GPIO             */
#define SDR0_PFC0_G7E        0x01000000   /* GPIO 7  Enable              Mask */
#define  SDR0_PFC0_G7E_INT   0x00000000   /* Enable IRQ07 as INT              */
#define  SDR0_PFC0_G7E_GPIO  0x01000000   /* Enable IRQ07 as GPIO             */
#define SDR0_PFC0_G8E        0x00800000   /* GPIO 8  Enable              Mask */
#define  SDR0_PFC0_G8E_INT   0x00000000   /* Enable IRQ08 as INT              */
#define  SDR0_PFC0_G8E_GPIO  0x00800000   /* Enable IRQ08 as GPIO             */
#define SDR0_PFC0_G9E        0x00400000   /* GPIO 9  Enable              Mask */
#define  SDR0_PFC0_G9E_INT   0x00000000   /* Enable IRQ09 as INT              */
#define  SDR0_PFC0_G9E_GPIO  0x00400000   /* Enable IRQ09 as GPIO             */
#define SDR0_PFC0_G10E       0x00200000   /* GPIO 10 Enable              Mask */
#define  SDR0_PFC0_G10E_INT  0x00000000   /* Enable IRQ10 as INT              */
#define  SDR0_PFC0_G10E_GPIO 0x00200000   /* Enable IRQ10 as GPIO             */
#define SDR0_PFC0_G11E       0x00100000   /* GPIO 11 Enable              Mask */
#define  SDR0_PFC0_G11E_DIS  0x00000000   /* Disable GPIO11                   */
#define  SDR0_PFC0_G11E_EN   0x00100000   /* Enable  GPIO11                   */
#define SDR0_PFC0_G12E       0x00080000   /* GPIO 12 Enable              Mask */
#define  SDR0_PFC0_G12E_UART 0x00000000   /* Enable UART1RX as UART1RX        */
#define  SDR0_PFC0_G12E_GPIO 0x00080000   /* Enable UART1RX as GPIO12         */
#define SDR0_PFC0_G13E       0x00040000   /* GPIO 13 Enable              Mask */
#define  SDR0_PFC0_G13E_UART 0x00000000   /* Enable UART1TX as UART1TX        */
#define  SDR0_PFC0_G13E_GPIO 0x00040000   /* Enable UART1TX as GPIO13         */
#define SDR0_PFC0_G14E       0x00020000   /* GPIO 14 Enable              Mask */
#define  SDR0_PFC0_G14E_UART 0x00000000   /* En UART1DSR_CTS as UART1DSR_CTS  */
#define  SDR0_PFC0_G14E_GPIO 0x00020000   /* En UART1DSR_CTS as GPIO14        */
#define SDR0_PFC0_G15E       0x00010000   /* GPIO 15 Enable              Mask */
#define  SDR0_PFC0_G15E_UART 0x00000000   /* En UART1RTS_DTR as UART1RTS_DTR  */
#define  SDR0_PFC0_G15E_GPIO 0x00010000   /* En UART1RTS_DTR as GPIO14        */
#define SDR0_PFC0_G16E       0x00008000   /* GPIO 16 Enable              Mask */
#define  SDR0_PFC0_G16E_IIC  0x00000000   /* Enable IIC1SCL as IIC1SCL        */
#define  SDR0_PFC0_G16E_GPIO 0x00008000   /* Enable IIC1SCL as GPIO16         */
#define SDR0_PFC0_G17E       0x00004000   /* GPIO 17 Enable              Mask */
#define  SDR0_PFC0_G17E_IIC  0x00000000   /* Enable IIC1SDA as IIC1SDA        */
#define  SDR0_PFC0_G17E_GPIO 0x00004000   /* Enable IIC1SDA as GPIO17         */
#define SDR0_PFC0_GE1E       0x00003E00   /* GPIO External Int Enable    Mask */
#define  SDR0_PFC0_GE1E_EN   0x00000000   /* Enable external interrupts       */
#define  SDR0_PFC0_GE1E_TRE  0x00003E00   /* Enable ext ints based on TRE bit */
#define SDR0_PFC0_TRE        0x00000100   /* GPIO Trace Enable           Mask */
#define  SDR0_PFC0_TRE_EN    0x00000000   /* GPIO18-31 enabled                */
#define  SDR0_PFC0_TRE_DIS   0x00000100   /* GPIO18-31 disabled               */
#define   SDR0_PFC0_TRE_ENCODE(n)      ((((unsigned long)(n))&0x01)<<8)
#define   SDR0_PFC0_TRE_DECODE(n)      ((((unsigned long)(n))>>8)&0x01)

/* Bits for Pin Function Control Register 1 (SDR0_PFC1) */
#define SDR0_PFC1_EARG       0xF0000000   /* EBC Address Receiver Gating Mask */
#define SDR0_PFC1_EDRG       0x0C000000   /* EBC Data Receiver Gating    Mask */
#define  SDR0_PFC1_EDRG_DIS  0x00000000   /* Receiver Gating disabled    Mask */
#define  SDR0_PFC1_EDRG_DIS1 0x04000000   /* Disable data bus in bytes 1,2,3  */
#define  SDR0_PFC1_EDRG_DIS2 0x08000000   /* Disable data bus in bytes 2,3    */
#define  SDR0_PFC1_EDRG_DIS3 0x0C000000   /* Disable data bus in byte 3       */
#define SDR0_PFC1_U1ME       0x02000000   /* UART1 Mode Enable           Mask */
#define  SDR0_PFC1_U1ME_DTR  0x00000000   /* Enable as DTR                    */
#define  SDR0_PFC1_U1ME_RTS  0x02000000   /* Enable as RTS                    */
#define SDR0_PFC1_EPS        0x01C00000   /* Ethernet Pin Selection      Mask */
#define  SDR0_PFC1_EPS_0     0x00000000   /* Group 0:                         */
#define  SDR0_PFC1_EPS_1     0x00400000   /* Group 1:                         */
#define  SDR0_PFC1_EPS_2     0x00800000   /* Group 2:                         */
#define  SDR0_PFC1_EPS_3     0x00C00000   /* Group 3:                         */
#define  SDR0_PFC1_EPS_4     0x01000000   /* Group 4:                         */
#define  SDR0_PFC1_EPS_5     0x01400000   /* Group 5:                         */
#define  SDR0_PFC1_EPS_6     0x01800000   /* Group 6:                         */
#define  SDR0_PFC1_EPS_7     0x01C00000   /* Group 7:                         */
#define   SDR0_PFC1_EPS_ENCODE(n)      ((((unsigned long)(n))&0x07)<<22)
#define   SDR0_PFC1_EPS_DECODE(n)      ((((unsigned long)(n))>>22)&0x07)
#define SDR0_PFC1_RMII       0x00200000   /* RMII Mode                   Mask */
#define  SDR0_PFC1_RMII_100  0x00000000   /* RMII Mode 100 MBit               */
#define  SDR0_PFC1_RMII_10   0x00200000   /* RMII Mode 10  MBit               */
#define   SDR0_PFC1_RMII_ENCODE(n)     ((((unsigned long)(n))&0x01)<<21)
#define   SDR0_PFC1_RMII_DECODE(n)     ((((unsigned long)(n))>>21)&0x01)
#define SDR0_PFC1_CTEMS      0x00100000   /* CPU Trace/External Master Sel    */
#define  SDR0_PFC1_CTEMS_EXT 0x00000000   /* Use external master signals      */
#define  SDR0_PFC1_CTEMS_TRC 0x00100000   /* Use CPU trace signals            */

/* Bits for Pin Strapping Register (SDR0_PINSTP) */
#define SDR0_PINSTP_UDCD     0x80000000   /* UART0_DCD Strapping         Mask */
#define  SDR0_PINSTP_UDCD_LO 0x00000000   /* UART0_DCD strapped low           */
#define  SDR0_PINSTP_UDCD_HI 0x80000000   /* UART0_DCD strapped high          */
#define SDR0_PINSTP_UDSR     0x40000000   /* UART0_DSR Strapping         Mask */
#define  SDR0_PINSTP_UDSR_LO 0x00000000   /* UART0_DSR strapped low           */
#define  SDR0_PINSTP_UDSR_HI 0x40000000   /* UART0_DSR strapped high          */
#define SDR0_PINSTP_GTCL     0x20000000   /* GMC1TxCtl Strapping         Mask */
#define  SDR0_PINSTP_GTCL_LO 0x00000000   /* GMC1TxCtl strapped low           */
#define  SDR0_PINSTP_GTCL_HI 0x20000000   /* GMC1TxCtl strapped high          */

/* Bits for Serial Device Controller Settings Register (SDR0_SDCS) */
#define SDR0_SDCS_SBA        0xFE000000   /* Serial Device Slave Address Mask */
#define SDR0_SDCS_SDC        0x00000002   /* Serial Device Control       Mask */
#define  SDR0_SDCS_SDC_DIS   0x00000000   /* Serial device control disabled   */
#define  SDR0_SDCS_SDC_EN    0x00000002   /* Serial device control enabled    */
#define SDR0_SDCS_SDD        0x00000001   /* Serial Device Detection     Mask */
#define  SDR0_SDCS_SDD_DIS   0x00000000   /* Serial device detection disabled */
#define  SDR0_SDCS_SDD_EN    0x00000001   /* Serial device detection enabled  */
 
/* Bits for Serial Device Strap Register 0 (SDR0_SDSTP0) */
#define SDR0_SDSTP0_ENG      0x80000000   /* Engage                      Mask */
#define  SDR0_SDSTP0_ENG_SYS 0x00000000   /* Sysclk is source for PLL fwd div */
#define  SDR0_SDSTP0_ENG_PLL 0x80000000   /* PLL VCO is source for PLL fwd div*/
#define   SDR0_SDSTP0_ENG_ENCODE(n)    ((((unsigned long)(n))&0x01)<<31)
#define   SDR0_SDSTP0_ENG_DECODE(n)    ((((unsigned long)(n))>>31)&0x01)
#define SDR0_SDSTP0_SRC      0x40000000   /* PLL Feedback Source         Mask */
#define  SDR0_SDSTP0_SRC_A   0x00000000   /* Feedback originates from PLLOUTA */
#define  SDR0_SDSTP0_SRC_B   0x40000000   /* Feedback originates from PLLOUTB */
#define   SDR0_SDSTP0_SRC_ENCODE(n)    ((((unsigned long)(n))&0x01)<<30)
#define   SDR0_SDSTP0_SRC_DECODE(n)    ((((unsigned long)(n))>>30)&0x01)
#define SDR0_SDSTP0_SEL      0x38000000   /* Feedback Selection          Mask */
#define  SDR0_SDSTP0_SEL_PLL 0x00000000   /* PLL output (A or B)              */
#define  SDR0_SDSTP0_SEL_CPU 0x08000000   /* CPU                              */
#define  SDR0_SDSTP0_SEL_EBC 0x28000000   /* PERClk                           */
#define   SDR0_SDSTP0_SEL_ENCODE(n)    ((((unsigned long)(n))&0x07)<<27)
#define   SDR0_SDSTP0_SEL_DECODE(n)    ((((unsigned long)(n))>>27)&0x07)
#define SDR0_SDSTP0_TUNE     0x07FE0000   /* TUNE bits                   Mask */
#define   SDR0_SDSTP0_TUNE_ENCODE(n)   ((((unsigned long)(n))&0x3FF)<<17)
#define   SDR0_SDSTP0_TUNE_DECODE(n)   ((((unsigned long)(n))>>17)&0x3FF)
#define SDR0_SDSTP0_FBDV     0x0001F000   /* PLL Feedback Divisor        Mask */
#define   SDR0_SDSTP0_FBDV_ENCODE(n)   ((((unsigned long)(n))&0x1F)<<12)
#define   SDR0_SDSTP0_FBDV_DECODE(n)   ((((((unsigned long)(n))>>12)-1)&0x1F)+1)
#define SDR0_SDSTP0_FWDVA    0x00000F00   /* PLL Forward Divisor A       Mask */
#define  SDR0_SDSTP0_FWDVA_16 0x00000000  /* Forward Divisor A = 16           */
#define  SDR0_SDSTP0_FWDVA_1  0x00000100  /* Forward Divisor A = 1            */
#define  SDR0_SDSTP0_FWDVA_2  0x00000200  /* Forward Divisor A = 2            */
#define  SDR0_SDSTP0_FWDVA_3  0x00000300  /* Forward Divisor A = 3            */
#define  SDR0_SDSTP0_FWDVA_4  0x00000400  /* Forward Divisor A = 4            */
#define  SDR0_SDSTP0_FWDVA_5  0x00000500  /* Forward Divisor A = 5            */
#define  SDR0_SDSTP0_FWDVA_6  0x00000600  /* Forward Divisor A = 6            */
#define  SDR0_SDSTP0_FWDVA_7  0x00000700  /* Forward Divisor A = 7            */
#define  SDR0_SDSTP0_FWDVA_8  0x00000800  /* Forward Divisor A = 8            */
#define  SDR0_SDSTP0_FWDVA_10 0x00000A00  /* Forward Divisor A = 10           */
#define  SDR0_SDSTP0_FWDVA_12 0x00000C00  /* Forward Divisor A = 12           */
#define  SDR0_SDSTP0_FWDVA_14 0x00000D00  /* Forward Divisor A = 14           */
#define   SDR0_SDSTP0_FWDVA_ENCODE(n)  ((((unsigned long)(n))&0x0F)<<8)
#define   SDR0_SDSTP0_FWDVA_DECODE(n)  ((((((unsigned long)(n))>>8)-1)&0x0F)+1)
#define SDR0_SDSTP0_FWDVB    0x000000E0   /* PLL Forward Divisor B       Mask */
#define  SDR0_SDSTP0_FWDVB_8  0x00000000  /* Forward Divisor B = 8            */
#define  SDR0_SDSTP0_FWDVB_1  0x00000020  /* Forward Divisor B = 1            */
#define  SDR0_SDSTP0_FWDVB_2  0x00000040  /* Forward Divisor B = 2            */
#define  SDR0_SDSTP0_FWDVB_3  0x00000060  /* Forward Divisor B = 3            */
#define  SDR0_SDSTP0_FWDVB_4  0x00000080  /* Forward Divisor B = 4            */
#define  SDR0_SDSTP0_FWDVB_5  0x000000A0  /* Forward Divisor B = 5            */
#define  SDR0_SDSTP0_FWDVB_6  0x000000C0  /* Forward Divisor B = 6            */
#define  SDR0_SDSTP0_FWDVB_7  0x000000E0  /* Forward Divisor B = 7            */
#define   SDR0_SDSTP0_FWDVB_ENCODE(n)  ((((unsigned long)(n))&0x07)<<5)
#define   SDR0_SDSTP0_FWDVB_DECODE(n)  ((((((unsigned long)(n))>>5)-1)&0x07)+1)
#define SDR0_SDSTP0_PRBDV0   0x0000001C   /* PLL Primary Divisor B       Mask */
#define  SDR0_SDSTP0_PRBDV0_8 0x00000000  /* Primary Divisor B = 8            */
#define  SDR0_SDSTP0_PRBDV0_1 0x00000004  /* Primary Divisor B = 1            */
#define  SDR0_SDSTP0_PRBDV0_2 0x00000008  /* Primary Divisor B = 2            */
#define  SDR0_SDSTP0_PRBDV0_3 0x0000000C  /* Primary Divisor B = 3            */
#define  SDR0_SDSTP0_PRBDV0_4 0x00000010  /* Primary Divisor B = 4            */
#define  SDR0_SDSTP0_PRBDV0_5 0x00000014  /* Primary Divisor B = 5            */
#define  SDR0_SDSTP0_PRBDV0_6 0x00000018  /* Primary Divisor B = 6            */
#define  SDR0_SDSTP0_PRBDV0_7 0x0000001C  /* Primary Divisor B = 7            */
#define   SDR0_SDSTP0_PRBDV0_ENCODE(n) ((((unsigned long)(n))&0x07)<<2)
#define   SDR0_SDSTP0_PRBDV0_DECODE(n) ((((((unsigned long)(n))>>2)-1)&0x07)+1)
#define SDR0_SDSTP0_OPBDV0    0x00000003  /* OPB Clock Divisor 0         Mask */
#define  SDR0_SDSTP0_OPBDV0_4 0x00000000  /* OPB Clock Divisor 0 = 4          */
#define  SDR0_SDSTP0_OPBDV0_1 0x00000001  /* OPB Clock Divisor 0 = 1          */
#define  SDR0_SDSTP0_OPBDV0_2 0x00000002  /* OPB Clock Divisor 0 = 2          */
#define  SDR0_SDSTP0_OPBDV0_3 0x00000003  /* OPB Clock Divisor 0 = 3          */
#define   SDR0_SDSTP0_OPBDV0_ENCODE(n) ((((unsigned long)(n))&0x03)<<0)
#define   SDR0_SDSTP0_OPBDV0_DECODE(n) ((((((unsigned long)(n))>>0)-1)&0x03)+1)

/* Bits for Serial Device Strap Register 1 (SDS0_SDSTP1) */
#define SDR0_SDSTP1_LFBDV    0xFC000000   /* PLL Local Feedback Divisor  Mask */
#define   SDR0_SDSTP1_LFBDV_ENCODE(n)  ((((unsigned long)(n))&0x3F)<<26)
#define   SDR0_SDSTP1_LFBDV_DECODE(n)  ((((unsigned long)(n))>>26)&0x3F)
#define SDR0_SDSTP1_PERD0    0x03000000   /* PER Clock Divisor 0         Mask */
#define  SDR0_SDSTP1_PERDV0_4 0x00000000  /* PER Clock Divisor 0 = 4          */
#define  SDR0_SDSTP1_PERDV0_1 0x01000000  /* PER Clock Divisor 0 = 1          */
#define  SDR0_SDSTP1_PERDV0_2 0x02000000  /* PER Clock Divisor 0 = 2          */
#define  SDR0_SDSTP1_PERDV0_3 0x03000000  /* PER Clock Divisor 0 = 3          */
#define   SDR0_SDSTP1_EBCDV0_ENCODE(n) ((((unsigned long)(n))&0x03)<<24)
#define   SDR0_SDSTP1_EBCDV0_DECODE(n) ((((unsigned long)(n))>>24)&0x03)
#define SDR0_SDSTP1_MALD0    0x00C00000   /* MAL Clock Divisor     Mask       */
#define  SDR0_SDSTP1_MALDV0_4 0x00000000  /* MAL Clock Divisor 0 = 4          */
#define  SDR0_SDSTP1_MALDV0_1 0x00400000  /* MAL Clock Divisor 0 = 1          */
#define  SDR0_SDSTP1_MALDV0_2 0x00800000  /* MAL Clock Divisor 0 = 2          */
#define  SDR0_SDSTP1_MALDV0_3 0x00C00000  /* MAL Clock Divisor 0 = 3          */
#define   SDR0_SDSTP1_MALDV0_ENCODE(n) ((((unsigned long)(n))&0x03)<<22)
#define   SDR0_SDSTP1_MALDV0_DECODE(n) ((((unsigned long)(n))>>22)&0x03)
#define SDR0_SDSTP1_RW       0x00300000   /* ROM Width                   Mask */
#define  SDR0_SDSTP1_RW_8    0x00000000   /* ROM Width = 8  bit               */
#define  SDR0_SDSTP1_RW_16   0x00100000   /* ROM Width = 16 bit               */
#define  SDR0_SDSTP1_RW_32   0x00200000   /* ROM Width = 32 bit               */
#define   SDR0_SDSTP1_RW_ENCODE(n)     ((((unsigned long)(n))&0x03)<<20)
#define   SDR0_SDSTP1_RW_DECODE(n)     ((((unsigned long)(n))>>20)&0x03)
#define SDR0_SDSTP1_EARV     0x00080000   /* ERPN Address Reset Vector   Mask */
#define  SDR0_SDSTP1_EARV_EBC 0x00000000  /* Boot from EBC                   */
#define  SDR0_SDSTP1_EARV_PCI 0x00080000  /* Boot from PCI                   */
#define   SDR0_SDSTP1_EARV_ENCODE(n)   ((((unsigned long)(n))&0x01)<<19)
#define   SDR0_SDSTP1_EARV_DECODE(n)   ((((unsigned long)(n))>>19)&0x01)
#define SDR0_SDSTP1_PAE      0x00040000   /* PCI Arbiter Enable          Mask */
#define  SDR0_SDSTP1_PAE_DIS 0x00000000   /* PCI arbiter disabled             */
#define  SDR0_SDSTP1_PAE_EN  0x00040000   /* PCI arbiter enabled              */
#define   SDR0_SDSTP1_PAE_ENCODE(n)    ((((unsigned long)(n))&0x01)<<18)
#define   SDR0_SDSTP1_PAE_DECODE(n)    ((((unsigned long)(n))>>18)&0x01)
#define SDR0_SDSTP1_PHCE     0x00020000   /* PCI Host Config Enable      Mask */
#define  SDR0_SDSTP1_PHCE_DIS 0x00000000  /* PCI host config disabled         */
#define  SDR0_SDSTP1_PHCE_EN  0x00020000  /* PCI host config enabled          */
#define   SDR0_SDSTP1_PHCE_ENCODE(n)   ((((unsigned long)(n))&0x01)<<17)
#define   SDR0_SDSTP1_PHCE_DECODE(n)   ((((unsigned long)(n))>>17)&0x01)
#define SDR0_SDSTP1_PISE     0x00010000   /* PCI Initial Seq Enable      Mask */
#define  SDR0_SDSTP1_PISE_DIS 0x00000000  /* PCI initial seq disabled         */
#define  SDR0_SDSTP1_PISE_EN  0x00010000  /* PCI initial seq enabled          */
#define   SDR0_SDSTP1_PISE_ENCODE(n)   ((((unsigned long)(n))&0x01)<<16)
#define   SDR0_SDSTP1_PISE_DECODE(n)   ((((unsigned long)(n))>>16)&0x01)
#define SDR0_SDSTP1_PCWE     0x00008000   /* PCI Local CPU Wait Enable   Mask */
#define  SDR0_SDSTP1_PCWE_DIS 0x00000000  /* PCI local CPU wait disabled      */
#define  SDR0_SDSTP1_PCWE_EN  0x00008000  /* PCI local CPU wait enabled       */
#define   SDR0_SDSTP1_PCWE_ENCODE(n)   ((((unsigned long)(n))&0x01)<<15)
#define   SDR0_SDSTP1_PCWE_DECODE(n)   ((((unsigned long)(n))>>15)&0x01)
#define SDR0_SDSTP1_PPIM     0x00007800   /* PCI Inbound Map Settings    Mask */
#define  SDR0_SDSTP1_PPIM_0  0x00000000   /* PIM0,1,2 off                     */
#define  SDR0_SDSTP1_PPIM_1  0x00000800   /* PIM0 4k,   PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_2  0x00001000   /* PIM0 1M,   PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_3  0x00001800   /* PIM0 64M,  PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_4  0x00002000   /* PIM0 4kp,  PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_5  0x00002800   /* PIM0 1Mp,  PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_6  0x00003000   /* PIM0 64Mp, PIM1,2 off            */
#define  SDR0_SDSTP1_PPIM_7  0x00003800   /* PIM0 64k,  PIM1 off, PIM2 16k    */
#define  SDR0_SDSTP1_PPIM_8  0x00004000   /* PIM0 1M,   PIM1 off, PIM2 64k    */
#define  SDR0_SDSTP1_PPIM_9  0x00004800   /* PIM0 64kp, PIM1 off, PIM2 16k    */
#define  SDR0_SDSTP1_PPIM_10 0x00005000   /* PIM0 1Mp,  PIM1 off, PIM2 64k    */
#define  SDR0_SDSTP1_PPIM_11 0x00005800   /* PIM0 64k,  PIM1 off, PIM2 64kp   */
#define  SDR0_SDSTP1_PPIM_12 0x00006000   /* PIM0 1M,   PIM1 off, PIM2 1Mp    */
#define  SDR0_SDSTP1_PPIM_13 0x00006800   /* PIM0 1Mp,  PIM1 off, PIM2 1Mp    */
#define  SDR0_SDSTP1_PPIM_14 0x00007000   /* PIM0 1M,   PIM1 on , PIM2 off    */
#define  SDR0_SDSTP1_PPIM_15 0x00007800   /* PIM0 1M,   PIM1 on , PIM2 16k    */
#define SDR0_SDSTP1_PR64E    0x00000400   /* PCI Initialize Req64 Enable Mask */
#define   SDR0_SDSTP1_PR64E_ENCODE(n)  ((((unsigned long)(n))&0x01)<<10)
#define   SDR0_SDSTP1_PR64E_DECODE(n)  ((((unsigned long)(n))>>10)&0x01)
#define SDR0_SDSTP1_PXFS     0x00000300   /* PCIX Frequency Selection    Mask */
#define  SDR0_SDSTP1_PXFS_100 0x00000000  /* 100-133MHz freq selection        */
#define  SDR0_SDSTP1_PXFS_66  0x00000100  /* 66 -100MHz freq selection        */
#define  SDR0_SDSTP1_PXFS_50  0x00000200  /* 50 -66 MHz freq selection        */
#define   SDR0_SDSTP1_PXFS_ENCODE(n)   ((((unsigned long)(n))&0x03)<<8)
#define   SDR0_SDSTP1_PXFS_DECODE(n)   ((((unsigned long)(n))>>8)&0x03)
#define SDR0_SDSTP1_PDM      0x00000040   /* PCIX Driver Mode            Mask */
#define  SDR0_SDSTP1_PDM_MP  0x00000000   /* PCIX multipoint mode             */
#define  SDR0_SDSTP1_PDM_PP  0x00000040   /* PCIX point to point mode         */
#define   SDR0_SDSTP1_PDM_ENCODE(n)    ((((unsigned long)(n))&0x01)<<6)
#define   SDR0_SDSTP1_PDM_DECODE(n)    ((((unsigned long)(n))>>6)&0x01)
#define SDR0_SDSTP1_EPS      0x00000038   /* Ethernet Pin Select         Mask */
#define  SDR0_SDSTP1_EPS_0   0x00000000   /* Group 0                          */
#define  SDR0_SDSTP1_EPS_1   0x00000008   /* Group 1                          */
#define  SDR0_SDSTP1_EPS_2   0x00000010   /* Group 2                          */
#define  SDR0_SDSTP1_EPS_3   0x00000018   /* Group 3                          */
#define  SDR0_SDSTP1_EPS_4   0x00000020   /* Group 4                          */
#define  SDR0_SDSTP1_EPS_5   0x00000028   /* Group 5                          */
#define  SDR0_SDSTP1_EPS_6   0x00000030   /* Group 6                          */
#define  SDR0_SDSTP1_EPS_7   0x00000038   /* Group 7                          */
#define   SDR0_SDSTP1_EPS_ENCODE(n)    ((((unsigned long)(n))&0x07)<<3)
#define   SDR0_SDSTP1_EPS_DECODE(n)    ((((unsigned long)(n))>>3)&0x07)
#define SDR0_SDSTP1_RMII     0x00000004   /* RMII Mode                   Mask */
#define  SDR0_SDSTP1_RMII_100 0x00000000  /* RMII Mode 100 MBit               */
#define  SDR0_SDSTP1_RMII_10  0x00000004  /* RMII Mode 10  MBit               */
#define   SDR0_SDSTP1_RMII_ENCODE(n)   ((((unsigned long)(n))&0x01)<<2)
#define   SDR0_SDSTP1_RMII_DECODE(n)   ((((unsigned long)(n))>>2)&0x01)
#define SDR0_SDSTP1_TRE      0x00000002   /* GPIO Trace Enable           Mask */
#define  SDR0_SDSTP1_TRE_EN  0x00000000   /* GPIO18-31 enabled                */
#define  SDR0_SDSTP1_TRE_DIS 0x00000002   /* GPIO18-31 disabled               */
#define   SDR0_SDSTP1_TRE_ENCODE(n)    ((((unsigned long)(n))&0x01)<<1)
#define   SDR0_SDSTP1_TRE_DECODE(n)    ((((unsigned long)(n))>>1)&0x01)
#define SDR0_SDSTP1_Nto1     0x00000001   /* PLB/CPU N to 1 clk ratio    Mask */
#define  SDR0_SDSTP1_Nto1_NP 0x00000000   /* N:P CPU:PLB ratio (P > 1)        */
#define  SDR0_SDSTP1_Nto1_N1 0x00000001   /* N:1 CPU:PLB ratio                */
#define   SDR0_SDSTP1_NTO1_ENCODE(n)   ((((unsigned long)(n))&0x01)<<0)
#define   SDR0_SDSTP1_NTO1_DECODE(n)   ((((unsigned long)(n))>>0)&0x01)

/* Bits for PLB Slave Address PipeLine Register (SDR0_SLPIPE) */
#define SDR0_SLPIPE_SRAP     0x80000000   /* SRAM Address Pipeline       Mask */
#define  SDR0_SLPIPE_SRAP_DIS 0x00000000  /* SRAM address pipeline disabled   */
#define  SDR0_SLPIPE_SRAP_EN  0x80000000  /* SRAM address pipeline enabled    */
#define SDR0_SLPIPE_DDRAP    0x40000000   /* DDR SDRAM Address Pipeline  Mask */
#define  SDR0_SLPIPE_DDRAP_DIS 0x00000000 /* DDR SDRAM address pipeline disabl*/
#define  SDR0_SLPIPE_DDRAP_EN  0x40000000 /* DDR SDRAM address pipeline enable*/
#define SDR0_SLPIPE_POBAP    0x20000000   /* PLB-OPB Br Address Pipeline Mask */
#define  SDR0_SLPIPE_POBAP_DIS 0x00000000 /* PLB-OPB Br address pipeline dis  */
#define  SDR0_SLPIPE_POBAP_EN  0x20000000 /* PLB-OPB Br address pipeline en   */
#define SDR0_SLPIPE_IMUAP    0x10000000   /* IMU       Address Pipeline  Mask */
#define  SDR0_SLPIPE_IMUAP_DIS 0x00000000 /* IMU address pipeline disabled    */
#define  SDR0_SLPIPE_IMUAP_EN  0x10000000 /* IMU address pipeline enabled     */

/* Bits for Soft Reset Register (SDR0_SRST) */
#define SDR0_SRST_BGO        0x80000000   /* PLB to OPB Bridge                */
#define SDR0_SRST_PLB        0x40000000   /* PLB arbiter                      */
#define SDR0_SRST_EBC        0x20000000   /* External Bus Controller          */
#define SDR0_SRST_OPB        0x10000000   /* OPB Arbiter                      */
#define SDR0_SRST_UART0      0x08000000   /* UART0                            */
#define SDR0_SRST_UART1      0x04000000   /* UART1                            */
#define SDR0_SRST_IIC0       0x02000000   /* IIC0                             */
#define SDR0_SRST_IIC1       0x01000000   /* IIC1                             */
#define SDR0_SRST_GPIO       0x00800000   /* GPIO                             */
#define SDR0_SRST_GPT        0x00400000   /* General Purpose Timer            */
#define SDR0_SRST_DMC        0x00200000   /* DDR SDRAM Memory Controller      */
#define SDR0_SRST_PCI        0x00100000   /* PCI                              */
#define SDR0_SRST_EMAC0      0x00080000   /* EMAC0                            */
#define SDR0_SRST_EMAC1      0x00040000   /* EMAC1                            */
#define SDR0_SRST_CPM        0x00020000   /* Clock and Power Management       */
#define SDR0_SRST_IMU        0x00010000   /* I2O Messaging Unit               */
#define SDR0_SRST_UIC01      0x00008000   /* UIC 0,1                          */
#define SDR0_SRST_UICB2      0x00004000   /* UIC B,2                          */
#define SDR0_SRST_SRAM       0x00002000   /* Internal SRAM Controller         */
#define SDR0_SRST_EBM        0x00001000   /* External Bus Master              */
#define SDR0_SRST_BGI        0x00000800   /* OPB to PLB Bridge                */
#define SDR0_SRST_DMA        0x00000400   /* DMA Controller                   */
#define SDR0_SRST_DMAC       0x00000200   /* DMA Channel                      */
#define SDR0_SRST_MAL        0x00000100   /* MAL                              */
#define SDR0_SRST_ZMII       0x00000080   /* ZMII                             */
#define SDR0_SRST_GPTR       0x00000040   /* General Purpose Timer            */
#define SDR0_SRST_PPM        0x00000020   /* PLB Performance Monitor          */
#define SDR0_SRST_EMAC2      0x00000010   /* EMAC2, TAHOE on EMAC0            */
#define SDR0_SRST_EMAC3      0x00000008   /* EMAC3, TAHOE on EMAC1            */
#define SDR0_SRST_L2C        0x00000004   /* L2 Cache Controller              */
#define SDR0_SRST_RGMII      0x00000001   /* RGMII                            */

/* Bits for UART0,1 Register (SDR0_UART0/1) */
#define SDR0_UARTX_UXICS     0xF0000000   /* Int Source for UART Clk Div Mask */
#define   SDR0_UARTX_UXICS_PLB 0x20000000 /* = PLB                            */
#define SDR0_UARTX_UXEC      0x00800000   /* UART Clock Selection        Mask */
#define   SDR0_UARTX_UXEC_INT  0x00000000 /* Internal Clock Source            */
#define   SDR0_UARTX_UXEC_EXT  0x00800000 /* External Clock Source            */
#define SDR0_UARTX_UXDTE     0x00400000   /* UART DMA Tx Enable          Mask */
#define   SDR0_UARTX_UXDTE_DIS 0x00000000 /* UART DMA Tx Disabled             */
#define   SDR0_UARTX_UXDTE_EN  0x00400000 /* UART DMA Tx Enabled              */
#define SDR0_UARTX_UXDRE     0x00200000   /* UART DMA Rx Enable          Mask */
#define   SDR0_UARTX_UXDRE_DIS 0x00000000 /* UART DMA Rx Disabled             */
#define   SDR0_UARTX_UXDRE_EN  0x00200000 /* UART DMA Rx Enabled              */
#define SDR0_UARTX_UXDC      0x00100000   /* UART DMA Allow Enable Clear Mask */
#define   SDR0_UARTX_UXDC_NC   0x00000000 /* U0DTE/DRE not cleared on TC      */
#define   SDR0_UARTX_UXDC_CLR  0x00100000 /* U0DTE/DRE cleared on TC          */
#define SDR0_UARTX_UXDIV     0x000000FF   /* UART Clock Divider          Mask */
#define   SDR0_UARTX_UXDIV_ENCODE(n)   ((((unsigned long)(n))&0xFF)<<0)

/* Conditionally define this macro in case BSPs already define it */
#ifndef SDR0_UARTX_UXDIV_DECODE
# define   SDR0_UARTX_UXDIV_DECODE(n)   ((((unsigned long)(n))>>0)&0xFF)
#endif

/* Bits for PCIX Control Register (SDR0_XCR) */
#define SDR0_XCR_PAE         0x80000000   /* PCI Arbiter Enable          Mask */
#define  SDR0_XCR_PAE_DIS    0x00000000   /* PCI arbiter disabled             */
#define  SDR0_XCR_PAE_EN     0x80000000   /* PCI arbiter enabled              */
#define   SDR0_XCR_PAE_ENCODE(n)       ((((unsigned long)(n))&0x01)<<31)
#define   SDR0_XCR_PAE_DECODE(n)       ((((unsigned long)(n))>>31)&0x01)
#define SDR0_XCR_PHCE        0x40000000   /* PCI Host Config Enable      Mask */
#define  SDR0_XCR_PHCE_DIS   0x00000000   /* PCI host config disabled         */
#define  SDR0_XCR_PHCE_EN    0x40000000   /* PCI host config enabled          */
#define   SDR0_XCR_PHCE_ENCODE(n)      ((((unsigned long)(n))&0x01)<<30)
#define   SDR0_XCR_PHCE_DECODE(n)      ((((unsigned long)(n))>>30)&0x01)
#define SDR0_XCR_PISE        0x20000000   /* PCI Initial Sequence Enable Mask */
#define  SDR0_XCR_PISE_DIS   0x00000000   /* PCI init sequence disabled       */
#define  SDR0_XCR_PISE_EN    0x20000000   /* PCI init sequence enabled        */
#define   SDR0_XCR_PISE_ENCODE(n)      ((((unsigned long)(n))&0x01)<<29)
#define   SDR0_XCR_PISE_DECODE(n)      ((((unsigned long)(n))>>29)&0x01)
#define SDR0_XCR_PCWE        0x10000000   /* PCI Local CPU Wait Enable   Mask */
#define  SDR0_XCR_PCWE_DIS   0x00000000   /* PCI local CPU wait disabled      */
#define  SDR0_XCR_PCWE_EN    0x10000000   /* PCI local CPU wait enabled       */
#define   SDR0_XCR_PCWE_ENCODE(n)      ((((unsigned long)(n))&0x01)<<28)
#define   SDR0_XCR_PCWE_DECODE(n)      ((((unsigned long)(n))>>28)&0x01)
#define SDR0_XCR_PPIM        0x0F000000   /* PCI Inbound Map Settings    Mask */
#define  SDR0_XCR_PPIM_0     0x00000000   /* PIM0,1,2 off                     */
#define  SDR0_XCR_PPIM_1     0x01000000   /* PIM0 4k,   PIM1,2 off            */
#define  SDR0_XCR_PPIM_2     0x02000000   /* PIM0 1M,   PIM1,2 off            */
#define  SDR0_XCR_PPIM_3     0x03000000   /* PIM0 64M,  PIM1,2 off            */
#define  SDR0_XCR_PPIM_4     0x04000000   /* PIM0 4kp,  PIM1,2 off            */
#define  SDR0_XCR_PPIM_5     0x05000000   /* PIM0 1Mp,  PIM1,2 off            */
#define  SDR0_XCR_PPIM_6     0x06000000   /* PIM0 64Mp, PIM1,2 off            */
#define  SDR0_XCR_PPIM_7     0x07000000   /* PIM0 64k,  PIM1 off, PIM2 16k    */
#define  SDR0_XCR_PPIM_8     0x08000000   /* PIM0 1M,   PIM1 off, PIM2 64k    */
#define  SDR0_XCR_PPIM_9     0x09000000   /* PIM0 64kp, PIM1 off, PIM2 16k    */
#define  SDR0_XCR_PPIM_10    0x0A000000   /* PIM0 1Mp,  PIM1 off, PIM2 64k    */
#define  SDR0_XCR_PPIM_11    0x0B000000   /* PIM0 64k,  PIM1 off, PIM2 64kp   */
#define  SDR0_XCR_PPIM_12    0x0C000000   /* PIM0 1M,   PIM1 off, PIM2 1Mp    */
#define  SDR0_XCR_PPIM_13    0x0D000000   /* PIM0 1Mp,  PIM1 off, PIM2 1Mp    */
#define  SDR0_XCR_PPIM_14    0x0E000000   /* PIM0 1M,   PIM1 on , PIM2 off    */
#define  SDR0_XCR_PPIM_15    0x0F000000   /* PIM0 1M,   PIM1 on , PIM2 16k    */
#define SDR0_XCR_PR64E       0x00800000   /* PCI Init Req64 Enable       Mask */
#define  SDR0_XCR_PR64E_DIS  0x00000000   /* PCI init Req64 disabled          */
#define  SDR0_XCR_PR64E_EN   0x00800000   /* PCI init Req64 enabled           */
#define   SDR0_XCR_PR64E_ENCODE(n)     ((((unsigned long)(n))&0x01)<<23)
#define   SDR0_XCR_PR64E_DECODE(n)     ((((unsigned long)(n))>>23)&0x01)
#define SDR0_XCR_PFXS        0x00600000   /* PCIX Frequency Selection    Mask */
#define  SDR0_XCR_PXFS_HIGH  0x00000000   /* 100-133MHz freq selection        */
#define  SDR0_XCR_PXFS_MED   0x00200000   /* 66 -100MHz freq selection        */
#define  SDR0_XCR_PXFS_LOW   0x00400000   /* 50 -66 MHz freq selection        */
#define   SDR0_XCR_PXFS_ENCODE(n)      ((((unsigned long)(n))&0x03)<<21)
#define   SDR0_XCR_PXFS_DECODE(n)      ((((unsigned long)(n))>>21)&0x03)
#define SDR0_XCR_PDM         0x00080000   /* PCIX Driver Mode            Mask */
#define  SDR0_XCR_PDM_MP     0x00000000   /* PCIX multipoint mode             */
#define  SDR0_XCR_PDM_PP     0x00080000   /* PCIX point to point mode         */
#define   SDR0_XCR_PDM_ENCODE(n)       ((((unsigned long)(n))&0x01)<<19)
#define   SDR0_XCR_PDM_DECODE(n)       ((((unsigned long)(n))>>19)&0x01)

/* Bits for PCIX PLL Control Register (SDR0_XPLLC) */
#define SDR0_XPLLC_RST       0x80000000   /* Reset                       Mask */
#define  SDR0_XPLLC_RST_LCK  0x00000000   /* PLL allowed to lock              */
#define  SDR0_XPLLC_RST_RST  0x80000000   /* PLL is forced into reset         */
#define SDR0_XPLLC_SRC       0x20000000   /* PLL Feedback Source         Mask */
#define  SDR0_XPLLC_SRC_OUTA 0x00000000   /* PLL feedback from OUTA           */
#define  SDR0_XPLLC_SRC_OUTB 0x20000000   /* PLL feedback from OUTB           */
#define SDR0_XPLLC_SEL       0x0F000000   /* PLL Feedback Selection      Mask */
#define  SDR0_XPLLC_SEL_PLL  0x00000000   /* PLL output (A or B)              */
#define SDR0_XPLLC_TUNE      0x000003FF   /* PLL TUNE bits               Mask */

/* Bits for PCIX PLL Divisor Register (SDR0_XPLLD) */
#define SDR0_XPLLD_FBDV      0xFF000000   /* PLL Feedback Divisor        Mask */
#define SDR0_XPLLD_FWDVA     0x00FF0000   /* PLL Forward  Divisor A      Mask */

#endif  /* INCsdrDcrh  */
