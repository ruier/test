/* rioPrivate.c - RapidIO common read and Write functions .*/

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
05/06/2013,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Initialize the RapidIO Switch.
 * 
 */

/* includes */
#include<vxworks.h>
#include "rioConfig.h"
#include "rioPrivate.h"



/*****************************************************************************
 * rioConfigLocalRead - m85xx RapidIO read the local config register
 */
STATUS rioConfigLocalRead(UINT32 offset,UINT32 *readData)
{
	*readData=m85xxRioRegRead(offset);
	return OK;
}

/*****************************************************************************
 * rioConfigLocalWrite - m85xx RapidIO write a value to the local config register
 */
STATUS rioConfigLocalWrite(UINT32 offset,UINT32 writeData)
{
	m85xxRioRegWrite(offset,writeData);
	return OK;
}

/*****************************************************************************
 * rioConfigRead - m85xx RapidIO read the value in the remote rapidIO config 
 * register.
 */
STATUS rioConfigRead(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 *readData)
{
	if(localPort==0) localPort=1;
	if(localPort>3) localPort=3;
	if(offset < 0x100000)
	{
#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
		//	DEBUG_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
		*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);

		if(rioReadState == 1)
		{
			rioReadState = 0;
			return ERROR;
		}
	}
	else
	{
#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077017);/*16M的空间*/
		//	DEBUG_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
		*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);

		if(rioReadState == 1)
		{
			rioReadState = 0;
			return ERROR;
		}
	}	

	//DEBUG_LOG("exit rioConfigRead\n",1,2,3,4,5,6);
	return OK;

	//	UINT32 time = CONFIG_READ_TIMEOUT;
	//	
	//	if(localPort==0)
	//	{
	//		localPort=1;
	//	}
	//	
	//	if(localPort>3)
	//	{
	//		localPort=3;
	//	}
	//	
	//	while(time)
	//	{
	//		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
	//		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	//		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
	//		//RIO_DEEP_LOG("enter rioConfigRead\n",1,2,3,4,5,6);
	//		*readData=*(UINT32*)(RIO_LOCAL_ADDR+offset);
	//		if(rioReadState == 0)
	//		{
	//			break;
	//		}
	//
	//		time --;
	//	}
	//	rioReadState = 0;
	//	if(time == 0)
	//	{
	//		ns16550Pollprintf("rioConfigRead timeout \n");
	//		//RIO_DEEP_LOG("exit  rioConfigRead   tfttttttimeout\n",1,2,3,4,5,6);
	//	}
	//
	//	return OK;

}

/*****************************************************************************
 * rioConfigWrite - m85xx RapidIO write a value to the remote rapidIO configuration 
 * register.
 */
STATUS rioConfigWrite(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
{
	UINT32 data,timeOut=3;

	if(localPort==0) localPort=1;
	if(localPort>3) localPort=3;
	if(offset < 0x100000)
	{

#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
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
	else
	{
#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077017);/*16M的空间*/



		while(timeOut--)
		{
			*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
			data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
			if(data == writeData) break;
		}
		if(timeOut) return OK;
		else return ERROR;	
	}
}

STATUS rioConfigWriteOne(UINT8 localPort,UINT16 destId,UINT8 hopCount,UINT32 offset,UINT32 writeData)
{
	UINT32 data;
	if(offset < 0x100000)
	{
#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
		*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
		data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
	}
	else
	{
#ifdef RIO_ID_16
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
		m85xxRioRegWrite((0x10c04+0x20*localPort),(destId >> 10));
#else
		m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (offset >> 20)));
#endif
		//	m85xxRioRegWrite((0x10c00+0x20*localPort),((destId << 22) | (hopCount << 12) | (0 >> 20)));
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
		m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077017);/*16M的空间*/
		*(UINT32*)(RIO_LOCAL_ADDR+offset) = writeData;
		data = *(UINT32*)(RIO_LOCAL_ADDR+offset);
	}

	return OK;
}

///******************************************************************************
// * setRoutePtoP - set rapidIo route,from srcPort to destPort for the packets 
// * whose Id is destId. 
// * localPort: the local config port
// * swId: the dest switch's access id
// * hopCount: the remote switch's hopCount 
// * srcPort: the port what packets comes from
// * destPort: the packets' route port
// * destId:  the packets' id
// */
//STATUS setRoutePtoP(UINT8 localPort,RIO_ID swId,UINT8 hopCount,RIO_ID destId,\
//		UINT8 destPort,UINT8 srcPort)
//{
//	UINT32 data;
//	UINT32 domain_id = 0;
//
//	CONFIG_READ(swId,hopCount,RDR,&domain_id);
//#ifdef RIO_ID_16
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c04+0x20*localPort),(swId >> 10));
//#else
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
//#endif
//	//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12)\
//	//			| (0 >> 20)));
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
//
//#if (SWITCH_TYPE == TSI_578)
//	*(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_DESTID + 0x100*srcPort) = destId;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_DESTID + 0x100*srcPort);
//	*(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_PORT + 0x100*srcPort) = destPort;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_PORT + 0x100*srcPort);
//#endif //(SWITCH_TYPE == TSI_578)
//	//#if (SWITCH_TYPE == TSI_578)
//	//	*(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_DESTID + 0x100*srcPort*/0x70) = destId;
//	//	data = *(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_DESTID + 0x100*srcPort*/0x70);
//	//	*(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_PORT + 0x100*srcPort*/0x74) = destPort;
//	//	data = *(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_PORT + 0x100*srcPort*/0x74);
//	//#endif //(SWITCH_TYPE == TSI_578)
//
//#if (SWITCH_TYPE == CPS_1848)
//#ifdef RIO_ID_16
//	if(destId == DEF_ID)
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = srcPort+1;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);
//	}
//	else
//	{
//		if(((destId&0xff00) == 0)||((destId&0xff00) == 0xdd00)||((domain_id&0xff) == ((destId&0xff00)>>8)))
//		{
//			*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = srcPort+1;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId&0x00ff;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);
//		}
//		else
//		{
//			*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = srcPort+1;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId&0xff00;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);
//		}
//	}
//#else
//	*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = srcPort+1;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//	*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//	*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);
//#endif //RIO_ID_16
//#endif //(SWITCH_TYPE == CPS_1848)
//}
//
//
///******************************************************************************
// * setRoutePforAll - set rapidIo route,set rapidIo route,for all ports, if the 
// * packets' id is destId, than route the packets to destPort. 
// * localPort: the local config port
// * swId: the dest switch's access id
// * hopCount: the remote switch's hopCount 
// * destPort: the packets' route port
// * destId:  the packets' id
// */
//STATUS setRoutePforAll(UINT8 localPort,RIO_ID swId,UINT8 hopCount,RIO_ID destId,\
//		UINT8 destPort)
//{
//	UINT32 data;
//	UINT32 domain_id = 0;
//
//	CONFIG_READ(swId,hopCount,RDR,&domain_id);
//#ifdef RIO_ID_16
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c04+0x20*localPort),(swId >> 10));
//#else
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
//#endif
//	//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
//	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
//
//#if (SWITCH_TYPE == TSI_578)
//	*(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_DESTID) = destId;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_DESTID);
//	*(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_PORT) = destPort;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_PORT );
//#endif //(SWITCH_TYPE == TSI_578)
//
//#if (SWITCH_TYPE == CPS_1848)
////		data = *(UINT32*)(RIO_LOCAL_ADDR+RDR);
//
//#ifdef RIO_ID_16
//	if(destId == DEF_ID)
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;//&0x00ff;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//	}
//	else
//	{
//		if(((destId&0xff00) == 0)||((destId&0xff00) == 0xdd))//||((domain_id&0xff) == ((destId&0xff00)>>8)))
//		{
//			*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;//&0x00ff;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//		}
//		else
//		{
//			*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;//&0xff00;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//			*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//			data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//		}
//	}
//#else
//	if(((destId&0xff00) == 0)||((destId&0xff00) == 0xdd))
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//	}
//	else
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//	}
//#endif   //RIO_ID_16
//
//#endif //(SWITCH_TYPE == CPS_1848)
//}



/******************************************************************************
 * setRoutePtoP - set rapidIo route,from srcPort to destPort for the packets 
 * whose Id is destId. 
 * localPort: the local config port
 * swId: the dest switch's access id
 * hopCount: the remote switch's hopCount 
 * srcPort: the port what packets comes from
 * destPort: the packets' route port
 * destId:  the packets' id
 */
STATUS setRoutePtoP(UINT8 localPort,RIO_ID swId,UINT8 hopCount,RIO_ID destId,\
		UINT8 destPort,UINT8 srcPort)
{
	UINT32 data;

#ifdef RIO_ID_16
	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c04+0x20*localPort),(swId >> 10));
#else
	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
#endif
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12)\
//			| (0 >> 20)));
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
	
#if (SWITCH_TYPE == TSI_578)
	*(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_DESTID + 0x100*srcPort) = destId;
	data = *(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_DESTID + 0x100*srcPort);
	*(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_PORT + 0x100*srcPort) = destPort;
	data = *(UINT32*)(RIO_LOCAL_ADDR+SPBC_ROUTE_CFG_PORT + 0x100*srcPort);
#endif //(SWITCH_TYPE == TSI_578)
//#if (SWITCH_TYPE == TSI_578)
//	*(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_DESTID + 0x100*srcPort*/0x70) = destId;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_DESTID + 0x100*srcPort*/0x70);
//	*(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_PORT + 0x100*srcPort*/0x74) = destPort;
//	data = *(UINT32*)(RIO_LOCAL_ADDR+/*SPBC_ROUTE_CFG_PORT + 0x100*srcPort*/0x74);
//#endif //(SWITCH_TYPE == TSI_578)
	
#if (SWITCH_TYPE == CPS_1848)
	*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = srcPort+1;
	data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
	*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
	data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
	*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
	data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);
#endif //(SWITCH_TYPE == CPS_1848)
}


/******************************************************************************
 * setRoutePforAll - set rapidIo route,set rapidIo route,for all ports, if the 
 * packets' id is destId, than route the packets to destPort. 
 * localPort: the local config port
 * swId: the dest switch's access id
 * hopCount: the remote switch's hopCount 
 * destPort: the packets' route port
 * destId:  the packets' id
 */
STATUS setRoutePforAll(UINT8 localPort,RIO_ID swId,UINT8 hopCount,RIO_ID destId,\
		UINT8 destPort)
{
	UINT32 data;

#ifdef RIO_ID_16
	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c04+0x20*localPort),(swId >> 10));
#else
	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
#endif
//	m85xxRioRegWrite((0x10c00+0x20*localPort),((swId << 22) | (hopCount << 12) | (0 >> 20)));
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x08),(RIO_LOCAL_ADDR >> 12))  ;
	m85xxRioRegWrite((0x10c00+0x20*localPort+0x10),0x80077013);/*1M的空间*/
	
#if (SWITCH_TYPE == TSI_578)
	*(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_DESTID) = destId;
	data = *(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_DESTID);
	*(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_PORT) = destPort;
	data = *(UINT32*)(RIO_LOCAL_ADDR+RIO_ROUTE_CFG_PORT );
#endif //(SWITCH_TYPE == TSI_578)
	
#if (SWITCH_TYPE == CPS_1848)
//	if(((destId&0xff00) == 0)||((destId&0xff00) == 0xdd))
//	{
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
//		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
//		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//	}
//	else
//	{
		*(UINT32*)(RIO_LOCAL_ADDR+RPSR) = 0x0;
		data = *(UINT32*)(RIO_LOCAL_ADDR+RPSR);
		*(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR) = destId;//&0xff00;
		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECSCSR);
		*(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR) = destPort;
		data = *(UINT32*)(RIO_LOCAL_ADDR+SRTECPSCSR);	
//	}

#endif //(SWITCH_TYPE == CPS_1848)
}

