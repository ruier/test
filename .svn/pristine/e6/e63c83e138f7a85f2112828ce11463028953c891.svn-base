/* sysI2cEeprom.c - apm_serengeti I2C EEprom driver */

/*
* Copyright (c) 2012 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,01mar12,syt  fixed compile warnings and updated function
                 sysI2cEepromDevinit().
01a,12Jan12,syt  Writen.
*/

/*
DESCRIPTION
This is a driver for I2C EEPROM connected on the APM's I2C bus. It is
intended for use by client drivers performing other high level functions.
This driver file is only validated on ATMEL's AT24C02.
*/

/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* externs */

IMPORT void d (void *, int, int);

/* defines */

#define I2C_ADDR_HI(x)          ((x >> 8) & 0xff)
#define I2C_ADDR_LO(x)          (x & 0xff)

#define PAGE_WRITE_MODE         1
#define BYTE_WRITE_MODE         2

typedef struct
    {
    UINT32  i2cRegBase;         /* I2C bus index */
    UINT8   devAddr;            /* device address */
    UINT8   wordAddrLen;        /* EEprom word address length */
    UINT32  pageSize;           /* EEprom page size */
    UINT32  devSize;            /* EEprom device size */
    UINT32  progTime;           /* EEprom device byte program time (millisecond) */
    } I2C_EEPROM_TBL;

/* I2C EEprom table */

/* Table defines devices we supported in system */

I2C_EEPROM_TBL i2cEepromTable [] = {
    { APM_I2C0_BASE, AT24C02_DEV_ADDR, 1, AT24C02_PAGE_SIZE, AT24C02_DEV_SIZE, 10 },
};

/*******************************************************************************
*
* sysI2cEepromDevinit - APM I2C EEPROM initialization
*
* This function create I2C EEPROM device instance and set I2C EEPROM's
* parameters in the it.
*
* RETURNS: pDevCtrl structure pointer if initialize success, others ERROR.
*
* ERRNO
*/

I2C_DEV_CTRL * sysI2cEepromDevinit
    (
    I2C_EEPROM_TBL * i2cEepromTbl,
    I2C_DRV_CTRL *   pI2cDrvCtrl
    )
    {
    I2C_DEV_CTRL * i2cDevCtlr;

    i2cDevCtlr = (I2C_DEV_CTRL *)malloc (sizeof(I2C_DEV_CTRL));

    if (i2cDevCtlr == NULL)
        return NULL;

    memset (i2cDevCtlr, 0x0, sizeof(I2C_DEV_CTRL));

    i2cDevCtlr->pTransBuf = malloc(APM_I2C_BUF_SIZE);

    if (i2cDevCtlr->pTransBuf == NULL)
        {
        free (i2cDevCtlr);
        return NULL;
        }

    memset (i2cDevCtlr->pTransBuf, 0x0, APM_I2C_BUF_SIZE);

    i2cDevCtlr->pDrvCtrl = pI2cDrvCtrl;
    i2cDevCtlr->devAddr = i2cEepromTbl->devAddr;
    i2cDevCtlr->wordAddrLen = i2cEepromTbl->wordAddrLen;
    i2cDevCtlr->devSize = i2cEepromTbl->devSize;
    i2cDevCtlr->pageSize = i2cEepromTbl->pageSize;
    i2cDevCtlr->progTime = i2cEepromTbl->progTime;

    return (i2cDevCtlr);
    }

/*******************************************************************************
*
* sysI2cEepromPageWrite - program a page data into I2C EEPROM
*
* This function is to program a page size data into I2C EEPROM.
* I2C EEPROM page address must be page aligned, otherwise return ERROR.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS  sysI2cEepromPageWrite
    (
    I2C_DEV_CTRL *  i2cDevCtlr,
    UINT16          wordAddr,
    UINT8 *         sourceBuf
    )
    {
    UINT8 * pTransBufBackup;
    I2C_DRV_CTRL * pDrvCtlr;

    if (wordAddr % i2cDevCtlr->pageSize)
        return ERROR;

    pDrvCtlr = i2cDevCtlr->pDrvCtrl;

    pTransBufBackup = i2cDevCtlr->pTransBuf;

    /* orgnazie the I2C EEprom word address */

    if (i2cDevCtlr->wordAddrLen == 2)
        {
        *i2cDevCtlr->pTransBuf = I2C_ADDR_HI(wordAddr);
        i2cDevCtlr->pTransBuf++;
        }

    *i2cDevCtlr->pTransBuf = I2C_ADDR_LO(wordAddr);
    i2cDevCtlr->pTransBuf++;

    memcpy (i2cDevCtlr->pTransBuf, sourceBuf, i2cDevCtlr->pageSize);

    i2cDevCtlr->pTransBuf = pTransBufBackup;
    i2cDevCtlr->transLen = (i2cDevCtlr->pageSize + i2cDevCtlr->wordAddrLen);

    if (sysI2cWritePoll (i2cDevCtlr) == ERROR)
        return ERROR;

    /* Wait for the program process done */

    sysMsDelay (i2cDevCtlr->progTime);

    return OK;
    }

/*******************************************************************************
*
* sysI2cFlashByteWrite - program a byte data into I2C EEPROM
*
* This function is to program a byte data into I2C EEPROM.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysI2cEepromByteWrite
    (
    I2C_DEV_CTRL *  i2cDevCtlr,
    UINT16          wordAddr,
    UINT8           data
    )
    {
    I2C_DRV_CTRL * pDrvCtlr;
    UINT8 * pTransBufBackup;

    pDrvCtlr = i2cDevCtlr->pDrvCtrl;

    pTransBufBackup = i2cDevCtlr->pTransBuf;

    /* orgnazie the I2C Flash word address */

    if (i2cDevCtlr->wordAddrLen == 2)
        {
        *i2cDevCtlr->pTransBuf = I2C_ADDR_HI(wordAddr);
        i2cDevCtlr->pTransBuf++;
        }

    *i2cDevCtlr->pTransBuf = I2C_ADDR_LO(wordAddr);
    *(i2cDevCtlr->pTransBuf + 1) = data;

    i2cDevCtlr->pTransBuf = pTransBufBackup;
    i2cDevCtlr->transLen = (i2cDevCtlr->wordAddrLen + 1);

    if (sysI2cWritePoll (i2cDevCtlr) == ERROR)
        return ERROR;

    /* Wait for the program process done */

    sysMsDelay (i2cDevCtlr->progTime);

    return OK;
    }

/*******************************************************************************
*
* sysI2cEepromRead - read data from I2C EEprom
*
* This function is to read data from I2C EEPROM.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysI2cEepromRead
    (
    I2C_DEV_CTRL *  i2cDevCtlr,
    UINT16          distAddr,
    UINT32          rcvLen,
    UINT8 *         rcvBuf
    )
    {
    I2C_DRV_CTRL * pDrvCtlr;
    UINT8 * pTansBufBack;

    pDrvCtlr = i2cDevCtlr->pDrvCtrl;

    /* organize the I2C Flash word address */

    pTansBufBack = i2cDevCtlr->pTransBuf;

    if (i2cDevCtlr->wordAddrLen == 2)
        {
        *i2cDevCtlr->pTransBuf = I2C_ADDR_HI(distAddr);
        i2cDevCtlr->pTransBuf++;
        }

    *i2cDevCtlr->pTransBuf = I2C_ADDR_LO(distAddr);

    i2cDevCtlr->pTransBuf = pTansBufBack;
    i2cDevCtlr->transLen = i2cDevCtlr->wordAddrLen;
    i2cDevCtlr->pRcvBuf = rcvBuf;
    i2cDevCtlr->rcvLen = rcvLen;

    /* Wait for the program process done */

    return (sysI2cReadPoll (i2cDevCtlr));
    }

/*******************************************************************************
*
* sysI2cEepromTest - test I2C EEprom AT24C02 read/write
*
* This function is to test I2C EEprom AT24C02 read/write functions.
*
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/

STATUS  sysI2cEepromTest
    (
    I2C_EEPROM_TBL * pEepromTable,
    UINT8            testMode
    )
    {
    I2C_DRV_CTRL *   pDrvCtrl;
    I2C_DEV_CTRL *   pDevCtrl;
    UINT8 *          pTbuf;
    UINT8 *          pRbuf;
    UINT32           i, j;

    pDrvCtrl = sysI2cDevInit (pEepromTable->i2cRegBase, I2C_FRQ_400KHZ);

    if (pDrvCtrl == NULL)
        return ERROR;

    pDevCtrl = sysI2cEepromDevinit (pEepromTable, pDrvCtrl);

    if (pDevCtrl == NULL)
        {
        free (pDrvCtrl);
        return ERROR;
        }

    pTbuf = malloc(pEepromTable->pageSize);
    if (pTbuf == NULL)
        {
        free (pDrvCtrl);
        free (pDevCtrl);

        return ERROR;
        }

    pRbuf = malloc(pEepromTable->pageSize);
    if (pRbuf == NULL)
        {
        free (pDrvCtrl);
        free (pDevCtrl);
        free (pTbuf);

        return ERROR;
        }

    for (j = 0; j < (pEepromTable->devSize / pEepromTable->pageSize); j++)
        {
        for (i = 0; i <pEepromTable->pageSize; i++)
            pTbuf[i] = (i + j + 0x5);

        if (testMode == PAGE_WRITE_MODE)
            {
            if (sysI2cEepromPageWrite (pDevCtrl, (j * pEepromTable->pageSize), pTbuf) == ERROR)
                goto sysI2cEepromTest_error;
            }
        else if (testMode == BYTE_WRITE_MODE)
            {
            for (i = 0; i < pEepromTable->pageSize; i++)
                if (sysI2cEepromByteWrite (pDevCtrl, (j * pEepromTable->pageSize) + i, pTbuf[i]) == ERROR)
                    goto sysI2cEepromTest_error;
            }

        memset(pRbuf, 0, pEepromTable->pageSize);

        if (sysI2cEepromRead (pDevCtrl, (j * pEepromTable->pageSize), pEepromTable->pageSize, pRbuf) == ERROR)
            goto sysI2cEepromTest_error;

        if (memcmp (pTbuf, pRbuf, pEepromTable->pageSize) != 0)
            {
            printf ("sysI2cEepromTest: Error in page %d 0x%04x\r\n", j, (j * pEepromTable->pageSize));

            d(pTbuf, pEepromTable->pageSize, 1);
            d(pRbuf, pEepromTable->pageSize, 1);

            goto sysI2cEepromTest_error;
            }
        }

    free (pDrvCtrl);
    free (pDevCtrl);
    free (pTbuf);
    free (pRbuf);

    printf ("sysI2cEepromTest: Success\r\n");

    return OK;

sysI2cEepromTest_error:

    free (pDrvCtrl);
    free (pDevCtrl);
    free (pTbuf);
    free (pRbuf);

    printf ("sysI2cEepromTest: Failure\r\n");

    return ERROR;
    }

