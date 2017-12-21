/* i2c.c - tsi108-specific support functions for I2C interface. */

/* Copyright (c) 2006 Wind River Systems, Inc */
/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01b,01feb06,dtr  tidy up.
01a,22aug05,vsc  created
*/

/*
DESCRIPTION
The routines offer access to the Tsi108 I2C interface.

USER CALLABLE ROUTINES
All of the routines can be called directly.

*/

/* includes */
#include <vxWorks.h>
#include "tsi108.h"
#include "i2c.h"


#define	TSI108_I2C_DEBUG

#ifdef  TSI108_I2C_DEBUG
#define TSI108_I2C_DBG_OFF	0x0000
#define TSI108_I2C_DBG_RD	0x0001
#define TSI108_I2C_DBG_WR	0x0002
#define TSI108_I2C_DBG_ERR	0x0004

int tsi108i2cDbg =  TSI108_I2C_DBG_ERR;

#define TSI108_I2C_LOG(FLG, X0, X1, X2, X3, X4, X5, X6) \
    do  {					    \
	if (tsi108i2cDbg & FLG)		    \
	    logMsg (X0, X1, X2, X3, X4, X5, X6);    \
	} while ((0))

#else
#define TSI108_I2C_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)
#endif


/*******************************************************************************
*
* i2cReadByte  - reads one byte from I2C
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cReadByte
    (
    UINT  i2cChan,           /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer            /* pointer to data buffer */
    )
    {
    ULONG  temp;
    ULONG  toCount = I2C_DELAY;
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;
    ULONG  chanOffset = TSI108_I2C_OFFSET;


    if(0 != i2cChan) 
        chanOffset = TSI108_I2C_SDRAM_OFFSET;

    /* Check if I2C operation is in progress */
    temp = TSI108_REG_RD(BL_I2C, I2C_CNTRL2);

    if(0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_WR_STATUS |
                     I2C_CNTRL2_START))) 
        {

        /* Set device address and operation (read = 0) */

        temp = (byteAddr << 16) | ((page & 0x07) << 8) | (devCode & 0x0F);

        TSI108_REG_WR(BL_I2C, I2C_CNTRL1, temp);

        /* Issue the read command
         * (at this moment all other parameters are 0 (size = 1 byte, lane = 0)
         */

         TSI108_REG_WR(BL_I2C, I2C_CNTRL2, I2C_CNTRL2_START);

        /* Wait until operation completed */
        do 
            {

            /* Read I2C operation status */

            temp = TSI108_REG_RD(BL_I2C, I2C_CNTRL2);

            if (0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_START))) 
                {
                if (0 == (temp & 
			  (I2C_CNTRL2_I2C_CFGERR | I2C_CNTRL2_I2C_TO_ERR)))
                    {
                    opStatus = TSI108_I2C_SUCCESS;
                    *buffer = TSI108_REG_RD(BL_I2C, I2C_RD_DATA) & 0xFF;
                    }
                else 
                    {
                    opStatus = TSI108_I2C_IF_ERROR; /* report HW error */
                    TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
				   ("I2C HW error reported: 0x%02lx\n"),
				   temp,2,3,4,5,6);
                    }
                break;
                }
            } while (toCount--); 
        } 
    else 
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
		       ("I2C Transaction start failed: 0x%02lx\n"),
		       temp,2,3,4,5,6);
        }

    return opStatus;
    }

/*******************************************************************************
*
* i2cWriteByte  - writes one byte to I2C
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cWriteByte
    (
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer           /* pointer to data buffer */
    )
    {
    ULONG  temp;
    ULONG  to_count = I2C_DELAY;
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;

    /* Check if I2C operation is in progress */

    temp = TSI108_REG_RD(BL_I2C, I2C_CNTRL2);

    if(0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_WR_STATUS |
                     I2C_CNTRL2_START))) 
        {

        /* Place data into the I2C Tx Register */

        TSI108_REG_WR(BL_I2C, I2C_TX_DATA, *buffer);

        /* Set device address and operation  */

        temp = I2C_CNTRL1_I2CWRITE | (byteAddr << 16) | 
               ((page & 0x07) << 8) | (devCode & 0x0F);
        TSI108_REG_WR(BL_I2C, I2C_CNTRL1, temp);

        /* Issue the write command
         * (at this moment all other parameters are 0 (size = 1 byte, lane = 0)
         */

         TSI108_REG_WR(BL_I2C, I2C_CNTRL2, I2C_CNTRL2_START);

        /* Wait until operation completed */

        do 
            {
            /* Read I2C operation status */

            temp = TSI108_REG_RD(BL_I2C, I2C_CNTRL2);
            if (0 == (temp & (I2C_CNTRL2_WR_STATUS | I2C_CNTRL2_START)))
                {
                if (0 == (temp &
			  (I2C_CNTRL2_I2C_CFGERR | I2C_CNTRL2_I2C_TO_ERR)))
                    {
                    opStatus = TSI108_I2C_SUCCESS;
                    }
                 else 
                    {
                    opStatus = TSI108_I2C_IF_ERROR;
                    TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
				   ("I2C HW error reported: 0x%08lx\n"),
				   temp, 2,3,4,5,6);
                    }
                 break;
                }
            } while (to_count--); 

        /*
	 * Some I2C devices require time after a write to settle
         * So do a successful read before allowing other I2C write cycles
	 */

        /* Set read address */

        temp = (byteAddr << 16) | ((page & 0x07) << 8) | I2C_EEPROM_DEVCODE;
        TSI108_REG_WR(BL_I2C, I2C_CNTRL1, temp);

        /* Issue the read command
         * (at this moment all other parameters are 0 (size = 1 byte, lane = 0)
         */

        TSI108_REG_WR(BL_I2C, I2C_CNTRL2, I2C_CNTRL2_START);

        /* Wait until operation completed */
        do 
            {
            /* Read I2C operation status */
            temp = TSI108_REG_RD(BL_I2C, I2C_CNTRL2);
            if (0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_START)))
                {
                if (0 == (temp &
			  (I2C_CNTRL2_I2C_CFGERR | I2C_CNTRL2_I2C_TO_ERR)))
                    break;  /* Do not need data */
                else 
                    {
                    /* Ignore TO error and do I2C read */
                    if(temp & I2C_CNTRL2_I2C_TO_ERR) 
                        {
                        TSI108_REG_WR(BL_I2C, I2C_CNTRL2, I2C_CNTRL2_START);
                        }
                    else
                        {
                        opStatus = TSI108_I2C_IF_ERROR;
                        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
				       ("I2C HW error reported: a 0x%08lx\n"),
				       temp, 2,3,4,5,6);
                        break;
                        }
                    }
                }
            } while (to_count--);
        } 
    else 
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
		       ("I2C Transaction start failed: 0x%08lx\n"),
		       temp, 2, 3, 4, 5, 6);
        }

    return opStatus;
    }

/*******************************************************************************
*
* i2cRead  - read <length> bytes from I2C
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cRead
    (
    UINT  i2cChan,           /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer,           /* pointer to data buffer */
    UINT  length             /* number of bytes to read */
    )
    {
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;
    
    if (page*I2C_PAGE_SIZE + byteAddr + length > I2C_MAX_SIZE)
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
		       ("i2cRead failed: final address > I2C_MAX_SIZE %03x\n"),
		       I2C_MAX_SIZE, 2,3,4,5,6);
        return ((ULONG)-1);
        }

    while(length--)
        {
        opStatus = i2cReadByte (i2cChan, byteAddr++, page, devCode, buffer++);
        if (TSI108_I2C_SUCCESS != opStatus)
            {
            TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
			   ("i2cReadByte() failed:  0x%02lx (%d left)\n"),
			   opStatus, length,3,4,5,6);
            }
        }
        if(byteAddr >= I2C_PAGE_SIZE)
            {
                byteAddr -= I2C_PAGE_SIZE;
                page++;
            }

    return opStatus;
    }

/*******************************************************************************
*
* i2cWrite - write <length> bytes to I2C
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cWrite
    (
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer,           /* pointer to data buffer */
    UINT  length             /* number of bytes to read */
    )
    {
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;

    if (page*I2C_PAGE_SIZE + byteAddr + length > I2C_MAX_SIZE)
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
		       ("i2cRead failed: final address > I2C_MAX_SIZE %03x\n"),
		       I2C_MAX_SIZE, 2,3,4,5,6);
        return ((ULONG)-1);
        }
    
    while(length--)
        {
        opStatus = i2cWriteByte (byteAddr++, page, devCode, buffer++);
        if (TSI108_I2C_SUCCESS != opStatus)
            {
            TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
			   ("i2cWriteByte() failed: 0x%08lx (%d left)\n"),
			   opStatus, length, 3,4,5,6);
            }
        if(byteAddr >= I2C_PAGE_SIZE)
            {
                byteAddr -= I2C_PAGE_SIZE;
                page++;
            }
        }

    return opStatus;
    }
