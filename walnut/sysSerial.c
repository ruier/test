/* sysSerial.c - IBM 405GP serial device initialization */

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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01g,30aug04,mdo  Documentation fixes for apigen
01f,26jun02,mcg  Add CPU:UART divisor values for CPU speeds of 333 and 400MHz.
                 Update divisor values for 266MHz.
01e,11jan02,pch  merge in wrSbc405gp version of file, make common
01d,19nov01,pch  cleanup
01c,25may00,mcg  UART clock derived from CPU clock support added
01b,15may00,mcg  register name updates to match 405GP User Manual
01a,05mar99,mcg  created from Spruce sysSerial.c ver 01a
*/

/*
DESCRIPTION
This library contains board-specific routines for serial devices.
There are two serial ports integrated into the IBM 405GP.  These
devices are 16550-like. The evbNs16550Sio driver is used for both devices.
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
LOCAL EVBNS16550_CHAN_PARAS dev0Paras =
      {0,UART0_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART0};

LOCAL EVBNS16550_CHAN_PARAS dev1Paras =
      {0,UART1_BASE,UART_REG_ADDR_INTERVAL,INT_LVL_UART1};

#define UART0_REG(reg)   (dev0Paras.baseAdrs + reg*dev0Paras.regSpace)
#define UART1_REG(reg)   (dev1Paras.baseAdrs + reg*dev1Paras.regSpace)

LOCAL EVBNS16550_CHAN  evbNs16550Chan0;
LOCAL EVBNS16550_CHAN  evbNs16550Chan1;


#ifndef UART_CLOCK_EXTERNAL

/* forward declarations */

STATUS sysSerialCalcDivisor(UINT freqProcMhz, UINT baudRate, UINT *divisor );

/* Structure for internal serial clock array */

typedef struct
    {
    UINT freqProcMhz;
    UINT baudRate;
    UINT uartClkDivisor;
    } INT_SERIAL_INFO;

/*
 * Table of CPU speeds and baud rates used to determine the CPU:UART divisor.
 * The CPU clock is divided by the CPU:UART divisor to get the UART clock which
 * is fed into the UARTs.  The UART clock is then divided again by the UART to
 * obtain the baud rate.  The table of CPU:UART divisors below helps to select
 * a value that will result in the lowest % error on the baud rate.
 * NOTE:
 * The CPU:UART divisor must be high enough so the resulting UART clock is
 * less than 1/2 of the OPB bus frequency.
 */

INT_SERIAL_INFO uartClkTbl[] =
    {
      /* CPU Freq     Baud   CPU:UART divisor */

        {  133,       1200,     15  },
        {  133,       2400,     16  },
        {  133,       4800,     28  },
        {  133,       9600,     14  },
        {  133,      19200,     14  },
        {  133,      28800,     17  },
        {  133,      33600,     31  },
        {  133,      38400,     31  },
        {  133,      57600,     29  },
        {  133,     115200,     12  },
        {  133,     307200,     27  },

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

        {  266,       1200,     19  },
        {  266,       2400,     31  },
        {  266,       4800,     28  },
        {  266,       9600,     28  },
        {  266,      19200,     28  },
        {  266,      28800,     29  },
        {  266,      33600,     31  },
        {  266,      38400,     31  },
        {  266,      57600,     29  },
        {  266,     115200,     29  },
        {  266,     307200,     27  },

        {  333,       1200,     27  },
        {  333,       2400,     31  },
        {  333,       4800,     31  },
        {  333,       9600,     31  },
        {  333,      19200,     31  },
        {  333,      28800,     29  },
        {  333,      33600,     31  },
        {  333,      38400,     17  },
        {  333,      57600,     30  },
        {  333,     115200,     30  },
        {  333,     307200,     17  },

        {  400,       1200,     22  },
        {  400,       2400,     31  },
        {  400,       4800,     31  },
        {  400,       9600,     31  },
        {  400,      19200,     31  },
        {  400,      28800,     31  },
        {  400,      33600,     31  },
        {  400,      38400,     31  },
        {  400,      57600,     31  },
        {  400,     115200,     31  },
        {  400,     307200,     27  }

    };

#endif /* UART_CLOCK_EXTERNAL */

/***********************************************************************
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

    UINT32 cr0;
    #ifndef UART_CLOCK_EXTERNAL
    UINT   clkDivisor;
    #endif /* UART_CLOCK_EXTERNAL */

    /*
     * Disable and clear serial interrupts in the UIC for both UARTs
     */
    intDisable (INT_LVL_UART0);
    intDisable (INT_LVL_UART1);

    /*
     * initialize serial device 0 descriptors (S0)
     */

    evbNs16550Chan0.int_vec = dev0Paras.vector;
    evbNs16550Chan0.channelMode = 0;
    evbNs16550Chan0.lcr  = UART0_REG(UART_LCR);
    evbNs16550Chan0.data = UART0_REG(UART_RDR);
    evbNs16550Chan0.brdl = UART0_REG(UART_BRDL);
    evbNs16550Chan0.brdh = UART0_REG(UART_BRDH);
    evbNs16550Chan0.ier  = UART0_REG(UART_IER);
    evbNs16550Chan0.iid  = UART0_REG(UART_IID);
    evbNs16550Chan0.mdc  = UART0_REG(UART_MDC);
    evbNs16550Chan0.lst  = UART0_REG(UART_LST);
    evbNs16550Chan0.msr  = UART0_REG(UART_MSR);

    evbNs16550Chan0.outByte = sysOutByte;
    evbNs16550Chan0.inByte  = sysInByte;

    /*
     * initialize serial device 1 descriptors (S1)
     */

    evbNs16550Chan1.int_vec = dev1Paras.vector;
    evbNs16550Chan1.channelMode = 0;
    evbNs16550Chan1.lcr  = UART1_REG(UART_LCR);
    evbNs16550Chan1.data = UART1_REG(UART_RDR);
    evbNs16550Chan1.brdl = UART1_REG(UART_BRDL);
    evbNs16550Chan1.brdh = UART1_REG(UART_BRDH);
    evbNs16550Chan1.ier  = UART1_REG(UART_IER);
    evbNs16550Chan1.iid  = UART1_REG(UART_IID);
    evbNs16550Chan1.mdc  = UART1_REG(UART_MDC);
    evbNs16550Chan1.lst  = UART1_REG(UART_LST);
    evbNs16550Chan1.msr  = UART1_REG(UART_MSR);

    evbNs16550Chan1.outByte = sysOutByte;
    evbNs16550Chan1.inByte  = sysInByte;

    /*
     * Set the CPC0_CR0 register for internal or external UART clock source.
     * Set UART1 to work in CTS/RTS mode by turning on the CTS enable bit
     * in the Chip Control Reg 0 plus setting the DEFAULT_UART_1 control
     * bit in the Walnut board's FPGA_BRDC register.  The FPGA routes the
     * correct signal from the connector to the chip.  Other boards sharing
     * this file (wrSbc405gp) may not implement FPGA_BRDC.
     */

#ifdef UART_CLOCK_EXTERNAL
    cr0 = sysDcrCr0Get ();
    sysDcrCr0Set (cr0 | CR0_UART1_CTS_EN
                      | CR0_UART0_EXT_CLK | CR0_UART1_EXT_CLK);
    evbNs16550Chan0.clkFreq = EXT_SER_CLK_FREQ;
    evbNs16550Chan1.clkFreq = EXT_SER_CLK_FREQ;
#else	/* UART_CLOCK_EXTERNAL */
    /* CPU to UART clock divisor is determined and placed into CPC0_CR0 */
    cr0 = sysDcrCr0Get ();
    cr0 &= ~(CR0_UART_DIV_MASK | CR0_UART0_EXT_CLK | CR0_UART1_EXT_CLK );
    sysSerialCalcDivisor (systemInfo.freqProcessor / 1000000,
                          CONSOLE_BAUD_RATE,
                          &clkDivisor);
    sysDcrCr0Set (cr0 | CR0_UART1_CTS_EN | ((clkDivisor - 1) << 1));
    evbNs16550Chan0.clkFreq = systemInfo.freqProcessor / clkDivisor;
    evbNs16550Chan1.clkFreq = systemInfo.freqProcessor / clkDivisor;
#endif /* UART_CLOCK_EXTERNAL */

#if defined(FPGA_BRDC) && defined(FPGA_BRDC_DEF_UART1)
    sysOutByte(FPGA_BRDC,(sysInByte(FPGA_BRDC) | FPGA_BRDC_DEF_UART1));
#endif	/* FPGA_BRDC && FPGA_BRDC_DEF_UART1 */

    /*
     * reset both devices
     */
    evbNs16550HrdInit (&evbNs16550Chan0);
    evbNs16550HrdInit (&evbNs16550Chan1);
    }

/***********************************************************************
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

    /*
     * Connect serial interrupt handlers for S0 (UART 0)
     * and enable serial interrupts for S0
     */
    (void) intConnect (INUM_TO_IVEC(INT_VEC_UART0), evbNs16550Int,
                       (int) sysSerialChanGet(0));
    intEnable (INT_LVL_UART0);

    /*
     * connect serial interrupt handlers for S1 (UART 1)
     * and enable serial interrupts for S1
     */
    (void) intConnect (INUM_TO_IVEC(INT_VEC_UART1), evbNs16550Int,
                       (int) sysSerialChanGet(1));
    intEnable (INT_LVL_UART1);

    intUnlock (intLevel);
    }

/***********************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine returns a pointer to the SIO_CHAN device associated
* with a specified serial channel.  It is called by usrRoot() to obtain
* pointers when creating the system serial devices, `/tyCo/x'.  It
* is also used by the WDB agent to locate its serial channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*
* ERRNO
*/
SIO_CHAN * sysSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    switch (channel)
        {
        case 0:                                 /* S0 */
            return ((SIO_CHAN *)&evbNs16550Chan0);

        case 1:                                 /* S1 */
            return ((SIO_CHAN *)&evbNs16550Chan1);

        default:
            return ((SIO_CHAN *)ERROR);
        }
    }

#ifndef UART_CLOCK_EXTERNAL
/***********************************************************************
*
* sysSerialCalcDivisor - determines CPU clock to UART clock divisor
*
* This routine determines the optimal CPU clock to UART clock divisor for a
* given CPU frequency and desired baud rate.  This function is required only
* when the serial clock is being derived from the CPU clock.
*
* RETURNS: The optimal CPU to UART clock divisor, or ERROR
*
* ERRNO
*/
STATUS sysSerialCalcDivisor
    (
    UINT freqProcMhz,
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
        if ((uartClkTbl[i].freqProcMhz == freqProcMhz) &&
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

#endif /* UART_CLOCK_EXTERNAL */
