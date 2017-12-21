/* vxbDtsecEnd.h - header file for Freescale QorIQ DTSEC VxBus END driver */

/*
 * Copyright (C) 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,04jan13,fao  Add error statistics. (WIND00394046)
01j,26oct12,y_c  Add one new macro for SGMII media mode. (WIND00316241)
01i,07mar11,kxb  NAP 2.0 merge: Change tx/rx/comp queue id's
01h,26may10,wap  Adjust DTSEC_TBI_ADDR to use the same value used by
		 Freescale's Linux code
01g,31mar10,wap  Clean up compiler warnings
01f,17nov09,wap  Handle 36-bit addressing
01e,27oct09,wap  Adjust buffer sizes slightly
01d,13oct09,wap  Use contextB values returned from Qman callout API
01c,09sep09,wap  Change TBIPA setting so that it doesn't conflict with P4080DS
		 board SERDES PHYs, add media selection support
01b,29aug09,wap  Add AMP support
01a,24jun09,wap  written
*/

#ifndef __INCvxbDtsecEndh
#define __INCvxbDtsecEndh

#ifdef __cplusplus
extern "C" {
#endif

IMPORT void vxbDtsecEndRegister (void);

#define DTSEC_MEDIA_RMII	0x1
#define DTSEC_MEDIA_GMII	0x2
#define DTSEC_MEDIA_RGMII	0x3
#define DTSEC_MEDIA_SGMII_1000X	0x4
#define DTSEC_MEDIA_TBI		    0x5
#define DTSEC_MEDIA_SGMII_CISCO	0x6

/* Choose which SGMII scheme to use by default. */

#define DTSEC_MEDIA_SGMII	DTSEC_MEDIA_SGMII_1000X

#define DTSEC_ID		0x000	/* Controller ID/vers */
#define DTSEC_ID2		0x004	/* Controller ID/conf */
#define DTSEC_IEVENT		0x008	/* Interrupt event register */
#define DTSEC_IMASK		0x010	/* Interrupt mask register */
#define DTSEC_ECNTRL		0x014	/* Ethernet ctrl/cfg */
#define DTSEC_PTV		0x018	/* Pause timer value */
#define DTSEC_TBIPA		0x01C	/* TBI PHY address */
#define DTSEC_TMR_CTRL		0x020	/* Timestamp control */
#define DTSEC_TMR_EVENT		0x024	/* Timer event */
#define DTSEC_TMR_MASK		0x028	/* Timer mask */
#define DTSEC_TCTRL		0x040	/* TX control */
#define DTSEC_RCTRL		0x050	/* RX control */
#define DTSEC_IADDR0		0x080	/* Indiv/grp hash table */
#define DTSEC_IADDR1		0x084
#define DTSEC_IADDR2		0x088
#define DTSEC_IADDR3		0x08C
#define DTSEC_IADDR4		0x090
#define DTSEC_IADDR5		0x094
#define DTSEC_IADDR6		0x098
#define DTSEC_IADDR7		0x09C
#define DTSEC_GADDR0		0x0A0	/* Groyp hash table */
#define DTSEC_GADDR1		0x0A4
#define DTSEC_GADDR2		0x0A8
#define DTSEC_GADDR3		0x0AC
#define DTSEC_GADDR4		0x0B0
#define DTSEC_GADDR5		0x0B4
#define DTSEC_GADDR6		0x0B8
#define DTSEC_GADDR7		0x0BC
#define DTSEC_MACCFG1		0x100	/* MAC config 1 */
#define DTSEC_MACCFG2		0x104	/* MAC config 2 */
#define DTSEC_IPGIFG		0x108	/* Interpacket gap cfg */
#define DTSEC_HALFDUP		0x10C	/* Half duplex ctrl */
#define DTSEC_MAXFRM		0x110	/* Max frame size */
#define DTSEC_MIIMCFG		0x1120	/* MII mgmt cfg */
#define DTSEC_MIIMCOM		0x1124	/* MII mgmt command */
#define DTSEC_MIIMADD		0x1128 	/* MII mgmt address */
#define DTSEC_MIIMCON		0x112C	/* MII mgmt control */
#define DTSEC_MIIMSTAT		0x1130	/* MII mgmt status */
#define DTSEC_MIIMIND		0x1134	/* MII mgmt indication */
#define DTSEC_IFSTAT		0x13C	/* interface sts */
#define DTSEC_PAR0		DTSEC_CAM0_0
#define DTSEC_PAR1		DTSEC_CAM0_1
#define DTSEC_CAM0_0		0x140
#define DTSEC_CAM0_1		0x144
#define DTSEC_CAM1_0		0x148
#define DTSEC_CAM1_1		0x14C
#define DTSEC_CAM2_0		0x150
#define DTSEC_CAM2_1		0x154
#define DTSEC_CAM3_0		0x158
#define DTSEC_CAM3_1		0x15C
#define DTSEC_CAM4_0		0x160
#define DTSEC_CAM4_1		0x164
#define DTSEC_CAM5_0		0x168
#define DTSEC_CAM5_1		0x16C
#define DTSEC_CAM6_0		0x170
#define DTSEC_CAM6_1		0x174
#define DTSEC_CAM7_0		0x178
#define DTSEC_CAM7_1		0x17C
#define DTSEC_CAM8_0		0x180
#define DTSEC_CAM8_1		0x184
#define DTSEC_CAM9_0		0x188
#define DTSEC_CAM9_1		0x18C
#define DTSEC_CAM10_0		0x190
#define DTSEC_CAM10_1		0x194
#define DTSEC_CAM11_0		0x198
#define DTSEC_CAM11_1		0x19C
#define DTSEC_CAM12_0		0x1A0
#define DTSEC_CAM12_1		0x1A4
#define DTSEC_CAM13_0		0x1A8
#define DTSEC_CAM13_1		0x1AC
#define DTSEC_CAM14_0		0x1B0
#define DTSEC_CAM14_1		0x1B4
#define DTSEC_CAM15_0		0x1B8
#define DTSEC_CAM15_1		0x1BC

/* Statistics registers */

#define DTSEC_TR64		0x200
#define DTSEC_TR127		0x204
#define DTSEC_TR255		0x208
#define DTSEC_TR511		0x20C
#define DTSEC_TR1K		0x210
#define DTSEC_TRMAX		0x214
#define DTSEC_TRMGV		0x218
#define DTSEC_RBYT		0x21C
#define DTSEC_RPKT		0x220
#define DTSEC_RFCS		0x224
#define DTSEC_RMCA		0x228
#define DTSEC_RBCA		0x22C
#define DTSEC_RXCF		0x230
#define DTSEC_RXPF		0x234
#define DTSEC_RXUO		0x238
#define DTSEC_RALN		0x23C
#define DTSEC_RFLR		0x240
#define DTSEC_RCDE		0x244
#define DTSEC_RCSE		0x248
#define DTSEC_RUND		0x24C
#define DTSEC_ROVR		0x250
#define DTSEC_RFRG		0x254
#define DTSEC_RJBR		0x258
#define DTSEC_RDRP		0x25C
#define DTSEC_TBYT		0x260
#define DTSEC_TPKT		0x264
#define DTSEC_TMCA		0x268
#define DTSEC_TBCA		0x26C
#define DTSEC_TXPF		0x270
#define DTSEC_TDFR		0x274
#define DTSEC_TEDF		0x278
#define DTSEC_TSCL		0x27C
#define DTSEC_TMCL		0x280
#define DTSEC_TLCL		0x284
#define DTSEC_TXCL		0x288
#define DTSEC_TNCL		0x28C
#define DTSEC_TDRP		0x294
#define DTSEC_TJBR		0x298
#define DTSEC_TFCS		0x29C
#define DTSEC_TXCF		0x2A0
#define DTSEC_TOVR		0x2A4
#define DTSEC_TUND		0x2A8
#define DTSEC_TFRG		0x2AC
#define DTSEC_CAR1		0x2B0
#define DTSEC_CAR2		0x2B4
#define DTSEC_CAM1		0x2B8
#define DTSEC_CAM2		0x2BC

/* IEVENT and IMASK Register definitions */

#define DTSEC_IEVENT_BABR		0x80000000 /* giant frame RXed */
#define DTSEC_IEVENT_RXC		0x40000000 /* pause frame RXed */
#define DTSEC_IEVENT_MSRO		0x04000000 /* stats oflow */
#define DTSEC_IEVENT_GTSC		0x02000000 /* graceful TX stop done */
#define DTSEC_IEVENT_BABT		0x01000000 /* giant frame TXed */
#define DTSEC_IEVENT_TXC		0x00800000 /* pause frame TXed */
#define DTSEC_IEVENT_TXE		0x00400000 /* TX error */
#define DTSEC_IEVENT_TXB		0x00200000 /* TX descriptor updated */
#define DTSEC_IEVENT_LC			0x00040000 /* late collision */
#define DTSEC_IEVENT_CRL		0x00020000 /* excess collision */
#define DTSEC_IEVENT_XFUN		0x00010000 /* TX underrun */
#define DTSEC_IEVENT_ABRT		0x00008000 /* TX abort */
#define DTSEC_IEVENT_MAG		0x00001000 /* Magic packet RXed */
#define DTSEC_IEVENT_MMRD		0x00000400 /* MDIO read done */
#define DTSEC_IEVENT_MMWR		0x00000200 /* MDIO write done */
#define DTSEC_IEVENT_GRSC		0x00000100 /* graceful RX stop done */
#define DTSEC_IEVENT_TDPE		0x00000002 /* TX data parity error */
#define DTSEC_IEVENT_RDPE		0x00000001 /* RX data parity error */

/* Ethernet Control Register */

#define DTSEC_ECNTRL_CFG_RO		0x80000000 /* Config bits read only */
#define DTSEC_ECNTRL_CLRCNT		0x00004000 /* zero stats counters */
#define DTSEC_ECNTRL_AUTOZ		0x00002000 /* zero counter on read */
#define DTSEC_ECNTRL_STEN		0x00001000 /* stats enable */
#define DTSEC_ECNTRL_GMIIM		0x00000040 /* GMII mode (strap) */
#define DTSEC_ECNTRL_TBIM		0x00000020 /* TBI mode (strap) */
#define DTSEC_ECNTRL_RPM		0x00000010 /* reduced pin mode 1000 */
#define DTSEC_ECNTRL_R100M		0x00000008 /* 0 = R10, 1 == R100 */
#define DTSEC_ECNTRL_SGMIIM		0x00000002 /* SGMII mode */

/* Pause time value register */

#define DTSEC_PTV_PT			0x0000FFFF /* pause time value */
#define DTSEC_PTV_PTE			0xFFFF0000 /* extended pause control */

/* Timer control register */

#define DTSEC_TMR_CTRL_ESFDP		0x00000800 /* External SFD polarity */
#define DTSEC_TMR_CTRL_ESFDE		0x00000400 /* External SFD enable */

/* Timer event register */

#define DTSEC_TMR_EVENT_TSRE		0x00010000 /* Tstamp RX error */
#define DTSEC_TMR_EVENT_TXP2		0x00000200 /* PTP frame TXed */
#define DTSEC_TMR_EVENT_TXP1		0x00000100 /* PTP frame TXed */
#define DTSEC_TMR_EVENT_RXP		0x00000001 /* PTP frame RXed */

/* Transmit control register */

#define DTSEC_TCTRL_THDF		0x00000800 /* half duplex flow ctl */
#define DTSEC_TCTRL_TTSE		0x00000040 /* TX Tstamp enable */
#define DTSEC_TCTRL_GTS			0x00000020 /* TX graceful stop */
#define DTSEC_TCTRL_RFC_PAUSE		0x00000010 /* received pause frame */

/* Receive control register */

#define DTSEC_RCTRL_PAL			0x001F0000 /* RX frame alignment pad */
#define DTSEC_RCTRL_CFA			0x00001000 /* control frame accept */
#define DTSEC_RCTRL_GHTX		0x00000400 /* mcast hash table extend */
#define DTSEC_RCTRL_ERSFD		0x00000080 /* Enable ext SFD Tstamp */
#define DTSEC_RCTRL_ERXT		0x00000040 /* Enable RX Tstamp */
#define DTSEC_RCTRL_GRS			0x00000020 /* RX graceful stop */
#define DTSEC_RCTRL_IPCSEN		0x00000200 /* IP header csum enable */
#define DTSEC_RCTRL_BC_REJ		0x00000010 /* reject broadcasts */
#define DTSEC_RCTRL_MPROM		0x00000008 /* m/bcast promisc mode */
#define DTSEC_RCTRL_RSF			0x00000004 /* receive short frames */
#define DTSEC_RCTRL_EMEN		0x00000002 /* enable CAM1-CAM15 */
#define DTSEC_RCTRL_UPROM		0x00000001 /* ucast promisc mode */

#define DTSEC_RXALIGN(x)		(((x) << 16) & DTSEC_RCTRL_PAL)

/* MAC configuration 1 register */

#define DTSEC_MACCFG1_SOFT_RESET	0x80000000 /* Reset the entire MAC */
#define DTSEC_MACCFG1_RESET_RX_MC	0x00080000 /* Reset RX mac control */
#define DTSEC_MACCFG1_RESET_TX_MC	0x00040000 /* Reset TX mac control */
#define DTSEC_MACCFG1_RESET_RX_FUN	0x00020000 /* Reset RX function */
#define DTSEC_MACCFG1_RESET_TX_FUN	0x00010000 /* Reset TX function */
#define DTSEC_MACCFG1_LOOPBACK		0x00000100 /* MAC loopback mode */
#define DTSEC_MACCFG1_RX_FLOW		0x00000020 /* enable RX flow control */
#define DTSEC_MACCFG1_TX_FLOW		0x00000010 /* enable TX flow control */
#define DTSEC_MACCFG1_SYNCD_RX_EN	0x00000008 /* RX sync'ed */
#define DTSEC_MACCFG1_RX_EN		0x00000004 /* RX enable */
#define DTSEC_MACCFG1_SYNCD_TX_EN	0x00000002 /* TX sync'ed */
#define DTSEC_MACCFG1_TX_EN		0x00000001 /* TX enable */


/* MAC configuration 2 register */

#define DTSEC_MACCFG2_PREAMBLE_LEN	0x0000F000 /* TX preamble length */
#define DTSEC_MACCFG2_IF_MODE		0x00000300 /* interface mode */
#define DTSEC_MACCFG2_PREAM_RX_EN	0x00000080 /* RX preamble enb */
#define DTSEC_MACCFG2_PREAM_TX_EN	0x00000040 /* TX preamble enb */
#define DTSEC_MACCFG2_HUGE_FRAME	0x00000020 /* RX huge frames */
#define DTSEC_MACCFG2_LENGTH_CHECK	0x00000010 /* RX length checks */
#define DTSEC_MACCFG2_MPEN		0x00000008 /* Magic pkt RX en */
#define DTSEC_MACCFG2_PADCRC		0x00000004 /* TX autopad/CRC en */
#define DTSEC_MACCFG2_CRC_EN		0x00000002 /* TX CRC en */
#define DTSEC_MACCFG2_FULL_DUPLEX	0x00000001 /* Full duplex mode */

#define DTSEC_IFMODE_RSVD0		0x00000000
#define DTSEC_IFMODE_MII		0x00000100 /* nibble mode */
#define DTSEC_IFMODE_GMII_TBI		0x00000200 /* byte mode */
#define DTSEC_IFMODE_RSVD1		0x00000300

/* Interpacket gap register */

#define DTSEC_IPGIFG_BACKTOBACK		0x0000007F
#define DTSEC_IPGIFG_MINIMUM		0x00007F00
#define DTSEC_IPGIFG_NONBACKTOBACK1	0x007F0000
#define DTSEC_IPGIFG_NONBACKTOBACK2	0x7F000000

/* Halfduplex register */

#define DTSEC_HALFDUP_ALTBEB_TRUNC	0x00F00000
#define DTSEC_HALFDUP_BEB		0x00080000
#define DTSEC_HALFDUP_BPNBO		0x00040000
#define DTSEC_HALFDUP_NBO		0x00020000
#define DTSEC_HALFDUP_EXCESS_DEF	0x00010000
#define DTSEC_HALFDUP_RETRY		0x0000F000
#define DTSEC_HALFDUP_COL_WINDOW	0x0000003F

/* MII management configuration register */

#define DTSEC_MIIMCFG_RESET		0x80000000
#define DTSEC_MIIMCFG_NO_PRE		0x00000010
#define DTSEC_MIIMCFG_MGMT_CLK		0x0000000F

#define DTSEC_MIIMCFG_MCS_2		0x00000000
#define DTSEC_MIIMCFG_MCS_4		0x00000001
#define DTSEC_MIIMCFG_MCS_6		0x00000002
#define DTSEC_MIIMCFG_MCS_8		0x00000003
#define DTSEC_MIIMCFG_MCS_10		0x00000004
#define DTSEC_MIIMCFG_MCS_14		0x00000005
#define DTSEC_MIIMCFG_MCS_20		0x00000006
#define DTSEC_MIIMCFG_MCS_28		0x00000007

/* MII management command register */

#define DTSEC_MIIMCOM_SCAN		0x00000002
#define DTSEC_MIIMCOM_READ		0x00000001

/* MII management address register */

#define DTSEC_MIIMADD_PHYADDR		0x0000001F
#define DTSEC_MIIMADD_REGADDR		0x00001F00

/* MII management control register */

#define DTSEC_MIIMCON_WRDATA		0x0000FFFF

/* MII management status register */

#define DTSEC_MIIMSTAT_RDDATA		0x0000FFFF

/* MII management indicator register */

#define DTSEC_MIIMIND_NOT_VALID		0x00000004
#define DTSEC_MIIMIND_SCAN		0x00000002
#define DTSEC_MIIMIND_BUSY		0x00000001

/* TBI registers */

#define DTSEC_TBICR		0x00	/* Control register */
#define DTSEC_TBISR		0x01	/* Status register */
#define DTSEC_TBIANAR		0x04	/* Autoneg advertisement */
#define DTSEC_TBILPAR		0x05	/* Link partner ability */
#define DTSEC_TBIANEX		0x06	/* Autoneg adv. expansion */
#define DTSEC_TBIANNP		0x07	/* AN next page */
#define DTSEC_TBIANLPNP		0x08	/* Link partner next page */
#define DTSEC_TBIEXST		0x0F	/* Extended status */
#define DTSEC_TBIJD		0x10	/* Jitter diagnostics */
#define DTSEC_TBICON		0x11	/* TBI control */

/*
 * Most TBI registers are defined in genericTbiPhy.h, but
 * the TBICON register is specific to the DTSEC hardware
 */

#define DTSEC_TBICON_RESET	0x8000	/* Soft reset */
#define DTSEC_TBICON_RXDIS_DIS	0x2000	/* Disable RX disparity */
#define DTSEC_TBICON_TXDIS_DIS	0x1000	/* Disable TX disparity */
#define DTSEC_TBICON_AN_SENSE	0x0100	/* 802.3z */
#define DTSEC_TBICON_CLKSEL	0x0020	/* 62.5Mhz/125Mhz clock */
#define DTSEC_TBICON_MIMODE	0x0010	/* TBI/GMII mode */

typedef struct endNetPool
    {
    NET_POOL            pool;
    void                * pMblkMemArea;
    void                * pClMemArea;
    } END_NET_POOL;

#define dtsecEndPoolTupleFree(x) netMblkClChainFree(x)
#define dtsecEndPoolTupleGet(x)      \
	netTupleGet((x), (x)->clTbl[0]->clSize, M_DONTWAIT, MT_DATA, 0)

#define DTSEC_INC_DESC(x, y)	(x) = ((x + 1) & (y - 1))
#define DTSEC_MAXFRAG		8
#define DTSEC_MAX_UNITS		8
#define DTSEC_MAX_RX		32
#define DTSEC_CAM_CNT		16
#define DTSEC_ALIGN		2

#define DTSEC_MTU	1500
#define DTSEC_JUMBO_MTU	9000
#define DTSEC_CLSIZE	1536 + FMAN_PKT_OFF
#define DTSEC_JUMBO_CLSIZE	9036 + FMAN_PKT_OFF
#define DTSEC_NAME	"dtsec"
#define DTSEC_TIMEOUT	100000
#define DTSEC_INTRS	(DTSEC_RXINTRS|DTSEC_TXINTRS|DTSEC_ERRINTRS)
#define DTSEC_RXINTRS	(DTSEC_IEVENT_RXC|DTSEC_IEVENT_BABR)
#define DTSEC_TXINTRS	(DTSEC_IEVENT_TXC|DTSEC_IEVENT_TXE)
#define DTSEC_ERRINTRS	(DTSEC_IEVENT_XFUN)
#define DTSEC_LINKINTRS	0

#define DTSEC_ETHER_ALIGN	(DTSEC_ALIGN + FMAN_PKT_OFF)

/* Conform to IPFORWARDER layout */
#define DTSEC_TX_FQID		0x10
#define DTSEC_RX_FQID		0x30
#define DTSEC_TXDONE_FQID	0x50
#define DTSEC_RX_DESC_CNT	256
#define DTSEC_TX_DESC_CNT	256

/*
 * Default address for the TBI management interface. Normally this would
 * be 0x1F, but the Freescale P4080 board uses address 0x1F for one
 * of its copper PHYs.
 */

#define DTSEC_TBI_ADDR	0x05

typedef struct dtsec_tx_desc
    {
    QPORTAL_SGLIST	dtsec_frag[DTSEC_MAXFRAG];
    } DTSEC_TX_DESC;

typedef UINT64 (*dtsecVirtToPhysFunc)(void *);
typedef void * (*dtsecPhysToVirtFunc)(UINT64);

/*
 * Private adapter context structure.
 */

typedef struct dtsec_drv_ctrl
    {
    END_OBJ		dtsecEndObj;
    VXB_DEVICE_ID	dtsecDev;
    void *		dtsecBar;
    void *		dtsecHandle;
    void *		dtsecMuxDevCookie;
    VXB_DEVICE_ID	dtsecFman;
    BOOL		dtsecFiler;
    UINT32		dtsecMask;

    BPORTAL *		dtsecBportal;
    QPORTAL *		dtsecQportal;

    UINT8		dtsecBpid;
    UINT32		dtsecRxFqId;
    UINT32		dtsecTxFqId;
    UINT32		dtsecTxDoneFqId;

    UINT32		dtsecRxCtx;
    UINT32		dtsecTxDoneCtx;

    UINT32		dtsecRxPort;
    UINT32		dtsecTxPort;

    UINT32		dtsecFmanChan;

    UINT32		dtsecFmanUnit;
    UINT32		dtsecNum;
    UINT32		dtsecCpu;

    JOB_QUEUE_ID	dtsecJobQueue;
    QJOB		dtsecIntJob;
    atomic_t		dtsecIntPending;

    BOOL		dtsecPolling;
    M_BLK_ID		dtsecPollBuf;
    UINT32		dtsecIntMask;

    UINT8		dtsecAddr[ETHER_ADDR_LEN];

    END_CAPABILITIES	dtsecCaps;

    END_IFDRVCONF	dtsecEndStatsConf;
    END_IFCOUNTERS	dtsecEndStatsCounters;


    /* Begin MII/ifmedia required fields. */
    END_MEDIALIST	*dtsecMediaList;
    END_ERR		dtsecLastError;
    UINT32		dtsecCurMedia;
    UINT32		dtsecCurStatus;
    VXB_DEVICE_ID	dtsecMiiBus;
    VXB_DEVICE_ID	dtsecMiiDev;
    FUNCPTR		dtsecMiiPhyRead;
    FUNCPTR		dtsecMiiPhyWrite;
    int			dtsecMiiPhyAddr;
    /* End MII/ifmedia required fields */

    /* DMA tags and maps. */

    VXB_DMA_TAG *	dtsecParentTag;

    UINT32		dtsecTxProd;
    UINT32		dtsecTxCons;
    UINT32		dtsecTxFree;
    volatile BOOL	dtsecTxStall;
    DTSEC_TX_DESC *	dtsecTxDescMem;
    M_BLK_ID		dtsecTxMblk[DTSEC_TX_DESC_CNT];

    SEM_ID		dtsecDevSem;
    spinlockIsrNd_t	dtsecLock;

    int			dtsecMaxMtu;
    int			dtsecTbiAddr;
    int			dtsecMedia;

    dtsecVirtToPhysFunc	dtsecVirtToPhys;
    dtsecPhysToVirtFunc	dtsecPhysToVirt;

    /* RX error statics */

    UINT32		dtsecRxDmeErr;
    UINT32		dtsecRxFpeErr;
    UINT32		dtsecRxFseErr;
    UINT32		dtsecRxDisErr;
    UINT32		dtsecRxEofErr;
    UINT32		dtsecRxNssErr;
    UINT32		dtsecRxIppErr;
    UINT32		dtsecRxPteErr;
    UINT32		dtsecRxIspErr;
    UINT32		dtsecRxPheErr;
      
    /* TX error statics */

    UINT32		dtsecTxUfdErr;
    UINT32		dtsecTxLgeErr;
    UINT32		dtsecTxDmeErr;
    } DTSEC_DRV_CTRL;

#define DTSEC_ADDR_LO(y)  ((UINT64)((UINT64) (y)) & 0xFFFFFFFF)
#define DTSEC_ADDR_HI(y)  (((UINT64)((UINT64) (y)) >> 32) & 0xFFFFFFFF)

#define DTSEC_VTOP(p, x) \
	p->dtsecVirtToPhys == NULL ? (unsigned long)(x) : (p->dtsecVirtToPhys)(x)

#define DTSEC_PTOV(p, x) \
	p->dtsecPhysToVirt == NULL ? (void *)(unsigned long)(x) : (p->dtsecPhysToVirt)(x)


#define CSR_READ_4(pDev, addr)					\
	*(volatile UINT32 *)((UINT32)pDev->pRegBase[0] + addr)

#define CSR_WRITE_4(pDev, addr, data)				\
	do {							\
	    volatile UINT32 *pReg =				\
		(UINT32 *)((UINT32)pDev->pRegBase[0] + addr);	\
	    *(pReg) = (UINT32)(data);				\
	    WRS_ASM("eieio");					\
	} while ((0))

#define CSR_SETBIT_4(pDev, offset, val)          \
	CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val)          \
	CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))


#ifdef __cplusplus
}
#endif

#endif /* __INCvxbDtsecEndh */
