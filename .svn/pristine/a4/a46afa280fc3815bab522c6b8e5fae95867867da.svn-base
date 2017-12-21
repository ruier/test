/* tsi108End.h - Tsi108 GIGE Port Definitions */

/* Copyright 2007 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01a,11may07,jjm  Initial version, based on IBM 750GX/GL EVB BSP
*/

/*
This file contains definitions for Tsi108 Gigabit Ethernet Controller
*/

#ifndef __INCtsi108Endh
#define __INCtsi108Endh

#include "vxWorks.h"
#include "config.h"

#define DEVICE_NAME			"tsiG"
#define DEVICE_NAME_ETH0		"tsiG0"
#define DEVICE_NAME_ETH1		"tsiG1"
#define DEVICE_NAME_LENGTH		5

/* FIXME: Revisit cluster and mblk factor */
#define DEFAULT_LOAN_RXBUF_FACTOR	4
#define DEFAULT_MBLK_NUM_FACTOR		4

#define CL_OVERHEAD			4

#define TSI108_ETH_PORT_NUM		2
#define TSI108_PBM_PORT			2
#define TSI108_SDRAM_PORT		4

#if (USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE)
#   define TSI108_GE_DEST_PORT		TSI108_PBM_PORT
#   define TSI108_GE_TX_BD_SWAP		TSI108_EC_TXQ_CFG_WSWP
#   define TSI108_GE_TX_DATA_SWAP	TSI108_EC_TXQ_BUFCFG_BSWP
#   define TSI108_GE_RX_BD_SWAP		TSI108_EC_RXQ_CFG_WSWP
#   define TSI108_GE_RX_DATA_SWAP	TSI108_EC_RXQ_BUFCFG_BSWP
#else
#   define TSI108_GE_DEST_PORT		TSI108_SDRAM_PORT
#   define TSI108_GE_TX_BD_SWAP		(TSI108_EC_TXQ_CFG_WSWP | TSI108_EC_TXQ_CFG_BSWP)
#   define TSI108_GE_TX_DATA_SWAP	(0)
#   define TSI108_GE_RX_BD_SWAP		(TSI108_EC_RXQ_CFG_WSWP | TSI108_EC_RXQ_CFG_BSWP)
#   define TSI108_GE_RX_DATA_SWAP	(0)
#endif

#define TSI108_MAC_CFG1			(0x000)
#define TSI108_MAC_CFG1_SOFTRST		(1 << 31)
#define TSI108_MAC_CFG1_LOOPBACK	(1 << 8)
#define TSI108_MAC_CFG1_RXEN		(1 << 2)
#define TSI108_MAC_CFG1_TXEN		(1 << 0)

#define TSI108_MAC_CFG2			(0x004)
#define TSI108_MAC_CFG2_DFLT_PREAMBLE	(7 << 12)
#define TSI108_MAC_CFG2_IFACE_MASK	(3 << 8)
#define TSI108_MAC_CFG2_NOGIG		(1 << 8)
#define TSI108_MAC_CFG2_GIG		(2 << 8)
#define TSI108_MAC_CFG2_PADCRC		(1 << 2)
#define TSI108_MAC_CFG2_FULLDUPLEX	(1 << 0)

#define TSI108_MAC_MII_MGMT_CFG		(0x020)
#define TSI108_MAC_MII_MGMT_CLK		(7 << 0)
#define TSI108_MAC_MII_MGMT_RST		(1 << 31)


#define TSI108_MAC_MII_CMD		(0x024)
#define TSI108_MAC_MII_CMD_READ		(1 << 0)

#define TSI108_MAC_MII_ADDR		(0x028)
#define TSI108_MAC_MII_ADDR_REG		0
#define TSI108_MAC_MII_ADDR_PHY		8

#define TSI108_MAC_MII_DATAOUT		(0x02c)
#define TSI108_MAC_MII_DATAIN		(0x030)

#define TSI108_MAC_MII_IND		(0x034)
#define TSI108_MAC_MII_IND_NOTVALID	(1 << 2)
#define TSI108_MAC_MII_IND_SCANNING	(1 << 1)
#define TSI108_MAC_MII_IND_BUSY		(1 << 0)

#define TSI108_MAC_ADDR1		(0x040)
#define TSI108_MAC_ADDR2		(0x044)

#define TSI108_STAT_RXBYTES		(0x06c)
#define TSI108_STAT_RXBYTES_CARRY	(1 << 24)

#define TSI108_STAT_RXPKTS		(0x070)
#define TSI108_STAT_RXPKTS_CARRY	(1 << 18)

#define TSI108_STAT_RXFCS		(0x074)
#define TSI108_STAT_RXFCS_CARRY	(1 << 12)

#define TSI108_STAT_RXMCAST		(0x078)
#define TSI108_STAT_RXMCAST_CARRY	(1 << 18)

#define TSI108_STAT_RXBCAST		(0x07c)
#define TSI108_STAT_RXBCAST_CARRY	(1 << 18)

#define TSI108_STAT_RXALIGN		(0x08c)
#define TSI108_STAT_RXALIGN_CARRY	(1 << 12)

#define TSI108_STAT_RXLENGTH		(0x090)
#define TSI108_STAT_RXLENGTH_CARRY	(1 << 12)

#define TSI108_STAT_RXRUNT		(0x09c)
#define TSI108_STAT_RXRUNT_CARRY	(1 << 12)

#define TSI108_STAT_RXJUMBO		(0x0a0)
#define TSI108_STAT_RXJUMBO_CARRY	(1 << 12)

#define TSI108_STAT_RXFRAG		(0x0a4)
#define TSI108_STAT_RXFRAG_CARRY	(1 << 12)

#define TSI108_STAT_RXJABBER		(0x0a8)
#define TSI108_STAT_RXJABBER_CARRY	(1 << 12)

#define TSI108_STAT_RXDROP		(0x0ac)
#define TSI108_STAT_RXDROP_CARRY	(1 << 12)

#define TSI108_STAT_TXBYTES		(0x0b0)
#define TSI108_STAT_TXBYTES_CARRY	(1 << 24)

#define TSI108_STAT_TXPKTS		(0x0b4)
#define TSI108_STAT_TXPKTS_CARRY	(1 << 18)

#define TSI108_STAT_TXMCAST		(0x0b8)
#define TSI108_STAT_TXMCAST_CARRY	(1 << 18)

#define TSI108_STAT_TXBCAST		(0x0bc)
#define TSI108_STAT_TXBCAST_CARRY	(1 << 18)

#define TSI108_STAT_TXEXDEF		(0x0c8)
#define TSI108_STAT_TXEXDEF_CARRY	(1 << 12)

#define TSI108_STAT_TXEXCOL		(0x0d8)
#define TSI108_STAT_TXEXCOL_CARRY	(1 << 12)

#define TSI108_STAT_TXTCOL		(0x0dc)
#define TSI108_STAT_TXTCOL_CARRY	(1 << 13)

#define TSI108_STAT_TXPAUSEDROP		(0x0e4)
#define TSI108_STAT_TXPAUSEDROP_CARRY	(1 << 12)

#define TSI108_STAT_CARRY1		(0x100)
#define TSI108_STAT_CARRY1_RXBYTES	(1 << 16)
#define TSI108_STAT_CARRY1_RXPKTS	(1 << 15)
#define TSI108_STAT_CARRY1_RXFCS	(1 << 14)
#define TSI108_STAT_CARRY1_RXMCAST	(1 << 13)
#define TSI108_STAT_CARRY1_RXBCAST	(1 << 12)
#define TSI108_STAT_CARRY1_RXALIGN	(1 <<  8)
#define TSI108_STAT_CARRY1_RXLENGTH	(1 <<  7)
#define TSI108_STAT_CARRY1_RXRUNT	(1 <<  4)
#define TSI108_STAT_CARRY1_RXJUMBO	(1 <<  3)
#define TSI108_STAT_CARRY1_RXFRAG	(1 <<  2)
#define TSI108_STAT_CARRY1_RXJABBER	(1 <<  1)
#define TSI108_STAT_CARRY1_RXDROP	(1 <<  0)

#define TSI108_STAT_CARRY2		(0x104)
#define TSI108_STAT_CARRY2_TXBYTES	(1 << 13)
#define TSI108_STAT_CARRY2_TXPKTS	(1 << 12)
#define TSI108_STAT_CARRY2_TXMCAST	(1 << 11)
#define TSI108_STAT_CARRY2_TXBCAST	(1 << 10)
#define TSI108_STAT_CARRY2_TXEXDEF	(1 <<  7)
#define TSI108_STAT_CARRY2_TXEXCOL	(1 <<  3)
#define TSI108_STAT_CARRY2_TXTCOL	(1 <<  2)
#define TSI108_STAT_CARRY2_TXPAUSE	(1 <<  0)

#define TSI108_STAT_CARRYMASK1		(0x108) 
#define TSI108_STAT_CARRYMASK2		(0x10c) 

#define TSI108_EC_PORTCTRL		(0x200)
#define TSI108_EC_PORTCTRL_STATRST	(1 << 31)
#define TSI108_EC_PORTCTRL_STATCLR	(1 << 30)
#define TSI108_EC_PORTCTRL_STATZOR	(1 << 29)
#define TSI108_EC_PORTCTRL_STATEN	(1 << 28)
#define TSI108_EC_PORTCTRL_NOGIG	(1 << 18)
#define TSI108_EC_PORTCTRL_HALFDUPLEX	(1 << 16)

#define TSI108_EC_INTSTAT		(0x204)
#define TSI108_EC_INTMASK		(0x208)

#define TSI108_INT_ANY			(1 << 31)
#define TSI108_INT_SFN			(1 << 30)
#define TSI108_INT_RXIDLE		(1 << 29)
#define TSI108_INT_RXABORT		(1 << 28)
#define TSI108_INT_RXERROR		(1 << 27)
#define TSI108_INT_RXOVERRUN		(1 << 26)
#define TSI108_INT_RXTHRESH		(1 << 25)
#define TSI108_INT_RXWAIT		(1 << 24)
#define TSI108_INT_RXQUEUE0		(1 << 16)
#define TSI108_INT_STATCARRY		(1 << 15)
#define TSI108_INT_TXIDLE		(1 << 13)
#define TSI108_INT_TXABORT		(1 << 12)
#define TSI108_INT_TXERROR		(1 << 11)
#define TSI108_INT_TXUNDERRUN		(1 << 10)
#define TSI108_INT_TXTHRESH		(1 <<  9)
#define TSI108_INT_TXWAIT		(1 <<  8)
#define TSI108_INT_TXQUEUE0		(1 <<  0)

#define TSI108_EC_TXCFG			(0x220)
#define TSI108_EC_TXCFG_RST		(1 << 31)

#define TSI108_EC_TXCTRL		(0x224)
#define TSI108_EC_TXCTRL_IDLEINT	(1 << 31)
#define TSI108_EC_TXCTRL_ABORT		(1 << 30)
#define TSI108_EC_TXCTRL_GO		(1 << 15)
#define TSI108_EC_TXCTRL_QUEUE0		(1 <<  0)

#define TSI108_EC_TXSTAT		(0x228)
#define TSI108_EC_TXSTAT_ACTIVE		(1 << 15)
#define TSI108_EC_TXSTAT_QUEUE0		(1 << 0)

#define TSI108_EC_TXESTAT		(0x22c)
#define TSI108_EC_TXESTAT_Q0_ERR	(1 << 24)
#define TSI108_EC_TXESTAT_Q0_DESCINT	(1 << 16)
#define TSI108_EC_TXESTAT_Q0_EOF	(1 <<  8)
#define TSI108_EC_TXESTAT_Q0_EOQ	(1 <<  0)

#define TSI108_EC_TXERR			(0x278)

#define TSI108_EC_TXQ_CFG		(0x280)
#define TSI108_EC_TXQ_CFG_DESC_INT	(1 << 20)
#define TSI108_EC_TXQ_CFG_EOQ_OWN_INT	(1 << 19)
#define TSI108_EC_TXQ_CFG_WSWP		(1 << 11)
#define TSI108_EC_TXQ_CFG_BSWP		(1 << 10)
#define TSI108_EC_TXQ_CFG_SFNPORT	0

#define TSI108_EC_TXQ_BUFCFG		(0x284)
#define TSI108_EC_TXQ_BUFCFG_BURST8	(0 << 8)
#define TSI108_EC_TXQ_BUFCFG_BURST32	(1 << 8)
#define TSI108_EC_TXQ_BUFCFG_BURST128	(2 << 8)
#define TSI108_EC_TXQ_BUFCFG_BURST256	(3 << 8)
#define TSI108_EC_TXQ_BUFCFG_WSWP	(1 << 11)
#define TSI108_EC_TXQ_BUFCFG_BSWP	(1 << 10)
#define TSI108_EC_TXQ_BUFCFG_SFNPORT	0

#define TSI108_EC_TXQ_PTRLOW		(0x288)

#define TSI108_EC_TXQ_PTRHIGH		(0x28c)
#define TSI108_EC_TXQ_PTRHIGH_VALID	(1 << 31)

#define TSI108_EC_TXTHRESH		(0x230)
#define TSI108_EC_TXTHRESH_STARTFILL	0
#define TSI108_EC_TXTHRESH_STOPFILL	16

#define TSI108_EC_RXCFG			(0x320)
#define TSI108_EC_RXCFG_RST		(1 << 31)

/* Station Enable -- accept packets destined for us */
#define TSI108_EC_RXCFG_SE		(1 << 13) 
/* Unicast Frame Enable -- for packets not destined for us */
#define TSI108_EC_RXCFG_UFE		(1 << 12)
/* Multicast Frame Enable */
#define TSI108_EC_RXCFG_MFE		(1 << 11)
/* Broadcast Frame Enable */
#define TSI108_EC_RXCFG_BFE		(1 << 10)
#define TSI108_EC_RXCFG_UC_HASH		(1 <<  9)
#define TSI108_EC_RXCFG_MC_HASH		(1 <<  8)

#define TSI108_EC_RXSTAT		(0x328)
#define TSI108_EC_RXSTAT_ACTIVE		(1 << 15)
#define TSI108_EC_RXSTAT_QUEUE0		(1 << 0)

#define TSI108_EC_RXESTAT		(0x32c)
#define TSI108_EC_RXESTAT_Q0_ERR	(1 << 24)
#define TSI108_EC_RXESTAT_Q0_DESCINT	(1 << 16)
#define TSI108_EC_RXESTAT_Q0_EOF	(1 <<  8)
#define TSI108_EC_RXESTAT_Q0_EOQ	(1 <<  0)

#define TSI108_EC_HASHADDR		(0x360)
#define TSI108_EC_HASHADDR_AUTOINC	(1 << 31)
#define TSI108_EC_HASHADDR_DO1STREAD	(1 << 30)
#define TSI108_EC_HASHADDR_UNICAST	(0 <<  4)
#define TSI108_EC_HASHADDR_MCAST	(1 <<  4)

#define TSI108_EC_HASHDATA		(0x364)

#define TSI108_EC_RXQ_PTRLOW		(0x388)

#define TSI108_EC_RXQ_PTRHIGH		(0x38c)
#define TSI108_EC_RXQ_PTRHIGH_VALID	(1 << 31)

#define TSI108_EC_RXQ_CFG		(0x380)
#define TSI108_EC_RXQ_CFG_DESC_INT	(1 << 20)
#define TSI108_EC_RXQ_CFG_EOQ_OWN_INT	(1 << 19)
#define TSI108_EC_RXQ_CFG_WSWP		(1 << 11)
#define TSI108_EC_RXQ_CFG_BSWP		(1 << 10)
#define TSI108_EC_RXQ_CFG_SFNPORT	0

#define TSI108_EC_RXQ_BUFCFG		(0x384)
#define TSI108_EC_RXQ_BUFCFG_BURST8	(0 << 8)
#define TSI108_EC_RXQ_BUFCFG_BURST32	(1 << 8)
#define TSI108_EC_RXQ_BUFCFG_BURST128	(2 << 8)
#define TSI108_EC_RXQ_BUFCFG_BURST256	(3 << 8)
#define TSI108_EC_RXQ_BUFCFG_WSWP	(1 << 11)
#define TSI108_EC_RXQ_BUFCFG_BSWP	(1 << 10)
#define TSI108_EC_RXQ_BUFCFG_SFNPORT	0

#define TSI108_EC_RXCTRL		(0x324)
#define TSI108_EC_RXCTRL_ABORT		(1 << 30)
#define TSI108_EC_RXCTRL_GO		(1 << 15)
#define TSI108_EC_RXCTRL_QUEUE0		(1 << 0)

#define TSI108_EC_RXERR			(0x378)

#define PHY_CTRL			0
#define PHY_CTRL_RESET			(1 << 15)
#define PHY_CTRL_AUTONEG_EN		(1 << 12)
#define PHY_CTRL_POWERDOWN		(1 << 11)
#define PHY_CTRL_AUTONEG_START		(1 << 9)

#define PHY_STAT			1
#define PHY_STAT_LINKUP			(1 << 2)

#if defined TSI108_PHY_MV88E10XX
/* Marvel 88E1020 PHY */
#   define PHY_SUM_STAT			0x11
#   define PHY_SUM_STAT_SPEED_MASK	(3 << 14)
#   define PHY_SUM_STAT_SPEED_10	(0 << 14)
#   define PHY_SUM_STAT_SPEED_100	(1 << 14)
#   define PHY_SUM_STAT_SPEED_1000	(2 << 14)
#   define PHY_SUM_STAT_FULLDUPLEX	(1 << 13)

#elif defined TSI108_PHY_BCM54XX
/* Broadcom BCM54xx PHY */
#   define PHY_SUM_STAT			0x19
#   define PHY_SUM_STAT_SPEED_MASK	(7 << 8) /* Auto Negotiation HCD field*/
#   define PHY_SUM_STAT_1000T_FD	(7 << 8) /* 1000BASE-T Full-Duplex */
#   define PHY_SUM_STAT_1000T_HD	(6 << 8) /* 1000BASE-T Half-Duplex */
#   define PHY_SUM_STAT_100TX_FD	(5 << 8) /* 100BASE-TX Full-Duplex */
#   define PHY_SUM_STAT_100T4		(4 << 8) /* 100BASE-T4 */
#   define PHY_SUM_STAT_100TX_HD	(3 << 8) /* 100BASE-TX Half-Duplex */
#   define PHY_SUM_STAT_10T_FD		(2 << 8) /* 10BASE-T Full-Duplex */
#   define PHY_SUM_STAT_10T_HD		(1 << 8) /* 10BASE-T Half-Duplex */
#   define PHY_SUM_STAT_AN_FAIL		(0 << 8) /* 1000BASE-T Half-Duplex */
#else
#   error "*** PHY TYPE NOT DEFINED ***"
#endif

#define TSI108_TX_EOF   (1 << 0)  /* End of frame; set on last fragment of packet */
#define TSI108_TX_SOF   (1 << 1)  /* Start of frame; set on first frag. of packet */
#define TSI108_TX_VLAN  (1 << 2)  /* Per-frame VLAN: enables VLAN override  */
#define TSI108_TX_HUGE  (1 << 3)  /* Huge frame enable			    */
#define TSI108_TX_PAD   (1 << 4)  /* Pad the packet if too short	    */
#define TSI108_TX_CRC   (1 << 5)  /* Generate CRC for this packet	    */
#define TSI108_TX_INT   (1 << 14) /* Generate an IRQ after frag. processed  */
#define TSI108_TX_RETRY 16	  /* 4 bit field indicating num. of retries */
#define TSI108_TX_COL   (1 << 20) /* Set if a collision occured		    */
#define TSI108_TX_LCOL  (1 << 24) /* Set if a late collision occured	    */
#define TSI108_TX_UNDER (1 << 25) /* Set if a FIFO underrun occured	    */
#define TSI108_TX_RLIM  (1 << 26) /* Set if the retry limit was reached     */ 
#define TSI108_TX_OK    (1 << 30) /* Set if the frame was successfully transmitted */
#define TSI108_TX_OWN   (1 << 31) /* Set if the device owns the descriptor */

/* Note: the descriptor layouts assume big-endian byte order. */
typedef struct tx_desc
    {
    UINT32 buf0;
    UINT32 buf1;	/* Base address of buffer		*/
    UINT32 next0;	/* Address of next descriptor, if any	*/
    UINT32 next1;
    UINT16 vlan;	/* VLAN, if override enabled for this packet */
    UINT16 len;		/* Length of buffer in bytes		*/
    UINT32 misc;	/* See TSI108_TX_* above		*/
    UINT32 reserved0; /* reserved0 and reserved1 are added to make the desc */
    UINT32 reserved1; /* 32-byte aligned */
    } __attribute__((aligned(32))) TX_DESC;

#define TSI108_RX_EOF	(1 << 0)  /* End of frame; set on last fragment of packet */
#define TSI108_RX_SOF	(1 << 1)  /* Start of frame; set on first frag. of packet */
#define TSI108_RX_VLAN	(1 << 2)  /* Set on SOF if packet has a VLAN       */
#define TSI108_RX_FTYPE	(1 << 3)  /* Length/Type field is type, not length */
#define TSI108_RX_RUNT	(1 << 4)  /* Packet is less than minimum size	   */
#define TSI108_RX_HASH	(1 << 7)  /* Hash table match			   */
#define TSI108_RX_BAD	(1 << 8)  /* Bad frame				   */
#define TSI108_RX_OVER	(1 << 9)  /* FIFO overrun occured		   */
#define TSI108_RX_TRUNC	(1 << 11) /* Packet truncated due to excess length */
#define TSI108_RX_CRC	(1 << 12) /* Packet had a CRC error		   */
#define TSI108_RX_INT	(1 << 13) /* Generate an IRQ after frag. processed */
#define TSI108_RX_OWN	(1 << 15) /* Set if the device owns the descriptor */ 


typedef struct rx_desc
    {
    UINT32 buf0;	/* Base address of buffer		*/
    UINT32 buf1;	/* Base address of buffer		*/
    UINT32 next0;	/* Address of next descriptor, if any	*/
    UINT32 next1;	/* Address of next descriptor, if any	*/
    UINT16 vlan;	/* VLAN of received packet, first frag only	*/
    UINT16 len;		/* Length of received fragment in bytes	*/
    UINT16 blen;	/* Length of buffer in bytes		*/
    UINT16 misc;	/* See TSI108_RX_* above		*/
    UINT32 reserved0;	/* reserved0 and reserved1 are added to make the desc */
    UINT32 reserved1;	/* 32-byte aligned */
    } __attribute__((aligned(32))) RX_DESC;

#endif /* __INCtsi108Endh */
