/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2006-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------
01m,12mar12,sye  Correct resource type of general purpose timer. (WIND00278669)
01l,10dec10,y_y  fix miss pci macro error. (WIND00245940)
01k,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01j,24jul08,z_l  VxWorks 6.7 update.
01i,14may08,wap  Switch to VxBus TSEC driver
01h,29oct07,h_k  fixed conflict entries for quiccTimerDev. (CQ:101100)
01g,19oct07,ami  USB Support added (Defect WIND00108412)
01f,05sep07,dtr  WIND0092393 - Fix GTM masks.
01e,20jul07,pdg  added m83xx pci support
01d,13jul07,dtr  Update for vxbus interrupt controller driver.
01c,20jun07,sup  serail driver vxBus compliant
01b,12oct06,dtr  Add motTsecHEnd resources.
01a,10oct06,sup  written

*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <mii/mv88E1x11Phy.h>
#include <busCtlr/m83xxPci.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>

#ifdef DRV_PCIBUS_M83XX
extern STATUS sysPciAutoConfigInclude (PCI_SYSTEM * pSys, 
                                               PCI_LOC * pLoc,
                                               UINT devVend);
extern UCHAR sysPciAutoConfigIntrAssign (PCI_SYSTEM * pSys,
                                                  PCI_LOC * pLoc,
                                                  UCHAR pin);

#define PCI_LAWBAR0 0x60
#define PCI_LAWAR0  0x64
#define PCI_LAWBAR1 0x68
#define PCI_LAWAR1  0x6C

#endif /* DRV_PCIBUS_M83XX */

#define IRQ_NUMBER(intNum) ((VOIDFUNCPTR *)intNum)

/* forward declarations */

#if defined (INCLUDE_USB) && defined (INCLUDE_EHCI)

/* import to do the BSP level initialization */

extern void sysM834xEhciInit(void);
extern UINT32 vxbSwap32 (UINT32 data);
IMPORT void usbOpAfterReset ();

#define EHCI_M834x_INT   39

#endif /* defined (INCLUDE_USB) & ... */

/* This can be modified as per user manual for chip to prioritize devices */

const SIU_INT_MASK quiccIntMask[QUICC_IVEC_MAX] = {
    {0x00000000, 0x00000000, 0x00000000},   /* interrupt 0 error - enable nothing */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* UART1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* UART2 */
    {0xFF000000, 0x00000000, 0x00000000},         /* SEC   */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* I2C1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* I2C2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* SPI */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ3 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ4 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ5 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ6 */
    {0xFF000020, 0x30000000, 0x00000000},         /* IRQ7 **23**/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#ifndef QUICC_ENGINE
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Err */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Err */
#else
    {0x00000000, 0x00000000, 0x00000000},         /* QE HOGH */
    {0x00000000, 0x00000000, 0x00000000},         /* QE LOW */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved*/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif
    {0xFC000020, 0x30000000, 0x00000000},         /* USB2 DR */
    {0xFC000020, 0x30000000, 0x00000000},         /* USB2 MPH */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* IRQ0 ***48**/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC SEC */
    {0xFF000020, 0x30000000, 0x00000000},         /* PIT */
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI1 */
#ifndef QUICC_ENGINE
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI2 */
#else
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif
    {0xFF000020, 0x30000000, 0x00000000},         /* RTC ALR */
    {0xFF000020, 0x30000000, 0x00000000},         /* MU */
    {0xFF000020, 0x30000000, 0x00000000},         /* SBA */
    {0xFF000020, 0x30000000, 0x00000000},         /* DMA */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM4 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM8 */
    {0xFF000020, 0x30000000, 0x00000000},         /* CPIO1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GPIO2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* DDR */
    {0xFF000020, 0x30000000, 0x00000000},         /* LBC */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM2 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM6 */
    {0xFF000020, 0x30000000, 0x00000000},         /* PMC */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM3 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM7 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM1 */
    {0xFF000020, 0x30000000, 0x00000000},         /* GTM5 */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000}          /* Reserved */
    };

/* Specific to CPU  */

const UINT32    quiccInumToMaskPattern[QUICC_IVEC_MAX] = {
    0x00000000,         /* interrupt number 0: Error */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000080,         /* UART1 */
    0x00000040,         /* UART2 */
    0x00000020,         /* SEC   */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000004,         /* I2C1 */
    0x00000002,         /* I2C2 */
    0x00000001,         /* SPI */
    0x40000000,         /* IRQ1 */
    0x20000000,         /* IRQ2 */
    0x10000000,         /* IRQ3 */
    0x08000000,         /* IRQ4 */
    0x04000000,         /* IRQ5 */
    0x02000000,         /* IRQ6 */
    0x01000000,         /* IRQ7 **23**/
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* TSEC1 Tx */
    0x40000000,         /* TSEC1 Rx */
    0x20000000,         /* TSEC1 Err */
    0x10000000,         /* TSEC2 Tx */
    0x08000000,         /* TSEC2 Rx */
    0x04000000,         /* TSEC2 Err */
    0x02000000,         /* USB2 DR */
    0x01000000,         /* USB2 MPH */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* IRQ0 ***48**/
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x80000000,         /* RTC SEC */
    0x40000000,         /* PIT */
    0x20000000,         /* PCI1 */
    0x10000000,         /* PCI2 */
    0x08000000,         /* RTC ALR */
    0x04000000,         /* MU */
    0x02000000,         /* SBA */
    0x01000000,         /* DMA */
    0x00800000,         /* GTM4 */
    0x00400000,         /* GTM8 */
    0x00200000,         /* CPIO1 */
    0x00100000,         /* GPIO2 */
    0x00080000,         /* DDR */
    0x00040000,         /* LBC */
    0x00020000,         /* GTM2 */
    0x00010000,         /* GTM6 */
    0x00008000,         /* PMC */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000800,         /* GTM3 */
    0x00000400,         /* GTM7 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000020,         /* GTM1 */
    0x00000010,         /* GTM5 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000          /* Reserved */
    };

/*
 * On the ads834x boards, PHYs are physically wired according
 * to the following table:
 *
 *              Data port pins  Management port pins    MII address
 *              --------------  --------------------    -----------
 * PHY0:        TSEC0           TSEC0                   0
 * PHY1:        TSEC1           TSEC0                   1
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "mottsec0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 0 for TSEC0
 * and 1 for TSEC1.
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)INUM_TSEC1_TX } },
    { "intr0Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_TX } },
    { "intr1",          HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr1Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr2",          HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "intr2Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    };
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",          HCF_RES_INT,    { (void *)INUM_TSEC2_TX } },
    { "intr0Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_TX } },
    { "intr1",          HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr1Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr2",          HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "intr2Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "phyAddr",        HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    };
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

struct hcfResource ppcDecTimerResources[] =  {
    { "regBase",      HCF_RES_INT,  {(void *)0}},
    { "minClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MIN}},
    { "maxClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MAX}},
    { "decClockFreq", HCF_RES_ADDR, {(void *)sysClkFreqGet}}
    };

#define ppcDecTimerNum         NELEMENTS(ppcDecTimerResources)

LOCAL const struct hcfResource generalPurposeTimerResources12[] =  {
    { "regBase",      HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE)}},
    { "regBase1",     HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x10)}},
    { "irq",          HCF_RES_INT,  {(void *)(IRQ_NUMBER(INUM_GTM2))}},
    { "irqLevel",     HCF_RES_INT,  {(void *)(INUM_GTM2)}},
    { "clkFrequency", HCF_RES_INT,  {(void *)SYS_CLK_FREQ}},
    { "clkRateMin",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    { "clkRateMax",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    };

#define generalPurposeTimerNum12       NELEMENTS(generalPurposeTimerResources12)

LOCAL const struct hcfResource generalPurposeTimerResources34[] =  {
    { "regBase",      HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x04)}},
    { "regBase1",     HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x20)}},
    { "irq",          HCF_RES_INT,  {(void *)(IRQ_NUMBER(INUM_GTM4))}},
    { "irqLevel",     HCF_RES_INT,  {(void *)(INUM_GTM4)}},
    { "clkFrequency", HCF_RES_INT,  {(void *)SYS_CLK_FREQ}},
    { "clkRateMin",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    { "clkRateMax",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    };

#define generalPurposeTimerNum34       NELEMENTS(generalPurposeTimerResources34)

struct hcfResource ns16652channel1Resources[] = {
    { "regBase",     HCF_RES_INT,  {(void *)COM1_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)INUM_UART1} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART1} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)sysClkFreqGet} }
    };

#define ns16552Channel1Num NELEMENTS(ns16652channel1Resources)

struct hcfResource ns16652channel2Resources[] = {
    { "regBase",     HCF_RES_INT,  {(void *)COM2_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)(IRQ_NUMBER(INUM_UART2))} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART2} }, 
    { "clkFreq",     HCF_RES_ADDR, {(void *)sysClkFreqGet} }
    };          

#define ns16552Channel2Num NELEMENTS(ns16652channel2Resources)

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "quiccIntCtlr", 0, 0 },
    { 1, "ppcDecTimerDev", 0, 0 },
    };


const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE} },
    { "input",          HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
    };
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] = {
    { INUM_UART2,     "ns16550", 1, 0 },
    { INUM_UART1,     "ns16550", 0, 0 },
    { INUM_TSEC1_TX,  "mottsec", 0, 0 },
    { INUM_TSEC1_RX,  "mottsec", 0, 1 },
    { INUM_TSEC1_ERR, "mottsec", 0, 2 },
    { INUM_TSEC2_TX,  "mottsec", 1, 0 },
    { INUM_TSEC2_RX,  "mottsec", 1, 1 },
    { INUM_TSEC2_ERR, "mottsec", 1, 2 },
    { INUM_GTM2,      "quiccTimerDev", 0, 0 },
    { INUM_GTM4,      "quiccTimerDev", 1, 0 }
    };

const struct hcfResource quiccIntCtlr0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  {(void *)(CCSBAR + SIC_REG_BA) } },
    { "input",             HCF_RES_ADDR, {(void *)&quiccIntCtlrInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  {(void *)NELEMENTS(quiccIntCtlrInputs) }},
    { "intMaskTable",      HCF_RES_ADDR, {(void *)&quiccIntMask[0]} },
    { "intMaskTableSize",  HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} },
    { "inumMaskTable",     HCF_RES_ADDR, {(void *)&quiccInumToMaskPattern[0]} },
    { "inumMaskTableSize", HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} }
    };
#define quiccIntCtlr0Num NELEMENTS(quiccIntCtlr0Resources)

#ifdef DRV_PCIBUS_M83XX
const struct hcfResource m83xxPci0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)(CCSBAR + 0x8000) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciAutoConfigIntrAssign }},
        
     /* non vxbPciAutoConfig() values */
    
    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS} },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)2 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },

    /* Window Attributes - Defaults to 8540 type if none given */
    
    { "owAttrMem",         HCF_RES_INT,   { (void *)(0xa00f8000) } },
    { "owAttrMemIo",       HCF_RES_INT,   { (void *)(0xa00fc000) } },
    { "owAttrIo",          HCF_RES_INT,   { (void *)(0xc00fc000) } },
    { "iwAttr",            HCF_RES_INT,   { (void *)(M83XXPCI_IB_WINDOW_ENABLE_BIT    | 
                                                     M83XXPCI_IB_ATTR_RTT_WRITE_SNOOP |
                                                     M83XXPCI_IB_ATTR_RTT_READ_SNOOP  | 
                                                     M83XXPCI_IB_ATTR_IWS_256M) } },
    { "lawbar",            HCF_RES_INT,   { (void *)(M83XXPCI_LAWBAR0)}},
    { "lawar",             HCF_RES_INT,   { (void *)(M83XXPCI_LAWAR0)}},
    { "lawarAttr",         HCF_RES_INT,   { (void *)(LAWAR_ENABLE | LAWAR_SIZE_256MB) } },
    { "pciExpressHost",    HCF_RES_INT,   { (void *)FALSE } }
    };

#define m83xxPci0Num NELEMENTS (m83xxPci0Resources)

#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[] = {
    { "regBase",  HCF_RES_INT,  {(void *)M83XX_CAPLENGTH(CCSBAR)} },
    { "irq",      HCF_RES_INT,  {(void *)(EHCI_M834x_INT)} },
    { "irqLevel", HCF_RES_INT,  {(void *)(EHCI_M834x_INT)} },
    { "ehciInit", HCF_RES_ADDR, {(void *)sysM834xEhciInit} },
    { "dataSwap", HCF_RES_ADDR, {(void *)vxbSwap32} },
    };

#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)

#endif /* INCLUDE_EHCI */

const struct hcfDevice hcfDeviceList[] = {
    { "quiccIntCtlr",   0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,   quiccIntCtlr0Resources },
    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,     ppcIntCtlr0Resources },
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerNum,     ppcDecTimerResources },
    { "ns16550",        0, VXB_BUSID_PLB, 0, ns16552Channel1Num, ns16652channel1Resources },
    { "ns16550",        1, VXB_BUSID_PLB, 0, ns16552Channel2Num, ns16652channel2Resources },

#ifdef DRV_TIMER_QUICC_PPC
    { "quiccTimerDev",  0, VXB_BUSID_PLB, 0, generalPurposeTimerNum12, generalPurposeTimerResources12 },
    { "quiccTimerDev",  1, VXB_BUSID_PLB, 0, generalPurposeTimerNum34, generalPurposeTimerResources34 },
#endif /* DRV_TIMER_QUICC_PPC */

#if (defined(INCLUDE_TSEC_VXB_END2) || defined(INCLUDE_TSEC_VXB_END) )
    { "mottsec",        0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { "mottsec",        1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
#endif /* INCLUDE_TSEC_VXB_END2 & ... */

#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci",       0, VXB_BUSID_PLB, 0, m83xxPci0Num, m83xxPci0Resources },
#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum, vxbPlbUsbEhciDevResources },
#endif /* INCLUDE_EHCI */

   };
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { "mv88E1x11Phy",0,"mode", VXB_PARAM_INT32, {(void*)MV88E_MODE_GMII}},
    { "mv88E1x11Phy",1,"mode", VXB_PARAM_INT32, {(void*)MV88E_MODE_GMII}},   
#if defined (INCLUDE_USB) && defined (INCLUDE_EHCI) 
    { "vxbPlbUsbEhci",0,"hasEmbeddedTT", VXB_PARAM_INT32,{(void *)TRUE}},
    { "vxbPlbUsbEhci",0,"fixupPortNumber", VXB_PARAM_INT32, {(void *)FALSE}},
    { "vxbPlbUsbEhci",0,"postResetHook", VXB_PARAM_FUNCPTR, {(void *)usbOpAfterReset}},
#endif /* INCLUDE_USB & ... */
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

