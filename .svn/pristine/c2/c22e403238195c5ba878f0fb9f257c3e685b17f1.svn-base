/* sysApmSpi.h - APM86290 SPI driver header file */

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
01a,10jun09,syt  written
*/

#ifndef __INCsysApmSpih
#define __INCsysApmSpih

#ifdef __cplusplus
extern "C" {
#endif

#include <semLib.h>
#include "config.h"

/* SPI registers, fields and masks */

/* SPI register offsets */

#define SPIx_CTRLR0         0x00
#define SPIx_CTRLR1         0x04
#define SPIx_SSIENR         0x08
#define SPIx_SER            0x10
#define SPIx_BAUDR          0x14
#define SPIx_TXFTLR         0x18
#define SPIx_RXFTLR         0x1c
#define SPIx_TXFLR          0x20
#define SPIx_RXFLR          0x24
#define SPIx_SR             0x28
#define SPIx_IMR            0x2c
#define SPIx_ISR            0x30
#define SPIx_RISR           0x34
#define SPIx_TXOICR         0x38
#define SPIx_RXOICR         0x3c
#define SPIx_RXUICR         0x40
#define SPIx_MSTICR         0x44
#define SPIx_ICR            0x48
#define SPIx_IDR            0x58
#define SPIx_VIDR           0x5c
#define SPIx_DR_LOW         0x60
#define SPIx_DR_HIGH        0xfc

/* SPI controller registers bit fields */

/* SPIxCTRLR0 bit defines */

#define TMOD_MASK           0x00000300  /* Transfer Mode Mask */
#define TMOD_SHIFT          8
#define TMOD_TR             0           /* Transmit & Receive */
#define TMOD_TO             1           /* Transmit Only */
#define TMOD_RO             2           /* Receive Only */
#define TMOD_EP_R           3           /* EEPROM Read */

#define SPI_MODE_MASK       0x000000c0
#define SPI_MODE_SHIFT      6
#define SPI_MODE_0          0           /* SPI mode 0 */
#define SPI_MODE_1          1           /* SPI mode 1 */
#define SPI_MODE_2          2           /* SPI mode 2 */
#define SPI_MODE_3          3           /* SPI mode 3 */

#define SPI_FRF_MASK        0x00000030
#define SPI_FRF_SHIFT       4
#define FRF_MOTO_SPI        0

#define DFS_MASK            0x0f
#define DFS_BIT_4           0x03        /* 4-bit serial data transfer */
#define DFS_BIT_5           0x04        /* 5-bit serial data transfer */
#define DFS_BIT_6           0x05        /* 6-bit serial data transfer */
#define DFS_BIT_7           0x06        /* 7-bit serial data transfer */
#define DFS_BIT_8           0x07        /* 8-bit serial data transfer */
#define DFS_BIT_9           0x08        /* 9-bit serial data transfer */
#define DFS_BIT_10          0x09        /* 10-bit serial data transfer */
#define DFS_BIT_11          0x0a        /* 11-bit serial data transfer */
#define DFS_BIT_12          0x0b        /* 12-bit serial data transfer */
#define DFS_BIT_13          0x0c        /* 13-bit serial data transfer */
#define DFS_BIT_14          0x0d        /* 14-bit serial data transfer */
#define DFS_BIT_15          0x0e        /* 15-bit serial data transfer */
#define DFS_BIT_16          0x0f        /* 16-bit serial data transfer */

/* SPIxTXFTLR bit defines */

#define TFT_0               0x00        /* Transmit FIFO threshold level is 0 */
#define TFT_1               0x01        /* Transmit FIFO threshold level is 1 */
#define TFT_2               0x02        /* Transmit FIFO threshold level is 2 */
#define TFT_3               0x03        /* Transmit FIFO threshold level is 3 */
#define TFT_4               0x04        /* Transmit FIFO threshold level is 4 */
#define TFT_5               0x05        /* Transmit FIFO threshold level is 5 */
#define TFT_6               0x06        /* Transmit FIFO threshold level is 6 */
#define TFT_8               0x07        /* Transmit FIFO threshold level is 7 */

/* SPIxRXFTLR bit defines */

#define RFT_1               0x00        /* Receive FIFO threshold level is 1 */
#define RFT_2               0x01        /* Receive FIFO threshold level is 2 */
#define RFT_3               0x02        /* Receive FIFO threshold level is 3 */
#define RFT_4               0x03        /* Receive FIFO threshold level is 4 */
#define RFT_5               0x04        /* Receive FIFO threshold level is 5 */
#define RFT_6               0x05        /* Receive FIFO threshold level is 6 */
#define RFT_7               0x06        /* Receive FIFO threshold level is 7 */
#define RFT_8               0x07        /* Receive FIFO threshold level is 8 */

/* SPIx_SER bit defines */

#define SPI_CS(x)           (0x01 << x)
#define SPI_HOST(x)         (APM_SPI0_BASE + (x * 0x1000))

/* SPIxSR bit defines */

#define SR_RFNE             0x08        /* Receive FIFO Not Empty */
#define SR_TFE              0x04        /* Transmit FIFO Empty */
#define SR_TFNF             0x02        /* Transmit FIFO Not Full */
#define SR_BUSY             0x01        /* SPI is actively transferring data */

/* SPIxSSIENR bit defines  */

#define SPI_ENABLE          0x01
#define SPI_DISABLE         0x00

#define ADDR_HI(x)          ((x >> 8) & 0xff)
#define ADDR_LO(x)          (x & 0xff)

typedef struct spiDrvCtrl
    {
    UINT32          regBase;
    BOOL            polling;
    FUNCPTR         apbClkFreqGet;
    SEM_ID          spiDevSem;
    spinlockTask_t  spinlockTask;    /* Task-callable spinlock */
    } SPI_DRV_CTRL;

typedef struct spiDevCtrl
    {
    SPI_DRV_CTRL *  pDrvCtrl;
    UINT32          csNum;              /* SPI CS bank number */

    union
        {
        struct
            {
            UINT32  rsv0:22;            /* Reserved */
            UINT32  transMode:2;        /* SPI Transfer Mode */
            UINT32  spiMode:2;          /* SPI mode */
            UINT32  frameFormat:2;      /* SPI Frame Format */
            UINT32  frameSize:4;        /* SPI Data Frame Size */
            } parameter;

        UINT32  ctrlr0;                 /* SPI controller register0 */
        } spiDev;

    UINT32  transMode;                  /* SPI Transfer Mode */
    UINT32  devFrq;                     /* SPI device frequency */
    UINT16  cmdIdx;                     /* SPI command */
    UINT16  distAddr;                   /* SPI device internal address */
    BOOL    needDistAddr;               /* Need to send SPI device internal address */
    UINT8 * pTransBuf;                  /* Transmit buffer base address */
    INT32   transLen;                   /* Length of data to be transmit */
    UINT8 * pRcvBuf;                    /* Recieve buffer base address */
    INT32   rcvLen;                     /* Length of data to be recieved */
    }SPI_DEV_CTRL;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCsysApmSpih */
