/* mpc8540.h - Register definitions for mpc8540 Integrated Host Processor */

/*
 * Copyright (c) 2004-2006, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01k,07may13,d_l Fix build warnings.
01j,12dec06,jch (WRS) Updated for VxWorks 6.x compatibility.
01i,02aug05,cak Very minor fix to one of the definitions.
01h,05apr05,cak Removed LAWAR_SZ... definitions that are no longer needed.
01g,21mar05,efb Added EIEIO_SYNC to CCSR register write macros.
01f,09dec04,cak Add DDR SDRAM memory controller related definitions.
01e,01dec04,scb Correct CCSR_IIDR26.
01d,19nov04,scb Add support for show routines plus some new PCI stuff.
01c,09nov04,scb Add I2C related #defines and support macros.
01b,11oct04,scb Add some TLB related defines.
01a,25aug04,scb Initial writing
*/

/*
This file contains all of the register definitions associated with the
mpc8540 Integrated Host Processor.  Registers which are in the memory
mapped Command Control and Status Register space have mnemonics which
are prepended with "CCSR_".
*/

#ifndef __INCmpc8540h
#define __INCmpc8540h

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* Configuration, Control, and Status Registers */

#define CCSR_CCSRBAR                       0x00000
#define CCSR_ALTCBAR                       0x00008
#define CCSR_ALTCAR                        0x00010
#define CCSR_BPTR                          0x00020

/* Local Access Window Base and Size Registers */

#define CCSR_LAWBAR0                       0x00C08
#define CCSR_LAWAR0                        0x00C10
#define CCSR_LAWBAR1                       0x00C28
#define CCSR_LAWAR1                        0x00C30
#define CCSR_LAWBAR2                       0x00C48
#define CCSR_LAWAR2                        0x00C50
#define CCSR_LAWBAR3                       0x00C68
#define CCSR_LAWAR3                        0x00C70
#define CCSR_LAWBAR4                       0x00C88
#define CCSR_LAWAR4                        0x00C90
#define CCSR_LAWBAR5                       0x00CA8
#define CCSR_LAWAR5                        0x00CB0
#define CCSR_LAWBAR6                       0x00CC8
#define CCSR_LAWAR6                        0x00CD0
#define CCSR_LAWBAR7                       0x00CE8
#define CCSR_LAWAR7                        0x00CF0

/* ECM Registers */

#define CCSR_EEBACR                        0x01000
#define CCSR_EEBPCR                        0x01010
#define CCSR_EEDR                          0x01E00
#define CCSR_EEER                          0x01E08
#define CCSR_EEATR                         0x01E0C
#define CCSR_EEADR                         0x01E10

/* DDR Memory Controller Memory Map */

#define CCSR_CS0_BNDS                      0x02000
#define CCSR_CS1_BNDS                      0x02008
#define CCSR_CS2_BNDS                      0x02010
#define CCSR_CS3_BNDS                      0x02018
#define CCSR_CS0_CONFIG                    0x02080
#define CCSR_CS1_CONFIG                    0x02084
#define CCSR_CS2_CONFIG                    0x02088
#define CCSR_CS3_CONFIG                    0x0208C
#define CCSR_TIMING_CFG_1                  0x02108
#define CCSR_TIMING_CFG_2                  0x0210C
#define CCSR_DDR_SDRAM_CFG                 0x02110
#define CCSR_DDR_SDRAM_MODE                0x02118
#define CCSR_DDR_SDRAM_INTERVAL            0x02124
#define CCSR_DATA_ERR_INJECT_HI            0x02E00
#define CCSR_DATA_ERR_INJECT_LO            0x02E04
#define CCSR_ECC_ERR_INJECT                0x02E08
#define CCSR_CAPTURE_DATA_HI               0x02E20
#define CCSR_CAPTURE_DATA_LO               0x02E24
#define CCSR_CAPTURE_ECC                   0x02E28
#define CCSR_ERR_DETECT                    0x02E40
#define CCSR_ERR_DISABLE                   0x02E44
#define CCSR_ERR_INT_EN                    0x02E48
#define CCSR_CAPTURE_ATTRIBUTES            0x02E4C
#define CCSR_CAPTURE_ADDRESS               0x02E50
#define CCSR_ERR_SBE                       0x02E58
#define CCSR_I2CADR                        0x03000
#define CCSR_I2CFDR                        0x03004
#define CCSR_I2CCR                         0x03008
#define CCSR_I2CSR                         0x0300C
#define CCSR_I2CDR                         0x03010
#define CCSR_I2CDFSRR                      0x03014

/* DUART Registers */

#define CCSR_URBR0                         0x04500
#define CCSR_UTHR0                         0x04500
#define CCSR_UDLB0                         0x04500
#define CCSR_UIER0                         0x04501
#define CCSR_UDMB0                         0x04501
#define CCSR_UIIR0                         0x04502
#define CCSR_UFCR0                         0x04502
#define CCSR_UAFR0                         0x04502
#define CCSR_ULCR0                         0x04503
#define CCSR_UMCR0                         0x04504
#define CCSR_ULSR0                         0x04505
#define CCSR_UMSR0                         0x04506
#define CCSR_USCR0                         0x04507
#define CCSR_UDSR0                         0x04510
#define CCSR_URBR1                         0x04600
#define CCSR_UTHR1                         0x04600
#define CCSR_UDLB1                         0x04600
#define CCSR_UIER1                         0x04601
#define CCSR_UDMB1                         0x04601
#define CCSR_UIIR1                         0x04602
#define CCSR_UFCR1                         0x04602
#define CCSR_UAFR1                         0x04602
#define CCSR_ULCR1                         0x04603
#define CCSR_UMCR1                         0x04604
#define CCSR_ULSR1                         0x04605
#define CCSR_UMSR1                         0x04606
#define CCSR_USCR1                         0x04607
#define CCSR_UDSR1                         0x04610

/* Local Bus Controller Registers */

#define CCSR_BR0                           0x05000
#define CCSR_BR1                           0x05008
#define CCSR_BR2                           0x05010
#define CCSR_BR3                           0x05018
#define CCSR_BR4                           0x05020
#define CCSR_BR5                           0x05028
#define CCSR_BR6                           0x05030
#define CCSR_BR7                           0x05038
#define CCSR_OR0                           0x05004
#define CCSR_OR1                           0x0500C
#define CCSR_OR2                           0x05014
#define CCSR_OR3                           0x0501C
#define CCSR_OR4                           0x05024
#define CCSR_OR5                           0x0502C
#define CCSR_OR6                           0x05034
#define CCSR_OR7                           0x0503C
#define CCSR_MAR                           0x05068
#define CCSR_MAMR                          0x05070
#define CCSR_MBMR                          0x05074
#define CCSR_MCMR                          0x05078
#define CCSR_MRTPR                         0x05084
#define CCSR_MDR                           0x05088
#define CCSR_LSDMR                         0x05094
#define CCSR_LURT                          0x050A0
#define CCSR_LSRT                          0x050A4
#define CCSR_LTESR                         0x050B0
#define CCSR_LTEDR                         0x050B4
#define CCSR_LTEIR                         0x050B8
#define CCSR_LTEATR                        0x050BC
#define CCSR_LTEAR                         0x050C0
#define CCSR_LBCR                          0x050D0
#define CCSR_LCRR                          0x050D4

/* PCI/X Configuration Access Registers */

#define CCSR_CFG_ADDR                      0x08000
#define CCSR_CFG_DATA                      0x08004
#define CCSR_INT_ACK                       0x08008


/* PCI/X ATMU Registers - Outbound and Inbound */

#define CCSR_POTAR0                        0x08C00
#define CCSR_POTEAR0                       0x08C04

/* Note POWBAR0 is read-only (not programmable) */

#define CCSR_POWBAR0                       0x08C08

#define CCSR_POWAR0                        0x08C10

/* Outbound window 1 */

#define CCSR_POTAR1                        0x08C20
#define CCSR_POTEAR1                       0x08C24
#define CCSR_POWBAR1                       0x08C28

#define CCSR_POWAR1                        0x08C30

/* Outbound window 2 */

#define CCSR_POTAR2                        0x08C40
#define CCSR_POTEAR2                       0x08C44
#define CCSR_POWBAR2                       0x08C48

#define CCSR_POWAR2                        0x08C50

/* Outbound window 3 */

#define CCSR_POTAR3                        0x08C60
#define CCSR_POTEAR3                       0x08C64
#define CCSR_POWBAR3                       0x08C68

#define CCSR_POWAR3                        0x08C70

/* Outbound window 4 */

#define CCSR_POTAR4                        0x08C80
#define CCSR_POTEAR4                       0x08C84
#define CCSR_POWBAR4                       0x08C88

#define CCSR_POWAR4                        0x08C90

/* Inbound window 3 */

#define CCSR_PITAR3                        0x08DA0

#define CCSR_PIWBAR3                       0x08DA8
#define CCSR_PIWBEAR3                      0x08DAC
#define CCSR_PIWAR3                        0x08DB0

/* Inbound window 2 */

#define CCSR_PITAR2                        0x08DC0

#define CCSR_PIWBAR2                       0x08DC8
#define CCSR_PIWBEAR2                      0x08DCC
#define CCSR_PIWAR2                        0x08DD0

/* Inbound window 1 */

#define CCSR_PITAR1                        0x08DE0

#define CCSR_PIWBAR1                       0x08DE8

#define CCSR_PIWAR1                        0x08DF0

/* PCI/X Error Management Registers */

#define CCSR_ERR_DR                        0x08E00
#define CCSR_ERR_CAP_DR                    0x08E04
#define CCSR_ERR_EN                        0x08E08
#define CCSR_ERR_ATTRIB                    0x08E0C
#define CCSR_ERR_ADDR                      0x08E10
#define CCSR_ERR_EXT_ADDR                  0x08E14
#define CCSR_ERR_DL                        0x08E18
#define CCSR_ERR_DH                        0x08E1C
#define CCSR_GAS_TIMR                      0x08E20
#define CCSR_PCIX_TIMR                     0x08E24

/* L2/SRAM Memory - Mapped Configuration Registers */

#define CCSR_L2CTL                         0x20000 
#define CCSR_L2CEWAR0                      0x20010
#define CCSR_L2CEWCR0                      0x20018
#define CCSR_L2CEWAR1                      0x20020
#define CCSR_L2CEWCR1                      0x20028
#define CCSR_L2CEWAR2                      0x20030
#define CCSR_L2CEWCR2                      0x20038
#define CCSR_L2CEWAR3                      0x20040
#define CCSR_L2CEWCR3                      0x20048
#define CCSR_L2SRBAR0                      0x20100
#define CCSR_L2SRBAR1                      0x20108
#define CCSR_L2ERRINJHI                    0x20E00
#define CCSR_L2ERRINJLO                    0x20E04
#define CCSR_L2ERRINJCTL                   0x20E08
#define CCSR_L2CAPTDATAHI                  0x20E20
#define CCSR_L2CAPTDATALO                  0x20E24
#define CCSR_L2CAPTECC                     0x20E28
#define CCSR_L2ERRDET                      0x20E40
#define CCSR_L2ERRDIS                      0x20E44
#define CCSR_L2ERRINTEN                    0x20E48
#define CCSR_L2ERRATTR                     0x20E4C
#define CCSR_L2ERRADDR                     0x20E50
#define CCSR_L2ERRCTL                      0x20E58

/* DMA Registers */

#define CCSR_MR0                           0x21100
#define CCSR_SR0                           0x21104

#define CCSR_CLNDAR0                       0x2110C
#define CCSR_SATR0                         0x21110
#define CCSR_SAR0                          0x21114
#define CCSR_DATR0                         0x21118
#define CCSR_DAR0                          0x2111C
#define CCSR_BCR0                          0x21120

#define CCSR_NLNDAR0                       0x21128

#define CCSR_CLSDAR0                       0x21134

#define CCSR_NLSDAR0                       0x2113C
#define CCSR_SSR0                          0x21140
#define CCSR_DSR0                          0x21144

#define CCSR_MR1                           0x21180
#define CCSR_SR1                           0x21184

#define CCSR_CLNDAR1                       0x2118C
#define CCSR_SATR1                         0x21190
#define CCSR_SAR1                          0x21194
#define CCSR_DATR1                         0x21198
#define CCSR_DAR1                          0x2119C
#define CCSR_BCR1                          0x211A0

#define CCSR_NLNDAR1                       0x211A8

#define CCSR_CLSDAR1                       0x211B4

#define CCSR_NLSDAR1                       0x211BC
#define CCSR_SSR1                          0x211C0
#define CCSR_DSR1                          0x211C4

#define CCSR_MR2                           0x21200
#define CCSR_SR2                           0x21204

#define CCSR_CLNDAR2                       0x2120C
#define CCSR_SATR2                         0x21210
#define CCSR_SAR2                          0x21214
#define CCSR_DATR2                         0x21218
#define CCSR_DAR2                          0x2121C
#define CCSR_BCR2                          0x21220

#define CCSR_NLNDAR2                       0x21228

#define CCSR_CLSDAR2                       0x21234

#define CCSR_NLSDAR2                       0x2123C
#define CCSR_SSR2                          0x21240
#define CCSR_DSR2                          0x21244

#define CCSR_MR3                           0x21280
#define CCSR_SR3                           0x21284

#define CCSR_CLNDAR3                       0x2128C
#define CCSR_SATR3                         0x21290
#define CCSR_SAR3                          0x21294
#define CCSR_DATR3                         0x21298
#define CCSR_DAR3                          0x2129C
#define CCSR_BCR3                          0x212A0

#define CCSR_NLNDAR3                       0x212A8

#define CCSR_CLSDAR3                       0x212B4

#define CCSR_NLSDAR3                       0x212BC
#define CCSR_SSR3                          0x212C0
#define CCSR_DSR3                          0x212C4

#define CCSR_DGSR                          0x21300

/* TSEC1 General Control and Status Registers */

#define CCSR_TSEC1_IEVENT                  0x24010
#define CCSR_TSEC1_IMASK                   0x24014
#define CCSR_TSEC1_EDIS                    0x24018

#define CCSR_TSEC1_ECNTRL                  0x24020
#define CCSR_TSEC1_MINFLR                  0x24024
#define CCSR_TSEC1_PTV                     0x24028
#define CCSR_TSEC1_DMACTRL                 0x2402C
#define CCSR_TSEC1_TBIPA                   0x24030

#define CCSR_TSEC1_FIFO_PAUSE_CTRL         0x2404C

/* TSEC1 FIFO Control and Status Registers */

#define CCSR_TSEC1_FIFO_TX_THR             0x2408C

#define CCSR_TSEC1_FIFO_TX_STARVE          0x24098
#define CCSR_TSEC1_FIFO_TX_STARVE_SHUTOFF  0x2409C

/* TSEC1 Transmit Control and Status Registers */

#define CCSR_TSEC1_TCTRL                   0x24100
#define CCSR_TSEC1_TSTAT                   0x24104

#define CCSR_TSEC1_TBDLEN                  0x2410C
#define CCSR_TSEC1_TXIC                    0x24110

#define CCSR_TSEC1_CTBPTR                  0x24124

#define CCSR_TSEC1_TBPTR                   0x24184

#define CCSR_TSEC1_TBASE                   0x24204

#define CCSR_TSEC1_OSTBD                   0x242B0 
#define CCSR_TSEC1_OSTBDP                  0x242B4

/* TSEC1 Receive Control and Status Registers */

#define CCSR_TSEC1_RCTRL                   0x24300
#define CCSR_TSEC1_RSTAT                   0x24304

#define CCSR_TSEC1_RBDLEN                  0x2430C
#define CCSR_TSEC1_RXIC                    0x24310

#define CCSR_TSEC1_CRBPTR                  0x24324

#define CCSR_TSEC1_MRBLR                   0x24340

#define CCSR_TSEC1_RBPTR                   0x24384

#define CCSR_TSEC1_RBASE                   0x24404

#define CCSR_TSEC1_MACCFG1                 0x24500
#define CCSR_TSEC1_MACCFG2                 0x24504
#define CCSR_TSEC1_IPGIFG                  0x24508
#define CCSR_TSEC1_HAFDUP                  0x2450C
#define CCSR_TSEC1_MAXFRM                  0x24510

#define CCSR_TSEC1_MIIMCFG                 0x24520
#define CCSR_TSEC1_MIIMCOM                 0x24524
#define CCSR_TSEC1_MIIMADD                 0x24528
#define CCSR_TSEC1_MIIMCON                 0x2452C
#define CCSR_TSEC1_MIIMSTAT                0x24530
#define CCSR_TSEC1_MIIMIND                 0x24534

#define CCSR_TSEC1_IFSTAT                  0x2453C
#define CCSR_TSEC1_MACSTNADDR1             0x24540
#define CCSR_TSEC1_MACSTNADDR2             0x24544

/* TSEC1 Transmit and Receive Counters */

#define CCSR_TSEC1_TR64                    0x24680
#define CCSR_TSEC1_TR127                   0x24684
#define CCSR_TSEC1_TR255                   0x24688
#define CCSR_TSEC1_TR511                   0x2468C
#define CCSR_TSEC1_TR1K                    0x24690
#define CCSR_TSEC1_TRMAX                   0x24694
#define CCSR_TSEC1_TRMGV                   0x24698
#define CCSR_TSEC1_RBYT                    0x2469C
#define CCSR_TSEC1_RPKT                    0x246A0
#define CCSR_TSEC1_RFCS                    0x246A4
#define CCSR_TSEC1_RMCA                    0x246A8
#define CCSR_TSEC1_RBCA                    0x246AC
#define CCSR_TSEC1_RXCF                    0x246B0
#define CCSR_TSEC1_RXPF                    0x246B4
#define CCSR_TSEC1_RXUO                    0x246B8
#define CCSR_TSEC1_RALN                    0x246BC
#define CCSR_TSEC1_RFLR                    0x246C0
#define CCSR_TSEC1_RCDE                    0x246C4
#define CCSR_TSEC1_RCSE                    0x246C8
#define CCSR_TSEC1_RUND                    0x246CC
#define CCSR_TSEC1_ROVR                    0x246D0
#define CCSR_TSEC1_RFRG                    0x246D4
#define CCSR_TSEC1_RJBR                    0x246D8
#define CCSR_TSEC1_RDRP                    0x246DC

#define CCSR_TSEC1_TBYT                    0x246E0
#define CCSR_TSEC1_TPKT                    0x246E4
#define CCSR_TSEC1_TMCA                    0x246E8
#define CCSR_TSEC1_TBCA                    0x246EC
#define CCSR_TSEC1_TXPF                    0x246F0
#define CCSR_TSEC1_TDFR                    0x246F4
#define CCSR_TSEC1_TEDF                    0x246F8
#define CCSR_TSEC1_TSCL                    0x246FC
#define CCSR_TSEC1_TMCL                    0x24700
#define CCSR_TSEC1_TLCL                    0x24704
#define CCSR_TSEC1_TXCL                    0x24708
#define CCSR_TSEC1_TNCL                    0x2470C

#define CCSR_TSEC1_TDRP                    0x24714
#define CCSR_TSEC1_TJBR                    0x24718
#define CCSR_TSEC1_TFCS                    0x2471C
#define CCSR_TSEC1_TXCF                    0x24720
#define CCSR_TSEC1_TOVR                    0x24724
#define CCSR_TSEC1_TUND                    0x24728
#define CCSR_TSEC1_TFRG                    0x2472C

/* TSEC1 General Registers */ 

#define CCSR_TSEC1_CAR1                    0x24730
#define CCSR_TSEC1_CAR2                    0x24734
#define CCSR_TSEC1_CAM1                    0x24738
#define CCSR_TSEC1_CAM2                    0x2473C

/* TSEC1 Hash Function Registers */

#define CCSR_TSEC1_IADDR0                  0x24800
#define CCSR_TSEC1_IADDR1                  0x24804
#define CCSR_TSEC1_IADDR2                  0x24808
#define CCSR_TSEC1_IADDR3                  0x2480C
#define CCSR_TSEC1_IADDR4                  0x24810
#define CCSR_TSEC1_IADDR5                  0x24814
#define CCSR_TSEC1_IADDR6                  0x24818
#define CCSR_TSEC1_IADDR7                  0x2481C

#define CCSR_TSEC1_GADDR0                  0x24880
#define CCSR_TSEC1_GADDR1                  0x24884
#define CCSR_TSEC1_GADDR2                  0x24888
#define CCSR_TSEC1_GADDR3                  0x2488C
#define CCSR_TSEC1_GADDR4                  0x24890
#define CCSR_TSEC1_GADDR5                  0x24894
#define CCSR_TSEC1_GADDR6                  0x24898
#define CCSR_TSEC1_GADDR7                  0x2489C

/* TSEC1 Attribute Registers */

#define CCSR_TSEC1_ATTR                    0x24BF8
#define CCSR_TSEC1_ATTRELI                 0x24BFC

/* TSEC1 Future Expansion Space */

/* TSEC2 General Control and Status Registers */

#define CCSR_TSEC2_IEVENT                  0x25010
#define CCSR_TSEC2_IMASK                   0x25014
#define CCSR_TSEC2_EDIS                    0x25018

#define CCSR_TSEC2_ECNTRL                  0x25020
#define CCSR_TSEC2_MINFLR                  0x25024
#define CCSR_TSEC2_PTV                     0x25028
#define CCSR_TSEC2_DMACTRL                 0x2502C
#define CCSR_TSEC2_TBIPA                   0x25030

#define CCSR_TSEC2_FIFO_PAUSE_CTRL         0x2504C

/* TSEC2 FIFO Control and Status Registers */

#define CCSR_TSEC2_FIFO_TX_THR             0x2508C

#define CCSR_TSEC2_FIFO_TX_STARVE          0x25098
#define CCSR_TSEC2_FIFO_TX_STARVE_SHUTOFF  0x2509C

/* TSEC2 Transmit Control and Status Registers */

#define CCSR_TSEC2_TCTRL                   0x25100
#define CCSR_TSEC2_TSTAT                   0x25104

#define CCSR_TSEC2_TBDLEN                  0x2510C
#define CCSR_TSEC2_TXIC                    0x25110

#define CCSR_TSEC2_CTBPTR                  0x25124

#define CCSR_TSEC2_TBPTR                   0x25184

#define CCSR_TSEC2_TBASE                   0x25204

#define CCSR_TSEC2_OSTBD                   0x252B0 
#define CCSR_TSEC2_OSTBDP                  0x252B4

/* TSEC2 Receive Control and Status Registers */

#define CCSR_TSEC2_RCTRL                   0x25300
#define CCSR_TSEC2_RSTAT                   0x25304

#define CCSR_TSEC2_RBDLEN                  0x2530C
#define CCSR_TSEC2_RXIC                    0x25310

#define CCSR_TSEC2_CRBPTR                  0x25324

#define CCSR_TSEC2_MRBLR                   0x25340

#define CCSR_TSEC2_RBPTR                   0x25384

#define CCSR_TSEC2_RBASE                   0x25404

#define CCSR_TSEC2_MACCFG1                 0x25500
#define CCSR_TSEC2_MACCFG2                 0x25504
#define CCSR_TSEC2_IPGIFG                  0x25508
#define CCSR_TSEC2_HAFDUP                  0x2550C
#define CCSR_TSEC2_MAXFRM                  0x25510

#define CCSR_TSEC2_MIIMCFG                 0x25520
#define CCSR_TSEC2_MIIMCOM                 0x25524
#define CCSR_TSEC2_MIIMADD                 0x25528
#define CCSR_TSEC2_MIIMCON                 0x2552C
#define CCSR_TSEC2_MIIMSTAT                0x25530
#define CCSR_TSEC2_MIIMIND                 0x25534

#define CCSR_TSEC2_IFSTAT                  0x2553C
#define CCSR_TSEC2_MACSTNADDR1             0x25540
#define CCSR_TSEC2_MACSTNADDR2             0x25544

/* TSEC2 Transmit and Receive Counters */

#define CCSR_TSEC2_TR64                    0x25680
#define CCSR_TSEC2_TR127                   0x25684
#define CCSR_TSEC2_TR255                   0x25688
#define CCSR_TSEC2_TR511                   0x2568C
#define CCSR_TSEC2_TR1K                    0x25690
#define CCSR_TSEC2_TRMAX                   0x25694
#define CCSR_TSEC2_TRMGV                   0x25698
#define CCSR_TSEC2_RBYT                    0x2569C
#define CCSR_TSEC2_RPKT                    0x256A0
#define CCSR_TSEC2_RFCS                    0x256A4
#define CCSR_TSEC2_RMCA                    0x256A8
#define CCSR_TSEC2_RBCA                    0x256AC
#define CCSR_TSEC2_RXCF                    0x256B0
#define CCSR_TSEC2_RXPF                    0x256B4
#define CCSR_TSEC2_RXUO                    0x256B8
#define CCSR_TSEC2_RALN                    0x256BC
#define CCSR_TSEC2_RFLR                    0x256C0
#define CCSR_TSEC2_RCDE                    0x256C4
#define CCSR_TSEC2_RCSE                    0x256C8
#define CCSR_TSEC2_RUND                    0x256CC
#define CCSR_TSEC2_ROVR                    0x256D0
#define CCSR_TSEC2_RFRG                    0x256D4
#define CCSR_TSEC2_RJBR                    0x256D8
#define CCSR_TSEC2_RDRP                    0x256DC

/* TSEC2 Transmit Counters */

#define CCSR_TSEC2_TBYT                    0x256E0
#define CCSR_TSEC2_TPKT                    0x256E4
#define CCSR_TSEC2_TMCA                    0x256E8
#define CCSR_TSEC2_TBCA                    0x256EC
#define CCSR_TSEC2_TXPF                    0x256F0
#define CCSR_TSEC2_TDFR                    0x256F4
#define CCSR_TSEC2_TEDF                    0x256F8
#define CCSR_TSEC2_TSCL                    0x256FC
#define CCSR_TSEC2_TMCL                    0x25700
#define CCSR_TSEC2_TLCL                    0x25704
#define CCSR_TSEC2_TXCL                    0x25708
#define CCSR_TSEC2_TNCL                    0x2570C

#define CCSR_TSEC2_TDRP                    0x25714
#define CCSR_TSEC2_TJBR                    0x25718
#define CCSR_TSEC2_TFCS                    0x2571C
#define CCSR_TSEC2_TXCF                    0x25720
#define CCSR_TSEC2_TOVR                    0x25724
#define CCSR_TSEC2_TUND                    0x25728
#define CCSR_TSEC2_TFRG                    0x2572C

/* TSEC2 General Registers */ 

#define CCSR_TSEC2_CAR1                    0x25730
#define CCSR_TSEC2_CAR2                    0x25734
#define CCSR_TSEC2_CAM1                    0x25738
#define CCSR_TSEC2_CAM2                    0x2573C

/* TSEC2 Hash Function Registers */

#define CCSR_TSEC2_IADDR0                  0x25800
#define CCSR_TSEC2_IADDR1                  0x25804
#define CCSR_TSEC2_IADDR2                  0x25808
#define CCSR_TSEC2_IADDR3                  0x2580C
#define CCSR_TSEC2_IADDR4                  0x25810
#define CCSR_TSEC2_IADDR5                  0x25814
#define CCSR_TSEC2_IADDR6                  0x25818
#define CCSR_TSEC2_IADDR7                  0x2581C

#define CCSR_TSEC2_GADDR0                  0x25880
#define CCSR_TSEC2_GADDR1                  0x25884
#define CCSR_TSEC2_GADDR2                  0x25888
#define CCSR_TSEC2_GADDR3                  0x2588C
#define CCSR_TSEC2_GADDR4                  0x25890
#define CCSR_TSEC2_GADDR5                  0x25894
#define CCSR_TSEC2_GADDR6                  0x25898
#define CCSR_TSEC2_GADDR7                  0x2589C

/* TSEC2 Attribute Registers */

#define CCSR_TSEC2_ATTR                    0x25BF8
#define CCSR_TSEC2_ATTRELI                 0x25BFC

/* TSEC2 Future Expansion Space */

/* FEC General Control and Status Registers */

#define CCSR_FEC_IEVENT                    0x26010
#define CCSR_FEC_IMASK                     0x26014
#define CCSR_FEC_EDIS                      0x26018

#define CCSR_FEC_MINFLR                    0x26024
#define CCSR_FEC_PTV                       0x26028
#define CCSR_FEC_DMACTRL                   0x2602C

#define CCSR_FIFO_PAUSE_CTRL               0x2604C

/* FEC FIFO Control and Status Registers */

#define CCSR_FIFO_TX_THR                   0x2608C

#define CCSR_FIFO_TX_STARVE                0x26098
#define CCSR_FIFO_TX_STARVE_SHUTOFF        0x2609C

/* FEC Transmit Control and Status Registers */

#define CCSR_TCTRL                         0x26100
#define CCSR_TSTAT                         0x26104

#define CCSR_TBDLEN                        0x2610C

#define CCSR_CTBPTR                        0x26124

#define CCSR_TBPTR                         0x26184

#define CCSR_TBASE                         0x26204

#define CCSR_OSTBD                         0x262B0
#define CCSR_OSTBDP                        0x262B4

/* FEC Receive Control and Status Registers */

#define CCSR_RCTRL                         0x26300
#define CCSR_RSTAT                         0x26304

#define CCSR_RBDLEN                        0x2630C

#define CCSR_CRBPTR                        0x26324

#define CCSR_MRBLR                         0x26340

#define CCSR_RBPTR                         0x26384 

#define CCSR_RBASE                         0x26404 

/* FEC MAC Registers */

#define CCSR_MACCFG1                       0x26500 
#define CCSR_MACCFG2                       0x26504
#define CCSR_IPGIFG                        0x26508
#define CCSR_HAFDUP                        0x2650C
#define CCSR_MAXFRM                        0x26510

#define CCSR_IFSTAT                        0x2653C
#define CCSR_MACSTNADDR1                   0x26540
#define CCSR_MACSTNADDR2                   0x26544

/* FEC Hash Function Registers */

#define CCSR_IADDR0                        0x26800
#define CCSR_IADDR1                        0x26804
#define CCSR_IADDR2                        0x26808
#define CCSR_IADDR3                        0x2680C
#define CCSR_IADDR4                        0x26810
#define CCSR_IADDR5                        0x26814
#define CCSR_IADDR6                        0x26818
#define CCSR_IADDR7                        0x2681C

#define CCSR_GADDR0                        0x26880
#define CCSR_GADDR1                        0x26884
#define CCSR_GADDR2                        0x26888
#define CCSR_GADDR3                        0x2688C
#define CCSR_GADDR4                        0x26890
#define CCSR_GADDR5                        0x26894
#define CCSR_GADDR6                        0x26898
#define CCSR_GADDR7                        0x2689C

/* FEC Attribute Registers */

#define CCSR_ATTR                          0x26BF8
#define CCSR_ATTRELI                       0x26BFC

/* Future Expansion Space */

/* PIC Register Address Map - Global Registers */

#define CCSR_GLBL_IPIDR0                   0x40040
#define CCSR_GLBL_IPIDR1                   0x40050
#define CCSR_GLBL_IPIDR2                   0x40060
#define CCSR_GLBL_IPIDR3                   0x40070
#define CCSR_GLBL_CTPR                     0x40080
#define CCSR_GLBL_WHOAMI                   0x40090
#define CCSR_GLBL_IACK                     0x400A0
#define CCSR_GLBL_EOI                      0x400B0

#define CCSR_GLBL_FRR                      0x41000

#define CCSR_GLBL_GCR                      0x41020

#define CCSR_GLBL_VIR                      0x41080 
#define CCSR_GLBL_PIR                      0x41090
#define CCSR_GLBL_IPIVPR0                  0x410A0
#define CCSR_GLBL_IPIVPR1                  0x410B0
#define CCSR_GLBL_IPIVPR2                  0x410C0
#define CCSR_GLBL_IPIVPR3                  0x410D0
#define CCSR_GLBL_SVR                      0x410E0
#define CCSR_GLBL_TFRR                     0x410F0
#define CCSR_GLBL_GTCCR0                   0x41100
#define CCSR_GLBL_GTBCR0                   0x41110
#define CCSR_GLBL_GTVPR0                   0x41120
#define CCSR_GLBL_GTDR0                    0x41130
#define CCSR_GLBL_GTCCR1                   0x41140
#define CCSR_GLBL_GTBCR1                   0x41150
#define CCSR_GLBL_GTVPR1                   0x41160
#define CCSR_GLBL_GTDR1                    0x41170
#define CCSR_GLBL_GTCCR2                   0x41180
#define CCSR_GLBL_GTBCR2                   0x41190
#define CCSR_GLBL_GTVPR2                   0x411A0
#define CCSR_GLBL_GTDR2                    0x411B0
#define CCSR_GLBL_GTCCR3                   0x411C0
#define CCSR_GLBL_GTBCR3                   0x411D0
#define CCSR_GLBL_GTVPR3                   0x411E0
#define CCSR_GLBL_GTDR3                    0x411F0

#define CCSR_GLBL_TCR                      0x41300
#define CCSR_GLBL_IRQSR0                   0x41310
#define CCSR_GLBL_IRQSR1                   0x41320
#define CCSR_GLBL_CISR0                    0x41330
#define CCSR_GLBL_CISR1                    0x41340
#define CCSR_GLBL_PM0MR0                   0x41350
#define CCSR_GLBL_PM0MR1                   0x41360
#define CCSR_GLBL_PM1MR0                   0x41370
#define CCSR_GLBL_PM1MR1                   0x41380
#define CCSR_GLBL_PM2MR0                   0x41390
#define CCSR_GLBL_PM2MR1                   0x413A0
#define CCSR_GLBL_PM3MR0                   0x413B0
#define CCSR_GLBL_PM3MR1                   0x413C0

#define CCSR_GLBL_MSGR0                    0x41400
#define CCSR_GLBL_MSGR1                    0x41410
#define CCSR_GLBL_MSGR2                    0x41420
#define CCSR_GLBL_MSGR3                    0x41430

#define CCSR_GLBL_MER                      0x41500
#define CCSR_GLBL_MSR                      0x41510

/* PIC Register Address Map - Internal Source Configuration Registers */

#define CCSR_EIVPR0                        0x50000
#define CCSR_EIDR0                         0x50010
#define CCSR_EIVPR1                        0x50020
#define CCSR_EIDR1                         0x50030
#define CCSR_EIVPR2                        0x50040
#define CCSR_EIDR2                         0x50050
#define CCSR_EIVPR3                        0x50060
#define CCSR_EIDR3                         0x50070
#define CCSR_EIVPR4                        0x50080
#define CCSR_EIDR4                         0x50090
#define CCSR_EIVPR5                        0x500A0
#define CCSR_EIDR5                         0x500B0
#define CCSR_EIVPR6                        0x500C0
#define CCSR_EIDR6                         0x500D0
#define CCSR_EIVPR7                        0x500E0
#define CCSR_EIDR7                         0x500F0
#define CCSR_EIVPR8                        0x50100
#define CCSR_EIDR8                         0x50110
#define CCSR_EIVPR9                        0x50120
#define CCSR_EIDR9                         0x50130
#define CCSR_EIVPR10                       0x50140
#define CCSR_EIDR10                        0x50150
#define CCSR_EIVPR11                       0x50160
#define CCSR_EIDR11                        0x50170

#define CCSR_IIVPR0                        0x50200
#define CCSR_IIDR0                         0x50210
#define CCSR_IIVPR1                        0x50220
#define CCSR_IIDR1                         0x50230
#define CCSR_IIVPR2                        0x50240
#define CCSR_IIDR2                         0x50250
#define CCSR_IIVPR3                        0x50260
#define CCSR_IIDR3                         0x50270
#define CCSR_IIVPR4                        0x50280
#define CCSR_IIDR4                         0x50290
#define CCSR_IIVPR5                        0x502A0
#define CCSR_IIDR5                         0x502B0
#define CCSR_IIVPR6                        0x502C0
#define CCSR_IIDR6                         0x502D0
#define CCSR_IIVPR7                        0x502E0
#define CCSR_IIDR7                         0x502F0
#define CCSR_IIVPR8                        0x50300
#define CCSR_IIDR8                         0x50310
#define CCSR_IIVPR9                        0x50320
#define CCSR_IIDR9                         0x50330
#define CCSR_IIVPR10                       0x50340
#define CCSR_IIDR10                        0x50350
#define CCSR_IIVPR11                       0x50360
#define CCSR_IIDR11                        0x50370
#define CCSR_IIVPR12                       0x50380
#define CCSR_IIDR12                        0x50390
#define CCSR_IIVPR13                       0x503A0
#define CCSR_IIDR13                        0x503B0
#define CCSR_IIVPR14                       0x503C0
#define CCSR_IIDR14                        0x503D0
#define CCSR_IIVPR15                       0x503E0
#define CCSR_IIDR15                        0x503F0
#define CCSR_IIVPR16                       0x50400
#define CCSR_IIDR16                        0x50410
#define CCSR_IIVPR17                       0x50420
#define CCSR_IIDR17                        0x50430
#define CCSR_IIVPR18                       0x50440
#define CCSR_IIDR18                        0x50450
#define CCSR_IIVPR19                       0x50460
#define CCSR_IIDR19                        0x50470
#define CCSR_IIVPR20                       0x50480
#define CCSR_IIDR20                        0x50490
#define CCSR_IIVPR21                       0x504A0
#define CCSR_IIDR21                        0x504B0
#define CCSR_IIVPR22                       0x504C0
#define CCSR_IIDR22                        0x504D0
#define CCSR_IIVPR23                       0x504E0
#define CCSR_IIDR23                        0x504F0
#define CCSR_IIVPR24                       0x50500
#define CCSR_IIDR24                        0x50510
#define CCSR_IIVPR25                       0x50520
#define CCSR_IIDR25                        0x50530
#define CCSR_IIVPR26                       0x50540
#define CCSR_IIDR26                        0x50550
#define CCSR_IIVPR27                       0x50560
#define CCSR_IIDR27                        0x50570
#define CCSR_IIVPR28                       0x50580
#define CCSR_IIDR28                        0x50590
#define CCSR_IIVPR29                       0x505A0
#define CCSR_IIDR29                        0x505B0
#define CCSR_IIVPR30                       0x505C0
#define CCSR_IIDR30                        0x505D0
#define CCSR_IIVPR31                       0x505E0
#define CCSR_IIDR31                        0x505F0

#define CCSR_MIVPR0                        0x51600
#define CCSR_MIDR0                         0x51610
#define CCSR_MIVPR1                        0x51620
#define CCSR_MIDR1                         0x51630
#define CCSR_MIVPR2                        0x51640
#define CCSR_MIDR2                         0x51650
#define CCSR_MIVPR3                        0x51660
#define CCSR_MIDR3                         0x51670

/* PIC Register Address Map - Per-CPU Registers */

#define CCSR_CPU_IPIDR0                    0x60040
#define CCSR_CPU_IPIDR1                    0x60050
#define CCSR_CPU_IPIDR2                    0x60060
#define CCSR_CPU_IPIDR3                    0x60070
#define CCSR_CPU_CTPR0                     0x60080
#define CCSR_CPU_WHOAMI0                   0x60090
#define CCSR_CPU_IACK0                     0x600A0
#define CCSR_CPU_EOI0                      0x600B0

/* RapidIO Architectural Registers */

#define CCSR_RIO_DIDCAR                    0xC0000
#define CCSR_RIO_DICAR                     0xC0004
#define CCSR_RIO_AIDCAR                    0xC0008
#define CCSR_RIO_AICAR                     0xC000C
#define CCSR_RIO_PEFCAR                    0xC0010
#define CCSR_RIO_SPICAR                    0xC0014
#define CCSR_RIO_SOCAR                     0xC0018
#define CCSR_RIO_DOCAR                     0xC001C
#define CCSR_RIO_MSR                       0xC0040
#define CCSR_RIO_PWDCSR                    0xC0044
#define CCSR_RIO_PELLCCSR                  0xC004C

#define CCSR_RIO_LCSBA1CSR                 0xC005C
#define CCSR_RIO_BDIDCSR                   0xC0060
#define CCSR_RIO_HBDIDLCSR                 0xC0068
#define CCSR_RIO_CTCSR                     0xC006C
#define CCSR_RIO_PMBH0CSR                  0xC0100
#define CCSR_RIO_PLTOCCSR                  0xC0120
#define CCSR_RIO_PRTOCCSR                  0xC0124
#define CCSR_RIO_PGCCSR                    0xC013C
#define CCSR_RIO_PLMREQCSR                 0xC0140
#define CCSR_RIO_PLMRESPCSR                0xC0144
#define CCSR_RIO_PLASCSR                   0xC0148
#define CCSR_RIO_PESCSR                    0xC0158
#define CCSR_RIO_PCCSR                     0xC015C

/* RapidIO Implementation Registers */

#define CCSR_CR                            0xD0000
#define CCSR_PCR                           0xD0010
#define CCSR_PEIR                          0xD0014

/* RapidIO ATMU Registers */

#define CCSR_ROWTAR0                       0xD0C00
#define CCSR_ROWAR0                        0xD0C10
#define CCSR_ROWTAR1                       0xD0C20
#define CCSR_ROWBAR1                       0xD0C28
#define CCSR_ROWAR1                        0xD0C30
#define CCSR_ROWTAR2                       0xD0C40
#define CCSR_ROWBAR2                       0xD0C48
#define CCSR_ROWAR2                        0xD0C50
#define CCSR_ROWTAR3                       0xD0C60
#define CCSR_ROWBAR3                       0xD0C68
#define CCSR_ROWAR3                        0xD0C70
#define CCSR_ROWTAR4                       0xD0C80
#define CCSR_ROWBAR4                       0xD0C88
#define CCSR_ROWAR4                        0xD0C90
#define CCSR_ROWTAR5                       0xD0CA0
#define CCSR_ROWBAR5                       0xD0CA8
#define CCSR_ROWAR5                        0xD0CB0
#define CCSR_ROWTAR6                       0xD0CC0
#define CCSR_ROWBAR6                       0xD0CC8
#define CCSR_ROWAR6                        0xD0CD0
#define CCSR_ROWTAR7                       0xD0CE0
#define CCSR_ROWBAR7                       0xD0CE8
#define CCSR_ROWAR7                        0xD0CF0
#define CCSR_ROWTAR8                       0xD0D00
#define CCSR_ROWBAR8                       0xD0D08
#define CCSR_ROWAR8                        0xD0D10
#define CCSR_RIWTAR4                       0xD0D60
#define CCSR_RIWBAR4                       0xD0D68
#define CCSR_RIWAR4                        0xD0D70
#define CCSR_RIWTAR3                       0xD0D80
#define CCSR_RIWBAR3                       0xD0D88
#define CCSR_RIWAR3                        0xD0D90
#define CCSR_RIWTAR2                       0xD0DA0
#define CCSR_RIWBAR2                       0xD0DA8
#define CCSR_RIWAR2                        0xD0DB0
#define CCSR_RIWTAR1                       0xD0DC0
#define CCSR_RIWBAR1                       0xD0DC8
#define CCSR_RIWAR1                        0xD0DD0
#define CCSR_RIWTAR0                       0xD0DE0
#define CCSR_RIWAR0                        0xD0DF0

/* RapidIO Error Management Registers */

#define CCSR_PNFEDR                        0xD0E00
#define CCSR_PNFEDIR                       0xD0E04
#define CCSR_PNFEIER                       0xD0E08
#define CCSR_PECSR                         0xD0E0C
#define CCSR_EPCR0                         0xD0E10
#define CCSR_EPCR1                         0xD0E14
#define CCSR_EPCR2                         0xD0E18
#define CCSR_PREDR                         0xD0E20
#define CCSR_PERTR                         0xD0E28
#define CCSR_PRTR                          0xD0E2C

/* RapidIO Outbound Message Registers */

#define CCSR_OMR                           0xD1000
#define CCSR_OSR                           0xD1004
#define CCSR_ODQDPAR                       0xD100C
#define CCSR_OSAR                          0xD1014
#define CCSR_ODPR                          0xD1018
#define CCSR_ODATR                         0xD101C
#define CCSR_ODCR                          0xD1020
#define CCSR_ODQEPAR                       0xD1028

/* RapidIO Inbound Message Registers */

#define CCSR_IMR                           0xD1060
#define CCSR_ISR                           0xD1064
#define CCSR_IFQDPAR                       0xD106C
#define CCSR_IFQEPAR                       0xD1074

/* RapidIO Doorbell Registers */

#define CCSR_DMR                           0xD1460
#define CCSR_DSR                           0xD1464
#define CCSR_DQDPAR                        0xD146C
#define CCSR_DQEPAR                        0xD1474

/* RapidIO Port-Write Registers */

#define CCSR_PWMR                          0xD14E0
#define CCSR_PWSR                          0xD14E4
#define CCSR_PWQBAR                        0xD14EC

/* Global Utilities - Power-on Reset Configuration Values */

#define CCSR_PORPLLSR                      0xE0000
#define CCSR_PORBMSR                       0xE0004
#define CCSR_PORIMPSCR                     0xE0008
#define CCSR_PORDEVSR                      0xE000C
#define CCSR_PORDBGMSR                     0xE0010
#define CCSR_GPPORCR                       0xE0020

/* Signal Multiplexing and GPIO Controls */

#define CCSR_GPIOCR                        0xE0030
#define CCSR_GPOUTDR                       0xE0040
#define CCSR_GPINDR                        0xE0050
#define CCSR_PMUXCR                        0xE0060

/* Device Disables */

#define CCSR_DEVDISR                       0xE0070

/* Power Management Registers */

#define CCSR_POWMGTCSR                     0xE0080

/* Interrupt Reporting */

#define CCSR_MCPSUMR                       0xE0090

/* Version Registers */

#define CCSR_PVR                           0xE00A0
#define CCSR_SVR                           0xE00A4

/* Debug Control */

#define CCSR_CLKOCR                        0xE0E00
#define CCSR_DDRDLLCR                      0xE0E10
#define CCSR_LBDLLCR                       0xE0E20

/* Performance Monitor Control Registers */

#define CCSR_PMGC0                         0xE1000
#define CCSR_PMLCA0                        0xE1010
#define CCSR_PMLCB0                        0xE1014
#define CCSR_PMC0U                         0xE1018
#define CCSR_PMC0L                         0xE101C
#define CCSR_PMLCA1                        0xE1020
#define CCSR_PMLCB1                        0xE1024
#define CCSR_PMC1                          0xE1028
#define CCSR_PMLCA2                        0xE1030
#define CCSR_PMLCB2                        0xE1034
#define CCSR_PMC2                          0xE1038
#define CCSR_PMLCA3                        0xE1040
#define CCSR_PMLCB3                        0xE1044
#define CCSR_PMC3                          0xE1048
#define CCSR_PMLCA4                        0xE1050
#define CCSR_PMLCB4                        0xE1054
#define CCSR_PMC4                          0xE1058
#define CCSR_PMLCA5                        0xE1060
#define CCSR_PMLCB5                        0xE1064
#define CCSR_PMC5                          0xE1068
#define CCSR_PMLCA6                        0xE1070
#define CCSR_PMLCB6                        0xE1074
#define CCSR_PMC6                          0xE1078
#define CCSR_PMLCA7                        0xE1080
#define CCSR_PMLCB7                        0xE1084
#define CCSR_PMC7                          0xE1088
#define CCSR_PMLCA8                        0xE1090
#define CCSR_PMLCB8                        0xE1094
#define CCSR_PMC8                          0xE1098

/* Debug and Watchpoint Monitor Registers */

#define CCSR_WMCR0                         0xE2000
#define CCSR_WMCR1                         0xE2004
#define CCSR_WMAR                          0xE200C
#define CCSR_WMAMR                         0xE2014
#define CCSR_WMTMR                         0xE2018
#define CCSR_WMSR                          0xE201C

/* Trace Buffer Registers */

#define CCSR_TBCR0                         0xE2040
#define CCSR_TBCR1                         0xE2044
#define CCSR_TBAR                          0xE204C
#define CCSR_TBAMR                         0xE2054
#define CCSR_TBTMR                         0xE2058
#define CCSR_TBSR                          0xE205C
#define CCSR_TBACR                         0xE2060
#define CCSR_TBADHR                        0xE2064
#define CCSR_TBADR                         0xE2068

/* Context ID Registers */

#define CCSR_PCIDR                         0xE20A0
#define CCSR_CCIDR                         0xE20A4

/* Other Registers */

#define CCSR_TOSR                          0xE20B0

/* CCSR access macros */

#define CCSR_ADDR(base,reg)         (base + reg)
#define CCSR_PTR(base,reg)          (VUINT32 *)(CCSR_ADDR(base,reg))
#define CCSR_PTR08(base,reg)        (VUINT8 *)(CCSR_ADDR(base,reg))
#define CCSR_READ32(base,reg)       *(CCSR_PTR(base,reg))
#define CCSR_WRITE32(base,reg,val)  {                                 \
                                    *(CCSR_PTR(base,reg)) = val;      \
                                    EIEIO_SYNC;                       \
                                    }
#define CCSR_READ08(base,reg)       *(CCSR_PTR08(base,reg))
#define CCSR_WRITE08(base,reg,val)  {                                 \
                                    *(CCSR_PTR08(base,reg)) = val;    \
                                    EIEIO_SYNC;                       \
                                    }

/* Size of an MCP8540 register */

#define MPC8540_REG_SIZE        4

/*
 * Many of the registers in the MCP8540 are initialized statically via
 * a table-driven procedure.  
 *
 * Let's define the table entry discriminators and end marker:
 */

#define MPC8540_INTERNAL        0       /* Mark register as direct access */
#define END_INIT_MPC8540_REG    0xffffffff   /* End of register list */

/* Now define the essentials for direct register write access: */

#define YES_INIT_MPC8540_REG(offset,val)  (MPC8540_INTERNAL),((UINT32)offset),((UINT32)val),
#define MPC8540_INTERNAL_INIT_SIZE        3  /* # of elements in above macro */

/*
 * Some of these MPC8540 registers are not initialized at all via the
 * static table-driven method but we still want to put a place holder
 * in the table which names the register and identifies it as one which
 * is not statically initialized.  The following macros generate no code
 * but allow us to make these place holder table entries.
 */

#define NOT_INIT_MPC8540_REG(offset,x)    /* No code for this macro */

/* LAWBAR/LAWAR related */ 

#define CCSR_LAWBAR_BASE_ADDR_BIT (0)
#define CCSR_LAWBAR_BASE_ADDR_MASK (0xffffff)
#define CCSR_LAWBAR_BASE_ADDR_ENCODE(x) (x >> 12)
#define CCSR_LAWBAR_BASE_ADDR_DECODE(x) (x << 12)

#define CCSR_LAWBARn_INVALID	(0)

#define CCSR_LAWAR_SZ_BIT	(0)
#define CCSR_LAWAR_SZ_MASK	(0x3f >> 0)

#define CCSR_LAWAR_TRGT_IF_BIT  (20)
#define CCSR_LAWAR_TRGT_IF_MASK (0xff << CCSR_LAWAR_TRGT_IF_BIT)

#define CCSR_LAWAR_EN_BIT	(31)
#define CCSR_LAWAR_EN_MASK	(1 << CCSR_LAWAR_EN_BIT)
#define CCSR_LAWAR_EN_VAL_ENABLED (1 << CCSR_LAWAR_EN_BIT)

#define CCSR_LAWAR_TGTIF_VAL_PCIX     (0 << CCSR_LAWAR_TRGT_IF_BIT)
#define CCSR_LAWAR_TGTIF_VAL_LBC      (4 << CCSR_LAWAR_TRGT_IF_BIT)
#define CCSR_LAWAR_TGTIF_VAL_RAPIDIO  (0xc << CCSR_LAWAR_TRGT_IF_BIT)
#define CCSR_LAWAR_TGTIF_VAL_DDRSDRAM (0xf << CCSR_LAWAR_TRGT_IF_BIT)

#define CCSR_LAWARn_INVALID	(0)

/* I2C control register related */ 

#define CCSR_I2CCR_MEN_BIT	(7-0)
#define CCSR_I2CCR_MEN_MASK	(1 << CCSR_I2CCR_MEN_BIT)

#define CCSR_I2CCR_MEIN_BIT	(7-1)
#define CCSR_I2CCR_MEIN_MASK	(1 << CCSR_I2CCR_MEIN_BIT)

#define CCSR_I2CCR_MSTA_BIT	(7-2)
#define CCSR_I2CCR_MSTA_MASK	(1 << CCSR_I2CCR_MSTA_BIT)

#define CCSR_I2CCR_MTX_BIT	(7-3)
#define CCSR_I2CCR_MTX_MASK	(1 << CCSR_I2CCR_MTX_BIT)

#define CCSR_I2CCR_TXAK_BIT	(7-4)
#define CCSR_I2CCR_TXAK_MASK	(1 << CCSR_I2CCR_TXAK_BIT)

#define CCSR_I2CCR_RSTA_BIT	(7-5)
#define CCSR_I2CCR_RSTA_MASK	(1 << CCSR_I2CCR_RSTA_BIT)

#define CCSR_I2CCR_BCST_BIT	(7-7)
#define CCSR_I2CCR_BCST_MASK	(1 << CCSR_I2CCR_BCST_BIT)

/* I2c status register related */

#define CCSR_I2CSR_MCF_BIT	(7-0)
#define CCSR_I2CSR_MCF_MASK	(1 << CCSR_I2CSR_MCF_BIT)

#define CCSR_I2CSR_MAAS_BIT	(7-1)
#define CCSR_I2CSR_MAAS_MASK	(1 << CCSR_I2CSR_MAAS_BIT)

#define CCSR_I2CSR_MBB_BIT	(7-2)
#define CCSR_I2CSR_MBB_MASK	(1 << CCSR_I2CSR_MBB_BIT)

#define CCSR_I2CSR_MAL_BIT	(7-3)
#define CCSR_I2CSR_MAL_MASK	(1 << CCSR_I2CSR_MAL_BIT)

#define CCSR_I2CSR_BCSTM_BIT	(7-4)
#define CCSR_I2CSR_BCSTM_MASK	(1 << CCSR_I2CSR_BCSTM_BIT)

#define CCSR_I2CSR_SRW_BIT	(7-5)
#define CCSR_I2CSR_SRW_MASK	(1 << CCSR_I2CSR_SRW_BIT)

#define CCSR_I2CSR_MIF_BIT	(7-6)
#define CCSR_I2CSR_MIF_MASK	(1 << CCSR_I2CSR_MIF_BIT)

#define CCSR_I2CSR_RXAK_BIT	(7-7)
#define CCSR_I2CSR_RXAK_MASK	(1 << CCSR_I2CSR_RXAK_BIT)

/* Base and option registers (Chip select, BRn/ORn) related */

#define CCSR_BRn_INVALID	(0)

#define CCSR_BRn_V_BIT		(0)
#define CCSR_BRn_V_MASK		(1 << CCSR_BRn_V_BIT)

#define CCSR_BRn_ATOM_BIT	(2)
#define CCSR_BRn_ATOM_MASK	(3 << CCSR_BRn_ATOM_BIT)
#define CCSR_BRn_ATOM_VAL_NONE	(0 << CCSR_BRn_ATOM_BIT)
#define CCSR_BRn_ATOM_VAL_RAWA	(1 << CCSR_BRn_ATOM_BIT)
#define CCSR_BRn_ATOM_VAL_WARA	(2 << CCSR_BRn_ATOM_BIT)

#define CCSR_BRn_MSEL_BIT	(5)
#define CCSR_BRn_MSEL_MASK	(3 << 5)

#define CCSR_BRn_MSEL_VAL_GPCM	(0 << CCSR_BRn_MSEL_BIT)
#define CCSR_BRn_MSEL_VAL_SDRAM	(3 << CCSR_BRn_MSEL_BIT)

#define CCSR_BRn_MSEL_VAL_UPMA	(4 << CCSR_BRn_MSEL_BIT)
#define CCSR_BRn_MSEL_VAL_UPMB	(5 << CCSR_BRn_MSEL_BIT)
#define CCSR_BRn_MSEL_VAL_UPMC	(6 << CCSR_BRn_MSEL_BIT)

#define CCSR_BRn_WP_BIT		(8)
#define CCSR_BRn_WP_MASK	(1 << CCSR_BRn_WP_BIT)

#define CCSR_BRn_DECC_BIT	(9)
#define CCSR_BRn_DECC_MASK	(3 << CCSR_BRn_DECC_BIT)
#define CCSR_BRn_DECC_VAL_NONE	(0 << CCSR_BRn_DECC_BIT)
#define CCSR_BRn_DECC_VAL_NORM	(1 << CCSR_BRn_DECC_BIT)
#define CCSR_BRn_DECC_VAL_RMW	(2 << CCSR_BRn_DECC_BIT)

#define CCSR_BRn_DECC_PS_BIT	(11)
#define CCSR_BRn_DECC_PS_MASK	(3 << CCSR_BRn_DECC_PS_BIT)
#define CCSR_BRn_DECC_PS_8BIT	(1 << CCSR_BRn_DECC_PS_BIT)
#define CCSR_BRn_DECC_PS_16BIT	(2 << CCSR_BRn_DECC_PS_BIT)
#define CCSR_BRn_DECC_PS_32BIT	(3 << CCSR_BRn_DECC_PS_BIT)

#define CCSR_BRn_BA_BIT		(16)
#define CCSR_BRn_BA_MASK	(0xffff << 16)

#define CCSR_ORn_INVALID	(0)

#define CCSR_ORn_EAD_BIT	(0)
#define CCSR_ORn_EAD_MASK	(1 << CCSR_ORn_EAD_BIT)

#define CCSR_ORn_EHTR_BIT	(1)
#define CCSR_ORn_EHTR_MASK	(1 << CCSR_ORn_EHTR_BIT)

#define CCSR_ORn_TRLX_BIT	(2)
#define CCSR_ORn_TRLX_MASK	(1 << CCSR_ORn_TRLX_BIT)

#define CCSR_ORn_SETA_BIT	(3)
#define CCSR_ORn_SETA_MASK	(1 << CCSR_ORn_SETA_BIT)

#define CCSR_ORn_SCY_BIT	(4)
#define CCSR_ORn_SCY_MASK	(0xf << CCSR_ORn_SCY_BIT)

#define CCSR_ORn_XACS_BIT	(8)
#define CCSR_ORn_XACS_MASK	(1 << CCSR_ORn_XACS_BIT)

#define CCSR_ORn_ACS_BIT	(9)
#define CCSR_ORn_ACS_MASK	(3 << CCSR_ORn_ACS_BIT)

#define CCSR_ORn_CSNT_BIT	(11)
#define CCSR_ORn_CSNT_MASK	(1 << CCSR_ORn_CSNT_BIT)

#define CCSR_ORn_BCTLD_BIT	(12)
#define CCSR_ORn_BCTLD_MASK	(1 << CCSR_ORn_BCTLD_BIT)

#define CCSR_ORn_AM_BIT		(16)
#ifdef _ASMLANGUAGE
#define CCSR_ORn_AM_MASK	((0xffff << CCSR_ORn_AM_BIT))
#else
#define CCSR_ORn_AM_MASK	((UINT32)(0xffff << CCSR_ORn_AM_BIT))
#endif /* _ASMLANGUAGE */

#define CCSR_ORn_AM_VAL(size)	(~(size-1) & CCSR_ORn_AM_MASK)

/* PCI/X Memory Mapped Register related */

#define CCSR_PAR_ADDR_ENCODE(x) (x >> 12)
#define CCSR_PAR_ADDR_DECODE(x) (x << 12)

#define CCSR_POWAR_OWS_BIT	(0)
#define CCSR_POWAR_OWS_MASK	(0x3f << CCSR_POWAR_OWS_BIT)
#define CCSR_POWAR_OWS_VAL(x)	((x & CCSR_POWAR_OWS_MASK) >> \
				 CCSR_POWAR_OWS_BIT)

#define CCSR_POWAR_WTT_BIT	(12)
#define CCSR_POWAR_WTT_MASK	(0xf << CCSR_POWAR_WTT_BIT)
#define CCSR_POWAR_WTT_VAL(x)	((x & CCSR_POWAR_WTT_MASK) >> \
				 CCSR_POWAR_WTT_BIT)

#define CCSR_POWAR_RTT_BIT	(16)
#define CCSR_POWAR_RTT_MASK	(0xf << CCSR_POWAR_RTT_BIT)
#define CCSR_POWAR_RTT_VAL(x)	((x & CCSR_POWAR_RTT_MASK) >> \
				 CCSR_POWAR_RTT_BIT)

#define CCSR_POWAR_EN_BIT	(31)
#define CCSR_POWAR_EN_MASK	((UINT32)(1 << CCSR_POWAR_EN_BIT))
#define CCSR_POWAR_EN_VAL(x)	((x & CCSR_POWAR_EN_MASK) >> \
				 CCSR_POWAR_EN_BIT)

#define CCSR_PIWAR_IWS_BIT	(0)
#define CCSR_PIWAR_IWS_MASK	(0x3f << CCSR_PIWAR_IWS_BIT)
#define CCSR_PIWAR_IWS_VAL(x)	((x & CCSR_PIWAR_IWS_MASK) >> \
				 CCSR_PIWAR_IWS_BIT)

#define CCSR_PIWAR_WTT_BIT	(12)
#define CCSR_PIWAR_WTT_MASK	(0xf << CCSR_PIWAR_WTT_BIT)
#define CCSR_PIWAR_WTT_VAL(x)	((x & CCSR_PIWAR_WTT_MASK) >> \
				 CCSR_PIWAR_WTT_BIT)

#define CCSR_PIWAR_RTT_BIT	(16)
#define CCSR_PIWAR_RTT_MASK	(0xf << CCSR_PIWAR_RTT_BIT)
#define CCSR_PIWAR_RTT_VAL(x)	((x & CCSR_PIWAR_RTT_MASK) >> \
				 CCSR_PIWAR_RTT_BIT)

#define CCSR_PIWAR_TGI_BIT	(20)
#define CCSR_PIWAR_TGI_MASK	(0xf << CCSR_PIWAR_TGI_BIT)
#define CCSR_PIWAR_TGI_VAL(x)	((x & CCSR_PIWAR_TGI_MASK) >> \
				 CCSR_PIWAR_TGI_BIT)

#define CCSR_PIWAR_PF_BIT	(29)
#define CCSR_PIWAR_PF_MASK	(1 << CCSR_PIWAR_PF_BIT)
#define CCSR_PIWAR_PF_VAL(x)	((x & CCSR_PIWAR_PF_MASK) >> \
				 CCSR_PIWAR_PF_BIT)

#define CCSR_PIWAR_EN_BIT	(31)
#define CCSR_PIWAR_EN_MASK	((UINT32)(1 << CCSR_PIWAR_EN_BIT))
#define CCSR_PIWAR_EN_VAL(x)	((x & CCSR_PIWAR_EN_MASK) >> \
				 CCSR_PIWAR_EN_BIT)

/*
 * MPC8540_PCI_SIZE_ENCODE takes a power of two number and computes
 * the MCP8540 size encoding value - of the type that would appear
 * in the POWAR1 register.  It's DECODE counterpart does the inverse.
 */

#define MPC8540_PCI_SIZE_ENCODE(x)	(WHAT_2POWER(x) - 1)
#define MPC8540_PCI_SIZE_DECODE(x)	(1 << (x + 1))

/* POR PLL Status Register Related */

#define CCSR_PORPLLSR_E500_RATIO_BIT  (16)
#define CCSR_PORPLLSR_E500_RATIO_MASK (0x3f << CCSR_PORPLLSR_E500_RATIO_BIT)
#define CCSR_PORPLLSR_E500_RATIO_VAL(x)	((x & CCSR_PORPLLSR_E500_RATIO_MASK) \
					  >> CCSR_PORPLLSR_E500_RATIO_BIT)

#define CCSR_PORPLLSR_PLAT_RATIO_BIT  (1)
#define CCSR_PORPLLSR_PLAT_RATIO_MASK (0x1f << CCSR_PORPLLSR_PLAT_RATIO_BIT)
#define CCSR_PORPLLSR_PLAT_RATIO_VAL(x)	((x & CCSR_PORPLLSR_PLAT_RATIO_MASK) \
					  >> CCSR_PORPLLSR_PLAT_RATIO_BIT)
/* TLB related (note many related defines are in "h/arch/ppc85xx.h" */

#define PPC_MAS0_TLBSEL_BIT	(28)
#define PPC_MAS0_TLBSEL_MASK	(1 << PPC_MAS0_TLBSEL_BIT)
#define PPC_MAS0_TLBSEL_VAL(x)	((x & PPC_MAS0_TLBSEL_MASK) >> \
				 PPC_MAS0_TLBSEL_BIT)

#define PPC_MAS0_ESEL_BIT	(16)
#define PPC_MAS0_ESEL_MASK	(0xf << PPC_MAS0_ESEL_BIT)
#define PPC_MAS0_ESEL_VAL(x)	((x & PPC_MAS0_ESEL_MASK) >> PPC_MAS0_ESEL_BIT)

#define PPC_MAS1_V_BIT		(31)
#define PPC_MAS1_V_MASK		(1 << PPC_MAS1_V_BIT)
#define PPC_MAS1_V_VAL(x)	((x & PPC_MAS1_V_MASK) >> PPC_MAS1_V_BIT)

#define PPC_MAS1_IPROT_BIT	(30)
#define PPC_MAS1_IPROT_MASK	(1 << PPC_MAS1_IPROT_BIT)
#define PPC_MAS1_IPROT_VAL(x)	((x & PPC_MAS1_IPROT_MASK) >> \
				  PPC_MAS1_IPROT_BIT)

#define PPC_MAS1_TID_BIT	(16)
#define PPC_MAS1_TID_MASK	(0xff << PPC_MAS1_TID_BIT)
#define PPC_MAS1_TID_VAL(x)	((x & PPC_MAS1_TID_MASK) >> PPC_MAS1_TID_BIT)

#define PPC_MAS1_TS_BIT		(12)
#define PPC_MAS1_TS_MASK	(1 << PPC_MAS1_TS_BIT)
#define PPC_MAS1_TS_VAL(x)	((x & PPC_MAS1_TS_MASK) >> PPC_MAS1_TS_BIT)

#define PPC_MAS1_TSIZE_BIT	(8)
#define PPC_MAS1_TSIZE_MASK	(0xf << PPC_MAS1_TSIZE_BIT)
#define PPC_MAS1_TSIZE_VAL(x)	((x & PPC_MAS1_TSIZE_MASK) >> \
				  PPC_MAS1_TSIZE_BIT)

#define PPC_MAS2_EPN_BIT	(12)
#define PPC_MAS2_EPN_MASK	(0xfffff << PPC_MAS2_EPN_BIT)
#define PPC_MAS2_EPN_VAL(x)	((x & PPC_MAS2_EPN_MASK) >> PPC_MAS2_EPN_BIT)

#define PPC_MAS2_WIMGE_BIT	(0)
#define PPC_MAS2_WIMGE_MASK	(0x1f << PPC_MAS2_WIMGE_BIT)
#define PPC_MAS2_WIMGE_VAL(x)	((x & PPC_MAS2_WIMGE_MASK) >> \
				  PPC_MAS2_WIMGE_BIT)

#define PPC_MAS2_W_BIT		(4)
#define PPC_MAS2_W_MASK		(1 << PPC_MAS2_W_BIT)
#define PPC_MAS2_W_VAL(x)	((x & PPC_MAS2_W_MASK) >> PPC_MAS2_W_BIT)

#define PPC_MAS2_I_BIT		(3)
#define PPC_MAS2_I_MASK		(1 << PPC_MAS2_I_BIT)
#define PPC_MAS2_I_VAL(x)	((x & PPC_MAS2_I_MASK) >> PPC_MAS2_I_BIT)

#define PPC_MAS2_M_BIT		(2)
#define PPC_MAS2_M_MASK		(1 << PPC_MAS2_M_BIT)
#define PPC_MAS2_M_VAL(x)	((x & PPC_MAS2_M_MASK) >> PPC_MAS2_M_BIT)

#define PPC_MAS2_G_BIT		(1)
#define PPC_MAS2_G_MASK		(1 << PPC_MAS2_G_BIT)
#define PPC_MAS2_G_VAL(x)	((x & PPC_MAS2_G_MASK) >> PPC_MAS2_G_BIT)

#define PPC_MAS2_E_BIT		(0)
#define PPC_MAS2_E_MASK		(1 << PPC_MAS2_E_BIT)
#define PPC_MAS2_E_VAL(x)	((x & PPC_MAS2_E_MASK) >> PPC_MAS2_E_BIT)

#define PPC_MAS3_RPN_BIT	(12)
#define PPC_MAS3_RPN_MASK	(0xfffff << PPC_MAS3_RPN_BIT)
#define PPC_MAS3_RPN_VAL(x)	((x & PPC_MAS3_RPN_MASK) >> PPC_MAS3_RPN_BIT)

#define PPC_MAS3_XWR_BIT	(0)
#define PPC_MAS3_XWR_MASK	(0x3f << PPC_MAS3_XWR_BIT)
#define PPC_MAS3_XWR_VAL(x)	((x & PPC_MAS3_XWR_MASK) >> PPC_MAS3_XWR_BIT)

#define PPC_MAS3_UX_BIT		(5)
#define PPC_MAS3_UX_MASK	(1 << PPC_MAS3_UX_BIT)
#define PPC_MAS3_UX_VAL(x)	((x & PPC_MAS3_UX_MASK) >> PPC_MAS3_UX_BIT)

#define PPC_MAS3_SX_BIT		(4)
#define PPC_MAS3_SX_MASK	(1 << PPC_MAS3_SX_BIT)
#define PPC_MAS3_SX_VAL(x)	((x & PPC_MAS3_SX_MASK) >> PPC_MAS3_SX_BIT)

#define PPC_MAS3_UW_BIT		(3)
#define PPC_MAS3_UW_MASK	(1 << PPC_MAS3_UW_BIT)
#define PPC_MAS3_UW_VAL(x)	((x & PPC_MAS3_UW_MASK) >> PPC_MAS3_UW_BIT)

#define PPC_MAS3_SW_BIT		(2)
#define PPC_MAS3_SW_MASK	(1 << PPC_MAS3_SW_BIT)
#define PPC_MAS3_SW_VAL(x)	((x & PPC_MAS3_SW_MASK) >> PPC_MAS3_SW_BIT)

#define PPC_MAS3_UR_BIT		(1)
#define PPC_MAS3_UR_MASK	(1 << PPC_MAS3_UR_BIT)
#define PPC_MAS3_UR_VAL(x)	((x & PPC_MAS3_UR_MASK) >> PPC_MAS3_UR_BIT)

#define PPC_MAS3_SR_BIT		(0)
#define PPC_MAS3_SR_MASK	(1 << PPC_MAS3_SR_BIT)
#define PPC_MAS3_SR_VAL(x)	((x & PPC_MAS3_SR_MASK) >> PPC_MAS3_SR_BIT)

/* PCI related */

#define PCI_PRIMARY_CAR		(CCSBAR + CCSR_CFG_ADDR)
#define PCI_PRIMARY_CDR		(CCSBAR + CCSR_CFG_DATA)

/* Special Purpose Registers */

#define TBLR_W              	284   /* Time Base Lower Register - write */
#define TBLR_R              	268   /* Time Base Lower Register - read */
#define TBUR_W              	285   /* Time Base Upper Register - write */
#define TBUR_R              	269   /* Time Base Upper Register - read */

/* DDR SDRAM Memory Controller */

#define DDR_SDRAM_PAGE_MODE             0
#define DDR_SDRAM_AUTO_PRECHARGE_MODE   1

#define MPC8540_DDR_SDRAM_TRAS_DFLT  0xF     /* tRAS default - 15 clocks */
#define MPC8540_DDR_SDRAM_TRP_DFLT   7       /* tRP default - 7 clocks */
#define MPC8540_DDR_SDRAM_TRCD_DFLT  7       /* tRCD default - 7 clocks */
#define MPC8540_DDR_SDRAM_TRRD_DFLT  4       /* tRRD default - 4 clocks */
#define MPC8540_DDR_SDRAM_TCL_DFLT   4       /* tCL default - 2.5 clocks */
#define MPC8540_DDR_SDRAM_TRFC_DFLT  0xF     /* tRFC default - 23 clocks */

#define DDR_SDRAM_CSn_CONFIG_COLUMN_BIT         (0)
#define DDR_SDRAM_CSn_CONFIG_COLUMN_MASK \
        (0x7 << DDR_SDRAM_CSn_CONFIG_COLUMN_BIT)
#define DDR_SDRAM_CSn_CONFIG_ROW_BIT            (8)
#define DDR_SDRAM_CSn_CONFIG_ROW_MASK \
        (0x7 << DDR_SDRAM_CSn_CONFIG_ROW_BIT)
#define DDR_SDRAM_CSn_CONFIG_AP_EN_BIT          (23)
#define DDR_SDRAM_CSn_CONFIG_AP_EN_MASK \
        (1 << DDR_SDRAM_CSn_CONFIG_AP_EN_BIT)
#define DDR_SDRAM_CSn_CONFIG_CS_EN_BIT          (31)
#define DDR_SDRAM_CSn_CONFIG_CS_EN_MASK \
        (1 << DDR_SDRAM_CSn_CONFIG_CS_EN_BIT)
#define DDR_SDRAM_CSn_BNDS_SA_BIT		(16)
#define DDR_SDRAM_CSn_BNDS_SA_MASK \
	(0xFF << DDR_SDRAM_CSn_BNDS_SA_BIT)
#define DDR_SDRAM_CSn_BNDS_EA_BIT		(0)              
#define DDR_SDRAM_CSn_BNDS_EA_MASK \
	(0xFF << DDR_SDRAM_CSn_BNDS_EA_BIT)
#define DDR_SDRAM_CSn_BNDS_SA_SHIFT             (8)
#define DDR_SDRAM_CSn_BNDS_EA_SHIFT             (24)
#define DDR_SDRAM_TIMING_CFG_1_WRTORD_BIT       (0)
#define DDR_SDRAM_TIMING_CFG_1_WRTORD_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_WRTORD_BIT)
#define DDR_SDRAM_TIMING_CFG_1_ACTTOACT_BIT     (4)
#define DDR_SDRAM_TIMING_CFG_1_ACTTOACT_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_ACTTOACT_BIT)
#define DDR_SDRAM_TIMING_CFG_1_WRREC_BIT        (8)
#define DDR_SDRAM_TIMING_CFG_1_WRREC_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_WRREC_BIT)
#define DDR_SDRAM_TIMING_CFG_1_REFREC_BIT       (12)
#define DDR_SDRAM_TIMING_CFG_1_REFREC_MASK \
        (0xF << DDR_SDRAM_TIMING_CFG_1_REFREC_BIT)
#define DDR_SDRAM_TIMING_CFG_1_CASLAT_BIT       (16)
#define DDR_SDRAM_TIMING_CFG_1_CASLAT_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_CASLAT_BIT)
#define DDR_SDRAM_TIMING_CFG_1_ACTTORW_BIT      (20)
#define DDR_SDRAM_TIMING_CFG_1_ACTTORW_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_ACTTORW_BIT)
#define DDR_SDRAM_TIMING_CFG_1_ACTTOPRE_BIT     (24)
#define DDR_SDRAM_TIMING_CFG_1_ACTTOPRE_MASK \
        (0xF << DDR_SDRAM_TIMING_CFG_1_ACTTOPRE_BIT)
#define DDR_SDRAM_TIMING_CFG_1_PRETOACT_BIT     (28)
#define DDR_SDRAM_TIMING_CFG_1_PRETOACT_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_1_PRETOACT_BIT)
#define DDR_SDRAM_SDRAM_CFG_2T_EN_BIT           (15)
#define DDR_SDRAM_SDRAM_CFG_2T_EN_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_2T_EN_BIT)
#define DDR_SDRAM_SDRAM_CFG_NCAP_BIT            (17)
#define DDR_SDRAM_SDRAM_CFG_NCAP_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_NCAP_BIT)
#define DDR_SDRAM_SDRAM_CFG_DYN_PWR_BIT         (21)
#define DDR_SDRAM_SDRAM_CFG_DYN_PWR_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_DYN_PWR_BIT)
#define DDR_SDRAM_SDRAM_CFG_SDRAM_TYPE_BIT      (24)
#define DDR_SDRAM_SDRAM_CFG_SDRAM_TYPE_MASK \
        (0x3 << DDR_SDRAM_SDRAM_CFG_SDRAM_TYPE_BIT)
#define DDR_SDRAM_SDRAM_CFG_RD_EN_BIT           (28)
#define DDR_SDRAM_SDRAM_CFG_RD_EN_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_RD_EN_BIT)
#define DDR_SDRAM_SDRAM_CFG_ECC_EN_BIT          (29)
#define DDR_SDRAM_SDRAM_CFG_ECC_EN_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_ECC_EN_BIT)
#define DDR_SDRAM_SDRAM_CFG_SREN_BIT            (30)
#define DDR_SDRAM_SDRAM_CFG_SREN_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_SREN_BIT)
#define DDR_SDRAM_SDRAM_CFG_MEM_EN_BIT          (31)
#define DDR_SDRAM_SDRAM_CFG_MEM_EN_MASK \
        (0x1 << DDR_SDRAM_SDRAM_CFG_MEM_EN_BIT)
#define DDR_SDRAM_TIMING_CFG_2_WR_DATA_DELAY_BIT (10)
#define DDR_SDRAM_TIMING_CFG_2_WR_DATA_DELAY_MASK \
        (0x7 << DDR_SDRAM_TIMING_CFG_2_WR_DATA_DELAY_BIT)
#define DDR_SDRAM_TIMING_CFG_2_ACSM_BIT         (19)
#define DDR_SDRAM_TIMING_CFG_2_ACSM_MASK \
        (0x1 << DDR_SDRAM_TIMING_CFG_2_ACSM_BIT)
#define DDR_SDRAM_TIMING_CFG_2_CPO_BIT          (24)
#define DDR_SDRAM_TIMING_CFG_2_CPO_MASK \
        (0xF << DDR_SDRAM_TIMING_CFG_2_CPO_BIT)
#define DDR_SDRAM_SDRAM_MODE_SDMODE_BIT         (0)
#define DDR_SDRAM_SDRAM_MODE_SDMODE_MASK \
        (0xFFFF << DDR_SDRAM_SDRAM_MODE_SDMODE_BIT)
#define DDR_SDRAM_SDRAM_MODE_ESDMODE_BIT        (16)
#define DDR_SDRAM_SDRAM_MODE_ESDMODE_MASK \
        (0xFFFF << DDR_SDRAM_SDRAM_MODE_ESDMODE_BIT)
#define DDR_SDRAM_SDRAM_INTERVAL_BSTOPRE_BIT    (0)
#define DDR_SDRAM_SDRAM_INTERVAL_BSTOPRE_MASK \
        (0x3FFF << DDR_SDRAM_SDRAM_INTERVAL_BSTOPRE_BIT)
#define DDR_SDRAM_SDRAM_INTERVAL_REFINT_BIT     (16)
#define DDR_SDRAM_SDRAM_INTERVAL_REFINT_MASK \
        (0x3FFF << DDR_SDRAM_SDRAM_INTERVAL_REFINT_BIT)
#define DDR_SDRAM_ERR_SBE_SBET_BIT              (16)
#define DDR_SDRAM_ECC_ERR_INJECT_EEIM_BIT       (0)
#define DDR_SDRAM_ECC_ERR_INJECT_EEIM_MASK \
        (0xFF << DDR_SDRAM_ECC_ERR_INJECT_EEIM_BIT)
#define DDR_SDRAM_ECC_ERR_INJECT_EIEN_BIT       (8)
#define DDR_SDRAM_ECC_ERR_INJECT_EIEN_MASK \
        (0x1 << DDR_SDRAM_ECC_ERR_INJECT_EIEN_BIT)
#define DDR_SDRAM_ECC_ERR_INJECT_EMB_BIT        (9)
#define DDR_SDRAM_ECC_ERR_INJECT_EMB_MASK \
        (0x1 << DDR_SDRAM_ECC_ERR_INJECT_EMB_BIT)
#define DDR_SDRAM_CAPTURE_ECC_ECE_BIT           (0)
#define DDR_SDRAM_CAPTURE_ECC_ECE_MASK \
        (0xFF << DDR_SDRAM_CAPTURE_ECC_ECE_BIT)
#define DDR_SDRAM_ERR_DETECT_MSE_BIT            (0)
#define DDR_SDRAM_ERR_DETECT_MSE_MASK \
        (0x1 << DDR_SDRAM_ERR_DETECT_MSE_BIT)
#define DDR_SDRAM_ERR_DETECT_SBE_BIT            (2)
#define DDR_SDRAM_ERR_DETECT_SBE_MASK \
        (0x1 << DDR_SDRAM_ERR_DETECT_SBE_BIT)
#define DDR_SDRAM_ERR_DETECT_MBE_BIT            (3)
#define DDR_SDRAM_ERR_DETECT_MBE_MASK \
        (0x1 << DDR_SDRAM_ERR_DETECT_MBE_BIT)
#define DDR_SDRAM_ERR_DETECT_MME_BIT            (31)
#define DDR_SDRAM_ERR_DETECT_MME_MASK \
        (0x1 << DDR_SDRAM_ERR_DETECT_MME_BIT)
#define DDR_SDRAM_ERR_DISABLE_MSED_BIT          (0)
#define DDR_SDRAM_ERR_DISABLE_MSED_MASK \
        (0x1 << DDR_SDRAM_ERR_DISABLE_MSED_BIT)
#define DDR_SDRAM_ERR_DISABLE_SBED_BIT          (2)
#define DDR_SDRAM_ERR_DISABLE_SBED_MASK \
        (0x1 << DDR_SDRAM_ERR_DISABLE_SBED_BIT)
#define DDR_SDRAM_ERR_DISABLE_MBED_BIT          (3)
#define DDR_SDRAM_ERR_DISABLE_MBED_MASK \
        (0x1 << DDR_SDRAM_ERR_DISABLE_MBED_BIT)
#define DDR_SDRAM_ERR_INT_EN_MSEE_BIT           (0)
#define DDR_SDRAM_ERR_INT_EN_MSEE_MASK \
        (0x1 << DDR_SDRAM_ERR_INT_EN_MSEE_BIT)
#define DDR_SDRAM_ERR_INT_EN_SBEE_BIT           (2)
#define DDR_SDRAM_ERR_INT_EN_SBEE_MASK \
        (0x1 << DDR_SDRAM_ERR_INT_EN_SBEE_BIT)
#define DDR_SDRAM_ERR_INT_EN_MBEE_BIT           (3)
#define DDR_SDRAM_ERR_INT_EN_MBEE_MASK \
        (0x1 << DDR_SDRAM_ERR_INT_EN_MBEE_BIT)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_VLD_BIT    (0)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_VLD_MASK \
        (0x1 << DDR_SDRAM_CAPTURE_ATTRIBUTES_VLD_BIT)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TTYP_BIT   (12)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TTYP_MASK \
        (0x3 << DDR_SDRAM_CAPTURE_ATTRIBUTES_TTYP_BIT)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TSRC_BIT   (16)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TSRC_MASK \
        (0x1F << DDR_SDRAM_CAPTURE_ATTRIBUTES_TSRC_BIT)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TSIZ_BIT   (24)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_TSIZ_MASK \
        (0x7 << DDR_SDRAM_CAPTURE_ATTRIBUTES_TSIZ_BIT)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_BNUM_BIT   (28)
#define DDR_SDRAM_CAPTURE_ATTRIBUTES_BNUM_MASK \
        (0x7 << DDR_SDRAM_CAPTURE_ATTRIBUTES_BNUM_BIT)
#define DDR_SDRAM_ERR_SBE_SBEC_BIT              (0)
#define DDR_SDRAM_ERR_SBE_SBEC_MASK (0xFF << DDR_SDRAM_ERR_SBE_SBEC_BIT)
#define DDR_SDRAM_ERR_SBE_SBET_BIT              (16)
#define DDR_SDRAM_ERR_SBE_SBET_MASK (0xFF << DDR_SDRAM_ERR_SBE_SBET_BIT)
#define	DDR_SDRAM_CFG_MEM_EN_BIT		(31)

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCmpc8540h */
