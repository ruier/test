/* ixp400I2c.c - Intel IXP400 I2C source file */

/*
 * Copyright (c) 2002, 2004, 2005, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 *
 * Certain portions of these files may be copyright (c) 2002-2005 Intel
 * Corporation.
 */

/*
modification history
--------------------
01f,18jun08,mdo  fix apigen errors
01e,08oct07,mdo  Driver needs to get its header file from the target/h/drv/i2c
01d,28feb05,pai  Picked up Intel mods to init functions.
01c,26jan05,pai  Reorganized C-preprocessor use.
01b,20dec04,pai  Updated copyright notice.
01a,05jun02,jb  initial version...
*/

/*
DESCRIPTION
This is a driver for the Intel IXP400 I2C bus protocol.  When the
INCLUDE_I2C_HW_CTRL BSP configuration constant is defined, the routines
in this module support the IXP465 I2C controller.  Otherwise, this is a
software only implementation which uses two pins on the IXP400 GPIO
Controller.

USAGE
An example read from an I2C device using the individual protocol functions
is shown below:
\cs
    ...
    {
    if (OK == ixp400I2CStart ())
        {
        ixp400I2CByteTransmit ((devAddr & IXP400_I2C_WRITE_MSK));
        ixp400I2CAckReceive();

        ixp400I2CByteTransmit (offset);
        ixp400I2CAckReceive();

        /@ Switch to read mode @/

        IXP400_I2C_SCL_SET_HIGH;
        IXP400_I2C_SDA_SET_HIGH;

        if(ixp400I2CStart() != OK)
            {
            ixp400I2CStop ();
            return ERROR;
            }

        ixp400I2CByteTransmit ((devAddr | IXP400_I2C_READ_FLAG));
        ixp400I2CAckReceive ();

        for (byteCnt = 0; byteCnt<num; ++byteCnt)
            {
            ixp400I2CByteReceive (Buf);
            ++Buf;

            /@ Prevent giving an ACK on the last byte @/

            if (byteCnt < (num - 1))
                ixp400I2CAckSend ();
            }

        ixp400I2CStop();
        }
    }
    ...
\ce

INCLUDE FILES: ixp400I2c.h

SEE ALSO:

\tb "ixp400 Data Sheet,"
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "ixp400.h"
#include "ixdp400.h"
#include "ixp400Gpio.h"
#include "drv/i2c/ixp400I2c.h"

#include "intLib.h"

#include "IxI2cDrv.h"

/* defines */

/* Microsecond delay for SCL setup and hold times */

#define IXP400_I2C_SCLDELAY sysMicroDelay(10)

/* Data setup time */

#define IXP400_I2C_SDADELAY sysMicroDelay(1)

#define IXP400_I2C_SDA_GET(val) (ixp400GPIOLineGet(IXP400_I2C_SDA, (val)))
#define IXP400_I2C_SCL_GET(val) (ixp400GPIOLineGet(IXP400_I2C_SCL, (val)))

#define IXP400_I2C_SDA_SET_HIGH {\
    ixp400GPIOLineSet(IXP400_I2C_SDA, IXP400_GPIO_HIGH); \
    IXP400_I2C_SDADELAY;}

#define IXP400_I2C_SDA_SET_LOW {\
    ixp400GPIOLineSet(IXP400_I2C_SDA, IXP400_GPIO_LOW); \
    IXP400_I2C_SDADELAY;}

#define IXP400_I2C_SCL_SET_HIGH {\
    ixp400GPIOLineSet(IXP400_I2C_SCL, IXP400_GPIO_HIGH); \
    IXP400_I2C_SCLDELAY;}

#define IXP400_I2C_SCL_SET_LOW {\
    ixp400GPIOLineSet(IXP400_I2C_SCL, IXP400_GPIO_LOW); \
    IXP400_I2C_SCLDELAY;}


/* imports */

extern void sysMicroDelay ();


/* forward declarations */

#ifndef INCLUDE_I2C_HW_CTRL
LOCAL STATUS ixp400I2CBusFree (void);
#endif


/******************************************************************************
*
* ixp400I2CStart - (Control signal)initiate an I2C bus transfer
*
* This routine initiates a transfer on the I2C bus.  This should only be
* called from a master device (i.e. GPIO controller on IXP400).
*
* RETURNS:
* OK when the bus is free, else ERROR if the bus is already in use by
* another task.
*/

STATUS ixp400I2CStart (void)
    {
#ifndef INCLUDE_I2C_HW_CTRL

    int const key = intLock();

    if (ixp400I2CBusFree () == OK)
        {
        ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
        ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

        IXP400_I2C_SCL_SET_HIGH;
        IXP400_I2C_SDA_SET_HIGH;
        IXP400_I2C_SDA_SET_LOW;

        intUnlock (key);
        return (OK);
        }

    intUnlock (key);

#endif

    return (ERROR);
    }

/******************************************************************************
*
* ixp400I2CStop - (Control signal) terminate an I2C bus transfer
*
* This routine terminates a transfer on the I2C bus.  The I2C bus will be
* left in a free state; namely, SCL HIGH and SDA HIGH.  This should only be
* called from a master device (i.e. GPIO controller on IXP400)
*
* RETURNS: N/A
*/

void ixp400I2CStop (void)
    {
#ifndef INCLUDE_I2C_HW_CTRL

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
    ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

    IXP400_I2C_SDA_SET_LOW;
    IXP400_I2C_SCL_SET_HIGH;
    IXP400_I2C_SDA_SET_HIGH;

#endif
    }

/******************************************************************************
*
* ixp400I2CAckSend - (Control signal) send an acknowledgement
*
* This routine sends an acknowledgement on the I2C bus.
*
* RETURNS: N/A
*/

void ixp400I2CAckSend (void)
    {
#ifndef INCLUDE_I2C_HW_CTRL

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
    ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

    IXP400_I2C_SDA_SET_LOW;

    IXP400_I2C_SCL_SET_HIGH;
    IXP400_I2C_SCL_SET_LOW;

    IXP400_I2C_SDA_SET_HIGH;

#endif
    }

/******************************************************************************
*
* ixp400I2CAckReceive - (Control Signal) get an acknowledgement
*
* This routine gets an acknowledgement from the I2C bus.
*
* RETURNS: OK if an acknowledgement is received, else ERROR.
*/

STATUS ixp400I2CAckReceive (void)
    {
    STATUS status = ERROR;

#ifndef INCLUDE_I2C_HW_CTRL

    int retryCnt = 0;

    IXP400_GPIO_SIG sda;

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
    ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

    IXP400_I2C_SDA_SET_HIGH;

    ixp400GPIOLineConfig(IXP400_I2C_SDA, IXP400_GPIO_IN);
    IXP400_I2C_SCL_SET_HIGH;

    do
        {
        IXP400_I2C_SDA_GET (&sda);
        ++retryCnt;
        } while ((sda != IXP400_GPIO_LOW) && (retryCnt < IXP400_I2C_ACK_RTY) );

    IXP400_I2C_SCL_SET_LOW;

    if (sda != IXP400_GPIO_LOW)
        status = ERROR;
    else
        status = OK;

#endif

    return status;
    }

/******************************************************************************
*
* ixp400I2CByteTransmit - transmit a byte on the I2C bus
*
* This routine transmits a specified <dataByte> on the I2C bus.  All
* byte transfers are Most Significant Bit (MSB) first.
*
* RETURNS: N/A
*/

void ixp400I2CByteTransmit
    (
    unsigned char dataByte
    )
    {
#ifndef INCLUDE_I2C_HW_CTRL

    int bitCnt = 0;

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
    ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

    IXP400_I2C_SCL_SET_LOW;

    for (bitCnt = 7; bitCnt >= 0; --bitCnt)
        {
        if (dataByte & BIT (bitCnt))
            {
            IXP400_I2C_SDA_SET_HIGH;
            }
        else
            {
            IXP400_I2C_SDA_SET_LOW;
            }

        IXP400_I2C_SCL_SET_HIGH;
        IXP400_I2C_SCL_SET_LOW;
        }

#endif
    }

/******************************************************************************
*
* ixp400I2CByteReceive - receive a byte on the I2C bus
*
* This routine receives a byte of data from the I2C bus and stores it to
* the location specified by the <dataByte> parameter.  All byte transfers
* are Most Significant Bit (MSB) first.
*
* RETURNS: N/A
*/

void ixp400I2CByteReceive
    (
    unsigned char * dataByte
    )
    {
#ifndef INCLUDE_I2C_HW_CTRL

    IXP400_GPIO_SIG sda = 0;

    unsigned char tmpByte = 0;

    int bitCnt = 0;

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_IN);
    ixp400GPIOLineConfig (IXP400_I2C_SCL, IXP400_GPIO_OUT);

    IXP400_I2C_SCL_SET_LOW;

    for (bitCnt = 7; bitCnt >= 0; --bitCnt)
        {
        IXP400_I2C_SCL_SET_HIGH;

        IXP400_I2C_SDA_GET (&sda);     /* Get the data bit */

        tmpByte |= (sda << bitCnt);
        IXP400_I2C_SCL_SET_LOW;
        }

    ixp400GPIOLineConfig (IXP400_I2C_SDA, IXP400_GPIO_OUT);
    IXP400_I2C_SDA_SET_LOW;

    *dataByte = tmpByte;

#endif
    }

/******************************************************************************
*
* ixp400I2CBusFree - test for a free I2C bus
*
* This routine can be used to determine whether or not the I2C bus is in
* use.
*
* RETURNS: OK if the bus is free, ERROR otherwise.
*/

#ifndef INCLUDE_I2C_HW_CTRL
LOCAL STATUS ixp400I2CBusFree (void)
    {
    STATUS status = ERROR;


    IXP400_GPIO_SIG sda = 0;
    IXP400_GPIO_SIG scl = 0;

    /*
     * Listen in on the data (SDA), and clock (SCL) lines. If both
     * are high then the bus is free.
     */

    IXP400_I2C_SDA_GET (&sda);
    IXP400_I2C_SCL_GET (&scl);

    if((sda == IXP400_GPIO_HIGH) && (scl == IXP400_GPIO_HIGH))
        status = OK;
    else
        status = ERROR;


    return status;
    }
#endif

/******************************************************************************
*
* ixp400I2CWriteTransfer - write to a slave device
*
* This routine writes <num> bytes from <buffer> to <offset> in a slave
* device with address <devAddr>.
*
* RETURNS:
* The number of bytes actually written, which may be zero or less
* than <num> in the event of an error.
*/

int ixp400I2CWriteTransfer
    (
    UINT8   devAddr,
    UINT8 * buffer,
    UINT32  num,
    UINT8   offset
    )
    {
    int     byteCnt = 0;

    if (NULL == buffer)
        return byteCnt;

#ifdef INCLUDE_I2C_HW_CTRL

    for (byteCnt = 0; byteCnt < num; ++byteCnt)
        {
        char tmp[2];

        tmp[0] = offset + byteCnt;
        tmp[1] = buffer[byteCnt];

        if (IX_I2C_SUCCESS != ixI2cDrvWriteTransfer (devAddr, tmp, 2, 0))
            return byteCnt;

        taskDelay (((sysClkRateGet () * 100) / 1000));
        }

#else

    if (ixp400I2CStart () == OK)
        {
        ixp400I2CByteTransmit ((devAddr & IXP400_I2C_WRITE_MSK));

        if(ixp400I2CAckReceive() != OK)
            {
            ixp400I2CStop ();
            return (byteCnt);
            }

        ixp400I2CByteTransmit (offset);

        if (ixp400I2CAckReceive () != OK)
            {
            ixp400I2CStop ();
            return (byteCnt);
            }

        for(byteCnt = 0; byteCnt < num; ++byteCnt)
            {
            ixp400I2CByteTransmit (*buffer);

            if(ixp400I2CAckReceive () != OK)
                {
                ixp400I2CStop ();
                return (byteCnt);
                }

            ++buffer;
            }

        ixp400I2CStop ();
        }
#endif

    return (byteCnt);
    }

/******************************************************************************
*
* ixp400I2CReadTransfer - read from a slave device
*
* This routine reads <num> bytes into <buffer> from <offset> in a slave
* device with address <devAddr>.
*
* RETURNS:
* The number of bytes actually read, which may be zero or less
* than <num> in the event of an error.
*/

int ixp400I2CReadTransfer
    (
    UINT8   devAddr,
    UINT8 * buffer,
    UINT32  num,
    UINT8   offset
    )
    {
    int     byteCnt = 0;

    if (NULL == buffer)
        return byteCnt;

#ifdef INCLUDE_I2C_HW_CTRL

    /* write the slave address and offset, this sets the internal address */

    if (IX_I2C_SUCCESS != ixI2cDrvWriteTransfer (devAddr, &offset, 1, 0))
        return byteCnt;

    /* now go into read mode */

    if (IX_I2C_SUCCESS == ixI2cDrvReadTransfer (devAddr, buffer, num, 0))
        {
        byteCnt = num;
        }

#else

    if (ixp400I2CStart () == OK)
        {
        ixp400I2CByteTransmit ((devAddr & IXP400_I2C_WRITE_MSK));

        if(ixp400I2CAckReceive () != OK)
            {
            ixp400I2CStop ();
            return byteCnt;
            }

        ixp400I2CByteTransmit (offset);

        if (ixp400I2CAckReceive() != OK)
            {
            ixp400I2CStop ();
            return byteCnt;
            }

        /* Switch to read mode */
        {
        int const key = intLock ();

        IXP400_I2C_SCL_SET_HIGH;
        IXP400_I2C_SDA_SET_HIGH;

        intUnlock (key);
        }

        if(ixp400I2CStart () != OK)
            {
            ixp400I2CStop ();
            return byteCnt;
            }

        ixp400I2CByteTransmit ((devAddr | IXP400_I2C_READ_FLAG));

        if(ixp400I2CAckReceive () != OK)
            {
            ixp400I2CStop ();
            return byteCnt;
            }

        for(byteCnt = 0; byteCnt < num; ++byteCnt)
            {
            ixp400I2CByteReceive (buffer);
            ++buffer;

            /* Prevent giving an ACK on the last byte */

            if (byteCnt < (num - 1))
                ixp400I2CAckSend ();
            }

        ixp400I2CStop ();
        }
#endif

    return (byteCnt);
    }


#ifdef INCLUDE_I2C_HW_CTRL

/******************************************************************************
*
* ixp400I2CInit -
*
* RETURNS:
*
* \NOMANUAL
*/

UINT32 ixp400I2CInit (void)
    {
    IxI2cInitVars InitVarsSelected;

    /* Select either normal (100kbps) or fast mode (400kbps) */

    InitVarsSelected.I2cSpeedSelect = IX_I2C_NORMAL_MODE;

    /* Select interrupt or poll mode */

    InitVarsSelected.I2cFlowSelect = IX_I2C_POLL_MODE;

    /* Enable/disable the unit from driving the SCL line during
     * master mode operation
     */

    InitVarsSelected.SCLEnable = 1;

    /* The address the unit will response to as a slave device */

    InitVarsSelected.I2cHWAddr = 0x01;

    return (UINT32) ixI2cDrvInit (&InitVarsSelected);
    }

#endif
