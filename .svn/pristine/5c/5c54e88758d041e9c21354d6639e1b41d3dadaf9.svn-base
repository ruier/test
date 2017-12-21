/* i2c.h - tsi108-specific support functions for I2C interface header file. */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01a,23aug22,vsc  created 
*/

#ifndef INCi2ch
#define INCi2ch

/* defines */

 /* register block offset for SPD I2C channel */
#define TSI108_I2C_SDRAM_OFFSET 0x4400

#define I2C_EEPROM_DEVCODE    0xA  /* I2C EEPROM device code */
#define I2C_RTC_DEVCODE       0xD  /* I2C MAXIM RTC DS1337 device code */
#define I2C_DELAY 0x10000

#define I2C_CHANNEL_EEPROM    0
#define I2C_CHANNEL_SDRAM     1
#define I2C_PAGE_SIZE         256
#define I2C_MAX_SIZE          (8*256)

/* I2C status codes */
#define TSI108_I2C_SUCCESS      0
#define TSI108_I2C_PARAM_ERR    1
#define TSI108_I2C_TIMEOUT_ERR  2
#define TSI108_I2C_IF_BUSY      3
#define TSI108_I2C_IF_ERROR     4

/* forward declarations */
ULONG i2cReadByte
    (
    UINT  i2cChan,           /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer            /* pointer to data buffer */
    );

ULONG i2cWriteByte
    (
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer            /* pointer to data buffer */
    );

ULONG i2cRead
    (
    UINT  i2cChan,           /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer,           /* pointer to data buffer */
    UINT  length             /* number of bytes to read */
    );

ULONG i2cWrite
    (
    UINT  byteAddr,          /* Byte address within I2C device */
    UINT  page,              /* Page select */
    UINT  devCode,           /* Device code, 0xa = EEPROM */
    UCHAR *buffer,           /* pointer to data buffer */
    UINT  length             /* number of bytes to write */
    );

#endif /* INCi2ch */
