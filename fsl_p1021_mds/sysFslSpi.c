/* sysFslSpi.c - FreeScale PowerPC eSPI driver */

/*
* Copyright (c) 2009-2010 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01c,17dec10,y_y  add delay for write data unstable 
01b,17aug10,e_d  fix issue for write data
01a,12jun09,syt  Writen.
*/

/*
DESCRIPTION
This is a driver for the FreeScale's enhanced serial peripheral interface
(eSPI) engine. It is intended for use by client drivers performing other high
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

#include "sysFslSpi.h"

/* externs */

IMPORT FUNCPTR _func_logMsg;

/* defines */

#define SPI_REG_READ_8(addr)    *((VUINT8 *)(eSPI_BASE + addr))
#define SPI_REG_READ_16(addr)   *((VUINT16 *)(eSPI_BASE + addr))
#define SPI_REG_READ_32(addr)   *((VUINT32 *)(eSPI_BASE + addr))

#define SPI_REG_WRITE_8(addr, data)     \
                                *((VUINT8 *)(eSPI_BASE + addr)) = data
#define SPI_REG_WRITE_16(addr, data)    \
                                *((VUINT16 *)(eSPI_BASE + addr)) = data
#define SPI_REG_WRITE_32(addr, data)    \
                                *((VUINT32 *)(eSPI_BASE + addr)) = data

#define RX_FIFO_USED_NUM    \
            ((SPI_REG_READ_32(SPIE) >> (31 - RXCNT)) & 0x3f)
#define TX_FIFO_FREE_NUM    \
            ((SPI_REG_READ_32(SPIE) >> (31 - TXCNT)) & 0x3f)

#ifdef _WRS_CONFIG_SMP
LOCAL spinlockTask_t spiSpinlockTask;    /* Task-callable spinlock */
#   define SPI_SPIN_LOCK_INIT  SPIN_LOCK_TASK_INIT((void *)&spiSpinlockTask, 0)
#   define SPI_SPIN_LOCK_TAKE  SPIN_LOCK_TASK_TAKE(&spiSpinlockTask)
#   define SPI_SPIN_LOCK_GIVE  SPIN_LOCK_TASK_GIVE(&spiSpinlockTask)
#else  /* _WRS_CONFIG_SMP */
#   define SPI_SPIN_LOCK_INIT
#   define SPI_SPIN_LOCK_TAKE
#   define SPI_SPIN_LOCK_GIVE
#endif  /* _WRS_CONFIG_SMP */

#undef SPI_FLASH_DEBUG

#ifdef SPI_FLASH_DEBUG
#   define SPI_FLASH_DEBUG_MSG(fmt, a1, a2, a3, a4, a5, a6) \
    do { \
        if (_func_logMsg != NULL) \
        _func_logMsg(fmt, (int)(a1), (int)(a2), (int)(a3),  \
                          (int)(a4), (int)(a5), (int)(a6)); \
    } while ((0))
#else  /* SPI_FLASH_DEBUG */
#   define SPI_FLASH_DEBUG_MSG(fmt, a1, a2, a3, a4, a5, a6)
#endif  /* SPI_FLASH_DEBUG */
/* locals */

LOCAL BOOL  firstSpiCmd;

/*******************************************************************************
*
* sysSpiInit - Freescale eSPI controller initialization
*
* The purpose of this function is to perform eSPI conctoller initialization
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSpiInit (void)
    {

    SPI_SPIN_LOCK_INIT;

    SPI_SPIN_LOCK_TAKE;

    /* clear any previous events */

    SPI_REG_WRITE_32(SPIE, SPI_EVENT_CLR);

    /* enable SPI, SPI_THR and SPI_RHR are all 32 Bytes. */

    SPI_REG_WRITE_32(SPMODE, SPI_TXTHR_LEN | SPI_RXTHR_LEN);

    SPI_REG_WRITE_32(SPMODE, SPI_EN | SPI_REG_READ_32(SPMODE));

    /* SPI controller works on semiduplex mode, disable all interrupt source */

    SPI_REG_WRITE_32(SPIM, 0x00);

    SPI_REG_WRITE_32(SPMODE0, SPI_CS0_CFG_INIT);
    SPI_REG_WRITE_32(SPMODE1, SPI_CS1_CFG_INIT);
    SPI_REG_WRITE_32(SPMODE2, SPI_CS2_CFG_INIT);
    SPI_REG_WRITE_32(SPMODE3, SPI_CS3_CFG_INIT);

    SPI_SPIN_LOCK_GIVE;

    firstSpiCmd = TRUE;

    return;
    }

/*******************************************************************************
*
* sysSpiRead - read data from SPI receive FIFO on poll mode
*
* The purpose of this function is to read data from eSPI receive FIFO to dest
* address. Parameter "dataNum" should be set equal to Parameter "dataLen" of
* the "sysSpiWriteCmd" function.
*
* RETURNS:  OK or ERROR if there is an error.
*
* ERRNO
*/

void sysSpiRead
    (
    UINT8 * destAddr,   /* dest address to save data readed from FIFO */
    UINT32  dataNum     /* data number to be read ftom FIFO */
    )
    {
    UINT32 i;

    SPI_SPIN_LOCK_TAKE;

    while (dataNum >= 4)
        {
        while (RX_FIFO_USED_NUM < 4);

        *((UINT32 *)destAddr) = SPI_REG_READ_32(SPIRF);

        destAddr += 4;
        dataNum -= 4;
        }

    if ((dataNum % 4) == 0)
        {
        SPI_SPIN_LOCK_GIVE;

        return;
        }

    while (RX_FIFO_USED_NUM < (dataNum % 4));

    for (i = 0; i < (dataNum % 4); i++)
        *(destAddr + i) = SPI_REG_READ_8(SPIRF + i);

    SPI_SPIN_LOCK_GIVE;

    return;
    }

/*******************************************************************************
*
* sysSpiWriteCmd - send a command to SPI device
*
* The purpose of this function is to write control commands to a specific
* device on the SPI bus. If the SPI master does not need a response from
* the slave device or not sent data to the salve device, then the parameter
* "dataLen" should be set to 0.Otherwise, set "dataLen" to the number of bytes
* for the slave's response or received .
*
* RETURNS: N/A
*
* ERRNO
*/

STATUS sysSpiWriteCmd
    (
    UINT32  csNum,      /* chip select number */
    UINT32  cmdType,    /* "No reception" or "Normal operation" type */
    UINT8 * pCmd,       /* SPI command pointer */
    UINT32  cmdLen,     /* SPI cmd length (bytes) */
    UINT32  dataLen     /* data length for read or write to slave device */
    )
    {
    UINT32  i;
    UINT32 * cmdtemp;
    SPI_SPIN_LOCK_TAKE;

    /*
     * The firstSpiCmd is used for first cmd is sent. we use the
     * DON bit in spi event register to estimate transmit command
     * or data success or not. But this bit is set to zero after reset.
     * So we must skip the test logic at first time.
     */

    if (firstSpiCmd == TRUE)
        {
        firstSpiCmd = FALSE;
        }
    else
        {
        while (!(SPI_REG_READ_32(SPIE) & DON))
            taskDelay(1);
        }

    if (cmdLen > 4)
        {
        SPI_FLASH_DEBUG_MSG("SPI command length should not be larger than 4 bytes\r\n",
                            0, 0, 0, 0, 0, 0);

        SPI_SPIN_LOCK_GIVE;

        return ERROR;
        }

    SPI_REG_WRITE_32(SPIE, SPI_EVENT_CLR);

    /* Only send out commands, SPI device needn't to respond */

    if (cmdType == TRANS_ONLY)
        {

        /* Only send out commands, SPI device needn't to respond */

        SPI_REG_WRITE_32(SPCOM, SPI_CS(csNum) | SPCOM_TO |
                                (cmdLen + dataLen - 1));
        }
    else
        {

        /* data stream should be responsed from SPI device  */

        SPI_REG_WRITE_32(SPCOM, SPI_CS(csNum) | SPI_SKIP(cmdLen) |
                                (cmdLen + dataLen - 1));
        }
    if(cmdLen == 4)
        {
        cmdtemp = (UINT32 *)pCmd;
        SPI_REG_WRITE_32(SPITF, *cmdtemp);
        SPI_SPIN_LOCK_GIVE;

        return OK;
        }
    for (i = 0; i < cmdLen; i++)
        SPI_REG_WRITE_8((SPITF + i), *(pCmd + i));

    SPI_SPIN_LOCK_GIVE;

    return OK;
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

void sysSpiWriteData
    (
    UINT8 * pData,      /* data source pointer */
    UINT32  dataLen     /* data length for read type command */
    )
    {
    UINT32  i;
    SPI_SPIN_LOCK_TAKE;
    while (dataLen / 4)
        {
        while (TX_FIFO_FREE_NUM < 4);

        SPI_REG_WRITE_32(SPITF, *((VUINT32 *)pData));
        
        /* 
		 * For some SPI flash(eg:p1021), Page Programm must meet some 
		 *  time requirements, so delay some time to let programm finish.
		 */
        sysUsDelay(10);

        pData += 4;
        dataLen -= 4;
        }

    if ((dataLen % 4) == 0)
        {
        SPI_SPIN_LOCK_GIVE;

        return;
        }

    while (TX_FIFO_FREE_NUM < (dataLen % 4));

    for (i = 0; i < (dataLen % 4); i++)
        SPI_REG_WRITE_8((SPITF + i), *(pData + i));

    SPI_SPIN_LOCK_GIVE;

    return;
    }

