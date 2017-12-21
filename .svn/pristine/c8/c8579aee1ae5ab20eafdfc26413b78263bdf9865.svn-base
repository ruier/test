/* sysRapidIOEnum.c - Initialization,Enum,Configration Routting Pathes .*/

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
 * Initialize the RapidIO Switch of CPS1848.
 * 
 */

/* includes */
#include<stdio.h>
#include<sys\fcntlcom.h>
#include "vxworks.h"
#include "rioConfig.h"
#include "rioExplore.h"
#include "rioPrivate.h"
#include "rioException.h"

#define QUAD_MODE_444 0
#define QUAD_MODE_2244 1
#define QUAD_MODE_22422 2
#define QUAD_MODE_21144 3

/* variables */
BOOL systemHasInited = FALSE;
static RIO_ID hostCpuID = 0;
static struct switch_desc * switchArray[MAX_SWITCH_NUM];
static int detectedSwitchNum = 1;
static BOOL topologyHasDetected = FALSE;
static UINT16 detectedTimes = 1;
static BOOL topologyHasChanged = FALSE;
static BOOL scanFail = FALSE;
static BOOL idHasAssigned = FALSE;
static UINT32 hostPort = 0;
static UINT32 hostHop = 0;
static UINT32 deviceNumInTopology = 0;
static UINT32 deviceNumInFile = 0;
static BOOL idDomainError = FALSE;

/******************************************************************************
 * storeTopologyToFile -- Store the rapidIO topology to the topology file.
 * 
 */
STATUS storeTopologyToFile()
{
	FILE *fd;
	UINT32 swIndex;
	UINT32 portIndex;
	struct deviceInfo* tempDevicesPtr = NULL;
	struct switch_desc * tempSwPtr = NULL;
	char *fileBuf = NULL;
	int fileTotalSize = 0;
	char *tempBufPtr = NULL;
	int tempSize = 0;
	int hopIndex;
	STATUS returnStatus = OK;


	fd = ffopen(TOPOLOGY_FILE_NAME,"r");
	if(fd != NULL) 
	{
		ffclose(fd);
		rm(TOPOLOGY_FILE_NAME);
	}

	fileBuf = (char *)malloc(0x1000);

	if(fileBuf == NULL)
	{
		printf("File buffer malloc error!\n");
		returnStatus = ERROR;
		return returnStatus;
	}

	tempBufPtr = fileBuf;
	fd = ffopen(TOPOLOGY_FILE_NAME,"w+");
	for(swIndex = 1;swIndex<detectedSwitchNum; swIndex++)
	{
		tempSwPtr = switchArray[swIndex];
		tempDevicesPtr = tempSwPtr->device_IDs;

		tempSize = sprintf(tempBufPtr,"%d:",tempSwPtr->hopCount);
		tempBufPtr += tempSize;
		fileTotalSize += tempSize;
		for(hopIndex=0;hopIndex<tempSwPtr->hopCount;hopIndex++)
		{
			tempSize = sprintf(tempBufPtr,"%d,",tempSwPtr->routePortsArray[hopIndex]);
			tempBufPtr += tempSize;
			fileTotalSize += tempSize;
		}
		tempSize = sprintf(tempBufPtr,"[");
		tempBufPtr += tempSize;
		fileTotalSize += tempSize;
		for(portIndex=0; portIndex<tempSwPtr->portNum;portIndex++)
		{
			if(tempDevicesPtr[portIndex].type == DEVICE)
			{
				//				tempSize = sprintf(tempBufPtr,"%d-%d,",portIndex,tempDevicesPtr[portIndex].ID);
				tempSize = sprintf(tempBufPtr,"%d-0x%x,",portIndex,tempDevicesPtr[portIndex].ID);
				tempBufPtr += tempSize;
				fileTotalSize += tempSize;
			}
		}
		tempSize = sprintf(tempBufPtr,"]\n");
		tempBufPtr += tempSize;
		fileTotalSize += tempSize;
	}
	fileBuf[fileTotalSize] = '\0';
	tempSize = ffwrite((UINT8*)fileBuf,1,fileTotalSize,fd);

	if(tempSize != fileTotalSize)
	{
		printf("Topology file write error!\n");
		//		ffclose(fd);
		returnStatus = ERROR;
	}
	ffclose(fd);
	free(fileBuf);

	return returnStatus;	
}

/******************************************************************************
 * setDeviceIDInOrder -- Set the devices' ID to the topology structure.
 * 
 */
STATUS setDeviceIDInOrder()
{
	UINT32 swIndex;
	UINT32 portIndex;
	struct deviceInfo* tempDevicesPtr = NULL;
	struct switch_desc * tempSwPtr = NULL;
	int hopIndex;
	STATUS returnStatus = OK;
	RIO_ID curId = 2;

	for(swIndex = 1;swIndex<detectedSwitchNum; swIndex++)
	{
		tempSwPtr = switchArray[swIndex];
		tempDevicesPtr = tempSwPtr->device_IDs;

		for(portIndex=0; portIndex<tempSwPtr->portNum;portIndex++)
		{
			if((portIndex == hostPort)&&(hostHop == tempSwPtr->hopCount))
			{
				continue;
			}
			if(tempDevicesPtr[portIndex].type == DEVICE)
			{
				tempDevicesPtr[portIndex].ID = curId++;
			}
		}
	}

	return returnStatus;	
}


/******************************************************************************
 * setDeviceIDFromFile -- Set the devices' ID to the topology structure.
 * 
 */
STATUS setDeviceIDFromFile()
{
	FILE *fd;
	UINT32 swIndex;
	UINT32 portIndex;
	struct deviceInfo* tempDevicesPtr = NULL;
	struct switch_desc * tempSwPtr = NULL;
	char *fileBuf = NULL;
	int fileTotalSize = 0;
	char *tempBufPtr = NULL;
	char *curTempBufPtr = NULL;
	int tempSize = 0;
	int hopIndex;
	STATUS returnStatus = OK;
	struct stat	fileInfo;
	int tempHopCount;
	int tempPortIndex;
	int tempRoutePort = 0;
	int tempID;
	BOOL endOfFile = FALSE;
	RIO_ID switchDomain = 0;

	deviceNumInTopology = 0;
	deviceNumInFile = 0;

	DEBUG_LOG("enter setDeviceIDFromFile!\n",1,2,3,4,5,6);
	DEBUG_LOG("-2!\n",1,2,3,4,5,6);
	DEBUG_LOG("0 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
	if (stat(TOPOLOGY_FILE_NAME,&fileInfo) != OK)
	{
		printf("ERROR:The topology file: \"%s\" state error!",TOPOLOGY_FILE_NAME);
		returnStatus = ERROR;
		return returnStatus;
	}
	else 
	{
		fileTotalSize = fileInfo.st_size;
	}
	DEBUG_LOG("-1!\n",1,2,3,4,5,6);
	fd = ffopen(TOPOLOGY_FILE_NAME,"r");
	DEBUG_LOG("0!\n",1,2,3,4,5,6);
	if(fd == NULL) 
	{
		printf("ERROR:The topology file: \"%s\" can't open!",TOPOLOGY_FILE_NAME);
		returnStatus = ERROR;
		return returnStatus;
	}
	DEBUG_LOG("1 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
	fileBuf = (char *)malloc(0x1000);
	DEBUG_LOG("1!\n",1,2,3,4,5,6);
	if(fileBuf == NULL)
	{
		printf("File buffer malloc error!\n");
		returnStatus = ERROR;
		return returnStatus;
	}
	tempSize = ffread((UINT8*)fileBuf,1,fileTotalSize,fd);
	ffclose(fd);
	DEBUG_LOG("2 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
	DEBUG_LOG("2!\n",1,2,3,4,5,6);
	fileBuf[fileTotalSize] = '\0';
	if(tempSize != fileTotalSize)
	{
		//		printf("ERROR:The topology file: \"%s\" read error!",TOPOLOGY_FILE_NAME);
		returnStatus = ERROR;
		return returnStatus;
	}
	DEBUG_LOG("3 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
	DEBUG_LOG("223 in setDeviceIDFromFile file size %d!\n",fileTotalSize,2,3,4,5,6);
	tempBufPtr = fileBuf;

	
	for(swIndex = 1;swIndex<curMaxIndex; swIndex++)
	{
		tempSwPtr = switchArray[swIndex];
		tempDevicesPtr = tempSwPtr->device_IDs;
		curTempBufPtr = tempBufPtr;
		DEBUG_LOG("4 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);

		if(curTempBufPtr >= (fileBuf+fileTotalSize))
		{
			endOfFile = TRUE;
		}

nextAggain:

		if(TRUE != endOfFile)
		{
			sscanf(curTempBufPtr,"%d:",&tempHopCount);
			DEBUG_LOG("242 in setDeviceIDFromFile switch index %d hopCount %d!\n",swIndex,tempHopCount,3,4,5,6);
			while((*curTempBufPtr)!=':')
			{
				curTempBufPtr++;
				if(curTempBufPtr >= (fileBuf+fileTotalSize))
				{
					endOfFile = TRUE;
					break;
				}
			}
			
			if((*curTempBufPtr)==':')
			{
				curTempBufPtr++;
			}
		}
		DEBUG_LOG("5 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
		for(hopIndex=0; hopIndex<tempHopCount; hopIndex++)
		{
			if(curTempBufPtr >= (fileBuf+fileTotalSize))
			{
				endOfFile = TRUE;
			}
			if(TRUE != endOfFile)
			{
				sscanf(curTempBufPtr,"%d,",&tempRoutePort);
//				DEBUG_LOG("286 buf %s tempRoutePort %d!\n",curTempBufPtr,tempRoutePort,3,4,5,6);
			}
			DEBUG_LOG("6 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
			while((*curTempBufPtr)!=',')
			{
				curTempBufPtr++;
				if(curTempBufPtr >= (fileBuf+fileTotalSize))
				{
					endOfFile = TRUE;
					break;
				}
			}	
			DEBUG_LOG("7 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
			if(tempRoutePort == tempSwPtr->routePortsArray[hopIndex])
			{
				
			}
			else
			{
				if(tempRoutePort >= tempSwPtr->routePortsArray[hopIndex])
				{
					break;
				}
				else
				{
					//					printf("Config file error,a switch has losted!\n");
					// jump to next line;
					while((*curTempBufPtr)!='\n')
					{
						curTempBufPtr++;
						if(curTempBufPtr >= (fileBuf+fileTotalSize))
						{
							endOfFile = TRUE;
							break;
						}
					}
					tempBufPtr = curTempBufPtr;
					goto nextAggain;
				}
			}
		}
		DEBUG_LOG("8 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
		//		if(tempRoutePort >= tempSwPtr->routePortsArray[hopIndex])
		//		{
		//			continue;
		//		}

//			DEBUG_LOG("in setDeviceIDFromFile temp Buf：%s!\n",curTempBufPtr,2,3,4,5,6);
		while((*curTempBufPtr)!='[')
		{
			curTempBufPtr++;
			if(curTempBufPtr >= (fileBuf+fileTotalSize))
			{
				endOfFile = TRUE;
				break;
			}
		}
		
		if((*curTempBufPtr)=='[')
		{
			curTempBufPtr++;
		}
		DEBUG_LOG("9 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
		switchDomain = 0;
		for(portIndex=0; portIndex<tempSwPtr->portNum;portIndex++)
		{
			if(tempDevicesPtr[portIndex].type == DEVICE)
			{
				
				DEBUG_LOG("10 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
//				DEBUG_LOG("358 in setDeviceIDFromFile portIndex %d is a device,buf %s!\n",portIndex,curTempBufPtr,3,4,5,6);
				deviceNumInTopology++;
				DEBUG_LOG("363\n",1,2,3,4,5,6);
				if(TRUE == endOfFile)
				{
					//					printf("Error: configFile lost a device\n");
				}
				DEBUG_LOG("369\n",1,2,3,4,5,6);
				if(TRUE != endOfFile)
				{
					if((*curTempBufPtr) == ']')
					{
						//						printf("Error: configFile lost a device\n");
						continue;
					}
				}
				DEBUG_LOG("378\n",1,2,3,4,5,6);
				DEBUG_LOG("11 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
getNext:				
				if(TRUE != endOfFile)
				{
//					DEBUG_LOG("337 buf %s tempPortIndex %d tempID %d\n",curTempBufPtr,tempPortIndex,tempID,4,5,6);
					sscanf(curTempBufPtr,"%d-0x%x",&tempPortIndex,&tempID);
					deviceNumInFile ++;
					if(0 == switchDomain)
					{
						switchDomain = tempID&0xff00;
						DEBUG_LOG("388\n",1,2,3,4,5,6);
					}
					else
					{
						DEBUG_LOG("392\n",1,2,3,4,5,6);
						if(((tempID&0xff00) != switchDomain)&&((tempID&0xff00) != 0))
						{
							DEBUG_LOG("395\n",1,2,3,4,5,6);
							printf("Devices' ID in the config file is error,");
							printf("the devices in the same switch must have ");
							printf("the same ID domain or device id's domain id 0x00\n");
							idDomainError = TRUE;
							return ERROR;
						}
					}
					DEBUG_LOG("400\n",1,2,3,4,5,6);
					//					getPortAndID(curTempBufPtr,&tempPortIndex,&tempID);
					//					sscanf(curTempBufPtr,"%d-%d",&tempPortIndex,&tempID);
					//					while((*curTempBufPtr)!='-')
					//					{
					//						curTempBufPtr++;
					//						if(curTempBufPtr >= (fileBuf+fileTotalSize))
					//						{
					//							endOfFile = TRUE;
					//							break;
					//						}
					//					}
					//					if((*curTempBufPtr) == '-')
					//					{
					//						curTempBufPtr++;
					//						if(curTempBufPtr >= (fileBuf+fileTotalSize))
					//						{
					//							endOfFile = TRUE;
					//						}
					//					}
					//					if(TRUE != endOfFile)
					//					{
					//						sscanf(curTempBufPtr,"%d,",&tempID);
					//					}
//					DEBUG_LOG("339 buf %s tempPortIndex %d tempID %d\n",curTempBufPtr,tempPortIndex,tempID,4,5,6);
				}
				DEBUG_LOG("12 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
//				DEBUG_LOG("339 portIndex %d is a device,tempPortIndex %d buf:%s,assign id %d!\n",portIndex,tempPortIndex,curTempBufPtr,tempID,5,6);
////				taskDelay(10);

				DEBUG_LOG("432\n",1,2,3,4,5,6);
				if(tempPortIndex == portIndex)
				{
					DEBUG_LOG("435\n",1,2,3,4,5,6);
					tempDevicesPtr[portIndex].ID = tempID;
				}
				else
				{
					DEBUG_LOG("440\n",1,2,3,4,5,6);
					if(tempPortIndex < portIndex)
					{
						DEBUG_LOG("443\n",1,2,3,4,5,6);
						//						printf("Error: topology lost a device\n");
						while(((*curTempBufPtr)!=',')&&((*curTempBufPtr)!=']'))
						{
							curTempBufPtr++;
							if(curTempBufPtr >= (fileBuf+fileTotalSize))
							{
								endOfFile = TRUE;
								break;
							}
						}
						if((*curTempBufPtr) == ',')
						{
							curTempBufPtr++;
							if(curTempBufPtr >= (fileBuf+fileTotalSize))
							{
								endOfFile = TRUE;
								break;
							}
						}
						goto getNext;
					}
					else
					{
						DEBUG_LOG("457\n",1,2,3,4,5,6);
						//						printf("Error: configFile lost a device\n");
						continue;
					}
				}
				DEBUG_LOG("13 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
				while(((*curTempBufPtr)!=',')&&((*curTempBufPtr)!=']'))
				{
					curTempBufPtr++;
					if(curTempBufPtr >= (fileBuf+fileTotalSize))
					{
						endOfFile = TRUE;
						break;
					}
				}
				if((*curTempBufPtr) == ',')
				{
					curTempBufPtr++;
					if(curTempBufPtr >= (fileBuf+fileTotalSize))
					{
						endOfFile = TRUE;
						break;
					}
				}
				tempBufPtr = curTempBufPtr;
			}
		}
		DEBUG_LOG("14 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
		if(TRUE != endOfFile)
		{
			if((*curTempBufPtr) != ']')
			{
				//				printf("Error: configFile lost a device\n");
				continue;
			}
		}
		DEBUG_LOG("15 switchArray 0:0x%x 1:0x%x 2:0x%x!\n",switchArray[0],switchArray[1],switchArray[2],4,5,6);
		while((*curTempBufPtr) != ']')
		{
			curTempBufPtr ++;
			if(curTempBufPtr >= (fileBuf+fileTotalSize))
			{
				endOfFile = TRUE;
				break;
			}
		}
		curTempBufPtr ++;
		tempBufPtr = curTempBufPtr;
	}
	free(fileBuf);
	return returnStatus;	
}

/* functions */
STATUS rioScanDevice(UINT8 mode)
{
	int tempfd = 0;
	int returnValue = -1;
	STATUS returnStatus = OK;

	/* 如果不存在配置文件，则探测系统拓扑结构，生成配置文件 */

	DEBUG_LOG("In rapidIoDetectDevice!\n",1,2,3,4,5,6);

	/* init rio error interrupt handle,and exception handle */ 

	rioExcInit();


	//	if ((tempfd = open(CONFIG_FILE_NAME,O_RDONLY,0777)) == (STATUS)ERROR) 
	//	{
	//DEBUG_LOG("Configure File: %s does not exit !\n",CONFIG_FILE_NAME,2,3,4,5,6,);

	topologyDiscovery();
	DEBUG_LOG("--------end topologyDiscovery\n",1,2,3,4,5,6);

	while(scanFail == TRUE)
	{
#if(SWITCH_TYPE == TSI_578)
		taskDelay(600);
#endif
		DEBUG_LOG("scanFail == TRUE\n",1,2,3,4,5,6);
		scanFail = FALSE;
		detectedTimes ++;
		topologyDiscovery();
		topologyHasChanged = TRUE;
		//扫拓扑结构不超过两次
		if(detectedTimes > 2)
		{
			break;
		}
	}
	systemHasInited = TRUE;
	topologyHasDetected = TRUE;
	detectedTimes ++;
	//	}
	//	else
	//	{
	//		close(tempfd);
	//	}
	if(STORE_TOPOLOGY_TO_FILE == mode)
	{
		returnStatus = storeTopologyToFile();
		return returnStatus;
	}
	if(idHasAssigned == FALSE)
	{
		if(ASSIGN_ID_BY_FILE == mode)
		{
			setDeviceIDFromFile();
			
			if(TRUE == idDomainError)
			{
				//if the IDs in the config file is not correct,return ERROR;
				return ERROR;
			}		
		}

		if(ASSIGN_ID_ORDER == mode)
		{
			setDeviceIDInOrder();
		}

		assignEpID();
		idHasAssigned = TRUE;
	}
	
	if(TRUE == topologyHasChanged)
	{
		/* Configure the route table*/
		/* If returns '0',the route table has configed successfully,
		 * else some thing wrong has happened*/
		returnValue = topologyRouteConfigure(systemHasInited);
		topologyHasChanged = FALSE;
	}

	if(deviceNumInTopology != deviceNumInFile)
	{
		printf("The number of device in config file is %d;\n",deviceNumInFile);
		printf(" The number of device in the srio topology is %d\n",deviceNumInTopology);
		return ERROR;
	}
	else
	{
		return OK;		
	}
}

#if(SWITCH_TYPE == CPS_1848)
void rioQuadSet(UINT8 quad,UINT8 mode)
{
//	if(sysGetCpuID() == 1)
	{
		rioSwPortCfg(quad,mode);
		taskDelay(10);

		rioSwQuadPortReset(quad);
		taskDelay(10);
	}
}
STATUS setQuadMode()
{
	rioQuadSet(0,QUAD_MODE_21144);
	rioQuadSet(1,QUAD_MODE_21144);
	rioQuadSet(2,QUAD_MODE_2244);
	return OK;
}


STATUS rioSwPortCfg_1(UINT8 quad,UINT8 mode) /*quad:{0:0,1,2,3},{1:0,1,2,3},{2:0,1},{3:0,1}*/
{
	UINT32 writeData=0,readData = 0;
	UINT32 regAddr = 0xF20200;
	
	int i;
	
	if((quad > 1)&&(mode > 1))
	{
		printf("Argument Error!\n");
		return OK;
	}
	CONFIG_READ(DEF_ID,0,regAddr,&readData);
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	printf("Read data is %x  %x\n",writeData,readData);
	
	writeData = readData &(~(0x3<<(quad*2)));
	writeData |= mode << (quad*2);
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
	CONFIG_READ(DEF_ID,0,regAddr,&readData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	printf("Read data is %x  %x\n",writeData,readData);
	
//	if(quad == 2)
//	{
//		regAddr = 0x15c + 0x20*gQuadNum[2][0];
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		writeData = readData &0xfcffffff;
//		writeData |= 0x02000000;
//		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		printf("new Read data is %x  %x\n",writeData,readData);
//	}
//	for(i=0; i<5; i++)
//	{
//		if((gQuadNum[quad][i] ==2)||(gQuadNum[quad][i] ==11)||(gQuadNum[quad][i] ==7)||(gQuadNum[quad][i] ==17))
//		{
//			continue;
//		}
//		regAddr = 0x15c + 0x20*gQuadNum[quad][i];
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		writeData = readData &0xff7fffff;
//		writeData |= 0x00800000;
//		if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
//		printf("port %d new Read data is %x  %x\n",gQuadNum[quad][i],writeData,readData);
//	}
	
}


STATUS rioPortReset_1(UINT8 port)
{
	UINT32 writeData=0,readData = 0;
	UINT32 regAddr;
	
	regAddr = 0x1044 +0x40*port;
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	regAddr = 0x106c +0x40*port;
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("1\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][0];
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("2\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][1];
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("3\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][2];
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("4\n");
	regAddr = 0xFF8010 +0x100*gPortLane[port][3];
	writeData=0x00000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("5\n");
	regAddr = 0x15c +0x20*port;
	CONFIG_READ(DEF_ID,0,regAddr,&readData);
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("6\n");
	writeData=readData&0xfffffff1;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	regAddr = 0x148+0x20*port;
	writeData=0x80000000;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	regAddr = 0x140+0x20*port;
	writeData=0x00000003;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	
	regAddr = 0x144+0x20*port;
	CONFIG_READ(DEF_ID,0,regAddr,&readData);
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	printf("8\n");
	regAddr = 0xf20300;
	writeData=0x80000000|(1<<port);
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
	
	regAddr = 0x140+0x20*port;
	writeData=0x00000004;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR)
//	{
//		return ERROR;
//	}
}

STATUS rioSwQuadPortReset_1(UINT8 quad)
{
	UINT32 writeData=0,i,readData;
	UINT32 regAddr = 0xf20300;
	UINT32 cmdValue = 0x3;

	for(i=0;i<5;i++)
	{
		regAddr = 0x140+0x20*gQuadNum[quad][i];
		CONFIG_WRITE(DEF_ID,0,regAddr,&cmdValue);
		CONFIG_READ(DEF_ID,0,regAddr,&readData);
//		if(sysI2cWrite(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&cmdValue,4,1)== ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&readData,4,1) == ERROR) return ERROR;
		printf("I2c Read: 0x%x = 0x%x\n",0x140+0x20*gQuadNum[quad][i],readData);
		writeData |=(1<<gQuadNum[quad][i]) ;
	}
	writeData |=(1<<31);
	regAddr = 0xf20300;
	CONFIG_WRITE(DEF_ID,0,regAddr,&writeData);
	CONFIG_READ(DEF_ID,0,regAddr,&readData);
//	if(sysI2cWrite(0,0xff,regAddr,(UINT8*)&writeData,4,1) == ERROR) return ERROR;
//	if(sysI2cRead(0,0xff,regAddr,(UINT8*)&readData,4,1) == ERROR) return ERROR;
	
	cmdValue = 0x4;
	for(i=0;i<5;i++)
	{
		regAddr = 0x140+0x20*gQuadNum[quad][i];
		CONFIG_WRITE(DEF_ID,0,regAddr,&cmdValue);
		CONFIG_READ(DEF_ID,0,regAddr,&readData);
//		if(sysI2cWrite(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&cmdValue,4,1)== ERROR) return ERROR;
//		if(sysI2cRead(0,0xff,0x140+0x20*gQuadNum[quad][i],(UINT8*)&readData,4,1) == ERROR) return ERROR;
		printf("I2c Read: 0x%x = 0x%x\n",0x140+0x20*gQuadNum[quad][i],readData);
		taskDelay(1);
	}
//	for(i=0;i<5;i++)
//	{
//		rioPortReset(gQuadNum[quad][i]);
//		taskDelay(1);
//	}
	//printf("I2c Read: 0x%x = 0x%x\n",regAddr,readData);
	return OK;
}



void rioQuadSet_1(UINT8 quad,UINT8 mode)
{
//	if(sysGetCpuID() == 1)
	{
		rioSwPortCfg_1(quad,mode);
		taskDelay(10);

		rioSwQuadPortReset_1(quad);
		taskDelay(10);
	}
}
STATUS setQuadMode_1()
{
	rioQuadSet_1(0,QUAD_MODE_21144);
	rioQuadSet_1(1,QUAD_MODE_21144);
	rioQuadSet_1(2,QUAD_MODE_2244);
	return OK;
}




#endif 


STATUS rioScanWithTask(int mode)
{
	int tid;
	cpuset_t affinity;

	
	tid = taskCreate ("tRioScan", 200, VX_FP_TASK|VX_ALTIVEC_TASK, 0x200000,(FUNCPTR)rioScanDevice,
			mode, 0, 0, 0,0, 0, 0, 0, 0, 0); 
	if (tid == NULL) return (ERROR);

	CPUSET_ZERO (affinity);
	CPUSET_SET (affinity, 0);
	if (taskCpuAffinitySet (tid, affinity) == ERROR)
	{
		/* Either CPUs are not enabled or we are in UP mode */
		taskDelete (tid);
		return (ERROR);
	}
	
	taskActivate (tid);
	
	return OK;
}
