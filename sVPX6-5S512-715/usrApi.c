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

	ucSlotNum = (~(RM_M8548_READ_REG(MPC_FPGA_SPARTEN + 0x10)) & 0x3f );
	logMsg("get Slot num %d",ucSlotNum,0,0,0,0,0);
	return ucSlotNum;
}
UINT8 getChasisNum(void)
{
	UINT8	ucChasisNum = 0;

	ucChasisNum = (~((RM_M8548_READ_REG(MPC_FPGA_SPARTEN + 0x10)) >> 6) & 0xf );
	logMsg("get Chasis num %d",ucChasisNum,0,0,0,0,0);
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

	slotNum = getSlotNum();
	chasisNum = getChasisNum();
	
	return (slotNum * 4 + chasisNum);
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
MSG_TRAN_BUF gRecBuf;
SEM_ID		msgSemId;

INT32 rioRecMsgIsr(MSG_TRAN_BUF * recBuf, STATUS status, void * cbArg)
{
	RIO_MSG_INT_INF_S	*rioMsgIntInf;

	rioMsgIntInf = &gRioMsgIntInf;

	logMsg("msg receive \n",0,0,0,0,0,0);
	rioMsgIntInf->msgIntInf.buf= recBuf->buf;
	rioMsgIntInf->msgIntInf.len = recBuf->len;
	rioMsgIntInf->msgIntInf.next = recBuf->next;
	semGive(rioMsgIntInf->msgSemId);
}

int rioRecMsg(UINT8 *msgPtr, UINT32 len, UINT16 *srcid, UINT32 timeout)
{
	UINT32 uiRecLen = 0;
	RIO_MSG_INT_INF_S	*rioMsgIntInf;

	rioMsgIntInf = &gRioMsgIntInf;
	while(TRUE)
	{
		if(semTake(rioMsgIntInf->msgSemId,timeout) < 0)
		{
			printf("receive msg timeout \n");
			return ERROR;
		}
		if(rioMsgIntInf->msgIntInf.len > len)
		{
			uiRecLen = len;
		}
		else
		{
			uiRecLen = rioMsgIntInf->msgIntInf.len;
		}
		memcpy(msgPtr,rioMsgIntInf->msgIntInf.buf,uiRecLen);
		logMsg("receive msg addr 0x%x,len 0x%x \n",
			rioMsgIntInf->msgIntInf.buf,uiRecLen,0,0,0,0);
		return uiRecLen;
	}
}

STATUS rioMsgInit(MSG_RXCB_FUNC pfMsgHook)
{
    FUNCPTR             method;
    FUNCPTR		        rmanIntRxInstall;
    VXB_DEVICE_ID       rioRmanDevId = NULL;    
    RIO_TRANS_PARAMETER msgArg;
    MSG_TRANS_API      *msgApi = NULL;


    logMsg("rioMsgInit ..\n", 0, 0, 0, 0, 0, 0);
	if(NULL == (gRioMsgIntInf.msgSemId = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY)))
    {
        return ERROR;
    }	
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
	/*logMsg("SEND msg ok !\n", 0, 0, 0, 0, 0, 0);*/

}
STATUS rioSendMsg(UINT16 localPort,UINT16 mailBox,UINT16 dstID,UINT8 *msgPtr,UINT32 len)
{
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

	uiVal = (tm->year << 16) | (tm->month << 8) | tm->date;
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
    tm->year = (uiVal >> 16) & 0xff;
    tm->month = (uiVal >> 8) & 0x3f;
    tm->date = uiVal & 0x1f;
	
   /*	printf(" 20%02d-%02d-%02d,%02d:%02d:%02d ms %d\n", tm->year, tm->month,
		tm->date, tm->hour, tm->minute, tm->second,tm->century);*/
    return OK;
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




