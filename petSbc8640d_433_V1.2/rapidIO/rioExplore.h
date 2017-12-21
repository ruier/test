/* RioExplore.h - Initialization,Enumeration .*/

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
DESCRIPTION
Initialize the RapidIO Switch of CPS1848.
 */

#ifndef __RIO_EXPLORE_H_
#define __RIO_EXPLORE_H_
/* Data Structure definations*/
#ifdef __cplusplus
extern "C" {
#endif

	

/* includes */
#include "sysRapidIO.h"

/* macroes */
#define RIO_PEF_CAR		0x10	/* [I] Processing Element Features CAR */
#define  RIO_PEF_SWITCH			0x10000000	/* [I] Switch */

/* externs */
extern struct switch_desc * switchArray[];
extern UINT32 hostCpuID;
extern struct switch_desc * switchArray[];
extern int detectedSwitchNum;
extern UINT16 detectedTimes;
extern BOOL topologyHasDetected;
extern BOOL topologyHasChanged;
extern BOOL scanFail;
extern UINT32 hostPort;
extern UINT32 hostHop;
/* type definations */

struct deviceInfo {
	char  type;        /* 'D' means it's a normal device,'S' means it's a switch,'N'means
	                      connect nothing. */
	RIO_ID ID;         /* normal device ID or switch ID,if type is 'N',then ID is '0' */
	UINT8 connPort;
	RIO_ID oldID;
	
};

struct switch_desc {
	UINT32 switch_ID;                /* Identify what Switch it is */
//	UINT32 fatherConnectPort;              /* Identify what port this switch has connected to the father switch */
/*	UINT32 deviceCount;*/
	UINT8 portNum;
	struct deviceInfo* device_IDs;	  /* TODO: the code need modified.The devices informations list witch are linked to 
	                                     the switch directly,-1 means this port i didn't connect any thing,0 means this 
	                                     port connect a switch, other value means this port connect a normal device*/
	UINT8 hopCount;                  /* Records the hops from the host to this switch */
	UINT8 * routePortsArray;         /* The Array head,which lists all the route ports in every hop*/ 
};

extern STATUS rioConfigLocalWrite(UINT32 offset,UINT32 writeData);

#if 0
struct switch_connect_info {
	int switch_first_ID;         /* The Switch which is been fined first */
	int switch_second_ID;        /* The Switch which is connected to the first switch */
	UINT8 first_port;               /* The connect port */
	UINT8 second_port;              /* The connect port */
	struct switch_connect_info * next;
};
#endif 

#ifdef __cplusplus
}
#endif
#endif /*__RIO_EXPLORE_H_*/
















