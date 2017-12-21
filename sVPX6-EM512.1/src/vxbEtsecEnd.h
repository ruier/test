/* vxbEtsecEnd.h - header file for Freescale ETSEC VxBus END driver */

/*
 * Copyright (C) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,20may09,wap  Avoid use of spinlocks WIND00167144
01h,27jan09,rec  change ETSEC_PHYSADDR macro
01g,06nov08,dlk  Combined RX/TX job.
01f,30jue08,x_s  Change the value of ETSEC_TIMEOUT to 100000.
01e,17jan07,dlk  Rename spinLockFastLib --> spinlockIsrNdLib.
01d,21dec07,dlk  Increase ETSEC_MAX_RX from 16 to 32.
01c,11dec07,dlk  Use fast spin locks.
01b,29aug07,wap  Make setting of the TBIPA register configurable (WIND00103172)
01a,21jun07,wap  written
*/

#ifndef __INCvxbEtsecEndh
#define __INCvxbEtsecEndh

#ifdef __cplusplus
extern "C" {
#endif

IMPORT void etsecRegister (void);


#define ETSEC_ID                             0x000
#define ETSEC_ID2                            0x004			
#define ETSEC_IEVENT                         0x010
#define ETSEC_IMASK                          0x014
#define ETSEC_EDIS                           0x018
#define ETSEC_ECNTRL                         0x020
#define ETSEC_MINFLR                         0x024
#define ETSEC_PTV                            0x028
#define ETSEC_DMACTRL                        0x02C
#define ETSEC_TBIPA                          0x030
#define ETSEC_FIFO_PAUSE_CTRL                0x04C
#define ETSEC_FIFO_RX_ALARM                  0x058
#define ETSEC_FIFO_RX_ALARM_SHUT             0x05C
#define ETSEC_FIFO_TX_THR                    0x08C
#define ETSEC_FIFO_STRV                      0x098
#define ETSEC_FIFO_STRV_SHUT                 0x09C
#define ETSEC_TCTRL                          0x100
#define ETSEC_TSTAT                          0x104
#define ETSEC_DFVLAN                         0x108
#define ETSEC_TBDLEN                         0x10C
#define ETSEC_TXIC                           0x110
#define ETSEC_TQUEUE                         0x114
#define ETSEC_TR03WT                         0x140
#define ETSEC_TR47WT                         0x144
#define ETSEC_TBPTRH                         0x180
#define ETSEC_TBPTR                          ETSEC_TBPTR0
#define ETSEC_TBPTR0                         0x184
#define ETSEC_TBPTR1                         0x18C
#define ETSEC_TBPTR2                         0x194
#define ETSEC_TBPTR3                         0x19C
#define ETSEC_TBPTR4                         0x1A4
#define ETSEC_TBPTR5                         0x1AC
#define ETSEC_TBPTR6                         0x1B4
#define ETSEC_TBPTR7                         0x1BC
#define ETSEC_TBASEH                         0x200
#define ETSEC_TBASE                          ETSEC_TBASE0
#define ETSEC_TBASE0                         0x204
#define ETSEC_TBASE1                         0x20C
#define ETSEC_TBASE2                         0x214
#define ETSEC_TBASE3                         0x21C
#define ETSEC_TBASE4                         0x224
#define ETSEC_TBASE5                         0x22C
#define ETSEC_TBASE6                         0x234
#define ETSEC_TBASE7                         0x23C
#define ETSEC_RCTRL                          0x300
#define ETSEC_RSTAT                          0x304
#define ETSEC_RBDLEN                         0x30C
#define ETSEC_RXIC                           0x310
#define ETSEC_RQUEUE                         0x314
#define ETSEC_CRBPTR                         0x324
#define ETSEC_RBIFX                          0x330
#define ETSEC_RQFAR                          0x334
#define ETSEC_RQFCR                          0x338
#define ETSEC_RQFPR                          0x33C
#define ETSEC_MRBLR                          0x340
#define ETSEC_RBPTRH                         0x380
#define ETSEC_RBPTR                          ETSEC_RBPTR0
#define ETSEC_RBPTR0                         0x384
#define ETSEC_RBPTR1                         0x38C
#define ETSEC_RBPTR2                         0x394
#define ETSEC_RBPTR3                         0x39C
#define ETSEC_RBPTR4                         0x3A4
#define ETSEC_RBPTR5                         0x3AC
#define ETSEC_RBPTR6                         0x3B4
#define ETSEC_RBPTR7                         0x3BC
#define ETSEC_RBASEH                         0x400
#define ETSEC_RBASE                          ETSEC_RBASE0
#define ETSEC_RBASE0                         0x404
#define ETSEC_RBASE1                         0x40C
#define ETSEC_RBASE2                         0x414
#define ETSEC_RBASE3                         0x41C
#define ETSEC_RBASE4                         0x424
#define ETSEC_RBASE5                         0x42C
#define ETSEC_RBASE6                         0x434
#define ETSEC_RBASE7                         0x43C
#define ETSEC_MACCFG1                        0x500
#define ETSEC_MACCFG2                        0x504
#define ETSEC_IPGIFG                         0x508
#define ETSEC_HALFDUP                        0x50C
#define ETSEC_MAXFRM                         0x510
#define ETSEC_MIIMCFG                        0x520
#define ETSEC_MIIMCOM                        0x524
#define ETSEC_MIIMADD                        0x528
#define ETSEC_MIIMCON                        0x52C
#define ETSEC_MIIMSTAT                       0x530
#define ETSEC_MIIMIND                        0x534
#define ETSEC_IFSTAT                         0x53C
#define ETSEC_PAR0                           ETSEC_CAM0_0
#define ETSEC_PAR1                           ETSEC_CAM0_1
#define ETSEC_CAM0_0                         0x540
#define ETSEC_CAM0_1                         0x544
#define ETSEC_CAM1_0                         0x548
#define ETSEC_CAM1_1                         0x54C
#define ETSEC_CAM2_0                         0x550
#define ETSEC_CAM2_1                         0x554
#define ETSEC_CAM3_0                         0x558
#define ETSEC_CAM3_1                         0x55C
#define ETSEC_CAM4_0                         0x560
#define ETSEC_CAM4_1                         0x564
#define ETSEC_CAM5_0                         0x568
#define ETSEC_CAM5_1                         0x56C
#define ETSEC_CAM6_0                         0x570
#define ETSEC_CAM6_1                         0x574
#define ETSEC_CAM7_0                         0x578
#define ETSEC_CAM7_1                         0x57C
#define ETSEC_CAM8_0                         0x580
#define ETSEC_CAM8_1                         0x584
#define ETSEC_CAM9_0                         0x588
#define ETSEC_CAM9_1                         0x58C
#define ETSEC_CAM10_0                        0x590
#define ETSEC_CAM10_1                        0x594
#define ETSEC_CAM11_0                        0x598
#define ETSEC_CAM11_1                        0x59C
#define ETSEC_CAM12_0                        0x5A0
#define ETSEC_CAM12_1                        0x5A4
#define ETSEC_CAM13_0                        0x5A8
#define ETSEC_CAM13_1                        0x5AC
#define ETSEC_CAM14_0                        0x5B0
#define ETSEC_CAM14_1                        0x5B4
#define ETSEC_CAM15_0                        0x5B8
#define ETSEC_CAM15_1                        0x5BC
#define ETSEC_TR64                           0x680
#define ETSEC_TR127                          0x684
#define ETSEC_TR255                          0x688
#define ETSEC_TR511                          0x68C
#define ETSEC_TR1K                           0x690
#define ETSEC_TRMAX                          0x694
#define ETSEC_TRMGV                          0x698
#define ETSEC_RBYT                           0x69C
#define ETSEC_RPKT                           0x6A0
#define ETSEC_RFCS                           0x6A4
#define ETSEC_RMCA                           0x6A8
#define ETSEC_RBCA                           0x6AC
#define ETSEC_RXCF                           0x6B0
#define ETSEC_RXPF                           0x6B4
#define ETSEC_RXUO                           0x6B8
#define ETSEC_RALN                           0x6BC
#define ETSEC_RFLR                           0x6C0
#define ETSEC_RCDE                           0x6C4
#define ETSEC_RCSE                           0x6C8
#define ETSEC_RUND                           0x6CC
#define ETSEC_ROVR                           0x6D0
#define ETSEC_RFRG                           0x6D4
#define ETSEC_RJBR                           0x6D8
#define ETSEC_RDRP                           0x6DC
#define ETSEC_TBYT                           0x6E0
#define ETSEC_TPKT                           0x6E4
#define ETSEC_TMCA                           0x6E8
#define ETSEC_TBCA                           0x6EC
#define ETSEC_TXPF                           0x6F0
#define ETSEC_TDFR                           0x6F4
#define ETSEC_TEDF                           0x6F8
#define ETSEC_TSCL                           0x6FC
#define ETSEC_TMCL                           0x700
#define ETSEC_TLCL                           0x704
#define ETSEC_TXCL                           0x708
#define ETSEC_TNCL                           0x70C
#define ETSEC_TDRP                           0x714
#define ETSEC_TJBR                           0x718
#define ETSEC_TFCS                           0x71C
#define ETSEC_TXCF                           0x720
#define ETSEC_TOVR                           0x724
#define ETSEC_TUND                           0x728
#define ETSEC_TFRG                           0x72C
#define ETSEC_CAR1                           0x730
#define ETSEC_CAR2                           0x734
#define ETSEC_CAM1                           0x738
#define ETSEC_CAM2                           0x73C
#define ETSEC_RREJ                           0x740
#define ETSEC_IADDR0                         0x800
#define ETSEC_IADDR1                         0x804
#define ETSEC_IADDR2                         0x808
#define ETSEC_IADDR3                         0x80C
#define ETSEC_IADDR4                         0x810
#define ETSEC_IADDR5                         0x814
#define ETSEC_IADDR6                         0x818
#define ETSEC_IADDR7                         0x81C
#define ETSEC_GADDR0                         0x880
#define ETSEC_GADDR1                         0x884
#define ETSEC_GADDR2                         0x888
#define ETSEC_GADDR3                         0x88C
#define ETSEC_GADDR4                         0x890
#define ETSEC_GADDR5                         0x894
#define ETSEC_GADDR6                         0x898
#define ETSEC_GADDR7                         0x89C
#define ETSEC_FIFOCFG                        0xA00
#define ETSEC_ATTR                           0xBF8
#define ETSEC_ATTRELI                        0xBFC

/* IEVENT and IMASK Register definitions */

#define ETSEC_IEVENT_BABR		0x80000000 /* giant frame RXed */
#define ETSEC_IEVENT_RXC		0x40000000 /* pause frame RXed */
#define ETSEC_IEVENT_BSY		0x20000000 /* RX overrun */
#define ETSEC_IEVENT_EBERR		0x10000000 /* eth bus error */
#define ETSEC_IEVENT_MSRO		0x04000000 /* stats oflow */
#define ETSEC_IEVENT_GTSC		0x02000000 /* graceful TX stop done */
#define ETSEC_IEVENT_BABT		0x01000000 /* giant frame TXed */
#define ETSEC_IEVENT_TXC		0x00800000 /* pause frame TXed */
#define ETSEC_IEVENT_TXE		0x00400000 /* TX error */
#define ETSEC_IEVENT_TXB		0x00200000 /* TX descriptor updated */
#define ETSEC_IEVENT_TXF		0x00100000 /* TX frame completed */
#define ETSEC_IEVENT_LC			0x00040000 /* late collision */
#define ETSEC_IEVENT_CRL		0x00020000 /* excess collision */
#define ETSEC_IEVENT_XFUN		0x00010000 /* TX underrun */
#define ETSEC_IEVENT_RXB		0x00008000 /* RX descriptor updated */
#define ETSEC_IEVENT_MMRD		0x00000400 /* MDIO read, 8349 only */
#define ETSEC_IEVENT_MMWR		0x00000200 /* MDIO write, 8349 only */
#define ETSEC_IEVENT_GRSC		0x00000100 /* graceful RX stop done */
#define ETSEC_IEVENT_RXF		0x00000080 /* RX frame completed */

/*
 * Error event disable register. Note that with the IMASK
 * register, setting a bit enables the interrupt, whereas with
 * this register, setting a bit disables the interrupt.
 */

#define ETSEC_EDIS_BSYDIS		0x20000000
#define ETSEC_EDIS_EBERRDIS		0x10000000
#define ETSEC_EDIS_TXEDIS		0x00400000
#define ETSEC_EDIS_LCDIS		0x00040000
#define ETSEC_EDIS_CRLDIS		0x00020000
#define ETSEC_EDIS_XFUNDIS		0x00010000

/* Ethernet Control Register */

#define ETSEC_ECNTRL_CLRCNT		0x00004000 /* zero status counters */
#define ETSEC_ECNTRL_AUTOZ		0x00002000 /* zero counter on read */
#define ETSEC_ECNTRL_STEN		0x00001000 /* status enable */
#define ETSEC_ECNTRL_GMIIM		0x00000040 /* GMII mode (strap) */
#define ETSEC_ECNTRL_TBIM		0x00000020 /* TBI mode (strap) */
#define ETSEC_ECNTRL_RPM		0x00000010 /* reduced pin mode 1000 */
#define ETSEC_ECNTRL_R100M		0x00000008 /* 0 = R10, 1 == R100 */
#define ETSEC_ECNTRL_RMM		0x00000004 /* reduced pin mode 10/100*/

/* Pause time value register */

#define ETSEC_PTV_PT			0x0000FFFF /* pause time value */
#define ETSEC_PTV_PTE			0xFFFF0000 /* extended pause control */

/* DMA control register */

#define ETSEC_DMACTRL_TDSEN		0x00000080 /* TX data snoop */
#define ETSEC_DMACTRL_TBDSEN		0x00000040 /* TX bd snoop */
#define ETSEC_DMACTRL_GRS		0x00000010 /* graceful RX stop */
#define ETSEC_DMACTRL_GTS		0x00000008 /* graceful TX stop */
#define ETSEC_DMACTRL_TOD		0x00000004 /* TX on demand */
#define ETSEC_DMACTRL_WWR		0x00000002 /* write with response */
#define ETSEC_DMACTRL_WOP		0x00000001 /* wait or poll */

/* Transmit control register */

#define ETSEC_TCTRL_IPCSEN		0x00004000 /* IP header csum enable */
#define ETSEC_TCTRL_TUCSEN		0x00002000 /* TCP/UDP csum enable */
#define ETSEC_TCTRL_VLINS		0x00001000 /* VLAN tag insert enable */
#define ETSEC_TCTRL_THDF		0x00000800 /* half duplex flow ctl */
#define ETSEC_TCTRL_RFC_PAUSE		0x00000010 /* received pause frame */
#define ETSEC_TCTRL_TFC_PAUSE		0x00000008 /* send pause frame */
#define ETSEC_TCTRL_TXSCHED		0x00000006 /* TX sched algorithm */

#define ETSEC_TXSCHED_SINGLEPOLL	0x00000000
#define ETSEC_TXSCHED_PRIOSCHED		0x00000002
#define ETSEC_TXSCHED_MWROUNDROBIN	0x00000004

/*
 * Transmit status register
 * The THLT bit will be set whenever the TX DMA engine encounters
 * an error condition, including when it runs out of ready descriptors
 * to process. The transmitter is resumed by clearing the bit (i.e.
 * writing 0 to this register).
 */

#define ETSEC_TSTAT_THLT		ETSEC_TSTAT_THLT0
#define ETSEC_TSTAT_THLT0		0x80000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT1		0x40000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT2		0x20000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT3		0x10000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT4		0x08000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT5		0x04000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT6		0x02000000 /* TX halted / resume */
#define ETSEC_TSTAT_THLT7		0x01000000 /* TX halted / resume */
#define ETSEC_TSTAT_TXF0		0x00008000 /* TX event */
#define ETSEC_TSTAT_TXF1		0x00004000 /* TX event */
#define ETSEC_TSTAT_TXF2		0x00002000 /* TX event */
#define ETSEC_TSTAT_TXF3		0x00001000 /* TX event */
#define ETSEC_TSTAT_TXF4		0x00000800 /* TX event */
#define ETSEC_TSTAT_TXF5		0x00000400 /* TX event */
#define ETSEC_TSTAT_TXF6		0x00000200 /* TX event */
#define ETSEC_TSTAT_TXF7		0x00000100 /* TX event */

/* Transmit interrupt coalescing register */

#define ETSEC_TXIC_ICEN			0x80000000 /* TX coal enable */
#define ETSEC_TXIC_ICFCT		0x1FE00000 /* frame count thresh */
#define ETSEC_TXIC_ICTT			0x0000FFFF /* timer thresh */

/* Transmit queue control register */

#define ETSEC_TQUEUE_EN0		0x00008000 /* TX queue enabled */
#define ETSEC_TQUEUE_EN1		0x00004000 /* TX queue enabled */
#define ETSEC_TQUEUE_EN2		0x00002000 /* TX queue enabled */
#define ETSEC_TQUEUE_EN3		0x00001000 /* TX queue enabled */
#define ETSEC_TQUEUE_EN4		0x00000800 /* TX queue enabled */
#define ETSEC_TQUEUE_EN5		0x00000400 /* TX queue enabled */
#define ETSEC_TQUEUE_EN6		0x00000200 /* TX queue enabled */
#define ETSEC_TQUEUE_EN7		0x00000100 /* TX queue enabled */

/* Receive control register */

#define ETSEC_RCTRL_PAL			0x001F0000 /* RX frame alignment pad */
#define ETSEC_RCTRL_VLEX		0x00002000 /* VLAN extract enable */
#define ETSEC_RCTRL_FILREN		0x00001000 /* filer enable */
#define ETSEC_RCTRL_FSQEN		0x00000800 /* single queue filer mode */
#define ETSEC_RCTRL_GHTX		0x00000400 /* mcast hash table extend */
#define ETSEC_RCTRL_IPCSEN		0x00000200 /* IP header csum enable */
#define ETSEC_RCTRL_TUCSEN		0x00000100 /* TCP/UDP csum enable */
#define ETSEC_RCTRL_PRSDEP		0x000000C0 /* Parser control */
#define ETSEC_RCTRL_BC_REJ		0x00000010 /* reject broadcasts */
#define ETSEC_RCTRL_PROM		0x00000008 /* promisc mode */
#define ETSEC_RCTRL_RSF			0x00000004 /* receive short frames */
#define ETSEC_RCTRL_EMEN		0x00000002 /* enable CAM1-CAM15 */

#define ETSEC_PARSER_DISABLE		0x00000000
#define ETSEC_PARSER_L2			0x00000040
#define ETSEC_PARSER_L2_L3		0x00000080
#define ETSEC_PARSER_L2_L3_L4		0x000000C0

#define ETSEC_RXALIGN(x)		(((x) << 16) & ETSEC_RCTRL_PAL)

/*
 * Receive status register
 * Like the TSTAT register, the QHLT bit indicates the RX DMA engine
 * has stalled due to running out of descriptors, or encountering an
 * error. The bit must be cleared to resume the RX engine.
 */

#define ETSEC_RSTAT_QHLT		ETSEC_RSTAT_QHLT0

#define ETSEC_RSTAT_QHLT0		0x00800000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT1		0x00400000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT2		0x00200000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT3		0x00100000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT4		0x00080000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT5		0x00040000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT6		0x00020000 /* RX halted / resume */
#define ETSEC_RSTAT_QHLT7		0x00010000 /* RX halted / resume */
#define ETSEC_RSTAT_RXF0		0x00000080 /* RX event */
#define ETSEC_RSTAT_RXF1		0x00000040 /* RX event */
#define ETSEC_RSTAT_RXF2		0x00000020 /* RX event */
#define ETSEC_RSTAT_RXF3		0x00000010 /* RX event */
#define ETSEC_RSTAT_RXF4		0x00000008 /* RX event */
#define ETSEC_RSTAT_RXF5		0x00000004 /* RX event */
#define ETSEC_RSTAT_RXF6		0x00000002 /* RX event */
#define ETSEC_RSTAT_RXF7		0x00000001 /* RX event */

/* Receive interrupt coalescing register */

#define ETSEC_RXIC_ICEN			0x80000000 /* RX coal enable */
#define ETSEC_RXIC_ICFCT		0x1FE00000 /* frame count thresh */
#define ETSEC_RXIC_ICTT			0x0000FFFF /* timer thresh */

#define ETSEC_COAL_FRAMES(x)		((x) & 0xFF) << 21
#define ETSEC_COAL_TIMER(x)		((x) & 0xFFFF)

/* RX Queue control register */

#define ETSEC_RQUEUE_EXT0		0x00800000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT1		0x00400000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT2		0x00200000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT3		0x00100000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT4		0x00080000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT5		0x00040000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT6		0x00020000 /* L2 extraction */
#define ETSEC_RQUEUE_EXT7		0x00010000 /* L2 extraction */
#define ETSEC_RQUEUE_EN0		0x00000080 /* queue enable */
#define ETSEC_RQUEUE_EN1		0x00000040 /* queue enable */
#define ETSEC_RQUEUE_EN2		0x00000020 /* queue enable */
#define ETSEC_RQUEUE_EN3		0x00000010 /* queue enable */
#define ETSEC_RQUEUE_EN4		0x00000008 /* queue enable */
#define ETSEC_RQUEUE_EN5		0x00000004 /* queue enable */
#define ETSEC_RQUEUE_EN6		0x00000002 /* queue enable */
#define ETSEC_RQUEUE_EN7		0x00000001 /* queue enable */

/* RX bit field extraction control register */

#define ETSEC_RBIFX_B0CTL		0xC0000000
#define ETSEC_RBIFX_B0OFF		0x3F000000
#define ETSEC_RBIFX_B1CTL		0x00C00000
#define ETSEC_RBIFX_B1OFF		0x003F0000
#define ETSEC_RBIFX_B2CTL		0x0000C000
#define ETSEC_RBIFX_B2OFF		0x00003F00
#define ETSEC_RBIFX_B3CTL		0x000000C0
#define ETSEC_RBIFX_B3OFF		0x0000003F

/*
 * byte 0 control bits decide how the offset value will be applied.
 * Options include no extraction, extract from <start of ethernet frame
 * header> - 8 (i.e. start of preamble), extract starting from L3
 * header, and extract starting from L4 header.
 */

#define ETSEC_BCTL_NONE			0x00 /* no extraction */
#define ETSEC_BCTL_PREAMBLE		0x01 /* -8 bytes from start of frame */
#define ETSEC_BCTL_L3			0x02 /* from start of L3 header */
#define ETSEC_BCTL_L4			0x03 /* from start of L4 header */

/* MAC configuration 1 register */

#define ETSEC_MACCFG1_SOFT_RESET	0x80000000 /* Reset the entire MAC */
#define ETSEC_MACCFG1_RESET_RX_MC	0x00080000 /* Reset RX mac control */
#define ETSEC_MACCFG1_RESET_TX_MC	0x00040000 /* Reset TX mac control */
#define ETSEC_MACCFG1_RESET_RX_FUN	0x00020000 /* Reset RX function */
#define ETSEC_MACCFG1_RESET_TX_FUN	0x00010000 /* Reset TX function */
#define ETSEC_MACCFG1_LOOPBACK		0x00000100 /* MAC loopback mode */
#define ETSEC_MACCFG1_RX_FLOW		0x00000020 /* enable RX flow control */
#define ETSEC_MACCFG1_TX_FLOW		0x00000010 /* enable TX flow control */
#define ETSEC_MACCFG1_SYNCD_RX_EN	0x00000008 /* RX sync'ed */
#define ETSEC_MACCFG1_RX_EN		0x00000004 /* RX enable */
#define ETSEC_MACCFG1_SYNCD_TX_EN	0x00000002 /* TX sync'ed */
#define ETSEC_MACCFG1_TX_EN		0x00000001 /* TX enable */


/* MAC configuration 2 register */

#define ETSEC_MACCFG2_IF_MODE_MASK	0x00000003
#define ETSEC_MACCFG2_IF_MODE_MII	0x00000001
#define ETSEC_MACCFG2_IF_MODE_GMII_TBI	0x00000002

#define ETSEC_MACCFG2_PREAMBLE_LEN	0x0000F000
#define ETSEC_MACCFG2_IF_MODE		0x00000300
#define ETSEC_MACCFG2_HUGE_FRAME	0x00000020 
#define ETSEC_MACCFG2_LENGTH_CHECK	0x00000010
#define ETSEC_MACCFG2_PADCRC		0x00000004
#define ETSEC_MACCFG2_CRC_EN		0x00000002
#define ETSEC_MACCFG2_FULL_DUPLEX	0x00000001

#define ETSEC_IFMODE_RSVD0		0x00000000
#define ETSEC_IFMODE_MII		0x00000100 /* nibble mode */
#define ETSEC_IFMODE_GMII_TBI		0x00000200 /* byte mode */
#define ETSEC_IFMODE_RSVD1		0x00000300

/* Interpacket gap register */

#define ETSEC_IPGIFG_BACKTOBACK		0x0000007F
#define ETSEC_IPGIFG_MINIMUM		0x00007F00
#define ETSEC_IPGIFG_NONBACKTOBACK1	0x007F0000
#define ETSEC_IPGIFG_NONBACKTOBACK2	0x7F000000

/* Halfduplex register */

#define ETSEC_HALFDUP_ALTBEB_TRUNC	0x00F00000
#define ETSEC_HALFDUP_BEB		0x00080000
#define ETSEC_HALFDUP_BPNBO		0x00040000
#define ETSEC_HALFDUP_NBO		0x00020000
#define ETSEC_HALFDUP_EXCESS_DEF	0x00010000
#define ETSEC_HALFDUP_RETRY		0x0000F000
#define ETSEC_HALFDUP_COL_WINDOW	0x0000003F

/* MII management configuration register */

#define ETSEC_MIIMCFG_RESET		0x80000000
#define ETSEC_MIIMCFG_NO_PRE		0x00000010
#define ETSEC_MIIMCFG_MGMT_CLK		0x0000000F

#define ETSEC_MIIMCFG_MCS_2		0x00000000
#define ETSEC_MIIMCFG_MCS_4		0x00000001
#define ETSEC_MIIMCFG_MCS_6		0x00000002
#define ETSEC_MIIMCFG_MCS_8		0x00000003
#define ETSEC_MIIMCFG_MCS_10		0x00000004
#define ETSEC_MIIMCFG_MCS_14		0x00000005
#define ETSEC_MIIMCFG_MCS_20		0x00000006
#define ETSEC_MIIMCFG_MCS_28		0x00000007

/* MII management command register */

#define ETSEC_MIIMCOM_SCAN		0x00000002
#define ETSEC_MIIMCOM_READ		0x00000001

/* MII management address register */

#define ETSEC_MIIMADD_PHYADDR		0x0000001F
#define ETSEC_MIIMADD_REGADDR		0x00001F00

/* MII management control register */

#define ETSEC_MIIMCON_WRDATA		0x0000FFFF

/* MII management status register */

#define ETSEC_MIIMSTAT_RDDATA		0x0000FFFF

/* MII management indicator register */

#define ETSEC_MIIMIND_NOT_VALID		0x00000004
#define ETSEC_MIIMIND_SCAN		0x00000002
#define ETSEC_MIIMIND_BUSY		0x00000001

/* Attribute register */

#define ETSEC_ATTR_ELCWT		0x00006000 /* data L2 cache extract */
#define ETSEC_ATTR_BDLWT		0x00000C00 /* BD L2 cache extract */
#define ETSEC_ATTR_RDSEN		0x00000080 /* RX buffer snooping */
#define ETSEC_ATTR_RBDSEN		0x00000040 /* RX descriptor snooping */

#define ETSEC_ELCWT_NONE		0x00000000
#define ETSEC_ELCWT_RSVD		0x00002000
#define ETSEC_ELCWT_L2			0x00004000
#define ETSEC_ELCWT_L2_LOCK		0x00006000

#define ETSEC_BDLWT_NONE		0x00000000
#define ETSEC_BDLWT_RSVD		0x00000400
#define ETSEC_BDLWT_L2			0x00000800
#define ETSEC_BDLWT_L2_LOCK		0x00000C00

/* Attribute extract length and index register */

#define ETSEC_ATTRELI_EXTLEN		0x3FFF0000
#define ETSEC_ATTRELI_EXTIDX		0x00003FFF

#define ETSEC_ATTRELI_EL(x)	(((x) & 0x3FFF) << 16)
#define ETSEC_ATTRELI_EI(x)      ((x) & 0x3FFF)

/*
 * Buffer descriptor format
 *
 * The descriptor structure is always 8 bytes. For the ETSEC, there's
 * an additional frame descriptor field which is hidden in
 * the packet buffer, which is used for specifying additional
 * information related to checksum offload.
 *
 * The descriptor layout is much the same as it is for all of
 * Motorola/Freescale's ethernet MACs (including the ETSEC, SCC
 * and CPM). There's a 16 bit status field, 16 bit length field,
 * and a 32-bit address field. Each descriptor describes a single
 * fragment. The last fragment has the 'last in frame' bit set.
 * Since there is no 'next' pointer, descriptors are allocated
 * in contiguous arrays. The last descriptor in the array must
 * have the 'wrap' bit set.
 */

typedef struct etsecDesc
    {
    volatile UINT16	bdSts;
    volatile UINT16	bdLen;
    volatile UINT32	bdAddr;
    } ETSEC_DESC;

/* Transmit descriptor status bits */

#define ETSEC_TBD_R		0x8000 /* owner, 0 == host, 1 == chip */
#define ETSEC_TBD_PADCRC	0x4000 /* autopad and append CRC */
#define ETSEC_TBD_W		0x2000 /* wrap, last descriptor in ring */
#define ETSEC_TBD_I		0x1000 /* interrupt after processing is done */
#define ETSEC_TBD_L		0x0800 /* last descriptor in frame */
#define ETSEC_TBD_TC		0x0400 /* append CRC */
#define ETSEC_TBD_DEF		0x0200 /* excess deferral condition */
#define ETSEC_TBD_TO1		0x0100 /* software scratch bit */
#define ETSEC_TBD_HFE_LC	0x0080 /* huge frame, or late collision */
#define ETSEC_TBD_RL		0x0040 /* retry limit exceeded */
#define ETSEC_TBD_RC		0x003C /* retry count */
#define ETSEC_TBD_UN		0x0002 /* TX underrun */
#define ETSEC_TBD_TOE		0x0002 /* offload enable */
#define ETSEC_TBD_TR		0x0001 /* packet truncated */

/* Receive descriptor status bits */

#define ETSEC_RBD_E		0x8000 /* owner, 0 == host, 1 == chip */
#define ETSEC_RBD_RO1		0x4000 /* software scratch bit */
#define ETSEC_RBD_W		0x2000 /* wrap, last descriptor in ring */
#define ETSEC_RBD_I		0x1000 /* interrupt after processing is done */
#define ETSEC_RBD_L		0x0800 /* last descriptor in frame */
#define ETSEC_RBD_F		0x0400 /* first descriptor in frame */
#define ETSEC_RBD_M		0x0200 /* filter miss */
#define ETSEC_RBD_BC		0x0080 /* broadcast frame */
#define ETSEC_RBD_MC		0x0040 /* multicast frame */
#define ETSEC_RBD_LG		0x0020 /* giant frame error */
#define ETSEC_RBD_NO		0x0010 /* alignment error */
#define ETSEC_RBD_SH		0x0008 /* runt frame error */
#define ETSEC_RBD_CR		0x0004 /* CRC error */
#define ETSEC_RBD_OV		0x0002 /* FIFO overrun */
#define ETSEC_RBD_TR		0x0001 /* packet truncated */

#define ETSEC_RX_DESC_CNT	128
#define ETSEC_TX_DESC_CNT	128

/* Transmit FCB format */

typedef struct etsecTxFcb
    {
    volatile UINT8	fcbFlags;
    volatile UINT8	fcbRsvd;
    volatile UINT8	fcbl4os;
    volatile UINT8	fcbl3os;
    volatile UINT16	fcbPhcs;
    volatile UINT16	fcbVlctl;
    } ETSEC_TX_FCB;

#define ETSEC_TX_FCB_VLAN	0x80	/* VLAN CTL is valid */
#define ETSEC_TX_FCB_IP		0x40	/* Packet is IP */
#define ETSEC_TX_FCB_IP6	0x20	/* Packet is IPv6 */
#define ETSEC_TX_FCB_TUP	0x10	/* Packet is TCP or UDP */
#define ETSEC_TX_FCB_UDP	0x08	/* Packet is UDP */
#define ETSEC_TX_FCB_CIP	0x04	/* Calculate IP header checksum */
#define ETSEC_TX_FCB_CTU	0x02	/* Calculate TCP/UDP checksum */
#define ETSEC_TX_FCB_NPH	0x01	/* Disable pseudo-header csum */

/* Receive FCB format */

typedef struct etsecRxFcb
    {
    volatile UINT8	fcbFlags;
    volatile UINT8	fcbPerr;
    volatile UINT8	fcbRxQueue;
    volatile UINT8	fcbProto;
    volatile UINT16	fcbRsvd;
    volatile UINT16	fcbVlctl;
    } ETSEC_RX_FCB;

#define ETSEC_RX_FCB_VLAN	0x80	/* VLAN tagged frame found */
#define ETSEC_RX_FCB_IP		0x40	/* Packet is IP */
#define ETSEC_RX_FCB_IP6	0x20	/* Packet is IPv6 */
#define ETSEC_RX_FCB_TUP	0x10	/* Packet is TCP/UDP */
#define ETSEC_RX_FCB_CIP	0x08	/* IP header checksum checked */
#define ETSEC_RX_FCB_CTU	0x04	/* TCP/UDP checksum checked */
#define ETSEC_RX_FCB_EIP	0x02	/* IP header checksum error */
#define ETSEC_RX_FCB_ETU	0x01	/* TCP/UCP checksum error */

#define ETSEC_RX_FCB_PERR_NONE	0x00	/* No parser error */
#define ETSEC_RX_FCB_PERR_BADL3	0x0C	/* Unrecognized L3 sequence */

#define ETSEC_INC_DESC(x, y)	(x) = ((x + 1) & (y - 1))
#define ETSEC_MAXFRAG		16
#define ETSEC_MAX_RX		32
#define ETSEC_MAX_RX_QUEUES	8
#define ETSEC_CAM_CNT		16
#define ETSEC_RXCHUNK		4
#define ETSEC_ALIGN		2
#define ETSEC_RX_ADJ	\
    (ETHER_CRC_LEN + sizeof(ETSEC_RX_FCB) + ETSEC_ALIGN)
#define ETSEC_LEADINGSPACE(m)	((m)->m_data - (m)->m_extBuf)
#define ETSEC_PHYSADDR(x)	((UINT32)(x) + pDrvCtrl->etsecMask)

#define ETSEC_MTU	1500
#define ETSEC_JUMBO_MTU	9000
#define ETSEC_CLSIZE	1536
#define ETSEC_NAME	"motetsec"
#define ETSEC_TIMEOUT	100000
#define ETSEC_INTRS	(ETSEC_RXINTRS|ETSEC_TXINTRS|ETSEC_ERRINTRS)
#define ETSEC_RXINTRS	(ETSEC_IEVENT_RXF|ETSEC_IEVENT_RXC)
#define ETSEC_TXINTRS	(ETSEC_IEVENT_TXF|ETSEC_IEVENT_TXC)
#define ETSEC_ERRINTRS	(ETSEC_IEVENT_XFUN|ETSEC_IEVENT_EBERR|ETSEC_IEVENT_BSY)

/*
 * Default address for the TBI management interface. Normally this would
 * be 0x1F, but the Wind River SBC8641D board uses address 0x1F for one
 * of its copper PHYs.
 */

#define ETSEC_TBI_ADDR	0x1E


/*
 * Private adapter context structure.
 */

typedef struct etsec_drv_ctrl
    {
    END_OBJ		etsecEndObj;
    VXB_DEVICE_ID	etsecDev;
    void		*etsecMuxDevCookie;
    BOOL		etsecFiler;
    UINT32		etsecMask;

    JOB_QUEUE_ID	etsecJobQueue;
    QJOB		etsecIntJob;
    atomicVal_t		etsecIntPending;

    UINT32		etsecRxTxEvents;

    UINT8		etsecTxCur;
    UINT8		etsecTxLast;
    volatile BOOL	etsecTxStall;
    UINT16		etsecTxThresh;

    BOOL		etsecPolling;
    M_BLK_ID		etsecPollBuf;
    UINT32		etsecIntMask;
    UINT32		etsecIntrs;

    UINT8		etsecAddr[ETHER_ADDR_LEN];

    END_CAPABILITIES	etsecCaps;

    END_IFDRVCONF	etsecEndStatsConf;
    END_IFCOUNTERS	etsecEndStatsCounters;


    /* Begin MII/ifmedia required fields. */
    END_MEDIALIST	*etsecMediaList;
    END_ERR		etsecLastError;
    UINT32		etsecCurMedia;
    UINT32		etsecCurStatus;
    VXB_DEVICE_ID	etsecMiiBus;
    VXB_DEVICE_ID	etsecMiiDev;
    FUNCPTR		etsecMiiPhyRead;
    FUNCPTR		etsecMiiPhyWrite;
    int			etsecMiiPhyAddr;

    /* End MII/ifmedia required fields */

    /* DMA tags and maps. */

    ETSEC_DESC *	etsecRxDesc;
    ETSEC_DESC *	etsecTxDesc;
    ETSEC_TX_FCB *	etsecTxFcb[ETSEC_TX_DESC_CNT];

    ETSEC_DESC *	etsecRxQueue[ETSEC_MAX_RX_QUEUES];
    M_BLK_ID *		etsecRxMblk;
    M_BLK_ID		etsecRxMblks[ETSEC_MAX_RX_QUEUES][ETSEC_RX_DESC_CNT];
    M_BLK_ID		etsecTxMblk[ETSEC_TX_DESC_CNT];

    UINT32		etsecTxProd;
    UINT32		etsecTxCons;
    UINT32		etsecTxFree;
    UINT32 *		etsecRxIdx;
    UINT32		etsecRxIdxs[ETSEC_MAX_RX_QUEUES];

    SEM_ID		etsecDevSem;

    int			etsecMaxMtu;
    int			etsecTbiAddr;

    } ETSEC_DRV_CTRL;

typedef union etsec_filer_rbifx
    {
    struct
        {
        UINT32 b0ctl:2;
        UINT32 b0offset:6;
        UINT32 b1ctl:2;
        UINT32 b1offset:6;
        UINT32 b2ctl:2;
        UINT32 b2offset:6;
        UINT32 b3ctl:2;
        UINT32 b3offset:6;
        } details;
    UINT32 val;
    } ETSEC_FILER_RBIFX;

typedef union etsec_filer_rqfar
    {
    struct
        {
        UINT32 reserved:24;
        UINT32 tableIndex:8;
        } details;
    UINT32 val;
    } ETSEC_FILER_RQFAR;

typedef union etsec_filer_rqfcr
    {
    struct
        {
        UINT32 rsvd1:16;
        UINT32 qnum:6;
        UINT32 clusterEnable:1;
        UINT32 reject:1;
        UINT32 and:1;
        UINT32 cmp:2;
        UINT32 rsvd2:1;
        UINT32 pid:4;
        } details;
    UINT32 val;
    } ETSEC_FILER_RQFCR;

typedef union etsec_filer_rqfpr
    {
    struct
        {
        UINT32 rsvd1:16;
        UINT32 ebc:1;
        UINT32 vln:1;
        UINT32 cfi:1;
        UINT32 jum:1;
        UINT32 ipf:1;
        UINT32 fif:1;
        UINT32 ip4:1;
        UINT32 ip6:1;
        UINT32 icc:1;
        UINT32 icv:1;
        UINT32 tcp:1;
        UINT32 udp:1;
        UINT32 tuc:1;
        UINT32 tuv:1;
        UINT32 per:1;
        UINT32 eer:1;
        } propertyType;
    struct
        {
        UINT32 byte0:8;
        UINT32 byte1:8;
        UINT32 byte2:8;
        UINT32 byte3:8;
        } arb;
    struct
        {
        UINT32 resv:8;
        UINT32 destMAChigh:24;
        } dah;
    struct
        {
        UINT32 resv:8;
        UINT32 destMAClow:24;
        } dal;
    struct
        {
        UINT32 resv:8;
        UINT32 srcMAChigh:24;
        } sah;
    struct
        {
        UINT32 resv:8;
        UINT32 srcMAClow:24;
        } sal;
    struct
        {
        UINT32 rsvd:16;
        UINT32 etherType:16;
        } ety;
    struct
        {
        UINT32 rsvd:20;
        UINT32 vlanId:12;
        } vln;
    struct
        {
        UINT32 rsvd:29;
        UINT32 vlanPrio:3;
        } pri;
    struct
        {
        UINT32 rsvd:24;
        UINT32 typeOfService:8;
        } tos;
    struct
        {
        UINT32 rsvd:8;
        UINT32 l4ProtoId:24;
        } l4p;
    struct
        {
        UINT32 destIp;
        } dia;
    struct
        {
        UINT32 srcIp;
        } sia;
    struct
        {
        UINT32 rsvd:8;
        UINT32 destPort:24;
        }dpt;
    struct
        {
        UINT32 rsvd:8;
        UINT32 srcPort:24;
        } spt;
    UINT32 val;
    } ETSEC_FILER_RQFPR;


typedef struct etsec_filer_entry
    {
    UINT32 filerEnable;
    ETSEC_FILER_RBIFX rbifx;
    ETSEC_FILER_RQFAR rqfar;
    ETSEC_FILER_RQFCR rqfcr;
    ETSEC_FILER_RQFPR rqfpr;
    } ETSEC_FILER_ENTRY;

#define ETSEC_FILER_TABLE_SIZE 256
#define ETSEC_FILER_DISABLE 0
#define ETSEC_FILER_ENABLE 1
#define ETSEC_FILER_END 0xFFFFFFFF

/*
 * Property values specified in the property ID field in the
 * RX queue filer table control register (RQFCR). Each property
 * defines a different kind of filtering rule that can be applied.
 */

#define ETSEC_RQFCR_PID_MASK	0	/* Set filer mask register */
#define ETSEC_RQFCR_PID_PROP	1	/* packet properties */
#define ETSEC_RQFCR_PID_ARB	2	/* arbitrary, user-defined bytes */
#define ETSEC_RQFCR_PID_DAH	3	/* dst MAC address, high bits */
#define ETSEC_RQFCR_PID_DAL	4	/* dst MAC address, low bits */
#define ETSEC_RQFCR_PID_SAH	5	/* src MAC address, high bits */
#define ETSEC_RQFCR_PID_SAL	6	/* src MAC address, low bits */
#define ETSEC_RQFCR_PID_ETY	7	/* ethertype */
#define ETSEC_RQFCR_PID_VID	8	/* VLAN ID */
#define ETSEC_RQFCR_PID_PRI	9	/* VLAN user priority */
#define ETSEC_RQFCR_PID_TOS	10	/* IPv4 header type of service */
#define ETSEC_RQFCR_PID_L4P	11	/* layer 4 protocol identifier */
#define ETSEC_RQFCR_PID_DIA	12	/* dst IP address */
#define ETSEC_RQFCR_PID_SIA	13	/* src IP address */
#define ETSEC_RQFCR_PID_DPT	14	/* destination TCP/UDP port */
#define ETSEC_RQFCR_PID_SPT	15	/* source TCP/UDP port */

/*
 * Comparison operator.
 */

#define ETSEC_RQFCR_CMP_EQ	0 /* equal */
#define ETSEC_RQFCR_CMP_GTE	1 /* greater than or equal */
#define ETSEC_RQFCR_CMP_NE	2 /* not equal */
#define ETSEC_RQFCR_CMP_LT	3 /* less than */

/* Fields in the RQFCR register */

#define ETSEC_RQFCR_PID(x)	(x & 0xF)
#define ETSEC_RQFCR_CMP(x)	((x & 3) << 5)
#define ETSEC_RQFCR_AND(x)	((x & 1) << 7)
#define ETSEC_RQFCR_REJ(x)	((x & 1) << 8)
#define ETSEC_RQFCR_CLE(x)	((x & 1) << 9)
#define ETSEC_RQFCR_Q(x)	((x & 0x3F) << 10)

/* Property bits used when "packet properties" PID is selected. */

#define ETSEC_RQFPR_PROP_EBC	0x8000 /* ethernet broadcast */
#define ETSEC_RQFPR_PROP_VLN	0x4000 /* VLAN tag */
#define ETSEC_RQFPR_PROP_CFI	0x2000 /* VLAN canonical format indicator */
#define ETSEC_RQFPR_PROP_JUM	0x1000 /* ethernet jumbo frame */
#define ETSEC_RQFPR_PROP_IPF	0x0800 /* fragmented IPv4 or IPv6 packet */
#define ETSEC_RQFPR_PROP_FIF	0x0400 /* frame entered eTSEC FIFO interface */
#define ETSEC_RQFPR_PROP_IP4	0x0200 /* IPv4 header */
#define ETSEC_RQFPR_PROP_IP6	0x0100 /* IPv6 header */
#define ETSEC_RQFPR_PROP_ICC	0x0080 /* IPv4 header checksum checked */
#define ETSEC_RQFPR_PROP_ICV	0x0040 /* IPv4 header checksum correct */
#define ETSEC_RQFPR_PROP_TCP	0x0020 /* TCP header */
#define ETSEC_RQFPR_PROP_UDP	0x0010 /* UDP header */
#define ETSEC_RQFPR_PROP_PER	0x0002 /* parse error */
#define ETSEC_RQFPR_PROP_EER	0x0001 /* ethernet framing error */


/* Custom ioctls for programming the filer */

#ifndef EIOCSIFRULES
#define EIOCSIFRULES	0x10000000
#define EIOCGIFRULES	0x20000000
#define EIOCINITRULES	0x30000000
#endif

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

#endif /* __INCvxbEtsecEndh */
