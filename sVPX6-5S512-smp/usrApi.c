#include <hwif/util/vxbDmaLib.h>
#include "vxbM8641Dma.h"
#include <vxbTimerLib.h>
#include <private/cpcLibP.h>
#include <logLib.h>
#include <cpuset.h>
#include <vxCpuLib.h>
#include <assert.h>
#include <vxbQorIQRman.h>
#include "usrApi.h"
#include "hostload.h"


RIO_MSG_INT_INF_S 	gRioMsgIntInf;

UINT8 getSlotNum(void)
{
	UINT8	ucSlotNum = 0;

	ucSlotNum = /*(~(RM_M8548_READ_REG(MPC_FPGA_SPARTEN + 0x10)) & 0x3f )*/
		(~(RM_M8548_READ_REG(MPC_FPGA_SPARTEN + 0x10)) & 0x1f );
	return ucSlotNum;
}
UINT8 getChasisNum(void)
{
	UINT8	ucChasisNum = 0;

	ucChasisNum = (((RM_M8548_READ_REG(MPC_FPGA_SPARTEN + 0x10)) >> 6) & 0xf );
	return ucChasisNum;
}

int sysCPUIdGet(void)
{
	cpuset_t affinity = 0;

	if(OK != taskCpuAffinityGet(0,&affinity))
	{
		return ERROR;
	}
	return affinity;
}
int sysRioIdGet(void)	
{
	UINT32 slotNum = 0,chasisNum = 0;
	INT32 rioId = 0;

	slotNum = getSlotNum();
	/*chasisNum = getChasisNum();*/
	rioId = (slotNum * 4 + 1);
	
	return rioId;
}
INT32 rmGetIpAddr(char *ip)
{
	UINT32 rmBnet = 0,rmAnet = 0;
	UINT32 rmSlot = 0;
	UINT32 rmChasisNum = 0;
	
	rmSlot = getSlotNum();
	rmChasisNum = getChasisNum();

	rmAnet = rmChasisNum *2;
	if(rmSlot <= 5)
	{
		rmBnet = ((rmSlot-1) * 32) + 5;
	}
	else
	{
		rmAnet += 1;
		rmBnet = ((rmSlot-8) * 32) + 5;
	}
	
	/*现在前面几位需要手动改变*/
	sprintf(ip,"192.168.%d.%d\0",rmAnet,rmBnet);
	return OK;
}

INT32 rioDmaNread(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize)
{
	DMA_M8641_DEDICATED_S  stChanInfo;
	VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID          pstDevId = NULL;
    STATUS                 iRet = 0;
	
	stChanInfo.uiMagic = DMA_M8641_MAGIC;
    stChanInfo.uiUnit = 0;
    stChanInfo.uiDestAttr = lOCAL_DST_ATTRIB_SNOOP;
    stChanInfo.uiSrcAttr = lOCAL_SRC_ATTRIB_SNOOP;
    stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
    
	pstDevId = vxbInstByNameFind("dma",0);
    if(NULL == pstDevId)
    {
        printf("cannot find dam engine\n");
        return DMA_ERROR_OTHER;
    }
    
	pstDmaChan = vxbDmaChanAlloc (pstDevId, 1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
	if(NULL == pstDmaChan)
    {
        printf("resource get failed\n");
        return DMA_ERROR_TIMEOUT;
    }
    
    iRet = vxbDmaReadAndWait(pstDmaChan,pcSrcAddr,pcDestAddr,&uiSize,0,0);
	vxbDmaChanFree(pstDmaChan);
	return iRet;
}


INT32 rioDmaNwrite(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize)
{
	DMA_M8641_DEDICATED_S  stChanInfo;
	VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID          pstDevId = NULL;
    STATUS                 iRet = 0;
	
	stChanInfo.uiMagic = DMA_M8641_MAGIC;
    stChanInfo.uiUnit = 0;
    stChanInfo.uiDestAttr = lOCAL_DST_ATTRIB_SNOOP;
    stChanInfo.uiSrcAttr = lOCAL_SRC_ATTRIB_SNOOP;
    stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
    
	pstDevId = vxbInstByNameFind("dma",0);
    if(NULL == pstDevId)
    {
        printf("cannot find dam engine\n");
        return DMA_ERROR_OTHER;
    }
    
	pstDmaChan = vxbDmaChanAlloc (pstDevId, 1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
	if(NULL == pstDmaChan)
    {
        printf("resource get failed\n");
        return DMA_ERROR_TIMEOUT;
    }
    
    iRet = vxbDmaReadAndWait(pstDmaChan,pcSrcAddr,pcDestAddr,&uiSize,0,0);
	vxbDmaChanFree(pstDmaChan);
	return iRet;
}




int rioRecvDoorbellIsr(UINT16 srcId, UINT16 dstId, UINT16 dbData)
{
	logMsg("srcId = 0x%x..dstId = 0x%x..dbData = 0x%x\n", srcId, dstId, dbData, 0, 0, 0);
	return OK;
}


VOID rioSendDoorbellIsr()
{
    logMsg("doorbell send finish !\n", 0, 0, 0, 0, 0, 0);
}
UINT32	 guiCnt = 0; 

INT32 rioRecMsgIsr(MSG_TRAN_BUF * recBuf, STATUS status, void * cbArg)
{
	RIO_MSG_INT_INF_S	*rioMsgIntInf;
	UINT32	 msgBuf[2] = {0};
	UINT32	 *addr = NULL;
	

	rioMsgIntInf = &gRioMsgIntInf;
	
	addr = (rioMsgIntInf->buf + rioMsgIntInf->uiTotal);
	memcpy(addr,recBuf->buf,recBuf->len);
	rioMsgIntInf->uiTotal += 256;
	/*中间件固定长度256*/
	if(rioMsgIntInf->uiTotal == 0x100000)
	{
		rioMsgIntInf->uiTotal = 0;
	}
	guiCnt ++;
	msgBuf[0] = (UINT32*)addr;
	msgBuf[1] = recBuf->len;
	msgQSend(rioMsgIntInf->msgId, 
			(char*)msgBuf,sizeof(UINT32)*2, NO_WAIT, MSG_PRI_NORMAL);
}

UINT32 gReuiCnt = 0;
int rioRecMsg(UINT8 *msgPtr, UINT32 len, UINT16 *srcid, UINT32 timeout)
{
	UINT32 uiRecLen = 0,uiCurTimeOut = 0,uiCnt = 0,i=0;
	RIO_MSG_INT_INF_S	*rioMsgIntInf;
	UINT32	 msgBuf[2] = {0};
	static	UINT32	uiTotalLen = 0;
	UINT32	uiLen = 0,uiTotal = 0,uiLoop = 0;
	MSG_TRAN_BUF * recBuf = NULL;
	char 		*curRecvBuf = NULL;
	UINT32		*addr = NULL;

	curRecvBuf = msgPtr;
	rioMsgIntInf = &gRioMsgIntInf;
	
	while(TRUE)
	{
		msgBuf[0] = 0;
		msgBuf[1] = 0;
		 uiRecLen = msgQReceive(gRioMsgIntInf.msgId, (char*)msgBuf, 
                          sizeof(UINT32)*2, timeout);
        if(uiRecLen == ERROR)
        {
            logMsg("msg receive error in rio\n",
                    0,0,0,0,0,0); 
			rioMsgIntInf->uiTotal = 0;
			memset(rioMsgIntInf->buf,0,0x100000);
            return ERROR;
        }
        gReuiCnt ++;
		addr = (UINT32*)msgBuf[0];
		uiLen = msgBuf[1];
		memcpy(msgPtr,addr,uiLen);
		
		uiTotal += uiLen;
		if(uiTotal >= len)
		{
			memset(addr,0,uiLen);
			uiTotal = 0;
			return len;
		}
		else
		{
			memset(addr,0,uiLen);
			rioMsgIntInf->uiFlag = 0;
			return uiTotal;
		}
	}
}

STATUS rioMsgInit(MSG_RXCB_FUNC pfMsgHook)
{
    FUNCPTR             method;
    FUNCPTR		        rmanIntRxInstall;
    VXB_DEVICE_ID       rioRmanDevId = NULL;    
    RIO_TRANS_PARAMETER msgArg;
    MSG_TRANS_API      *msgApi = NULL;

	RIO_MSG_INT_INF_S	*RioMsgIntInf = NULL;

    logMsg("rioMsgInit ..\n", 0, 0, 0, 0, 0, 0);
	/*if(NULL == (gRioMsgIntInf.msgSemId = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY)))
    {
        return ERROR;
    }	*/
    RioMsgIntInf = &gRioMsgIntInf;
	/*memset(RioMsgIntInf,0,sizeof(RIO_MSG_INT_INF_S));*/
	if(NULL == (RioMsgIntInf->msgId = msgQCreate(0x10000, 8, MSG_Q_FIFO)))
    {
        return ERROR;
    }  
	RioMsgIntInf->buf = memalign(0x100000,0x100000);
	if(RioMsgIntInf->buf == NULL)
	{
		return ERROR;
	}
	SPIN_LOCK_ISR_INIT(&RioMsgIntInf->stLock, 0);
    rioRmanDevId = vxbInstByNameFind(QORIQ_RMAN_NAME, 0);
	if(NULL == rioRmanDevId)
	{
		logMsg("cannot find rio RMAN DEV\n", 0, 0, 0, 0, 0, 0);
		return ERROR;
	}

    method = vxbDevMethodGet (rioRmanDevId, (VXB_METHOD_ID)&vxbMsgApiGet_desc);
    if (method == NULL)
    {
        logMsg("get RMAN method failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
    
    msgApi = (MSG_TRANS_API *)(*method)(rioRmanDevId);
    if(msgApi == NULL)
    {
        logMsg("get RMAN API failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
       
    *(ULONG *)SM_INT_ARG1  = rioRmanDevId;
    *(volatile UINT32 *)SM_INT_ARG2  = 0xff; /*待确定*/
    msgArg.destId = 0xff;
    msgArg.transType = RIO_TYPE_MBOX;
    msgArg.mbox = 0;
    msgArg.letter = 0;
    msgArg.mboxObChan = 0;
    return (msgApi->intRxAddHook(rioRmanDevId, &msgArg, pfMsgHook, NULL));   
}

VOID rioSendMsgIsr(VOID)
{
	/*logMsg("&&&&&&&&SEND msg ok !\n", 0, 0, 0, 0, 0, 0);*/

}
STATUS rioSendMsg(UINT16 localPort,UINT16 mailBox,UINT16 dstID,UINT8 *msgPtr,UINT32 len)
{
	STATUS		     sRetrun;
    FUNCPTR          method;
    FUNCPTR		     rmanIntRxInstall;
    FUNCPTR	         rmanIntSend;
    MSG_TRANS_API   *msgApi = NULL;
    VXB_DEVICE_ID    rioRmanDevId = NULL;
    RIO_TRANS_PARAMETER  msgArg;
    struct msgbuf  desc;

    
    rioRmanDevId = vxbInstByNameFind(QORIQ_RMAN_NAME, 0);
	if(NULL == rioRmanDevId)
	{
		logMsg("cannot find rio RMAN DEV\n", 0, 0, 0, 0, 0, 0);
		return ERROR;
	}

    method = vxbDevMethodGet (rioRmanDevId, (VXB_METHOD_ID)&vxbMsgApiGet_desc);
    if (method == NULL)
    {
        logMsg("get RMAN method failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }

    msgApi = (MSG_TRANS_API *)(*method)(rioRmanDevId);
    if(msgApi == NULL)
    {
        logMsg("get RMAN API failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
    msgArg.destId = dstID;
    msgArg.transType = RIO_TYPE_MBOX;
    msgArg.mbox = mailBox;
    msgArg.letter = 2;
    msgArg.mboxObChan = 0;

    desc.buf = msgPtr;
    desc.len = len;

	return (msgApi->intSend(rioRmanDevId, &msgArg, &desc, rioSendMsgIsr, NULL));     
}


VOID rioDoorRecv(UINT8 src,UINT8 dst ,UINT16 info)
{
	logMsg("recv doorbell src %x dst %x info 0x%x\n", src, dst, info, 0, 0, 0);
}

INT32 rioDoorbellInit(pfHook pfMsgHook)
{
    FUNCPTR             method;
    FUNCPTR		        rmanIntRxInstall;
    VXB_DEVICE_ID       rioRmanDevId = NULL;    
    RIO_TRANS_PARAMETER msgArg;
    MSG_TRANS_API      *msgApi = NULL;


    rioRmanDevId = vxbInstByNameFind(QORIQ_RMAN_NAME, 0);
	if(NULL == rioRmanDevId)
	{
		logMsg("cannot find rio RMAN DEV\n", 0, 0, 0, 0, 0, 0);
		return ERROR;
	}

    method = vxbDevMethodGet (rioRmanDevId, (VXB_METHOD_ID)&vxbMsgApiGet_desc);
    if (method == NULL)
    {
        logMsg("get RMAN method failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
    
    msgApi = (MSG_TRANS_API *)(*method)(rioRmanDevId);
    if(msgApi == NULL)
    {
        logMsg("get RMAN API failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
       
    *(ULONG *)SM_INT_ARG1  = rioRmanDevId;
    *(volatile UINT32 *)SM_INT_ARG2  = 0xff; /*待确定*/
    msgArg.destId = 0xff;
    msgArg.transType = RIO_TYPE_DBELL;
    msgArg.mbox = 0;
    msgArg.letter = 0;
    msgArg.mboxObChan = 0;
    return (msgApi->intRxAddHook(rioRmanDevId, &msgArg, 
		     pfMsgHook, NULL));   
}

void rioDoorInit()
{
	rioDoorbellInit(rioDoorRecv);
}
STATUS rioSendDoorbell( UINT8 dstId, UINT16 dbData)
{
    UINT16           dbBuf = 0;
    FUNCPTR          method;
    FUNCPTR		     rmanIntRxInstall;
    FUNCPTR	         rmanIntSend;
    MSG_TRANS_API   *msgApi = NULL;
    VXB_DEVICE_ID    rioRmanDevId = NULL;
    RIO_TRANS_PARAMETER  msgArg;
    struct msgbuf * desc;

    
    rioRmanDevId = vxbInstByNameFind(QORIQ_RMAN_NAME, 0);
	if(NULL == rioRmanDevId)
	{
		logMsg("cannot find rio RMAN DEV\n", 0, 0, 0, 0, 0, 0);
		return ERROR;
	}

    method = vxbDevMethodGet (rioRmanDevId, (VXB_METHOD_ID)&vxbMsgApiGet_desc);
    if (method == NULL)
    {
        logMsg("get RMAN method failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }

    msgApi = (MSG_TRANS_API *)(*method)(rioRmanDevId);
    if(msgApi == NULL)
    {
        logMsg("get RMAN API failed !\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }

    msgArg.destId = dstId;
    msgArg.transType = RIO_TYPE_DBELL;
    msgArg.mbox = 0;
    msgArg.letter = 0;
    msgArg.mboxObChan = 0;

    dbBuf = dbData;
    desc->buf = &dbBuf;
    desc->len = 2;
    
    return (msgApi->intSend(rioRmanDevId, &msgArg, desc, rioSendDoorbellIsr, NULL));     
}


INT32 ddrDma(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize)
{
	DMA_M8641_DEDICATED_S  stChanInfo;
	VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID          pstDevId = NULL;
    STATUS                 iRet = 0;
	
	stChanInfo.uiMagic = DMA_M8641_MAGIC;
    stChanInfo.uiUnit = 0;
    stChanInfo.uiDestAttr = lOCAL_DST_ATTRIB_SNOOP;
    stChanInfo.uiSrcAttr = lOCAL_SRC_ATTRIB_SNOOP;
    stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
    
	pstDevId = vxbInstByNameFind("dma",0);
    if(NULL == pstDevId)
    {
        printf("cannot find dam engine\n");
        return DMA_ERROR_OTHER;
    }
    
	pstDmaChan = vxbDmaChanAlloc (pstDevId, 1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
	if(NULL == pstDmaChan)
    {
        printf("resource get failed\n");
        return DMA_ERROR_TIMEOUT;
    }
    
    iRet = vxbDmaReadAndWait(pstDmaChan,pcSrcAddr,pcDestAddr,&uiSize,0,0);
	vxbDmaChanFree(pstDmaChan);
	return iRet;
}


M8641_TIMER_S  gastTimer[TIMER_CNT];
SEM_ID sysTimerSemId1;
SEM_ID sysTimerSemId2;
SEM_ID sysTimerSemId3;
SEM_ID sysTimerSemId4;


VOID sysTimerISR(UINT32 id)
{
    UINT32   uiArg3 = 0;
    
    assert(id < TIMER_CNT);
    
    if((TIMER_MODE_ONCE == gastTimer[id].uiMode) ||
       (TIMER_MODE_SEM_ONCE == gastTimer[id].uiMode))
    {
        P5020_WRITE_REG(TIMER_GTBCRB(id),(1 << 31));
    }
    
    if((TIMER_MODE_SEM_CONTINUE == gastTimer[id].uiMode) ||
       (TIMER_MODE_SEM_ONCE == gastTimer[id].uiMode))
    {
        switch(id)
        {
            case 0:
              semGive(sysTimerSemId1);
              break;
            case 1:
              semGive(sysTimerSemId2);
              break;
            case 2:
              semGive(sysTimerSemId3);
              break;
            case 3:
              semGive(sysTimerSemId4);
              break;
        }
    }
    
    logMsg("id = 0x%x\n", id, 0, 0, 0, 0, 0);
    
    if(NULL != gastTimer[id].pfISR)
    {
        
        gastTimer[id].pfISR(id, gastTimer[id].uiArg1,
                            gastTimer[id].uiArg2, gastTimer[id].uiArg3);
    }
}


void sysTimeSrcInit()
{
    UINT32 uiLoop = 0;
    
    for(uiLoop = 0;uiLoop < TIMER_CNT;uiLoop++)
    {
        memset(&gastTimer[uiLoop], 0, sizeof(M8641_TIMER_S));
        gastTimer[uiLoop].semLock = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
		intConnect ((VOIDFUNCPTR *)(OPENPIC_TIMERB0_INT_VEC + uiLoop),
                    (VOIDFUNCPTR)sysTimerISR, uiLoop);
    }
    
	sysTimerSemId1 = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	sysTimerSemId2 = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	sysTimerSemId3 = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
	sysTimerSemId4 = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
}


VOID sysTimerInit(UINT32  id)
{
	UINT32  uiRegVal = 0;
    
	semTake(gastTimer[id].semLock,WAIT_FOREVER);
    
	if(TIMER_FLAG_INITED == gastTimer[id].uiFlag)
	{
		semGive(gastTimer[id].semLock);
		return ;
	}
    
    gastTimer[id].uiFlag = TIMER_FLAG_INITED;
    
    semGive(gastTimer[id].semLock);
    
    uiRegVal = P5020_READ_REG(TIMER_TCRB);
	if(0x0 != uiRegVal)
	{
        P5020_WRITE_REG(TIMER_TCRB,0);
    }
    
    P5020_WRITE_REG(TIMER_GTBCRB(id),(1 << 31));
}


INT32 sysTimerOpen(UINT32 id,UINT32 mode)
{
    assert(id < TIMER_CNT);
    
    if(TIMER_FLAG_UNINIT == gastTimer[id].uiFlag)
    {
        sysTimerInit(id);
        gastTimer[id].uiFlag = TIMER_FLAG_INITED;
    }
    
    if(TIMER_FLAG_OPEN == gastTimer[id].uiFlag)
    {
        printf("timer %d has been open\n",id);
        return ERROR;
    }
    
    semTake(gastTimer[id].semLock,WAIT_FOREVER);
    gastTimer[id].uiFlag = TIMER_FLAG_OPEN;
    gastTimer[id].uiMode = mode;
    semGive(gastTimer[id].semLock);
    
    return OK;
}


INT32 sysTimerClose(UINT32 id)
{
    assert(id < TIMER_CNT);
    
    semTake(gastTimer[id].semLock,WAIT_FOREVER);
    
    P5020_WRITE_REG(TIMER_GTBCRB(id),(1 << 31));
    
    intDisable(OPENPIC_TIMERB0_INT_VEC + id);
    
    gastTimer[id].uiFlag = TIMER_FLAG_UNINIT;
    gastTimer[id].uiMode = 0;
    gastTimer[id].pfISR = NULL;
    gastTimer[id].uiArg1 = 0;
    gastTimer[id].uiArg2 = 0;
    gastTimer[id].uiArg3 = 0;
    
    semGive(gastTimer[id].semLock);
    
	return OK;
}


INT32 sysTimerCreat(UINT32 id,UINT32 dlyTime)
{
    UINT32 uiFreq = 0;
    UINT32 uiDivisor = 0;
    UINT32 uiRemain = 0;
    UINT32 uiTimeVal = 0;
    
    if(dlyTime >= 0x1ffffff)
    {
        printf("delay time %d is too big\n",dlyTime);
        return ERROR;
    }
    uiFreq = sysClkFreqGet() / 1000000;/*MHZ*/
    uiDivisor = uiFreq / 8;
    uiRemain = uiFreq % (8);
    uiTimeVal = uiDivisor * dlyTime + (dlyTime * uiRemain)/8;
    
    P5020_WRITE_REG(TIMER_GTBCRB(id),uiTimeVal);
    
    intEnable(OPENPIC_TIMERB0_INT_VEC + id);
    
    return OK;
}


INT32 usrTimerDelay(UINT32 id,UINT32 delayTime,VOIDFUNCPTR usrISR,
                     UINT32 arg1,UINT32 arg2,UINT32 arg3)
{
    assert(id < TIMER_CNT);
    
    semTake(gastTimer[id].semLock,WAIT_FOREVER);
    
    gastTimer[id].pfISR = usrISR;
    gastTimer[id].uiArg1 = arg1;
    gastTimer[id].uiArg2 = arg2;
    gastTimer[id].uiArg3 = arg3;
    
    semGive(gastTimer[id].semLock);
    
    return sysTimerCreat(id,delayTime);
}




/*******************************************************************************
*
* sysRtcInit - Initializes the rtc clock application
*
* This routine initializes the rtc clock application.
*
* RETURNS: OK
*
* ERRNO: N/A
*
*/
STATUS sysRtcInit (void)
{
	UINT8 uiVal = 0;

	P5020_WRITE_REG_8BIT(RTC_OSCILLATOR, uiVal);
				
	return OK;
}

/*******************************************************************************
*
* sysRtcSet - set the rtc clock application
*
* This routine set the rtc clock application.
*
* RETURNS: OK
*
* ERRNO: N/A
*
*/
STATUS sysRtcSet(RTC_DATE_TIME *tm)
{
	/*实际需要以16进制来传入*/

    tm->mSec = (tm->mSec / 10) * 16 + (tm->mSec % 10);
    tm->year = (tm->year / 10) * 16 + (tm->year % 10);
    tm->month = (tm->month / 10) * 16 + (tm->month % 10);
	tm->date = (tm->date / 10) * 16 + (tm->date % 10);
    tm->minute = (tm->minute / 10) * 16 + (tm->minute % 10);
	tm->second = (tm->second / 10) * 16 + (tm->second % 10);
    tm->hour =   (tm->hour / 10) * 16 + (tm->hour % 10);
    
	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL, 0x2);
	P5020_WRITE_REG_8BIT(RTC_TIME_YEAR, tm->year);
	P5020_WRITE_REG_8BIT(RTC_TIME_MONTH, tm->month);
	P5020_WRITE_REG_8BIT(RTC_TIME_DATE, tm->date);
	P5020_WRITE_REG_8BIT(RTC_TIME_DAY, tm->day);
	P5020_WRITE_REG_8BIT(RTC_TIME_HOUR, tm->hour);
	P5020_WRITE_REG_8BIT(RTC_TIME_MINUTES, tm->minute);
	P5020_WRITE_REG_8BIT(RTC_TIME_SECONDS, tm->second);
	/*tm->century |= 0x80;*/
	P5020_WRITE_REG_8BIT(RTC_TIME_CENTURY, tm->mSec);
	
	/*清写标志, 数据刷入内部reg*/
	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL,0x0);

	return OK;
}

/*******************************************************************************
*
* sysRtcGet - get the rtc clock application
*
* This routine get the rtc clock application.
*
* RETURNS: OK
*
* ERRNO: N/A
*
*/

STATUS  sysRtcGet(RTC_DATE_TIME *tm)
{
	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL, 0x1);

	tm->year = P5020_READ_REG_8BIT(RTC_TIME_YEAR);
	tm->month = P5020_READ_REG_8BIT(RTC_TIME_MONTH) & 0x1f;
	tm->date = P5020_READ_REG_8BIT(RTC_TIME_DATE) & 0x3f;
	tm->day = P5020_READ_REG_8BIT(RTC_TIME_DAY) & 0x7;
	tm->hour = P5020_READ_REG_8BIT(RTC_TIME_HOUR) & 0x3f;
   
	tm->minute = P5020_READ_REG_8BIT(RTC_TIME_MINUTES) & 0x7f;
	tm->second = P5020_READ_REG_8BIT(RTC_TIME_SECONDS) & 0x7f;
	tm->mSec= P5020_READ_REG_8BIT(RTC_TIME_CENTURY);
    
	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL,0x0);

	tm->year = (tm->year >> 4) * 10+ (tm->year &0xf);
	tm->month = (tm->month>>4 & 0x1) * 10 + (tm->month & 0xf);
	tm->date = (tm->date>>4 & 0x3) * 10 + (tm->date & 0xf);
	tm->day &= 0x7; 
	tm->hour = (tm->hour>>4 & 0x3) * 10 + (tm->hour & 0xf);
	tm->minute = (tm->minute>>4 & 0x7) * 10 + (tm->minute & 0xf);
	tm->second= (tm->second>>4 & 0x7) * 10 + (tm->second & 0xf);
	tm->mSec= (tm->mSec>>4) * 10 + (tm->mSec & 0xf);

	return OK;
}


/*******************************************************************************
*
* sysRtcGet - Shutdown the rtc clock application
*
* This routine Shutdown the rtc clock application.
*
* RETURNS: OK
*
* ERRNO: N/A
*
*/
void sysRtcShutdown(void)
{
	UINT8 uiVal = 0;


	uiVal = P5020_READ_REG_8BIT(RTC_RW_CONTROL);
	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL, uiVal | 0x80);

	uiVal = P5020_READ_REG_8BIT(RTC_OSCILLATOR);
	P5020_WRITE_REG_8BIT(RTC_OSCILLATOR, uiVal | 0x80);


	P5020_WRITE_REG_8BIT(RTC_RW_CONTROL, uiVal &0x7f);
}
UINT32 sysFpgaRtcSet(RTC_DATE_TIME *tm)
{
    UINT32 uiVal = 0;

	uiVal = ((tm->year + 2000) << 16) | (tm->month << 8) | tm->date;
	P5020_WRITE_REG(RTC_SET_REG_2,uiVal);
	
    uiVal =(tm->mSec << 21)|(tm->hour << 16) | 
		(tm->minute << 8) | tm->second; 
    P5020_WRITE_REG(RTC_SET_REG_1,uiVal);
    uiVal |= (1 << 24);
    P5020_WRITE_REG(RTC_SET_REG_1,uiVal);
	
    return OK;
}

INT32 sysFpgaRtcGet(RTC_DATE_TIME *tm)
{
    UINT32 uiVal = 0;

    uiVal = P5020_READ_REG(RTC_GET_REG_1);
    tm->hour = (uiVal >> 16) & 0x1f;
    tm->minute = (uiVal >> 8) & 0x3f;
    tm->second = uiVal & 0x3f;
	tm->mSec = (uiVal >> 21) & 0x3ff;
    uiVal = P5020_READ_REG(RTC_GET_REG_2);
    tm->year = (((uiVal >> 16) & 0xfff) % 100);
    tm->month = (uiVal >> 8) & 0x3f;
    tm->date = uiVal & 0x1f;
	
   	/*printf(" 20%02d-%02d-%02d,%02d:%02d:%02d ms %d\n", tm->year, tm->month,
		tm->date, tm->hour, tm->minute, tm->second,tm->mSec);*/
    return OK;
}

INT32 sysTimeToBit(RTC_DATE_TIME *tm,UINT32 *writeDataLow,UINT32 *writeDataHi)
{
	INT8 curData =0;
	*writeDataHi |= ((tm->mSec  & 0XFF)<< 24);				/* ms*/
	curData = ((tm->second / 10) << 4) | ((tm->second % 10)&0xf);
	*writeDataHi |= ((curData  & 0X7F)<< 16);					/* s*/
	curData = ((tm->minute / 10) << 4) | ((tm->minute % 10)&0xf);
	*writeDataHi |= ((curData & 0X7F) << 8);					/* minute*/
	curData = ((tm->hour / 10) << 4) | ((tm->hour % 10)&0xf);
	*writeDataHi |= ((curData & 0x3f) | (0x2 << 6));			/* hour*/
	*writeDataLow |= ((((UINT32)tm->day) << 24) & 0xff000000); 	/* day*/
	curData = ((tm->date / 10) << 4) | ((tm->date % 10)&0xf);
	*writeDataLow |= ((((UINT32)curData) << 16) & 0x00ff0000) ;	/* date*/
	curData = ((tm->month / 10) << 4) | ((tm->month % 10)&0xf);
	*writeDataLow |= ((((UINT32)curData) << 8) &0x0000ff00);	/* month*/
	curData = ((tm->year / 10) << 4) | ((tm->year % 10)&0xf);
	*writeDataLow |= (0xff & ((UINT32)curData));	
	return OK;
}



INT32 sysGetFpgaRtc(UINT8 regNum,UINT8 addr,UINT32 *readDataLow,UINT32 *readDataHi)
{
	RTC_DATE_TIME tm;
	UINT32 rtcFlag = 0,readReg = 0;
	while((P5020_READ_REG(RTC_BUSY_REG2) & 0x1) != 0)
	{
		sysDelay();
	}
	readReg = ((0x1 << 13) |(regNum << 0x8) | (addr));
	P5020_WRITE_REG(RTC_CONTROL_REG2,readReg);
	sysDelay();
	while((P5020_READ_REG(RTC_BUSY_REG2) & 0x1) != 0)
	{
		sysDelay();	
	}
	*readDataLow = P5020_READ_REG(RTC_GET_REG2_1);
	*readDataHi = P5020_READ_REG(RTC_GET_REG2_2);
	P5020_WRITE_REG(RTC_CONTROL_REG2,0x0);
}

INT32 sysSetFpgaRtc(UINT8 regNum,UINT8 addr,UINT32 lowData,UINT32 hiData)
{
	RTC_DATE_TIME tm;
	UINT32 rtcFlag = 0,writeReg = 0;
	UINT32 writeDataLow = 0,writeDataHi = 0;
	while((P5020_READ_REG(RTC_BUSY_REG2) & 0x1) != 0);
	{
		sysDelay();
	}
	writeDataLow = P5020_WRITE_REG(RTC_SET_REG2_1,lowData);
	writeDataHi = P5020_WRITE_REG(RTC_SET_REG2_2,hiData);
	writeReg = ((0x1 << 12) |(regNum << 0x8) | (addr)); 
	P5020_WRITE_REG(RTC_CONTROL_REG2,writeReg);
	sysDelay();
	while((P5020_READ_REG(RTC_BUSY_REG2) & 0x1) != 0)
	{
		sysDelay();	
	}
	P5020_WRITE_REG(RTC_CONTROL_REG2,0x0);
}

INT32 sysSetFpgaTime(RTC_DATE_TIME *tm)
{
	UINT32 writeDataLow = 0,writeDataHi = 0,uiLoop = 0;
	INT8 curData =0;
	
	sysTimeToBit(tm,&writeDataLow,&writeDataHi);
	sysSetFpgaRtc(0x1,0x1,writeDataLow,((writeDataHi) | (0x80 << 24)));
	sysSetFpgaRtc(0x1,0x1,writeDataLow,((writeDataHi) & 0x7fffffff));
	sysSetFpgaRtc(0x1,0xc,0,0x0);
	sysSetFpgaRtc(0x8,0x00,writeDataLow,writeDataHi);

	sysFpgaRtcSet(tm);
	
	return OK;
}

INT32 sysGetFpgaTime(RTC_DATE_TIME *tm)
{
	UINT32 readDataLow = 0,readDataHi = 0;
	UINT8 centery = 0;
	UINT8 curData = 0;
	RTC_DATE_TIME fpgaTm; 
	static UINT32 oldMs = 0;
	static UINT32 oldS =  0;
	
	sysGetFpgaRtc(0x1,0xf,&readDataLow,&readDataHi);
	if((readDataHi & 0x4000000) != 0)
	{
		sysSetFpgaRtc(0x1,0xf,readDataLow,(readDataHi & 0xfbffffff));
	}
	readDataLow = 0;
	readDataHi = 0;
	sysGetFpgaRtc(0x1,0xC,&readDataLow,&readDataHi);
	if((readDataHi & 0x40000000) != 0)
	{
		sysSetFpgaRtc(0x1,0xC,readDataLow,((0xbfffffff) & readDataHi));
	}
	readDataLow = 0;
	readDataHi = 0;
	sysGetFpgaRtc(8,0x0,&readDataLow,&readDataHi);
	sysFpgaRtcGet(&fpgaTm);
	
	//tm->mSec = ((readDataHi >> 24) & 0xff);
	
	tm->mSec = fpgaTm.mSec;
	
	curData = ((readDataHi >> 16) & 0x7f);
	tm->second= ((curData >> 4) * 10) + (curData & 0xf);
	curData = ((readDataHi >> 8)& 0x7f);
	tm->minute = (curData  >> 4) *10 + (curData & 0xf);
	curData = (readDataHi & 0x3f);
	tm->hour = (curData >> 4)*10 + (curData & 0xf);
	centery = ((readDataHi & 0xc0) >> 6);
	tm->day = ((readDataLow >> 24) & 0x7);
	curData = ((readDataLow >> 16) & 0x3f);
	tm->date = (curData >> 4) * 10 + (curData & 0xf);
	curData = ((readDataLow >> 8) & 0x1f);
	tm->month = (curData >> 4) * 10 + (curData & 0xf);
	curData = (readDataLow & 0xff);
	tm->year = ((curData >> 4) * 10) + (curData & 0xf);
	/*printf("year:%d%02d month %02d date %02d day %02d,",(centery*10),tm->year,
		tm->month,tm->date,tm->day);
	printf("hour:%02d min %02d sec %02d %02d\n",tm->hour,
		tm->minute,tm->second,tm->mSec);*/

	/*由于使用的是两个未同步的时间，所以这里要处理一下*/
	if(oldS == tm->second && oldMs >= tm->mSec) 
	{
		/*如果，当前的秒数和上次获取的秒数相同且
		当前的ms秒数居然还比之前获取的还小了。则需要修正时间*/
		tm->second++;
		if(tm->second > 59)
		{
			tm->minute++;
			tm->second = 0;
			if(tm->minute > 59)
			{
				tm->minute = 0;
				tm->hour++;
				if(tm->hour > 23)
				{
					tm->hour = 0;
					tm->date++;
					if(tm->date > 31)
					{
						tm->date= 0;
						tm->month++;
					}
				}
			}
		}
		
		oldS = tm->second;
		oldMs = tm->mSec;
	}
}


VOID sysFpgaRtcShutdown(VOID)
{
    UINT32 uiVal = 0;

    uiVal = P5020_READ_REG(RTC_SET_REG_1);
    uiVal = uiVal & (~(1 << 24));
    P5020_WRITE_REG(RTC_SET_REG_1,uiVal);
}

INT32 readRTC(RTC_DATE_TIME *tmpTime)
{
	memset(tmpTime, 0, sizeof(RTC_DATE_TIME));
	
	sysRtcGet(tmpTime);
   	printf(" 20%02d-%02d-%02d, week%d %02d:%02d:%02d ms %d\n", tmpTime->year, tmpTime->month,
		tmpTime->date, tmpTime->day, tmpTime->hour, tmpTime->minute, tmpTime->second,tmpTime->mSec);
	return OK;
}
INT32 rmRetReadTest()
{
	RTC_DATE_TIME tm;
	RTC_DATE_TIME curtm;

	while(1)
	{
		sysFpgaRtcGet(&tm);
		sysMsDelay(30);
		sysFpgaRtcGet(&curtm);

		printf("last 20%02d-%02d-%02d,%02d:%02d:%02d ms %d\n", tm.year, tm.month,
		tm.date, tm.hour, tm.minute, tm.second,tm.mSec);
		printf("cur  20%02d-%02d-%02d,%02d:%02d:%02d ms %d\n", curtm.year, curtm.month,
		curtm.date, curtm.hour, curtm.minute, curtm.second,curtm.mSec);
	}
}
void setRtc(    UINT8   year,
    UINT8   month,
    UINT8   date,
    UINT8   day,
    UINT8   hour,
    UINT8   minute,
    UINT8   second,
    UINT8   mSec)
{

	RTC_DATE_TIME tmpTime;

	tmpTime.year = year;	
	tmpTime.month = month;
	tmpTime.date = date;	 
	tmpTime.day = day;
	tmpTime.hour = hour;
	tmpTime.minute = minute;
	tmpTime.second = second;
	tmpTime.mSec = mSec;

#ifdef NVRAM_RTC
	sysRtcInit();
	taskDelay(10);
	sysRtcSet(&tmpTime);
	printf("Set RTC Successful \n");
#else
	sysFpgaRtcSet(&tmpTime);
	printf("Set RTC Successful \n");
#endif
}



#if 0
void testRTC()
{
	RTC_DATE_TIME tmpTime;

	tmpTime.year = 15;  
	tmpTime.month = 5;
	tmpTime.date = 12;   
	tmpTime.day = 2;
	tmpTime.hour = 14;
	tmpTime.minute = 59;
	tmpTime.second = 27;
	tmpTime.century = 21;


	sysRtcInit();
	taskDelay(10);
	sysRtcSet(&tmpTime);	
	printf("Set RTC Successful \n");

    memset(&tmpTime, 0, sizeof(RTC_DATE_TIME));
	sysRtcGet(&tmpTime);
    printf("get RTC Successful \n");
	printf(" %02dcentury, 20%02d-%02d-%02d, week%d %02d:%02d:%02d \n", tmpTime.century, tmpTime.year, tmpTime.month,
		tmpTime.date, tmpTime.day, tmpTime.hour, tmpTime.minute, tmpTime.second);
}
#endif

/*****************************************************************************
 函 数 名  : sysSetFpgaExtReset
 功能描述  : 配置外部复位寄存器，通过接插件P0  输出SYSRESETn给其它单板进行复位。
 输入参数  : UINT8 vucExtResetnCtl , 为1时释放复位,为0时使能复位
 输出参数  : 
 返回值    :
*****************************************************************************/
void sysSetFpgaExtReset(UINT8 vucExtResetnCtl)
{
	UINT16	ucExtResetnCtl = 0;

	ucExtResetnCtl =  (vucExtResetnCtl & 0x1);	
	P5020_WRITE_REG(SYS_OR4_BASE_ADRS + 0xa4, ucExtResetnCtl);
}


/*****************************************************************************
 函 数 名  : sysGetFpgaExtReset
 功能描述  : 读取外部复位寄存器。
 输入参数  : 
 输出参数  : 
 返回值    :
*****************************************************************************/
UINT16 sysGetFpgaExtReset()
{	
	return P5020_READ_REG(SYS_OR4_BASE_ADRS + 0xa4);
}

void sysSetFpgaData(UINT16 uiOffset, UINT16 uiNum)
{
	P5020_WRITE_REG_16BIT(SYS_OR2_BASE_ADRS + uiOffset, uiNum);
}
UINT16 sysGetFpgaData(UINT16 uiOffset)
{	
	return P5020_READ_REG_16BIT(SYS_OR2_BASE_ADRS + uiOffset);
}

UINT32 sysWatchDogInit (UINT32 delayMs)
{
    UINT32 uiCnt = 0;

    if(delayMs > 10000)
    {
        logMsg("time is too long\n", 0, 0, 0, 0, 0, 0);
        return ERROR;
    }
    
    uiCnt = delayMs * 100000;
    uiCnt = uiCnt & 0x3fffffff;
    
    P5020_WRITE_REG(CPU_WDG_REG,(uiCnt | (1 << 30)));
    P5020_WRITE_REG(CPU_WDG_REG,(uiCnt | (1 << 30) | (1 << 31)));
    
    return OK;
}


VOID sysWatchDogCounterStop (VOID)
{
    P5020_WRITE_REG(CPU_WDG_REG,0);
}


VOID sysWatchDogCounterReload (VOID)
{
    UINT32 uiVal = 0;
    
    uiVal = P5020_READ_REG(CPU_WDG_REG);
    P5020_WRITE_REG(CPU_WDG_REG,uiVal | (1 << 30));
}




