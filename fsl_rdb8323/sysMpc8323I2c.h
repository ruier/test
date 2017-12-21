/* sysMpc8323I2c.h - Mpc8323 I2C Driver Header Module */

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
Mpc8323 Memory Controller (MPC8323 - PowerPlus Architecture)

Notes:
1. The low level routines were modeled after the original
driver written by Pamela Wolfe.
*/

#ifndef	INCsysMpc8323I2ch
#define	INCsysMpc8323I2ch

#include "vxWorks.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Mpc8323 Register Addresses */
#define MPC8323_I2C_ADR_REG			(0x00000)
#define MPC8323_I2C_FREQ_DIV_REG		(0x00004)
#define MPC8323_I2C_CONTROL_REG			(0x00008)
#define MPC8323_I2C_STATUS_REG			(0x0000c)
#define MPC8323_I2C_DATA_REG			(0x00010)
#define MPC8323_I2C_DIG_FILTER_REG		(0x00014)

/* Mpc8323 Register masks */

#define MPC8323_I2C_ADDRESS_REG_MASK 		0xFE
#define MPC8323_I2C_FREQ_DIV_REG_MASK		0x3F
#define MPC8323_I2C_CONTROL_REG_MASK		0xFD
#define MPC8323_I2C_STATUS_REG_MASK		0xFF
#define MPC8323_I2C_DATA_REG_MASK		0xFF
#define MPC8323_I2C_DIG_FILTER_REG_MASK		0x3F


/* Mpc8323 Control register values */

#define MPC8323_I2C_CONTROL_REG_MEN     0x80	/* module enable */
#define MPC8323_I2C_CONTROL_REG_MIEN	0x40  	/* module interrupt enable */
#define MPC8323_I2C_CONTROL_REG_MSTA	0x20  	/* master/slave mode */
#define MPC8323_I2C_CONTROL_REG_MTX     0x10  	/* transmit/receiver mode  */
#define MPC8323_I2C_CONTROL_REG_TXAK	0x08  	/* transfer ack enable */
#define MPC8323_I2C_CONTROL_REG_RSTA	0x04  	/* repeat start */
#define MPC8323_I2C_CONTROL_REG_BCST	0x01  	/* accept broadcast messages */


/* Mpc8323 Status register values */

#define MPC8323_I2C_STATUS_REG_MCF      0x80  	/* data transferring */
#define MPC8323_I2C_STATUS_REG_MAAS	0x40/* addressed as a slave */
#define MPC8323_I2C_STATUS_REG_MBB      0x20  	/* bus busy */
#define MPC8323_I2C_STATUS_REG_MAL      0x10  	/* arbitration lost */
#define MPC8323_I2C_STATUS_REG_BCSTM    0x08  	/* broadcast match */
#define MPC8323_I2C_STATUS_REG_SRW      0x04  	/* slave read/write */
#define MPC8323_I2C_STATUS_REG_MIF      0x02  	/* module interrupt */
#define MPC8323_I2C_STATUS_REG_RXAK     0x01  	/* receive ack */


IMPORT int i2cCycleMpc8323Write (unsigned char);
IMPORT void i2cCycleMpc8323Delay (int);
IMPORT int i2cCycleMpc8323Start (void);
IMPORT int i2cCycleMpc8323Stop (void);
IMPORT int i2cCycleMpc8323Read (unsigned char *,int);
IMPORT int i2cCycleMpc8323AckIn (void);
IMPORT int i2cCycleMpc8323AckOut (void);
IMPORT int i2cCycleMpc8323KnownState (void);


#ifdef __cplusplus
}
#endif


#endif /* INCsysMpc8323I2ch */
