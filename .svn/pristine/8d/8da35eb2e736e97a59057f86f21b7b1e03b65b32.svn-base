/* mpc8640dI2c.h - MPC8640D I2C Driver Header Module */


/* create by:YZF */

#ifndef	INCmpc8640dI2ch_1
#define	INCmpc8640dI2ch_1

#include <vxWorks.h>
#include <config.h>

#ifdef __cplusplus
extern "C" {
#endif
	


/* MPC8640D Register Addresses */
#define MPC8640D_I2C_ADR_REG		       						(0xe8003000)
#define MPC8640D_I2C_FREQ_DIV_REG								(0xe8003004)
#define MPC8640D_I2C_CONTROL_REG								(0xe8003008)
#define MPC8640D_I2C_STATUS_REG									(0xe800300c)
#define MPC8640D_I2C_DATA_REG									(0xe8003010)
#define MPC8640D_I2C_DIG_FILTER_REG								(0xe8003014)

/* MPC8640D Register masks */

#define MPC8640D_I2C_ADDRESS_REG_MASK 							0xFE
#define MPC8640D_I2C_FREQ_DIV_REG_MASK							0x3F
#define MPC8640D_I2C_CONTROL_REG_MASK							0xFD
#define MPC8640D_I2C_STATUS_REG_MASK							0xFF
#define MPC8640D_I2C_DATA_REG_MASK								0xFF
#define MPC8640D_I2C_DIG_FILTER_REG_MASK						0x3F


/* MPC8640D Control register values */

#define MPC8640D_I2C_CONTROL_REG_MEN	    					0x80	/* module enable */
#define MPC8640D_I2C_CONTROL_REG_MIEN							0x40  	/* module interrupt enable */
#define MPC8640D_I2C_CONTROL_REG_MSTA							0x20  	/* master/slave mode */
#define MPC8640D_I2C_CONTROL_REG_MTX	    					0x10  	/* transmit/receiver mode  */
#define MPC8640D_I2C_CONTROL_REG_TXAK							0x08  	/* transfer ack enable */
#define MPC8640D_I2C_CONTROL_REG_RSTA							0x04  	/* repeat start */
#define MPC8640D_I2C_CONTROL_REG_BCST							0x01  	/* accept broadcast messages */


/* MPC8640D Status register values */

#define MPC8640D_I2C_STATUS_REG_MCF      						0x80  	/* data transferring */
#define MPC8640D_I2C_STATUS_REG_MAAS	    					0x40  	/* addressed as a slave */
#define MPC8640D_I2C_STATUS_REG_MBB	    						0x20  	/* bus busy */
#define MPC8640D_I2C_STATUS_REG_MAL	    						0x10  	/* arbitration lost */
#define MPC8640D_I2C_STATUS_REG_BCSTM    						0x08  	/* broadcast match */
#define MPC8640D_I2C_STATUS_REG_SRW	    						0x04  	/* slave read/write */
#define MPC8640D_I2C_STATUS_REG_MIF	    						0x02  	/* module interrupt */
#define MPC8640D_I2C_STATUS_REG_RXAK	    					0x01  	/* receive ack */
	
#define REG8_MPC8640D_I2C1(x)		*(volatile UINT8*)(x)

IMPORT int 	i2cCycleMpc8640dDWrite (unsigned char);
IMPORT void i2cCycleMpc8640dDelay ();
IMPORT int 	i2cCycleMpc8640dStart ();
IMPORT int 	i2cCycleMpc8640dStop ();
IMPORT int 	i2cCycleMpc8640dRead (unsigned char *,int);
IMPORT int 	i2cCycleMpc8640dAckIn ();
IMPORT int 	i2cCycleMpc8640dAckOut ();
IMPORT int 	i2cCycleMpc8640dKnownState ();
IMPORT UINT8 i2cRegRead(UINT32 ,UINT8 );
IMPORT STATUS i2cRegWrite(UINT32, UINT8, UINT8 );
IMPORT UINT16 i2cRegReadWord(UINT32,UINT8);
IMPORT STATUS i2cRegWriteWord(UINT32, UINT8, UINT16);
IMPORT void sysMsDelay(UINT);

#ifdef __cplusplus
}
#endif


#endif /* INCmpc8640dI2ch */
