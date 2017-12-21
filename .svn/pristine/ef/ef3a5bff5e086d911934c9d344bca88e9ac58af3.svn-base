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
extern UINT32 guiDbgLevel;
extern SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern STATUS rioAcquireDevLock(UINT32 ,UINT32 ,UINT32 ,UINT32 ,UINT32 *);
extern VOID rioSetMaintenanceWin(UINT32 LocalAddr,UINT8 hopCount,UINT8 targetId);
/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
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
    
    return 5;
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

VOID rioSetAgentMode()
{
    UINT32 uiHost = 0;
	
	uiHost = FSL_READ_REG(CCSBAR + SRIO_GCCSR);
    if(0x0 != (uiHost & (1 << 31)))
    {
        FSL_WRITE_REG((CCSBAR + 0xc0060),0xffffff);
    }
}



