/* sysApmI2c.c - APM86290 I2C driver */

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
01a,12Jan12,syt  written.
*/

/*
DESCRIPTION
This is a driver for the APM86x90 I2C controller. It is intended for use by
client drivers performing other high level functions.
*/

/* includes */

#include <vxWorks.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <taskLib.h>
#include <../h/hwif/util/hwMemLib.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sysApmI2c.h"

/* IMPORT */

IMPORT void   sysUsDelay (int);
IMPORT UINT32 sysApbClkFreqGet (void);
IMPORT _Vx_ticks_t tickGet (void);

#define I2C_REG_WRITE(pDrv, addr, data) pDrv->pI2cRegWrite(pDrv->regBase + addr, data)
#define I2C_REG_READ(pDrv, addr)        pDrv->pI2cRegRead(pDrv->regBase + addr)

#ifdef _WRS_CONFIG_SMP
#   define I2C_SPIN_LOCK_INIT(pDrv)     SPIN_LOCK_TASK_INIT((void *)&(pDrv->spinlockTask), 0)
#   define I2C_SPIN_LOCK_TAKE(pDrv)     SPIN_LOCK_TASK_TAKE(&(pDrv->spinlockTask))
#   define I2C_SPIN_LOCK_GIVE(pDrv)     SPIN_LOCK_TASK_GIVE(&(pDrv->spinlockTask))
#else  /* _WRS_CONFIG_SMP */
#   define I2C_SPIN_LOCK_INIT(pDrv)
#   define I2C_SPIN_LOCK_TAKE(pDrv)
#   define I2C_SPIN_LOCK_GIVE(pDrv)
#endif  /* _WRS_CONFIG_SMP */

#define CHECK_STATUS(checkStatus, errHandle)                \
    tickSave = tickGet();                                   \
    while (checkStatus)                                     \
        {                                                   \
        if ((tickGet() - tickSave) > CHECK_STATUS_TIMEOUT ) \
            goto errHandle;                                 \
        }

/*******************************************************************************
*
* i2cBusEnable - enable the I2C controller
*
* This function enables the I2C master controller.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

STATUS i2cBusEnable
    (
    I2C_DRV_CTRL *  pDrvCtrl
    )
    {
    UINT32  timeout = I2C_ENABLE_TIMEOUT;

    if (I2C_REG_READ(pDrvCtrl, I2C_ENABLE) == 0)
        {
        I2C_REG_WRITE(pDrvCtrl, I2C_ENABLE, 1);

        while (!I2C_REG_READ(pDrvCtrl, I2C_ENABLE_STATUS) && (timeout > 0))
            {
            timeout--;
            sysUsDelay (10);
            };
        }

    if (timeout == 0)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* i2cBusDisable - Disable the I2C controller
*
* This function Disables the I2C master controller.
*
* RETURNS: OK or ERROR if I2C controller disable failed
*
* ERRNO: N/A
*/

STATUS i2cBusDisable
    (
    I2C_DRV_CTRL *  pDrvCtrl
    )
    {
    UINT32  timeout = I2C_DISABLE_TIMEOUT;

    I2C_REG_WRITE(pDrvCtrl, I2C_ENABLE, 0);

    while (I2C_REG_READ(pDrvCtrl, I2C_ENABLE_STATUS) && (timeout > 0))
        {
        timeout--;
        sysUsDelay (10);
        };

    if (timeout == 0)
        return ERROR;

    return OK;
    }

/*****************************************************************************
*
* i2cBusSpeedSet - configure the controller's clock registers
*
* This routine configures the clock divider registers in the I2C controller
* for a given I2C bus <speed>, based on the the APB clock speed.
*
* RETURNS: the actual selected bus speed
*
* ERRNO: N/A
*/

void i2cBusSpeedSet
    (
    I2C_DRV_CTRL *  pDrvCtrl,
    UINT32  speed
    )
    {
    UINT32 i2cClkKhz;
    UINT32 sclHcnt;
    UINT32 sclLcnt;

    /* For APM86290, the APB clock is set as I2C clock by default */

    i2cClkKhz = sysApbClkFreqGet () / 1000;

    /* Standard speed mode dividers for high/low periods */

    sclHcnt = SCL_HCNT_VAL(i2cClkKhz, SclCntParameter[I2C_SS].i2cSclHcnt);
    sclLcnt = SCL_LCNT_VAL(i2cClkKhz, SclCntParameter[I2C_SS].i2cSclLcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_SS_SCL_HCNT, sclHcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_SS_SCL_LCNT, sclLcnt);

    /* Fast speed mode dividers for high/low periods */

    sclHcnt = SCL_HCNT_VAL(i2cClkKhz, SclCntParameter[I2C_FS].i2cSclHcnt);
    sclLcnt = SCL_LCNT_VAL(i2cClkKhz, SclCntParameter[I2C_FS].i2cSclLcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_FS_SCL_HCNT, sclHcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_FS_SCL_LCNT, sclLcnt);

    /* High speed mode dividers for high/low periods */

    sclHcnt = SCL_HCNT_VAL(i2cClkKhz, SclCntParameter[I2C_HS_400PF].i2cSclHcnt);
    sclLcnt = SCL_LCNT_VAL(i2cClkKhz, SclCntParameter[I2C_HS_400PF].i2cSclLcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_HS_SCL_HCNT, sclHcnt);
    I2C_REG_WRITE(pDrvCtrl, I2C_HS_SCL_LCNT, sclLcnt);

    return;
    }

/*******************************************************************************
*
* sysI2cDevInit - APM I2C controller initialization
*
* The purpose of this function is to perform I2C conctoller initialization
*
* RETURNS: pDrvCtrl structure pointer if initialize success, others ERROR.
*
* ERRNO
*/

I2C_DRV_CTRL * sysI2cDevInit
    (
    UINT32  regBase,    /* I2C controller registers bank baseaddress */
    UINT32  busSpeed    /* I2C bus speed frequency */
    )
    {
    I2C_DRV_CTRL * pDrvCtrl;
    UINT32  regVal;

    if (!((busSpeed == I2C_FRQ_100KHZ) || (busSpeed == I2C_FRQ_400KHZ)))
        {
        printf("Error, Only 100KHz and 400KHz I2C bus speed are accepted\r\n");
        return  NULL;
        }

    pDrvCtrl = (I2C_DRV_CTRL *)malloc (sizeof (I2C_DRV_CTRL));

    if (pDrvCtrl == NULL)
        return  NULL;

    memset (pDrvCtrl, 0, sizeof (I2C_DRV_CTRL));

    pDrvCtrl->regBase = regBase;
    pDrvCtrl->i2cDevSem = semMCreate (SEM_Q_PRIORITY);
    
    if (pDrvCtrl->i2cDevSem == SEM_ID_NULL)
        {
        free (pDrvCtrl);
        return  NULL;    
        }

    pDrvCtrl->pI2cRegWrite = (void *)sysWrite32;
    pDrvCtrl->pI2cRegRead = (void *)sysRead32;

    I2C_SPIN_LOCK_INIT(pDrvCtrl);

    I2C_SPIN_LOCK_TAKE(pDrvCtrl);

    /*
     * Judge I2C controller register bank endian mode, I2C register bank0 is
     * big endian while I2C register bank1 is little endian
     */

    regVal = I2C_REG_READ(pDrvCtrl, I2C_COMP_TYPE);
    if ((SWAP32(regVal) & I2C_COMP_TYPE_MASK) == I2C_COMP_TYPE_MASK)
        {
        pDrvCtrl->pI2cRegWrite = (void *)sysWrite32Le;
        pDrvCtrl->pI2cRegRead = (void *)sysRead32Le;
        }

    /* Disable I2C bus */

    i2cBusDisable (pDrvCtrl);

    /* Set I2C controller FIFO depth */

    regVal = I2C_REG_READ(pDrvCtrl, I2C_COMP_PARAM_1);
    I2C_REG_WRITE(pDrvCtrl, I2C_TX_TL, I2C_TX_BUF_DEPTH(regVal));
    I2C_REG_WRITE(pDrvCtrl, I2C_RX_TL, I2C_RX_BUF_DEPTH(regVal));

    /* Set I2C bus speed */

    i2cBusSpeedSet (pDrvCtrl, busSpeed);

    /* Set I2C controller working mode */

    regVal = SLAVE_DIS | RESTART_EN | MASTER_EN;

    if ((busSpeed > 100000) && (busSpeed <= 400000))
        regVal |= FS_MODE;
    else
        regVal |= SS_MODE;

    I2C_REG_WRITE(pDrvCtrl, I2C_CON, regVal);

    I2C_SPIN_LOCK_GIVE(pDrvCtrl);

    return (pDrvCtrl);
    }

/*******************************************************************************
*
* sysI2cWritePoll - write data to I2C device with poll mode
*
* The purpose of this function is write data to I2C device with poll mode
* on poll mode.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysI2cWritePoll
    (
    I2C_DEV_CTRL * i2cDevCtlr
    )
    {
    I2C_DRV_CTRL *  pDrvCtrl = (I2C_DRV_CTRL *)i2cDevCtlr->pDrvCtrl;
    UINT32  i;
    UINT32  tickSave;

    semTake (pDrvCtrl->i2cDevSem, WAIT_FOREVER);
    I2C_SPIN_LOCK_TAKE(pDrvCtrl);

    CHECK_STATUS(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & I2C_ACTIVITY, \
                 sysI2cWritePoll_err);

    /* Disable I2C bus */

    if (i2cBusDisable (pDrvCtrl) == ERROR)
        {
        I2C_SPIN_LOCK_GIVE(pDrvCtrl);
        semGive (pDrvCtrl->i2cDevSem);

        return ERROR;
        }

    /* Fill I2C target device address */

    I2C_REG_WRITE(pDrvCtrl, I2C_TAR, i2cDevCtlr->devAddr & I2C_ADDR_MASK);

    /* Enable I2C bus */

    if (i2cBusEnable (pDrvCtrl) == ERROR)
        {
        I2C_SPIN_LOCK_GIVE(pDrvCtrl);
        semGive (pDrvCtrl->i2cDevSem);

        return ERROR;
        }

    for(i = 0; i < i2cDevCtlr->transLen; i++)
        {
        CHECK_STATUS(!(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & I2C_TFNF), \
                 sysI2cWritePoll_err);

        I2C_REG_WRITE(pDrvCtrl, I2C_DATA_CMD, *(i2cDevCtlr->pTransBuf + i));
        }

    /* Wait for I2C transmate completed */

    CHECK_STATUS(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & (I2C_FSM_ACTIVITY | I2C_ACTIVITY), \
                 sysI2cWritePoll_err);

    sysUsDelay(6);

    I2C_SPIN_LOCK_GIVE(pDrvCtrl);
    semGive (pDrvCtrl->i2cDevSem);

    return OK;

sysI2cWritePoll_err:

    I2C_SPIN_LOCK_GIVE(pDrvCtrl);
    semGive (pDrvCtrl->i2cDevSem);

    return ERROR;    
    }

/*******************************************************************************
*
* sysI2cReadPoll - read data from I2C device with poll mode
*
* The purpose of this function is to read data from I2C device with poll mode
* on poll mode.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysI2cReadPoll
    (
    I2C_DEV_CTRL * i2cDevCtlr
    )
    {
    I2C_DRV_CTRL *  pDrvCtrl = (I2C_DRV_CTRL *)i2cDevCtlr->pDrvCtrl;
    UINT32  i;
    UINT32  tickSave;

    semTake (pDrvCtrl->i2cDevSem, WAIT_FOREVER);
    I2C_SPIN_LOCK_TAKE(pDrvCtrl);

    CHECK_STATUS(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & I2C_ACTIVITY, \
                 sysI2cReadPoll_err);

    /* Disable I2C bus */

    if (i2cBusDisable (pDrvCtrl) == ERROR)
        {
        I2C_SPIN_LOCK_GIVE(pDrvCtrl);
        semGive (pDrvCtrl->i2cDevSem);

        return ERROR;
        }

    /* Fill I2C target device address */

    I2C_REG_WRITE(pDrvCtrl, I2C_TAR, i2cDevCtlr->devAddr & I2C_ADDR_MASK);

    /* Enable I2C bus */

    if (i2cBusEnable (pDrvCtrl) == ERROR)
        {
        I2C_SPIN_LOCK_GIVE(pDrvCtrl);
        semGive (pDrvCtrl->i2cDevSem);

        return ERROR;
        }

    /* Send out device address and the dist word address */

    for(i = 0; i < i2cDevCtlr->transLen; i++)
        {
        CHECK_STATUS(!(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & I2C_TFNF), \
                 sysI2cReadPoll_err);

        I2C_REG_WRITE(pDrvCtrl, I2C_DATA_CMD, *(i2cDevCtlr->pTransBuf + i));
        }

    /* Wait for I2C transmate completed */

    CHECK_STATUS(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & (I2C_FSM_ACTIVITY | I2C_ACTIVITY), \
                 sysI2cReadPoll_err);

    sysUsDelay(6);

    for (i = 0; i < i2cDevCtlr->rcvLen; i++)
        {
        /* I2C random read operation */

        I2C_REG_WRITE(pDrvCtrl, I2C_DATA_CMD, I2C_RD);

        CHECK_STATUS(!(I2C_REG_READ(pDrvCtrl, I2C_STATUS) & I2C_RFNE), \
                 sysI2cReadPoll_err);

        *(i2cDevCtlr->pRcvBuf + i) = (UINT8)I2C_REG_READ(pDrvCtrl, I2C_DATA_CMD);
        }

    I2C_SPIN_LOCK_GIVE(pDrvCtrl);
    semGive (pDrvCtrl->i2cDevSem);

    return OK;

sysI2cReadPoll_err:
    
    I2C_SPIN_LOCK_GIVE(pDrvCtrl);
    semGive (pDrvCtrl->i2cDevSem);

    return ERROR;
    }

