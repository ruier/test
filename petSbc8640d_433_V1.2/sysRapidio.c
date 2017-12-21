/*include*/

#include <vxWorks.h>
#include <vxLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <vxBusLib.h>
#include <logLib.h>			/* for logMsg() */
#include "hwif/util/vxbParamSys.h"
#include "wrSbc8641.h"
#include "m8641Srio.h"
#include "bspBridgeDma8641.h"
#include "./rapidIO/sysRapidIO.h"

#define RAPIDIO_BASE   (CCSBAR + RAPIDIO_REG_BASE)

#define RIO_RMU_DBG_ON
#undef RIO_RMU_DBG_ON

#ifdef RIO_RMU_DBG_ON
#define RIO_RMU_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)
#undef LOCAL
#define LOCAL 

#else /* RIO_RMU_DBG_ON */
#define RIO_RMU_DBG_MSG(fmt,a,b,c,d,e,f)
#endif /* RIO_RMU_DBG_ON */

#define INCLUDE_RIO_MSGIN_INT 1
#undef INCLUDE_RIO_MSGIN_INT


SEM_ID rioMsgOutIntSetupSem,rioMsgOutAccessSem[2],rioMsgInAccessSem[2],rioMsgSendEndSem[2];
SEM_ID rioDBOutIntSetupSem,rioDoorbellSendEndSem,rioDBInIntSetupSem,rioDBInAccessSem,rioDBOutAccessSem;
SEM_ID rioDoorbellrecSem;

STATUS rioMsgOutIntInit[2]={0,0},rioMsgQInit=0,rioDBInIntInit=0,rioDBInit=0;
UINT8 rioMsgOutIntStatus=0,rioDBOutIntStatus=0;
UINT32 count=0;

int (*rioDoorbellRevInt)(UINT16 doorbellInfo,UINT16 srcID)=NULL;

#if(INCLUDE_RIO_MSGIN_INT)
int (*rioMsgInInt[2])(UINT16 data)={NULL,NULL};
#endif

#if 0
LOCAL void rioMsgOutIntHandle();
LOCAL void rioMsgOutIntHandle1();

/*LOCAL  void rioErrorIntHandle();*/

LOCAL STATUS rioSendMsgDirectInit(UINT8 unit);
LOCAL STATUS rioSendMsgChainInit(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl,UINT32 descriptorAddr);
LOCAL STATUS rioSendMsgchainAdd(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl);
LOCAL STATUS rioSendMsgchainStart(UINT8 unit);

STATUS rioDoorbellOutInit(void);
#endif

RIO_ID rioGetID(void)
{
	RIO_ID rioID = RIO_FF_ID;
#ifdef RIO_ID_16
	rioID=(*(UINT32*)(CCSBAR+0xc0060))&RIO_FF_ID;
#else
	rioID=(*(UINT32*)(CCSBAR+0xc0060)>>16)&RIO_FF_ID;
#endif //RIO_ID_16
	return (rioID);
}

STATUS sysRioSetIW(UINT8 unit,UINT32 translationAddr,UINT32 baseAddr,UINT8 windowSize)/*unit(1--3) windowSize( 2^(N+1),smallest 4K */
{
	UINT32 regAddr,tryTime,regData;
	
	if(unit>4)
	{
		unit=4;
	}
	else
	{
		if(unit==0)
		{
			unit=1;
		}
	}
	regAddr=RAPIDIO_BASE+0x20*(4-unit);
	
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_RIWTAR(regAddr) = (translationAddr>>12); 
		regData = *M8641RIO_RIWTAR(regAddr);
		regData &= 0xfffff;
		if(regData == ((translationAddr>>12)&0xfffff))
		{
			break;
		}
	}
	if(tryTime==0)
	{
		return ERROR;
	}
	
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_RIWBAR(regAddr)=(baseAddr>>12); 
		regData=*M8641RIO_RIWBAR(regAddr);
		regData &= 0xfffff;
		if(regData == ((baseAddr>>12)&0xfffff))
		{
			break;
		}
	}
	
	if(tryTime==0) 
	{
		return ERROR;
	}
	
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_RIWAR(regAddr) = 0x80f55000;
		regData=*M8641RIO_RIWBAR(regAddr);
		if(regData == 0x80f55000) break;
	}
	if(tryTime==0)
	{
		return ERROR;
	}


	if(windowSize<0xb)
	{
		windowSize=0xb;
	}
	else
	{
		if(windowSize>0x21)
		{
			windowSize=0x21;
		}
	}

	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_RIWAR(regAddr) |= windowSize;
		regData=*M8641RIO_RIWBAR(regAddr);
		regData &=0x3f;
		if(regData == windowSize) 
		{
			break;
		}
	}
	if(tryTime==0) 
	{
		return ERROR;
	}
	return OK;
	
}




STATUS sysRioSetOW(UINT8 unit,RIO_ID targetId,UINT32 translationAddr,UINT32 baseAddr,UINT8 windowSize)/*unit(1--3) windowSize( 2^(N+1),smallest 4K */
{
	UINT32 regAddr,tryTime,regData;
	
	if(unit>8)
	{
		unit=8;
	}
	else
	{
		if(unit==0)
		{
			unit=1;
		}
	}

	regAddr=RAPIDIO_BASE+0x20*(unit-1);
	/*if(((*(unsigned int*)(CCSBAR+0xe0004))&0x00060000)==0x00060000)*/

	tryTime=4;
	while(tryTime--)
	{
#ifdef RIO_ID_16
		*M8641RIO_ROWTAR(regAddr) = (translationAddr>>12)|((targetId&0x03ff)<<22);
		*M8641RIO_ROWTEAR(regAddr) = (targetId&0xfc00) >> 10;
#else
		*M8641RIO_ROWTAR(regAddr) = (translationAddr>>12)|(targetId<<22);
#endif //RIO_ID_16
		regData = *M8641RIO_ROWTAR(regAddr);
		printf("M8641RIO_ROWTAR read data is 0x%x\n",regData);
		regData &= 0xfffff;
		if(regData == (((translationAddr>>12)|((targetId&0x03ff)<<22))&0xfffff))
		{
			break;
		}
	}
	if(tryTime==0)
	{
		return ERROR;
	}


	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWBAR(regAddr)=(baseAddr>>12); 
		regData=*M8641RIO_ROWBAR(regAddr);
		regData &= 0xfffff;
		if(regData == ((baseAddr>>12)&0xfffff)) 
		{
			break;
		}
	}
	if(tryTime==0)
	{
		return ERROR;
	}

	/* set the outbound window to NREAD/NWRITE mode */
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWAR(regAddr) = 0x80044000;
		regData=*M8641RIO_RIWBAR(regAddr);
		if(regData == 0x80044000) break;
	}
	if(tryTime==0)
	{
		return ERROR;
	}

	if(windowSize<0xb)
	{
		windowSize=0xb;
	}
	else
	{
		if(windowSize>0x21)
		{
			windowSize=0x21;
		}
	}

	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWAR(regAddr) |= windowSize;
		regData=*M8641RIO_ROWBAR(regAddr);
		regData &=0x3f;
		if(regData == windowSize) 
		{
			break;
		}
	}
	if(tryTime==0)
	{
		return ERROR;
	}
	return OK;

}


STATUS sysRioSetOWWithSubSeg(UINT8 unit,RIO_ID targetId,UINT32 translationAddr,\
		UINT32 baseAddr,UINT8 windowSize,UINT8 segNum,UINT8 subWindNum,\
		RIO_ID targetID1,RIO_ID targetID2,RIO_ID targetID3)\
		/*unit(1--3) windowSize( 2^(N+1),smallest 4K */
{
	UINT32 regAddr,tryTime,regData;
	UINT8 subSegMode = 0;
	UINT8 segMode = 0;
	
	
	if(unit>8)
	{
		unit=8;
	}
	
	if(unit==0)
	{
		unit=1;
	}

	regAddr=RAPIDIO_BASE+0x20*(unit-1);
	/*if(((*(unsigned int*)(CCSBAR+0xe0004))&0x00060000)==0x00060000)*/
	switch(subWindNum)
	{
	case 0:
		subSegMode = 0;
		break;
	case 2:
		subSegMode = 1;
		break;
	case 4:
		subSegMode = 2;
		break;
	case 8:
		subSegMode = 3;
		break;
	default:
		printf("Set outbound with wrong subWindNum!\n");
		return ERROR;
	}

	switch(segNum)
	{
	case 0:
		segMode = 0;
		break;
	case 2:
		segMode = 1;
		break;
	case 4:
		segMode = 2;
		break;
	default:
		printf("Set outbound with wrong segNum!\n");
		return ERROR;	

	}

	tryTime=4;
	while(tryTime--)
	{
#ifdef RIO_ID_16
		*M8641RIO_ROWTAR(regAddr) = (translationAddr>>12)|((targetId&0x03ff)<<22);
		regData = *M8641RIO_ROWTAR(regAddr);
		*M8641RIO_ROWTEAR(regAddr) = (targetId&0xfc00) >> 10;
		regData = *M8641RIO_ROWTEAR(regAddr);
#else
		*M8641RIO_ROWTAR(regAddr) = (translationAddr>>12)|(targetId<<22);
		regData = *M8641RIO_ROWTAR(regAddr);
#endif //RIO_ID_16
		regData &= 0xfffff;
		if(regData == (((translationAddr>>12)|((targetId&0x03ff)<<22))&0xfffff)) break;
	}

	if(tryTime==0) 
	{
		return ERROR;
	}
	/* set the outbound window's base address */
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWBAR(regAddr)=(baseAddr>>12); 
		regData=*M8641RIO_ROWBAR(regAddr);
		regData &= 0xfffff;
		if(regData == ((baseAddr>>12)&0xfffff)) break;
	}
	if(tryTime==0)
	{
		return ERROR;
	}

	
	/* set the outbound window to NREAD/NWRITE mode */
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWAR(regAddr) = 0x80044000;
		regData=*M8641RIO_RIWBAR(regAddr);
		if(regData == 0x80044000) break;
	}
	if(tryTime==0)
	{
		return ERROR;
	}


	if(windowSize<0xb)
	{
		windowSize=0xb;
	}
	else
	{
		if(windowSize>0x21) 
		{
			windowSize=0x21;
		}
	}
	
	/* set the outbound window's segments、subsegments and window size */
	tryTime=4;
	while(tryTime--)
	{
		regData=*M8641RIO_ROWAR(regAddr);
		regData &= 0xff0fffff;
		regData = regData | (subSegMode<<20) | (segMode<<22) | windowSize;
		*M8641RIO_ROWAR(regAddr)  = regData;
		regData=*M8641RIO_ROWAR(regAddr);
		regData &=0x3f;
		if(regData == windowSize) 
		{
			break;
		}
	}
	if(tryTime==0)
	{
		return ERROR;
	}
	
	/* set segments' attribute */
	regAddr=RAPIDIO_BASE+0x20*(unit-1);
	switch(segMode)
	{
	case 2:
		tryTime=4;
		while(tryTime--)
		{
			regData=*M8641RIO_ROWS2R(regAddr);
			regData = (0x44<<16) | (targetID2&0xff);
			*M8641RIO_ROWS2R(regAddr)  = regData;
			regData=*M8641RIO_ROWS2R(regAddr);
			regData &=0x3f;
			if(regData == (targetID1&0xff)) 
			{
				break;
			}
		}
		
		if(tryTime==0)
		{
			return ERROR;
		}
		tryTime=4;
		while(tryTime--)
		{
			regData=*M8641RIO_ROWS3R(regAddr);
			regData = (0x44<<16) | (targetID3&0xff);
			*M8641RIO_ROWS3R(regAddr)  = regData;
			regData=*M8641RIO_ROWS3R(regAddr);
			regData &=0x3f;
			if(regData == (targetID1&0xff)) 
			{
				break;
			}
		}
		if(tryTime==0)
		{
			return ERROR;
		}
	case 1:
		tryTime=4;
		while(tryTime--)
		{
			regData=*M8641RIO_ROWS1R(regAddr);
			regData = (0x44<<16) | (targetID1&0xff);
			*M8641RIO_ROWS1R(regAddr)  = regData;
			regData=*M8641RIO_ROWS1R(regAddr);
			regData &=0x3f;
			if(regData == (targetID1&0xff)) 
			{
				break;
			}
		}
		if(tryTime==0)
		{
			return ERROR;
		}
		break;

	default:
		break;
	}
	return OK;

}




#if 0
STATUS sysRioSetOWMulID(UINT8 unit,UINT8 baseTargetId,UINT32 translationAddr,UINT32 baseAddr,UINT8 windowSize)/*unit(1--3) windowSize( 2^(N+1),smallest 4K */
{
	UINT32 regAddr,tryTime,regData;
	
	if(unit>8)unit=8;
	else if(unit==0) unit=1;
	regAddr=RAPIDIO_BASE+0x20*(unit-1);
	/*if(((*(unsigned int*)(CCSBAR+0xe0004))&0x00060000)==0x00060000)*/
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWTAR(regAddr) = (translationAddr>>12)|(targetId<<22); 
		regData = *M8641RIO_ROWTAR(regAddr);
		regData &= 0xfffff;
		if(regData == (((translationAddr>>12)|(targetId<<22))&0xfffff)) break;
	}
	if(tryTime==0) return ERROR;
	
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWBAR(regAddr)=(baseAddr>>12); 
		regData=*M8641RIO_ROWBAR(regAddr);
		regData &= 0xfffff;
		if(regData == ((baseAddr>>12)&0xfffff)) break;
	}
	if(tryTime==0) return ERROR;
	
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWAR(regAddr) = 0x80044000;
		regData=*M8641RIO_RIWBAR(regAddr);
		if(regData == 0x80044000) break;
	}
	if(tryTime==0) return ERROR;
	
	
	if(windowSize<0xb) windowSize=0xb;
	else if(windowSize>0x21) windowSize=0x21;
	tryTime=4;
	while(tryTime--)
	{
		*M8641RIO_ROWAR(regAddr) |= windowSize;
		regData=*M8641RIO_ROWBAR(regAddr);
		regData &=0x3f;
		if(regData == windowSize) break;
	}
	if(tryTime==0) return ERROR;
	return OK;
	
}

#endif

LOCAL void rioDBInIntHandle(void * param)
{
	UINT32 inboundStatus,IDQEPAR,baseAddr=RAPIDIO_BASE;
	UINT16 doorbellInfo;
	UINT16 srcID;
	
	*M8641RIO_IDMR(baseAddr) &=(~((1<<8)|(1<<6)|(1<<5)));
	inboundStatus = *M8641RIO_IDSR(baseAddr);
	/*count++;*/
	if(inboundStatus&0x01)
	{
		IDQEPAR=*M8641RIO_IDQDPAR(baseAddr);
		doorbellInfo=*(UINT32*)(IDQEPAR+4) & 0xffff;
		srcID =(*(UINT32*)(IDQEPAR+4)&0xffff0000)>>16;   //16位源ID|16位门铃信息
		/*
		*(UINT32*)0xe0000300=count;
		*(UINT32*)0xe0000304=doorbellInfo;
		*/
		if(rioDoorbellRevInt != NULL) (*rioDoorbellRevInt)(doorbellInfo,srcID);
		/*semGive(rioDoorbellrecSem);*/
		*M8641RIO_IDMR(baseAddr) |= 0x01<<1;
	}
	/*
	else if(inboundStatus&(~(1<<4)))
	{
		printf("Queue full interrupt.\n");		
	}
	else if(inboundStatus&(~(1<<7)))
	{
		printf("Transaction error(inbound).\n");		
	}	
	*/
	*M8641RIO_IDSR(baseAddr) |= ((1<<0)|(1<<4)|(1<<7));	
	*M8641RIO_IDMR(baseAddr) |= (1<<6);/*((1<<8)|(1<<6)|(1<<5));*/
}



LOCAL void rioDBOutIntHandle(void)
{
	UINT32 outboundStatus;
	UINT32 baseAddr=RAPIDIO_BASE;
	
	*M8641RIO_ODDATR(baseAddr) &= (~(1<<29)); 
	outboundStatus = *M8641RIO_ODSR(baseAddr);
	/*
	if(outboundStatus&(~(1<<12)|(1<<11)|(1<<10)))
	{
		printf("doorbell Outbound Status is 0x%x\n",outboundStatus);
	}
	if(!(outboundStatus&0x04)) 
	else printf("rioDBOutIntHandle:doorbell is busy\n");*/
	if(outboundStatus&0x02) semGive(rioDoorbellSendEndSem);
	*M8641RIO_ODSR(baseAddr) |= ((1<<12)|(1<<11)|(1<<10)|(1<<1));
	*M8641RIO_ODDATR(baseAddr) |= (1<<29);
}



LOCAL STATUS rioDBIntSetup(void)
{
	VXB_DEVICE_ID pInst;
	pInst=(VXB_DEVICE_ID)vxbInstByNameFind ("m85xxCPU", 0);
	if (pInst == NULL)
	{
		ns16550Pollprintf ("rioDBIntSetup:Get pInst failed!!!\n");
		return ERROR;
	}
	vxbIntConnect (pInst, 1, rioDBOutIntHandle, NULL);
	vxbIntEnable  (pInst, 1, rioDBOutIntHandle, NULL);
	vxbIntConnect (pInst, 0, rioDBInIntHandle, NULL);
	vxbIntEnable  (pInst, 0, rioDBInIntHandle, NULL);
	return OK;
}

/**********************doorbell out init****************************/
STATUS rioDBOutInit(void)
{
	UINT16 timeOut=0x10;
	UINT32 baseAddr=RAPIDIO_BASE;
	if(semTake(rioDBOutAccessSem,0x600)==ERROR)
	{
		printf("rioSendDoorbell:Doorbell is using by other\n");
		return ERROR;
	}
	*M8641RIO_ODMR(baseAddr) = 0;
	while(timeOut)
	{
		if(*M8641RIO_ODSR(baseAddr)&0x02)   timeOut--;
		else 
		{
			*M8641RIO_ODSR(baseAddr) |= ((1<<12)|(1<<11)|(1<<10)|(1<<1));
			break;
		}
	}
	if(timeOut==0)  
	{
		printf("rioDBOutInit Fail!!(Doorbell is busy )\n");
		semGive(rioDBOutAccessSem);
		return ERROR;
	}
	
	*M8641RIO_ODRETCR(baseAddr)=3;
	*M8641RIO_ODDATR(baseAddr)=0;
	*M8641RIO_ODDATR(baseAddr) |= ((1<<29)); /*enable End-of-Message interrupt*/
	semGive(rioDBOutAccessSem);
	return OK;
	
}

STATUS rioDoorBellInit(void)
{
	if(!rioDBInit)
	{
		rioDBInAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioDBOutAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioDoorbellSendEndSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		if(rioDBOutInit()==ERROR)
		{
			ns16550Pollprintf("rioDBOutInit error\n");
			return ERROR;
		}
		if(rioDBIntSetup()==ERROR) 
		{
			ns16550Pollprintf("rioDBIntSetup error\n");
			return ERROR;
		}		
		rioDBInit=1;
	}
	return OK;
	
	
}


STATUS rioDBInInit(UINT32 DBQueueStartAddr,UINT32 DBQueueSize,int(*intFuc)(UINT16,UINT16))
{
	UINT32 baseAddr=RAPIDIO_BASE;
	UINT8 CIRQ_Size,MIQ_Thresh;
	UINT16 tempData;
	/*VXB_DEVICE_ID pInst;*/
	
	if(semTake(rioDBInAccessSem,0x600)==ERROR)
	{
		printf("rioDBInInit:Doorbell is using by other\n");
		return ERROR;
	}
	
	*M8641RIO_IDMR(baseAddr) &= 0x0;
	*M8641RIO_IDSR(baseAddr) |= ((1<<0)|(1<<4)|(1<<7));/* Clear interrupt status */
	
	*M8641RIO_IDQDPAR(baseAddr) = DBQueueStartAddr;/* Point dequeue/enqueue pointers at first entry in ring */
	*M8641RIO_IDQEPAR(baseAddr)= DBQueueStartAddr;
	
	
	tempData= DBQueueSize;
	if(tempData<2) tempData=2;
	else if(tempData>2048) tempData=2048;
	CIRQ_Size=0;
	while(tempData)
	{
		tempData =tempData/2;
		CIRQ_Size++;
	}
	CIRQ_Size -= 2;
	MIQ_Thresh = 0;
	*M8641RIO_IDMR(baseAddr) |= ((1<<20)|(CIRQ_Size << 12)|(MIQ_Thresh << 28)|(1<<6))/*|(1<<5)|(1<<8))*/;
	*M8641RIO_IDMR(baseAddr)  |= 0x01;
	
	rioDoorbellRevInt=intFuc;
	
	semGive(rioDBInAccessSem);
	return OK;
}



/***************************DOORBELL OUT******************************/

STATUS rioSendDB(RIO_ID targetID,UINT16 DBData)
{
	UINT32 baseAddr=RAPIDIO_BASE;
	UINT32 /*outboundStatus,*/timeOut=0x100;

	if(semTake(rioDBOutAccessSem,0x600)==ERROR)
	{
		printf("rioSendDoorbell:Doorbell is using by other\n");
		return ERROR;		
	}
	while(timeOut--)
	{
		if(timeOut == 0)
		{
			printf("rioSendDB:doorBell is busy\n");
			semGive(rioDBOutAccessSem);
			return ERROR;
		}
		else if(!(*M8641RIO_ODSR(baseAddr) & 0x04))
		{
			*M8641RIO_ODSR(baseAddr)|=((1<<1)|(1<<10)|(1<<11)|(1<<12));
			break;
		}
	}

	*M8641RIO_ODMR(baseAddr) = 0;
	*M8641RIO_ODDPR(baseAddr) = targetID << 16;
	*M8641RIO_ODDATR(baseAddr) &= (~(0xffff));
	*M8641RIO_ODDATR(baseAddr) |= DBData;
	*M8641RIO_ODMR(baseAddr) = 1;/*start*/
	//while((*M8641RIO_ODSR(baseAddr)>>2)&0x01);
	//while(!((*M8641RIO_ODSR(baseAddr)>>1)&0x01));

	if(semTake(rioDoorbellSendEndSem, 0x600)==ERROR) /*WAIT_FOREVER*/
	{
		printf("rioDoorbellSendEndSem semTake timeOut!!!!!!!!\n");
		semGive(rioDBOutAccessSem);
		return ERROR;
	}	

	semGive(rioDBOutAccessSem);
	return 	 OK;
}


#if 1
int DBRecH(int bb)
{
	semGive(rioDoorbellrecSem);
	return OK;
}


void aabb()
{
	rioDoorbellrecSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
}
int testDBRec()
{
	if(rioDBInInit(0x30000000,0x2048,DBRecH)==ERROR) return ERROR;
	printf("befor semTake");
	while(1)
	{
	semTake(rioDoorbellrecSem, WAIT_FOREVER);
	rioSendDB(2,0x9876);
	}
}

int testDBSend(UINT32 time)
{
	STATUS cc;
	UINT32 startHi,startLo,i;
	UINT32 endHi,endLo;
	float timeSpend,timeSpend1;
	cc=rioDBInInit(0x30000000,0x2048,DBRecH);
	sysGetTime(&startHi,&startLo);
	for(i=0;i<time;i++)
	{
	rioSendDB(0,0x1234);
	semTake(rioDoorbellrecSem, WAIT_FOREVER); /*WAIT_FOREVER*/
	}
	sysGetTime(&endHi,&endLo);
	timeSpend= ((endHi-startHi)*1024*1024*1024*4.0+(endLo-startLo));
	timeSpend1=timeSpend*40/1000000000.0;
	printf("Door bell send and return time spend:%fs.\n",timeSpend1);
}

void timeTest()
{
	UINT32 startHi,startLo;
	UINT32 endHi,endLo;
	float timeSpend;
	sysGetTime(&startHi,&startLo);
	taskDelay(120);
	sysGetTime(&endHi,&endLo);
	timeSpend= ((endHi-startHi)*1024*1024*1024*4.0+(endLo-startLo))*40/1000000000;
	printf("Door bell send and return time spend:%fs.\n",timeSpend);
	
}
#endif


#if 0


void rioMsgQinit(void)
{			
	if(!rioMsgQInit)
	{
		rioMsgOutIntSetupSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioMsgOutAccessSem[0] = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioMsgOutAccessSem[1] = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioMsgInAccessSem[0] = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioMsgInAccessSem[1] = semBCreate (SEM_Q_FIFO, SEM_FULL);
	
		/*
		rioDBInIntSetupSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioDBOutIntSetupSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioDBInAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		rioDBOutAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
		
		rioDoorbellSendEndSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		*/
		rioMsgSendEndSem[0] = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		rioMsgSendEndSem[1] = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		
		

		rioMsgQInit=1;
		/*
		VXB_DEVICE_ID pInst;
		pInst=vxbInstByNameFind ("m85xxCPU", 0);
		if (pInst == NULL)
		{
			printf ("Get pInst failed\n");
			return;
		}		
		vxbIntConnect (pInst, 6, rioErrorIntHandle, NULL);
		vxbIntEnable  (pInst, 6, rioErrorIntHandle, NULL);
		*/
	}
}





/************************MSG IN INIT********************************/

/*be called before rev MSG*/

void rioInMsgInit(RIO_IN_MSG_CTRL *pRioInMsgCtrl)
{
	RIO_IN_MSG_CTRL *pInMsgCtrl=pRioInMsgCtrl; 
	UINT8 CIRQ_Size,FrameSize,MIQ_Thresh;
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(pInMsgCtrl->unit&0x01);
	UINT16 tempData=0;
	
	/*RIO_RMU_DBG_MSG("rioInMsgInit: SRIO reg baseAddr=0x%x\n",baseAddr,2,3,4,5,6);*/
	
	semTake(rioMsgInAccessSem[pInMsgCtrl->unit&0x01],WAIT_FOREVER);
	
	*M8641RIO_IMR(baseAddr) &=(~(1<<0));/*disable Inbound */
	/* Clear interrupt status */
	*M8641RIO_ISR(baseAddr) |= ((1<<0)|(1<<4)|(1<<7)|(1<<10));
	
	CIRQ_Size=FrameSize=MIQ_Thresh=0;
	tempData=pInMsgCtrl->MIQ_Thresh & 0xfff;
	if(tempData == 0) tempData=1;
	while(tempData) 
	{
		tempData=tempData/2;
		 MIQ_Thresh++;
	}
	MIQ_Thresh -= 1;
	
	tempData=pInMsgCtrl->FrameSize & 0xffff;
	if(tempData<8) tempData=8;
	while(tempData ) 
	{	
		tempData=tempData/2;
		FrameSize++;
	}
	FrameSize -= 2;
	
	tempData= (pInMsgCtrl->CIRQ_Size & 0xfff);
	if(tempData<2) tempData=2; 
	while(tempData )
	{
		tempData =tempData/2;
		CIRQ_Size++;
	}
	CIRQ_Size -= 2;
	
	*M8641RIO_IMR(baseAddr) &= 0x0;
	*M8641RIO_IMR(baseAddr) |= ((1<<20)|(CIRQ_Size << 12)|(FrameSize << 16)|(MIQ_Thresh << 28));
	 RIO_RMU_DBG_MSG("rioInMsgInit: M8641RIO_IMR = 0x%x\n",*M8641RIO_IMR(baseAddr),2,3,4,5,6);

	/* Point dequeue/enqueue pointers at first entry in ring */
	*M8641RIO_IFQDPAR(baseAddr) =pInMsgCtrl->localAddr;
	*M8641RIO_IFQEPAR(baseAddr)=pInMsgCtrl->localAddr;

	*M8641RIO_IMR(baseAddr) |= 0x01;/*enable Inbound */
	
	semGive(rioMsgInAccessSem[pInMsgCtrl->unit&0x01]);
}






/*revice MSG ,return: the count of MSG and  the local addr for save MSG*/

INT16 rioRevMsg(UINT8 unit,UINT32 *revMsgAddr)	
{
	UINT16 timeOut=5,frameCount=0;
	UINT32 baseAddr=RAPIDIO_BASE+0x100*unit;

	

	semTake(rioMsgInAccessSem[unit&0x01],WAIT_FOREVER);
	*revMsgAddr= *M8641RIO_IFQDPAR(baseAddr);
	RIO_RMU_DBG_MSG("rioRevMsg: revMsgAddr = 0x%x\n",*revMsgAddr,2,3,4,5,6);
	while(1)
	{
		while(timeOut)
		{
			if((*M8641RIO_ISR(baseAddr)>>2)&0x01)   timeOut--;
			else break;
		}
		if(0 == timeOut)  
		{
			semGive(rioMsgInAccessSem[unit&0x01]);
			return ERROR;
		}
		if(*M8641RIO_IFQDPAR(baseAddr) ==*M8641RIO_IFQEPAR(baseAddr)) 
		{
			/**M8641RIO_IMR(baseAddr) &= (~0x01);
			*M8641RIO_IMR(baseAddr) |= (0x01);*/
			semGive(rioMsgInAccessSem[unit&0x01]);
			return frameCount;
		}
		else
		{
		/*	addr=*M8641RIO_IFQDPAR(baseAddr);*/
			*M8641RIO_IMR(baseAddr) |= 0x01<<1;
			frameCount++;
		}
	}
/*	semGive(rioMsgInAccessSem[unit&0x01]);
	return OK;*/
}


STATUS rioSendMsgDirect(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl)/*pRioOutMsgCtrl->sendNum must be min 8B,and must be 2^n*/
{
	UINT16 timeOut=0x50;
	RIO_OUT_MSG_CTRL *pOutMsgCtrl=pRioOutMsgCtrl;	
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(pOutMsgCtrl->unit&0x01),tempData=0;/*,temp;*/
#ifdef RIO_RMU_DBG_ON
	static UINT32 count=0;
#endif
	
	semTake(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01],WAIT_FOREVER);
	
	rioSendMsgDirectInit(pOutMsgCtrl->unit&0x01);
	
	*M8641RIO_OMR(baseAddr) &= (~(1<<0));
	
	
	if((*M8641RIO_OSR(baseAddr)>>20)&0x01) 
	{
		printf("ERROR:the queue is full!!![rioSendMsgDirect()]\n");
		goto error;
	}
	
	*M8641RIO_OMR(baseAddr) |= (1<<2);/*direct mode*/
	
	*M8641RIO_OSAR(baseAddr) =pOutMsgCtrl->localAddr | 0x04; 
	*M8641RIO_ODPR(baseAddr) = (pOutMsgCtrl->targetId<<16)|(pOutMsgCtrl->mailbox&0x01);
#if 0
	if(pOutMsgCtrl->sendNum<8) pOutMsgCtrl->sendNum=0x8;
	else
	{
		temp=0x1000;
		while(1) 
		{
			
			if(pOutMsgCtrl->sendNum >= temp)
			{
				pOutMsgCtrl->sendNum=temp;
				break;
			}
			else temp>>=1;
		}	
	}
#endif
	/*RIO_RMU_DBG_MSG("pOutMsgCtrl->sendNum=%d\n",pOutMsgCtrl->sendNum,2,3,4,5,6);*/
	
	if(pOutMsgCtrl->MulticastEnable)
	{
		*M8641RIO_ODCR(baseAddr) = 256;/*((pOutMsgCtrl->sendNum)>256)?(256):(pOutMsgCtrl->sendNum);*/
		*M8641RIO_OMMGR(baseAddr)= (pOutMsgCtrl->MulticastGroup)&0x7;
		*M8641RIO_OMMLR(baseAddr)= pOutMsgCtrl->MulticastList;
		*M8641RIO_ODATR(baseAddr) |= (1<<31);
	}
	else
	{
		*M8641RIO_ODCR(baseAddr) =4096;/* ((pOutMsgCtrl->sendNum)>4096)?(4096):(pOutMsgCtrl->sendNum);*/
		*M8641RIO_OMMGR(baseAddr)= 0;
		*M8641RIO_OMMLR(baseAddr)= 0;
		*M8641RIO_ODATR(baseAddr) &= (~(1<<31));
		
	}
	*M8641RIO_OMR(baseAddr)  |= 0x01; /*start*/
#ifdef RIO_RMU_DBG_ON
	count++;
	*(UINT32*)(0xe0000720+(pOutMsgCtrl->unit &0x01) *0x30)=count;
#endif
	if(semTake(rioMsgSendEndSem[pOutMsgCtrl->unit&0x01],0x100/* WAIT_FOREVER*/)==ERROR) /*WAIT_FOREVER*/
	{
		printf("semTake timeOut!!!!!!!!\n");	
		RIO_RMU_DBG_MSG("*M8641RIO_OSR(baseAddr)= 0x%x\n",*M8641RIO_OSR(baseAddr),2,3,4,5,6);
		*M8641RIO_OMR(baseAddr) &= (~(1<<0));/*disable outbount control*/		
		*M8641RIO_OSR(baseAddr) |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
		goto error;
	}
	
#ifdef RIO_RMU_DBG_ON
	*(UINT32*)(0xe0000724+(pOutMsgCtrl->unit &0x01) *0x30)=count;
	*(UINT32*)(0xe0000728+(pOutMsgCtrl->unit &0x01) *0x30)=*M8641RIO_OSR(baseAddr);
#endif
	/*if(rioMsgOutIntStatus) goto error;
	else */
	{
		semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
		return OK;
	}
error:
	semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
	return ERROR;
}


STATUS rioSendMsgChain(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl,UINT32 descriptorAddr)
{
	RIO_OUT_MSG_CTRL *pOutMsgCtrl=pRioOutMsgCtrl;	
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(pOutMsgCtrl->unit&0x01);
	/*UINT32 temp;*/
	semTake(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01],WAIT_FOREVER);

	if(rioSendMsgChainInit(pOutMsgCtrl,descriptorAddr)==ERROR)
	{
		semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
		printf("rioSendMsgChainInit error!!!\n");
		return ERROR;
	}

	if(pOutMsgCtrl->MulticastEnable) 
	{
		if(pOutMsgCtrl->sendNum>256*2047) pOutMsgCtrl->sendNum=256*2047;
		else if(pOutMsgCtrl->sendNum<256) return ERROR;
		printf("pOutMsgCtrl->MulticastEnable!!\n");
	}
	else
	{
		if(pOutMsgCtrl->sendNum>4096*2047) pOutMsgCtrl->sendNum=4096*2047;
		else if(pOutMsgCtrl->sendNum<4096) return ERROR;
	
	}

#if 0
	if(pOutMsgCtrl->sendNum<8) pOutMsgCtrl->sendNum=0x8;
	else if(pOutMsgCtrl->sendNum>0x800000) pOutMsgCtrl->sendNum=0x800000;
	else
	{
		temp=0x1000;
		while(1) 
		{
			
			if(pOutMsgCtrl->sendNum%4096 >= temp)
			{
				pOutMsgCtrl->sendNum=(pOutMsgCtrl->sendNum/4096)*4096+temp;
				break;
			}
			else temp>>=1;
		}	
	}
#endif
	
	RIO_RMU_DBG_MSG("pOutMsgCtrl->sendNum=%d\n",pOutMsgCtrl->sendNum,2,3,4,5,6);
	
	if(rioSendMsgchainAdd(pOutMsgCtrl)==ERROR)
	{
		semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
		printf("rioSendMsgchainAdd error!!!\n");
		return ERROR;
	}

	if(rioSendMsgchainStart(pOutMsgCtrl->unit) == ERROR)
	{
		
		RIO_RMU_DBG_MSG("*M8641RIO_OSR(baseAddr)= 0x%x\n",*M8641RIO_OSR(baseAddr),2,3,4,5,6);
		*M8641RIO_OMR(baseAddr) &= (~(1<<0));/*disable outbount control*/		
		*M8641RIO_OSR(baseAddr) |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
		semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
		RIO_RMU_DBG_MSG("rioSendMsgchainStart error!!!\n",1,2,3,4,5,6);
		return ERROR;
	}
	else
	{
		semGive(rioMsgOutAccessSem[pOutMsgCtrl->unit&0x01]);
		return OK;
	}
	
}


LOCAL  void rioMsgOutIntHandle1();
LOCAL  void rioMsgOutIntHandle();

/*****************************MSG OUT INIT ***********************************/
LOCAL STATUS rioMsgOutIntSetup(UINT32 unit)
{
	VXB_DEVICE_ID pInst;
	semTake(rioMsgOutIntSetupSem,WAIT_FOREVER); /*rioMsgOutIntSetupSem*/
	if(!rioMsgOutIntInit[unit&0x01])
	{
		printf("rioMsgOutIntSetup unit=%d\n",unit&0x01);
	
		pInst=vxbInstByNameFind ("m85xxCPU", 0);
		if (pInst == NULL)
		{
			printf ("ERROR:Get pInst failed!!![rioMsgOutInit()]\n");
			semGive(rioMsgOutIntSetupSem);
			return ERROR;
		}
		if((unit&0x01)==0)
		{
			vxbIntConnect (pInst, 3, rioMsgOutIntHandle, NULL);
			vxbIntEnable  (pInst, 3, rioMsgOutIntHandle, NULL);	
		}
		else if((unit&0x01)==1)
		{
			vxbIntConnect (pInst, 5, rioMsgOutIntHandle1, NULL);
			vxbIntEnable  (pInst, 5, rioMsgOutIntHandle1, NULL);
		}
		rioMsgOutIntInit[unit&0x01]=1;
	}
	semGive(rioMsgOutIntSetupSem);
	return OK;
	
}




/***************************DOORBELL OUT INT******************************/






STATUS srioIntSetup(void)
{
	/*
	if(rioDBOutIntSetup()==ERROR) {ns16550Pollprintf("rioDBOutIntSetup error\n");return ERROR;}
	if(rioDBInIntSetup()==ERROR) {ns16550Pollprintf("rioDBInIntSetup error\n");return ERROR;}
	*/
	if(rioMsgOutIntSetup(0)==ERROR) {ns16550Pollprintf("rioMsgOutIntSetup(0) error\n");return ERROR;}
	if(rioMsgOutIntSetup(1)==ERROR) {ns16550Pollprintf("rioMsgOutIntSetup(1) error\n");return ERROR;}
	
#if 0
	if(rioDoorbellOutInit()==ERROR) return ERROR;
#endif
	
	return OK;
}

/*****************************MSG SEND (Direct)*init*********************************/

LOCAL STATUS rioSendMsgDirectInit(UINT8 unit)
{
	UINT32 timeOut=0x20,baseAddr=RAPIDIO_BASE+0x100*(unit&0x01);
	
	while(timeOut)
	{
		if((*M8641RIO_OSR(baseAddr)>>2)&0x01)   timeOut--;
		else break;
	}
	if(0 == timeOut)  
	{
		printf("ERROR:Message unit is busy!!![rioMsgOutInit()]!!\n");
		return ERROR;
	}
	
	*M8641RIO_OMR(baseAddr) &= (~(1<<0));	

	/* Clear interrupt status */
	*M8641RIO_OSR(baseAddr) |= ((1<<12)|(1<<11)|(1<<10)|(1<<7)|(1<<5)|(1<<4)|(1<<1)|(1<<0));
	*M8641RIO_OMR(baseAddr) |= ((1<<20)|(0x0a<<12)|/*(1<<5)|*/(1<<2));/* enable error interrupt*/
	/*RIO_RMU_DBG_MSG("rioSendMsgDirectInit: M8641RIO_OMR = 0x%x\n",*M8641RIO_OMR(baseAddr),2,3,4,5,6);*/
	*M8641RIO_ODATR(baseAddr) |= (1<<29); /*enable End-of-Message interrupt*/
	*M8641RIO_OMRETCR(baseAddr)=2;/*3;*/
#if 0	
	if(rioMsgOutIntSetup(unit) == ERROR) return ERROR;
#endif
	return OK;
	
}


/****************MSG Chain Mode Init*******************/

LOCAL STATUS rioSendMsgChainInit(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl,UINT32 descriptorAddr) /*descriptorAddr must be 32*2048-byte aligned.*/
{
	UINT16 timeOut=100;
	RIO_OUT_MSG_CTRL *pOutMsgCtrl=pRioOutMsgCtrl;
	
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(pOutMsgCtrl->unit&0x01);
	RIO_RMU_DBG_MSG("rioSendMsgchain--baseAddr=0x%x\n",baseAddr,2,3,4,5,6);
	
	/* wait until message Unit is not busy*/
	while(timeOut)
	{
		if((*M8641RIO_OSR(baseAddr)>>2)&0x01)   timeOut--;
		else break;
	}
	if(0 == timeOut)  
	{
		printf("rioSendMsgchainInit: timeout error!!\n");
		return ERROR;
	}
	
	*M8641RIO_OMR(baseAddr) &= (~((1<<0)|(1<<2)));
	*M8641RIO_OSR(baseAddr) |= ((1<<12)|(1<<11)|(1<<10)|(1<<7)|(1<<5)|(1<<4)|(1<<1)|(1<<0));/* Clear interrupt status */
	*M8641RIO_OMR(baseAddr) |= ((1<<20)|(0x0a<<12)/*|(1<<5)*/);/*all interrupt is enable*/
	RIO_RMU_DBG_MSG("rioSendMsgchainInit: M8641RIO_OMR = 0x%x\n",*M8641RIO_OMR(baseAddr),2,3,4,5,6);
	
	*M8641RIO_ODQDPAR(baseAddr) =descriptorAddr;
	*M8641RIO_ODQEPAR(baseAddr)=descriptorAddr;
	*M8641RIO_ODATR(baseAddr) &= (~(1<<29)); /*enable End-of-Message interrupt*/
	*M8641RIO_OMRETCR(baseAddr)=0;/*3;*/
#if 0
	rioMsgOutIntSetup(pOutMsgCtrl->unit&0x01);
#endif
	return 	 OK;
}


/***************************MSG CHAIN******************************/

LOCAL STATUS rioSendMsgchainAdd(RIO_OUT_MSG_CTRL *pRioOutMsgCtrl)
{
	UINT32 enqueueAddr,sendNo=0,sourceAddr,count=0;
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(pRioOutMsgCtrl->unit&0x01);
	RIO_OUT_MSG_CTRL *pOutMsgCtrl=pRioOutMsgCtrl;	
	STATUS intEnable=0;
	
	enqueueAddr=*M8641RIO_ODQEPAR(baseAddr);
	if(enqueueAddr==0) 
	{
		printf("rioSendMsgchainAdd:M8641RIO_ODQEPAR is not init!!\n");
		return ERROR;
	}
	sourceAddr=pRioOutMsgCtrl->localAddr;
	
	while(pOutMsgCtrl->sendNum)
	{
		if((*M8641RIO_OSR(baseAddr)>>20)&0x01) 
		{
			printf("rioSendMsgchainAdd:M8641RIO_OSR- - FULL!!\n");
			return ERROR;
		}
		*(UINT32*)(enqueueAddr)=0;
		*(UINT32*)(enqueueAddr+0x08)=(pOutMsgCtrl->targetId<<16)|(pOutMsgCtrl->mailbox&0x01);
		
		if(pRioOutMsgCtrl->MulticastEnable == 1) 
		{
			
			
			if(pOutMsgCtrl->sendNum>=256)
			{
				pOutMsgCtrl->sendNum -=256;
				sendNo=256;
				if(pOutMsgCtrl->sendNum<256)
				{
					intEnable=1;
					pOutMsgCtrl->sendNum=0;
				}
				else intEnable=0;
			}
			
			*(UINT32*)(enqueueAddr+0x0c) =(0x80000000 |intEnable<<29); 
			*(UINT32*)(enqueueAddr+0x10)=pRioOutMsgCtrl->MulticastGroup&0x07;
			*(UINT32*)(enqueueAddr+0x14)=pRioOutMsgCtrl->MulticastList;
		
		}
		else
		{
			if(pOutMsgCtrl->sendNum>=4096)
			{
				pOutMsgCtrl->sendNum -=4096;
				sendNo=4096;
				if(pOutMsgCtrl->sendNum<4096)
				{
					intEnable=1;
					pOutMsgCtrl->sendNum=0;
				}
				else intEnable=0;
			}
			
			*(UINT32*)(enqueueAddr+0x0c)=0;
			*(UINT32*)(enqueueAddr+0x0c) |= (intEnable<<29);
			*(UINT32*)(enqueueAddr+0x10)=0;
			*(UINT32*)(enqueueAddr+0x14)=0;
		}
		*(UINT32*)(enqueueAddr+0x04)=sourceAddr;
		*(UINT32*)(enqueueAddr+0x18)=sendNo;
		*(UINT32*)(enqueueAddr+0x1c)=0;	
		*M8641RIO_OMR(baseAddr) |= (0x1<<1);
		RIO_RMU_DBG_MSG("rioSendMsgchainAdd: sendNo= 0x%x ; sourceAddr = 0x%x ;enqueueAddr=0x%x \n",sendNo,sourceAddr,enqueueAddr,4,5,6);
		if(pRioOutMsgCtrl->MulticastEnable == 1) sourceAddr+=256;
		else sourceAddr+=4096;
		enqueueAddr +=32;
		count++;
	}
	printf("rioSendMsgchainAdd--send %d !!!\n",count);
	return OK;
}



LOCAL STATUS rioSendMsgchainStart(UINT8 unit)
{
	UINT32 baseAddr=RAPIDIO_BASE+0x100*(unit&0x01);
	*M8641RIO_OMR(baseAddr)  |= 0x01; /*start*/

	if(semTake(rioMsgSendEndSem[unit&0x01], 0x100/*WAIT_FOREVER*/)==ERROR) /*WAIT_FOREVER*/
	{
		printf("semTake timeOut!!!!!!!!\n");
		return ERROR;
	}
	/*if(rioMsgOutIntStatus) return ERROR;
	else */
		return OK;
}





/*******************interrupt sever fuction****************/

/*MSG*/
LOCAL void rioMsgOutIntHandle()
{
	UINT32	baseAddr=RAPIDIO_BASE,timeOut=5;
	UINT32	outboundStatus;
#ifdef RIO_RMU_DBG_ON
	static UINT32 count=0;
#endif
	/*disable all interrupt*/
	*M8641RIO_ODATR(baseAddr) &=(~(1<<29));
	/**M8641RIO_OMR(baseAddr) &= (~(1<<5));*/
	
	/*rioMsgOutIntStatus=0;*//*init rioMsgOutIntStatus*/
	
	outboundStatus = *M8641RIO_OSR(baseAddr);
#ifdef RIO_RMU_DBG_ON	
	*(UINT32*)0xe0000700=0x2200+count;
	*(UINT32*)0xe0000704=outboundStatus+(count<<16);
#endif

/*	if((outboundStatus>>12)&0x01)	rioMsgOutIntStatus |=(1<<0);
	if((outboundStatus>>11)&0x01)	rioMsgOutIntStatus |=(1<<1);
	if((outboundStatus>>10)&0x01)	rioMsgOutIntStatus |=(1<<2);
	if((outboundStatus>>7)&0x01)	rioMsgOutIntStatus |=(1<<3);

	while(timeOut)
	{
		if((*M8641RIO_OSR(baseAddr)>>2)&0x01)   timeOut--;
		else break;
	}
	*M8641RIO_OMR(baseAddr) &= (~(1<<0));
	*/
	
	semGive(rioMsgSendEndSem[0]);

	*M8641RIO_OSR(baseAddr) |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
	/* *M8641RIO_OSR(baseAddr) |= (1<<1);*/
	
#ifdef RIO_RMU_DBG_ON	
	outboundStatus = *M8641RIO_OSR(baseAddr);
	*(UINT32*)0xe000070c=outboundStatus;
	count++;
	*(UINT32*)0xe0000710=count;
	/**(UINT32*)0xe0000714=rioMsgOutIntStatus;*/
#endif
	
	*M8641RIO_ODATR(baseAddr) |=(1<<29);
	/**M8641RIO_OMR(baseAddr) |= (1<<5);*/
}



LOCAL  void rioMsgOutIntHandle1()
{
	UINT32 baseAddr=RAPIDIO_BASE+0x100,timeOut=5;
	UINT32 outboundStatus;
#ifdef RIO_RMU_DBG_ON
	static UINT32 count=0;
#endif
	
	*M8641RIO_ODATR(baseAddr) &=(~(1<<29));
	/**M8641RIO_OMR(baseAddr) &= (~(1<<5));*/
	
	outboundStatus = *M8641RIO_OSR(baseAddr);
	
#ifdef RIO_RMU_DBG_ON
	*(UINT32*)0xe0000730=0x12345678;
	*(UINT32*)0xe0000734=outboundStatus;
#endif
	/*	
	if((outboundStatus>>12)&0x01)	rioMsgOutIntStatus |=(1<<0);
	if((outboundStatus>>11)&0x01)	rioMsgOutIntStatus |=(1<<1);
	if((outboundStatus>>10)&0x01)	rioMsgOutIntStatus |=(1<<2);
	if((outboundStatus>>7)&0x01)	rioMsgOutIntStatus |=(1<<3);
	
	while(timeOut)
	{
		if((*M8641RIO_OSR(baseAddr)>>2)&0x01)   timeOut--;
		else break;
	}
	
	if(rioMsgOutIntStatus) *M8641RIO_OMR(baseAddr) &= (~(1<<0));
	*/
	semGive(rioMsgSendEndSem[1]);
	
	*M8641RIO_OSR(baseAddr) |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
	 
#ifdef RIO_RMU_DBG_ON
	outboundStatus = *M8641RIO_OSR(baseAddr);
/*	*(UINT32*)0xe000073c=outboundStatus;*/
	count++;
	*(UINT32*)0xe0000740=count;
#endif
	
	*M8641RIO_ODATR(baseAddr) |=(1<<29);
	/**M8641RIO_OMR(baseAddr) |= (1<<5);*/
}

/*doorbell*/

#endif



#if 0
LOCAL  void rioErrorIntHandle()
{
	UINT32 status=0,temp=0,baseAddr=0,timeOut=5;
	
#ifdef RIO_RMU_DBG_ON
	static UINT32 count=0;
#endif
	/*vxbIntEnable  (pInst, 6, rioErrorIntHandle, NULL);*/
	rioMsgOutIntStatus=0;
	temp=(1<<12)|(1<<11)|(1<<10)|(1<<7);
	
	for(i=0;i<2;i++)
	{
		baseAddr=RAPIDIO_BASE+0x100*i;
		status = *M8641RIO_ODSR(baseAddr);
		if(status&temp) i=2;
	}
	
	*M8641RIO_OMR(baseAddr) &= (~(1<<5));
	
	
#ifdef RIO_RMU_DBG_ON
	*(UINT32*)0xe0000a00=0x12345678;
	*(UINT32*)0xe0000a04=status;
#endif
		
	if((status>>12)&0x01)	rioMsgOutIntStatus |=(1<<0);
	if((status>>11)&0x01)	rioMsgOutIntStatus |=(1<<1);
	if((status>>10)&0x01)	rioMsgOutIntStatus |=(1<<2);
	if((status>>7)&0x01)	rioMsgOutIntStatus |=(1<<3);
	
	while(timeOut)
	{
		if((*M8641RIO_OSR(baseAddr)>>2)&0x01)   timeOut--;
		else break;
	}
	
	if(rioMsgOutIntStatus) *M8641RIO_OMR(baseAddr) &= (~(1<<0));/*disable outbount control*/
	
	semGive(rioMsgSendEndSem[1]);
	
	*M8641RIO_OSR(baseAddr) |= ((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7)|(1<<10)|(1<<11)|(1<<11));
	 
#ifdef RIO_RMU_DBG_ON
	status = *M8641RIO_OSR(baseAddr);
	*(UINT32*)0xe0000a08=status;
	count++;
	*(UINT32*)0xe0000a0c=count;
	*(UINT32*)0xe0000a10=rioMsgOutIntStatus;
#endif
	
	/**M8641RIO_ODATR(baseAddr) |=(1<<29);*/
	*M8641RIO_OMR(baseAddr) |= (1<<5);
}

#endif


#if 0

void rioMsgInIntHandle(UINT32 unit)
{
	UINT32 addr;

	UINT32 baseAddr=RAPIDIO_BASE+0x100*unit;
	UINT32 inboundStatus,inboundStatus1;
	UINT16 msgInfo=unit;
	static UINT32 count=0;
	
	*M8641RIO_IMR(baseAddr) &=(~((1<<5)|(1<<6)|(1<<8)));
#ifdef RIO_RMU_DBG_ON
	inboundStatus = *M8641RIO_ISR(baseAddr);
	*(UINT32*)0xe0000900=0x12345678;
	*(UINT32*)0xe0000904=inboundStatus;
	*(UINT32*)0xe0000910=count;
#endif
	
	if(inboundStatus&0x01)
	{
	
		if(rioMsgInInt[unit] == NULL) return;
		(*rioMsgInInt[unit])(msgInfo);
#ifdef RIO_RMU_DBG_ON		
		*(UINT32*)0xe0000908=0x87654321;
#endif
	}
	count++;
	*M8641RIO_ISR(baseAddr) |= ((1<<0)|(1<<4)|(1<<7)|(1<<10));
#ifdef RIO_RMU_DBG_ON	
	inboundStatus = *M8641RIO_ISR(baseAddr);
	*(UINT32*)0xe000090c=inboundStatus;
	*(UINT32*)0xe0000914=count;
#endif
	*M8641RIO_IMR(baseAddr) |=(((1<<5)|(1<<6)|(1<<8)));
}


void rioMsgInIntHandle0(void * param)
{
	rioMsgInIntHandle(0);
}

void rioMsgInIntHandle1(void * param)
{
	rioMsgInIntHandle(1);
}


void  rioMsgInIntSetup(int(*intFuc)(UINT16 data),UINT8 unit)
{
	VXB_DEVICE_ID pInst;

	UINT32 baseAddr;
	
	if(unit>2) unit=0;
	baseAddr=RAPIDIO_BASE+0x100*unit;
	
	*M8641RIO_ISR(baseAddr) |= ((1<<0)|(1<<4)|(1<<7)|(1<<10));
	*M8641RIO_IMR(baseAddr) |=(((1<<5)|(1<<6)|(1<<8)));
	rioMsgInInt[unit]=intFuc;
	
	pInst=vxbInstByNameFind ("m85xxCPU", 0);
	if (pInst == NULL)
	{
		printf ("Get pInst failed\n");
		return;
	}	
	if(unit==0)	
	{
		vxbIntConnect (pInst, 2, rioMsgInIntHandle0, NULL);
		vxbIntEnable  (pInst, 2, rioMsgInIntHandle0, NULL);	
	}
	else if(unit==1)	
	{
		vxbIntConnect (pInst, 4, rioMsgInIntHandle1, NULL);
		vxbIntEnable  (pInst, 4, rioMsgInIntHandle1, NULL);	
	}
}

#endif
