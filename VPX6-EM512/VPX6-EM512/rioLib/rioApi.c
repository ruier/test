/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : rioApi.c
  version    : 1.0
  author     : lgh
  data       : 2013.1.29
  function   : rapid io application interface 
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <semLib.h>
#include "assert.h"
#include "rioDrv.h"
#include "rioHal.h"
#include "rioFsl.h"
#include "ppcApi.h"


#include "rioFsl.c"
#include "rioHal.c"
#include "rioDrv.c"


/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS rioSystemEnumerate(UINT32 vuiHostId);
extern STATUS rioMsgInit(msgHook hook);
extern void  rioSetSrioOW(void);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

 extern UINT32 guiRioDbgLevel;
 extern SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/
UINT32 rioDmaNread(UINT8 chan,UINT32 localAddr,UINT32 rioAddr,UINT32  byteCnt)
{
	DMA_M8641_DEDICATED_S  stChanInfo;
	VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID		   pstDevId = NULL;
	UINT32				   rioDevId = 0;
	UINT32				   uiOffset = 0;
	STATUS				   iRet = OK;

	if(0 == byteCnt)
	{
		return OK;
	}
	
	rioDevId = rioGetDevIdFromAddr(rioAddr,&uiOffset);
	if(ERROR == rioDevId)
	{
		printf("device outbound is not set\n");
		return DMA_ERROR_PARM;
	}
	
	if(OK != rioDevOnLine1(rioDevId))
	{
		printf("device is not online\n");
		return DMA_ERROR_PROM;
	}
		
    stChanInfo.uiMagic = DMA_M8641_MAGIC;
    stChanInfo.uiSrcAttr = RAPIDIO_SRC_ATTRIB | DMA8641_SATR_SRIO_ID(rioDevId);
    stChanInfo.uiDestAttr = lOCAL_DST_ATTRIB_SNOOP; 
	stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
	stChanInfo.uiUnit = chan;
	
	pstDevId = vxbInstByNameFind("dma",chan);
	if(NULL == pstDevId)
	{
		printf("cannot find dma engine\n");
		return DMA_ERROR_OTHER;
	}
	
	pstDmaChan = vxbDmaChanAlloc(pstDevId,1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
	if(NULL == pstDmaChan)
	{
		printf("resource get failed\n");
		return DMA_ERROR_TIMEOUT;
	}
	
	iRet = vxbDmaReadAndWait(pstDmaChan,uiOffset,localAddr,&byteCnt,0,0);
	vxbDmaChanFree(pstDmaChan);
	
	return iRet;
}
	
	
UINT32 rioDmaNwrite(UINT8 chan,UINT32 localAddr,UINT32 rioAddr,UINT32 byteCnt)
{
    DMA_M8641_DEDICATED_S  stChanInfo;
    VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID		   pstDevId = NULL;
	UINT32				   rioDevId = 0;
	UINT32				   uiOffset = 0;
	STATUS				   iRet = OK;

	if(0 == byteCnt)
	{
		return OK;
	}
	rioDevId = rioGetDevIdFromAddr(rioAddr,&uiOffset);
	if(ERROR == rioDevId)
	{
		printf("device outbound is not set\n");
	
		return DMA_ERROR_PARM;
	}
	if(OK != rioDevOnLine1(rioDevId))
	{
		printf("device is not online\n");
		return DMA_ERROR_PROM;
	}
	stChanInfo.uiMagic = DMA_M8641_MAGIC;
	stChanInfo.uiDestAttr = RAPIDIO_DST_ATTRIB| DMA8641_DATR_SRIO_ID(rioDevId);
	stChanInfo.uiSrcAttr = lOCAL_SRC_ATTRIB_SNOOP;
	stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
	stChanInfo.uiUnit = chan;

	pstDevId = vxbInstByNameFind("dma",chan);
	if(NULL == pstDevId)
	{
		printf("cannot find dma engine\n");
		return DMA_ERROR_OTHER;
	}
	pstDmaChan = vxbDmaChanAlloc(pstDevId,1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
	if(NULL == pstDmaChan)
	{
		printf("resource get failed\n");
		return DMA_ERROR_TIMEOUT;
	}
	
	iRet = vxbDmaWriteAndWait(pstDmaChan,localAddr,uiOffset,&byteCnt,0,0);
	vxbDmaChanFree(pstDmaChan);

	return iRet;
}

UINT32 rioSysInit(pfHook pfDbHook,msgHook pfMsgHook,int flag)
{
    UINT32 uiHostId = 0;

    if(OK != rioSetup())
    {
        return -1;
    }
    

    uiHostId = rioGetLocalPort() + 0x10;
    uiHostId = 0xe;
    if(OK != rioSystemEnumerate(uiHostId))
    {
        return -4;
    }
    
    if(OK != rioInitDoorbell(pfDbHook))
    {
        return -2;
    }
    
    if(OK != rioMsgInit(pfMsgHook))
    {
        return -5;
    }

    if(0 != flag)
    {
        rioSetSrioOW();
    }

    SRIO_LOG(SRIO_DBG_INFO,"rioSysInit ok\n",0,0,0,0,0);

    return 0;
}

void rioStub(void)
{
    
}
