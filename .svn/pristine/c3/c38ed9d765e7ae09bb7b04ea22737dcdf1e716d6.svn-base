/* sysMpc512xI2c.c - MPC512x I2C Driver Source Module */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,05nov07,b_m  derived from version 01c of ads834x/sysMpc834xI2c.c.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "sysMotI2c.h"
#include "sysMpc512xI2c.h"

/* externals */

IMPORT I2C_DRV_CTRL * pI2cDrvCtrl[MPC512X_I2C_NUMS];

IMPORT void sysMsDelay(UINT);

/* indicates if a "Repeat Start" is requested */

int I2CDoRepeatStart = 0;


/*******************************************************************************
*
* i2cCycleMpc512xStart - perform I2C "start" cycle
*
* This function's purpose is to perform an I2C start cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xStart
    (
    int unit
    )
    {
    unsigned int timeOutCount;
    UINT8 statusReg = 0;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * if this is a repeat start, then set the required bits and return.
     *
     * NOTE:
     * this driver ONLY supports one repeat start between the start
     * stop and cycles.
     */

    if (I2CDoRepeatStart == 1)
        {
        i2cIoctl(I2C_IOCTL_RMW_OR,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
                (MPC512X_I2C_CONTROL_REG_RSTA |
                 MPC512X_I2C_CONTROL_REG_MSTA |
                 MPC512X_I2C_CONTROL_REG_MTX), 0);

        /* one repeat start only, so clear this bit */
        I2CDoRepeatStart = 0;

        return (0);
        }

    /*
     * Wait until the I2C bus is free.  If it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for (timeOutCount = 1000; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_STATUS_REG),
                0, 0);

        if (!(statusReg & MPC512X_I2C_STATUS_REG_MBB))
            {
            break;
            }
        }

    if (!timeOutCount)
        {
        return (-1);
        }

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */

    i2cIoctl(I2C_IOCTL_RMW_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            (MPC512X_I2C_CONTROL_REG_MTX |
             MPC512X_I2C_CONTROL_REG_MSTA), 0);

    /*
     * The first time through, set "I2CDoRepeatStart".  If this function
     * is called again BEFORE a STOP is sent, then we are doing a
     * "dummy write", which sets the devices internal byte pointer
     * to the byte we intend to read.
     */

    I2CDoRepeatStart = 1;

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xStop - perform I2C "stop" cycle
*
* This function's purpose is to perform an I2C stop cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xStop
    (
    int unit
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * turn off MSTA bit(which will generate a STOP bus cycle)
     * turn off MTX bit(which places the I2C interface into receive mode
     * turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */

    i2cIoctl(I2C_IOCTL_RMW_AND,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            ((UINT8)~(MPC512X_I2C_CONTROL_REG_MTX |
             MPC512X_I2C_CONTROL_REG_MSTA |
             MPC512X_I2C_CONTROL_REG_TXAK)), 0);

    /* clear the global I2C "Repeat Start" flag */

    I2CDoRepeatStart = 0;

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xRead - perform I2C "read" cycle
*
* This function's purpose is to perform an I2C read cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xRead
    (
    int             unit,
    unsigned char * pReadDataBuf,   /* pointer to read data buffer */
    int             ack             /* 0 = don't ack, 1 = ack */
    )
    {
    unsigned int readData = 0;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * place the I2C interface into receive mode(MTX=0) and set the interface
     * to NOT acknowledge(TXAK=1) the incoming data on the 9th clock cycle.
     * this is required when doing random reads of a I2C device.
     */

    if (!ack)
        {
        /* don't send master ack */

        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            ((UINT8)~MPC512X_I2C_CONTROL_REG_MTX),
            MPC512X_I2C_CONTROL_REG_TXAK);
        }
    else
        {
        /* send master ack */

        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            ((UINT8)~(MPC512X_I2C_CONTROL_REG_MTX |
             MPC512X_I2C_CONTROL_REG_TXAK)), 0);
        }

    /* do a "dummy read", this latches the data off the bus */

    i2cIoctl(I2C_IOCTL_RD,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_DATA_REG),
            0, 0);

    /* now do the actual read, make this one count */

    readData = i2cIoctl(I2C_IOCTL_RD,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_DATA_REG),
            0, 0);

    *pReadDataBuf = (unsigned char)readData;

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xWrite - perform I2C "write" cycle
*
* This function's purpose is to perform an I2C write cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xWrite
    (
    int             unit,
    unsigned char   writeData   /* character to write */
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * write the requested data to the data register, which will cause
     * it to be transmitted on the I2C bus.
     */

    i2cIoctl(I2C_IOCTL_WR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_DATA_REG),
            (UINT32)writeData, 0);

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xAckIn - perform I2C "acknowledge-in" cycle
*
* This function's purpose is to perform an I2C acknowledge-in cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc512xAckIn
    (
    int unit
    )
    {
    unsigned int statusReg = 0;
    unsigned int timeOutCount;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * Wait until an *internal* device interrupt has been generated, then
     * clear it.  If it is not received, return with an error.
     * We are polling, so NO processor interrupt is generated.
     */

    for (timeOutCount = 100; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_STATUS_REG),
                0, 0);

        if (statusReg & MPC512X_I2C_STATUS_REG_MIF)
            {
            i2cIoctl(I2C_IOCTL_RMW_AND,
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_STATUS_REG),
                    ((UINT8)~MPC512X_I2C_STATUS_REG_MIF), 0);
            break;
            }
        }

    if (!timeOutCount)
        {
        return (-1);
        }

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xAckOut - perform I2C "acknowledge-out" cycle
*
* This function's purpose is to perform an I2C acknowledge-out cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xAckOut
    (
    int unit
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    return (0);
    }

/*******************************************************************************
*
* i2cCycleMpc512xKnownState - initialize the I2C bus to a known state
*
* This function's purpose is to initialize the I2C bus to a known state.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc512xKnownState
    (
    int unit
    )
    {
    STATUS status;
    UINT   timeOutCount;
    UINT   statusReg;

    status = OK;

    if (pI2cDrvCtrl[unit] == NULL)
        return ERROR ;

    /*
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for (timeOutCount = 10; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_STATUS_REG),
                0, 0);

        if (!(statusReg & MPC512X_I2C_STATUS_REG_MBB))
            {
            status = OK;
            break;
            }

        /*
         * re-initialize the I2C if the BUS BUSY does not clear
         * after trying half the *reasonable* amount of reads of the
         * status register.
         */

        if (!(timeOutCount % 5))
            {
            status = i2cDrvInit(unit);
            if (status == OK)
                break;
            else
                return ERROR;
            }
        }

    if (!timeOutCount)
        status = ERROR;

    return (status);
    }

/*******************************************************************************
*
* i2cCycleMpc512xDelay - perform interface's I2C delay routine
*
* This function's purpose is to perform whatever delay required for the device.
*
* RETURNS: N/A
*
* ERRNO
*/
void i2cCycleMpc512xDelay
    (
    int mSeconds
    )
    {
    sysMsDelay(mSeconds);
    }
