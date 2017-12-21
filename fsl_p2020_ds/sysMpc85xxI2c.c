/* sysMpc85xxI2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2005-2007, 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,14dec11,swu  Added I2C page write and sequential read   
                 for BSP wrSbcP4080. (WIND00290411)
01f,22aug11,swu  Improve I2C performance. (WIND00290411)
01e,10feb09,b_m  make I2CDoRepeatStart LOCAL.
01d,08jun07,b_m  cleanup unnecessary debug routines.
01c,27jan06,dtr  Fix EepromShow and tidyup.
01b,04apr05,mdo  Documentation fixes for apigen
01a,28feb05,dtr  created.
*/

/*
DESCRIPTION

I2C Driver Source (Low Level Routines) Module
MPC85xx Memory Controller (PowerPlus Architecture)

Notes:
   1. GREAT care must be made when sending commands to the
      MPC85XX(Mpc85xx).  Several "i2cCycleMpc85xxDelay" calls are made
      in order to not overwhelm the I2C interface.  If commands
      are sent too fast, the I2C interface will lockup and the I2C
      bus may become unusable without a powercycle.  Generally, if
      you cause a I2C bus cycle, you should wait for "a while".
      A possible cause is that the caches were turned on when this
      driver was written.
   2. Polling changes have only been tested on wrSbcP4080 BSP, with
      no adverse affects. Do not enable OPTIMIZE_I2C elsewhere without 
      sufficient testing.

INCLUDE FILES: sysMpc85xxI2c.h
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

#include "config.h"
#include "sysMotI2c.h"
#include "sysMpc85xxI2c.h"

/* externals */

IMPORT I2C_DRV_CTRL * pI2cDrvCtrl[2];
IMPORT void sysMsDelay(UINT);

#ifdef OPTIMIZE_I2C
IMPORT void sysUsDelay(UINT);
#endif /* OPTIMIZE_I2C */

/* forward declarations */

void sysMpc85xxMsDelay(UINT mSeconds);
void i2cCycleMpc85xxDelay(int mSeconds);

#ifdef OPTIMIZE_I2C
LOCAL int i2cCycleMpc85xxDataTransferred (int unit);
void sysMpc85xxUsDelay(UINT uSeconds);
#endif /* OPTIMIZE_I2C */

/* Global flag */

LOCAL int I2CDoRepeatStart = 0;   /* indicates if a "Repeat Start" is requested */

#ifdef OPTIMIZE_I2C
/*
 * MPC85XX I2C bus busy timeout value in us
 * Note: Macro MPC85XX_I2C_BUSY_TIMEOUT should be modified
 * to fit specific I2C devices according to user manual.
 */

#   define MPC85XX_I2C_BUSY_TIMEOUT 1000
#endif /* OPTIMIZE_I2C */

/*******************************************************************************
*
* i2cCycleMpc85xxStart - perform I2C "start" cycle
*
* This function's purpose is to perform an I2C start cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxStart
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
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
                (MPC85XX_I2C_CONTROL_REG_RSTA | MPC85XX_I2C_CONTROL_REG_MSTA | MPC85XX_I2C_CONTROL_REG_MTX),
                0);
        I2CDoRepeatStart = 0;   /* one repeat start only, so clear this bit */
        return 0;
        }

    /*
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for (timeOutCount = 100; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_STATUS_REG),
                0,
                0);

        if (!(statusReg & MPC85XX_I2C_STATUS_REG_MBB))
            {
            break;
            }

#ifndef OPTIMIZE_I2C
        sysMpc85xxMsDelay(1);
#endif /* OPTIMIZE_I2C */

        }

    if (!timeOutCount)
        {
        return -1;
        }

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */

    i2cIoctl(I2C_IOCTL_RMW_OR,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
            (MPC85XX_I2C_CONTROL_REG_MTX | MPC85XX_I2C_CONTROL_REG_MSTA),
            0);

    /*
     * The first time through, set "I2CDoRepeatStart".  If this function
     * is called again BEFORE a STOP is sent, then we are doing a
     * "dummy write", which sets the devices internal byte pointer
     * to the byte we intend to read.
     */

    I2CDoRepeatStart = 1;
    
#ifdef OPTIMIZE_I2C
    pI2cDrvCtrl[unit] -> I2CFirstReadAfterStart = 1;
#endif

    return 0;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxStop - perform I2C "stop" cycle
*
* This function's purpose is to perform an I2C stop cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxStop
    (
    int unit
    )
    {

#ifdef OPTIMIZE_I2C
	unsigned int timeOutCount = 0;
	int   mbbCl = 0;
    UINT8 statusReg = 0;
#endif /* OPTIMIZE_I2C */

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * turn off MSTA bit(which will generate a STOP bus cycle)
     * turn off MTX bit(which places the MPC85XX interface into receive mode
     * turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */

#ifndef OPTIMIZE_I2C
    sysMpc85xxMsDelay(1);
#endif /* OPTIMIZE_I2C */

    i2cIoctl(I2C_IOCTL_RMW_AND,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
            ((UINT8)~(MPC85XX_I2C_CONTROL_REG_MTX | MPC85XX_I2C_CONTROL_REG_MSTA | MPC85XX_I2C_CONTROL_REG_TXAK)),
            0);

#ifdef OPTIMIZE_I2C

    /*
     * The Bus Busy bit in MPC85XX_I2C_STATUS_REG indicate the status
     * of the bus. If the bus was always busy, then I2C bus should be 
     * re-initialized. 
     * Note: Macro MPC85XX_I2C_BUSY_TIMEOUT should be modified to fit 
     * specific I2C devices according to user manual.
     */

    for (timeOutCount = MPC85XX_I2C_BUSY_TIMEOUT; timeOutCount; timeOutCount--)
        {
    	statusReg = i2cIoctl(I2C_IOCTL_RD, 
    	                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_STATUS_REG), 0, 0);
    	
        if ( !(statusReg & MPC85XX_I2C_STATUS_REG_MBB) )
            {
        	mbbCl = 1;
        	break;
            }
        else
    	    sysMpc85xxUsDelay(1);

        }
    
    if (!mbbCl)
        {
 	    i2cDrvInit( unit, I2C_DRV_TYPE );
        }
#endif /* OPTIMIZE_I2C */

    /*
     * Clear the global I2C "Repeat Start" flag.
     */

    I2CDoRepeatStart = 0;

    return 0;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxRead - perform I2C "read" cycle
*
* This function's purpose is to perform an I2C read cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxRead
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

    if (!ack)
        {
        /* don't send master ack */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
                ((UINT8)~MPC85XX_I2C_CONTROL_REG_MTX),
                MPC85XX_I2C_CONTROL_REG_TXAK);
        }
    else
        {
        /* send master ack */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_CONTROL_REG),
                ((UINT8)~(MPC85XX_I2C_CONTROL_REG_MTX | MPC85XX_I2C_CONTROL_REG_TXAK)),
                0);
        }

#ifndef OPTIMIZE_I2C
    sysMpc85xxMsDelay(1);
#endif /* OPTIMIZE_I2C */

#ifdef OPTIMIZE_I2C
    /* 
     * when we doing a sequential read, only the very first read should be 
     * a "dummy read". 
     */

    if(pI2cDrvCtrl[unit]->I2CFirstReadAfterStart == 1)
    {
#endif /* OPTIMIZE_I2C */
    /* do a "dummy read", this latches the data off the bus */

    i2cIoctl(I2C_IOCTL_RD,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_DATA_REG),
            0,
            0);



#ifdef OPTIMIZE_I2C
    if(i2cCycleMpc85xxDataTransferred(unit))
        {
    	return -1;
        }
    
    pI2cDrvCtrl[unit]->I2CFirstReadAfterStart = 0;
    }
#endif /* OPTIMIZE_I2C */

    /* now do the actual read, make this one count */

#ifndef OPTIMIZE_I2C
    sysMpc85xxMsDelay(1);
#endif /* OPTIMIZE_I2C */

    readData = i2cIoctl(I2C_IOCTL_RD,
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_DATA_REG),
            0,
            0);

#ifdef OPTIMIZE_I2C
    if(i2cCycleMpc85xxDataTransferred(unit))
        {
    	return -1;
        }
#endif /* OPTIMIZE_I2C */

    *pReadDataBuf = (unsigned char)readData;

    return 0;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxWrite - perform I2C "write" cycle
*
* This function's purpose is to perform an I2C write cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxWrite
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
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_DATA_REG),
            (UINT32)writeData,
            0);

    return 0;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxAckIn - perform I2C "acknowledge-in" cycle
*
* This function's purpose is to perform an I2C acknowledge-in cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxAckIn
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

    for (timeOutCount = 10000; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_STATUS_REG),
                0,
                0);

        if (statusReg & MPC85XX_I2C_STATUS_REG_MIF)
            {
            i2cIoctl(I2C_IOCTL_RMW_AND,
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_STATUS_REG),
                    ((UINT8)~MPC85XX_I2C_STATUS_REG_MIF),
                    0);

#ifndef OPTIMIZE_I2C
            break;       
#else
            if ( !(statusReg & MPC85XX_I2C_STATUS_REG_RXAK ))
        	    {
	            break;
        	    }
#endif /* OPTIMIZE_I2C */

            }
        }

    if (!timeOutCount)
        {
        return -1;
        }

    return 0;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxAckOut - perform I2C "acknowledge-out" cycle
*
* This function's purpose is to perform an I2C acknowledge-out cycle.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxAckOut
    (
    int unit
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    return 0;
    }

#ifdef OPTIMIZE_I2C
/******************************************************************************
* 
* i2cCycleMpc85xxDataTransferred - wait for data transfer completion
* 
* This function's purpose is to wait for data transfer completion.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/
int i2cCycleMpc85xxDataTransferred ( int unit )
    {
    unsigned int statusReg    = 0;
    unsigned int timeOutCount = 0;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * wait until an *internal* device interrupt has been generated, then
     * clear it.  if it is not received, return with an error.
     * we are polling, so NO processor interrupt is generated.
     * Note: Macro MPC85XX_I2C_BUSY_TIMEOUT should be modified to fit
     * specific I2C devices according to user manual.
     */

    for (timeOutCount = MPC85XX_I2C_BUSY_TIMEOUT; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_STATUS_REG), 0, 0);

        if ( statusReg & MPC85XX_I2C_STATUS_REG_MIF )
            {
            i2cIoctl(I2C_IOCTL_RMW_AND, 
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC85XX_I2C_STATUS_REG), 
                    ((UINT8)~MPC85XX_I2C_STATUS_REG_MIF), 0);
					
			break;
            }
        else
            sysMpc85xxUsDelay(1);

        }

    if(!timeOutCount)
        return ERROR;
    else
        return OK;

    }

/*******************************************************************************
*
* sysMpc85xxUsDelay - delay for the specified amount of time (US)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMpc85xxUsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMpc85xxUsDelay
    (
    UINT        delay       /* length of time in US to delay */
    )
    {
    sysUsDelay(delay);
    }
#endif /* OPTIMIZE_I2C */

/*******************************************************************************
*
* i2cCycleMpc85xxKnownState - initialize the I2C bus to a known state
*
* This function's purpose is to initialize the I2C bus to a known state.
*
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO: N/A
*/

int i2cCycleMpc85xxKnownState
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

    for (timeOutCount = 1000; timeOutCount; timeOutCount--)
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD,
                (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + MPC85XX_I2C_STATUS_REG),
                0,
                0);

        if (!(statusReg & MPC85XX_I2C_STATUS_REG_MBB))
            {
            status = OK;
            break;
            }

        /*
         * re-initialize the I2C if the BUS BUSY does not clear
         * after trying half the *reasonable* amount of reads of the
         * status register.
         */

        if (!(timeOutCount % 50))
            {
            status = i2cDrvInit(unit, I2C_DRV_TYPE);
            if (status == OK)
                break;
            else
                return ERROR;
            }
        }

    if (!timeOutCount)
        status = ERROR;

    return status;
    }

/*******************************************************************************
*
* i2cCycleMpc85xxDelay - perform interface's I2C delay routine
*
* This function's purpose is to perform whatever delay required for the device.
*
* RETURNS:  N/A
*
* ERRNO: N/A
*/

void i2cCycleMpc85xxDelay
    (
    int mSeconds    /* time to delay in milliseconds */
    )
    {
    sysMpc85xxMsDelay(mSeconds);
    }

/*******************************************************************************
*
* sysMpc85xxMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMpc85xxMsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHz this amounts to
* about 258 seconds.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMpc85xxMsDelay
    (
    UINT        delay       /* length of time in MS to delay */
    )
    {
    sysMsDelay(delay);
    }
