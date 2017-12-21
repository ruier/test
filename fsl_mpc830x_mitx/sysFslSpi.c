/* sysFslSpi.c - FreeScale PowerPC SPI driver */

/*
* Copyright (c) 2011 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01a,30jun11,e_d  Writen.
*/

/*
DESCRIPTION
This is a driver for the FreeScale's serial peripheral interface
(SPI) engine. It is intended for use by client drivers performing other high
level functions.
*/


/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <taskLib.h>
#include <hwif/util/hwMemLib.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* defines */

#define SPI_BASE            IMMR_BASE + 0x7020
#define SPMODE              0x00   /* eSPI mode register */
#define SPIE                0x04   /* eSPI event register */
#define SPIM                0x08   /* eSPI mask register */
#define SPCOM               0x0C   /* eSPI command register */
#define SPITF               0x10   /* eSPI transmit FIFO access register*/
#define SPIRF               0x14   /* eSPI receive FIFO access register */

/* bit fields */

/* eSPI Mode Register bits */

#define SPI_LOOP_BIT        1
#define SPI_LOOP            (0x01 << (31 - SPI_LOOP))
#define CI_BIT              2
#define CI                  (0x01 << (31 - CI_BIT))
#define CP_BIT              3
#define CP                  (0x01 << (31 - CP_BIT))
#define DIV16_BIT           4
#define DIV16               (0x01 << (31 - DIV16_BIT))
#define REV_BIT             5
#define REV                 (0x01 << (31 - REV_BIT))
#define MS_BIT              6
#define MS                  (0x01 << (31 - MS_BIT))
#define SPI_EN_BIT          7
#define SPI_EN              (0x01 << (31 - SPI_EN_BIT))
#define LEN_BIT             11
#define LEN                 (0x01 << (31 - LEN_BIT))
#define PM_BIT              15
#define PM_BIT_MASK         0x000F0000
#define ODD_BIT             19
#define ODD                 (0x01 << (31 - ODD_BIT))

#define SPI_CLK_VALUE       (0x1 << (31 - PM_BIT))
#define SPI_MODEINIT_DATA   REV | MS | SPI_EN | DIV16 | SPI_CLK_VALUE |0x00700000;
#define LEN_MASK            0xFF0FFFFF

/* eSPI Event Register bits */

#define LT_BIT             17
#define LT                 (0x01 << (31 - LT_BIT))
#define DNR_BIT            18
#define DNR                (0x01 << (31 - DNR_BIT))
#define OV_BIT             19
#define OV                (0x01 << (31 - OV_BIT))
#define UN_BIT             20
#define UN                 (0x01 << (31 - UN_BIT))
#define MME_BIT            21
#define MME                (0x01 << (31 - MME_BIT))
#define NE_BIT             22
#define NE                 (0x01 << (31 - NE_BIT))
#define NF_BIT             23
#define NF                 (0x01 << (31 - NF_BIT))

#define SPI_EVENT_CLR       0xFFFFFFFF

/* eSPI Command Register bits */

#define LCT_BIT             9
#define LCT                 (0x01 << (31 - LCT_BIT))

#define SPI_REG_READ_8(addr)    *((VUINT8 *)(SPI_BASE + addr))
#define SPI_REG_READ_16(addr)   *((VUINT16 *)(SPI_BASE + addr))
#define SPI_REG_READ_32(addr)   *((VUINT32 *)(SPI_BASE + addr))

#define SPI_REG_WRITE_8(addr, data)     \
                                *((VUINT8 *)(SPI_BASE + addr)) = data
#define SPI_REG_WRITE_16(addr, data)    \
                                *((VUINT16 *)(SPI_BASE + addr)) = data
#define SPI_REG_WRITE_32(addr, data)    \
                                *((VUINT32 *)(SPI_BASE + addr)) = data

#define SPI_TIMEOUT 1000

/* locals */

/*******************************************************************************
*
* sysSpiInit - Freescale SPI controller initialization
*
* The purpose of this function is to perform SPI conctoller initialization
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSpiInit (void)
    {

    SPI_REG_WRITE_32(SPMODE, 0x0);

    /* clear any previous events */

    SPI_REG_WRITE_32(SPIE, SPI_EVENT_CLR);

    /* SPI controller disable all interrupt source */

    SPI_REG_WRITE_32(SPIM, 0x00);

    SPI_REG_WRITE_32(SPMODE, SPI_MODEINIT_DATA );

    return;
    }

/*******************************************************************************
*
* sysSpiRead - read data from SPI receive FIFO on poll mode
*
* The purpose of this function is to read data from SPI receive FIFO to dest
* address.
*
* RETURNS: N/A.
*
* ERRNO
*/

void sysSpiRead
    (
    UINT16 * destAddr,   /* dest address to save data readed from FIFO */
    UINT8    dataLen8
    )
    {
    UINT32 regValue = 0;

    regValue = SPI_REG_READ_32(SPMODE);
    regValue = (regValue & REV) >> REV_BIT;

    if((SPI_REG_READ_32(SPIE) & NE) != 0x0)
        {
        *destAddr = SPI_REG_READ_32(SPIRF);
        }
    if(dataLen8)
        *destAddr = *destAddr >> (8 * regValue);

    return;
    }

/*******************************************************************************
*
* sysSpiWriteData - write data to SPI device on poll mode
*
* The purpose of this function is to write data to SPI device on poll mode.
*
* RETURNS: N/A
*
* ERRNO
*/

STATUS sysSpiWriteData
    (
    void   * pData,      /* data source pointer */
    UINT8    bitLen      /* data length for read type command */
    )
    {
    UINT32 regValue = 0;
    UINT32 timeOut = 0;

    if((bitLen > 32) || (bitLen > 16))
        return ERROR;

    regValue = SPI_REG_READ_32(SPMODE);
    regValue &= LEN_MASK;

    if(bitLen < 32)
        {
        regValue |= ((bitLen - 1) << (31 - LEN_BIT));
        }

    regValue |= REV;
    SPI_REG_WRITE_32(SPMODE, regValue);

    regValue = *(UINT32 *)(pData);
    regValue = (regValue >> (32 - bitLen));
    SPI_REG_WRITE_32(SPITF, regValue);

    regValue = LCT;
    SPI_REG_WRITE_32(SPCOM, regValue);

    while(((SPI_REG_READ_32(SPIE) & LT) == 0x0) || ((timeOut++) < SPI_TIMEOUT));

    if(timeOut >= SPI_TIMEOUT)
        return ERROR;

    return OK;
    }


