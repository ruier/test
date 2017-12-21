#ifndef _vxbm8641I2c_h
#define _vxbm8641I2c_h

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

/* typedefs */

#define VXB_VER_4_2_1		0x00040201

#define I2C_GET_WRITE_FUNCS       1
#define I2C_GET_READ_FUNCS       2

struct i2cDrvCtrl
{
    UINT32  i2cSlaveAddr;
    UINT32 i2cFdr;
    UINT32  customMethodCount; 
    int(*i2cWrite)( UINT32 baseAddr,UINT8 deviceAddress,UINT32   startBlk,UINT8   numBlks,UINT8  *pBuf,UINT8 mode);
    int(*i2cRead)(UINT32 baseAddr,UINT8  deviceAddress,UINT32    startBlk,UINT8     numBlks,UINT8  *pBuf,UINT8 mode);
    UINT32 baseAddr;
};



#endif
