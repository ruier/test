/* i2c.c - tsi108-specific support functions for I2C interface. */

/* Copyright 2007 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01a,11may07,jjm  Initial version, based on IBM 750GX/GL EVB BSP
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


/* defines */
#define INCLUDE_I2C_SHOW_ROUTINES

#undef  TSI108_I2C_DEBUG

#ifdef  TSI108_I2C_DEBUG
#define TSI108_I2C_DBG_OFF      0x0000
#define TSI108_I2C_DBG_RD       0x0001
#define TSI108_I2C_DBG_WR       0x0002
#define TSI108_I2C_DBG_ERR      0x0004

int tsi108i2cDbg =  TSI108_I2C_DBG_ERR;

#define TSI108_I2C_LOG(FLG, X0, X1, X2, X3, X4, X5, X6) \
    do  {                                               \
        if (tsi108i2cDbg & FLG)                         \
            logMsg (X0, X1, X2, X3, X4, X5, X6);        \
        } while ((0))

#else
#define TSI108_I2C_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)
#endif

#define I2C_SEM_TAKE(bus, tmout)                        \
    semTake (i2cMutex[bus], (int) (tmout))

#define I2C_SEM_GIVE(bus)                               \
    semGive (i2cMutex[bus])

/* locals */

LOCAL SEM_ID    i2cMutex[2];    /* mutex semaphores for two I2C channels */
LOCAL BOOL      i2cLibInitialized = FALSE;      /* have we been initialized? */
BOOL            i2cSdcWriteEnable = FALSE;      /* Write Enable flag for SDC channel */

/**************************************************************************
*
* i2cLibInit - initialize the I2C library
*
* This routine initializes the I2C library.
*
* RETURNS: OK or ERROR.
*/
 
STATUS i2cLibInit (void)
    {
    if (i2cLibInitialized)
        return (OK);

    /* Create the mutex semaphore */

    i2cMutex[0] = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                           SEM_INVERSION_SAFE);

    i2cMutex[1] = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                           SEM_INVERSION_SAFE);

    i2cLibInitialized = TRUE;

    return (OK);
    }

/**************************************************************************
*
* i2cLibUnInit - uninitialize the I2C library
*
* This routine uninitializes the I2C library. Previously allocated resources
* are reclaimed back to the system.
*
* RETURNS: OK or ERROR.
*/
 
STATUS i2cLibUnInit (void)
    {
    if (!i2cLibInitialized)
        return (OK);

    /* delete mutexes */
    if (semDelete (i2cMutex[0]) == ERROR)
        return (ERROR);

    if (semDelete (i2cMutex[1]) == ERROR)
        return (ERROR);

    i2cLibInitialized = FALSE;

    return (OK);
    }

/*******************************************************************************
*
* i2cReadDev  - reads one unit of data (8-, 16- or 32-bit) from I2C device
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cReadDev
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  size          /* data unit size */
    )
    {
    ULONG  temp;
    ULONG  toCount = I2C_DELAY;
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;
    ULONG  chanOffset = (i2cChan)?TSI108_I2C_SDRAM_OFFSET:TSI108_I2C_OFFSET;
    UINT32 rdVal;

    /* Check if I2C operation is in progress */
    temp = TSI108_REG_RD(chanOffset, I2C_CNTRL2);

    if(0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_WR_STATUS |
                     I2C_CNTRL2_START))) 
        {
        I2C_SEM_TAKE (i2cChan, WAIT_FOREVER);

        /* Set device address and operation (read = 0) */
        temp = (byteAddr << 16) | ((page & 0x07) << 8) | (devCode & 0x0F);
        TSI108_REG_WR(chanOffset, I2C_CNTRL1, temp);

        /* Issue the read command */
        TSI108_REG_WR(chanOffset, I2C_CNTRL2, I2C_CNTRL2_START | ((size - 1) & 0x03));

        /* Wait until operation completed */
        do
            {

            /* Read I2C operation status */

            temp = TSI108_REG_RD(chanOffset, I2C_CNTRL2);

            if (0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_START))) 
                {
                if (0 == (temp & 
                            (I2C_CNTRL2_I2C_CFGERR | I2C_CNTRL2_I2C_TO_ERR)))
                    {
                    rdVal = TSI108_REG_RD(chanOffset, I2C_RD_DATA);
                    if (size == 2)
                        *(UINT16*)buffer = (UINT16)rdVal;
                    else if (size == 4)
                        *(UINT32*)buffer = rdVal;
                    else /* if (size == 1) */
                        *buffer = (UCHAR)rdVal;
                    opStatus = TSI108_I2C_SUCCESS;
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
        
        I2C_SEM_GIVE (i2cChan);

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
* i2cWriteDev  - writes one data unit (8-, 16- or 32-bit) to I2C device
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cWriteDev
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UINT32 wrData,      /* data to write into the device */
    UINT  size          /* data unit size */
    )
    {
    ULONG  temp;
    ULONG  to_count = I2C_DELAY;
    ULONG  opStatus = TSI108_I2C_TIMEOUT_ERR;
    ULONG  chanOffset = (i2cChan)?TSI108_I2C_SDRAM_OFFSET:TSI108_I2C_OFFSET;

    if (i2cChan && !i2cSdcWriteEnable) /* Check if write to SDC channel is enabled */
        return TSI108_I2C_PARAM_ERR;

    /* Check if I2C operation is in progress */

    temp = TSI108_REG_RD(chanOffset, I2C_CNTRL2);

    if(0 == (temp & (I2C_CNTRL2_RD_STATUS | I2C_CNTRL2_WR_STATUS |
                I2C_CNTRL2_START))) 
        {
        I2C_SEM_TAKE (i2cChan, WAIT_FOREVER);

        /* Place data into the I2C Tx Register */
        TSI108_REG_WR(chanOffset, I2C_TX_DATA, wrData);

        /* Set device address and operation  */

        temp = I2C_CNTRL1_I2CWRITE | (byteAddr << 16) | 
                ((page & 0x07) << 8) | (devCode & 0x0F);
        TSI108_REG_WR(chanOffset, I2C_CNTRL1, temp);

        /* Issue the write command */
        TSI108_REG_WR(chanOffset, I2C_CNTRL2,
                I2C_CNTRL2_START | I2C_CNTRL2_MULTIBYTE |
                ((size - 1) & I2C_CNTRL2_SIZE) );

        /* Wait until operation completed */

        do
            {
            /* Read I2C operation status */

            temp = TSI108_REG_RD(chanOffset, I2C_CNTRL2);
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

            I2C_SEM_GIVE (i2cChan);
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
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  length        /* number of bytes to read */
    )
    {
    ULONG   opStatus = TSI108_I2C_TIMEOUT_ERR;
    UINT8   devAddr = (devCode << 3) | page;  

    if (page*I2C_PAGE_SIZE + byteAddr + length > I2C_MAX_SIZE)
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
                ("i2cRead failed: final address > I2C_MAX_SIZE %03x\n"),
                I2C_MAX_SIZE, 2,3,4,5,6);
        return ((ULONG)-1);
        }

    while(length--)
        {
        I2C_READ_BYTE(i2cChan, devAddr, byteAddr++, buffer++, opStatus);

        if (TSI108_I2C_SUCCESS != opStatus)
            {
            TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
                        ("i2cReadDev() failed:  0x%02lx (%d left)\n"),
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
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  length        /* number of bytes to read */
    )
    {
    ULONG opStatus = TSI108_I2C_TIMEOUT_ERR;
    UINT8 devAddr = (devCode << 3) | page;  

    if (page*I2C_PAGE_SIZE + byteAddr + length > I2C_MAX_SIZE)
        {
        TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
                ("i2cRead failed: final address > I2C_MAX_SIZE %03x\n"),
                I2C_MAX_SIZE, 2,3,4,5,6);
        return ((ULONG)-1);
        }

    while(length--)
        {
        I2C_WRITE_BYTE(i2cChan, devAddr, byteAddr++, *buffer++, opStatus);

        if (TSI108_I2C_SUCCESS != opStatus)
            {
            TSI108_I2C_LOG(TSI108_I2C_DBG_ERR,
            ("i2cWriteDev() failed: 0x%08lx (%d left)\n"),
            opStatus, length, 3,4,5,6);
            }
        if(byteAddr >= I2C_PAGE_SIZE)
            {
            byteAddr -= I2C_PAGE_SIZE;
            page++;
            }
        /* EEPROM requires up to 5 ms (tWR) to complete internal write cycle */
        sysMsDelay (5);
        }

    return opStatus;
    }

/*******************************************************************************
*
* i2cDevProbe - probe for presence of the specified I2C device
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
ULONG i2cDevProbe
    (
    int chanNum,
    UCHAR i2cAddr
    )
    {
    UCHAR tmp;
    ULONG stat;

    /*
     * The Tsi108 HW doesn't support sending just the chip address
     * and checkong for an <ACK> back. Therefore, to probe device
     * we try to read the first location of the chip.
     */

    I2C_READ_BYTE(chanNum, i2cAddr, 0x00, &tmp, stat);
    return stat;
    }

#ifdef INCLUDE_I2C_SHOW_ROUTINES

/*******************************************************************************
*
* i2cProbe - probed devices on the specified I2C channel
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
STATUS i2cProbe
    (
    int chanNum
    )
    {
        int j;

        puts ("Valid chip addresses:");
        for(j = 0; j < 128; j++) {
                if(i2cDevProbe(chanNum, j) == 0) {
                        printf(" %02X", j);
                }
        }
        puts("\n");

        return (OK);
    }

/*******************************************************************************
*
* i2cEepromDump - dumps content of the specified page of I2C EEPROM
*
* RETURNS: 0 on success, not 0 on failure
*
* ERRNO: N/A
*/
STATUS i2cEepromDump
    (
    UINT chanNum,
    UINT page
    )
    {
    UCHAR buf[256];
    int i;
    UINT    byteAddr = 0;

    i2cRead(chanNum, byteAddr, page, 0xa, buf, sizeof(buf));

    for (i = 0; i < sizeof(buf); i++)
        {
        if (i % 0x10 == 0)
            printf("\n0x%02x:", i);
        
        printf(" %02x", buf[i]);

        }

        puts("\n");

        return (OK);
    }

#endif /* INCLUDE_I2C_SHOW_ROUTINES */
