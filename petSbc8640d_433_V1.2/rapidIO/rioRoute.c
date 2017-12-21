/* CPS1848RioRoute.c - This file contains the route path configuration for CPS1848.*/

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
12/09/2012,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Configuration the RapidIO Switch Route Pathes of CPS1848.
 * 
 */


/* includes */

#include<vxWorks.h>
#include<stdio.h>
#include "rioConfig.h"
#include "rioExplore.h"
#include "rioRoute.h"
#include "rioPrivate.h"

#define LOCAL static

/* variables */
//struct switch_desc * hostSwitchArray[MAX_SWITCH_NUM] = {(struct switch_desc *)0};
struct route_info routeInfoTable[MAX_SWITCH_NUM] = {{DEF_ID,0,0,NULL}};
int switchesDetectedTable[MAX_SWITCH_NUM] = {0};
UINT32 curDetectIndex = 0;
UINT32 maxDetectIndex = 1;
//int switchNum = 0;
//
//int switchPortNums = 0;
//struct switch_desc * switchArray[MAX_SWITCH_NUM];

/* functions */

/*****************************************************************************
* 
* routeSysInit - Initialize the route partition of the system
* 
* Read the system topology information from the configuration file and use it 
* to initialize the data structure and do other things.
* 
*/

LOCAL int routeSysInit()
{
	FILE * fd;
	int returnVal = 0;
	int tempID = 0;
	char tempType = 'N';
	int tempSwID = 0;
	int switchIndex = 0;
	int portIndex = 0;
//	
//	/* read the configuration file. */
//	if ((fd = fopen(CONFIG_FILE_NAME,"r")) == NULL)
//	{
//		/* TODO: do something to hold the error */
//		printf("Can't open file:%s\n",CONFIG_FILE_NAME);
//		return -1;
//	}
//	
//	returnVal = fscanf(fd,"HostID:%d SwitchNum:%d switchPortNum:%d\n",&hostCpuID,\
//			&switchNum,&switchPortNums);
//	//DEBUG_LOG("HostID:%d SwitchNum:%d switchPortNum:%d\n",hostCpuID,switchNum,switchPortNums,4,5,6);
//	for (switchIndex=1; switchIndex<detectedSwitchNum; switchIndex++)
//	{
//		
//		hostSwitchArray[switchIndex] = (struct switch_desc *) \
//				malloc(sizeof(struct switch_desc));
//		hostSwitchArray[switchIndex]->device_IDs = (struct deviceInfo *)\
//				malloc(sizeof(struct deviceInfo) * switchPortNums);
//		switchArray[switchIndex]->hopCount= 0;
//		switchArray[switchIndex]->routePortsArray = NULL;
//		returnVal = fscanf(fd,"SwitchID:%d,",&tempSwID);
//		DEBUG_LOG("SwitchID:%d,",tempSwID,2,3,4,5,6);
//		switchArray[switchIndex]->switch_ID = tempSwID;
//		
//		for(portIndex=0; portIndex<switchPortNums; portIndex++)
//		{
//			returnVal = fscanf(fd,"%c%d,",&tempType,&tempID);
//			DEBUG_LOG("%c%d,",tempType,tempID,3,4,5,6);
//			switchArray[switchIndex]->device_IDs[portIndex].type = tempType;
//			switchArray[switchIndex]->device_IDs[portIndex].ID = tempID;
//		}
//		returnVal = fscanf(fd,"\n");
//		DEBUG_LOG("\n",1,2,3,4,5,6);
//	}
//	
//	fclose(fd);
//	
	for (switchIndex=1; switchIndex<detectedSwitchNum; switchIndex++)
	{
		routeInfoTable[switchIndex].portList = (UINT32 *) malloc(sizeof(UINT32 *) * NUM_OF_SWITCH_PORTS);
	}
	return 1;	
}

/*****************************************************************************
*
*findHostRouteInfo - Find each shortest paths,from Host switch to every other switch.
*
*/

LOCAL void findHostRouteInfo()
{
	int curSwitchIndex = 1;
	struct deviceInfo * tempDevicesPtr = NULL;
	UINT8 * destPtr = NULL;
	UINT8 * srcPtr = NULL;
	int index = 0;
	int i = 0;
	
	ROUTE_DBG_LOG("enter findHostRouteInfo\n",1,2,3,4,5,6);
	
//	switchArray[curSwitchIndex]->hopCount = 0;
	switchArray[curSwitchIndex]->routePortsArray = -1;
	while(curSwitchIndex < detectedSwitchNum)
	{
		tempDevicesPtr = switchArray[curSwitchIndex]->device_IDs;
		for(index=0; index<switchArray[curSwitchIndex]->portNum; index++)
		{
			/* 寻找到与curSwitchIndex相连且未初始化过routePortsArray的switch,
			 * 然后初始化routePortsArray
			 */
			if((tempDevicesPtr->type == SWITCH)&&\
					(switchArray[tempDevicesPtr->ID]->routePortsArray == NULL))
			{
//				switchArray[tempDevicesPtr->ID]->hopCount = \
//						switchArray[curSwitchIndex]->hopCount + 1;
				switchArray[tempDevicesPtr->ID]->routePortsArray = \
						(UINT8 *)malloc(sizeof(UINT8 *) * \
						(switchArray[curSwitchIndex]->hopCount + 1));
				srcPtr = switchArray[curSwitchIndex]->routePortsArray;
				destPtr = switchArray[tempDevicesPtr->ID]->routePortsArray;
				for(i=0; i<switchArray[curSwitchIndex]->hopCount; i++)
				{
					*destPtr++ = *srcPtr++;
				}
				*destPtr = index;
			}
			tempDevicesPtr++;
		}
		
		curSwitchIndex++;
	}
	ROUTE_DBG_LOG("exit findHostRouteInfo\n",1,2,3,4,5,6);
}


/*****************************************************************************
* 
* getRouteInfo() - Get the route port for every remote switch.
* 
*/

void getRouteInfo(UINT32 switchID)
{
	
	UINT32 curSwitchID = switchID;
	UINT32 tempID = 0;
	UINT32 tempPort = 0;
	UINT32 * tempPtr = NULL;
	UINT32 *srcPortPtr = NULL;
	UINT32 *destPortPtr = NULL;
	UINT8 hopCount = 0;
	int portIndex = 0;
	struct deviceInfo* tempDeviceInfo = NULL;
	
	int i = 0;

	
	for(i=0;i<maxDetectIndex;i++)
	{
		if(routeInfoTable[i].portList != NULL)
		{
			free(routeInfoTable[i].portList);
			routeInfoTable[i].portList = NULL;
			routeInfoTable[i].portList = (UINT32 *) malloc(sizeof(UINT32 *) * NUM_OF_SWITCH_PORTS);
		}
	}
	
	maxDetectIndex = 1;
	curDetectIndex = 0;
	
	
	ROUTE_DBG_LOG("\nenter getRouteInfo(switchID %d)\n",switchID,2,3,4,5,6);
	/* init the route info table */
	ROUTE_DBG_LOG("detectedSwitchNum = %d\n",detectedSwitchNum,2,3,4,5,6);
	
	for(i=1; i<detectedSwitchNum; i++)
	{
		//ROUTE_DBG_LOG("i = %d\n",i,2,3,4,5,6);
		routeInfoTable[i].remoteSwitchID = 0;
		routeInfoTable[i].hopCount = -1;
		routeInfoTable[i].numOfPort = 0;
	}
	
	/* init the detected info table */
	for (i=1; i<detectedSwitchNum; i++)
	{
		switchesDetectedTable[i] = 0;
		//ROUTE_DBG_LOG("i %d switchesDetectedTable %d :\n",i,switchesDetectedTable[i],3,4,5,6);
	}
	ROUTE_DBG_LOG("\n",1,2,3,4,5,6);
	/* The switch whose ID is 'switchID' does not need detected */
	switchesDetectedTable[switchID] = 1;

	routeInfoTable[curDetectIndex].hopCount = 0;
	routeInfoTable[curDetectIndex].remoteSwitchID = switchID;
	routeInfoTable[curDetectIndex].numOfPort = 0;
	
	curSwitchID = routeInfoTable[curDetectIndex].remoteSwitchID;
	tempDeviceInfo = switchArray[curSwitchID]->device_IDs;
	ROUTE_DBG_LOG("before for portNum %d\n",switchArray[curSwitchID]->portNum,2,3,4,5,6);
	
	for(portIndex=0; portIndex<switchArray[curSwitchID]->portNum; portIndex++)
	{
	//	ROUTE_DBG_LOG("in for portIndex = %d\n",portIndex,2,3,4,5,6);		
		if (tempDeviceInfo->type == SWITCH)
		{
			tempID = tempDeviceInfo->ID;
		//	ROUTE_DBG_LOG("switch ID %d maxDetectIndex %d detected %d: %d\n",tempDeviceInfo->ID,maxDetectIndex,switchesDetectedTable[tempID],tempID,5,6);
			
			if (switchesDetectedTable[tempID] == 0)
			{
//#ifdef SET_ROUTE_DEBUG
//	ROUTE_DBG_LOG("\n11-------------------switch ID %d :\n",switchID,2,3,4,5,6);
//	routeInfoPrint();
//	ROUTE_DBG_LOG("---------------------\n\n",1,2,3,4,5,6);
//#endif
				ROUTE_DBG_LOG("switch ID %d connport %d maxDetectIndex %d :\n",tempDeviceInfo->ID,tempDeviceInfo->connPort,maxDetectIndex,4,5,6);		
				switchesDetectedTable[tempID] = maxDetectIndex;
				routeInfoTable[maxDetectIndex].hopCount = 1;
				routeInfoTable[maxDetectIndex].remoteSwitchID = tempDeviceInfo->ID;
				tempPtr = routeInfoTable[maxDetectIndex].portList;
				tempPtr[routeInfoTable[maxDetectIndex].numOfPort] = tempDeviceInfo->connPort;
				ROUTE_DBG_LOG("switch ID %d connport %d numOfPort%d tempPtr %d routeTable portList %d:\n",tempDeviceInfo->ID,tempDeviceInfo->connPort,routeInfoTable[maxDetectIndex].numOfPort,*tempPtr,routeInfoTable[maxDetectIndex].portList[0],6);
			//	ROUTE_DBG_LOG("curIndex %d %d \n",curDetectIndex,routeInfoTable[curDetectIndex].portList[0],3,4,5,6);
				routeInfoTable[maxDetectIndex].numOfPort++;
				maxDetectIndex++;
//#ifdef SET_ROUTE_DEBUG
//	ROUTE_DBG_LOG("\n22-------------------switch ID %d :\n",switchID,2,3,4,5,6);
//	routeInfoPrint();
//	ROUTE_DBG_LOG("---------------------\n\n",1,2,3,4,5,6);
//#endif
		
			}
			else
			{
				if ((switchesDetectedTable[tempID] > switchesDetectedTable[curSwitchID]) \
						&& (hopCount == routeInfoTable[switchesDetectedTable[tempID]].hopCount))
				{
					tempPtr[routeInfoTable[switchesDetectedTable[tempID]].numOfPort++] = tempDeviceInfo->connPort;
					ROUTE_DBG_LOG("in if switch ID %d connport %d numOfPort%d tempPtr %d routeTable portList %d:\n",tempDeviceInfo->ID,tempDeviceInfo->connPort,routeInfoTable[maxDetectIndex].numOfPort,*tempPtr,routeInfoTable[curDetectIndex].portList[routeInfoTable[curDetectIndex].numOfPort],6);
				}
			}
		}
		tempDeviceInfo ++;
	}

	curDetectIndex++;
	
	while(curDetectIndex < maxDetectIndex)
	{
		curSwitchID = routeInfoTable[curDetectIndex].remoteSwitchID;
		tempDeviceInfo = switchArray[curSwitchID]->device_IDs;
		for(portIndex=0; portIndex<switchArray[curSwitchID]->portNum; portIndex++)
		{
			if (tempDeviceInfo->type == SWITCH)
			{
				//ROUTE_DBG_LOG("in if \n",1,2,3,4,5,6);
				tempID = tempDeviceInfo->ID;
				if (switchesDetectedTable[tempID] == 0)
				{
					ROUTE_DBG_LOG("in if 2\n",1,2,3,4,5,6);
					switchesDetectedTable[tempID] = maxDetectIndex;
					routeInfoTable[maxDetectIndex].hopCount = \
							routeInfoTable[curDetectIndex].hopCount + 1;
					routeInfoTable[maxDetectIndex].remoteSwitchID = tempDeviceInfo->ID;
					tempPtr = routeInfoTable[maxDetectIndex].portList;
//					srcPortPtr = routeInfoTable[curDetectIndex].portList;
//					destPortPtr = tempPtr;
//					for(i=0; i<routeInfoTable[curDetectIndex].numOfPort;i++)
//					{
//						*destPortPtr++ = *srcPortPtr++;
//					}
					tempPtr[routeInfoTable[maxDetectIndex].numOfPort] = tempDeviceInfo->connPort;
					routeInfoTable[maxDetectIndex].numOfPort++;
					maxDetectIndex++;
				}
				else
				{
					if ((switchesDetectedTable[tempID] > switchesDetectedTable[curSwitchID]) \
							&& ((routeInfoTable[curDetectIndex].hopCount + 1) == \
									routeInfoTable[switchesDetectedTable[tempID]].hopCount))
					{
						ROUTE_DBG_LOG("in if 2 switch ID %d connport %d numOfPort%d tempPtr %d routeTable portList %d:\n",tempDeviceInfo->ID,tempDeviceInfo->connPort,routeInfoTable[maxDetectIndex].numOfPort,*tempPtr,routeInfoTable[curDetectIndex].portList[routeInfoTable[curDetectIndex].numOfPort],6);
						tempPtr[routeInfoTable[switchesDetectedTable[tempID]].numOfPort++] = tempDeviceInfo->connPort;
					}
				}
			}
			tempDeviceInfo ++;
		}
		curDetectIndex++;
	}
	ROUTE_DBG_LOG("\nexit getRouteInfo\n",1,2,3,4,5,6);
}

/*****************************************************************************
*
*setSwitchRoute() - 
*/

void setSwitchRoute(UINT32 destSwitchID,UINT32 remoteSwitchID, UINT32 * routePortListPtr,UINT32 numOfPort)
{
	UINT32 devicePortList[NUM_OF_SWITCH_PORTS] = {0};
	UINT32 switchPortList[NUM_OF_SWITCH_PORTS] = {0};
	UINT32 destDeviceIDsList[NUM_OF_SWITCH_PORTS] = {0};
	UINT32 numOfDevices = 0;
	UINT32 numOfDestDevices = 0;
	UINT32 numOfSwitches = 0;
	UINT32* tempPortPtr = routePortListPtr;
	UINT32 curPort = 0;
	int portIndex;
	UINT8 *routeArray = NULL; 
	UINT32 regVal = 0;
	UINT8 i = 0;
	UINT32 destid = DEF_ID;
	int index = 0;
	
	struct deviceInfo * tempPtr = switchArray[destSwitchID]->device_IDs; 
	ROUTE_DBG_LOG("enter setSwitchRoute\n",1,2,3,4,5,6);
	/* 根据相对于Host 的最短路径来配置远程switch */
	
	/* 设置switch通路中的路径路由表 */
	routeArray = switchArray[remoteSwitchID]->routePortsArray;
	
	destid = DEF_ID;
	for(i=0; i<switchArray[remoteSwitchID]->hopCount; i++)
	{
		CONFIG_READ(DEF_ID,i,SPICAR,&regVal);
		curPort = regVal & 0xFF;
		ROUTE_SET_P_TO_P(DEF_ID,i,&destid,routeArray++,&curPort);
	}
	ROUTE_DBG_LOG("after for(i=0; i<switchArray[remoteSwitchID]->hopCount; i++)\n",1,2,3,4,5,6);
	/* get destination switch's normal devices' ID list */
	for(portIndex=0; portIndex<switchArray[remoteSwitchID]->portNum; portIndex++)
	{
		if (tempPtr->type == DEVICE)
		{
			destDeviceIDsList[numOfDestDevices++] = tempPtr->ID;
		}
		tempPtr++;
	}
	ROUTE_DBG_LOG("numOfPort  %d\n",numOfPort,2,3,4,5,6);
	/* TODO:这里可以使用多地址路由配置指令进行配置 */
	/* if there has only one port can route packetes to the destSwitch,we will 
	 * route every destDeviceID to that port for all port
	 */
	if (numOfPort == 1)
	{
		ROUTE_DBG_LOG("after if (numOfPort == 1)\n",1,2,3,4,5,6);	
		for(index=0; index<numOfDestDevices; index++)
		{
			ROUTE_SET_P_FOR_ALL(DEF_ID,switchArray[remoteSwitchID]->hopCount,&(destDeviceIDsList[index]),tempPortPtr);
			ROUTE_DBG_LOG("hopCount = %d,deviceID = %d,port = %d\n",switchArray[remoteSwitchID]->hopCount,destDeviceIDsList[index],*tempPortPtr,4,5,6);
		}
		ROUTE_DBG_LOG("after for(index=0; index<numOfDestDevices; index++)\n",1,2,3,4,5,6);
	}
	else
	{
		/* get source device list and switch list */
		tempPtr = switchArray[remoteSwitchID]->device_IDs;
		for(portIndex=0; portIndex<switchArray[remoteSwitchID]->portNum; portIndex++)
		{
			if (tempPtr->type == DEVICE)
			{
				devicePortList[numOfDevices++] = portIndex;
			}
			else
			{
				if (tempPtr->type == SWITCH)
				{
					tempPortPtr = routePortListPtr;
					for (i=0; i<numOfPort; i++)
					{
						/* if portIndex is in the routePortList,we don't need to \ 
						 set its route item */
						if (tempPortPtr[i] == portIndex)
						{
							break;
						}
					}
					if (i == numOfPort)
					{
						switchPortList[numOfSwitches++] = portIndex;
					}
				}
			}
		}
		
		/* set route items */
		
		for (i=0,curPort = 0; i<numOfDevices; i++,curPort++)
		{
			if (curPort == numOfPort)
			{
				curPort = 0;
			}
			for(index=0; index<numOfDestDevices; index++)
			{
				ROUTE_SET_P_TO_P(DEF_ID,switchArray[remoteSwitchID]->hopCount,\
						&(destDeviceIDsList[index]),routePortListPtr+curPort,\
						&devicePortList[i]);
			}
		}
		
		for (i=0,curPort = 0; i<numOfSwitches; i++,curPort++)
		{
			if (curPort == numOfPort)
			{
				curPort = 0;
			}
			for(index=0; index<numOfDestDevices; index++)
			{
				ROUTE_SET_P_TO_P(DEF_ID,switchArray[remoteSwitchID]->hopCount,\
						&(destDeviceIDsList[index]),routePortListPtr+curPort,\
						&switchPortList[i]);
			}
		}
	}	
	ROUTE_DBG_LOG("exit setSwitchRoute\n",1,2,3,4,5,6);
}


/*****************************************************************************
*
*setRoute() - Set the switch route table for a switch.
*Input: switchID, the switch ID who need to set the route table.
*
*/ 

void setRoute(UINT32 switchID)
{
	UINT32 remoteSwitchID = 0;
	UINT32 deviceID = 0;
	UINT32 * portListPtr = NULL;
	UINT8 * routeArray = NULL;
	struct deviceInfo * deviceInfoPtr;
	UINT32 numOfPort = 0;
	int index = 0;
	int i = 0;
	RIO_ID destid = DEF_ID;
	UINT8 connPort = 0;
	UINT32 regVal = 0;
	UINT32 portIndex = 0;
	RIO_ID domain_id = 0x0;
	RIO_ID deviceId;
	
	ROUTE_DBG_LOG("enter setRoute(swID %d)\n",switchID,2,3,4,5,6);
	/* set all local normal devices' route-in items */
	
	/* 设置switch通路中的路径路由表 */
	routeArray = switchArray[switchID]->routePortsArray;

	destid = DEF_ID;
	for(i=0; i<switchArray[switchID]->hopCount; i++)
	{
		CONFIG_READ(DEF_ID,i,SPICAR,&regVal);
		connPort = regVal & 0xFF;
		ROUTE_SET_P_TO_P(DEF_ID,i,&destid,routeArray++,&connPort);
	}
	
	deviceInfoPtr = switchArray[switchID]->device_IDs;

	/* set local device IDs' route port */
	for(portIndex=0; portIndex<switchArray[switchID]->portNum; portIndex++)
	{
		if (deviceInfoPtr[portIndex].type == DEVICE)
		{
#if (SWITCH_TYPE == CPS_1848)
#ifdef RIO_ID_16
			deviceId = (deviceInfoPtr[portIndex].ID)&0x00ff;  //set local switch device route table
#else
			deviceId = (deviceInfoPtr[portIndex].ID);
#endif //RIO_ID_16
#endif //(SWITCH_TYPE == CPS_1848)
			ROUTE_SET_P_FOR_ALL(DEF_ID,switchArray[switchID]->hopCount,&deviceId,&portIndex);
		}
	}
	
	/* get what port is routed to for a remote switch. */
	getRouteInfo(switchID);
	
#ifdef SET_ROUTE_DEBUG
	ROUTE_DBG_LOG("\n-------------------switch ID %d :\n",switchID,2,3,4,5,6);
	routeInfoPrint();
	ROUTE_DBG_LOG("---------------------\n\n",1,2,3,4,5,6);
#endif
	
	/* set all normal devices' route-out items */
	for (index=1; index<maxDetectIndex; index++)
	{
		remoteSwitchID = routeInfoTable[index].remoteSwitchID;
		portListPtr = routeInfoTable[index].portList;
		numOfPort = routeInfoTable[index].numOfPort;
		
		/* set all normal devices' route-out items for switch i */
		setSwitchRoute(switchID,remoteSwitchID,portListPtr,numOfPort);	
	}
	ROUTE_DBG_LOG("exit setRoute()\n",1,2,3,4,5,6);
}

/*****************************************************************************
* initTopologyDevices -  Init the devices(normal devices and switches),get 
* HBDLCSR lock,set discover bit,set device ID, etg.
* 
*/
void initTopologyDevices()
{
	UINT32 destid = DEF_ID;
	UINT8 * routeArray = NULL;
	UINT32 regVal = 0;
	UINT32 connPort = 0;
	int hopCount = 0;
	int i = 0;
	struct deviceInfo * deviceInfoPtr;
	int portIndex = 0;
	UINT32 index = 0;
	
	ROUTE_DBG_LOG("enter initTopologyDevices\n",1,2,3,4,5,6);	
	
	for(index=1; index<detectedSwitchNum; index++)
	{
					
		/* 设置switch通路中的路径路由表 */
		routeArray = switchArray[index]->routePortsArray;
	
		destid = DEF_ID;
		for(i=0; i<switchArray[index]->hopCount; i++)
		{
			CONFIG_READ(DEF_ID,i,SPICAR,&regVal);
			connPort = regVal & 0xFF;
			ROUTE_SET_P_TO_P(DEF_ID,i,&destid,routeArray++,&connPort);
		}
		
		CONFIG_READ(DEF_ID,i,SPICAR,&regVal);
		connPort = regVal & 0xFF;
		ROUTE_SET_P_TO_P(DEF_ID,i,&destid,&index,&connPort);
		
		ROUTE_SET_P_FOR_ALL(DEF_ID,i,&hostCpuID,&connPort);
		
		hopCount = switchArray[index]->hopCount + 1;
		/* Initialize current switch */		
		
		/* get Host Base Device ID Lock CSR */
		CONFIG_WRITE(DEF_ID,hopCount,HBDLCSR,&hostCpuID);
		/* Set discovered bit */
		CONFIG_READ(DEF_ID,hopCount,PGCCSR,&regVal);
		regVal |= 0x20000000;
		CONFIG_WRITE(DEF_ID,hopCount,PGCCSR,&regVal);

		/*Set Switch's ID */
		CONFIG_WRITE(DEF_ID,hopCount,COMTAGCSR,&switchArray[index]->switch_ID);
		/* curSwID ++ ---has done in switchExplore() */
		/* TODO: do something else for this switch if it is needed */
		
		
		/* Init local devices */
		hopCount = switchArray[index]->hopCount;
		CONFIG_READ(DEF_ID,hopCount,SPICAR,&regVal);
		connPort = regVal & 0xFF;
		
		/* Set all Maintenance respones packet route to connPort*/
		ROUTE_SET_P_FOR_ALL(DEF_ID,hopCount,&hostCpuID,&connPort); 
		deviceInfoPtr = switchArray[index]->device_IDs;
		for(portIndex=0; portIndex<switchArray[index]->portNum; portIndex++)
		{
			if (portIndex == connPort)
			{
				continue;
			}
			else
			{
				if (deviceInfoPtr[portIndex].type == DEVICE)
				{
					
					/*由于设备是先获取Switch锁的，所以这里不会出现CPU配置冲突问题 ,但也可以用Lock锁一下，这里没有锁*/
					/*  set Host Base DeviceID Lock CSR */
					/* acquire Host Base DeviceID Lock */
					CONFIG_WRITE(DEF_ID,hopCount+1,HBDLCSR,&hostCpuID);
					/* Set discovered bit */
					CONFIG_READ(DEF_ID,hopCount+1,PGCCSR,&regVal);
					regVal |= 0x20000000;
					CONFIG_WRITE(DEF_ID,hopCount+1,PGCCSR,&regVal);
					
					/* Set Device ID */
					//TODO 16 BITS ID
					regVal = deviceInfoPtr[portIndex].ID << 16;
					CONFIG_WRITE(DEF_ID,hopCount+1,BDIDCSR,&regVal);
					ROUTE_DBG_LOG("SET DEVICE ID IS 0x%x\n",deviceInfoPtr[portIndex].ID,2,3,4,5,6);
					
					/* TODO:do some Thing else, if needed*/
				}
			}
		}
	}
	ROUTE_DBG_LOG("enter initTopologyDevices\n",1,2,3,4,5,6);
}
/*****************************************************************************
*
*releaseResource() - Release all the resources,what we had allocated during 
*the set route process.
*/
LOCAL void releaseResource()
{
	int index = 0;
	
	for(index=0; index<MAX_SWITCH_NUM; index++)
	{
		if ((switchArray[index])!= NULL)
		{
			if (switchArray[index]->device_IDs != NULL)
			{
				free(switchArray[index]->device_IDs);
			}
			if (switchArray[index]->routePortsArray != NULL)
			{
				free(switchArray[index]->routePortsArray);
			}
			free(switchArray[index]);
		}
	}
	
	for(index=1; index<=detectedSwitchNum; index++)
	{
		if (routeInfoTable[index].portList != NULL)
		{
			free(routeInfoTable[index].portList);
		}
	}
}

/*****************************************************************************
* 
* topologyRouteConfigure() - The main function which configure the routting 
* paths.
* 
*/

int topologyRouteConfigure(BOOL systemHasInited)
{
	/* read the configuration file,get the informations,and construct the 
	 * data structure
	 */
	UINT32 hostID = 0;
	int i = 0;
	int returnValue = 0;
	ROUTE_DBG_LOG("enter topologyRouteConfigure\n",1,2,3,4,5,6);
	returnValue = routeSysInit();
//	if (returnValue == -1)
//	{
//		printf("An error has occured during the system init process!\n");
//		return -1;
//	}
//	hostID = GET_CPU_ID(); /*get the CPU's device ID */
//	if (hostCpuID != hostID)
//	{
//		releaseResource();
//		return -1;
//	}
	//findHostRouteInfo();
	
	/* if the devices in the system has not Initialized, we do it now. */
	if (systemHasInited == FALSE)
	{
		initTopologyDevices();  /* Init the devices(normal devices and switches),get 
	                               HBDLCSR lock,set discover bit,set device ID, etg. */
	}
	
	for(i=1; i<detectedSwitchNum; i++)
	{
		ROUTE_DBG_LOG("i=%d detectedSwitchNum = %d\n\n",i,detectedSwitchNum,3,4,5,6);
		setRoute(i);
	}
	
	/* The route items setting has finished, so we must release all the resources
	 * what we had allocated.
	 */
	
	//releaseResource();
	ROUTE_DBG_LOG("exit topologyRouteConfigure\n",1,2,3,4,5,6);
	return 0;
}
#ifdef SET_ROUTE_DEBUG

void routeInfoPrint()
{
	int i = 0;
	int j = 0;
	ROUTE_DBG_LOG("maxDetectIndex = %d\n",maxDetectIndex,2,3,4,5,6);
	for(i=1; i< maxDetectIndex;i++)
	{
		ROUTE_DBG_LOG("Remote ID %d,hopCount %d,portNum %d: portListAddr 0x%x ",routeInfoTable[i].remoteSwitchID,routeInfoTable[i].hopCount,routeInfoTable[i].numOfPort,routeInfoTable[i].portList,5,6);
		for(j=0;j<routeInfoTable[i].numOfPort;j++)
		{
			ROUTE_DBG_LOG("%d ",routeInfoTable[i].portList[0],2,3,4,5,6);
		}
		ROUTE_DBG_LOG("\n",1,2,3,4,5,6);
	}
	
}

#endif

