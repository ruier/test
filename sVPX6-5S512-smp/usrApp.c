#include "fsApi.h"
#include "usrApi.h"
#include "FS/driver/ahciDrv.h"
#include "ppcApi.h"

#include <hwif/vxbus/vxBus.h>		
#include <vxBusLib.h>			
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		
#include <hwif/vxbus/hwConf.h>	
#include <hwif/vxbus/vxbPciLib.h>
#include <driverControl.h>
#ifndef COMPILE_BOOTROM

extern  void sysTimeSrcInit();
extern void p5020SetPayLoad256();
extern INT32 rioRecMsgIsr(MSG_TRAN_BUF * recBuf, STATUS status, void * cbArg);

VOID pciDrvInit(VOID)
{
    UINT16 Type = 1;
    
   
    vxbQorIQPexRegister();
	/*p5020SetPayLoad256();*/
    Type = p5020GetDiskType(3);
    if(Type == 1)
    {
        nvmeVxbRegister();
    }
    else
    {
       ahciVxbRegister(); 
    }
	
	/*extIntInit();
	extPabackIntInit();*/
	rioSysInit(0,rioRecMsgIsr,0);
}
UINT32 rioReSysInit()
{
	rioSysInit(0,rioRecMsgIsr,0);
}
/*
 *for example "/vol/"
*/
UINT32 fsGetVolCapTest(UCHAR *vpcName)
{
    INT32   iFd = 0;
    UINT32  uiMByte = 0;
    
    iFd = open (vpcName,O_RDONLY, 0644);
    if (OK != ioctl(iFd,FIOLABELGET,&uiMByte))
    {
        printf("get vol capacity failed\n");
        close(iFd);
        return uiMByte;
    }
    
    /*printf("get vol capacity %d MByte\n",uiMByte);*/
    close(iFd);
    return uiMByte;
}

/*
 *for example "/vol/"
*/
UINT32 fsGetVolFreeCapTest(UCHAR *vpcName)
{
    INT32   iFd = 0;
    UINT32  uiMByte = 0;
    
    iFd = open (vpcName,O_RDONLY, 0644);
    
    if (OK != ioctl(iFd, FIONFREE, &uiMByte))
    {
        printf("get vol capacity failed\n");
        close(iFd);
        return uiMByte;
    }
    
   /* printf("get vol capacity %d MByte\n",uiMByte);*/
    close(iFd);
    return uiMByte;
}

UINT32 blkSetInc(UINT32* pcBuf,UINT32 uiLen)
{
	UINT32 uiLoop = 0;
	

	for(uiLoop = 0;uiLoop < (uiLen/4);uiLoop++)
	{
		pcBuf[uiLoop] = uiLoop;
	}
}

INT32 blkCheckInc(UINT32* pcBuf,UINT32 uiLen)
{
	UINT32 uiLoop = 0;
	for(uiLoop = 0;uiLoop < (uiLen/4);uiLoop++)
	{
		if(pcBuf[uiLoop] != uiLoop)
		{
			printf("check inc failed\n");
			return -1;
		}
	}
	return 0;
}

STATUS flWrTest(void)
{
	UINT32 *buf = NULL;
	UINT32 fd = 0,uiLoop = 0;

	buf = malloc(0x100000);
	blkSetInc(buf,0x100000);

	fd = open("/vol/myfile1",0x209,0);
	if(fd > 0)
	{
		for(uiLoop = 0;uiLoop < 4096;uiLoop++)
		{
			if(write(fd,buf,0x100000) != 0x100000)
			{		
				printf("write file error\n");
				close(fd);
				return ERROR;
			}
		}
	}
	close(fd);
	#if 1
	fd = open("/vol/myfile1",0,0);
	memset(buf,0,0x100000);
	if(fd > 0)
	{
		for(uiLoop = 0;uiLoop < 4096;uiLoop++)
		{
			if(read(fd,buf,0x100000) != 0x100000)
		{
			printf("read file error");
			close(fd);
			return ERROR;
			}
			if(blkCheckInc(buf,0x100000) <0)
			{
				printf("read file check error \n");
				close(fd);
				return ERROR;
			}
		}
	}
	close(fd);
	#endif
	free(buf);
	printf("check file check ok \n");
	return OK;
}



#if 0
void rmSsdLedSet(UINT32 ledFlag)
{
	UINT32  rmGetMapDisk = 0;
	UINT32  uiLoop = 0;
	INT32	rmLedReg = 0;
    UINT32  uiBarAddr = 0x803000a4;
	
	rmGetMapDisk = uiPhyNumEx;

	while(rmGetMapDisk != 0)
	{
		if((rmGetMapDisk & 0x1) != 0)
		{
			/*	ÏµÍ³Æô¶¯Ê±ºò£¬ÐèÒªÆô¶¯µÄÅÌÁÁµÆ
				00-Ãð£¬01-ÁÁ£¬10-ÉÁË¸
			*/	
			if(ledFlag == RM_LED_FLAG_OPEN)
			{
				rmLedReg |= (1 << (uiLoop*2));
			}
			else if(ledFlag == RM_LED_FLAG_BLINK)
			{
				rmLedReg |= (1 << ((uiLoop*2)+1));
			}
			else
			{
				rmLedReg = 0;
			}		
		}
		rmGetMapDisk = rmGetMapDisk >> 1;
		uiLoop ++;
	}
	RM_M8548_WRITE_REG(uiBarAddr, rmLedReg);
}

INT32 rmMntVol(void)
{	
	UINT32 rmGetMapDisk = 0;
	UINT32 uiLoop = 0;
	
	rmGetMapDisk = uiPhyNumEx;
	while(rmGetMapDisk != 0)
	{
		if((rmGetMapDisk & 0x1) != 0)
		{
			if(mntvol(uiLoop) == OK)
			{
				return OK;
			}
		}
		rmGetMapDisk = rmGetMapDisk >> 1;
		uiLoop ++;
	}

	if(uiLoop >= 6)
	{
		printf("can't mount vol\n");
	}
	return ERROR;
}



INT32 sysBSPInit()
{
    sysTimeSrcInit( );
    tempModuleInit(0);
    hostLoadFile("/tffs0/fpga1.bit");
    pciDrvInit( ); 

	
	taskDelay(sysClkRateGet()*2);

	#if 0
	if(mntvol(0)  == ERROR)
	{
        addvol("/vol", 0x400*420*6, 0x3f);
	}
    #endif
	
	if(rmMntVol() == ERROR)
	{
		printf("mntvol error \n");
		/*ÃðµÆ*/
		rmSsdLedSet(RM_LED_FLAG_OPEN);
		return ERROR;
	}
	
	/*µãÁÁµÆ*/
	rmSsdLedSet(RM_LED_FLAG_OPEN);	
}
#endif
#endif

