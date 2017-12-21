/* sysMpc837xI2c.c - I2C Driver Source Module */

/*
 * Copyright (c) 2005, 2007-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01g,09aug11,swu  Improve I2C performance for mds837x. (WIND00289153)
01f,18dec09,x_z  remove sysMpc837xMsDelay().
01e,15feb08,x_s  added PIB board support. (WIND00113447)
01d,20oct07,x_s  modified for mds837x.
01c,04aug05,dtr  Added PCI2 support for PIB.
01b,04apr05,mdo  Documentation fixes for apigen
01a,28feb05,dtr  created.
*/

/*
DESCRIPTION

I2C Driver Source (Low Level Routines) Module
Mpc834x Memory Controller (PowerPlus Architecture)

Notes:
   1. GREAT care must be made when sending commands to the 
      MPC834X(Mpc834x).  Several "i2cCycleMpc834xDelay" calls are made
      in order to not overwhelm the I2C interface.  If commands
      are sent too fast, the I2C interface will lockup and the I2C
      bus may become unusable without a powercycle.  Generally, if
      you cause a I2C bus cycle, you should wait for "a while".
      A possible cause is that the caches were turned on when this
      driver was written.

INCLUDE FILES:   sysMpc837xI2c.h
*/

/* includes */

#include <vxWorks.h>        /* vxWorks generics */
#include "sysMotI2c.h"
#include "sysMpc837xI2c.h"  /* low level definitions, specific */
#include "config.h"     /* BSP specifics */
#include <stdio.h>
#include <string.h>
#ifdef DEBUG_I2C
    #include <logLib.h>
#endif
#include <stdlib.h>
#include <taskLib.h>
#include <arch/ppc/vxPpcLib.h>

/* defines */

#define TIMEOUT 10

/* externals */

IMPORT I2C_DRV_CTRL * pI2cDrvCtrl[2] ;

/* forward declarations */

void i2cCycleMpc837xDelay (int mSeconds);
int i2cCycleMpc837xDataTransferred (int unit);

/* debug declarations */

#ifdef  DEBUG_I2C
    #define SPD_SIZE        256
    #define REAL_SPD_SIZE   256
char defaultSPD[SPD_SIZE];
#endif /* DEBUG_I2C */

/* Global flag */

int I2CDoRepeatStart = 0;   /* indicates if a "Repeat Start" is requested */


/******************************************************************************
* 
* i2cCycleMpc834xStart - perform I2C "start" cycle
* 
* This function's purpose is to perform an I2C start cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xStart 
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
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_CONTROL_REG), 
                    (MPC837X_I2C_CONTROL_REG_RSTA |
                     MPC837X_I2C_CONTROL_REG_MSTA |
                     MPC837X_I2C_CONTROL_REG_MTX),0);
        I2CDoRepeatStart = 0;  /* one repeat start only, so clear this bit */
        return(0);
        }

    /* 
     * wait until the I2C bus is free.  if it doesn't become free
     * within a *reasonable* amount of time, exit with an error.
     */

    for ( timeOutCount = 1000; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_STATUS_REG), 0, 0);

        if ( !(statusReg & MPC837X_I2C_STATUS_REG_MBB) )
            {
            break;
            }
        }

    if ( !timeOutCount )
        {
        return(-1); 
        }

    /*
     * since this is the first time through, generate a START(MSTA) and
     * place the I2C interface into a master transmitter mode(MTX).
     */

    i2cIoctl(I2C_IOCTL_RMW_OR, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_CONTROL_REG), 
            (MPC837X_I2C_CONTROL_REG_MTX |
             MPC837X_I2C_CONTROL_REG_MSTA),0);

    /* 
     * The first time through, set "I2CDoRepeatStart".  If this function
     * is called again BEFORE a STOP is sent, then we are doing a 
     * "dummy write", which sets the devices internal byte pointer 
     * to the byte we intend to read.
     */

    I2CDoRepeatStart = 1;

    return(0);

    }


/******************************************************************************
* 
* i2cCycleMpc834xStop - perform I2C "stop" cycle
* 
* This function's purpose is to perform an I2C stop cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xStop 
    (
    int unit
    )
    {
    UINT32 timeOutCount;
    int   status = -1;
    UINT8 statusReg = 0;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * turn off MSTA bit(which will generate a STOP bus cycle)
     * turn off MTX bit(which places the MPC834X interface into receive mode
     * turn off TXAK bit(which allows 9th clock cycle acknowledges)
     */
    i2cIoctl(I2C_IOCTL_RMW_AND, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_CONTROL_REG), 
            ((UINT8)~(MPC837X_I2C_CONTROL_REG_MTX  |
               MPC837X_I2C_CONTROL_REG_MSTA |
               MPC837X_I2C_CONTROL_REG_TXAK)), 0);

    for(timeOutCount = 1000; timeOutCount; timeOutCount--)
        {
    	statusReg = i2cIoctl(I2C_IOCTL_RD, 
    	                     (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + 
			              MPC837X_I2C_STATUS_REG), 0, 0);
    	
        if ( !(statusReg & MPC837X_I2C_STATUS_REG_MBB) )
            {
            status = 0;
            break;
            }
    	
        }
    
    /* Clear the global I2C "Repeat Start" flag. */

    I2CDoRepeatStart = 0;

    return status;

    }


/******************************************************************************
* 
* i2cCycleMPC837xRead - perform I2C "read" cycle
* 
* This function's purpose is to perform an I2C read cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xRead
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
        /* Don't send master ack. */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_CONTROL_REG),
            ((UINT8)~MPC837X_I2C_CONTROL_REG_MTX),
              MPC837X_I2C_CONTROL_REG_TXAK);
        }
    else
        {
        /* Send master ack. */
        i2cIoctl(I2C_IOCTL_RMW_AND_OR, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_CONTROL_REG),
            ((UINT8)~(MPC837X_I2C_CONTROL_REG_MTX|MPC837X_I2C_CONTROL_REG_TXAK)),
              0);
        }


    /* do a "dummy read".  this latches the data off the bus. */

    i2cIoctl(I2C_IOCTL_RD, 
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_DATA_REG), 
            0, 0);

    if(i2cCycleMpc837xDataTransferred(unit))
        {
    	return -1;
        }

    /* now do the actual read, make this one count */

    readData = i2cIoctl(I2C_IOCTL_RD, 
                       (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_DATA_REG), 
                       0, 0);

    if(i2cCycleMpc837xDataTransferred(unit))
        {
    	return -1;
        }

    *pReadDataBuf = (unsigned char)readData;

    return(0);

    }


/******************************************************************************
* 
* i2cCycleMPC837xWrite - perform I2C "write" cycle
* 
* This function's purpose is to perform an I2C write cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xWrite
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
            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_DATA_REG), 
            (UINT32)writeData, 0);

    return(0);
    }


/******************************************************************************
* 
* i2cCycleMPC837xAckIn - perform I2C "acknowledge-in" cycle
* 
* This function's purpose is to perform an I2C acknowledge-in cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xAckIn 
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

    for ( timeOutCount = 100000; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_STATUS_REG), 0, 0);

        if ( statusReg & MPC837X_I2C_STATUS_REG_MIF )
            {
            i2cIoctl(I2C_IOCTL_RMW_AND, 
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_STATUS_REG), 
                    ((UINT8)~MPC837X_I2C_STATUS_REG_MIF), 0);

            if ( !(statusReg & MPC837X_I2C_STATUS_REG_RXAK ))
        	{
	            break;
        	}
			
            }
        }

    if ( !timeOutCount )
        {
        return(-1);
        }

    return(0);

    }

/******************************************************************************
* 
* i2cCycleMpc837xDataTransferred - wait for data transfer completion
* 
* This function's purpose is to wait for data transfer completion.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xDataTransferred ( int unit )
{
    unsigned int statusReg    = 0;
    unsigned int timeOutCount = 0;

    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    /*
     * wait until an *internal* device interrupt has been generated, then
     * clear it.  if it is not received, return with an error.
     * we are polling, so NO processor interrupt is generated.
     */

    for ( ; ; )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs + 
				     MPC837X_I2C_STATUS_REG), 0, 0);

        if ( statusReg & MPC837X_I2C_STATUS_REG_MIF )
            {
            i2cIoctl(I2C_IOCTL_RMW_AND, 
                    (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_STATUS_REG), 
                    ((UINT8)~MPC837X_I2C_STATUS_REG_MIF), 0);
					
	    break;
            }  
        
	if ( ++timeOutCount > 10000 )
	    {
	    return ( -1 );
	    }
        }

    return(0);

}

/******************************************************************************
* 
* i2cCycleMPC837xAckOut - perform I2C "acknowledge-out" cycle
* 
* This function's purpose is to perform an I2C acknowledge-out cycle.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/
int i2cCycleMpc837xAckOut 
    (
    int unit
    )
    {
    if (pI2cDrvCtrl[unit] == NULL)
        return -1 ;

    return(0);
    }


/******************************************************************************
* 
* i2cCycleMPC837xKnownState - initialize the I2C bus to a known state
* 
* This function's purpose is to initialize the I2C bus to a known state.
* 
* RETURNS:
*   zero        = operation successful
*   non-zero    = operation failed
*
* ERRNO
*/

int i2cCycleMpc837xKnownState 
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

    for ( timeOutCount = TIMEOUT; timeOutCount; timeOutCount-- )
        {
        statusReg = i2cIoctl(I2C_IOCTL_RD, 
                            (UINT32)(pI2cDrvCtrl[unit]->baseAdrs+MPC837X_I2C_STATUS_REG), 0, 0);

        if ( !(statusReg & MPC837X_I2C_STATUS_REG_MBB) )
            {
            status = OK;
            break;
            }

        /* 
         * re-initialize the I2C if the BUS BUSY does not clear
         * after trying half the *reasonable* amount of reads of the 
         * status register.
         */

        if (timeOutCount == TIMEOUT / 2)
            {
            status = i2cDrvInit(unit, I2C_DRV_TYPE);
            if ( status == OK )
                continue;
            else
                return ERROR ;
            }
        }

    if ( !timeOutCount )
        status = ERROR;

    return(status);
    }


/******************************************************************************
* 
* i2cCycleMPC837xDelay - perform interface's I2C delay routine 
* 
* This function's purpose is to perform whatever delay required for the device.
* 
* RETURNS:  N/A
*
* ERRNO
*/
void i2cCycleMpc837xDelay
    (
    int mSeconds    /* time to delay in milliseconds */
    )
    {
    sysMsDelay(mSeconds);
    }

/******************************************************************************
*
* i2cIOPortRegWrite - write the IO Port register
*
* This routine writes the IO Port register on the I2C.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*
* NOTES:
* Reg 0..7 
* Actual read and writes require accessing a pair of registers.
* command byte 0 : input port 0,1 
* command byte 2 : output port 0,1 
* command byte 4 : polarity inversion 0,1
* command byte 6 : config port 0,1 
* config port 0 = output, 1 = input, powerup all 1 
* so if config port 1.4 = 0 then output port 1.4 is applicable 
*/

int i2cIOPortRegWrite 
    (
    int unit,
    int deviceAddress,
    int reg,
    UINT16 data
    )
    {

    /* write register pair */
    if ( i2cWrite (unit, 
                   deviceAddress, 
                   I2C_DEVICE_TYPE_IOPORT_PCA9555,
                   reg, 
                   2, 
                   (char*)&data) == ERROR )
        {
        logMsg("Driver returned error\n",0,0,0,0,0,0);
        return(ERROR);
        }


    return OK ;

    }



/******************************************************************************
*
* i2cIOPortRegRead - read the IO Port register
*
* This routine reads the IO Port register on the I2C.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

int i2cIOPortRegRead
    (
    int unit,
    int deviceAddress,
    int reg,
    UINT16 *data
    )
    {

    /* read register pair */

    if ( i2cRead (unit,  
                  deviceAddress, 
                  I2C_DEVICE_TYPE_IOPORT_PCA9555,
                  reg, 
                  2,  /* 2 bytes */
                 (char*)data) == ERROR )
        {
        logMsg("Driver returned error\n",0,0,0,0,0,0);
        return(ERROR);
        }

    return OK ;
    }

/* Setup PIB board for PCI on PMC1 */
/* Need 32bit, 66MHz card on PMC1 */

void initPibBoard (int unit)
    {

    /* Input ports 0, 1 */
    /* Output ports 2, 3 */
    /* Polarity ports 4, 5 */
    /* Configuration ports 6, 7 */

    /* Set config reg bits for IO port direction. */

    i2cIOPortRegWrite(unit, 0x23, 6, 0x0000) ;
    i2cIOPortRegWrite(unit, 0x26, 6, 0x0134) ;
    i2cIOPortRegWrite(unit, 0x27, 6, 0x0000) ;

    /* Set desired output port values */
    i2cIOPortRegWrite(unit, 0x23, 2, 0xffff);
    #if 1
    /* 66MHz */
    i2cIOPortRegWrite(unit, 0x26, 2, 0xf3ff);
    #else
    /* 33MHz */
    i2cIOPortRegWrite(unit, 0x26, 2, 0xf3bf);
    #endif
    i2cIOPortRegWrite(unit, 0x27, 2, 0xffef);


    }
/** DEBUG FUNCTIONS **/
#ifdef DEBUG_I2C 
void testI2c (void)
    {
    UINT16 data ;

    /* Try I2C1 */

    logMsg("Adrs 0x23...\n",1,2,3,4,5,6);
    i2cIOPortRegWrite(1, 0x23, 6, 0x55aa);
    i2cIOPortRegRead(1, 0x23, 6, &data);
    logMsg("config reg data : 0x%x.\n", data,2,3,4,5,6);

    logMsg("Adrs 0x23...\n",1,2,3,4,5,6);
    i2cIOPortRegWrite(1, 0x23, 6, 0xcafe);
    i2cIOPortRegRead(1, 0x23, 6, &data);
    logMsg("config reg data : 0x%x.\n", data,2,3,4,5,6);

    logMsg("Adrs 0x26...\n",1,2,3,4,5,6);
    i2cIOPortRegWrite(1, 0x26, 6, 0x3344);
    i2cIOPortRegRead(1, 0x26, 6, &data);
    logMsg("config reg data : 0x%x.\n", data,2,3,4,5,6);

    logMsg("Adrs 0x27...\n",1,2,3,4,5,6);
    i2cIOPortRegWrite(1, 0x27, 6, 0x0000);
    i2cIOPortRegRead(1, 0x27, 6, &data);
    logMsg("config reg data : 0x%x.\n", data,2,3,4,5,6);

    }

/******************************************************************************
*
* i2cProgramSPD - Demonstration function to program an SPD EEPROM
*
* This routine programs an SPD EEPROM with the contents of
* defaultSPD.  This program is enabled by defining DEBUG_I2C.
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

int i2cProgramEeprom
    (
    int unit,
    int deviceAddress   /* address of I2C device - 0xa0, 0xa2 */
    )
    {
    int index = 0;
    char verifyData[REAL_SPD_SIZE];

    for ( index=0;index<REAL_SPD_SIZE;index++ )
        defaultSPD[index] = index ;

    i2cWrite (unit, 
              deviceAddress, 
              I2C_DEVICE_TYPE_EEPROM_AT24C256, 
              0, 
              REAL_SPD_SIZE, 
              defaultSPD);

    if ( i2cRead (unit, 
                  deviceAddress, 
                  I2C_DEVICE_TYPE_EEPROM_AT24C256, 
                  0, 
                  REAL_SPD_SIZE, 
                  verifyData) == OK )
        {
        for ( index=0;index<REAL_SPD_SIZE;index++ )
            {
            if ( verifyData[index] != defaultSPD[index] )
                {
                logMsg("SPD programming Failed: %d: %x != %x\r\n", 
                       index, verifyData[index], defaultSPD[index],0,0,0);
                return(ERROR);
                }
            }
        }
    return(OK);
    }


/******************************************************************************
*
* i2cShowEeprom - Dump i2c device
*
* This routine prints the number "size" bytes of EEPROM addresses at
* "deviceAddress".  Valid addresses for the prpmc600 are 0xa0 
* for SPD SDRAM banks and VPD_BRD_EEPROM_ADRS for the board's VPD
* EEPROM. 
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

int i2cShowEeprom
    (
    int unit,
    int deviceAddress,  /* address of I2C device - 0xa0, 0xa2 */
    int size        /* number of bytes to read/display */
    )
    {
    int i;
    char i2cBuff[REAL_SPD_SIZE] = {0};

    if ( size > 32768 )
        return(ERROR);

    memset(i2cBuff,0,REAL_SPD_SIZE);

    if ( i2cRead (unit, 
                  deviceAddress, 
                  I2C_DEVICE_TYPE_EEPROM_AT24C256, 
                  0, 
                  size, 
                  i2cBuff) == ERROR )
        {
        logMsg("Driver returned error\n",0,0,0,0,0,0);
        return(ERROR);
        }

    for ( i=0;i<size;i++ )
        printf("%x ", i2cBuff[i]);

    printf("\r\n");

    return(0);
    }


/******************************************************************************
*
* i2cFill - block fill a i2c device
*
* This routine writes the number "size" bytes of EEPROM addresses at
* "deviceAddress" with the value "value".  Valid addresses for the prpmc600
* are 0xa0 for SPD SDRAM banks and VPD_BRD_EEPROM_ADRS for the board's VPD 
* EEPROM.  This will wipe out the existing data at the address. 
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

int i2cFillEeprom
    (
    int unit,
    int deviceAddress,  /* address of I2C device - 0xa0, 0xa2 */
    int size,       /* number of bytes to write */
    int value       /* value to fill */
    )
    {
    int ii;
    char ptr[1];

    if ( size > REAL_SPD_SIZE )
        return(ERROR);

    for ( ii=0;ii<size;ii++ )
        {
        ptr[0] = value;
        i2cWrite (unit, 
                  deviceAddress, 
                  I2C_DEVICE_TYPE_EEPROM_AT24C256, 
                  ii, 
                  1, 
                  ptr);
        }

    i2cShowEeprom (unit, deviceAddress, size);

    return(OK);
    }

#endif




