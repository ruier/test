/* sysMotI2c.h - I2C Driver Header Module */

/*
 * Copyright (c) 2000, 2005, 2007-2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 1996-2000 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01e,30jun11,e_d added EEPROM and GPIO device support.
01d,28mar08,z_l modified for code convention. fixed "make man" error.
01c,26jun07,l_g modified for fsl_rdb8313.
01b,16aug05,dtr Tidy up.
01a,28feb00,rhk created from version 01b, MV2100 BSP.
*/

/*
DESCRIPTION
I2C Driver Header Module
*/

#ifndef __INCsysMotI2ch
#define __INCsysMotI2ch

#ifdef __cplusplus
extern "C" {
#endif

/* device types */

#define MPC8313_I2C_CONTROLLER       0
#define I2C_DRV_TYPE                 MPC8313_I2C_CONTROLLER

#define I2C_DEVICE_TYPE_EEPROM_AT24C256           0
#define I2C_DEVICE_TYPE_RTC_DS1339                1
#define I2C_DEVICE_TYPE_GPIO_PCA9536              2

/* driver device data template (os + driver-specifics) */

typedef struct i2cDev 
    {
    UINT32 deviceAddress;   /* address of the I2C device */
    int    deviceType;      /* device type: 0 = EEPROM, 1 = other */
    UINT32 blockSize;       /* logical block size in bytes */
    UINT32 blockOffset;     /* logical block offset from zero of start */
    UINT32 blockTotal;      /* number of logical blocks from offset */
    UINT32 controlAddress;  /* controller address */
    UINT32 pageSize;        /* page size in number of blocks */
    UINT32 controlWord;     /* control word */
    UINT32 sysClkRate;      /* system clock rate */
    } i2cDev_t;

/* driver command packet */

typedef struct i2cCmdPckt 
    {
    int    command;         /* command identifier */
    int    status;          /* status (error code) */
    UINT32 memoryAddress;   /* memory address */
    UINT32 blockNumber;     /* block number */
    UINT32 nBlocks;         /* number of blocks (bytes) to transfer */
    UINT32 eCount;          /* expected data count */
    UINT32 aCount;          /* actual data count */
    int deviceType ;
    } i2cCmdPckt_t;

typedef struct 
    {
    int     unit ;
    UINT32  baseAdrs ;
    } I2C_DRV_CTRL ;

/* ioctl flags */

#define I2C_IOCTL_WR            0
#define I2C_IOCTL_RD            1
#define I2C_IOCTL_RMW_OR        2
#define I2C_IOCTL_RMW_AND       3
#define I2C_IOCTL_RMW_AND_OR    4
    
/* command identifiers */

#define I2C_READOP              0   /* read operation */
#define I2C_WRITOP              1   /* write operation */
#define I2C_FRMTOP              2   /* format operation */
#define I2C_CHCKOP              3   /* check operation */

#define I2C_BYTE_NUM_MASK       0x700
#define I2C_BYTE_NUM_SHIFT      7

/* controller specific function pointers table template */

typedef struct i2cDrvRoutines 
    {
    int (*pI2cCycleStart)();        /* start cycle */
    int (*pI2cCycleStop)();         /* stop cycle */
    int (*pI2cCycleRead)();         /* read cycle */
    int (*pI2cCycleWrite)();        /* write cycle */
    int (*pI2cCycleAckIn)();        /* acknowledge in cycle */
    int (*pI2cCycleAckOut)();       /* acknowledge out cycle */
    int (*pI2cKnownState)();        /* known state */
    void (*pI2cDelay)();            /* delay routine */
    } i2cDrvRoutines_t;

/* routine call macros */

/*
 * Note: this driver could dynamically assign function pointers
 * but ROM based switch statements are not permitted so the 
 * function pointers are set here for the specific board BSP.
 */
 
#define I2C_CYCLE_START(x)          i2cCycleMpc8313Start(x)
#define I2C_CYCLE_STOP(x)           i2cCycleMpc8313Stop(x)
#define I2C_CYCLE_READ(x,ad,y)      i2cCycleMpc8313Read(x,ad,y)
#define I2C_CYCLE_WRITE(x,wd)       i2cCycleMpc8313Write(x,wd)
#define I2C_CYCLE_ACKIN(x)          i2cCycleMpc8313AckIn(x)
#define I2C_CYCLE_ACKOUT(x)         i2cCycleMpc8313AckOut(x)
#define I2C_KNOWN_STATE(x)          i2cCycleMpc8313KnownState(x)
#define I2C_DELAY(d)                i2cCycleMpc8313Delay(d)

/* error codes */

#define I2C_ERROR_CYCLE_START   1   /* start cycle */
#define I2C_ERROR_CYCLE_STOP    2   /* stop cycle */
#define I2C_ERROR_CYCLE_READ    3   /* read cycle */
#define I2C_ERROR_CYCLE_WRITE   4   /* write cycle */
#define I2C_ERROR_CYCLE_ACKIN   5   /* acknowledge in cycle */
#define I2C_ERROR_CYCLE_ACKOUT  6   /* acknowledge out cycle */
#define I2C_ERROR_KNOWN_STATE   7   /* known state */
#define I2C_ERROR_CYCLE_TIMEOUT (1 << 16)    /* cycle timed out */

UINT8 i2cIoctl 
    (
    UINT32   ioctlflg,      /* input/ouput control flag */
    UINT32   address,       /* address of device register to be operated upon */
    UINT8    bdata1,        /* data item 1 for read/write operation */
    UINT8    bdata2         /* data item 2 for read/write operation */
    ) ;

STATUS i2cDrvInit 
    (
    int       unit,
    int       i2cControllerType     /* I2C controller type */
    ) ;

int i2cRead 
    (
    int      unit,
    UINT32   deviceAddress, /* Device's I2C bus address */
    int      deviceType,
    UINT32   startBlk,      /* starting block to read, or temp reg # */
    UINT32   numBlks,  /* number of blocks to read, or temp reg size (1 or 2) */
    char *   pBuf           /* pointer to buffer to receive data */
    ) ;

int i2cWrite 
    (
    int      unit,
    UINT32   deviceAddress, /* Device's I2C bus address */
    int      deviceType,
    UINT32   startBlk,      /* starting block to write */
    UINT32   numBlks,       /* number of blocks to write */
    char *   pBuf           /* pointer to buffer of send data */
    ) ;

int i2cDoOp 
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    i2cCmdPckt_t *  pI2cCmdPacket   /* pointer to command packet */
    ) ;

UINT32 i2cAddressMunge 
    (
    UINT32 deviceAddress
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCsysMotI2ch */

