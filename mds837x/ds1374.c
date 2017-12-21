/* ds1374.c - Dallas DS1374 RTC driver */

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
01a,08jun07,b_m  written.
*/

/*
DESCRIPTION
This file provides the Dallas DS1374 RTC driver for Freescale MDS8568 board.

The driver simply supplies two RTC access routines, one for getting time and the
other one for setting time. Advanced features like time converting, os library
hooking are not supported in this driver. The upper layer software can make use
of the two low level apis to implement such features.

The driver is tested on MDS8568 board. It might work on other boards if all the
i2c settings are correct.

INCLUDE FILES:

SEE ALSO:
*/

/* includes */

#include <vxWorks.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "sysMotI2c.h"

/* DS1374 Registers */

#define DS1374_TOD_COUNTER_BYTE0    0x00
#define DS1374_TOD_COUNTER_BYTE1    0x01
#define DS1374_TOD_COUNTER_BYTE2    0x02
#define DS1374_TOD_COUNTER_BYTE3    0x03
#define DS1374_WD_ALM_COUNTER_BYTE0 0x04
#define DS1374_WD_ALM_COUNTER_BYTE1 0x05
#define DS1374_WD_ALM_COUNTER_BYTE2 0x06
#define DS1374_CONTROL              0x07
#define DS1374_STATUS               0x08
#define DS1374_TRICKLE_CHARGER      0x09

/* MDS8568 Specific Settings */

#define MDS8568_DS1374_I2C_BUS      0
#define MDS8568_DS1374_I2C_ADDR     0x68

/* register access routines */

#define DS1374_REG_READ(addr, data) \
    do  \
    {   \
        i2cRead(MDS8568_DS1374_I2C_BUS, \
                MDS8568_DS1374_I2C_ADDR,    \
                I2C_DEVICE_TYPE_RTC_DS1374, \
                addr,   \
                1,  \
                &data); \
    } while(0);

#define DS1374_REG_WRITE(addr, data)    \
    do  \
    {   \
        i2cWrite(MDS8568_DS1374_I2C_BUS,    \
                MDS8568_DS1374_I2C_ADDR,    \
                I2C_DEVICE_TYPE_RTC_DS1374, \
                addr,   \
                1,  \
                &data); \
    } while(0);

/*******************************************************************************
*
* sysDS1374GetTime - get time from RTC
*
* This routine is called to get the seconds elapsed since 1970/01/01 from RTC.
*
* RETURNS: the seconds elapsed since 1970/01/01
*
* ERRNO: N/A
*/

time_t sysDS1374GetTime
    (
    void
    )
    {
    UINT8  data = 0;
    time_t seconds = 0;

    DS1374_REG_READ(DS1374_TOD_COUNTER_BYTE3, data);
    seconds |= (data << 24);
    DS1374_REG_READ(DS1374_TOD_COUNTER_BYTE2, data);
    seconds |= (data << 16);
    DS1374_REG_READ(DS1374_TOD_COUNTER_BYTE1, data);
    seconds |= (data << 8);
    DS1374_REG_READ(DS1374_TOD_COUNTER_BYTE0, data);
    seconds |= data;

    printf("RTC seconds = 0x%08x\n", seconds);

	return seconds;
    }

/*******************************************************************************
*
* sysDS1374SetTime - set time to RTC
*
* This routine is called to set the seconds to RTC registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysDS1374SetTime
    (
    time_t seconds
    )
    {
    UINT8 data = 0;

    data = (UINT8)(seconds >> 24);
    DS1374_REG_WRITE(DS1374_TOD_COUNTER_BYTE3, data);
    data = (UINT8)(seconds >> 16);
    DS1374_REG_WRITE(DS1374_TOD_COUNTER_BYTE2, data);
    data = (UINT8)(seconds >> 8);
    DS1374_REG_WRITE(DS1374_TOD_COUNTER_BYTE1, data);
    data = (UINT8)(seconds);
    DS1374_REG_WRITE(DS1374_TOD_COUNTER_BYTE0, data);
    }
