/* rioConfig.h - This file contains the topology system configuration informations.*/

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
 * Notes:rioConfigurationRead() and rioConfigurationWrite() are imported from the
 * Annex1.pdf--RapidIO Specification 2.1
 * STATUS rioConfigurationRead(
 *        UINT8      localport,
 *        UINT32     destid,
 *        UINT8      hopcount,
 *        UINT32     offset,
 *        UINT32     *readdata
 *        )
 *        
 * STATUS rioConfigurationWrite(
 *        UINT8      localport,
 *        UINT32     destid,
 *        UINT8      hopcount,
 *        UINT32     offset,
 *        UINT32     *writedata
 *        )
 *        
 * 
 */

#ifndef __RIO_CONFIG_H_
#define __RIO_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define CPS_1848    1
#define TSI_578   2
#define SWITCH_TYPE CPS_1848

//#define NUM_OF_CPUS  3    /*Define the Processors who has RapidIO port.*/
#ifdef RIO_ID_16
#define DEF_ID 0xFFFF         /* The default device ID which is used for Maintenance packets routting*/
#else
#define DEF_ID 0xFF         /* The default device ID which is used for Maintenance packets routting*/
#endif //RIO_ID_16
	
#define MAX_SWITCH_NUM 100  /* This defines the number of the switches */
#define NUM_OF_SWITCH_PORTS 18                 /* Every CPS1848 has 18 ports */
//#define NUM_OF_SWITCHES    100               /* Defined the number of switches in the topology */
#define TOPOLOGY_FILE_NAME    "/tffs0/rioTopologyConfig.txt"             /* Ths configuration file name is "config.txt" */
#define NOTHING 'N'                                  /* This is Nothing */
#define SWITCH 'S'                                   /* This is a switch */
#define DEVICE 'D'                                   /* This is a normal device */
#define LOCAL_CONFIG_PORT 1                          /* define the config port number on the CPU's RapidIO Interface */ 
#define ASSIGN_ID_DEFAULT 0
#define ASSIGN_ID_ORDER   1
#define ASSIGN_ID_BY_FILE 2
#define STORE_TOPOLOGY_TO_FILE 0x10

#define RAPIDIO_REG_BASE 0xc0000
#define RAPIDIO_ADRS   0xc0000000

#define CONFIG_READ(destId,hopCount,offset,readData)  rioConfigRead(LOCAL_CONFIG_PORT,destId,hopCount,offset,readData)
#define CONFIG_WRITE(destId,hopCount,offset,writeData)  rioConfigWrite(LOCAL_CONFIG_PORT,destId,hopCount,offset,*writeData) 
#define GET_CPU_ID()         sysGetCpuID()                            /* Get the Host CPU's device ID */

/* Set route,Route destID 'destid' to destPort for srcPort */
/* 这里需要注意，在数据手册中的路由配置时，写入的值是端口号加1 */
#if (SWITCH_TYPE == CPS_1848)
#define ROUTE_SET_P_TO_P(swid,hopCount,destid,destPort,srcPort) \
		setRoutePtoP(LOCAL_CONFIG_PORT,swid,hopCount,*destid,*destPort,*srcPort)
//		do {\
//		CONFIG_WRITE(swid,hopCount,RPSR,srcPort+1);\
//		CONFIG_WRITE(swid,hopCount,SRTECSCSR,destid); \
//		CONFIG_WRITE(swid,hopCount,SRTECPSCSR,destPort);\
//		}while(0)//tempPort? UINT32 tempPort = *srcPort + 1;	

 

#define ROUTE_SET_P_FOR_ALL(swid,hopCount,destid,destPort) \
		setRoutePforAll(LOCAL_CONFIG_PORT,swid,hopCount,*destid,*destPort)
//		do {\
//		CONFIG_WRITE(swid,hopCount,SRTECSCSR,destid); \
//		CONFIG_WRITE(swid,hopCount,SRTECPSCSR,destPort); \
//		}while(0)
#ifdef RIO_ID_16
#define ROUTE_SET_DOMAIN(swid,hopCount,domainid) \
	setSwitchDomain(LOCAL_CONFIG_PORT,swid,hopCount,*domainid)
#endif //RIO_ID_16

#endif //(SWITCH_TYPE == CPS_1848)
	
#if (SWITCH_TYPE == TSI_578)
#define ROUTE_SET_P_TO_P(swid,hopCount,destid,destPort,srcPort) do {\
	CONFIG_WRITE(swid,hopCount,RPSR,srcPort); \
	CONFIG_WRITE(swid,hopCount,SRTECSCSR,destid); \
	CONFIG_WRITE(swid,hopCount,SRTECPSCSR,destPort);\
	}while(0)//tempPort? UINT32 tempPort = *srcPort + 1;\

#define ROUTE_SET_P_FOR_ALL(swid,hopCount,destid,destPort) do { \
	CONFIG_WRITE(swid,hopCount,SRTECSCSR,destid); \
	CONFIG_WRITE(swid,hopCount,SRTECPSCSR,destPort); \
	}while(0)
#endif //(SWITCH_TYPE == TSI_578)
	
#define SHOW_DEBUG_LOG 
#undef SHOW_DEBUG_LOG
#define EXPLORE_DEBUG
//#undef EXPLORE_DEBUG	
#define ASSIGN_ID_DEBUG
//#undef ASSIGN_ID_DEBUG
#define SET_ROUTE_DEBUG	

#ifdef SHOW_DEBUG_LOG
extern int     ns16550Pollprintf(char* pChar);
char sprintBuf_rio[100];
#define DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6) {sprintf(sprintBuf_rio,fmt,a1,a2,a3,a4,a5,a6);\
	ns16550Pollprintf(sprintBuf_rio);}
#else
#define DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#endif

#ifdef EXPLORE_DEBUG	
#define EXPLORE_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6)  DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#else
#define EXPLORE_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6) 
#endif

#ifdef ASSIGN_ID_DEBUG	
#define ASSIGN_ID_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6)  DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#else
#define ASSIGN_ID_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6) 
#endif

#ifdef SET_ROUTE_DEBUG
#define  ROUTE_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6)  DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#else
#define  ROUTE_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#endif


#ifdef __cplusplus
}
#endif
#endif /*__RIO_CONFIG_H_ */
