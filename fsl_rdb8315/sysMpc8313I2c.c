/* sysMpc8313I2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2005, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,21mar08,z_l Replaced sysMpc8313MsDelay() with sysMsDelay() to prevent
                system hang when rom_resident bootloader is running.
                Modify for code covention.
01d,26jun07,l_g modified for fsl_rdb8313 based on ads834x.
01c,04aug05,dtr Added PCI2 support for PIB.
01b,04apr05,mdo Documentation fixes for apigen
01a,28feb05,dtr created.
*/

/*
DESCRIPTION

I2C Driver Source (Low Level Routines) Module
Mpc8313 Memory Controller (PowerPlus Architecture)

Notes:
   1. GREAT care must be made when sending commands to the 
      MPC8313(Mpc8313).  Several "i2cCycleMpc8313Delay" calls are made
      in order to not overwhelm the I2C interface.  If commands
      are sent too fast, the I2C interface will lockup and the I2C
      bus may become unusable without a powercycle.  Generally, if
      you cause a I2C bus cycle, you should wait for "a while".
      A possible cause is that the caches were turned on when this
      driver was written.

INCLUDE FILES:   sysMpc8313I2c.h
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "sysMotI2c.h"
#include "sysMpc8313I2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

/* externals */

IMPORT I2C_DRV_CTRL * pI2cDrvCtrl[2] ;

/* forward declarations */

void i2cCycleMpc8313Delay (int mSeconds);

char rtcData;

/* Global flag */

int I2CDoRepeatStart = 0;   /* indicates if a "Repeat Start" is requested */

/*******************************************************************************
* 
* i2cCycleMpc8313Start - perform I2C "start" cycle
* 
* This function's purpose is to perform an I2C start cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313Start 
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

    if ( I2CDoRepeatStart == 1 )
        {
        i2cIoctl(I2C_IOCTL_RMW_OR, 
               (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG), 
               (MPC8313_I2C_CONTROL_REG_RSTA |
               MPC8313_I2C_CONTROL_REG_MSTA  |
               MPC8313_I2C_CONTROL_REG_MTX), 0);
        I2CDoRepeatStart = 0;  /* one repeat start only, so clear this bit */
        return (0);
        }

    /* 
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for ( timeOutCount = 1000; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                 (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_STATUS_REG),
                 0, 0);

        if ( (statusReg & MPC8313_I2C_STATUS_REG_MBB) == 0 )
            {
            break;
            }
        }

    if ( timeOutCount == 0 )
        {
        return (-1); 
        }

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */

    i2cIoctl(I2C_IOCTL_RMW_OR, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG), 
            (MPC8313_I2C_CONTROL_REG_MTX |
             MPC8313_I2C_CONTROL_REG_MSTA), 0);

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
* i2cCycleMpc8313Stop - perform I2C "stop" cycle
* 
* This function's purpose is to perform an I2C stop cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313Stop 
    (
    int unit
    )
    {

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * turn off MSTA bit(which will generate a STOP bus cycle)
     * turn off MTX bit(which places the MPC8313 interface into receive mode
     * turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */
     
    i2cIoctl(I2C_IOCTL_RMW_AND, 
             (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG), 
             ((UINT8)~(MPC8313_I2C_CONTROL_REG_MTX  |
             MPC8313_I2C_CONTROL_REG_MSTA |
             MPC8313_I2C_CONTROL_REG_TXAK)), 0);

     /* Clear the global I2C "Repeat Start" flag. */

    I2CDoRepeatStart = 0;

    return (0);

    }

/*******************************************************************************
* 
* i2cCycleMpc8313Read - perform I2C "read" cycle
* 
* This function's purpose is to perform an I2C read cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313Read
    (
    int unit,
    unsigned char *pReadDataBuf,    /* pointer to read data buffer */
    int ack    /* 0 = don't ack, 1 = ack */
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

    if (ack == 0)
        {
        /* Don't send master ack. */
        
        i2cIoctl(I2C_IOCTL_RMW_AND_OR, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG),
            ((UINT8)~MPC8313_I2C_CONTROL_REG_MTX),
              MPC8313_I2C_CONTROL_REG_TXAK);
        }
    else
        {
        /* Send master ack. */
        
        i2cIoctl(I2C_IOCTL_RMW_AND_OR, 
         (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_CONTROL_REG),
         ((UINT8)~(MPC8313_I2C_CONTROL_REG_MTX | MPC8313_I2C_CONTROL_REG_TXAK)),
         0);
        }

    /* do a "dummy read".  this latches the data off the bus. */

    i2cIoctl(I2C_IOCTL_RD, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_DATA_REG), 
            0, 0);

    /* now do the actual read, make this one count */

    readData = i2cIoctl(I2C_IOCTL_RD, 
                  (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_DATA_REG), 
                  0, 0);

    *pReadDataBuf = (unsigned char)readData;

    return (0);

    }


/*******************************************************************************
* 
* i2cCycleMpc8313Write - perform I2C "write" cycle
* 
* This function's purpose is to perform an I2C write cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313Write
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
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_DATA_REG), 
            (UINT32)writeData, 0);

    return (0);
    }


/*******************************************************************************
* 
* i2cCycleMpc8313AckIn - perform I2C "acknowledge-in" cycle
* 
* This function's purpose is to perform an I2C acknowledge-in cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313AckIn 
    (
    int unit
    )
    {
    unsigned int statusReg = 0;
    unsigned int timeOutCount;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * wait until an *internal* device interrupt has been generated, then
     * clear it.  if it is not received, return with an error.
     * we are polling, so NO processor interrupt is generated.
     */

    for ( timeOutCount = 100; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_STATUS_REG), 
                0, 0);

        if ( statusReg & MPC8313_I2C_STATUS_REG_MIF )
            {
            i2cIoctl(I2C_IOCTL_RMW_AND, 
               (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_STATUS_REG), 
               ((UINT8)~MPC8313_I2C_STATUS_REG_MIF), 0);
            break;
            }
        }

    if ( timeOutCount == 0 )
        {
        return (-1);
        }

    return (0);

    }


/*******************************************************************************
* 
* i2cCycleMpc8313AckOut - perform I2C "acknowledge-out" cycle
* 
* This function's purpose is to perform an I2C acknowledge-out cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313AckOut 
    (
    int unit
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    return(0);
    }

/*******************************************************************************
* 
* i2cCycleMpc8313KnownState - initialize the I2C bus to a known state
* 
* This function's purpose is to initialize the I2C bus to a known state.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc8313KnownState 
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

    for ( timeOutCount = 10; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC8313_I2C_STATUS_REG), 
                0, 0);

        if ( (statusReg & MPC8313_I2C_STATUS_REG_MBB) == 0 )
            {
            status = OK;
            break;
            }

        /* 
         * re-initialize the I2C if the BUS BUSY does not clear
         * after trying half the *reasonable* amount of reads of the 
         * status register.
         */

        if ( (timeOutCount % 5) == 0 )
            {
            status = i2cDrvInit(unit, I2C_DRV_TYPE);
            if ( status == OK )
                break;
            else
                return ERROR ;
            }
        }

    if ( timeOutCount == 0 )
        status = ERROR;

    return(status);
    }

/*******************************************************************************
* 
* i2cCycleMpc8313Delay - perform interface's I2C delay routine 
* 
* This function's purpose is to perform whatever delay required for the device.
* 
* RETURNS:  N/A
*
* ERRNO
*/
void i2cCycleMpc8313Delay
    (
    int mSeconds    /* time to delay in milliseconds */
    )
    {    
    sysMsDelay(mSeconds);
    }

/*******************************************************************************
* 
* ds1339I2cRead - perform interface's I2C read for DS1339RTC 
* 
* This function's purpose is to perform I2C read for DS1339RTC.
* 
* RETURNS:  data read from I2C
*
* ERRNO
*/

unsigned char ds1339I2cRead
    (
    unsigned char deviceadd,
    unsigned char reg
    )
    {
    char rtcDatause;
    
    i2cRead (0, deviceadd, I2C_DEVICE_TYPE_RTC_DS1339, (unsigned int)reg, 1, 
             &rtcDatause);

    return ((unsigned char) rtcDatause);
    }

/*******************************************************************************
* 
* ds1339I2cWrite - perform interface's I2C write for DS1339RTC 
* 
* This function's purpose is to perform I2C write for DS1339RTC.
* 
* RETURNS:  N/A
*
* ERRNO
*/

void ds1339I2cWrite
    (
    unsigned char deviceadd,
    unsigned char reg,
    unsigned char val
    )
    {
    i2cWrite (0, deviceadd, I2C_DEVICE_TYPE_RTC_DS1339, (unsigned int)reg, 1, 
              (char *)&val);
    }

