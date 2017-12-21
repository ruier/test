/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20aug08,wap  add VxBus PCI support via virtual/legacy PCI driver
01a,16apr08,b_m  created.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>

#include "config.h"

/* externs */

IMPORT UINT32 sysGetUartClockFreq(void);
IMPORT UINT32 sysGetDecClockFreq(void);
IMPORT UINT32 sysGetGptClockFreq(void);

/* device resources */

const struct hcfResource m5xxxPscSioResources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)PSC1_BASE_ADRS } },
    { "irq",      HCF_RES_INT,  { (void *)INUM_PSC1 } },
    { "irqLevel", HCF_RES_INT,  { (void *)IV_PSC1 } },
    { "clkFreq",  HCF_RES_ADDR, { (void *)sysGetUartClockFreq } },
};
#define m5xxxPscSioNum  NELEMENTS(m5xxxPscSioResources)

const struct hcfResource ppcDecTimerResources[] = {
    { "regBase",      HCF_RES_INT,  { (void *)0 } },
    { "minClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "maxClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "decClockFreq", HCF_RES_ADDR, { (void *)sysGetDecClockFreq } },
};
#define ppcDecTimerNum  NELEMENTS(ppcDecTimerResources)

#ifdef DRV_TIMER_M5XXX_GPT
const struct hcfResource m5xxxGPTimerResources[] = {
    { "regBase",    HCF_RES_INT,  { (void *)GPT_BASE(0) } },
    { "irq",        HCF_RES_INT,  { (void *)INUM_TMR0 } },
    { "irqLevel",   HCF_RES_INT,  { (void *)IV_TMR0 } },
    { "clkRateMin", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "clkRateMax", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "clkFreq",    HCF_RES_ADDR, { (void *)sysGetGptClockFreq } },
};
#define m5xxxGPTimerNum NELEMENTS(m5xxxGPTimerResources)
#endif

#ifdef INCLUDE_USB
const struct hcfResource vxbPlbUsbOhciResources[]= {
    { "regBase",  HCF_RES_INT, { (void *)USB_BASE_ADRS } },
    { "irq",      HCF_RES_INT, { (void *)INUM_USB } },
    { "irqLevel", HCF_RES_INT, { (void *)IV_USB } },
};
#define vxbPlbUsbOhciNum    NELEMENTS(vxbPlbUsbOhciResources)
#endif

#ifdef INCLUDE_PCI
void * sysPciVirtToBus (void *);
void * sysPciBusToVirt (void *);
STATUS sysPciIntConnect (VOIDFUNCPTR *, VOIDFUNCPTR, int);
STATUS sysPciIntDisconnect (VOIDFUNCPTR *, VOIDFUNCPTR, int);

void sysPciIntEnable (int);
void sysPciIntDisable (int);

LOCAL const struct hcfResource pci0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)0 } },
    { "ioStart", HCF_RES_ADDR, { (void *)CPU_PCI_IO_ADRS } },
    { "intConnect", HCF_RES_ADDR, { (void *)sysPciIntConnect } },
    { "intDisconnect", HCF_RES_ADDR, { (void *)sysPciIntDisconnect } },
    { "intEnable", HCF_RES_ADDR, { (void *)sysPciIntEnable } },
    { "intDisable", HCF_RES_ADDR, { (void *)sysPciIntDisable } },
    { "virtToBus", HCF_RES_ADDR, { (void *)sysPciVirtToBus } },
    { "busToVirt", HCF_RES_ADDR, { (void *)sysPciBusToVirt } },
};
#define pci0Num NELEMENTS(pci0Resources)
#endif

const struct hcfDevice hcfDeviceList[] = {
#ifdef INCLUDE_PCI
    { "vxbVirtualPciAdaptor", 0, VXB_BUSID_PLB, 0, pci0Num, pci0Resources },
#endif
    { "m5xxxPscSio", 0, VXB_BUSID_PLB, 0, m5xxxPscSioNum, m5xxxPscSioResources },
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerNum, ppcDecTimerResources },
#ifdef DRV_TIMER_M5XXX_GPT
    { "m5xxxGPTimerDev", 0, VXB_BUSID_PLB, 0, m5xxxGPTimerNum, m5xxxGPTimerResources},
#endif
#ifdef INCLUDE_USB
    { "vxbPlbUsbOhci", 0, VXB_BUSID_PLB, 0, vxbPlbUsbOhciNum, vxbPlbUsbOhciResources },
#endif
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);


VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
