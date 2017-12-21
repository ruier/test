/* sysApmSpi.c - APM86290 SPI driver */

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
01b,01mar12,syt  fixed compile warnings.
01a,12Dec11,syt  written.
*/

/*
DESCRIPTION
This is a driver for the APM's Serial Peripheral Interface (SPI) engine.
It is intended for use by client drivers performing other high level functions.
*/

/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <taskLib.h>
#include <../h/hwif/util/hwMemLib.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sysApmSpi.h"

/* externs */
IMPORT UINT32 sysApbClkFreqGet (void);

/* defines */

#define SPI_REG_READ_32(pDrv, addr)     sysRead32 (pDrv->regBase + addr)
#define SPI_REG_WRITE_32(pDrv, addr, data)  sysWrite32 (pDrv->regBase + addr, data)

#ifdef _WRS_CONFIG_SMP
#   define SPI_SPIN_LOCK_INIT(pDrv)     SPIN_LOCK_TASK_INIT((void *)&(pDrv->spinlockTask), 0)
#   define SPI_SPIN_LOCK_TAKE(pDrv)     SPIN_LOCK_TASK_TAKE(&(pDrv->spinlockTask))
#   define SPI_SPIN_LOCK_GIVE(pDrv)     SPIN_LOCK_TASK_GIVE(&(pDrv->spinlockTask))
#else  /* _WRS_CONFIG_SMP */
#   define SPI_SPIN_LOCK_INIT(pDrv)
#   define SPI_SPIN_LOCK_TAKE(pDrv)
#   define SPI_SPIN_LOCK_GIVE(pDrv)
#endif  /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* sysSpiDevInit - APM SPI controller initialization
*
* The purpose of this function is to perform SPI conctoller initialization
*
* RETURNS: N/A
*
* ERRNO
*/

SPI_DRV_CTRL * sysSpiDevInit
    (
    UINT32  regBase     /* SPI controller registers bank baseaddress */
    )
    {
    SPI_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (SPI_DRV_CTRL *)malloc (sizeof (SPI_DRV_CTRL));

    if (pDrvCtrl == NULL)
        return NULL;

    memset (pDrvCtrl, 0, sizeof (SPI_DRV_CTRL));

    pDrvCtrl->spiDevSem = semMCreate (SEM_Q_PRIORITY);

    if (pDrvCtrl->spiDevSem == SEM_ID_NULL)
        {
        free (pDrvCtrl);
        return NULL;
        }

    SPI_SPIN_LOCK_INIT(pDrvCtrl);

    SPI_SPIN_LOCK_TAKE(pDrvCtrl);

    pDrvCtrl->regBase = regBase;
    pDrvCtrl->apbClkFreqGet = (void *)sysApbClkFreqGet;

    /* Set SPI control register with default values */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_CTRLR0, (TMOD_EP_R << TMOD_SHIFT)       | \
                                            (SPI_MODE_3 << SPI_MODE_SHIFT)  | \
                                            (FRF_MOTO_SPI << SPI_FRF_SHIFT) | \
                                            DFS_BIT_8);
    SPI_REG_WRITE_32(pDrvCtrl, SPIx_CTRLR1, 0);

    /* Diable SPI controller */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_SSIENR, 0);

    /* Mask all SPI interrupt sources */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_IMR, 0);

    /* Configre threshold value for the transmit and receive FIFO memory */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_TXFTLR, 0);
    SPI_REG_WRITE_32(pDrvCtrl, SPIx_RXFTLR, 0);

    SPI_SPIN_LOCK_GIVE(pDrvCtrl);

    return (pDrvCtrl);
    }

/*******************************************************************************
*
* sysSpiCsConfig - Configurate and enable APM SPI CS bank
*
* The purpose of this function is to configurate and enable APM SPI CS bank
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSpiCsConfig
    (
    SPI_DEV_CTRL * spiDevCtlr
    )
    {
    UINT32  regValue;
    UINT32  frqDiv;
    SPI_DRV_CTRL * pDrvCtrl = (SPI_DRV_CTRL *)spiDevCtlr->pDrvCtrl;

    while (SPI_REG_READ_32(pDrvCtrl, SPIx_SR) & SR_BUSY);

    /* Diable SPI controller */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_SSIENR, 0);

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_SER, spiDevCtlr->csNum);

    /* Configure SPI controller chip select */

    regValue = SPI_REG_READ_32(pDrvCtrl, SPIx_CTRLR0);
    regValue &= ~(TMOD_MASK | SPI_MODE_MASK | SPI_FRF_MASK | DFS_MASK);
    regValue |= spiDevCtlr->spiDev.ctrlr0;
    SPI_REG_WRITE_32(pDrvCtrl, SPIx_CTRLR0, regValue);

    /* Configure SPI clock frequency */

    frqDiv = pDrvCtrl->apbClkFreqGet() / spiDevCtlr->devFrq;

    if (frqDiv == 0)
        frqDiv = 1;

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_BAUDR, frqDiv);
    }

/*******************************************************************************
*
* sysSpiTransfer - write/read data to/from SPI device (with poll mode)
*
* The purpose of this function is to write/read data to/from SPI device
* on poll mode.
*
* RETURNS: N/A
*
* ERRNO
*/

VOID sysSpiTransfer
    (
    SPI_DEV_CTRL * spiDevCtlr
    )
    {
    SPI_DRV_CTRL *  pDrvCtrl = (SPI_DRV_CTRL *)spiDevCtlr->pDrvCtrl;

    semTake (pDrvCtrl->spiDevSem, WAIT_FOREVER);
    SPI_SPIN_LOCK_TAKE(pDrvCtrl);

    /* config SPI CS parameters */

    sysSpiCsConfig (spiDevCtlr);

    /* Set the data length to be received */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_CTRLR1, (spiDevCtlr->rcvLen) ? (spiDevCtlr->rcvLen - 1) : 0);

    /* Enable SPI controller */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_SSIENR, SPI_ENABLE);

    /* Wait for SPI enable process compelted */

    sysUsDelay(100);

    /* Fill in SPI command */

    while (!(SPI_REG_READ_32(pDrvCtrl, SPIx_SR) & SR_TFE))
        sysUsDelay (10);

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_DR_LOW, spiDevCtlr->cmdIdx);

    /*
     * Write or read data to or from SPI device, need to send SPI data
     * address first.
     */

    if (spiDevCtlr->needDistAddr == TRUE)
        {
        SPI_REG_WRITE_32(pDrvCtrl, SPIx_DR_LOW, ADDR_HI(spiDevCtlr->distAddr));
        SPI_REG_WRITE_32(pDrvCtrl, SPIx_DR_LOW, ADDR_LO(spiDevCtlr->distAddr));
        }

    while ((SPI_REG_READ_32(pDrvCtrl, SPIx_SR) & SR_TFNF) && (spiDevCtlr->transLen > 0))
        {
        SPI_REG_WRITE_32(pDrvCtrl, SPIx_DR_LOW, *spiDevCtlr->pTransBuf);
        spiDevCtlr->pTransBuf++;
        spiDevCtlr->transLen--;
        }

    /* Start SPI host transfer */

    SPI_REG_WRITE_32(pDrvCtrl, SPIx_SER, spiDevCtlr->csNum);

    while (spiDevCtlr->transLen > 0)
        {
        while (SPI_REG_READ_32(pDrvCtrl, SPIx_SR) & SR_TFNF)
            {
            SPI_REG_WRITE_32(pDrvCtrl, SPIx_DR_LOW, *spiDevCtlr->pTransBuf);
            spiDevCtlr->pTransBuf++;
            spiDevCtlr->transLen--;

            if (spiDevCtlr->transLen == 0)
                break;
            }
        }

    /* Wait for SPI transmate completed */

    while (SPI_REG_READ_32(pDrvCtrl, SPIx_TXFLR) != 0)
        sysUsDelay (10);

    /* Start to recieve data send from SPI device */

    while (spiDevCtlr->rcvLen > 0)
        {
        while (SPI_REG_READ_32(pDrvCtrl, SPIx_RXFLR) != 0)
            {
            *((UINT8 *)(spiDevCtlr->pRcvBuf)) = (UINT8)SPI_REG_READ_32(pDrvCtrl, SPIx_DR_LOW);
            spiDevCtlr->pRcvBuf++;
            spiDevCtlr->rcvLen--;
            }
        sysUsDelay(10);
        }

    SPI_SPIN_LOCK_GIVE(pDrvCtrl);
    semGive (pDrvCtrl->spiDevSem);
    }
