/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22apr09,syt  derived from version 02b of ads8572/hwconf.c
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>
#include "config.h"
#include "mot85xxPci.h"

extern unsigned int divisorGet(unsigned int xtal,unsigned int baud);

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_INTCTLR_EPIC
extern UINT32 sysPicClkFreqGet(UINT32);
#endif  /* DRV_INTCTLR_EPIC */

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS  sysPciEx1AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx1AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS  sysPciEx2AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx2AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS  sysPciEx3AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx3AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_EHCI
IMPORT void   ehci0Init (void);
IMPORT void   ehci0PostResetHook (void);
#endif /* INCLUDE_EHCI */


/*
 * On the P2020ds board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins    MII address
 *      --------------  --------------------    -----------
 * PHY0:    TSEC0       TSEC0
 * PHY1:    TSEC1       TSEC0
 * PHY2:    TSEC2       TSEC0
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

#if 1
const struct hcfResource etsec0Resources[] = {
	{ "regBase",		HCF_RES_INT,	{ (void *)(CCSBAR + 0x24000) } },
	{ "intr0",			HCF_RES_INT,	{ (void *)EPIC_TSEC1TX_INT_VEC } },
	{ "intr0Level", 	HCF_RES_INT,	{ (void *)EPIC_TSEC1TX_INT_VEC } },
	{ "intr1",			HCF_RES_INT,	{ (void *)EPIC_TSEC1RX_INT_VEC } },
	{ "intr1Level", 	HCF_RES_INT,	{ (void *)EPIC_TSEC1RX_INT_VEC } },
	{ "intr2",			HCF_RES_INT,	{ (void *)EPIC_TSEC1ERR_INT_VEC } },
	{ "intr2Level", 	HCF_RES_INT,	{ (void *)EPIC_TSEC1ERR_INT_VEC } },
	{ "phyAddr",		HCF_RES_INT,	{ (void *)1 } },
	{ "miiIfName",		HCF_RES_STRING, { (void *)"motetsec" } },
	{ "miiIfUnit",		HCF_RES_INT,	{ (void *)0 } }
};
#define etsec0Num NELEMENTS(etsec0Resources)

/*2号PHY接在J63A上, changed by dongyang*/
const struct hcfResource etsec1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
};
#else

const struct hcfResource etsec0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "phyAddr",            HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } }
};
#define etsec0Num NELEMENTS(etsec0Resources)



const struct hcfResource etsec1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "phyAddr",            HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } }
};
#endif

#define etsec1Num NELEMENTS(etsec1Resources)


#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",       HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",  HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR1",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR2",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR3",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR4",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR5",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR6",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR7",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR8",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR9",            HCF_RES_STRING, { (void *)NULL } }
};
#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

#ifdef VXB_M8641_DMA
const struct hcfResource fslDma1Resources[] = {
	{ "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE } },
	{ "irq",		 HCF_RES_INT, {(void *)EPIC_DMA1_CHAN0_INT_VEC} }
};
#define fslDma1Num NELEMENTS(fslDma1Resources)

const struct hcfResource fslDma1Resources1[] = {
	{ "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE1 } },
	{ "irq",		 HCF_RES_INT, {(void *)EPIC_DMA1_CHAN1_INT_VEC} }
};
#define fslDma1Num1 NELEMENTS(fslDma1Resources1)

const struct hcfResource fslDma1Resources2[] = {
	{ "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE2 } },
	{ "irq",		 HCF_RES_INT, {(void *)EPIC_DMA1_CHAN2_INT_VEC} }
};
#define fslDma1Num2 NELEMENTS(fslDma1Resources2)

const struct hcfResource fslDma1Resources3[] = {
	{ "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE3 } },
	{ "irq",		 HCF_RES_INT, {(void *)EPIC_DMA1_CHAN3_INT_VEC} }
};
#define fslDma1Num3 NELEMENTS(fslDma1Resources3)



const struct hcfResource fslDma2Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA2_REGBASE } },
};
#define fslDma2Num NELEMENTS(fslDma2Resources)
#endif  /* VXB_M8641_DMA */


const struct hcfResource ns1655x1Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4500) } },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)1} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
    { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet}},
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

#if 1
const struct hcfResource ns1655x2Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4600) } },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)1} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
    { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet}},
};
#else
const struct hcfResource ns1655x2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)COM2_ADR } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#endif

#define ns1655x2Num NELEMENTS(ns1655x2Resources)

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT   },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT   },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT  },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { 251, EPIC_IPI_IRQ_NUM,  EPIC_IPI_REG_BASE,  EPIC_IPI_REG_INTERVAL,  EPIC_IPI_INTERRUPT  }
};



/*注意此处写法: 两列数字中, 后面一列表示控制器编号, 前面1列表示该控制器中的几个中断*/
struct intrCtlrInputs epicInputs[] = 
{
    { EPIC_DUART_INT_VEC,       "ns16550",          0,      0 },
    { EPIC_DUART_INT_VEC,       "ns16550",          0,      1 },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { EPIC_TSEC1TX_INT_VEC,     "motetsec",         0,      0 },
    { EPIC_TSEC1RX_INT_VEC,     "motetsec",         0,      1 },
    { EPIC_TSEC1ERR_INT_VEC,    "motetsec",         0,      2 },
    { EPIC_TSEC2TX_INT_VEC,     "motetsec",         1,      0 },
    { EPIC_TSEC2RX_INT_VEC,     "motetsec",         1,      1 },
    { EPIC_TSEC2ERR_INT_VEC,    "motetsec",         1,      2 },
  
#endif  /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#if 0   /*changed by dongyang*/
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,  "openPicTimer",     0,      0 },
    { OPENPIC_TIMERA1_INT_VEC,  "openPicTimer",     0,      1 },
    { OPENPIC_TIMERA2_INT_VEC,  "openPicTimer",     0,      2 },
    { OPENPIC_TIMERA3_INT_VEC,  "openPicTimer",     0,      3 },
    { OPENPIC_TIMERB0_INT_VEC,  "openPicTimer",     1,      0 },
    { OPENPIC_TIMERB1_INT_VEC,  "openPicTimer",     1,      1 },
    { OPENPIC_TIMERB2_INT_VEC,  "openPicTimer",     1,      2 },
    { OPENPIC_TIMERB3_INT_VEC,  "openPicTimer",     1,      3 },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */
#endif

#ifdef  VXB_M8641_DMA

	{ EPIC_DMA0_INT_VEC,   "dma", 0, 0 },
	{ EPIC_DMA1_INT_VEC,   "dma", 1, 0 },
	{ EPIC_DMA2_INT_VEC,   "dma", 2, 0 },
	{ EPIC_DMA3_INT_VEC,   "dma", 3, 0 },    

	{ EPIC_DMA0_INT_VEC,   "dma", 0, 1 },
	{ EPIC_DMA1_INT_VEC,   "dma", 1, 1 },
	{ EPIC_DMA2_INT_VEC,   "dma", 2, 1 },
	{ EPIC_DMA3_INT_VEC,   "dma", 3, 1 },	 
#endif  /* VXB_M8641_DMA */


#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,        "ipi",              0,      0 },
#endif /* _WRS_CONFIG_SMP */
};

struct intrCtlrCpu epicCpu[] = {
    { EPIC_TSEC3ERR_INT_VEC,    1 },
    { EPIC_TSEC1ERR_INT_VEC,    1 },
    { EPIC_TSEC2ERR_INT_VEC,    1 }
};

#if 1    /*changed by dongyang*/

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
    { "cpuRoute",           HCF_RES_ADDR,   { (void *)&epicCpu[0] } },
    { "cpuRouteTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicCpu) } },

#ifdef INCLUDE_PCI_BUS
    { "exPolar",            HCF_RES_INT,    { (void *)EPIC_INT_ACT_LOW} },
#endif

/*changed by dongyang*/
/*
    { "exSense",            HCF_RES_INT,    { (void *)EPIC_SENSE_EDG } },
    { "exIrqWkrd",          HCF_RES_INT,    { (void *)FALSE } },
    { "intDesc_0",          HCF_RES_ADDR,   { (void *)&epicIntDesc[0] } },
    { "intDesc_1",          HCF_RES_ADDR,   { (void *)&epicIntDesc[1] } },
    { "intDesc_2",          HCF_RES_ADDR,   { (void *)&epicIntDesc[2] } },
    { "intDesc_3",          HCF_RES_ADDR,   { (void *)&epicIntDesc[3] } },
    { "intDesc_4",          HCF_RES_ADDR,   { (void *)&epicIntDesc[4] } },
    { "intDesc_5",          HCF_RES_ADDR,   { (void *)&epicIntDesc[5] } },
*/

};
#else
const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
    { "cpuRoute",           HCF_RES_ADDR,   { (void *)&epicCpu[0] } },
    { "cpuRouteTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicCpu) } },
    { "exPolar",            HCF_RES_INT,    { (void *)EPIC_INT_ACT_LOW } },
    { "exSense",            HCF_RES_INT,    { (void *)EPIC_SENSE_LVL } },
    { "exIrqWkrd",          HCF_RES_INT,    { (void *)FALSE } },
    { "intDesc_0",          HCF_RES_ADDR,   { (void *)&epicIntDesc[0] } },
    { "intDesc_1",          HCF_RES_ADDR,   { (void *)&epicIntDesc[1] } },
    { "intDesc_2",          HCF_RES_ADDR,   { (void *)&epicIntDesc[2] } },
    { "intDesc_3",          HCF_RES_ADDR,   { (void *)&epicIntDesc[3] } },
    { "intDesc_4",          HCF_RES_ADDR,   { (void *)&epicIntDesc[4] } },
    { "intDesc_5",          HCF_RES_ADDR,   { (void *)&epicIntDesc[5] } },
};
#endif

#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX

#if 1    /*changed by dongyang*/
struct hcfResource m85xxTimerResources[] =  {
    {"regBase",             HCF_RES_INT,    { (void *)0 } },
    {"decMinClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    {"decMaxClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },

    {"fitMinClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MIN } },
    {"fitMaxClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },

    {"sysClockFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet } },
};
#else
struct hcfResource m85xxTimerResources[] =  {
    {"regBase",             HCF_RES_INT,    { (void *)0 } },
    {"decMinClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    {"decMaxClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    {"fitMinClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MIN } },
    {"fitMaxClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
    {"sysClockFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet } },
};

#endif


#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,    "epic",             0,      0 },
    { 1,    "m85xxTimerDev",    0,      0 },
    { 3,    "m85xxTimerDev",    0,      2 },
    { 4,    "m85xxTimerDev",    0,      1 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)TRUE } },
    { "input",              HCF_RES_ADDR,   { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource usbEhci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(CCSBAR + 0x22000) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci0Init } },
    { "dataSwap",           HCF_RES_ADDR,   { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"A" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",         HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",         HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA3_INT_VEC } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"B" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)0 } },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB3_INT_VEC } },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif   /* DRV_TIMER_OPENPIC */


#ifndef COMPILE_BOOTROM
extern UINT32 sysSdhcClkFreqGet();

struct hcfResource fslSdhcResources[] = {
    { "regBase", HCF_RES_INT,  { (void *)CCSBAR + 0x2e000} },
    { "clkFreq", HCF_RES_ADDR, { (void *)sysSdhcClkFreqGet } },
    { "dmaMode", HCF_RES_INT,  { (void *)0 } },
    { "polling", HCF_RES_INT,  { (void *)TRUE } },

    { "irq",         HCF_RES_INT, {(void *)EPIC_SDHC_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)1} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_SDHC_INT_VEC} },
	
};
#define fslSdhcResourcesNum NELEMENTS(fslSdhcResources)

#endif


const struct hcfDevice hcfDeviceList[] = {

/*
 * Initialize ppcIntCtlr before epic.
 * The vector table for external interrupts are over written by epic
 * for an optimized purpose.
 */

    { "ppcIntCtlr", 0,  VXB_BUSID_PLB,  0,  ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0,  VXB_BUSID_PLB,  0,  epic0Num,       epic0Resources },


#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec",   0,  VXB_BUSID_PLB,  0,  etsec0Num, etsec0Resources },
    { "motetsec",   1,  VXB_BUSID_PLB,  0,  etsec1Num, etsec1Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */


#ifdef DRV_SIO_NS16550
    { "ns16550",    0,  VXB_BUSID_PLB,  0,  ns1655x1Num,    ns1655x1Resources },
    { "ns16550",    1,  VXB_BUSID_PLB,  0,  ns1655x2Num,    ns1655x2Resources },
#endif  /* DRV_SIO_NS16550 */


#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR",  0,  VXB_BUSID_PLB,  0,  m85xxCCSR0Num,  m85xxCCSR0Resources },
#endif  /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev",  0,  VXB_BUSID_PLB,  0,  m85xxTimerNum,  m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer",   0,  VXB_BUSID_PLB,  0,  openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",   1,  VXB_BUSID_PLB,  0,  openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_M85XX */

#ifdef VXB_M8641_DMA
    { "dma", 0,  VXB_BUSID_PLB,  0,  fslDma1Num, fslDma1Resources },
    { "dma", 1,  VXB_BUSID_PLB,  0,  fslDma1Num1, fslDma1Resources1 },
    { "dma", 2,  VXB_BUSID_PLB,  0,  fslDma1Num2, fslDma1Resources2 },
    { "dma", 3,  VXB_BUSID_PLB,  0,  fslDma1Num3, fslDma1Resources3 },
#endif /* VXB_M8641_DMA */


#ifndef COMPILE_BOOTROM
    {"fslSdhc", 0, VXB_BUSID_PLB, 0, fslSdhcResourcesNum, fslSdhcResources},
#endif

};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);


VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
	{ "motetsec", 0, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
	{ "motetsec", 1, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
	{ "mv88E1x11Phy", 0, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
	{ "mv88E1x11Phy", 1, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },

	{ NULL,            0, NULL,            VXB_PARAM_END_OF_LIST, { (void *)0 } }
};


