/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------
01g,30nov12,j_z  add RapidIO support.
01f,07jun11,x_s  removed components dependency for mpicInputs to temporary
                 resolve interrupt pins initialized only on core0. (WIND00279768)
01e,25mar11,x_z  added AXIS timer support.
01d,08mar11,x_z  replaced INCLUDE_PCI with DRV_PCIBUS_LSI_PCIEX.
01c,22jan11,x_s  added PCIe support.
01b,10nov10,x_z  added support for uAMP and USB.
01a,01jun10,x_z  written.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/intCtlr/vxbMpicIntCtlr.h>
#include <arch/ppc/dcr4xxALib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>

#ifdef DRV_PCIBUS_LSI_PCIEX
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#endif /* DRV_PCIBUS_LSI_PCIEX */

#ifdef INCLUDE_RAPIDIO_BUS
#include <vxbus/vxbRapidIO.h>
#endif /* INCLUDE_RAPIDIO_BUS */

#include "config.h"

/* globals */

IMPORT UINT32 sysClkFreqGet (void);
IMPORT UINT32 sysTimerClkFreqGet (void);
IMPORT UINT32 sysUart0ClkGet (void);
IMPORT UINT32 sysUart1ClkGet (void);

#if defined (INCLUDE_USB) && defined (INCLUDE_EHCI)

/* import to do the BSP level initialization */

extern void   sysEhciInit(void);
extern void   usbOpAfterReset();
extern UINT32 vxbSwap32 (UINT32);
#endif /* defined (INCLUDE_USB) && defined (INCLUDE_EHCI) */

#ifdef DRV_PCIBUS_LSI_PCIEX
IMPORT UCHAR sysPciEx0AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,
    PCI_LOC *       pLoc,
    UCHAR           pin
    );
IMPORT STATUS sysPciEx0AutoCfgInclude();
#endif /* DRV_PCIBUS_LSI_PCIEX */

/* device resources */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    {0, "mpic",         0, 0},
    {5, "mpic",         0, 1},
    {1, "ppc440Timer",  0, 0},
    {3, "ppc440Timer",  0, 2},
    {4, "ppc440Timer",  0, 1}
};

const struct hcfResource ppcIntCtlrResources[] = {
    {"regBase",         HCF_RES_INT,  {(void *) TRUE}},
    {"input",           HCF_RES_ADDR, {(void *) &ppcIntCtlrInputs[0]}},
    {"inputTableSize",  HCF_RES_INT,  {(void *) NELEMENTS (ppcIntCtlrInputs)}},
};
#define ppcIntCtlrNum   NELEMENTS(ppcIntCtlrResources)

struct hcfResource ppc440TimerResources[] =  {
    {"regBase",         HCF_RES_INT,  {(void *)0}},
    {"decMinClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MIN}},
    {"decMaxClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MAX}},
    {"fitMinClkRate",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    {"fitMaxClkRate",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    {"wdtMinClkRate",   HCF_RES_INT,  {(void *)WDT_RATE_MIN}},
    {"wdtMaxClkRate",   HCF_RES_INT,  {(void *)WDT_RATE_MAX}},
    {"clkFreq",         HCF_RES_ADDR, {(void *) &sysClkFreqGet}}
};
#define ppc440TimerNum  NELEMENTS(ppc440TimerResources)

#if defined(INCLUDE_AMP_UART_0) || !defined(INCLUDE_AMP_CPU)
const struct hcfResource uart0Resources[] = {
    {"regBase",         HCF_RES_INT,  {(void *) (UART0_BA)}},
    {"clkFreq",         HCF_RES_ADDR, {(void *) sysUart0ClkGet}}
};
#   define uart0Num        NELEMENTS(uart0Resources)
#endif  /* INCLUDE_UART_0 || !INCLUDE_AMP_CPU */

#if defined(INCLUDE_AMP_UART_1) || !defined(INCLUDE_AMP_CPU)

const struct hcfResource uart1Resources[] = {
    {"regBase",         HCF_RES_INT,  {(void *) (UART1_BA)}},
    {"clkFreq",         HCF_RES_ADDR, {(void *) sysUart1ClkGet}}
};
#define uart1Num        NELEMENTS(uart1Resources)

#   if  defined(INCLUDE_AMP_UART_0) || !defined(INCLUDE_AMP_CPU)
#       define uart1Unit   1
#   else
#       define uart1Unit   0
#   endif  /* INCLUDE_AMP_UART_0 || !INCLUDE_AMP_CPU */
#endif  /* INCLUDE_AMP_UART_1 || !INCLUDE_AMP_CPU */

#if defined(INCLUDE_AMP_AXIS_TIMER5) || !defined(INCLUDE_AMP_CPU)

LOCAL const struct hcfResource lsiTimer0Resources[] =  {
    { "regBase",    HCF_RES_INT,  {(void *) (GP5_TIMER_BA) } },
    { "clkFreq",    HCF_RES_ADDR, {(void *) sysTimerClkFreqGet } },
    { "clkRateMin", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MAX } }
    };
#define lsiTimer0Num    NELEMENTS(lsiTimer0Resources)
#define lsiTimerCnt     1
#endif /* INCLUDE_AMP_AXIS_TIMER5 || !INCLUDE_AMP_CPU */

#if defined(INCLUDE_AMP_AXIS_TIMER6) || !defined(INCLUDE_AMP_CPU)

LOCAL const struct hcfResource lsiTimer1Resources[] =  {
    { "regBase",    HCF_RES_INT,  {(void *) (GP6_TIMER_BA) } },
    { "clkFreq",    HCF_RES_ADDR, {(void *) sysTimerClkFreqGet } },
    { "clkRateMin", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MAX } }
    };
#   define lsiTimer1Num     NELEMENTS(lsiTimer1Resources)

#   ifdef lsiTimerCnt

#   define lsiTimer1Unit    1
#   undef  lsiTimerCnt
#   define lsiTimerCnt      2

#   else

#   define lsiTimer1Unit    0
#   undef  lsiTimerCnt
#   define lsiTimerCnt      1

#   endif /* lsiTimerCnt */

#endif /* INCLUDE_AMP_AXIS_TIMER6 || !INCLUDE_AMP_CPU */

#if defined(INCLUDE_AMP_AXIS_TIMER7) || !defined(INCLUDE_AMP_CPU)

LOCAL const struct hcfResource lsiTimer2Resources[] =  {
    { "regBase",    HCF_RES_INT,  {(void *) (GP7_TIMER_BA) } },
    { "clkFreq",    HCF_RES_ADDR, {(void *) sysTimerClkFreqGet } },
    { "clkRateMin", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MAX } }
    };
#define lsiTimer2Num    NELEMENTS(lsiTimer2Resources)

#   ifdef lsiTimerCnt

#       if (lsiTimerCnt == 1)
#           define lsiTimer2Unit    1
#           undef  lsiTimerCnt
#           define lsiTimerCnt      2
#       else
#           define lsiTimer2Unit    2
#           undef  lsiTimerCnt
#           define lsiTimerCnt      3
#       endif
#   else

#   define lsiTimer2Unit    0
#   undef  lsiTimerCnt
#   define lsiTimerCnt      1

#   endif /* lsiTimerCnt */

#endif /* INCLUDE_AMP_AXIS_TIMER6 || !INCLUDE_AMP_CPU */

/* UART1 timer can only be used as one GP-TImer when UART1 is not in use */

#if !defined(uart1Num) && defined(INCLUDE_AMP_AXIS_TIMER_UART1)

LOCAL const struct hcfResource lsiTimer3Resources[] =  {
    { "regBase",    HCF_RES_INT,  {(void *) (UART1_TIMER_BA) } },
    { "clkFreq",    HCF_RES_ADDR, {(void *) sysTimerClkFreqGet } },
    { "clkRateMin", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  {(void *) AUX_CLK_RATE_MAX } }
    };
#define lsiTimer3Num    NELEMENTS(lsiTimer3Resources)

#   ifdef lsiTimerCnt

#       if (lsiTimerCnt == 1)
#           define lsiTimer3Unit    1
#           undef  lsiTimerCnt
#           define lsiTimerCnt      2
#       elif (lsiTimerCnt == 2)
#           define lsiTimer3Unit    2
#           undef  lsiTimerCnt
#           define lsiTimerCnt      3
#       else
#           define lsiTimer3Unit    3
#           undef  lsiTimerCnt
#           define lsiTimerCnt      4
#       endif
#   else

#   define lsiTimer3Unit    0
#   undef  lsiTimerCnt
#   define lsiTimerCnt      1

#   endif /* lsiTimerCnt */

#endif /* !uart1Num && INCLUDE_AMP_AXIS_TIMER_UART1 */

#if defined(INCLUDE_AMP_LSI_END_DRIVER) || !defined(INCLUDE_AMP_CPU)
LOCAL const struct hcfResource lsiMdio0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *) (MDIO_BA ) } },
    { "clkPeriod",      HCF_RES_INT,    { (void *) MDIO_CLK_PERD } },
    { "clkOffset",      HCF_RES_INT,    { (void *) MDIO_CLK_OFFSET } }
};
#define lsiMdio0Num NELEMENTS(lsiMdio0Resources)

struct hcfResource lsiMac0Resources[]= {
    { "regBase",        HCF_RES_INT,    { (void *)(FEMAC_BA) } },
    { "phyAddr",        HCF_RES_INT,    { (void *) FEMAC_PHY_ADDR } },
    { "miiIfName",      HCF_RES_STRING, { (void *) "lsiMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *) 0 } }
    };
#define lsiMac0Num NELEMENTS(lsiMac0Resources)
#endif /* INCLUDE_AMP_LSI_END_DRIVER || !AMP */

struct intrCtlrInputs mpicInputs[] = {
    { INUM_FEMAC_DMA,       "lsiMac",           0,          0 },

#ifdef uart0Num
    { INUM_UART0,           "primeCellSioDev",  0,          0 },
#endif /* uart0Num */

#ifdef uart1Num
    { INUM_UART1,           "primeCellSioDev",  uart1Unit,  0 },
#endif /* uart1Num */

    { INUM_USB,             "vxbPlbUsbEhci",    0,          0 },

#ifdef lsiTimer0Num
    { INUM_TIMER5,          "lsiTimer",         0,          0 },
#endif /* lsiTimer0Num */

#ifdef lsiTimer1Num
    { INUM_TIMER6,          "lsiTimer",         lsiTimer1Unit, 0 },
#endif /* lsiTimer1Num */

#ifdef lsiTimer2Num
    { INUM_TIMER7,          "lsiTimer",         lsiTimer2Unit, 0 },
#endif /* lsiTimer2Num */

#ifdef lsiTimer3Num
    { INUM_TIMER3,          "lsiTimer",         lsiTimer3Unit, 0 },
#endif /* lsiTimer3Num */

#ifdef DRV_SRIO_LSI
    { INUM_SRIO,            "lsiSRio",          0,             0 },
#endif /* DRV_SRIO_LSI */

#ifdef DRV_PCIBUS_LSI_PCIEX
    { INUM_PEI0,            "lsiPciEx",         0,          0}
#endif /* DRV_PCIBUS_LSI_PCIEX */
};

struct intrCtlrPriority mpicPriority[] = {
    { INUM_FEMAC_DMA,   10 },
    { INUM_UART0,       10 },
    { INUM_UART1,       10 },
    { INUM_USB,         10 },

#ifdef lsiTimer0Num
    { INUM_TIMER5,      10 },
#endif /* lsiTimer0Num */

#ifdef lsiTimer1Num
    { INUM_TIMER6,      10 },
#endif /* lsiTimer1Num */

#ifdef lsiTimer2Num
    { INUM_TIMER7,      10 },
#endif /* lsiTimer2Num */

#ifdef lsiTimer3Num
    { INUM_TIMER3,      10 },
#endif /* lsiTimer3Num */

#ifdef DRV_SRIO_LSI
    { INUM_SRIO,        10},
#endif /* DRV_SRIO_LSI */

#ifdef DRV_PCIBUS_LSI_PCIEX
    { INUM_PEI0,        10}
#endif /* DRV_PCIBUS_LSI_PCIEX */
};

struct intrCtlrTrigger mpicTrigger[] = {
    { INUM_FEMAC_DMA,   VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { INUM_UART0,       VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { INUM_UART1,       VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
    { INUM_USB,         VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },

#ifdef lsiTimer0Num
    { INUM_TIMER5,      VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
#endif /* lsiTimer0Num */

#ifdef lsiTimer1Num
    { INUM_TIMER6,      VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
#endif /* lsiTimer1Num */

#ifdef lsiTimer2Num
    { INUM_TIMER7,      VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
#endif /* lsiTimer2Num */

#ifdef lsiTimer3Num
    { INUM_TIMER3,      VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS },
#endif /* lsiTimer3Num */

#ifdef DRV_SRIO_LSI
    { INUM_SRIO,        VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS},
#endif /* DRV_SRIO_LSI */

#ifdef DRV_PCIBUS_LSI_PCIEX
    { INUM_PEI0,        VXB_INTR_TRIG_LEVEL | VXB_INTR_TRIG_POS }
#endif /* DRV_PCIBUS_LSI_PCIEX */

};

const struct hcfResource mpic0Resources[] = {
    { "regBase",            HCF_RES_INT,  {(void *) MPIC0_DCR_BA } },
    { "input",              HCF_RES_ADDR, {(void *) &mpicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  {(void *) NELEMENTS(mpicInputs) } },
    { "priority",           HCF_RES_ADDR, {(void *) &mpicPriority[0] } },
    { "priorityTableSize",  HCF_RES_INT,  {(void *) NELEMENTS(mpicPriority) } },
    { "trigger",            HCF_RES_ADDR, {(void *) &mpicTrigger[0] } },
    { "triggerTableSize",   HCF_RES_INT,  {(void *) NELEMENTS(mpicTrigger) } }
};
#define mpic0Num NELEMENTS(mpic0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]= {
    {"regBase",          HCF_RES_INT,  {(void *) (USBDR_CAPLENGTH) } },
    {"ehciInit",         HCF_RES_ADDR, {(void *) sysEhciInit } },
    {"dataSwap",         HCF_RES_ADDR, {(void *) vxbSwap32 } }
};

#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)

#endif /* INCLUDE_EHCI */

#ifdef DRV_PCIBUS_LSI_PCIEX

struct hcfResource lsiPciEx0Resources[] = {
    { "regBase",          HCF_RES_INT,  { (void *) PCIE0_BA } },
    { "regBase1",         HCF_RES_INT,  { (void *) PCIE0_CFG_ADRS } },
    { "mem32Addr",        HCF_RES_ADDR, { (void *) PCIE0_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT,  { (void *) PCIE0_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR, { (void *) PCIE0_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT,  { (void *) PCIE0_MEMIO_SIZE } },
    { "io16Addr",         HCF_RES_ADDR, { (void *) PCIE0_IO_ADRS } },
    { "io16Size",         HCF_RES_INT,  { (void *) PCIE0_IO_SIZE } },
    { "pcieMemBaseAddr",  HCF_RES_INT,  { (void *) PCIE0_MEM_BASE_ADRS } },
    { "maxBusSet",        HCF_RES_INT,  { (void *) 15 } },
    { "autoConfig",       HCF_RES_INT,  { (void *) 1 } },
    { "includeFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx0AutoCfgInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *) sysPciEx0AutoCfgIntAssign } }
};
#define lsiPciEx0Num NELEMENTS(lsiPciEx0Resources)

#endif/* DRV_PCIBUS_LSI_PCIEX */

#ifdef DRV_SRIO_LSI
const struct hcfResource lsiRio0Resources[] = {
    { "regBase",         HCF_RES_INT,  {(void *)(SRIO_STD_BA)} },
    { "regBase1",        HCF_RES_INT,  {(void *)(SRIO_PER_BA)} },
    { "deviceBase",      HCF_RES_INT,  {(void *)(LSI_RIO_MEM_ADRS)}},
    { "smWinSize",       HCF_RES_INT,  {(void *)RIO_SM_WIN_SIZE_16M}},
    { "smHostAdrs",      HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},
};
#define lsiRio0Num    NELEMENTS(lsiRio0Resources)
#endif /* DRV_SRIO_LSI */

/* device list */

const struct hcfDevice hcfDeviceList[] = {
    {"ppcIntCtlr",      0, VXB_BUSID_PLB, 0, ppcIntCtlrNum,  ppcIntCtlrResources},
    {"mpic",            0, VXB_BUSID_PLB, 0, mpic0Num,       mpic0Resources},
    {"ppc440Timer",     0, VXB_BUSID_PLB, 0, ppc440TimerNum, ppc440TimerResources},

#ifdef uart0Num
    {"primeCellSioDev", 0, VXB_BUSID_PLB, 0, uart0Num,       uart0Resources},
#endif /* uart0Num */

#ifdef uart1Num
    {"primeCellSioDev", uart1Unit, VXB_BUSID_PLB, 0, uart1Num,       uart1Resources},
#endif /* uart1Num */

#if defined(INCLUDE_AMP_LSI_END_DRIVER) || !defined(INCLUDE_AMP_CPU)
    {"lsiMdio",         0, VXB_BUSID_PLB, 0, lsiMdio0Num,    lsiMdio0Resources},
    {"lsiMac",          0, VXB_BUSID_PLB, 0, lsiMac0Num,     lsiMac0Resources},
#endif /* INCLUDE_AMP_LSI_END_DRIVER || !AMP */

#ifdef INCLUDE_EHCI
    {"vxbPlbUsbEhci",   0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum, vxbPlbUsbEhciDevResources},
#endif /* INCLUDE_EHCI */

#ifdef DRV_PCIBUS_LSI_PCIEX
    {"lsiPciEx",        0, VXB_BUSID_PLB, 0, lsiPciEx0Num, lsiPciEx0Resources},
#endif /* DRV_PCIBUS_LSI_PCIEX */

#ifdef lsiTimer0Num
    { "lsiTimer",  0, VXB_BUSID_PLB, 0, lsiTimer0Num, lsiTimer0Resources },
#endif /* lsiTimer0Num */

#ifdef lsiTimer1Num
    { "lsiTimer",  lsiTimer1Unit, VXB_BUSID_PLB, 0, lsiTimer1Num, lsiTimer1Resources },
#endif /* lsiTimer1Num */

#ifdef lsiTimer2Num
    { "lsiTimer",  lsiTimer2Unit, VXB_BUSID_PLB, 0, lsiTimer2Num, lsiTimer2Resources },
#endif /* lsiTimer2Num */

#ifdef lsiTimer3Num
    { "lsiTimer",  lsiTimer3Unit, VXB_BUSID_PLB, 0, lsiTimer3Num, lsiTimer3Resources },
#endif /* lsiTimer3Num */

#ifdef DRV_SRIO_LSI
    { "lsiSRio",  0, VXB_BUSID_PLB, 0, lsiRio0Num, lsiRio0Resources },
#endif /* DRV_SRIO_LSI */

    {NULL, 0, 0, 0, 0, NULL}
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,   {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,   {(void *)FALSE} },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR, {(void *)usbOpAfterReset}},
    { "vxbPlbUsbEhci", 0, "descBigEndian",   VXB_PARAM_INT32,   { (void *)FALSE } },
    { "vxbPlbUsbEhci", 0, "regBigEndian",    VXB_PARAM_INT32,   { (void *)FALSE } },
#endif /* INCLUDE_EHCI */
    {NULL,           0, NULL,   VXB_PARAM_END_OF_LIST, {(void *) 0}}
};

