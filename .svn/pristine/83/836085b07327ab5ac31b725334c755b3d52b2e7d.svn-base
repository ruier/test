#include <drv/sio/ns16552Sio.h>
#include <iprip.h>
#include <ipftps.h>

#include "sysFlash.c"

#ifdef INCLUDE_PCI_BUS
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_FSL_HOST_LOAD
#include "hostLoad.c"
#endif


#include "mv88E1x11Phy.c"


int sysPollPutchar(char outChar)
{
    UINT8      ucStatus = 0;
    UINT32     uiLoop = 0;

    ucStatus = FSL_READ_BYTE((char*)(CCSBAR + 0x4500 + LSR));
    for(uiLoop = 100000; uiLoop > 0 ;uiLoop--)
    {
        if(0x00 != (ucStatus & 0x20))
        {
            break;
        }
        ucStatus = FSL_READ_BYTE((char*)(CCSBAR + 0x4500 + LSR));
    }

    if(0x00 == uiLoop)
    {
        return ERROR;
    }

    FSL_WRITE_BYTE((char*)(CCSBAR + 0x4500 + THR),outChar);

    return OK;
}
    
int sysPollprintf(char* pchar)
{
    while(*(pchar)!='\0')
    {
        if (*pchar == '\n')
        {
            while (sysPollPutchar('\r')!=0);
            while (sysPollPutchar('\n')!=0);
        }
        else
        {
            while (sysPollPutchar(*pchar)!=0);
        }
        pchar++; 
    }
    return OK;
}


/*****************************************************************************
 Prototype    : sysIpChange
 Description  : 修改IP地址
 Input        : 
 Output       : 
 Return Value : 
 Calls        : 
 Called By    : 
 
History       :
1.Date        : 2013/12/27
Author        : 
Modification  : 1.data:2014年1月26日
				  modified the restore location of ip(1,2,3)  by wang dong yang 
*****************************************************************************/
STATUS sysIpChange(char *string, unsigned int netNum)
{
	UINT8    *pucBuf = NULL;
	UINT8    *uiBuf = NULL;
	UINT8 	ipLen = 16;
	UINT32   uiOffset = 0;
	UINT32   uiflag;
	UINT32   flag;

	pucBuf = malloc(FLASH_SECTOR_BYTE);
	if(NULL == pucBuf)
	{
		printf("malloc memory failed\n");
		return ERROR;
	}

	uiBuf = malloc(FLASH_SECTOR_BYTE);
	if(NULL == uiBuf)
	{
		printf("malloc memory failed\n");
		return ERROR;
	}

	bcopyBytes (FLASH_BOOT_OFFSET, pucBuf, FLASH_SECTOR_BYTE);

	for(flag = 0; flag < FLASH_SECTOR_BYTE; flag++)
	{
		if((pucBuf[flag] == 'e')&&(pucBuf[flag + 1] == '='))
		{
		    break;
		}
	}

	if(flag >= FLASH_SECTOR_BYTE)
	{
		free(pucBuf);
		free(uiBuf);
		return ERROR;
	}

	for(uiflag = flag + 1; uiflag < FLASH_SECTOR_BYTE; uiflag++)
	{
		if(pucBuf[uiflag] == ':')
		{
		    break;
		}
	}

	if(uiflag >= FLASH_SECTOR_BYTE)
	{
		free(pucBuf);
		free(uiBuf);
		return ERROR;
	}

	/*此处应修改为动态偏移,因时间原因,目前暂使用固定地址*/
	if (netNum == 0)
	{
		/*保留IP地址之前的其它启动信息*/
		bcopyBytes (pucBuf, uiBuf, flag + 2);
		/*将新的IP地址插入到启动信息中*/
		bcopyBytes (string, (uiBuf + flag + 2), strlen(string));
		/*IP地址之后的信息插入到新字符串中*/
		bcopyBytes((pucBuf+uiflag), (uiBuf + flag + 2 + strlen(string)),
									FLASH_SECTOR_BYTE-(flag + 2 + strlen(string)));
	}
	else
	{
		bzero(pucBuf + NV_IP_ADRS_OFFSET + ipLen*(netNum-1), ipLen);
		bcopyBytes(string, (pucBuf + NV_IP_ADRS_OFFSET + ipLen*(netNum-1)), strlen(string));	
		bcopyBytes(pucBuf, uiBuf, FLASH_SECTOR_BYTE);
	}
	
	if(OK != flashErase(FLASH_BASE_ADRS, FLASH_BOOT_OFFSET))
	{
		free(pucBuf);
		free(uiBuf);
		printf("erase flash failed\n");
		return ERROR;;
	}

	if(OK != flashProgram(FLASH_BASE_ADRS,FLASH_BOOT_OFFSET,uiBuf,FLASH_SECTOR_BYTE,0))
	{
		free(pucBuf);
		free(uiBuf);
		printf("program flash failed\n");
		return ERROR;;
	}

	/*更新nVRAM内存中的缓存*/
	if ((sysNvRamGet (BOOT_LINE_ADRS, BOOT_LINE_SIZE, 0) == ERROR) ||
				  (*BOOT_LINE_ADRS == EOS) || (*BOOT_LINE_ADRS == (char) -1))
	{
		/* no non-volatile RAM -- use default boot line */
		strcpy (BOOT_LINE_ADRS, DEFAULT_BOOT_LINE);
	}

	free(pucBuf);
	free(uiBuf);
	return OK;
}


VOID sysDwordWrite(UINT32 addr,UINT32 val)
{
    UINT32 uiVal = 0;
    
    FSL_WRITE_REG(addr,val);
}

VOID sysDwordRead(UINT32 addr)
{
    UINT32 uiVal = 0;
    
    uiVal = FSL_READ_REG(addr);
    printf("read val 0x%x\n",uiVal);
}

/*
 *netCtlPort：网络号，有效值为0-1，分别对应网卡1和网卡2；
 *ipAddr：IP地址"202.115.203.244"；
 *ipMaskAddr：掩码地址"0xFFFFFF00"
*/
#if 0
UINT32 sysNetIpSet(UINT32 netCtrlPort,UINT8 *ipAddr,UINT32 ipMaskAddr)
{
    char   buf[NET_STRING_LEN];
    UINT8  auiMask[4] = {0};
    
    if(netCtrlPort > 3)
    {
        printf("netCtrlPort %d overflow\n");
        return ERROR;
    }
    auiMask[0] = (ipMaskAddr >> 24) & 0xff;
    auiMask[1] = (ipMaskAddr >> 16) & 0xff;
    auiMask[2] = (ipMaskAddr >> 8) & 0xff;
    auiMask[3] = (ipMaskAddr >> 0) & 0xff;
        
    memset(buf,0,NET_STRING_LEN);
    sprintf(buf,"motetsec%d %s netmask %d.%d.%d.%d up",netCtrlPort,ipAddr,
            auiMask[0],auiMask[1],auiMask[2],auiMask[3]);
    ipAttach(netCtrlPort,"motetsec");
    ifconfig (buf);
    
    return OK;
}
#endif

INT32 sysNetIpSet(UINT32 netCtrlPort, char *netName, UINT8 *ipAddr, UINT32 ipMaskAddr)
{
    char   buf[NET_STRING_LEN];
    UINT8  auiMask[4] = {0};
    
    if(netCtrlPort > 5)
    {
        printf("netCtrlPort %d overflow\n");
        return ERROR;
    }
    auiMask[0] = (ipMaskAddr >> 24) & 0xff;
    auiMask[1] = (ipMaskAddr >> 16) & 0xff;
    auiMask[2] = (ipMaskAddr >> 8) & 0xff;
    auiMask[3] = (ipMaskAddr >> 0) & 0xff;
        
    memset(buf,0,NET_STRING_LEN);
    sprintf(buf,"%s%d %s netmask %d.%d.%d.%d up",netName,netCtrlPort,ipAddr,
            auiMask[0],auiMask[1],auiMask[2],auiMask[3]);
    ipAttach(netCtrlPort,netName);
    ifconfig (buf);
    
    return OK;
}


/*
 *netCtlPort：网络号，有效值为0-1，分别对应网卡1和网卡2；
 **ipMacAddr: BYTE buf[6]={1,2,3,4,5,6}
*/
STATUS sysNetMacSet (UINT32 netCtrlPort, UINT8 *ipMacAddr)
{
    char buf[NET_STRING_LEN];
    
    if(netCtrlPort > 3)
    {
        printf("netCtrlPort %d overflow\n");
        return ERROR;
    }

    memset(buf,0,NET_STRING_LEN);
    ipAttach(netCtrlPort,"dtsec");
    sprintf(buf,"dtsec%d lladdr %x:%x:%x:%x:%x:%x up",netCtrlPort,*ipMacAddr,
            *(ipMacAddr + 1),*(ipMacAddr + 2),*(ipMacAddr + 3),*(ipMacAddr + 4),
            *(ipMacAddr+ 5));
    ifconfig (buf);
    
    return OK;
}

STATUS sysNetMacGet (UINT32 netCtlPort,UINT8 *ipMacAddr)
{
    UINT32 uiMacAddr1 = 0;
    UINT32 uiMacAddr2 = 0;
    UINT32 uiAddr = 0;
    UINT32 uiLoop = 0;
    
    if(netCtlPort > 3)
    {
        printf("netCtrlPort %d overflow\n");
        return ERROR;
    }
    uiAddr = CCSBAR + netCtlPort * 0x1000 + 0x24540;
    uiMacAddr1 = FSL_READ_REG(uiAddr);
    uiAddr = CCSBAR + netCtlPort * 0x1000 + 0x24544;
    uiMacAddr2 = FSL_READ_REG(uiAddr);
    *(ipMacAddr + 5) = (uiMacAddr1 >> 24) & 0xff;
    *(ipMacAddr + 4) = (uiMacAddr1 >> 16) & 0xff;
    *(ipMacAddr + 3) = (uiMacAddr1 >> 8 ) & 0xff;
    *(ipMacAddr + 2) = (uiMacAddr1 >> 0 ) & 0xff;
    *(ipMacAddr + 1) = (uiMacAddr2 >> 24) & 0xff;
    *(ipMacAddr + 0) = (uiMacAddr2 >> 16) & 0xff;

	return OK;
}


VOID intSetMode(UINT8 intNum,UINT8 intMode)
{
    UINT32  uiVal = 0;
        
    uiVal = FSL_READ_REG(EPIC_REG(intNum)) & (~(3 << 22));
    switch(intMode)
    {
        case EPIC_MODE_LOW_LEV:
            uiVal |= (1 << 22);
            break;
        case EPIC_MODE_HIGH_LEV:
            uiVal |= (3 << 22);
            break;
        case EPIC_MODE_LOW_EDG:
            uiVal |= (0 << 22);
            break;
        case EPIC_MODE_HIGH_EDG:
            uiVal |= (2 << 22);
            break;        
    }
    FSL_WRITE_REG(EPIC_REG(intNum),uiVal);
}

STATUS intOpenInit(UINT8 intNum,UINT8 intMode,VOIDFUNCPTR intIsr,UINT32 intArg)
{
    UINT32  uiIrq = 0;
    /*换算中断号, 此处规则是基于一个号上面累加*/
    uiIrq = intNum + EPIC_VEC_EXT_IRQ0;
    intConnect(uiIrq,intIsr,intArg);
     
    intSetMode(intNum,intMode);
    
    return OK;
}

VOID intMaskEnable(UINT8 intNum)
{
    UINT32   uiIrq = 0;
    UINT32   uiReg = 0;

    uiIrq = intNum + EPIC_VEC_EXT_IRQ0;
    intEnable(uiIrq);
}

VOID intMaskDisable(UINT8 intNum)
{
    UINT32   uiIrq = 0;
    UINT32   uiReg = 0;

    uiIrq = intNum + EPIC_VEC_EXT_IRQ0;
    intDisable(uiIrq);
}

#define EPIC_INT_NUM   1

EXT_INT_S gstExtInt;
EXT_INT_S gstPabackExtInt;

IMPORT VXB_DEVICE_ID globalBusCtrlID;
 
VOID intISR(VOID)
{
	gstExtInt.uiIntChan = FSL_READ_REG(gstExtInt.uiIntAddr);
	semGive(gstExtInt.stIntSem);
}

VOID extIntInit(VOID)
{
	UINT32			uiVal = 0;

	/*uiBarOffset = 0x10 + (4 * uiLoop);*/
#ifndef COMPILE_BOOTROM
	vxbPciConfigInLong(globalBusCtrlID,6,0,0,0x10,&uiVal);
#endif
    gstExtInt.stIntSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
    gstExtInt.uiIntChan = 0xffff;
	/*熊杰的新版本为0x18,老版本为0x14*/
    gstExtInt.uiIntAddr = /*0x40100014*/(uiVal+0x18);
    /*printf("int addr 0x%x \n",gstExtInt.uiIntAddr);*/
    intOpenInit(EPIC_INT_NUM,EPIC_MODE_LOW_LEV,intISR,NULL);
    intMaskEnable(EPIC_INT_NUM);
}
VOID pabackintISR(VOID)
{
	gstPabackExtInt.uiIntChan = FSL_READ_REG(gstPabackExtInt.uiIntAddr);
	semGive(gstPabackExtInt.stIntSem);
}

VOID extPabackIntInit(VOID)
{
	UINT32			uiVal = 0;
	
#ifndef COMPILE_BOOTROM
	vxbPciConfigInLong(globalBusCtrlID,6,0,0,0x10,&uiVal);
#endif
    gstPabackExtInt.stIntSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
    gstPabackExtInt.uiIntChan = 0xffff;
    gstPabackExtInt.uiIntAddr = (uiVal+0x6c);
    /*printf("int addr 0x%x \n",gstPabackExtInt.uiIntAddr);*/
    intOpenInit(0,EPIC_MODE_LOW_LEV,pabackintISR,NULL);
    intMaskEnable(0);
}

#if 0
STATUS bspBoardReset(VOID)
{
    sysToMonitor();
}
#endif


SEM_ID   timeIntSem;

VOID timerIntISR(VOID)
{
    semGive(timeIntSem);
    logMsg("Timer int occure!\n",0,0,0,0,0,0);
}


VOID timerIntClose(VOID)
{
    FSL_WRITE_REG(TIMER_GTBCR0,(1 << 31));
    intDisable(OPENPIC_TIMERA0_INT_VEC);
    logMsg("time int close !\n",0,0,0,0,0,0);
}


INT32 timerIntInit()
{
    UINT32  uiFreq = 0;

    timeIntSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);

    intConnect ((VOIDFUNCPTR *)(OPENPIC_TIMERA0_INT_VEC ),(VOIDFUNCPTR)timerIntISR, 0);
  	
    FSL_WRITE_REG(TIMER_GTBCR0,(1 << 31));

    uiFreq = sysClkFreqGet()/8*10;
    
    FSL_WRITE_REG(TIMER_GTBCR0,uiFreq); 
}





