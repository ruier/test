/* hwconf.c - HW configuration support module for wrVbP4080 */

/*
 * Copyright (c) 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,25may11,agf  Add openPicTimer B
01p,04may11,agf  document resource array restrict imposed by sysDeviceFilter()
                 move Uart to end of hcfDeviceList to work-around WIND00255393
01o,07mar11,kxb  NAP 2.0 merge: TGEC support
01n,28jan11,kab  CQ:243790: Add MIPC buses
01m,12jun10,rab  Add guest IPI support
01l,25may10,wap  Add support for alternate FMAN setup on newer boards
01k,22mar10,wap  Add direct interrupt support
01j,23feb10,wap  Bman/Qman portal initialization is now automatic
01i,23feb10,wap  Remove references to fman microcode
01h,03feb10,wap  Fix defect WIND00198257
01g,07jan10,wap  Remove miiIfName and miiIfUnit properties from DTSEC entries
		 since, thanks to paravirtualized MDIO accesses in the DTSEC
		 driver, they're not necessary
01f,23dec09,wap  Make BMAN, QMAN and DTSEC resources dynamically configurable
01e,22dec09,to   add MIPC configuration
01d,21dec09,to   update openpic timer resources
01c,21dec09,wap  Add support for LAW, BMAN, QMAN, FMAN and DTSEC
01b,18dec09,to   add uart2 resources
01a,01dec09,to   created from wrVbE500Common/hwconf.c, v02d
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <hwif/util/vxbParamSys.h>
#include <wrhv/vbi.h>
#include "sysDuart.h"
#include <../src/hwif/h/end/vxbDtsecEnd.h>

IMPORT UINT32 sysClkFreqGet (void);
IMPORT UINT32 sysEprGet (void);

#ifdef DRV_TIMER_OPENPIC
IMPORT UINT32 sysPicClkFreqGet (UINT32);

/* OpenPic timer vector definition */
#define OPENPIC_TIMERA0_INT_VEC		140
#define OPENPIC_TIMERA1_INT_VEC		141
#define OPENPIC_TIMERA2_INT_VEC		142
#define OPENPIC_TIMERA3_INT_VEC		143
#define OPENPIC_TIMERB0_INT_VEC		144
#define OPENPIC_TIMERB1_INT_VEC		145
#define OPENPIC_TIMERB2_INT_VEC		146
#define OPENPIC_TIMERB3_INT_VEC		147
#endif /* DRV_TIMER_OPENPIC */

#undef  EPIC_VEC_IPI_IRQ0
#define EPIC_VEC_IPI_IRQ0 0x94
#define EPIC_VEC_IPI_IRQ1 (EPIC_VEC_IPI_IRQ0 + 1)
#define EPIC_VEC_IPI_IRQ2 (EPIC_VEC_IPI_IRQ0 + 2)
#define EPIC_VEC_IPI_IRQ3 (EPIC_VEC_IPI_IRQ0 + 3)

#ifdef DRV_RESOURCE_QORIQBMAN
/* Bman interrupt vector definitions */
#define EPIC_BMAN_PORTAL0_INT_VEC       105
#define EPIC_BMAN_PORTAL1_INT_VEC       107
#define EPIC_BMAN_PORTAL2_INT_VEC       109
#define EPIC_BMAN_PORTAL3_INT_VEC       111
#define EPIC_BMAN_PORTAL4_INT_VEC       113
#define EPIC_BMAN_PORTAL5_INT_VEC       115
#define EPIC_BMAN_PORTAL6_INT_VEC       117
#define EPIC_BMAN_PORTAL7_INT_VEC       119
#define EPIC_BMAN_PORTAL8_INT_VEC       121
#define EPIC_BMAN_PORTAL9_INT_VEC       123
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
/* Qman interrupt vector definitions */
#define EPIC_QMAN_PORTAL0_INT_VEC       104
#define EPIC_QMAN_PORTAL1_INT_VEC       106
#define EPIC_QMAN_PORTAL2_INT_VEC       108
#define EPIC_QMAN_PORTAL3_INT_VEC       110
#define EPIC_QMAN_PORTAL4_INT_VEC       112
#define EPIC_QMAN_PORTAL5_INT_VEC       114
#define EPIC_QMAN_PORTAL6_INT_VEC       116
#define EPIC_QMAN_PORTAL7_INT_VEC       118
#define EPIC_QMAN_PORTAL8_INT_VEC       120
#define EPIC_QMAN_PORTAL9_INT_VEC       122
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef INCLUDE_MIPC_SM
#define VIRT_MIPC_BUS_00		256
#define VIRT_MIPC_BUS_01		257
#define VIRT_MIPC_BUS_02		258
#define VIRT_MIPC_BUS_03		259
#define VIRT_MIPC_BUS_04		260
#define VIRT_MIPC_BUS_05		261
#define VIRT_MIPC_BUS_06		262
#define VIRT_MIPC_BUS_07		263
#endif /* INCLUDE_MIPC_SM */

#define MAX_VB_INTERRUPTS	256
#define MAX_HW_INTERRUPTS	256
#define MAX_VIRTUAL_INTERRUPTS	32

UINT32 hwIntTable[MAX_VB_INTERRUPTS];
UINT32 vbIntTable[MAX_HW_INTERRUPTS+MAX_VIRTUAL_INTERRUPTS];

/******************************************************************************
*
* interruptConversionTableSetup - VB vectors.
*
* This function convert loads hwIntTable and vbIntTable arrays with
* interrupt sources to the VB vectors.  It is called once during initialization.
* The arrays are used to convert between HW and VB vectors.
*
* RETURNS: NONE
*
* ERRNO
*
* \NOMANUAL
*/
void interruptConversionTableSetup()
    {
    int     intNum;
    int32_t vector;

    bfill ((char*)hwIntTable, sizeof(hwIntTable), 0xff);
    bfill ((char*)vbIntTable, sizeof(vbIntTable), 0xff);

    for (intNum=0;intNum<(MAX_HW_INTERRUPTS + MAX_VIRTUAL_INTERRUPTS);intNum++)
	{
	switch (intNum)
	    {
	    case EPIC_DUART_INT_VEC:
		vector = vbiIntVecFind ("duart1", VB_INPUT_INT);
		if (vector == VBI_INVALID_IRQ)
		    vector = vbiIntVecFind ("duart2", VB_INPUT_INT);
		break;
	    case EPIC_VEC_IPI_IRQ0:
		vector = vbiIntVecFind ("ipi0", VB_INPUT_INT);
		break;
	    case EPIC_VEC_IPI_IRQ1:
		vector = vbiIntVecFind ("ipi1", VB_INPUT_INT);
		break;
	    case EPIC_VEC_IPI_IRQ2:
		vector = vbiIntVecFind ("ipi2", VB_INPUT_INT);
		break;
	    case EPIC_VEC_IPI_IRQ3:
		vector = vbiIntVecFind ("ipi3", VB_INPUT_INT);
		break;
#ifdef DRV_TIMER_OPENPIC
	    case OPENPIC_TIMERA0_INT_VEC:
		vector = vbiIntVecFind ("picTimerA0", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERA1_INT_VEC:
		vector = vbiIntVecFind ("picTimerA1", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERA2_INT_VEC:
		vector = vbiIntVecFind ("picTimerA2", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERA3_INT_VEC:
		vector = vbiIntVecFind ("picTimerA3", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERB0_INT_VEC:
		vector = vbiIntVecFind ("picTimerB0", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERB1_INT_VEC:
		vector = vbiIntVecFind ("picTimerB1", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERB2_INT_VEC:
		vector = vbiIntVecFind ("picTimerB2", VB_INPUT_INT);
		break;
	    case OPENPIC_TIMERB3_INT_VEC:
		vector = vbiIntVecFind ("picTimerB3", VB_INPUT_INT);
		break;
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_RESOURCE_QORIQBMAN
	    case EPIC_BMAN_PORTAL0_INT_VEC:
		vector = vbiIntVecFind ("bman0", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL1_INT_VEC:
		vector = vbiIntVecFind ("bman1", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL2_INT_VEC:
		vector = vbiIntVecFind ("bman2", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL3_INT_VEC:
		vector = vbiIntVecFind ("bman3", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL4_INT_VEC:
		vector = vbiIntVecFind ("bman4", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL5_INT_VEC:
		vector = vbiIntVecFind ("bman5", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL6_INT_VEC:
		vector = vbiIntVecFind ("bman6", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL7_INT_VEC:
		vector = vbiIntVecFind ("bman7", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL8_INT_VEC:
		vector = vbiIntVecFind ("bman8", VB_INPUT_INT);
		break;
	    case EPIC_BMAN_PORTAL9_INT_VEC:
		vector = vbiIntVecFind ("bman9", VB_INPUT_INT);
		break;
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
	    case EPIC_QMAN_PORTAL0_INT_VEC:
		vector = vbiIntVecFind ("qman0", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL1_INT_VEC:
		vector = vbiIntVecFind ("qman1", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL2_INT_VEC:
		vector = vbiIntVecFind ("qman2", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL3_INT_VEC:
		vector = vbiIntVecFind ("qman3", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL4_INT_VEC:
		vector = vbiIntVecFind ("qman4", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL5_INT_VEC:
		vector = vbiIntVecFind ("qman5", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL6_INT_VEC:
		vector = vbiIntVecFind ("qman6", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL7_INT_VEC:
		vector = vbiIntVecFind ("qman7", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL8_INT_VEC:
		vector = vbiIntVecFind ("qman8", VB_INPUT_INT);
		break;
	    case EPIC_QMAN_PORTAL9_INT_VEC:
		vector = vbiIntVecFind ("qman9", VB_INPUT_INT);
		break;
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef INCLUDE_MIPC_SM
	    case VIRT_MIPC_BUS_00:
		vector = vbiIntVecFind ("mipcbus0", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_01:
		vector = vbiIntVecFind ("mipcbus1", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_02:
		vector = vbiIntVecFind ("mipcbus2", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_03:
		vector = vbiIntVecFind ("mipcbus3", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_04:
		vector = vbiIntVecFind ("mipcbus4", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_05:
		vector = vbiIntVecFind ("mipcbus5", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_06:
		vector = vbiIntVecFind ("mipcbus6", VB_INPUT_INT);
		break;
	    case VIRT_MIPC_BUS_07:
		vector = vbiIntVecFind ("mipcbus7", VB_INPUT_INT);
		break;
#endif /* INCLUDE_MIPC_SM */

	    default:
		vector = 0xff;
		break;
	    }

	if (vector >= 0 && vector < MAX_VB_INTERRUPTS)
	    {
#ifdef DEBUG
	    char outStr[80];
	    sprintf (outStr,"find vector 0x%x 0x%x\n", vector, intNum);
	    vbiKputs (outStr);
#endif
	    hwIntTable[vector] = intNum;
	    vbIntTable[intNum] = vector;
	    }
	}
    }

/******************************************************************************
*
* hw2VbConvert - conversion function for hw interrupt to VB interrupt
*
* RETURNS: virtboard interrupt number
*
* ERROR
*
* \NOMANUAL
*/
int hw2VbConvert
    (
    int inputPin
    )
    {
    return vbIntTable[inputPin];
    }

/******************************************************************************
*
* vb2HwConvert - conversion function for VB interrupt to hw interrupt
*
* RETURNS: hw interrupt number
*
* ERROR
*
* \NOMANUAL
*/
int vb2HwConvert
    (
    int vbVector
    )
    {
    return hwIntTable[vbVector];
    }

/*
 * --------------- Device Resource Structures ---------------
 */

#ifdef DRV_SIO_NS16550
const struct hcfResource ns1655x1Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)UART1_BASE} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { VXB_CLK_FREQ,  HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)UART2_BASE} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { VXB_CLK_FREQ,  HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_TIMER_VB
const struct hcfResource vbTimerResources[] = {
    { VXB_REG_BASE, HCF_RES_INT, {(void *)NULL} }
};
#define vbTimerNum NELEMENTS(vbTimerResources)
#endif /* DRV_TIMER_VB */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(0xFE0410F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"A" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)8}},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC }},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA1_INT_VEC }},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA2_INT_VEC }},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA3_INT_VEC }}
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(0xFE0420F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"B" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)8}},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC }},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB1_INT_VEC }},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB2_INT_VEC }},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB3_INT_VEC }}
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

const struct intrCtlrInputs vbIntCtlrInputs[] = {
#ifdef DRV_SIO_NS16550
    { EPIC_DUART_INT_VEC, "ns16550", 0, 0 },
#endif /* DRV_SIO_NS16550 */
#if ((defined DRV_TIMER_OPENPIC) && !(defined INCLUDE_INTCTLR_DYNAMIC_LIB))
    { OPENPIC_TIMERA0_INT_VEC, "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA1_INT_VEC, "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA2_INT_VEC, "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA3_INT_VEC, "openPicTimer", 0, 3 },
    { OPENPIC_TIMERB0_INT_VEC, "openPicTimer", 1, 0 },
    { OPENPIC_TIMERB1_INT_VEC, "openPicTimer", 1, 1 },
    { OPENPIC_TIMERB2_INT_VEC, "openPicTimer", 1, 2 },
    { OPENPIC_TIMERB3_INT_VEC, "openPicTimer", 1, 3 },
#endif /* DRV_TIMER_OPENPIC */
#ifdef DRV_RESOURCE_QORIQQMAN
    { EPIC_QMAN_PORTAL0_INT_VEC, "QorIQQman", 0, 0 },
    { EPIC_QMAN_PORTAL1_INT_VEC, "QorIQQman", 0, 1 },
    { EPIC_QMAN_PORTAL2_INT_VEC, "QorIQQman", 0, 2 },
    { EPIC_QMAN_PORTAL3_INT_VEC, "QorIQQman", 0, 3 },
    { EPIC_QMAN_PORTAL4_INT_VEC, "QorIQQman", 0, 4 },
    { EPIC_QMAN_PORTAL5_INT_VEC, "QorIQQman", 0, 5 },
    { EPIC_QMAN_PORTAL6_INT_VEC, "QorIQQman", 0, 6 },
    { EPIC_QMAN_PORTAL7_INT_VEC, "QorIQQman", 0, 7 },
    { EPIC_QMAN_PORTAL8_INT_VEC, "QorIQQman", 0, 8 },
    { EPIC_QMAN_PORTAL9_INT_VEC, "QorIQQman", 0, 9 },
#endif /* DRV_RESOURCE_QORIQQMAN */
#ifdef DRV_RESOURCE_QORIQBMAN
    { EPIC_BMAN_PORTAL0_INT_VEC, "QorIQBman", 0, 0 },
    { EPIC_BMAN_PORTAL1_INT_VEC, "QorIQBman", 0, 1 },
    { EPIC_BMAN_PORTAL2_INT_VEC, "QorIQBman", 0, 2 },
    { EPIC_BMAN_PORTAL3_INT_VEC, "QorIQBman", 0, 3 },
    { EPIC_BMAN_PORTAL4_INT_VEC, "QorIQBman", 0, 4 },
    { EPIC_BMAN_PORTAL5_INT_VEC, "QorIQBman", 0, 5 },
    { EPIC_BMAN_PORTAL6_INT_VEC, "QorIQBman", 0, 6 },
    { EPIC_BMAN_PORTAL7_INT_VEC, "QorIQBman", 0, 7 },
    { EPIC_BMAN_PORTAL8_INT_VEC, "QorIQBman", 0, 8 },
    { EPIC_BMAN_PORTAL9_INT_VEC, "QorIQBman", 0, 9 },
#endif /* DRV_RESOURCE_QORIQBMAN */
#ifdef INCLUDE_MIPC_SM
    { VIRT_MIPC_BUS_00, "mipcbus0", 0, 0 },
    { VIRT_MIPC_BUS_01, "mipcbus1", 0, 0 },
    { VIRT_MIPC_BUS_02, "mipcbus2", 0, 0 },
    { VIRT_MIPC_BUS_03, "mipcbus3", 0, 0 },
    { VIRT_MIPC_BUS_04, "mipcbus4", 0, 0 },
    { VIRT_MIPC_BUS_05, "mipcbus5", 0, 0 },
    { VIRT_MIPC_BUS_06, "mipcbus6", 0, 0 },
    { VIRT_MIPC_BUS_07, "mipcbus7", 0, 0 },
#endif /* INCLUDE_MIPC_SM */
};

const struct hcfResource vbIntCtlrResources[] = {
    { VXB_REG_BASE,	HCF_RES_INT,	{(void *)CCSBAR } },
    { "input",		HCF_RES_ADDR,	{(void *)&vbIntCtlrInputs[0] } },
    { "inputTableSize",	HCF_RES_INT,	{(void *)NELEMENTS(vbIntCtlrInputs) } },
    { "hw2vbIntFunc",	HCF_RES_ADDR,	{(void *)hw2VbConvert} },
    { "vb2hwIntFunc",	HCF_RES_ADDR,	{(void *)vb2HwConvert} },
    { "eprFunc",	HCF_RES_ADDR,	{(void *)sysEprGet} }
};
#define vbIntCtlrNum NELEMENTS(vbIntCtlrResources)

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "vbIntCtlr",     0, 0 }, /* INTR vector (0x0500) */
    { 6, "vbIntCtlr",     0, 1 }, /* DIRECT vector (0x2000) */
    { 1, "vxbVbTimerDev", 0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,	HCF_RES_INT,	{(void *)TRUE} },
    { "input",		HCF_RES_ADDR,	{(void *)&ppcIntCtlrInputs[0]} },
    { "inputTableSize",	HCF_RES_INT,	{(void *)NELEMENTS(ppcIntCtlrInputs)} }
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

/* LAW resources */

const struct hcfResource law0Resources[] = {
    { VXB_REG_BASE, HCF_RES_INT, { (void *)(CCSBAR + 0xC00) } },
};
#define law0Num NELEMENTS(law0Resources)

/* BMAN resources */

const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)(CCSBAR + 0x31A000) } },
    { "bmanLawBase", HCF_RES_ADDR, {(void *)BMAN_LAW_BASE} }
};
#define bman0Num NELEMENTS(bman0Resources)

/* QMAN resources */

const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)(CCSBAR + 0x318000) } },
    { "qmanLawBase", HCF_RES_ADDR, {(void *)QMAN_LAW_BASE} }
};
#define qman0Num NELEMENTS(qman0Resources)


/* FMAN resources */

/*
 * WARNING: sysDeviceFilter() in sysLib.c _requires_ 'globalInit' be
 * at array index 1.
 */

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    {(void *)(CCSBAR + 0x400000) } },
    { "globalInit",     HCF_RES_INT,    {(void *)1} },
};
#define fman0Num NELEMENTS(fman0Resources)

struct hcfResource fman1Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    {(void *)(CCSBAR + 0x500000) } },
    { "globalInit",     HCF_RES_INT,    {(void *)1} },
};
#define fman1Num NELEMENTS(fman1Resources)


/* DTSEC resources */

/*
 * WARNING: sysDeviceFilter() in sysLib.c _requires_ 'coreNum' be
 * at array index 1.
 */

struct hcfResource dtsec0Resources[] = {
#ifdef FMAN_SWAP
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x5E0000) } },
#else
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x4E0000) } },
#endif
    { "coreNum",        HCF_RES_INT,    { (void *)0 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0x1C } },
#ifdef FMAN_SWAP
    { "fmanNum",        HCF_RES_INT,    { (void *)1 } },
#endif
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } }
};
#define dtsec0Num NELEMENTS(dtsec0Resources)

struct hcfResource dtsec1Resources[] = {
#ifdef FMAN_SWAP
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x5E2000) } },
#else
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x4E2000) } },
#endif
    { "coreNum",        HCF_RES_INT,    { (void *)0 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0x1D } },
#ifdef FMAN_SWAP
    { "fmanNum",        HCF_RES_INT,    { (void *)1 } },
#endif
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } }
};
#define dtsec1Num NELEMENTS(dtsec1Resources)

struct hcfResource dtsec2Resources[] = {
#ifdef FMAN_SWAP
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x5E4000) } },
#else
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x4E4000) } },
#endif
    { "coreNum",        HCF_RES_INT,    { (void *)0 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0x1E } },
#ifdef FMAN_SWAP
    { "fmanNum",        HCF_RES_INT,    { (void *)1 } },
#endif
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } }
};
#define dtsec2Num NELEMENTS(dtsec2Resources)

struct hcfResource dtsec3Resources[] = {
#ifdef FMAN_SWAP
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x5E6000) } },
#else
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x4E6000) } },
#endif
    { "coreNum",        HCF_RES_INT,    { (void *)0 } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0x1F } },
#ifdef FMAN_SWAP
    { "fmanNum",        HCF_RES_INT,    { (void *)1 } },
#endif
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } }
};
#define dtsec3Num NELEMENTS(dtsec3Resources)

struct hcfResource dtsec4Resources[] = {
#ifdef FMAN_SWAP
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x4E2000) } },
#else
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x5E0000) } },
#endif
    { "coreNum",        HCF_RES_INT,    { (void *)0 } },
#ifdef SECONDARY_SGMII_RISER
    { "phyAddr",        HCF_RES_INT,    { (void *)0x1C } },
#else
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
#endif
#ifdef FMAN_SWAP
    { "fmanNum",        HCF_RES_INT,    { (void *)0 } },
    { "dtsecNum",       HCF_RES_INT,    { (void *)1 } },
#endif
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_RGMII } },
};
#define dtsec4Num NELEMENTS(dtsec4Resources)


/* TGEC resources */

/*
 * WARNING: sysDeviceFilter() in sysLib.c _requires_ 'coreNum' be
 * at array index 1.
 */

struct hcfResource tgec0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4F0000) } },
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
};
#define tgec0Num NELEMENTS(tgec0Resources)

struct hcfResource tgec1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x5F0000) } },
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
};
#define tgec1Num NELEMENTS(tgec1Resources)


HCF_DEVICE hcfDeviceList[] = {
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "vbIntCtlr", 0, VXB_BUSID_PLB, 0, vbIntCtlrNum, vbIntCtlrResources },
#ifdef DRV_TIMER_VB
    { "vxbVbTimerDev", 0, VXB_BUSID_PLB, 0, vbTimerNum, vbTimerResources },
#endif /* DRV_TIMER_VB */
#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0, VXB_BUSID_PLB, 0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer", 1, VXB_BUSID_PLB, 0, openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_RESOURCE_QORIQLAW
    { "QorIQLaw", 0, VXB_BUSID_PLB, 0, law0Num, law0Resources },
#endif

#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman", 0, VXB_BUSID_PLB, 0, bman0Num, bman0Resources },
#endif

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman", 0, VXB_BUSID_PLB, 0, qman0Num, qman0Resources },
#endif

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman", 0, VXB_BUSID_PLB, 0, fman0Num, fman0Resources },
    { "QorIQFman", 1, VXB_BUSID_PLB, 0, fman1Num, fman1Resources },
#endif

#ifdef DRV_VXBEND_DTSEC
    { "dtsec", 0, VXB_BUSID_PLB, 0, dtsec0Num, dtsec0Resources },
    { "dtsec", 1, VXB_BUSID_PLB, 0, dtsec1Num, dtsec1Resources },
    { "dtsec", 2, VXB_BUSID_PLB, 0, dtsec2Num, dtsec2Resources },
    { "dtsec", 3, VXB_BUSID_PLB, 0, dtsec3Num, dtsec3Resources },
    { "dtsec", 4, VXB_BUSID_PLB, 0, dtsec4Num, dtsec4Resources },
#endif

#ifdef DRV_VXBEND_TGEC
    { "tgec", 0, VXB_BUSID_PLB, 0, tgec0Num, tgec0Resources },
    { "tgec", 1, VXB_BUSID_PLB, 0, tgec1Num, tgec1Resources },
#endif

#ifdef DRV_SIO_NS16550
    /* 
     * The UART device gets disabled during initialization, specifically by the default
     * baud setting in ns16550vxbInitChannel(). Therefore, place ns16550 driver last in
     * the hcfDevice table since, when the UART baud is reset, all serial output,
     * including that from the Hypervisor context such as calls to vbiKputs(), is stopped.
     * WIND00255393
     */

    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },
#endif
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
};
