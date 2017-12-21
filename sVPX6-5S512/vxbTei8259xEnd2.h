/* vxbTei8259xEnd2.h - header file for Intel 8259x 10Gbe VxBus END2 driver */

/*
 * Copyright (c) 2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,21mar13,fao  Correct count define for 82599. (WIND00403956)
01e,26feb13,wyt  Fix coverity error (WIND00401412).
01d,27sep10,dlk  Do not split packet buffers from Ipcom_pkt structures, as
                 the NAE stack does not support it.
01c,17aug10,dlk  Add PCI IDs for new devices (from 01g,16jun10,wap).
01b,25jul10,dlk  added MNT forwarding offload support.
01a,02jun10,dlk  written based upon vxbTei82598End.h (01d,31aug09,wap).
*/

#ifndef __INCvxbTei8259xEnd2h
#define __INCvxbTei8259xEnd2h

#include  <end.h>
#include  <hwif/util/vxbDmaBufLib.h>



#undef TEI_SECRC	/* Strip ethernet CRC by default. */

#if defined (IPCOM_USE_NEHALEM_FORWARDER) &&  (_VX_CPU_FAMILY == _VX_I80X86)

#if defined (_WRS_CONFIG_WRHV_GUEST) \
    && !defined (WRHV_USE_VTD) \
    && (_VX_CPU_FAMILY == _VX_I80X86)
/* 32-bit guest, not enabling VT-d, so we need to do guest-virt-to-host-phys
   translations manually when giving physical addresses to the device for
   DMA */
#define TEI_GUEST_VIRT_TO_PHYS
#endif

/*
 * When TEI_FORWARDER is defined, forwarder offload support is added
 * to the driver.  Presently only supported for Intel Nehalem.
 */
#undef TEI_FORWARDER

/*
 * When TEI_USE_FW_STATS is defined, the driver collects in software
 * statistics related to forwarding offload that may be useful in
 * diagnosing performance issues.
 */
#undef TEI_USE_FW_STATS

/*
 * Don't strip ethernet CRC; this lets a minimal sized packet
 * written by the device be exactly 64-bytes, which matches the Nehalem
 * cache line size; avoiding the partial cache line write helps performance
 * much more than having to adjust the packet length by 4 bytes hurts,
 * for minimal sized packets.
 */
#undef TEI_SECRC

#if defined(IPCOM_FORWARDER_NAE)
/*
 * When doing NAE, TCP needs the packet back since it
 * may be referenced by the TCP code. If the batch's larger than
 * 1, indicating that we dont free TX'd frame until the
 * Nth frame has been sent, TCP will be stalled until this
 * event has been completed. If there's a single TCP 
 * connection and no other traffic currently being served,
 * the TCP connection will stall indefinately
 */
#define TEI_FW_TX_CLEANUP_BATCH	1
#else
#define TEI_FW_TX_CLEANUP_BATCH	16
#endif

#endif /* Nehalem forwarder */

/*
 * How many descriptors to batch in a DMA ring tail register update.
 * Applies only to forwarder ports at present. Must be a power of two.
 */
#define TEI_RDT_BATCH	8
#define TEI_TDT_BATCH	8	/* applies only for forwarder case */

#if (_VX_CPU_FAMILY != _VX_I80X86)
#ifdef TEI_FORWARDER
#warning "TEI_FORWARDER only supported on Intel Nehalem."
#undef TEI_FORWARDER
#endif
#endif

/*
 * In the SMP forwarder prototype, receive queue zero and transmit queue zero
 * on forwarder ports were assigned for normal stack use through the MUX. Since
 * RX queue zero receives all non-IP packets regardless of the redirection table
 * settings, and the forwarders couldn't handle non-IP packets anyhow, this
 * was convenient.
 * However, in an AMP environment in which the forwarder tasks must send
 * all packets (including non-IP packets) either to a local NAE stack, or to the
 * the management OS stack, RX queue zero should be polled by a forwarder, and
 * should not be handled via the MUX (which may not even exist for a slave
 * forwarder virtual board).  We actually want to support three different
 * port modes:
 *
 *  1. Conventional:
 *     A port has just one RX and one TX queue, both operated in interrupt mode,
 *     accessed via the MUX. Also applies to any port not supporting forwarding
 *     acceleration.
 *
 *  2. SMP forwarding (or various AMP prototype cases):
 *     For a forwarding port, RX and TX queue zero are handled via the MUX
 *     in interrupt mode; other RX and TX queues on the port are handled in
 *     polled mode by forwarder tasks.
 *
 *  3. AMP forwarding:
 *     For a forwarding port, all (of multiple) RX and TX queues are handled by
 *     forwarder tasks in polled mode. No RX or TX queue on the port is available
 *     via MUX. [Optionally, a TX queue can be made directly available via the
 *     MUX in interrupt mode.]
 *
 * When TEI_MUX_RX_ALWAYS is defined, RX queue 0 on each port is
 * reserved for normal MUX RX usage and operated in interrupt mode.
 * When TEI_MUX_TX_ALWAYS is defined, TX queue 0 on each port is
 * reserved for normal MUX TX usage and operated in interrupt mode.
 *
 * In AMP forwarder builds, TEI_MUX_RX_ALWAYS and/or
 * TEI_MUX_TX_ALWAYS may be left undefined; in that case, for
 * forwarder ports only: 
 *
 *  - If TEI_MUX_RX_ALWAYS is not defined, RX queue 0 on a forwarder
 *    port is reserved for forwarder usage and is operated in polled
 *    mode. Receives from the port via the MUX are not supported in
 *    this case.
 *  - If TEI_TUX_RX_ALWAYS is not defined, TX queue 0 on a forwarder
 *    port is reserved for forwarder usage and is operated in polled
 *    mode. Sends over the port via the MUX are not supported in
 *    this case.
 */

#undef TEI_MUX_RX_ALWAYS
#define TEI_MUX_TX_ALWAYS

#if !defined (TEI_FORWARDER) || !(defined (IPCOM_FORWARDER_AMP) || defined(IPCOM_FORWARDER_NAE))
#define TEI_MUX_TX_ALWAYS
#define TEI_MUX_RX_ALWAYS
#endif

#define TEI_MUX_RX	1
#define TEI_MUX_TX	2

#ifdef TEI_MUX_RX_ALWAYS
#define TEI_MUX_RX_ENABLED(p) (TRUE)
#else
#define TEI_MUX_RX_ENABLED(p) ((p)->muxqs & TEI_MUX_RX)
#endif

#ifdef TEI_MUX_TX_ALWAYS
#define TEI_MUX_TX_ENABLED(p) (TRUE)
#else
#define TEI_MUX_TX_ENABLED(p) ((p)->muxqs & TEI_MUX_TX)
#endif

#if defined (TEI_MUX_RX_ALWAYS) && defined (TEI_MUX_TX_ALWAYS)
#define TEI_MUX_ANY_ENABLED(p) (TRUE)
#define TEI_MUX_BOTH_ENABLED(p) (TRUE)
#else
#define TEI_MUX_ANY_ENABLED(p) ((p)->muxqs & (TEI_MUX_RX|TEI_MUX_TX))
#define TEI_MUX_BOTH_ENABLED(p)  \
    (((p)->muxqs & (TEI_MUX_RX|TEI_MUX_TX)) == (TEI_MUX_RX|TEI_MUX_TX))
#endif

#ifdef TEI_FORWARDER
#include <ipcom_forwarder.h>
#include <ipcom_vxworks_nehalem_forwarder.h>
#endif

/*
 * When TEI_VXB_DMA_BUF is defined, the driver makes use of
 * vxbDmaBufLib.  When TEI_VXB_DMA_BUF is not defined,
 * it relies only on CACHE_USER_FUNCS, and allocates
 * descriptor memory using cacheDmaMalloc().
 * Use of vxbDmaBufLib is required on architectures
 * for which the PCI device may not be able to access
 * all memory (that might be used for either descriptors
 * or data clusters) on the target.
 *
 * Presently we disable TEI_VXB_DMA_BUF only for the I80X86
 * CPU family.
 */
#if 0
#if (_VX_CPU_FAMILY == _VX_I80X86)
#undef TEI_VXB_DMA_BUF
#else
#define TEI_VXB_DMA_BUF
#endif

#ifdef __cplusplus
extern "C" {
#endif
#endif

IMPORT void teiEnd2Register (void);



/*
 * VxWorks 6.6 compatibility
 * These media types weren't added to endMedia.h until
 * VxWorks 6.7.
 */

#ifndef IFM_10G_LR
#define IFM_10G_LR      18              /* 10GBase-LR 1310nm Single-mode */
#define IFM_10G_SR      19              /* 10GBase-SR 850nm Multi-mode */
#define IFM_10G_CX4     20              /* 10GBase CX4 copper */
#endif

#ifndef IFM_2500_SX
#define IFM_2500_SX     21              /* 2500BaseSX - multi-mode fiber */
#define IFM_10G_TWINAX  22              /* 10GBase Twinax copper */
#define IFM_10G_TWINAX_LONG 2           /* 10GBase Twinax Long copper */
#define IFM_10G_LRM     24              /* 10GBase-LRM 850nm Multi-mode */
#define IFM_UNKNOWN     25              /* New media types that have not been defined yet */
#define IFM_10G_T       26              /* 10GBase-T */
#endif

#define INTEL_VENDORID	0x8086

#define INTEL_DEVICEID_82598			0x10B6
#define INTEL_DEVICEID_82598_BX			0x1508
#define INTEL_DEVICEID_82598AF_DUAL_PORT	0x10C6
#define INTEL_DEVICEID_82598AF_SINGLE_PORT	0x10C7
#define INTEL_DEVICEID_82598AT			0x10C8
#define INTEL_DEVICEID_82598AT2			0x150B
#define INTEL_DEVICEID_82598EB_SFP_LOM		0x10DB
#define INTEL_DEVICEID_82598EB_CX4		0x10DD
#define INTEL_DEVICEID_82598_CX4_DUAL_PORT	0x10EC
#define INTEL_DEVICEID_82598_DA_DUAL_PORT	0x10F1
#define INTEL_DEVICEID_82598_SR_DUAL_PORT_EM	0x10E1
#define INTEL_DEVICEID_82598EB_XF_LR		0x10F4

#define INTEL_DEVICEID_82599_KX4		0x10F7
#define INTEL_DEVICEID_82599_COMBO_BACKPLANE	0x10F8
#define INTEL_DEVICEID_82599_CX4		0x10F9
#define INTEL_DEVICEID_82599_SFP		0x10FB
#define INTEL_DEVICEID_82599_XAUI_LOM		0x10FC
#define INTEL_DEVICEID_82599_SFP_EM             0x1507
#define INTEL_DEVICEID_82599_BX                 0x1508
#define INTEL_DEVICEID_82599_KX4_MEZZ           0x1514
#define INTEL_DEVICEID_82599_KR                 0x1517
#define INTEL_DEVICEID_82599_T3_LOM             0x151C


#define TEI_DEVTYPE_82598		0x1
#define TEI_DEVTYPE_82599		0x2

#define TEI_CTRL	0x00000	/* Device control */
#define TEI_STS		0x00008	/* Device status */
#define TEI_CTRLEXT	0x00018 /* Extended control register */
#define TEI_ESDP	0x00020	/* Extended SDP control */
#define TEI_EODSDP	0x00028	/* Extended OD SDP control */
#define TEI_TCPTIMER	0x0004C	/* TCP timer */
#define TEI_LEDCTL	0x00200	/* LED control */
#define TEI_EICR	0x00800	/* Extended Interrupt Cause Read */
#define TEI_EICS	0x00808	/* Extended Interrupt Cause Set */
#define TEI_EIAC	0x00810	/* Extended Interrupt Auto Clear */
#define TEI_EITR0	0x00820	/* Extended Interrupt throttle rate 0 */
#define TEI_EITT19	0x0086C /* Extended Interrupt throttle rate 19 */
#define TEI_EIMS	0x00880	/* Extended Interrupt Mask Set/Read */
#define TEI_EIMC	0x00888	/* Extended Interrupt Mask Clear */
#define TEI_EIAM	0x00880	/* Extended Interrupt Auto Mask Enable */
#define TEI_GPIE	0x00898	/* General purpose interrupt enable */
#define TEI_IVAR0	0x00900	/* Interrupt vector allocation register 0 */
#define TEI_IVARX	0x00960	/* Interrupt vector allocation register X */
#define TEI_IVARX_599	0x009FC	/* Max IVAR for 82599 */

#define TEI_RDBAL0	0x01000	/* RX descriptor base low */
#define TEI_RDBAH0	0x01004	/* RX descriptor base high */
#define TEI_RDLEN0	0x01008	/* RX descriptor length */
#define TEI_RDH0	0x01010	/* RX descriptor head */
#define TEI_SRRCTL99_0  0x01014 /* Split RX Control for 82599 */
#define TEI_RDT0	0x01018	/* RX descriptor tail */
#define TEI_RXDCTL0	0x01028	/* RX descriptor control */
#define TEI_RSCCTL0     0x0102C /* Receive Side Coalescing control */

#define TEI_RDBAL	TEI_RDBAL0
#define TEI_RDBAH	TEI_RDBAH0
#define TEI_RDLEN	TEI_RDLEN0
#define TEI_RDH		TEI_RDH0
#define TEI_RDT		TEI_RDT0
#define TEI_SRRCTL99    TEI_SRRCTL99
#define TEI_RXDCTL	TEI_RXDCTL0
#define TEI_RSCCTL      TEI_RSCCTL0

/* 82599: for RX queues 64-127 */
#define TEI_RDBAL64	0x0D000	/* RX descriptor base low */
#define TEI_RDBAH64	0x0D004	/* RX descriptor base high */
#define TEI_RDLEN64	0x0D008	/* RX descriptor length */
#define TEI_RDH64	0x0D010	/* RX descriptor head */
#define TEI_SRRCTL99_64 0x0D014 /* Split RX Control for 82599 */
#define TEI_RDT64	0x0D018	/* RX descriptor tail */
#define TEI_RXDCTL64	0x0D028	/* RX descriptor control */
#define TEI_RSCCTL64    0x0D02C /* Receive Side Coalescing control */

#define TEI_QPRC	0x01030 /* RX queue packets */
#define TEI_QBRC	0x01034 /* RX bytes received */

/* 82598 locations, supported for compatibility by 82599 */
#define TEI_SRRCTL0	0x02100	/* RX split and replication control */
#define TEI_SRRCTL15	0x0213C	/* RX split and replication control */

#define TEI_SRRCTL	TEI_SRRCTL0

#define TEI_DCA_RXCTL0	0x02200	/* RX DCA control */
#define TEI_DCA_RXCTL15	0x0223C	/* RX DCA control */

#define TEI_RQSMR	0x02300 /* Receive Queue Statistics Mapping */

#define TEI_RDRXCTL	0x02F00	/* RX DMA control */

#define TEI_RXCTRL	0x03000	/* RX control */

#define TEI_PFCTOP	0x03008	/* Priority flow control type opcode */
#define TEI_FCTTV0	0x03200	/* Flow control TX timer value */
#define TEI_FCTTV1	0x03204	/* Flow control TX timer value */
#define TEI_FCTTV2	0x03208	/* Flow control TX timer value */
#define TEI_FCTTV3	0x0320C	/* Flow control TX timer value */
#define TEI_FCRTL0	0x03220	/* Flow control RX threshold low */
#define TEI_FCRTL1	0x03228	/* Flow control RX threshold low */
#define TEI_FCRTL2	0x03230	/* Flow control RX threshold low */
#define TEI_FCRTL3	0x03238	/* Flow control RX threshold low */
#define TEI_FCRTL4	0x03240	/* Flow control RX threshold low */
#define TEI_FCRTL5	0x03248	/* Flow control RX threshold low */
#define TEI_FCRTL6	0x03250	/* Flow control RX threshold low */
#define TEI_FCRTL7	0x03258	/* Flow control RX threshold low */
#define TEI_FCRTH0	0x03260	/* Flow control RX threshold high */
#define TEI_FCRTH1	0x03268	/* Flow control RX threshold high */
#define TEI_FCRTH2	0x03270	/* Flow control RX threshold high */
#define TEI_FCRTH3	0x03278	/* Flow control RX threshold high */
#define TEI_FCRTH4	0x03280	/* Flow control RX threshold high */
#define TEI_FCRTH5	0x03288	/* Flow control RX threshold high */
#define TEI_FCRTH6	0x03290	/* Flow control RX threshold high */
#define TEI_FCRTH7	0x03298	/* Flow control RX threshold high */
#define TEI_FCRTV	0x032A0	/* Flow control RX threshold value */

#define TEI_RXPBSIZE0	0x03C00	/* RX packet buffer size */
#define TEI_RXPBSIZE7	0x03C1C	/* RX packet buffer size */

#define TEI_RMCS	0x03D00 /* TX flow control (DCB related) */
#define TEI_FCCFG	TEI_RMCS /* TX flow control - 82599 */

#define TEI_DROPEN0	0x03D04	/* Drop enable control */
#define TEI_DROPEN1	0x03D08	/* Drop enable control */

#define TEI_PXONTXC0	0x03F00 /* Priority XON TX count */
#define TEI_PXONTXC7	0x03F1C /* Priority XON TX count */
#define TEI_PXOFFTXC0	0x03F20 /* Priority XOFF TX count */
#define TEI_PXOFFTXC7	0x03F2C /* Priority XOFF TX count */

#define TEI_LXONTXC	0x03F60 /* Link XON TX count */
#define TEI_LXOFFTXC	0x03F68 /* Link XOFF TX count */

#define TEI_MPC0	0x03FA0	/* Missed packets count */
#define TEI_MPC7	0x03FBC	/* Missed packets count */

#define TEI_RNBC0	0x03FC0	/* RX no buffers count */
#define TEI_RNBC7	0x03FDC	/* RX no buffers count */

#define TEI_CRCERRS	0x04000 /* CRC error count */
#define TEI_ILLERRC	0x04004 /* Illegal byte error count */
#define TEI_ERRBC	0x04008 /* Error byte count */
#define TEI_MSPDC	0x04010 /* MAC short packet discard count */
#define TEI_MLCF	0x04034 /* MAC local fault count */
#define TEI_MRFC	0x04038 /* MAC remote fault count */
#define TEI_RLEC	0x04040 /* RX length error count */
#define TEI_PRC64	0x0405C	/* RX 64 byte frame count */
#define TEI_PRC127	0x04060	/* RX 65-127 byte frame count */
#define TEI_PRC255	0x04064	/* RX 128-255 byte frame count */
#define TEI_PRC511	0x04068	/* RX 256-511 byte frame count */
#define TEI_PRC1023	0x0406C	/* RX 512-1023 byte frame count */
#define TEI_PRC1522	0x04070	/* RX 1024-1522 byte frame count */
#define TEI_GPRC	0x04074	/* RX good packet count */
#define TEI_BPRC	0x04078	/* RX bcast packet count */
#define TEI_MPRC	0x0407C	/* RX mcast packet count */
#define TEI_GPTC	0x04080	/* TX good packet count */
#define TEI_GORCL	0x04088	/* RX good octet count low word for 82599 */
#define TEI_GORC	0x0408C	/* RX good octet count or high word for 82599 */
#define TEI_GOTCL	0x04090	/* TX good octet count word for 82599 */
#define TEI_GOTC	0x04094	/* TX good octet count or high word for 82599 */
#define TEI_RUC		0x040A4	/* RX runt frame count */
#define TEI_RFC		0x040A8	/* RX fragment count */
#define TEI_ROC		0x040AC	/* RX giant frame count */
#define TEI_RJC		0x040B0	/* RX jabber count */
#define TEI_MNGPRC	0x040B4	/* RX mgmt frame count */
#define TEI_MNGPDC	0x040B4	/* RX mgmt frame dropped count */
#define TEI_TORL	0x040C0	/* RX total octets, low */
#define TEI_TORH	0x040C4	/* RX total octets, high */
#define TEI_TPR		0x040D0	/* RX total packets */
#define TEI_TPT		0x040D4	/* TX total octets */
#define TEI_PTC64	0x040D8	/* TX 64 byte frame count */
#define TEI_PTC127	0x040DC	/* TX 65-127 byte frame count */
#define TEI_PTC255	0x040E0	/* TX 128-255 byte frame count */
#define TEI_PTC511	0x040E4	/* TX 256-511 byte frame count */
#define TEI_PTC1023	0x040E8	/* TX 512-1023 byte frame count */
#define TEI_PTC1522	0x040EC	/* TX 1024-1522 byte frame count */
#define TEI_MPTC	0x040F0	/* TX mcast frames */
#define TEI_BPTC	0x040F4	/* TX bcast frames */
#define TEI_XEC		0x04120 /* XSUM error count */

#define TEI_PCS1GCFIG	0x04200 /* PCS_1G Global config 1 */
#define TEI_PCS1CLCTL	0x04208 /* PCS_1G Link control */
#define TEI_PCS1CLSTA	0x0420C /* PCS_1G Link status */
#define TEI_PCS1CLANA	0x04218 /* PCS_1G Autoneg advanced */
#define TEI_PCS1CLANLP	0x0421C /* PCS_1G AN link partner ability */
#define TEI_PCS1GANNP	0x04220 /* PCS_1G AN Next Page TX */
#define TEI_PCS1GANLPNP	0x04224 /* PCS_1G AN LP's Next Page */
#define TEI_HLREG0	0x04240 /* Flow control 0 */
#define TEI_HLREG1	0x04244 /* Flow control status 1 */
#define TEI_PAP		0x04248 /* Pause and pace */
#define TEI_MACA	0x0424C /* MDI auto scan cmd and addr */
#define TEI_APAE	0x04250 /* Autoscan PHY addr enable */
#define TEI_ARD		0x04254 /* Autoscan read data */
#define TEI_AIS		0x04258 /* Autoscan intr status */
#define TEI_MSCA	0x0425C /* MDI signal cmd and addr */
#define TEI_MSRWD	0x04260 /* MDI Single Read and Write data */
#define TEI_MHADD	0x04268 /* MAC addr high and max frame size */
#define TEI_MAXFRS	TEI_MHADD
#define TEI_PCSS1	0x04288 /* XGXS Status 1 */
#define TEI_PCSS2	0x0428C /* XGXS Status 2 */
#define TEI_XPCSS	0x04290 /* 10GBase-X PCS Status */
#define TEI_MFLCN	0x04294 /* MAC flow control -- 82599 only */
#define TEI_SERDESC	0x04298 /* SerDes IF control */
#define TEI_MACS	0x0429C /* FIFO Status/Ctrl Report */
#define TEI_AUTOC	0x042A0	/* Auto-Detect Ctrl/Sts */
#define TEI_LINKS	0x042A4	/* Link Status */
#define TEI_AUTOC2	0x042A8	/* Autoneg Ctrl 2 */
#define TEI_AUTOC3	0x042AC	/* Autoneg Ctrl 3 */
#define TEI_ANLP1	0x042B0	/* Autoneg LP Ctrl Word 1 */
#define TEI_ANLP2	0x042B4	/* Autoneg LP Ctrl Word 2 */
#define TEI_MMNGC	0x042D0	/* MAC Manageability ctrl */
#define TEI_ANLPNP1	0x042D4	/* Aneg LP next page 1 */
#define TEI_ANLPNP2	0x042D8 /* Aneg LP next page 2 */
#define TEI_ATLASCTL	0x04800 /* Core analog config */
#define TEI_DMATXCTL	0x04A80 /* TX DMA control -- 82599 only */

#define TEI_RXCSUM	0x05000	/* RX checksum control */
#define TEI_RFCTL	0x05008	/* RX filter control */

#define TEI_MVTV0	0x05010 /* VLAN TAG value */
#define TEI_MVTV7	0x0502C /* VLAN TAG value */

#define TEI_MFUTP0	0x05030 /* Mgmt Flex UDP/TCP ports */
#define TEI_MFUTP7	0x0504C /* Mgmt Flex UDP/TCP ports */

#define TEI_FCTL	0x05080	/* filter control */
#define TEI_VLNCTL	0x05088	/* VLAN control */
#define TEI_MCSTCTL	0x05090	/* Multicast control */

#define TEI_MAR0	0x05200	/* Multicast hash table */
#define TEI_MAR127	0x053FC	/* Multicast hash table */

#define TEI_PARL0	0x05400	/* RX address low */
#define TEI_PARH0	0x05404	/* RX address high */
#define TEI_PARL15	0x05478	/* RX address low */
#define TEI_PARH15	0x0547C	/* RX address high */

#define TEI_PAR_CNT     15
#define TEI_MAR_CNT     128

#define TEI_PSRTYPE0	0x05480	/* Packet split RX type */
#define TEI_PSRTYPE15	0x054BC	/* Packet split RX type */

#define TEI_WUC		0x05800	/* Wake up control */
#define TEI_WUFC	0x05808	/* Wake up filter control */
#define TEI_WUS		0x05810	/* Wake up status */

#define TEI_MRQC	0x05818	/* Multiple receive queues command */
#define TEI_VMD_CTL	0x0581C	/* VMDq control */

#define TEI_MANC	0x05820 /* Mgmt control */
#define TEI_MFVAL	0x05824 /* Mgmt Filters Valid */

#define TEI_IPAV	0x05830	/* IP address valid */
#define TEI_IP4AT0	0x05840 /* IP address table */
#define TEI_MANC2H	0x05860 /* Mgmt control to host */
#define TEI_IP4ATX	0x05858 /* IP address table */
#define TEI_IP6AT0	0x05880 /* IP address table */
#define TEI_IP6ATX	0x0588F /* IP address table */

#define TEI_MDEF0	0x05890 /* Manageability filters valid */
#define TEI_MDEF7	0x058AC /* Manageability filters valid */
#define TEI_MIPAF0	0x058B0	/* Manageability IP addr filter */
#define TEI_MIPAFX	0x058EC	/* Manageability IP addr filter */

#define TEI_WUPL	0x05900	/* Wake up packet length */
#define TEI_MMAL0	0x05910	/* Manageability MAC address low 0 */
#define TEI_MMAH0	0x05914	/* Manageability MAC address high 0 */
#define TEI_MMAL3	0x05928	/* Manageability MAC address low 0 */
#define TEI_MMAH3	0x0592C	/* Manageability MAC address high 0 */

#define TEI_WUPM0	0x05A00	/* Wake up packet memory */
#define TEI_WUPMX	0x05A7C	/* Wake up packet memory */

#define TEI_IMIR0	0x05A80	/* Immediate Interrupt RX */
#define TEI_IMIR1	0x05A84	/* Immediate Interrupt RX */
#define TEI_IMIR2	0x05A88	/* Immediate Interrupt RX */
#define TEI_IMIR3	0x05A8C	/* Immediate Interrupt RX */
#define TEI_IMIR4	0x05A90	/* Immediate Interrupt RX */
#define TEI_IMIR5	0x05A94	/* Immediate Interrupt RX */
#define TEI_IMIR6	0x05A98	/* Immediate Interrupt RX */
#define TEI_IMIR7	0x05A9C	/* Immediate Interrupt RX */

#define TEI_IMIREXT0	0x05AA0	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT1	0x05AA4	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT2	0x05AA8	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT3	0x05AAC	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT4	0x05AB0	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT5	0x05AB4	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT6	0x05AB8	/* Immediate Interrupt RX extended */
#define TEI_IMIREXT7	0x05ABC	/* Immediate Interrupt RX extended */

#define TEI_IMIRVP	0x05AC0	/* Immediate interrupt RX VLAN prio */

#define TEI_RETA	0x05C00	/* Redirection table, 82598 or 82599 */
#define TEI_RETA0     TEI_RETA0 /* 1st reg. in redirection table */
#define TEI_RETAXX	0x05C7C	/* last 4-byte reg. in redirection table */
#define TEI_RETA99      0x0EB00 /* Alternate RETA location for 82599 */

#define TEI_RSSRK0	0x05C80	/* RSS Random Key */
#define TEI_RSSRKXX	0x05CAF	/* RSS Random Key */

#define TEI_TDBAL0	0x06000	/* TX descriptor base low */
#define TEI_TDBAH0	0x06004	/* TX descriptor base high */
#define TEI_TDLEN0	0x06008	/* TX descriptor length */
#define TEI_TDH0	0x06010	/* TX descriptor head */
#define TEI_TDT0	0x06018	/* TX descriptor tail */
#define TEI_TXDCTL0	0x06028	/* TX descriptor control */
#define TEI_QBTC	0x06034	/* TX queue bytes transmitted */
#define TEI_TDWBAL0	0x06038	/* TX writeback address low */
#define TEI_TDWBAH0	0x0603C	/* TX writeback address high */

#define TEI_TDBAL	TEI_TDBAL0
#define TEI_TDBAH	TEI_TDBAH0
#define TEI_TDLEN	TEI_TDLEN0
#define TEI_TDH		TEI_TDH0
#define TEI_TDT		TEI_TDT0
#define TEI_TXDCTL	TEI_TXDCTL0

#define TEI_TQSMR	0x07300 /* TX Queue Statistics Mapping */

#define TEI_DCA_TXCTL0	0x07200 /* TX DCA control */
#define TEI_DCA_TXCTL15	0x0723C /* TX DCA control */

#define TEI_TDTXCTL	0x07E00	/* TX DMA control */

#define TEI_FHFT0	0x09000	/* Flexible host filter table  */
#define TEI_FHFTX	0x093FC /* Flexible host filter table */

#define TEI_FTFT0	0x09400 /* Flexible TCP filter table */
#define TEI_FTFTX	0x097FC /* Flexible TCP filter table */

#define TEI_VFTA0	0x0A000 /* VLAN Filter table array */
#define TEI_VFTA640	0x0A9FC /* VLAN Filter table array */

#define TEI_PARL0_599	0x0A200	/* RX address low */
#define TEI_PARH0_599	0x0A204	/* RX address high */
#define TEI_PARL127	0x0A5F8	/* RX address low */
#define TEI_PARH127	0x0A5FC	/* RX address high */

#define TEI_PAR_CNT_599	127

#define TEI_TIPG	0x0CB00	/* TX IPG control */

#define TEI_TXPBSIZE0	0x0CC00	/* TX packet buffer size */
#define TEI_TXPBSIZE7	0x0CC1C	/* TX packet buffer size */

#define TEI_MNGTXMAP	0x0CD10	/* Managability TX mapping */

#define TEI_TFCS	0x0CE00	/* TX flow control status */

#define TEI_PXONRXC0	0x0CF00 /* Priority XON RX count */
#define TEI_PXONRXC7	0x0CF1C /* Priority XON RX count */
#define TEI_PXOFFRXC0	0x0CF20 /* Priority XOFF RX count */
#define TEI_PXOFFRXC7	0x0CF2C /* Priority XOFF RX count */

#define TEI_LXONRXC	0x0CF60 /* Link XON RX count */
#define TEI_LXOFFRXC	0x0CF68 /* Link XOFF RX count */
#define TEI_MNGPTC	0x0CF90	/* TX mgmt frame count */

#define TEI_EEC		0x10010	/* EEPROM/Flash control */
#define TEI_EERD	0x10014	/* EEPROM read */
#define TEI_FLA		0x1001C	/* Flash access */
#define TEI_EEMNGCTL	0x10110	/* Managability EEPROM control */
#define TEI_EEMNGDATA	0x10114	/* Managability EEPROM data */
#define TEI_FLMNGCTL	0x10118	/* Managability flash control */
#define TEI_FLMNGDATA	0x1011C	/* Managability flash data */
#define TEI_FLMNGCNT	0x10120	/* Managability flash read counter */
#define TEI_FLOP	0x1013C	/* Flash opcode */

#define TEI_SWSM	0x10140	/* Software semaphore */
#define TEI_FWSM	0x10148	/* Firmware semaphore */
#define TEI_FACTPS	0x10150 /* Function Active and Power State */
#define TEI_GSSR	0x10160 /* General Software Semaphore */

#define TEI_GRC		0x10200	/* General receive control */

#define TEI_GCR		0x11000 /* PCIe Control */
#define TEI_GTV		0x11004 /* PCIe Timer Value */
#define TEI_FUNCTAG	0x11008 /* Function Tag */
#define TEI_GLT		0x1100C /* PCIe Latency Timer */
#define TEI_GSCL1	0x11010 /* PCIe Stats control 1 */
#define TEI_GSCL2	0x11014 /* PCIe Stats control 2 */
#define TEI_GSCL3	0x11018 /* PCIe Stats control 3 */
#define TEI_GSCL4	0x1101C /* PCIe Stats control 4 */
#define TEI_GSCN0	0x11020	/* PCIe counter 0 */
#define TEI_GSCN1	0x11024	/* PCIe counter 1 */
#define TEI_GSCN2	0x11028	/* PCIe counter 2 */
#define TEI_GSCN3	0x1102C	/* PCIe counter 3 */
#define TEI_PCIEANACTL	0x11040	/* PCIe Analong config */
#define TEI_MREVID	0x11064 /* Mirrored rev ID */
#define TEI_PBACL	0x11068	/* MSI-X PBA clear */
#define TEI_DCA_ID	0x11070 /* DCA Requester ID info */
#define TEI_DCA_CTRL	0x11074 /* DCA control */

#define TEI_GHECCR	0x110B0 /* ECC control register */

/*
 * The 82599 has up to 16 RX queues distinguishable via RSS,
 * when virtualization is not enabled.
 * (It has a total of 128 sets of RX queue registers available
 * in 2 groups of 64, starting addresses 0x01000 and 0x0D000.)
 * Register sets are separated by 0x40 bytes.
 * The 82599 has a total of 128 sets of TX queue registers.
 */
#define TEI_RXQ_NUM_99	16
#define TEI_RXQ_IVL_99  0x40
#define TEI_TXQ_NUM_99	128
#define TEI_TXQ_IVL_99  0x40

/*
 * The 82598 has another BAR containing MSI-X specific registers.
 * BAR 3
 */

#define TEI_MSIXT0	0x00000	/* MSI-X table */
#define TEI_MSIXTXX	0x0013C	/* MSI-X table */
#define TEI_MSIXPBA	0x02000	/* MSI-X pending bit array */

/* Device control register */

#define TEI_CTRL_PCIE_MST_DIS	0x00000004 /* PCIe master disable */
#define TEI_CTRL_LRST		0x00000008 /* Link reset */
#define TEI_CTRL_RST		0x04000000 /* Device reset */

/* Device status register */

#define TEI_STS_LANID		0x0000000C /* LAN ID */
#define TEI_STS_PCIE_MSTENB	0x00080000 /* PCIe master enable status */

#define TEI_LANID_0		0x00000000
#define TEI_LANID_1		0x00000004

/* Extended device control register */

#define TEI_CTRLX_NS_DIS	0x00010000 /* No snoop disable */
#define TEI_CTRLX_RO_DIS	0x00020000 /* Relaxed ordering disable */
#define TEI_CTRLX_DRV_LOAD	0x10000000 /* Driver loaded */

/* Extended SDP control */

#define TEI_ESDP_SDP0_DATA	0x00000001
#define TEI_ESDP_SDP1_DATA	0x00000002
#define TEI_ESDP_SDP2_DATA	0x00000004
#define TEI_ESDP_SDP3_DATA	0x00000008
#define TEI_ESDP_SDP4_DATA	0x00000010
#define TEI_ESDP_SDP5_DATA	0x00000020
#define TEI_ESDP_IODIR0		0x00000100
#define TEI_ESDP_IODIR1		0x00000200
#define TEI_ESDP_IODIR2		0x00000400
#define TEI_ESDP_IODIR3		0x00000800
#define TEI_ESDP_IODIR4		0x00001000
#define TEI_ESDP_IODIR5		0x00002000

/* Extended OD SDP control */

#define TEI_EODSDP_SDP6_DIN	0x00000001
#define TEI_EODSDP_SDP6_DOUT	0x00000002
#define TEI_EODSDP_SDP7_DIN	0x00000004
#define TEI_EODSDP_SDP7_DOUT	0x00000008

/* LED Control */

#define TEI_LEDCTL_LED0_MODE	0x0000000F /* LED0 mode */
#define TEI_LEDCTL_GBL_BLINK	0x00000020 /* Global blink mode */
#define TEI_LEDCTL_LED0_IVRT	0x00000040 /* LED0 invert */
#define TEI_LEDCTL_LED0_BLINK	0x00000080 /* LED0 blink */
#define TEI_LEDCTL_LED1_MODE	0x00000F00 /* LED1 mode */
#define TEI_LEDCTL_LED1_IVRT	0x00004000 /* LED1 invert */
#define TEI_LEDCTL_LED1_BLINK	0x00008000 /* LED1 blink */
#define TEI_LEDCTL_LED2_MODE	0x000F0000 /* LED2 mode */
#define TEI_LEDCTL_LED2_IVRT	0x00400000 /* LED2 invert */
#define TEI_LEDCTL_LED2_BLINK	0x00800000 /* LED2 blink */
#define TEI_LEDCTL_LED3_MODE	0x0F000000 /* LED3 mode */
#define TEI_LEDCTL_LED3_IVRT	0x40000000 /* LED3 invert */
#define TEI_LEDCTL_LED3_BLINK	0x80000000 /* LED3 blink */

#define TEI_LED_LINKUP		0x00000000
#define TEI_LED_LINK10G		0x00000001
#define TEI_LED_MAC_ACTIVITY	0x00000002
#define TEI_LED_FILTER_ACTIVITY	0x00000003
#define TEI_LED_LINK_ACT	0x00000004
#define TEI_LED_LINK_1G		0x00000005
#define TEI_LED_ON		0x0000000E
#define TEI_LED_OFF		0x0000000F

/* TCP Timer */

#define TEI_TCPTIMER_DURATION	0x00000FF /* Timer duration in ms */
#define TEI_TCPTIMER_KICKSTART	0x0000100 /* Counter kick-start */
#define TEI_TCPTIMER_COUNTEN	0x0000200 /* Count enable */
#define TEI_TCPTIMER_COUNTFIN	0x0000400 /* Count finish */
#define TEI_TCPTIMER_LOOP	0x0000800 /* TCP loop */


/* Extended Interrupt Cause Register */

#define TEI_EICR_RTXQ0		0x00000001 /* RX/TX queue 0 interrupt */
#define TEI_EICR_RTXQ1		0x00000002 /* RX/TX queue 1 interrupt */
#define TEI_EICR_RTXQ2		0x00000004 /* RX/TX queue 2 interrupt */
#define TEI_EICR_RTXQ3		0x00000008 /* RX/TX queue 3 interrupt */
#define TEI_EICR_RTXQ4		0x00000010 /* RX/TX queue 4 interrupt */
#define TEI_EICR_RTXQ5		0x00000020 /* RX/TX queue 5 interrupt */
#define TEI_EICR_RTXQ6		0x00000040 /* RX/TX queue 6 interrupt */
#define TEI_EICR_RTXQ7		0x00000080 /* RX/TX queue 7 interrupt */
#define TEI_EICR_RTXQ8		0x00000100 /* RX/TX queue 8 interrupt */
#define TEI_EICR_RTXQ9		0x00000200 /* RX/TX queue 9 interrupt */
#define TEI_EICR_RTXQ10		0x00000400 /* RX/TX queue 10 interrupt */
#define TEI_EICR_RTXQ11		0x00000800 /* RX/TX queue 11 interrupt */
#define TEI_EICR_RTXQ12		0x00001000 /* RX/TX queue 12 interrupt */
#define TEI_EICR_RTXQ13		0x00002000 /* RX/TX queue 13 interrupt */
#define TEI_EICR_RTXQ14		0x00004000 /* RX/TX queue 14 interrupt */
#define TEI_EICR_RTXQ15		0x00008000 /* RX/TX queue 15 interrupt */
#define TEI_EICR_FLOWDIR        0x00010000 /* 82599 - flow director */
#define TEI_EICR_RXMISS         0x00020000 /* 82599 - RX miss interrupt */
#define TEI_EICR_PCI            0x00040000 /* 82599 - PCI Exception */
#define TEI_EICR_MAILBOX        0x00080000 /* 82599 - Mailbox interupt */
#define TEI_EICR_LSC		0x00100000 /* Link status change */
#define TEI_EICR_LINKSEC	0x00200000 /* 82599 - LinkSec */
#define TEI_EICR_MNG		0x00400000 /* Manageablilty event */
#define TEI_EICR_SDP0		0x01000000 /* Gen. Purp. SDP0 interrupt */
#define TEI_EICR_SDP1		0x02000000 /* Gen. Purp. SDP1 interrupt */
#define TEI_EICR_SDP2		0x04000000 /* Gen. Purp. SDP2 interrupt */
#define TEI_EICR_SDP3		0x08000000 /* Gen. Purp. SDP3 interrupt */
#define TEI_EICR_PBUR		0x10000000 /* RX/TX pkt buffer error */
#define TEI_EICR_DHER		0x20000000 /* RX/TX desc handler err (82598) */
#define TEI_EICR_TCPTIMER	0x40000000 /* TCP timer expired */
#define TEI_EICR_OTHER		0x80000000

/* Interrup throttling register */

#define TEI_EITR_INTERVAL	0x0000FFFF
#define TEI_EITR_COUNTER	0xFFFF0000

#define TEI_IRQ_PER_SEC(x)	(1000000000/((x) * 256))

/* Interrupt vector allocation register */

#define TEI_IVAR_ALLOC0		0x0000001F
#define TEI_IVAR_599_ALLOC0	0x0000003F
#define TEI_IVAR_VALID0		0x00000080
#define TEI_IVAR_ALLOC1		0x00001F00
#define TEI_IVAR_599_ALLOC1	0x00003F00
#define TEI_IVAR_VALID1		0x00008000
#define TEI_IVAR_ALLOC2		0x001F0000
#define TEI_IVAR_599_ALLOC2	0x003F0000
#define TEI_IVAR_VALID2		0x00800000
#define TEI_IVAR_ALLOC3		0x1F000000
#define TEI_IVAR_599_ALLOC3	0x3F000000
#define TEI_IVAR_VALID3		0x80000000

#define TEI_ALLOC0(x)		((x) & TEI_IVAR_ALLOC0)
#define TEI_ALLOC1(x)		(((x) << 8) & TEI_IVAR_ALLOC1)
#define TEI_ALLOC2(x)		(((x) << 16) & TEI_IVAR_ALLOC2)
#define TEI_ALLOC3(x)		(((x) << 24) & TEI_IVAR_ALLOC3)

#define TEI_599_ALLOC0(x)	((x) & TEI_IVAR_599_ALLOC0)
#define TEI_599_ALLOC1(x)	(((x) << 8) & TEI_IVAR_599_ALLOC1)
#define TEI_599_ALLOC2(x)	(((x) << 16) & TEI_IVAR_599_ALLOC2)
#define TEI_599_ALLOC3(x)	(((x) << 24) & TEI_IVAR_599_ALLOC3)

#define TEI_IVAR_RXQ(x)	\
	(TEI_IVAR0 + (4 * (x)))

#define TEI_IVAR_TXQ(x)	\
	(TEI_IVAR0 + (4 * (x)) + 64)

#define TEI_RXQ 0
#define TEI_TXQ 1

/* MAC flow control register -- 82599 only */

#define TEI_MFLCN_PMCF		0x00000001 /* Pass MAC control frames */
#define TEI_MFLCN_DPF		0x00000002 /* Discard pause frame */
#define TEI_MFLCN_RPFCE		0x00000004 /* RX priority flow ctl enable */
#define TEI_MFLCN_RFCE		0x00000008 /* RX flow ctl enable */

/* Autodetect control/status */

#define TEI_AUTOC_FLU		0x00000001 /* Force link up */
#define TEI_AUTOC_ANACK2	0x00000002 /* AN acknowledfe2 field */
#define TEI_AUTOC_ANSD		0x0000007C /* AN Selector */
#define TEI_AUTOC_10G_PMA_PMD	0x00000180 /* 10G PMA/PMD */
#define TEI_AUTOC_1G_PMA_PMD	0x00000200 /* 1G PMA/PMD */
#define TEI_AUTOC_D10GMP	0x00000400 /* Disable 10G/KX4 on power loss */
#define TEI_AUTOC_RATD		0x00000800 /* Restart autoneg on transition */
#define TEI_AUTOC_RESTART_AN	0x00001000 /* Restart KX/KX4 autoneg */
#define TEI_AUTOC_LMS		0x0000E000 /* Link mode select */
#define TEI_AUTOC_ANRXAT	0x007C0000 /* Autoneg RX align threshold */
#define TEI_AUTOC_ANRXDM	0x00800000 /* Autoneg RX drift mode */
#define TEI_AUTOC_ANRXLM	0x01000000 /* Autoneg RX loose mode */
#define TEI_AUTOC_ANPDT		0x06000000 /* Autoneg par. detect timer */
#define TEI_AUTOC_RF		0x08000000 /* RF in autoneg word */
#define TEI_AUTOC_PB		0x30000000 /* Pause bits */
#define TEI_AUTOC_KXSUP		0xC0000000 /* KX support */

#define TEI_10GPMAPMD_XAUI	0x00000000 /* XAUI PMA/PMD */
#define TEI_10GPMAPMD_KX4	0x00000080 /* KX4 PMA/PMD */
#define TEI_10GPMAPMD_CX4	0x00000100 /* CX4 PMA/PMD */

#define TEI_1GPMAPMD_BX		0x00000000 /* BX PMA/PMD */
#define TEI_1GPMAPMD_KX		0x00000200 /* KX PMA/PMD */

#define TEI_LMS_1G		0x00000000 /* 1G, no autoneg */
#define TEI_LMS_10G		0x00002000 /* 10G, no autoneg */
#define TEI_LMS_1GAN_C37	0x00004000 /* 1G with clause 37 autoneg */
#define TEI_LMS_KX4_KX_C37	0x00008000 /* KX4/KX with clause 37 autoneg */
#define TEI_LMS_KX4_KX		0x0000C000 /* KX4/KX autoneg */

/* Link status */

#define TEI_LINKS_KX_KX4_NPR	0x00000020 /* KX/KX4 next page received */
#define TEI_LINKS_KX_KX4_ANPR	0x00000040 /* KX/KX4 autoneg page received */
#define TEI_LINKS_LINKSTS	0x00000080 /* Link status */
#define TEI_LINKS_SIGDET	0x00000F00 /* Signal detect */
#define TEI_LINKS_TXLOCALFLT	0x00001000 /* TX local fault */
#define TEI_LINKS_10GLN0_SYNSTS	0x00002000 /* 10G lane 0 sync sts */
#define TEI_LINKS_10GLN1_SYNSTS	0x00004000 /* 10G lane 1 sync sts */
#define TEI_LINKS_10GLN2_SYNSTS	0x00008000 /* 10G lane 2 sync sts */
#define TEI_LINKS_10GLN3_SYNSTS	0x00010000 /* 10G lane 3 sync sts */
#define TEI_LINKS_10G_ALIGNSTS	0x00020000 /* 10G align status */
#define TEI_LINKS_1G_SYNSTS	0x00040000 /* 1G sync status */
#define TEI_LINKS_KX_KX4_RXIDLE	0x00080000 /* KX/KX4 autoneg RX idle */
#define TEI_LINKS_1G_AN_ENB	0x00100000 /* 1G autoneg on (clause 37) */
#define TEI_LINKS_PCS_1G	0x00200000 /* PCS_1G enabled for 1Gbps */
#define TEI_LINKS_XGXS_10G	0x00400000 /* XGXS enables for 10G */
#define TEI_LINKS_SPEED		0x20000000 /* Link speed */
#define TEI_LINKS_LINKUP	0x40000000 /* Link up */
#define TEI_LINKS_KXK4_ANDONE	0x80000000 /* KX/KX4 autoneg completed */

/* EEPROM/flash control */

#define TEI_EEC_CLKIN		0x00000001 /* Clock input */
#define TEI_EEC_CS		0x00000002 /* Chip select */
#define TEI_EEC_DATAIN		0x00000004 /* Data input to EEPROM */
#define TEI_EEC_DATAOUT		0x00000008 /* Data out from EEPROM */
#define TEI_EEC_FWCTL		0x00000030 /* Flash write enable ctl */
#define TEI_EEC_REQ		0x00000040 /* Request EEPROM access */
#define TEI_EEC_GNT		0x00000080 /* Grant EEPROM access */
#define TEI_EEC_PRES		0x00000100 /* EEPROM present */
#define TEI_EEC_ARDONE		0x00000200 /* EEPROM autoload done */
#define TEI_EEC_EEASIZE		0x00000400 /* EEPROM address size */
#define TEI_EEC_EESIZE		0x00007800 /* EEPROM size */
#define TEI_EEC_PCIANDONE	0x00008000 /* PCIe analog done */
#define TEI_EEC_PCICOREDONE	0x00010000 /* PCIe core done */
#define TEI_EEC_PCIGENDONE	0x00020000 /* PCIe general done */
#define TEI_EEC_PCIFUNCDONE	0x00040000 /* PCIe function done */
#define TEI_EEC_COREDONE	0x00080000 /* Core done */
#define TEI_EEC_CORECSRDONE	0x00100000 /* Core CSR done */
#define TEI_EEC_MACDONE		0x00200000 /* MAC done */

#define TEI_FWCTL_ERASE		0x00000000
#define TEI_FWCTL_WRITE_DIS	0x00000010
#define TEI_FWCTL_WRITE_EN	0x00000020

#define TEI_EEASIZE_8OR9	0x00000000
#define TEI_EEASIZE_16		0x00000400

#define TEI_EESIZE_128B		0x00000000
#define TEI_EESIZE_256B		0x00000800
#define TEI_EESIZE_512B		0x00001000
#define TEI_EESIZE_1KB		0x00001800
#define TEI_EESIZE_2KB		0x00002000
#define TEI_EESIZE_4KB		0x00002800
#define TEI_EESIZE_8KB		0x00003000
#define TEI_EESIZE_16KB		0x00003800
#define TEI_EESIZE_32KB		0x00004000

/* EEPROM Read Register */

#define TEI_EERD_START		0x00000001
#define TEI_EERD_DONE		0x00000002
#define TEI_EERD_ADDR		0x0000FFFC
#define TEI_EERD_DATA		0xFFFF0000

#define TEI_EEADDR(x)		(((x) << 2) & TEI_EERD_ADDR)
#define TEI_EEDATA(x)		(((x) & TEI_EERD_DATA) >> 16)

/* Flash access register */

#define TEI_FLA_SCK		0x00000001 /* Flash clock input */
#define TEI_FLA_CE		0x00000002 /* Flash chip select */
#define TEI_FLA_SI		0x00000004 /* Flash data input */
#define TEI_FLA_SO		0x00000008 /* Flash data output */
#define TEI_FLA_REQ		0x00000010 /* Request flash access */
#define TEI_FLA_GNT		0x00000020 /* Flash access granted */
#define TEI_FLA_BUSY		0x40000000 /* Flash busy */
#define TEI_FLA_ER		0x80000000 /* Erase command */

/* Manageability EEPROM control register */

#define TEI_EEMNGCTL_ADDR	0x0000EFFF /* Address offset */
#define TEI_EEMNGCTL_START	0x00008000 /* Start transaction */
#define TEI_EEMNGCTL_WRITE	0x00010000 /* 1 = write, 0 = read */
#define TEI_EEMNGCTL_EEBUSY	0x00020000 /* EEPROM busy */
#define TEI_EEMNGCTL_CFGDONE	0x00040000 /* Config cycle done */
#define TEI_EEMNGCTL_DONE	0x80000000 /* Transaction done */

/* Manageability EEPROM data register */

#define TEI_EEMNGDATA_WRDATA	0x0000FFFF
#define TEI_EEMNGDATA_RDDATA	0xFFFF0000

/* RX descriptor control */

#define TEI_RXDCTL_PTHRESH	0x0000007F /* Pre-fetch threshold */
#define TEI_RXDCTL_HTHRESH	0x00007F00 /* Host threshold */
#define TEI_RXDCTL_WTHRESH	0x007F0000 /* Write-back threshold */
#define TEI_RXDCTL_ENABLE	0x02000000 /* Queue enable */
#define TEI_RXDCTL_VME		0x40000000 /* VLAN Mode Enable */

#define TEI_PTHRESH(x)		((x) & TEI_RXDCTL_PTHRESH)
#define TEI_HTHRESH(x)		(((x) << 8) & TEI_RXDCTL_HTHRESH)
#define TEI_WTHRESH(x)		(((x) << 16) & TEI_RXDCTL_WTHRESH)

/* Split and replication RX control registers */

#define TEI_SRRCTL_BSIZEPKT	0x0000007F /* RX buffer size */
#define TEI_SRRCTL_BSIZEHDR	0x00003F00 /* RX header size */
#define TEI_SRRCTL_RDMTS        0x01C00000 /* RX Desc. Minimum Threshold Sz. */
#define TEI_SRRCTL_DESCTYPE	0x0E000000 /* RX Descriptor type */
#define TEI_SRRCTL_DROP_ENB     0x10000000 /* Drop Enabled */

#define TEI_DESCTYPE_LEGACY	0x00000000 /* Legacy */
#define TEI_DESCTYPE_ADV_ONE	0x02000000 /* Advanced, one buffer */
#define TEI_DESCTYPE_ADV_SP	0x04000000 /* Advanced, header splitting */
#define TEI_DESCTYPE_ADV_REP	0x06000000 /* Advanced, header replication */
#define TEI_DESCTYPE_ADV_LRREP	0x08000000 /* Adv, hdr repl, large pkt only */
#define TEI_DESCTYPE_ADV_SPALW	0x0A000000 /* Adv, hdr split always */

/* RX DMA control register */

#define TEI_RDRXCTL_RDMTS	0x00000003 /* RX desc min thresh size */
#define TEI_RDRXCTL_DMAIDONE	0x00000008 /* DMA Init done */
#define TEI_RDRXCTL_MVMEN	0x00000020 /* DMA cfg for MAC/VLAM reg map */
#define TEI_RDRXCTL_MCEN	0x00000040 /* DMA cfg for RSS reg map */
#define TEI_RDRXCTL_RXDPSIZE	0x00000F00 /* RX data pipe size, in KB */

#define TEI_RDMTS_HALF		0x00000000
#define TEI_RDMTS_QUARTER	0x00000001
#define TEI_RDMTS_EIGHTH	0x00000002

/* RX DMA control register for 82599 */

#define TEI_RDRXCTL99_CRCSTRIP     0x00000001
#define TEI_RDRXCTL99_DMAIDONE	   0x00000008 /* DMA Init done */
#define TEI_RDRXCTL99_RSCFRSTSIZE  0x003E0000 /* RSC first pkt size min */
#define TEI_RDRXCTL99_RSCACKC      0x02000000 /* RSC coalescing on ACK chg. */
#define TEI_RDRXCTL99_FCOE_WRFIX   0x04000000 /* FCoE Write exchange fix */

/* Receive control register */

#define TEI_RXCTRL_RXEN		0x00000001 /* Enable RX */
#define TEI_RXCTRL_DMBYPS	0x00000002 /* Descriptor monitor bypass */

/* Flow control TX timer value */

#define TEI_FCTTV_TTV0		0x0000FFFF /* XOFF timer 0 */
#define TEI_FCTTV_TTV1		0xFFFF0000 /* XOFF timer 1 */

#define TEI_FCTTV_0(x)		((x) & TEI_FCTTV_TTV0)
#define TEI_FCTTV_1(x)		(((x) << 16) & TEI_FCTTV_TTV1)

/* RX flow control threshold low */

#define TEI_FCRTL_RTL		0x0007FFF0 /* RX threshold low */
#define TEI_FCRTL_RTL_599	0x0007FFE0 /* RX threshold low (82599) */
#define TEI_FCRTL_XONE		0x80000000 /* XON enable */

#define TEI_RTL(x)		(((x) << 4) & TEI_FCRTL_RTL)

/* RX flow control threshold high */

#define TEI_FCRTL_RTH		0x0007FFF0 /* RX threshold low */
#define TEI_FCRTL_RTH_599	0x0007FFE0 /* RX threshold low (82599) */
#define TEI_FCRTL_FCEN		0x80000000 /* XON enable */

#define TEI_RTH(x)		(((x) << 4) & TEI_FCRTL_RTH)

/* Flow control threshold value */

#define TEI_FCRTV_REFRESHTHR	0x0000FFFF /* Refresh threshold */

/* RMCS register */

#define TEI_RMCS_RRM		0x00000002 /* RX recycle mode enable */
#define TEI_RMCS_RAC		0x00000004 /* RX arbitration mode control */
#define TEI_RMCS_TFCE_802_3X	0x00000008 /* TX flow control enable */
#define TEI_RMCS_TFCE_PRIO	0x00000010 /* TX priority flow control enable */
#define TEI_RMCS_ARBDIS		0x00000040 /* Arbitration disable */

/*
 * Flow control register 0
 * (which sees to have nothing to do with flow control)
 */

#define TEI_HLREG0_TXCRCEN	0x00000001 /* TX CRC enable */
#define TEI_HLREG0_RXCRCSTRP	0x00000002 /* RX CRC strip */
#define TEI_HLREG0_JUMBOEN	0x00000004 /* Enable jumbo frames */
#define TEI_HLREG0_TXPADEN	0x00000400 /* TX short frame pad enable */
#define TEI_HLREG0_LPBK		0x00008000 /* Loopback enable */
#define TEI_HLREG0_MDCSPD	0x00010000 /* MDIO clock speed select */
#define TEI_HLREG0_CONTMDC	0x00020000 /* Continuous MDC  */
#define TEI_HLREG0_PREPEND      0x00F00000 /* Prepend value */
#define TEI_HLREG0_RXLENERREN	0x08000000 /* RX length error reporting */

/* Flow control register 1 */

#define TEI_HLREG1_REVID	0x0000000F /* MAC core revision ID */
#define TEI_HLREG1_AUTOSCAN	0x00000010 /* Autoscan in progress */
#define TEI_HLREG1_RXERRSYM	0x00000020 /* RX error symbol */
#define TEI_HLREG1_RXILLSYM	0x00000040 /* RX illegal symbol */
#define TEI_HLREG1_RIDLERR	0x00000080 /* RX idle error */
#define TEI_HLREG1_RXLCLFLT	0x00000100 /* RX local fault */
#define TEI_HLREG1_RXRMTFLT	0x00000200 /* RX remote fault */
#define TEI_HLREG1_PACE		0x000F0000 /* Pace */

/* MAC address high and max frame size */

#define TEI_MHADD_MFS		0xFFFF0000 /* Max frame size */

#define TEI_MFS(x)		(((x) << 16) & TEI_MHADD_MFS)

/* DMA TX control -- 82599 only */

#define TEI_DMATXCTL_TE		0x00000001 /* TX enable */
#define TEI_DMATXCTL_NSHDR	0x00000002 /* No snoop header of TSO pkts */
#define TEI_DMATXCTL_GDV	0x00000008 /* Global double VLAN mode */
#define TEI_DMATXCTL_VT		0xFFFF0000 /* VLAN ether type */

/* RX checksum control */

#define TEI_RXCSUM_IPPCSE	0x00001000 /* IP fragment checksum enb */
#define TEI_RXCSUM_PCSD		0x00002000 /* 1 == RSS, 0 == fragment csum */

/* RX filter control register */

#define TEI_FCTL_SBP		0x00000002 /* Store bad packets */
#define TEI_FCTL_MPE		0x00000100 /* Multicast promisc enable */
#define TEI_FCTL_UPE		0x00000200 /* Unicast promisc enable */
#define TEI_FCTL_BAM		0x00000400 /* Broadcast accept mode */
#define TEI_FCTL_PMCF		0x00001000 /* Pass MAC control frames */
#define TEI_FCTL_DPF		0x00002000 /* Discard pause frames */
#define TEI_FCTL_RPFCE		0x00004000 /* RX prio flow control enable */
#define TEI_FCTL_RFCE		0x00008000 /* RX flow control enable */


/* Multiple Receive Queues Command Register */

#define TEI_MRQC_ENABLE_MODE	0x0000000F /* Enable mode */
#define TEI_MRQC_RSS_EN_MASK	0xFFFF0000 /* Enable individual RSS hashes */

/* For non VT systems, only 0x0 and 0x2 are supported for 82575;
   0x6 also supported for 82576. */

#define TEI_MRQC_MODE_DISBLED   0x0   /* Multiple RX Q's disabled */
#define TEI_MRQC_MODE_RSS_ONLY  0x1   /* 1 set of 16 RSS queues */
#define TEI_MRQC_MODE_DCBNORSS8 0x2   /* DCB/no RSS, 8TCs, 1 Q each */
#define TEI_MRQC_MODE_DCBNORSS4 0x3   /* DCB/no RSS, 4TCs, 1 Q each */
#define TEI_MRQC_MODE_DCBRSS8   0x4   /* DCB/RSS, 8TCs, 16 Q each */
#define TEI_MRQC_MODE_DCBRSS4   0x5   /* DCB/RSS, 4TCs, 16 Q each */
#define TEI_MRQC_MODE_VIRT_ONLY 0x8   /* Virt. only - 64 pools, 2Q each */
#define TEI_MRQC_MODE_VIRTRSS4  0xA   /* VIRT/RSS - 32 pools, 4Q each */
#define TEI_MRQC_MODE_VIRTRSS2  0xB   /* VIRT/RSS - 64 pools, 2Q each */
#define TEI_MRQC_MODE_VIRTDCB8  0xC   /* VIRT/DCB - 16 pools, 8 TC each */
#define TEI_MRQC_MODE_VIRTDCB4  0xD   /* VIRT/DCB - 32 pools, 4 TC each */

#define TEI_MRQC_RSS_EN_TCPIPV4  0x00010000 /* Enable TcpIPv4 hash function */
#define TEI_MRQC_RSS_EN_IPV4     0x00020000 /* Enable IPv4 hash function */
#define TEI_MRQC_RSS_EN_TCPIPV6X 0x00040000 /* RSVD (TcpIPv6Ex hash func) */
#define TEI_MRQC_RSS_EN_IPV6X    0x00080000 /* RSVD (IPv6Ex hash function) */
#define TEI_MRQC_RSS_EN_IPV6     0x00100000 /* Enable IPv6 hash function */
#define TEI_MRQC_RSS_EN_TCPIPV6  0x00200000 /* Enable TCPIPv6 hash function */
#define TEI_MRQC_RSS_EN_UDPIPV4  0x00400000 /* Enable UDPIPv4 */
#define TEI_MRQC_RSS_EN_UDPIPV6  0x00800000 /* Enable UDPIPv6 */
#define TEI_MRQC_RSS_EN_UDPIPV6X 0x01000000 /* RSVD (UDPIPv6Ext) */

/*
 * The Redirection Table (TEI_RETA), a.k.a. the indirection table,
 * is a 128-byte table with each entry being 8 bits wide.
 * The low-order 4 bits of the entry determines one of 16 RSS queues.
 * The RETA is indexed by the low-order 7 bits of the RSS hash.
 */
#define TEI_RETA_QIX		0x0f
#define TEI_RETA_NUM_ENTRIES	128

/* VLAN control register */

#define TEI_VLNCTL_VTE		0x0000FFFF /* VLAN ethertype (0x8100) */
#define TEI_VLNCTL_CFI		0x10000000 /* CFI value */
#define TEI_VLNCTL_CFIEN	0x20000000 /* CFI enable */
#define TEI_VLNCTL_VFE		0x40000000 /* VLAN filter enable */
#define TEI_VLNCTL_VME		0x80000000 /* VLAN strip enable (82598 only) */

/* Multicast control register */

#define TEI_MCSTCTL_MO		0x00000003 /* Multicast offset */
#define TEI_MCSTCTL_MFE		0x00000004 /* Mcast filter enable */

#define TEI_MO_47_36		0x00000000
#define TEI_MO_46_35		0x00000001
#define TEI_MO_45_34		0x00000002
#define TEI_MO_43_32		0x00000003

/* RX address excact match filter registers */

#define TEI_PARH_VIND		0x003C0000 /* VMDq output index */
#define TEI_PARH_AV		0x80000000 /* Address valid */

/* TX descriptor control */

#define TEI_TXDCTL_PTHRESH	0x0000007F /* Pre-fetch threshold */
#define TEI_TXDCTL_HTHRESH	0x00007F00 /* Host threshold */
#define TEI_TXDCTL_WTHRESH	0x007F0000 /* Write-back threshold */
#define TEI_TXDCTL_ENABLE	0x02000000 /* Queue Enable */

/* TX DMA control */

#define TEI_DTXCTL_ENDBUBD	0x00000004 /* Enable buffer division */

/* Ghost ECC register */

#define TEI_GHECCR_CDAM0EE	0x00000040 /* Completion data. mem LAN 0 */
#define TEI_GHECCR_CDEM0EE	0x00000200 /* Completion desc. mem LAN 0 */
#define TEI_GHECCR_CDAM1EE	0x00040000 /* Completion data. mem LAN 1 */
#define TEI_GHECCR_CDEM1EE	0x00200000 /* Completion desc. mem LAN 1 */

/* EEPROM map (partial) */

#define TEI_EE_CTL1		0x00
#define TEI_EE_CTL2		0x01
#define TEI_EE_PCIEAP		0x03
#define TEI_EE_CORECFG_PTR0	0x04
#define TEI_EE_CORECFG_PTR1	0x05
#define TEI_EE_PCIEGEN_PTR	0x06
#define TEI_EE_PCIECFG_PTR0	0x07
#define TEI_EE_PCIECFG_PTR1	0x08
#define TEI_EE_CORE0SECT_PTR	0x09
#define TEI_EE_CORE1SECT_PTR	0x0A
#define TEI_EE_FW_PTR		0x0F

typedef struct tei_core_section
    {
    UINT16	tei_sectlen;
    UINT8	tei_addr[6];
    UINT8	tei_ledcfg[4];
    UINT16	tei_sdpctrl;
    UINT16	tei_filtctl;
    } TEI_CORE_SECTION;

/* Advanced DMA descriptor structures */

/* Advanced RX descriptor format */

typedef union tei_adv_rdesc
    {
    struct
        {
        volatile UINT32		tei_addrlo; /* NSE */
        volatile UINT32		tei_addrhi;
        volatile UINT32		tei_hdrlo; /* DD */
        volatile UINT32		tei_hdrhi;
        } read;
    struct
        {
        volatile UINT16		tei_ptype_rsstype;
        volatile UINT16		tei_sph_hdrlen;
        volatile UINT32		tei_rss;
	volatile UINT16		tei_sts;
	volatile UINT16		tei_err;
	volatile UINT16		tei_pkt_buflen;
	volatile UINT16		tei_vlan;
        } write;
    struct
        {
        volatile UINT32	tei_pktinfo;
	volatile UINT32 tei_rss;
        volatile UINT32	tei_errsts;
        volatile UINT16	tei_len;
        volatile UINT16 tei_vlan;
        } write_rss;
    } TEI_ADV_RDESC;

#define TEI_ADV_RDESC_READ_NSE	0x00000001
#define TEI_ADV_RDESC_READ_DD	0x00000001

#define TEI_ADV_RDESC_SPHHDRLEN_SPH	0x8000

#define TEI_ADV_RDESC_RINFO_RSSTYPE	0x0000000F
#define TEI_ADV_RDESC_RINFO_PKTTYPE	0x0001FFF0
#define TEI_ADV_RDESC_RINFO_RSCCNT	0x001E0000
#define TEI_ADV_RDESC_RINFO_HDRLEN	0x7FE00000
#define TEI_ADV_RDESC_RINFO_SPH		0x80000000	/* Split header */

#define TEI_ADV_RDESC_RTYPE_NONE	0x00000000	/* No hash */
#define TEI_ADV_RDESC_RTYPE_TCPIPV4	0x00000001
#define TEI_ADV_RDESC_RTYPE_IPV4	0x00000002
#define TEI_ADV_RDESC_RTYPE_TCPIPV6	0x00000003
#define TEI_ADV_RDESC_RTYPE_IPV6_EX	0x00000004  /* RSVD */
#define TEI_ADV_RDESC_RTYPE_IPV6	0x00000005
#define TEI_ADV_RDESC_RTYPE_TCPIPV6_EX	0x00000006  /* RSVD */
#define TEI_ADV_RDESC_RTYPE_UDPIPV4	0x00000007
#define TEI_ADV_RDESC_RTYPE_UDPIPV6	0x00000008
#define TEI_ADV_RDESC_RTYPE_UDPIPV6_EX	0x00000009  /* 9-E RSVD */
#define TEI_ADV_RDESC_RTYPE_FDFS	0x0000000f

#define TEI_ADV_RDESC_PTYPE_RSVD       0x10000
#define TEI_ADV_RDESC_PTYPE_L2PKT       0x8000 /* When set, interpretation of
						  other PTYPE bits changes */
#define TEI_ADV_RDESC_PTYPE_LINKSEC     0x4000
#define TEI_ADV_RDESC_PTYPE_IPSEC_AH    0x2000
#define TEI_ADV_RDESC_PTYPE_IPSEC_ESP   0x1000
#define TEI_ADV_RDESC_PTYPE_NFS		0x0800
#define TEI_ADV_RDESC_PTYPE_SCTP	0x0400
#define TEI_ADV_RDESC_PTYPE_UDP		0x0200
#define TEI_ADV_RDESC_PTYPE_TCP		0x0100
#define TEI_ADV_RDESC_PTYPE_IPV6E	0x0080
#define TEI_ADV_RDESC_PTYPE_IPV6	0x0040
#define TEI_ADV_RDESC_PTYPE_IPV4E	0x0020
#define TEI_ADV_RDESC_PTYPE_IPV4	0x0010

#define TEI_ADV_RDESC_ERR_IPE		0x8000 /* IPv4 checksum error */
#define TEI_ADV_RDESC_ERR_TCPE		0x4000 /* TCP/UDP checksum error */
#define TEI_ADV_RDESC_ERR_USE		0x2000 /* Undersize (runt) frame */
#define TEI_ADV_RDESC_ERR_OSE		0x1000 /* Oversize (giant) frame */
#define TEI_ADV_RDESC_ERR_PE		0x0800 /* Packet symbol error */
#define TEI_ADV_RDESC_ERR_LE		0x0200 /* Length error */
#define TEI_ADV_RDESC_ERR_CE		0x0100 /* CRC error */
#define TEI_ADV_RDESC_ERR_HBO		0x0080 /* Header buffer overflow */
#define TEI_ADV_RDESC_ERSTS_LB          0x0004 /* Loopback - VM to VM */
#define TEI_ADV_RDESC_ERSTS_SECP        0x0002 /* Security encap. processed */
#define TEI_ADV_RDESC_ERSTS_TS          0x0001 /* Time sync packet */

#define TEI_ADV_RDESC_ERRSUM	\
    (TEI_ADV_RDESC_ERR_CE|TEI_ADV_RDESC_ERR_USE|	\
     TEI_ADV_RDESC_ERR_LE|TEI_ADV_RDESC_ERR_PE|TEI_ADV_RDESC_ERR_OSE)

#define TEI_ADV_RDESC_STS_DYNINT	0x0800 /* Did low-latency interrupt */
#define TEI_ADV_RDESC_STS_UDPV		0x0400 /* UDP csum valid */
#define TEI_ADV_RDESC_STS_VEXT          0x0200 /* Outer VLAN found */
#define TEI_ADV_RDESC_STS_CRCV		0x0100 /* Speculative CRC OK */
#define TEI_ADV_RDESC_STS_PIF		0x0080 /* Passed inexact filter */
#define TEI_ADV_RDESC_STS_IPCS		0x0040 /* IP csum calculated */
#define TEI_ADV_RDESC_STS_L4CS		0x0020 /* TCP csum calculated */
#define TEI_ADV_RDESC_STS_UDPCS		0x0010 /* UDP checksum calculated */
#define TEI_ADV_RDESC_STS_VLAN		0x0008 /* 802.1q packet, stripped */
#define TEI_ADV_RDESC_STS_FLM		0x0004 /* Flow director match */
#define TEI_ADV_RDESC_STS_EOP		0x0002 /* End of packet */
#define TEI_ADV_RDESC_STS_DD		0x0001 /* Descriptor done */

/* Extended Error and Extended Status considered as a single 32-bit field */

#define TEI_ADV_RDESC_ERRSTS_IPE     0x80000000 /* IPv4 checksum error */
#define TEI_ADV_RDESC_ERRSTS_TCPE    0x40000000 /* TCP/UDP checksum error */
#define TEI_ADV_RDESC_ERRSTS_USE     0x20000000 /* Undersize (runt) frame */
#define TEI_ADV_RDESC_ERRSTS_OSE     0x10000000 /* Oversize (giant) frame */
#define TEI_ADV_RDESC_ERRSTS_PE	     0x08000000 /* Packet symbol error */
#define TEI_ADV_RDESC_ERRSTS_LE	     0x02000000 /* Length error */
#define TEI_ADV_RDESC_ERRSTS_CE	     0x01000000 /* CRC error */
#define TEI_ADV_RDESC_ERRSTS_HBO     0x00800000 /* Header buffer overflow */

#define TEI_ADV_RDESC_ERR_NSUM	\
    (TEI_ADV_RDESC_ERRSTS_CE|TEI_ADV_RDESC_ERRSTS_USE|	\
     TEI_ADV_RDESC_ERRSTS_LE|TEI_ADV_RDESC_ERRSTS_PE|TEI_ADV_RDESC_ERRSTS_OSE)

#define TEI_ADV_RDESC_ERRSTS_LB      0x00040000 /* Loopback - VM to VM */
#define TEI_ADV_RDESC_ERRSTS_SECP    0x00020000 /* Security encap. processed */
#define TEI_ADV_RDESC_ERRSTS_TS      0x00010000 /* Time sync packet */
#define TEI_ADV_RDESC_ERRSTS_DYNINT  0x00000800 /* Did low-latency interrupt */
#define TEI_ADV_RDESC_ERRSTS_UDPV    0x00000400 /* UDP csum valid */
#define TEI_ADV_RDESC_ERRSTS_VEXT    0x00000200 /* Outer VLAN found */
#define TEI_ADV_RDESC_ERRSTS_CRCV    0x00000100 /* Speculative CRC OK */
#define TEI_ADV_RDESC_ERRSTS_PIF     0x00000080 /* Passed inexact filter */
#define TEI_ADV_RDESC_ERRSTS_IPCS    0x00000040 /* IP csum calculated */
#define TEI_ADV_RDESC_ERRSTS_L4CS    0x00000020 /* TCP csum calculated */
#define TEI_ADV_RDESC_ERRSTS_UDPCS   0x00000010 /* UDP checksum calculated */
#define TEI_ADV_RDESC_ERRSTS_VLAN    0x00000008 /* 802.1q packet, stripped */
#define TEI_ADV_RDESC_ERRSTS_FLM     0x00000004 /* Flow director match */
#define TEI_ADV_RDESC_ERRSTS_EOP     0x00000002 /* End of packet */
#define TEI_ADV_RDESC_ERRSTS_DD	     0x00000001 /* Descriptor done */

/* Advanced TCP/IP context descriptor format (DEXT = 1, DTYP = 2) */

typedef struct tei_adv_cdesc
    {
    volatile UINT16     tei_macip;
    volatile UINT16     tei_vlan;
    volatile UINT32     tei_rsvd;
    volatile UINT32     tei_cmd;
    volatile UINT8      tei_idx;
    volatile UINT8      tei_l4len;
    volatile UINT16     tei_mss;
    } TEI_ADV_CDESC;

#define TEI_ADV_CDESC_MACIP_IP  0x01FF          /* IP header length */
#define TEI_ADV_CDESC_MACIP_MAC 0xFE00          /* Frame header length */

#define TEI_ADV_IPLEN(x)        ((x) & TEI_ADV_CDESC_MACIP_IP)
#define TEI_ADV_MACLEN(x)       (((x) << 9) & TEI_ADV_CDESC_MACIP_MAC)

#define TEI_ADV_CDESC_CMD_DEXT  0x20000000      /* 1 = advanced */
#define TEI_ADV_CDESC_CMD_DTYP  0x00F00000      /* Descriptor type */
#define TEI_ADV_CDESC_CMD_MRKRQ 0x00002000      /* Markers required */
#define TEI_ADV_CDESC_CMD_L4T   0x00001800      /* L4 packet type */
#define TEI_ADV_CDESC_CMD_IPV4  0x00000400      /* 1 = IPV4, 0 = IPV6 */
#define TEI_ADV_CDESC_CMD_SNAP  0x00000200      /* SNAP frame */
#define TEI_ADV_CDESC_CMD_MKRL  0x000000FF      /* Marker offset */

#define TEI_ADV_CDESC_L4T_UDP   0x00000000
#define TEI_ADV_CDESC_L4T_TCP   0x00000800

/* Advanced TCP/IP data descriptor format (DEXT = 1, DTYP = 3) */

typedef struct tei_adv_tdesc
    {
    volatile UINT32     tei_addrlo;
    volatile UINT32     tei_addrhi;
    volatile UINT32     tei_cmd;
    volatile UINT32     tei_sts;
    } TEI_ADV_TDESC;

#define TEI_ADV_TDESC_CMD_TSE   0x80000000      /* TSO enable */
#define TEI_ADV_TDESC_CMD_VLE   0x40000000      /* VLAN insertion enable */
#define TEI_ADV_TDESC_CMD_DEXT  0x20000000      /* 1 = advanced */
#define TEI_ADV_TDESC_CMD_RS    0x08000000      /* report status */
#define TEI_ADV_TDESC_CMD_IFCS  0x02000000      /* Insert FCS */
#define TEI_ADV_TDESC_CMD_EOP   0x01000000      /* End of packet */
#define TEI_ADV_TDESC_CMD_DTYP  0x00F00000      /* Descriptor type */
#define TEI_ADV_TDESC_CMD_LEN   0x0000FFFF      /* Data length */

#define TEI_AVD_TDESC_STS_STA   0x0000000F      /* Status */
#define TEI_ADV_TDESC_STS_IDX   0x000000F0      /* Context index */
#define TEI_ADV_TDESC_STS_POPTS 0x00003F00      /* Packet options */
#define TEI_ADV_TDESC_STS_PAYLN 0xFFFFC000      /* Payload length */

#define TEI_ADV_STS_DD          0x00000001
#define TEI_ADV_IDX(x)          (((x) << 4) & TEI_ADV_TDESC_STS_IDX)
#define TEI_ADV_PAYLEN(x)       (((x) << 14) & TEI_ADV_TDESC_STS_PAYLN)

#define TEI_ADV_POPT_IXSM       0x00000100      /* Insert IP header checksum */
#define TEI_ADV_POPT_TXSM       0x00000200      /* Insert TCP/UDP checksum */

#define TEI_ADV_TDESC_DTYP_CTX  0x00200000      /* TCP/IP context descriptor */
#define TEI_ADV_TDESC_DTYP_DSC  0x00300000      /* TCP/IP data descriptor */

#define TEI_MTU 1500
#define TEI_JUMBO_MTU 9000
#define TEI_CLSIZE	1536
#define TEI_NAME	"tei"
#define TEI_TIMEOUT 10000

#define TEI_RXINTRS TEI_EICR_RTXQ0
#define TEI_TXINTRS TEI_EICR_RTXQ1
#define TEI_LINKINTRS TEI_EICR_LSC

#define TEI_INTRS (TEI_LINKINTRS|TEI_RXINTRS|TEI_TXINTRS|TEI_EICR_OTHER)

/*
 * Maximum ethernet link header size: 14 bytes + VLAN. Note that LLC/SNAP isn't
 * counted as part of the link header for the purpose of relating the MTU
 * to the full packet size.
 */
#define MAX_ETHER_LINKHDR	18

/*
 * TEI_MTU_ADJUST is how much we need to add to the nominal MTU to
 * get the maximum packet data that the hardware will write to RX buffers.
 *
 * Since we enable hardware stripping of the ethernet CRC, we don't need
 * to add 4 bytes.
 */
#define TEI_MTU_ADJUST (MAX_ETHER_LINKHDR)

/*
 * The ipnet stack can handle 2-byte aligned headers on all
 * architectures. For high throughput, it's better to optimize the
 * I/O by making the packet start on a cache line boundary.
 * Leave some space for additional headers.
 * (TODO: 64 bytes won't achieve that everywhere).
 */
#define TEI_RXPKT_OFFSET 64

#define TEI_RX_DESC_CNT 512
#define TEI_TX_DESC_CNT 512

#define TEI_TX_CLEAN_THRESH 32
#define TEI_MAXFRAG 16
#define TEI_MAX_RX 64

#ifdef TEI_FORWARDER
#define TEI_FW_RX_DESC_CNT 512
#define TEI_FW_TX_DESC_CNT 512

#define TEI_FW_TX_CLEAN_THRESH TEI_FW_TX_CLEANUP_BATCH * 2

#define TEI_FW_PKTS_ALLOC_PER_RXQ  (2 * TEI_FW_RX_DESC_CNT)
#define TEI_FW_PKTS_ALLOC_PER_TXQ  (TEI_FW_TX_CLEAN_THRESH)

#if TEI_FW_PKTS_ALLOC_PER_RXQ > IPCOM_FW_MAX_PKTS_ALLOC_PER_RXQ
#error "IPCOM_FW_MAX_PKTS_ALLOC_PER_RXQ is too small!"
#endif

#if TEI_FW_PKTS_ALLOC_PER_TXQ > IPCOM_FW_MAX_PKTS_ALLOC_PER_TXQ
#error "IPCOM_FW_MAX_PKTS_ALLOC_PER_TXQ is too small!"
#endif
#endif /* TEI_FORWARDER */

/* Flow control receive threshold low watermark */

#define TEI_RX_FLOW_THRESH_LOW	0x1000

/* Flow control receive threshold high watermark */

#define TEI_RX_FLOW_THRESH_HIGH	0x2000

/* Default flow control timer value */

#define TEI_FC_TIMER	0x680

/* Default interrupts/second rate */

#define TEI_INTR_RATE	50000

#define TEI_INC_DESC(x, y)     (x) = ((x + 1) & (y - 1))

#define TEI_ADDR_LO(y)  ((UINT32)((UINT64)(y) & 0xFFFFFFFF))
#define TEI_ADDR_HI(y)  ((UINT32)(((UINT64)(y) >> 32) & 0xFFFFFFFF))

#define TEI_ADJ(m)      (m)->m_data += 2

struct Ipcom_pkt_struct;

/*
 * Private adapter context structure.
 */

#define ETHER_ADDR_LEN          6
typedef struct tei_drv_ctrl
    {
    END_OBJ		teiEndObj;
    VXB_DEVICE_ID	teiDev;
    void *		teiBar;
    void *		teiHandle;
    void *		teiMuxDevCookie;

    JOB_QUEUE_ID	teiJobQueue;
    QJOB		teiIntJob;
    atomic_t		teiIntPending;
    UINT32		teiIntrs;
    UINT32		teiMoreRx;

    BOOL		teiPolling;
    UINT32		teiIntMask;

    UINT8		teiAddr[ETHER_ADDR_LEN];

    END_CAPABILITIES	teiCaps;

    END_IFDRVCONF	teiEndStatsConf;
    END_IFCOUNTERS	teiEndStatsCounters;

    SEM_ID		teiDevSem;

    /* Begin MII/ifmedia required fields. */
    END_MEDIALIST	*teiMediaList;
    END_ERR		teiLastError;
    UINT32		teiCurMedia;
    UINT32		teiCurStatus;
    /* End MII/ifmedia required fields */

#ifdef TEI_VXB_DMA_BUF
    /* DMA tags and maps. */
    VXB_DMA_TAG_ID	teiParentTag;

    VXB_DMA_TAG_ID	teiRxDescTag;
    VXB_DMA_TAG_ID	teiTxDescTag;

    VXB_DMA_MAP_ID	teiRxDescMap;
    VXB_DMA_MAP_ID	teiTxDescMap;

    VXB_DMA_TAG_ID	teiPktTag;

    VXB_DMA_MAP_ID	teiRxPktMap[TEI_RX_DESC_CNT];
    VXB_DMA_MAP_ID	teiTxPktMap[TEI_TX_DESC_CNT];
#else
    /* possibly unaligned desc buffer to free */
    char *		teiDescBuf;
#endif

    TEI_ADV_RDESC *	teiRxDescMem;
    TEI_ADV_TDESC *	teiTxDescMem;

    struct Ipcom_pkt_struct * teiRxPkt[TEI_RX_DESC_CNT];
    struct Ipcom_pkt_struct * teiTxPkt[TEI_TX_DESC_CNT];

    UINT32		teiTxProd;
    UINT32		teiTxCons;
    UINT32		teiTxFree;
    BOOL		teiTxStall;

    UINT32		teiRxIdx;

    UINT32		teiLastCtx;
    int			teiLastOffsets;
    UINT16		teiLastVlan;
#if !defined (TEI_MUX_TX_ALWAYS) || !defined(TEI_MUX_RX_ALWAYS)
    UINT16		muxqs;
#endif

    int			teiMaxMtu;

    UINT32		teiDevType;

#ifdef TEI_FORWARDER
    Ipcom_fw_port_t fw_port;
#endif
    } TEI_DRV_CTRL;

#if (CPU_FAMILY == I80X86)

#define CSR_READ_4(pDev, addr)                                  \
        *(volatile UINT32 *)((UINT32)pDev->pRegBase[0] + addr)

#define CSR_WRITE_4(pDev, addr, data)                           \
        do {                                                    \
            volatile UINT32 *pReg =                             \
                (UINT32 *)((UINT32)pDev->pRegBase[0] + addr);   \
            *(pReg) = (UINT32)(data);                           \
        } while ((0))

#else /* CPU_FAMILY == I80X86 */

#define TEI_BAR(p)   ((TEI_DRV_CTRL *)(p)->pDrvCtrl)->teiBar
#define TEI_HANDLE(p)   ((TEI_DRV_CTRL *)(p)->pDrvCtrl)->teiHandle

#define CSR_READ_4(pDev, addr)                                  \
    vxbRead32 (TEI_HANDLE(pDev), (UINT32 *)((char *)TEI_BAR(pDev) + addr))

#define CSR_WRITE_4(pDev, addr, data)                           \
    vxbWrite32 (TEI_HANDLE(pDev),                             \
        (UINT32 *)((char *)TEI_BAR(pDev) + addr), data)

#endif /* CPU_FAMILY == I80X86 */

#define CSR_SETBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))



#ifdef __cplusplus
}
#endif

#endif /* __INCvxbTei8259xEnd2h */
