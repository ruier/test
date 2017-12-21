/* sysMotI2c.h - I2C Driver Header Module */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1996-2000 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,14oct05,dtr  created from version 01a, ads834x BSP.
*/

/*
DESCRIPTION

I2C Driver Header Module
*/

#ifndef	INCsysMotI2ch
#define	INCsysMotI2ch

#ifdef __cplusplus
extern "C" {
#endif

/* device types */

#define MPC85XX_I2C_CONTROLLER      0
#define I2C_DRV_TYPE                MPC85XX_I2C_CONTROLLER
#define I2C_DEVICE_TYPE_RTC_DS1374  0


#define MPC85XX_I2C_FREQ_DIVIDER    0x31    /* 3072 */


/* driver device data template (os + driver-specifics) */

typedef struct i2cDev
	{
	unsigned int deviceAddress;     /* address of the I2C device */
	int          deviceType;		/* device type: 0 = EEPROM, 1 = other */
	unsigned int blockSize;			/* logical block size in bytes */
	unsigned int blockOffset;		/* logical block offset from zero of start */
	unsigned int blockTotal;		/* number of logical blocks from offset */
	unsigned int controlAddress;	/* controller address */
	unsigned int pageSize;			/* page size in number of blocks */
	unsigned int controlWord;		/* control word */
	unsigned int sysClkRate;		/* system clock rate */
	} i2cDev_t;

/* driver command packet */

typedef struct i2cCmdPckt
    {
    int          command;           /* command identifier */
    int          status;            /* status (error code) */
    unsigned int memoryAddress;	    /* memory address */
    unsigned int blockNumber;	    /* block number */
    unsigned int nBlocks;	        /* number of blocks (bytes) to transfer */
    unsigned int eCount;	        /* expected data count */
    unsigned int aCount;	        /* actual data count */
    int          deviceType;        /* device type */
    } i2cCmdPckt_t;

/* I2C drvier control */

typedef struct {
    int          unit;
    UINT32       baseAdrs;
} I2C_DRV_CTRL ;

/* ioctl flags */

#define I2C_IOCTL_WR            0
#define I2C_IOCTL_RD			1
#define I2C_IOCTL_RMW_OR 		2
#define I2C_IOCTL_RMW_AND 		3
#define I2C_IOCTL_RMW_AND_OR 	4

/* command identifiers */

#define I2C_READOP              0   /* read operation */
#define I2C_WRITOP	            1	/* write operation */
#define I2C_FRMTOP	            2	/* format operation */
#define I2C_CHCKOP	            3	/* check operation */

#define I2C_BYTE_NUM_MASK       0x700
#define I2C_BYTE_NUM_SHIFT      7

/* controller specific function pointers table template */

typedef struct i2cDrvRoutines
{
    int (*pI2cCycleStart)();    /* start cycle */
    int (*pI2cCycleStop)();	    /* stop cycle */
    int (*pI2cCycleRead)();	    /* read cycle */
    int (*pI2cCycleWrite)();	/* write cycle */
    int (*pI2cCycleAckIn)();	/* acknowledge in cycle */
    int (*pI2cCycleAckOut)();	/* acknowledge out cycle */
    int (*pI2cKnownState)();	/* known state */
    void (*pI2cDelay)();	    /* delay routine */
} i2cDrvRoutines_t;

/* routine call macros */

/*
 * Note: this driver could dynamically assign function pointers
 * but ROM based switch statements are not permitted so the
 * function pointers are set here for the specific board BSP.
 */

#define I2C_CYCLE_START(x)      i2cCycleMpc85xxStart(x)
#define I2C_CYCLE_STOP(x)		i2cCycleMpc85xxStop(x)
#define I2C_CYCLE_READ(x,ad,y)	i2cCycleMpc85xxRead(x,ad,y)
#define I2C_CYCLE_WRITE(x,wd)	i2cCycleMpc85xxWrite(x,wd)
#define I2C_CYCLE_ACKIN(x)		i2cCycleMpc85xxAckIn(x)
#define I2C_CYCLE_ACKOUT(x)	    i2cCycleMpc85xxAckOut(x)
#define I2C_KNOWN_STATE(x)		i2cCycleMpc85xxKnownState(x)
#define I2C_DELAY(d)		    i2cCycleMpc85xxDelay(d)

/* error codes */

#define I2C_ERROR_CYCLE_START	1   /* start cycle */
#define I2C_ERROR_CYCLE_STOP	2	/* stop cycle */
#define I2C_ERROR_CYCLE_READ	3	/* read cycle */
#define I2C_ERROR_CYCLE_WRITE	4	/* write cycle */
#define I2C_ERROR_CYCLE_ACKIN	5	/* acknowledge in cycle */
#define I2C_ERROR_CYCLE_ACKOUT	6	/* acknowledge out cycle */
#define I2C_ERROR_KNOWN_STATE	7	/* known state */
#define I2C_ERROR_CYCLE_TIMEOUT	(1 << 16)   /* cycle timed out */

/* foward declarations */

UINT8   i2cIoctl
    (
    UINT32          ioctlflg,
    UINT32          address,
    UINT8           bdata1,
    UINT8           bdata2
    );

STATUS i2cDrvInit
    (
    int             unit,
    int             i2cControllerType
    );

int i2cRead
    (
    int             unit,
    UINT32          deviceAddress,
    int             deviceType,
    unsigned int    startBlk,
    unsigned int    numBlks,
    char *          pBuf
    );

int i2cWrite
    (
    int             unit,
    UINT32          deviceAddress,
    int             deviceType,
    unsigned int    startBlk,
    unsigned int    numBlks,
    char *          pBuf
    );

int i2cDoOp
    (
    int             unit,
    UINT32          deviceAddress,
    i2cCmdPckt_t *  pI2cCmdPacket
    );

unsigned int i2cAddressMunge
    (
    unsigned int    deviceAddress
    );

#ifdef __cplusplus
}
#endif

#endif  /* INCsysMotI2ch */
