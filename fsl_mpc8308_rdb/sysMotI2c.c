/* sysMotI2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2000, 2002, 2005, 2007-2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 1996-2000 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01g,30jun11,e_d  added EEPROM and GPIO device support.
01f,28mar08,z_l  modified for code convention. fixed "make man" error.
01e,26jun07,l_g  modified for fsl_rdb8313.
01d,03aug05,dtr  Compiler warnings removed.
01c,21may02,gtf  modified for Raytheon NetFires bsp.
01b,10mar00,rhk  changed over to use sysCalcBusSpd, removed 100MHz speed.
01a,28feb00,rhk  created from version 01d, MV2100 BSP.
*/

/*
DESCRIPTION
This file contains generic functions to read/write an I2C device.
Currently the file only supports the Mpc8313 I2C interface.
However, additional I2C bus "controllers can be easily be added
as required.
*/

/* includes */

#include <vxWorks.h>        /* vxWorks generics */
#include "config.h"
#include <ioLib.h>          /* input/output generics */
#include <blkIo.h>          /* block input/output specifics */
#include <semLib.h>         /* semaphore operations */
#include <cacheLib.h>       /* cache control */
#include <intLib.h>         /* interrupt control */
#include <semLib.h>
#include "sysMotI2c.h"      /* driver specifics */
#include "sysMpc8313I2c.h"  /* Mpc8313 I2C Driver Header Module */
#include <stdio.h>
#include <logLib.h>
#include <stdlib.h>

/* defines */

#undef I2C_DRIVER_DEBUG

/* externals */

#ifdef I2C_DRIVER_TESTS
IMPORT int printf();        /* formatted print */
#endif

IMPORT int  sysClkRateGet();    /* system clock rate */
IMPORT int  rawFsDevInit();    /* raw file system device init */
IMPORT void sysMpc8313MsDelay (UINT mSeconds);

/* locals */

LOCAL UINT8 i2cInByte (UINT32);
LOCAL void i2cOutByte (UINT32,UINT8);

/* Driver/controller routines table for the Mpc8313 device. */

i2cDrvRoutines_t i2cDrvRoutinesTableMpc8313 = 
    {
    (int(*)())  i2cCycleMpc8313Start,
    (int(*)())  i2cCycleMpc8313Stop,
    (int(*)())  i2cCycleMpc8313Read,
    (int(*)())  i2cCycleMpc8313Write,
    (int(*)())  i2cCycleMpc8313AckIn,
    (int(*)())  i2cCycleMpc8313AckOut,
    (int(*)())  i2cCycleMpc8313KnownState,
    (void(*)()) i2cCycleMpc8313Delay
    };

/* driver/controller routines table, indexed by the "I2C_DRV_TYPE". */

i2cDrvRoutines_t *i2cDrvRoutinesTables[] = 
    {
    (i2cDrvRoutines_t *)&i2cDrvRoutinesTableMpc8313    /* index 0 */
    };

I2C_DRV_CTRL    i2C1DrvCtrl ;
I2C_DRV_CTRL    i2C2DrvCtrl ;
I2C_DRV_CTRL *  pI2cDrvCtrl[2] = { NULL, NULL } ;

/*******************************************************************************
*
* i2cIoctl - i2cIn/OutByte and/or-ing wrapper.
*
* The purpose of this function is to perform and, or and
* and/or i2cIn/OutByte operations with synchronization.
*
* INPUT: 
*    ioctlflg - input/ouput control flag
*        0, write
*        1, read
*        2, read/modify/write (ORing)
*        3, read/modify/write (ANDing)
*        4, read/modify/write (AND/ORing)
*
* RETURNS: UINT32, for read operations.
*/

UINT8 i2cIoctl
    (
    UINT32  ioctlflg, /* input/ouput control flag */
    UINT32  address,  /* address of device register to be operated upon */
    UINT8   bdata1,   /* data item 1 for read/write operation */
    UINT8   bdata2    /* data item 2 for read/write operation */
    )
    {
    UINT8 u8temp;

#ifdef I2C_DRIVER_DEBUG
    logMsg("i2cIoctl: adrs - 0x%x.\n", address,2,3,4,5,6);
#endif /* I2C_DRIVER_DEBUG */

    i2cCycleMpc8313Delay(1);

    if ( ioctlflg == I2C_IOCTL_WR )             /* write */
        {
        i2cOutByte (address, bdata1);
        }
    else if ( ioctlflg == I2C_IOCTL_RD )        /* read */
        {
        bdata1  = i2cInByte (address);
        }
    else if ( ioctlflg == I2C_IOCTL_RMW_OR )    /* ORing */
        {
        u8temp  = i2cInByte (address);
        u8temp |= bdata1;
        i2cCycleMpc8313Delay (1);

        i2cOutByte (address, u8temp);
        }
    else if ( ioctlflg == I2C_IOCTL_RMW_AND )   /* ANDing */
        {
        u8temp  = i2cInByte (address);
        u8temp &= bdata1;
        i2cCycleMpc8313Delay (1);

        i2cOutByte (address, u8temp);
        }
    else if ( ioctlflg == I2C_IOCTL_RMW_AND_OR ) /* AND/ORing */
        {
        u8temp  = i2cInByte (address);
        u8temp &= bdata1;
        u8temp |= bdata2;
        i2cCycleMpc8313Delay (1);

        i2cOutByte (address, u8temp);
        }

    i2cCycleMpc8313Delay (1);

    return(bdata1);
    }

/******************************************************************************
*
* i2cDrvInit - initialize the i2c device
*
* This function's purpose is to the initialize the I2C device
* controller device for operation.  This function should only
* be executed once during system initialization time.
*
* NOTE: not printf or logMsg statements here because called during sysHwInit().
* If output is desired a pollled or debug dump routine should be used.
*
* RETURNS: OK, or ERROR if not prpmc600 board.
*/

STATUS i2cDrvInit
    (
    int unit,
    int i2cControllerType      /* I2C controller type */
    )
    {
    UINT8 divider ;

    /*
     * Check for unknown controller type, and initialize I2C controller
     * for operation (if needed). 
     * Note: a switch statement will not work here if
     * executing from ROM due to branch history table creation.
     */

    if (pI2cDrvCtrl[unit] == NULL)
        {
        if (unit == 0)
            {
            pI2cDrvCtrl[unit] = &i2C1DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = M8313_I2C1_BASE ;
            }
        else if (unit == 1)
            {
            pI2cDrvCtrl[unit] = &i2C2DrvCtrl ;
            pI2cDrvCtrl[unit]->baseAdrs = M8313_I2C2_BASE ;
            }

        pI2cDrvCtrl[unit]->baseAdrs += CCSBAR ;
        }

    /* disable the I2C module, set the device to Master Mode */
    
    i2cIoctl (I2C_IOCTL_RMW_AND_OR, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG),
              ((UINT8)~MPC8313_I2C_CONTROL_REG_MEN), 
              MPC8313_I2C_CONTROL_REG_MSTA);

    /* initialize and enable the I2C interface */

    switch (sysClkFreqGet())
        {
        case 133000000:
            {
            divider = 0x33; 
            break;
            }
        default:
            {
            divider = 0x9;
            }   
        }

    i2cIoctl (I2C_IOCTL_WR, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_FREQ_DIV_REG),
              divider, 
              divider); 

    i2cIoctl (I2C_IOCTL_WR, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_DIG_FILTER_REG),
              0x10, 
              0x10); 

    /* set the slave address */

    i2cIoctl (I2C_IOCTL_RMW_AND_OR, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_ADR_REG),
              ((UINT8)~MPC8313_I2C_ADDRESS_REG_MASK), 0xfe);

    /* enable the interface */

    i2cIoctl (I2C_IOCTL_RMW_OR, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG), 
              MPC8313_I2C_CONTROL_REG_MEN, 0);

    /*
     * set the device to slave mode.  This is required for
     * clearing a BUS BUSY lockup condition.
     */

    i2cIoctl (I2C_IOCTL_RMW_AND, 
              (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG),
              ((UINT8)~MPC8313_I2C_CONTROL_REG_MSTA), 0);

    return(OK);
}

/******************************************************************************
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
    int    unit,
    UINT32 deviceAddress,  /* Device's I2C bus address */
    int    deviceType,
    UINT32 startBlk,       /* starting block to read or register to read */
    UINT32 numBlks, /* number of blocks to read or single/double byte register */
    char * pBuf            /* pointer to buffer to receive data */
    )
    {
    int             localStatus;    /* local status variable */
    i2cCmdPckt_t    i2cCmdPacket;   /* command packet */

    /* Check to see if the driver's been installed */

    if(pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit,2,3,4,5,6);
        return ERROR ;
        }

    /* Check for a bad request. */

    if ( numBlks == 0 )
        {
        return(ERROR);
        }

    /* Build command packet. */

    i2cCmdPacket.command        = I2C_READOP;
    i2cCmdPacket.status         = 0;
    i2cCmdPacket.memoryAddress  = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber    = startBlk;
    i2cCmdPacket.nBlocks        = numBlks;
    i2cCmdPacket.eCount         = numBlks;
    i2cCmdPacket.aCount         = 0;
    i2cCmdPacket.deviceType     = deviceType ;

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

#ifdef I2C_DRIVER_DEBUG
    logMsg("command          =%08X\r\n", i2cCmdPacket.command,2,3,4,5,6);
    logMsg("status           =%08X\r\n", i2cCmdPacket.status,2,3,4,5,6);
    logMsg("memory address   =%08X\r\n", i2cCmdPacket.memoryAddress,2,3,4,5,6);
    logMsg("block number     =%08X\r\n", i2cCmdPacket.blockNumber,2,3,4,5,6);
    logMsg("number of blocks =%08X\r\n", i2cCmdPacket.nBlocks,2,3,4,5,6);
    logMsg("expected count   =%08X\r\n", i2cCmdPacket.eCount,2,3,4,5,6);
    logMsg("actual count     =%08X\r\n", i2cCmdPacket.aCount,2,3,4,5,6);
#endif

    /* Return the appropriate status. */

    if ( i2cCmdPacket.status != 0 )
        {
        logMsg("i2cCmdPacket.status - 0x%x\n",i2cCmdPacket.status,2,3,4,5,6);
        localStatus = ERROR ;
        }
    else
        localStatus = OK ;

    return(localStatus);
    }

/******************************************************************************
*
* i2cWrite - write blocks through i2c
*
* This function's purpose is to write the specified number of
* blocks to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cWrite
    (
    int          unit,           /* device unit number */
    UINT32       deviceAddress,  /* device's I2C bus address */
    int          deviceType,     /* device type */
    unsigned int startBlk,       /* starting block to write */
    unsigned int numBlks,        /* number of blocks to write */
    char *       pBuf            /* pointer to buffer of send data */
    )
    {
    int localStatus;        /* local status variable */

    i2cCmdPckt_t i2cCmdPacket;    /* command packet */

    /* Check to see if the driver's been installed */

    if(pI2cDrvCtrl[unit]->baseAdrs == 0)
        {
        logMsg("I2C driver for unit %d not initialized.\n", unit,2,3,4,5,6);
        return ERROR ;
        }

    /* Check for a NOP request. */

    if ( numBlks == 0 )
        {
        return(ERROR);
        }

    /* Build command packet. */

    i2cCmdPacket.command        = I2C_WRITOP;
    i2cCmdPacket.status         = 0;
    i2cCmdPacket.memoryAddress  = (unsigned int)pBuf;
    i2cCmdPacket.blockNumber    = startBlk;
    i2cCmdPacket.nBlocks        = numBlks;
    i2cCmdPacket.eCount         = numBlks;
    i2cCmdPacket.aCount         = 0;
    i2cCmdPacket.deviceType     = deviceType ;

    /* Take ownership, call driver, release ownership. */

    localStatus = i2cDoOp(unit, deviceAddress, (i2cCmdPckt_t *)&i2cCmdPacket);

#ifdef I2C_DRIVER_DEBUG
    logMsg("command          =%08X\r\n", i2cCmdPacket.command,2,3,4,5,6);
    logMsg("status           =%08X\r\n", i2cCmdPacket.status,2,3,4,5,6);
    logMsg("memory address   =%08X\r\n", i2cCmdPacket.memoryAddress,2,3,4,5,6);
    logMsg("block number     =%08X\r\n", i2cCmdPacket.blockNumber,2,3,4,5,6);
    logMsg("number of blocks =%08X\r\n", i2cCmdPacket.nBlocks,2,3,4,5,6);
    logMsg("expected count   =%08X\r\n", i2cCmdPacket.eCount,2,3,4,5,6);
    logMsg("actual count     =%08X\r\n", i2cCmdPacket.aCount,2,3,4,5,6);
#endif

    /* Return the appropriate status. */

    if ( i2cCmdPacket.status != 0 )
        {
        logMsg("i2cCmdPacket.status - 0x%x\n",i2cCmdPacket.status,2,3,4,5,6);
        localStatus = ERROR ;
        }
    else
        localStatus = OK ;

    return localStatus ;
    }

/*******************************************************************************
*
* i2cDoOpEeprom24c256 - write or read eeprom through i2c
*
* This function's purpose is to write or read the specified number of
* reg to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cDoOpEeprom24c256
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket   /* pointer to command packet */
    )
    {
    i2cDrvRoutines_t *pRoutine; /* low level routines table pointer */
    int byteCount;              /* byte counter */
    int statusVariable;         /* local status variable */
    unsigned char *pWriteData;  /* pointer to write data buffer */
    UINT8 byteoffset;

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
            byteoffset = (UINT8)(0x7F &
                                 ((pI2cCmdPacket->blockNumber + byteCount) >> 8));

            if (I2C_CYCLE_WRITE(unit, (byteoffset & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            byteoffset = (UINT8)(0xff &
                                 (pI2cCmdPacket->blockNumber + byteCount));

            if (I2C_CYCLE_WRITE(unit, (byteoffset & 0xff)))
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
            byteoffset = (UINT8)(0x7F &
                                 ((pI2cCmdPacket->blockNumber + byteCount) >> 8));
            if (I2C_CYCLE_WRITE(unit, (byteoffset & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if (I2C_CYCLE_ACKIN(unit))
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            byteoffset = (UINT8)(0xFF &
                                 (pI2cCmdPacket->blockNumber + byteCount));

            if (I2C_CYCLE_WRITE(unit, (byteoffset & 0xff)))
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

/******************************************************************************
*
* i2cDoOpDS1339 - write ds1339 through i2c
*
* This function's purpose is to write the specified number of
* reg to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cDoOpDS1339
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket   /* pointer to command packet */
    )
    {
    i2cDrvRoutines_t *  pRoutine;       /* low level routines table pointer */
    int                 byteCount;      /* byte counter */
    int                 statusVariable; /* local status variable */
    unsigned char *     pWriteData;     /* pointer to write data buffer */

    /* Initialize pointer to driver routines table. */
    
    pRoutine = i2cDrvRoutinesTables[I2C_DRV_TYPE];

    if ( pI2cCmdPacket->command == I2C_READOP )
        {
            
        /* Read the specified number of bytes from the EEPROM. */
        /* NOTE: random read has a dummy write first. */
        
        statusVariable = 0;

        if ( I2C_KNOWN_STATE(unit) )
            {
            statusVariable = I2C_ERROR_KNOWN_STATE; 
            byteCount = pI2cCmdPacket->nBlocks;
            }
        for ( byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++ )
            {
            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START; 
                break;
                }
                    
            /* device address - write */
            
            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            
            if ( I2C_CYCLE_WRITE(unit, ((pI2cCmdPacket->blockNumber + \
                                         byteCount) & 0xff)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            
            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START; 
                break;
                }
             
            /* device address - read */
            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress) | 0x1) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            if ( I2C_CYCLE_READ(unit, (unsigned char *) \
                                pI2cCmdPacket->memoryAddress + byteCount, 0))
                {
                statusVariable = I2C_ERROR_CYCLE_READ; 
                break;
                }
            if ( I2C_CYCLE_STOP(unit) )
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

    else if ( pI2cCmdPacket->command == I2C_WRITOP )
        {
            
        /* Initialize pointer to caller's write data buffer. */
        
        pWriteData = (unsigned char *)pI2cCmdPacket->memoryAddress;

        statusVariable = 0;

        if ( I2C_KNOWN_STATE(unit) )
            {
            statusVariable  = I2C_ERROR_KNOWN_STATE; 
            byteCount       = pI2cCmdPacket->nBlocks;
            }
        for ( byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++ )
            {
            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START; 
                break;
                }
            
            
            /* device address */
            
            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress )) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            if ( I2C_CYCLE_WRITE(unit,
                    ((pI2cCmdPacket->blockNumber + byteCount) & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            
            /* write data */
            
            if ( I2C_CYCLE_WRITE(unit, *(pWriteData + byteCount)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE; 
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN; 
                break;
                }
            if ( I2C_CYCLE_STOP(unit) )
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
* i2cDoOpGpioP9536 - write or read PCA9536 through i2c
*
* This function's purpose is to write or read the specified number of
* reg to the specified device.
*
* RETURNS: Number of bytes written, or ERROR if bad request.
*/

int i2cDoOpGpioP9536
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket   /* pointer to command packet */
    )
    {
    i2cDrvRoutines_t *  pRoutine;       /* low level routines table pointer */
    int                 byteCount;      /* byte counter */
    int                 statusVariable; /* local status variable */
    unsigned char *     pWriteData;     /* pointer to write data buffer */

    /* Initialize pointer to driver routines table. */

    pRoutine = i2cDrvRoutinesTables[I2C_DRV_TYPE];

    if ( pI2cCmdPacket->command == I2C_READOP )
        {

        /* Read the specified number of bytes from the EEPROM. */
        /* NOTE: random read has a dummy write first. */

        statusVariable = 0;

        if ( I2C_KNOWN_STATE(unit) )
            {
            statusVariable = I2C_ERROR_KNOWN_STATE;
            byteCount = pI2cCmdPacket->nBlocks;
            }
        for ( byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++ )
            {
            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }

            /* device address - write */

            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if ( I2C_CYCLE_WRITE(unit, ((pI2cCmdPacket->blockNumber + \
                                         byteCount) & 0xff)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }

            /* device address - read */
            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress) | 0x1) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }
            if ( I2C_CYCLE_READ(unit, (unsigned char *) \
                                pI2cCmdPacket->memoryAddress + byteCount, 0))
                {
                statusVariable = I2C_ERROR_CYCLE_READ;
                break;
                }
            if ( I2C_CYCLE_STOP(unit) )
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

    else if ( pI2cCmdPacket->command == I2C_WRITOP )
        {

        /* Initialize pointer to caller's write data buffer. */

        pWriteData = (unsigned char *)pI2cCmdPacket->memoryAddress;

        statusVariable = 0;

        if ( I2C_KNOWN_STATE(unit) )
            {
            statusVariable  = I2C_ERROR_KNOWN_STATE;
            byteCount       = pI2cCmdPacket->nBlocks;
            }
        for ( byteCount = 0; byteCount < pI2cCmdPacket->nBlocks; byteCount++ )
            {
            if ( I2C_CYCLE_START(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_START;
                break;
                }


            /* device address */

            if ( I2C_CYCLE_WRITE(unit, i2cAddressMunge(deviceAddress )) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }
            if ( I2C_CYCLE_WRITE(unit,
                    ((pI2cCmdPacket->blockNumber + byteCount) & 0xff)))
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }

            /* write data */

            if ( I2C_CYCLE_WRITE(unit, *(pWriteData + byteCount)) )
                {
                statusVariable = I2C_ERROR_CYCLE_WRITE;
                break;
                }
            if ( I2C_CYCLE_ACKIN(unit) )
                {
                statusVariable = I2C_ERROR_CYCLE_ACKIN;
                break;
                }
            if ( I2C_CYCLE_STOP(unit) )
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

/******************************************************************************
*
* i2cDoOp - i2c do operation
*
* This function's purpose is to execute the operation as specified
* by the passed command packet.  Currently, the only device types
* that are recognized are the ATMEL AT24C256 and the Analog Devices
* AD7417ARU.
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
#ifdef I2C_DRIVER_DEBUG
    logMsg("i2cDoOp: deviceAddress - 0x%x, pI2cCmdPacket - 0x%x\n",
           deviceAddress, pI2cCmdPacket,3,4,5,6);
#endif

    /* Command interface to stop.  This is for previous error exits. */

    I2C_CYCLE_STOP(unit);

    switch(pI2cCmdPacket->deviceType)
        {
        case I2C_DEVICE_TYPE_RTC_DS1339:
            if ( i2cDoOpDS1339(unit, deviceAddress, pI2cCmdPacket) != OK )
                return ERROR ;
            break;
        case I2C_DEVICE_TYPE_EEPROM_AT24C256:
            if ( i2cDoOpEeprom24c256(unit, deviceAddress, pI2cCmdPacket) != OK )
                return ERROR ;
            break;
        case I2C_DEVICE_TYPE_GPIO_PCA9536:
            if ( i2cDoOpGpioP9536(unit, deviceAddress, pI2cCmdPacket) != OK )
                return ERROR ;
            break;
        default :
            return ERROR ;
        }

    return (OK);
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
*/

unsigned int i2cAddressMunge
    (
    unsigned int deviceAddress
    )
    {
    return ((deviceAddress << 1) & 0xfe) ;
    }

/* AT24C256 -> 512 pages of 64 bytes each, 15 bits reqd for addressing */
/* 64 bytes -> address masked with 0xffc0 */
/* lsb of device address is for 0/1 read/write */

/*******************************************************************************
*
* i2cInByte - reads a byte from I2C Regs.
*
* This function reads a longword from a specified PCI I/O or Memory address
* via the PCI bridge chip.  This function should be used for access
* to the I/O or Memory mapped registers of a PCI device.  These
* registers are mapped as little-endian, so we byte reverse the data in order
* to make the value returned look the same as it would in PCI space.
*
* RETURNS: longword from address.
*
*/

LOCAL UINT8 i2cInByte 
    (
    UINT32 dataPtr
    )
    {
    /* Read a longword from the address, and reverse the bytes */

    UINT8 *ptr = (UINT8*)dataPtr ;

    return (*ptr);
    }

/*******************************************************************************
*
* i2cOutByte - writes a byte to I2C Regs.
*
* This function writes a longword to a specified PCI I/O or Memory address
* via the PCI bridge chip.  This function should be used for access
* to the I/O or Memory mapped registers of a PCI device.  These
* registers are mapped as little-endian, so we byte reverse the data in order
* to make the value written correct for the registers in PCI space.
*
* RETURNS: N/A
*/

LOCAL void i2cOutByte
    (
    UINT32  dataPtr, 
    UINT8   data
    )
    {
    UINT8 * ptr = (UINT8*)dataPtr ;

    *ptr = data ;
    }
