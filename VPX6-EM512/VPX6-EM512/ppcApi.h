#ifndef __PPC_API_H__
#define __PPC_API_H__

#include "vxWorks.h"
#include "string.h"
#include "sysLib.h"
#include "stdlib.h"
#include "intLib.h"
#include "iv.h"
#include "semLib.h"
#include "assert.h"

#include "memPartLib.h"
#include "drv/pci/pciConfigLib.h"
#include "vmlib.h"
#include "logLib.h"


typedef struct tagEXT_INT_S
{
    SEM_ID    stIntSem;
    UINT32    uiIntChan;
    UINT32    uiIntAddr;
	UINT64    uiIntCnt;
	UINT32 	  uiIntChanCnt[32];
}EXT_INT_S;

typedef struct tagM8641_TIMER_S
{
    UINT32            uiFlag;
    UINT32            uiMode;
    VOIDFUNCPTR       pfISR;
    UINT32            uiArg1;
    UINT32            uiArg2;
    UINT32            uiArg3;
    SEM_ID            semLock;
}M8641_TIMER_S;

extern EXT_INT_S gstExtInt;
extern EXT_INT_S gstPabackExtInt;
extern SEM_ID   timeIntSem;

extern VOID extIntInit(VOID);
#endif

