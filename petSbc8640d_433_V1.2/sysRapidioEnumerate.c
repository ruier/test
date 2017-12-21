/* includes */

#include <vxWorks.h>
#include <taskLib.h>			/* for taskSpawn() and taskDelay() */
#include <sysLib.h>			/* for sysClkRateGet() */
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for bfill() */
#include <vxBusLib.h>			/* for vxbDeviceMethodRun()
 * and  VXB_DEVICE_ID
 */
#include <arch/ppc/private/excArchLibP.h>
#include <driverControl.h>		/* for struct vxbDriverControl */
#include <hwif/vxbus/vxbMethodDecl.h>	/* for DEVMETHOD_CALL(driverControl) */
#include <vxbm8641I2c.h>			/* for struct wrsampleDriverControl
 * and WRSAMPLE_GET_SAMPLE_FUNCS					 */
#include <cpuset.h>

#include "rapidioReg.h"

#define RIO_DEBUG
//#undef RIO_DEBUG

#define RIO_DEEP_DEBUG
//#undef RIO_DEEP_DEBUG

int rioReadState = 0;

#define CONFIG_WRITE_TIMEOUT 100
#define CONFIG_READ_TIMEOUT 100   //config read timeout
#define CONFIG_READ_TIMEOUT_TIME 0x800000
#ifdef RIO_DEEP_DEBUG
extern int     ns16550Pollprintf(char* pChar);
char sprintBuf_rio[100];
#define RIO_DEEP_LOG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBuf_rio,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBuf_rio);}
#else
#define RIO_DEEP_LOG(X0, X1, X2, X3, X4, X5, X6)
#endif

#define RIO_EP_DEBUG
//#undef RIO_EP_DEBUG

#ifdef RIO_EP_DEBUG
extern int     ns16550Pollprintf(char* pChar);
char sprintBuf_rio[100];
#define RIO_EP_DEEP_LOG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBuf_rio,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBuf_rio);}
#else
#define RIO_EP_DEEP_LOG(X0, X1, X2, X3, X4, X5, X6)
#endif


/* locals */

LOCAL FUNCPTR oldExcBaseHook;		/* original _func_excBaseHook */
#ifdef RIO_DEBUG
extern int     ns16550Pollprintf(char* pChar);
char sprintBuf_rio[100];
#define RIO_LOG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBuf_rio,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBuf_rio);}

#undef LOCAL
#define LOCAL 

#else /* RIO_DEBUG */

#define RIO_LOG(X0, X1, X2, X3, X4, X5, X6)	
#endif


#define RIO_LOCAL_ADDR RAPIDIO_ADRS


#define maxSwCount 80
#define maxEpCount 50
#define maxSwPortCount 20
#define maxSwNum 100;

/* sjy 2013-04-16 added */

void rioErrorIntHandle()
{
	//ns16550Pollprintf("E!\n");
	*(UINT32 *)0xe80c0608 = 0x0;
	rioReadState = 1;
	//	*(UINT32 *)0x30000000 += 0x1;
	//	*(UINT32 *)0x30000004 = *(UINT32 *)0xe80d0010;
	//	*(UINT32 *)0x30000004 = *(UINT32 *)0xe80d0010;
	//	*(UINT32 *)0xe80d0010 = 0;
	//	*(UINT32 *)0x30000100 = *(UINT32 *)0xe8001e00;
	//	*(UINT32 *)0x30000104 = *(UINT32 *)0xe8001e04;
	//	*(UINT32 *)0x30000108 = *(UINT32 *)0xe8001e08;
	//	*(UINT32 *)0x3000010c = *(UINT32 *)0xe8001e0c;
	//	*(UINT32 *)0x300001010 = *(UINT32 *)0xe8001e10;
}

LOCAL STATUS rioErrorIntSetup(void)
{
	VXB_DEVICE_ID pInst;
	pInst=(VXB_DEVICE_ID)vxbInstByNameFind ("m85xxCPU", 0);
	if (pInst == NULL)
	{
		ns16550Pollprintf ("rioDBIntSetup:Get pInst failed!!!\n");
		return ERROR;
	}
	vxbIntConnect (pInst, 6, rioErrorIntHandle, NULL);
	vxbIntEnable  (pInst, 6, rioErrorIntHandle, NULL);

	return OK;
}
/* sjy 2013-04-16 added */
UINT32 m85xxRioRegRead(UINT32 offset)
{
	UINT32 data;
	data=*(volatile UINT32*)(CCSBAR+RAPIDIO_REG_BASE+offset);
	return data;
}

void m85xxRioRegWrite(UINT32 offset,UINT32 value)
{	
	*(volatile UINT32*)(CCSBAR+RAPIDIO_REG_BASE+offset)=value;
}


INT32 rioGetNumLocalPorts(void)
{
	return 2;
}


/***********************************************
 * 
 * m85xx Rapidio remote/local config write/read
 * 
 *********************************************/
STATUS rioConfigLocalRead(UINT32 offset,UINT32 *readData)
{
	*readData=m85xxRioRegRead(offset);
	return OK;
}

STATUS rioConfigLocalWrite(UINT32 offset,UINT32 writeData)
{
	m85xxRioRegWrite(offset,writeData);
	return OK;
}

STATUS rioConfigRead(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 *readData)
{
	UINT32 time = CONFIG_READ_TIMEOUT;
	if(localPort==0) localPort=1;
	if(localPort>3) localPort=3;
	while(time)
	{
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
		//RIO_DEEP_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
		*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);
		if(rioReadState == 0)
		{
			break;
		}
		//		else
		//		{
		//			rioReadState = 0;
		//		}
		//RIO_DEEP_LOG(" rioConfigRead %x %x\n ",*readData,offset,3,4,5,6);
		time --;
	}
	rioReadState = 0;
	if(time == 0)
	{
		ns16550Pollprintf("rioConfigRead timeout \n");
		//RIO_DEEP_LOG("exit  rioConfigRead   tfttttttimeout\n",1,2,3,4,5,6);
	}

	return OK;

}


STATUS rioConfigWrite(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
{
	UINT32 data,timeOut=CONFIG_WRITE_TIMEOUT;

	if(localPort==0) localPort=1;
	if(localPort>3) localPort=3;
	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/

	while(timeOut--)
	{
		*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
		data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
		if(data == writeData) break;
	}
	if(timeOut) return OK;
	else return ERROR;	
}

STATUS rioConfigWriteOne(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
{
	UINT32 data,timeOut=3;

	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
	*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
	data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
	return OK;
}

/********************************
 * 
 * get rapidio feature
 * 
 *********************************/

UINT32 rioGetFeature(UINT8 localPort,UINT8 local,UINT16 destId,UINT8 hopCount,int ftr)
{
	UINT32 asmInfo,extFtrPtr,ftrHeader;
	if(local) rioConfigLocalRead(RIO_ASM_INFO_CAR,&asmInfo);
	else rioConfigRead(localPort,destId,hopCount,RIO_ASM_INFO_CAR,&asmInfo);

	extFtrPtr = asmInfo&RIO_EXT_FTR_PTR_MASK;
	while(extFtrPtr)
	{
		if(local) rioConfigLocalRead(extFtrPtr,&ftrHeader);
		else rioConfigRead(localPort,destId,hopCount,extFtrPtr,&ftrHeader);
		if(RIO_GET_BLOCK_ID(ftrHeader) == ftr) return extFtrPtr;
		if(!(extFtrPtr = RIO_GET_BLOCK_PTR(ftrHeader))) break;
	}
	return 0;
}


INT8 rioMasterPortPhysTable[] = {
		RIO_EFB_PAR_EP_ID,
		RIO_EFB_PAR_EP_REC_ID,
		RIO_EFB_SER_EP_ID,
		RIO_EFB_SER_EP_REC_ID,
		-1,
};

INT8 rioSwPortPhysTable[] = {
		RIO_EFB_PAR_EP_FREE_ID,
		RIO_EFB_SER_EP_FREE_ID,
		RIO_EFB_SW_REC_ID,
		-1,
};




UINT32 rioSwPortIsActive(UINT8 localPort, UINT16 destId, UINT8 hopCount, UINT8 swPort)
{
	UINT32 result,extFtrPtr;
	INT8 *entry = rioSwPortPhysTable;

	do {
		if ((extFtrPtr = rioGetFeature(localPort,0,destId,hopCount,*entry)))  break;
	} while (*++entry >= 0);

	if (extFtrPtr) rioConfigRead(localPort,destId,hopCount,extFtrPtr+RIO_PORT_N_ERR_STS_CSR(swPort),&result);
	return (result & PORT_N_ERR_STS_PORT_OK);
}


UINT32 rioMasterPortIsActive()
{
	UINT32 result=0,extFtrPtr;
	INT8 *entry = rioMasterPortPhysTable;
	RIO_LOG("enter rioMasterPortIsActive\n",1,2,3,4,5,6);
	do {
		if ((extFtrPtr = rioGetFeature(0,1,0,0,*entry)))  break;
	} while (*++entry >= 0);

	if (extFtrPtr) rioConfigLocalRead(extFtrPtr+RIO_PORT_N_ERR_STS_CSR(0),&result);
	RIO_LOG("exit rioMasterPortIsActive\n",1,2,3,4,5,6);
	return (result & PORT_N_ERR_STS_PORT_OK);
}



/**
 * rioSetDeviceId - Set the base/extended device id for a device
 *@
 *@
 * Writes the base/extended device id from a device.
 */
void rioSetDeviceId(UINT8 localPort, UINT16 destId, UINT8 hopCount,  UINT16 did)
{
	rioConfigWrite(localPort,destId,hopCount,RIO_DID_CSR,(did<<16));
}


/**
 * rioLocalSetDeviceId - Set the base/extended device id for a port
 * Writes the base/extended device id from a device.
 */
void rioLocalSetDeviceId(UINT16 did)
{
	rioConfigLocalWrite(RIO_DID_CSR,(did<<16));
}


void rioLocalGetDeviceId(UINT16 *did)
{
	UINT32 result;
	rioConfigLocalRead(RIO_DID_CSR,&result);
	*did =( (UINT16)(result>>16))&0xffff;
}

void rioGetDeviceId(UINT8 localPort, UINT16 destId, UINT8 hopCount, UINT16 *did)
{
	UINT32 result;
	rioConfigRead(localPort,destId,hopCount,RIO_DID_CSR,&result);
	*did = (result>>16)&0xffff;
}

int rioIsSwitch(UINT8 localPort,UINT16 destId, UINT8 hopCount)
{
	UINT32 result;
	rioConfigRead(localPort,destId,hopCount,RIO_PEF_CAR,&result);
	if(result&RIO_PEF_SWITCH) return 1;
	return 0;
}

//sjy 20120428 added
int rioGetPEF(UINT8 localPort,UINT16 destId, UINT8 hopCount)
{
	UINT32 result;
	rioConfigRead(localPort,destId,hopCount,RIO_PEF_CAR,&result);

	return result;//if(result&RIO_PEF_SWITCH) return 1;
	//return 0;
}
//sjy 20120428 added

STATUS rioAcquirLocalLock(UINT16 deviceId,UINT16 *lockId)
{
	UINT32 result;
	RIO_LOG("enter rioAcquirLocalLock\n",1,2,3,4,5,6);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	if((result&0xffff)!=0xffff)
	{
		*lockId=result&0xffff;
		return ERROR;
	}
	rioConfigLocalWrite(RIO_HOST_DID_LOCK_CSR,deviceId);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	*lockId=result&0xffff;
	if((result&0xffff) != (deviceId&0xffff)) return ERROR;
	RIO_LOG("exit rioAcquirLocalLock\n",1,2,3,4,5,6);
	return OK;
}


STATUS rioReleaseLocalLock(UINT16 deviceId,UINT16 *lockId)
{
	UINT32 result;
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	if((result&0xffff)==0xffff)
	{
		*lockId=result&0xffff;
		return ERROR;
	}
	rioConfigLocalWrite(RIO_HOST_DID_LOCK_CSR,deviceId);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	*lockId=result&0xffff;
	if((result&0xffff) != 0xffff) return ERROR;
	return OK;
}


STATUS rioAcquirLock(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT16 hostDeviceId,UINT16 *hostLockId)
{
	UINT32 timeOut=2, readData=0;
	RIO_LOG("enter rioAcquirLock\n",1,2,3,4,5,6);
	RIO_LOG("enter rioAcquirLock:rioConfigRead-destId=0x%x,hopCount=0x%x\n",destId,hopCount,3,4,5,6);

#ifdef RIO_EP_DEBUG
	RIO_EP_DEEP_LOG("Enter rioAcquirLock 0 in Sw !\n",1,2,3,4,5,6);
#endif

	if(rioConfigRead(localPort,destId,hopCount,RIO_HOST_DID_LOCK_CSR,&readData)==ERROR) goto error;

#ifdef RIO_EP_DEBUG
	RIO_EP_DEEP_LOG("Enter rioAcquirLock 1 in Sw !\n",1,2,3,4,5,6);
#endif
	RIO_LOG("enter rioAcquirLock1\n",1,2,3,4,5,6);
	*hostLockId=(UINT16)(readData&0xffff);
	if((readData&0xffff) != 0xffff)
	{
		/*	*hostLockId=(UINT16)(readData&0xffff);*/
		goto error;
	}
	RIO_EP_DEEP_LOG("----------in rioAcquirLock 1 in Sw readData 0x%x !\n",readData,2,3,4,5,6);
	while(timeOut)
	{
		RIO_LOG("enter rioAcquirLock2\n",1,2,3,4,5,6);
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 2 in Sw !\n",1,2,3,4,5,6);
#endif
		if(rioConfigWriteOne(localPort,destId,hopCount,RIO_HOST_DID_LOCK_CSR,hostDeviceId)==ERROR) goto error;
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 3 in Sw !\n",1,2,3,4,5,6);
#endif
		if(rioConfigRead(localPort,destId,hopCount,RIO_HOST_DID_LOCK_CSR,&readData)==ERROR) goto error;
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 4 in Sw !\n",1,2,3,4,5,6);
#endif
		if((readData&0xffff) == hostDeviceId) break;
		timeOut--;
		RIO_LOG("enter rioAcquirLock3\n",1,2,3,4,5,6);
	}
	/*
	 *hostLockId = (UINT16)(readData&0xffff);
	printf("*hostLockId = 0x%x....timeOut= %d\n",*hostLockId,timeOut);
	 */
	RIO_LOG("enter rioAcquirLock4\n",1,2,3,4,5,6);
	/* 0604if(timeOut)*/ return OK;
	RIO_LOG("exit rioAcquirLock\n",1,2,3,4,5,6);
	error: 
	return ERROR;
}


STATUS rioReleaseLock(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT16 hostDeviceId,UINT16 *hostLockId)
{
	UINT32 timeOut=2, readData=0;
	if(rioConfigRead(localPort,destId,hopCount,RIO_HOST_DID_LOCK_CSR,&readData)==ERROR) goto error;
	*hostLockId=(UINT16)(readData&0xffff);
	if((readData&0xffff) == 0xffff)
	{
		/**hostLockId=(UINT16)(readData&0xffff);*/
		return OK;
	}
	while(timeOut)
	{
		if(rioConfigWriteOne(localPort,destId,hopCount,0x68,hostDeviceId)==ERROR) goto error;
		if(rioConfigRead(localPort,destId,hopCount,0x68,&readData)==ERROR) goto error;
		if((readData&0xffff) ==0xffff) break;
		timeOut--;
	}
	/*
	 *hostLockId = (UINT16)readData&0xffff;
	printf("*hostLockId = 0x%x....timeOut= %d\n",*hostLockId,timeOut);
	 */
	if(timeOut) return OK;
	error:	
	return ERROR;
}



static UINT16 nextDestId = 0;  /*NOTE!!!!!!!!!!!!!!!*/
/*********************************************************************
 * rioEnumHost- Set host lock and initialize host destination ID
 * @port: Master port to issue transaction
 *
 * Sets the host master port lock and destination ID register
 * with the host device ID value. The host device ID value is provided
 * by the platform. Returns %0 on success or %-1 on failure.
 */
STATUS rioEnumHost(UINT16 hostId)
{
	UINT16 lockId;
	RIO_LOG("enter rioEnumHost\n",1,2,3,4,5,6);
	if(rioAcquirLocalLock(hostId,&lockId)==ERROR) 
	{
		printf("RIO:the host device has been locked by another host(ID=%d)\n",lockId);
		return ERROR;
	}
	rioLocalSetDeviceId(hostId);
	if(nextDestId == hostId) nextDestId++;
	RIO_LOG("exit rioEnumHost\n",1,2,3,4,5,6);
	return OK;
}

void rioGetSwPortInf(UINT8 localPort,UINT8 hopCount,UINT32 *readData)
{
	rioConfigRead(localPort,0xff,hopCount,RIO_SWP_INFO_CAR,readData);
}

STATUS rioRouteAddEntry(UINT8 localPort,UINT16 deviceId,UINT8 hopCount,UINT8 portNum)
{	
	UINT32 data;
	/*if(rioConfigWrite(localPort,0xff,hopCount,0x70,deviceId) == ERROR) return ERROR;
	if(rioConfigWrite(localPort,0xff,hopCount,0x74,portNum) == ERROR) return ERROR;*/

	m85xxRioRegWrite((0x10c00+0x20*localPort),((0xff << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/

	*(UINT32*)(RIO_LOCAL_ADDR+0x70) = deviceId;
	data = *(UINT32*)(RIO_LOCAL_ADDR+0x70);

	*(UINT32*)(RIO_LOCAL_ADDR+0x74) = portNum;
	data = *(UINT32*)(RIO_LOCAL_ADDR+0x74);

	/*RIO_LOG("\nRIO:rioRouteAddEntry-- hopCount %d port %d deviceId %d\n",hopCount,portNum,deviceId,4,5,6);*/
	return OK;	
}





typedef struct 
{
	UINT8 swPortFlag;/*0:no connect  1:switch  2:endPoint*/
	/*UINT16 neighborSw;*/
	UINT16 switchNo;
	/*UINT8 neighborSwPort;*/
	UINT8  switchPort;
	UINT16 deviceId;
	/*UINT8 currentSwPort;*/
	/*UINT8 state;	*/
}PORTINFO;

typedef struct 
{
	/*UINT16 switchNo;*/
	UINT8 portNum;
	/*UINT8 hostPort;*/
	/*UINT16 neighborSwCount;*/
	UINT8 currentActivePort;
	PORTINFO swPortInfo[maxSwPortCount];	
}SWINFO;

#if 0
typedef struct 
{
	UINT16 switchNo;
	UINT16 deviceId;
	UINT8  switchPort;
	/*UINT8 portNum;*/
	/*
	UINT8 hostPort;
	PORTINFO * pSwPortInfo;
	 */
}EPINFO;
#endif

SWINFO gSwInfo[maxSwCount];
/*EPINFO gEpInfo[maxEpCount];*/
UINT16 gSwitchCount=0;
#ifndef INCLUDE_RAPIDIO_SLOTNUM
static UINT16 deviceId=2;
#endif


#if 0
UINT16 rioGetEpRapidIoId(void)
{
#if 0
	UINT32 deviceId;
	rioConfigWrite(localPort,0xff,hopCount,0x60,epRapidIoId<<16);
	return (UINT16)(deviceId&0xffff);
#else
	static UINT16 deviceId=2;
	return (deviceId++);
#endif
}
#endif


typedef struct 
{
	UINT16 switchNo;
	UINT8  switchPort;
}SWNODEINFO;


typedef struct 
{
	UINT16 switchNo;
	UINT8  switchPort;
	UINT16 deviceId;

}EPNODEINFO;




UINT16 gSwNodeCount=0,gPreSwNodeCount=0,gSaveSwNodeCount=0;
SWNODEINFO gSwNodeInfo[maxSwCount],gPreSwNodeInfo[maxSwCount];
int gSwIsGet[100];
UINT16 gFindSwNodeFinal=0;

void rioShow()
{
	UINT16 i;
	for(i=0;i<gPreSwNodeCount;i++)
		RIO_DEEP_LOG("swNo = %d;; swPort = %d\n",gPreSwNodeInfo[i].switchNo,gPreSwNodeInfo[i].switchPort,3,4,5,6);


}

void rioShow1()
{
	UINT16 i;
	for(i=0;i<gSwNodeCount;i++)
		printf("swNo1 = %d;; swPort1 = %d\n",gSwNodeInfo[i].switchNo,gSwNodeInfo[i].switchPort);
}

void  rioFindSwNode(SWNODEINFO currentSw,SWNODEINFO destSw)
{

	UINT16 i,j;
	/*static UINT16 swNodeCount;*/
	SWNODEINFO nextSw;

#ifdef RIO_DEEP_DEBUG
	RIO_DEEP_LOG("rioFindSwNode start!!(%d,%d)<-->(%d,%d)  gSwNodeCount= %d\n",currentSw.switchNo,currentSw.switchPort,destSw.switchNo,destSw.switchPort,gSwNodeCount,6);
#endif
	if(currentSw.switchNo == destSw.switchNo)   
	{
		gPreSwNodeCount = gSwNodeCount=0;
		return;
	}
	/*
	printf("\nat switch(%d) all prot to  find switch(%d)\n",currentSw.switchNo,destSw.switchNo);
	taskDelay(10);
	 */
	for(i=0;i<gSwInfo[currentSw.switchNo].portNum;i++)
	{
		/*printf("aaaaaaaaaa switchNo =%d ;;port= %d\n",currentSw.switchNo,i);*/
		if((gSwInfo[currentSw.switchNo].swPortInfo[i].swPortFlag != 1)||(i == currentSw.switchPort))	continue;

		/*
		printf("switchNo = %d;; switchPort = %d;;\n",currentSw.switchNo,i);
		taskDelay(10);
		 */
		if( gSwInfo[currentSw.switchNo].swPortInfo[i].switchNo == destSw.switchNo)
		{

			gSwNodeInfo[gSwNodeCount].switchNo = currentSw.switchNo;
			gSwNodeInfo[gSwNodeCount].switchPort =i; 
			gSwNodeCount++;

			gSwNodeInfo[gSwNodeCount].switchNo = gSwInfo[currentSw.switchNo].swPortInfo[i].switchNo;
			gSwNodeInfo[gSwNodeCount].switchPort =gSwInfo[currentSw.switchNo].swPortInfo[i].switchPort; 
			gSwNodeCount++;
			/*
			rioShow1();
			printf("\ngSwNodeCount = %d;;gPreSwNodeCount =%d;;\n",gSwNodeCount,gPreSwNodeCount);
			taskDelay(10);
			 */
			gSaveSwNodeCount=gSwNodeCount;
			gFindSwNodeFinal =1;
			if((gPreSwNodeCount == 0)||(gPreSwNodeCount>gSwNodeCount))
			{
				gPreSwNodeCount=gSwNodeCount;
				for(j=0;j<gPreSwNodeCount;j++) 
				{	
					gPreSwNodeInfo[j].switchNo = gSwNodeInfo[j].switchNo;
					gPreSwNodeInfo[j].switchPort = gSwNodeInfo[j].switchPort; 
				}
			}
			/*taskDelay(10);*/
			//gSaveSwNodeCount -= 2;
			return ;
		}
		/* altered by sjy*/
		else
		{
			gSwIsGet[currentSw.switchNo] = 1; //added by sjy
			gSwNodeInfo[gSwNodeCount].switchNo = currentSw.switchNo;
			gSwNodeInfo[gSwNodeCount].switchPort = i; 
			gSwNodeCount++;


			nextSw.switchNo = gSwInfo[currentSw.switchNo].swPortInfo[i].switchNo;
			nextSw.switchPort = gSwInfo[currentSw.switchNo].swPortInfo[i].switchPort;

			gSwNodeInfo[gSwNodeCount].switchNo = nextSw.switchNo;
			gSwNodeInfo[gSwNodeCount].switchPort = nextSw.switchPort; 
			gSwNodeCount++;
			/* altered by sjy */
			if(gSwIsGet[nextSw.switchNo] != 1)
			{
				rioFindSwNode(nextSw,destSw);	
			}
			/* altered by sjy */
			/*if(gSaveSwNodeCount != 0)
			{
				gSaveSwNodeCount -= 2;
			}
			gSwNodeCount=gSaveSwNodeCount;*/
#ifdef RIO_DEEP_DEBUG
			//				RIO_DEEP_LOG("rioFindSwNode saveSwNodeCount %d! gSwNodeCount= %d\n",gSwNodeCount,gSwNodeCount,3,4,5,6);
#endif


			if(gFindSwNodeFinal)
			{
				//gFindSwNodeFinal=0;

				taskDelay(10);
				rioShow();
				//	printf("\nrioFindSwNode final!!(%d,%d)<-->(%d,%d);;;swNodeCount=%d\n",nextSw.switchNo,nextSw.switchPort,destSw.switchNo,destSw.switchPort,gSwNodeCount);

				return;	
			}
			else
			{
				gSwNodeCount -= 2;
			}
		}
		/* altered by sjy*/
	}

#if 0   //added by sjy		
	gSwIsGet[currentSw.switchNo] = 1; //added by sjy

	for(i=0;i<gSwInfo[currentSw.switchNo].portNum;i++)
	{

		/*
		printf("rioFindSwNode1111 %d::%d\n",currentSw.switchNo,i);
		taskDelay(10);
		 */
		if(gSwInfo[currentSw.switchNo].swPortInfo[i].swPortFlag != 1) continue;
		if(i == currentSw.switchPort) continue;
		/*
		printf("rioFindSwNode2222 %d::%d\n",currentSw.switchNo,i);
		taskDelay(10);
		 */
		for(j=0;j<gSwNodeCount;j++)
		{
			if(gSwInfo[currentSw.switchNo].swPortInfo[i].switchNo == gSwNodeInfo[j].switchNo ) goto next;
		}

		/*
		printf("\nat switch(%d) port(%d) to find switch(%d) ;;gSwNodeCount=%d;; swNodeCount = %d\n",currentSw.switchNo,i,destSw.switchNo,gSwNodeCount,gSwNodeCount);
		taskDelay(10);
		 */
		gSwNodeInfo[gSwNodeCount].switchNo = currentSw.switchNo;
		gSwNodeInfo[gSwNodeCount].switchPort = i; 
		gSwNodeCount++;


		nextSw.switchNo = gSwInfo[currentSw.switchNo].swPortInfo[i].switchNo;
		nextSw.switchPort = gSwInfo[currentSw.switchNo].swPortInfo[i].switchPort;

		gSwNodeInfo[gSwNodeCount].switchNo = nextSw.switchNo;
		gSwNodeInfo[gSwNodeCount].switchPort = nextSw.switchPort; 
		gSwNodeCount++;
		/*

		/printf("rioFindSwNode3333 %d::%d\n",currentSw.switchNo,i);
		taskDelay(10);
		rioShow1();

		printf("rioFindSwNode start!!(%d,%d)<-->(%d,%d)  gSwNodeCount= %d\n",nextSw.switchNo,nextSw.switchPort,destSw.switchNo,destSw.switchPort,gSwNodeCount);
		taskDelay(10);
		 */
#ifdef RIO_DEEP_DEBUG
		RIO_DEEP_LOG("rioFindSwNode start!!(%d,%d)<-->(%d,%d)  gSwNodeCount= %d\n",nextSw.switchNo,nextSw.switchPort,destSw.switchNo,destSw.switchPort,gSwNodeCount,6);
#endif
		/* altered by sjy */
		if(gSwIsGet[nextSw.switchNo] != 1)
		{
			rioFindSwNode(nextSw,destSw);	
		}
		/* altered by sjy */

		/*printf("rioFindSwNode444 %d::%d\n",currentSw.switchNo,i);
		taskDelay(10);
		 */
		gSaveSwNodeCount -= 2;
		gSwNodeCount=gSaveSwNodeCount;

		if(gFindSwNodeFinal)
		{
			gFindSwNodeFinal=0;
			/*
			taskDelay(10);
			rioShow();
			printf("\nrioFindSwNode final!!(%d,%d)<-->(%d,%d);;;swNodeCount=%d\n",nextSw.switchNo,nextSw.switchPort,destSw.switchNo,destSw.switchPort,gSwNodeCount);
			 */
			return;	
		}
		next:
		/*
		printf("rioFindSwNode next!!\n");
		taskDelay(10);
		 */
	}
#endif	//added by sjy	

}

UINT16 rioFindEndPoint(EPNODEINFO *epNode)
{
	UINT16 i,j,epCount=0;
	EPNODEINFO *pEpNode=epNode;
	for(i=0;i<gSwitchCount;i++)
	{
		for(j=0;j<gSwInfo[i].portNum;j++)
		{
			if(gSwInfo[i].swPortInfo[j].swPortFlag != 2) continue;
			pEpNode->switchNo = gSwInfo[i].swPortInfo[j].switchNo;
			pEpNode->switchPort = gSwInfo[i].swPortInfo[j].switchPort;
			pEpNode->deviceId = gSwInfo[i].swPortInfo[j].deviceId;
			epCount++;
			pEpNode++;
		}
	}	
	return epCount;
}


/* RapidIO exception handle, this function is used to handle the RapidIO 
 * remote read and write exceptions.
 */
LOCAL BOOL rapidIoScanHandle
(
		vecTblOffset  vecNum,	/* exception vector number */
		ESFPPC *      pEsf,		/* pointer to exception stack frame */
		REG_SET *     pRegSet,	/* pointer to registers @ exception */
		EXC_INFO *    pExcInfo	/* not used here */
)
{

	if(rioReadState == 1)
	{
		return TRUE;
	}

	/*
	 * Exception was NOT due to vxMemProbe, let other hooks have a crack
	 * at processing the exception.
	 */

	if (oldExcBaseHook != NULL)
	{
		return oldExcBaseHook (vecNum, pEsf, pRegSet, pExcInfo);
	}

	/*
	 * Exception was NOT due to vxMemProbe, and there are no other hooks
	 * that have registered to process exceptions.  Inform excExcHandle()
	 * to continue default exception handling.
	 */

	return FALSE;
}

/* Set the rapidIO exception handle.*/
STATUS rioScanInit (void)
{
	if (oldExcBaseHook == NULL)
	{
		oldExcBaseHook = _func_excBaseHook;
		_func_excBaseHook = &rapidIoScanHandle;
		return OK;
	}
	return ERROR;
}


STATUS rioScanLockDevice(UINT8 localPort,UINT8 hopCount,UINT16 hostId,UINT8 mode);
#if 1
STATUS rioScanDevice(UINT8 mode)
{
	SWNODEINFO currentSw,destSw,swNodeInfo[maxSwCount],hostSw;
	EPNODEINFO epNode[maxEpCount];
	UINT8 sourceDevId,destDevId,hostSwNum=0;

	INT16 epCount,i,j,k,m,swNodeCount;
#ifdef RIO_DEEP_DEBUG
	UINT32 result;
#endif

	UINT16 hostId=0;
#if 0
	UINT32 timeh1,timeh2,timeh3,timel1,timel2,timel3;
	float timeSpend3,timeSpend1,timeSpend2,timeSpend4;
#endif

#if 0	
	vxTimeBaseGet(&timeh1,&timel1);
	/*sysGetTime(&timeh1,&timel1);*/
#endif

	rioReadState = 0;
	rioScanInit();
	rioErrorIntSetup(); //sjy 20130426 added

	/* If somebody else enumerated our master port device, fail. */
	//printf("enter rioEnumHost\n");  sjy 0417
	if (rioEnumHost(hostId) == ERROR)
	{
		printf("ERROR!!!RIO: master port %d device has been enumerated by a remote host\n",hostId);
		return ERROR;
	}

	rioConfigLocalWrite(0x0124,CONFIG_READ_TIMEOUT_TIME);
	rioConfigLocalWrite(0x060c,0x01000000);

	/* If master port has an active link, allocate net and enum peers */
	//	printf("enter rioMasterPortIsActive\n"); sjy 0417
	if (rioMasterPortIsActive()) 
	{
		if(rioScanLockDevice(1,0,hostId,mode) == ERROR)
		{
			ns16550Pollprintf("ERROR!!!RIO: scan device fail!!\n");
			return ERROR;
		}
	}
	else
	{
		ns16550Pollprintf("RapidIO Hardware not connnected\n");
		return ERROR;

	}       
	printf("enter rioFindEndPoint\n");
	epCount=rioFindEndPoint(epNode);
	printf("epCount = %d\n",epCount);
#if 0
	vxTimeBaseGet(&timeh2,&timel2);
	/*sysGetTime(&timeh2,&timel2);*/
#endif

	for(i=0;i<epCount;i++)
	{
		if((epNode[i].switchNo == 0 )&&(epNode[i].deviceId == hostId))
		{
			hostSw.switchNo=0;
			hostSw.switchPort=epNode[i].switchPort;
			break;
		}
	}

#ifdef RIO_DEEP_DEBUG

	for(i=0;i<epCount;i++)
		RIO_DEEP_LOG("\nepNode--switchNo = %d,,switchPort = %d,,deviceId = %d \n",epNode[i].switchNo,epNode[i].switchPort,epNode[i].deviceId,4,5,6);
#endif	

	for(i=0;i<(epCount-1);i++)
	{
		for(j=(i+1);j<epCount;j++)
		{	
#ifdef RIO_DEEP_DEBUG

			RIO_DEEP_LOG("\ni=%d,,j=%d,\n",i,j,3,4,5,6);
#endif
			/*ensure "switchNo=0" is previous*/
			if(epNode[j].switchNo == 0) 
			{
				destSw.switchNo=epNode[i].switchNo;
				destSw.switchPort=epNode[i].switchPort;
				currentSw.switchNo=epNode[j].switchNo;
				currentSw.switchPort=epNode[j].switchPort;
				sourceDevId =epNode[j].deviceId;
				destDevId =epNode[i].deviceId;
			}
			else
			{				
				currentSw.switchNo=epNode[i].switchNo;
				currentSw.switchPort=epNode[i].switchPort;
				destSw.switchNo=epNode[j].switchNo;
				destSw.switchPort=epNode[j].switchPort;
				sourceDevId =epNode[i].deviceId;
				destDevId =epNode[j].deviceId;
			}

			/*get  the path of "B->C"*/
			gSwNodeCount=gPreSwNodeCount=gSaveSwNodeCount=0;
			gFindSwNodeFinal=0;	

			/* added by sjy */
			for(k=0;k<100;k++)
			{
				gSwIsGet[k] = -1;
			}
			gSwIsGet[currentSw.switchNo] = 1;

			/* added by sjy */
			rioFindSwNode(currentSw,destSw);
#ifdef RIO_DEEP_DEBUG

			RIO_DEEP_LOG("\ni=%d,,j=%d,,(%d,%d)<<-->>(%d,%d) \n",i,j,currentSw.switchNo,currentSw.switchPort,destSw.switchNo,destSw.switchPort);
			taskDelay(10);

			rioShow();
			taskDelay(10);
#endif
			/*if B and C is not at the same switch*/
			if(gPreSwNodeCount != 0)
			{
				/*backup "gPreSwNodeInfo" to "swNodeInfo"*/

				swNodeCount=gPreSwNodeCount;
				gPreSwNodeCount=0;
				hostSwNum=0;
				for(m=0;m<swNodeCount;m++)
				{
					swNodeInfo[m].switchNo=gPreSwNodeInfo[m].switchNo;
					swNodeInfo[m].switchPort=gPreSwNodeInfo[m].switchPort;
					if(swNodeInfo[m].switchNo==0) hostSwNum=m;

				}
				/*	printf("hostSwNum=%d\n",hostSwNum);*/
				if(hostSwNum==0)
				{
					/*get from the path of "host->B"*/
					gSwNodeCount=gPreSwNodeCount=gSaveSwNodeCount=0;
					gFindSwNodeFinal=0;	

					/* added by sjy */
					for(k=0;k<100;k++)
					{
						gSwIsGet[k] = -1;
					}
					gSwIsGet[currentSw.switchNo] = 1;

					/* added by sjy */
					rioFindSwNode(hostSw,currentSw);

					if(gPreSwNodeCount != 0)
					{
#ifdef RIO_DEEP_DEBUG
						RIO_DEEP_LOG("\n,(%d,%d)<<-->>(%d,%d) \n",hostSw.switchNo,hostSw.switchPort,currentSw.switchNo,currentSw.switchPort,5,6);
						taskDelay(10);
						rioShow();
						taskDelay(10);
#endif
						for(m=0;m<gPreSwNodeCount/2;m++)
						{

							rioRouteAddEntry(1,0xff,m,gPreSwNodeInfo[2*m].switchPort);
#ifdef RIO_DEEP_DEBUG

							rioConfigRead(1,0xff,m,0x6c,&result);
							RIO_DEEP_LOG("\nhost -> B::switchNo = 0x%x;;\n",result,2,3,4,5,6);
#endif
							rioRouteAddEntry(1,0x0,m+1,gPreSwNodeInfo[2*m+1].switchPort);
#ifdef RIO_DEEP_DEBUG

							rioConfigRead(1,0xff,m+1,0x6c,&result);
							RIO_DEEP_LOG("host -> B::switchNo1 = 0x%x;;\n",result,2,3,4,5,6);
#endif		
						}	
					}

					for(m=0;m<swNodeCount/2;m++)
					{
						rioRouteAddEntry(1,0xff,m+gPreSwNodeCount/2,swNodeInfo[2*m].switchPort);
#ifdef RIO_DEEP_DEBUG

						rioConfigRead(1,0xff,m+gPreSwNodeCount/2,0x6c,&result);
						RIO_DEEP_LOG("\nB -> C:::switchNo2 = 0x%x;;\n",result,2,3,4,5,6);
#endif
						rioRouteAddEntry(1,0x0,m+gPreSwNodeCount/2+1,swNodeInfo[2*m+1].switchPort);				
#ifdef RIO_DEEP_DEBUG

						rioConfigRead(1,0xff,m+gPreSwNodeCount/2+1,0x6c,&result);
						RIO_DEEP_LOG("B -> C:::switchNo3 = 0x%x;;\n",result,2,3,4,5,6);
#endif				
					}
					for(m=swNodeCount/2;m>0;m--)
					{
#ifdef RIO_DEEP_DEBUG			
						rioConfigRead(1,0xff,m+gPreSwNodeCount/2,0x6c,&result);
						RIO_DEEP_LOG("\n switchNo = 0x%x\n",result,2,3,4,5,6);
#endif
						rioRouteAddEntry(1,sourceDevId,m+gPreSwNodeCount/2,swNodeInfo[2*m-1].switchPort);
#ifdef RIO_DEEP_DEBUG

						rioConfigRead(1,0xff,m+gPreSwNodeCount/2-1,0x6c,&result);
						RIO_DEEP_LOG("\n switchNo1 = 0x%x\n",result,2,3,4,5,6);
#endif
						rioRouteAddEntry(1,destDevId,m+gPreSwNodeCount/2-1,swNodeInfo[2*m-2].switchPort);	
					}		
				}

				else
				{
#ifdef RIO_DEEP_DEBUG

					RIO_DEEP_LOG("hostSwNum=%d\n",hostSwNum,2,3,4,5,6);
					for(m=0;m<swNodeCount;m++)
						RIO_DEEP_LOG("%d::%d-->\n",swNodeInfo[m].switchNo,swNodeInfo[m].switchPort,3,4,5,6);	
					taskDelay(60);
#endif 

					for(m=(hostSwNum)/2;m>0;m--)
					{
						rioRouteAddEntry(1,0xff,(hostSwNum)/2-m,swNodeInfo[2*m-1].switchPort);
						rioRouteAddEntry(1,0,(hostSwNum)/2-m+1,swNodeInfo[2*m-2].switchPort);
#ifdef RIO_DEEP_DEBUG

						RIO_DEEP_LOG("1: %d::%d\n",(hostSwNum)/2-m,swNodeInfo[2*m-1].switchPort,3,4,5,6);
						RIO_DEEP_LOG("1: %d::%d\n\n",(hostSwNum)/2-m+1,swNodeInfo[2*m-2].switchPort,3,4,5,6);
#endif		
					}

					for(m=0;m<hostSwNum/2;m++)
					{
						rioRouteAddEntry(1,destDevId,hostSwNum/2-m,swNodeInfo[2*m].switchPort);
						rioRouteAddEntry(1,sourceDevId,hostSwNum/2-m-1,swNodeInfo[2*m+1].switchPort);
#ifdef RIO_DEEP_DEBUG

						RIO_DEEP_LOG("2: %d::%d\n",hostSwNum/2-m,swNodeInfo[2*m].switchPort,3,4,5,6);
						RIO_DEEP_LOG("2: %d::%d\n\n",hostSwNum/2-m-1,swNodeInfo[2*m+1].switchPort,3,4,5,6);
#endif	
					}

					for(m=hostSwNum/2;m<swNodeCount/2;m++)
					{
						rioRouteAddEntry(1,0xff,m-hostSwNum/2,swNodeInfo[2*m].switchPort);
						rioRouteAddEntry(1,0,m-hostSwNum/2+1,swNodeInfo[2*m+1].switchPort);
#ifdef RIO_DEEP_DEBUG

						RIO_DEEP_LOG("3: %d::%d\n",m-hostSwNum/2,swNodeInfo[2*m].switchPort,3,4,5,6);
						RIO_DEEP_LOG("3: %d::%d\n\n",m-hostSwNum/2+1,swNodeInfo[2*m+1].switchPort,3,4,5,6);
#endif
					}
					for(m=swNodeCount/2;m>hostSwNum/2;m--)
					{
						rioRouteAddEntry(1,sourceDevId,m-hostSwNum/2,swNodeInfo[2*m-1].switchPort);
						rioRouteAddEntry(1,destDevId,m-hostSwNum/2-1,swNodeInfo[2*m-2].switchPort);	
#ifdef RIO_DEEP_DEBUG

						RIO_DEEP_LOG("4: %d::%d\n",m-hostSwNum/2,swNodeInfo[2*m-1].switchPort,3,4,5,6);
						RIO_DEEP_LOG("4: %d::%d\n\n",m-hostSwNum/2-1,swNodeInfo[2*m-2].switchPort,3,4,5,6);
#endif
					}	
					/*			printf("end\n");*/
				}
			}
		}
	}
#if 0
	/*sysGetTime(&timeh3,&timel3);*/
	vxTimeBaseGet(&timeh3,&timel3);

	timeSpend1 = (timeh2 -timeh1 )*4.0*1024*1024*1024 + (timel2-timel1);
	/*timeSpend1 = timeSpend1*40/1000000000.0;*/
	timeSpend2 = timeSpend1*8/1000000000.0;

	timeSpend3 = (timeh3 -timeh2 )*4.0*1024*1024*1024 + (timel3-timel2);
	/*timeSpend2 = timeSpend2*40/1000000000.0;*/
	timeSpend4 = timeSpend3*8/1000000000.0;
	printf("%d:%d--%d:%d--%d:%d\n",timeh1,timel1,timeh2,timel2,timeh3,timel3);
	printf("\n %f --- %f \n",timeSpend2,timeSpend4);
#endif
	/*RIO_LOG("show rapidIo net info\n",1,2,3,4,5,6);
	rioShowRapidIoInfo();
	 */
	return OK;
}
#endif
#if 0
STATUS rioScanDevice()
{
	SWNODEINFO currentSw,destSw,swNodeInfo[maxSwCount],hostSw;
	EPNODEINFO epNode[maxEpCount];
	UINT8 sourceDevId,destDevId,hostSwNum=0;

	UINT16 epCount,i,j,m,swNodeCount;
	/*UINT32 result;*/
	UINT16 hostId=0;
#if 0
	UINT32 timeh1,timeh2,timeh3,timel1,timel2,timel3;
	float timeSpend3,timeSpend1,timeSpend2,timeSpend4;
#endif

#if 0	
	vxTimeBaseGet(&timeh1,&timel1);
	/*sysGetTime(&timeh1,&timel1);*/
#endif
	/* If somebody else enumerated our master port device, fail. */
	printf("enter rioEnumHost\n");
	taskDelay(20);
	if (rioEnumHost(hostId) == ERROR)
	{
		ns16550Pollprintf("ERROR!!!RIO: master port %d device has been enumerated by a remote host\n",hostId);
		return ERROR;
	}
	/* If master port has an active link, allocate net and enum peers */
	printf("enter rioMasterPortIsActive\n");
	taskDelay(20);
	if (rioMasterPortIsActive()) 
	{
		if(rioScanLockDevice(1,0,hostId) == ERROR)
		{
			ns16550Pollprintf("ERROR!!!RIO: scan device fail!!\n");
			return ERROR;
		}
	}
	else
	{
		ns16550Pollprintf("RapidIO Hardware not connnected\n");
		return ERROR;

	}    
	printf("enter rioFindEndPoint\n");
	taskDelay(20);
	epCount=rioFindEndPoint(epNode);
	printf("epCount = %d\n",epCount);
#if 0
	vxTimeBaseGet(&timeh2,&timel2);
	/*sysGetTime(&timeh2,&timel2);*/
#endif

	for(i=0;i<epCount;i++)
	{
		if((epNode[i].switchNo == 0 )&&(epNode[i].deviceId == hostId))
		{
			hostSw.switchNo=0;
			hostSw.switchPort=epNode[i].switchPort;
			break;
		}
	}

#if 0
	for(i=0;i<epCount;i++)
		printf("\nepNode--switchNo = %d,,switchPort = %d,,deviceId = %d \n",epNode[i].switchNo,epNode[i].switchPort,epNode[i].deviceId);
#endif	

	for(i=0;i<(epCount-1);i++)
	{
		for(j=(i+1);j<epCount;j++)
		{			
			/*ensure "switchNo=0" is previous*/
			if(epNode[j].switchNo == 0) 
			{
				destSw.switchNo=epNode[i].switchNo;
				destSw.switchPort=epNode[i].switchPort;
				currentSw.switchNo=epNode[j].switchNo;
				currentSw.switchPort=epNode[j].switchPort;
				sourceDevId =epNode[j].deviceId;
				destDevId =epNode[i].deviceId;
			}
			else
			{				
				currentSw.switchNo=epNode[i].switchNo;
				currentSw.switchPort=epNode[i].switchPort;
				destSw.switchNo=epNode[j].switchNo;
				destSw.switchPort=epNode[j].switchPort;
				sourceDevId =epNode[i].deviceId;
				destDevId =epNode[j].deviceId;
			}

			/*get  the path of "B->C"*/
			gSwNodeCount=gPreSwNodeCount=gSaveSwNodeCount=0;
			gFindSwNodeFinal=0;		
			rioFindSwNode(currentSw,destSw);
#if 0
			printf("\ni=%d,,j=%d,,(%d,%d)<<-->>(%d,%d) \n",i,j,currentSw.switchNo,currentSw.switchPort,destSw.switchNo,destSw.switchPort);
			taskDelay(10);
			rioShow();
			taskDelay(10);
#endif
			/*if B and C is not at the same switch*/
			if(gPreSwNodeCount != 0)
			{
				/*backup "gPreSwNodeInfo" to "swNodeInfo"*/

				swNodeCount=gPreSwNodeCount;
				gPreSwNodeCount=0;
				for(m=0;m<swNodeCount;m++)
				{
					swNodeInfo[m].switchNo=gPreSwNodeInfo[m].switchNo;
					swNodeInfo[m].switchPort=gPreSwNodeInfo[m].switchPort;
					if(swNodeInfo[m].switchNo==0) hostSwNum=m;
				}
				if(hostSwNum==0)
				{

					/*get from the path of "host->B"*/
					gSwNodeCount=gPreSwNodeCount=gSaveSwNodeCount=0;
					gFindSwNodeFinal=0;		
					rioFindSwNode(hostSw,currentSw);

					if(gPreSwNodeCount != 0)
					{
#if 0
						printf("\ni=%d,,j=%d,,(%d,%d)<<-->>(%d,%d) \n",i,j,hostSw.switchNo,hostSw.switchPort,currentSw.switchNo,currentSw.switchPort);
						taskDelay(10);
						rioShow();
						taskDelay(10);
#endif
						for(m=0;m<gPreSwNodeCount/2;m++)
						{

							rioRouteAddEntry(1,0xff,m,gPreSwNodeInfo[2*m].switchPort);
#if 0
							rioConfigRead(1,0xff,m,0x6c,&result);
							printf("\nhost -> B::switchNo = 0x%x;;\n",result);
#endif
							rioRouteAddEntry(1,0x0,m+1,gPreSwNodeInfo[2*m+1].switchPort);
#if 0
							rioConfigRead(1,0xff,m+1,0x6c,&result);
							printf("host -> B::switchNo1 = 0x%x;;\n",result);
#endif		
						}	
					}

					for(m=0;m<swNodeCount/2;m++)
					{
						rioRouteAddEntry(1,0xff,m+gPreSwNodeCount/2,swNodeInfo[2*m].switchPort);
#if 0
						rioConfigRead(1,0xff,m+gPreSwNodeCount/2,0x6c,&result);
						printf("\nB -> C:::switchNo2 = 0x%x;;\n",result);
#endif
						rioRouteAddEntry(1,0x0,m+gPreSwNodeCount/2+1,swNodeInfo[2*m+1].switchPort);				
#if 0
						rioConfigRead(1,0xff,m+gPreSwNodeCount/2+1,0x6c,&result);
						printf("B -> C:::switchNo3 = 0x%x;;\n",result);
#endif				
					}
					for(m=swNodeCount/2;m>0;m--)
					{
#if 0			
						rioConfigRead(1,0xff,m+gPreSwNodeCount/2,0x6c,&result);
						printf("\n switchNo = 0x%x\n",result);
#endif
						rioRouteAddEntry(1,sourceDevId,m+gPreSwNodeCount/2,swNodeInfo[2*m-1].switchPort);
#if 0
						rioConfigRead(1,0xff,m+gPreSwNodeCount/2-1,0x6c,&result);
						printf("\n switchNo1 = 0x%x\n",result);
#endif
						rioRouteAddEntry(1,destDevId,m+gPreSwNodeCount/2-1,swNodeInfo[2*m-2].switchPort);	
					}		
				}
				else
				{
#if 1
					printf("hostSwNum=%d\n",hostSwNum);
					for(m=0;i<swNodeCount;i++)
						printf("%d::%d-->\n",swNodeInfo[i].switchNo,swNodeInfo[i].switchPort);					
#endif 
					for(m=hostSwNum-1;m>=0;m--)
					{
						rioRouteAddEntry(1,0xff,m,swNodeInfo[m].switchPort);
						rioRouteAddEntry(1,0,m-1,swNodeInfo[m-1].switchPort);
						m--;
					}
					for(m=0;m<hostSwNum;m++)
					{
						rioRouteAddEntry(1,destDevId,m,swNodeInfo[m].switchPort);
						rioRouteAddEntry(1,sourceDevId,m+1,swNodeInfo[m+1].switchPort);
						m++;		
					}

					for(m=hostSwNum;m<swNodeCount;m++)
					{
						rioRouteAddEntry(1,0xff,m,swNodeInfo[m].switchPort);
						rioRouteAddEntry(1,0,m+1,swNodeInfo[m+1].switchPort);
						m++;
					}
					for(m=swNodeCount-1;m>=hostSwNum;m--)
					{
						rioRouteAddEntry(1,sourceDevId,m,swNodeInfo[m].switchPort);
						rioRouteAddEntry(1,destDevId,m-1,swNodeInfo[m-1].switchPort);
						m--;		
					}					
				}
			}
		}
	}
#if 0
	/*sysGetTime(&timeh3,&timel3);*/
	vxTimeBaseGet(&timeh3,&timel3);

	timeSpend1 = (timeh2 -timeh1 )*4.0*1024*1024*1024 + (timel2-timel1);
	/*timeSpend1 = timeSpend1*40/1000000000.0;*/
	timeSpend2 = timeSpend1*8/1000000000.0;

	timeSpend3 = (timeh3 -timeh2 )*4.0*1024*1024*1024 + (timel3-timel2);
	/*timeSpend2 = timeSpend2*40/1000000000.0;*/
	timeSpend4 = timeSpend3*8/1000000000.0;
	printf("%d:%d--%d:%d--%d:%d\n",timeh1,timel1,timeh2,timel2,timeh3,timel3);
	printf("\n %f --- %f \n",timeSpend2,timeSpend4);
#endif
	/*RIO_LOG("show rapidIo net info\n",1,2,3,4,5,6);
	rioShowRapidIoInfo();
	 */
	return OK;
}
#endif

#if 0
STATUS rioReleaseLockAll(UINT16 hostId)
{
	EPNODEINFO epNode[maxEpCount];
	SWNODEINFO hostSw;
	UINT16 epCount,i,j,hostLockId;

	/*release all endPoint lock*/ 
	epCount=rioFindEndPoint(epNode);
	for(i=0;i<epCount;i++)
	{
		if((epNode[i].switchNo == 0 )&&(epNode[i].deviceId == hostId))
		{
			hostSw.switchNo=0;
			hostSw.switchPort=epNode[i].switchPort;
			if(rioReleaseLocalLock(hostId,&hostLockId) == ERROR) goto error;
		}
		else
			if(rioReleaseLock(1,epNode[i].deviceId,0xff,hostId,&hostLockId) == ERROR) goto error;
	}
	/*release all switch lock*/ 

}

#endif


STATUS rioScanLockDevice(UINT8 localPort,UINT8 hopCount,UINT16 hostId,UINT8 mode)
{
	UINT16 currentSwNo,previousSwNo,previousSwPort,currentSwPort,hostLockId;
	UINT32 result,switchNo;
	UINT8 portNum,hostPort,i;/*,j*/
	UINT8 deviceLockflag=0;
	UINT16 epRapidIoId;
	UINT32 retryCount=0;
	UINT32 rioScanStop=0;
	UINT32 regVal = 0;
	RIO_LOG("enter rioScanLockDevice\n",1,2,3,4,5,6);

#ifdef RIO_EP_DEBUG
	RIO_EP_DEEP_LOG("Enter rioScanLockDevice 0 in Sw !\n",1,2,3,4,5,6);
#endif
#if 1
	if(rioAcquirLock(localPort,0xff,hopCount,hostId,&hostLockId)==ERROR)
	{
		deviceLockflag=1;
		/*printf("RIO: hopCount = %d--- hostId =%d\n",hopCount,hostId);*/
		if(hostLockId != hostId ) 		
		{
			printf("RIO: PE already discovered by this host(ID = %d)\n",hostLockId);
			return ERROR;
		}
		/*else return OK;*/
	}
#endif	


	regVal = rioGetPEF(localPort,0xff,hopCount);
	RIO_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",hopCount,regVal,3,4,5,6);
	if(regVal == 0x80000000)
	{
		return ERROR;
	}
	if(regVal&RIO_PEF_SWITCH) //sjy 20130428 alter if(rioIsSwitch(localPort,0xff,hopCount))
	{
#ifdef RIO_DEEP_DEBUG	
		//RIO_DEEP_LOG("Get a switch!\n",1,2,3,4,5,6);
#endif
		rioConfigRead(localPort,0xff,hopCount,0x6c,&switchNo);
		/*	printf("\n hopCount=%d,switchNo=0x%x \n",hopCount,switchNo);*/
		if(!(switchNo&0x80000000))  
		{
			if(deviceLockflag) 
			{
				ns16550Pollprintf("SRIO:ERROR!!!the sw has been locked ,but the swNO is not set!!!\n");
				deviceLockflag=0;
				return ERROR;
			}
			rioConfigWrite(localPort,0xff,hopCount,0x6c,gSwitchCount|0x80000000);
			currentSwNo=gSwitchCount;
			gSwitchCount++;
		}
		else currentSwNo= switchNo&0xffff;

		rioGetSwPortInf(localPort,hopCount,&result);

		hostPort = result&0xff;
		portNum = (result&0xff00)>>8;


		if(hopCount != 0)
		{
			rioConfigRead(localPort,0xff,hopCount-1,0x6c,&switchNo);
			if(!(switchNo&0x80000000))  return ERROR;
			else previousSwNo= switchNo&0xffff;

			previousSwPort=gSwInfo[previousSwNo].currentActivePort;
			/*currentSwPort=gSwInfo[currentSwNo].currentActivePort;*/

			gSwInfo[previousSwNo].swPortInfo[previousSwPort].swPortFlag=1;
			gSwInfo[previousSwNo].swPortInfo[previousSwPort].switchNo = currentSwNo;
			gSwInfo[previousSwNo].swPortInfo[previousSwPort].switchPort = hostPort;/*currentSwPort;*/		

			gSwInfo[currentSwNo].swPortInfo[hostPort].swPortFlag=1;
			gSwInfo[currentSwNo].swPortInfo[hostPort].switchNo= previousSwNo;
			gSwInfo[currentSwNo].swPortInfo[hostPort].switchPort = previousSwPort;
#ifdef RIO_DEEP_DEBUG

			RIO_DEEP_LOG("\n",1,2,3,4,5,6);
			RIO_DEEP_LOG("/***************RIO:add switch INFO***************/\n",1,2,3,4,5,6);
			RIO_DEEP_LOG("switch %d::port %d<----->switch %d::%d \n",previousSwNo,previousSwPort,currentSwNo,hostPort,5,6);
			RIO_DEEP_LOG("\n",1,2,3,4,5,6);
			/*#else*/
			RIO_DEEP_LOG("switch   %d: %d  %d %d %d \n",previousSwNo,previousSwPort,1,currentSwNo,hostPort,6);
			RIO_DEEP_LOG("switch   %d: %d  %d %d %d \n",currentSwNo,hostPort,1,previousSwNo,previousSwPort,6);
#endif

		}
		else 
		{		

			gSwInfo[currentSwNo].swPortInfo[hostPort].swPortFlag =2;
			gSwInfo[currentSwNo].swPortInfo[hostPort].switchNo = currentSwNo;
			gSwInfo[currentSwNo].swPortInfo[hostPort].switchPort = hostPort;
			gSwInfo[currentSwNo].swPortInfo[hostPort].deviceId = hostId;

			rioRouteAddEntry(localPort,hostId,hopCount,hostPort); /*hdz*/


#ifdef RIO_DEEP_DEBUG	
			RIO_DEEP_LOG("\n",1,2,3,4,5,6);
			RIO_DEEP_LOG("/***************RIO:add EndPoint INFO***************/\n",1,2,3,4,5,6);
			RIO_DEEP_LOG("SW:ID %d -----> switch %d::hostPort %d :: hostId %d \n",hostId,currentSwNo,hostPort,hostId,5,6);
			RIO_DEEP_LOG("\n",1,2,3,4,5,6);
			/*#else*/
			//			RIO_DEEP_LOG("EndPoint %d: %d  %d %d %d %d \n",currentSwNo,hostPort,2,currentSwNo,hostPort,hostId);
#endif

		}
		if(deviceLockflag) return OK;
		gSwInfo[currentSwNo].portNum=portNum;

		rioRouteAddEntry(localPort,hostId,hopCount,hostPort);
		for(i=0;i<portNum;i++)
		{
			gSwInfo[currentSwNo].currentActivePort=i;
			if(i == hostPort)
			{
				rioConfigRead(localPort,0xff,hopCount,0x15c+0x20*i,&result);
				//printf("%d:result 1= 0x%x\n",i,result);
				result |= 0x600000;
				//printf("%d:result 2= 0x%x\n",i,result);
				rioConfigWrite(localPort,0xff,hopCount,0x15c+0x20*i,result);
				continue; 
			}
			//	if(i==2) continue; /*hdz add 20110309*/
#ifdef RIO_DEEP_DEBUG	
			RIO_DEEP_LOG("Current switch %d,current port %d!\n",currentSwNo,i,3,4,5,6);
#endif
			if(rioSwPortIsActive(localPort,0xff,hopCount,i))
			{	
				/*printf("gSwInfo[%d].currentActivePort = %d \n",currentSwNo,i);*/
				rioConfigRead(localPort,0xff,hopCount,0x15c+0x20*i,&result);
				//printf("%d:result 1= 0x%x\n",i,result);
				result |= 0x600000;
				//printf("%d:result 2= 0x%x\n",i,result);
				rioConfigWrite(localPort,0xff,hopCount,0x15c+0x20*i,result);
				
				/*printf("gSwInfo[%d].currentActivePort = %d \n",currentSwNo,i);*/
				rioRouteAddEntry(localPort,0xff,hopCount,i);
				/*debug*/
				rioConfigWrite(localPort,0xff,hopCount,0x140+0x20*i,4);
				taskDelay(10);
				rioConfigRead(localPort,0xff,hopCount,0x158+0x20*i,&regVal);
				RIO_DEEP_LOG("hopCount %d port %d error&status 0x%x!\n",hopCount,i,regVal,4,5,6);
				rioConfigRead(localPort,0xff,hopCount,0x1040+0x40*i,&regVal);
				RIO_DEEP_LOG("----debug hopCount %d port %d err detect value 0x%x!\n",hopCount,i,regVal,4,5,6);
				//				rioConfigRead(localPort,0xff,hopCount,0xF40008+0x100*i,&regVal);
				//				RIO_DEEP_LOG("----debug hopCount %d port %d specific err detect value 0x%x!\n",hopCount,i,regVal,4,5,6);
				/*debug*/
				if (rioScanLockDevice(localPort,hopCount + 1,hostId,mode) < 0)
				{

					return ERROR;
				}
			}
			else gSwInfo[currentSwNo].swPortInfo[i].swPortFlag=0;

		}

	}
	if(rioGetPEF(localPort,0xff,hopCount)&0xe0000000) //sjy 20130428 alter **else
	{	
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter ep info 0 in Sw %d!\n",currentSwNo,2,3,4,5,6);
#endif
		rioConfigRead(localPort,0xff,hopCount-1,0x6c,&switchNo);
		if(!(switchNo&0x80000000))  
		{
			ns16550Pollprintf("RIO:ERROR!!!get switchNo ERROR!!\n");
			return ERROR;
		}
		currentSwNo= switchNo&0xffff;

		currentSwPort=gSwInfo[currentSwNo].currentActivePort;

		/*if(currentId == hostId) currentId++;*/
		/*epRapidIoId=rioGetEpRapidIoId();*/
		//#ifndef INCLUDE_RAPIDIO_SLOTNUM
		if(mode==1)
		{
			epRapidIoId=deviceId;
			deviceId++;
			ns16550Pollprintf("rioConfigRead deviceId++ \n");
		}
		//#else
		else
		{
#if 0
			rioConfigRead(localPort,0xff,hopCount,0x8,&result);
			epRapidIoId=result&0xffff;
			/*	printf("result=0x%x;;;epRapidIoId=0x%x\n",result,epRapidIoId);*/
#endif

#ifdef RIO_EP_DEBUG
			RIO_EP_DEEP_LOG("Get ep info 1!\n",1,2,3,4,5,6);
#endif
			rioConfigRead(localPort,0xff,hopCount,0x8,&result);
#ifdef RIO_EP_DEBUG
			RIO_EP_DEEP_LOG("Get ep info 2!\n",1,2,3,4,5,6);
#endif
			epRapidIoId=result&0xffff;
			if(epRapidIoId==0xff) 
			{
				ns16550Pollprintf("slot num is error!!\n");
				return ERROR;
			}
			else if(epRapidIoId==0) 
			{

				while(!rioScanStop)
				{
					retryCount=0xfffffffe;

					rioConfigRead(localPort,0xff,hopCount,0x8,&result);
					epRapidIoId=result&0xffff;
					if((epRapidIoId==0xff))
					{
						ns16550Pollprintf("slot num is error!!\n");
						return ERROR;
					}
					else if(epRapidIoId!=0) break;
					else if(retryCount == 0)
					{	
						ns16550Pollprintf("slot num is error!!\n");
						return ERROR;
					}
					retryCount--;
				}

			}
		}
		//#endif
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Get ep info 3 in Sw %d!\n",currentSwNo,2,3,4,5,6);
#endif

		rioConfigWrite(localPort,0xff,hopCount,0x60,epRapidIoId<<16);
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Get ep info 4!\n",1,2,3,4,5,6);
#endif

#if 0
		rioConfigRead(localPort,0xff,hopCount,0x13c,&result); /*hdz*/
		rioConfigWrite(localPort,0xff,hopCount,0x13c,result|0x40000000);
#endif		
		rioRouteAddEntry(localPort,epRapidIoId,hopCount-1,currentSwPort); /*hdz*/

		gSwInfo[currentSwNo].swPortInfo[currentSwPort].swPortFlag =2;
		gSwInfo[currentSwNo].swPortInfo[currentSwPort].switchNo = currentSwNo;
		gSwInfo[currentSwNo].swPortInfo[currentSwPort].switchPort = currentSwPort;
		gSwInfo[currentSwNo].swPortInfo[currentSwPort].deviceId =epRapidIoId;/* currentId;*/
#ifdef RIO_DEEP_DEBUG

		RIO_DEEP_LOG("\n",1,2,3,4,5,6);
		RIO_DEEP_LOG("/***************RIO:add EndPoint INFO***************/\n",1,2,3,4,5,6);
		RIO_DEEP_LOG("EP:ID %d -----> switch %d::port %d \n",epRapidIoId,currentSwNo,currentSwPort,4,5,6);
		RIO_DEEP_LOG("\n",1,2,3,4,5,6);
		/*#else*/
		//	RIO_DEEP_LOG("EndPoint %d: %d  %d %d %d %d \n",currentSwNo,currentSwPort,2,currentSwNo,currentSwPort,epRapidIoId);
#endif		
		/*currentId++;*/

	}	
	RIO_LOG("exit rioScanLockDevice\n",1,2,3,4,5,6);
	return OK;	
}



void rioShowRapidIoInfo()
{
	UINT16 i,j;
	printf("******show switch INFO******\n");
	/*RIO_LOG("******show switch INFO******\n",1,2,3,4,5,6);;*/
	for(i=0;i<gSwitchCount;i++) 
	{
		printf("\n*************switch:%d  has %d ports  currentActivePort:%d ************\n",i,gSwInfo[i].portNum,gSwInfo[i].currentActivePort);
		/*RIO_LOG("\n*************switch:%d  has %d ports  currentActivePort:%d ************\n",i,gSwInfo[i].portNum,gSwInfo[i].currentActivePort,3,4,5,6);*/
		for(j=0;j<gSwInfo[i].portNum;j++)
		{
			if(gSwInfo[i].swPortInfo[j].swPortFlag == 0)
			{
				printf("port %d : not connect!!\n",j);
				/*RIO_LOG("port %d : not connect!!\n",j,2,3,4,5,6);*/
			}
			else
			{
				printf("port:%d",j);
				/*RIO_LOG("port:%d",j,2,3,4,5,6);*/
				if(gSwInfo[i].swPortInfo[j].swPortFlag == 1) 
				{

					printf("    switch   ");
					printf("SWNO:%d   SWPORT:%d   \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort);
					/*
					RIO_LOG("    switch   ",1,2,3,4,5,6);
					RIO_LOG("SWNO:%d   SWPORT:%d   \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort,3,4,5,6);
					 */
				}

				else
				{

					printf("    EndP     ");
					printf("SWNO:%d   SWPORT:%d   DevId:%d  \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort,gSwInfo[i].swPortInfo[j].deviceId);
					/*	
					RIO_LOG("    EndP     ",1,2,3,4,5,6);
					RIO_LOG("SWNO:%d   SWPORT:%d   DevId:%d  \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort,gSwInfo[i].swPortInfo[j].deviceId,4,5,6);
					 */	
				}
			}
		}
	}	
}






#if 0
UINT16 rioCreadNode()

{
	UINT16 neighborSwCount,sourceSwId,destSwId,i,j;
	SWNODEINFO *pSwNodeInfo;
	SWNODEINFO sourceSwNodeInfo
	/*SWNODEINFO currentSwPathInfo[switchCount],preSwPathInfo[switchCount];*/





	SWNODE *pSwNodeHeader;
	pSwNodeInfo=(SWNODE*)malloc(sizeof(SWNODE));
	pSwNodeHeader->pNextSwNode = NULL;
	pSwNodeHeader->swNodeInfo.switchNo = sourceSwId;
	pSwNodeHeader->swNodeInfo.switchPort = ;

	pSwNodeInfo=(SWNODEINFO*)malloc(gSwInfo[sourceSwId].portNum*sizeof(SWNODEINFO));

	neighborSwCount = rioGetNeighborSw(sourceSwId,pSwNodeInfo);

	for(i=0;i<neighborSwCount;i++)
	{
		if(pSwNodeInfo->switchNo == destSwId) 
	}


}
#endif


#if 1
static UINT16 currentId=2;
static UINT16 switchCount=0;
static UINT16 portNo[255];

/*static UINT16 epCount=0;*//*,currentSW=0;*/

int rioEnumPeer(UINT8 localPort,UINT8 hopCount,UINT16 hostId)
{
	UINT16 hostLockId;
	UINT32 result,switchNo;
	UINT8 portNum,hostPort,i,j;
	/* Attempt to acquire device lock */
	/*printf("RIO:rioEnumPeer hopCount=%d;hostId=%d\n",hopCount,hostId );*/
#if 1
	if(rioAcquirLock(localPort,0xff,hopCount,hostId,&hostLockId)==ERROR)
	{
		printf("RIO: PE already discovered by this host(ID = %d)\n",hostLockId);
		/*printf("RIO: hopCount = %d--- hostId =%d\n",hopCount,hostId);*/
		if(hostLockId != hostId ) 		return ERROR;
		else return OK;
	}
#endif
	if(rioIsSwitch(localPort,0xff,hopCount))
	{
		printf("RIO:find a switch device--gSwitchCount = %d --hopCount = %d \n",gSwitchCount,hopCount);
		rioConfigWrite(localPort,0xff,hopCount,0x6c,gSwitchCount);
		/*rioConfigWrite(1,0xff,0,0x6c,1);*/
		switchCount++;
		/*currentSW++;*/
		rioGetSwPortInf(localPort,hopCount,&result);
		hostPort = result&0xff;
		portNum = (result&0xff00)>>8;
		/*printf("RIO:switch has %d port,the hostport is %d\n",portNum,hostPort);*/
		rioRouteAddEntry(localPort,hostId,hopCount,hostPort);
		for(i=0;i<portNum;i++)
		{
			if((i == hostPort)) continue;
			if(rioSwPortIsActive(localPort,0xff,hopCount,i))
			{
				rioConfigRead(localPort,0xff,hopCount,0x6c,&switchNo);
				/*rioConfigRead(1,0xff,0,0x6c,&switchNo);*/
				/*printf("RIO:swtitch %d -- port %d is active\n",switchNo,i);*/
				portNo[hopCount]=i;
				rioRouteAddEntry(localPort,0xff,hopCount,i);
				if (rioEnumPeer(localPort,hopCount + 1,hostId) < 0) return ERROR;
			}
		}
		/*portNo[switchCount-1]=0;*/
		/*switchCount--;*/

	}
	else
	{
		/*printf("RIo:switch count = %d; hopcount = %d\n",switchCount,hopCount);*/
		printf("RIO:find a endpoint at switch %d  port %d\n",switchCount-1,portNo[switchCount-1]);

		for(j=0;j<hopCount;j++) 
			rioRouteAddEntry(localPort,currentId,j,portNo[j]);

		printf("RIO:set EP ID= %d\n",currentId);
		rioSetDeviceId(localPort,0xff,hopCount, currentId);
		currentId++;
		if(currentId == hostId) currentId++;
	}	
	return OK;
}


void rioEnumComplete(void)
{


}


STATUS rioEnumDevice()
{
	UINT16 hostId;
	rioLocalGetDeviceId(&hostId);
	printf("RIO: enumerate master port %d\n",hostId);

	/* If somebody else enumerated our master port device, fail. */
	if (rioEnumHost(hostId) < 0)
	{
		printf("ERROR!!!RIO: master port %d device has been enumerated by a remote host\n",hostId);
		/*rc = ERROR;
		goto out;*/
		return ERROR;
	}
	/* If master port has an active link, allocate net and enum peers */
	if (rioMasterPortIsActive()) 
	{
		if(rioEnumPeer(1,0,hostId)== ERROR) return ERROR;
		else rioEnumComplete();
	}
	return OK;
}


#endif



#if 0
STATUS rioDiscoveryScan1
(
		UINT8 localPort,
		UINT16 hostDeviceId,
		UINT8 hopCount,
		UINT32 devIdStart
)
{
	UINT32 portCount,hostPortNum,i,devIdAssign,devId,regVal;
	UINT8 hopCountNum;

	hopCountNum = hopCount;
	devIdAssign = devIdStart;

	/*host read port NO*/
	if(rioConfigRead(localPort,0xff,hopCount,0x14,&regVal) == ERROR) goto error;
	portCount = (regVal>>8) & 0xff;
	hostPortNum = regVal & 0xff;
	RIO_LOG("portCount is %d, hostportNum is %d\n",portCount,hostPortNum,3,4,5,6);

	if(rioConfigRead(localPort,0xff,hopCount,0x68,&regVal) == ERROR) goto error;
	if (regVal != 0xffff)
	{
		RIO_LOG("switch TSI578 has locked by another host\n",1,2,3,4,5,6);
		return ERROR;
	}
	if(rioConfigWrite(localPort,0xff,hopCount,0x68,hostDeviceId) == ERROR) goto error;
	/* route the response packet to host port */	
	if(rioConfigWrite(localPort,0xff,hopCount,0x70,hostDeviceId) == ERROR) goto error;
	if(rioConfigWrite(localPort,0xff,hopCount,0x74,hostPortNum) == ERROR) goto error;

	RIO_LOG("scan switch TSI578 port.....\n",1,2,3,4,5,6);
	for(i=0;i<portCount;i++)
	{
		if(i == hostPortNum) continue;

		if(rioConfigRead(localPort,0xff,hopCount,0x158+i*0x20,&regVal) == ERROR) goto error;
		if((regVal &0x2) != 0x2) continue;
		RIO_LOG("find a device from port %d \n",i,2,3,4,5,6);
		devId=0xff;
		if(rioConfigWrite(localPort,0xff,hopCount,0x70,devId) == ERROR) goto error;
		if(rioConfigWrite(localPort,0xff,hopCount,0x74,i) == ERROR) goto error;

		if(rioConfigRead(localPort,0xff,hopCount,0,&regVal)== ERROR) goto error;

		if((regVal != 0x70110002)&& (regVal != 0x0578000d))
		{
			RIO_LOG("didn't support a device other than mpc8641d and Tsi578\n",1,2,3,4,5,6);
			continue;
		}
		else if(regVal == 0x70110002)
		{	
			RIO_LOG("It is MPC8640d\n",1,2,3,4,5,6);
			if(rioConfigRead(localPort,0xff,hopCount+1,0x68,&regVal)== ERROR) goto error;
			if (regVal != 0xffff)
			{
				RIO_LOG("mpc8641d has locked by another host\n",1,2,3,4,5,6);
				goto error;
			}
			if(rioConfigWrite(localPort,0xff,hopCount+1,0x68,hostDeviceId)== ERROR) goto error;   
			/* update the devID */
			RIO_LOG("update devID\n",1,2,3,4,5,6);
			devId = devIdAssign<<16;
			if(rioConfigWrite(localPort,0xff,hopCount+1,0x60,devId)== ERROR) goto error; 

			RIO_LOG("update route\n",1,2,3,4,5,6);
			if(rioConfigWrite(localPort,0xff,hopCount,0x70,devIdAssign)== ERROR) goto error;  
			if(rioConfigWrite(localPort,0xff,hopCount,0x74,i)== ERROR) goto error;  

			/*set the device to agen*/
			if(rioConfigRead(localPort,devIdAssign,0xff,0x13c,&regVal)== ERROR) goto error; 
			regVal |= 0x40000000;
			if(rioConfigWrite(localPort,devIdAssign,0xff,0x74,regVal)== ERROR) goto error; 

			devIdAssign++;          
		}
		else if(regVal == 0x0578000d)
		{
			RIO_LOG("it is TSI578\n",1,2,3,4,5,6);
			hopCountNum++;
			rioDiscoveryScan1(localPort,hostDeviceId,hopCountNum,devIdAssign);
		}	
	}
	return OK;		
	error:
	return ERROR;
}


STATUS sysRioDiscoveryScan()
{

	UINT32 devIdAssign,regVal;
	UINT16 hostDeviceId,localPort;

	localPort=1;
	hostDeviceId=*(UINT32*)(0xf80c0060);
	devIdAssign = hostDeviceId+1;

	RIO_LOG("the host device ID is %d\n",hostDeviceId,2,3,4,5,6);

	if(rioConfigRead(localPort,0xff,0,0,&regVal)== ERROR) goto error;
	if(regVal == 0x0578000d)
	{
		RIO_LOG("find a switch TSI578\n",1,2,3,4,5,6);
		if(rioDiscoveryScan1(1,hostDeviceId,0,devIdAssign)==ERROR) goto error;
	}
	return OK;
	error:
	return ERROR;

}
#endif
#if 0
void rioConfigReadNB(UINT8 hopCount,UINT32 offset,UINT32 count,UINT32 *readData)
{
	UINT32 i,data;
	for(i=0;i<count;i+=4)
	{
		rioConfigRead(1,0xff,hopCount,offset+i,&data);
		printf("0x%x:0x%x\n",offset+i,data);
		*readData++=data;
	}
}

void sysCpuRegReadNB(UINT32 offset,UINT32 count,UINT32 *readData)
{
	UINT32 i,data;
	for(i=0;i<count;i+=4)
	{
		data=*(UINT32*)(offset+i);
		printf("0x%x:0x%x\n",offset+i,data);
		*readData++=data;
	}
}

void rioPowerDown(UINT8 cpuNum)
{
	UINT32 data;
	UINT8 portNum,num[4]={2,0,6,4};
	portNum=num[cpuNum]/2;
	rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
	if((data>>2)&0x1 == 1) printf("fail::port %d has been powerDown\n",portNum*2);
	else
	{
		rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
		data |= (1<<2);

		rioConfigWrite(1,0xff,0,(0x130c8+portNum*0x200),data);
		rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;

		while((data>>2)&0x1 == 0)
		{
			data |= (1<<2);
			rioConfigWrite(1,0xff,0,(0x130c8+portNum*0x200),data);
			rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
		}
	}
	printf("port %d  powerDown\n",portNum*2);
}


void rioPowerUp(UINT8 cpuNum)
{
	UINT32 data;
	UINT8 portNum,num[4]={2,0,6,4};
	portNum=num[cpuNum]/2;	 
	rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
	if((data>>2)&0x1 == 0) printf("fail::port %d has been powerUp\n",portNum*2);
	else
	{
		rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
		data &=(~(1<<2));
		rioConfigWrite(1,0xff,0,(0x130c8+portNum*0x200),data);
		rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
		while((data>>2)&0x1 == 1)
		{
			data &=(~(1<<2));
			rioConfigWrite(1,0xff,0,(0x130c8+portNum*0x200),data);
			rioConfigRead(1,0xff,0,(0x130c8+portNum*0x200),&data) ;
		}
	}
	printf("port %d  powerUp\n",portNum*2);
}




void rioInit(int targetID)
{
	UINT32 InLocalADDR,InSrioADDR,OutTargetADDR,OutSrioADDR,mm;
	InLocalADDR = 0x20000000;
	InSrioADDR = 0x80000000;
	OutTargetADDR = 0xc0000000;
	OutSrioADDR = 0x80000000;

	sysRioSetOW(0,targetID,OutSrioADDR,OutTargetADDR,28);
	logMsg("init data...\n",0,0,0,0,0,0);
	for(mm=0;mm<0x10000000;mm+=4)	*((UINT*)(InLocalADDR+mm)) = mm;
	sysRioSetIW(0,InLocalADDR,InSrioADDR,28);
}

void getCpuStatus()
{
	printf("CpuStatus:0x%x",*(UINT32*)0xe80c0158);
}

void get578Status(UINT8 cpuNum)
{
	UINT32 data;
	UINT8 portNum,num[4]={2,0,6,4};
	portNum=num[cpuNum];
	rioConfigRead(1,0xff,0,0x158+0x20*portNum,&data);
	printf("get578Status--port%d::0x%x",portNum,data);
}

#endif
//sysDmaSend(1,0x30000000,0xc0000000)


STATUS rioScanLockDevice1(UINT8 localPort,UINT8 hopCount,UINT16 hostId,UINT8 mode)
{
	UINT16 currentSwNo,previousSwNo,previousSwPort,currentSwPort,hostLockId;
	UINT32 result,switchNo;
	UINT8 portNum,hostPort,i;/*,j*/
	UINT8 deviceLockflag=0;
	UINT16 epRapidIoId;
	UINT32 retryCount=0;
	UINT32 rioScanStop=0;
	UINT32 regVal = 0;
	RIO_LOG("enter rioScanLockDevice\n",1,2,3,4,5,6);


#if 1
	if(rioAcquirLock(localPort,0xff,hopCount,hostId,&hostLockId)==ERROR)
	{
		deviceLockflag=1;
		/*printf("RIO: hopCount = %d--- hostId =%d\n",hopCount,hostId);*/
		if(hostLockId != hostId ) 		
		{
			printf("RIO: PE already discovered by this host(ID = %d)\n",hostLockId);
			return ERROR;
		}
		/*else return OK;*/
	}
#endif	


	regVal = rioGetPEF(localPort,0xff,hopCount);
	RIO_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",hopCount,regVal,3,4,5,6);

	if(regVal&RIO_PEF_SWITCH) //sjy 20130428 alter if(rioIsSwitch(localPort,0xff,hopCount))
	{

		rioConfigRead(localPort,0xff,hopCount,0x6c,&switchNo);
		/*	printf("\n hopCount=%d,switchNo=0x%x \n",hopCount,switchNo);*/
		if(!(switchNo&0x80000000))  
		{
			if(deviceLockflag) 
			{
				ns16550Pollprintf("SRIO:ERROR!!!the sw has been locked ,but the swNO is not set!!!\n");
				deviceLockflag=0;
				return ERROR;
			}
			rioConfigWrite(localPort,0xff,hopCount,0x6c,gSwitchCount|0x80000000);
			currentSwNo=gSwitchCount;
			gSwitchCount++;
		}
		else currentSwNo= switchNo&0xffff;

		rioGetSwPortInf(localPort,hopCount,&result);

		hostPort = result&0xff;
		portNum = (result&0xff00)>>8;


		if(hopCount != 0)
		{

		}
		else 
		{		

			rioRouteAddEntry(localPort,hostId,hopCount,hostPort); /*hdz*/

		}

		rioRouteAddEntry(localPort,hostId,hopCount,hostPort);
		for(i=0;i<portNum;i++)
		{

			if(i == hostPort) continue; 
			if(rioSwPortIsActive(localPort,0xff,hopCount,i))
			{				
				/*printf("gSwInfo[%d].currentActivePort = %d \n",currentSwNo,i);*/
				rioRouteAddEntry(localPort,0xff,hopCount,i);
				/*debug*/
				rioConfigWrite(localPort,0xff,hopCount,0x140+0x20*i,4);
				taskDelay(10);
				rioConfigRead(localPort,0xff,hopCount,0x158+0x20*i,&regVal);
				RIO_DEEP_LOG("hopCount %d port %d error&status 0x%x!\n",hopCount,i,regVal,4,5,6);
				rioConfigRead(localPort,0xff,hopCount,0x1040+0x40*i,&regVal);
				RIO_DEEP_LOG("----debug hopCount %d port %d err detect value 0x%x!\n",hopCount,i,regVal,4,5,6);

				if (rioScanLockDevice1(localPort,hopCount + 1,hostId,mode) < 0)
				{

					return ERROR;
				}
			}
			else 
			{
				RIO_DEEP_LOG("hopCount %d port %d not active!\n",hopCount,i,3,4,5,6);
			}

		}

	}
	if(rioGetPEF(localPort,0xff,hopCount)&0xe0000000) //sjy 20130428 alter **else
	{	
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter ep info 0 in Sw %d!\n",currentSwNo,2,3,4,5,6);
#endif
		rioConfigRead(localPort,0xff,hopCount-1,0x6c,&switchNo);
		if(!(switchNo&0x80000000))  
		{
			ns16550Pollprintf("RIO:ERROR!!!get switchNo ERROR!!\n");
			return ERROR;
		}
		currentSwNo= switchNo&0xffff;

		currentSwPort=gSwInfo[currentSwNo].currentActivePort;

		/*if(currentId == hostId) currentId++;*/
		/*epRapidIoId=rioGetEpRapidIoId();*/
		//#ifndef INCLUDE_RAPIDIO_SLOTNUM
		if(mode==1)
		{
			epRapidIoId=deviceId;
			deviceId++;
			ns16550Pollprintf("rioConfigRead deviceId++ \n");
		}
		//#else
		else
		{

			rioConfigRead(localPort,0xff,hopCount,0x8,&result);

			epRapidIoId=result&0xffff;
			if(epRapidIoId==0xff) 
			{
				ns16550Pollprintf("slot num is error!!\n");
				return ERROR;
			}
			else if(epRapidIoId==0) 
			{

				while(!rioScanStop)
				{
					retryCount=0xfffffffe;

					rioConfigRead(localPort,0xff,hopCount,0x8,&result);
					epRapidIoId=result&0xffff;
					if((epRapidIoId==0xff))
					{
						ns16550Pollprintf("slot num is error!!\n");
						return ERROR;
					}
					else if(epRapidIoId!=0) break;
					else if(retryCount == 0)
					{	
						ns16550Pollprintf("slot num is error!!\n");
						return ERROR;
					}
					retryCount--;
				}

			}
		}


		rioConfigWrite(localPort,0xff,hopCount,0x60,epRapidIoId<<16);

		rioRouteAddEntry(localPort,epRapidIoId,hopCount-1,currentSwPort); /*hdz*/


		/*currentId++;*/

	}	
	RIO_LOG("exit rioScanLockDevice\n",1,2,3,4,5,6);
	return OK;	
}




void testRead(int mode)
{
	UINT32 hostCpuID = 0;
	UINT32 connPort = 2;
	UINT32 destid = 0xff;
	UINT32 portIndex = 10;
	UINT16 hostId = 0;

	UINT16 lockId;
	UINT32 result;
	UINT32 regVal = 0;
	UINT32 switchNo;
	UINT8 portNum,hostPort,i;/*,j*/



	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	if((result&0xffff)!=0xffff)
	{
		*lockId=result&0xffff;
		return ERROR;
	}
	rioConfigLocalWrite(RIO_HOST_DID_LOCK_CSR,hostId);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	lockId=result&0xffff;
	if((result&0xffff) != (hostId&0xffff)) return ERROR;

	rioConfigLocalWrite(RIO_DID_CSR,(hostId<<16));

	if(rioAcquirLock(1,0xff,0,hostId,&lockId)==ERROR)
	{

	}	
	
	regVal = rioGetPEF(1,0xff,0);
	RIO_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",0,regVal,3,4,5,6);

	if(regVal&RIO_PEF_SWITCH) //sjy 20130428 alter if(rioIsSwitch(localPort,0xff,hopCount))
	{

		rioGetSwPortInf(1,0,&result);

		hostPort = result&0xff;
		portNum = (result&0xff00)>>8;

		rioRouteAddEntry(1,hostId,0,hostPort); 

//		rioRouteAddEntry(1,hostId,0,hostPort);
		for(i=0;i<portNum;i++)
		{
			if(i == hostPort) continue; 
			if(rioSwPortIsActive(1,0xff,0,i))
			{				
				rioRouteAddEntry(1,0xff,0,i);

				if(rioAcquirLock(1,0xff,1,hostId,&lockId)==ERROR)
					;
				regVal = rioGetPEF(1,0xff,1);
				RIO_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",1,regVal,3,4,5,6);

			}
			else 
			{
				RIO_DEEP_LOG("hopCount %d port %d not active!\n",1,i,3,4,5,6);
			}

		}

	}

}
#define QUAD_MODE_444 0
#define QUAD_MODE_2244 1
#define QUAD_MODE_22422 2
#define QUAD_MODE_21144 3
void rioQuadSet(UINT8 quad,UINT8 mode)
{
	if(sysGetCpuID() == 1)
	{
		rioSwPortCfg(quad,mode);
		taskDelay(10);

		rioSwQuadPortReset(quad);
		taskDelay(10);
	}
}
void setQuadMode()
{
	rioQuadSet(0,QUAD_MODE_21144);
	rioQuadSet(1,QUAD_MODE_21144);
	rioQuadSet(2,QUAD_MODE_2244);
}
