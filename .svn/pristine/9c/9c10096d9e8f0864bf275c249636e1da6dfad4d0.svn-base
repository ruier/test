/* malDcr.h - IBM Memory Access Layer (MAL) DCR access assembly routines */

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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/* Copyright 2001-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,29sep04,mdo  Documentation fixes for apigen
01e,15aug03,jtp  Formal code inspection changes: documentation
01d,10oct02,mcg  added bit definitions for 440 MAL_CFG register
01c,12jun02,pch  SPR 74987: C++
01b,07nov01,jtp  prepare for emac support
01a,24oct01,jtp  created from walnut malDcr.h version 01e
*/

/*
This file contains the definitions of the Device Control Register (DCR)
access functions of the IBM Memory Access Layer (MAL) core.
MAL is a hardware core in IBM's Blue Logic ASIC core library that is
normally situated between system memory and packet-oriented
Communications Media Access Controllers (COMMACS) in a chip. MAL's job
is to DMA data packets between the COMMACs and memory with minimal
processor core intervention.
*/

#ifndef INCmalDcrh
#define INCmalDcrh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * Memory Access Layer (MAL) core register OFFSETs. Each is a separate
 * DCR register.
 */

#define MAL_CFG      (0x00)   /* MAL Config reg 		 */
#define MAL_ESR      (0x01)   /* Error Status reg (Read/Clear)	 */
#define MAL_IER      (0x02)   /* Interrupt enable reg		 */
#define MAL_DBR      (0x03)   /* Mal Debug reg (Read only)	 */
#define MAL_TXCASR   (0x04)   /* TX Channel active reg (set)	 */
#define MAL_TXCARR   (0x05)   /* TX Channel active reg (Reset)	 */
#define MAL_TXEOBISR (0x06)   /* TX End of buffer int status reg */
#define MAL_TXDEIR   (0x07)   /* TX Descriptor Error Int reg	 */
#define MAL_RXCASR   (0x10)   /* RX Channel active reg (set)	 */
#define MAL_RXCARR   (0x11)   /* RX Channel active reg (Reset)	 */
#define MAL_RXEOBISR (0x12)   /* RX End of buffer int status reg */
#define MAL_RXDEIR   (0x13)   /* RX Descriptor Error Int reg	 */
#define MAL_TXCTPxR  (0x20)   /* TX 0 Channel table pointer reg  */
#define MAL_TXCTP0R  (0x20)   /* TX 0 Channel table pointer reg  */
#define MAL_TXCTP1R  (0x21)   /* TX 1 Channel table pointer reg  */
#define MAL_TXCTP2R  (0x22)   /* TX 2 Channel table pointer reg  */
#define MAL_TXCTP3R  (0x23)   /* TX 3 Channel table pointer reg  */
#define MAL_TXCTP4R  (0x24)   /* TX 4 Channel table pointer reg  */
#define MAL_TXCTP5R  (0x25)   /* TX 5 Channel table pointer reg  */
#define MAL_TXCTP6R  (0x26)   /* TX 6 Channel table pointer reg  */
#define MAL_TXCTP7R  (0x27)   /* TX 7 Channel table pointer reg  */
#define MAL_TXCTP8R  (0x28)   /* TX 8 Channel table pointer reg  */
#define MAL_TXCTP9R  (0x29)   /* TX 9 Channel table pointer reg  */
#define MAL_TXCTP10R (0x2A)   /* TX 10 Channel table pointer reg */
#define MAL_TXCTP11R (0x2B)   /* TX 11 Channel table pointer reg */
#define MAL_TXCTP12R (0x2C)   /* TX 12 Channel table pointer reg */
#define MAL_TXCTP13R (0x2D)   /* TX 13 Channel table pointer reg */
#define MAL_TXCTP14R (0x2E)   /* TX 14 Channel table pointer reg */
#define MAL_TXCTP15R (0x2F)   /* TX 15 Channel table pointer reg */
#define MAL_TXCTP16R (0x30)   /* TX 16 Channel table pointer reg */
#define MAL_TXCTP17R (0x31)   /* TX 17 Channel table pointer reg */
#define MAL_TXCTP18R (0x32)   /* TX 18 Channel table pointer reg */
#define MAL_TXCTP19R (0x33)   /* TX 19 Channel table pointer reg */
#define MAL_TXCTP20R (0x34)   /* TX 20 Channel table pointer reg */
#define MAL_TXCTP21R (0x35)   /* TX 21 Channel table pointer reg */
#define MAL_TXCTP22R (0x36)   /* TX 22 Channel table pointer reg */
#define MAL_TXCTP23R (0x37)   /* TX 23 Channel table pointer reg */
#define MAL_TXCTP24R (0x38)   /* TX 24 Channel table pointer reg */
#define MAL_TXCTP25R (0x39)   /* TX 25 Channel table pointer reg */
#define MAL_TXCTP26R (0x3A)   /* TX 26 Channel table pointer reg */
#define MAL_TXCTP27R (0x3B)   /* TX 27 Channel table pointer reg */
#define MAL_TXCTP28R (0x3C)   /* TX 28 Channel table pointer reg */
#define MAL_TXCTP29R (0x3D)   /* TX 29 Channel table pointer reg */
#define MAL_TXCTP30R (0x3E)   /* TX 30 Channel table pointer reg */
#define MAL_TXCTP31R (0x3F)   /* TX 31 Channel table pointer reg */

#define MAL_RXCTPxR  (0x40)   /* RX 0 Channel table pointer reg  */
#define MAL_RXCTP0R  (0x40)   /* RX 0 Channel table pointer reg  */
#define MAL_RXCTP1R  (0x41)   /* RX 1 Channel table pointer reg  */
#define MAL_RXCTP2R  (0x42)   /* RX 2 Channel table pointer reg  */
#define MAL_RXCTP3R  (0x43)   /* RX 3 Channel table pointer reg  */
#define MAL_RXCTP4R  (0x44)   /* RX 4 Channel table pointer reg  */
#define MAL_RXCTP5R  (0x45)   /* RX 5 Channel table pointer reg  */
#define MAL_RXCTP6R  (0x46)   /* RX 6 Channel table pointer reg  */
#define MAL_RXCTP7R  (0x47)   /* RX 7 Channel table pointer reg  */
#define MAL_RXCTP8R  (0x48)   /* RX 8 Channel table pointer reg  */
#define MAL_RXCTP9R  (0x49)   /* RX 9 Channel table pointer reg  */
#define MAL_RXCTP10R (0x4A)   /* RX 10 Channel table pointer reg */
#define MAL_RXCTP11R (0x4B)   /* RX 11 Channel table pointer reg */
#define MAL_RXCTP12R (0x4C)   /* RX 12 Channel table pointer reg */
#define MAL_RXCTP13R (0x4D)   /* RX 13 Channel table pointer reg */
#define MAL_RXCTP14R (0x4E)   /* RX 14 Channel table pointer reg */
#define MAL_RXCTP15R (0x4F)   /* RX 15 Channel table pointer reg */
#define MAL_RXCTP16R (0x50)   /* RX 16 Channel table pointer reg */
#define MAL_RXCTP17R (0x51)   /* RX 17 Channel table pointer reg */
#define MAL_RXCTP18R (0x52)   /* RX 18 Channel table pointer reg */
#define MAL_RXCTP19R (0x53)   /* RX 19 Channel table pointer reg */
#define MAL_RXCTP20R (0x54)   /* RX 20 Channel table pointer reg */
#define MAL_RXCTP21R (0x55)   /* RX 21 Channel table pointer reg */
#define MAL_RXCTP22R (0x56)   /* RX 22 Channel table pointer reg */
#define MAL_RXCTP23R (0x57)   /* RX 23 Channel table pointer reg */
#define MAL_RXCTP24R (0x58)   /* RX 24 Channel table pointer reg */
#define MAL_RXCTP25R (0x59)   /* RX 25 Channel table pointer reg */
#define MAL_RXCTP26R (0x5A)   /* RX 26 Channel table pointer reg */
#define MAL_RXCTP27R (0x5B)   /* RX 27 Channel table pointer reg */
#define MAL_RXCTP28R (0x5C)   /* RX 28 Channel table pointer reg */
#define MAL_RXCTP29R (0x5D)   /* RX 29 Channel table pointer reg */
#define MAL_RXCTP30R (0x5E)   /* RX 30 Channel table pointer reg */
#define MAL_RXCTP31R (0x5F)   /* RX 31 Channel table pointer reg */

#define MAL_RCBSx    (0x60)   /* RX 0 Channel buffer size reg	 */
#define MAL_RCBS0    (0x60)   /* RX 0 Channel buffer size reg	 */
#define MAL_RCBS1    (0x61)   /* RX 1 Channel buffer size reg	 */
#define MAL_RCBS2    (0x62)   /* RX 2 Channel buffer size reg	 */
#define MAL_RCBS3    (0x63)   /* RX 3 Channel buffer size reg	 */
#define MAL_RCBS4    (0x64)   /* RX 4 Channel buffer size reg	 */
#define MAL_RCBS5    (0x65)   /* RX 5 Channel buffer size reg	 */
#define MAL_RCBS6    (0x66)   /* RX 6 Channel buffer size reg	 */
#define MAL_RCBS7    (0x67)   /* RX 7 Channel buffer size reg	 */
#define MAL_RCBS8    (0x68)   /* RX 8 Channel buffer size reg	 */
#define MAL_RCBS9    (0x69)   /* RX 9 Channel buffer size reg	 */
#define MAL_RCBS10   (0x6A)   /* RX 10 Channel buffer size reg	 */
#define MAL_RCBS11   (0x6B)   /* RX 11 Channel buffer size reg	 */
#define MAL_RCBS12   (0x6C)   /* RX 12 Channel buffer size reg	 */
#define MAL_RCBS13   (0x6D)   /* RX 13 Channel buffer size reg	 */
#define MAL_RCBS14   (0x6E)   /* RX 14 Channel buffer size reg	 */
#define MAL_RCBS15   (0x6F)   /* RX 15 Channel buffer size reg	 */
#define MAL_RCBS16   (0x60)   /* RX 16 Channel buffer size reg	 */
#define MAL_RCBS17   (0x61)   /* RX 17 Channel buffer size reg	 */
#define MAL_RCBS18   (0x62)   /* RX 18 Channel buffer size reg	 */
#define MAL_RCBS19   (0x63)   /* RX 19 Channel buffer size reg	 */
#define MAL_RCBS20   (0x64)   /* RX 20 Channel buffer size reg	 */
#define MAL_RCBS21   (0x65)   /* RX 21 Channel buffer size reg	 */
#define MAL_RCBS22   (0x66)   /* RX 22 Channel buffer size reg	 */
#define MAL_RCBS23   (0x67)   /* RX 23 Channel buffer size reg	 */
#define MAL_RCBS24   (0x68)   /* RX 24 Channel buffer size reg	 */
#define MAL_RCBS25   (0x69)   /* RX 25 Channel buffer size reg	 */
#define MAL_RCBS26   (0x6A)   /* RX 26 Channel buffer size reg	 */
#define MAL_RCBS27   (0x6B)   /* RX 27 Channel buffer size reg	 */
#define MAL_RCBS28   (0x6C)   /* RX 28 Channel buffer size reg	 */
#define MAL_RCBS29   (0x6D)   /* RX 29 Channel buffer size reg	 */
#define MAL_RCBS30   (0x6E)   /* RX 30 Channel buffer size reg	 */
#define MAL_RCBS31   (0x6F)   /* RX 31 Channel buffer size reg	 */

/*
 * MAL Configuration Register bits (MAL_CFG)
 * Bits assignments are slightly different for the MAL used in 440 vs. 405
 * chips.
 */

#ifdef MAL_440

#define MAL_CFG_SR	   0x80000000	   /* MAL software reset	      */
#define MAL_CFG_RPLB_LO    0x00000000      /* Read PLB Priority Low           */
#define MAL_CFG_RPLB_MLO   0x00400000      /* Read PLB Priority Medium Low    */
#define MAL_CFG_RPLB_MHI   0x00800000      /* Read PLB Priority Medium High   */
#define MAL_CFG_RPLB_HI    0x00C00000      /* Read PLB Priority High          */
#define MAL_CFG_PLB_RBS4   0x00000000      /* Read Max Burst size of 4        */
#define MAL_CFG_PLB_RBS8   0x00100000      /* Read Max Burst size of 8        */
#define MAL_CFG_PLB_RBS16  0x00200000      /* Read Max Burst size of 16       */
#define MAL_CFG_PLB_RBS32  0x00300000      /* Read Max Burst size of 32       */
#define MAL_CFG_WPLB_LO    0x00000000      /* Write PLB Priority Low          */
#define MAL_CFG_WPLB_MLO   0x00040000      /* Write PLB Priority Medium Low   */
#define MAL_CFG_WPLB_MHI   0x00080000      /* Write PLB Priority Medium High  */
#define MAL_CFG_WPLB_HI    0x000C0000      /* Write PLB Priority High         */
#define MAL_CFG_PLB_WBS4   0x00000000      /* Write Max Burst size of 4       */
#define MAL_CFG_PLB_WBS8   0x00010000      /* Write Max Burst size of 8       */
#define MAL_CFG_PLB_WBS16  0x00020000      /* Write Max Burst size of 16      */
#define MAL_CFG_PLB_WBS32  0x00030000      /* Write Max Burst size of 32      */
#define MAL_CFG_PLBLE	   0x00008000	   /* PLB Lock error		      */
#define MAL_CFG_PLBB	   0x00004000	   /* PLB burst 		      */
#define MAL_CFG_OPBBL	   0x00000080	   /* OPB lock enable		      */
#define MAL_CFG_EOPIE	   0x00000004	   /* End of packet interrupt enable  */
#define MAL_CFG_LEA	   0x00000002	   /* Lock error active 	      */
#define MAL_CFG_SD	   0x00000001	   /* Scroll descriptor 	      */

#else /* MAL_440 */

#define MAL_CFG_SR	   0x80000000	   /* MAL software reset	      */
#define MAL_CFG_PLBP_MASK  0x00C00000	   /* PLB priority		      */
#define MAL_CFG_GA	   0x00200000	   /* Guarded active		      */
#define MAL_CFG_OA	   0x00100000	   /* Ordered active		      */
#define MAL_CFG_PLBLE	   0x00080000	   /* PLB Lock error		      */
#define MAL_CFG_PLBLT_MASK 0x00078000	   /* PLB latency timer 	      */
#define MAL_CFG_PLBB	   0x00004000	   /* PLB burst 		      */
#define MAL_CFG_OPBBL	   0x00000080	   /* OPB lock enable		      */
#define MAL_CFG_EOPIE	   0x00000004	   /* End of packet interrupt enable  */
#define MAL_CFG_LEA	   0x00000002	   /* Lock error active 	      */
#define MAL_CFG_SD	   0x00000001	   /* Scroll descriptor 	      */

#endif /* MAL_440 */

/*
 * MAL Error Status Register bits (MAL_ESR)
 *    The 1st group of bits is for the "error status" part of the register
 *	 (locked or last occurring)
 *    The 2nd group of bits is for the "interrupt status" part of the register
 */

#define MAL_ESR_EVB	  0x80000000	   /* Is there a valid error?	      */
#define MAL_ESR_CID_RX	  0x40000000	   /* Is Channel ID an RX channel?    */
#define MAL_ESR_CID_MASK  0x3E000000	   /* Channel ID mask		      */
#define MAL_ESR_DE	  0x00100000	   /* TX descriptor (not first) error */
#define MAL_ESR_ONE	  0x00080000	   /* OPB non-fullword error	      */
#define MAL_ESR_OTE	  0x00040000	   /* OPB timeout error 	      */
#define MAL_ESR_OSE	  0x00020000	   /* OPB slave error		      */
#define MAL_ESR_PEIN	  0x00010000	   /* PLB error 		      */

#define MAL_ESR_DEI	  0x00000010	   /* TX descriptor (not first) error */
#define MAL_ESR_ONEI	  0x00000008	   /* OPB non-fullword error	      */
#define MAL_ESR_OTEI	  0x00000004	   /* OPB timeout error 	      */
#define MAL_ESR_OSEI	  0x00000002	   /* OPB slave error		      */
#define MAL_ESR_PBEI	  0x00000001	   /* PLB error 		      */

/*
 * MAL Interrupt Enable Register bits (MAL_IER)
 *   These bits enable the events that will cause the activation of
 *   EXT_IRQ_MAL_SERR in the Universal Interrupt Controller.
 */

#define MAL_IER_DE	  0x00000010	   /* TX descriptor (not first) error */
#define MAL_IER_NE	  0x00000008	   /* OPB non-fullword error	      */
#define MAL_IER_TE	  0x00000004	   /* OPB timeout error 	      */
#define MAL_IER_OPBE	  0x00000002	   /* OPB slave error		      */
#define MAL_IER_PLBE	  0x00000001	   /* PLB error 		      */

/*
 * More Register definitions for MAL lib
 */

/* MADMAL transmit and receive status/control bits  */
/* for COMMAC bits, refer to the COMMAC header file */

#define MAL_TX_CTRL_READY 0x8000
#define MAL_TX_CTRL_WRAP  0x4000
#define MAL_TX_CTRL_CM	  0x2000
#define MAL_TX_CTRL_LAST  0x1000
#define MAL_TX_CTRL_INTR  0x0400

#define MAL_RX_CTRL_EMPTY 0x8000
#define MAL_RX_CTRL_WRAP  0x4000
#define MAL_RX_CTRL_CM	  0x2000
#define MAL_RX_CTRL_LAST  0x1000
#define MAL_RX_CTRL_FIRST 0x0800
#define MAL_RX_CTRL_INTR  0x0400

      /* Configuration Reg  */
#define MAL_CR_MMSR	  0x80000000
#define MAL_CR_PLBP_0	  0x00000000   /* lowest	*/
#define MAL_CR_PLBP_1	  0x00400000
#define MAL_CR_PLBP_2	  0x00800000
#define MAL_CR_PLBP_3	  0x00C00000   /* highest	*/
#define MAL_CR_PLBP_MASK  0x00C00000
#define MAL_CR_GA	  0x00200000
#define MAL_CR_OA	  0x00100000
#define MAL_CR_PLBLE	  0x00080000
#define MAL_CR_PLBLT_MASK 0x00078000
#define MAL_CR_PLBLT_1	  0x00040000
#define MAL_CR_PLBLT_2	  0x00020000
#define MAL_CR_PLBLT_3	  0x00010000
#define MAL_CR_PLBLT_4	  0x00008000
#define MAL_CR_PLBLT_DEFAULT 0x00078000 /* ????? */	
#define MAL_CR_PLBB	  0x00004000
#define MAL_CR_OPBBL	  0x00000080
#define MAL_CR_EOPIE	  0x00000004
#define MAL_CR_LEA	  0x00000002
#define MAL_CR_MSD	  0x00000001

    /* Error Status Reg    */
#define MAL_ESR_EVB	  0x80000000
#define MAL_ESR_CID	  0x40000000
#define MAL_ESR_DE	  0x00100000
#define MAL_ESR_ONE	  0x00080000
#define MAL_ESR_OTE	  0x00040000
#define MAL_ESR_OSE	  0x00020000
#define MAL_ESR_PEIN	  0x00010000
      /* same bit position as the IER */
      /* VV			 VV   */
#define MAL_ESR_DEI	  0x00000010
#define MAL_ESR_ONEI	  0x00000008
#define MAL_ESR_OTEI	  0x00000004
#define MAL_ESR_OSEI	  0x00000002
#define MAL_ESR_PBEI	  0x00000001
      /* ^^			 ^^   */
      /* Mal IER		      */
#define MAL_IER_DE	  0x00000010
#define MAL_IER_NE	  0x00000008
#define MAL_IER_TE	  0x00000004
#define MAL_IER_OPBE	  0x00000002
#define MAL_IER_PLBE	  0x00000001

#define MAL_CHANNEL0	  0x80000000
#define MAL_CHANNEL1	  0x40000000
#define MAL_CHANNEL2	  0x20000000
#define MAL_CHANNEL3	  0x10000000
#define MAL_CHANNEL4	  0x08000000
#define MAL_CHANNEL5	  0x04000000
#define MAL_CHANNEL6	  0x02000000
#define MAL_CHANNEL7	  0x01000000
#define MAL_CHANNEL8	  0x00800000
#define MAL_CHANNEL9	  0x00400000
#define MAL_CHANNEL10	  0x00200000
#define MAL_CHANNEL11	  0x00100000
#define MAL_CHANNEL12	  0x00080000
#define MAL_CHANNEL13	  0x00040000
#define MAL_CHANNEL14	  0x00020000
#define MAL_CHANNEL15	  0x00010000
#define MAL_CHANNEL16	  0x00008000
#define MAL_CHANNEL17	  0x00004000
#define MAL_CHANNEL18	  0x00002000
#define MAL_CHANNEL19	  0x00001000
#define MAL_CHANNEL20	  0x00000800
#define MAL_CHANNEL21	  0x00000400
#define MAL_CHANNEL22	  0x00000200
#define MAL_CHANNEL23	  0x00000100
#define MAL_CHANNEL24	  0x00000080
#define MAL_CHANNEL25	  0x00000040
#define MAL_CHANNEL26	  0x00000020
#define MAL_CHANNEL27	  0x00000010
#define MAL_CHANNEL28	  0x00000008
#define MAL_CHANNEL29	  0x00000004
#define MAL_CHANNEL30	  0x00000002
#define MAL_CHANNEL31	  0x00000001

#define MAL_MAXDESCRIPTORS 256
#define MAL_MAXTXBLOCK	   4095
#define MAL_MAXRXBLOCK	   4080

#define MAL_BUFF_ALIGNMENT_MASK 0xFFFFFFE0  /* 32 byte buffer alignment */

#ifdef __cplusplus
    }
#endif

#endif	/* INCmalDcrh */
