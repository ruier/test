/* rioExplore.c - Initialization,Enumeration .*/

/*
 * Copyright (c) 2012- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */

/*
modification history
-----------------------
10/09/2012,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Initialize the RapidIO Switch of CPS1848.
 * 
 */


/* includes */
#include <vxWorks.h>
#include <sys\fcntlcom.h>
#include "rioConfig.h"
#include "rioExplore.h"
#include "rioPrivate.h"
#include "wrSbc8641.h"
#include "sysRapidIO.h"

#define LOCAL static
#define RIO_LOCAL_ADDR RAPIDIO_ADRS

/* variables */
//int curDevID = 0;        

int curSwID = 1;              /* Switches is numbered from '1'*/


int currExploringIndex = 1;
int curMaxIndex = 1;
int maxSwitchIndex = 1;


//int switchPortNum = 0;


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

/* functions */

/*****************************************************************************
 *
 * m85xxRioRegRead - Read a value from the M85xx's rapidIO interface
 * RETURNS:the register's value
 */
UINT32 m85xxRioRegRead(UINT32 offset)
{
	UINT32 data;
	data=*(volatile UINT32*)(CCSBAR+RAPIDIO_REG_BASE+offset);
	return data;
}

/*****************************************************************************
 *m85xxRioRegWrite - write a value to the M85xx's rapidIO interface's register
 */
void m85xxRioRegWrite(UINT32 offset,UINT32 value)
{	
	*(volatile UINT32*)(CCSBAR+RAPIDIO_REG_BASE+offset) = value;
//	return;
}

/*****************************************************************************
 *rioGetNumLocalPorts - get the local RapidIO ports,in the m85xx CPU
 *为什么是两个？
 */
INT32 rioGetNumLocalPorts(void)
{
	return 2;
}


///*****************************************************************************
// * rioConfigLocalRead - m85xx RapidIO read the local config register
// */
//STATUS rioConfigLocalRead(UINT32 offset,UINT32 *readData)
//{
//	*readData=m85xxRioRegRead(offset);
//	return OK;
//}
//
///*****************************************************************************
// * rioConfigLocalWrite - m85xx RapidIO write a value to the local config register
// */
//STATUS rioConfigLocalWrite(UINT32 offset,UINT32 writeData)
//{
//	m85xxRioRegWrite(offset,writeData);
//	return OK;
//}
//
///*****************************************************************************
// * rioConfigRead - m85xx RapidIO read the value in the remote rapidIO config 
// * register.
// */
//STATUS rioConfigRead(UINT8 localPort,RIO_ID destId,UINT8 hopCount,UINT32 offset,UINT32 *readData)
//{
//	if(localPort==0) localPort=1;
//	if(localPort>3) localPort=3;
//	
//#ifdef RIO_ID_16
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
//#else
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//#endif
////	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
////	DEBUG_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
//	*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);
//	if(rioReadState == 1)
//	{
//		rioReadState = 0;
//		return ERROR;
//	}
//	//DEBUG_LOG("exit rioConfigRead\n",1,2,3,4,5,6);
//	return OK;
////	UINT32 time = CONFIG_READ_TIMEOUT;
////	
////	if(localPort==0)
////	{
////		localPort=1;
////	}
////	
////	if(localPort>3)
////	{
////		localPort=3;
////	}
////	
////	while(time)
////	{
////		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
////		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
////		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
////		//RIO_DEEP_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
////		*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);
////		if(rioReadState == 0)
////		{
////			break;
////		}
////		else
////		{
////			if(time != 1)
////			{
////				rioReadState = 0;
////			}	
////		}
////
////		time --;
////	}
////	if((time == 0)&&(rioReadState == 1))
////	{
////		rioReadState = 0;
////		ns16550Pollprintf("rioConfigRead timeout \n");
////		
////		return ERROR;
////		//RIO_DEEP_LOG("exit  rioConfigRead   tfttttttimeout\n",1,2,3,4,5,6);
////	}
////	rioReadState = 0;
////	return OK;
//}
//
///*****************************************************************************
// * rioConfigWrite - m85xx RapidIO write a value to the remote rapidIO configuration 
// * register.
// */
//STATUS rioConfigWrite(UINT8 localPort,RIO_ID destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
//{
//	UINT32 data,timeOut=3;
//
//	if(localPort==0) localPort=1;
//	if(localPort>3) localPort=3;
//#ifdef RIO_ID_16
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
//#else
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//#endif
////	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
//
//	while(timeOut--)
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
//		if(data == writeData) break;
//	}
//	if(timeOut) return OK;
//	else return ERROR;	
//}
//
//STATUS rioConfigWriteOne(UINT8 localPort,RIO_ID destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
//{
//	UINT32 data,timeOut=3;
//
//#ifdef RIO_ID_16
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
//#else
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
//#endif
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
//	*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
//	return OK;
//}

/*****************************************************************************
 *
 * systemInit - Initialize the system
 * Initialize the data Structure and do other things.
 */
LOCAL void systemInit()
{
	UINT32 regVal = 0;
	
	curSwID = 1;              /* Switches is numbered from '1'*/
	currExploringIndex = 1;
	curMaxIndex = 1;
	maxSwitchIndex = detectedSwitchNum;
	
	
	hostCpuID = 0; /*get the CPU's device ID */

	EXPLORE_DBG_LOG("The cpu ID is %d\n",hostCpuID,2,3,4,5,6);


	/*TODO: 读取Host CPU的PORT_OK的值，确定其是否与switch已完成物理连接和初始化 */	


	//curDevID = cpuID + 2; 

	/* Assumes that the CPU has the Max Priory in all CPUs,
	 * if it is not,this CPU will be terminated in the Explorring process.
	 */

	/* do something else that is needed */

}

/*****************************************************************************
 *
 *switchInit - Initialization the switch 
 *RETURNS:  -1---ERROR or The has been initialized by hight priory CPU.
 *           0 ---The switch has been initialzed by current cpu.
 *           1 ---The switch is discoverring at This time.
 */

LOCAL int switchInit(UINT8 hopCount)
{
	UINT32 regVal;
	EXPLORE_DBG_LOG("enter switchInit\n",1,2,3,4,5,6);
	CONFIG_READ(DEF_ID,hopCount,HBDLCSR,&regVal);

	CONFIG_WRITE(DEF_ID,hopCount,HBDLCSR,&hostCpuID);
	CONFIG_READ(DEF_ID,hopCount,HBDLCSR,&regVal);
	if (regVal > hostCpuID)
	{
		/* TODO: There is another cpu exporting the system,which has 
		 * hight priory,so wait for it's end.
		 */

		return -1;

	}

	while(regVal < hostCpuID)
	{
		//TODO(wait(););/*调用系统中的等待函数，等待一个小的时间段，然后继续尝试着去获取Lock*/
		/* spins in a delay loop and repeatedly attempts to acquire the lock */
		CONFIG_WRITE(DEF_ID,hopCount,HBDLCSR,&hostCpuID);
		CONFIG_READ(DEF_ID,hopCount,HBDLCSR,&regVal);
		/* 这里可能会在调试时出现情况，需小心*/
	}

	if (regVal == hostCpuID)
	{
		CONFIG_READ(DEF_ID,hopCount,PGCCSR,&regVal);
		if (regVal & 0x20000000) /* The switch has discovered */
		{
			CONFIG_READ(DEF_ID,hopCount,COMTAGCSR,&regVal);
			if((regVal&0xffff0000)>>16 == detectedTimes)
			{
				return 0;
			}
			else
			{
				EXPLORE_DBG_LOG("in switchInit reg value %d,regVal %d\n",(regVal&0xffff0000)>>16,detectedTimes,3,4,5,6);
				regVal = curSwID | (detectedTimes<<16);
				CONFIG_WRITE(DEF_ID,hopCount,COMTAGCSR,&regVal);
				return 2;
			}
		}
	}
	/* The switch hasn't discovered or the switch has discovered by other cpu who has lower priory*/

	/* Set discovered bit */
	CONFIG_READ(DEF_ID,hopCount,PGCCSR,&regVal);
	regVal |= 0x20000000;
	CONFIG_WRITE(DEF_ID,hopCount,PGCCSR,&regVal);

	/*Set Switch's ID ，需要，因为连接线路很可能有回路，用它可以唯一标识每一个Switch*/
//	if(mode != ID_SLOT_MODE)
//	{
		//CONFIG_WRITE(DEF_ID,hopCount,COMTAGCSR,&curSwID);
//	}
	/* curSwID ++ ---has done in switchExplore() */
	regVal = curSwID | (detectedTimes<<16);
	CONFIG_WRITE(DEF_ID,hopCount,COMTAGCSR,&regVal);
	EXPLORE_DBG_LOG("exit switchInit\n",1,2,3,4,5,6);
	return 1;
}

/*****************************************************************************
 *  
 *  deviceInit - Initialize the device
 */

LOCAL void deviceInit(UINT8 hopCount)  /* 设备是不是也有Host Base DeviceID Lock CSR？ */ 
{
	UINT32 regVal = 0;

	/*由于设备是先获取Switch锁的，所以这里不会出现CPU配置冲突问题 ,但也可以用Lock锁一下，这里没有锁*/
	/*  set Host Base DeviceID Lock CSR */

	/* acquire Host Base DeviceID Lock */
	CONFIG_WRITE(DEF_ID,hopCount,HBDLCSR,&hostCpuID);

	/* Set discovered bit */
	CONFIG_READ(DEF_ID,hopCount,PGCCSR,&regVal);
	regVal |= 0x20000000;
	CONFIG_WRITE(DEF_ID,hopCount,PGCCSR,&regVal);

//	/* Set Device ID */
//	regVal = curDevID << 16;
//	CONFIG_READ(DEF_ID,hopCount,AIDCAR,&regVal);
//	regVal = regVal << 16;
//	CONFIG_WRITE(DEF_ID,hopCount,BDIDCSR,&regVal);

	//DEBUG_LOG("HopCount %d,deviceID %d\n",hopCount,curDevID,3,4,5,6);
	/* do some Thing else, if needed*/
}
/*****************************************************************************
 *
 *resumeSwitch - Resume the switch to the original state.
 * The reverse operation with deviceInit.
 * 
 */

LOCAL void resumeSwitch(struct switch_desc * swPtr)
{
	UINT32 regVal;
	int returnStatus = 0;
	UINT8 * routeArray = swPtr->routePortsArray;
	UINT32 destid = DEF_ID;
	UINT32 connPort = 0;
	int portNum = 0;
	UINT32 srcPort = 0;
	int i=0;
	UINT8 hopIndex = 0;
	int portIndex = 0;

	/* 设置switch通路中的路径路由表 */
	for(hopIndex=0; hopIndex<swPtr->hopCount; hopIndex++)
	{
		CONFIG_READ(DEF_ID,hopIndex,SPICAR,&regVal);
		connPort = regVal & 0xFF;
		ROUTE_SET_P_TO_P(DEF_ID,hopIndex,&destid,routeArray++,&connPort);
	}

	CONFIG_READ(DEF_ID,swPtr->hopCount,SPICAR,&regVal);
	portNum = regVal & 0xFF00;
	connPort = regVal & 0xFF;

	/* Set all Maintenance respones packet route to connPort*/
	ROUTE_SET_P_TO_P(DEF_ID,swPtr->hopCount,&hostCpuID,&connPort,&srcPort); 

	/* Reset all the devices which connected to the switch */
	for(portIndex=0; portIndex< portNum; portIndex++)
	{
		if (portIndex == connPort) /* 如果端口号为维护包输入端口，则不检测该端口*/
		{
			continue;
		}

		/* 设置从Host Port发出的数据包到Port i的路由表项 */
		destid = DEF_ID;
		ROUTE_SET_P_TO_P(DEF_ID,swPtr->hopCount,&destid,&portIndex,&connPort); /* 如果对于同一个地址的多个路由表项不能相互覆盖，这里将出现小的问题，需要调试测试 */

		/*test the PORT_OK bit, if this bit is set,then this port is connected to a device */
		CONFIG_READ(DEF_ID,swPtr->hopCount,PORT_ERR_STAT_CSR(portIndex),&regVal);		

		EXPLORE_DBG_LOG("Port %d PORT_ESCSR value is %d !\n",portIndex,regVal,3,4,5,6);		

		/* if this port connectes nothing,then we detect the next port */
		if ((regVal & 0x00000002) != 0x02)
		{
			EXPLORE_DBG_LOG("This port did not connect to a device !\n",1,2,3,4,5,6);
			continue;
		}

		CONFIG_READ(DEF_ID,swPtr->hopCount + 1,PEFCAR,&regVal);
		if (!(regVal & 0x10000000))     /* defines this is not a switch */
		{
			/* Reset Device ID */
			regVal = 0x00000;/* TODO: 查看一下Base Device ID CSR的默认值，这里可能是0x0也可能是0xFFFFFFFFF*/
			CONFIG_WRITE(DEF_ID,swPtr->hopCount + 1,BDIDCSR,&regVal);
			/* Reset discovered bit */
			CONFIG_READ(DEF_ID,swPtr->hopCount + 1,PGCCSR,&regVal);
			regVal &= 0xdfffffff;
			CONFIG_WRITE(DEF_ID,swPtr->hopCount + 1,PGCCSR,&regVal);
		}
	}

	/* Reset the switch */
	regVal = 0;
	CONFIG_WRITE(DEF_ID,swPtr->hopCount,COMTAGCSR,&regVal);

	/* Clear the discovered bit */
	CONFIG_READ(DEF_ID,swPtr->hopCount,PGCCSR,&regVal);
	regVal &= 0xdfffffff;
	CONFIG_WRITE(DEF_ID,swPtr->hopCount,PGCCSR,&regVal);

	/* Release the Host Base DeviceID Lock CSR*/
	CONFIG_WRITE(DEF_ID,swPtr->hopCount,HBDLCSR,&hostCpuID);

}


/*****************************************************************************
 * 
 * releaseResources - Release all the resources which has alloced during the exploring
 * process.
 */
LOCAL void releaseResources()
{
	int index;
	struct switch_connect_info * tempPtr;

	for (index=curMaxIndex-1; index>=0; index--)
	{

		/* free memory 'routePortsArray' which is in switchArray[index] structure */
		free(switchArray[index]->routePortsArray);
		/* free memory 'device_IDs' which is in switchArray[index] structure */
		free(switchArray[index]->device_IDs);

		/*free memory switchArray[index] structure */
		free(switchArray[index]);
	}

}
/********************************
 * 
 * get rapidio feature
 * 
 *********************************/
/*****************************************************************************
 * 
 * rioGetFeature - Get the rapidIo device feature.
 * 
 */

UINT32 rioGetFeature(UINT8 localPort,UINT8 local,UINT16 destId,UINT8 hopCount,int ftr)
{
	UINT32 asmInfo,extFtrPtr,ftrHeader;
	if(local) rioConfigLocalRead(RIO_ASM_INFO_CAR,&asmInfo);
	else CONFIG_READ(destId,hopCount,RIO_ASM_INFO_CAR,&asmInfo);

	extFtrPtr = asmInfo&RIO_EXT_FTR_PTR_MASK;
	while(extFtrPtr)
	{
		if(local)
		{
			rioConfigLocalRead(extFtrPtr,&ftrHeader);
		}
		else 
		{
			CONFIG_READ(destId,hopCount,extFtrPtr,&ftrHeader);
		}
		if(RIO_GET_BLOCK_ID(ftrHeader) == ftr) 
		{
			return extFtrPtr;
		}
		if(!(extFtrPtr = RIO_GET_BLOCK_PTR(ftrHeader)))
		{
			break;
		}
	}
	return 0;
}


UINT32 rioSwPortIsActive(UINT8 localPort, RIO_ID destId, UINT8 hopCount, UINT8 swPort)
{
	UINT32 result,extFtrPtr;
	INT8 *entry = rioSwPortPhysTable;
//	UINT32 regVal;
//	CONFIG_READ(DEF_ID,0,0x158+swPort*0x20,&regVal);
//	DEBUG_LOG("port %d,addr 0x%x,value 0x%x!\n",swPort,0x158+swPort*0x20,regVal,3,4,5,6);
//	return 0;
	do {
		if ((extFtrPtr = rioGetFeature(localPort,0,destId,hopCount,*entry)))  break;
	} while (*++entry >= 0);

	if (extFtrPtr)
	{
		CONFIG_READ(destId,hopCount,extFtrPtr+RIO_PORT_N_ERR_STS_CSR(swPort),&result);
	//	DEBUG_LOG("port %d,addr 0x%x,value 0x%x!\n",swPort,extFtrPtr+RIO_PORT_N_ERR_STS_CSR(swPort),result,3,4,5,6);
	}
	

	return (result & PORT_N_ERR_STS_PORT_OK);
}

LOCAL void rioPortPowerDown(UINT8 hopCount,UINT8 portNum)
{
	UINT32 regVal;
	RIO_ID destId = DEF_ID;
	CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
	
	if((regVal>>2)&0x1 != 1) 
	{
		CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		regVal |= (1<<2);

		CONFIG_WRITE(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		

		while(((regVal>>2)&0x1) == 0)
		{
			regVal |= (1<<2);
			CONFIG_WRITE(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
			CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		}
	}
//	printf("port %d  powerDown\n",portNum);
}

LOCAL void rioPortPowerUp(UINT8 hopCount,UINT8 portNum)
{
	UINT32 regVal;
	RIO_ID destId = DEF_ID;
	CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
	//rioConfigReadNew(RIO_CONFIG_WIN_NO,0xff,0,(0x130c8+portNum*0x100),&data) ;
	if((regVal>>2)&0x1 != 0)
	{
		CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		regVal &=(~(1<<2));
		CONFIG_WRITE(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		while((regVal>>2)&0x1 == 1)
		{
			regVal &=(~(1<<2));
			CONFIG_WRITE(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
			CONFIG_READ(destId,hopCount,(0x130c8+portNum*0x100),&regVal);
		}
	}
	//printf("port %d  powerUp\n",portNum);
}

LOCAL void rioPortRecover(UINT8 hopCount,UINT8 port)
{
	RIO_ID destId = DEF_ID;
	UINT32 regVal;
	UINT32 localAckID;
	UINT32 outBoundAckID;

	EXPLORE_DBG_LOG("enter rioPortRecover\n",1,2,3,4,5,6);
	CONFIG_READ(destId,hopCount,PORT_ERR_STAT_CSR(port),&regVal);
	if((regVal&0x02)== 2)
	{
		EXPLORE_DBG_LOG("in rioPortRecover enter if\n",1,2,3,4,5,6);
		if(regVal !=2)
		{
			CONFIG_READ(destId,hopCount,PORT_ERR_STAT_CSR(port),&regVal);
			if((regVal &0x10100) !=0) /*if(firstRecover == TRUE)//0606 JSY test  */
			{
				//rioFlag[i] =1 ;
				EXPLORE_DBG_LOG("open and recover port %d\n",port,2,3,4,5,6);
				//printf("open and recover port %d\n",portNo);
				//tsi578PortRecover(portNo);
				rioPortPowerDown(hopCount,port);
				rioPortPowerUp(hopCount,port);
				regVal = 4;
				CONFIG_WRITE(destId,hopCount,PORT_LINK_MAINT_REQ_CSR(port),&regVal);
				taskDelay(5);//taskDelay(10);	
				CONFIG_READ(destId,hopCount,PORT_LINK_MAINT_RESP_CSR(port),&regVal);
				while((regVal&0x80000010)!=0x80000010)
				{
					EXPLORE_DBG_LOG("wait 6s\n",1,2,3,4,5,6);
					taskDelay(300);
					regVal = 4;
					CONFIG_WRITE(destId,hopCount,PORT_LINK_MAINT_REQ_CSR(port),&regVal);
					taskDelay(10);	
					CONFIG_READ(destId,hopCount,PORT_LINK_MAINT_RESP_CSR(port),&regVal);
					EXPLORE_DBG_LOG("input TSI578 port%d status: 0x%x\n",port,regVal,3,4,5,6);
				}
				CONFIG_READ(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				regVal |= 0x2;
				CONFIG_WRITE(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				CONFIG_READ(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				regVal |= 0x2;
				CONFIG_WRITE(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				
				taskDelay(2);
				CONFIG_READ(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				regVal &= 0xfffffffd;
				CONFIG_WRITE(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				taskDelay(2);
				CONFIG_READ(destId,hopCount,PORT_LOCAL_ACKID_CSR(port),&regVal);
				//rioConfigReadNew(RIO_CONFIG_WIN_NO,0xff,0,TSI578_PORT_ACKID_STATUS_CSR(portNo),&regVal); /*read port ackid status*/
				localAckID = regVal;
				outBoundAckID = localAckID&0x1f;
				//	SRIO_RECOVER_DEBUG_MSG(6,"local ackID is 0x%08x, outBoundAckID is 0x%08x\n",localAckID,outBoundAckID,3,4,5,6);

				//SRIO_RECOVER_DEBUG_MSG(6,"clear switch port %d ackID\n",portNo,2,3,4,5,6);
				regVal = 0;
				CONFIG_WRITE(destId,hopCount,PORT_LOCAL_ACKID_CSR(port),&regVal);
				regVal = 4;
				CONFIG_WRITE(destId,hopCount,PORT_LINK_MAINT_REQ_CSR(port),&regVal); 
				taskDelay(10);	
				CONFIG_READ(destId,hopCount,PORT_LINK_MAINT_RESP_CSR(port),&regVal);
				//rioConfigReadNew(RIO_CONFIG_WIN_NO,0xff,0,TSI578_PORT_MAINTENANCE_REQUEST_RESPONSE_CSR(portNo),&regVal);
				//	SRIO_RECOVER_DEBUG_MSG(6,"input TSI578 port%d status: 0x%x\n",portNo,regVal,3,4,5,6);
				if((regVal&0x80000000)!=0x80000000)
				{
					logMsg("Havn't Receive Responde regVal is 0x%08x\n",regVal,2,3,4,5,6);
					return ERROR;
				}	
				regVal = 0x50600001;
				CONFIG_WRITE(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				taskDelay(120);
			}
			
		}
	}
	
	EXPLORE_DBG_LOG("exit rioPortRecover\n",1,2,3,4,5,6);
}

int rioGetPEF(RIO_ID destId, UINT8 hopCount,UINT8 upStreamPort)
{
	UINT32 result;
	STATUS readStatus;
	int timeOut = 10;

	EXPLORE_DBG_LOG("Enter rioGetPEF hopCount %d upStreamPort %d!\n",hopCount,upStreamPort,3,4,5,6);
	EXPLORE_DBG_LOG("destId 0x%x hopCount %d offset 0x%x!\n",destId,hopCount,RIO_PEF_CAR,4,5,6);
	readStatus = CONFIG_READ(destId,hopCount,RIO_PEF_CAR,&result);
	
	
	
	
	while((/*(result == 0x80000000)||*/(readStatus == ERROR))&&(timeOut))
	{
		
#if (SWITCH_TYPE == TSI_578)                             //sjy 2013.07.01 modified
		rioPortRecover(hopCount-1,upStreamPort);
#endif
		
		readStatus = CONFIG_READ(destId,hopCount,RIO_PEF_CAR,&result);
		timeOut --;
	}
	
	if((timeOut == 0)&&(result == 0x80000000))
	{
		scanFail = TRUE;
		EXPLORE_DBG_LOG("scanFail = TRUE timeOut = %d!\n",timeOut,2,3,4,5,6);
	}
	
	EXPLORE_DBG_LOG("read result 0x%x!\n",result,2,3,4,5,6);
	EXPLORE_DBG_LOG("Exit rioGetPEF!\n",1,2,3,4,5,6);
	
	return result;
}

/*****************************************************************************
 * 
 * switchExplore - Explore the devices which are connected to the switch
 * swPtr: point to the switch which need explore
 * 
 */
LOCAL int switchExplore(struct switch_desc * swPtr)
{
	UINT32 regVal = 0;
	int connPort = 0;
	int portIndex = 0;
	RIO_ID destid = 0;
	UINT8 *srcPort = 0;
	UINT8 *destPort = 0;
	UINT32 regValue  = 0;
//	int timeOut;

//	int *devices;
	int i=0;

	struct switch_desc * tempSwPtr;

	int returnStatus = 0;
	UINT8 * routeArray = swPtr->routePortsArray;
	
	EXPLORE_DBG_LOG("enter switchExplore,switchID %d hopCount %d\n",swPtr->switch_ID,swPtr->hopCount,3,4,5,6);

	/* 设置switch通路中的路径路由表 */
	destid = DEF_ID;
	for(i=0; i<swPtr->hopCount; i++)
	{
		CONFIG_READ(DEF_ID,i,SPICAR,&regVal);
		connPort = regVal & 0xFF;
//		ROUTE_SET_P_TO_P(DEF_ID,i,&destid,routeArray++,&connPort);
		ROUTE_SET_P_TO_P(DEF_ID,i,&destid,routeArray,&connPort);
		EXPLORE_DBG_LOG("1111set 0x%x to port 0x%x!\n",destid,*routeArray,3,4,5,6);
		
	}
	
	/* debug */
	//DEBUG_LOG("i is %d,hopCount is %d\n",i,swPtr->hopCount,3,4,5,6);
	/* debug */

	CONFIG_READ(DEF_ID,swPtr->hopCount,SPICAR,&regVal);

	connPort = regVal & 0xFF;

	/* Set all Maintenance respones packet route to connPort*/
	ROUTE_SET_P_FOR_ALL(DEF_ID,swPtr->hopCount,&hostCpuID,&connPort); 

	
	for(portIndex=0; portIndex< swPtr->portNum; portIndex++)
	{
		/* 如果端口号为维护包输入端口，则不检测该端口*/
		EXPLORE_DBG_LOG("000000cur switch ID is %d Current port index is %d!\n",swPtr->switch_ID,portIndex,3,4,5,6);
		if (portIndex == connPort) 
		{
			/* enable port input & output */
			CONFIG_READ(DEF_ID,swPtr->hopCount,0x15c+0x20*portIndex,&regVal);
			regVal |= 0x600000;
			CONFIG_WRITE(DEF_ID,swPtr->hopCount,0x15c+0x20*portIndex,&regVal);
			EXPLORE_DBG_LOG("This is the port which connected to the upstream device!\n",1,2,3,4,5,6);
			continue;
		}
		
		/* 设置从Host Port发出的数据包到Port i的路由表项 */
		destid = DEF_ID;
		ROUTE_SET_P_TO_P(DEF_ID,swPtr->hopCount,&destid,&portIndex,&connPort);   //TODO有问题，问题究竟在哪？
		//port to port 设置失败
//		ROUTE_SET_P_FOR_ALL(DEF_ID,swPtr->hopCount,&destid,&portIndex);
		/* 如果对于同一个地址的多个路由表项不能相互覆盖，这里将出现小的问题，需要调试测试 */
//	    sysI2cRead(0,0xff,0xe1b000+0xff*4,&regVal,4,1);
//	    EXPLORE_DBG_LOG("==++++++0x%x = 0x%x!\n",0xe1b000+0xff*4,regVal,3,4,5,6);
//	    sysI2cRead(0,0xff,0xe1b400+0xff*4,&regVal,4,1);
//	    EXPLORE_DBG_LOG("==++++++0x%x = 0x%x!\n",0xe1b400+0xff*4,regVal,3,4,5,6);
				

//		/*test the PORT_OK bit, if this bit is set,then this port is connected to a device */
//		CONFIG_READ(DEF_ID,swPtr->hopCount,PORT_ESCSR + (portIndex * 0x20),&regVal);		

		//DEBUG_LOG("Port %d PORT_ESCSR value is %d !\n",portIndex,regVal,3,4,5,6);		
		
		//CONFIG_READ(DEF_ID,0,0x158+portIndex*0x20,&regVal);
//		CONFIG_READ(DEF_ID,swPtr->hopCount,0x158+portIndex*0x20,&regVal);
//		DEBUG_LOG("port %d,addr 0x%x,value 0x%x  hopCount %d!\n",portIndex,0x158+portIndex*0x20,regVal,swPtr->hopCount,5,6);
//		continue;
		
		/* if this port connectes nothing,then we detect the next port */
		if(!rioSwPortIsActive(1,DEF_ID,swPtr->hopCount,portIndex))
		{
			EXPLORE_DBG_LOG("Port %d did not connect to a device !\n",portIndex,2,3,4,5,6);
			if(swPtr->device_IDs[portIndex].type != NOTHING)
			{
				topologyHasChanged = TRUE;
				if(swPtr->device_IDs[portIndex].type == DEVICE)
				{
					swPtr->device_IDs[portIndex].type = NOTHING;
					swPtr->device_IDs[portIndex].ID = 0;
					swPtr->device_IDs[portIndex].oldID = 0xFF;
				}
				else
				{
					swPtr->device_IDs[portIndex].type = NOTHING;
					swPtr->device_IDs[portIndex].ID = 0;
					tempSwPtr = switchArray[curMaxIndex];
					for(i=curMaxIndex;i<maxSwitchIndex;i++)
					{
						switchArray[i] = switchArray[i+1];
					}
					maxSwitchIndex--;
					if(tempSwPtr != NULL)
					{
						if(tempSwPtr->device_IDs != NULL)
						{
							free(tempSwPtr->device_IDs);
							tempSwPtr->device_IDs  = NULL;
						}
						if(tempSwPtr->routePortsArray != NULL)
						{
							free(tempSwPtr->routePortsArray);
							tempSwPtr->routePortsArray = NULL;
						}
						free(tempSwPtr);
						tempSwPtr = NULL;
					}
				}
			}
			continue;
		}
		
		EXPLORE_DBG_LOG("Port %d did  connect to a device !\n",portIndex,2,3,4,5,6);
//		if((swPtr->device_IDs[portIndex].type == SWITCH)&&(swPtr->device_IDs[portIndex].ID != 1))
//		{
//			ROUTE_SET_P_FOR_ALL(DEF_ID,swPtr->hopCount+1,&hostCpuID,&swPtr->device_IDs[portIndex].connPort);
//		}
		if((swPtr->device_IDs[portIndex].type == DEVICE))
		{
			continue;
		}
		
		/* enable port input & output */
		CONFIG_READ(DEF_ID,swPtr->hopCount,0x15c+0x20*portIndex,&regVal);
		regVal |= 0x600000;
		CONFIG_WRITE(DEF_ID,swPtr->hopCount,0x15c+0x20*portIndex,&regVal);

		regValue = rioGetPEF(DEF_ID,swPtr->hopCount+1,portIndex);

		EXPLORE_DBG_LOG("\n\n------------====Port %d --------PEF value 0x%x!\n",portIndex,regValue,3,4,5,6);
		if(regValue&RIO_PEF_SWITCH)    /* defines this is a switch */
		{
			EXPLORE_DBG_LOG("Port %d did  connect to a switch !\n",portIndex,2,3,4,5,6);
			//taskDelay(180);
			returnStatus = switchInit(swPtr->hopCount + 1);
			EXPLORE_DBG_LOG("switchInit return value is %d!\n",returnStatus,2,3,4,5,6);
			if (returnStatus == -1)
			{

				/* Explorring failed, there has a highter priory CPU is explorring now
				 * wait for other CPU finish the Topology Discovery,or that
				 *  CPU fail,and this CPU try reDiscoverring again. 
				 */
				/*TODO:  wait for other CPU finish the Topology Discovery,or that
				 *  CPU fail,and this CPU reDiscover again */


				/* If that CPU Explores the topology successfully,then do the following
				   things*/

				/* Resume all the devices and switches to the original state: 
				 * release the Host Base DeviceID Lock CSR,etg.*/
				int index = curMaxIndex;
				while(--index >= 0)
				{
					resumeSwitch(switchArray[index]);
				}
				/*Release all the resources which had alloced. */
				releaseResources();

				/* the explorring process has been finished by other CPU*/
				return -1;
			}
			else
			{
				if (returnStatus == 1)
				{
					topologyHasChanged = TRUE;
					if(swPtr->device_IDs[portIndex].type == NOTHING)
					{
						tempSwPtr = (struct switch_desc *) malloc(sizeof(*tempSwPtr));
						tempSwPtr->hopCount = swPtr->hopCount + 1;
						tempSwPtr->routePortsArray = (UINT8 *) malloc(sizeof(UINT8)*(tempSwPtr->hopCount));
						for(i=maxSwitchIndex;i>=curMaxIndex;i--)
						{
							switchArray[i] = switchArray[i-1];
						}
						maxSwitchIndex++;
						EXPLORE_DBG_LOG("in line 812 maxSwitchIndex added !\n",1,2,3,4,5,6);
						switchArray[curMaxIndex++] = tempSwPtr;
					}
					else
					{
						tempSwPtr = switchArray[curMaxIndex++];
						
					}

					//					tempSwPtr->switch_ID = curSwID++;
					//					if(mode == ID_SLOT_MODE)
					//					{
					//						CONFIG_READ(DEF_ID,tempSwPtr->hopCount,COMTAGCSR,&regVal);
					//						tempSwPtr->switch_ID = regVal;
					//					}
					//					else
					//					{
					tempSwPtr->switch_ID = curSwID++;
					//					}

					EXPLORE_DBG_LOG("detected a switch curSwID is %d\n",curSwID,2,3,4,5,6);

					swPtr->device_IDs[portIndex].type = (char)SWITCH;
					swPtr->device_IDs[portIndex].ID = tempSwPtr->switch_ID;

					srcPort = swPtr->routePortsArray;
					destPort = tempSwPtr->routePortsArray;
					for (i=0; i<swPtr->hopCount;i++)
					{
						*destPort++ = *srcPort++;
					}
					*destPort = portIndex;
					EXPLORE_DBG_LOG("--------routePortsArray add 0x%x \n",portIndex,2,3,4,5,6);
					

					CONFIG_READ(DEF_ID,tempSwPtr->hopCount,SPICAR,&regVal);
					tempSwPtr->portNum = (regVal & 0xFF00) >> 8;

					tempSwPtr->device_IDs = (struct deviceInfo *)malloc(sizeof(struct deviceInfo) * tempSwPtr->portNum);

					/* set all port connected nothing */
					for(i=0; i<tempSwPtr->portNum; i++) 
					{
						(tempSwPtr->device_IDs[i]).type = NOTHING;
						(tempSwPtr->device_IDs[i]).ID = 0;
						(tempSwPtr->device_IDs[i]).oldID = DEF_ID;
					}

				
					/* Add the switch description structure pointer to the switchArray */
		

					/* set the connection from new switch to the detectting switch. */
					CONFIG_READ(DEF_ID,swPtr->hopCount+1,SPICAR,&regVal);
//					connPort = regVal & 0xFF;
					tempSwPtr->device_IDs[regVal & 0xFF].type = (int)SWITCH;
					tempSwPtr->device_IDs[regVal & 0xFF].ID = swPtr->switch_ID;
					tempSwPtr->device_IDs[regVal & 0xFF].connPort = portIndex;
					swPtr->device_IDs[portIndex].connPort = (regVal & 0xFF);
					
				}
				if(returnStatus == 2)
				{
					curMaxIndex++;
					curSwID ++;
				}
				else
				{

					/* If the destination switch has been detected,then we only record
					 * the connect information.
					 */
					if (returnStatus == 0) 
					{
						/* Record the connect Information */
						CONFIG_READ(DEF_ID,swPtr->hopCount + 1,COMTAGCSR,&regVal);
						swPtr->device_IDs[portIndex].type = SWITCH;
						swPtr->device_IDs[portIndex].ID = regVal;
						CONFIG_READ(DEF_ID,swPtr->hopCount+1,SPICAR,&regVal);
						swPtr->device_IDs[portIndex].connPort = regVal & 0xFF;
						EXPLORE_DBG_LOG("a switch has being detected %d \n",regVal,2,3,4,5,6);
					}
				}
			//	DEBUG_LOG("Switch %d,port %d, switch ID %d\n",swPtr->switch_ID,portIndex,curDevID,4,5,6);
			}
		}
		else                         /* If this PE is not a switch */
		{
			if(regValue == 0x80000000)
			{
				swPtr->device_IDs[portIndex].type = NOTHING;
				continue;
			}
			if(swPtr->device_IDs[portIndex].type == NOTHING)
			{
				topologyHasChanged = TRUE;
				deviceInit(swPtr->hopCount + 1);
				taskDelay(1);
				CONFIG_READ(DEF_ID,swPtr->hopCount + 1,AIDCAR,&regVal);//rioConfigRead(localPort,0xff,hopCount,0x8,&result);
				/* add the device information to the switch struct */
				swPtr->device_IDs[portIndex].type = DEVICE;
				swPtr->device_IDs[portIndex].ID = regVal;
				/* TODO assign ID */
//				regVal = regVal << 16;
//				CONFIG_WRITE(DEF_ID,swPtr->hopCount + 1,BDIDCSR,&regVal);
				/* TODO */
				//EXPLORE_DBG_LOG("a switch has being detected %d \n",regVal,2,3,4,5,6);

				//curDevID ++;             /* Increase the current device ID */
				EXPLORE_DBG_LOG("Switch %d,port %d, device ID %d\n",swPtr->switch_ID,portIndex,regVal,4,5,6);
			}
		}
	}
	EXPLORE_DBG_LOG("exit switchExplore,\n",1,2,3,4,5,6);
	/* if this switch has been explored successfully, we return '0' to tell the 
	 * caller function.
	 */
	return 0;
}

///*****************************************************************************
// *
// *storeExplorringInfoToConfigFile - This file is used to store the Explorring 
// *result to the config file.
// */
//
//LOCAL void storeExplorringInfoToConfigFile()
//{
//	int tempfd = 0;
//	char buffer[150];
//	int charNum = 0;
//	char *tempPtr = (char *)0;
//	int index = 0;
//	int portIndex = 0;
//
//	if ((tempfd = open(CONFIG_FILE_NAME,O_RDWR | O_CREAT,0777)) == (STATUS)ERROR)
//	{
//		logMsg("Can't open configure file: %s\n",CONFIG_FILE_NAME,2,3,4,5,6);
//	}
//
//	//charNum = sprintf(buffer,"HostID:%d SwitchNum:%d switchPortNum:%d\n",\
//			cpuID,curMaxIndex,switchPortNum);
//	//DEBUG_LOG("HostID:%d SwitchNum:%d switchPortNums:%d\n",cpuID,curMaxIndex,switchPortNum,4,5,6);
//	write(tempfd,buffer,charNum);
//	for(index=1; index<curMaxIndex; index++)
//	{
//		tempPtr = buffer;
//		charNum = sprintf(tempPtr,"SwitchID:%d,",index);
//		tempPtr += charNum;
//
//	//	for(portIndex = 0; portIndex<switchPortNum; portIndex++)
//		{
//	//		charNum = sprintf(tempPtr,"%c%d,",switchArray[index]->device_IDs[portIndex].type,\
//					switchArray[index]->device_IDs[portIndex].ID);
//			tempPtr += charNum;
//		}
//		charNum = sprintf(tempPtr,"\n");
//		DEBUG_LOG("%s",tempPtr,2,3,4,5,6);
//		tempPtr += charNum;
//		write(tempfd,buffer,tempPtr-buffer);
//	}
//
//	close(tempfd);
//}


UINT32 rioMasterPortIsActive()
{
	UINT32 result=0,extFtrPtr;
	INT8 *entry = rioMasterPortPhysTable;
	EXPLORE_DBG_LOG("enter rioMasterPortIsActive\n",1,2,3,4,5,6);
	do {
		if ((extFtrPtr = rioGetFeature(0,1,0,0,*entry)))  break;
	} while (*++entry >= 0);

	if (extFtrPtr) rioConfigLocalRead(extFtrPtr+RIO_PORT_N_ERR_STS_CSR(0),&result);
	EXPLORE_DBG_LOG("exit rioMasterPortIsActive\n",1,2,3,4,5,6);
	return (result & PORT_N_ERR_STS_PORT_OK);
}

STATUS rioAcquirLock(UINT8 localPort,RIO_ID destId,UINT8 hopCount,RIO_ID hostDeviceId,RIO_ID *hostLockId)
{
	UINT32 timeOut=2, readData=0;
	EXPLORE_DBG_LOG("enter rioAcquirLock\n",1,2,3,4,5,6);
	EXPLORE_DBG_LOG("enter rioAcquirLock:rioConfigRead-destId=0x%x,hopCount=0x%x\n",destId,hopCount,3,4,5,6);

#ifdef RIO_EP_DEBUG
	RIO_EP_DEEP_LOG("Enter rioAcquirLock 0 in Sw !\n",1,2,3,4,5,6);
#endif

	if(CONFIG_READ(destId,hopCount,RIO_HOST_DID_LOCK_CSR,&readData)==ERROR) goto error;

#ifdef RIO_EP_DEBUG
	RIO_EP_DEEP_LOG("Enter rioAcquirLock 1 in Sw !\n",1,2,3,4,5,6);
#endif
	EXPLORE_DBG_LOG("enter rioAcquirLock1\n",1,2,3,4,5,6);
	*hostLockId=(RIO_ID)(readData&0xffff);
	if((readData&0xffff) != 0xffff)
	{
		if(hostDeviceId == (readData&0xffff))
		{
			return OK;
		}
		else
		{
		goto error;
		}
	}
	while(timeOut)
	{
		EXPLORE_DBG_LOG("enter rioAcquirLock2\n",1,2,3,4,5,6);
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 2 in Sw !\n",1,2,3,4,5,6);
#endif
		if(rioConfigWriteOne(localPort,destId,hopCount,RIO_HOST_DID_LOCK_CSR,hostDeviceId)==ERROR) goto error;
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 3 in Sw !\n",1,2,3,4,5,6);
#endif
		if(CONFIG_READ(destId,hopCount,RIO_HOST_DID_LOCK_CSR,&readData)==ERROR) goto error;
#ifdef RIO_EP_DEBUG
		RIO_EP_DEEP_LOG("Enter rioAcquirLock 4 in Sw !\n",1,2,3,4,5,6);
#endif
		if((readData&0xffff) == hostDeviceId) break;
		timeOut--;
		EXPLORE_DBG_LOG("enter rioAcquirLock3\n",1,2,3,4,5,6);
	}
	/*
	 *hostLockId = (UINT16)(readData&0xffff);
	printf("*hostLockId = 0x%x....timeOut= %d\n",*hostLockId,timeOut);
	 */
	EXPLORE_DBG_LOG("enter rioAcquirLock4\n",1,2,3,4,5,6);
	if(timeOut) 
	{
		return OK;
	}
	EXPLORE_DBG_LOG("exit rioAcquirLock\n",1,2,3,4,5,6);
	error: 
	return ERROR;
}



/*****************************************************************************
 *
 * topologyDiscovery - second stage device connection
 * This function explores the RapidIO network topology,devices and connected 
 * relationship between them.And then initialize the devices. 
 *
 * RETURNS: N/A
 *
 */
STATUS topologyDiscovery()
{
	UINT32 regVal;
	INT32  discoverStatus;
	struct switch_desc * root = NULL;
	int connPort = -1;
	int returnValue = -1;
	int i = 0;
	RIO_ID hostId = 0;
	RIO_ID hostLockId = 0;
	UINT16 hopCount = 0;
	UINT16 localPort = 1;
	RIO_ID destId = DEF_ID;

	systemInit();    

	/* initial self,get the Host Base DeviceID Lock CSR,etg.*/

	/* If somebody else enumerated our master port device, fail. */
	
	if (rioEnumHost(hostId) == ERROR)
	{
		printf("ERROR!!!RIO: master port %d device has been enumerated by a remote host\n",hostId);
		return ERROR;
	}

	/*excute regVal */

	if (rioMasterPortIsActive()) 
	{
		if(rioAcquirLock(localPort,0xff,0,hostId,&hostLockId)==ERROR)
		{
			/*printf("RIO: hopCount = %d--- hostId =%d\n",hopCount,hostId);*/
			if(hostLockId != hostId ) 		
			{
				printf("RIO: PE already discovered by this host(ID = %d)\n",hostLockId);
				return ERROR;
			}
		}
		if(rioGetPEF(destId,hopCount,1)&RIO_PEF_SWITCH)    /* defines this is a switch */
		{
			discoverStatus = switchInit(0);
		
			EXPLORE_DBG_LOG("switchInit return value is %d\n",discoverStatus,2,3,4,5,6);
			if (discoverStatus == -1)
			{
				/* TODO: release the Host Base DeviceID Lock CSR,etg.*/
				/* release all the resources which had alloced */
				/* wait for other CPU finish the Topology Discovery,or that
				 *  CPU fail,and this CPU reDiscover again */


				/* For simply,we directly finish the discovery process */
				return returnValue;
			}
			else
			{
				if (discoverStatus == 1)
				{
					/* debug */
					EXPLORE_DBG_LOG("in if(discoverStatus == 1)!\n",1,2,3,4,5,6);
					/* debug */
					topologyHasChanged = TRUE;
					root = (struct switch_desc *) malloc(sizeof(*root));
					switchArray[curMaxIndex] = root;
					root->hopCount = 0;
					root->routePortsArray = NULL;
//					if(mode == ID_SLOT_MODE)
//					{
//						CONFIG_READ(DEF_ID,root->hopCount,COMTAGCSR,&regVal);
//						root->switch_ID = regVal;
//					}
//					else
//					{
					root->switch_ID = curSwID;
//					}

					root->device_IDs = NULL;

					CONFIG_READ(DEF_ID,root->hopCount,SPICAR,&regVal);
					root->portNum = (regVal & 0xFF00) >> 8;
					EXPLORE_DBG_LOG("Number of switch's Ports is %d!\n",root->portNum,2,3,4,5,6);
					connPort = regVal & 0xFF;
					hostPort = connPort;
					root->device_IDs = (struct deviceInfo *)malloc(sizeof(struct deviceInfo) * root->portNum);

					/* enable port input & output */
					CONFIG_READ(DEF_ID,root->hopCount,0x15c+0x20*connPort,&regVal);
					regVal |= 0x600000;
					CONFIG_WRITE(DEF_ID,root->hopCount,0x15c+0x20*connPort,&regVal);

					/* set all port connected nothing */
					for(i=0; i<root->portNum; i++)
					{
						(root->device_IDs[i]).type = NOTHING;
						(root->device_IDs[i]).ID = 0;
						(root->device_IDs[i]).oldID = DEF_ID;
					}
					/* set port 'connPort' connected to the CPU */
					root->device_IDs[connPort].ID = hostCpuID;
					root->device_IDs[connPort].type = DEVICE;
					maxSwitchIndex ++;
					EXPLORE_DBG_LOG("in line 1148 maxSwitchIndex added !\n",1,2,3,4,5,6);
				}
			
				if((discoverStatus == 1)||(discoverStatus == 2))
				{
					curMaxIndex ++;
					curSwID++;
					while(currExploringIndex < curMaxIndex)   //0701 sjy debug
					{
						returnValue = switchExplore(switchArray[currExploringIndex++]);

						EXPLORE_DBG_LOG("Switch %d has been explored.\n",
								switchArray[currExploringIndex -1]->switch_ID,2,3,4,5,6);
						EXPLORE_DBG_LOG("currExploringIndex %d,curMaxIndex %d maxSwitchIndex %d\n",\
								currExploringIndex,curMaxIndex,maxSwitchIndex,4,5,6);
						/* if the explorring at this step is failed, then we finish the explorring process*/
						if (returnValue == -1)
						{
							return ERROR ;
						}
					}
				}
			}
		}
		else
		{
			/* If the cpu connect to a PE which is not a swtich, do something else */ 
		}
	}
	else
	{
		ns16550Pollprintf("RapidIO Hardware not connnected\n");
		return ERROR;

	} 
	/* Switch Explorring has finished,write the explorring informations and devices
	 * connect informations to the configuration file,then release all memory resources.
	 */ 
	//storeExplorringInfoToConfigFile();
	//releaseResources();
	//rioShowRapidIoInfo();
	detectedSwitchNum = curMaxIndex;
	EXPLORE_DBG_LOG("detectedSwitchNum %d,maxSwitchIndex %d\n",detectedSwitchNum,maxSwitchIndex,3,4,5,6);
	return OK;
}

STATUS rioAcquirLocalLock(RIO_ID deviceId,RIO_ID *lockId)
{
	UINT32 result;
	
	EXPLORE_DBG_LOG("enter rioAcquirLocalLock\n",1,2,3,4,5,6);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	if((result&0xffff)!=0xffff)
	{
		*lockId=result&0xffff;
		if((result&0xffff)==deviceId)
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	rioConfigLocalWrite(RIO_HOST_DID_LOCK_CSR,deviceId);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);
	*lockId=result&0xffff;
	if((result&0xffff) != (deviceId&0xffff))
	{
		return ERROR;
	}
	EXPLORE_DBG_LOG("exit rioAcquirLocalLock\n",1,2,3,4,5,6);
	return OK;
}

/**
 * rioLocalSetDeviceId - Set the base/extended device id for a port
 * Writes the base/extended device id from a device.
 */
void rioLocalSetDeviceId(RIO_ID did)
{
#ifdef RIO_ID_16
	rioConfigLocalWrite(RIO_DID_CSR,(did));
#else
	rioConfigLocalWrite(RIO_DID_CSR,(did<<16));
#endif
}

/*********************************************************************
 * rioEnumHost- Set host lock and initialize host destination ID
 * @port: Master port to issue transaction
 *
 * Sets the host master port lock and destination ID register
 * with the host device ID value. The host device ID value is provided
 * by the platform. Returns %0 on success or %-1 on failure.
 */
STATUS rioEnumHost(RIO_ID hostId)
{
	RIO_ID lockId;
	EXPLORE_DBG_LOG("enter rioEnumHost\n",1,2,3,4,5,6);
	if(rioAcquirLocalLock(hostId,&lockId)==ERROR) 
	{
		printf("RIO:the host device has been locked by another host(ID=%d)\n",lockId);
		return ERROR;
	}
	rioLocalSetDeviceId(hostId);
	//if(nextDestId == hostId) nextDestId++;
	EXPLORE_DBG_LOG("exit rioEnumHost\n",1,2,3,4,5,6);
	return OK;
}



void rioShowRapidIoInfo()
{
	UINT16 i,j,hopIndex;
	//struct deviceInfo* devicePoint;
	printf("******show switch INFO******\n");
	/*RIO_LOG("******show switch INFO******\n",1,2,3,4,5,6);;*/
	
	for(i=1;i<curMaxIndex;i++) 
	{
		printf("\n*************switch:%d, hopCount %d  has %d ports  ************\n",switchArray[i]->switch_ID,switchArray[i]->hopCount,switchArray[i]->portNum);
		for(hopIndex=0; hopIndex<switchArray[i]->hopCount;hopIndex++)
		{
			printf("%d ",switchArray[i]->routePortsArray[hopIndex]);
		}
		printf("\n");
		
		/*RIO_LOG("\n*************switch:%d  has %d ports  currentActivePort:%d ************\n",i,gSwInfo[i].portNum,gSwInfo[i].currentActivePort,3,4,5,6);*/
		for(j=0;j<switchArray[i]->portNum;j++)
		{
			//devicePoint=switchArray[i].device_IDs
			if(switchArray[i]->device_IDs[j].type == NOTHING)
			{
				printf("port %d : not connect!!\n",j);
				/*RIO_LOG("port %d : not connect!!\n",j,2,3,4,5,6);*/
			}
			else
			{
				printf("port:%d",j);
				/*RIO_LOG("port:%d",j,2,3,4,5,6);*/
				if(switchArray[i]->device_IDs[j].type == SWITCH) 
				{
						
					printf("    switch   ");
					printf("SWID %d:%d ->  SWID %d:%d  %c \n",switchArray[i]->switch_ID,j,switchArray[i]->device_IDs[j].ID,switchArray[i]->device_IDs[j].connPort, switchArray[i]->device_IDs[j].type);
				/*
					RIO_LOG("    switch   ",1,2,3,4,5,6);
					RIO_LOG("SWNO:%d   SWPORT:%d   \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort,3,4,5,6);
				*/
				}
				
				else
				{
					
					printf("    EndP     ");
					printf("SWID %d:%d ->  EPID 0x%x  %c\n",switchArray[i]->switch_ID,j,switchArray[i]->device_IDs[j].ID,switchArray[i]->device_IDs[j].type);
				/*	
					RIO_LOG("    EndP     ",1,2,3,4,5,6);
					RIO_LOG("SWNO:%d   SWPORT:%d   DevId:%d  \n",gSwInfo[i].swPortInfo[j].switchNo,gSwInfo[i].swPortInfo[j].switchPort,gSwInfo[i].swPortInfo[j].deviceId,4,5,6);
				*/	
				}
			}
		}
	}	
}
void testRead()
{
	UINT32 hostCpuID = 0;
	UINT32 connPort = 2;
	UINT32 destid = 0xff;
	UINT32 portIndex = 10;
	
	ROUTE_SET_P_FOR_ALL(DEF_ID,0,&hostCpuID,&connPort); 
	ROUTE_SET_P_TO_P(DEF_ID,0,&destid,&portIndex,&connPort); 
}
