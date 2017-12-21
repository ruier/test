/*
 *  nvmeinc.h - nvme driver definitions and declarations
 */

#ifndef _NVME_INC_H
#define _NVME_INC_H

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
#include <hwif/vxbus/vxbPciLib.h>
#include <driverControl.h>
#include <memPartLib.h>
#include <drv/pci/pciConfigLib.h>
#include <assert.h>
#include "../blk/blkInc.h"

#define NVME_DRV_NAME	    "nvme"
#define NVME_DRV_VERSION    "R20160723"

#define NVME_Q_DEPTH        64
#define SQ_SIZE(depth)		(depth * sizeof(NVME_CMND_S))
#define CQ_SIZE(depth)		(depth * sizeof(NVME_CMPL_S))

#define CHAR            char

#if 0
#define INT32           int
#define STATUS          int
#define UINT32          unsigned int
#define LONG            long
#define ULONG           unsigned long
#define UINT64          unsigned long long
#define UINT8           unsigned char
#define UINT16          unsigned short
#define BOOL            bool
#define FLOAT           float
#define VOID            void
#define OK              (0)
#define ERROR           (-1)
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif
/*
typedef volatile unsigned int      VUINT32;
typedef volatile unsigned short    VUINT16;
*/


typedef enum tagNVME_DBG_LEVEL_E
{
    NVME_DBG_ERR = 0,
    NVME_DBG_WARN,
    NVME_DBG_INFO,
    NVME_DBG_DATA,
    NVME_DBG_DBG,
    NVME_DBG_BUTT
}NVME_DBG_LEVEL_E;

extern UINT32  guiNvmeDbgLevel;

#define NVME_LOG(level,X2, X3, X4, X5, X6)                          \
{                                                                   \
    if (guiNvmeDbgLevel >= level)                                   \
    {                                                               \
        logMsg ("%s:%d "X2,__FUNCTION__,__LINE__,X3, X4, X5, X6); \
    }                                                               \
}

#define BIG_ENDIAN

#ifdef BIG_ENDIAN
#define NVME_SWAP32(x)   ((UINT32)(\
                    	(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
                    	(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
                    	(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
                    	(((UINT32)(x) & (UINT32)0xff000000UL) >> 24)))
#define NVME_SWAP64(x)  ((UINT64)(\
    					(((UINT64)(x) & (UINT64)0x00000000000000ffULL) << 56) | \
    					(((UINT64)(x) & (UINT64)0x000000000000ff00ULL) << 40) | \
    					(((UINT64)(x) & (UINT64)0x0000000000ff0000ULL) << 24) | \
    					(((UINT64)(x) & (UINT64)0x00000000ff000000ULL) << 8) | \
    					(((UINT64)(x) & (UINT64)0x000000ff00000000ULL) >> 8) | \
    					(((UINT64)(x) & (UINT64)0x0000ff0000000000ULL) >> 24) | \
    					(((UINT64)(x) & (UINT64)0x00ff000000000000ULL) >> 40) | \
    					(((UINT64)(x) & (UINT64)0xff00000000000000ULL) >> 56)))	
#define NVME_SWAP16(x)   ((((x) & 0xFF) << 8)|(((x) >> 8) & 0xFF))
#else
#define NVME_SWAP32(x)   (x)
#define NVME_SWAP64(x)   (x)
#define NVME_SWAP16(x)   (x)
#endif

#define MIN(a,b)                     ((a)<(b) ? (a):(b))
#define MAX(a,b)                     ((a)>(b) ? (a):(b))

#define NVME_SECTOR_SIZE	512


static UINT32 NVME_READ_DWORD(const volatile void  *addr)
{ 
	return NVME_SWAP32(*(const volatile UINT32 *) addr);
}

static UINT64 NVME_READ_QWORD(const volatile void  *addr)
{
	const volatile UINT32  *p = addr;
	UINT32 low, high;
	UINT64 ullVal = 0;

	high = NVME_READ_DWORD(p + 1);
	low = NVME_READ_DWORD(p);
	ullVal = (UINT64)high;
	ullVal = (ullVal << 32) | low;
	
	return ullVal;
}

static void NVME_WRITE_DWORD(volatile void *addr,UINT32 val)
{
	*(volatile UINT32 *) addr = NVME_SWAP32(val);
}

static void NVME_WRITE_QWORD(void  *reg,UINT64 val)
{
	NVME_WRITE_DWORD(reg,val & 0xffffffff);
	NVME_WRITE_DWORD((void*)((ULONG)reg + 4),val >> 32);
}

#include "nvmeSpec.h"


/*
 * Represents an NVM Express device.  Each nvme_dev is a PCI function.
 */
typedef struct tagNVME_DEV_S 
{
	UINT32         *puiDbs;			/*doorbell base address*/
	NVME_BAR_S     *pstBar;			/*bar address */
	VOID           *pvPciDev;
	/*struct msix_entry *entry;*/
	CHAR            acName[12];
	CHAR            acSerial[20];
	CHAR            acModel[40];
	CHAR            acFirmwareRev[8];
	UINT32          uiMPXMAX;
	UINT32          uiMaxSec;
	UINT32          uiStripeSize;
	UINT32          uiDbStripe;
	UINT32			uiPhyNum;
    UINT32          uiLink;
    UINT32          uiDevId;
    UINT32          ui48BitLBA;
    UINT32          uiNCQMaxIO;
    UINT32          uiNCQSupport;
    UINT32          uiNCQEnable;
    UINT32          uiRemovable;
    UINT32          uiRemoveEnable;
    UINT32          uiDMASupport;
    UINT32          uiDMAEnable;
    UINT32          uiMaxAddrSectors;
    UINT32          uiWrCacheEnable;
    UINT32          uiNcqTagCtrl;
    UINT64          ullCapacity;
    
}NVME_DEV_S;

#define NVME_VS(major, minor)	(major << 16 | minor)

#define NVME_DOORBELL_OFFSET    0x1000

#define NVME_MAX_PHY            16
#define NVME_MAX_REQ            32  /*cannot exceed 32*/

#define NVME_AQA_ASQS_MASK 0
#define NVME_AQA_ACQS_MASK 16
#define NVME_ADMIN_QID 0x0

typedef enum tagNVME_REQ_STATUS_S
{
    NVME_REQ_UNUSED = 0,
    NVME_REQ_USED,
    NVME_REQ_BUTT
}NVME_REQ_STATUS_S;


typedef enum tagPCIE_MPS_SIZE_E
{
	PCIE_MPS_128_BYTE = 0,
	PCIE_MPS_256_BYTE,
	PCIE_MPS_512_BYTE,
	PCIE_MPS_1024_BYTE,
	PCIE_MPS_2048_BYTE,
	PCIE_MPS_4096_BYTE,
}PCIE_MPS_SIZE_E;

struct tagNVME_CMD_INFO;
typedef struct tagNVME_CMD_INFO NVME_CMD_INFO;

typedef struct tagNVME_PCI_S
{
    UINT32    uiIrq;    
    INT32     uiBus;
    INT32     uiDev;
    INT32     uiFunc;	
    ULONG     ulBaseAddr;
	UINT32    uiBarSize;
    UINT8    *pucVirAddr; 	/*pcie bar 0 base address(virtual)*/
	VOID     *pvUpLayer;
}NVME_PCI_S;

typedef struct tagNVME_MEM_S
{
    VOID			*pvAllocVirPtr;/*allocated memory pointer before align*/
    VOID			*pvVirPtr;
	ULONG  		     ulAllocPhyAddr;/*allocated phycial address before align*/
	ULONG			 ulPhyAddr;
	UINT32			 uiLen;
	UINT32			 uiAlign;      
}NVME_MEM_S;

/*
 * An NVM Express queue.  Each device has at least two (one for admin
 * commands and one for I/O commands).
 */
typedef struct tagNVME_QUEUE_S
{
	spinlockIsr_t                   stLock;
	NVME_DEV_S					   *pstNvmeDev;
	
	NVME_MEM_S 	                    stCQMem;
	NVME_CMND_S                    *pstCmnd;

	NVME_MEM_S 	                    stSQMem;
	NVME_CMPL_S                    *pstCqes;

	VOID                           *pstPrpList;
	NVME_MEM_S 	                    stPrpListMem;
			   
	UINT32                         *puiQdb;
	UINT32                          uiQueueID;
	UINT16                          usQdepth;
	UINT16                          usCqVector;
	UINT16                          usSqHead;
	UINT16                          usSqTail;
	UINT16                          usCqHead;
	UINT8                           ucCqPhase;
	UINT8                           ucCqeSeen;
	UINT8                           ucQSuspended;
	UINT32                          uiCmdIdFlag;
	NVME_CMD_INFO				   *pstCmdInfo;
	
}NVME_QUEUE_S;

typedef VOID (*pfNvmeBlkResp)(VOID *pstReq);


typedef struct tagNVME_REQ_S
{
    UINT32            uiStatus;
    UINT8             ucDirection;
    UINT8             ucReqType;
    UINT8             ucTag;
    UINT8             ucSGLNum;

	UINT64            ullLBA;
	UINT64            ullAddr;
	UINT32            uiSecNum;
	
    UINT8            *pucBuf;

	pfNvmeBlkResp     pfDone;
    BLK_REQ_S        *pstBlkReq;
    VOID             *pvDrv;
}NVME_REQ_S;

typedef enum  tagNVME_LINK_SATUS_E
{
    NVME_LINK_DOWN = 0,
    NVME_LINK_UP, 
}NVME_LINK_SATUS_E;


typedef struct tagNVME_DRV_S
{
    NVME_PCI_S       stPci;
    NVME_DEV_S       stDev;
    NVME_QUEUE_S     stAdminQueue;
    NVME_QUEUE_S     stIOQueue;
    NVME_REQ_S       astReq[NVME_MAX_REQ];
    VOID            *pvRegAddr;		/*nvme bar base addr*/
	UINT32			 uiPhyId;
	UINT32           uiStatus;
	spinlockIsr_t    stLock;
}NVME_DRV_S;

#endif

