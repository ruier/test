#ifndef __RIO_DEPEND_H__
#define __RIO_DEPEND_H__

#include "common_lib_cfg/rioCfg.h"


#define RIO_PORT_SPEED25        0x1      /*2.5Gbps*/

#define SRIO_PORT_WRITE_MODE_REG		0xd34e0

#define SRIO_LOG(level, X0, X1, X2, X3, X4, X5)            \
{                                                          \
    if (guiDbgLevel >= level)                              \
    {                                                      \
      logMsg (X0, X1, X2, X3, X4, X5,0);                   \
    }                                                      \
}

typedef enum tagSRIO_DBG_LEVEL_E
{
    SRIO_DBG_ERR = 0,
    SRIO_DBG_WARN,
    SRIO_DBG_INFO,
    SRIO_DBG_DATA,
    SRIO_DBG_BUTT
}SRIO_DBG_LEVEL_E;

typedef  VOID (*pfHook)(UINT8 src,UINT8 dst ,UINT16 info);

typedef  VOID (*msgHook)(UINT8 mailBox,UINT8 * msgPtr,UINT16 len);

#endif /* __RIO_DEPEND_H__ */
