#ifndef __RIO_DRV_H__
#define __RIO_DRV_H__

extern UINT32 guiDbgLevel;

typedef enum tagSRIO_DBG_LEVEL_E
{
    SRIO_DBG_ERR = 0,
    SRIO_DBG_WARN,
    SRIO_DBG_INFO,
    SRIO_DBG_DATA,
    SRIO_DBG_BUTT
}SRIO_DBG_LEVEL_E;

extern int logMsg(char *,int ,int ,int ,int ,int ,int );

#define SRIO_LOG(level, X0, X1, X2, X3, X4, X5)            \
{                                                          \
    if (guiDbgLevel >= level)                              \
    {                                                      \
      logMsg (X0, X1, X2, X3, X4, X5,0);                   \
    }                                                      \
}

#define SRIO_MAX_MPORT            1
#define SRIO_MSG_FRM_SIZE         4096
#define SRIO_MAX_DEV_NUM          256
#define SRIO_MAX_DEV_ID           0xFF


#define SRIO_RESV_ADDR            0x3fff0000
#define SRIO_RESV_ADDR_2          0x3fff8000

#define SRIO_DOORBELL_INB_SIZE       256

#define SRIO_MAX_PORT_PER_SW         20
#define SRIO_MAX_SWITCH              8

#define SRIO_MAX_MBOX                2
#define SRIO_MAX_OUB                 32
#define SRIO_MAX_INB                 3
#define SRIO_MSG_INB_SIZE           (2048 * 4096)
#define SRIO_MSG_OUB_SIZE           (4096)

#define SRIO_OSR_EODI               0x2

#define SRIO_INVALID_PORT           0xdf
#define SRIO_INVALID_SW             0xff
#define SRIO_INVALID_DEV            0xff

#define SRIO_HOP_LOC                0xff

#define SRIO_MIN_COMP_TAG           0x55
#define SRIO_MAX_COMP_TAG           0x63

#define SRIO_HOST_DEV_TYE           0x55555555

#define SRIO_TSI578_DID             0x0578000d
#define SRIO_CP1848_DID             0x03740038

typedef struct tagSRIO_OW_S
{
	UINT32    uiDevId;
	UINT32    uiMemLoc;
    UINT32    uiMemTran;
	UINT32    uiSize;
}SRIO_OW_S;

typedef struct tagSRIO_IW_S
{
	UINT32    uiMemLoc;
    UINT32    uiMemTran;
	UINT32    uiSize;
}SRIO_IW_S;


typedef struct tagSRIO_PORT_S
{
	UINT32    uiDevId; 
	UINT32    uiDevType;
	UINT32    uiStatus;
}SRIO_PORT_S;

#define SRIO_MAX_NAME_LEN       8

typedef struct tagSRIO_DID_S
{
	UINT32    uiDevId;
	char      aucName[SRIO_MAX_NAME_LEN];
}SRIO_DID_S;

typedef enum tagSRIO_DISC_STATUS_E
{
    SRIO_DISC_FREE,
    SRIO_WAIT_DISC,
    SRIO_DISCED,
    SRIO_DISC_BUZZ
}SRIO_DISC_STATUS_E;

typedef enum tagSRIO_PORT_STATUS_E
{
    SRIO_PORT_DOWN,
    SRIO_PORT_RECOVER,
    SRIO_PORT_UP,
    SRIO_PORT_BUZZ
}SRIO_PORT_STATUS_E;

#define SRIO_SW_WAIT_CFG   0
#define SRIO_SW_CFG_OK     1

#define SRIO_ALLOCED       0
#define SRIO_NEW_ALLOC     1

typedef enum tagSRIO_DEV_TYPE_E
{
    SRIO_TYPE_BUZZ
}SRIO_DEV_TYPE_E;


#define SRIO_MASTER_SW_IDX   0xff
#define SRIO_INIT_HOSTID     0xff

typedef struct tagSRIO_SWITCH_S
{
    UINT32       uiIdx;
    UINT32       uiNewAlloc;
    UINT32       uiDiscStatus;
    UINT32       uiCfgStatus;
    UINT32       uiDID;
    UINT32       uiSWMap; /*switch id bitmap connect to this switch*/
    UINT32       uiUpPort;
    UINT32       uiUpSWIdx;
    UINT32       uiHop;
    UINT32       uiCompTag;
	SRIO_PORT_S  astPort[SRIO_MAX_PORT_PER_SW];
	UINT8        aucSWPort[SRIO_MAX_PORT_PER_SW];
	UINT8        aucRouteTable[SRIO_MAX_DEV_NUM];
}SRIO_SWITCH_S;

typedef struct tagSRIO_MPORT_S
{
    UINT32           uiHostId;
    UINT32           uiLocPort;

    SEM_ID           semDbSend;
    SEM_ID           semDbAccess;
    SEM_ID           semMsgSend[SRIO_MAX_MBOX];
    SEM_ID           semMsgAccess[SRIO_MAX_MBOX];
    UINT32           uiOWBitmap;
    UINT32           uiOWWin;
    UINT32           uiDownSwIdx;
	
    UINT32           uiSWMap;/*switch id bitmap belong to master port*/
    SRIO_OW_S        astOW[SRIO_MAX_OUB];
    UINT32           uiIWBitmap;
    SRIO_IW_S        astIW[SRIO_MAX_INB];
    SRIO_SWITCH_S    astSwitch[SRIO_MAX_SWITCH];

    UINT32          *puiDBMem;
	UINT8           *pucMsgMemAlloc;
    UINT8           *pucMsgOubMemAlloc;
	
    UINT8           *pucMsgMem;
    UINT8           *pucMsgOubMem;
}SRIO_MPORT_S;



typedef  VOID (*pfHook)(UINT8 src,UINT8 dst ,UINT16 info);

typedef  VOID (*msgHook)(UINT8 mailBox,UINT8 * msgPtr,UINT16 len);


STATUS rioDoorbellInit(VOID (*hook)(UINT8 src,UINT8 dst ,UINT16 info));

STATUS rioInitMsg(VOID (*hook)(UINT8 src,UINT8 dst ,UINT16 info));


VOID rioSystemDisplay(VOID);

STATUS rioSetup(VOID);

STATUS rioSendDoorbell(UINT8 targetID,UINT16 DBData);

UINT8 rioFindPortByDevId(UINT8 devId);

UINT8 rioFindDevIdByPort(UINT8 port);


#endif

