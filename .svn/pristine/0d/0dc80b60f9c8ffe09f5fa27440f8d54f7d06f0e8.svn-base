#ifndef __BLK_INC_H__
#define __BLK_INC_H__

#include "../fsInc.h"

#define BLK_MAX_DISK             8
#define BLK_MAX_DEV              16
#define BLK_MAX_REQ              32/*jingzhong 2014-12-22空间开大，原始为32M*/
#define BLK_INVALID_PHY          0xff

#define BLK_PART_SEC_ID          0x2
#define BLK_PART_BYTE            512
#define BLK_PART_FLAG            0x5555aaaa


/*CLU SIZE = STRIPE_SIZE * member number */

#define BLK_STRIPE_SIZE          (1 * 1024 * 1024)

#define BLK_GET_SEC_HI(clu)      (clu >> (32 - 11))
#define BLK_GET_SEC_LO(clu)      (clu << 11)

#define BLK_BYTE_PER_SEC         512

#define BLK_GET_MBYTE(cap,sec) \
     do \
     { \
        cap = sec.lo >> 11;  \
        cap |= sec.hi << 20; \
     }while(0)
     
typedef enum tagBLK_DEV_TYPE_E
{
    BLK_TYPE_DISK = 1,
    BLK_TYPE_RAID,
    BLK_TYPE_BUTT
}BLK_DEV_TYPE_E;

typedef enum tagBLK_DEV_STATUS_E
{
    BLK_STATUS_FREE = 0,
    BLK_DEV_NOT_READY,
    BLK_DEV_WAIT_MOUNT,
    BLK_STATUS_READY,
    BLK_STATUS_BUTT
}BLK_DEV_STATUS_E;

typedef enum tagBLK_REQ_STATUS_E
{
    BLK_REQ_FREE = 0,
    BLK_REQ_USED,
    BLK_REQ_BUTT
}BLK_REQ_STATUS_E;

#define BLK_RAID_MAX_NUM     8
#define BLK_RAID_INVALID_ID  0xff

typedef struct tagBLK_RAID_S
{
    UINT32   uiMemNum;                /*number of member*/
    UINT32   uiStripSec;              /*sector per stripe*/
    UINT8    aucMem[BLK_RAID_MAX_NUM];/*member physical id of raid*/
}BLK_RAID_S;

typedef struct tagBLK_DEV_S
{
    UINT32     uiDevNo;                     /*device no*/
    UINT32     uiPhyNum;                    /*phy number of device*/
    UINT32     uiStatus;                    /*device status connect or not*/
    UINT32     uiDevType;                   /*device type*/

    UINT32     uiBytePerSec;                /*bytes of per sector*/
    UINT64_S   ullSec;                      /*total sectors of the disk*/

    BLK_RAID_S stRaid;                      /*raid information*/
    SEM_ID     semDevSem;                   /*file semaphore for sync*/
}BLK_DEV_S;


struct tagBLK_REQ_S;

typedef STATUS (*pfSecRwDone)(struct tagBLK_REQ_S *pstFsReq);


typedef struct tagBLK_REQ_S
{
    UINT32            uiFlag;        /*free or used*/
    UINT32            uiDevId;       /*device unique id*/
    UINT32            uiPhyId;       /*physcial position*/
    UINT32            uiSecHi;       /*request start block high*/
    UINT32            uiSecLo;       /*request start block high*/
        
    UINT32            uiByte;        /*request byte*/
    UINT32            uiSecOff;      /*offset in sector*/
    
    UINT32            uiSecNum;      /*number of block*/
    UINT8            *pucBuf;        /*request buffer*/
    UINT8            *pucAlloc;      /*allocated buffer*/
    UINT8            *pucOrgBuf;     /*pointer for read/write byte buffer*/
    
    UINT32            uiOper;        /*read or write*/
    pfSecRwDone       pfDone;        /*request completion notify*/
    UINT32            uiStatus;      /*request completion status*/
    SEM_ID            stReqSem;      /*request wait completion*/

    /*
     *raid request member 
    */
    UINT32                uiReqNum;
    UINT32                uiDoneNum;
    struct tagBLK_REQ_S  *pstOrgReq;
    struct tagBLK_REQ_S  *apstSubReq[BLK_RAID_MAX_NUM];
        
    FS_REQ_S         *pstReq;
}BLK_REQ_S;

typedef struct tagBLK_SPLIT_REQ_S
{
    UINT32            uiDevId;       /*device unique id*/
    UINT32            uiSecHi;       /*request start block high*/
    UINT32            uiSecLo;       /*request start block high*/

    UINT32            uiSecNum;
    UINT32            uiByte;
    UINT8            *pucBuf;
}BLK_SPLIT_REQ_S;

typedef STATUS (*BLK_SEC_RW_F)(BLK_REQ_S *pstReq);
typedef STATUS (*BLK_FORMAT_F)(UINT32 uiPhyId);

typedef struct tagBLK_CTRL_S
{
    BOOL            bTaskRun;

    BLK_SEC_RW_F    pfSecRw;
    BLK_FORMAT_F    pfFormat;

    SEM_ID          stBlkMem;
	spinlockIsr_t   stLock;
    BLK_DEV_S       astDev[BLK_MAX_DEV];
    BLK_REQ_S       astReq[BLK_MAX_REQ];
}BLK_CTRL_S;


extern STATUS blkDevAdd(UINT64_S vullSec,UINT32  vuiBytePerSec,UINT32 vuiDevId);
extern INT32 blkRegFunc(BLK_SEC_RW_F pfFunc,BLK_FORMAT_F pfFormat);
extern STATUS blkInit(VOID);

typedef struct tagBLK_PART_S
{
    UINT32    uiFlag;
    UINT8     aucName[FS_MAX_NAME_LEN];
    UINT32    uiType;
    UINT32    uiMemNum;
    UINT8     aucMem[BLK_RAID_MAX_NUM]; /*0 ~ 7:physcial 8~ virsual device*/
    UINT32    uiStartClu;
    UINT32    uiEndClu;
    UINT32    uiCluLen;
}BLK_PART_S;

#endif

