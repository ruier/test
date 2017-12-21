/* gpio.c - gpio configure functions */

/*
******************************************************************************
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

/* Copyright 2006 Wind River Systems, Inc. */

/* 
modification history
--------------------
01a,20aug03,tr   Created based on ../bamboo/gpio.c 01h
*/

/*
DESCRIPTION

This file provides routines to configure the shared interfaces (GPIO on the
chip and associated regs on the board) for various functionality.
The GPIO configuration for the chip is as follows for more details see 440EP
user's manual:

-----------------------------------------------------------------------------------
GPIO #      Alternate 1        I/O   Alternate 2       I/O     Alternate 3     I/O
-----------------------------------------------------------------------------------

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

Note that functions which share GPIO are mutually exclusive and cannot
be configured together.

*/

#include <vxWorks.h>
#include <config.h>

/*
 *  Externals
 */

IMPORT UINT32 romDcrGet (UINT32 sdraddr);
IMPORT void romDcrSet (UINT32 sdraddr, UINT32 dat);
IMPORT UINT8 int8 (UINT32);
IMPORT void out8 (UINT32 addr, UINT8 dat);
IMPORT UINT32 romSdrGet (UINT32 sdraddr);
IMPORT void romSdrSet (UINT32 sdraddr, UINT32 dat);
IMPORT void sysSdrOutLong (UINT32, UINT32);
IMPORT void sdramResetDelay (void);
/*
 * Local definitions
 */

/*******************************************************************************
*
* enableUARTIntf(UINT8 numUart) - enable UART0,1,2,3 interfaces (GPIO+board)
*
* Enable UART0,1,2,3 interfaces (GPIO+board)
*
* INPUT  : number of uarts (from 1 to 4 on amcc440ep 
*
* RETURNS: void
*
* ERRNO
*/

LOCAL void enableUARTIntf
   (
   UINT8 numUart
   )  
   {
   UINT   gpioReg;

   switch (numUart)
   {
      case 1: /* L1 config: UART0 = 8 pins */

        /* configure inputs */

        gpioReg = (*(UINT32 *)GPIO1_IS1L);
        gpioReg = ( (gpioReg & ~(IS1L1_UARTL1_MASK)) | IS1L1_UARTL1_VAL);
        *(UINT32 *)GPIO1_IS1L = gpioReg;

        /* configure outputs */

        gpioReg = (*(UINT32 *)GPIO1_TSL);
        gpioReg = ( (gpioReg & ~(TSL1_UARTL1_MASK)) | TSL1_UARTL1_VAL);
        *(UINT32 *)GPIO1_TSL = gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_OSL);
        gpioReg = ( (gpioReg & ~(OSL1_UARTL1_MASK)) | OSL1_UARTL1_VAL);
        *(UINT32 *)GPIO1_OSL = gpioReg;
        break;
      case 2: /* L2 config: UART0 = 4 pins,UART1 = 4 pins */

        /* configure inputs */

        gpioReg = (*(UINT32 *)GPIO1_IS1L);
        gpioReg = ( (gpioReg & ~(IS1L1_UARTL2_MASK)) | IS1L1_UARTL2_VAL);
        *(UINT32 *)GPIO1_IS1L = gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_IS2L);
        gpioReg = ( (gpioReg & ~(IS2L1_UARTL2_MASK)) | IS2L1_UARTL2_VAL);
        *(UINT32 *)GPIO1_IS2L = gpioReg;

        /* configure outputs */

        gpioReg = (*(UINT32 *)GPIO1_TSL);
        gpioReg = ( (gpioReg & ~(TSL1_UARTL2_MASK)) | TSL1_UARTL2_VAL);
        *(UINT32 *)GPIO1_TSL = gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_OSL);
        gpioReg = ( (gpioReg & ~(OSL1_UARTL2_MASK)) | OSL1_UARTL2_VAL);
        *(UINT32 *)GPIO1_OSL = gpioReg;

        break;
      case 3: /* L3 config: UART0 = 4 pins,UART1 = 2 pins,UART 2 = 2 pins */
        /* configure inputs */

        gpioReg = (*(UINT32 *)GPIO1_IS1L);
        gpioReg = ( (gpioReg & ~(IS1L1_UARTL3_MASK)) | IS1L1_UARTL3_VAL);
        *(UINT32 *)GPIO1_IS1L = gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_IS2L);
        gpioReg = ( (gpioReg & ~(IS2L1_UARTL3_MASK)) | IS2L1_UARTL3_VAL);
        *(UINT32 *)GPIO1_IS2L = gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_IS3L);
        gpioReg = ( (gpioReg & ~(IS3L1_UARTL3_MASK)) | IS3L1_UARTL3_VAL);
        *(UINT32 *)GPIO1_IS3L = gpioReg;

        /* configure outputs */

        gpioReg = (*(UINT32 *)GPIO1_TSL);
        gpioReg = ( (gpioReg & ~(TSL1_UARTL3_MASK)) | TSL1_UARTL3_VAL);
        *(UINT32 *)GPIO1_TSL= gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_OSL);
        gpioReg = ( (gpioReg & ~(OSL1_UARTL3_MASK)) | OSL1_UARTL3_VAL);
        *(UINT32 *)GPIO1_OSL= gpioReg;

        break;
      case 4: /* L4 config: UART0 = 2 pins,UART1 = 2 pins,UART 2 = 2 pins,UART 3 = 2 pins */

        /* configure inputs */

        gpioReg = (*(UINT32 *)GPIO1_IS1L);
        gpioReg = ( (gpioReg & ~(IS1L1_UARTL4_MASK)) | IS1L1_UARTL4_VAL);
        *(UINT32 *)GPIO1_IS1L= gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_IS2L);
        gpioReg = ( (gpioReg & ~(IS2L1_UARTL4_MASK)) | IS2L1_UARTL4_VAL);
        *(UINT32 *)GPIO1_IS2L= gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_IS3L);
        gpioReg = ( (gpioReg & ~(IS3L1_UARTL4_MASK)) | IS3L1_UARTL4_VAL);
        *(UINT32 *)GPIO1_IS3L= gpioReg;

        /* configure outputs */

        gpioReg = (*(UINT32 *)GPIO1_TSL);
        gpioReg = ( (gpioReg & ~(TSL1_UARTL4_MASK)) | TSL1_UARTL4_VAL);
        *(UINT32 *)GPIO1_TSL= gpioReg;
        gpioReg = (*(UINT32 *)GPIO1_OSL);
        gpioReg = ( (gpioReg & ~(OSL1_UARTL4_MASK)) | OSL1_UARTL4_VAL);
        *(UINT32 *)GPIO1_OSL= gpioReg;

        break;

   }

}

/*******************************************************************************
*
* enableEMACIntf - enable EMAC interfaces  (GPIO+board)
*
* Enable EMAC interfaces  (GPIO+board)
*
* INPUT  : pktRjctMode : 0 : Disable Packet Reject, 1 : Enable Packet Reject
* Note :  if SEL_NAND_FLASH flag is set means no MII interface (error in this case)
*         and only EMAC0 is supported for other interfaces
*
* RETURNS: void
*
* ERRNO
*/

LOCAL void enableEMACIntf
    (
    UINT8 pktRjctMode
    )
    {
   UINT32 gpioReg;
   UINT32 sdrReg;

   /* Configure Gpio to enable always complete MII interface so RMII and SMII 
    * configure inputs 
    */

   gpioReg = (*(UINT32 *)GPIO0_IS1L);
   gpioReg = ( (gpioReg & ~(IS1L0_MII_MASK)) | IS1L0_MII_VAL);
   *(UINT32 *)GPIO0_IS1L = gpioReg;
   gpioReg = (*(UINT32 *)GPIO0_IS1H);
   gpioReg = ( (gpioReg & ~(IS1H0_MII_MASK)) | IS1H0_MII_VAL);
   *(UINT32 *)GPIO0_IS1H = gpioReg;

   /* configure outputs */

   gpioReg = (*(UINT32 *)GPIO0_TSH);
   gpioReg = ( (gpioReg & ~(TSH0_MII_MASK)) | TSH0_MII_VAL);
   *(UINT32 *)GPIO0_TSH = gpioReg;
   gpioReg = (*(UINT32 *)GPIO0_OSH);
   gpioReg = ( (gpioReg & ~(OSH0_MII_MASK)) | OSH0_MII_VAL);
   *(UINT32 *)GPIO0_OSH = gpioReg;

   /* Set pin shared by EMAC and NDFC */

   sdrReg = 0x40082350;
   romSdrSet (SDR0_CUST0, sdrReg);

   /* Read SDR0_MFR register */

   sdrReg = romSdrGet(SDR0_MFR);

   if (pktRjctMode ==1)
   {

     /* Enable packet reject */

     sdrReg = sdrReg | SDR0_MFR_PKT_REJECT_0_ENA | SDR0_MFR_PKT_REJECT_1_ENA;
   }
   else
   {

     /* Disable packet reject */

     sdrReg = sdrReg & ~SDR0_MFR_PKT_REJECT_0_ENA & ~SDR0_MFR_PKT_REJECT_1_ENA;
   }

   /* save in SDR0_MFR register */

   romSdrSet(SDR0_MFR,sdrReg);

}

/*******************************************************************************
*
* enableExtUIC() - enable all Ext UIC interrupt lines (GPIO + PFC1)
*                  as ext DMA not supported in this BSP
*
* Enable all Ext UIC interrupt lines (GPIO + PFC1) as ext DMA not supported 
* in this BSP
*
* INPUT  : void
*
*
* RETURNS: void
*
* ERRNO
*/

LOCAL void enableExtUIC()
{
   UINT   gpioReg;
   UINT32 pfc1Reg;

   /* Configure Gpio to enable all Ext UIC interrupt */

   gpioReg = (*(UINT32 *)GPIO1_IS1L);
   gpioReg = ( (gpioReg & ~(IS1L1_UIC_MASK)) | IS1L1_UIC_VAL);
   *(UINT32 *)GPIO1_IS1L = gpioReg;
   gpioReg = (*(UINT32 *)GPIO1_IS1H);
   gpioReg = ( (gpioReg & ~(IS1H1_UIC_MASK)) | IS1H1_UIC_VAL);
   *(UINT32 *)GPIO1_IS1H = gpioReg;

   /* Init IRQ5 sharing (which is not muxed with a GPIO) */

   pfc1Reg = romSdrGet(SDR0_PFC1);
   pfc1Reg |= SDR0_PFC1_DIS_IRQ5;
   romSdrSet(SDR0_PFC1,pfc1Reg);
}

#ifdef USE_USB1_1_DEVICE
/*******************************************************************************
*
* selectUSBDevIntf() - select USB Device Interface Type (USB 1.1 or USB 2.0)
*
* Select USB Device Interface Type (USB 1.1 or USB 2.0)
*
*
* INPUT  : usbType : 1 : USB1.1, 2 : USB2.0
*
*
* RETURNS: void
*
* ERRNO
*/

LOCAL void selectUSBDevIntf
    (
    UINT8 usbType
    )
    {
   UINT32 usb0Reg;

  /* Read USB config info in the chip */

  usb0Reg = romSdrGet(SDR0_USB0);

  if(usbType == 1)
  {

  /* USB1.1 Interface selected */

  /* Configure the 440EP with the corresponding config */
  usb0Reg |= SDR0_USB0_USB2D_DEVSEL;
  romSdrSet(SDR0_USB0,usb0Reg);

  }
  else
  {

  /* USB2.0 Interface selected */

  /* Configure the 440EP with the corresponding config */

  usb0Reg &= ~SDR0_USB0_USB2D_DEVSEL;
  romSdrSet(SDR0_USB0,usb0Reg);

  }
}
#endif /* USE_USB1_1_DEVICE */

#ifdef PPC440EP_CHIP_3_ERRATA_HARD_PATCH
/*******************************************************************************
*
* enableExtHLDA() - enable EBC_EXT_HDLA (GPIO + PFC1)
*                   to enable PPC440EP CHIP_3 errata workaround (hardware patch)
*                   Note that in this case the USB2 Device interface cannot be used
*
* Enable EBC_EXT_HDLA (GPIO + PFC1) to enable PPC440EP CHIP_3 errata workaround 
* (hardware patch) Note that in this case the USB2 Device interface cannot be used
*
* INPUT  : void
*
* RETURNS: void
*
* ERRNO
*/

LOCAL void enableExtHLDA()
{
   UINT   gpioReg;
   UINT32 pfc1Reg;


   /* Configure Gpio to enable ExtHLDA */

   gpioReg = (*(UINT32 *)GPIO0_TSH);
   gpioReg |= TSH0_EBC_EXT_HDLA;
   *(UINT32 *)GPIO0_TSH = gpioReg;
   gpioReg = (*(UINT32 *)GPIO0_OSH);
   gpioReg |= OSH0_EBC_EXT_HDLA;
   *(UINT32 *)GPIO0_OSH = gpioReg;

   /* Enable Ext EBC Master */

   pfc1Reg = romSdrGet(SDR0_PFC1);
   pfc1Reg |= SDR0_PFC1_UES_EBCHR;
   romSdrSet(SDR0_PFC1,pfc1Reg);
}
#endif

/*******************************************************************************
*
* pinSharingInit() - This function configures the chip and board pin sharing
*
* This function configures the chip and board pin sharing.
*
* INPUT  : void
*
* RETURNS: void
*
* ERRNO
*/

void pinSharingInit()
{

  /*clear tmrclk divisor*/

  *BCSR4_CFG_TMRCLK = 0x00;
	
  /*enable ethernet*/

  *BCSR8_ETHER_CNTRL = (BCSR8_PHY0_RST | BCSR8_PHY0_EN | 
                        BCSR8_PHY1_RST | BCSR8_PHY1_EN);  
	
  /* enable usb 1.1 fs device and remove usb 2.0 reset */

  *BCSR9_USB_CNTRL = 0x00;

  /* Get rid of flash write protect */

  *(BCSR7_MEM_STTM) = 0x00;
	
  /* Enable all ext UIC */

  enableExtUIC();
  
  /* Configure UARTs wiring */

  enableUARTIntf(NUM_TTY);
 
  /* Configure ethernet interface */

  enableEMACIntf(1);

  /* Configure USB Device interface */

  #ifdef USE_USB1_1_DEVICE
  selectUSBDevIntf(1);
  #endif

  #ifdef PPC440EP_PCI_ERRATA1
  /* if workaround (hardware patch) to PPC440EP_PCI_ERRATA1 is to be used,
   * EBC External HLDA signal and EBC Master function must be enabled.
   */
  enableExtHLDA()
  #endif
	  
}
