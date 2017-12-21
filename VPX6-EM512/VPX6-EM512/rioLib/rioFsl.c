/******************************************************************************

  Copyright (C), 2001-2011, PROPHET ELECTRONIC TECHNOLOGY Co., Ltd.

 ******************************************************************************
  File Name     : rioFsl.c
  Version       : Initial Draft
  Author        : lighuohui
  date          : 2013.1.24
  modify        :
  function      : powerpc software interface to configuration registers
******************************************************************************/

#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <semLib.h>
#include "assert.h"
#include "rioDrv.h"
#include "rioHal.h"
#include "rioFsl.h"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/
extern UINT32 guiRioDbgLevel;
extern SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern STATUS rioAcquireDevLock(UINT32 ,UINT32 ,UINT32 ,UINT32 ,UINT32 *);
extern VOID rioSetMaintenanceWin(UINT32 LocalAddr,UINT8 hopCount,UINT8 targetId);
/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
void rioRecvMsgISR(msgHook hook);
void rioRecvMsg1ISR(msgHook hook);
void rioSendDBISR(void);
void rioSendMsgISR(UINT8 mailBox);

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*****************************************************************************
 func : rioCfgRead
 desc : performs a configuration read transaction from CAR and/or CSR
        register(s) belonging to a local or remote RapidIO device
 parm : localport: Local port number [IN]
        destid:    Destination ID of the target device [IN]
        hopcount:  Hop count [IN]
        offset:    Word-aligned (four byte boundary) 
                   offset in bytes of the CAR or CSR [IN]
        *readdata: Pointer to storage for received data [OUT]
*****************************************************************************/
VOID rioCfgRead(UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 offset,
                      UINT32 *readData)
{
    if(RIO_HOP_LOCAL == hopCnt)
    {
        *readData = FSL_READ_REG((SRIO_BASE_ADDR + 0xc0000 + offset));
    }
    else
    {
        rioSetMaintenanceWin(SRIO_MAINTENANCE_ADDR,hopCnt,destId);
        *readData = FSL_READ_REG(SRIO_MAINTENANCE_ADDR + offset);
    } 
}

/*****************************************************************************
 func : rioCfgWrite
 desc : performs a configuration write transaction from CAR and/or CSR
        register(s) belonging to a local or remote RapidIO device
 parm : localport: Local port number [IN]
        destid:    Destination ID of the target device [IN]
        hopcount:  Hop count [IN]
        offset:    Word-aligned (four byte boundary) 
                   offset in bytes of the CAR or CSR [IN]
        writeData: data to be written [IN]
*****************************************************************************/
VOID rioCfgWrite(UINT32  localPort,UINT32 destId,UINT32 hopCnt,UINT32 offset,
                 UINT32  writeData)
{
    if(RIO_HOP_LOCAL == hopCnt)
    {
        FSL_WRITE_REG((SRIO_BASE_ADDR + 0xc0000 + offset),writeData);
    }
    else
    {
        rioSetMaintenanceWin(SRIO_MAINTENANCE_ADDR,hopCnt,destId);
        FSL_WRITE_REG((SRIO_MAINTENANCE_ADDR + offset),writeData);
    }
}

VOID rioSetMaintenanceWin(UINT32 LocalAddr,UINT8 hopCount,UINT8 targetId)
{
	UINT32 uiVal = 0;

    uiVal = (targetId << 22) | (hopCount << 12);
    FSL_WRITE_REG((M8641RIO_ROWTAR1 + SRIO_BASE_ADDR),uiVal);
    uiVal = LocalAddr >> 12;
    FSL_WRITE_REG((M8641RIO_ROWBAR1 + SRIO_BASE_ADDR),uiVal);
    FSL_WRITE_REG((M8641RIO_ROWAR1 + SRIO_BASE_ADDR),0x88077017);
    while(0x88077017 != FSL_READ_REG(M8641RIO_ROWAR1 + SRIO_BASE_ADDR));
}

BOOL rioIsAgent(UINT32 vuiLocPort)
{
    if(0x00 == (FSL_READ_REG((SRIO_BASE_ADDR + 0xc013c)) & (1 << 31)))
    {
         return TRUE;
    }
    
    return FALSE;
}

UINT32 rioGetHostPort(UINT32 localPort,UINT32 destId,UINT32 hopCount)
{
	UINT32 uiVal = 0;

    rioCfgRead(localPort,destId,hopCount,RIO_SWP_INFO_CAR,&uiVal);
    
	return (uiVal & 0xff);
}

BOOL rioMPortIsActive(UINT32 vuiLocPort)
{
    UINT32 uiLoop = 0;

    for(uiLoop = 0; uiLoop < SRIO_MAX_RETRY_REQ;uiLoop++)
    {
        FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_LMREQCSR),SRIO_REQ_INPUT_STATUS);
        if(SRIO_REQ_RESP_VALID & FSL_READ_REG(SRIO_BASE_ADDR + SRIO_LMRESPCSR))
        {
            return TRUE;
        }
    }

    return FALSE;
}

VOID rioSetHostId(UINT32 vuiLocPort,UINT32 uiHostId)
{
	UINT32 uiVal = 0;

    uiVal = ((uiHostId) | (uiHostId<<8) | (uiHostId<<16)) & 0xffffff;
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_BDIDCSR),uiVal);
}

VOID rioGetHostId(UINT32 vuiLocPort,UINT32 *vpuiHostId)
{
	UINT32 uiVal = 0;

    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_BDIDCSR));
    *vpuiHostId = uiVal & 0xff;
}

UINT32 rioGetFreeOub(VOID)
{
    UINT32  uiLoop = 0;

    for(uiLoop = 0;uiLoop < 7;uiLoop++)
    {
        if(0x0 == (gstSrioMport[0].uiOWBitmap & (1 << uiLoop)))
        {
            gstSrioMport[0].uiOWBitmap |= (1 << uiLoop);
            return uiLoop;
        }
    }

    return ERROR;
}

UINT32 rioGetLawSize(UINT32 size)
{
    UINT32  uiSize = 0;
    UINT32  uiLawSize = 0;

    if((size < 4096) || (0 != (size % 4096)))
    {
        return ERROR;
    }
    uiSize = size / 4096;
    uiLawSize = 0xb;
    while(0 != (uiSize / 2))
    {
        uiLawSize++;
        uiSize = uiSize / 2;
    }

    return uiLawSize;
}


UINT32 rioMapOutb(UINT32 localAddr, UINT32 srioAddr, UINT32 size,
                  UINT32 dstID, UINT32 flags)
{
	UINT32  uiOub = 0;
	UINT32  uiBaseAddr = 0;
	UINT32  uiLawSize = 0;
	UINT32  uiVal = 0;
	UINT32  uiSeq = 0;

	uiOub = rioGetFreeOub();
	if(ERROR == uiOub)
	{
	    return ERROR;
	}
	uiLawSize = rioGetLawSize(size);
	if(ERROR == uiLawSize)
	{
	    return ERROR;
	}
	uiBaseAddr = SRIO_BASE_ADDR + 0x40 + 0xd0c00 + uiOub * 0x20;
	uiVal = ((dstID & 0xff) << 22) | (srioAddr >> 12);
	FSL_WRITE_REG(uiBaseAddr,uiVal);
	uiBaseAddr = SRIO_BASE_ADDR + 0x40 + 0xd0c08 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,(localAddr >> 12));
	uiBaseAddr = SRIO_BASE_ADDR + 0x40 + 0xd0c10 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,(0x80044000 | uiLawSize));
	
	uiSeq = gstSrioMport[0].uiOWWin;
	gstSrioMport[0].uiOWWin++;
	if(uiSeq < SRIO_MAX_OUB)
	{
	    gstSrioMport[0].astOW[uiSeq].uiMemLoc = localAddr;
	    gstSrioMport[0].astOW[uiSeq].uiMemTran = srioAddr;
	    gstSrioMport[0].astOW[uiSeq].uiDevId = dstID;
	    gstSrioMport[0].astOW[uiSeq].uiSize = size / 0x100000;
	}

	return OK;
}

UINT32 rioGetFreeInb(VOID)
{
    UINT32  uiLoop = 0;

    for(uiLoop = 0;uiLoop < 4;uiLoop++)
    {
        if(0x0 == (gstSrioMport[0].uiIWBitmap & (1 << uiLoop)))
        {
            gstSrioMport[0].uiIWBitmap |= (1 << uiLoop);
            return uiLoop;
        }
    }

    return ERROR;
}


UINT32 rioMapInb(UINT32 localAddr, UINT32 srioAddr, UINT32 size, UINT32 flags)
{
    UINT32  uiLawSize = 0;
    UINT32  uiInb = 0;
    UINT32  uiBaseAddr = 0;
    static UINT32  uiSeq = 0;

    uiLawSize = rioGetLawSize(size);
    if(ERROR == uiLawSize)
    {
        return ERROR;
    }
    uiInb = rioGetFreeInb();
    if(ERROR == uiInb)
    {
        return ERROR;
    }

    uiBaseAddr = SRIO_BASE_ADDR + 0xd0d60 + uiInb * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(localAddr >> 12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0d68 + uiInb * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(srioAddr >> 12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0d70 + uiInb * 0x20;
	FSL_WRITE_REG(uiBaseAddr,(0x80f55000 | uiLawSize));
	if(uiInb < SRIO_MAX_INB)
    {
        gstSrioMport[0].astIW[uiInb].uiMemLoc = localAddr;
        gstSrioMport[0].astIW[uiInb].uiMemTran = srioAddr;
        gstSrioMport[0].astIW[uiInb].uiSize = size / 0x100000;
        uiSeq++;
    }

    return OK;
}

STATUS rioMsgInbInit(UINT32 uiMsgUnit,UINT32 uiInAddr,msgHook hook)
{
	UINT32  uiBaseAddr = 0;
	UINT32  uiCirqSize = 0xa;/*2048 frame*/
	UINT32  uiFrameSize = 0xb; /*4096 byte*/
	UINT32  uiMIQThresh = 1;
	UINT32  uiVal = 0;
	
	assert(uiMsgUnit < SRIO_MAX_MBOX);

	uiBaseAddr = SRIO_BASE_ADDR + 0x100 * uiMsgUnit;
    
	/*
     *disable inbound 
	*/
	uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_IMR);
	uiVal &= (~(1<<0));
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IMR),uiVal);
	/*
     *clear interrupt status
	*/
	uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_ISR);
	uiVal |= ((1<<0)|(1<<4)|(1<<7)|(1<<10));
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_ISR),uiVal);

    uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_IMR);
	uiVal &= 0x0;
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IMR),uiVal);
	
	uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_IMR);
	uiVal |= ((1<<20) | (1 << 6) | (1 << 8) | (uiCirqSize << 12)|
		      (uiFrameSize << 16)|(uiMIQThresh << 28));
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IMR),uiVal);
	
    FSL_WRITE_REG((uiBaseAddr + M8641RIO_IMMIRIR),0x100);

	/*
	 * Point dequeue/enqueue pointers at first entry in ring 
	*/
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IFQDPAR),uiInAddr);
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IFQEPAR),uiInAddr);
	
	uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_IMR);
	uiVal |= 0x01;
	FSL_WRITE_REG((uiBaseAddr + M8641RIO_IMR),uiVal);
	printf("receive msg data \n");
	if(0x0 == uiMsgUnit)
	{  
        intConnect((VOIDFUNCPTR *)FSL_SR_IN_MU1_INT_VEC,rioRecvMsgISR,(UINT32)hook);
	    intEnable(FSL_SR_IN_MU1_INT_VEC);
	}
    else
    {
        intConnect((VOIDFUNCPTR *)FSL_SR_IN_MU2_INT_VEC,rioRecvMsg1ISR,(UINT32)hook);
	    intEnable(FSL_SR_IN_MU2_INT_VEC);
    }

    return OK;
}

STATUS rioMsgInit(msgHook hook)
{
    UINT32 uiLoop = 0;
	UINT32 uiAddr = 0;

	/*
     *1M align
	*/
    gstSrioMport[0].pucMsgMemAlloc = malloc(SRIO_MSG_INB_SIZE * SRIO_MAX_MBOX + 0x100000);
    if(NULL == gstSrioMport[0].pucMsgMemAlloc)
    {
        SRIO_LOG(SRIO_DBG_ERR,"malloc msg memory failed\n",0,0,0,0,0);
        return ERROR;
    }
	uiAddr = (UINT32)(gstSrioMport[0].pucMsgMemAlloc + 0x100000);
	uiAddr = uiAddr & 0xfff00000;
	gstSrioMport[0].pucMsgMem = (UINT8*)uiAddr; 
	
    memset(gstSrioMport[0].pucMsgMem,0,(SRIO_MSG_INB_SIZE * SRIO_MAX_MBOX));

    gstSrioMport[0].pucMsgOubMemAlloc = malloc(SRIO_MSG_OUB_SIZE * SRIO_MAX_MBOX + 0x100000);
    if(NULL == gstSrioMport[0].pucMsgOubMemAlloc)
    {
        SRIO_LOG(SRIO_DBG_ERR,"malloc oub msg memory failed\n",0,0,0,0,0);
        free(gstSrioMport[0].pucMsgMemAlloc);
        return ERROR;
    }
	uiAddr = (UINT32)(gstSrioMport[0].pucMsgOubMemAlloc + 0x100000);
	uiAddr = uiAddr & 0xfff00000;
	gstSrioMport[0].pucMsgOubMem = (UINT8*)uiAddr; 
    memset(gstSrioMport[0].pucMsgOubMem,0,(SRIO_MSG_OUB_SIZE * SRIO_MAX_MBOX));
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_MBOX;uiLoop++)
    {
        gstSrioMport[0].semMsgAccess[uiLoop] = semBCreate(SEM_Q_FIFO,SEM_FULL);
        gstSrioMport[0].semMsgSend[uiLoop] = semBCreate (SEM_Q_FIFO,SEM_EMPTY);
    }
    
    rioMsgInbInit(0,(UINT32)(gstSrioMport[0].pucMsgMem),hook);
    rioMsgInbInit(1,(UINT32)(gstSrioMport[0].pucMsgMem + SRIO_MSG_INB_SIZE),hook);
    
    intConnect((VOIDFUNCPTR *)FSL_SR_OUT_MU1_INT_VEC,(VOIDFUNCPTR)rioSendMsgISR,0);
	intEnable(FSL_SR_OUT_MU1_INT_VEC);
    intConnect((VOIDFUNCPTR *)FSL_SR_OUT_MU2_INT_VEC,(VOIDFUNCPTR)rioSendMsgISR,1);
	intEnable(FSL_SR_OUT_MU2_INT_VEC);

    return OK;
}

void rioRecvMsgHandle(UINT32 uiUnit,msgHook hook)
{
	UINT32 baseAddr = 0;
    UINT32 uiRecvAddr = 0;
    UINT32 uiIrqStatus = 0;
    UINT32 uiDlyTime = 0;
    UINT32 uiQueue = 0;
    UINT32 uiDeQueue = 0;
    UINT32 uiVal = 0;

	baseAddr = SRIO_BASE_ADDR + 0x100 * uiUnit;
	uiRecvAddr = FSL_READ_REG(baseAddr + M8641RIO_IFQDPAR);
	uiIrqStatus = FSL_READ_REG(baseAddr + M8641RIO_ISR);
    FSL_WRITE_REG(baseAddr + M8641RIO_ISR,uiIrqStatus);
    uiQueue = FSL_READ_REG(baseAddr + M8641RIO_IFQDPAR);
    uiDeQueue = FSL_READ_REG(baseAddr + M8641RIO_IFQEPAR);
    
    while(uiQueue != uiDeQueue)
    {
        uiVal = FSL_READ_REG(baseAddr + M8641RIO_ISR);
        if((uiVal >> 2) & 0x01)
        {
            SRIO_LOG(SRIO_DBG_INFO,"message unit %d busy\n",uiUnit,0,0,0,0);
            break;
        }
        if(NULL != hook)
        {
            hook(uiUnit,(UINT8*)uiQueue,(UINT16)SRIO_MSG_FRM_SIZE);
        }
        
        uiVal = FSL_READ_REG(baseAddr + M8641RIO_IMR);
        uiVal |= 2;
        FSL_WRITE_REG(baseAddr + M8641RIO_IMR,uiVal);
        uiVal = FSL_READ_REG(baseAddr + M8641RIO_IMR);
        while((uiVal & 2) != 0)
        {
            if(0x10000 == uiDlyTime)
            {
                SRIO_LOG(SRIO_DBG_ERR,"receive message timeout\n",0,0,0,0,0);
                intDisable(FSL_SR_IN_MU1_INT_VEC + uiUnit * 2);
                break;
            }
            uiDlyTime ++;
        }
        uiQueue = FSL_READ_REG(baseAddr + M8641RIO_IFQDPAR);
        uiDeQueue = FSL_READ_REG(baseAddr + M8641RIO_IFQEPAR);
    }
}

void rioRecvMsgISR(msgHook hook)
{
    rioRecvMsgHandle(0,hook);
}

void rioRecvMsg1ISR(msgHook hook)
{
	rioRecvMsgHandle(1,hook);
}

LOCAL STATUS rioSendMsgDirectInit(UINT8 unit)
{
	UINT32 timeOut = 0x20;
    UINT32 baseAddr = SRIO_BASE_ADDR + 0x100 * unit;
	UINT32 uiVal = 0;
	
	while(timeOut)
	{
	    uiVal = FSL_READ_REG(baseAddr + M8641RIO_OSR);
		if(uiVal & 0x01)
		{
            timeOut--;
        }
		else 
        {
            break;
        }
	}
	if(0 == timeOut)  
	{
        SRIO_LOG(SRIO_DBG_ERR,"send message failed\n",0,0,0,0,0);
		return ERROR;
	}

	uiVal = FSL_READ_REG(baseAddr + M8641RIO_OMR);
	uiVal &= (~(1<<0));
	FSL_WRITE_REG(baseAddr + M8641RIO_OMR,uiVal);
	
	uiVal = FSL_READ_REG(baseAddr + M8641RIO_OSR);
	uiVal |= ((1<<12)|(1<<11)|(1<<10)|(1<<7)|(1<<5)|(1<<4)|(1<<1)|(1<<0));
	FSL_WRITE_REG(baseAddr + M8641RIO_OSR,uiVal);
	
	uiVal = FSL_READ_REG(baseAddr + M8641RIO_OMR);
	uiVal |= ((1<<20)|(0x0a<<12)|(1<<2));
	FSL_WRITE_REG(baseAddr + M8641RIO_OMR,uiVal);

	uiVal = FSL_READ_REG(baseAddr + M8641RIO_ODATR);
	uiVal |= (1<<29);
	FSL_WRITE_REG(baseAddr + M8641RIO_ODATR,uiVal);
	FSL_WRITE_REG(baseAddr + M8641RIO_OMRETCR,2); 

	return OK;
}

void rioSendMsgISR(UINT8 mailBox)
{
	UINT32	baseAddr = 0;
	UINT32	uiStatus = 0;
	UINT32  uiVal = 0;

    baseAddr = SRIO_BASE_ADDR + 0x100 * mailBox;

    uiVal = FSL_READ_REG(baseAddr + M8641RIO_ODATR);
	uiVal &=(~(1<<29));
	FSL_WRITE_REG(baseAddr + M8641RIO_ODATR,uiVal);
	
	uiStatus = FSL_READ_REG(baseAddr + M8641RIO_OSR);
    if(SRIO_OSR_EODI == (uiStatus & SRIO_OSR_EODI))
    {
        semGive(gstSrioMport[0].semMsgSend[mailBox]);
    }
    uiVal = FSL_READ_REG(baseAddr + M8641RIO_OSR);
	uiVal |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
	FSL_WRITE_REG(baseAddr + M8641RIO_OSR,uiVal);

	uiVal = FSL_READ_REG(baseAddr + M8641RIO_ODATR);
	uiVal |=(1<<29);
	FSL_WRITE_REG(baseAddr + M8641RIO_ODATR,uiVal);
}

VOID rioSendMsg(UINT8 localPort,UINT8 mailBox,UINT8 dstID,UINT8 *msgPtr,UINT16 len)
{
    UINT32 uiBaseAddr = 0;
    UINT32 uiSendAddr = 0;
    UINT8 *pucSendBuf = NULL;
    UINT32 uiVal = 0;

    assert(mailBox < SRIO_MAX_MBOX);
    
    if(len > SRIO_MSG_FRM_SIZE)
    {
        SRIO_LOG(SRIO_DBG_ERR,"the len should smaller than %d\n",
                 SRIO_MSG_FRM_SIZE,0,0,0,0);
        return;
    }
    
    semTake(gstSrioMport[0].semMsgAccess[mailBox],WAIT_FOREVER);
    pucSendBuf = gstSrioMport[0].pucMsgOubMem;
    pucSendBuf = pucSendBuf + (mailBox * SRIO_MSG_OUB_SIZE);
    memcpy(pucSendBuf,msgPtr,len);
    uiBaseAddr = SRIO_BASE_ADDR + 0x100 * mailBox;
    uiSendAddr = (UINT32)pucSendBuf;
    if(OK != rioSendMsgDirectInit(mailBox))
    {
        goto ErrExit;
    }
    uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OMR);
	uiVal &= (~(1<<0));
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMR,uiVal);
	
    uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OSR);
    if((uiVal >> 20) & 0x01) 
	{
        SRIO_LOG(SRIO_DBG_ERR,"mailbox %d queue is full\n",mailBox,0,0,0,0);
		goto ErrExit;
	}

	uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OMR);
	uiVal |= (1<<2);
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMR,uiVal);
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OSAR,uiSendAddr | 0x04);
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_ODPR,(dstID << 16) | mailBox);
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_ODCR,SRIO_MSG_FRM_SIZE);
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMMGR,0);
    FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMMLR,0);

    uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_ODATR);
	uiVal &= (~(1<<31));
	FSL_WRITE_REG(uiBaseAddr + M8641RIO_ODATR,uiVal);
    
    /*
     *start
    */
    uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OMR);
    uiVal |= 0x01;
    FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMR,uiVal);

    if(ERROR == semTake(gstSrioMport[0].semMsgSend[mailBox],60))
    {
        SRIO_LOG(SRIO_DBG_ERR,"wait send message failed\n",0,0,0,0,0);
        uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OMR);
    	uiVal &= (~(1<<0));	
    	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OMR,uiVal);
	
        uiVal = FSL_READ_REG(uiBaseAddr + M8641RIO_OSR);
    	uiVal |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)| (1<<10)|(1<<11)|(1<<11));
    	FSL_WRITE_REG(uiBaseAddr + M8641RIO_OSR,uiVal);
 
        goto ErrExit;
    }
    
ErrExit:
    semGive(gstSrioMport[0].semMsgAccess[mailBox]);
    return ;
}

STATUS rioGetCIRQSize(UINT32 DBQueueSize,UINT32 *puiCIRQSize)
{
    UINT32 uiData = 0;
    UINT32 uiCIRQSize = 0;
    
	if(DBQueueSize < 16)
    {
        return ERROR;
    }

    uiData = DBQueueSize / 8;
    
	if(DBQueueSize > 2048)
    {
        uiData = 2048;
    }
    
	uiCIRQSize = 0;
    
	while(uiData)
	{
		uiData = uiData/2;
		uiCIRQSize++;
	}
    
	uiCIRQSize -= 2;
    *puiCIRQSize = uiCIRQSize;

    return OK;
}

void rioRecvDBISR(pfHook hook)
{
    UINT32  uiDeQueuePointer = 0;
    UINT32  uiModeReg =0;
    UINT32  uiVal = 0;
    UINT32  uiSrc = 0;
    UINT32  uiDest = 0;

    uiModeReg = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR));
    uiModeReg = uiModeReg & (~((1<<8)|(1<<6)|(1<<5)));
    FSL_WRITE_REG(((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR)),uiModeReg);
    /*
     *clear interrupt status register
    */
    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDSR));
    uiVal |=  (1 << 7) | (1 << 4) | (1 << 0);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + SRIO_DOORBELL_IDSR)),uiVal);  
    
    uiDeQueuePointer = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDQDPAR));
    uiDest = FSL_READ_REG(uiDeQueuePointer) & 0xffff;
    uiVal = FSL_READ_REG(uiDeQueuePointer + 4);
    uiSrc = (uiVal >> 16) & 0xffff;
    uiVal = uiVal & 0xffff;
	printf("receive db data \n");
    if(NULL != hook)
    {
        hook(uiSrc,uiDest,uiVal);
    }

    /*
     *enable doorbell
    */
    uiModeReg = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR));
    uiModeReg |= (1 << 1);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR)),uiModeReg);

    uiModeReg = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR));
    uiModeReg |= ((1<<8)|(1<<6)|(1<<5));
    FSL_WRITE_REG(((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR)),uiModeReg);
}


void rioDBInbInit(UINT32 DBStartAddr,UINT32 DBSize,pfHook hook)
{
    UINT32  uiData = 0;
    UINT32  uiModeReg = 0;
    UINT32  uiCIRQSize = 0;

    if(ERROR == rioGetCIRQSize(DBSize,&uiCIRQSize))
    {
        SRIO_LOG(SRIO_DBG_ERR,"doorbell size %x too small \n",0,0,0,0,0);
        return;
    }
    
    /*
     *set mode register
    */
    uiModeReg = (0 << 28) | (1 << 20) | (uiCIRQSize << 12);
    uiModeReg |= (1 << 8) | (1 << 6) | (1 << 5);
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR),uiModeReg);

    /*
     *write queue dequeue and enqueue pointer address register
    */
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDQDPAR),DBStartAddr);
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDQEPAR),DBStartAddr);

    /*
     *clear interrupt status
    */
    uiData = FSL_READ_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDSR));
    uiData |=  (1 << 7) | (1 << 4) | (1 << 0);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + SRIO_DOORBELL_IDSR)),uiData);
    
    /*
     * doorbell enable  
    */
    uiModeReg |= 1;
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_IDMR),uiModeReg);

    /*
     *set max interrupt interval
    */
    FSL_WRITE_REG((SRIO_BASE_ADDR + SRIO_DOORBELL_MIRI),0x100);
    
    /*
     *connect interrupt 
    */
    intConnect((VOIDFUNCPTR *)FSL_SR_IN_DB1_INT_VEC,rioRecvDBISR,(int)hook);
	intEnable(FSL_SR_IN_DB1_INT_VEC);
}

STATUS rioDBOutInit(void)
{
	UINT32 uiTimeOut = 0x10;
	UINT32 uiVal = 0;

	
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODMR)),0);
    
	while(uiTimeOut)
	{
	    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
		if(uiVal & 0x02)   
        {
            uiTimeOut--;
        }
		else 
		{
		    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
            uiVal |= ((1<<12) | (1<<11) | (1<<10)| (1<<1));
            FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODSR)),uiVal);
			break;
		}
	}
	if(0 == uiTimeOut)  
	{
        SRIO_LOG(SRIO_DBG_ERR,"doorbell init failed\n",0,0,0,0,0);
		return ERROR;
	}
	FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODRETCR)),3);
	FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),0);
	
	uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODDATR));
    uiVal |= (1<<29);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),uiVal);

	intConnect((VOIDFUNCPTR *)FSL_SR_OUT_DB1_INT_VEC,rioSendDBISR,0);
	intEnable(FSL_SR_OUT_DB1_INT_VEC);
	
	return OK;
}

void rioSendDBISR(void)
{
    UINT32 uiVal = 0;

    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODDATR));
    uiVal &= (~(1<<29));
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),uiVal);

    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
	if(uiVal & 0x02)
	{
        semGive(gstSrioMport[0].semDbSend);
	}

	uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
    uiVal |= ((1<<12)|(1<<11)|(1<<10)|(1<<1));
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODSR)),uiVal);
  
    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODDATR));
    uiVal |= (1<<29);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),uiVal); 
}

STATUS rioInitDoorbell(pfHook hook)
{
    gstSrioMport[0].puiDBMem = malloc(SRIO_DOORBELL_INB_SIZE * 8);
    if(NULL == gstSrioMport[0].puiDBMem)
    {
        SRIO_LOG(SRIO_DBG_ERR,"malloc msg memory failed\n",0,0,0,0,0);
        return ERROR;
    }
    memset(gstSrioMport[0].puiDBMem,0,(SRIO_DOORBELL_INB_SIZE * 8));
    
	if(ERROR == rioDBOutInit())
	{
	    SRIO_LOG(SRIO_DBG_ERR,"rioDBOutInit error\n",0,0,0,0,0);
		return ERROR;
	}
	
	rioDBInbInit((UINT32)(gstSrioMport[0].puiDBMem),SRIO_DOORBELL_INB_SIZE,hook);
	gstSrioMport[0].semDbSend = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
    gstSrioMport[0].semDbAccess = semBCreate (SEM_Q_FIFO, SEM_FULL);
	
	return OK;	
}

VOID rioSendDoorbell(UINT8 localport,UINT8 targetID,UINT16 DBData)
{
	UINT32 uiTimeOut = 0x100;
	UINT32 uiVal = 0;

    semTake(gstSrioMport[0].semDbAccess,WAIT_FOREVER);
	while(uiTimeOut--)
	{
	    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
		if(0 == (uiVal & 0x04))
		{
		    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODSR));
            uiVal |= ((1<<1)|(1<<10)|(1<<11)|(1<<12));
            FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODSR)),uiVal);
			break;
		}
	}

    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODMR)),0);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDPR)),targetID << 16);
	
	uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODDATR));
    uiVal &= (~(0xffff));
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),uiVal);
            
    uiVal = FSL_READ_REG((SRIO_BASE_ADDR + M8641RIO_ODDATR));
    uiVal |= DBData;
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODDATR)),uiVal);
    FSL_WRITE_REG(((SRIO_BASE_ADDR + M8641RIO_ODMR)),1);
    
	if(ERROR == semTake(gstSrioMport[0].semDbSend,60))
	{
	    SRIO_LOG(SRIO_DBG_ERR,"send doorbell failed\n",0,0,0,0,0);
	}
	semGive(gstSrioMport[0].semDbAccess);	
}

VOID  rioSetOWSegment(VOID)
{
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,2,1)=0x4<<20|0x4<<16|(0x11);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,2,2)=0x4<<20|0x4<<16|(0x12);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,2,3)=0x4<<20|0x4<<16|(0x13);
	
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,3,1)=0x4<<20|0x4<<16|(0x15);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,3,2)=0x4<<20|0x4<<16|(0x16);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,3,3)=0x4<<20|0x4<<16|(0x17);
	
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,4,1)=0x4<<20|0x4<<16|(0x19);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,4,2)=0x4<<20|0x4<<16|(0x1a);
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,4,3)=0x4<<20|0x4<<16|(0x1b);	
	
	*M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,5,1)=0x4<<20|0x4<<16|(0x1d);
    *M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,5,2)=0x4<<20|0x4<<16|(0x1e);
    *M8641RIO_SEGMENT_SET(SRIO_BASE_ADDR,5,3)=0x4<<20|0x4<<16|(0x1f);
}

VOID  rioSetSrioOW(VOID)
{
	UINT32  uiOub = 0;
    UINT32  uiBaseAddr = 0;
    UINT32  uiLoop = 0;
    UINT32  uiSeq = 0;

    gstSrioMport[0].uiOWBitmap = 0xf;
    
    uiOub = 0;
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c00 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,((SRIO_TRAN_ADDR >> 12) | (0x10) << 22));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c08 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(0xa0000000 >> 12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c10 + 0x40 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,0x8084401a);

	uiOub = 1;
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c00 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,((SRIO_TRAN_ADDR >> 12) | (0x14) << 22));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c08 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(0xa8000000>>12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c10 + 0x40 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,0x8084401a);

	uiOub = 2;
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c00 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,((SRIO_TRAN_ADDR >> 12) | (0x18) << 22));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c08 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(0xb0000000>>12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c10 + 0x40 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,0x8084401a);

	uiOub = 3;
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c00 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,((SRIO_TRAN_ADDR >> 12) | (0x1c) << 22));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c08 + 0x40 + uiOub * 0x20;
    FSL_WRITE_REG(uiBaseAddr,(0xb8000000>>12));
    uiBaseAddr = SRIO_BASE_ADDR + 0xd0c10 + 0x40 + uiOub * 0x20;
	FSL_WRITE_REG(uiBaseAddr,0x8084401a);
	
    rioSetOWSegment();
    for(uiLoop = 0;uiLoop < 16;uiLoop++)
    {
        uiSeq = gstSrioMport[0].uiOWWin;
        if(uiSeq < SRIO_MAX_OUB)
        {
            gstSrioMport[0].astOW[uiSeq].uiDevId = 0x10 + uiLoop;
            gstSrioMport[0].astOW[uiSeq].uiMemLoc = 
                                                uiLoop * 0x2000000 + 0xa0000000;
            gstSrioMport[0].astOW[uiSeq].uiMemTran = SRIO_TRAN_ADDR;
            gstSrioMport[0].astOW[uiSeq].uiSize = 32;
        }
        
        gstSrioMport[0].uiOWWin++;
    }
}

UINT32 rioGetLocalPort(VOID)
{
    UINT32 uiPort = 0;
    
    if(0 == (FSL_READ_REG((SRIO_BASE_ADDR + 0xe0020)) & 0xf))
    {
        uiPort = 2; 
    }
    else
    {
        uiPort = 0; 
    }

    return uiPort;
}

VOID rioCloseRapidIO(VOID)
{
    FSL_WRITE_REG((SRIO_BASE_ADDR + 0xe0f44),0xff000000);
    FSL_WRITE_REG((SRIO_BASE_ADDR + 0xc0060),0xffffff);
    FSL_WRITE_REG((SRIO_BASE_ADDR + 0xc0148),0);
}

VOID rioRstHostId(VOID)
{
    UINT32 uiHost = 0;
    
    uiHost = FSL_READ_REG(SRIO_BASE_ADDR + SRIO_GCCSR);

    if(0x0 != (uiHost & (1 << 31)))
    {
        FSL_WRITE_REG((SRIO_BASE_ADDR + 0xc0060),0xffffff);
    }
}

VOID rioOpenRapidIO(VOID)
{
    FSL_WRITE_REG((SRIO_BASE_ADDR + 0xe0f44),0xf0000000);
}




#if 0
void rioPrintfSrioMportInfo()
{
	UINT32 uiLoop = 0;

	for (uiLoop=0; uiLoop<8; uiLoop++)
	{
		sysPrintf("gstSrioMport[0].astOW[%d].uiMemLoc: 0x%x, tran: 0x%x, id: %d, size: 0x%x\n",
			uiLoop,
			gstSrioMport[0].astOW[uiLoop].uiMemLoc, gstSrioMport[0].astOW[uiLoop].uiMemTran,
			gstSrioMport[0].astOW[uiLoop].uiDevId, gstSrioMport[0].astOW[uiLoop].uiSize);
	}
}
#endif

