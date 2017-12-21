/* sysLsiSpi.c - LSI ACP34xx ELPASO SPI driver */

/*
* Copyright (c) 2010, 2011 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01c,09oct11,x_s  Correct wrong code for waitting time calculation. (WIND00310430)
01b,12feb11,x_s  Reduce waitting time for operating SPI controller.
01a,01dec10,x_s  Writen.
*/

/*
DESCRIPTION
This is a driver for the ARM PrimeCell Synchronous Serial Port(PL1022), which
includes three features: SPI feature, Microwire feature and Texas Instruments
synchronous serial interface feature. In this driver, only SPI feature with 
polling mode is supported. It is intended for use by client drivers performing 
other high level functions.

INCLUDE FILES: tickLib.h config.h

SEE ALSO:
\tb "ARM PrimeCell Synchronous Serial Port(PL1022)"
*/

/* includes */

#include <vxWorks.h>
#include <tickLib.h>
#include "config.h"

/* defines */

#define LSI_SSP_TIMEOUT     5  /* timeout(seconds) for data transfer */
#define LSI_SPI_REG_WRITE(addr, value)  sysOutLong(addr, value)
#define LSI_SPI_REG_READ(addr)          sysInWord(addr)

/* locals */

LOCAL UINT32 sysSpiChipSel; /* chip device selection number */

/*******************************************************************************
*
* sysSpiInit - Lsi SPI controller initialization
*
* The purpose of this function is to perform SSP timer and SPI controller
* initialization
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSpiInit (void)
    {

    /* Set the SSP timer to provide clock for SSP controller */

    LSI_SPI_REG_WRITE(SSP_TIMER_BA + AXIS_TIMER_CTRL, 0);

    /*
     * The value 0x1 is set indicating that peripheral clock is used directly for
     * SSP controller, no extra clock divison from peripheral clock frequency.
     */

    LSI_SPI_REG_WRITE(SSP_TIMER_BA + AXIS_TIMER_LOAD_VAL, 0x1);
    LSI_SPI_REG_WRITE(SSP_TIMER_BA + AXIS_TIMER_CTRL, (AXIS_TIMER_EN |
                                                       AXIS_TIMER_MODE_PERIOD));

    /* Initialize the SSP controller */

    LSI_SPI_REG_WRITE(SSP_SSPCR0, (SSP_SSPCR0_SCR_VALUE |  /* clock rate */
                                   SSP_SSPCR0_FPF_MOTO  |  /* frame mode */
                                   SSP_SSPCR0_DSS(8)));    /* data size*/
    LSI_SPI_REG_WRITE(SSP_SSPCR1, SSP_SSPCR1_SSE_ENABLE);
    LSI_SPI_REG_WRITE(SSP_SSPCPSR, SSP_SSPCPSR_CPSDVSR);
    LSI_SPI_REG_WRITE(SSP_SSPCSR, SSP_SSPCSR_CHIP_SEL_MASK);

    /* Select chip device, by default the chip 1 is selected */

    sysSpiChipSel = 1;

    return;
    }

/*******************************************************************************
*
* sysSpiChipSelect - select some chip device
*
* The purpose of this function is to select some chip device to attach the SPI
* controller.
*
* RETURNS:  N/A.
*
* ERRNO
*/

void sysSpiChipSelect
    (
    BOOL    select       /*1 - selected, 0 - not selected */
    )
    {
    UINT32 tmpCsrVal;

    if (select)
        tmpCsrVal = SSP_SSPCSR_CHIP_SEL_MASK & ~ (1 << sysSpiChipSel);
    else
        tmpCsrVal = SSP_SSPCSR_CHIP_SEL_MASK;  /* deselcet all */

    LSI_SPI_REG_WRITE(SSP_SSPCSR, tmpCsrVal);

    return;
    }

/*******************************************************************************
*
* sysSpiSendCmd - send a command to SPI device
*
* The purpose of this function is to write SPI controll command to specifical
* device on SPI bus.
*
* RETURNS: OK, or ERROR if the operation fails.
*
* ERRNO
*/

STATUS sysSpiSendCmd
    (
    UINT32  command     /* command to SPI device */
    )
    {
    STATUS status = OK;
    UINT32 tickSave;
    UINT32 timeoutTicks = LSI_SSP_TIMEOUT * sysClkRateGet ();

    /* Send the command to the device. */

    LSI_SPI_REG_WRITE(SSP_SSPDR, command);
  
    /*
     * Get the response from slave device, note tickGet could NOT be executed 
     * in the ISR or before the system clock is set up and running
     */

    tickSave = tickGet ();

    while (0 == (LSI_SPI_REG_READ(SSP_SSPSR) & SSP_SSPSR_REFIFO_NOT_EMPTY))
        {
        if ((tickGet () - tickSave) > timeoutTicks)
            {
            status = ERROR;
            break;
            }
        }

    /* Read clear the data */

    LSI_SPI_REG_READ(SSP_SSPDR);

    return (status);
    }

/*******************************************************************************
*
* sysSpiRead - read data from SPI recieve FIFO on poll mode
*
* The purpose of this function is to read data from SPI recieve FIFO to dest
* address.
*
* RETURNS: OK, or ERROR if the operation fails.
*
* ERRNO
*/

STATUS sysSpiRead
    (
    UINT8 * destAddr,   /* dest address to save data readed from FIFO */
    INT32   dataNum     /* data number to be read ftom FIFO */
    )
    {
    STATUS status = OK;
    UINT32 tickSave;
    UINT32 timeoutTicks = LSI_SSP_TIMEOUT * sysClkRateGet ();

    while (dataNum > 0)
        {
        /* Trigger one reading */
        
        LSI_SPI_REG_WRITE(SSP_SSPDR, 0);
  
        /*
         * Get the response from slave device, note tickGet could NOT be executed 
         * in the ISR or before the system clock is set up and running
         */

        tickSave = tickGet ();

        while (0 == (LSI_SPI_REG_READ(SSP_SSPSR) & SSP_SSPSR_REFIFO_NOT_EMPTY))
            {
            if ((tickGet () - tickSave) > timeoutTicks)
                {
                status = ERROR;
                break;
                }
            }
        
        *destAddr = (LSI_SPI_REG_READ(SSP_SSPDR) & SSP_SSPCR0_DATA_MASK_8);
    
        destAddr ++;
        dataNum --;
        }

    return (status);
    }

/*******************************************************************************
*
* sysSpiWriteData - write data to SPI device on poll mode
*
* The purpose of this function is to write data to SPI device on poll mode.
*
* RETURNS: OK, or ERROR if the operation fails.
*
* ERRNO
*/

STATUS sysSpiWrite
    (
    UINT8 * pData,       /* data source pointer */
    INT32   dataLen      /* data length for read type command */
    )
    {
    STATUS status = OK;
    UINT32 tickSave;
    UINT32 timeoutTicks = LSI_SSP_TIMEOUT * sysClkRateGet ();

    while (dataLen > 0)
        {
        LSI_SPI_REG_WRITE(SSP_SSPDR, *pData);
  
        /*
         * Get the response from slave device, note tickGet could NOT be executed 
         * in the ISR or before the system clock is set up and running
         */

        tickSave = tickGet ();

        while (0 == (LSI_SPI_REG_READ(SSP_SSPSR) & SSP_SSPSR_REFIFO_NOT_EMPTY))
            {
            if ((tickGet () - tickSave) > timeoutTicks)
                {
                status = ERROR;
                break;
                }
            }
    
        LSI_SPI_REG_READ(SSP_SSPDR);

        pData ++;
        dataLen --;
        }

    return (status);
    }
