/* sysMpc8313I2c.h - Mpc8313 I2C Driver Header Module */

/*
 * Copyright (c) 2005, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,28mar08,z_l modified for code convention. fixed "make man" error.
01c,26jun07,l_g modified for fsl_rdb8313 based on ads834x.
01b,16aug05,dtr Fix indentation.
01a,21feb05,gtf created.
*/

/*
DESCRIPTION

I2C Driver Header (Low Level Routines) Module
Mpc8313 Memory Controller (MPC8313 - PowerPlus Architecture)

Notes:
1. The low level routines were modeled after the original
driver written by Pamela Wolfe.
*/

#ifndef __INCsysMpc8313I2ch
#define __INCsysMpc8313I2ch

#include <vxWorks.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Mpc8313 Register Addresses */

#define MPC8313_I2C_ADR_REG             (0x00000)
#define MPC8313_I2C_FREQ_DIV_REG        (0x00004)
#define MPC8313_I2C_CONTROL_REG         (0x00008)
#define MPC8313_I2C_STATUS_REG          (0x0000c)
#define MPC8313_I2C_DATA_REG            (0x00010)
#define MPC8313_I2C_DIG_FILTER_REG      (0x00014)

/* Mpc8313 Register masks */

#define MPC8313_I2C_ADDRESS_REG_MASK     0xFE
#define MPC8313_I2C_FREQ_DIV_REG_MASK    0x3F
#define MPC8313_I2C_CONTROL_REG_MASK     0xFD
#define MPC8313_I2C_STATUS_REG_MASK      0xFF
#define MPC8313_I2C_DATA_REG_MASK        0xFF
#define MPC8313_I2C_DIG_FILTER_REG_MASK  0x3F

/* Mpc8313 Control register values */

#define MPC8313_I2C_CONTROL_REG_MEN     0x80    /* module enable */
#define MPC8313_I2C_CONTROL_REG_MIEN    0x40    /* module interrupt enable */
#define MPC8313_I2C_CONTROL_REG_MSTA    0x20    /* master/slave mode */
#define MPC8313_I2C_CONTROL_REG_MTX     0x10    /* transmit/receiver mode  */
#define MPC8313_I2C_CONTROL_REG_TXAK    0x08    /* transfer ack enable */
#define MPC8313_I2C_CONTROL_REG_RSTA    0x04    /* repeat start */
#define MPC8313_I2C_CONTROL_REG_BCST    0x01    /* accept broadcast messages */

/* Mpc8313 Status register values */

#define MPC8313_I2C_STATUS_REG_MCF      0x80    /* data transferring */
#define MPC8313_I2C_STATUS_REG_MAAS     0x40    /* addressed as a slave */
#define MPC8313_I2C_STATUS_REG_MBB      0x20    /* bus busy */
#define MPC8313_I2C_STATUS_REG_MAL      0x10    /* arbitration lost */
#define MPC8313_I2C_STATUS_REG_BCSTM    0x08    /* broadcast match */
#define MPC8313_I2C_STATUS_REG_SRW      0x04    /* slave read/write */
#define MPC8313_I2C_STATUS_REG_MIF      0x02    /* module interrupt */
#define MPC8313_I2C_STATUS_REG_RXAK     0x01    /* receive ack */

IMPORT int i2cCycleMpc8313Write (int, unsigned char);
IMPORT void i2cCycleMpc8313Delay (int);
IMPORT int i2cCycleMpc8313Start (int);
IMPORT int i2cCycleMpc8313Stop (int);
IMPORT int i2cCycleMpc8313Read (int,unsigned char *,int);
IMPORT int i2cCycleMpc8313AckIn (int);
IMPORT int i2cCycleMpc8313AckOut (int);
IMPORT int i2cCycleMpc8313KnownState (int);

#ifdef __cplusplus
}
#endif

#endif /* __INCsysMpc8313I2ch */
