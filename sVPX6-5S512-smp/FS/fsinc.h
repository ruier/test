#ifndef __FS_INC_H__
#define __FS_INC_H__

#include "vxWorks.h"
#include "string.h"
#include "sysLib.h"
#include "stdlib.h"
#include "intLib.h"
#include "iv.h"
#include "semLib.h"
#include "assert.h"

#include "memPartLib.h"
#include "drv/pci/pciConfigLib.h"
#include "vmlib.h"
#include "logLib.h"
#include "iosLib.h"
#include "stat.h"
#include "dirent.h"
#include "fcntl.h"

#include "../fsApi.h"
#include "lib/listLib.h"
#include "lib/fsLib.h"
#if 0
typedef char   CHAR;
#endif
/******************************* debug related ********************************/

typedef enum tagFS_DBG_LEVEL_E
{
    FS_DBG_ERR = 1,
    FS_DBG_WARN,
    FS_DBG_INFO,
    FS_DBG_DATA,
    FS_DBG_BUTT
}FS_DBG_LEVEL_E;

#if 1
#define FS_LOG(level, X0, X1, X2, X3, X4, X5, X6)             \
{                                                             \
    if (guiFsDbgLevel >= level)                               \
    {                                                         \
        logMsg (X0, (int)X1, (int)X2, (int)X3, (int)X4, (int)X5, (int)X6); \
    }                                                         \
}
#else
#define FS_LOG(level, X0, X1, X2, X3, X4, X5, X6)
#endif

#define MIN(a,b)                     ((a)<(b) ? (a):(b))
#define MAX(a,b)                     ((a)>(b) ? (a):(b))

/******************************************************************************/
#define	FIOATTRIBGET		   72
#define FS_MAX_VOL             8
#define FS_MAX_FD              128

#define FS_FD_FREE             0
#define FS_FD_USED             1

#define FILE_END               -2
 
#define FS_CACHE_ENABLE

struct tagFS_VOL_S;
typedef struct tagFS_VOL_S FS_VOL_S;

/*
 *file description struct
*/
typedef struct tagFS_FD_S
{
    FS_VOL_S       *pstVol;

    UINT32          uiOper;
    UINT32          uiDirFirstClu;   /*directory first cluster*/
    UINT32          uiFDTCluId;      /*file directory desc cluster no*/
    UINT32          uiFDTCluOffset;  /*FDT offset in the cluster*/

    char            aucFullName[FS_MAX_ABS_NAME];
    char            aucName[FS_MAX_NAME_LEN];
    UINT32          uiFirstClu;      /*file data cluster no*/
    UINT32          uiLastClu;       /*file last one cluster*/
    UINT64_S        ullFileSize;     /*file size*/ 
    UINT64_S        ullPos;          /*file position*/
	UINT64_S        ullCyclePos;     /*cycle record position*/
    UINT32          uiAttr;          /*file attribute*/
    UINT32          uiMode;          /*file open mode*/
    UINT32          uiFlag;          /*file operation flag*/
    UINT32          uiUsed;          /*file desc is used or not*/
    
    UINT32          uiCluOffset;     /*file operation position*/
    UINT32          uiPreClu;        /*sector offset*/
    UINT32          uiCurClu;        /*sector offset*/

    UINT32          uiRcvrId;        /*file attribute*/

    UINT32          uiTime;
    UINT32          uiStartFrame;
    UINT32          uiEndFrame;
} FS_FD_S;

#define FS_RECOVER_MAGIC     0x2389AA55
#define FS_RCVR_FD_SIZE      512


/*
 *file recover struct
*/
typedef struct tagFS_RECOVER_S
{
    UINT32   uiFlag;
    CHAR     aucName[FS_MAX_NAME_LEN];
    UINT32   uiFirstClu;      /*file data cluster no*/
    UINT32   uiDirFirstClu;   /*directory first cluster*/
    UINT32   uiFDTCluId;      /*file directory desc cluster no*/
    UINT32   uiFDTCluOffset;  /*FDT offset in the cluster*/
    UINT32   uiResv[20];      /*reserves for future*/
}FS_RECOVER_S;

struct tagFS_REQ_S;

typedef STATUS (*pfCluRWDone)(struct tagFS_REQ_S *pstFsReq);

/*
 *file system request struct
*/
typedef struct tagFS_REQ_S
{
    UINT32            uiFlag;        /*free or used*/
    UINT32            uiVolId;       /*volume unique id*/
    UINT32            uiClu;         /*request start cluster*/
	UINT32            uiOffset;      /*offset in cluster*/
    UINT32            uiBytes;       /*number of byte*/
    UINT8            *pucBuf;        /*buffer*/
    UINT32            uiDevId;       /*device id*/
    UINT32            uiOper;        /*read or write*/
    pfCluRWDone       pfDone;        /*request completion notify*/
    UINT32            uiStatus;      /*request completion status*/
    SEM_ID            stReqSem;      /*request wait completion*/
}FS_REQ_S;

/*
 *device volume information struct
*/
struct tagFS_VOL_S
{
    DEV_HDR       stDevHdr;                     /*must be in the first position*/
    UINT32        uiVolId;                      /*volume identify*/
    UINT32        uiDevId;                      /*point to device struct*/
    CHAR	      aucPartName[FS_MAX_NAME_LEN]; /*partition name */

    UINT32        uiFlag;                       /*record device status*/
    UINT32        uiCluByte;                    /*byte per cluster */
    UINT32        uiCluPerFat;                  /*clusters of fs table */
    UINT32        uiFatClu;                     /*cluster of fs table*/
    UINT32        uiStartClu;                   /*start cluster of volume*/
    UINT32        uiRootClu;                    /*root cluster*/
    UINT32        uiVolClu;                     /*volume cluster*/
    
    SEM_ID        stFdSem;                      /*semphore for fd*/
    SEM_ID        stVolSem;                     /*semphore for volume*/
    SEM_ID        stReqSem;                   /*semphore for request*/
    SEM_ID        stFsOpenSem;    /*semphore for lock fsOpen resource, add by dongyang140513*/

    BOOL          bMnt;                           /*volume mount or not*/ 
    
    FS_FD_S       astFd[FS_MAX_FD];            /*file description*/

    UINT32        uiMinCacheClu;              /*minimal cluster in the cache*/
    UINT32        uiMaxCacheClu;              /*maximal cluster in the cache*/
    
    CHAR         *pcCacheMgt;                 /* FS_CACHE_S pointer*/
    CHAR         *pcCacheData;                /* cache data pointer */
    UINT32        uiCacheDataLen;

    FS_REQ_S      astReq[FS_MAX_REQ_PER_VOL];  /*io request information*/
};

/*
 *file directory table information
 *exist in memory
*/
typedef struct tagFS_FDT_INFO_S 
{
  FS_FDT_S    stFDT;       /*file directory table*/
  UINT32      uiClu;       /*FDT cluster id*/
  UINT32      uiCluOffset; /*FDT offset in the cluster*/
} FS_FDT_INFO_S;

/*
 *the fat table is in the cache
*/
typedef struct tagFS_CACHE_S 
{
    BOOL      bDirty;   /* data is dirty or not*/
    UINT8    *pucBuf;   /* data of sector*/
} FS_CACHE_S;


typedef struct tagFS_FDT_FUNC_S 
{
	STATUS (*fsWriteFDT)(FS_VOL_S*,FS_FDT_S *,UINT32,UINT32);
	STATUS (*fsReadFDT)(FS_VOL_S*,FS_FDT_S *,UINT32,UINT32);
    STATUS (*fsGetFDTCnt)(FS_VOL_S *,UINT32,UINT32*);
	STATUS (*fsListFDT)(FS_VOL_S*,UINT32);
	STATUS (*fsFindFDT)(FS_VOL_S*,CHAR*,UINT32,FS_FDT_INFO_S*);
    STATUS (*fsGetModWrFDT)(FS_VOL_S*,UINT32,FS_FDT_INFO_S*);
}FS_FDT_FUNC_S;

typedef struct tagFS_ENT_FUNC_S 
{
	STATUS (*fsReadEnt)(FS_VOL_S*,UINT32,UINT32*);
	STATUS (*fsWriteEnt)(FS_VOL_S*,UINT32,UINT32,UINT32);
	STATUS (*fsGetFreeEnt)(FS_VOL_S*,UINT32*);
	STATUS (*fsClearEntChain)(FS_VOL_S*,UINT32);
	UINT32 (*fsGetLastEnt)(FS_VOL_S*,UINT32,UINT32*);
    STATUS (*fsGetModLastEnt)(FS_VOL_S *,UINT32,UINT32*);
        
}FS_ENT_FUNC_S;

typedef struct tagFS_CACHE_FUNC_S 
{
	STATUS (*fsReadFatToCache)(FS_VOL_S*);
	STATUS (*fsFreeFatCache)(FS_VOL_S*);
	STATUS (*fsCacheByteWrite)(FS_VOL_S *,UINT32,UINT32,UINT8*,UINT32);
    STATUS (*fsCacheByteRead)(FS_VOL_S *,UINT32,UINT32,UINT8*,UINT32);
    STATUS (*fsCacheCluRead)(FS_VOL_S *,UINT32,UINT8*);
    STATUS (*fsCacheCluWrite)(FS_VOL_S *,UINT32,UINT8*);
    STATUS (*fsCacheByteWrBack)(FS_VOL_S *,UINT32,UINT32,UINT8*,UINT32);
}FS_CACHE_FUNC_S;

typedef STATUS (*FS_CLU_RW_F)(FS_REQ_S *pstReq);


typedef struct tagFS_CTRL_S
{
    UINT32          uiDrvNum;
    FS_CLU_RW_F     pfCluRw;
	FS_FDT_FUNC_S   stFDTFunc;
	FS_ENT_FUNC_S   stEntFunc;
	FS_CACHE_FUNC_S stCacheFunc;

    FS_VOL_S        astVol[FS_MAX_VOL];
}FS_CTRL_S;

extern FS_CTRL_S   gstFsCtrl;
extern UINT32   guiFsDbgLevel;

extern STATUS fsInit(VOID);
extern VOID fsExit(VOID);
extern STATUS fsAddVol(FS_VOL_S *vpstVol);
extern FS_VOL_S* fsFindVolByName(CHAR *vpcName);
extern FS_VOL_S* fsAllocVol(VOID);
extern VOID fsFreeVol(FS_VOL_S *vpstVol);
extern INT32 fsRegFunc(FS_CLU_RW_F pfFunc);
extern STATUS fsMountVol(FS_VOL_S *vpstVol);
extern STATUS fsUnMountVol(FS_VOL_S *vpstVol);
extern STATUS fsCluRW(FS_VOL_S*,UINT32,UINT32,UINT32,UINT8*,UINT32);
extern STATUS fsPrintDir(CHAR *vpcVolName,CHAR *vpcDirName);

extern void fsDummp(void);

#endif


