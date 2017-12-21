/* sysFslSpi.c - FreeScale PowerPC eSPI Driver */

/*
* Copyright (c) 2011, 2012 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01c,22nov12,d_l  fix a wrong divide ratio.(WIND00389496)
01b,19sep12,d_l  add checking divide ratio.(WIND00376535)
01a,12may11,y_y  written.
*/

/*
DESCRIPTION

The enhanced serial peripheral interface (eSPI) allows the device to exchange
data with peripheral devices such as EEPROMs, real-time clocks, A/D converters,
and ISDN devices. The eSPI is a full-duplex, synchronous, character-oriented
channel that supports a simple interface (receive, transmit, clock and
chipselects).

This is a driver for the FreeScale's enhanced serial peripheral interface
(eSPI) engine. It is intended for use by client drivers performing other high
level functions.
*/


/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sysFslSpi.h"

/* defines */

#define SPI_REG_READ_32(addr)         *((VUINT32 *)(SPI_REG_BASE + addr))
#define SPI_REG_WRITE_32(addr, data)  *((VUINT32 *)(SPI_REG_BASE + addr)) = data

#define EIEIO_SYNC                    WRS_ASM (" eieio; sync")

/* debug */

#ifdef  SPI_DBG_ON

#define SPI_DBG_OFF           0x00000000
#define SPI_DBG_CMD           0x00000001
#define SPI_DBG_READ          0x00000010
#define SPI_DBG_WRITE         0x00000100
#define SPI_DBG_ALL           0xffffffff

UINT32 spiDbgMask = SPI_DBG_OFF;

#define SPI_DBG(mask, string, a, b, c, d, e, f)          \
    if ((spiDbgMask & mask) || (mask == SPI_DBG_OFF))    \
        logMsg(string, a, b, c, d, e, f)
        
#else
#define SPI_DBG(mask, string, a, b, c, d, e, f)
#endif  /* SPI_DBG_ON */

/*******************************************************************************
*
* sysSpiDrvInit - Freescale eSPI controller initialization
*
* The purpose of this function is to perform eSPI controller initialization.
*
* RETURNS: OK or ERROR if there is an error.
*/

void sysSpiDrvInit
    (
    UINT8 chipSelect,
    UINT32 busFreq
    )
    {
    UINT32 clk = 0;
    UINT32 temp = 0;
    UINT32 clkFreq;
    UINT16 divideRatio = 0;
    UINT8 pm = 0;
    BOOL  div16 = FALSE;
    UINT32 freq = 0;

    clkFreq = sysClkFreqGet ();

    /* clear any previous events */

    SPI_REG_WRITE_32 (SPIE, SPI_EVENT_CLR);

    /* disable all interrupt source */

    SPI_REG_WRITE_32 (SPIM, 0);

    divideRatio = clkFreq / busFreq;
                                                
    /*                                          
     * The range of divide ratio of the prescale divider in the eSPI clock
     * generator is from 2 to 32*16.            
     */                                         
                                                
    if (divideRatio < 2)                        
        divideRatio = 2;                        
    else if (divideRatio > 32 * 16)             
        divideRatio = 32 * 16;                  
                                                
    /*                                          
     * Selects the clock source for the eSPI baud rate generator (eSPI BRG)
     * when configured as an eSPI master.       
     */                                         
                                                
    if (divideRatio > 32)                       
        {                                       
        /* System clock/16 is the input to the eSPI BRG.*/
                                                
        divideRatio = divideRatio + 31;         
        div16 = TRUE;                           
        divideRatio /= 16;                      
        }                                       
    else                                        
        {                                       
        /* System clock is the input to the eSPI BRG */
                                                
        divideRatio = divideRatio + 2;          
        div16 = FALSE;                          
        }                                       
                                                
    pm = ((divideRatio / 4) - 1);               
                                                
    freq = (clkFreq / (2 * (pm + 1)));          
                                                
    if (div16)                                  
        freq /= 16;                             
                                                
    temp = ((div16 ? SPICSMODE_DIV16 : 0) |
            ((divideRatio & 1) ? SPICSMODE_ODD : 0) | SPI_CSMODE_PMn(pm));

    SPI_REG_WRITE_32 (SPMODE0, SPI_CSMODE_POLn_LOW | SPI_CSMODE_CSnCG |
                      SPI_CSMODE_REVn_MSB | SPI_CSMODE_LENn | temp);
    SPI_REG_WRITE_32 (SPMODE1, SPI_CSMODE_POLn_LOW | SPI_CSMODE_CSnCG |
                      SPI_CSMODE_REVn_MSB | SPI_CSMODE_LENn | temp);
    SPI_REG_WRITE_32 (SPMODE2, SPI_CSMODE_POLn_LOW | SPI_CSMODE_CSnCG |
                      SPI_CSMODE_REVn_MSB | SPI_CSMODE_LENn | temp);
    SPI_REG_WRITE_32 (SPMODE3, SPI_CSMODE_POLn_LOW | SPI_CSMODE_CSnCG |
                      SPI_CSMODE_REVn_MSB | SPI_CSMODE_LENn | temp);

    /* enable SPI_THR and SPI_RHR */

    SPI_REG_WRITE_32 (SPMODE, SPI_TXTHR_LEN | SPI_RXTHR_LEN);

    /* reset the controller */
    
    SPI_REG_WRITE_32 (SPMODE, SPI_REG_READ_32 (SPMODE) | (SPI_EN));
    }

/*******************************************************************************
*
* sysSpiReadData - read data from SPI receive FIFO on poll mode
*
* The purpose of this function is to read data from eSPI receive FIFO to dest
* address.
*
* RETURNS:  OK or ERROR if there is an error.
*/

STATUS sysSpiReadData
    (
    UINT32 datalen,
    UINT8 * data
    )
    {
    VUINT32 readData;
    UINT8 * pointer = NULL;

    UINT32 numCount = (datalen / 4 + ((datalen % 4 != 0) ? 1 : 0));
    UINT32 numChars = datalen % 4;
    UINT32 i;

    if (datalen > 0xffff)
        {
        return ERROR;
        }
    
    /* read the SPIRF numCount times  */

    for (i = numCount; i > 0; i--)
        {
          
        /* read the SPIRF always in 32 bit */
  
        readData = SPI_REG_READ_32 (SPIRF);

        /* handle the 8bit data */

        if (numCount == 0 && numChars != 0)
            {
            /* get the pointer of the byte*/

            pointer = (UINT8 *) &readData;
            do
                {
                *(UINT8 *) data++ = *pointer++;
                }
            while (numChars--);
            }
        else
            {
 
            /* read data with 32 bit directly stored into buffer */

            *(UINT32 *) data = readData;
            data += 4;
            }

        /* 
         * delay for one IO READ cycle.
         * This delay is the experienced value but needed, to get high 
         * performance, the delay can be optimized.
         */

        sysUsDelay (2);
        EIEIO_SYNC;    /* eieio, sync */

        SPI_DBG(SPI_DBG_READ, "read: 0x%08x, numCount(%d) \n",
                                     readData, numCount, 0, 0, 0, 0);
        }

    /* clear the RXCNT via enable eSPI */

    SPI_REG_WRITE_32 (SPMODE, SPI_REG_READ_32 (SPMODE) & (~SPI_EN));
    SPI_REG_WRITE_32 (SPMODE, SPI_REG_READ_32 (SPMODE) | (SPI_EN));

    return OK;
    }

/*******************************************************************************
*
* sysSpiWriteData - write data to SPI device on poll mode
*
* The purpose of this function is to write data to SPI device on poll mode. The
* routine put the command as the data.
*
* RETURNS:  OK or ERROR if there is an error.
*/

STATUS sysSpiWriteData
    (
    UINT32 cs,
    UINT32 cmdlen,
    UINT32 datalen,
    UINT8 * cmd,
    UINT8 * data,
    UINT32 mode
    )
    {
    UINT32 command = 0;
    UINT32 dataCount = 0;

    UINT32 cmdCount = (cmdlen / 4 + ((cmdlen % 4 != 0) ? 1 : 0));

    if (data != NULL)
        {
        dataCount = (datalen / 4 + ((datalen % 4 != 0) ? 1 : 0));
        }

    command = SPCOM_CS (cs) | (cmdlen + datalen - 1) ;

    /*
     * if TRANSONLY=0, RxSKIP=0, the eSPI changes to full duplex mode
     * if TRANSONLY=1, RxSKIP is ignored, no reception, useful for write 
     * transcations.
     * RxSKIP: Num of ch skipped for reception from frame start, Non-zero 
     * values of RxSKIP force the eSPI to half-duplex mode.
     */

    command |= SPCOM_TO (mode);

    /* only write command, so skip the cmdlen */

    if (data == NULL)
        {
        command |= SPCOM_RxSKIP (cmdlen);
        }

    SPI_REG_WRITE_32 (SPCOM, command);

    /* determined only by command length */

    while (cmdCount--)
        {
        SPI_REG_WRITE_32 (SPITF, *(VUINT32 *) cmd);
        SPI_DBG(SPI_DBG_CMD, "cmdCount(%d), cmd 0x%08x written \n",
                                  cmdCount, *(VUINT32 *) cmd, 0, 0, 0, 0);

        /* 
         * delay for one IO WRTIE command cycle. 
         * This delay is the experienced value. To get high performance,
         * it could be optimized by reducing the delay.
         */

        sysUsDelay (3);
        EIEIO_SYNC;    /* eieio, sync */
        }

    while (dataCount--)
        {
        SPI_REG_WRITE_32 (SPITF, *(VUINT32 *) data);
        SPI_DBG(SPI_DBG_WRITE, "dataCount(%d), data 0x%08x written \n",
                                     dataCount, *(VUINT32 *) data, 0,0,0,0);
        data += 4;

        /* 
         * delay for one IO WRTIE data cycle. 
         * This delay is the experienced value. To get high performance,
         * it could be optimized by reducing the delay.
         */

        sysUsDelay (2);
        EIEIO_SYNC;    /* eieio, sync */
        }

    return OK;
    }


