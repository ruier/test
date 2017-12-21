/* rioSwitchInit.c - Init the RapidIO switch in the board.*/

/*
 * Copyright (c) 2013- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */

/*
modification history
-----------------------
03/06/2013,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Initialize the RapidIO Switch.
 * 
 */

/* includes */
#include <vxworks.h>
#include <stdio.h>
#include "rioPrivate.h"
#include "rioConfig.h"
#include "rioException.h"
#include "rioSwitchInit.h"





//void initRioSwitch()
//{
//	RIO_ID destId;
//	UINT8 hopCount;
//	UINT32 regVal;
//	UINT8 portNum;
//	UINT8 connPort;
//	UINT8 portIndex;
//	int i = 0;
//	
//	destId = DEF_ID;
//	hopCount = 0;
//	rioExcInit();
//	//CONFIG_READ(destId,hopCount,RIO_PEF_CAR,&regVal);
//	SWITCH_INIT_DBG_LOG("enter initRioSwitch!\n",1,2,3,4,5,6);
//	
//	//if((regVal&0x2) == 0x2)
//	{
//		CONFIG_READ(DEF_ID,hopCount,SPICAR,&regVal);
//		portNum =  (regVal & 0xFF00) >> 8;
//		CONFIG_READ(DEF_ID,hopCount,SPICAR,&regVal);
//		
//		connPort = regVal & 0xFF;
//		SWITCH_INIT_DBG_LOG("in initRioSwitch portNum = %d,connPort = %d!\n",portNum,connPort,3,4,5,6);
//		/* debug */
//		//DEBUG_LOG("regVal is %d,connect port is %d\n",regVal,connPort,3,4,5,6);
//		/* debug */
//		/* Set all Maintenance respones packet route to connPort*/
//	//	ROUTE_SET_P_FOR_ALL(DEF_ID,hopCount,&destId,&connPort); 
//
//		/* debug */
//		//DEBUG_LOG("detect ports,cpuID is %d\n",cpuID,2,3,4,5,6);
//		/* debug */
//			
//		for(portIndex=0; portIndex< portNum; portIndex++)
//		{
//			/* 如果端口号为维护包输入端口，则不检测该端口*/
//			if (portIndex == connPort) 
//			{
//				EXPLORE_DBG_LOG("This is the port which connected to the upstream device!\n",1,2,3,4,5,6);
//			//	continue;
//			}
//
//			/* 设置从Host Port发出的数据包到Port i的路由表项 */
//			destId = DEF_ID;
//			ROUTE_SET_P_TO_P(DEF_ID,hopCount,&destId,&portIndex,&connPort);
//			ROUTE_SET_P_TO_P(DEF_ID,hopCount,&destId,&connPort,&portIndex);
//			/* 如果对于同一个地址的多个路由表项不能相互覆盖，这里将出现小的问题，需要调试测试 */
//
//
//			/* debug */
//			//DEBUG_LOG("portIndex is %d\n",portIndex,2,3,4,5,6);
//			/* debug */
//
//			//		/*test the PORT_OK bit, if this bit is set,then this port is connected to a device */
//			//		CONFIG_READ(DEF_ID,swPtr->hopCount,PORT_ESCSR + (portIndex * 0x20),&regVal);		
//
//			//DEBUG_LOG("Port %d PORT_ESCSR value is %d !\n",portIndex,regVal,3,4,5,6);		
//
//			//CONFIG_READ(DEF_ID,0,0x158+portIndex*0x20,&regVal);
//			//		CONFIG_READ(DEF_ID,swPtr->hopCount,0x158+portIndex*0x20,&regVal);
//			//		DEBUG_LOG("port %d,addr 0x%x,value 0x%x  hopCount %d!\n",portIndex,0x158+portIndex*0x20,regVal,swPtr->hopCount,5,6);
//			//		continue;
//
//			/* if this port connectes nothing,then we detect the next port */
//			if(rioSwPortIsActive(1,DEF_ID,hopCount,portIndex))
//			{
////				CONFIG_READ(destId,hopCount+1,RIO_PEF_CAR,&regVal);
////				SWITCH_INIT_DBG_LOG("in initRioSwitch port %d RIO_PEF_CAR value is 0x%x!\n",portIndex,regVal,3,4,5,6);
////				if((regVal&0x2) != 0x2)
////				{
////					continue;
////				}
//				if(portIndex != 17)
//				{
//					continue;
//				}
//				SWITCH_INIT_DBG_LOG("in initRioSwitch port %d is active!\n",portIndex,2,3,4,5,6);
//				regVal = 0x80000000|1<<portIndex;
//				CONFIG_WRITE(DEF_ID,hopCount,0xf20300,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("1!\n",1,2,3,4,5,6);
//				regVal = 0x00000000;
//				CONFIG_WRITE(DEF_ID,hopCount,0x1044+0x40*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("2!\n",1,2,3,4,5,6);
//				regVal = 0x00000000;
//				CONFIG_WRITE(DEF_ID,hopCount,0x106c+0x40*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("3!\n",1,2,3,4,5,6);
//				regVal = 0x00000000;
//				CONFIG_WRITE(DEF_ID,hopCount,0x1068+0x40*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("4!\n",1,2,3,4,5,6);
//				// TODO CONFIG_WRITE(0xff,hopCount,0xff8010+0x40*port,0x00000000);
//				regVal = 0x07000000;
//				CONFIG_WRITE(DEF_ID,hopCount,0x158+0x20*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("5!\n",1,2,3,4,5,6);
//				regVal = 0x00000003;
//				CONFIG_WRITE(DEF_ID,hopCount,0x140+0x20*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("6!\n",1,2,3,4,5,6);
//				regVal = 0x80000000|1<<portIndex;
//				CONFIG_WRITE(DEF_ID,hopCount,0xf20300,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("7!\n",1,2,3,4,5,6);
//				regVal = 0x00000004;
//				CONFIG_WRITE(DEF_ID,hopCount,0x140+0x20*portIndex,&regVal);
//				taskDelay(10);
//				SWITCH_INIT_DBG_LOG("8!\n",1,2,3,4,5,6);
//				CONFIG_READ(DEF_ID,hopCount+1,0x000000,&regVal);
//				SWITCH_INIT_DBG_LOG("regVal = 0x%x!\n",regVal,2,3,4,5,6);
//				if(regVal != 0x80000000)
//				{
//					regVal = 0x80000000;
//					CONFIG_WRITE(DEF_ID,hopCount+1,0x148+0x20*portIndex,&regVal);
//					SWITCH_INIT_DBG_LOG("9!\n",1,2,3,4,5,6);
//					regVal = (0x80000000|1<<portIndex);
//					CONFIG_WRITE(DEF_ID,hopCount,0xf20300,&regVal);
//					SWITCH_INIT_DBG_LOG("10!\n",1,2,3,4,5,6);
//					regVal = 0x00000003;
//					CONFIG_WRITE(DEF_ID,hopCount,0x140 + 0x20*portIndex,&regVal);
//					SWITCH_INIT_DBG_LOG("11!\n",1,2,3,4,5,6);
//				}
//				else
//				{
//					//TODO Step 7
//					for(i=0; i<63; i++)
//					{
//						SWITCH_INIT_DBG_LOG("12!\n",1,2,3,4,5,6);
//						CONFIG_READ(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//						regVal &= 0xc0ffffff;
//						regVal |= i<<24;
//						CONFIG_WRITE(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//						SWITCH_INIT_DBG_LOG("13!\n",1,2,3,4,5,6);
//						regVal = 0x00000004;
//						CONFIG_WRITE(DEF_ID,hopCount,0x140+0x20*portIndex,&regVal);
//						CONFIG_READ(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//						if(((regVal&0x3f000000)>>24)!=i)
//						{
//							break;
//						}
//					}
//
//					CONFIG_READ(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//					regVal &= 0xffff;
//					CONFIG_WRITE(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//					regVal = 0x81000000;
//					CONFIG_WRITE(DEF_ID,hopCount+1,0x148 + 0x20*portIndex,&regVal);
//					regVal = 0x80000000;
//					CONFIG_WRITE(DEF_ID,hopCount,0x148 + 0x20*portIndex,&regVal);
//					regVal = 0x00000003;
//					CONFIG_WRITE(DEF_ID,hopCount,0x140 + 0x20*portIndex,&regVal);
//					regVal = (0x80000000|1<<portIndex);
//					CONFIG_WRITE(DEF_ID,hopCount,0xf20300,&regVal);
//				}
//
//			}
//		}
//	}
//	//	else
//	{
//		SWITCH_INIT_DBG_LOG("exit switch Init!\n",1,2,3,4,5,6);
//	}
//}



STATUS portRecover(UINT8 hopCount,UINT8 port)
{
	UINT16 destId = DEF_ID;
	UINT32 regVal;
	UINT32 localAckID;
	UINT32 outBoundAckID;
	EXPLORE_DBG_LOG("enter rioPortRecover\n",1,2,3,4,5,6);
	CONFIG_READ(destId,hopCount,PORT_ERR_STAT_CSR(port),&regVal);
	EXPLORE_DBG_LOG("in rioPortRecover ERR_STAT_CSR = 0x%x\n",regVal,2,3,4,5,6);
	if((regVal&0x02)== 2)
	{
		EXPLORE_DBG_LOG("in rioPortRecover enter if\n",1,2,3,4,5,6);
		if(regVal !=2)
		{
			CONFIG_READ(destId,hopCount,PORT_ERR_STAT_CSR(port),&regVal);
			if((regVal &0x10100) !=0) 
			{
				//rioFlag[i] =1 ;
				EXPLORE_DBG_LOG("open and recover port %d\n",port,2,3,4,5,6);
				//printf("open and recover port %d\n",portNo);
				//tsi578PortRecover(portNo);
				//rioPortPowerDown(hopCount,port);
				rioPortPowerUp(hopCount,port);
				CONFIG_READ(destId,hopCount,PORT_CTL_CSR(port),&regVal);
				regVal |= 0x2;
			}

		}
	}

	EXPLORE_DBG_LOG("exit rioPortRecover\n",1,2,3,4,5,6);
}
//LOCAL void rioPortRecover(UINT8 hopCount,UINT8 port)
//{
//	
//}

/******************************************************************************
 * initRioSwitch - hang the rio exception handler,and init the RapidIO switch
 * 
 */
void initRioSwitch()
{
	UINT32 hostCpuID = 0;
	UINT32 connPort = 2;
	UINT32 destId = 0xff;
	UINT32 portIndex = 10;
	UINT16 hostId = 1;

	UINT16 lockId;
	UINT32 result;
	UINT32 regVal = 0;
	UINT32 switchNo;
	UINT8 portNum,hostPort;//,i;/*,j*/
	UINT8 hopCount = 0;

	rioExcInit();

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

//	if(rioAcquirLock(1,0xff,0,hostId,&lockId)==ERROR)
//	{
//
//	}	
	
	regVal = rioGetPEF(0xff,hopCount,1);
//	SWITCH_INIT_DBG_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",0,regVal,3,4,5,6);

	if(regVal&RIO_PEF_SWITCH) //sjy 20130428 alter if(rioIsSwitch(localPort,0xff,hopCount))
	{

//		rioGetSwPortInf(1,0,&result);
		CONFIG_READ(0xff,0,RIO_SWP_INFO_CAR,&result);
		

		hostPort = result&0xff;
		portNum = (result&0xff00)>>8;

		//rioRouteAddEntry(1,hostId,hopCount,hostPort); 
		ROUTE_SET_P_TO_P(DEF_ID,hopCount,&hostId,&hostPort,&portIndex);
//		rioRouteAddEntry(1,hostId,0,hostPort);
		for(portIndex=0;portIndex<portNum;portIndex++)
		{
			if(portIndex == hostPort)
			{
				
				continue; 
			}
			if(rioSwPortIsActive(1,0xff,0,portIndex))
			{				
			//	rioRouteAddEntry(1,0xff,0,portIndex);
				ROUTE_SET_P_TO_P(DEF_ID,hopCount,&destId,&portIndex,&hostPort);
//				if(rioAcquirLock(1,0xff,1,hostId,&lockId)==ERROR)
//				{
//					
//				}
					
				regVal = rioGetPEF(0xff,hopCount+1,1);
				SWITCH_INIT_DBG_LOG("in rioScanLockDevice hopCount %d pef value = 0x%x\n",1,regVal,3,4,5,6);
//				if(regVal == ERROR)
//				{
//					rioPortRecover(hopCount,portIndex);
//				}
				printf("in rioScanLockDevice hopCount %d pef value = 0x%x %d\n",hopCount,regVal,regVal);

			}
			else 
			{
				SWITCH_INIT_DBG_LOG("hopCount %d port %d not active!\n",1,portIndex,3,4,5,6);
				printf("hopCount %d port %d not active!\n",1,portIndex);
			}

		}

	}
	rioConfigLocalWrite(RIO_HOST_DID_LOCK_CSR,hostId);
	rioConfigLocalRead(RIO_HOST_DID_LOCK_CSR,&result);

	if((result&0xffff) != 0xffff)
	{
		return ERROR;
	}
	else
	{
		return OK;
	}

	
	
}

