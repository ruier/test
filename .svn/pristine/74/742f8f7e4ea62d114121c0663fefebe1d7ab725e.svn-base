/* sysMpc512xI2c.h - MPC512x I2C Driver Header Module */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,05nov07,b_m  derived from version 01b of ads834x/sysMpc834xI2c.h.
*/

#ifndef INCsysMpc512xI2ch
#define INCsysMpc512xI2ch

#ifdef __cplusplus
extern "C" {
#endif

/* I2C Register Address */

#define MPC512X_I2C_ADR_REG             0x00
#define MPC512X_I2C_FREQ_DIV_REG        0x04
#define MPC512X_I2C_CONTROL_REG         0x08
#define MPC512X_I2C_STATUS_REG          0x0c
#define MPC512X_I2C_DATA_REG            0x10

/* I2C Register Mask */

#define MPC512X_I2C_ADDRESS_REG_MASK    0xFE
#define MPC512X_I2C_FREQ_DIV_REG_MASK   0x3F
#define MPC512X_I2C_CONTROL_REG_MASK    0xFD
#define MPC512X_I2C_STATUS_REG_MASK     0xFF
#define MPC512X_I2C_DATA_REG_MASK       0xFF
#define MPC512X_I2C_DIG_FILTER_REG_MASK 0x3F

/* Frequency Divider */

#define MPC512X_I2C_FREQ_DIVIDER        0x3B

/* Control Register Values */

#define MPC512X_I2C_CONTROL_REG_MEN     0x80    /* module enable */
#define MPC512X_I2C_CONTROL_REG_MIEN    0x40    /* module interrupt enable */
#define MPC512X_I2C_CONTROL_REG_MSTA    0x20    /* master/slave mode */
#define MPC512X_I2C_CONTROL_REG_MTX     0x10    /* transmit/receiver mode */
#define MPC512X_I2C_CONTROL_REG_TXAK    0x08    /* transfer ack enable */
#define MPC512X_I2C_CONTROL_REG_RSTA    0x04    /* repeat start */
#define MPC512X_I2C_CONTROL_REG_BCST    0x01    /* accept broadcast messages */

/* Status Register Values */

#define MPC512X_I2C_STATUS_REG_MCF      0x80    /* data transferring */
#define MPC512X_I2C_STATUS_REG_MAAS     0x40    /* addressed as a slave */
#define MPC512X_I2C_STATUS_REG_MBB      0x20    /* bus busy */
#define MPC512X_I2C_STATUS_REG_MAL      0x10    /* arbitration lost */
#define MPC512X_I2C_STATUS_REG_BCSTM    0x08    /* broadcast match */
#define MPC512X_I2C_STATUS_REG_SRW      0x04    /* slave read/write */
#define MPC512X_I2C_STATUS_REG_MIF      0x02    /* module interrupt */
#define MPC512X_I2C_STATUS_REG_RXAK     0x01    /* receive ack */

/* MPC512x I2C Controller Numbers */

#define MPC512X_I2C_NUMS                3

/* forward declarations */

IMPORT int  i2cCycleMpc512xStart(int);
IMPORT int  i2cCycleMpc512xStop(int);
IMPORT int  i2cCycleMpc512xRead(int, unsigned char *, int);
IMPORT int  i2cCycleMpc512xWrite(int, unsigned char);
IMPORT int  i2cCycleMpc512xAckIn(int);
IMPORT int  i2cCycleMpc512xAckOut(int);
IMPORT int  i2cCycleMpc512xKnownState(int);
IMPORT void i2cCycleMpc512xDelay(int);

#ifdef __cplusplus
}
#endif

#endif
