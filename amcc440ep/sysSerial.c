/* sysSerial.c - IBM 440EP serial device initialization */

/*
****************************************************************************
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

****************************************************************************
\NOMANUAL
*/

/* Copyright 2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01e,25nov04,cjj  mods to fix compiler warnings
01d,28oct04,dr   resync'ed with ocotea
01c,08sep04,dr   additional documentation fixes for apigen
01b,07sep04,mdo  Documentation fixes for apigen
01a,15jul03,tr   Written based on Ebony sysSerial.c version 01c
*/

/*
DESCRIPTION
This library contains board-specific routines for serial devices.
There are four serial ports integrated into the chip. These
devices are 16550-like. The evbNs16550Sio driver is used for all 4 devices.

INCLUDE FILES:
*/

#include "vxWorks.h"
#include "config.h"
#include "iv.h"
#include "intLib.h"
#include "sysLib.h"
#include "drv/sio/evbNs16550Sio.h"


/* device initialization structure */

typedef struct
    {
    USHORT vector;
    ULONG  baseAdrs;
    USHORT regSpace;
    USHORT intLevel;
    } EVBNS16550_CHAN_PARAS;


/* initialize the structure for each serial device */
static EVBNS16550_CHAN_PARAS dev0Paras =
      {0,UART0_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART0};

static EVBNS16550_CHAN_PARAS dev1Paras =
      {0,UART1_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART1};

static EVBNS16550_CHAN_PARAS dev2Paras =
      {0,UART2_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART2};

static EVBNS16550_CHAN_PARAS dev3Paras =
      {0,UART3_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART3};

#define UART0_REG(reg)   (dev0Paras.baseAdrs + reg*dev0Paras.regSpace)
#define UART1_REG(reg)   (dev1Paras.baseAdrs + reg*dev1Paras.regSpace)
#define UART2_REG(reg)   (dev2Paras.baseAdrs + reg*dev2Paras.regSpace)
#define UART3_REG(reg)   (dev3Paras.baseAdrs + reg*dev3Paras.regSpace)

static EVBNS16550_CHAN  evbNs16550Chan0;
static EVBNS16550_CHAN  evbNs16550Chan1;
static EVBNS16550_CHAN  evbNs16550Chan2;
static EVBNS16550_CHAN  evbNs16550Chan3;


#ifndef UART_CLOCK_EXTERNAL

/* forward declarations */

STATUS sysSerialCalcDivisor(UINT freqSourceMhz, UINT baudRate, UINT *divisor );

/* Structure for internal serial clock array */

typedef struct
    {
    UINT freqSourceMhz;
    UINT baudRate;
    UINT uartClkDivisor;
    } INT_SERIAL_INFO;

/*
 * Table of PLB speeds and baud rates used to determine the PLB:UART
 * divisor. The PLB clock is divided by the PLB:UART divisor to get the
 * UART clock which is fed into the UARTs. The UART clock is then
 * divided again by the UART baud rate divisor to obtain the baud rate. The
 * table of PLB:UART divisors below helps to select a value that will result
 * in the lowest % error on the baud rate.
 *
 * NOTE:
 * The PLB:UART divisor must be high enough so the resulting UART clock
 * is less than 1/2 of the OPB bus frequency.
 */

INT_SERIAL_INFO uartClkTbl[] =
    {
      /* PLBFreq      Baud     PLB:UART divisor */

        {   75,       1200,     21  },
        {   75,       2400,     21  },
        {   75,       4800,     16  },
        {   75,       9600,      8  },
        {   75,      19200,      7  },
        {   75,      28800,     18  },
        {   75,      33600,     28  },
        {   75,      38400,     11  },
        {   75,      57600,      9  },
        {   75,     115200,     20  },
        {   75,     307200,     15  },

        {   83,       1200,     20  },
        {   83,       2400,     10  },
        {   83,       4800,      5  },
        {   83,       9600,     32  },
        {   83,      19200,     17  },
        {   83,      28800,     18  },
        {   83,      33600,     31  },
        {   83,      38400,     17  },
        {   83,      57600,     18  },
        {   83,     115200,      9  },
        {   83,     307200,      6  },

        {  100,       1200,     28  },
        {  100,       2400,     28  },
        {  100,       4800,     14  },
        {  100,       9600,      7  },
        {  100,      19200,     25  },
        {  100,      28800,     31  },
        {  100,      33600,     31  },
        {  100,      38400,     27  },
        {  100,      57600,     12  },
        {  100,     115200,      6  },
        {  100,     307200,     20  },

        {  125,       1200,     30  },
        {  125,       2400,     15  },
        {  125,       4800,     22  },
        {  125,       9600,     22  },
        {  125,      19200,     11  },
        {  125,      28800,     17  },
        {  125,      33600,     29  },
        {  125,      38400,     29  },
        {  125,      57600,     17  },
        {  125,     115200,     17  },
        {  125,     307200,     25  },

        {  133,       1200,     32  },
        {  133,       2400,     28  },
        {  133,       4800,     28  },
        {  133,       9600,     28  },
        {  133,      19200,     14  },
        {  133,      28800,     17  },
        {  133,      33600,     31  },
        {  133,      38400,      7  },
        {  133,      57600,      5  },
        {  133,     115200,     24  },
        {  133,     307200,     27  },

        {  150,       1200,     13  },
        {  150,       2400,     21  },
        {  150,       4800,     21  },
        {  150,       9600,     16  },
        {  150,      19200,      8  },
        {  150,      28800,     13  },
        {  150,      33600,      9  },
        {  150,      38400,      7  },
        {  150,      57600,     18  },
        {  150,     115200,      9  },
        {  150,     307200,     30  },

        {  166,       1200,     20  },
        {  166,       2400,     20  },
        {  166,       4800,     10  },
        {  166,       9600,      5  },
        {  166,      19200,     32  },
        {  166,      28800,     19  },
        {  166,      33600,     31  },
        {  166,      38400,     17  },
        {  166,      57600,     18  },
        {  166,     115200,     18  },
        {  166,     307200,     17  },

        {  200,       1200,     11  },
        {  200,       2400,     28  },
        {  200,       4800,     28  },
        {  200,       9600,     14  },
        {  200,      19200,     31  },
        {  200,      28800,     31  },
        {  200,      33600,     31  },
        {  200,      38400,     25  },
        {  200,      57600,     31  },
        {  200,     115200,     12  },
        {  200,     307200,     20  },

        {  240,       1200,     25  },
        {  240,       2400,     25  },
        {  240,       4800,     25  },
        {  240,       9600,     22  },
        {  240,      19200,     11  },
        {  240,      28800,     26  },
        {  240,      33600,     32  },
        {  240,      38400,     23  },
        {  240,      57600,     26  },
        {  240,     115200,     26  },
        {  240,     307200,     24  },

        {  266,       1200,     19  },
        {  266,       2400,     32  },
        {  266,       4800,     31  },
        {  266,       9600,     31  },
        {  266,      19200,     31  },
        {  266,      28800,     17  },
        {  266,      33600,     31  },
        {  266,      38400,     31  },
        {  266,      57600,     17  },
        {  266,     115200,     29  },
        {  266,     307200,     27  },

        {  300,       1200,     25  },
        {  300,       2400,     13  },
        {  300,       4800,     21  },
        {  300,       9600,     21  },
        {  300,      19200,     16  },
        {  300,      28800,     31  },
        {  300,      33600,     18  },
        {  300,      38400,     14  },
        {  300,      57600,     13  },
        {  300,     115200,     18  },
        {  300,     307200,     31  }
    };

#endif

/******************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiescent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiescent state.  It is called from sysHwInit() with
* interrupts locked.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysHwInit()
*/
void sysSerialHwInit
    (
    void
    )
    {

    UINT32 sdrUart;
    UINT32 sdrPfc1;
    UINT   uart;
#ifndef UART_CLOCK_EXTERNAL
    UINT   clkDivisor;
    UINT   freqSerialSource;

    /*
     * In 440EP, the internal serial clock source is equal to the PLB
     * bus speed.
     */
    freqSerialSource = systemInfo.freqPLB;
#endif

    /*
     * Disable and clear serial interrupts in the UIC for both UARTs
     */

     for (uart=0; uart<NUM_TTY;uart++)
     {
         sdrUart = sysSdrInLong(SDR0_UART0+uart);

         switch (uart)
         {
           case 0: /* UART0 */
           /*
           * Disable and clear serial interrupts in the UIC for this UART
           */
           intDisable(INT_LVL_UART0);
           /*
           * initialize corresponding serial device descriptors
           */
           evbNs16550Chan0.int_vec = dev0Paras.vector;
           evbNs16550Chan0.channelMode = 0;
           evbNs16550Chan0.lcr =  UART0_REG(UART_LCR);
           evbNs16550Chan0.data = UART0_REG(UART_RDR);
           evbNs16550Chan0.brdl = UART0_REG(UART_BRDL);
           evbNs16550Chan0.brdh = UART0_REG(UART_BRDH);
           evbNs16550Chan0.ier =  UART0_REG(UART_IER);
           evbNs16550Chan0.iid =  UART0_REG(UART_IID);
           evbNs16550Chan0.mdc =  UART0_REG(UART_MDC);
           evbNs16550Chan0.lst =  UART0_REG(UART_LST);
           evbNs16550Chan0.msr =  UART0_REG(UART_MSR);

           evbNs16550Chan0.outByte = sysOutByte;
           evbNs16550Chan0.inByte  = sysInByte;

          /*
           * Set the SDR0_UARTx register for internal or external UART clock
           * source.
           */
           #ifdef UART_CLOCK_EXTERNAL

           sysSdrOutLong(SDR0_UART0+uart, (sdrUart | SDR0_UARTX_UXEC_EXT));
           evbNs16550Chan0.clkFreq = EXT_SER_CLK_FREQ;

           #else

          /*
           * PLB to UART clock divisor is determined and placed into SDR0_UARTx
           */

           sdrUart &= ~(SDR0_UARTX_UXDIV_MASK | SDR0_UARTX_UXEC_EXT);
           sysSerialCalcDivisor(freqSerialSource / 1000000, CONSOLE_BAUD_RATE,
                                    &clkDivisor);
           sdrUart = sdrUart | SDR0_UARTX_UXDIV_ENCODE(clkDivisor);
           sysSdrOutLong(SDR0_UART0+uart,sdrUart);

           evbNs16550Chan0.clkFreq = freqSerialSource / clkDivisor;

           #endif  /* UART_CLOCK_EXTERNAL */
           sdrPfc1 = sysSdrInLong(SDR0_PFC1);

           #if (NUM_TTY==1)
           {
            /* UART CONFIG 1 : 1 UART 8 pins */
            sdrPfc1 &= ~(SDR0_PFC1_U0IM_MASK );
            sdrPfc1 = sdrPfc1 | SDR0_PFC1_U0IM_8PINS;
           }
           #else
           {
            /* Set UART0 to work in DSR/DTR mode by turning off the CTS
             * enable bit in the Pin Function Control Register 1 (PFC1)
             */
            sdrPfc1 &= ~(SDR0_PFC1_U0ME_MASK | SDR0_PFC1_U0IM_MASK);
            sdrPfc1 = sdrPfc1 | SDR0_PFC1_U0ME_DSR | SDR0_PFC1_U0IM_4PINS;
           }
           #endif

           sysSdrOutLong(SDR0_PFC1,sdrPfc1);
          /*
           * reset current device
           */
           evbNs16550HrdInit(&evbNs16550Chan0);
           break;
           case 1:  /* UART1 */
           /*
           * Disable and clear serial interrupts in the UIC for this UART
           */
           intDisable(INT_LVL_UART1);

           /*
            * initialize corresponding serial device descriptors
            */
           evbNs16550Chan1.int_vec = dev1Paras.vector;
           evbNs16550Chan1.channelMode = 0;
           evbNs16550Chan1.lcr =  UART1_REG(UART_LCR);
           evbNs16550Chan1.data = UART1_REG(UART_RDR);
           evbNs16550Chan1.brdl = UART1_REG(UART_BRDL);
           evbNs16550Chan1.brdh = UART1_REG(UART_BRDH);
           evbNs16550Chan1.ier =  UART1_REG(UART_IER);
           evbNs16550Chan1.iid =  UART1_REG(UART_IID);
           evbNs16550Chan1.mdc =  UART1_REG(UART_MDC);
           evbNs16550Chan1.lst =  UART1_REG(UART_LST);
           evbNs16550Chan1.msr =  UART1_REG(UART_MSR);

           evbNs16550Chan1.outByte = sysOutByte;
           evbNs16550Chan1.inByte  = sysInByte;

          /*
           * Set the SDR0_UARTx register for internal or external UART clock
           * source.
           */
           #ifdef UART_CLOCK_EXTERNAL

           sysSdrOutLong(SDR0_UART0+uart, (sdrUart | SDR0_UARTX_UXEC_EXT));
           evbNs16550Chan1.clkFreq = EXT_SER_CLK_FREQ;

           #else

          /*
           * PLB to UART clock divisor is determined and placed into SDR0_UARTx
           */

           sdrUart &= ~(SDR0_UARTX_UXDIV_MASK | SDR0_UARTX_UXEC_EXT);
           sysSerialCalcDivisor(freqSerialSource / 1000000, CONSOLE_BAUD_RATE,
                                    &clkDivisor);
           sdrUart = sdrUart | SDR0_UARTX_UXDIV_ENCODE(clkDivisor);
           sysSdrOutLong(SDR0_UART0+uart,sdrUart);

           evbNs16550Chan1.clkFreq = freqSerialSource / clkDivisor;

           #endif  /* UART_CLOCK_EXTERNAL */

          /* Set UART1 to work in CTS/RTS mode by turning on the CTS
           * enable bit in the Pin Function Control Register 1 (PFC1)
           */
           sdrPfc1 = sysSdrInLong(SDR0_PFC1);
           sdrPfc1 &= ~(SDR0_PFC1_U1ME_MASK);
           sdrPfc1 = sdrPfc1 | SDR0_PFC1_U1ME_CTS;
           sysSdrOutLong(SDR0_PFC1,sdrPfc1);

           /*
           * reset current device
           */

           evbNs16550HrdInit(&evbNs16550Chan1);
           break;
           case 2:  /* UART 2 */
           /*
            * Disable and clear serial interrupts in the UIC for this UART
            */
            intDisable(INT_LVL_UART2);
            /*
            * initialize corresponding serial device descriptors
            */
            evbNs16550Chan2.int_vec = dev2Paras.vector;
            evbNs16550Chan2.channelMode = 0;
            evbNs16550Chan2.lcr =  UART2_REG(UART_LCR);
            evbNs16550Chan2.data = UART2_REG(UART_RDR);
            evbNs16550Chan2.brdl = UART2_REG(UART_BRDL);
            evbNs16550Chan2.brdh = UART2_REG(UART_BRDH);
            evbNs16550Chan2.ier =  UART2_REG(UART_IER);
            evbNs16550Chan2.iid =  UART2_REG(UART_IID);
            evbNs16550Chan2.mdc =  UART2_REG(UART_MDC);
            evbNs16550Chan2.lst =  UART2_REG(UART_LST);
            evbNs16550Chan2.msr =  UART2_REG(UART_MSR);

            evbNs16550Chan2.outByte = sysOutByte;
            evbNs16550Chan2.inByte  = sysInByte;

           /*
            * Set the SDR0_UARTx register for internal or external UART clock
            * source.
            */
            #ifdef UART_CLOCK_EXTERNAL

            sysSdrOutLong(SDR0_UART0+uart, (sdrUart | SDR0_UARTX_UXEC_EXT));
            evbNs16550Chan2.clkFreq = EXT_SER_CLK_FREQ;

            #else

           /*
            * PLB to UART clock divisor is determined and placed into SDR0_UARTx
            */

            sdrUart &= ~(SDR0_UARTX_UXDIV_MASK | SDR0_UARTX_UXEC_EXT);
            sysSerialCalcDivisor(freqSerialSource / 1000000, CONSOLE_BAUD_RATE,
                                     &clkDivisor);
            sdrUart = sdrUart | SDR0_UARTX_UXDIV_ENCODE(clkDivisor);
            sysSdrOutLong(SDR0_UART0+uart,sdrUart);

            evbNs16550Chan2.clkFreq = freqSerialSource / clkDivisor;

            #endif  /* UART_CLOCK_EXTERNAL */
           /*
            * reset current device
            */
           evbNs16550HrdInit(&evbNs16550Chan2);
           break;
           case 3:
           /*
            * Disable and clear serial interrupts in the UIC for this UART
            */
            intDisable(INT_LVL_UART3);
            /*
            * initialize corresponding serial device descriptors
            */
            evbNs16550Chan3.int_vec = dev3Paras.vector;
            evbNs16550Chan3.channelMode = 0;
            evbNs16550Chan3.lcr =  UART3_REG(UART_LCR);
            evbNs16550Chan3.data = UART3_REG(UART_RDR);
            evbNs16550Chan3.brdl = UART3_REG(UART_BRDL);
            evbNs16550Chan3.brdh = UART3_REG(UART_BRDH);
            evbNs16550Chan3.ier =  UART3_REG(UART_IER);
            evbNs16550Chan3.iid =  UART3_REG(UART_IID);
            evbNs16550Chan3.mdc =  UART3_REG(UART_MDC);
            evbNs16550Chan3.lst =  UART3_REG(UART_LST);
            evbNs16550Chan3.msr =  UART3_REG(UART_MSR);

            evbNs16550Chan3.outByte = sysOutByte;
            evbNs16550Chan3.inByte  = sysInByte;

           /*
            * Set the SDR0_UARTx register for internal or external UART clock
            * source.
            */
            #ifdef UART_CLOCK_EXTERNAL

            sysSdrOutLong(SDR0_UART0+uart, (sdrUart | SDR0_UARTX_UXEC_EXT));
            evbNs16550Chan3.clkFreq = EXT_SER_CLK_FREQ;

            #else

           /*
            * PLB to UART clock divisor is determined and placed into SDR0_UARTx
            */

            sdrUart &= ~(SDR0_UARTX_UXDIV_MASK | SDR0_UARTX_UXEC_EXT);
            sysSerialCalcDivisor(freqSerialSource / 1000000, CONSOLE_BAUD_RATE,
                                     &clkDivisor);
            sdrUart = sdrUart | SDR0_UARTX_UXDIV_ENCODE(clkDivisor);
            sysSdrOutLong(SDR0_UART0+uart,sdrUart);

            evbNs16550Chan3.clkFreq = freqSerialSource / clkDivisor;

            #endif  /* UART_CLOCK_EXTERNAL */
           /*
            * reset current device
            */
           evbNs16550HrdInit(&evbNs16550Chan3);
           break;
        }

    }

 }

/***************************************************************************
*
* sysSerialHwInit2 - connect BSP serial device interrupts
*
* This routine connects the BSP serial device interrupts.  It is called from
* sysHwInit2().
*
* Serial device interrupts cannot be connected in sysSerialHwInit() because
* the kernel memory allocator is not initialized at that point, and
* intConnect() calls malloc().
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysHwInit2()
*/
void sysSerialHwInit2
    (
    void
    )
    {
    int intLevel = intLock ();
    int uartNum;

    for (uartNum=0;uartNum<NUM_TTY;uartNum++)
    {
    switch (uartNum)
    {
      case 0 :
      /*
       * Connect serial interrupt handlers for S0 (UART 0) and enable
       * serial interrupts for S0
       */
      (void) intConnect (INUM_TO_IVEC(INT_VEC_UART0), evbNs16550Int,
                     (int) sysSerialChanGet(uartNum));
      intEnable (INT_LVL_UART0);
      break;

      case 1 :
      /*
       * connect serial interrupt handlers for S1 (UART 1) and enable
       * serial interrupts for S1
       */
      (void) intConnect (INUM_TO_IVEC(INT_VEC_UART1), evbNs16550Int,
                     (int) sysSerialChanGet(uartNum));
      intEnable (INT_LVL_UART1);
      break;

      case 2 :
      /*
       * connect serial interrupt handlers for S2 (UART 2) and enable
       * serial interrupts for S2
       */
      (void) intConnect (INUM_TO_IVEC(INT_VEC_UART2), evbNs16550Int,
                     (int) sysSerialChanGet(uartNum));
      intEnable (INT_LVL_UART2);
      break;

      case 3 :
      /*
       * connect serial interrupt handlers for S3 (UART 3) and enable
       * serial interrupts for S3
       */
      (void) intConnect (INUM_TO_IVEC(INT_VEC_UART3), evbNs16550Int,
                     (int) sysSerialChanGet(uartNum));
      intEnable (INT_LVL_UART3);
      break;

    }
  }

  intUnlock (intLevel);
}

/***************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine returns a pointer to the SIO_CHAN device associated with
* a specified serial channel. It is called by usrRoot() to obtain
* pointers when creating the system serial devices, `/tyCo/x'. It is
* also used by the WDB agent to locate its serial channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*
* ERRNO
*
*/
SIO_CHAN * sysSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    switch (channel)
        {
        case 0:                                 /* S0 */
            return ((SIO_CHAN *)&evbNs16550Chan0.pDrvFuncs);

        case 1:                                 /* S1 */
            return ((SIO_CHAN *)&evbNs16550Chan1.pDrvFuncs);

        case 2:                                 /* S2 */
            return ((SIO_CHAN *)&evbNs16550Chan2.pDrvFuncs);

        case 3:                                 /* S3 */
            return ((SIO_CHAN *)&evbNs16550Chan3.pDrvFuncs);

        default:
            return ((SIO_CHAN *)ERROR);
        }
    }

#ifndef UART_CLOCK_EXTERNAL
/***************************************************************************
*
* sysSerialCalcDivisor - determines PLB clock to UART clock divisor
*
* This routine determines the optimal PLB clock to UART clock divisor
* for a given PLB frequency and desired baud rate. This function is
* required only when the serial clock is being derived from the PLB
* clock.
*
* RETURNS: The optimal PLB to UART clock divisor, or ERROR
*
* ERRNO
*
*/
STATUS sysSerialCalcDivisor
    (
    UINT freqSourceMhz,
    UINT baudRate,
    UINT *divisor
    )
    {

    int numEntries;
    int i;

    numEntries = sizeof(uartClkTbl) / sizeof(INT_SERIAL_INFO);

    /*
     * Search the table of processor frequencies and Baud rates to determine
     * the optimal divisor for the UART clock speed
     */

    for (i=0; i < numEntries; i++)
        {
        if ((uartClkTbl[i].freqSourceMhz == freqSourceMhz) &&
            (uartClkTbl[i].baudRate == baudRate))
            {
            *divisor = uartClkTbl[i].uartClkDivisor;
            break;
            }
        }

    if (i == numEntries)             /* not found */
        return(ERROR);

    return(OK);

    }

#endif
