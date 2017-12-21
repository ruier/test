/* sysMpc837xI2c.h - Mpc837x I2C Driver Header Module */
/*
 * Copyright (c) 2005, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
/* Copyright 1998-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01c,20oct07,x_s  modified for mds837x.
01b,16aug05,dtr  Fix indentation.
01a,21feb05,gtf  created.
*/

/*
DESCRIPTION

I2C Driver Header (Low Level Routines) Module
Mpc834x Memory Controller (MPC837X - PowerPlus Architecture)

Notes:
1. The low level routines were modeled after the original
driver written by Pamela Wolfe.
*/

#ifndef	INCsysMpc837xI2ch
#define	INCsysMpc837xI2ch

#include <vxWorks.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Mpc837X Register Addresses */
#define MPC837X_I2C_ADR_REG				(0x00000)
#define MPC837X_I2C_FREQ_DIV_REG		(0x00004)
#define MPC837X_I2C_CONTROL_REG			(0x00008)
#define MPC837X_I2C_STATUS_REG			(0x0000c)
#define MPC837X_I2C_DATA_REG			(0x00010)
#define MPC837X_I2C_DIG_FILTER_REG		(0x00014)

/* Mpc837X Register masks */

#define MPC837X_I2C_ADDRESS_REG_MASK 	0xFE
#define MPC837X_I2C_FREQ_DIV_REG_MASK	0x3F
#define MPC837X_I2C_CONTROL_REG_MASK	0xFD
#define MPC837X_I2C_STATUS_REG_MASK		0xFF
#define MPC837X_I2C_DATA_REG_MASK		0xFF
#define MPC837X_I2C_DIG_FILTER_REG_MASK	0x3F


/* Mpc837X Control register values */

#define MPC837X_I2C_CONTROL_REG_MEN     0x80	/* module enable */
#define MPC837X_I2C_CONTROL_REG_MIEN	0x40  	/* module interrupt enable */
#define MPC837X_I2C_CONTROL_REG_MSTA	0x20  	/* master/slave mode */
#define MPC837X_I2C_CONTROL_REG_MTX     0x10  	/* transmit/receiver mode  */
#define MPC837X_I2C_CONTROL_REG_TXAK	0x08  	/* transfer ack enable */
#define MPC837X_I2C_CONTROL_REG_RSTA	0x04  	/* repeat start */
#define MPC837X_I2C_CONTROL_REG_BCST	0x01  	/* accept broadcast messages */


/* Mpc837X Status register values */

#define MPC837X_I2C_STATUS_REG_MCF      0x80  	/* data transferring */
#define MPC837X_I2C_STATUS_REG_MAAS	0x40/* addressed as a slave */
#define MPC837X_I2C_STATUS_REG_MBB      0x20  	/* bus busy */
#define MPC837X_I2C_STATUS_REG_MAL      0x10  	/* arbitration lost */
#define MPC837X_I2C_STATUS_REG_BCSTM    0x08  	/* broadcast match */
#define MPC837X_I2C_STATUS_REG_SRW      0x04  	/* slave read/write */
#define MPC837X_I2C_STATUS_REG_MIF      0x02  	/* module interrupt */
#define MPC837X_I2C_STATUS_REG_RXAK     0x01  	/* receive ack */


IMPORT int i2cCycleMpc837xWrite (int, unsigned char);
IMPORT void i2cCycleMpc837xDelay (int);
IMPORT int i2cCycleMpc837xStart (int);
IMPORT int i2cCycleMpc837xStop (int);
IMPORT int i2cCycleMpc837xRead (int,unsigned char *,int);
IMPORT int i2cCycleMpc837xAckIn (int);
IMPORT int i2cCycleMpc837xAckOut (int);
IMPORT int i2cCycleMpc837xKnownState (int);


#ifdef __cplusplus
}
#endif


#endif /* INCsysMpc837xI2ch */
