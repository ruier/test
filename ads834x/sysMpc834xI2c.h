/* sysMpc834xI2c.h - Mpc834x I2C Driver Header Module */

/* Copyright 1998-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01b,16aug05,dtr  Fix indentation.
01a,21feb05,gtf  created.
*/

/*
DESCRIPTION

I2C Driver Header (Low Level Routines) Module
Mpc834x Memory Controller (MPC834X - PowerPlus Architecture)

Notes:
1. The low level routines were modeled after the original
driver written by Pamela Wolfe.
*/

#ifndef	INCsysMpc834xI2ch
#define	INCsysMpc834xI2ch

#include "vxWorks.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Mpc834x Register Addresses */
#define MPC834X_I2C_ADR_REG				(0x00000)
#define MPC834X_I2C_FREQ_DIV_REG		(0x00004)
#define MPC834X_I2C_CONTROL_REG			(0x00008)
#define MPC834X_I2C_STATUS_REG			(0x0000c)
#define MPC834X_I2C_DATA_REG			(0x00010)
#define MPC834X_I2C_DIG_FILTER_REG		(0x00014)

/* Mpc834x Register masks */

#define MPC834X_I2C_ADDRESS_REG_MASK 	0xFE
#define MPC834X_I2C_FREQ_DIV_REG_MASK	0x3F
#define MPC834X_I2C_CONTROL_REG_MASK	0xFD
#define MPC834X_I2C_STATUS_REG_MASK		0xFF
#define MPC834X_I2C_DATA_REG_MASK		0xFF
#define MPC834X_I2C_DIG_FILTER_REG_MASK	0x3F


/* Mpc834x Control register values */

#define MPC834X_I2C_CONTROL_REG_MEN     0x80	/* module enable */
#define MPC834X_I2C_CONTROL_REG_MIEN	0x40  	/* module interrupt enable */
#define MPC834X_I2C_CONTROL_REG_MSTA	0x20  	/* master/slave mode */
#define MPC834X_I2C_CONTROL_REG_MTX     0x10  	/* transmit/receiver mode  */
#define MPC834X_I2C_CONTROL_REG_TXAK	0x08  	/* transfer ack enable */
#define MPC834X_I2C_CONTROL_REG_RSTA	0x04  	/* repeat start */
#define MPC834X_I2C_CONTROL_REG_BCST	0x01  	/* accept broadcast messages */


/* Mpc834x Status register values */

#define MPC834X_I2C_STATUS_REG_MCF      0x80  	/* data transferring */
#define MPC834X_I2C_STATUS_REG_MAAS	0x40/* addressed as a slave */
#define MPC834X_I2C_STATUS_REG_MBB      0x20  	/* bus busy */
#define MPC834X_I2C_STATUS_REG_MAL      0x10  	/* arbitration lost */
#define MPC834X_I2C_STATUS_REG_BCSTM    0x08  	/* broadcast match */
#define MPC834X_I2C_STATUS_REG_SRW      0x04  	/* slave read/write */
#define MPC834X_I2C_STATUS_REG_MIF      0x02  	/* module interrupt */
#define MPC834X_I2C_STATUS_REG_RXAK     0x01  	/* receive ack */


IMPORT int i2cCycleMpc834xWrite (int, unsigned char);
IMPORT void i2cCycleMpc834xDelay (int);
IMPORT int i2cCycleMpc834xStart (int);
IMPORT int i2cCycleMpc834xStop (int);
IMPORT int i2cCycleMpc834xRead (int,unsigned char *,int);
IMPORT int i2cCycleMpc834xAckIn (int);
IMPORT int i2cCycleMpc834xAckOut (int);
IMPORT int i2cCycleMpc834xKnownState (int);


#ifdef __cplusplus
}
#endif


#endif /* INCsysMpc834xI2ch */
