#include "vxWorks.h"
#include "stdio.h"
#include "hostLoad.h"
#include "assert.h"


extern void sysMsDelay(UINT delay);
#define LOAD_FPGA_MAX_LEN		0x2000000

/***********************************************************
* clock rising edge
***********************************************************/

#if 0
VOID CLKRISE(VOID)
{
    FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,(UINT32)(0xffff << 16));
    
    FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,(UINT32)((0xffff << 16) | 1));
}
/***********************************************************
* host load data prepare
***********************************************************/

VOID hostLoadPrep(VOID)
{
    UINT32  uiCtrl = 0;
    UINT32  uiVal = 0;
    UINT32  uiLoop = 0;
    
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x10);

    uiCtrl = 0x10;
    
    /*
     * PROGB - LOW; CLK - rising edge; PROGB - HIGH  
    */
    uiVal = uiCtrl | (1 << 3)|(0 << HOST_CTRL_RDWR) | (0 << HOST_CTRL_CSI) | 0;
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,uiVal);
	sysMsDelay(1);
    uiVal = uiCtrl | (1 << 3)|(0 << HOST_CTRL_RDWR) | (0 << HOST_CTRL_CSI) | 1;
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,uiVal);
    sysMsDelay(1);
    /*
     * wait INTIB high level
    */
    uiVal= FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG);
 
    while(0 == (uiVal & 0x2))
    {
        uiVal= FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG);
    }
}

/***********************************************************
* write data to FPGA
***********************************************************/

VOID hostLoadData(UINT16 *pusBuf,UINT32  uiLen)
{
    UINT32  uiLoop = 0;
    UINT32  uiData = 0;
    UINT32  uiTmp = 0;
    
    for(uiLoop = 0;uiLoop < uiLen; uiLoop++)
    { 
        uiData  = pusBuf[uiLoop] << 16;
        
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,(uiData  | 1));
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
    }
}

/***********************************************************
* write data to FPGA
***********************************************************/

STATUS hostLoadDone(VOID)
{
    UINT32  uiLoop = 0;
    UINT32  uiData = 0;
    UINT32  uiCtrl = 0;

    uiData = (1 << 3)|(0 << HOST_CTRL_RDWR) | (1 << HOST_CTRL_CSI) | 1;
    for(uiLoop = 0;uiLoop < HOST_LOAD_DONE_MAX; uiLoop++)
    {

        uiData = FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG) & 0x4;
        if(0x0 != uiData)
        {
            logMsg("host load done is received\n",0,0,0,0,0,0);
            break;
        }
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x1);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
    }
    if(HOST_LOAD_DONE_MAX == uiLoop)
    {
        logMsg("wait host load done failed\n",0,0,0,0,0,0);
        return ERROR;
    }
#if 1
    uiCtrl = FSL_READ_REG(HOST_LOAD_CTRL_REG);
    uiCtrl = uiCtrl & (~7);
    uiCtrl |= 0x10;
    uiData = uiCtrl | (1 << 3)|(0 << HOST_CTRL_RDWR) | (1 << HOST_CTRL_CSI) | 1;
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,uiData);
    uiData = uiCtrl | (1 << 3)|(1 << HOST_CTRL_RDWR) | (1 << HOST_CTRL_CSI) | 1;
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,uiData);
#endif

    for(uiLoop = 0;uiLoop < HOST_LOAD_CLK_AFDONE; uiLoop++)
    {
        CLKRISE();
    }

    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0);
    
    return OK;
}

/***********************************************************
* host load main program
***********************************************************/

/*****************************************************************************
 Prototype	  : hotHostLoadFile
 Description  : 
 Input		  : 
 Output 	  : 
 Return Value : 
 Calls		  : 
 Called By	  : 
 
History 	  :
1.Date		  : 2014/1/21
Author		  : wangdongyang
Modification  : Created function
*****************************************************************************/
STATUS hotHostLoadFile(char *pucFileName)
{
    FILE            *pstFp;
    UINT16        usBuf[HOST_DATA_BUF_LEN] = {0};
    UINT32        usFileLen = 0;
    UINT32        uiReadLen = 0;
    UINT32        uiData = 0;
    STATUS        loadStatus = 0;
    UINT32        uiLoop = 0;
	
    UINT32        busNo = 4;
    UINT32        deviceNo = 0;
    
    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);

		 
    taskDelay(sysClkRateGet()/2);
	
    hostLoadPrep();
    do
    {
        uiLoop++;
        uiReadLen = fread(usBuf,sizeof(UINT16),HOST_DATA_BUF_LEN,pstFp);
        hostLoadData(usBuf,uiReadLen);
    }while(HOST_DATA_BUF_LEN == uiReadLen);
    
    loadStatus = hostLoadDone();
    fclose(pstFp);

    taskDelay(sysClkRateGet()); /*加延时, 以便FPGA真正连上了再设置*/

	
    return loadStatus;
}



STATUS hostLoadFile(char *pucFileName)
{
    FILE         *pstFp;
    UINT16        usBuf[HOST_DATA_BUF_LEN] = {0};
    UINT32        usFileLen = 0;
    UINT32        uiReadLen = 0;
    UINT32        uiData = 0;
    STATUS        loadStatus = 0;
    UINT32        uiLoop = 0;
    
    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);
    hostLoadPrep();
    do
    {
        uiLoop++;
        uiReadLen = fread(usBuf,sizeof(UINT16),HOST_DATA_BUF_LEN,pstFp);
        hostLoadData(usBuf,uiReadLen);
    }while(HOST_DATA_BUF_LEN == uiReadLen);
    
    loadStatus = hostLoadDone();
	for(uiLoop = 0;uiLoop < 200; uiLoop++)
    { 
        uiData  = 0;
 
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,(uiData  | 1));
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
    }
    fclose(pstFp);
    taskDelay(60);
    return loadStatus;
}
#else

VOID hostLoadPrep(VOID)
{
    UINT32  uiCtrl = 0;
    UINT32  uiVal = 0;
    UINT32  uiLoop = 0;
    
    
    /*
     * PROGB - LOW; CLK - rising edge; PROGB - HIGH  
    */
    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1f);
	sysMsDelay(1);
	FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1e);/*复位*/
	sysMsDelay(1);
	FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1f);/*停止复位*/
	sysMsDelay(1);

    /*
     * wait INTIB high level
    */
    uiVal= FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG);
 
    while(0 == (uiVal & 0x2))
    {
        uiVal= FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG);
    }

   FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x19);/*写片选与读写使能*/
   sysMsDelay(1);
   
}


/***********************************************************
* write data to FPGA
***********************************************************/

VOID hostLoadData(UINT16 *pusBuf,UINT32  uiLen)
{     UINT32  uiLoop = 0;
    UINT32  uiData = 0;
    UINT32  uiTmp = 0;
    
    for(uiLoop = 0;uiLoop < uiLen; uiLoop++)
    { 
        uiData  = pusBuf[uiLoop] << 16;
        
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
 
        FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,(uiData  | 1));

		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,uiData);
    }
}
STATUS hostLoadDone(VOID)
{     UINT32  uiLoop = 0;
    UINT32  uiData = 0;
    UINT32  uiCtrl = 0;

    for(uiLoop = 0;uiLoop < HOST_LOAD_DONE_MAX; uiLoop++)
    {

        uiData = FSL_READ_REG((UINT32*)HOST_LOAD_STAT_REG) & 0x4;
        if(0x0 != uiData)
        {
            logMsg("host load done is received \n",
					0,0,0,0,0,0);
            break;
        }

		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x1);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
    }
	/*继续写200个上升沿*/
    if(HOST_LOAD_DONE_MAX == uiLoop)
    {
        logMsg("wait host load done failed\n",0,0,0,0,0,0);
        return ERROR;
    }
	for(uiLoop = 0;uiLoop < 200; uiLoop++)
    {
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x1);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_DATA_REG,0x0);
    }

    FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x7);
    
    return OK;
}

STATUS hostLoadFile(char *pucFileName)
{
    FILE          *pstFp;
    UINT32        usFileLen = 0;
    UINT32        uiReadLen = 0;
    STATUS        loadStatus = 0;
    UINT32        uiLoop = 0;
    char		  *testBuf = NULL,*curbuf = NULL;
	UINT32 time =0;
#if 1
	time = tickGet();
     pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1f);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1e);/*复位*/
		sysMsDelay(1);
		FSL_WRITE_REG((UINT32*)HOST_LOAD_CTRL_REG,0x1f);/*停止复位*/
		sysMsDelay(1);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);
	hostLoadPrep();

	testBuf = malloc(LOAD_FPGA_MAX_LEN);
	if(testBuf == NULL )
	{
		printf("malloc buf error\n");
		return ERROR;
	}
	uiReadLen = fread(testBuf,sizeof(UINT16),LOAD_FPGA_MAX_LEN/2,pstFp);
	curbuf = testBuf;
	logMsg("open file time %d\n",(tickGet()- time),0,0,0,0,0);
	time = tickGet();
    do
    {
	    if(uiReadLen > HOST_DATA_BUF_LEN)
    	{
	    	usFileLen = HOST_DATA_BUF_LEN;
    	}
		else
		{
			usFileLen = uiReadLen;
		}
		uiLoop += usFileLen;
        hostLoadData(curbuf,usFileLen);
		curbuf += usFileLen*sizeof(UINT16);
		uiReadLen -= usFileLen;
    }while(uiReadLen > 0);
	loadStatus = hostLoadDone();
	logMsg("load t %d\n",(tickGet()- time),0,0,0,0,0);
	fclose(pstFp);
	free(testBuf);
#else
    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);
	hostLoadPrep();

	do
	{
		uiReadLen = fread(usBuf,sizeof(UINT16),HOST_DATA_BUF_LEN,pstFp);
		hostLoadData(usBuf,uiReadLen);
		uiLoop += uiReadLen;		
		
	}while(HOST_DATA_BUF_LEN == uiReadLen);
	printf("write len 0x%x \n",uiLoop);

    loadStatus = hostLoadDone();
	fclose(pstFp);
#endif
	taskDelay(60);/*等待FPGA 与PCIE交换初始化成功*/
    return loadStatus;
}

STATUS hotHostLoadFile(char *pucFileName)
{
    FILE            *pstFp;
    UINT16        usBuf[HOST_DATA_BUF_LEN] = {0};
    UINT32        usFileLen = 0;
    UINT32        uiReadLen = 0;
    UINT32        uiData = 0;
    STATUS        loadStatus = 0;
    UINT32        uiLoop = 0;
	
    UINT32        busNo = 4;
    UINT32        deviceNo = 0;
    
    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);

		 
    taskDelay(sysClkRateGet()/2);
	
    hostLoadPrep();
    do
    {
        uiLoop++;
        uiReadLen = fread(usBuf,sizeof(UINT16),HOST_DATA_BUF_LEN,pstFp);
        hostLoadData(usBuf,uiReadLen);
    }while(HOST_DATA_BUF_LEN == uiReadLen);
    
    loadStatus = hostLoadDone();
    fclose(pstFp);

    taskDelay(sysClkRateGet()); /*加延时, 以便FPGA真正连上了再设置*/

	
    return loadStatus;
}

#endif
#if 0
STATUS hostLoadPart(UINT32 uiPart)
{
    UINT16 *pusBuf = NULL;
    UINT32  uiLen = 0;
    UINT32  uiPartOffset = 0;
    STATUS  loadStatus = 0;
    UINT32  uiLoop = 0;

    assert(uiPart < 8);
    
    uiPartOffset = M8641_USR_ADDR + uiPart * M8641_USR_LEN;
    setLBCTOCurrentCPU();
    uiLen = *((UINT32*)(uiPartOffset + M8641_USR_LEN_OFFSET));
    pusBuf = malloc(uiLen);
    if(NULL == pusBuf)
    {
        sysPrintf("malloc memory failed\n");
        return ERROR;
    }
    memcpy (pusBuf,(uiPartOffset + M8641_USR_LEN_BYTE),uiLen);
    
    hostLoadPrep();
    hostLoadData(pusBuf,(uiLen/sizeof(UINT16)));
    loadStatus = hostLoadDone();
    free(pusBuf);

    return loadStatus;
}
#endif
/***********************************************************
* host load main program
***********************************************************/

VOID hostDummp(VOID)
{
    /*logMsg("test is ok\n",0,0,0,0,0,0);*/
}

