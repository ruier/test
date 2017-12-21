/* sysMotI2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,05feb09,x_f  modified i2cDoOpM41T00 and added RTC support.
01a,05nov07,b_m  derived from version 01d of ads834x/sysMotI2c.c.
*/

/* includes */

#include <vxWorks.h>
#include <logLib.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "sysMotI2c.h"
#include "sysMpc512xI2c.h"

/* forward declarations */

IMPORT void sysMsDelay(UINT);

/* I2C driver routines table for the MPC512x device */

i2cDrvRoutines_t i2cDrvRoutinesTableMpc512x =
{
    (int(*)())i2cCycleMpc512xStart,
    (int(*)())i2cCycleMpc512xStop,
    (int(*)())i2cCycleMpc512xRead,
    (int(*)())i2cCycleMpc512xWrite,
    (int(*)())i2cCycleMpc512xAckIn,
    (int(*)())i2cCycleMpc512xAckOut,
    (int(*)())i2cCycleMpc512xKnownState,
    (void(*)())i2cCycleMpc512xDelay
};

/* controller routines table, indexed by the "I2C_DRV_TYPE" */

i2cDrvRoutines_t *i2cDrvRoutinesTables[] =
{
    (i2cDrvRoutines_t *)&i2cDrvRoutinesTableMpc512x
};

/* I2C driver controls */

I2C_DRV_CTRL    i2C1DrvCtrl;
I2C_DRV_CTRL    i2C2DrvCtrl;
I2C_DRV_CTRL    i2C3DrvCtrl;

I2C_DRV_CTRL *  pI2cDrvCtrl[MPC512X_I2C_NUMS] = { NULL, NULL, NULL };


/*******************************************************************************
*
* i2cInByte - reads a byte from I2C register.
*
* This function reads a byte from I2C register.
*
* RETURNS: register value.
*/

LOCAL UINT8 i2cInByte
    (
    UINT32  dataPtr
    )
    {
    UINT8 *ptr = (UINT8 *)dataPtr;
    return (*ptr);
    }

/*******************************************************************************
*
* i2cOutByte - writes a byte to I2C register.
*
* This function writes a byte to I2C register.
*
* RETURNS: N/A
*/

LOCAL void i2cOutByte
    (
    UINT32  dataPtr,
    UINT8   data
    )
    {
    UINT8 * ptr = (UINT8*)dataPtr;
    *ptr = data;
    }

/*******************************************************************************
*
* i2cAddressMunge - generate the I2C slave address.
*
* This function's purpose is to munge/modify the I2C device address based upon
* the byte offset into the device.  This only applies to EEPROM type devices.
* Dependent upon the size of the device, A0-A2 address lines are utilized as
* 256 byte size page offsets.
*
* RETURNS: munged address
*/

unsigned int i2cAddressMunge
    (
    unsigned int deviceAddress
    )
    {
    return ((deviceAddress << 1) & 0xfe);
    }

/*******************************************************************************
*
* i2cDoOpPCF8574 - I2C operations on PCF8574
*
* This function's purpose is to write the specified number of
* blocks to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cDoOpPCF8574
    (
    int             unit,
    UINT32          deviceAddress,      /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket       /* pointer to command packet */
    )
    {
    /* only accept 1 byte RW is allowed */

    if (pI2cCmdPacket->nBlocks != 1)
        {
        I2C_KNOWN_STATE(unit);
        pI2cCmdPacket->status = I2C_ERROR_KNOWN_STATE;
        logMsg("[i2cDoOpPCF8574]: only 1 byte RW is allowed\n", 1,2,3,4,5,6);
        return ERROR;
        }

    /* read operation */

    if (pI2cCmdPacket->command == I2C_READOP)
        {
        if (I2C_KNOWN_STATE(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_KNOWN_STATE;
            return ERROR;
            }

        if (I2C_CYCLE_START(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_START;
            return ERROR;
            }

        if (I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress) | 0x01))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_WRITE;
            return ERROR;
            }

        if (I2C_CYCLE_ACKIN(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_ACKIN;
            return ERROR;
            }

        if (I2C_CYCLE_READ(unit, (unsigned char *)pI2cCmdPacket->memoryAddress, 1))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_READ;
            return ERROR ;
            }

        if (I2C_CYCLE_ACKIN(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_ACKIN;
            return ERROR;
            }

        if (I2C_CYCLE_READ(unit, (unsigned char *)pI2cCmdPacket->memoryAddress, 1))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_READ;
            return ERROR ;
            }

        pI2cCmdPacket->aCount++;

        if (I2C_CYCLE_STOP(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_STOP;
            return ERROR ;
            }
        }
    else if (pI2cCmdPacket->command == I2C_WRITOP)
        {
        if (I2C_KNOWN_STATE(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_KNOWN_STATE;
            return ERROR;
            }

        if (I2C_CYCLE_START(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_START;
            return ERROR;
            }

        if (I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress)))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_WRITE;
            return ERROR;
            }

        if (I2C_CYCLE_ACKIN(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_ACKIN;
            return ERROR;
            }

        if (I2C_CYCLE_WRITE(unit, pI2cCmdPacket->blockNumber))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_WRITE;
            return ERROR;
            }

        if (I2C_CYCLE_ACKIN(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_ACKIN;
            return ERROR;
            }

        if (I2C_CYCLE_WRITE(unit, *(unsigned char *)(pI2cCmdPacket->memoryAddress)))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_WRITE;
            return ERROR;
            }

        if (I2C_CYCLE_ACKIN(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_ACKIN;
            return ERROR;
            }

        pI2cCmdPacket->aCount++;

        if (I2C_CYCLE_STOP(unit))
            {
            pI2cCmdPacket->status = I2C_ERROR_CYCLE_STOP;
            return ERROR;
            }
        }
    else
        {
        I2C_KNOWN_STATE(unit);
        return ERROR;
        }

    /* leave the I2C bus in a known state */
    I2C_KNOWN_STATE(unit);

    /* update the caller's command packet with status of the operation */
    pI2cCmdPacket->status = 0;

    return OK;
}

/*******************************************************************************
*
* i2cDoOpM41T00 - I2C operations on M41T00
*
* This function's purpose is to write the specified number of
* blocks to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cDoOpM41T00
    (
    int             unit,
    UINT32          deviceAddress,      /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket       /* pointer to command packet */
    )
    {
    int             byteCount;          /* byte counter */
    int             statusVariable;     /* local status variable */
    unsigned char * pWriteData;         /* pointer to write data buffer */

    if (pI2cCmdPacket->command == I2C_READOP)
        {

        /* Read the specified number of bytes from RTC */

        /* NOTE: random read has a dummy write first */

        statusVariable = 0;

        if ( I2C_KNOWN_STATE(unit) )
            {
            statusVariable = I2C_ERROR_KNOWN_STATE;
            byteCount = pI2cCmdPacket->nBlocks;
            }

        for (byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++)
            {
            if (I2C_CYCLE_START(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }

            /* device address - write */

            if (I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if (I2C_CYCLE_WRITE(unit, ((pI2cCmdPacket->blockNumber +
                                         byteCount) & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if (I2C_CYCLE_START(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }

            /* device address - read */

            if (I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress) | 0x1))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }

            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if (I2C_CYCLE_READ(unit, (unsigned char *)
                                pI2cCmdPacket->memoryAddress + byteCount, 0))
                {
                statusVariable = I2C_ERROR_CYCLE_READ;
                break;
                }

            if (I2C_CYCLE_STOP(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_STOP;
                break;
                }

            /* Increment the actual count of the command packet */

            pI2cCmdPacket->aCount += 1;
            }

        /*
         * update the caller's command packet with status of
         * the operation
         */

        pI2cCmdPacket->status = statusVariable;
        }
    else if (pI2cCmdPacket->command == I2C_WRITOP)
        {

        /* Initialize pointer to caller's write data buffer. */

        pWriteData = (unsigned char *)pI2cCmdPacket->memoryAddress;

        statusVariable = 0;

        if (I2C_KNOWN_STATE(unit))
            {
            statusVariable  = I2C_ERROR_KNOWN_STATE;
            byteCount       = pI2cCmdPacket->nBlocks;
            }

        for (byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++)
            {
            if (I2C_CYCLE_START(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }

            /* device address */

            if (I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress )))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }

            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if (I2C_CYCLE_WRITE(unit,
                    ((pI2cCmdPacket->blockNumber + byteCount) & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }

            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            /* write data */

            if (I2C_CYCLE_WRITE(unit, *(pWriteData + byteCount)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }

            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if (I2C_CYCLE_STOP(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_STOP;
                break;
                }

            /* Increment the actual count of the command packet */

            pI2cCmdPacket->aCount += 1;

            /*
             * delay for at least 10ms to allow EEPROM to complete
             * the write cycle (internal operation)
             */

            I2C_DELAY(10);
            }
        }
    else
        {
        I2C_KNOWN_STATE(unit);
        return ERROR ;
        }

    /* Leave the I2C bus in a known state. */

    I2C_KNOWN_STATE(unit);

    /*
     * update the caller's command packet with status of
     * the operation
     */

    pI2cCmdPacket->status = statusVariable;

    return OK ;
    }

/*******************************************************************************
*
* i2cDoOp - I2C do operation
*
* This function's purpose is to execute the operation as specified
* by the passed command packet.
*
* RETURNS: OK, always.
*/

int i2cDoOp
    (
    int             unit,
    UINT32          deviceAddress,      /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket       /* pointer to command packet */
    )
    {
    /*
     * Command interface to stop.
     * This is for previous error exits.
     */

    I2C_CYCLE_STOP(unit);

    switch (pI2cCmdPacket->deviceType)
        {
        case I2C_DEVICE_TYPE_IOPORT_PCF8574:
            if (i2cDoOpPCF8574(unit, deviceAddress, pI2cCmdPacket) != OK)
                return ERROR;
            break;
        case I2C_DEVICE_TYPE_RTC_M41T00:
            if (i2cDoOpM41T00(unit, deviceAddress, pI2cCmdPacket) != OK)
                return ERROR;
            break;
        default:
            return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* i2cIoctl - I2C register access wrapper routine.
*
* The purpose of this function is to perform various register operations.
*
* RETURNS: UINT8, for read operations.
*/

UINT8 i2cIoctl
    (
    UINT32  ioctlflg,  /*
                        * input/ouput control flag
                        * 0, write
                        * 1, read
                        * 2, read/modify/write (ORing)
                        * 3, read/modify/write (ANDing)
                        * 4, read/modify/write (AND/ORing)
                        */
    UINT32  address,    /* address of device register to be operated upon */
    UINT8   bdata1,     /* data item 1 for read/write operation */
    UINT8   bdata2      /* data item 2 for read/write operation */
    )
    {
    UINT8 u8temp;

    i2cCycleMpc512xDelay(1);

    if (ioctlflg == I2C_IOCTL_WR)               /* write */
        {
        i2cOutByte(address, bdata1);
        }
    else if (ioctlflg == I2C_IOCTL_RD)          /* read */
        {
        bdata1 = i2cInByte(address);
        }
    else if (ioctlflg == I2C_IOCTL_RMW_OR)      /* ORing */
        {
        u8temp = i2cInByte(address);
        u8temp |= bdata1;
        i2cCycleMpc512xDelay(1);

        i2cOutByte(address, u8temp);
        }
    else if (ioctlflg == I2C_IOCTL_RMW_AND)     /* ANDing */
        {
        u8temp = i2cInByte(address);
        u8temp &= bdata1;
        i2cCycleMpc512xDelay(1);

        i2cOutByte(address, u8temp);
        }
    else if (ioctlflg == I2C_IOCTL_RMW_AND_OR)  /* AND/ORing */
        {
        u8temp = i2cInByte(address);
        u8temp &= bdata1;
        u8temp |= bdata2;
        i2cCycleMpc512xDelay(1);

        i2cOutByte(address, u8temp);
        }

    i2cCycleMpc512xDelay(1);

    return (bdata1);
    }

/*******************************************************************************
*
* i2cDrvInit - initialize the I2C device
*
* This function's purpose is to the initialize the I2C device controller device
* for operation.  This function should only be executed once during system
* initialization time.
*
* NOTE: not printf or logMsg statements here because called during sysHwInit().
* If output is desired a pollled or debug dump routine should be used.
*
* RETURNS: OK
*/

STATUS i2cDrvInit
    (
    int unit
    )
    {
    /*
     * Note: a switch statement will not work here if executing from ROM
     * due to branch history table creation.
     */

    if (pI2cDrvCtrl[unit] == NULL)
        {
        if (unit == 0)
            {
            pI2cDrvCtrl[unit] = &i2C1DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = I2C_OFF(0);
            }
        else if (unit == 1)
            {
            pI2cDrvCtrl[unit] = &i2C2DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = I2C_OFF(1);
            }
        else if (unit == 2)
            {
            pI2cDrvCtrl[unit] = &i2C3DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = I2C_OFF(2);
            }

        pI2cDrvCtrl[unit]->baseAdrs += IMMRBAR;
        }

    /* disable the I2C module, set the device to master mode */

    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            ((UINT8)~MPC512X_I2C_CONTROL_REG_MEN),
            MPC512X_I2C_CONTROL_REG_MSTA);

    /* set the clock frequency divider */

    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_FREQ_DIV_REG),
            ((UINT8)~MPC512X_I2C_FREQ_DIV_REG_MASK),
            MPC512X_I2C_FREQ_DIVIDER);

    /* set the slave address */

    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_ADR_REG),
            ((UINT8)~MPC512X_I2C_ADDRESS_REG_MASK), 1);

    /* enable the interface */

    i2cIoctl(I2C_IOCTL_RMW_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            MPC512X_I2C_CONTROL_REG_MEN, 0);

    /*
     * Set the device to slave mode.
     * This is required for clearing a BUS BUSY lockup condition.
     */

    i2cIoctl(I2C_IOCTL_RMW_AND,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC512X_I2C_CONTROL_REG),
            ((UINT8)~MPC512X_I2C_CONTROL_REG_MSTA), 0);

    return (OK);
}

/*******************************************************************************
*
* i2cRead - i2c read blocks
*
* This function's purpose is to read the specified number of
* blocks from the specified device.
*
* RETURNS: OK, or Error on a bad request
*/

int i2cRead
    (
    int          unit,
    UINT32       deviceAddress, /* device I2C bus address */
    int          deviceType,
    unsigned int startBlk,      /* starting block to read */
    unsigned int numBlks,       /* number of blocks to read */
    char *       pBuf           /* pointer to buffer to receive data */
    )
    {
    i2cCmdPckt_t i2cCmdPacket;
    int localStatus;

    /* check to see if the driver's been installed */

    if (pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit, 2,3,4,5,6);
        return (ERROR);
        }

    /* check for a bad request */

    if (!numBlks)
        {
        return (ERROR);
        }

    /* build command packet */

    i2cCmdPacket.command = I2C_READOP;
    i2cCmdPacket.status = 0;
    i2cCmdPacket.memoryAddress = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber = startBlk;
    i2cCmdPacket.nBlocks = numBlks;
    i2cCmdPacket.eCount = numBlks;
    i2cCmdPacket.aCount = 0;
    i2cCmdPacket.deviceType = deviceType;

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

    /* return the appropriate status */

    if (i2cCmdPacket.status != 0)
        {
        logMsg("i2cCmdPacket.status = 0x%x\n", i2cCmdPacket.status, 2,3,4,5,6);
        localStatus = ERROR;
        }
    else
        localStatus = OK;

    return (localStatus);
    }

/*******************************************************************************
*
* i2cWrite - i2c write blocks
*
* This function's purpose is to write the specified number of
* blocks to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cWrite
    (
    int          unit,
    UINT32       deviceAddress, /* device I2C bus address */
    int          deviceType,
    unsigned int startBlk,      /* starting block to write */
    unsigned int numBlks,       /* number of blocks to write */
    char *       pBuf           /* pointer to buffer of send data */
    )
    {
    i2cCmdPckt_t i2cCmdPacket;
    int localStatus;

    /* check to see if the driver's been installed */

    if (pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit, 2,3,4,5,6);
        return (ERROR);
        }

    /* check for a NOP request */

    if (!numBlks)
        {
        return (ERROR);
        }

    /* build command packet */

    i2cCmdPacket.command = I2C_WRITOP;
    i2cCmdPacket.status = 0;
    i2cCmdPacket.memoryAddress = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber = startBlk;
    i2cCmdPacket.nBlocks = numBlks;
    i2cCmdPacket.eCount = numBlks;
    i2cCmdPacket.aCount = 0;
    i2cCmdPacket.deviceType = deviceType;

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

    /* return the appropriate status */

    if (i2cCmdPacket.status != 0)
        {
        logMsg("i2cCmdPacket.status = 0x%x\n", i2cCmdPacket.status, 2,3,4,5,6);
        localStatus = ERROR;
        }
    else
        localStatus = OK;

    return (localStatus);
    }

/*******************************************************************************
* 
* m41t00I2cRead - perform I2C read for M41T00 RTC timer
* 
* This routine performs I2C read for M41T00 RTC timer.
* 
* RETURNS: data read from I2C
*
* ERRNO
*/

unsigned char m41t00I2cRead
    (
    unsigned char deviceAddr,
    unsigned char reg
    )
    {
    char rtcDataUse;
    
    i2cRead (0, deviceAddr, I2C_DEVICE_TYPE_RTC_M41T00, (unsigned int)reg, 1, 
             &rtcDataUse);

    return ((unsigned char) rtcDataUse);
    }

/*******************************************************************************
* 
* m41t00I2cWrite - perform I2C write for M41T00 RTC timer
* 
* This routine performs I2C write for M41T00 RTC timer.
* 
* RETURNS:  N/A
*
* ERRNO
*/

void m41t00I2cWrite
    (
    unsigned char deviceAddr,
    unsigned char reg,
    unsigned char val
    )
    {
    i2cWrite (0, deviceAddr, I2C_DEVICE_TYPE_RTC_M41T00, (unsigned int)reg, 1, 
              (char *)&val);
    }

#ifdef I2C_DRIVER_DEBUG
LOCAL int i2cTypeToAddr[3] =
    {
    I2C_ADDR_PCF8574A,
    I2C_ADDR_AT24C32A,
    I2C_ADDR_M41T00
    };

void i2cTestRead
    (
    int type,
    int off,
    int num
    )
    {
    char * buf;
    char * tmp;
    int i;

    buf = malloc(num);
    if (buf == NULL)
        return;
    bzero(buf, num);

    tmp = buf;
    for (i = 0; i < num; i++)
        i2cRead(0, i2cTypeToAddr[type], type, off++, 1, tmp++);

    tmp = buf;
    for (i = 0; i < num; i++)
        {
        if (i % 16 == 0)
            printf("\n%02x: ", i);
        printf("%02x ", *tmp++);
        }
    printf("\n");

    free(buf);
    }

int i2cTestWrite
    (
    int type,
    int data
    )
    {
    UINT8 buf = data;
    return i2cWrite(0, i2cTypeToAddr[type], type, 0, 1, &buf);
    }
#endif /* I2C_DRIVER_DEBUG */
