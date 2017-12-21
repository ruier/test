/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,23sep09,x_z  update quiccEngine configuration.
01h,15jul09,b_m  add QE IO port configuration table.
01g,24jul08,b_m  switch to use new vxbus etsec and ucc driver.
01f,26jul07,b_m  add m85xxTimer support.
01e,18jul07,b_m  add epic/qe interrupt controller support.
01d,12jul07,b_m  add m85xxPci support.
01c,28may07,b_m  add qefccHEnd support.
01b,23may07,b_m  add DUART(ns16550) support.
01a,15may07,b_m  derived from version 01d of wrSbc8548/hwconf.c.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <../src/hwif/h/mii/mv88E1x11Phy.h>
#ifdef DRV_INTCTLR_QE
#include <hwif/intCtlr/vxbQeIntCtlr.h>
#endif
#ifdef INCLUDE_UCC_VXB_END
#include <../src/hwif/h/end/vxbUccEnd.h>
#endif
#ifdef INCLUDE_QUICC_ENGINE_UTILS
#include <../src/hwif/h/resource/qeIOPort.h>
#endif
#ifdef DRV_PCIBUS_M85XX
#include <drv/pci/pciAutoConfigLib.h>
#endif

#include "config.h"

/* externs */

#ifdef DRV_PCIBUS_M85XX
extern STATUS sysPciExAutoconfigInclude(PCI_SYSTEM * pSys, PCI_LOC * pLoc, UINT devVend);
extern UCHAR  sysPciExAutoconfigIntrAssign(PCI_SYSTEM * pSys, PCI_LOC * pLoc, UCHAR pin);
#endif


const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic", 0, 0 },
    { 1, "m85xxTimerDev", 0, 0 },
    { 3, "m85xxTimerDev", 0, 2 },
    { 4, "m85xxTimerDev", 0, 1 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)TRUE } },
    { "input",          HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,     "ns16550",  0, 0 },
    { EPIC_DUART_INT_VEC,     "ns16550",  1, 0 },
#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { EPIC_TSEC1TX_INT_VEC,   "motetsec", 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,   "motetsec", 0, 1 },
    { EPIC_TSEC1ERR_INT_VEC,  "motetsec", 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,   "motetsec", 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,   "motetsec", 1, 1 },
    { EPIC_TSEC2ERR_INT_VEC,  "motetsec", 1, 2 },
#endif
#ifdef DRV_INTCTLR_QE
    { INUM_QE_HIGH, "quiccEngineIntCtlr", 0, 0 },
    { INUM_QE_LOW,  "quiccEngineIntCtlr", 0, 1 },
#endif
};

const struct hcfResource epic0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)CCSBAR } },
    { "input",          HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "numCpus",        HCF_RES_INT,  { (void *)1 } },
#ifdef DRV_PCIBUS_M85XX
    { "exPolar",        HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
#endif
};
#define epic0Num NELEMENTS(epic0Resources)

struct hcfResource m85xxTimerResources[] =  {
    { "regBase",       HCF_RES_INT,  { (void *)0 } },
    { "decMinClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "fitMinClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
#endif
    { "sysClockFreq",  HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

const struct hcfResource ns165500Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4500) } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)sysClkFreqGet } },
    { "regInterval", HCF_RES_INT,  { (void *)1 } }
};
#define ns165500Num NELEMENTS(ns165500Resources)

const struct hcfResource ns165501Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4600) } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)sysClkFreqGet} },
    { "regInterval", HCF_RES_INT,  { (void *)1 } }
};
#define ns165501Num NELEMENTS(ns165501Resources)

/*
 * On the Freescale MDS8568 board, PHYs are physically wired according to
 * the following table:
 *
 * Data port pins   Management port pins    MII address
 * --------------   --------------------    -----------
 * PHY0: etsec0     etsec0                  2
 * PHY1: etsec1     etsec0                  3
 * PHY2: qefcc0     etsec0                  7
 * PHY3: qefcc1     etsec0                  1
 *
 * The tricky part is that all the 4 PHYs have their management pins connected
 * to etsec0.  The VxBus ETSEC driver allows us to specify the name of the
 * device that provides PHY management support.  This device is "motetsec0"
 * in all the cases.  We also specify the "phyAddr" resource for each device,
 * which is 2 for ETSEC0, 3 for ETSEC1, 7 for UCC0 and 1 for UCC1.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

const struct hcfResource etsec0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "phyAddr",   HCF_RES_INT,    { (void *)2 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },};
#define etsec0Num   NELEMENTS(etsec0Resources)

const struct hcfResource etsec1Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "phyAddr",   HCF_RES_INT,    { (void *)3 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },};
#define etsec1Num   NELEMENTS(etsec1Resources)

#endif /* defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2) */

#ifdef INCLUDE_UCC_VXB_END

const struct hcfResource qefcc0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + 0x2000) } },
    { "uccNum",    HCF_RES_INT,    { (void *)1 } },
    { "uccMedia",  HCF_RES_INT,    { (void *)UCC_MEDIA_RGMII } },
    { "phyAddr",   HCF_RES_INT,    { (void *)7 } },
#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
#else
    { "miiIfName", HCF_RES_STRING, { (void *)"tsecMdio" } },
#endif
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },
};
#define qefcc0Num   NELEMENTS(qefcc0Resources)

const struct hcfResource qefcc1Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + QUICC_ENGINE_BA + 0x3000) } },
    { "uccNum",    HCF_RES_INT,    { (void *)2 } },
    { "uccMedia",  HCF_RES_INT,    { (void *)UCC_MEDIA_RGMII } },
    { "phyAddr",   HCF_RES_INT,    { (void *)1 } },
#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
#else
    { "miiIfName", HCF_RES_STRING, { (void *)"tsecMdio" } },
#endif
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },
};
#define qefcc1Num   NELEMENTS(qefcc1Resources)

#if !defined(INCLUDE_ETSEC_VXB_END) && !defined(INCLUDE_ETSEC_VXB_END2)

LOCAL const struct hcfResource tsecMdio0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x24000) } },
};
#define tsecMdio0Num    NELEMENTS(tsecMdio0Resources)

#endif /* !defined(INCLUDE_ETSEC_VXB_END) && !defined(INCLUDE_ETSEC_VXB_END2) */

#endif /* INCLUDE_UCC_VXB_END */

#ifdef DRV_INTCTLR_QE

struct intrCtlrInputs quiccEngineIntCtlrInputs[] = {
    { QE_INUM_UCC1, "qefcc", 0, 0 },
    { QE_INUM_UCC2, "qefcc", 1, 0 },
};

const QE_SIU_INT_MASK qeEnableHighPriInts[QE_IVEC_MAX] = {
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000 }
};

const UINT32 qeINumToMaskPattern[QE_IVEC_MAX] = {
    0x00000000,         /* interrupt number 0: Error */
    0x00008000,         /* interrupt number 1: I2C */
    0x00004000,         /* interrupt number 2: SPI */
    0x00002000,         /* interrupt number 3: Risc Timer Table */
    0x00000000,         /* interrupt number 4: */
    0x00000000,         /* interrupt number 5: */
    0x00000000,         /* interrupt number 6:  */
    0x00000000,         /* interrupt number 7:  */
    0x00000000,         /* interrupt number 8:  */
    0x00000000,         /* interrupt number 9:  */
    0x00000040,         /* interrupt number 10: SDMA */
    0x00000020,         /* interrupt number 11: USB */
    0x00000010,         /* interrupt number 12: Timer 1 */
    0x00000008,         /* interrupt number 13: Timer 2 */
    0x00000004,         /* interrupt number 14: Timer 3 */
    0x00000002,         /* interrupt number 15: Timer 4 */
    0x00000000,         /* interrupt number 16: */
    0x00000000,         /* interrupt number 17: */
    0x00000000,         /* interrupt number 18: */
    0x00000000,         /* interrupt number 19: */
    0x10000000,         /* interrupt number 20: VT*/
    0x00000000,         /* interrupt number 21: */
    0x00000000,         /* interrupt number 22: */
    0x00000000,         /* interrupt number 23: */
    0x00000000,         /* interrupt number 24: */
    0x00800000,         /* interrupt number 25: EXT1 */
    0x00400000,         /* interrupt number 26: EXT2 */
    0x00200000,         /* interrupt number 27: EXT3 */
    0x00100000,         /* interrupt number 28: EXT4 */
    0x00000000,         /* interrupt number 29: */
    0x00000000,         /* interrupt number 30: */
    0x00000000,         /* interrupt number 31: */
    0x80000000,         /* interrupt number 32: UCC1 */
    0x40000000,         /* interrupt number 33: UCC2 */
    0x20000000,         /* interrupt number 34: UCC3 */
    0x10000000,         /* interrupt number 35: UCC4 */
    0x08000000,         /* interrupt number 36: MCC1 */
    0x00000000,         /* interrupt number 37: */
    0x00000000,         /* interrupt number 38: */
    0x00000000,         /* interrupt number 39: */
    0x00800000,         /* interrupt number 40: UCC5 */
    0x00400000,         /* interrupt number 41: UCC6 */
    0x00200000,         /* interrupt number 42: UCC7 */
    0x00100000,         /* interrupt number 43: UCC8 */
    0x00000000,         /* interrupt number 44: */
    0x00000000,         /* interrupt number 45: */
    0x00000000,         /* interrupt number 46: */
    0x00000000,         /* interrupt number 47: */
    0x00000000,         /* interrupt number 48: */
    0x00000000,         /* interrupt number 49: */
    0x00000000,         /* interrupt number 50: */
    0x00000000,         /* interrupt number 51: */
    0x00000000,         /* interrupt number 52: */
    0x00000000,         /* interrupt number 53: */
    0x00000000,         /* interrupt number 54: */
    0x00000000,         /* interrupt number 55: */
    0x00000000,         /* interrupt number 56: reserved */
    0x00000000,         /* interrupt number 57: reserved */
    0x00000000,         /* interrupt number 58: reserved */
    0x00000000,         /* interrupt number 59: reserved */
    0x00000000,         /* interrupt number 60: reserved */
    0x00000000,         /* interrupt number 61: reserved */
    0x00000000,         /* interrupt number 62: reserved */
    0x00000000          /* interrupt number 63: reserved */
};

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { "regBase",           HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA) } },
    { "input",             HCF_RES_ADDR, { (void *)&quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *)NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",      HCF_RES_ADDR, { (void *)&qeEnableHighPriInts[0] } },
    { "intMaskTableSize",  HCF_RES_INT,  { (void *)QE_IVEC_MAX } },
    { "inumMaskTable",     HCF_RES_ADDR, { (void *)&qeINumToMaskPattern[0] } },
    { "inumMaskTableSize", HCF_RES_INT,  { (void *)QE_IVEC_MAX } }
};
#define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

#endif /* DRV_INTCTLR_QE */

#ifdef INCLUDE_QUICC_ENGINE_UTILS

const QE_IOPORT_TABLE qeIoTable[] = {
    /* UCC1 Ethernet */

    { 4, 10, 1, 0, 2 }, /* TxD0 */
    { 4,  9, 1, 0, 2 }, /* TxD1 */
    { 4,  8, 1, 0, 2 }, /* TxD2 */
    { 4,  7, 1, 0, 2 }, /* TxD3 */
    { 4, 23, 1, 0, 2 }, /* TxD4 */
    { 4, 22, 1, 0, 2 }, /* TxD5 */
    { 4, 21, 1, 0, 2 }, /* TxD6 */
    { 4, 20, 1, 0, 2 }, /* TxD7 */

    { 4, 15, 2, 0, 2 }, /* RxD0 */
    { 4, 14, 2, 0, 2 }, /* RxD1 */
    { 4, 13, 2, 0, 2 }, /* RxD2 */
    { 4, 12, 2, 0, 2 }, /* RxD3 */
    { 4, 29, 2, 0, 2 }, /* RxD4 */
    { 4, 28, 2, 0, 2 }, /* RxD5 */
    { 4, 27, 2, 0, 2 }, /* RxD6 */
    { 4, 26, 2, 0, 2 }, /* RxD7 */

    { 4, 11, 1, 0, 2 }, /* TX_EN */
    { 4, 24, 1, 0, 2 }, /* TX_ER */
    { 4, 19, 1, 0, 2 }, /* GTX_CLK */
    { 4, 30, 2, 0, 2 }, /* RX_ER */
    { 4, 16, 2, 0, 2 }, /* RX_DV */
    { 4, 17, 2, 0, 2 }, /* RX_CLK */
    { 4, 18, 2, 0, 2 }, /* TX_CLK */
    { 4, 31, 2, 0, 2 }, /* COL */
    { 4, 25, 2, 0, 2 }, /* CRS */

    /* UCC2 Ethernet */

    { 5, 10, 1, 0, 2 }, /* TxD0 */
    { 5,  9, 1, 0, 2 }, /* TxD1 */
    { 5,  8, 1, 0, 2 }, /* TxD2 */
    { 5,  7, 1, 0, 2 }, /* TxD3 */
    { 5, 23, 1, 0, 2 }, /* TxD4 */
    { 5, 22, 1, 0, 2 }, /* TxD5 */
    { 5, 21, 1, 0, 2 }, /* TxD6 */
    { 5, 20, 1, 0, 2 }, /* TxD7 */

    { 5, 15, 2, 0, 2 }, /* RxD0 */
    { 5, 14, 2, 0, 2 }, /* RxD1 */
    { 5, 13, 2, 0, 2 }, /* RxD2 */
    { 5, 12, 2, 0, 2 }, /* RxD3 */
    { 5, 29, 2, 0, 2 }, /* RxD4 */
    { 5, 28, 2, 0, 2 }, /* RxD5 */
    { 5, 27, 2, 0, 2 }, /* RxD6 */
    { 5, 26, 2, 0, 2 }, /* RxD7 */

    { 5, 11, 1, 0, 2 }, /* TX_EN */
    { 5, 24, 1, 0, 2 }, /* TX_ER */
    { 5, 19, 1, 0, 2 }, /* GTX_CLK */
    { 5, 30, 2, 0, 2 }, /* RX_ER */
    { 5, 16, 2, 0, 2 }, /* RX_DV */
    { 5, 17, 2, 0, 2 }, /* RX_CLK */
    { 5, 18, 2, 0, 2 }, /* TX_CLK */
    { 5, 31, 2, 0, 2 }, /* COL */
    { 5, 25, 2, 0, 2 }, /* CRS */

    { 1, 31, 2, 0, 3 }, /* GE125 */
};

const struct hcfResource quiccEngineResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA) } },
    { "regBase1",       HCF_RES_INT,  { (void *)(CCSBAR + QE_IO_PORTS) } },
    { "muRamBase",      HCF_RES_INT,  { (void *)(CCSBAR + QUICC_ENGINE_BA + QE_MURAM_BA) } },
    { "muRamPoolSize",  HCF_RES_INT,  { (void *)QE_MURAM_SIZE } },
    { "qeIoTable",      HCF_RES_ADDR, { (void *)&qeIoTable[0] } },
    { "qeIoTableSize",  HCF_RES_INT,  { (void *)NELEMENTS(qeIoTable) } }
};
#define quiccEngine0Num NELEMENTS(quiccEngineResources)

#endif /* INCLUDE_QUICC_ENGINE_UTILS */

#ifdef DRV_RESOURCE_M85XXCCSR

const struct hcfResource m85xxLawBarResources[] = {
    { "regBase",           HCF_RES_INT,    { (void *)CCSBAR } },
    { "lawBarNum",         HCF_RES_INT,    { (void *)8 } },
    { "LAWBAR0",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR1",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR2",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR3",           HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR4",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR5",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR6",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR7",           HCF_RES_STRING, { "m85xxRio" } },
    { "LAWAR_ENABLE",      HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT", HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
};
#define m85xxLawBarNum  NELEMENTS(m85xxLawBarResources)

#endif /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_PCIBUS_M85XX

const struct hcfResource m85xxPci0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)(CCSBAR + 0xa000) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX_MEM_ADRS} },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE} },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE} },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE} },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciExAutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciExAutoconfigIntrAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)LAWAR_TGTIF_PCIEX } },

    /* Window Attributes */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM | PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM | PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO | PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_PREFETCHABLE | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)TRUE } },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)0x10000000 } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",             HCF_RES_INT, { (void *)(TRUE)} },
    { "dynamicInterrupts",     HCF_RES_INT, { (void *)(TRUE)} },
    { "msiDataWorkaround",     HCF_RES_INT, { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr", HCF_RES_INT, { (void *)(0x80000000)} }
#else
    { "msiEnable",             HCF_RES_INT, { (void *)(FALSE)} },
    { "dynamicInterrupts",     HCF_RES_INT, { (void *)(FALSE)} },
    { "msiDataWorkaround",     HCF_RES_INT, { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr", HCF_RES_INT, { (void *)(0x80000000)} }
#endif
};
#define m85xxPci0Num    NELEMENTS(m85xxPci0Resources)

#endif /* DRV_PCIBUS_M85XX */


const struct hcfDevice hcfDeviceList[] = {
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0, VXB_BUSID_PLB, 0, epic0Num,       epic0Resources },

    { "m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},

    { "ns16550", 0, VXB_BUSID_PLB, 0, ns165500Num, ns165500Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns165501Num, ns165501Resources },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec", 0, VXB_BUSID_PLB, 0, etsec0Num, etsec0Resources },
    { "motetsec", 1, VXB_BUSID_PLB, 0, etsec1Num, etsec1Resources },
#endif

#ifdef INCLUDE_UCC_VXB_END
    { "qefcc", 0, VXB_BUSID_PLB, 0, qefcc0Num, qefcc0Resources },
    { "qefcc", 1, VXB_BUSID_PLB, 0, qefcc1Num, qefcc1Resources },
#if !defined(INCLUDE_ETSEC_VXB_END) && !defined(INCLUDE_ETSEC_VXB_END2)
    { "tsecMdio", 0, VXB_BUSID_PLB, 0, tsecMdio0Num, tsecMdio0Resources },
#endif /* !defined(INCLUDE_ETSEC_VXB_END) && !defined(INCLUDE_ETSEC_VXB_END2) */
#endif /* INCLUDE_UCC_VXB_END */

#ifdef DRV_INTCTLR_QE
    { "quiccEngineIntCtlr", 0, VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num, quiccEngineIntCtlr0Resources },
#endif
#ifdef INCLUDE_QUICC_ENGINE_UTILS
    { "quiccEngine", 0, VXB_BUSID_PLB, 0, quiccEngine0Num, quiccEngineResources },
#endif

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR", 0, VXB_BUSID_PLB, 0, m85xxLawBarNum, m85xxLawBarResources },
#endif

#ifdef DRV_PCIBUS_M85XX
    { "m85xxPci",  0, VXB_BUSID_PLB, 0, m85xxPci0Num,   m85xxPci0Resources },
#endif
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);


VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#if !defined(INCLUDE_ETSEC_VXB_END)
    { "mv88E1x11Phy", 0, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { "mv88E1x11Phy", 1, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
#else
    { "mv88E1x11Phy", 2, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { "mv88E1x11Phy", 3, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
#endif
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
