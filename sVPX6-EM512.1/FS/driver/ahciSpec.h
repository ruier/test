#ifndef __SAS_SPEC_H__
#define __SAS_SPEC_H__

#define SAS_ATA_FIS_H2D               0x27
#define SAS_ATA_FIS_D2H               0x34
#define SAS_ATA_FIS_SDB               0xa1
#define SAS_ATA_FIS_DMA_ACTIVE        0x39


#define AHCI_PORT_OFFSET(PortNum) (0x100 + (0x80*(PortNum)))

#define AHCI_CAP             0x00
#define AHCI_GHC             0x04
#define AHCI_IS              0x08
#define AHCI_PI              0x0C
#define AHCI_VS              0x10
#define AHCI_CCC_CTL         0x14
#define AHCI_CCC_PORTS       0x18
#define AHCI_EM_LOC          0x1C
#define AHCI_EM_CTL          0x20
#define AHCI_CAP2            0x24
#define AHCI_BOHC            0x28
#define AHCI_PxCLB           0x00
#define AHCI_PxCLBU          0x04
#define AHCI_PxFB            0x08
#define AHCI_PxFBU           0x0C
#define AHCI_PxIS            0x10
#define AHCI_PxIE            0x14
#define AHCI_PxCMD           0x18
#define AHCI_PxTFD           0x20
#define AHCI_PxSIG           0x24
#define AHCI_PxSSTS          0x28
#define AHCI_PxSCTL          0x2C
#define AHCI_PxSERR          0x30
#define AHCI_PxSACT          0x34
#define AHCI_PxCI            0x38
#define AHCI_PxSNTF          0x3C
#define AHCI_PxFBS           0x40
#define AHCI_PxVS_0          0x70
#define AHCI_PxVS_1          0x74
#define AHCI_PxVS_2          0x78
#define AHCI_PxVS_3          0x7C

#define AHCI_GHC_AE    0x80000000 /* AHCI Enable */
#define AHCI_GHC_IE    0x00000002 /* Interrupt Enable */
#define AHCI_GHC_HR    0x00000001 /* Controller Reset */

#define AHCI_SDH_IBM        0xa0    /* chs, 512 bytes sector, ecc */
#define AHCI_SDH_LBA        0xe0    /* lba, 512 bytes sector, ecc */

/* control register */

#define AHCI_CTL_4BIT       0x8 /* use 4 head bits (wd1003) */
#define AHCI_CTL_RST        0x4 /* reset controller */
#define AHCI_CTL_IDS        0x2 /* disable interrupts */


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

enum 
{
  AHCI_MAX_PORTS    = 32,
  AHCI_MAX_SG   = 168, /* hardware max is 64K */
  AHCI_DMA_BOUNDARY = 0xffffffff,
  AHCI_MAX_CMDS   = 32,
  AHCI_CMD_SZ   = 32,
  AHCI_CMD_SLOT_SZ  = AHCI_MAX_CMDS * AHCI_CMD_SZ,
  AHCI_RX_FIS_SZ    = 256,
  AHCI_CMD_TBL_CDB  = 0x40,
  AHCI_CMD_TBL_HDR_SZ = 0x80,
  AHCI_CMD_TBL_SZ   = AHCI_CMD_TBL_HDR_SZ + (AHCI_MAX_SG * 16),
  AHCI_CMD_TBL_AR_SZ  = AHCI_CMD_TBL_SZ * AHCI_MAX_CMDS,
  AHCI_PORT_PRIV_DMA_SZ = AHCI_CMD_SLOT_SZ + AHCI_CMD_TBL_AR_SZ +
          AHCI_RX_FIS_SZ,
  AHCI_PORT_PRIV_FBS_DMA_SZ = AHCI_CMD_SLOT_SZ +
            AHCI_CMD_TBL_AR_SZ +
            (AHCI_RX_FIS_SZ * 16),
  AHCI_IRQ_ON_SG    = (1 << 31),
  AHCI_CMD_ATAPI    = (1 << 5),
  AHCI_CMD_WRITE    = (1 << 6),
  AHCI_CMD_PREFETCH = (1 << 7),
  AHCI_CMD_RESET    = (1 << 8),
  AHCI_CMD_CLR_BUSY = (1 << 10),

  RX_FIS_D2H_REG    = 0x40, /* offset of D2H Register FIS data */
  RX_FIS_SDB    = 0x58, /* offset of SDB FIS data */
  RX_FIS_UNK    = 0x60, /* offset of Unknown FIS data */

 /* global controller registers */
  HOST_CAP    = 0x00, /* host capabilities */
  HOST_CTL    = 0x04, /* global host control */
  HOST_IRQ_STAT   = 0x08, /* interrupt status */
  HOST_PORTS_IMPL   = 0x0c, /* bitmap of implemented ports */
  HOST_VERSION    = 0x10, /* AHCI spec. version compliancy */
  HOST_EM_LOC   = 0x1c, /* Enclosure Management location */
  HOST_EM_CTL   = 0x20, /* Enclosure Management Control */
  HOST_CAP2   = 0x24, /* host capabilities, extended */

  /* HOST_CTL bits */
  HOST_RESET    = (1 << 0),  /* reset controller; self-clear */
  HOST_IRQ_EN   = (1 << 1),  /* global IRQ enable */
  HOST_AHCI_EN    = (1 << 31), /* AHCI enabled */

  /* HOST_CAP bits */
  HOST_CAP_SXS    = (1 << 5),  /* Supports External SATA */
  HOST_CAP_EMS    = (1 << 6),  /* Enclosure Management support */
  HOST_CAP_CCC    = (1 << 7),  /* Command Completion Coalescing */
  HOST_CAP_PART   = (1 << 13), /* Partial state capable */
  HOST_CAP_SSC    = (1 << 14), /* Slumber state capable */
  HOST_CAP_PIO_MULTI  = (1 << 15), /* PIO multiple DRQ support */
  HOST_CAP_FBS    = (1 << 16), /* FIS-based switching support */
  HOST_CAP_PMP    = (1 << 17), /* Port Multiplier support */
  HOST_CAP_ONLY   = (1 << 18), /* Supports AHCI mode only */
  HOST_CAP_CLO    = (1 << 24), /* Command List Override support */
  HOST_CAP_LED    = (1 << 25), /* Supports activity LED */
  HOST_CAP_ALPM   = (1 << 26), /* Aggressive Link PM support */
  HOST_CAP_SSS    = (1 << 27), /* Staggered Spin-up */
  HOST_CAP_MPS    = (1 << 28), /* Mechanical presence switch */
  HOST_CAP_SNTF   = (1 << 29), /* SNotification register */
  HOST_CAP_NCQ    = (1 << 30), /* Native Command Queueing */
  HOST_CAP_64   = (1 << 31), /* PCI DAC (64-bit DMA) support */

  /* HOST_CAP2 bits */
  HOST_CAP2_BOH   = (1 << 0),  /* BIOS/OS handoff supported */
  HOST_CAP2_NVMHCI  = (1 << 1),  /* NVMHCI supported */
  HOST_CAP2_APST    = (1 << 2),  /* Automatic partial to slumber */

  /* registers for each SATA port */
  PORT_LST_ADDR   = 0x00, /* command list DMA addr */
  PORT_LST_ADDR_HI  = 0x04, /* command list DMA addr hi */
  PORT_FIS_ADDR   = 0x08, /* FIS rx buf addr */
  PORT_FIS_ADDR_HI  = 0x0c, /* FIS rx buf addr hi */
  PORT_IRQ_STAT   = 0x10, /* interrupt status */
  PORT_IRQ_MASK   = 0x14, /* interrupt enable/disable mask */
  PORT_CMD    = 0x18, /* port command */
  PORT_TFDATA   = 0x20, /* taskfile data */
  PORT_SIG    = 0x24, /* device TF signature */
  PORT_CMD_ISSUE    = 0x38, /* command issue */
  PORT_SCR_STAT   = 0x28, /* SATA phy register: SStatus */
  PORT_SCR_CTL    = 0x2c, /* SATA phy register: SControl */
  PORT_SCR_ERR    = 0x30, /* SATA phy register: SError */
  PORT_SCR_ACT    = 0x34, /* SATA phy register: SActive */
  PORT_SCR_NTF    = 0x3c, /* SATA phy register: SNotification */
  PORT_FBS    = 0x40, /* FIS-based Switching */
/* PORT_IRQ_{STAT,MASK} bits */
  PORT_IRQ_COLD_PRES  = (1 << 31), /* cold presence detect */
  PORT_IRQ_TF_ERR   = (1 << 30), /* task file error */
  PORT_IRQ_HBUS_ERR = (1 << 29), /* host bus fatal error */
  PORT_IRQ_HBUS_DATA_ERR  = (1 << 28), /* host bus data error */
  PORT_IRQ_IF_ERR   = (1 << 27), /* interface fatal error */
  PORT_IRQ_IF_NONFATAL  = (1 << 26), /* interface non-fatal error */
  PORT_IRQ_OVERFLOW = (1 << 24), /* xfer exhausted available S/G */
  PORT_IRQ_BAD_PMP  = (1 << 23), /* incorrect port multiplier */

  PORT_IRQ_PHYRDY   = (1 << 22), /* PhyRdy changed */
  PORT_IRQ_DEV_ILCK = (1 << 7), /* device interlock */
  PORT_IRQ_CONNECT  = (1 << 6), /* port connect change status */
  PORT_IRQ_SG_DONE  = (1 << 5), /* descriptor processed */
  PORT_IRQ_UNK_FIS  = (1 << 4), /* unknown FIS rx'd */
  PORT_IRQ_SDB_FIS  = (1 << 3), /* Set Device Bits FIS rx'd */
  PORT_IRQ_DMAS_FIS = (1 << 2), /* DMA Setup FIS rx'd */
  PORT_IRQ_PIOS_FIS = (1 << 1), /* PIO Setup FIS rx'd */
  PORT_IRQ_D2H_REG_FIS  = (1 << 0), /* D2H Register FIS rx'd */

  PORT_IRQ_FREEZE   = PORT_IRQ_HBUS_ERR |
          PORT_IRQ_IF_ERR |
          PORT_IRQ_CONNECT |
          PORT_IRQ_PHYRDY |
          PORT_IRQ_UNK_FIS |
          PORT_IRQ_BAD_PMP,
  PORT_IRQ_ERROR    = PORT_IRQ_FREEZE |
          PORT_IRQ_TF_ERR |
          PORT_IRQ_HBUS_DATA_ERR,
  DEF_PORT_IRQ    = PORT_IRQ_ERROR | PORT_IRQ_SG_DONE |
          PORT_IRQ_SDB_FIS | PORT_IRQ_DMAS_FIS |
          PORT_IRQ_PIOS_FIS | PORT_IRQ_D2H_REG_FIS,

  /* PORT_CMD bits */
  PORT_CMD_ASP    = (1 << 27), /* Aggressive Slumber/Partial */
  PORT_CMD_ALPE   = (1 << 26), /* Aggressive Link PM enable */
  PORT_CMD_ATAPI    = (1 << 24), /* Device is ATAPI */
  PORT_CMD_FBSCP    = (1 << 22), /* FBS Capable Port */
  PORT_CMD_PMP    = (1 << 17), /* PMP attached */
  PORT_CMD_LIST_ON  = (1 << 15), /* cmd list DMA engine running */
  PORT_CMD_FIS_ON   = (1 << 14), /* FIS DMA engine running */
  PORT_CMD_FIS_RX   = (1 << 4), /* Enable FIS receive DMA engine */
  PORT_CMD_CLO    = (1 << 3), /* Command list override */
  PORT_CMD_POWER_ON = (1 << 2), /* Power up device */
  PORT_CMD_SPIN_UP  = (1 << 1), /* Spin up device */
  PORT_CMD_START    = (1 << 0), /* Enable port DMA engine */

  PORT_CMD_ICC_MASK = (0xf << 28), /* i/f ICC state mask */
  PORT_CMD_ICC_ACTIVE = (0x1 << 28), /* Put i/f in active state */
  PORT_CMD_ICC_PARTIAL  = (0x2 << 28), /* Put i/f in partial state */
  PORT_CMD_ICC_SLUMBER  = (0x6 << 28), /* Put i/f in slumber state */
  PORT_FBS_DWE_OFFSET = 16, /* FBS device with error offset */
  PORT_FBS_ADO_OFFSET = 12, /* FBS active dev optimization offset */
  PORT_FBS_DEV_OFFSET = 8,  /* FBS device to issue offset */
  PORT_FBS_DEV_MASK = (0xf << PORT_FBS_DEV_OFFSET),  /* FBS.DEV */
  PORT_FBS_SDE    = (1 << 2), /* FBS single device error */
  PORT_FBS_DEC    = (1 << 1), /* FBS device error clear */
  PORT_FBS_EN   = (1 << 0), /* Enable FBS */

  /* hpriv->flags bits */
  AHCI_HFLAG_NO_NCQ   = (1 << 0),
  AHCI_HFLAG_IGN_IRQ_IF_ERR = (1 << 1), /* ignore IRQ_IF_ERR */
  AHCI_HFLAG_IGN_SERR_INTERNAL  = (1 << 2), /* ignore SERR_INTERNAL */
  AHCI_HFLAG_32BIT_ONLY   = (1 << 3), /* force 32bit */
  AHCI_HFLAG_MV_PATA    = (1 << 4), /* PATA port */
  AHCI_HFLAG_NO_MSI   = (1 << 5), /* no PCI MSI */
  AHCI_HFLAG_NO_PMP   = (1 << 6), /* no PMP */
  AHCI_HFLAG_NO_HOTPLUG   = (1 << 7), /* ignore PxSERR.DIAG.N */
  AHCI_HFLAG_SECT255    = (1 << 8), /* max 255 sectors */
  AHCI_HFLAG_YES_NCQ    = (1 << 9), /* force NCQ cap on */
  AHCI_HFLAG_NO_SUSPEND   = (1 << 10), /* don't suspend */
  AHCI_HFLAG_SRST_TOUT_IS_OFFLINE = (1 << 11), /* treat SRST timeout as
              link offline */
  AHCI_HFLAG_NO_SNTF    = (1 << 12), /* no sntf */
  AHCI_HFLAG_NO_FPDMA_AA    = (1 << 13), /* no FPDMA AA */
  AHCI_HFLAG_YES_FBS    = (1 << 14), /* force FBS cap on */

  /* ap->flags bits */


  ICH_MAP       = 0x90, /* ICH MAP register */

  /* em constants */
  EM_MAX_SLOTS      = 8,
  EM_MAX_RETRY      = 5,

  /* em_ctl bits */
  EM_CTL_RST    = (1 << 9), /* Reset */
  EM_CTL_TM   = (1 << 8), /* Transmit Message */
  EM_CTL_MR   = (1 << 0), /* Message Recieved */
  EM_CTL_ALHD   = (1 << 26), /* Activity LED */
  EM_CTL_XMT    = (1 << 25), /* Transmit Only */
  EM_CTL_SMB    = (1 << 24), /* Single Message Buffer */

  /* em message type */
  EM_MSG_TYPE_LED   = (1 << 0), /* LED */
  EM_MSG_TYPE_SAFTE = (1 << 1), /* SAF-TE */
  EM_MSG_TYPE_SES2  = (1 << 2), /* SES-2 */
  EM_MSG_TYPE_SGPIO = (1 << 3), /* SGPIO */
};


typedef enum tagSAS_DEV_TYPE_E 
{
	SAS_PHY_UNUSED = 0,
	SAS_END_DEVICE = 1,
	SAS_EDGE_EXPANDER_DEVICE = 2,
	SAS_FANOUT_EXPANDER_DEVICE = 3,
	SAS_DEV_BUTT
}SAS_DEV_TYPE_E;

enum
{
    SATA_SRST_ASSERT         = 1,
    SATA_SRST_DEASSERT       = 2,
    SATA_EXEC_DEVDIG         = 3,
    SATA_NO_DATA             = 4,
    SATA_PIO                 = 5,
    SATA_DMA                 = 6,
    SATA_FPDMA               = 7
};

/* struct ata_taskfile flags */
enum
{
	ATA_TFLAG_LBA48 	= (1 << 0), /* enable 48-bit LBA and "HOB" */
	ATA_TFLAG_ISADDR	= (1 << 1), /* enable r/w to nsect/lba regs */
	ATA_TFLAG_DEVICE	= (1 << 2), /* enable r/w to device reg */
	ATA_TFLAG_WRITE 	= (1 << 3), /* data dir: host->dev==1 (write) */
	ATA_TFLAG_LBA		= (1 << 4), /* enable LBA */
	ATA_TFLAG_FUA		= (1 << 5), /* enable FUA */
	ATA_TFLAG_POLLING	= (1 << 6), /* set nIEN to 1 and use polling */
};

#define ATA_SUBCMD_FPDMA_SEND_DSM   0x00

enum
{
    ATA_CMD_PIO_READ         = 0x20,
    ATA_CMD_PIO_READ_EXT     = 0x24,
    ATA_CMD_READ_DMA_EXT     = 0x25,
    ATA_CMD_PIO_WRITE        = 0x30,
    ATA_CMD_PIO_WRITE_EXT    = 0x34,
    ATA_CMD_WRITE_DMA_EXT    = 0x35,
    ATA_CMD_VERIFY           = 0x40,
    ATA_CMD_VERIFY_EXT       = 0x42,
    ATA_READ_FPDMA_QUEUED    = 0x60,
    ATA_WRITE_FPDMA_QUEUED   = 0x61,
    ATA_CMD_FPDMA_SEND	     = 0x64,
    ATA_CMD_EDD              = 0x90, /* execute device diagnostic */
    ATA_CMD_PACKET           = 0xA0,
    ATA_CMD_ID_ATAPI         = 0xA1,
    ATA_CMD_READ_DMA         = 0xC8,
    ATA_CMD_WRITE_DMA        = 0xCA,
    ATA_CMD_STANDBY          = 0xE2,
    ATA_CMD_IDLE             = 0xE3,
    ATA_CMD_CHECK_POWER_MODE = 0xE5,
    ATA_CMD_SLEEP            = 0xE6,
    ATA_CMD_FLUSH            = 0xE7,
    ATA_CMD_FLUSH_EXT        = 0xEA,
    ATA_CMD_ID_ATA           = 0xEC,
    ATA_CMD_SET_PASSWORD     = 0xF1,
    ATA_CMD_ERASE_PREPARE    = 0xF3,
    ATA_CMD_ERASE_UNIT       = 0xF4,
    ATA_CMD_SET_FEATURES     = 0xEF
};


typedef enum tagSAS_PROTOCOL_TYPE_E  
{
	SAS_PROTOCOL_SATA		= 0x01,
	SAS_PROTOCOL_SMP		= 0x02,
	SAS_PROTOCOL_STP		= 0x04,
	SAS_PROTOCOL_SSP		= 0x08,
	SAS_PROTOCOL_ALL		= 0x0E,
	SAS_PROTOCOL_BUTT
}SAS_PROTOCOL_TYPE_E;

typedef struct tagSAS_IDEN_FRAME_S
{
    UINT8  ucType;
    UINT8  ucReason;                /* reserved */
    UINT8  ucIniSspStpSmp;
    UINT8  ucTgtSspStpSmp;
    UINT8  ucDevName[8];            /* reserved */
    
    UINT8  ucSasAddrHi[4];          /* BE SAS address Lo */
    UINT8  ucSasAddrLo[4];          /* BE SAS address Hi */

    UINT8  ucPhyId;                 /* phy identifier */
    UINT8  ucZpsds;  
    UINT8  ucResv[6];               /* reserved */  
} SAS_IDEN_FRAME_S;

typedef struct  tagSAS_DEV_TO_HOST_S
{
	UINT8     ucFisType;	  /* 0x34 */
	UINT8     ucFlags;
	UINT8     ucStatus;
	UINT8     ucError;

	UINT8     ucLbal;
	UINT8     ucLbam;
	UINT8     ucLbah;
	UINT8     ucDevice;

	UINT8     ucLbalExp;
	UINT8     ucLbamExp;
	UINT8     ucLbahExp;
	UINT8     ucRA;

	UINT8     ucSectorCnt;
	UINT8     ucSectorCntExp;
	UINT8     ucRB;
	UINT8     ucRc;

	UINT8     ucResv[48];
}SAS_DEV_TO_HOST_S;

typedef struct tagSAS_HOST_TO_DEV_S 
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

	UINT8     ucAuxiliary;
	UINT8     ucRb[47];
} SAS_HOST_TO_DEV_S;

/** \brief data structure provides the identify data of the SATA device 
 *
 * data structure provides the identify data of the SATA device 
 *
 */
typedef struct tagSATA_ID_DATA_S
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
} SATA_ID_DATA_S;              



#endif

