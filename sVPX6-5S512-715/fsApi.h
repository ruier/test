#ifndef __FS_API_H__
#define __FS_API_H__

#include "vxWorks.h"
#include "string.h"
#include "sysLib.h"
#include "stdlib.h"
#include "logLib.h"
#include "iosLib.h"
#include "Config.h"
#include "sysUsr.h"

#define FS_VER          "R20150925-V2.0"

#define FS_MAX_NAME_LEN    32/*JINGZHONG*/
#define FS_MAX_HASH_KEY    100
#define FS_PART_NAME_LEN   32
#define FS_CACHE_MON_POLICY
#undef FS_DISK_MULTI_PART
#define FS_MAX_FDT_CLU_CNT  10
#define FS_EXTENTED_FDT

#if 0
typedef float                      FLOAT;
typedef char                       CHAR;
typedef volatile unsigned int      VUINT32;
typedef volatile unsigned short    VUINT16;
typedef volatile unsigned char     VUINT8;
#endif
typedef struct tagUINT64_S 
{
	UINT32  lo;
	UINT32  hi;
}UINT64_S;

typedef struct tagFS_ATTR_S
{
   UINT32	   uiTime;
   UINT32  uiStartFrame;
   UINT32  uiEndFrame;
   UINT32  uiAttr;/*JINGZHONG 2014-12-17*/
   UINT64_S ullFileSize;	 /*file size*/ 
   CHAR 	aucName[FS_MAX_NAME_LEN];

}FS_ATTR_S;



/*
 *file directory table struct
 *exist in disk space
*/
typedef struct tagFS_FDT_S 
{
  UINT8		 ucFileStat;    /*file states*/
  UINT8	     ucAttr;		/* mapped below */
  UINT8	     aucResv[1];
  UINT8      ucNameLen;
  UINT8      aucName[FS_MAX_NAME_LEN];
  UINT8  	 ucUpdateTime[2];
                         	/* The date field is encoded as follows */
                         	/* Bit 0-4:	Day of month (1-31)		    */
                         	/* Bit 5-8:	Month (1-12)			    */
                         	/* Bit 9-15:	Year relative to 1980	*/
  UINT8	     ucUpdateDate[2];
                           	/* time field is encoded as follows     */
                           	/* Bit 0-4:	seconds divided by 2 (0-29)	*/
                           	/* Bit 5-10:	minutes (0-59)			*/
                           	/* Bit 11-15:	hours (0-23)			*/
  UINT32  	 uiFirstClu;    /*first data cluster of the file*/
  UINT64_S	 ullFileSize;   /*file size*/
  UINT64_S   ullCyclePos;    /*current file position*/
#ifdef FS_EXTENTED_FDT
  UINT32     uiStartFrame;
  UINT32     uiEndFrame;
  UINT8	     aucResv1[444];/*452 16*/
#else
  UINT8	     aucResv1[8];/*460*/  
#endif  
} FS_FDT_S;

/*jingzhong*/
#define FSL_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define FSL_READ_REG(addr)          (*(volatile UINT32 *)(addr))

#define FSL_WRITE_BYTE(addr,data)   (*(volatile UINT8 *)(addr) = data)  
#define FSL_READ_BYTE(addr)         (*(volatile UINT8 *)(addr))

#define FS_WRITE_DWORD(addr,data)    (*(volatile UINT32 *)(addr) = data) 

extern UINT32 spcGetLinkUpPhyNum(UINT32 uiLinkRate);
extern STATUS spcStartAllPhy(UINT32 uiRateSupport);
extern STATUS spcStopAllPhy();
extern VOID  spcRemove(VOID);
extern VOID extIntInit(VOID);
extern void extPabackIntInit();
extern STATUS tempModuleInit(UINT8 busNo);
extern UINT8 ADT7483AWrite(UINT8 busNo,UINT8 devAddr,UINT8 regAddr,UINT8 data);
extern void spcVxbRegister(void);
extern VOID spcEventTask(VOID);

extern STATUS mntvol(UINT32 vuiDevId);
extern STATUS addvol(char *pucName,UINT32 uiMByte,UINT32 uiDevmap);
extern STATUS delvol(char *pucName);
/*extern STATUS formatvol(char *vpcName);*/
extern STATUS lsdir(char *vpcVolName,char *vpcDirName);
extern void lshelp(void);
extern void lsvol(void);
extern void lsdev(void);

extern UINT32 spcGetPhyRate(UINT32 uiPhyID);
extern UINT32 blkGetLinkPhyMap(void);

extern UINT32 guiPhyLinkMap;

#endif

