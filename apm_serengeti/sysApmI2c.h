/* sysApmi2c.h - APM86290 I2C driver header file */

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
01b,01mar12,syt  fixed compile warnings.
01a,04jan12,syt  written
*/

#ifndef __INCsysApmI2ch
#define __INCsysApmI2ch

#ifdef __cplusplus
extern "C" {
#endif

#include <semLib.h>
#include "config.h"

/* I2C registers, fields and masks */

/* I2C register offsets */

#define I2C_CON                     0x00
#define I2C_TAR                     0x04
#define I2C_SAR                     0x08
#define I2C_HS_MADDR                0x0c
#define I2C_DATA_CMD                0x10
#define I2C_SS_SCL_HCNT             0x14
#define I2C_SS_SCL_LCNT             0x18
#define I2C_FS_SCL_HCNT             0x1c
#define I2C_FS_SCL_LCNT             0x20
#define I2C_HS_SCL_HCNT             0x24
#define I2C_HS_SCL_LCNT             0x28
#define I2C_INTR_STAT               0x2c
#define I2C_INTR_MASK               0x30
#define I2C_RAW_INTR_STAT           0x34
#define I2C_RX_TL                   0x38
#define I2C_TX_TL                   0x3c
#define I2C_CLR_INTR                0x40
#define I2C_CLR_RX_UNDER            0x44
#define I2C_CLR_RX_OVER             0x48
#define I2C_CLR_TX_OVER             0x4c
#define I2C_CLR_RD_REQ              0x50
#define I2C_CLR_TX_ABRT             0x54
#define I2C_CLR_RX_DONE             0x58
#define I2C_CLR_ACTIVITY            0x5c
#define I2C_CLR_STOP_DET            0x60
#define I2C_CLR_START_DET           0x64
#define I2C_CLR_GEN_CALL            0x68
#define I2C_ENABLE                  0x6c
#define I2C_STATUS                  0x70
#define I2C_TXFLR                   0x74
#define I2C_RXFLR                   0x78
#define I2C_TX_ABRT_SOURCE          0x80
#define I2C_SLV_DATA_NACK_ONLY      0x84
#define I2C_SDA_SETUP               0x94
#define I2C_ACK_GENERAL_CALL        0x98
#define I2C_ENABLE_STATUS           0x9c
#define I2C_COMP_PARAM_1            0xf4
#define I2C_COMP_VERSION            0xf8
#define I2C_COMP_TYPE               0xfc

/* I2C controller registers bit fields */

#define SLAVE_DIS                   0x40
#define RESTART_EN                  0x20
#define SS_MODE                     (1 << 1)
#define FS_MODE                     (2 << 1)
#define HS_MODE                     (3 << 1)
#define MASTER_EN                   0x01

/* I2C_TAR register bit fields */

#define I2C_ADDR_MASK               0xff

/* I2C_DATA_CMD register bit fields */

#define I2C_RD                      0x100   /* I2C read operation */
#define I2C_WR                      0x000   /* I2C write operation */

/* I2C_STATUS register bit fields */

#define I2C_FSM_ACTIVITY            0x20
#define I2C_RFNE                    0x08
#define I2C_TFE                     0x04
#define I2C_TFNF                    0x02
#define I2C_ACTIVITY                0x01

/* I2C_COMP_PARAM_1 register bit fields */

#define I2C_RX_BUF_DEPTH(x)         ((x >> 8) & 0xff)
#define I2C_TX_BUF_DEPTH(x)         ((x >> 16) & 0xff)

/* I2C operation timeout value */

#define I2C_ENABLE_TIMEOUT          100
#define I2C_DISABLE_TIMEOUT         100
#define I2C_READ_TIMEOUT            1000

/* I2C_COMP_TYPE bit fields */

#define I2C_COMP_TYPE_MASK          0x44570000

#define SWAP32(x)                   ((((x) & 0x000000ff) << 24) |  \
                                     (((x) & 0x0000ff00) <<  8) |  \
                                     (((x) & 0x00ff0000) >>  8) |  \
                                     (((unsigned int)(x)) >> 24))

/* I2C bus clock frequency */

#define I2C_FRQ_400KHZ              (400 * 1000)
#define I2C_FRQ_100KHZ              (100 * 1000)

/* I2C bus status check timeout value */

#define CHECK_STATUS_TIMEOUT        2

/* I2C device transmit and recieve buffer length */

#define APM_I2C_BUF_SIZE            256

typedef struct {
    UINT32  i2cSclHcnt;
    UINT32  i2cSclLcnt;
    } I2C_SCL_CNT_PARA;

#define I2C_SS                      0
#define I2C_FS                      1
#define I2C_HS_400PF                2
#define I2C_HS_100PF                3

I2C_SCL_CNT_PARA    SclCntParameter[] = {
    { 4300,  5000 },        /* Standard Speed mode (100 Kbit/s) */
    {  900,  1600 },        /* Fast Speed mode (100 Kbit/s) */
    {  460,   620 },        /* High Speed mode (3.4 Mbits) 400pf */
    {  360,   420 }         /* High Speed mode (3.4 Mbits) 100pf */
};

#define SCL_HCNT_VAL(i2cClk, sclHcnt)   \
                        (((i2cClk * sclHcnt + 500000) / 1000000) - 3)
#define SCL_LCNT_VAL(i2cClk, sclLcnt)   \
                        (((i2cClk * sclLcnt + 500000) / 1000000) - 1)

typedef UINT32  (* pI2C_REG_READ_FN) (UINT32  addr);
typedef void    (* pI2C_REG_WRITE_FN) (UINT32 addr, UINT32 data);

typedef struct i2cDrvCtrl
    {
    UINT32              regBase;
    UINT32              intVect;
    BOOL                polling;
    SEM_ID              i2cDevSem;
    spinlockTask_t      spinlockTask;   /* Task-callable spinlock */
    pI2C_REG_READ_FN    pI2cRegRead;
    pI2C_REG_WRITE_FN   pI2cRegWrite;
    } I2C_DRV_CTRL;

typedef struct i2cDevCtrl
    {
    I2C_DRV_CTRL *      pDrvCtrl;
    UINT8               devAddr;        /* I2C device address */
    UINT8               wordAddrLen;    /* I2C device wordaddress length */
    UINT32              pageSize;       /* I2C EEprom device size*/
    UINT32              devSize;        /* I2C EEprom device page size*/
    UINT32              progTime;       /* I2C EEprom device program time (ms) */
    UINT16              distAddr;       /* I2C device internal address */
    UINT8 *             pTransBuf;      /* Transmit buffer address */
    INT32               transLen;       /* Length of data to be transmit */
    UINT8 *             pRcvBuf;        /* Recieve buffer address */
    INT32               rcvLen;         /* Length of data to be recieved */
    } I2C_DEV_CTRL;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* INCsysApmI2ch */
