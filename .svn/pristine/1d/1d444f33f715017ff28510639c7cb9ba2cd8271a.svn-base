/* sysMotI2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2000, 2002, 2005-2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1996-2000 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01g,10feb09,b_m  code cleanup.
01f,08jun07,b_m  add DS1374 RTC support.
01e,27jan06,dtr  Tidy up.
01d,10nov05,mdo  Documentation fixes for apigen
01c,21may02,gtf  modified for Raytheon NetFires bsp.
01b,10mar00,rhk  changed over to use sysCalcBusSpd, removed 100MHz speed.
01a,28feb00,rhk  created from version 01d, MV2100 BSP.
*/

/*
DESCRIPTION

This file contains generic functions to read/write an I2C device.
Currently the file only supports the MPC8568 I2C interface.
However, additional I2C bus controllers can be easily be added
as required. This dirver doesn't support mutiple threads/tasks and
therfore the user should control access to driver via mutex if that
is reqd.

INCLUDE FILES: sysMotI2c.h
*/


/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <ioLib.h>
#include <blkIo.h>
#include <semLib.h>
#include <cacheLib.h>
#include <intLib.h>
#include <semLib.h>
#include <logLib.h>

#include "config.h"
#include "sysMotI2c.h"
#include "sysMpc85xxI2c.h"

/* defines */

#undef I2C_DRIVER_DEBUG

/* externals */

IMPORT void sysMpc85xxMsDelay(UINT mSeconds);

/* locals */

LOCAL UINT8 i2cInByte(UINT32);
LOCAL void  i2cOutByte(UINT32, UINT8);

/* driver/controller routines table for the Mpc85xx device */
LOCAL i2cDrvRoutines_t i2cDrvRoutinesTableMpc85xx =
{
    (int(*)())i2cCycleMpc85xxStart,
    (int(*)())i2cCycleMpc85xxStop,
    (int(*)())i2cCycleMpc85xxRead,
    (int(*)())i2cCycleMpc85xxWrite,
    (int(*)())i2cCycleMpc85xxAckIn,
    (int(*)())i2cCycleMpc85xxAckOut,
    (int(*)())i2cCycleMpc85xxKnownState,
    (void(*)())sysMpc85xxMsDelay
};

/* driver/controller routines table, indexed by the "I2C_DRV_TYPE" */

LOCAL i2cDrvRoutines_t *i2cDrvRoutinesTables[] =
{
    (i2cDrvRoutines_t *)&i2cDrvRoutinesTableMpc85xx /* index 0 */
};

LOCAL I2C_DRV_CTRL i2C1DrvCtrl ;
LOCAL I2C_DRV_CTRL i2C2DrvCtrl ;
I2C_DRV_CTRL * pI2cDrvCtrl[2] = { NULL, NULL } ;


/*******************************************************************************
*
* i2cIoctl - perform I2C read/write operations
*
* The purpose of this function is to perform i2cIn/OutByte operations
* with synchronization.
*
* RETURNS: data written or data read
*
* ERRNO: N/A
*/

UINT8 i2cIoctl
    (
    UINT32 ioctlflg,    /* input/output control flag */
                        /* 0, write */
                        /* 1, read */
                        /* 2, read/modify/write (ORing) */
                        /* 3, read/modify/write (ANDing) */
                        /* 4, read/modify/write (AND/ORing) */
    UINT32  address,    /* address of device register to be operated upon */
    UINT8   bdata1,     /* data item 1 for read/write operation */
    UINT8   bdata2      /* data item 2 for read/write operation */
    )
    {
    UINT8 u8temp;

    sysMpc85xxMsDelay(1);

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
        sysMpc85xxMsDelay(1);

        i2cOutByte(address, u8temp);
        }
    else if (ioctlflg == I2C_IOCTL_RMW_AND)     /* ANDing */
        {
        u8temp = i2cInByte(address);
        u8temp &= bdata1;
        sysMpc85xxMsDelay(1);

        i2cOutByte(address, u8temp);
        }
    else if (ioctlflg == I2C_IOCTL_RMW_AND_OR)  /* AND/ORing */
        {
        u8temp = i2cInByte(address);
        u8temp &= bdata1;
        u8temp |= bdata2;
        sysMpc85xxMsDelay(1);

        i2cOutByte(address, u8temp);
        }

    sysMpc85xxMsDelay(1);

    return bdata1;
    }


/*******************************************************************************
*
* i2cDrvInit - initialize I2C device controller
*
* This function initializes the I2C device controller device for operation.
* This function should only be executed once during system initialization
* time.
*
* NOTE: no printf or logMsg statements should be called during this routine
* since it is called in sysHwInit(). If output is desired a polled or debug
* dump routine should be used.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

STATUS i2cDrvInit
    (
    int unit,
    int i2cControllerType       /* I2C controller type */
    )
    {
    /*
     * Check for unknown controller type, and initialize I2C controller
     * for operation (if needed).  Note: a switch statement does not work here if
     * executing from ROM due to branch history table creation.
     */

    if (pI2cDrvCtrl[unit] == NULL)
        {
        if (unit == 0)
            {
            pI2cDrvCtrl[unit] = &i2C1DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = M85XX_I2C1_BASE;
            }
        else if (unit == 1)
            {
            pI2cDrvCtrl[unit] = &i2C2DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = M85XX_I2C2_BASE;
            }

        pI2cDrvCtrl[unit]->baseAdrs += CCSBAR;
        }


    /* disable the I2C module, set the device to Master Mode */
    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
         ((UINT8)~MPC85XX_I2C_CONTROL_REG_MEN),
         MPC85XX_I2C_CONTROL_REG_MSTA);

    /* initialize and enable the I2C interface */

    /*
     * I2C clock = CCB / 2 = 200MHz -> / divider
     */
    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_FREQ_DIV_REG),
         ((UINT8)~MPC85XX_I2C_FREQ_DIV_REG_MASK),
         MPC85XX_I2C_FREQ_DIVIDER);

    /* set the slave address */
    i2cIoctl(I2C_IOCTL_RMW_AND_OR,
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_ADR_REG),
         ((UINT8)~MPC85XX_I2C_ADDRESS_REG_MASK), 1);

    /* enable the interface */
    i2cIoctl(I2C_IOCTL_RMW_OR,
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_CONTROL_REG),
         MPC85XX_I2C_CONTROL_REG_MEN, 0);

    /*
     * set the device to slave mode.  This is required for
     * clearing a BUS BUSY lockup condition.
     */

    i2cIoctl(I2C_IOCTL_RMW_AND,
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_CONTROL_REG),
         ((UINT8)~MPC85XX_I2C_CONTROL_REG_MSTA), 0);

    return OK;
    }


/*******************************************************************************
*
* i2cRead - read blocks from I2C
*
* This function's purpose is to read the specified number of
* blocks from the specified device.
*
* RETURNS: OK, or ERROR on a bad request
*
* ERRNO: N/A
*/

int i2cRead
    (
    int          unit,
    UINT32       deviceAddress, /* Device's I2C bus address */
    int          deviceType,
    unsigned int startBlk,      /* starting block to read or register to read */
    unsigned int numBlks,       /* number of blocks to read or single/double byte register */
    char *       pBuf           /* pointer to buffer to receive data */
    )
    {
    int localStatus;            /* local status variable */
    i2cCmdPckt_t i2cCmdPacket;  /* command packet */

    /* check to see if the driver's been installed */

    if (pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit, 2, 3, 4, 5, 6);
        return ERROR ;
        }

    /* check for a bad request */

    if (!numBlks)
        {
        return ERROR;
        }

    /* build command packet */

    i2cCmdPacket.command = I2C_READOP;
    i2cCmdPacket.status = 0;
    i2cCmdPacket.memoryAddress = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber = startBlk;
    i2cCmdPacket.nBlocks = numBlks;
    i2cCmdPacket.eCount = numBlks;
    i2cCmdPacket.aCount = 0;
    i2cCmdPacket.deviceType = deviceType ;

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

#ifdef I2C_DRIVER_DEBUG
    logMsg("command          = %08x\r\n", i2cCmdPacket.command, 2,3,4,5,6);
    logMsg("status           = %08x\r\n", i2cCmdPacket.status, 2,3,4,5,6);
    logMsg("memory address   = %08x\r\n", i2cCmdPacket.memoryAddress, 2,3,4,5,6);
    logMsg("block number     = %08x\r\n", i2cCmdPacket.blockNumber, 2,3,4,5,6);
    logMsg("number of blocks = %08x\r\n", i2cCmdPacket.nBlocks, 2,3,4,5,6);
    logMsg("expected count   = %08x\r\n", i2cCmdPacket.eCount, 2,3,4,5,6);
    logMsg("actual count     = %08x\r\n", i2cCmdPacket.aCount, 2,3,4,5,6);
#endif

    /* return the appropriate status */
    if (i2cCmdPacket.status != 0)
        {
        logMsg("[i2cRead]: i2c command status returns 0x%08x\n", i2cCmdPacket.status, 2,3,4,5,6);
        localStatus = ERROR ;
        }
    else
        localStatus = OK ;

    return localStatus;
    }


/*******************************************************************************
*
* i2cWrite - read blocks from I2C
*
* This function purpose is to write the specified number of
* blocks to the specified device.
*
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/

int i2cWrite
    (
    int          unit,
    UINT32       deviceAddress, /* Device's I2C bus address */
    int          deviceType,
    unsigned int startBlk,      /* starting block to write */
    unsigned int numBlks,       /* number of blocks to write */
    char *       pBuf           /* pointer to buffer of send data */
    )
    {
    int localStatus;            /* local status variable */
    i2cCmdPckt_t i2cCmdPacket;  /* command packet */

    /* check to see if the driver's been installed */

    if (pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* check for a NOP request */

    if (!numBlks)
        {
        return ERROR;
        }

    /* build command packet */

    i2cCmdPacket.command = I2C_WRITOP;
    i2cCmdPacket.status = 0;
    i2cCmdPacket.memoryAddress = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber = startBlk;
    i2cCmdPacket.nBlocks = numBlks;
    i2cCmdPacket.eCount = numBlks;
    i2cCmdPacket.aCount = 0;
    i2cCmdPacket.deviceType = deviceType ;

    /* take ownership, call driver, release ownership */

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

#ifdef I2C_DRIVER_DEBUG
    logMsg("command          = %08x\r\n", i2cCmdPacket.command, 2,3,4,5,6);
    logMsg("status           = %08x\r\n", i2cCmdPacket.status, 2,3,4,5,6);
    logMsg("memory address   = %08x\r\n", i2cCmdPacket.memoryAddress, 2,3,4,5,6);
    logMsg("block number     = %08x\r\n", i2cCmdPacket.blockNumber, 2,3,4,5,6);
    logMsg("number of blocks = %08x\r\n", i2cCmdPacket.nBlocks, 2,3,4,5,6);
    logMsg("expected count   = %08x\r\n", i2cCmdPacket.eCount, 2,3,4,5,6);
    logMsg("actual count     = %08x\r\n", i2cCmdPacket.aCount, 2,3,4,5,6);
#endif

    /* return the appropriate status */

    if (i2cCmdPacket.status != 0)
        {
        logMsg("i2cCmdPacket.status - 0x%x\n", i2cCmdPacket.status, 2,3,4,5,6);
        localStatus = ERROR ;
        }
    else
        localStatus = OK ;

    return localStatus ;
    }

/*******************************************************************************
*
* i2cDoOpDS1374 - execute commands to the AT24C256 I2C device
*
* This function executes the command operations specified by the command
* packet.
*
* RETURNS: OK, or ERROR
*
* NOTE: This driver only uses random byte reads and writes.
* It can be upgraded to use page mode which is faster.
*
* ERRNO: N/A
*/

LOCAL int i2cDoOpDS1374
    (
    int             unit,
    UINT32          deviceAddress,      /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket       /* pointer to command packet */
    )
    {
    i2cDrvRoutines_t *pRoutine; /* low level routines table pointer */    
    int byteCount;              /* byte counter */
    int statusVariable;         /* local status variable */
    unsigned char *pWriteData;  /* pointer to write data buffer */

    /* Initialize pointer to driver routines table. */
    pRoutine = i2cDrvRoutinesTables[I2C_DRV_TYPE];

    if (pI2cCmdPacket->command == I2C_READOP)
        {
        /* Read the specified number of bytes from the EEPROM. */
        /* NOTE: random read has a dummy write first. */
        statusVariable = 0;

        if (I2C_KNOWN_STATE(unit))
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
            if (I2C_CYCLE_WRITE(unit, ((pI2cCmdPacket->blockNumber + byteCount) & 0xff)))
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
            if (I2C_CYCLE_READ(unit, (unsigned char *)pI2cCmdPacket->memoryAddress + byteCount, 0))
                {
                statusVariable = I2C_ERROR_CYCLE_READ;
                break;
                }
            if (I2C_CYCLE_STOP(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_STOP;
                break;
                }

            /* Increment the actual count of the command packet. */
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

        /* Write the specified number of bytes from the EEPROM. */
        statusVariable = 0;

        if (I2C_KNOWN_STATE(unit))
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
            /* device address */
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
            if (I2C_CYCLE_WRITE(unit, ((pI2cCmdPacket->blockNumber + byteCount) & 0xff)))
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

            /* Increment the actual count of the command packet. */
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
        return ERROR;
        }

    /* Leave the I2C bus in a known state. */
    I2C_KNOWN_STATE(unit);

    /*
     * update the caller's command packet with status of
     * the operation
     */
    pI2cCmdPacket->status = statusVariable;

    return OK;
    }

/*******************************************************************************
*
* i2cDoOp - execute I2C do operation
*
* This function executes the operation as specified
* by the passed command packet.  Currently, the only device types
* that are recognized are the ATMEL AT24C256, Analog Devices AD7417ARU
* and PCA9555.
*
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/

int i2cDoOp
    (
    int             unit,
    UINT32          deviceAddress,      /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket       /* pointer to command packet */
    )
    {
#ifdef I2C_DRIVER_DEBUG
    logMsg("i2cDoOp: deviceAddress 0x%x, pI2cCmdPacket 0x%x\n",
            deviceAddress, pI2cCmdPacket, 3, 4, 5, 6);
#endif

    /* Command interface to stop. This is for previous error exits. */
    I2C_CYCLE_STOP(unit);

    switch (pI2cCmdPacket->deviceType)
        {
        case I2C_DEVICE_TYPE_RTC_DS1374:
            if (i2cDoOpDS1374(unit, deviceAddress, pI2cCmdPacket) != OK)
                return ERROR;
            break;
        default:
            logMsg("i2cDoOp: unsupported device type!\n", 0, 1, 2, 3, 4, 5);
            return ERROR;
        }

    return OK;
    }


/*******************************************************************************
*
* i2cAddressMunge - initialize the i2c device driver
*
* This function's purpose is to munge/modify the I2C device address
* based upon the byte offset into the device.  This only applies to
* EEPROM type devices.  Dependent upon the size of the device, A0-A2
* address lines are utilized as 256 byte size page offsets.
*
* RETURNS: munged address
*
* ERRNO: N/A
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
* i2cInByte - reads a byte from I2C Regs
*
* This function reads a byte from an I2C register.
*
* RETURNS: value of byte read
*
* ERRNO: N/A
*/

LOCAL UINT8 i2cInByte
    (
    UINT32 dataPtr
    )
    {
    UINT8 *ptr = (UINT8 *)dataPtr ;
    return (*ptr);
    }

/*******************************************************************************
*
* i2cOutByte - writes a byte to an I2C register
*
* This function writes a byte to an I2C register.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void i2cOutByte
    (
    UINT32 dataPtr,
    UINT8 data
    )
    {
    UINT8 * ptr = (UINT8 *)dataPtr ;
    *ptr = data ;
    }
