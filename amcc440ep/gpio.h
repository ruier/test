/* gpio.h - gpio include file */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM’S OR ITS LICENSOR’S
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

   COPYRIGHT   I B M   CORPORATION 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 2004 Wind River Systems, Inc. */

/* 
modification history
--------------------
01d,08nov04,dr   Additional documentation fixes for apigen
01c,07sep04,mdo  Documentation fixes for apigen
01b,02jun04,tr   Add definitions needed for PPC440EP CHIP_3 ERRATA workaround (hardware patch)
01a,25jul03,tr   Created
*/

#ifndef INCgpioh
#define INCgpioh

#ifdef __cplusplus
extern "C" {
#endif

/* Reg Definitions */

#define GPIO0_OR  (GPIO0_BASE+0x00) /* R/W GPIO0 Output Register                    */
#define GPIO0_TCR (GPIO0_BASE+0x04) /* R/W GPIO0 Three State Control Register       */
#define GPIO0_OSL (GPIO0_BASE+0x08) /* R/W GPIO0 Output Select Register(0:31)       */
#define GPIO0_OSH (GPIO0_BASE+0x0C) /* R/W GPIO0 Output Select Register(32:63)       */
#define GPIO0_TSL (GPIO0_BASE+0x10) /* R/W GPIO0 Three State Select Register(0:31)  */
#define GPIO0_TSH (GPIO0_BASE+0x14) /* R/W GPIO0 Three State Select Register(32:63) */
#define GPIO0_ODR (GPIO0_BASE+0x18) /* R/W GPIO0 Open drain Register                */
#define GPIO0_IR  (GPIO0_BASE+0x1C) /* R GPIO0 Input Register                       */
#define GPIO0_RR1 (GPIO0_BASE+0x20) /* R/W GPIO0 Receive Register 1                 */
#define GPIO0_RR2 (GPIO0_BASE+0x24) /* R/W GPIO0 Receive Register 2                 */
#define GPIO0_RR3 (GPIO0_BASE+0x28) /* R/W GPIO0 Receive Register 3                 */
#define GPIO0_IS1L (GPIO0_BASE+0x30) /* R/W GPIO0 Input SelectRegister 1 (0:31)      */
#define GPIO0_IS1H (GPIO0_BASE+0x34) /* R/W GPIO0 Input SelectRegister 1 (32:63)     */
#define GPIO0_IS2L (GPIO0_BASE+0x38) /* R/W GPIO0 Input SelectRegister 2 (0:31)      */
#define GPIO0_IS2H (GPIO0_BASE+0x3C) /* R/W GPIO0 Input SelectRegister 2 (32:63)     */
#define GPIO0_IS3L (GPIO0_BASE+0x40) /* R/W GPIO0 Input SelectRegister 3 (0:31)      */
#define GPIO0_IS3H (GPIO0_BASE+0x44) /* R/W GPIO0 Input SelectRegister 3 (32:63)     */

#define GPIO1_OR  (GPIO1_BASE+0x00) /* R/W GPIO1 Output Register                    */
#define GPIO1_TCR (GPIO1_BASE+0x04) /* R/W GPIO1 Three State Control Register       */
#define GPIO1_OSL (GPIO1_BASE+0x08) /* R/W GPIO1 Output Select Register(0:31)       */
#define GPIO1_OSH (GPIO1_BASE+0x0C) /* R/W GPIO1 Output Select Register(32:63)       */
#define GPIO1_TSL (GPIO1_BASE+0x10) /* R/W GPIO1 Three State Select Register(0:31)  */
#define GPIO1_TSH (GPIO1_BASE+0x14) /* R/W GPIO1 Three State Select Register(32:63) */
#define GPIO1_ODR (GPIO1_BASE+0x18) /* R/W GPIO1 Open drain Register                */
#define GPIO1_IR  (GPIO1_BASE+0x1C) /* R GPIO1 Input Register                       */
#define GPIO1_RR1 (GPIO1_BASE+0x20) /* R/W GPIO1 Receive Register 1                 */
#define GPIO1_RR2 (GPIO1_BASE+0x24) /* R/W GPIO1 Receive Register 2                 */
#define GPIO1_RR3 (GPIO1_BASE+0x28) /* R/W GPIO1 Receive Register 3                 */
#define GPIO1_IS1L (GPIO1_BASE+0x30) /* R/W GPIO1 Input SelectRegister 1 (0:31)      */
#define GPIO1_IS1H (GPIO1_BASE+0x34) /* R/W GPIO1 Input SelectRegister 1 (32:63)     */
#define GPIO1_IS2L (GPIO1_BASE+0x38) /* R/W GPIO1 Input SelectRegister 2 (0:31)      */
#define GPIO1_IS2H (GPIO1_BASE+0x3C) /* R/W GPIO1 Input SelectRegister 2 (32:63)     */
#define GPIO1_IS3L (GPIO1_BASE+0x40) /* R/W GPIO1 Input SelectRegister 3 (0:31)      */
#define GPIO1_IS3H (GPIO1_BASE+0x44) /* R/W GPIO1 Input SelectRegister 3 (32:63)     */

/*
DESCRIPTION

The GPIO configuration for the chip is as follows for more details see 440EP user's manual:

-------------------------------------------------------------------------------
GPIO #      Alternate 1        I/O   Alternate 2       I/O     Alternate 3     I/O
-------------------------------------------------------------------------------

GPIO0_0     EBC_ADDR(7)       I/O   DMA_REQ(2)        I
GPIO0_1     EBC_ADDR(6)       I/O   DMA_ACK(2)        O
GPIO0_2     EBC_ADDR(5)       I/O   DMA_EOT/TC(2)     I/O
GPIO0_3     EBC_ADDR(4)       I/O   DMA_REQ(3)        I
GPIO0_4     EBC_ADDR(3)       I/O   DMA_ACK(3)        O
GPIO0_5     EBC_ADDR(2)       I/O   DMA_EOT/TC(3)     I/O
GPIO0_6     EBC_CS_N(1)       O
GPIO0_7     EBC_CS_N(2)       O
GPIO0_8     EBC_CS_N(3)       O
GPIO0_9     EBC_CS_N(4)       O
GPIO0_10    EBC_CS_N(5)       O
GPIO0_11    EBC_BUS_ERR       I
GPIO0_12    ZII_p0Rxd(0)      I
GPIO0_13    ZII_p0Rxd(1)      I
GPIO0_14    ZII_p0Rxd(2)      I
GPIO0_15    ZII_p0Rxd(3)      I
GPIO0_16    ZII_p0Txd(0)      O
GPIO0_17    ZII_p0Txd(1)      O
GPIO0_18    ZII_p0Txd(2)      O
GPIO0_19    ZII_p0Txd(3)      O
GPIO0_20    ZII_p0Rx_er       I
GPIO0_21    ZII_p0Rx_dv       I
GPIO0_22    ZII_p0Crs         I
GPIO0_23    ZII_p0Tx_er       O
GPIO0_24    ZII_p0Tx_en       O
GPIO0_25    ZII_p0Col         I
GPIO0_26    USB2D_RXVALID     I
GPIO0_27    EXT_EBC_REQ       I     USB2D_RXERROR     I
GPIO0_28    USB2D_TXVALID     O
GPIO0_29    EBC_EXT_HDLA      O     USB2D_PAD_SUSPNDM O
GPIO0_30    EBC_EXT_ACK       O     USB2D_XCVRSELECT  O
GPIO0_31    EBC_EXR_BUSREQ    O     USB2D_TERMSELECT  O

GPIO1_0     USB2D_OPMODE0     O
GPIO1_1     USB2D_OPMODE1     O
GPIO1_2     UART0_DCD_N       I     UART1_DSR_CTS_N   I       UART2_SOUT    O
GPIO1_3     UART0_8PIN_DSR_N  I     UART1_RTS_DTR_N   O       UART2_SIN     I
GPIO1_4     UART0_8PIN_CTS_N  I     UART3_SIN         I
GPIO1_5     UART0_RTS_N       O     UART3_SOUT        O
GPIO1_6     UART0_DTR_N       O     UART1_SOUT        O
GPIO1_7     UART0_RI_N        I     UART1_SIN         I
GPIO1_8     UIC_IRQ(0)        I
GPIO1_9     UIC_IRQ(1)        I
GPIO1_10    UIC_IRQ(2)        I
GPIO1_11    UIC_IRQ(3)        I
GPIO1_12    UIC_IRQ(4)        I     DMA_ACK(1)        O
GPIO1_13    UIC_IRQ(6)        I     DMA_EOT/TC(1)     I/O
GPIO1_14    UIC_IRQ(7)        I     DMA_REQ(0)        I
GPIO1_15    UIC_IRQ(8)        I     DMA_ACK(0)        O
GPIO1_16    UIC_IRQ(9)        I     DMA_EOT/TC(0)     I/O
GPIO1_17
GPIO1_18
GPIO1_19
GPIO1_20
GPIO1_21
GPIO1_22
GPIO1_23
GPIO1_24
GPIO1_25
GPIO1_26
GPIO1_27
GPIO1_28
GPIO1_29
GPIO1_30
GPIO1_31
*/


/*
 * Bit definitions
 */

/* EBC Basic */
#define TSL0_EBC_MASK   0xFFFFFC00 /* Bits mask for  EBC addresses and CS */
#define TSL0_EBC_VAL    0x55555400 /* Enable EBC addresses and CS (ALT1) */
#define OSL0_EBC_MASK   0xFFFFFC00 /* Bits mask for  EBC addresses and CS */
#define OSL0_EBC_VAL    0x55555400 /* Source ALT1 for EBC addresses and CS pins */

/* Enable all external UIC */
#define IS1L1_UIC_MASK  0x0000FFFF  /* Bits mask for UIC_IRQ0-8 */
#define IS1L1_UIC_VAL   0x00005555  /* Select corresponding chip inputs */
#define IS1H1_UIC_MASK  0xC0000000  /* Bits mask for UIC_IRQ9 */
#define IS1H1_UIC_VAL   0x40000000  /* Select corresponding chip input */


/* UART Interfaces */
/* L1 config: UART0 = 8 pins */
  /* Inputs */
#define IS1L1_UARTL1_MASK    0x0FC30000  /* Mask for UART L1 config */
#define IS1L1_UARTL1_VAL     0x05410000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSL1_UARTL1_MASK     0x003C0000  /* Mask for UART L1 config */
#define TSL1_UARTL1_VAL      0x00140000  /* Enable corresponding chip outputs */
#define OSL1_UARTL1_MASK     0x003C0000  /* Mask for UART L1 config */
#define OSL1_UARTL1_VAL      0x00140000  /* Source ALT1 for corresponding outputs */
/* L2 config: UART0 = 4 pins,UART1 = 4 pins */
  /* Inputs */
#define IS1L1_UARTL2_MASK    0x0FC30000  /* Mask for UART L2 config ALT1 */
#define IS1L1_UARTL2_VAL     0x0F430000  /* Select corresponding chip inputs */
#define IS2L1_UARTL2_MASK    0x0C030000  /* Mask for UART L2 config ALT2 */
#define IS2L1_UARTL2_VAL     0x04010000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSL1_UARTL2_MASK     0x033C0000  /* Mask for UART L2 config */
#define TSL1_UARTL2_VAL      0x02180000  /* Enable corresponding chip outputs */
#define OSL1_UARTL2_MASK     0x033C0000  /* Mask for UART L2 config */
#define OSL1_UARTL2_VAL      0x02180000  /* Source ALT1 or ALT2 for corresponding outputs */
/* L3 config: UART0 = 4 pins,UART1 = 2 pins,UART 2 = 2 pins */
  /* Inputs */
#define IS1L1_UARTL3_MASK    0x0FC30000  /* Mask for UART L3 config ALT1 */
#define IS1L1_UARTL3_VAL     0x0F430000  /* Select corresponding chip inputs */
#define IS2L1_UARTL3_MASK    0x0C030000  /* Mask for UART L3 config ALT2 */
#define IS2L1_UARTL3_VAL     0x0C010000  /* Select corresponding chip inputs */
#define IS3L1_UARTL3_MASK    0x03000000  /* Mask for UART L3 config ALT3 */
#define IS3L1_UARTL3_VAL     0x01000000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSL1_UARTL3_MASK     0x0C3C0000  /* Mask for UART L3 config */
#define TSL1_UARTL3_VAL      0x0C180000  /* Enable corresponding chip outputs */
#define OSL1_UARTL3_MASK     0x0C3C0000  /* Mask for UART L3 config */
#define OSL1_UARTL3_VAL      0x0C180000  /* Source ALT1 or ALT2 or ALT3 for corresponding outputs */
/* L4 config: UART0 = 2 pins,UART1 = 2 pins,UART 2 = 2 pins,UART 3 = 2 pins */
  /* Inputs */
#define IS1L1_UARTL4_MASK    0x0FC30000  /* Mask for UART L4 config ALT1 */
#define IS1L1_UARTL4_VAL     0x0FC30000  /* Select Alternate receiver (no signals used)*/
#define IS2L1_UARTL4_MASK    0x0C030000  /* Mask for UART L4 config ALT2 */
#define IS2L1_UARTL4_VAL     0x0C010000  /* Select corresponding chip inputs */
#define IS3L1_UARTL4_MASK    0x03C00000  /* Mask for UART L4 config ALT3 */
#define IS3L1_UARTL4_VAL     0x01400000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSL1_UARTL4_MASK     0x0C3C0000  /* Mask for UART L4 config */
#define TSL1_UARTL4_VAL      0x0C380000  /* Enable corresponding chip outputs */
#define OSL1_UARTL4_MASK     0x0C3C0000  /* Mask for UART L4 config */
#define OSL1_UARTL4_VAL      0x0C380000  /* Source ALT1 or ALT2 or ALT3 for corresponding outputs */

/* Ethernet Interfaces */
/* MII */
  /* Inputs */
#define IS1L0_MII_MASK    0x000000FF  /* Mask for Rxd(0:3) */
#define IS1L0_MII_VAL     0x00000055  /* Select corresponding chip inputs */
#define IS1H0_MII_MASK    0x00FC3000  /* Mask for Rx_er,Rx_dv,Crs,Col */
#define IS1H0_MII_VAL     0x00541000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSH0_MII_MASK     0xFF03C000  /* Mask for Txd(0:3),Tx_er,Tx_en */
#define TSH0_MII_VAL      0x55014000  /* Enable corresponding chip outputs */
#define OSH0_MII_MASK     0xFF03C000  /* Mask for Txd(0:3),Tx_er,Tx_en */
#define OSH0_MII_VAL      0x55014000  /* Source ALT1 for corresponding outputs */
/* RMII A */
  /* Inputs */
#define IS1L0_RMIIA_MASK  0x000000F0  /* Mask for A_rxd(0:1) */
#define IS1L0_RMIIA_VAL   0x00000050  /* Select corresponding chip inputs */
#define IS1H0_RMIIA_MASK  0x00CC0000  /* Mask for A_rx_er,A_crs_dv */
#define IS1H0_RMIIA_VAL   0x00440000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSH0_RMIIA_MASK   0xF000C000  /* Mask for A_txd(0:1),A_tx_en */
#define TSH0_RMIIA_VAL    0x50004000  /* Enable corresponding chip outputs */
#define OSH0_RMIIA_MASK   0xF000C000  /* Mask for A_txd(0:1),A_tx_en */
#define OSH0_RMIIA_VAL    0x50004000  /* Source ALT1 for corresponding outputs */
/* RMII B */
  /* Inputs */
#define IS1L0_RMIIB_MASK  0x0000000F  /* Mask for B_rxd(0:1) */
#define IS1L0_RMIIB_VAL   0x00000005  /* Select corresponding chip inputs */
#define IS1H0_RMIIB_MASK  0x00303000  /* Mask for B_rx_er,B_crs_dv */
#define IS1H0_RMIIB_VAL   0x00101000  /* Select corresponding chip inputs */
  /* Outputs */
#define TSH0_RMIIB_MASK   0x0F030000  /* Mask for B_txd(0:1),B_tx_en */
#define TSH0_RMIIB_VAL    0x05010000  /* Enable corresponding chip outputs */
#define OSH0_RMIIB_MASK   0x0F030000  /* Mask for B_txd(0:1),B_tx_en */
#define OSH0_RMIIB_VAL    0x05010000  /* Source ALT1 for corresponding outputs */
/* SMII A */
  /* Inputs */
#define IS1L0_SMIIA_MASK  0x000000C0  /* Mask for A_rxd */
#define IS1L0_SMIIA_VAL   0x00000040  /* Select corresponding chip inputs */
  /* Outputs */
#define TSH0_SMIIA_MASK   0xC000C000  /* Mask for A_txd,Sync */
#define TSH0_SMIIA_VAL    0x40004000  /* Enable corresponding chip outputs */
#define OSH0_SMIIA_MASK   0xC000C000  /* Mask for A_txd,Sync */
#define OSH0_SMIIA_VAL    0x40004000  /* Source ALT1 for corresponding outputs */
/* SMII B */
  /* Inputs */
#define IS1L0_SMIIB_MASK  0x00000030  /* Mask for B_rxd */
#define IS1L0_SMIIB_VAL   0x00000010  /* Select corresponding chip inputs */
  /* Outputs */
#define TSH0_SMIIB_MASK   0x3000C000  /* Mask for B_txd,Sync */
#define TSH0_SMIIB_VAL    0x10004000  /* Enable corresponding chip outputs */
#define OSH0_SMIIB_MASK   0x3000C000  /* Mask for B_txd,Sync */
#define OSH0_SMIIB_VAL    0x10004000  /* Source ALT1 for corresponding outputs */

#ifdef PPC440EP_CHIP_3_ERRATA_HARD_PATCH
/* Enable EBC External HDLA */
#define TSH0_EBC_EXT_HDLA 0x00000010  /* Enable corresponding chip output */
#define OSH0_EBC_EXT_HDLA 0x00000010  /* Source ALT1 for corresponding output */
#endif

#ifdef __cplusplus
}
#endif

#endif  /* INCgpioh */

