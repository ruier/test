/* vxbIntelAhciStorage.h - AHCI SATA disk controller header */

/*
 * Copyright (c) 2006-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,12aug10,sye  Add support for ICH8M, ICH10 controller, and 
                 update ID for ICH7M. (WIND00204517)
01j,10sep09,j_z  Code clean(WIND00184020).
01i,14aug08,j_z  Updated reg access method to VxBus access method
01h,14aug08,vf   Added support for Intel ICH9M controller
01g,11oct07,jks  Ported to VxBUS
01f,08Jun06,mdg  Removed cacheNoSnoop flag, as XBD buffers in 6.2
                 are cache safe, and it was not needed.
01e,25May06,mdg  Initial VxWorks 6.2 XBD version.
01d,03May06,mdg  Added initActive flag for use in COMRESET, and lowered
                 default watchdog.
01c,01May06,mdg  Added Error counters
01b,28Apr06,mdg  Added ahciRawio for mkboot.c support.
01a,04Apr06,mdg  written
*/

#ifndef __INCahciDrvh
#define __INCahciDrvh

#ifdef __cplusplus
extern "C" {
#endif


#ifndef _ASMLANGUAGE

#include <dosFsLib.h>
#include <blkIo.h>
#include <semLib.h>
#include <wdLib.h>
#include <drv/xbd/xbd.h>           /* XBD library header */
#include <drv/erf/erfLib.h>        /* event frame work library header */

enum
{
    PLTEX_VENDOR_ID     = 0x1c28,
	PLTEX_DEVICE_ID     = 0x0122,
};

enum
{
    AHCI_SRST_ON        = 1,
    AHCI_SRST_OFF       = 2,
    AHCI_NON_SEC_DATA   = 4,
    AHCI_ATAPI          = 8,
    AHCI_NCQ            = 16,
    AHCI_WAIT_SPINUP    = 32
};

#define AHCI_NAME  "intelAhciSata"

typedef struct ahciParams 
    {
    short config;                 /* general configuration */
    short cylinders;              /* number of cylinders */
    short removcyl;               /* number of removable cylinders */
    short heads;                  /* number of heads */
    short bytesTrack;             /* number of unformatted bytes/track */
    short bytesSec;               /* number of unformatted bytes/sector */
    short sectors;                /* number of sectors/track */
    short bytesGap;               /* minimum bytes in intersector gap */
    short bytesSync;              /* minimum bytes in sync field */
    short vendstat;               /* number of words of vendor status */
    char serial[20];              /* controller serial number */
    short type;                   /* controller type */
    short size;                   /* sector buffer size, in sectors */
    short bytesEcc;               /* ecc bytes appended */
    char rev[8];                  /* firmware revision */
    char model[40];               /* model name */
    short multiSecs;              /* RW multiple support. bits 7-0 ia max secs */
    short reserved48;             /* reserved */
    short capabilities;           /* capabilities */
    short reserved50;             /* reserved */
    short pioMode;                /* PIO data transfer cycle timing mode */
    short dmaMode;                /* single word DMA data transfer cycle timing */
    short valid;                  /* field validity */
    short currentCylinders;       /* number of current logical cylinders */
    short currentHeads;           /* number of current logical heads */
    short currentSectors;         /* number of current logical sectors / track */
    short capacity0;              /* current capacity in sectors */
    short capacity1;              /* current capacity in sectors */
    short multiSet;               /* multiple sector setting */
    short sectors0;               /* total number of user addressable sectors */
    short sectors1;               /* total number of user addressable sectors */
    short singleDma;              /* single word DMA transfer */
    short multiDma;               /* multi word DMA transfer */
    short advancedPio;            /* flow control PIO transfer modes supported */
    short cycletimeDma;           /* minimum multiword DMA transfer cycle time */
    short cycletimeMulti;         /* recommended multiword DMA cycle time */
    short cycletimePioNoIordy;    /* min PIO transfer cycle time wo flow ctl */
    short cycletimePioIordy;      /* min PIO transfer cycle time w IORDY */
    short reserved69;             /* reserved */
    short reserved70;             /* reserved */
    short pktCmdRelTime;          /* [71]Typical Time for Release after Packet */
    short servCmdRelTime;         /* [72]Typical Time for Release after SERVICE */
    short reservedPacketID[2];    /* reserved for packet id*/
    short queueDepth;             /* maximum queue depth - 1 */
    short sataCapabilities;       /* [76] SATA capabilities */
    short reserved77;         
    short sataFeaturesSupported;  /* [78] SATA features Supported */
    short sataFeaturesEnabled;    /* [79] SATA features Enabled */
    short majorRevNum;            /* Major ata version number */
    short minorVersNum;           /* minor version number */
    short featuresSupported0;     /* [82] Features Supported word 0 */
    short featuresSupported1;     /* [83] Features Supported word 1 */
    short featuresSupported2;     /* [84] Features Supported word 2 */
    short featuresEnabled0;       /* [85] Features Enabled word 0 */
    short featuresEnabled1;       /* [86] Features Enabled word 1 */
    short featuresEnabled2;       /* [87] Features Enabled word 2 */
    short ultraDma;               /* ultra DMA transfer */
    short timeErase;              /* time to perform security erase */
    short timeEnhancedErase;      /* time to perform enhanced security erase */
    short currentAPM;             /* current power management value*/
    short passwordRevisionCode;   /* master password revision code */
    short resetResult;            /* result of last reset */
    short acousticManagement;     /* current and recommended acoustic management values */
    short reserved95[5];          /* reserved */
    short lba48Size[4];           /* 48-bit LBA size (stored little endian?) */
    short reserved104[23];        /* reserved */
    short removableFeatureSupport;/* removable media status notification feature set support */
    short securityStatus;         /* security status */
    short vendor[31];             /* vendor specific */
    short cfaPowerMode1;          /* */
    short reserved161[15];        /* reserved for assignment by Compact Flash Association */
    short currentMediaSN[30];     /* Current Media Serial Number */
    short reserved206[49];        /* reserved */
    short checksum;               /* integrity word */
    } AHCI_PARAM;

#define AHCI_MAX_CTRLS            6  /* max number of ATA controller */
#define AHCI_MAX_DRIVES           32 /* max number of SATA drives/controller */
#define AHCI_MAX_CMD_SLOTS        32 /* max number of command slots per port */
#define AHCI_MAX_PRD_ENTRIES      16
#define AHCI_ATAPI_MAX_CMD_LENGTH 16 /* maximum length in bytes of a ATAPI command */

/* Device types */

#define AHCI_TYPE_NONE      0x00    /* device is faulty or not present */
#define AHCI_TYPE_ATA       0x01    /* ATA device */
#define AHCI_TYPE_ATAPI     0x02    /* ATAPI device */
#define AHCI_TYPE_INIT      255     /* device must be identified */

/* Device  states */

#define AHCI_DEV_OK         0       /* device is OK */
#define AHCI_DEV_NONE       1       /* device absent or does not respond */
#define AHCI_DEV_DIAG_F     2       /* device diagnostic failed */
#define AHCI_DEV_PREAD_F    3       /* read device parameters failed */
#define AHCI_DEV_MED_CH     4       /* medium have been changed */
#define AHCI_DEV_NO_BLKDEV  5       /* No block device available */
#define AHCI_DEV_INIT       255     /* uninitialized device */

typedef enum 
    {
    AHCI_NON_DATA,
    AHCI_OUT_DATA,
    AHCI_IN_DATA
    } AHCI_DIR;

typedef struct ahciPrd
    {
    UINT32 dataBaseAddressLow;
    UINT32 dataBaseAddressHigh;
    UINT32 reserved;
    UINT32 dataByteCountInterrupt;
    } AHCI_PRD;

typedef struct ahciCmdTable
    {
    UINT8    commandFis[64];
    UINT8    atapiCommand[16];
    UINT8    reserved[48];
    AHCI_PRD prd[AHCI_MAX_PRD_ENTRIES];
    } AHCI_CMD_TABLE;

typedef struct ahciCmdList
    {
    UINT32 flagsPrdLength;
    UINT32 recvByteCount;
    UINT32 cmdTableAddressLow;
    UINT32 cmdTableAddressHigh;
    UINT32 reserved0;
    UINT32 reserved1;
    UINT32 reserved2;
    UINT32 reserved3;
    } AHCI_CMD_LIST; 

typedef struct ahciRecvFis
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
    } AHCI_RECV_FIS;

typedef struct ahciCmd{
    union 
        {
        struct 
            {
            UINT8   ataCommand;
            UINT16  ataFeature;
            UINT16  ataCount;
            UINT64  ataLba;
            } ata;

        struct
            {
            UINT8   atapiCommand;
            UINT8   atapiFeature;
            UINT8   cmdPkt[AHCI_ATAPI_MAX_CMD_LENGTH];
            } atapi; 
        } cmd;

    UINT32      bufLength;
    AHCI_DIR    direction;
    UINT8       *data;
    int         flags;
} AHCI_CMD;

#if _BYTE_ORDER == _BIG_ENDIAN
#   define AHCI_SWAP(x) LONGSWAP(x)
#else
#   define AHCI_SWAP(x) (x)
#endif

#ifndef VXB_ADDR_LOW32
#define VXB_ADDR_LOW32(x)      (UINT32) (x)
#endif

#ifndef VXB_ADDR_HIGH32
#define VXB_ADDR_HIGH32(x)      0
#endif

/* define AHCI_XBD structure/typedef so the xbd functions can find the
 * pointer to the device structure ataDev.  This is a locally defined
 * structure that is passed to the xbd routines.  The first entity is
 * XBD structure - and this must always be the first entity.  Other external
 * routines will modify this part.  The rest of the structure can
 * be ata driver specific.
 */

typedef struct ahci_xbd
    {
    XBD        xbd;                 /* must be first, add new items below here */
    SEM_ID     xbdSemId;            /* used for xbdBlkDevCreateSync() */
    devname_t  name;                /* name of device            */
    device_t   device;              /* device number of this instance */
    UINT32     xbdInserted;         /* device is inserted */
    BOOL       xbdInstantiated;     /* TRUE if stack init is complete */
    struct ahciDev *ahciDev;        /* necessary to access the device structure */
    } AHCI_XBD;

typedef struct ahciDev
    {
    struct ahciDrive  *pDrive;            
    BLK_DEV     blkDev;             /* must be here */
    AHCI_XBD    ahciXbd;            /* actual xbd for this device */
    int         ctrl;               /* ctrl no.  0 - 1 */
    int         drive;              /* drive no. 0 - 1 */
    int         blkOffset;          /* sector offset */
    int         nBlocks;            /* number of sectors */

    char *      pBuf;               /* Current position in an user buffer */
    char *      pBufEnd;            /* End of user buffer */
    AHCI_DIR    direction;          /* Transfer direction */
    int         transCount;         /* Number of transfer cycles */
    int         errNum;             /* Error description message number */

    /* ATAPI Registers contents */

    UINT8 intReason;                /* Interrupt Reason Register */
    UINT8 status;                   /* Status Register */
    UINT16 transSize;               /* Byte Count Register */
    } AHCI_DEV;

typedef struct ahciDrive
    {
    void *          regsAddr;
    UINT32          portNum;
    struct ahciDrvCtrl *pCtrl;
    AHCI_CMD_LIST   *commandList;
    AHCI_RECV_FIS   *recvFis;
    AHCI_CMD_TABLE  *commandTable;
    SEM_ID          syncSem[AHCI_MAX_CMD_SLOTS];
    SEM_ID          muteSem;
    SEM_ID          xbdMuteSem;
    SEM_ID          tagMuteSem;
    SEM_ID          queueSlotSem;
    SEM_ID          monSyncSem;
    int             svcTaskId[AHCI_MAX_CMD_SLOTS]; /* id of xbd service tasks for this drive */
    SEM_ID          ahciBioReadySem;/* bio queue counting semaphore */ 
    struct bio      *bioQueueh;     /* bio queue head for drive   */
    struct bio      *bioQueuet;     /* bio queue tail for drive   */
    UINT32          cmdStarted;
    AHCI_PARAM      param;          /* geometry parameter */
    BOOL            okMulti;        /* MULTI: TRUE if supported */
    BOOL            okIordy;        /* IORDY: TRUE if supported */
    BOOL            okDma;          /* DMA:   TRUE if supported */
    BOOL            okLba;          /* LBA:   TRUE if supported */
    BOOL            okLba48;        /* 48-bit LBA: TRUE if supported (allows 16 bit sector counts for transfers) */
    short           okNcq;          /* NCQ:   TRUE if supported */
    short           multiSecs;      /* supported max sectors for multiple RW */
    short           pioMode;        /* supported max PIO mode */
    short           singleDmaMode;  /* supported max single word DMA mode */
    short           multiDmaMode;   /* supported max multi word DMA mode */
    short           ultraDmaMode;   /* supported max ultra DMA mode */
    short           rwMode;         /* RW mode: PIO[0,1,2,3,4] or DMA[0,1,2] */
    short           rwPio;          /* RW PIO unit: single or multi sector */
    int             rwDma;          /* RW DMA unit: single, multi word, or ultra */
    int             cylinders;            /* number of cylinders */
    int             heads;                /* number of heads */
    int             sectors;              /* number of sectors per track */
    int             bytes;                /* number of bytes per sector */
    int             intCount;             /* interrupt count */
    UINT32          intStatus;            /* interrupt status */
    UINT32          intError;             /* interrupt status */
    int             taskFileErrorCount;   /* Error counters*/
    int             timeoutErrorCount;    /* Error counters */
    WDOG_ID         wdgId;                /* watch dog */
    BOOL            portError;
    BOOL            wdgOkay;
    int             nextTag;
    BOOL            queuedMode;
    int             queueDepth;
    UINT32          maxSectors;
    UINT8           *alignedDmaBuffer;
    UINT8           state;
    UINT8           type;
    UINT32          initActive;
    int             pmState;    /* Power Management State */
    STATUS          (*Reset) (struct ahciDrvCtrl *, int drive);
    AHCI_DEV        *pAhciDev;  /* pointer to ATA block device structure */
    } AHCI_DRIVE;

typedef struct ahciDrvCtrl
    {
    VXB_DEVICE_ID  pDev;
    UINT16         ahciDevId;
    void *         ahciBar;
    void *         regHandle;
    int  *         pAhciTypes;
    int            useBlockWrapper;
    BOOL           installed;
    int            numPorts;
    int            numImpPorts;
    int            numCommandSlots;
    BOOL           okNcq;
    AHCI_DRIVE     *drives;
    BOOL           wdgOkay;       /* watch dog status */
    int            semTimeout;    /* timeout ticks for sync semaphore */
    int            wdgTimeout;    /* timeout ticks for watch dog */
    int            *configType;   /* or'd configuration flags     */
    int            svcTaskCount;  /* number of service tasks per drive */
    UINT32         ahciLbaTotalSecs[AHCI_MAX_DRIVES]; /* hold LBA information */
    UINT64         ahciLba48TotalSecs[AHCI_MAX_DRIVES];
    } AHCI_DRV_CTRL;

typedef struct ahciRaw
    {                   /* this is for ATARAWACCESS ioctl */
    UINT cylinder;      /* cylinder (0 -> (cylindres-1)) */
    UINT head;          /* head (0 -> (heads-1)) */
    UINT sector;        /* sector (1 -> sectorsTrack) */
    char *pBuf;         /* pointer to buffer (bytesSector * nSecs) */
    UINT nSecs;         /* number of sectors (1 -> sectorsTrack) */
    UINT direction;     /* read=0, write=1 */
    } AHCI_RAW;

typedef struct vxbAhciMsg
    {
     AHCI_DRV_CTRL * pCtrl;      /* AHCI controller structure pointer */
     char drive;                 /* drive number */
     char msgId;                 /* message ID */
    } VXB_AHCI_MSG;

#define VXB_AHCI_MSG_SIZE	sizeof(VXB_AHCI_MSG)

/* AHCI SATA Controller Generic Host Control Register Offsets */

#define AHCI_CAP   0x00   /* Host Capabilities */
#define AHCI_GHC   0x04   /* Global Host Control */
#define AHCI_IS    0x08   /* Interrupt Status */
#define AHCI_PI    0x0C   /* Ports Implemented */
#define AHCI_VS    0x10   /* Version */

/* AHCI SATA Controller Generic Host Control Register Offsets */

#define AHCI_PxCLB  0x00  /* Port x Command List Base Address Lower 32bits */
#define AHCI_PxCLBU 0x04  /* Port x Command List Base Address Upper 32bits */
#define AHCI_PxFB   0x08  /* Port x FIS Base Address Lower 32bits */
#define AHCI_PxFBU  0x0C  /* Port x FIS Base Address Upper 32bits */
#define AHCI_PxIS   0x10  /* Port x Interrupt Status */
#define AHCI_PxIE   0x14  /* Port x Interrupt Enable */
#define AHCI_PxCMD  0x18  /* Port x Command and Status */
#define AHCI_PxTFD  0x20  /* Port x Task File Data */
#define AHCI_PxSIG  0x24  /* Port x Signature */
#define AHCI_PxSSTS 0x28  /* Port x Serial ATA Status */
#define AHCI_PxSCTL 0x2C  /* Port x Serial ATA Control */
#define AHCI_PxSERR 0x30  /* Port x Serial ATA Error */
#define AHCI_PxSACT 0x34  /* Port x Serial ATA Active */
#define AHCI_PxCI   0x38  /* Port x Command Issue */
#define AHCI_PxSNTF 0x3C  /* Port x SNotification */
#define AHCI_PxVS   0x70  /* Port x Vendor Specific */

/* Register access macros */

#define CTRL_REG_READ(pCtrl,offset)					\
    vxbRead32 (pCtrl->regHandle,					\
	       (UINT32 *)((ULONG)(pCtrl)->ahciBar + offset))
#define CTRL_REG_WRITE(pCtrl,offset,value)				\
    vxbWrite32 (pCtrl->regHandle,					\
		(UINT32 *)((ULONG)(pCtrl)->ahciBar + offset), value)
#define PORT_REG_READ(port,offset)					\
    vxbRead32 (port->pCtrl->regHandle,					\
	       (UINT32 *)((ULONG)(port)->regsAddr + offset)) 
#define PORT_REG_WRITE(port,offset,value)				\
    vxbWrite32 (port->pCtrl->regHandle,					\
		(UINT32 *)((ULONG)(port)->regsAddr + offset),value)

/* AHCI Host Capabilities Bit Mask Definitions */

#define AHCI_CAP_S64A  0x80000000 /* Supports 64Bit Addressing */
#define AHCI_CAP_SNCQ  0x40000000 /* Supports NCQ */
#define AHCI_CAP_SSNTF 0x20000000 /* Supports SNotification register */
#define AHCI_CAP_SMPS  0x10000000 /* Supports Mechanical Presence Switch */
#define AHCI_CAP_SSS   0x08000000 /* Supports Staggered Spin Up */
#define AHCI_CAP_SALP  0x04000000 /* Supports Agressive Link Power Manage */
#define AHCI_CAP_SAL   0x02000000 /* Supports Activity LED */
#define AHCI_CAP_SCLO  0x01000000 /* Supports Command List Override */
#define AHCI_CAP_ISS   0x00F00000 /* Interface Speed Support */
#define AHCI_CAP_ISS_SHFT 20
#define AHCI_CAP_SNZO  0x00080000 /* Supports Non-Zero DMA Offsets */
#define AHCI_CAP_SAM   0x00040000 /* Supports ACHI mode only */
#define AHCI_CAP_SPM   0x00020000 /* Supports Port Multiplier */
#define AHCI_CAP_FBSS  0x00010000 /* Supports FIS Based Switching */
#define AHCI_CAP_PMD   0x00008000 /* PIO Multiple DRQ Block */
#define AHCI_CAP_SSC   0x00004000 /* Slumber State Capable */
#define AHCI_CAP_PSC   0x00002000 /* Parial State Capable */
#define AHCI_CAP_NCS   0x00001F00 /* Number of Command Slots per port */
#define AHCI_CAP_NCS_SHFT 8
#define AHCI_CAP_CCCS  0x00000080 /* Supports Command Completion Coalescing */
#define AHCI_CAP_EMS   0x00000040 /* Supports Enclosure Management */
#define AHCI_CAP_SXS   0x00000020 /* Supports External SATA */
#define AHCI_CAP_NP    0x0000001F /* Number of Ports */

/* AHCI Global Host Control Bit Mask Definitions */

#define AHCI_GHC_AE    0x80000000 /* AHCI Enable */
#define AHCI_GHC_IE    0x00000002 /* Interrupt Enable */
#define AHCI_GHC_HR    0x00000001 /* Controller Reset */

/* AHCI Port Interrupt Status Bit Mask Definitions */

#define AHCI_PIS_CPDS  0x80000000 /* Cold Port Detect Status */
#define AHCI_PIS_TFES  0x40000000 /* Task File Error Status */
#define AHCI_PIS_HBFS  0x20000000 /* Host Bus Fatal Error Status */
#define AHCI_PIS_HBDS  0x10000000 /* Host Bus Data Error Status */
#define AHCI_PIS_IFS   0x08000000 /* Interface Fatal Error Status */
#define AHCI_PIS_INFS  0x04000000 /* Interface Non-Fatal Error Status */
#define AHCI_PIS_OFS   0x01000000 /* Overflow Status */
#define AHCI_PIS_IPMS  0x00800000 /* Incorrect Port Multiplier Status */
#define AHCI_PIS_PRCS  0x00400000 /* PhyDry Change Status */
#define AHCI_PIS_DPMS  0x00000080 /* Device Mechanical Presence Status */
#define AHCI_PIS_PCS   0x00000040 /* Port Connect Change Status */
#define AHCI_PIS_DPS   0x00000020 /* Descriptor Processed Status */
#define AHCI_PIS_UFS   0x00000010 /* Umknown FIS Interrupt Status */
#define AHCI_PIS_SDBS  0x00000008 /* Set Device Bits Interrupt Status */
#define AHCI_PIS_DSS   0x00000004 /* DMA Setup FIS Interrupt Status */
#define AHCI_PIS_PSS   0x00000002 /* PIO Setup FIS Interrupt Status */
#define AHCI_PIS_DHRS  0x00000001 /* Device to Host Register FIS Status */

/* AHCI Port Interrupt Enable Bit Mask Definitions */

#define AHCI_PIE_CPDE  0x80000000 /* Cold Port Detect Enable */
#define AHCI_PIE_TFEE  0x40000000 /* Task File Error Enable */
#define AHCI_PIE_HBFE  0x20000000 /* Host Bus Fatal Error Enable */
#define AHCI_PIE_HBDE  0x10000000 /* Host Bus Data Error Enable */
#define AHCI_PIE_IFE   0x08000000 /* Interface Fatal Error Enable */
#define AHCI_PIE_INFE  0x04000000 /* Interface Non-Fatal Error Enable */
#define AHCI_PIE_OFE   0x01000000 /* Overflow Enable */
#define AHCI_PIE_IPME  0x00800000 /* Incorrect Port Multiplier Enable */
#define AHCI_PIE_PRCE  0x00400000 /* PhyDry Change Enable */
#define AHCI_PIE_DPME  0x00000080 /* Device Mechanical Presence Enable */
#define AHCI_PIE_PCE   0x00000040 /* Port Connect Change Enable */
#define AHCI_PIE_DPE   0x00000020 /* Descriptor Processed Enable */
#define AHCI_PIE_UFE   0x00000010 /* Unknown FIS Interrupt Enable */
#define AHCI_PIE_SDBE  0x00000008 /* Set Device Bits Interrupt Enable */
#define AHCI_PIE_DSE   0x00000004 /* DMA Setup FIS Interrupt Enable */
#define AHCI_PIE_PSE   0x00000002 /* PIO Setup FIS Interrupt Enable */
#define AHCI_PIE_DHRE  0x00000001 /* Device to Host Register FIS Enable */

/* AHCI Port Command and Status Bit Mask Definitions */

#define AHCI_PCMD_ICC   0xF0000000 /* Interface Communication Control */
#define AHCI_PCMD_ICC_A 0x10000000 /* Active */
#define AHCI_PCMD_ICC_P 0x20000000 /* Partial */
#define AHCI_PCMD_ICC_S 0x60000000 /* Slumber */
#define AHCI_PCMD_ICC_SHFT 28
#define AHCI_PCMD_ASP   0x08000000 /* Aggressive Slumber/Partial */
#define AHCI_PCMD_ALPE  0x04000000 /* Aggressive Link Power Manage Enable */
#define AHCI_PCMD_DLAE  0x02000000 /* Drive LED on ATAPI Enable */
#define AHCI_PCMD_ATAPI 0x01000000 /* Device is ATAPI */
#define AHCI_PCMD_ESP   0x00200000 /* External SATA Port*/
#define AHCI_PCMD_CPD   0x00100000 /* Cold Presence Detection */
#define AHCI_PCMD_MPSP  0x00080000 /* Mechanical Presence Switch Attached */
#define AHCI_PCMD_HPCP  0x00040000 /* Hot Plug Capable Port */
#define AHCI_PCMD_PMA   0x00020000 /* Port Multiplier Attached */
#define AHCI_PCMD_CPS   0x00010000 /* Cold Presence State */
#define AHCI_PCMD_CR    0x00008000 /* Command List Running */
#define AHCI_PCMD_FR    0x00004000 /* FIS Receive Running */
#define AHCI_PCMD_MPSS  0x00002000 /* Mechanical Presence Switch */
#define AHCI_PCMD_CCS   0x00001F00 /* Current Command Slot */
#define AHCI_PCMD_CCS_SHFT 8
#define AHCI_PCMD_FRE   0x00000010 /* FIS Receive Enable */
#define AHCI_PCMD_CLO   0x00000008 /* Command List Override */
#define AHCI_PCMD_POD   0x00000004 /* Power On Device */
#define AHCI_PCMD_SUD   0x00000002 /* Spin-Up Device */
#define AHCI_PCMD_ST    0x00000001 /* Start */

/* AHCI Port Serial ATA Status Bit Mask Definitions */

#define AHCI_PSSTS_IPM_MSK       0x00000F00 /* Interface Power Management */
#define AHCI_PSSTS_IPM_NO_DEVICE 0x00000000
#define AHCI_PSSTS_IPM_ACTIVE    0x00000100
#define AHCI_PSSTS_IPM_PARTIAL   0x00000200
#define AHCI_PSSTS_IPM_SLUMBER   0x00000600
#define AHCI_PSSTS_SPD_MSK       0x000000F0 /* Current Interface Speed */
#define AHCI_PSSTS_SPD_NO_DEVICE 0x00000000
#define AHCI_PSSTS_SPD_GEN1      0x00000010
#define AHCI_PSSTS_SPD_GEN2      0x00000020
#define AHCI_PSSTS_DET_MSK       0x0000000F /* Device Detection */
#define AHCI_PSSTS_DET_NO_DEVICE 0x00000000
#define AHCI_PSSTS_DET_NO_PHY    0x00000001
#define AHCI_PSSTS_DET_PHY       0x00000003
#define AHCI_PSSTS_DET_PHY_OFF   0x00000004

/* AHCI Port Serial ATA Control Bit Mask Definitions */

#define AHCI_PSCTL_IPM_MSK         0x00000F00 /* Interface Power Management */
#define AHCI_PSCTL_IPM_NO_RESTRICT 0x00000000 
#define AHCI_PSCTL_IPM_NO_PARTIAL  0x00000100 
#define AHCI_PSCTL_IPM_NO_SLUMBER  0x00000200 
#define AHCI_PSCTL_IPM_NO_PARSLUM  0x00000300
#define AHCI_PSCTL_SPD_MSK         0x000000F0 /* Speed Allowed */
#define AHCI_PSCTL_SPD_NO_RESTRICT 0x00000000 
#define AHCI_PSCTL_SPD_LIMIT_GEN1  0x00000010 
#define AHCI_PSCTL_SPD_LIMIT_GEN2  0x00000020 
#define AHCI_PSCTL_DET_MSK         0x0000000F /* Speed Allowed */
#define AHCI_PSCTL_DET_NO_ACTION   0x00000000 
#define AHCI_PSCTL_DET_RESET       0x00000001 
#define AHCI_PSCTL_DET_DISABLE     0x00000004 

/* AHCI Port Interrupt Enable Bit Mask Definitions */

#define AHCI_PSERR_DIAG_X   0x04000000 /* Exchanged */
#define AHCI_PSERR_DIAG_F   0x02000000 /* Unknown FIS Type */
#define AHCI_PSERR_DIAG_T   0x01000000 /* Transport State Transition Error */
#define AHCI_PSERR_DIAG_S   0x00800000 /* Link Sequence Error */
#define AHCI_PSERR_DIAG_H   0x00400000 /* Handshake Error */
#define AHCI_PSERR_DIAG_C   0x00200000 /* CRC Error */
#define AHCI_PSERR_DIAG_D   0x00100000 /* Disparity Error */
#define AHCI_PSERR_DIAG_B   0x00080000 /* 10B to 8B Decode Error */
#define AHCI_PSERR_DIAG_W   0x00040000 /* Comm Wake */
#define AHCI_PSERR_DIAG_I   0x00020000 /* Phy Internal Error*/
#define AHCI_PSERR_DIAG_N   0x00010000 /* Phy Rdy Change */
#define AHCI_PSERR_ERR_E    0x00000800 /* Internal Error */
#define AHCI_PSERR_ERR_P    0x00000400 /* Protocol Error */
#define AHCI_PSERR_ERR_C    0x00000200 /* Data Integrity Error */
#define AHCI_PSERR_ERR_T    0x00000100 /* Transient Data Integrity Error */
#define AHCI_PSERR_ERR_M    0x00000002 /* Recovered Communication Error */
#define AHCI_PSERR_ERR_I    0x00000001 /* Recoever Data Integrity Error */

/* AHCI Command List Bit Mask Definitions */

#define AHCI_CMD_LIST_PRDTL 0xFFFF0000 /* PDT Table Length (entries) */
#define AHCI_CMD_LIST_PRDTL_SHFT 16
#define AHCI_CMD_LIST_PMP   0x0000F000 /* Port Multiplier Port */
#define AHCI_CMD_LIST_PMP_SHFT   12
#define AHCI_CMD_LIST_C     0x00000400 /* Clear Busy upon R_OK */
#define AHCI_CMD_LIST_B     0x00000200 /* BIST FIS */
#define AHCI_CMD_LIST_R     0x00000100 /* Reset Command */
#define AHCI_CMD_LIST_P     0x00000080 /* Prefetchable */
#define AHCI_CMD_LIST_W     0x00000040 /* Write */
#define AHCI_CMD_LIST_A     0x00000020 /* ATAPI */
#define AHCI_CMD_LIST_CFL   0x0000001F /* Command Length (in 32 bit words */

/* AHCI PRD Bit Maks Definitions */

#define AHCI_PRD_I          0x80000000 /* Interrupt upon completion */
#define AHCI_PRD_MAX_BYTES  0x400000

#define AHCI_NCQ_MODE       0x1000      /* Native Command Queueing Mode */
#define AHCI_DMA_ULTRA      0x0c00      /* RW DMA ultra */
#define AHCI_DMA_AUTO       0x0017      /* DMA max supported mode */

#define AHCI_MODE_ALL       (AHCI_DMA_AUTO | AHCI_DMA_ULTRA | AHCI_NCQ_MODE)

/* default timeout for ATA sync sem */

#define AHCI_SEM_TIMEOUT_DEF     (5*sysClkRateGet())

/* default timeout for ATA watch dog */

#define AHCI_WDG_TIMEOUT_DEF     (sysClkRateGet()/2)

/* default for number of service tasks per drive */

#define AHCI_SVC_TASK_COUNT_DEF 1

#ifndef AHCI_SVC_TASK_COUNT
#define AHCI_SVC_TASK_COUNT 1 /* Number of service tasks per drive */
#endif

/* Monitor Task Message types */

#define AHCI_ATTACH_MSG     'A'
#define AHCI_REMOVE_MSG     'R'
#define AHCI_PORT_ERROR_MSG 'E'

/* diagnostic code */

#define AHCI_DIAG_OK        0x01

/* control register */

#define AHCI_CTL_4BIT       0x8 /* use 4 head bits (wd1003) */
#define AHCI_CTL_RST        0x4 /* reset controller */
#define AHCI_CTL_IDS        0x2 /* disable interrupts */

/* status register */

#define AHCI_STAT_ACCESS        (AHCI_STAT_BUSY | AHCI_STAT_DRQ)

/* device accessible */

#define AHCI_STAT_BUSY      0x80    /* controller busy */
#define AHCI_STAT_READY     0x40    /* selected drive ready */
#define AHCI_STAT_WRTFLT    0x20    /* write fault */
#define AHCI_STAT_SEEKCMPLT 0x10    /* seek complete */
#define AHCI_STAT_DRQ       0x08    /* data request */
#define AHCI_STAT_ECCCOR    0x04    /* ECC correction made in data */
#define AHCI_STAT_INDEX     0x02    /* index pulse from selected drive */
#define AHCI_STAT_ERR       0x01    /* error detect */

/* size/drive/head register: addressing mode CHS or LBA */

#define AHCI_SDH_IBM        0xa0    /* chs, 512 bytes sector, ecc */
#define AHCI_SDH_LBA        0xe0    /* lba, 512 bytes sector, ecc */

/* ATA commands to drives */

#define AHCI_CMD_RECALIB    0x10    /* recalibrate */
#define AHCI_CMD_SEEK       0x70    /* seek */
#define AHCI_CMD_READ       0x20    /* read sectors with retries */
#define AHCI_CMD_READ_EXT   0x24    /* read sectors with retries (48-bit LBA) */
#define AHCI_CMD_WRITE      0x30    /* write sectors with retries */
#define AHCI_CMD_WRITE_EXT  0x34    /* write sectors with retries (48-bit LBA) */
#define AHCI_CMD_FORMAT     0x50    /* format track */
#define AHCI_CMD_READ_FPDMA_QUEUED  0x60    /* read sector NCQ */
#define AHCI_CMD_WRITE_FPDMA_QUEUED 0x61    /* write sector NCQ */
#define AHCI_CMD_DIAGNOSE           0x90    /* execute controller diagnostic */
#define AHCI_CMD_INITP              0x91    /* initialize drive parameters */
#define AHCI_CMD_READP              0xEC    /* identify */
#define AHCI_CMD_SET_FEATURE        0xEF    /* set features */
#define AHCI_CMD_SET_MULTI          0xC6    /* set multi */
#define AHCI_CMD_READ_MULTI         0xC4    /* read multi */
#define AHCI_CMD_READ_MULTI_EXT     0x29    /* read multi (48-bit LBA) */
#define AHCI_CMD_READ_DMA           0xC8    /* read dma */
#define AHCI_CMD_READ_DMA_EXT       0x25    /* read dma (48-bit LBA) */
#define AHCI_CMD_WRITE_MULTI        0xC5    /* write multi */
#define AHCI_CMD_WRITE_MULTI_EXT    0x39    /* write multi (48-bit LBA) */
#define AHCI_CMD_WRITE_DMA          0xCA    /* write dma */
#define AHCI_CMD_WRITE_DMA_EXT      0x35    /* write dma (48-bit LBA) */
#define AHCI_CMD_CHECK_POWER_LEVEL  0xE5    /* Check Power Level */
#define AHCI_CMD_IDLE_IMMEDIATE     0xE1    /* Go to Idle State */
#define AHCI_CMD_STANDBY_IMMEDIATE  0xE0    /* Go to Standby State */
#define AHCI_CMD_SLEEP              0xE6    /* Go to Sleep State */
#define AHCI_CMD_DEVICE_RESET       0x08    /* Go to Sleep State */
#define AHCI_CMD_SMART              0xB0

/* SMART sub Commands */

#define AHCI_SMART_CMD_READ_DATA         0xD0
#define AHCI_SMART_CMD_READ_THRESHOLDS   0xD1
#define AHCI_SMART_CMD_EN_DIS_AUTOSAVE   0xD2
#define AHCI_SMART_CMD_SAVE_ATTRIBUTES   0xD3
#define AHCI_SMART_CMD_OFFLINE_DIAGS     0xD4
#define AHCI_SMART_CMD_ENABLE_SMART      0xD8
#define AHCI_SMART_CMD_DISABLE_SMART     0xD9
#define AHCI_SMART_CMD_RETURN_STATUS     0xDA

/* SMART Defines */

#define AHCI_SMART_OK                    0
#define AHCI_SMART_EXCEEDED              1

/* ATAPI commands */

#define AHCI_PI_CMD_SRST    0x08    /* ATAPI Soft Reset */
#define AHCI_PI_CMD_PKTCMD  0xA0    /* ATAPI Pakcet Command */
#define AHCI_PI_CMD_IDENTD  0xA1    /* ATAPI Identify Device */
#define AHCI_PI_CMD_SERVICE 0xA2    /* Service */

/* CDROM commands */

#define AHCI_CDROM_CMD_TEST_UNIT_READY  0x00     /* CDROM Test Unit Ready      */
#define AHCI_CDROM_CMD_REQUEST_SENSE    0x03     /* CDROM Request Sense        */
#define AHCI_CDROM_CMD_INQUIRY          0x12     /* CDROM Inquiry              */
#define AHCI_CDROM_CMD_READ_CDROM_CAP   0x25     /* CDROM Read CD-ROM Capacity */
#define AHCI_CDROM_CMD_READ_12          0xA8     /* CDROM Read (12)            */

/* sub command of AHCI_CMD_SET_FEATURE */

#define AHCI_SUB_ENABLE_8BIT    0x01    /* enable 8bit data transfer */
#define AHCI_SUB_ENABLE_WCACHE  0x02    /* enable write cache */
#define AHCI_SUB_SET_RWMODE     0x03    /* set transfer mode */
#define AHCI_SUB_ENABLE_APM     0x05    /* enable advanced power management */
#define AHCI_SUB_DISABLE_RETRY  0x33    /* disable retry */
#define AHCI_SUB_SET_LENGTH     0x44    /* length of vendor specific bytes */
#define AHCI_SUB_SET_CACHE      0x54    /* set cache segments */
#define AHCI_SUB_DISABLE_LOOK   0x55    /* disable read look-ahead feature */
#define AHCI_SUB_DISABLE_REVE   0x66    /* disable reverting to power on def */
#define AHCI_SUB_DISABLE_ECC    0x77    /* disable ECC */
#define AHCI_SUB_DISABLE_8BIT   0x81    /* disable 8bit data transfer */
#define AHCI_SUB_DISABLE_WCACHE 0x82    /* disable write cache */
#define AHCI_SUB_DISABLE_APM    0x85    /* disable advanced power management */
#define AHCI_SUB_ENABLE_ECC     0x88    /* enable ECC */
#define AHCI_SUB_ENABLE_RETRY   0x99    /* enable retries */
#define AHCI_SUB_ENABLE_LOOK    0xaa    /* enable read look-ahead feature */
#define AHCI_SUB_SET_PREFETCH   0xab    /* set maximum prefetch */
#define AHCI_SUB_SET_4BYTES     0xbb    /* 4 bytes of vendor specific bytes */
#define AHCI_SUB_ENABLE_REVE    0xcc    /* enable reverting to power on def */

/* bits in feature supported paramater word 82 */

#define AHCI_WCACHE_SUPPORTED  0x0040 /* Write Cache Supported */
#define AHCI_LOOK_SUPPORTED    0x0020 /* Read Look Ahead Supported */
#define AHCI_APM_SUPPORTED     0x0008 /* Advanced Power Management Supported */
#define AHCI_SMART_SUPPORTED   0x0001 /* SMART Supported */

/* transfer modes of AHCI_SUB_SET_RWMODE */

#define AHCI_PIO_DEF_W      0x00    /* PIO default trans. mode */
#define AHCI_PIO_DEF_WO     0x01    /* PIO default trans. mode, no IORDY */
#define AHCI_PIO_W_0        0x08    /* PIO flow control trans. mode 0 */
#define AHCI_PIO_W_1        0x09    /* PIO flow control trans. mode 1 */
#define AHCI_PIO_W_2        0x0a    /* PIO flow control trans. mode 2 */
#define AHCI_PIO_W_3        0x0b    /* PIO flow control trans. mode 3 */
#define AHCI_PIO_W_4        0x0c    /* PIO flow control trans. mode 4 */
#define AHCI_DMA_SINGLE_0   0x10    /* singleword DMA mode 0 */
#define AHCI_DMA_SINGLE_1   0x11    /* singleword DMA mode 1 */
#define AHCI_DMA_SINGLE_2   0x12    /* singleword DMA mode 2 */
#define AHCI_DMA_MULTI_0    0x20    /* multiword DMA mode 0 */
#define AHCI_DMA_MULTI_1    0x21    /* multiword DMA mode 1 */
#define AHCI_DMA_MULTI_2    0x22    /* multiword DMA mode 2 */
#define AHCI_DMA_ULTRA_0    0x40    /* ultra DMA mode 0 */
#define AHCI_DMA_ULTRA_1    0x41    /* ultra DMA mode 1 */
#define AHCI_DMA_ULTRA_2    0x42    /* ultra DMA mode 2 (UDMA/33) */
#define AHCI_DMA_ULTRA_3    0x43    /* ultra DMA mode 3 */
#define AHCI_DMA_ULTRA_4    0x44    /* ultra DMA mode 4 (UDMA/66) */
#define AHCI_DMA_ULTRA_5    0x45    /* ultra DMA mode 5 (UDMA/100) */
#define AHCI_DMA_ULTRA_6    0x46    /* ultra DMA mode 5 (UDMA/133) */

#define AHCI_MAX_RW_SECTORS         0x100   /* max sectors per transfer */
#define AHCI_MAX_RW_48LBA_SECTORS   0x10000 /* max sectors per transfer in 48-bit LBA mode */

/* configuration flags: transfer mode, bits, unit, geometry */

#define AHCI_PIO_DEF_0  AHCI_PIO_DEF_W  /* PIO default mode */
#define AHCI_PIO_DEF_1  AHCI_PIO_DEF_WO /* PIO default mode, no IORDY */
#define AHCI_PIO_0      AHCI_PIO_W_0    /* PIO mode 0 */
#define AHCI_PIO_1      AHCI_PIO_W_1    /* PIO mode 1 */
#define AHCI_PIO_2      AHCI_PIO_W_2    /* PIO mode 2 */
#define AHCI_PIO_3      AHCI_PIO_W_3    /* PIO mode 3 */
#define AHCI_PIO_4      AHCI_PIO_W_4    /* PIO mode 4 */
#define AHCI_PIO_AUTO       0x000d      /* PIO max supported mode */
#define AHCI_DMA_0          0x0010      /* DMA mode 0 */
#define AHCI_DMA_1          0x0011      /* DMA mode 1 */
#define AHCI_DMA_2          0x0012      /* DMA mode 2 */
#define AHCI_DMA_3          0x0013      /* DMA mode 3 */
#define AHCI_DMA_4          0x0014      /* DMA mode 4 */
#define AHCI_DMA_5          0x0015      /* DMA mode 5 */
#define AHCI_DMA_6          0x0016      /* DMA mode 6 */
#define AHCI_MODE_MASK      0x00FF      /* transfer mode mask */

#define AHCI_PIO_SINGLE     0x0100      /* RW PIO single sector */
#define AHCI_PIO_MULTI      0x0200      /* RW PIO multi sector */
#define AHCI_PIO_MASK       0x0300      /* RW PIO mask */

#define AHCI_DMA_SINGLE     0x0400      /* RW DMA single word */
#define AHCI_DMA_MULTI      0x0800      /* RW DMA multi word */
#define AHCI_DMA_MASK       0x0c00      /* RW DMA mask */

#define AHCI_NCQ_MASK       0x1000      /* Native Command Queueing Mask */

/****************************** ATAPI Devices ******************************/

/* config */

#define AHCI_CONFIG_PROT_TYPE       0xc000 /* Protocol Type */
#define AHCI_CONFIG_PROT_TYPE_ATAPI 0x8000 /* ATAPI */

#define AHCI_CONFIG_DEV_TYPE        0x1f00 /* Device Type */
#define AHCI_CONFIG_DEV_TYPE_CD_ROM 0x0500

#define AHCI_CONFIG_REMOVABLE       0x0080 /* Removable */

#define AHCI_CONFIG_PKT_TYPE        0x0060 /* CMD DRQ Type */
#define AHCI_CONFIG_PKT_TYPE_MICRO  0x0000 /* Microprocessor DRQ */
#define AHCI_CONFIG_PKT_TYPE_INTER  0x0020 /* Interrupt DRQ */
#define AHCI_CONFIG_PKT_TYPE_ACCEL  0x0040 /* Accelerated DRQ */

#define AHCI_CONFIG_PKT_SIZE        0x0003 /* Command Packet Size */
#define AHCI_CONFIG_PKT_SIZE_12     0x0000 /* 12 bytes */

/* capabilities */

#define AHCI_CAPABIL_DMA            0x0100  /* DMA Supported */
#define AHCI_CAPABIL_LBA            0x0200  /* LBA Supported */
#define AHCI_CAPABIL_IORDY_CTRL     0x0400  /* IORDY can be disabled */
#define AHCI_CAPABIL_IORDY          0x0800  /* IORDY Supported */
#define AHCI_CAPABIL_OVERLAP        0x2000  /* Overlap Operation Supported */

/* valid */

#define AHCI_FIELDS_VALID       0x0002

/* singleDma */
 
#define AHCI_SINGLEDMA_MODE     0xff00  /* 15-8: mode active */
#define AHCI_SINGLEDMA_SUPPORT  0x00ff  /* 7-0: modes supported */

/* multiDma */
 
#define AHCI_MULTIDMA_MODE      0xff00  /* 15-8: mode active */
#define AHCI_MULTIDMA_SUPPORT   0x00ff  /* 7-0: modes supported */

/* advPio */

#define AHCI_ADVPIO_MODE3       0x0001  /* The Device supports PIO Mode 3 */

/* ATAPI registers */

#define AHCI_ATAPI_DATA      pCtrl->data     /* AHCI_DATA (RW) data reg. (16 bits) */
#define AHCI_ATAPI_ERROR     pCtrl->error    /* AHCI_ERROR (R) error reg. */
#define AHCI_ATAPI_FEATURE   pCtrl->feature  /* AHCI_FEATURE (W) feature reg. */
#define AHCI_ATAPI_INTREASON pCtrl->seccnt   /* AHCI_SECCNT (R) interrupt reason */
#define AHCI_ATAPI_BCOUNT_LO pCtrl->cylLo    /* AHCI_CYL_LO (RW) byte count (low) */
#define AHCI_ATAPI_BCOUNT_HI pCtrl->cylHi    /* AHCI_CYL_HI (RW) byte count (high) */
#define AHCI_ATAPI_D_SELECT  pCtrl->sdh      /* AHCI_SDH (RW) drive select reg. */
#define AHCI_ATAPI_STATUS    pCtrl->status   /* AHCI_STATUS (R) status reg. */
#define AHCI_ATAPI_COMMAND   pCtrl->command  /* AHCI_COMMAND (W) command reg. */
#define AHCI_ATAPI_D_CONTROL pCtrl->dControl /* AHCI_D_CONTROL (W) device control */

/* Error Register */

#define AHCI_ERR_SENSE_KEY           0xf0    /* Sense Key mask */
#define AHCI_SENSE_NO_SENSE          0x00    /* no sense sense key */
#define AHCI_SENSE_RECOVERED_ERROR   0x10    /* recovered error sense key */
#define AHCI_SENSE_NOT_READY         0x20    /* not ready sense key */
#define AHCI_SENSE_MEDIUM_ERROR      0x30    /* medium error sense key */
#define AHCI_SENSE_HARDWARE_ERROR    0x40    /* hardware error sense key */
#define AHCI_SENSE_ILLEGAL_REQUEST   0x50    /* illegal request sense key */
#define AHCI_SENSE_UNIT_ATTENTION    0x60    /* unit attention sense key */
#define AHCI_SENSE_DATA_PROTECT      0x70    /* data protect sense key */
#define AHCI_SENSE_ABBORTED_COMMAND  0xb0    /* aborted command sense key */
#define AHCI_SENSE_MISCOMPARE        0xe0    /* miscompare sense key */
#define AHCI_ERR_MCR                 0x08    /* Media Change Requested */
#define AHCI_ERR_ABRT                0x04    /* Aborted command */
#define AHCI_ERR_EOM                 0x02    /* End Of Media */
#define AHCI_ERR_ILI                 0x01    /* Illegal Length Indication */

/* Feature Register */

#define AHCI_FEAT_OVERLAP    0x02    /* command may be overlapped */
#define AHCI_FEAT_DMA        0x01    /* data will be transferred via DMA */

/* Interrupt Reason Register */

#define AHCI_INTR_RELEASE    0x04    /* Bus released before completing the command */
#define AHCI_INTR_IO         0x02    /* 1 - In to the Host; 0 - Out to the device */
#define AHCI_INTR_COD        0x01    /* 1 - Command; 0 - user Data */

/* Drive Select Register */

#define AHCI_DSEL_FILLER     0xa0    /* to fill static fields */
#define AHCI_DSEL_DRV        0x10    /* Device 0 (DRV=0) or 1 (DRV=1) */

/* Status Register */

#define AHCI_STAT_BUSY       0x80    /* controller busy */
#define AHCI_STAT_READY      0x40    /* selected drive ready */

#define AHCI_STAT_DMA_READY  0x20    /* ready to a DMA data transfer */

#define AHCI_STAT_WRTFLT     0x20    /* write fault */

#define AHCI_STAT_SERVICE    0x10    /* service or interrupt request */

#define AHCI_STAT_SEEKCMPLT  0x10    /* seek complete */
#define AHCI_STAT_DRQ        0x08    /* data request */
#define AHCI_STAT_ECCCOR     0x04    /* ECC correction made in data */
#define AHCI_STAT_ERR        0x01    /* error detect */

/* Device Control Register */

#define AHCI_CTL_FILLER  0x8     /* bit 3 must be always set */
#define AHCI_CTL_RST     0x4     /* reset controller */
#define AHCI_CTL_IDS     0x2     /* disable interrupts */

/* Power Management States */

#define AHCI_PM_ACTIVE_IDLE  0
#define AHCI_PM_STANDBY      1
#define AHCI_PM_SLEEP        2


/* ATA Ioctl function codes */

#define AHCIIOAPMENABLE              0x100
#define AHCIIOAPMDISABLE             0x101
#define AHCIIOAPMGET                 0x102
#define AHCIIOCHECKPOWERLEVEL        0x103
#define AHCIIOPMIDLE                 0x104
#define AHCIIOPMSTANDBY              0x105
#define AHCIIOPMSLEEP                0x106
#define AHCIIOPMWAKE                 0x107
#define AHCIIOSMARTENABLE            0x108
#define AHCIIOSMARTDISABLE           0x109
#define AHCIIOSMARTSAVEATTR          0x10A
#define AHCIIOSMARTISENABLED         0x10B
#define AHCIIOSMARTAUTOSAVEENABLE    0x10C
#define AHCIIOSMARTAUTOSAVEDISABLE   0x10D
#define AHCIIOSMARTOFFLINEDIAG       0x10E
#define AHCIIOSMARTRETURNSTATUS      0x10F
#define AHCIIOSMARTREADDATA          0x110
#define AHCIIOSMARTREADTHRESHOLDS    0x111
#define AHCIIOPARMSGET               0x112
#define AHCIIOMAXSECTORXFERSET       0x113
#define AHCIIOMAXSECTORXFERGET       0x114
#define AHCIIODRIVEHALT              0x115
#define AHCIIODRIVESTOP              0x116
#define AHCIIODRIVESTART             0x117

IMPORT STATUS ahciCtrlShow(VXB_DEVICE_ID pDev);
IMPORT STATUS ahciShow(VXB_DEVICE_ID pDev, int drive);
IMPORT STATUS ahciSmartShow(VXB_DEVICE_ID pDev, int drive);

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCahciDrvh */
