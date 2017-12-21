/* romI2cDrv.c - I2C Driver to support boot rom */

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

/* Copyright 2005 Wind River Systems, Inc.  All rights reserved. */

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

********************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01e,26jan05,dr   Add error log on Hex display
01d,30aug04,mdo  Documentation fixes for apigen
01c,12may04,j_b  fix compiler warnings
01b,10jul03,jtp  Share with other ppc440 bsps
01a,12jul02,jtp  written based on IBM OpenBIOS mainLib/iic.c
*/

/*
DESCRIPTION

This file contains generic functions to read an I2C device.  It is used
by romInit to read Serial Presence Detect devices present on the DDR
SDRAM DIMMs.  This library only supports IIC0 and those functions
essential to supporting romInit.

Since the only memory facility available to these routines is the stack
(initialized to the PPC440's onchip SRAM), they are written to avoid use
of any data or BSS memory.

INCLUDE FILES:
*/


/* includes */

#include "vxWorks.h"
#include "config.h"

/* defines */

/* make spin use displayProg if it exists */
#ifdef DISPLAY_PROG
#define SPIN(x)      for (;;) { displayProg(x);}  /* Spin loop */
#else
#define SPIN(x)      for (;;) {} /* spin loop */
#endif

#define MAX_CPU_CLK_RATE  533333333   /* 533 MHz */
#define WAIT_1_SEC    MAX_CPU_CLK_RATE  /*   1 s  */
#define WAIT_167_MSECS    MAX_CPU_CLK_RATE/6  /* 167 ms */
#define WAIT_10_MSECS   MAX_CPU_CLK_RATE/100  /*  10 ms */

/* define hex code to display in this module */
#define IE01 0x49453031     /* IIC error 01: (busyWait) IIC operation timed out */
#define IE02 0x49453032     /* IIC error 02: (romI2cReadByte) IIC STS ERROR after STS init */
#define IE03 0x49453033     /* IIC error 03: (romI2cReadByte) IIC STS ERROR after STS init 2 */
#define IE04 0x49453034     /* IIC error 04: (romI2cReadByte) IIC STS ERROR after 1 byte read */
#define IE05 0x49453035     /* IIC error 05: (romI2cWriteBytes) IIC STS ERROR after STS init */
#define IE06 0x49453036     /* IIC error 06: (romI2cWriteBytes) IIC STS ERROR after buffer write */


/* externals */

IMPORT UINT32 timeBaseGet(void);
#ifdef DISPLAY_PROG
IMPORT void displayProg (UINT32 data);
#endif

/* locals */


/***************************************************************************
*
* busyWait - wait for bus idle
*
* Examines the state of the IIC0 STS register repeatedly until the busy
* bit turns off or the requested number of timer clocks expires.
*
* RETURNS: OK, or ERROR on error, and never returns on timeout
*
* ERRNO
*/

LOCAL STATUS busyWait
    (
    UINT32  numClocks       /* timebase counts until timeout */
    )
    {
    volatile UINT8 * pBase; /* pointer to IIC0 registers */
    UINT32  timeBaseStart;  /* beginning timebase counter */
    UINT8   status;         /* current IIC operation status */

    /* initialize base pointer */

    pBase = (volatile UINT8 *)IIC0_BASE;

    /* start the counter */

    timeBaseStart = timeBaseGet();

    /* loop until status completes or timeout */

    do
        {
        EIEIO;
        status = pBase[IIC_STS];
        if ((status & IIC_STS_PT) == 0x00)
            {
            if ((status & IIC_STS_ERR) == IIC_STS_ERR)
                return(ERROR);
            else
                return(OK);
            }
        }
    while ((timeBaseGet() - timeBaseStart) < numClocks);

    SPIN(IE01); /* IIC operation timed out */
    }


/***************************************************************************
*
* romI2cInit - initialize i2c bus drivers
*
* This function prepares the PPC440's onchip I2C device for use.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

void romI2cInit (void)
    {
    volatile UINT8 * pBase;     /* pointer to IIC0 registers */

    /* initialize register base */

    pBase = (volatile UINT8 *)IIC0_BASE;

    pBase[IIC_LMADR] = 0x00;    /* clear lo & hi master and slave addresses */
    EIEIO;
    pBase[IIC_HMADR] = 0x00;
    EIEIO;
    pBase[IIC_LSADR] = 0x00;
    EIEIO;
    pBase[IIC_HSADR] = 0x00;
    EIEIO;
    pBase[IIC_STS] = IIC_STS_SCMP | IIC_STS_IRQA;
    EIEIO;
    pBase[IIC_EXTSTS] = IIC_EXTSTS_IRQP | IIC_EXTSTS_IRQD | IIC_EXTSTS_LA |
            IIC_EXTSTS_ICT | IIC_EXTSTS_XFRA;
    EIEIO;
    pBase[IIC_CLKDIV] = 0x04;   /* set clock divisor OPB 41-50Mhz -> 4 */
    EIEIO;
    pBase[IIC_INTRMSK] = 0x00;  /* no interrupts */
    EIEIO;
    pBase[IIC_XFRCNT] = 0x00;
    EIEIO;
    pBase[IIC_XTCNTLSS] = IIC_XTCNTLSS_SRC | IIC_XTCNTLSS_SRS |
              IIC_XTCNTLSS_SWC | IIC_XTCNTLSS_SWS;
    EIEIO;
    pBase[IIC_MDCNTL] = IIC_MDCNTL_FMDB | IIC_MDCNTL_EUBS | IIC_MDCNTL_HSCL;
    EIEIO;

    }

/***************************************************************************
*
* romI2cReadByte - Read a byte from an i2c device
*
* This function's purpose is to read a byte at an address from the
* specified device.
*
* Note i2c combined format read requires a write of the address we want
* to read, before doing the read.
*
* RETURNS: -1 on bad request, otherwise, the character read.
*
* ERRNO
*/

int romI2cReadByte
    (
    UINT32  deviceAdrs,     /* i2c bus address of device */
    UINT32  byteAdrs        /* byte address on device */
    )
    {
    volatile UINT8 * pBase;     /* pointer to IIC0 registers */
    UINT8 tmp;                  /* scratch area */

    /* initialize register base */

    pBase = (volatile UINT8 *)IIC0_BASE;

    pBase[IIC_STS] = IIC_STS_SCMP | IIC_STS_IRQA;
    if (busyWait(WAIT_167_MSECS) == ERROR)
  SPIN(IE02) ; /* IIC STS ERROR after STS init */

    pBase[IIC_MDCNTL] |= IIC_MDCNTL_FMDB; /* flush master data buffer */
    EIEIO;
    pBase[IIC_LMADR] = deviceAdrs;  /* set device address */
    EIEIO;
    pBase[IIC_MDBUF] = byteAdrs;    /* put byte address in fifo */
    EIEIO;
    pBase[IIC_CNTL] = IIC_CNTL_PT;  /* write 1 byte */
    if (busyWait(WAIT_167_MSECS) == ERROR)
        {
        tmp = pBase[IIC_EXTSTS];    /* clear error */
        pBase[IIC_EXTSTS] = tmp;
        return(-1);                 /* probably, device doesn't exist */
        }

    pBase[IIC_STS] = IIC_STS_SCMP | IIC_STS_IRQA;
    if (busyWait(WAIT_167_MSECS) == ERROR)
        SPIN(IE03); /* IIC STS ERROR after STS init 2 */

    pBase[IIC_MDCNTL] |= IIC_MDCNTL_FMDB;
    EIEIO;
    pBase[IIC_LMADR] = deviceAdrs + 0x01;
    EIEIO;
    pBase[IIC_CNTL] = IIC_CNTL_RW | IIC_CNTL_PT; /* read 1 byte */
    if (busyWait(WAIT_167_MSECS) == ERROR)
  SPIN(IE04); /* IIC STS ERROR after 1 byte read */

    return(pBase[IIC_MDBUF]);
    }


/***************************************************************************
*
* romI2cWriteBytes - Write several bytes to an i2c device
*
* This function writes several bytes at a supplied address.
*
* RETURNS: void
*
* ERRNO
*/

void romI2cWriteBytes
    (
    UINT32  deviceAdrs,     /* i2c bus address of device */
    UINT32  byteAdrs,       /* byte address on device */
    UINT32  nBytes,         /* number of bytes of data */
    UINT8   *pData          /* pointer to data array */
    )
    {
    volatile UINT8 * pBase; /* pointer to IIC0 registers */
    int     xferCount;      /* count of bytes transferred */
    int     dataCount;      /* count of data bytes xfered */
    UINT8   iicCntlReg;     /* control register value */

    pBase = (volatile UINT8 *)IIC0_BASE;

    pBase[IIC_STS] = IIC_STS_SCMP | IIC_STS_IRQA;
    if (busyWait(WAIT_1_SEC) == ERROR)
  SPIN(IE05); /* IIC STS ERROR after STS init */

    pBase[IIC_MDCNTL] |= IIC_MDCNTL_FMDB;   /* flush master data buf */
    EIEIO;
    pBase[IIC_LMADR] = deviceAdrs;          /* set device address */
    EIEIO;
    pBase[IIC_MDBUF] = byteAdrs;            /* put byte address in fifo */
    EIEIO;

    /*
     * We just stuffed byteAdrs into the IIC FIFO.
     * Fill up the rest of the FIFO with data bytes.
     */

    dataCount = max(nBytes, IIC_MDBUF_FIFO_SIZE - 1);
    xferCount = dataCount + 1;

    do
        {
        nBytes -= dataCount;

        /*
         * Prepare the IIC Control Register.
         * Set the transfer start bit, the count of bytes to transfer,
         * and the chain bit if there is more data to come.
         */

        iicCntlReg = IIC_CNTL_PT;
        iicCntlReg |= ((xferCount - 1) << IIC_CNTL_TCT_SHIFT);
        if (nBytes > 0)
            iicCntlReg |= IIC_CNTL_CHT;

        /*
         * Fill up the (rest) of the MDBUF FIFO with characters to
         * transmit
         */

        while (dataCount-- > 0)
            {
            pBase[IIC_MDBUF] = *(pData++);
            EIEIO;
            }

        /*
         * Write the buffer out
         */

        pBase[IIC_CNTL] = iicCntlReg;

        if (busyWait(WAIT_1_SEC) == ERROR)
            SPIN(IE06); /* IIC STS ERROR after buffer write */

        /*
         * Set up for next transfer
         */

        dataCount = max(nBytes, IIC_MDBUF_FIFO_SIZE);
        xferCount = dataCount;
        }
    while (nBytes > 0);

    }
