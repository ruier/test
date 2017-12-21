#ifndef __AHCI_BLK_H__
#define __AHCI_BLK_H__


#include "../blk/blkInc.h"
#include "ahciSpec.h"

#define AHCI_SECTOR_SIZE	512
#define AHCI_DATA_IN        1
#define AHCI_DATA_OUT       2

typedef VOID (*pfBlkResp)(VOID *pstReq);

typedef struct tagAHCI_SGL_S
{
    UINT32  uiSGLLo;
    UINT32  uiSGLHi;
    UINT32  uiLen;
}AHCI_SGL_S;


typedef struct tagAHCI_REQ_S
{
    SAS_HOST_TO_DEV_S stFis;

    UINT32            uiStatus;
    UINT8             ucDirection;
    UINT8             ucReqType;
    UINT8             ucTag;
    UINT8             aucLBA[6];
    UINT8             ucSGLNum;

    AHCI_SGL_S        stSGL;
    UINT8            *pucBuf;

    VOID             *pvFsReq;
    VOID             *pvDrv;
	pfBlkResp         pfDone;
    BLK_REQ_S        *pstBlkReq;
    
}AHCI_REQ_S;

extern STATUS ahciRWSector(BLK_REQ_S  *pstBlkReq);

#endif

