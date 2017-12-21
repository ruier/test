/* mpc8640DI2CMaster.h - MPC8640D on chip I2C controller */

/* Copyright 2000-2002 Wind River Systems, Inc. */
/* Copyright 2000-2002 Wind River Services */


/*
modification history
--------------------
01a,13oct03,e_c	 Written
*/


#ifndef __INCmpc8640DI2CMasterh
#define __INCmpc8640DI2CMasterh
#ifdef __cplusplus
extern "C" {
#endif	 /* __cplusplus */

#define MPC8640DI2C_DEV_NAME		"mpc8640Di2c"
#define MPC8640DI2C_DEV_NAME_LEN	8


/* register offsets from the base */
#define MPC8640D_I2C_ADR_REG_OFFSET		   	0x00
#define MPC8640D_I2C_FREQDIV_REG_OFFSET		0x04
#define MPC8640D_I2C_CTRL_REG_OFFSET		   	0x08
#define MPC8640D_I2C_STAT_REG_OFFSET		   	0x0c
#define MPC8640D_I2C_DATA_REG_OFFSET		   	0x10
#define MPC8640D_I2C_INTCTL_OFFSET		   		0x20

/* register defines */
/* adress register */
#define MPC8640D_I2C_ADR_REG_MASK				0xfe000000

/* frequency divider register */
#define MPC8640D_I2C_FREQDIV_REG_MASK			0x3f000000

/* I2C control register */
#define MPC8640D_I2C_CTL_REG_MASK				0xfc000000
#define MPC8640D_I2C_CTL_REG_EN				0x80000000					
#define MPC8640D_I2C_CTL_REG_IEN				0x40000000
#define MPC8640D_I2C_CTL_REG_STA				0x20000000
#define MPC8640D_I2C_CTL_REG_TX				0x10000000
#define MPC8640D_I2C_CTL_REG_TXAK				0x08000000
#define MPC8640D_I2C_CTL_REG_RSTA				0x04000000

/* I2C status register */
#define MPC8640D_I2C_STAT_REG_MASK		   		0xf7000000
#define MPC8640D_I2C_STAT_REG_CF				0x80000000					
#define MPC8640D_I2C_STAT_REG_AAS				0x40000000
#define MPC8640D_I2C_STAT_REG_BB				0x20000000
#define MPC8640D_I2C_STAT_REG_AL				0x10000000
#define MPC8640D_I2C_STAT_REG_SRW				0x04000000
#define MPC8640D_I2C_STAT_REG_IF				0x02000000
#define MPC8640D_I2C_STAT_REG_RXAK				0x01000000


/* Data I/O register */
#define MPC8640D_I2C_DATA_REG_MASK		   		0xff000000

/* I2C interrupt control register */
#define MPC8640D_I2C_INTCTL_MASK		   		0xff000000


#define MPC8640D_TO_I2C_DATA(data)		(((UINT32)data << 24) & 0xff000000)
#define I2C_TO_MPC8640D_DATA(data)		(UINT8)(data >> 24)



typedef struct mpc8640DI2CDrvCtrl
{
 I2C_MASTER_OBJ	i2cMasterObj;
 int			unit;
 UINT32			regBase;
} MPC8640D_I2C_DRV_CTRL;

#if defined(__STDC__) || defined(__cplusplus)
    IMPORT void * mpc8640DI2CDrvLoad		(char * initString);
#else
    IMPORT void * mpc8640DI2CDrvLoad		();
#endif  /* __STDC__ */



#ifdef __cpluscplus
}
#endif	/* __cplusplus */
#endif	/* __INCmpc8640DI2CMasterh */

