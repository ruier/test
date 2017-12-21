/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2008-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,10jun13,y_y  add GTM 64 bit timer support. (WIND00392055)
01f,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01e,21may09,e_d  Remove the sharedMem variable from struct mdio0Resources (WIND00163075) 
01d,07may09,wap  Use the correct flag for enabling RGMII mode in the Marvell
                 PHY (WIND00157835)
01c,13oct08,x_z  Add embedded-TT support for USB. (WIND00134142)
01b,07aug08,l_g  Support END2 versions of the driver and modify usb hook.
01a,03mar08,z_l  Created based on fsl_mpc8360_rdk/hwconf.c
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/mii/mv88E1x11Phy.h>
#include "config.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>

IMPORT STATUS   sysPci1AutoconfigInclude    ();
IMPORT UCHAR    sysPci1AutoconfigIntrAssign (PCI_SYSTEM *,PCI_LOC *,UCHAR);
extern void     sysM8313EhciInit            (void);
extern void     usbOpAfterReset             ();
extern UINT32   vxbSwap32                   (UINT32);

const SIU_INT_MASK quiccIntMask[QUICC_IVEC_MAX] =
    {
    {0x00000000, 0x00000000, 0x00000000}, /* interrupt 0 error - enable nothing */
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
#else  /* !QUICC_ENGINE */
    {0x00000000, 0x00000000, 0x00000000},         /* QE HOGH */
    {0x00000000, 0x00000000, 0x00000000},         /* QE LOW */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved*/
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif /* !QUICC_ENGINE */
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
#else  /* !QUICC_ENGINE */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
#endif /* !QUICC_ENGINE */
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

const UINT32    quiccInumToMaskPattern[QUICC_IVEC_MAX] = 
    {
    0x00000000,         /* interrupt number 0: Error */
    0x00008000,         /* PEX1 CNT */
    0x00004000,         /* PEX2 CNT */
    0x00000000,         /* Reserved */
    0x00001000,         /* MSIR1 */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00000080,         /* UART1 */
    0x00000040,         /* UART2 */
    0x00000020,         /* SEC   */
    0x00000010,         /* ETSEC1 1588 TIMER */
    0x00000008,         /* ETSEC2 1588 TIMER */
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
    0x20000000,         /* TSEC1 Tx */
    0x40000000,         /* TSEC1 Rx */
    0x80000000,         /* TSEC1 Err */
    0x04000000,         /* TSEC2 Tx */
    0x08000000,         /* TSEC2 Rx */
    0x10000000,         /* TSEC2 Err */
    0x02000000,         /* USB2 DR */
    0x01000000,         /* USB2 MPH */
    0x00000000,         /* Reserved */
    0x00000000,         /* Reserved */
    0x00200000,         /* eSDHC */
    0x00000000,         /* Reserved */
    0x00080000,         /* SATA1 */
    0x00040000,         /* SATA2 */
    0x00020000,         /* SATA3 */
    0x00010000,         /* SATA4 */
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
    0x10000000,         /* PCI2 or MSIR0 */
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
    0x00004000,         /* MSIR2 */
    0x00002000,         /* MSIR3 */
    0x00000000,         /* Reserved */
    0x00000800,         /* GTM3 */
    0x00000400,         /* GTM7 */
    0x00000200,         /* MSIR4 */
    0x00000100,         /* MSIR5 */
    0x00000080,         /* MSIR6 */
    0x00000040,         /* MSIR7 */
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

const struct intrCtlrInputs ppcIntCtlrInputs[] = 
    {
    { 0, "quiccIntCtlr",    0, 0 },
    { 1, "ppcDecTimerDev",  0, 0 },
    };

const struct hcfResource ppcIntCtlr0Resources[] = 
    {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE } },
    { "input",          HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
    };
#define ppcIntCtlr0Num  NELEMENTS (ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] = 
    {
    { INUM_UART2,    "ns16550",     1, 0 },
    { INUM_UART1,    "ns16550",     0, 0 },
    { INUM_TSEC1_TX, "motetsec",    0, 0 },
    { INUM_TSEC1_RX, "motetsec",    0, 1 },
    { INUM_TSEC1_ERR,"motetsec",    0, 2 },
    { INUM_TSEC2_TX, "motetsec",    1, 0 },
    { INUM_TSEC2_RX, "motetsec",    1, 1 },
    { INUM_TSEC2_ERR,"motetsec",    1, 2 },
    };

const struct hcfResource quiccIntCtlr0Resources[] = 
    {
    { VXB_REG_BASE,       HCF_RES_INT,  {(void *)(CCSBAR + SIC_REG_BA)}},
    { "input",            HCF_RES_ADDR, {(void *)&quiccIntCtlrInputs[0]}},
    { "inputTableSize",   HCF_RES_INT,  {(void *)NELEMENTS(quiccIntCtlrInputs)}},
    { "intMaskTable",     HCF_RES_ADDR, {(void *)&quiccIntMask[0]}},
    { "intMaskTableSize", HCF_RES_INT,  {(void *) QUICC_IVEC_MAX}},
    { "inumMaskTable",    HCF_RES_ADDR, {(void *)&quiccInumToMaskPattern[0]}},
    { "inumMaskTableSize",HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} }
    };
#define quiccIntCtlr0Num NELEMENTS(quiccIntCtlr0Resources)

#ifdef INCLUDE_DUMMY_MDIO
const struct hcfResource dummyMdio0Resources[] = 
    {
    { "regBase",    HCF_RES_INT,  {(void *)(0)}},
    { "fullDuplex", HCF_RES_INT,  {(void *)1}},
    { "speed",      HCF_RES_INT,  {(void *)1000}}
    };
#   define dummyMdio0Num NELEMENTS(dummyMdio0Resources)
#endif /* INCLUDE_DUMMY_MDIO */

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = 
    {
    { "regBase",   HCF_RES_INT,  {(void *)(CCSBAR + 0x24000)}}
    };
#   define mdio0Num NELEMENTS(mdio0Resources) 
#endif /* INCLUDE_TSEC_MDIO */

const struct hcfResource ppcDecTimerDevResources[] = 
    {
    {VXB_REG_BASE,   HCF_RES_INT,  {(void *)0}},
    {"decClockFreq", HCF_RES_ADDR, {(void *)sysClkFreqGet}},
    {"minClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MIN}},
    {"maxClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MAX}}
    };
#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

/* vxBus resource files for onboard UARTS */

const struct hcfResource ns1655x1Resources[] = 
    {
    { "regBase",     HCF_RES_INT,  {(void *)COM1_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)INUM_UART1} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART1} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = 
    {
    { "regBase",     HCF_RES_INT,  {(void *)COM2_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)INUM_UART2} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)INUM_UART2} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

#ifdef DRV_TIMER_QUICC_PPC
const struct hcfResource generalPurposeTimerResources12[] = 
    {
    {"regBase",      HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE)}},
    {"irq",          HCF_RES_INT,  {(void *)(INUM_GTM2)} },
    {"irqLevel",     HCF_RES_INT,  {(void *)(INUM_GTM2)} },
    {"regBase1",     HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x10)}},
    {"clkRateMin",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    {"clkRateMax",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    {"clkFrequency", HCF_RES_INT,  {(void *)TIMESTAMP_HZ}}
    };

#define generalPurposeTimerNum12   NELEMENTS(generalPurposeTimerResources12)

const struct hcfResource generalPurposeTimerResources34[] = 
    {
    {"regBase",      HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x04)}},
    {"irq",          HCF_RES_INT,  {(void *)(INUM_GTM4)} },
    {"irqLevel",     HCF_RES_INT,  {(void *)(INUM_GTM4)} },
    {"regBase1",     HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE | 0x20)}},
    {"clkRateMin",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    {"clkRateMax",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    {"clkFrequency", HCF_RES_INT,  {(void *)TIMESTAMP_HZ}}
    };

#define generalPurposeTimerNum34   NELEMENTS(generalPurposeTimerResources34)
#endif

/*
 * 64bit GTM timer resource.
 * Note: The parameter restartEnable "MUST" be set to "TRUE" when use GTM 64bit
 * timer as system clock. It could be set to FALSE when use it as AUX or timestamp
 * since timer count continues to increment even reference value is reached.
 */

#ifdef DRV_TIMER64_QUICC_PPC
const struct hcfResource generalPurposeTimerResources[] = 
    {
    {"regBase",      HCF_RES_INT,  {(void *)(CCSBAR | QUICC_GTM1_BASE)}},
    {"irq",          HCF_RES_INT,  {(void *)(INUM_GTM4)} },
    {"irqLevel",     HCF_RES_INT,  {(void *)(INUM_GTM4)} },
    {"restartEnable",HCF_RES_INT,  {(void *)(TRUE)} },
    {"clkRateMin",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MIN}},
    {"clkRateMax",   HCF_RES_INT,  {(void *)AUX_CLK_RATE_MAX}},
    {"clkFrequency", HCF_RES_INT,  {(void *)TIMESTAMP_HZ}}
    };

#define generalPurposeTimerNum   NELEMENTS(generalPurposeTimerResources)
#endif

#ifdef DRV_PCIBUS_M83XX
const struct hcfResource m83xxPci0Resources[] = 
    {
    { "regBase",          HCF_RES_INT, { (void *)(CCSBAR | PCI_REG_BASE) } },
    { "mem32Addr",        HCF_RES_ADDR,{ (void *)PCI_MEM_ADRS } },
    { "mem32Size",        HCF_RES_INT, { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",      HCF_RES_ADDR,{ (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",      HCF_RES_INT, { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",         HCF_RES_ADDR,{ (void *)PCI_IO_ADRS } },
    { "io32Size",         HCF_RES_INT, { (void *)PCI_IO_SIZE } }, 
    { "io16Addr",         HCF_RES_ADDR,{ (void *)PCI_IO_ADRS } },
    { "io16Size",         HCF_RES_INT, { (void *)PCI_IO_SIZE } }, 
    { "fbbEnable",        HCF_RES_INT, { (void *)TRUE } },
    { "cacheSize",        HCF_RES_INT, { (void *)(_CACHE_ALIGN_SIZE / 4) } }, 
    { "maxLatAllSet",     HCF_RES_INT, { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",  HCF_RES_INT, { (void *)TRUE } }, 
    { "includeFuncSet",   HCF_RES_ADDR,{ (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR,{ (void *)sysPci1AutoconfigIntrAssign}},
    { "maxBusSet",        HCF_RES_INT, { (void *)3 } },
    { "pciConfigMechanism",HCF_RES_INT,{ (void *)(0)}},
        
    /* non vxbPciAutoConfig() values */
    
    { "pimmrBase",     HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",    HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize", HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",    HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "owAttrMem",     HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                PCI_PREFETCHABLE      |
                                                PCI_OUT_ATTR_WS_128M) } },
    { "owAttrMemIo",   HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                PCI_OUT_ATTR_WS_64M) } },
    { "owAttrIo",      HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                PCI_ATTR_IO_BIT       |
                                                PCI_OUT_ATTR_WS_64M) } },
    { "iwAttr",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT  |
                                                PCI_PREFETCHABLE               |
                                                PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                PCI_IN_ATTR_WS_256M) } },
    { "lawbar",        HCF_RES_INT,  { (void *)(M83XX_PCILAWBARn(CCSBAR,0))}},
    { "lawar",         HCF_RES_INT,  { (void *)(M83XX_PCILAWARn(CCSBAR,0))}},
    { "lawarAttr",     HCF_RES_INT,  { (void *)(PCI_LAWAR_ENABLE |
                                                PCI_LAWAR_SIZE_256MB)}},
    { "autoConfig",    HCF_RES_INT,  { (void *)(TRUE)}},
};

#   define m83xxPci0Num NELEMENTS(m83xxPci0Resources)
#endif /* DRV_PCIBUS_M83XX  */

#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)

const struct hcfResource motEtsecHEnd0Resources[] = 
    {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",      HCF_RES_INT,    { (void *)INUM_TSEC1_TX} },
    { "intr0Level", HCF_RES_INT,    { (void *)INUM_TSEC1_TX } },
    { "intr1",      HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr1Level", HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr2",      HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "intr2Level", HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0 }},
    { "miiIfName",  HCF_RES_STRING, { (void *)"dummyMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
    };
#define etsecHEnd0Num NELEMENTS(motEtsecHEnd0Resources)

const struct hcfResource motEtsecHEnd1Resources[] = 
    {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",      HCF_RES_INT,    { (void *)INUM_TSEC2_TX} },
    { "intr0Level", HCF_RES_INT,    { (void *)INUM_TSEC2_TX } },
    { "intr1",      HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr1Level", HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr2",      HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "intr2Level", HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "phyAddr",    HCF_RES_INT,    { (void *)4 }},
    { "miiIfName",  HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
    };
#define etsecHEnd1Num NELEMENTS(motEtsecHEnd1Resources)

#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]=
    {
    {"regBase",  HCF_RES_INT,  {(void *)M83XX_CAPLENGTH(CCSBAR)} },
    {"irq",      HCF_RES_INT,  {(void *)(INUM_USB2_DR)} },
    {"irqLevel", HCF_RES_INT,  {(void *)(INUM_USB2_DR)} },
    {"ehciInit", HCF_RES_ADDR, {(void *)sysM8313EhciInit} },
    {"dataSwap", HCF_RES_ADDR, {(void *)vxbSwap32}},
    };
#   define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)
#endif /* INCLUDE_EHCI */

const struct hcfDevice hcfDeviceList[] = 
    {
    { "quiccIntCtlr",   0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,  quiccIntCtlr0Resources },
    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,    ppcIntCtlr0Resources },
        
#ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio",      0, VXB_BUSID_PLB, 0, dummyMdio0Num,     dummyMdio0Resources },
#endif /* INCLUDE_DUMMY_MDIO */
#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio",       0, VXB_BUSID_PLB, 0, mdio0Num,          mdio0Resources },
#endif /* INCLUDE_TSEC_MDIO */

#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
    { "motetsec",       0, VXB_BUSID_PLB, 0, etsecHEnd0Num,     motEtsecHEnd0Resources },
    { "motetsec",       1, VXB_BUSID_PLB, 0, etsecHEnd1Num,     motEtsecHEnd1Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum, ppcDecTimerDevResources },
        
#ifdef DRV_TIMER_QUICC_PPC        
    { "quiccTimerDev",  0, VXB_BUSID_PLB, 0, generalPurposeTimerNum12, generalPurposeTimerResources12},
    { "quiccTimerDev",  1, VXB_BUSID_PLB, 0, generalPurposeTimerNum34, generalPurposeTimerResources34},
#endif /* DRV_TIMER_QUICC_PPC */

#ifdef DRV_TIMER64_QUICC_PPC 
    { "quiccTimer64Dev",  0, VXB_BUSID_PLB, 0, generalPurposeTimerNum, generalPurposeTimerResources},
#endif /* DRV_TIMER64_QUICC_PPC */

    { "ns16550",        0, VXB_BUSID_PLB, 0, ns1655x1Num,       ns1655x1Resources },
    { "ns16550",        1, VXB_BUSID_PLB, 0, ns1655x2Num,       ns1655x2Resources },

#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci",       0, VXB_BUSID_PLB, 0, m83xxPci0Num,      m83xxPci0Resources },
#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum,      vxbPlbUsbEhciDevResources},
#endif /* INCLUDE_USB */
    };

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);
    
VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { "mv88E1x11Phy",   0,  "mode", VXB_PARAM_INT32,        {(void*)MV88E_MODE_RGMII}},
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,       {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,       {(void *)FALSE} },
    { "vxbPlbUsbEhci",  0, "postResetHook",   VXB_PARAM_FUNCPTR,    {(void *)usbOpAfterReset} },
#endif
    { NULL,             0,  NULL,   VXB_PARAM_END_OF_LIST,  {(void *)0} }
    };
