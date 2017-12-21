/* sysApmSpiFlash.c - apm_serengeti SPI Flash driver */

/*
* Copyright (c) 2011, 2012 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,01mar12,syt  updated function sysSpiFlashDevinit().
01a,12Dec11,syt  written.
*/

/*
DESCRIPTION
This is a driver for SPI Flash device connected on the APM86290's
Serial Peripheral Interface (SPI). It is intended for use by client drivers
performing other high level functions.
*/

/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* forward */

VOID    sysSpiFlashWriteEnable (SPI_DEV_CTRL *);
VOID    sysSpiFlashWriteDisable (SPI_DEV_CTRL *);
UINT8   sysSpiFlashReadStatusReg (SPI_DEV_CTRL *);
VOID    sysSpiFlashWriteStatusReg (SPI_DEV_CTRL *, UINT8);

/* defines */

#define SPI_FLASH_SIZE              0x800
#define SPI_FLASH_PAGE_SIZE         0x20
#define SPI_FLASH_PAGE_ADDR_MASK    0x1f
#define SPI_FLASH_TRANS_BUF_LEN     SPI_FLASH_PAGE_SIZE

#define FLASH_CMD_WRSR              0x01    /* Write Status Register */
#define FLASH_CMD_WRITE             0x02    /* Write Data to Memory */
#define FLASH_CMD_READ              0x03    /* Read Data from Memory */
#define FLASH_CMD_WRDI              0x04    /* Disable Write Operations */
#define FLASH_CMD_RDSR              0x05    /* Read Status Register */
#define FLASH_CMD_WREN              0x06    /* Enable Write Operations */

#define FLASH_RDY_MASK              0x01    /* Ready to accept commands */

#define PAGE_WRITE                  0       /* SPI flash page write mode */
#define BYTE_WRITE                  1       /* SPI flash byte write mode */

/*******************************************************************************
*
* sysSpiFlashDevinit - APM SPI Flash initialization
*
* This function create SPI Flash device instance and set the default parameters
* to the SPI CS bank which SPI Flash connected. The important steps we do here
* are to allocate a memory region for the SPI transmit buffer, set SPI frame
* format, SPI bus mode, SPI Data Frame and SPI CLK frequency parameters.
*
* RETURNS: pDevCtrl structure pointer if initialize success, others ERROR.
*
* ERRNO
*/

SPI_DEV_CTRL * sysSpiFlashDevinit
    (
    SPI_DRV_CTRL * pSpiDrvCtlr,
    UINT32         spiCsIndex
    )
    {
    SPI_DEV_CTRL * spiDevCtlr;

    spiDevCtlr = (SPI_DEV_CTRL *)malloc (sizeof(SPI_DEV_CTRL));

    if (spiDevCtlr == NULL)
        return NULL;

    memset (spiDevCtlr, 0x0, sizeof(SPI_DEV_CTRL));

    spiDevCtlr->pDrvCtrl = pSpiDrvCtlr;

    /* Create SPI transmit buffer */

    spiDevCtlr->pTransBuf = malloc(SPI_FLASH_TRANS_BUF_LEN);

    if (spiDevCtlr->pTransBuf == NULL)
        {
        free (spiDevCtlr);
        return NULL;
        }

    memset (spiDevCtlr->pTransBuf, 0x0, SPI_FLASH_TRANS_BUF_LEN);

    /* Set SPI_CS bank configuration parameters */

    spiDevCtlr->spiDev.parameter.frameFormat = FRF_MOTO_SPI; /* MOTO SPI frame format.*/
    spiDevCtlr->spiDev.parameter.spiMode = SPI_MODE_3;  /* SPI mode 3 */
    spiDevCtlr->spiDev.parameter.frameSize = DFS_BIT_8; /* SPI data frame size 8 bit */
    spiDevCtlr->devFrq = 5000000;                       /* SPI_CLK = 5MHz */
    spiDevCtlr->csNum = spiCsIndex;

    return (spiDevCtlr);
    }

/*******************************************************************************
*
* sysSpiFlashByteWrite - program a byte data into SPI flash device
*
* This function is to program a byre data into SPI flash device
*
* RETURNS: N/A
*
* ERRNO
*/

VOID  sysSpiFlashByteWrite
    (
    SPI_DEV_CTRL *  spiDevCtlr,
    UINT16          byteAddr,
    UINT8 *         sourceBuf
    )
    {
    /* Enable write to SPI flash */

    sysSpiFlashWriteEnable (spiDevCtlr);

    spiDevCtlr->spiDev.parameter.transMode = TMOD_TO;
    spiDevCtlr->cmdIdx = FLASH_CMD_WRITE;
    spiDevCtlr->distAddr = byteAddr;
    spiDevCtlr->pTransBuf = sourceBuf;
    spiDevCtlr->transLen = 1;
    spiDevCtlr->needDistAddr = TRUE;
    spiDevCtlr->rcvLen = 0;

    /* Program SPI Flash */

    sysSpiTransfer (spiDevCtlr);

    /* Wait for the program compeletd */

    while (sysSpiFlashReadStatusReg (spiDevCtlr) & FLASH_RDY_MASK)
        sysUsDelay (100);
    }

/*******************************************************************************
*
* sysSpiFlashPageWrite - program a page of data into SPI flash device
*
* This function is to program a page (32 bytes) of data into SPI flash device.
* SPI flash page address must be page aligned.
*
* RETURNS: N/A
*
* ERRNO
*/

STATUS  sysSpiFlashPageWrite
    (
    SPI_DEV_CTRL *  spiDevCtlr,
    UINT16          pageAddr,
    UINT8 *         sourceBuf
    )
    {
    if (pageAddr % SPI_FLASH_PAGE_SIZE)
        return ERROR;

    /* Enable write to SPI Flash */

    sysSpiFlashWriteEnable (spiDevCtlr);

    spiDevCtlr->spiDev.parameter.transMode = TMOD_TO;
    spiDevCtlr->cmdIdx = FLASH_CMD_WRITE;
    spiDevCtlr->needDistAddr = TRUE;
    spiDevCtlr->distAddr = pageAddr;
    spiDevCtlr->pTransBuf = sourceBuf;
    spiDevCtlr->transLen = SPI_FLASH_PAGE_SIZE;
    spiDevCtlr->rcvLen = 0;

    /* Program SPI Flash */

    sysSpiTransfer (spiDevCtlr);

    /* Wait for the program compeletd */

    while (sysSpiFlashReadStatusReg (spiDevCtlr) & FLASH_RDY_MASK)
        sysUsDelay (100);
    
    return OK;
    }

/*******************************************************************************
*
* sysSpiFlashRead - read data from SPI flash device
*
* This function is to read data from SPI flash device
*
* RETURNS: N/A
*
* ERRNO
*/

VOID sysSpiFlashRead
    (
    SPI_DEV_CTRL *  spiDevCtlr,
    UINT16          distAddr,
    UINT32          dataLen,
    UINT8 *         rcvBuf
    )
    {
    spiDevCtlr->spiDev.parameter.transMode = TMOD_EP_R;
    spiDevCtlr->cmdIdx = FLASH_CMD_READ;
    spiDevCtlr->distAddr = distAddr;
    spiDevCtlr->needDistAddr = TRUE;
    spiDevCtlr->transLen = 0;
    spiDevCtlr->pRcvBuf = rcvBuf;
    spiDevCtlr->rcvLen = dataLen;

    /* Send out EPPROM read command */

    sysSpiTransfer (spiDevCtlr);
    }

/*******************************************************************************
*
* sysSpiFlashWriteEnable - enable write operation on SPI Flash device
*
* This function is to enable write operation on SPI Flash device.
*
* RETURNS: N/A
*
* ERRNO
*/

VOID sysSpiFlashWriteEnable
    (
    SPI_DEV_CTRL * spiDevCtlr
    )
    {
    /* Fill the configuration parameters for the Flash Write enable action */

    spiDevCtlr->spiDev.parameter.transMode = TMOD_TO;
    spiDevCtlr->cmdIdx = FLASH_CMD_WREN;
    spiDevCtlr->needDistAddr = FALSE;
    spiDevCtlr->transLen = 0;
    spiDevCtlr->rcvLen = 0;

    /* Send out the Flash command */

    sysSpiTransfer (spiDevCtlr);
    }

/*******************************************************************************
*
* sysSpiFlashWriteDisable - disable write operation on SPI Flash device
*
* This function is to disable write operation on SPI Flash device.
*
* RETURNS: N/A
*
* ERRNO
*/

VOID sysSpiFlashWriteDisable
    (
    SPI_DEV_CTRL * spiDevCtlr
    )
    {
    /* Fill the configuration parameters for the Flash Write enable action */

    spiDevCtlr->spiDev.parameter.transMode = TMOD_TO;
    spiDevCtlr->cmdIdx = FLASH_CMD_WRDI;
    spiDevCtlr->needDistAddr = FALSE;
    spiDevCtlr->transLen = 0;
    spiDevCtlr->rcvLen = 0;

    /* Send out the Flash command */

    sysSpiTransfer (spiDevCtlr);
    }

/*******************************************************************************
*
* sysSpiFlashReadStatusReg - read SPI Flash device status register
*
* This function is to read SPI Flash device status register.
*
* RETURNS: N/A
*
* ERRNO
*/

UINT8 sysSpiFlashReadStatusReg
    (
    SPI_DEV_CTRL * spiDevCtlr
    )
    {
    UINT8   regVal = 0;

    /* Fill the configuration parameters for the Flash Write enable action */

    spiDevCtlr->spiDev.parameter.transMode = TMOD_EP_R;
    spiDevCtlr->cmdIdx = FLASH_CMD_RDSR;
    spiDevCtlr->needDistAddr = FALSE;
    spiDevCtlr->transLen = 0;
    spiDevCtlr->pRcvBuf = (UINT8 *)&regVal;
    spiDevCtlr->rcvLen = 1;

    /* Send out the Flash command */

    sysSpiTransfer (spiDevCtlr);

    return regVal;
    }

/*******************************************************************************
*
* sysSpiFlashWriteStatusReg - Write SPI Flash device status register
*
* This function is to write SPI Flash device status register.
*
* RETURNS: N/A
*
* ERRNO
*/

VOID  sysSpiFlashWriteStatusReg
    (
    SPI_DEV_CTRL *  spiDevCtlr,
    UINT8           regVal
    )
    {
    /* Fill the configuration parameters for the Flash Write enable action */

    spiDevCtlr->spiDev.parameter.transMode = TMOD_TO;
    spiDevCtlr->cmdIdx = FLASH_CMD_WRSR;
    spiDevCtlr->needDistAddr = FALSE;
    spiDevCtlr->transLen = 1;
    spiDevCtlr->rcvLen = 0;
    *spiDevCtlr->pTransBuf = regVal;

    /* Send out the Flash command */

    sysSpiTransfer (spiDevCtlr);
    }

/*******************************************************************************
*
* sysSpiFlashTest - test SPI flash CAT25160 read/write function
*
* test page write and read routine.
*
* RETURNS: OK, or ERROR
*
* ERRNO: N/A
*/

STATUS  sysSpiFlashTest
    (
    UINT32  spiHostNo,
    UINT32  spiCsNo,
    UINT32  progMode
    )
    {
    SPI_DRV_CTRL * pDrvCtrl = NULL;
    SPI_DEV_CTRL * spiDevCtlr = NULL;
    UINT8 *        pTbuf;
    UINT8 *        pRbuf;
    UINT32         i, j;

    if (spiHostNo == 0)
        pDrvCtrl = sysSpiDevInit (APM_SPI0_BASE);
    else if (spiHostNo == 1)
        pDrvCtrl = sysSpiDevInit (APM_SPI1_BASE);

    if (pDrvCtrl == NULL)
        return ERROR;

    spiDevCtlr = sysSpiFlashDevinit (pDrvCtrl, SPI_CS(spiCsNo));

    if (spiDevCtlr == NULL)
        {
        free (pDrvCtrl);
        return ERROR;
        }

    pTbuf = malloc(SPI_FLASH_PAGE_SIZE);
    if (pTbuf == NULL)
        {
        free (pDrvCtrl);
        free (spiDevCtlr);

		return ERROR;
        }

    pRbuf = malloc(SPI_FLASH_PAGE_SIZE);
    if (pRbuf == NULL)
        {
        free (pDrvCtrl);
        free (spiDevCtlr);
        free (pTbuf);

        return ERROR;
        }

    for (j = 0; j < (SPI_FLASH_SIZE / SPI_FLASH_PAGE_SIZE); j++)
        {
        for (i = 0; i <SPI_FLASH_PAGE_SIZE; i++)
            pTbuf[i] = (i + j + 10);

        memset(pRbuf, 0, SPI_FLASH_PAGE_SIZE);

        if (progMode == PAGE_WRITE)
            sysSpiFlashPageWrite (spiDevCtlr, (j*SPI_FLASH_PAGE_SIZE), pTbuf);
        else if (progMode == BYTE_WRITE)
            {
            for (i =0; i < SPI_FLASH_PAGE_SIZE; i++)
                sysSpiFlashByteWrite (spiDevCtlr, (j*SPI_FLASH_PAGE_SIZE) + i, pTbuf+i);
            }

        sysSpiFlashRead (spiDevCtlr, (j*SPI_FLASH_PAGE_SIZE), SPI_FLASH_PAGE_SIZE, pRbuf);

        if (memcmp (pTbuf, pRbuf, SPI_FLASH_PAGE_SIZE) != 0)
            {
            printf ("sysSpiFlashTest: Error in sector %d 0x%04x\r\n", j, (j*SPI_FLASH_PAGE_SIZE));

            free (pDrvCtrl);
            free (spiDevCtlr);
            free (pTbuf);
            free (pRbuf);

            return ERROR;
            }
        }

    free (pDrvCtrl);
    free (spiDevCtlr);
    free (pTbuf);
    free (pRbuf);

    printf ("sysSpiFlashTest: OK\r\n");

    return OK;
    }
