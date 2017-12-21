/*
 * Definitions for the NVM Express interface
 */

#ifndef __FSATAINC_H__
#define __FSATAINC_H__

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

/******************** debug related macro begin *******************************/

typedef enum tagFSATA_DBG_LEVEL_E
{
    FSATA_DBG_ERR = 0,
    FSATA_DBG_WARN,
    FSATA_DBG_INFO,
    FSATA_DBG_DATA,
    FSATA_DBG_BUTT
}FSATA_DBG_LEVEL_E;

extern UINT32  guiFSataDbgLevel;

#define FSATA_LOG(level,X2, X3, X4, X5, X6)                          \
{                                                                   \
    if (guiFSataDbgLevel >= level)                                   \
    {                                                               \
        logMsg ("%s:%d "X2,__FUNCTION__,__LINE__,X3, X4, X5, X6); \
    }                                                               \
}

#define FSATA_VERSION   "SATA-DRV-1.0"
#define FSATA_BASE      0x220000
/******************** debug related macro end *********************************/

#define BIG_ENDIAN

#ifdef BIG_ENDIAN
#define FSATA_CPU_LE32(x)	((UINT32)(\
                    	(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
                    	(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
                    	(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
                    	(((UINT32)(x) & (UINT32)0xff000000UL) >> 24)))

#define FSATA_CPU_LE16(x)	((UINT16)(\
                    	(((UINT16)(x) & (UINT16)0x00ffUL) <<  8) | \
                    	(((UINT16)(x) & (UINT16)0xff00UL) >>  8)))               
#define FSATA_CPU_LE64(x)  ((UINT64)(\
    					(((UINT64)(x) & (UINT64)0x00000000000000ffULL) << 56) | \
    					(((UINT64)(x) & (UINT64)0x000000000000ff00ULL) << 40) | \
    					(((UINT64)(x) & (UINT64)0x0000000000ff0000ULL) << 24) | \
    					(((UINT64)(x) & (UINT64)0x00000000ff000000ULL) << 8) | \
    					(((UINT64)(x) & (UINT64)0x000000ff00000000ULL) >> 8) | \
    					(((UINT64)(x) & (UINT64)0x0000ff0000000000ULL) >> 24) | \
    					(((UINT64)(x) & (UINT64)0x00ff000000000000ULL) >> 40) | \
    					(((UINT64)(x) & (UINT64)0xff00000000000000ULL) >> 56)))
#else
#define FSATA_CPU_LE16(x)	(x)
#define FSATA_CPU_LE32(x)	(x)
#endif

#define FSATA_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = FSATA_CPU_LE32(data))  
#define FSATA_READ_REG(addr)          FSATA_CPU_LE32((*(volatile UINT32 *)(addr)))

#define FSATA_WRITE_WORD(addr,data)    (*(volatile UINT16 *)(addr) = (data))  
#define FSATA_READ_WORD(addr)          ((*(volatile UINT16 *)(addr)))

#define FSAS_ATA_FIS_H2D               0x27
#define FSAS_ATA_FIS_D2H               0x34
#define FSAS_ATA_FIS_SDB               0xa1
#define FSAS_ATA_FIS_DMA_ACTIVE        0x39

enum
{
    FATA_CMD_PIO_READ         = 0x20,
    FATA_CMD_PIO_READ_EXT     = 0x24,
    FATA_CMD_READ_DMA_EXT     = 0x25,
    FATA_CMD_PIO_WRITE        = 0x30,
    FATA_CMD_PIO_WRITE_EXT    = 0x34,
    FATA_CMD_WRITE_DMA_EXT    = 0x35,
    FATA_CMD_VERIFY           = 0x40,
    FATA_CMD_VERIFY_EXT       = 0x42,
    FATA_READ_FPDMA_QUEUED    = 0x60,
    FATA_WRITE_FPDMA_QUEUED   = 0x61,
    FATA_CMD_EDD              = 0x90, /* execute device diagnostic */
    FATA_CMD_PACKET           = 0xA0,
    FATA_CMD_ID_ATAPI         = 0xA1,
    FATA_CMD_READ_DMA         = 0xC8,
    FATA_CMD_WRITE_DMA        = 0xCA,
    FATA_CMD_STANDBY          = 0xE2,
    FATA_CMD_IDLE             = 0xE3,
    FATA_CMD_CHECK_POWER_MODE = 0xE5,
    FATA_CMD_SLEEP            = 0xE6,
    FATA_CMD_FLUSH            = 0xE7,
    FATA_CMD_FLUSH_EXT        = 0xEA,
    FATA_CMD_ID_ATA           = 0xEC,
    FATA_CMD_SET_FEATURES     = 0xEF
};

enum
{
    FSATA_SRST_ASSERT         = 1,
    FSATA_SRST_DEASSERT       = 2,
    FSATA_EXEC_DEVDIG         = 3,
    FSATA_NO_DATA             = 4,
    FSATA_PIO                 = 5,
    FSATA_DMA                 = 6,
    FSATA_FPDMA               = 7
};

/*
 * Description information bitdefs
 */
enum 
{
	FSATA_CMD_DESC_RES = (1 << 11),
	FSATA_VENDOR_SPECIFIC_BIST = (1 << 10),
	FSATA_CMD_DESC_SNOOP_ENABLE = (1 << 9),
	FSATA_FPDMA_QUEUED_CMD = (1 << 8),
	FSATA_SRST_CMD = (1 << 7),
	FSATA_BIST = (1 << 6),
	FSATA_ATAPI_CMD = (1 << 5),
};

/* SATA Status and Control Registers */
#define  FSATA_SUPERSET_SSTATUS    0
#define  FSATA_SUPERSET_SERROR     4
#define  FSATA_SUPERSET_SCONTROL   8
#define  FSATA_SUPERSET_SNOTIFY    0xC
#define  FSATA_SCR_ACTIVE		   3

#define  FSATA_INT_ON_DATA_LENGTH_MISMATCH   (1 << 12)
#define  FSATA_INT_ON_FATAL_ERR              (1 << 5)
#define  FSATA_INT_ON_PHYRDY_CHG             (1 << 4)
#define  FSATA_INT_ON_SIGNATURE_UPDATE       (1 << 3)
#define  FSATA_INT_ON_SNOTIFY_UPDATE         (1 << 2)
#define  FSATA_INT_ON_SINGL_DEVICE_ERR       (1 << 1)
#define  FSATA_INT_ON_CMD_COMPLETE           1

#define  FSATA_INT_ON_ERROR   (FSATA_INT_ON_FATAL_ERR | FSATA_INT_ON_SNOTIFY_UPDATE | \
	                           FSATA_INT_ON_SINGL_DEVICE_ERR)
	

#define  FSATA_SETFEATURES_WC_ON	           0x02 /* Enable write cache */
#define  FSATA_SETFEATURES_WC_OFF	           0x82 /* Disable write cache */

#define  FSATA_MAX_PORT                             2
#define  FSATA_MAX_REQ                              16
#define  FSATA_SECTOR_SIZE                          512
#define  FSATA_DATA_IN                              1
#define  FSATA_DATA_OUT                             2
#define  FSATA_MAX_NCQ_TAG                          32
#define  FSATA_NODATA_TRAN                          0
#define  FSATA_FORM_DEV_TRAN                        1  
#define  FSATA_TO_DEV_TRAN                          2 
#define  FSATA_HCONTROL_DATA_SNOOP_ENABLE_V2       (1 << 28)
#define  FSATA_CRC_ERROR_FLAG                       1

#define  FSATA_COMMAND_CQR                  0
#define  FSATA_COMMAND_CAR                  8
#define  FSATA_COMMAND_CCR                  0x10
#define  FSATA_COMMAND_CER                  0x18
#define  FSATA_COMMAND_DER                  0x20
#define  FSATA_COMMAND_CHBA                 0x24
#define  FSATA_COMMAND_HSTATUS              0x28
#define  FSATA_COMMAND_HCONTROL             0x2C
#define  FSATA_COMMAND_CQPMP                0x30
#define  FSATA_COMMAND_SIGNATURE            0x34
#define  FSATA_COMMAND_ICC                  0x38
#define  FSATA_COMMAND_SSTATUS              0x100

#define  FSATA_SUPSETREG_OFFSET             0x100
#define  FSATA_CONSTSREG_OFFSET             0x140
#define  FSATA_SYSREG_OFFSET                0x40C
#define  FSATA_PRD_NUM                      16
#define  FSATA_PRD_SIZE                     16
#define  FSATA_MAX_PRD_DIRECT               16
#define  FSATA_MAX_CMDS                     16
#define  FSATA_CMD_SLOT_SIZE                (FSATA_MAX_CMDS * 16)
           
#define  FSATA_CMD_DESC_CFIS_SZ             32
#define  FSATA_CMD_DESC_SFIS_SZ             32
#define  FSATA_CMD_DESC_ACMD_SZ             16
#define  FSATA_CMD_DESC_RSRVD               16
#define  FSATA_PRDE_RSRVD                   8
#define  FSATA_SSTATUS_RDY                  3
#define  FSATA_SSTATUS_SPEED_MASK           0xF0
#define  FSATA_SSTATUS_SPEED_15             0x10
#define  FSATA_SSTATUS_SPEED_30             0x20


/* Host Status Register (HStatus) definement */
#define  FSATA_HSTATUS_MASK                     0x3F
#define  FSATA_HSTATUS_ONLINE                   (1 << 31)
#define  FSATA_HSTATUS_GOING_OFFLINE            (1 << 30)
#define  FSATA_HSTATUS_BIST_ERR                 (1 << 29)

#define  FSATA_HSTATUS_FATAL_ERR_HC_MASTER_ERR  (1 << 18)
#define  FSATA_HSTATUS_FATAL_ERR_PARITY_ERR_TX  (1 << 17)
#define  FSATA_HSTATUS_FATAL_ERR_PARITY_ERR_RX  (1 << 16)
#define  FSATA_HSTATUS_FATAL_ERR_DATA_UNDERRUN  (1 << 13)
#define  FSATA_HSTATUS_FATAL_ERR_DATA_OVERRUN   (1 << 12)
#define  FSATA_HSTATUS_FATAL_ERR_CRC_ERR_TX     (1 << 11)
#define  FSATA_HSTATUS_FATAL_ERR_CRC_ERR_RX     (1 << 10)
#define  FSATA_HSTATUS_FATAL_ERR_FIFO_OVRFL_TX  (1 << 9)
#define  FSATA_HSTATUS_FATAL_ERR_FIFO_OVRFL_RX  (1 << 8)

#define  FSATA_HSTATUS_FATAL_ERROR_DECODE       (FSATA_HSTATUS_FATAL_ERR_HC_MASTER_ERR | \
                                                 FSATA_HSTATUS_FATAL_ERR_PARITY_ERR_TX | \
                                                 FSATA_HSTATUS_FATAL_ERR_PARITY_ERR_RX | \
                                                 FSATA_HSTATUS_FATAL_ERR_DATA_UNDERRUN | \
                                                 FSATA_HSTATUS_FATAL_ERR_DATA_OVERRUN  | \
                                                 FSATA_HSTATUS_FATAL_ERR_CRC_ERR_TX    | \
                                                 FSATA_HSTATUS_FATAL_ERR_CRC_ERR_RX    | \
                                                 FSATA_HSTATUS_FATAL_ERR_FIFO_OVRFL_TX | \
                                                 FSATA_HSTATUS_FATAL_ERR_FIFO_OVRFL_RX)

#define  FSATA_HSTATUS_INT_ON_FATAL_ERR         (1 << 5)
#define  FSATA_HSTATUS_INT_ON_PHYRDY_CHG        (1 << 4)
#define  FSATA_HSTATUS_INT_ON_SIGNATURE_UPDATE  (1 << 3)
#define  FSATA_HSTATUS_INT_ON_SNOTIFY_UPDATE    (1 << 2)
#define  FSATA_HSTATUS_INT_ON_SINGL_DEVICE_ERR  (1 << 1)
#define  FSATA_HSTATUS_INT_ON_CMD_COMPLETE      1

#define  FSATA_HSTATUS_INT_ON_ERROR             (FSATA_HSTATUS_INT_ON_FATAL_ERR | \
                                                 FSATA_HSTATUS_INT_ON_SINGL_DEVICE_ERR)


                                         
/* SATA Superset Registers */

#define  FSATA_SUPERSET_SSTATUS                    0
#define  FSATA_SUPERSET_SERROR                     4
#define  FSATA_SUPERSET_SCONTROL                   8
#define  FSATA_SUPERSET_SNOTIFY                    0xC

/* Host Control Register (HControl) definement */
#define  FSATA_HCONTROL_ONLINE_PHY_RST          (1 << 31)
#define  FSATA_HCONTROL_FORCE_OFFLINE           (1 << 30)
#define  FSATA_HCONTROL_CLEAR_ERROR             (1 << 27)
#define  FSATA_HCONTROL_PARITY_PROT_MOD         (1 << 14)
#define  FSATA_HCONTROL_DPATH_PARITY            (1 << 12)
#define  FSATA_HCONTROL_SNOOP_ENABLE            (1 << 10)
#define  FSATA_HCONTROL_PMP_ATTACHED            (1 << 9)
#define  FSATA_HCONTROL_COPYOUT_STATFIS         (1 << 8)
#define  FSATA_HCONTROL_IE_ON_FATAL_ERR         (1 << 5)
#define  FSATA_HCONTROL_IE_ON_PHYRDY_CHG        (1 << 4)
#define  FSATA_HCONTROL_IE_ON_SIGNATURE_UPDATE  (1 << 3)
#define  FSATA_HCONTROL_IE_ON_SNOTIFY_UPDATE    (1 << 2)
#define  FSATA_HCONTROL_IE_ON_SINGL_DEVICE_ERR  (1 << 1)
#define  FSATA_HCONTROL_IE_ON_CMD_COMPLETE      1
/* SATA interface status Register */
#define  FSATA_SSTATUS_DET_MASK                 0x0F
#define  FSATA_SSTATUS_DET_NODEVICE             0
#define  FSATA_SSTATUS_DET_NOPHY                1
#define  FSATA_SSTATUS_DET_DEVICEANDPHY         3
#define  FSATA_SSTATUS_DET_PHYDISABLE           4
         
/* SATA Interface Error Register (SError) */
#define  FSATA_SERROR_CRC_ERROR                 (1 << 21)
#define  FSATA_HCONTROL_DEFAULT_PORT_IRQ_ENABLE_MASK \
                     ( FSATA_HCONTROL_IE_ON_FATAL_ERR        | \
                       FSATA_HCONTROL_IE_ON_PHYRDY_CHG       | \
                       FSATA_HCONTROL_IE_ON_SIGNATURE_UPDATE | \
                       FSATA_HCONTROL_IE_ON_SNOTIFY_UPDATE   | \
                       FSATA_HCONTROL_IE_ON_SINGL_DEVICE_ERR | \
                       FSATA_HCONTROL_IE_ON_CMD_COMPLETE )
#define  FSATA_HCONTROL_DEFAULT_PORT_IRQ_EXCEPT_PHYRDY  \
                     ( FSATA_HCONTROL_IE_ON_FATAL_ERR        | \
                       FSATA_HCONTROL_IE_ON_SIGNATURE_UPDATE | \
                       FSATA_HCONTROL_IE_ON_SNOTIFY_UPDATE   | \
                       FSATA_HCONTROL_IE_ON_SINGL_DEVICE_ERR | \
                       FSATA_HCONTROL_IE_ON_CMD_COMPLETE )

#define FSATA_CMD_LEN                (FSATA_CMD_DESC_CFIS_SZ +  \
                                      FSATA_CMD_DESC_SFIS_SZ +  \
                                      FSATA_CMD_DESC_ACMD_SZ +  \
                                      FSATA_CMD_DESC_RSRVD)

#define FSATA_CMD_DESC_SIZE          (FSATA_CMD_DESC_CFIS_SZ +  \
                                      FSATA_CMD_DESC_SFIS_SZ +  \
                                      FSATA_CMD_DESC_ACMD_SZ +  \
                                      FSATA_CMD_DESC_RSRVD +    \
                                      (FSATA_PRD_NUM * FSATA_PRD_SIZE))
                                               

#define FSATA_PORT_PRIV_DMA_SZ       (FSATA_CMD_SLOT_SIZE +     \
                                      FSATA_CMD_DESC_SIZE *  \
                                      FSATA_MAX_CMDS)

typedef enum  tagFSATA_LINK_SATUS_E
{
    FSATA_LINK_DOWN = 0,
    FSATA_LINK_UP, 
}FSATA_LINK_SATUS_E;

typedef enum tagFSATA_LINK_RATE_E
{
    FSATA_RATE_1_5_G,
    FSATA_RATE_3_0_G,
    FSATA_RATE_6_0_G,
    FSATA_LINK_RATE_BUTT
}FSATA_LINK_RATE_E;

typedef enum tagFSATA_REQ_STATUS_S
{
    FSATA_REQ_UNUSED = 0,
    FSATA_REQ_USED,
    FSATA_REQ_BUTT
}FSATA_REQ_STATUS_S;

typedef VOID (*pfBlkResp)(VOID *pstReq);

typedef struct tagFSATA_HOST_TO_DEV_S 
{
	UINT8     ucFisType;	  /* 0x27 */
	UINT8     ucFlags;
	UINT8     ucCmd;
	UINT8     ucFeature;

	UINT8     ucLbal;
	UINT8     ucLbam;
	UINT8     ucLbah;
	UINT8     ucDevice;

	UINT8     ucLbalExp;
	UINT8     ucLbamExp;
	UINT8     ucLbahExp;
	UINT8     ucFeaturesExp;

	UINT8     ucSecCnt;
	UINT8     ucSecCntExp;
	UINT8     ucRA;
	UINT8     ucCtrl;

	UINT8     ucRb[4];
}FSATA_HOST_TO_DEV_S;

typedef struct tagFSATA_SGL_S
{
    UINT32  uiSGLLo;
    UINT32  uiSGLHi;
    UINT32  uiLen;
}FSATA_SGL_S;

typedef struct tagFSATA_REQ_S
{
    FSATA_HOST_TO_DEV_S stFis;
  
    UINT32              uiStatus;
    UINT32              uiTag;
    
    UINT8               ucReqType;
    UINT8               ucDirection;
    UINT8               ucNCQTag;
    UINT8               aucLBA[6];

    UINT32              uiOper;
    UINT32              uiFlag;
    FSATA_SGL_S         stSGL;
    UINT8              *pucBuf;
	VOID               *pvPort;
	
    pfBlkResp           pfDone;
    BLK_REQ_S          *pstBlkReq;
    VOID               *pstLLReq;

    UINT32             uiRespStatus;
    SEM_ID             semComp;
}FSATA_REQ_S;

typedef struct tagFSATA_ID_DATA_S
{                                  
  UINT16   usAtaDevice;            /*[word 0]*/
  UINT16   usWord1[9];             /*[word 1~9]*/
  UINT8    usSerialNum[20];        /*[word 10~29]*/
  UINT16   usWord20[3];            /*[word 20~22]*/
  UINT8    usFirmwareVer[8];       /*[word 23~26]*/
  UINT8    usModelNumber[40];      /*[word 27~46]*/
  UINT16   usWord47[2];            /*[word 47~48]*/
  UINT16   usDmaLbaIodTimer;       /*[word 49]*/
  UINT16   usWord50[3];            /*[word 50~52]*/
  UINT16   usValidW88;             /*[word 53]*/
  UINT16   usWord54[6];            /*[word 54~59]*/
  UINT16   usUserSecLo;            /*[word 60]*/
  UINT16   usUserSecHi;            /*[word 61]*/
  UINT16   usWord62[13];           /*[word 62~74]*/
  UINT16   usQueueDepth;           /*[word 75]*/
  UINT16   usCapabilities;         /*[word 76]*/
  UINT16   usWord77;               /*[word 77]*/
  UINT16   usFeaturesSupport;      /*[word 78]*/
  UINT16   usFeaturesEnabled;      /*[word 79]*/
  UINT16   usMajorVer;             /*[word 80]*/
  UINT16   usMinorVersionNumber;   /*[word 81]*/
  UINT16   usCmndSetSupported;     /*[word 82]*/
  UINT16   usCmndSetSupported1;    /*[word 83]*/
  UINT16   usCmndSetFeaSupportExt; /*[word 84]*/
  UINT16   usCmndSetFeaEnabled;    /*[word 85]*/
  UINT16   usCmndSetFeaEnabled1;   /*[word 86]*/
  UINT16   usCmndSetFeaDefault;    /*[word 87]*/
  UINT16   usUltraDMAModes;        /*[word 88]*/
  UINT16   usTimeToSecurityErase;  /*[word 89]*/
  UINT16   usTimeToSecurityErase1;  /*[word 90]*/
  UINT16   usCurrentAPMValue;      /*[word 91]*/
  UINT16   usMasterPwdRevCode;     /*[word 92]*/
  UINT16   usHardwareResetResult;  /*[word 93]*/
  UINT16   usAutoAccouMgntValue;   /*[word 94]*/
  UINT16   usWord95[5];            /*[word 95~99]*/
  UINT16   usMaxLBA0;              /*[word 100]*/
  UINT16   usMaxLBA16;             /*[word 101]*/
  UINT16   usMaxLBA32;             /*[word 102]*/
  UINT16   usMaxLBA48;             /*[word 103]*/
  UINT16   usWord104[4];           /*[word 104~107]*/
  UINT16   usNamingAuthority;      /*[word 108]*/
  UINT16   usNamingAuthority1;     /*[word 109]*/
  UINT16   usUniqueID16;           /*[word 110]*/
  UINT16   usUniqueID0;            /*[word 111]*/
  UINT16   usWord112[15];          /*[word 112~126]*/
  UINT16   usRemovableMediaStatus; /*[word 127]*/
  UINT16   usSecurityStatus;       /*[word 128]*/
  UINT16   usVendorSpecific[31];   /*[word 129~159]*/
  UINT16   usCfaPowerMode1;        /*[word 160]*/
  UINT16   usWord161[15];          /*[word 161~175]*/
  UINT16   usMediaSerialNumber[30];/*[word 176~205]*/
  UINT16   usWord206[49];          /*[word 206~254]*/
  UINT16   usIntegrityWord;        /*[word 255]*/
} FSATA_ID_DATA_S; 

typedef struct tagFSATA_MEM_S
{
    VOID			*pvAllocVirPtr;/*allocated memory pointer before align*/
    VOID			*pvVirPtr;
	UINT32 			 uiPhyAddrHi;
	UINT32			 uiPhyAddrLo;
	UINT32			 uiTotalLen;
	UINT32			 uiAlign;      
}FSATA_MEM_S;

typedef struct tagFSATA_CMDHDR_S
{
    UINT32   uiCda;
    UINT32   uiPrdeFisLen;
    UINT32   uiTtl;
    UINT32   uiDescInfo;
}FSATA_CMDHDR_S;

typedef struct tagFSATA_PRDE_S
{
	UINT32  uiDba;
	UINT8   aucFill[2 * 4];
	UINT32  uiDdcAndExt;
}FSATA_PRDE_S;

typedef struct tagFSATA_COMMAND_DESC
{
    UINT8         aucCfis[FSATA_CMD_DESC_CFIS_SZ];
    UINT8         aucSfis[FSATA_CMD_DESC_SFIS_SZ];
    UINT8         aucAcmd[FSATA_CMD_DESC_ACMD_SZ];
    UINT8         aucResv[FSATA_CMD_DESC_RSRVD];
    FSATA_PRDE_S  auiPrdt[FSATA_PRD_NUM];
} FSATA_COMMAND_DESC_S;

struct tagFSATA_DRV_S;

typedef struct tagFSATA_PORT_S
{
    UINT32                 uiPortId;
    UINT32                 uiStatus;
	UINT32		           uiFlags;
    UINT32 			       uiIrq;
	UINT32                 uiCmndActive;
	VOID                  *pvRegBase;
	struct tagFSATA_DRV_S *pstDrv;
	UINT32                 uiSsrAdrsOff;
    UINT32                 uiCsrAdrsOff;
    UINT32                 uiScrAdrsOff;
	FSATA_CMDHDR_S        *pstCmdslot;
    UINT32                 uiCmdSlotPhyAddr;
    FSATA_COMMAND_DESC_S  *pstCmdDentry;
    UINT32                 uiCmdDentryPhyAddr;
	FSATA_MEM_S            stMem;

	UINT32                 ui48BitLBA;
    UINT32                 uiNCQMaxIO;
    UINT32                 uiNCQSupport;
    UINT32                 uiNCQEnable;/*ncq enable switch*/
    UINT32                 uiRemovable;
    UINT32                 uiRemoveEnable;
    UINT32                 uiDMASupport;
    UINT32                 uiDMAEnable;
    UINT32                 uiMaxAddrSectors;
    UINT32                 uiWrCacheEnable;
    UINT32                 uiNcqTagCtrl;
    UINT8                  ucCapacity[8];
    FSATA_ID_DATA_S        stIden;
    SEM_ID                 semMux;
	spinlockIsr_t          stLock;
	FSATA_REQ_S            astReq[FSATA_MAX_REQ];
}FSATA_PORT_S;


typedef struct tagFSATA_DRV_S
{
    FSATA_PORT_S    astPort[FSATA_MAX_PORT];
}FSATA_DRV_S;

#endif

