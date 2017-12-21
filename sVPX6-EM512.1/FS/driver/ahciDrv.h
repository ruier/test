#ifndef __AHCI_DRV_H__
#define __AHCI_DRV_H__

#include <stdlib.h>
#include <string.h>
#include <vxWorks.h>
#include <errno.h>
#include <string.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <ioLib.h>
#include <drv/erf/erfLib.h>
#include <blkIo.h>
#include <drv/xbd/xbd.h>
#include <hwif/vxbus/vxBus.h>		
#include <vxBusLib.h>			
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		
#include <hwif/vxbus/hwConf.h>	
#ifndef COMPILE_BOOTROM
#include <hwif/vxbus/vxbPciLib.h>
#endif
#include <driverControl.h>
#include <memPartLib.h>
#include <drv/pci/pciConfigLib.h>
#include <assert.h>

#include "ahciSpec.h"
#include "ahciReq.h"
#include "xbdBlkDev.h"

extern UINT32 guiAhciDbgLevel; 


#define AHCI_VERSION            "ver2.0"
#define AHCI_MAX_PORT           32
#define AHCI_MAX_REQ            32
#define AHCI_MAX_PHY			8/*暂时3个盘*/
#define AHCI_MAX_BUS_NO         16
#define AHCI_RFIS_TABLE_ALIGN   256
#define AHCI_RFIS_TABLE_SIZE    256
#define AHCI_CMND_LIST_ALIGN    1024
#define AHCI_CMND_LIST_NUM      32
/*浦科特*/
#define AHCI_DEVICE_PKT_ID          0x0122  
#define AHCI_VENDOR_PKT_ID          0x1c28
/*金斯顿*/
#define AHCI_DEVICE_KST_ID          0x0010  
#define AHCI_VENDOR_KST_ID          0x2646
/*三星*/
#define AHCI_DEVICE_SAM_ID          0xa801
#define AHCI_VENDOR_SAM_ID          0x144d 

#define SAS_ATA_SECURITY_PWD        "prophet"

/******************** debug related macro begin *******************************/

typedef enum tagAHCI_DBG_LEVEL_E
{
    AHCI_DBG_ERR = 0,
    AHCI_DBG_WARN,
    AHCI_DBG_INFO,
    AHCI_DBG_DATA,
    AHCI_DBG_BUTT
}AHCI_DBG_LEVEL_E;

#define AHCI_LOG(level, X0, X1, X2, X3, X4, X5, X6)            \
{                                                             \
    if (guiAhciDbgLevel >= level)                                 \
    {                                                         \
    	logMsg (X0, X1, X2, X3, X4, X5, X6);                  \
    }                                                         \
}

/******************** debug related macro end *********************************/
#define BIG_ENDIAN
#define  _INT_MACRO

#ifdef BIG_ENDIAN
#define	AHCI_CPU_LE64(x) ((UINT64)(\
   					(((UINT64)(x) & (UINT64)0x00000000000000ffULL) << 56) | \
   					(((UINT64)(x) & (UINT64)0x000000000000ff00ULL) << 40) | \
   					(((UINT64)(x) & (UINT64)0x0000000000ff0000ULL) << 24) | \
   					(((UINT64)(x) & (UINT64)0x00000000ff000000ULL) << 8) | \
   					(((UINT64)(x) & (UINT64)0x000000ff00000000ULL) >> 8) | \
   					(((UINT64)(x) & (UINT64)0x0000ff0000000000ULL) >> 24) | \
   					(((UINT64)(x) & (UINT64)0x00ff000000000000ULL) >> 40) | \
   					(((UINT64)(x) & (UINT64)0xff00000000000000ULL) >> 56)))

#define AHCI_CPU_LE32(x)	((UINT32)(\
                    	(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
                    	(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
                    	(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
                    	(((UINT32)(x) & (UINT32)0xff000000UL) >> 24)))

#define AHCI_CPU_LE16(x)	((UINT16)(\
                    	(((UINT16)(x) & (UINT16)0x00ffUL) <<  8) | \
                    	(((UINT16)(x) & (UINT16)0xff00UL) >>  8)))

#else
#define AHCI_CPU_LE16(x)	(x)
#define AHCI_CPU_LE32(x)	(x)
#endif

#define AHCI_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = AHCI_CPU_LE32(data))  
#define AHCI_READ_REG(addr)          AHCI_CPU_LE32((*(volatile UINT32 *)(addr)))
 

typedef enum  tagAHCI_LINK_SATUS_E
{
    AHCI_LINK_DOWN = 0,
    AHCI_LINK_UP, 
}AHCI_LINK_SATUS_E;

typedef enum tagAHCI_REQ_STATUS_S
{
    AHCI_REQ_UNUSED = 0,
    AHCI_REQ_USED,
    AHCI_REQ_BUTT
}AHCI_REQ_STATUS_S;

typedef struct tagAHCI_PCI_S
{
    UINT8     ucIrq;
    
    INT32     uiBus;
    INT32     uiDev;
    INT32     uiFunc;
    
    UINT32    auiBarSize;
    ULONG     aulBarAddr;
    UINT8    *pucVirAddr; 

    VOID     *pvUpLayer;
}AHCI_PCI_S;

/*
 *allocated memory information
*/
typedef struct tagAHCI_MEM_S
{
    VOID			*pvAllocVirPtr;/*allocated memory pointer before align*/
    VOID			*pvVirPtr;
	ULONG  		     ulAllocPhyAddr;/*allocated phycial address before align*/
	UINT32 			 uiPhyAddrHi;
	UINT32			 uiPhyAddrLo;
	UINT32			 uiLen;
	UINT32			 uiAlign;      
}AHCI_MEM_S;

typedef struct tagAHCI_CMND_LIST_S
{
    UINT32    uiCmndCtrl;
    UINT32    uiTranLen;
    UINT32    uiCmndTblAddrLo;
    UINT32    uiCmndTblAddrHi;
    UINT32    uiResv[4];
}AHCI_CMND_LIST_S;

typedef struct tagAHCI_RECV_FIS_S
 {
    UINT8 dmaSetupFis[28];
    UINT8 reserved0[4];
    UINT8 pioSetupFis[20];
    UINT8 reserved1[12];
    UINT8 d2hRegisterFis[20];
    UINT8 reserved2[4];
    UINT8 setDeviceBitsFis[8];
    UINT8 unknownFis[64];
    UINT8 reserved3[96];
} AHCI_RECV_FIS_S;

typedef struct tagAHCI_PRDT_S
{
    UINT32 uiAddrLo;
    UINT32 uiAddrHi;
    UINT32 uiResv;
    UINT32 uiByteCnt;
}AHCI_PRDT_S;


typedef struct tagAHCI_CMND_TBL_S
{
    SAS_HOST_TO_DEV_S   stFis;
    UINT8               aucResv[64];
    AHCI_PRDT_S         stPrdt[16];
}AHCI_CMND_TBL_S;

typedef struct tagAHCI_PORT_S
{
    UINT32            uiStatus;
    UINT32            uiCapExt;
    AHCI_MEM_S        stRecvFisMem;
    AHCI_MEM_S        stCmndListMem;
    AHCI_MEM_S        stCmndTbl;

	AHCI_CMND_LIST_S *pstCmdList;
	AHCI_CMND_TBL_S	 *pstCmdTbl;
	AHCI_RECV_FIS_S  *pstCmdRecFis;

	UINT32			  uiQueueDepth;
}AHCI_PORT_S;

typedef struct tagAHCI_DEV_S
{
	UINT32			uiPhyNum;
    UINT32          uiLink;
    UINT32          uiDevId;
    UINT32          ui48BitLBA;
    UINT32          uiNCQMaxIO;
    UINT32          uiNCQSupport;
    UINT32          uiNCQEnable;/*ncq enable switch*/
    UINT32          uiRemovable;
    UINT32          uiRemoveEnable;
    UINT32          uiDMASupport;
    UINT32          uiDMAEnable;
    UINT32          uiMaxAddrSectors;
    UINT32          uiWrCacheEnable;
    UINT32          uiNcqTagCtrl;
    UINT8           ucCapacity[8];
    SATA_ID_DATA_S  stIdenDev;
}AHCI_DEV_S;

typedef struct tagAHCI_DRV_S
{
	VXB_DEVICE_ID    pDev;
	spinlockIsr_t    stLock;
    AHCI_PCI_S       stPci;
    AHCI_PORT_S      stPort;
    AHCI_DEV_S       stDev;
    AHCI_REQ_S       astReq[AHCI_MAX_REQ];
	LIST_HEAD_S      stReqList;
	UINT32           uiActiveNCQ;
	UINT32           uiReqNum;
    VOID            *pvRegAddr;
	UINT8           *pucRecvTbl;
    UINT8           *pucCmndList;
    UINT32           uiCmndTblFlag;
	UINT32			 uiPhyId;
}AHCI_DRV_S;


extern STATUS ahciDMAAlloc(AHCI_MEM_S *pstMem);

extern VOID ahciSetATAFeature(SATA_ID_DATA_S *,AHCI_DEV_S *);
extern STATUS ahciFormat(UINT32 uiPhyId);
extern AHCI_DRV_S   gastAhciDrv[AHCI_MAX_PHY];

#endif


