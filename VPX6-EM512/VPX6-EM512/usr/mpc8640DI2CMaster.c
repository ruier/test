/* mpc8640DI2CMaster.c - MGT5200 I2C generic bus driver */

/* Copyright 2003 Wind River Systems, Inc. */
/* Copyright 2003 Wind River Services */
#include "copyright_wrs.h"

/*
modification history
--------------------
01e,19feb04,p_h  introduce the variable i2cFreqSetting for i2c frequency
01c,05nov03,e_c	 Add comment for documentation
01b,24oct03,e_c	 Rewrite read cycle
01a,13oct03,e_c	 Written
*/


/*
DESCRIPTION

I2C bus driver for onchip MGT5200 i2c controller.

This master driver can be used for both onchip I2C controllers on the MGT5200 cpu.

This driver presents the same interface as the END drivers used by the Mux layer and 
is initialized by using a load fonction by using the i2cMasterDevLoad function 
of the i2cLib library. This library is provided in the cp2 BSP directory.

The I2C master device load string used to initialize routine of the driver
has the following format.
"<unit>:<regBase>"
<unit>
Device Unit number 
<regBase>
Device register base memory address offset from the MBAR.

Slave device drivers will use this master device drivers when specifying 
"mpc8640Di2c:<unitNumber>" in their load string. 
After that slave device drivers can access the I2C bus with 
the help of i2cRead/i2cWrite functionof the i2cLib.
Also the master will never be used directly by an application but only by slave drivers
through the i2cLib : 

Application
-------------------------------------------
I2C Slave driver
-------------------------------------------
I2C Lib
-------------------------------------------
I2C Master driver
-------------------------------------------
I2C MG5200 Controller

An application will use directly slave device drivers APIs

INCLUDE FILES: mpc8640DI2CMaster.h i2cLib.h

SEE ALSO:
.pG "i2cLib"
*/

/* includes */

#include "vxWorks.h"
#include "string.h"
#include "stdlib.h"

#include "drv/i2c/i2cLib.h"
#include "mpc8640DI2CMaster.h"
#include "logLib.h"

#ifndef EIEIO
/*IMPORT void sysEieio (void);
#define EIEIO sysEieio ()*/
#   define EIEIO    _WRS_ASM (" eieio")
#endif

/* externals */

/* globals */

unsigned int i2cFreqSetting 	=
				/*	0x1a000000; */	/* 85,9khz */
				/*	0x19000000; */	/* 103khz */
				/*	0x15000000; */ /* 206khz */
				0x18000000;    /* 343khz */

#define DRV_DEBUG_I2C

/* Driver debug control */
#ifdef DRV_DEBUG_I2C
#define DRV_DEBUG_OFF		0x0000
#define DRV_DEBUG_RX		0x0001
#define DRV_DEBUG_TX		0x0002
#define DRV_DEBUG_ALL    	0xffff
#define DRV_DEBUG_LOAD		0x0004

int	mpc8640DI2CDebug = 0;

#define DRV_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)			\
    do {								\
    if (mpc8640DI2CDebug & FLG)						\
	if (logMsg != NULL)					\
	    logMsg (X0, (int)X1, (int)X2, (int)X3, (int)X4,	\
			    (int)X5, (int)X6);				\
    } while (0)

#define DRV_PRINT(FLG, X)						\
    do {								\
    if (mpc8640DI2CDebug & FLG)						\
	printf X;							\
    } while (0)

#else /* DRV_DEBUG_I2C */

#define DRV_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)
#define DRV_PRINT(FLG, X)

#endif /* DRV_DEBUG_I2C */



/* forwards */
STATUS mpc8640DI2CRead
( 
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit, 			/* Seven bit address */
	UINT8 * data,     			/* ptr to data buffer */
	UINT32  nbytes,   			/* number of bytes to read */
	UINT32  options,
	int     timeout             /* time out in ticks */
);

/* forwards */
STATUS mpc8640DI2CRead
( 
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit, 			/* Seven bit address */
	UINT8 * data,     			/* ptr to data buffer */
	UINT32  nbytes,   			/* number of bytes to read */
	UINT32  options,
	int     timeout             /* time out in ticks */
);

STATUS mpc8640DI2CWrite
(
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit,			/* Seven bit address */
	UINT8 * data,				/* ptr to data buffer */
	UINT32  nbytes,				/* number of bytes to write */
	UINT32  options,
	int 	timeout             /* timeout in ticks */
);

LOCAL STATUS mpc8640DI2CRx
( 
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8 * data,     			/* ptr to data buffer */
	int     stop,				/* time out in ticks */
	int		nack,     			/* ptr to data buffer */
	int     timeout             /* time out in ticks */
);

LOCAL STATUS mpc8640DI2CTx
(
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8	data,				/* ptr to data buffer */
	int		timeout             /* timeout in ticks */
);


STATUS mpc8640DI2CInitParse
    (
    /* pointer to X220_I2C_DRV_CTRL structure */
    MPC8640D_I2C_DRV_CTRL *		pDrvCtrl,      		
    char *	                initString	/* parameter string */
    );

STATUS mpc8640DI2CReset
	(
    MPC8640D_I2C_DRV_CTRL * pDrvCtrl /* ptr to driver structure */
	);

void * mpc8640DI2CDrvLoad(char *);
/* locals */


#define MPC8640D_I2C_WRITE32(reg_offset, val) \
(* (volatile UINT32 *) (pDrvCtrl->regBase + reg_offset) = (UINT32) val)

#define MPC8640D_I2C_READ32(reg_offset) \
(* (volatile UINT32 *) (pDrvCtrl->regBase + reg_offset))

#define MPC8640D_I2C_SET32(reg_offset, val) \
(MPC8640D_I2C_WRITE32(reg_offset, (MPC8640D_I2C_READ32(reg_offset) | (UINT32) val)))

#define MPC8640D_I2C_CLEAR32(reg_offset, val) \
(MPC8640D_I2C_WRITE32(reg_offset, (MPC8640D_I2C_READ32(reg_offset) & (UINT32) ~val)))



/* 
 * Define the device function table.  This is static across all driver
 * instances.
 */

LOCAL I2C_MASTER_FUNCS i2cMasterFuncs = 
    {
    (FUNCPTR) mpc8640DI2CWrite,		/* write func. */		 
    (FUNCPTR) mpc8640DI2CRead,		/* read func. */
    };		

/***************************************************************************
*
* mpc8640DI2CDrvLoad - Load i2c driver module 
*
* Load i2c driver module 
*
* The I2C master device load string formed by this routine is in the following
* following format.
* "<unit>:<regBase>"
* <unit>
* Device Unit number 
* <regBase>
* Device register base memory address from MBAR value : 
*    - 0x3d00 for I2C1 MGT5200 controller
*    - 0x3d40 for I2C2s MGT5200 controller
*
* RETURNS: OK on success, ERROR on failure
*/

void * mpc8640DI2CDrvLoad
	(
	char * initString     		/* ptr to initialization string */
	)
	{
		/* pointer to X220 I2C driver structure */
		MPC8640D_I2C_DRV_CTRL * pDrvCtrl;       


		DRV_LOG (DRV_DEBUG_LOAD, ("Loading mpc8640D I2C driver..\n"), 1, 2, 3, 4, 5, 6);

		if (initString == NULL)
		return (NULL);

		if (initString[0] == 0)
		{
		bcopy ((char *)MPC8640DI2C_DEV_NAME,(void *)initString, MPC8640DI2C_DEV_NAME_LEN);
		return (0);
		}

		/* allocate the device structure */
		pDrvCtrl = (MPC8640D_I2C_DRV_CTRL *) malloc (sizeof (MPC8640D_I2C_DRV_CTRL));
		if (pDrvCtrl == NULL)
		return (NULL);
        
		/* Parse InitString */
		if (mpc8640DI2CInitParse (pDrvCtrl, initString) == ERROR)
		goto errorExit;	

		/* 
		 * reset the chip: this should be replaced by a true 
		 * adapter reset routine, once the init code is here.
		 */

		if (mpc8640DI2CReset (pDrvCtrl) != OK)
		goto errorExit;

		/* sanity check the unit number */

		if (pDrvCtrl->unit < 0 )
		goto errorExit;    

		/* i2CObj initializations */

		if (i2cMasterObjInit ((I2C_MASTER_OBJ*) pDrvCtrl,
				  MPC8640DI2C_DEV_NAME, 
				  pDrvCtrl->unit, 
				  &i2cMasterFuncs,
				  "MPC8640D0 I2C Master Controller",
				  I2C_MASTER_FLG_STANDARD_MODE_ONLY) == ERROR)
		goto errorExit;


		DRV_LOG (DRV_DEBUG_LOAD, ("mpc8640DDrvLoad... Done \n"), 1, 2, 3, 4, 5, 6);

		return (&pDrvCtrl->i2cMasterObj);

	errorExit:
		DRV_LOG (DRV_DEBUG_LOAD, "mpc8640DDrvLoad failed\n", 0,0,0,0,0,0);

		free ((char *) pDrvCtrl);
		return NULL;

	}

/*******************************************************************************
*
* mpc8640DI2CInitParse - parse parameter values from initString
*
* The I2C master device load string formed by this routine is in the following
* following format.
* "<unit>:<regBase>"
* <unit>
* Device Unit number 
* <regBase>
* Device register base memory address
*
* RETURNS: OK or ERROR
*/

STATUS mpc8640DI2CInitParse
    (
     /* pointer to X220_I2C_DRV_CTRL structure */
    MPC8640D_I2C_DRV_CTRL *	pDrvCtrl,      		
    char *	                initString	/* parameter string */
    )

    {
    char *  tok;		/* an initString token */
    char *  holder = NULL;	/* points to initString fragment beyond tok */
    
    tok = (char *) strtok_r (initString, ":", &holder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->unit = atoi (tok);

    tok = (char *) strtok_r (NULL, ":", &holder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->regBase = strtoul (tok, NULL, 16);

    DRV_LOG (DRV_DEBUG_LOAD,
	    "mpc8640DI2CInitParse: unit=%d regBase=0x%x flags=0x%x\n",
	    pDrvCtrl->unit, (UINT32) pDrvCtrl->regBase,
	    0, 0,0,0);

    return (OK);
    }


/***************************************************************************
*
* mpc8640DI2CReset - Reset the master I2C Contoller
*
* Reset the master I2C controller
*
* RETURNS: OK on success, ERROR on failure
*/

STATUS mpc8640DI2CReset
	(
    MPC8640D_I2C_DRV_CTRL * pDrvCtrl /* ptr to driver structure */
	)
	{

	/* reset I2C module*/
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
						0x00000000); 
	EIEIO;

	/* set frequency to 85,9 kHz, SDA hold time to 1,95 ms */
	/* SCL_Tap:12, SDA_Tap:4, SCL2Tap:62, Tap2Tap: 64 */  	
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_FREQDIV_REG_OFFSET, i2cFreqSetting);

	EIEIO; 

	/* Enable I2C Bus (Settings: Enable I2C, Disable I2C Interrupts, Transmit mode) */
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
					MPC8640D_I2C_CTL_REG_EN); 

	EIEIO;


	/* reset status register */
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_STAT_REG_OFFSET, 
						0x00000000); 
	EIEIO;

	return(OK);
	}


/***************************************************************************
*
* mpc8640DI2CRead - read data from i2c slave device
*
* Read data from i2c slave device
*
* RETURNS: OK on success, ERROR on failure
*/

STATUS mpc8640DI2CRead
	(
    MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8   addr7bit, 			/* Seven bit address */
	UINT8 * data,     			/* ptr to data buffer */
	UINT32  nbytes,   			/* number of bytes to read */
	UINT32  options,
	int     timeout             /* time out in time loop wait */
	)
	{
	/* index into data buffer */
	int timeoutCount = 0;

	if (data == NULL)
	{
		return ERROR;
	}

	mpc8640DI2CReset(pDrvCtrl);


	/* generate start Signal */
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
				MPC8640D_I2C_READ32(MPC8640D_I2C_CTRL_REG_OFFSET)|MPC8640D_I2C_CTL_REG_TX|MPC8640D_I2C_CTL_REG_STA); 
	EIEIO;

	/* transmit slave adress */
	if (mpc8640DI2CTx (pDrvCtrl,addr7bit|0x1,timeout) != OK) 
	{	
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		DRV_LOG (DRV_DEBUG_RX, ("mpc8640DI2CRead... Tx error \n"), 1, 2, 3, 4, 5, 6);
		return ERROR;
	}

		
	/* wait for bus busy */
	timeoutCount = 0;
	while ( ( !(MPC8640D_I2C_READ32(MPC8640D_I2C_STAT_REG_OFFSET) & MPC8640D_I2C_STAT_REG_BB) ) &&
			(timeoutCount < timeout )  )
	{
		timeoutCount++;
	}

	if (timeoutCount == timeout)
	{
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		DRV_LOG (DRV_DEBUG_RX, ("mpc8640DI2CRead... Timeout \n"), 1, 2, 3, 4, 5, 6);
		return ERROR;
	}


	/* setup Receive mode */	
	MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_TX); 
	EIEIO;

	if (nbytes == 1) 
	{					
		/* last byte must not be acknowledged */
		MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
				MPC8640D_I2C_READ32(MPC8640D_I2C_CTRL_REG_OFFSET)|MPC8640D_I2C_CTL_REG_TXAK); 
	} 
	else 
	{
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_TXAK); 
	}
	EIEIO;
	
	if (nbytes) 
	{
		/* dummy read to start transfer */
		*data = I2C_TO_MPC8640D_DATA(MPC8640D_I2C_READ32(MPC8640D_I2C_DATA_REG_OFFSET));			
	}
	EIEIO;

	while (nbytes) 
	{
		if ((mpc8640DI2CRx (pDrvCtrl, data++, nbytes==1, nbytes==2,timeout)) != OK) 
		{
			/* generate STOP */
			MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
			EIEIO;
			DRV_LOG (DRV_DEBUG_RX, ("mpc8640DI2CRead... Rx error \n"), 1, 2, 3, 4, 5, 6);
			return ERROR;
		}
		nbytes--;
	}


	return(OK);
	}


/***************************************************************************
*
* mpc8640DI2CWrite - write data to i2c slave device
*
* Write data to i2c slave device
*
* RETURNS: OK on success, ERROR on failure
*/

STATUS mpc8640DI2CWrite
(
MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
UINT8   addr7bit,			/* Seven bit address in 7 msb bits*/
UINT8 * data,				/* ptr to data buffer */
UINT32  nbytes,				/* number of bytes to write */
UINT32  options,    
int 	timeout             /* timeout in ticks */
)
{
	int timeoutCount = 0;

	if (data == NULL)
	{
		return ERROR;
	}

	mpc8640DI2CReset(pDrvCtrl);

	/* generate start Signal */
	MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
				MPC8640D_I2C_READ32(MPC8640D_I2C_CTRL_REG_OFFSET)|MPC8640D_I2C_CTL_REG_TX|MPC8640D_I2C_CTL_REG_STA); 
	EIEIO;

	/* transmit slave adress */
	if (mpc8640DI2CTx (pDrvCtrl,addr7bit,timeout) != OK) 
	{	
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		return ERROR;
	}

	/* wait for bus busy */
	timeoutCount = 0;
	while ( ( !( MPC8640D_I2C_READ32(MPC8640D_I2C_STAT_REG_OFFSET) & MPC8640D_I2C_STAT_REG_BB) ) &&
			(timeoutCount < timeout )  )
	{
		timeoutCount++;
	}

	if (timeoutCount == timeout)
	{
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		return ERROR;
	}
	
	while (nbytes) 
	{
		if (mpc8640DI2CTx (pDrvCtrl,*data++,timeout) != OK)
		{
			/* generate STOP */
			MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
			EIEIO;
			return ERROR;
		}
		nbytes--;
	}
	
	/* generate STOP */
	MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
	EIEIO;


	return(OK);
}



/***************************************************************************
*
* mpc8640DI2CRx - Read one byte of data on i2c bus
*
* 
*
* RETURNS: OK on success, ERROR on failure
*/
LOCAL STATUS mpc8640DI2CRx
( 
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl,	/* ptr to driver structure */
	UINT8 * data,     				/* ptr to data buffer */
	int		stop,					/* stop to send */
	int		nack,					/* no ack */
	int     timeout					/* time out in ticks */
)
{
	int timeoutCount = 0;

	/* wait for tranfer finished */
	timeoutCount = 0;
	while ( ( !(MPC8640D_I2C_READ32(MPC8640D_I2C_STAT_REG_OFFSET) & MPC8640D_I2C_STAT_REG_IF ) ) &&
			(timeoutCount < timeout ) )
	{
		timeoutCount++;
	}

	if (timeoutCount == timeout)
	{
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		DRV_LOG (DRV_DEBUG_RX, ("mpc8640DI2CRx... Timeout \n"), 1, 2, 3, 4, 5, 6);
		return ERROR;
	}

	MPC8640D_I2C_CLEAR32(MPC8640D_I2C_STAT_REG_OFFSET,MPC8640D_I2C_STAT_REG_IF);
	EIEIO;
	
	if (stop) 
	{
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
	}
	
	if (nack)
	{
		MPC8640D_I2C_WRITE32(MPC8640D_I2C_CTRL_REG_OFFSET, 
				MPC8640D_I2C_READ32(MPC8640D_I2C_CTRL_REG_OFFSET)|MPC8640D_I2C_CTL_REG_TXAK); 
	} 
	else 
	{
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_TXAK); 
		EIEIO;
	}
		
	*data = I2C_TO_MPC8640D_DATA(MPC8640D_I2C_READ32(MPC8640D_I2C_DATA_REG_OFFSET));
	EIEIO;
	
	return OK;
}



/***************************************************************************
*
* mpc8640DI2CTx - Transmit one byte on I2C bus 
*
* 
*
* RETURNS: OK on success, ERROR on failure
*/

LOCAL STATUS mpc8640DI2CTx
(
	MPC8640D_I2C_DRV_CTRL * pDrvCtrl, /* ptr to driver structure */
	UINT8	data,				/* ptr to data buffer */
	int		timeout             /* timeout */
)
{
	int timeoutCount = 0;

	MPC8640D_I2C_WRITE32(MPC8640D_I2C_DATA_REG_OFFSET,MPC8640D_TO_I2C_DATA(data));
	EIEIO;

	/* wait for tranfer finished */
	timeoutCount = 0;
	while ( ( !( MPC8640D_I2C_READ32(MPC8640D_I2C_STAT_REG_OFFSET) & MPC8640D_I2C_STAT_REG_IF) ) &&
			( timeoutCount < timeout )  )
	{
		timeoutCount++;
	}

	if (timeoutCount == timeout)
	{
		/* generate STOP */
		MPC8640D_I2C_CLEAR32(MPC8640D_I2C_CTRL_REG_OFFSET,MPC8640D_I2C_CTL_REG_STA); 
		EIEIO;
		DRV_LOG (DRV_DEBUG_TX, ("mpc8640DI2CTx... timeout \n"), 1, 2, 3, 4, 5, 6);
		return ERROR;
	}

	MPC8640D_I2C_CLEAR32(MPC8640D_I2C_STAT_REG_OFFSET,MPC8640D_I2C_STAT_REG_IF);
	EIEIO;
	
	if (MPC8640D_I2C_READ32(MPC8640D_I2C_STAT_REG_OFFSET) & MPC8640D_I2C_STAT_REG_RXAK) 
	{	/* Ack received? */
		DRV_LOG (DRV_DEBUG_TX, ("mpc8640DI2CTx... No ack received \n"), 1, 2, 3, 4, 5, 6);
		return ERROR;
	}
	
	return OK;

}

void testTemp()
{

}

