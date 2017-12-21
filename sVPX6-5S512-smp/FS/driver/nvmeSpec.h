/*
 * Definitions for the NVM Express interface
 */

#ifndef _NVME_SPEC_H
#define _NVME_SPEC_H

#include "cacheLib.h"

#define PCI_EXT_RXCAP         0x10
#define NVME_MAX_DELAY	      60

enum
{
    NVME_SATA_SRST_ASSERT         = 1,
    NVME_SATA_SRST_DEASSERT       = 2,
    NVME_SATA_EXEC_DEVDIG         = 3,
    NVME_SATA_NO_DATA             = 4,
    NVME_SATA_PIO                 = 5,
    NVME_SATA_DMA                 = 6,
    NVME_SATA_FPDMA               = 7
};

typedef struct tagNVME_BAR_S 
{
	UINT64			uiCap;	    /* Controller Capabilities */
	UINT32			uiVs;	    /* Version */
	UINT32			uiIntms;	/* Interrupt Mask Set */
	UINT32			uiIntmc;	/* Interrupt Mask Clear */
	UINT32			uiCC;	    /* Controller Configuration */
	UINT32			uiRsvd1;	/* Reserved */
	UINT32			uiCsts;	    /* Controller Status */
	UINT32			uiRsvd2;	/* Reserved */
	UINT32			uiAqa;	    /* Admin Queue Attributes */
	UINT64			uiAsq;	    /* Admin SQ Base Address */
	UINT64			uiAcq;	    /* Admin CQ Base Address */
}NVME_BAR_S;

#define NVME_CAP_MQES(cap)	     ((cap) & 0xffff)
#define NVME_CAP_TIMEOUT(cap)	 (((cap) >> 24) & 0xff)
#define NVME_CAP_STRIDE(cap)	 (((cap) >> 32) & 0xf)
#define NVME_CAP_MPSMIN(cap)	 (((cap) >> 48) & 0xf)
#define NVME_PAGE_SHIFT          20
#define NVME_PAGE_SIZE           (1 << NVME_PAGE_SHIFT)
#define NVME_PRP_TRAN            0
#define NVME_SGL_TRAN            (0x40)
enum 
{
	NVME_CC_ENABLE		    = 1 << 0,
	NVME_CC_CSS_NVM		    = 0 << 4,
	NVME_CC_MPS_SHIFT	    = 7,
	NVME_CC_ARB_RR		    = 0 << 11,
	NVME_CC_ARB_WRRU	    = 1 << 11,
	NVME_CC_ARB_VS		    = 7 << 11,
	NVME_CC_SHN_NONE	    = 0 << 14,
	NVME_CC_SHN_NORMAL	    = 1 << 14,
	NVME_CC_SHN_ABRUPT	    = 2 << 14,
	NVME_CC_SHN_MASK	    = 3 << 14,
	NVME_CC_IOSQES		    = 6 << 16,
	NVME_CC_IOCQES		    = 4 << 20,
	NVME_CSTS_RDY		    = 1 << 0,
	NVME_CSTS_CFS		    = 1 << 1,
	NVME_CSTS_SHST_NORMAL	= 0 << 2,
	NVME_CSTS_SHST_OCCUR	= 1 << 2,
	NVME_CSTS_SHST_CMPLT	= 2 << 2,
	NVME_CSTS_SHST_MASK	    = 3 << 2,
};

typedef struct tagNVME_ID_POWER_STATE_S
{
	UINT16			uiMaxPower;	/* centiwatts */
	UINT8			uiRsvd2;
	UINT8			uiFlags;
	UINT32			uiEntryLat;	/* microseconds */
	UINT32			uiExitLat;	/* microseconds */
	UINT8			ucReadTput;
	UINT8			ucReadLat;
	UINT8			ucWriteTput;
	UINT8			ucWriteLat;
	UINT8			ucRsvd16[16];
}NVME_ID_POWER_STATE_S;

enum 
{
	NVME_PS_FLAGS_MAX_POWER_SCALE	= 1 << 0,
	NVME_PS_FLAGS_NON_OP_STATE	    = 1 << 1,
};

typedef struct tagNVME_ID_CTRL_S
{
	UINT16			        usVid;
	UINT16			        usSsvid;
	CHAR			        aucSn[20];
	CHAR			        aucMn[40];
	CHAR			        aucFr[8];
	UINT8			        ucRab;
	UINT8			        aucIeee[3];
	UINT8			        ucMic;
	UINT8			        ucMdts;
	UINT8			        aucRsvd178[178];
	UINT16			        usOacs;
	UINT8			        ucAcl;
	UINT8			        ucAerl;
	UINT8			        ucFrmw;
	UINT8			        ucLpa;
	UINT8			        ucElpe;
	UINT8			        ucNpss;
	UINT8			        aucRsvd264[248];
	UINT8			        ucSqes;
	UINT8			        ucCqes;
	UINT8			        aucRsvd514[2];
	UINT32			        uiNn;
	UINT16			        usOncs;
	UINT16			        usFuses;
	UINT8			        ucFNA;
	UINT8			        ucVWC;
	UINT16			        usAwun;
	UINT16			        usAwupf;
	UINT8			        arsvd530[1518];
	NVME_ID_POWER_STATE_S	astPsd[32];
	UINT8			        aucVS[1024];
}NVME_ID_CTRL_S;

enum 
{
	NVME_CTRL_ONCS_COMPARE			    = 1 << 0,
	NVME_CTRL_ONCS_WRITE_UNCORRECTABLE	= 1 << 1,
	NVME_CTRL_ONCS_DSM			        = 1 << 2,
};

typedef struct tagNVME_LBAF_S
{
	UINT16			usms;
	UINT8			ucDs;
	UINT8			ucRp;
}NVME_LBAF_S;

typedef struct tagNVME_ID_NS_S 
{
	UINT64			ullNsze;
	UINT64			ullNcap;
	UINT64			ullNuse;
	UINT8			ucNsfeat;
	UINT8			ucNlbaf;
	UINT8			ucFlbas;
	UINT8			ucMc;
	UINT8			ucDpc;
	UINT8			ucDps;
	UINT8			aucRsvd98[98];
	NVME_LBAF_S  	astLbaf[16];
	UINT8			aucRsvd192[192];
	UINT8			aucVs[3712];
}NVME_ID_NS_S;

enum 
{
	NVME_NS_FEAT_THIN	= 1 << 0,
	NVME_LBAF_RP_BEST	= 0,
	NVME_LBAF_RP_BETTER	= 1,
	NVME_LBAF_RP_GOOD	= 2,
	NVME_LBAF_RP_DEGRADED	= 3,
};

typedef struct tagNVME_SMART_LOG_S
{
	UINT8			ucCriticalWarning;
	UINT8			aucTemperature[2];
	UINT8			ucAvailSpare;
	UINT8			ucSpareThresh;
	UINT8			ucPercentUsed;
	UINT8			aucRsvd26[26];
	UINT8			aucDataUnitsRead[16];
	UINT8			aucDataUnitsWritten[16];
	UINT8			aucHostReads[16];
	UINT8			aucHostWrites[16];
	UINT8			aucCtrlBusyTime[16];
	UINT8			aucPowerCycles[16];
	UINT8			aucPowerOnHours[16];
	UINT8			aucUnsafeShutdowns[16];
	UINT8			aucMediaErrors[16];
	UINT8			aucNumErrLogEntries[16];
	UINT8			aucRsvd320[320];
}NVME_SMART_LOG_S;

enum 
{
	NVME_SMART_CRIT_SPARE		    = 1 << 0,
	NVME_SMART_CRIT_TEMPERATURE	    = 1 << 1,
	NVME_SMART_CRIT_RELIABILITY	    = 1 << 2,
	NVME_SMART_CRIT_MEDIA		    = 1 << 3,
	NVME_SMART_CRIT_VOLATILE_MEMORY	= 1 << 4,
};

typedef struct tagNVME_LBA_RANGE_TYPE_S
{
	UINT8			ucType;
	UINT8			ucAttributes;
	UINT8			ucRsvd2[14];
	UINT64			ullSlba;
	UINT64			ullNlb;
	UINT8			aucGuid[16];
	UINT8			aucRsvd48[16];
}NVME_LBA_RANGE_TYPE_S;

enum 
{
	NVME_LBART_TYPE_FS	    = 0x01,
	NVME_LBART_TYPE_RAID	= 0x02,
	NVME_LBART_TYPE_CACHE	= 0x03,
	NVME_LBART_TYPE_SWAP	= 0x04,
	NVME_LBART_ATTRIB_TEMP	= 1 << 0,
	NVME_LBART_ATTRIB_HIDE	= 1 << 1,
};

/* I/O commands */

enum NVME_OPCODE_E 
{
	NVME_CMD_FLUSH		    = 0x00,
	NVME_CMD_WRITE		    = 0x01,
	NVME_CMD_READ		    = 0x02,
	NVME_CMD_WRITE_UNCOR	= 0x04,
	NVME_CMD_COMPARE	    = 0x05,
	NVME_CMD_WRITE_ZERO	    = 0x08,
	NVME_CMD_DSM		    = 0x09,
};

#define NVME_MAX_PRP_LIST     512

typedef struct tagNVME_PRP_LIST_S
{
	UINT64			aullPrpList[NVME_MAX_PRP_LIST];
}NVME_PRP_LIST_S;

typedef struct tagNVME_COMMON_CMND_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			uiNsId;
	UINT32			auiCdw2[2];
	UINT64			ullMetaData;
	UINT64			ullPrp1;
	UINT64			ullPrp2;
	UINT32			auiCdw10[6];
}NVME_COMMON_CMND_S;

typedef struct tagNVME_RW_CMND_S 
{
	UINT8			ucOpCode;    /*byte 0*/
	UINT8			ucFlags;     /*byte 1*/
	UINT16			usCmdId;     /*byte 2 3*/
	UINT32			uiNsId;      /*byte 4 5 6 7*/
	UINT64			ullRsvd2;    /*byte 8~15*/
	UINT64			ullMetaData; /*byte 16~23*/
	UINT64			ullPrp1;     /*byte 24~31*/
	UINT64			ullPrp2;     /*byte 32~39*/
	UINT64			ullSlba;     /*byte 40~47*/
	UINT16			usLen;       /*byte 48~49*/
	UINT16			usCtrl;      /*byte 50~51*/
	UINT32			uiDsmGmt;    /*byte 52~55*/
	UINT32			uiRefTag;    /*byte 56~59*/
	UINT16			usAppTag;    /*byte 60~61*/
	UINT16			usAppMask;   /*byte 62~63*/
}NVME_RW_CMND_S;

typedef struct tagNVME_TRIM_CMND_S 
{
	UINT8			ucOpCode;    /*byte 0*/
	UINT8			ucFlags;     /*byte 1*/
	UINT16			usCmdId;     /*byte 2 3*/
	UINT32			uiNsId;      /*byte 4 5 6 7*/
	UINT64			ullRsvd2;    /*byte 8~15*/
	UINT64			ullMetaData; /*byte 16~23*/
	UINT64			ullPrp1;     /*byte 24~31*/
	UINT64			ullPrp2;     /*byte 32~39*/
	UINT64			ullSlba;     /*byte 40~47*/
	UINT32			uiAttr;      /*byte 48~51*/
	UINT32			auiResv[3];  /*byte 52~63*/
}NVME_TRIM_CMND_S ;

typedef struct tagNVME_TRIM_DATA_S 
{
	UINT32			uiCattr;
	UINT32			uiNlb;
	UINT64			ullSlba;
}NVME_TRIM_DATA_S;

enum 
{
	NVME_RW_LR			= 1 << 15,
	NVME_RW_FUA			= 1 << 14,
	NVME_RW_DSM_FREQ_UNSPEC		= 0,
	NVME_RW_DSM_FREQ_TYPICAL	= 1,
	NVME_RW_DSM_FREQ_RARE		= 2,
	NVME_RW_DSM_FREQ_READS		= 3,
	NVME_RW_DSM_FREQ_WRITES		= 4,
	NVME_RW_DSM_FREQ_RW		= 5,
	NVME_RW_DSM_FREQ_ONCE		= 6,
	NVME_RW_DSM_FREQ_PREFETCH	= 7,
	NVME_RW_DSM_FREQ_TEMP		= 8,
	NVME_RW_DSM_LATENCY_NONE	= 0 << 4,
	NVME_RW_DSM_LATENCY_IDLE	= 1 << 4,
	NVME_RW_DSM_LATENCY_NORM	= 2 << 4,
	NVME_RW_DSM_LATENCY_LOW		= 3 << 4,
	NVME_RW_DSM_SEQ_REQ		= 1 << 6,
	NVME_RW_DSM_COMPRESSED		= 1 << 7,
};

enum 
{
	NVME_DSMGMT_IDR		= 1 << 0,
	NVME_DSMGMT_IDW		= 1 << 1,
	NVME_DSMGMT_AD		= 1 << 2,
};

typedef struct tagNVME_DSM_RANGE_S 
{
	UINT32			uiCattr;
	UINT32			uiNlb;
	UINT64			ullSlba;
}NVME_DSM_RANGE_S;

/* Admin commands */

enum NVME_ADMIN_OPCODE_E 
{
	NVME_ADMIN_DELETE_SQ		= 0x00,
	NVME_ADMIN_CREATE_SQ		= 0x01,
	NVME_ADMIN_GET_LOG_PAGE		= 0x02,
	NVME_ADMIN_DELETE_CQ		= 0x04,
	NVME_ADMIN_CREATE_CQ		= 0x05,
	NVME_ADMIN_IDENTIFY 		= 0x06,
	NVME_ADMIN_ABORT_CMND		= 0x08,
	NVME_ADMIN_SET_FEATURES		= 0x09,
	NVME_ADMIN_GET_FEATURES		= 0x0a,
	NVME_ADMIN_ASYNC_EVENT		= 0x0c,
	NVME_ADMIN_ACTIVATE_FW		= 0x10,
	NVME_ADMIN_DOWNLOAD_FW		= 0x11,
	NVME_ADMIN_FORMAT_NVM		= 0x80,
	NVME_ADMIN_SECURITY_SEND	= 0x81,
	NVME_ADMIN_SECURITY_RECV	= 0x82,
};

#define NVME_GET_SMART_LOG_ERROR			0x01
#define NVME_GET_SMART_LOG_SMART			0x02
#define NVME_GET_FEAT_TEMP_THRESH			0x04

#define NVME_USER_DATA_ERASE  			    (0x01 << 9)
#define NVME_USER_DATA_CRYPTO_ERASE			(0x02 << 9)

enum 
{
	NVME_QUEUE_PHYS_CONTIG	= (1 << 0),
	NVME_CQ_IRQ_ENABLED	    = (1 << 1),
	NVME_SQ_PRIO_URGENT	    = (0 << 1),
	NVME_SQ_PRIO_HIGH	    = (1 << 1),
	NVME_SQ_PRIO_MEDIUM	    = (2 << 1),
	NVME_SQ_PRIO_LOW	    = (3 << 1),
	NVME_FEAT_ARBITRATION	= 0x01,
	NVME_FEAT_POWER_MGMT	= 0x02,
	NVME_FEAT_LBA_RANGE	    = 0x03,
	NVME_FEAT_TEMP_THRESH	= 0x04,
	NVME_FEAT_ERR_RECOVERY	= 0x05,
	NVME_FEAT_VOLATILE_WC	= 0x06,
	NVME_FEAT_NUM_QUEUES	= 0x07,
	NVME_FEAT_IRQ_COALESCE	= 0x08,
	NVME_FEAT_IRQ_CONFIG	= 0x09,
	NVME_FEAT_WRITE_ATOMIC	= 0x0a,
	NVME_FEAT_ASYNC_EVENT	= 0x0b,
	NVME_FEAT_SW_PROGRESS	= 0x0c,
	NVME_FWACT_REPL		    = (0 << 3),
	NVME_FWACT_REPL_ACTV	= (1 << 3),
	NVME_FWACT_ACTV		    = (2 << 3),
};

typedef struct tagNVME_IDENTIFY_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			uiNsid;
	UINT64			aullRsvd2[2];
	UINT64			ullPrp1;
	UINT64			ullPrp2;
	UINT32			uiCns;
	UINT32			auiRsvd11[5];
}NVME_IDENTIFY_S;

typedef struct tagNVME_FEATURES_S 
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			uiNsid;
	UINT64			aullRsvd2[2];
	UINT64			ullPrp1;
	UINT64			ullPrp2;
	UINT32			uiFid;
	UINT32			uiDword11;
	UINT32			auiRsvd12[4];
}NVME_FEATURES_S;

typedef struct tagNVME_CREATE_CQ_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			auiRsvd1[5];
	UINT64			ullPrp1;
	UINT64			ullRsvd8;
	UINT16			usCqid;
	UINT16			usQSize;
	UINT16			usCqFlags;
	UINT16			usIrqVec;
	UINT32			auiRsvd12[4];
}NVME_CREATE_CQ_S;

typedef struct tagNVME_CREATE_SQ_S 
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			auiRsvd1[5];
	UINT64			ullPrp1;
	UINT64			ullRsvd8;
	UINT16			usSqid;
	UINT16			usQSize;
	UINT16			usSqFlags;
	UINT16			usCqid;
	UINT32			auiRsvd12[4];
}NVME_CREATE_SQ_S;

typedef struct tagNVME_DELETS_QUEUE_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			auiRsvd1[9];
	UINT16			usQid;
	UINT16			usRsvd10;
	UINT32			auiRsvd11[5];
}NVME_DELETS_QUEUE_S;

typedef struct tagNVME_DOWNLOAD_FIRMWARE_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			auiRsvd1[5];
	UINT64			ullPrp1;
	UINT64			ullPrp2;
	UINT32			uiNumd;
	UINT32			uiOffset;
	UINT32			auiRsvd12[4];
}NVME_DOWNLOAD_FIRMWARE_S;

typedef struct tagNVME_FORMAT_CMD_S
{
	UINT8			ucOpCode;
	UINT8			ucFlags;
	UINT16			usCmdId;
	UINT32			uiNsid;
	UINT64			ullRsvd2[4];
	UINT32			uiCdw10;
	UINT32			auiRsvd11[5];
}NVME_FORMAT_CMD_S;

typedef struct tagNVME_CMND_S 
{
	union 
	{
		NVME_COMMON_CMND_S        stCommon;
		NVME_RW_CMND_S            stRW;		
		NVME_TRIM_CMND_S          stTrim;
		NVME_IDENTIFY_S           stIdentify;
		NVME_FEATURES_S           stFeatures;
		NVME_CREATE_CQ_S          stCreateCq;
		NVME_CREATE_SQ_S          stCreateSq;
		NVME_DELETS_QUEUE_S       stDelQueue;
		NVME_DOWNLOAD_FIRMWARE_S  stDlFw;
		NVME_FORMAT_CMD_S         stFormat;
	}unCmd;
}NVME_CMND_S;

typedef struct tagNVME_SGL_S
{
	UINT64     ullAddr;
	UINT32     uiLen;
	UINT8      ucSGLIden;
	UINT8      astResv[3];
}NVME_SGL_S;

enum 
{
	NVME_SC_SUCCESS			    = 0x0,
	NVME_SC_INVALID_OPCODE		= 0x1,
	NVME_SC_INVALID_FIELD		= 0x2,
	NVME_SC_CMDID_CONFLICT		= 0x3,
	NVME_SC_DATA_XFER_ERROR		= 0x4,
	NVME_SC_POWER_LOSS		    = 0x5,
	NVME_SC_INTERNAL		    = 0x6,
	NVME_SC_ABORT_REQ		    = 0x7,
	NVME_SC_ABORT_QUEUE		    = 0x8,
	NVME_SC_FUSED_FAIL		    = 0x9,
	NVME_SC_FUSED_MISSING		= 0xa,
	NVME_SC_INVALID_NS		    = 0xb,
	NVME_SC_CMD_SEQ_ERROR		= 0xc,
	NVME_SC_LBA_RANGE		    = 0x80,
	NVME_SC_CAP_EXCEEDED		= 0x81,
	NVME_SC_NS_NOT_READY		= 0x82,
	NVME_SC_CQ_INVALID		    = 0x100,
	NVME_SC_QID_INVALID		    = 0x101,
	NVME_SC_QUEUE_SIZE		    = 0x102,
	NVME_SC_ABORT_LIMIT		    = 0x103,
	NVME_SC_ABORT_MISSING		= 0x104,
	NVME_SC_ASYNC_LIMIT		    = 0x105,
	NVME_SC_FIRMWARE_SLOT		= 0x106,
	NVME_SC_FIRMWARE_IMAGE		= 0x107,
	NVME_SC_INVALID_VECTOR		= 0x108,
	NVME_SC_INVALID_LOG_PAGE	= 0x109,
	NVME_SC_INVALID_FORMAT		= 0x10a,
	NVME_SC_BAD_ATTRIBUTES		= 0x180,
	NVME_SC_WRITE_FAULT		    = 0x280,
	NVME_SC_READ_ERROR		    = 0x281,
	NVME_SC_GUARD_CHECK		    = 0x282,
	NVME_SC_APPTAG_CHECK		= 0x283,
	NVME_SC_REFTAG_CHECK		= 0x284,
	NVME_SC_COMPARE_FAILED		= 0x285,
	NVME_SC_ACCESS_DENIED		= 0x286,
};

typedef struct tagNVME_CMPL_S
{
	UINT32	uiRet;		/* Used by admin commands to return data */
	UINT32	uiRsvd;
	UINT16	usSqHead;	/* how much of this queue may be reclaimed */
	UINT16	usSqId;		/* submission queue that generated this entry */
	UINT16	usCmdId;	/* of the command which completed */
	UINT16	usStatus;	/* did the command fail, and if so, why? */
}NVME_CMPL_S;

typedef struct tagNVME_USER_IO_S
{
	UINT8	ucOpCode;
	UINT8	ucFlags;
	UINT16	usCtrl;
	UINT16	usNBlocks;
	UINT16	usRsvd;
	UINT64	ullMetaData;
	UINT64	ullAddr;
	UINT64	ullSlba;
	UINT32	uiDsmGmt;
	UINT32	uiRefTag;
	UINT16	usAppTag;
	UINT16	usAppMask;
}NVME_USER_IO_S;

typedef struct tagNVME_ADMIN_CMD_S
{
	UINT8	ucOpCode;
	UINT8	ucFlags;
	UINT16	usRsvd1;
	UINT32	uiNsid;
	UINT32	uiCdw2;
	UINT32	uiCdw3;
	UINT64	ullMetaData;
	UINT64	ullAddr;
	UINT32	uiMetaDataLen;
	UINT32	uiDataLen;
	UINT32	uiCdw10;
	UINT32	uiCdw11;
	UINT32	uiCdw12;
	UINT32	uiCdw13;
	UINT32	uiCdw14;
	UINT32	uiCdw15;
	UINT32	uiTimeoutMs;
	UINT32	uiRet;
}NVME_ADMIN_CMD_S;


#endif /*_NVME_SPEC_H*/

