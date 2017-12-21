/* sysMotI2c.h - I2C Driver Header Module */

/*
 * Copyright (c) 2007, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,21may10,b_m  add TFP410 support.
01a,05nov07,b_m  derived from version 01b of ads834x/sysMotI2c.h.
*/

#ifndef INCsysMotI2ch
#define INCsysMotI2ch

#ifdef __cplusplus
extern "C" {
#endif

/* driver type */

#define I2C_DRV_TYPE                    0

/* device type */

#define I2C_DEVICE_TYPE_IOPORT_PCF8574  0
#define I2C_DEVICE_TYPE_EEPROM_AT24C32  1
#define I2C_DEVICE_TYPE_RTC_M41T00      2
#define I2C_DEVICE_TYPE_TFP410          2

/* device addresses on ADS5121e */

#define I2C_ADDR_PCF8574A               0x38
#define I2C_ADDR_AT24C32A               0x50
#define I2C_ADDR_M41T00                 0x68
#define I2C_ADDR_TFP410                 0x38

/* driver command packet */

typedef struct i2cCmdPckt
    {
    int          command;       /* command identifier */
    int          status;        /* status (error code) */
    unsigned int memoryAddress; /* memory address */
    unsigned int blockNumber;   /* block number */
    unsigned int nBlocks;       /* number of blocks (bytes) to transfer */
    unsigned int eCount;        /* expected data count */
    unsigned int aCount;        /* actual data count */
    int          deviceType;
    } i2cCmdPckt_t;

/* driver control */

typedef struct
    {
    int     unit;
    UINT32  baseAdrs;
    } I2C_DRV_CTRL;

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

/* controller specific function pointers table template */

typedef struct i2cDrvRoutines
    {
    int  (*pI2cCycleStart)();   /* start cycle */
    int  (*pI2cCycleStop)();    /* stop cycle */
    int  (*pI2cCycleRead)();    /* read cycle */
    int  (*pI2cCycleWrite)();   /* write cycle */
    int  (*pI2cCycleAckIn)();   /* acknowledge in cycle */
    int  (*pI2cCycleAckOut)();  /* acknowledge out cycle */
    int  (*pI2cKnownState)();   /* known state */
    void (*pI2cDelay)();        /* delay routine */
    } i2cDrvRoutines_t;

/* routine call macros */

/*
 * Note: this driver could dynamically assign function pointers
 * but ROM based switch statements are not permitted so the
 * function pointers are set here for the specific board BSP.
 */

#define I2C_CYCLE_START(x)      i2cCycleMpc512xStart(x)
#define I2C_CYCLE_STOP(x)       i2cCycleMpc512xStop(x)
#define I2C_CYCLE_READ(x,ad,y)  i2cCycleMpc512xRead(x,ad,y)
#define I2C_CYCLE_WRITE(x,wd)   i2cCycleMpc512xWrite(x,wd)
#define I2C_CYCLE_ACKIN(x)      i2cCycleMpc512xAckIn(x)
#define I2C_CYCLE_ACKOUT(x)     i2cCycleMpc512xAckOut(x)
#define I2C_KNOWN_STATE(x)      i2cCycleMpc512xKnownState(x)
#define I2C_DELAY(d)            i2cCycleMpc512xDelay(d)

/* error codes */

#define I2C_ERROR_CYCLE_START   1           /* start cycle */
#define I2C_ERROR_CYCLE_STOP    2           /* stop cycle */
#define I2C_ERROR_CYCLE_READ    3           /* read cycle */
#define I2C_ERROR_CYCLE_WRITE   4           /* write cycle */
#define I2C_ERROR_CYCLE_ACKIN   5           /* acknowledge in cycle */
#define I2C_ERROR_CYCLE_ACKOUT  6           /* acknowledge out cycle */
#define I2C_ERROR_KNOWN_STATE   7           /* known state */
#define I2C_ERROR_CYCLE_TIMEOUT (1 << 16)   /* cycle timed out */

/* forward declarations */

UINT8 i2cIoctl
    (
    UINT32  ioctlflg,  /*
                        * input/ouput control flag
                        * 0, write
                        * 1, read
                        * 2, read/modify/write (ORing)
                        * 3, read/modify/write (ANDing)
                        * 4, read/modify/write (AND/ORing)
                        */
    UINT32  address,    /* address of device register to be operated upon */
    UINT8   bdata1,     /* data item 1 for read/write operation */
    UINT8   bdata2      /* data item 2 for read/write operation */
    );

STATUS i2cDrvInit
    (
    int     unit
    );

int i2cRead
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    int             deviceType,
    unsigned int    startBlk,       /* starting block to read */
    unsigned int    numBlks,        /* number of blocks to read */
    char *          pBuf            /* pointer to buffer to receive data */
    );

int i2cWrite
    (
    int             unit,
    UINT32          deviceAddress,  /* device I2C bus address */
    int             deviceType,
    unsigned int    startBlk,       /* starting block to write */
    unsigned int    numBlks,        /* number of blocks to write */
    char *          pBuf            /* pointer to buffer of send data */
    );

#ifdef __cplusplus
}
#endif

#endif
