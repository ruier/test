/* sysLsiI2c.c - LSI ACP34XX I2C master API library */

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
01b,12feb11,x_s  Reduce waitting time for operating I2C controller.
01a,23dec10,x_s  Initial creation.
*/

/*
DESCRIPTION
This driver provides routines useful for driving the LSI ACP34XX I2C (IIC)
controller in master mode. The I2C is initialized with the routine sysI2CInit(),
the data is transferred in and out with sysI2cSend() and sysI2CRcv(). This driver 
only supports polling mode.

See those routines for further detail.

To use this module, define the macro INCLUDE_I2C in config.h.

The data transfer always use the polled mode.

INCLUDE FILES: sysLib.h tickLib.h config.h

SEE ALSO:
\tb "APP350 ARM I2C Interface",
\tb "Axxia Communication Processor (ACP3400) Technical Guide",
\tb "ACP3400 Validation Board (Mission) Architecture",
\tb "LSI ACP34XX Board Schematics".
*/

/* includes */

#include <vxWorks.h>
#include <sysLib.h>
#include <tickLib.h>
#include "config.h"

/* defines */

#define LSI_I2C_TIMEOUT     5  /* timeout(seconds) for data transfer */
#define LSI_I2C_WRITE_TIME  5  /* time(5ms) for data write */
#define LSI_I2C_TIMER_PERIOD_VALUE  0x10

/* Read/Write Register */

#define LSI_I2C_REG_WRITE(addr, value)  sysOutLong(addr, value)

#define LSI_I2C_REG_READ(addr)          sysInLong(addr)

/*******************************************************************************
*
* sysI2cSend - write data to I2C device on poll mode
*
* The purpose of this function is to write data to I2C device on poll mode.
*
* RETURNS: OK, or ERROR if the operation fails.
*
* ERRNO
*/

STATUS sysI2cSend
    (
    char   addr,       /* I2C device address */
    char   offset,     /* offset into the I2C */
    char * pSrc,       /* data transfer to slave device */
    int    nbytes      /* number of bytes to transfer */
    )
    {
    STATUS      status = OK;
    UINT32      tickSave;
    UINT32      timeoutTicks = LSI_I2C_TIMEOUT * sysClkRateGet ();
    UINT32      data0 = 0;
    
    /* transmit config */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, I2C_TX_CFG_ZEROALL |     /* zeros all registers */
                                  I2C_TX_CFG_MASTER_MODE); /* master mode active */
    
    /* initialize clock division */
    
    LSI_I2C_REG_WRITE(I2C_CLK_CFG, I2C_CLK_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_START_CFG, I2C_START_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_STOP_CFG, I2C_STOP_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_DATA_CFG, I2C_DATA_CFG_VALUE);

    /* transmit config */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, I2C_TX_CFG_MASTER_MODE | /* master mode active */
                                  I2C_TX_CFG_STOP_CLOCK);  /* stop I2C clock */
    
    /* write slave device address */

    LSI_I2C_REG_WRITE(I2C_SLAVE_ADRS, addr);

    /*
     * write data to data register, every time two bytes could be written at most
     * due to I2C controller would report ERROR when more than two bytes were 
     * written.
     */
     
    if (pSrc)
        data0 = (offset << 8) | (*pSrc);  /* offset behind the data */
    else
        data0 = offset;
    
    LSI_I2C_REG_WRITE(I2C_TX_DATA0, data0); 
    
    /* transmit config */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, (I2C_TX_CFG_READY|        /* transmit ready */
                                   (nbytes << 1)|           /* bytes to transfer */ 
                                   I2C_TX_CFG_MASTER_MODE|  /* master mode active */
                                   I2C_TX_CFG_STOP_CLOCK)); /* stop I2C clock */
    
    /* write delay */
    
    sysMsDelay(LSI_I2C_WRITE_TIME);

    /*
     * Get the response from slave device, note tickGet could NOT be executed 
     * in the ISR or before the system clock is set up and running
     */

    tickSave = tickGet ();

    while ((LSI_I2C_REG_READ (I2C_TX_STATUS) & I2C_TX_RX_DONE) == 0)
        {
        if ((tickGet () - tickSave) > timeoutTicks)
            {
            status = ERROR;
            break;
            }
        }

    return (status);
    }

/*******************************************************************************
*
* sysI2cRcv - read data from I2C recieve FIFO on poll mode
*
* The purpose of this function is to read data from I2C recieve FIFO to dest
* address.
*
* RETURNS: OK, or ERROR if the operation fails.
*
* ERRNO
*/

STATUS sysI2cRcv
    (
    char   addr,       /* I2C device address */
    char * pDst        /* data from slave device */
    )
    {
    STATUS      status = OK;
    UINT32      tickSave;
    UINT32      timeoutTicks = LSI_I2C_TIMEOUT * sysClkRateGet ();
    
    /* transmit config */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, I2C_TX_CFG_ZEROALL |     /* zeros all registers */
                                  I2C_TX_CFG_MASTER_MODE); /* master mode active */
    
    /* initialize clock division */
    
    LSI_I2C_REG_WRITE(I2C_CLK_CFG, I2C_CLK_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_START_CFG, I2C_START_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_STOP_CFG, I2C_STOP_CFG_VALUE);
    LSI_I2C_REG_WRITE(I2C_DATA_CFG, I2C_DATA_CFG_VALUE);
    
    /* transmit config */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, I2C_TX_CFG_MASTER_MODE | /* master mode active */
                                  I2C_TX_CFG_STOP_CLOCK);  /* stop I2C clock */
    
    /* write slave device address */
    
    LSI_I2C_REG_WRITE(I2C_SLAVE_ADRS, addr);

    /* receive config */
    
    LSI_I2C_REG_WRITE(I2C_RX_CFG,  I2C_RX_CFG_READY |  /* Receive Ready */
                                    (1 << 1));         /* one bytes to receive */

    /* transmit config again */
    
    LSI_I2C_REG_WRITE(I2C_TX_CFG, (I2C_TX_CFG_READY |         /* transmit ready */
                                   I2C_TX_CFG_MASTER_MODE |   /* master mode active */
                                   I2C_TX_CFG_TRANSCMD_READ | /* READ mode */
                                   I2C_TX_CFG_STOP_CLOCK));   /* stop I2C clock */
    /* write delay */
    
    sysMsDelay(LSI_I2C_WRITE_TIME);
    
    /*
     * Get the response from slave device, note tickGet could NOT be executed 
     * in the ISR or before the system clock is set up and running
     */

    tickSave = tickGet ();

    while ((LSI_I2C_REG_READ(I2C_RX_STATUS) & I2C_TX_RX_DONE) == 0)
        {
        if ((tickGet () - tickSave) > timeoutTicks)
            {
            status = ERROR;
            break;
            }
        }   
    
    /* read data one byte every time*/
    
    *pDst = LSI_I2C_REG_READ(I2C_RX_DATA0) & I2C_RX_DATA_8BIT_MASK;

    return (status);
    }

/*******************************************************************************
*
* i2cInit - initialize the I2C port
*
* This routine initializes an I2C port.
*
* RETURNS: N/A
*/

void sysI2cInit (void)
    {
    /* Set the I2C timer to provide clock for I2C controller */

    sysOutLong (I2C_TIMER_BA + AXIS_TIMER_LOAD_VAL, LSI_I2C_TIMER_PERIOD_VALUE);

    sysOutLong (I2C_TIMER_BA + AXIS_TIMER_CTRL, AXIS_TIMER_EN | AXIS_TIMER_MODE_PERIOD);

    sysOutLong (I2C_TIMER_BA + AXIS_TIMER_RAW_INT_STATUS, 0x01);

    sysOutLong (I2C_TIMER_BA + AXIS_TIMER_BGLOAD_VAL, LSI_I2C_TIMER_PERIOD_VALUE);

    return;
    }
