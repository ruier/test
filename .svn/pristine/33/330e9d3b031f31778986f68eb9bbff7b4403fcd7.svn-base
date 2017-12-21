/* i2c.h - tsi108-specific support functions for I2C interface header file. */

/* Copyright 2007 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01a,11may07,jjm  Initial version, based on IBM 750GX/GL EVB BSP
*/


#ifndef INCi2ch
#define INCi2ch

/* defines */

/* register block offset for SPD I2C channel */
#define TSI108_I2C_SDRAM_OFFSET 0x4400

#define I2C_EEPROM_DEVCODE      0xA     /* I2C EEPROM device code */
#define I2C_EEPROM_DEVADDR      (I2C_EEPROM_DEVCODE << 3) /* Full I2C address */
#define I2C_RTC_DEVCODE         0xD     /* I2C MAXIM RTC DS1337 device code */
#define I2C_RTC_DEVADDR         (I2C_RTC_DEVCODE << 3)  /* Full I2C address */
#define I2C_DELAY 0x10000

#define I2C_CHANNEL_EEPROM      0
#define I2C_CHANNEL_RTC         0
#define I2C_CHANNEL_SDRAM       1
#define I2C_PAGE_SIZE           256
#define I2C_MAX_SIZE            (8*256)

/* I2C status codes */
#define TSI108_I2C_SUCCESS      0
#define TSI108_I2C_PARAM_ERR    1
#define TSI108_I2C_TIMEOUT_ERR  2
#define TSI108_I2C_IF_BUSY      3
#define TSI108_I2C_IF_ERROR     4

/* forward declarations */

STATUS i2cLibInit (void);

STATUS i2cLibUnInit (void);

ULONG i2cReadDev
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  size          /* data unit size */
    );

ULONG i2cWriteDev
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UINT32 wrData,      /* data */
    UINT  size          /* data unit size */
    );

ULONG i2cRead
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  length        /* number of bytes to read */
    );

ULONG i2cWrite
    (
    UINT  i2cChan,      /* I2C channel number: 0 - main, 1 - SDC SPD */
    UINT  byteAddr,     /* Byte address within I2C device */
    UINT  page,         /* Page select */
    UINT  devCode,      /* Device code, 0xa = EEPROM */
    UCHAR *buffer,      /* pointer to data buffer */
    UINT  length        /* number of bytes to write */
    );

#define I2C_WRITE_BYTE(bus, dev, reg, val, stat)        \
    stat = i2cWriteDev(bus, reg, ((dev) & 0x07),        \
                ((dev) >> 3) & 0x0F, val, sizeof(UINT8))

#define I2C_READ_BYTE(bus, dev, reg, val_ptr, stat)     \
    stat = i2cReadDev(bus, reg, ((dev) & 0x07),         \
                ((dev) >> 3) & 0x0F, val_ptr, sizeof(UINT8))

#define I2C_WRITE_UINT16(bus, dev, reg, val, stat)      \
    stat = i2cWriteDev(bus, reg, ((dev) & 0x07),        \
                ((dev) >> 3) & 0x0F, val, sizeof(UINT16))

#define I2C_READ_UINT16(bus, dev, reg, val_ptr, stat)   \
    stat = i2cReadDev(bus, reg, ((dev) & 0x07),         \
                ((dev) >> 3) & 0x0F, val_ptr, sizeof(UINT16))

#endif /* INCi2ch */
