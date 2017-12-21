#include "../fsInc.h"
#include "stdio.h"
#include "usrLib.h"
#include "dirent.h"
#include "semLib.h"
#include "ioLib.h"
#include "taskLib.h"
#include "time.h"
#include "vxWorks.h"
#include "stdio.h"
#include "sysLib.h"
/*#include "ppcApi.h"*/

STATUS fsFatClearTest(UINT32 vuiDevId,UINT32 vuiFirstClu)
{
    FS_VOL_S *pstVol = NULL;
    UINT32    uiVal = 0;

    pstVol = &gstFsCtrl.astVol[0];
    
    if(OK != fsClearEntChain(pstVol,vuiFirstClu))
    {
        FS_LOG(FS_DBG_ERR,"clear dev %d fat failed\n",vuiDevId,0,0,0,0,0);
        return ERROR;
    }

    FS_LOG(FS_DBG_ERR,"clear dev %d fat sucefully\n",vuiDevId,uiVal,0,0,0,0); 
    
    return OK;
}

STATUS fsFdtWriteTest(UINT32 vuiDevId,CHAR *vpcName,UINT32 vuiClustId,UINT32 uiOffset)
{
    FS_VOL_S    *pstVol = NULL;
    FS_FDT_S     stFDT;
    UINT32       uiVal = 0;

    pstVol = &gstFsCtrl.astVol[0];
    
    bzero((CHAR*)&stFDT,sizeof(FS_FDT_S));
    
    stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
    strcpy(stFDT.aucName,vpcName);
    stFDT.ullFileSize.lo = 0x100000;
    stFDT.ullFileSize.hi = 0;
    stFDT.uiFirstClu = FS_CLUST_INVALID;
    
    if(OK != gstFsCtrl.stFDTFunc.fsWriteFDT(pstVol,&stFDT,vuiClustId,uiOffset))
    {
        FS_LOG(FS_DBG_ERR,"write FDT failed\n",vuiDevId,0,0,0,0,0);
        return ERROR;
    }

	FS_LOG(FS_DBG_ERR,"write FDT %p\n",&stFDT,0,0,0,0,0);
	
    return OK;
}

STATUS fsFdtReadTest(UINT32 vuiDevId,UINT32 vuiClustId,UINT32 vuiOffset)
{
    FS_VOL_S    *pstVol = NULL;
    FS_FDT_S     stFDT;
    UINT32       uiVal = 0;

    pstVol = &gstFsCtrl.astVol[0];
    
    bzero((CHAR*)&stFDT,sizeof(FS_FDT_S));
    
    if(OK != gstFsCtrl.stFDTFunc.fsReadFDT(pstVol,&stFDT,vuiClustId,vuiOffset))
    {
        FS_LOG(FS_DBG_ERR,"read FDT failed\n",vuiDevId,0,0,0,0,0);
        return ERROR;
    }

    if(FS_FILE_STAT_SUBDIR == stFDT.ucFileStat)
    {
        FS_LOG(FS_DBG_INFO,"read FDT %s stat 0x%x offset 0x%x size 0x%x attr %x\n",
           stFDT.aucName,stFDT.ucFileStat,vuiOffset,(UINT32)(stFDT.ullFileSize.lo),
           stFDT.ucAttr,0);
    }
    else
    {
        FS_LOG(FS_DBG_INFO,"read FDT failed\n",0,0,0,0,0,0);
    }
    FS_LOG(FS_DBG_ERR,"read FDT %p\n",&stFDT,0,0,0,0,0);
    return OK;
}


STATUS fsFdtFindTest(UINT32 vuiDevId,CHAR *vpcName,UINT32  vuiClu)
{
    FS_VOL_S         *pstVol = NULL;
    FS_FDT_INFO_S     stFDTInfo;
    UINT32            uiVal = 0;

    pstVol = &gstFsCtrl.astVol[0];

    bzero((CHAR*)&stFDTInfo,sizeof(FS_FDT_INFO_S));
    if(OK != fsGetFile(pstVol,"/",1,&stFDTInfo))
    {
        FS_LOG(FS_DBG_ERR,"get dev %d fat failed\n",vuiDevId,0,0,0,0,0);
        return ERROR;
    }

    FS_LOG(FS_DBG_ERR,"get dev %d fdt %s clust %d offset 0x%x attr %x %x\n",vuiDevId,
           stFDTInfo.stFDT.aucName,stFDTInfo.uiClu,stFDTInfo.uiCluOffset,
           stFDTInfo.stFDT.ucAttr,stFDTInfo.stFDT.ucFileStat); 
}

STATUS fsFdtListTest(UINT32 vuiDevId,UINT32  vuiClu)
{
    FS_VOL_S         *pstVol = NULL;
    FS_FDT_INFO_S     stFDTInfo;
    UINT32            uiVal = 0;

    pstVol = &gstFsCtrl.astVol[0];

    bzero((CHAR*)&stFDTInfo,sizeof(FS_FDT_INFO_S));
    if(OK != gstFsCtrl.stFDTFunc.fsListFDT(pstVol,vuiClu))
    {
        FS_LOG(FS_DBG_ERR,"get dev %d fat failed\n",vuiDevId,0,0,0,0,0);
        return ERROR;
    }

    FS_LOG(FS_DBG_ERR,"list dev %d succeed\n",vuiDevId,0,0,0,0,0); 

    return OK;
}

STATUS fsCopyFile(CHAR *vpcInName,CHAR *vpcOutName)
{
    FILE     *pstRdFd = NULL;
    FILE     *pstWrFd = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiRdLen = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;

    uiByteNum = 1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
   
    pstRdFd = fopen(vpcInName,"r");
    if(NULL == pstRdFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        free(pucBuf);
        return ERROR;
    }
    
    pstWrFd = fopen(vpcOutName,"w+b");
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        fclose(pstRdFd);
        free(pucBuf);
        return ERROR;
    }
    
    uiRdLen = uiByteNum;
    do
    {
        uiRdLen = fread(pucBuf,1,uiByteNum,pstRdFd);
        uiWrLen = fwrite(pucBuf,1,uiRdLen,pstWrFd);
        if(0 == uiWrLen)
        {
            printf("write file %d failed\n",uiLoop);
            break;
        }
        uiLoop++;
    }while(uiRdLen == uiByteNum);

    printf("open file succeed %p buf %x loop %d\n",pstWrFd,pucBuf,uiLoop);
    free(pucBuf);
    pucBuf = NULL;
    fclose(pstWrFd);
    fclose(pstRdFd);
   
    printf("test end\n");
}

STATUS fsMulCopyFile(CHAR *vpcInName,CHAR *vpcOutName,UINT32 uiMem)
{
    FILE     *pstRdFd = NULL;
    FILE     *pstWrFd = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiRdLen = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    tickStart = 0;
    UINT32    tickEnd = 0;
    UINT32    tickSpend = 0;

    uiByteNum = uiMem*1024*1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
    
    tickStart = tickGet();
    pstRdFd = open (vpcInName,O_RDONLY, 0644);
    if(NULL == pstRdFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        free(pucBuf);
        return ERROR;
    }
    /*pstWrFd = open (vpcOutName,O_CREAT | O_RDWR, 0644);
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        close(pstRdFd);
        free(pucBuf);
        return ERROR;
    }*/
    
    uiRdLen = uiByteNum;
    do
    {
        uiRdLen = read(pstRdFd,pucBuf,uiByteNum);
        /*uiWrLen = write(pstWrFd,pucBuf,uiRdLen);
        if(0 == uiWrLen)
        {
            printf("write file %d failed\n",uiLoop);
            break;
        }
        if(0 == (uiLoop % 1024))
        {
            printf("read file %d \n",(uiLoop/1024));
        }*/
        uiLoop++;
    }while(uiRdLen == uiByteNum);
    tickEnd = tickGet();
    close(pstRdFd);
    /*close(pstWrFd);*/
    tickSpend = (tickEnd - tickStart)/60;
    
    /*printf("open file succeed %p buf %x loop %d %d MByte/s %ds\n",pstWrFd,pucBuf,
           uiLoop,((uiLoop*uiMem)/tickSpend),tickSpend);*/
    free(pucBuf);
    pucBuf = NULL;
    /*printf("test end\n");*/
}

STATUS fsWrFile(CHAR *vpcInName)
{
    INT32     pstWrFd = NULL;
    UINT8    *pucAlign = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiOffset = 0;
    UINT32    uiAddr = 0;
    UINT32    uiLoop = 0;
#if 1
    uiByteNum = 6*1024 * 1024;
    pucBuf = malloc(uiByteNum + 0x10);
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    uiAddr = (UINT32)pucBuf;
    uiAddr = uiAddr + 0xf;
    uiAddr = uiAddr & (~0xf);
    pucAlign = uiAddr;
    for(uiLoop = 0;uiLoop < uiByteNum;uiLoop++)
    {
        pucAlign[uiLoop] = uiLoop;
    }
   #endif
    /*bzero((CHAR*)pucBuf,uiByteNum);*/
    printf("memory %p \n",pucAlign);
    
    pstWrFd = open(vpcInName,0x209,0);
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        fclose(pstWrFd);
        free(pucBuf);
        return ERROR;
    }
    /*do 8G*/
    uiByteNum = 1*1024 * 1024;
    for(uiLoop = 0;uiLoop < 4;uiLoop++)
    {
        uiWrLen = write(pstWrFd,pucAlign,uiByteNum);
        if(0 == (uiLoop % 1024))
        {
            logMsg("write loop %d \n",(uiLoop / 1024),0,0,0,0,0);
        }
    }

    /*free(pucBuf);*/
    printf("write test ok\n",pucAlign);
    close(pstWrFd);

    return OK;
}

STATUS fsCheckMem(UINT8 *pucBuf,UINT32 uiLen)
{
    UINT32 uiLoop = 0;
    UINT8  uiVal = 0;

    for(uiLoop = 0;uiLoop < uiLen;uiLoop++)
    {
        if(uiVal != pucBuf[uiLoop])
        {
            return ERROR;
        }
        uiVal++;
    }

    return OK;
}

STATUS fsCheckFile(CHAR *vpcInName)
{
    INT32     pstRdFd = NULL;
    UINT8    *pucAlign = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiRdLen = 0;
    UINT32    uiOffset = 0;
    UINT32    uiAddr = 0;
    UINT32    uiLoop = 0;

    uiByteNum = 6*1024 * 1024;
    pucBuf = malloc(uiByteNum + 0x10);
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    uiAddr = (UINT32)pucBuf;
    uiAddr = uiAddr + 0xf;
    uiAddr = uiAddr & (~0xf);
    pucAlign = uiAddr;
    for(uiLoop = 0;uiLoop < uiByteNum;uiLoop++)
    {
        pucAlign[uiLoop] = uiLoop;
    }
   
    /*bzero((CHAR*)pucBuf,uiByteNum);*/
    printf("memory %p \n",pucAlign);
    
    pstRdFd = open(vpcInName,0x209,0);
    if(NULL == pstRdFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        fclose(pstRdFd);
        free(pucBuf);
        return ERROR;
    }

    /*do 8G*/
    for(uiLoop = 0;uiLoop < 1024*8;uiLoop++)
    {
        uiRdLen = read(pstRdFd,pucBuf,uiByteNum);
        taskDelay(13);
        if(0 == (uiLoop % 512))
        {
            logMsg("read loop %d len %d\n",(uiLoop / 512),uiRdLen,0,0,0,0);
        }
        /*if(OK != fsCheckMem(pucBuf,uiByteNum))
        {
            printf("check file failed\n");
            free(pucBuf);
            return ERROR;
        }*/
    }

    free(pucBuf);
    printf("check file %s ok\n",vpcInName);
    close(pstRdFd);
}

STATUS taskFdWr(CHAR *vpcInName)
{
    UINT32 uiLoop = 0;
    
    #if 1
    if(OK != fsWrFile(vpcInName))
    {
        return ERROR;
    }
    
    #else
    for(uiLoop= 0;uiLoop < 64;uiLoop++)
    {
        uiClu++;
        fsCluRW(vpcInName,uiClu,1024*1024,0,0x10000000,2);
    }
    printf("file %s test ok\n",vpcInName);
    #endif
    
}

STATUS taskFdRd(CHAR *vpcInName)
{
    UINT32 uiLoop = 0;
    
    #if 1
    if(OK != fsCheckFile(vpcInName))
    {
        return ERROR;
    }
    #else
    for(uiLoop= 0;uiLoop < 64;uiLoop++)
    {
        uiClu++;
        fsCluRW(vpcInName,uiClu,1024*1024,0,0x10000000,2);
    }
    printf("file %s test ok\n",vpcInName);
    #endif
    
}
extern STATUS blkPerfTest(FS_VOL_S *pstVol,UINT32 vuiOper,UINT32 dataOption);


UINT8  gpucBuf[16][16];
INT8   gpucName[16][16];

STATUS fsOpenFileTest(void)
{
    INT32 pstWrFd = 0;
    UINT32 tickStart = 0;
    UINT32 tickEnd = 0;
    
    tickStart = tickGet(); 
    pstWrFd = open("/vol/t1",0x209,0);
    pstWrFd = open("/vol/t2",0x209,0);
    pstWrFd = open("/vol/t3",0x209,0);
    pstWrFd = open("/vol/t4",0x209,0);
    pstWrFd = open("/vol/t5",0x209,0);
    pstWrFd = open("/vol/t6",0x209,0);
    pstWrFd = open("/vol/t7",0x209,0);
    pstWrFd = open("/vol/t8",0x209,0);
    pstWrFd = open("/vol/t9",0x209,0);
    pstWrFd = open("/vol/t10",0x209,0);
    pstWrFd = open("/vol/t11",0x209,0);
    pstWrFd = open("/vol/t12",0x209,0);
    pstWrFd = open("/vol/t13",0x209,0);
    pstWrFd = open("/vol/t14",0x209,0);
    pstWrFd = open("/vol/t15",0x209,0);
    pstWrFd = open("/vol/t16",0x209,0);
    pstWrFd = open("/vol/t17",0x209,0);
    pstWrFd = open("/vol/t18",0x209,0);
    pstWrFd = open("/vol/t19",0x209,0);
    pstWrFd = open("/vol/t20",0x209,0);
    tickEnd = tickGet();

    printf("creat file tick %d\n",(tickEnd - tickStart));
}
    

void multiFdTest(UINT32 uiTaskNum)
{
    UINT32 uiLoop = 0;
    
    /*for(uiLoop = 0;uiLoop < uiTaskNum;uiLoop++)
    {
        memset(gpucBuf[uiLoop],0,8);
        memset(gpucName[uiLoop],0,16);
        sprintf (gpucBuf[uiLoop], "task%x",uiLoop);
        sprintf (gpucName[uiLoop], "/vol/f%x",uiLoop);
        
        taskSpawn(gpucBuf[uiLoop],100,0,0x10000,(FUNCPTR)taskFd,gpucName[uiLoop],
                  0,0,0,0,0,0,0,0,0);
    }*/
    
    memset(gpucBuf[0],0,8);
    memset(gpucName[0],0,16);
    sprintf (gpucBuf[0], "task%x",0);
    sprintf (gpucName[0], "/vol/f%x.bin",0);

    memset(gpucBuf[1],0,8);
    memset(gpucName[1],0,16);
    sprintf (gpucBuf[1], "task%x",1);
    sprintf (gpucName[1], "/vol/f%x",1);
#if 1        
    taskSpawn(gpucBuf[0],110,0,0x10000,(FUNCPTR)fsWrFile,gpucName[0],
                  0,0,0,0,0,0,0,0,0);
    taskSpawn(gpucBuf[1],100,0,0x10000,(FUNCPTR)fsWrFile,gpucName[1],
                  0,0,0,0,0,0,0,0,0);
#else
    taskSpawn(gpucBuf[0],110,0,0x10000,(FUNCPTR)blkPerfTest,uiTaskNum,
                  2,0,0,0,0,0,0,0,0);
    taskSpawn(gpucBuf[1],100,0,0x10000,(FUNCPTR)blkPerfTest,uiTaskNum,
                  2,0,0,0,0,0,0,0,0);
#endif                  
}

/*multiFdTest*/
/*rioTestPort 50000,0,10*/
STATUS fsPerfTest(CHAR *vpcOutName,UINT32 vuiOper)
{
    FILE     *pstWrFd = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    uiClu = 0x18;
    UINT32  beginTimeH = 0;
    UINT32  beginTimeL = 0;
	UINT32  endTimeH = 0;
    UINT32  endTimeL = 0;
    UINT32      tickStart = 0;
    UINT32      tickEnd = 0;
    UINT32      tickSpend = 0;
    float   timeSpend = 0;

    uiByteNum = 1*1024 * 1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
    for(uiLoop = 0;uiLoop < (1*1024*1024);uiLoop++)
    {
        pucBuf[uiLoop] = rand();/*0*/
    }
    
    printf("test begin\n");

    pstWrFd = fopen(vpcOutName,"w+b");
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        free(pucBuf);
        return ERROR;
    }
    
    vxTimeBaseGet(&beginTimeH,&beginTimeL);
    tickStart = tickGet();
    for(uiLoop = 0;uiLoop < 4096;uiLoop++)
    {
        uiWrLen = fwrite(pucBuf,uiByteNum,1,pstWrFd);
        if(0 == uiWrLen)
        {
            printf("fsPerfTest file %d failed\n",uiLoop);
            break;
        }
        /*uiClu = 30;
        fsCluRW(vuiOper,uiClu,uiByteNum,0,pucBuf,2);*/
    }
    vxTimeBaseGet(&endTimeH,&endTimeL);
    tickEnd = tickGet();
    tickSpend = (tickEnd-tickStart);

    free(pucBuf);
    pucBuf = NULL;
    fclose(pstWrFd);

    printf("--tick dma write speed %dMB/s %ds ok\n",(4096* 60)/(tickSpend),
           (tickSpend/60));
    timeSpend = (endTimeH -beginTimeH )*4*1024*1024*1024 + (endTimeL-beginTimeL);
	timeSpend = timeSpend*8*2/1000000000;

    printf("dma write speed %fMB/s %f ok\n",(1*4096)/timeSpend,timeSpend);  
}
UINT32    guiClu = 0;

STATUS fsPcieTest(CHAR *vpcOutName,UINT8  *pucBuf,UINT32 deviceSel,UINT32 uiMem)
{
    FILE     *pstWrFd = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    tickStart = 0;
    UINT32    tickEnd = 0;
	UINT32   *puiAllocBuf = NULL;
	UINT32   *puiBuf = NULL;
	UINT32    uiAddr = 0x48000000;
	
    UINT32   timeSpend = 0;

    uiByteNum = uiMem*1024 * 1024;
    
    printf("test begin\n");

	puiAllocBuf = malloc((uiByteNum));
    if(NULL == puiAllocBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)puiAllocBuf,uiByteNum);
	puiBuf = (UINT32*)((UINT32)puiAllocBuf & (~0xfffff)); 
		
    pstWrFd = fopen(vpcOutName,"w+b");
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    tickStart = tickGet();
    for(uiLoop = 0;uiLoop < 1024*8;uiLoop++)
    {
        uiWrLen = fwrite(pucBuf,uiByteNum,1,pstWrFd);
        if(0 == uiWrLen)
        {
            printf("write file %d failed\n",uiLoop);
            break;
        } 
        if(0x0 == (uiLoop % 10240))
        {
            printf("loop %d\n",uiLoop/10240);
        }
    }
    tickEnd = tickGet();
    fclose(pstWrFd);
	timeSpend = (tickEnd-tickStart);

    printf("dma write speed %dMB/s %ds ok\n",(1024*8*uiMem * 1000)/(timeSpend),
           (timeSpend/1000));  
}

STATUS fsPcieTest1(CHAR *vpcOutName,UINT8  *pucBuf,UINT32 deviceSel,UINT32 uiMem)
{
#if 0
    FILE     *pstWrFd = NULL;
	FILE     *pstWrFd1 = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    tickStart = 0;
    UINT32    tickEnd = 0;
	UINT32    uiAddr =  0x50000000;
	UINT32    uiAddr1 = 0x51000000;

    UINT32   timeSpend = 0;

    uiByteNum = uiMem*1024 * 1024;
    
    /*printf("test begin\n");*/

    pstWrFd = fopen(vpcOutName,"w+b");
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
	
	pstWrFd1 = fopen("/vol/xxx.bin","w+b");
    if(NULL == pstWrFd1)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
	
    for(uiLoop = 0;uiLoop < (uiMem*1024*1024);uiLoop++)
    {
        /*if(deviceSel==0)
            pucBuf[uiLoop] = uiLoop;*/
    }
    tickStart = tickGet(); 
	FSL_WRITE_REG(0x4010000c,0xc);
	
    for(uiLoop = 0;uiLoop < 200;uiLoop++)
    {
		if(gstExtInt.uiIntChan & 1)
		{
			FSL_WRITE_REG(0x40100000,uiAddr);
			FSL_WRITE_REG(0x40100004,1);

			uiWrLen = fwrite(uiAddr,uiByteNum,1,pstWrFd);
			uiAddr += 0x200000;
    		if(0x51000000 == uiAddr)
    		{
    			uiAddr = 0x50000000;
    		}
		}
		else
		{
			FSL_WRITE_REG(0x40100000,uiAddr1);
			FSL_WRITE_REG(0x40100004,1);
			uiWrLen = fwrite(uiAddr1,uiByteNum,1,pstWrFd1);
			uiAddr1 += 0x200000;
    		if(0x52000000 == uiAddr1)
    		{
    			uiAddr1 = 0x51000000;
    		}
		}
        
		guiClu++;
        if(0 == uiWrLen)
        {
            printf("write file %d failed\n",uiLoop);
            break;
        } 
        if(0x0 == (uiLoop % 10240))
        {
            printf("loop %d\n",uiLoop/10240);
        }
    }
	FSL_WRITE_REG(0x4010000c,0xf);
    tickEnd = tickGet();
    fclose(pstWrFd);
	fclose(pstWrFd1);
	timeSpend = (tickEnd-tickStart);

    printf("dma write speed %dMB/s %ds ok\n",(1024*8*uiMem * 60)/(timeSpend),
           (timeSpend/60));  
#endif	
}


void testMem(UINT8  *pucBuf,UINT32 uiMem)
{
    UINT32    uiLoop = 0;

    for(uiLoop = 0;uiLoop < (uiMem*1024*1024);uiLoop++)
    {
         pucBuf[uiLoop] = uiLoop;
    }
}

void timeTest(void)
{
    UINT32  tickStart = 0;
    UINT32  tickEnd = 0;

    UINT32    timeSpend = 0;

    
    tickStart=tickGet();
    printf("tickStart=%d\n",tickStart); 
    taskDelay(120);
    tickEnd=tickGet();
    printf("tickEnd=%d\n",tickEnd); 
    timeSpend = (tickEnd-tickStart)/60;

    printf("dma write speed %dMB/s %d ok\n",(1024*110)/timeSpend,timeSpend); 
    
}



STATUS blkPerfTest(FS_VOL_S *pstVol,UINT32 vuiOper,UINT32 dataOption)
{    
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiLoop = 0;
    UINT32    tickStart = 0;
    UINT32    tickEnd = 0;
    INT32     pstWrFd = 0;
    UINT32    timeSpend = 0;
    FS_RECOVER_S  stRecv;
    
    uiByteNum = 1*1024 * 1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    pucBuf = (UINT8*)0x18000000;
    /*for(uiLoop = 0;uiLoop < (1024*1024);uiLoop++)
    {
        if(dataOption==0)
            pucBuf[uiLoop] = rand();
        else
            pucBuf[uiLoop] = 0;
    }*/
#if 0
    pstWrFd = open("/vol/f0.bin",0x209,0);
    if(NULL == pstWrFd)
    {
        FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
#endif
    printf("test buf 0x%x begin\n",&stRecv);
    
    fsCluRW(pstVol,1,132,0,&stRecv,2);
               
    tickStart=tickGet();
    /*for(uiLoop = 0;uiLoop < 1024*8;uiLoop++)
    {
        fsCluRW(pstVol,0xd,uiByteNum,0,pucBuf,vuiOper);
        if(0x0 == (uiLoop % 10240))
        {
            printf("loop %d\n",uiLoop/10240);
        }
        guiClu++;
    }*/
    tickEnd=tickGet();

	timeSpend = (tickEnd-tickStart)/60;

    printf("dma write speed %dMB/s %d ok\n",(1024*8*3)/timeSpend,timeSpend);             
}

STATUS task(CHAR *vpcInName)
{
   UINT32 uiLoop = 0;
 
    while(1)
    {
        if(0x0 == (uiLoop % 100000))
        {
            logMsg("task is running\n",0,0,0,0,0,0);
        }
    }
    
}

STATUS task1(CHAR *vpcInName)
{
    #if 0
    UINT32 uiLoop = 0;
 
    while(1)
    {
        if(0x0 == (uiLoop % 100000))
        {
            logMsg("task1 is running\n",0,0,0,0,0,0);
        }
    }
    #endif
}


void taskTest(UINT32 uiTaskNum)
{
    UINT32 uiLoop = 0;

    for(uiLoop = 0;uiLoop < 100;uiLoop++)
    {
        taskSpawn("task1",50,0,0x10000,(FUNCPTR)task1,0,0,0,0,0,0,0,0,0,0);
        taskDelay(1);
    }
    
    /*taskSpawn("task",100,0,0x10000,(FUNCPTR)task,0,0,0,0,0,0,0,0,0,0);*/
}

STATUS fsNfsTest(CHAR *vpcOutName)
{
    FILE     *pstWrFd = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;

    uiByteNum = 1024 * 1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
    for(uiLoop = 0;uiLoop < (1024*1024);uiLoop++)
    {
        pucBuf[uiLoop] = rand();
    }
    
    printf("test begin\n");
    for(uiLoop = 0;uiLoop < 512;uiLoop++)
    {
        printf("uiLoop %d\n",uiLoop);
        pstWrFd = fopen(vpcOutName,"a");
        if(NULL == pstWrFd)
        {
            FS_LOG(FS_DBG_ERR,"open file failed\n",0,0,0,0,0,0);
            free(pucBuf);
            return ERROR;
        }
        
        uiWrLen = fwrite(pucBuf,uiByteNum,1,pstWrFd);
        if(0 == uiWrLen)
        {
            printf("fsPerfTest file %d failed\n",uiLoop);
            break;
        } 
        fclose(pstWrFd);
    }
    
    free(pucBuf);
    pucBuf = NULL;
}

void ullAddTest(UINT32 uiHi,UINT32 uiLo,UINT32 uiAdd)
{
    UINT64_S ullRst;

    ullRst.hi = uiHi;
    ullRst.lo = uiLo;
    UINT64_ADD(&ullRst,&ullRst,uiAdd);
    
    printf("test result %x %x %d\n",ullRst.hi,ullRst.lo,sizeof(FS_FDT_S));
}

void testStat(void)
{
    struct stat stStat;
    long long test = 0x1234567898765432;
    long long test1 = 0;
    UINT32 lo = 0;
    UINT32 hi = 0;
    
    bzero ((char *) &stStat, sizeof (struct stat));

    hi = (UINT32)(test / 0x100000000);
    lo = (UINT32)(test & 0xffffffff);
    
    stStat.st_size = 0x9853ea;
	stStat.st_blksize = 0x100000;

    printf("int64 %d %d long %d long long %d hi 0x%x lo 0x%x\n",
           sizeof(INT64),sizeof(blksize_t),
           sizeof(long),sizeof(long long),hi,lo);
    printf("size 0x%llx blksize %d\n",stStat.st_size,
           stStat.st_blksize);
}

ULONG fsPrintFileStat(char  *fileName)
{
    struct stat  fileStat; 
    
    if (stat (fileName, &fileStat) != OK)
    {
        return 0;
    }
    
    printf("fileStat.st_size  =0x%llx\n",fileStat.st_size);
    printf("fileStat.st_dev   =0x%x\n",fileStat.st_dev);
    printf("fileStat.st_ino   =%x\n",fileStat.st_ino);
    printf("fileStat.st_mode  =%x\n",fileStat.st_mode);
    printf("fileStat.st_nlink  =%x\n",fileStat.st_nlink);
    printf("fileStat.st_uid  =%x\n",fileStat.st_uid);
    printf("fileStat.st_gid  =%x\n",fileStat.st_gid);
    printf("fileStat.st_rdev  =%x\n",fileStat.st_rdev);
    printf("fileStat.st_atime  =%x\n",fileStat.st_atime);
    printf("fileStat.st_mtime  =%x\n",fileStat.st_mtime);
    printf("fileStat.st_ctime  =%x\n",fileStat.st_ctime);
    printf("fileStat.st_blksize  =%x\n",fileStat.st_blksize);
    printf("fileStat.st_blocks  =%x\n",fileStat.st_blocks);
    printf("fileStat.st_attrib  =%x\n",fileStat.st_attrib);
    printf("fileStat.st_reservered1  =%x\n",fileStat.st_reserved1);
    printf("fileStat.st_reservered2  =%x\n",fileStat.st_reserved2);
    printf("fileStat.st_reservered3  =%x\n",fileStat.st_reserved3);
    printf("fileStat.st_reservered4  =%x\n",fileStat.st_reserved4);
}


#define BUF_SIZE 512

char rdBuf[BUF_SIZE];
char wrBuf[BUF_SIZE];

void fsTest()
{
    int fd = 0;
    int size = 0;
    int loop = 0;

    for(loop = 0;loop < rdBuf;loop++)
    {
        rdBuf[loop] = loop;
    }
    
    fd = open("/vol/test.bin",O_WRONLY|O_CREAT,0);
    write(fd,wrBuf,BUF_SIZE);
    close(fd);
    
    fd = open("/vol/test.bin",O_RDONLY,0);
    size = read(fd,rdBuf,BUF_SIZE);
    if(size != BUF_SIZE)
    {
        printf("read file size %d\n",size);
    }
    close(fd);

    if(0 != memcmp(rdBuf,wrBuf,BUF_SIZE))
    {
        printf("check file failed\n");
    }
    else
    {
        printf("check file ok\n");
    }
}

void testx()
{
    printf("sizeof FS_FDT_S %d\n",sizeof(FS_FDT_S));
}

INT32 testCreatFile()
{
    UINT32 uiLoop = 0;
    INT32 fd = 0;
    char filePath[100] = {0};
    FS_FD_S  *pstFd = NULL;
    FS_VOL_S *pstVol = NULL;


    for(uiLoop = 0;uiLoop < 36000;uiLoop++)
    {
        memset(filePath,0,100);
        sprintf(filePath,"/vol/f%d",uiLoop);
        fd = open(filePath,0x209,0);
        if(ERROR == fd)
        {
            logMsg("test creat file error,uiLoop = %d!\n",uiLoop,0,0,0,0,0);
            return ERROR;
        }
        close(fd);
        free(pstFd);
        
    }
    return OK;
}

STATUS fsPerfTest1(CHAR *vpcOutName,UINT32 vuiOper)
{
    FILE     *pstWrFd = NULL;
    UINT8    *pucBuf = NULL;
    UINT32    uiByteNum = 0;
    UINT32    uiWrLen = 0;
    UINT32    uiLoop = 0;
    UINT32    uiClu = 0x18;
    UINT32  beginTimeH = 0;
    UINT32  beginTimeL = 0;
	UINT32  endTimeH = 0;
    UINT32  endTimeL = 0;
    float   timeSpend = 0;
    FS_VOL_S *pstVol = NULL;
    FS_FD_S  *pstFd = NULL;

    pstVol = fsFindVolByName("/vol");/*fsFindVolByName("/vol")*/
    if(NULL == pstVol)
    {
        FS_LOG(FS_DBG_ERR,"vol not exist\n",0,0,0,0,0,0);
        return ERROR;
    }

    uiByteNum = 7*1024 * 1024;
    pucBuf = malloc((uiByteNum));
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc buffer failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    bzero((CHAR*)pucBuf,uiByteNum);
    for(uiLoop = 0;uiLoop < (7*1024*1024);uiLoop++)
    {
        pucBuf[uiLoop] = rand();/**/
    }
    
    printf("test begin\n");

    pstFd = (FS_FD_S*)fsOpen(pstVol,"/vol/test.bin",0x209,0);
    if(NULL == pstFd)
    {
        return ERROR;
    }
    
    vxTimeBaseGet(&beginTimeH,&beginTimeL);
    for(uiLoop = 0;uiLoop < 4096;uiLoop++)
    {
        fsWrite(pstFd,0x48000000,uiByteNum);
    }
    vxTimeBaseGet(&endTimeH,&endTimeL);

    free(pucBuf);
    pucBuf = NULL;
    fclose(pstWrFd);
    
    timeSpend = (endTimeH -beginTimeH )*4*1024*1024*1024 + (endTimeL-beginTimeL);
	timeSpend = timeSpend*8*2.5/1000000000;

    printf("dma write speed %fMB/s %f ok\n",(1*4096)/timeSpend,timeSpend);  
}

void fsSeekTest()
{
    int fd = 0;
    int size = 0;
    int loop = 0;
    int cnt = 0;
    int len = 0x600000;
    int *wrBuf = NULL;
    INT64 offset = 0x600004ULL;
    INT32 uiOffset = 0x600008;

    len = 0x600000;
    wrBuf = malloc(len);
    fd = open("/vol/test.bin",O_WRONLY|O_CREAT,0);
    
    for(cnt = 0; cnt < 5;cnt++)
    {
        for(loop = 0;loop < (len / 4);loop++)
        {
            wrBuf[loop] = loop + cnt;
        }
        write(fd,wrBuf,len);
    }

    ioctl(fd,FIOSEEK64,&offset);
    /*ioctl(fd,FIOSEEK,uiOffset);*/
    
    for(cnt = 0; cnt < 10;cnt++)
    {
        for(loop = 0;loop < (len / 4);loop++)
        {
            wrBuf[loop] = loop + 0x80000000;
        }
        write(fd,wrBuf,len);
    }
    close(fd);
    free(wrBuf);
}

void fsWrTest()
{
    int fd = 0;
    int size = 0;
    int loop = 0;
    int cnt = 0;
    int len = 0x600000;
    int *wrBuf = NULL;
    INT64 offset = 0x600004ULL;
    INT32 uiOffset = 0x600008;

    len = 0x600000;
    wrBuf = malloc(len);
    fd = open("/vol/test.bin",O_WRONLY|O_CREAT,0);
    
    for(cnt = 0; cnt < 33;cnt++)
    {
        for(loop = 0;loop < (len / 4);loop++)
        {
            wrBuf[loop] = loop + cnt;
        }
        write(fd,wrBuf,len);
    }
    
    for(loop = 0;loop < (len / 4);loop++)
    {
        wrBuf[loop] = loop;
    }
    write(fd,wrBuf,0x200000);
    
    close(fd);
    free(wrBuf);
}
void fsTimeTest()
{
	int fd = 0;
	FS_ATTR_S 	SetFileTime;
	FS_ATTR_S   GetFileTime;
	
	SetFileTime.uiTime = 0x457786c4;
	SetFileTime.uiStartFrame= 0;
	SetFileTime.uiEndFrame= 0;

    fd = open("/vol/test.bin",O_WRONLY|O_CREAT,0);
	ioctl(fd,FIOATTRIBSET,&SetFileTime); 
	ioctl(fd,FIOATTRIBGET,&GetFileTime);
	printf("====attr set time 0x%x get time 0x%x\n",SetFileTime.uiTime,GetFileTime.uiTime);
    close(fd);

	fd = open("/vol/test.bin",O_RDONLY,0);
	ioctl(fd,FIOATTRIBGET,&GetFileTime);
	printf("attr get point %p\n",&GetFileTime);
	close(fd);
	printf("==22 ==attr set time 0x%x get time 0x%x\n",SetFileTime.uiTime,GetFileTime.uiTime);	

}

