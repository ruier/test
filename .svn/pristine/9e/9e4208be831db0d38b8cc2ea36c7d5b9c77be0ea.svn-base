/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2007-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,03mar10,x_z  update for new PCIe driver.
01h,11jan10,x_z  code clean and add ppcDecTimerDev and quiccTimerDev.
01g,23oct08,syt  added 'regBase1' in m83xxPciExResources.(WIND00131698)
01f,13oct08,x_z  Add embedded-TT support for USB. (WIND00134142)
01e,18aug08,x_s  removed sysDuart.h file.
01d,08jul08,x_s  replaced MV_HWCFG_RGMII_TO_COPPER with MV88E_MODE_RGMII_FIXUP.
01c,20feb08,x_s  added SATA support and updated vxbus dirver: interrupt controller and USB.
01b,23jan08,x_s  fixed PCI bus controller driver wrong address of lawbar and lawar registers.
01a,03aug07,dtr  written.
*/

#include <vxWorks.h>
#include "config.h"
#include <../src/hwif/h/mii/mv88E1x11Phy.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>

#ifdef DRV_PCIBUS_M83XX_EX
IMPORT STATUS sysPciEx1AutoconfigInclude ();
IMPORT UCHAR  sysPciEx1AutoconfigIntrAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS sysPciEx2AutoconfigInclude ();
IMPORT UCHAR  sysPciEx2AutoconfigIntrAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef DRV_PCIBUS_M83XX
IMPORT STATUS sysPciAutoconfigInclude ();
IMPORT UCHAR  sysPciAutoconfigIntrAssign (PCI_SYSTEM *,PCI_LOC *, UCHAR);

#   define MPC83XX_PCIE_MEMORY_OWATTR   (PCI_WINDOW_ENABLE_BIT          | \
                                         PCI_PREFETCHABLE               | \
                                         PCI_OUT_ATTR_WS_128M)
#   define MPC83XX_PCIE_MEMORYIO_OWATTR (PCI_WINDOW_ENABLE_BIT          | \
                                         PCI_OUT_ATTR_WS_128M)
#   define MPC83XX_PCIE_IO_OWATTR       (PCI_WINDOW_ENABLE_BIT          | \
                                         PCI_ATTR_IO_BIT                | \
                                         PCI_OUT_ATTR_WS_256M)
#   define MPC83XX_PCIE_IWATTR          (PCI_WINDOW_ENABLE_BIT          | \
                                         PCI_PREFETCHABLE               | \
                                         PCI_IN_ATTR_RTT_LM_READ_SNOOP  | \
                                         PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | \
                                         PCI_IN_ATTR_WS_512M)
#endif /* DRV_PCIBUS_M83XX */

#if defined (INCLUDE_USB) && defined (INCLUDE_EHCI)

/* import to do the BSP level initialization */

extern void   sysM837xEhciInit(void);
extern void   usbOpAfterReset();
extern UINT32 vxbSwap32 (UINT32);
#endif /* defined (INCLUDE_USB) && defined (INCLUDE_EHCI) */

/*
 * For a give inum, return a pair of masks, one for SIMR_H and one for SIMR_L,
 * which enables only higher priority interrupts.
 */

LOCAL SIU_INT_MASK enableHighPriInts[] = {
    {0x00000000, 0x00000000, 0x00000000},         /* Error (no interrupt) */
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
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC1 Err */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Tx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Rx */
    {0x00000000, 0x00000000, 0x00000000},         /* TSEC2 Err */
    {0xFC000020, 0x30000000, 0x00000000},         /* USB DR */
    {0x00000000, 0x00000000, 0x00000000},         /* Reserved */
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
    {0xFF000020, 0x00000000, 0x00000000},         /* PCI */
    {0xFF000020, 0x00000000, 0x00000000},         /* MSIR0 */
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

LOCAL UINT32        iNumToMaskPattern[] = {
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
    0x80000000,         /* TSEC1 Tx */
    0x40000000,         /* TSEC1 Rx */
    0x20000000,         /* TSEC1 Err */
    0x10000000,         /* TSEC2 Tx */
    0x08000000,         /* TSEC2 Rx */
    0x04000000,         /* TSEC2 Err */
    0x02000000,         /* USB DR */
    0x00000000,         /* Reserved */
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
    0x20000000,         /* PCI */
    0x10000000,         /* MSIR0 */
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

#ifdef DRV_PCIBUS_M83XX_EX
const struct hcfResource m83xxPciEx0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) (CCSBAR + PCIEX1_REG_BASE) } },
    { "regBase1",           HCF_RES_INT,  { (void *) CPU_PCIEX1_CFG_BASE } },
    { "cfgBase",            HCF_RES_ADDR, { (void *) CPU_PCIEX1_CFG_BASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *) PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *) PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *) PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *) PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *) PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *) PCIEX1_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *) PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *) PCIEX1_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *) TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *) (_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *) PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *) TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *) sysPciEx1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx1AutoconfigIntrAssign }},
    { "maxBusSet",          HCF_RES_INT,  { (void *) PCIEX1_MAX_BUS } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *) 0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *) PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *) LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *) LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *) PCI_MSTR_MEM_BUS } },
    { "autoConfig",         HCF_RES_INT,  { (void *) (TRUE)}},
    { "lawbar",             HCF_RES_INT,  { (void *) (M83XX_PCIEXLAWBARn (0)) } },
    { "lawar",              HCF_RES_INT,  { (void *) (M83XX_PCIEXLAWARn (0)) } },
    { "lawarAttr",          HCF_RES_INT,  { (void *) (PCI_LAWAR_ENABLE | PCI_LAWAR_SIZE_512MB) } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *) (TRUE) } },
    { "msiEnable",          HCF_RES_INT,  { (void *) (FALSE) } },
};

#define m83xxPciEx0Num NELEMENTS(m83xxPciEx0Resources)

const struct hcfResource m83xxPciEx1Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) (CCSBAR + PCIEX2_REG_BASE) } },
    { "regBase1",           HCF_RES_INT,  { (void *) CPU_PCIEX2_CFG_BASE } },
    { "cfgBase",            HCF_RES_ADDR, { (void *) CPU_PCIEX2_CFG_BASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *) PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *) PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *) PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *) PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *) PCIEX2_IO_ADRS} },
    { "io32Size",           HCF_RES_INT,  { (void *) PCIEX2_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *) PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *) PCIEX2_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *) TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *) (_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *) PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *) TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *) sysPciEx2AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *) sysPciEx2AutoconfigIntrAssign } },
    { "maxBusSet",          HCF_RES_INT,  { (void *) PCIEX2_MAX_BUS} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *) 0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *) PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *) LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *) LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *) PCI_MSTR_MEM_BUS } },
    { "lawbar",             HCF_RES_INT,  { (void *) (M83XX_PCIEXLAWBARn (1)) } },
    { "lawar",              HCF_RES_INT,  { (void *) (M83XX_PCIEXLAWARn (1)) } },
    { "lawarAttr",          HCF_RES_INT,  { (void *) (PCI_LAWAR_ENABLE | PCI_LAWAR_SIZE_512MB) } },
    { "autoConfig",         HCF_RES_INT,  { (void *) (TRUE) } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *) (TRUE) } },
    { "msiEnable",          HCF_RES_INT,  { (void *) (FALSE) } }
};

#define m83xxPciEx1Num NELEMENTS(m83xxPciEx1Resources)
#endif /* DRV_PCIBUS_M83XX_EX */

#ifdef DRV_PCIBUS_M83XX
const struct hcfResource m83xxPci0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *) (CCSBAR + PCI_REG_BASE) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *) PCI_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *) PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *) PCI_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *) PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *) PCI_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *) PCI_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *) PCI_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *) PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *) TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *) (_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *) PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *) TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *) sysPciAutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *) sysPciAutoconfigIntrAssign } },
    { "maxBusSet",          HCF_RES_INT,  { (void *) 3 } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *) 0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *) PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *) LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *) LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *) PCI_MSTR_MEM_BUS } },
    { "owAttrMem",          HCF_RES_INT,  { (void *) MPC83XX_PCIE_MEMORY_OWATTR } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *) MPC83XX_PCIE_MEMORYIO_OWATTR } },
    { "owAttrIo",           HCF_RES_INT,  { (void *) MPC83XX_PCIE_IO_OWATTR } },
    { "iwAttr",             HCF_RES_INT,  { (void *) MPC83XX_PCIE_IWATTR } },
    { "lawbar",             HCF_RES_INT,  { (void *) (M83XX_PCILAWBARn (0)) } },
    { "lawar",              HCF_RES_INT,  { (void *) (M83XX_PCILAWARn (0)) } },
    { "lawarAttr",          HCF_RES_INT,  { (void *) (PCI_LAWAR_ENABLE | PCI_LAWAR_SIZE_512MB) } },
    { "autoConfig",         HCF_RES_INT,  { (void *) (TRUE) } },
};

#define m83xxPci0Num NELEMENTS(m83xxPci0Resources)
#endif /* DRV_PCIBUS_M83XX */

/*
 * On the mds837x board, PHYs are physically wired according to the following
 * table when RGMII interfaces are used for 2 ETSECs:
 *
 *          Data port pins  Management port pins    MII address
 *          --------------  --------------------    -----------
 * PHY0:    TSEC0           TSEC0                   2
 * PHY1:    TSEC1           TSEC0                   3
 *
 * The tricky part is that all PHYs have their management pins
 * connected to TSEC0.
 */

const struct hcfResource motEtsecEnd0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *) ETSEC1_REG_BA } },
    { "phyAddr",    HCF_RES_INT,    { (void *) 2 } },
    { "miiIfName",  HCF_RES_STRING, { (void *) "motetsec" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *) 0 } }
};
#define etsecEnd0Num NELEMENTS(motEtsecEnd0Resources)

const struct hcfResource motEtsecEnd1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *) ETSEC2_REG_BA } },
    { "phyAddr",    HCF_RES_INT,    { (void *) 3 } },
    { "miiIfName",  HCF_RES_STRING, { (void *) "motetsec" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *) 0 } }
};
#define etsecEnd1Num NELEMENTS(motEtsecEnd1Resources)

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,                "quiccIntCtlr",    0, 0 },
    { 1,                "ppcDecTimerDev",  0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *) TRUE} },
    { "input",          HCF_RES_ADDR, {(void *) &ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *) NELEMENTS(ppcIntCtlrInputs) } }
};

#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

struct intrCtlrInputs quiccIntCtlrInputs[] = {
    { INUM_UART1,       "ns16550",  0, 0 },
    { INUM_UART2,       "ns16550",  1, 0 },
    { INUM_TSEC1_TX,    "motetsec", 0, 0 },
    { INUM_TSEC1_RX,    "motetsec", 0, 1 },
    { INUM_TSEC1_ERR,   "motetsec", 0, 2 },
    { INUM_TSEC2_TX,    "motetsec", 1, 0 },
    { INUM_TSEC2_RX,    "motetsec", 1, 1 },
    { INUM_TSEC2_ERR,   "motetsec", 1, 2 }
};

const struct hcfResource quiccIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  {(void *) SIC_REG_BA } },
    { "input",              HCF_RES_ADDR, {(void *) &quiccIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  {(void *) NELEMENTS(quiccIntCtlrInputs) } },
    { "intMaskTable",       HCF_RES_ADDR, {(void *) &enableHighPriInts[0]} },
    { "intMaskTableSize",   HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} },
    { "inumMaskTable",      HCF_RES_ADDR, {(void *) &iNumToMaskPattern[0] } },
    { "inumMaskTableSize",  HCF_RES_INT,  {(void *) QUICC_IVEC_MAX} }
};

#define quiccIntCtlr0Num NELEMENTS(quiccIntCtlr0Resources)

/* vxBus resource files for onboard UARTS */

const struct hcfResource ns1655x1Resources[] ={
    { "regBase",     HCF_RES_INT,  { (void *) DUART1_REG_BA} },
    { "regInterval", HCF_RES_INT,  { (void *) DUART_REG_ADDR_INTERVAL} },
    { "clkFreq",     HCF_RES_ADDR, { (void *) &sysClkFreqGet} }
};

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] ={
    { "regBase",     HCF_RES_INT,  { (void *) DUART2_REG_BA} },
    { "regInterval", HCF_RES_INT,  { (void *) DUART_REG_ADDR_INTERVAL} },
    { "clkFreq",     HCF_RES_ADDR, { (void *) &sysClkFreqGet} }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ppcDecTimerDevResources[] = {
    {VXB_REG_BASE,        HCF_RES_INT,  { (void *) 0 } },
    {"decClockFreq",      HCF_RES_ADDR, { (void *) sysClkFreqGet } },
    {"minClkRate",        HCF_RES_INT,  { (void *) SYS_CLK_RATE_MIN } },
    {"maxClkRate",        HCF_RES_INT,  { (void *) SYS_CLK_RATE_MAX } }
};
#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

const struct hcfResource generalPurposeTimerResources12[] = {
    {"regBase",           HCF_RES_INT,  { (void *) QUICC_GTM1_GTCFR1 } },
    {"irq",               HCF_RES_INT,  { (void *) INUM_GTM2 } },
    {"irqLevel",          HCF_RES_INT,  { (void *) INUM_GTM2 } },
    {"regBase1",          HCF_RES_INT,  { (void *) QUICC_GTM1_GTMDR1} },
    {"clkRateMin",        HCF_RES_INT,  { (void *) AUX_CLK_RATE_MIN } },
    {"clkRateMax",        HCF_RES_INT,  { (void *) AUX_CLK_RATE_MAX } },
    {"clkFrequency",      HCF_RES_INT,  { (void *) SYS_CLK_FREQ } }
};

#define generalPurposeTimerNum12   NELEMENTS(generalPurposeTimerResources12)

const struct hcfResource generalPurposeTimerResources34[] = {
    {"regBase",           HCF_RES_INT,  { (void *) QUICC_GTM1_GTCFR2 } },
    {"irq",               HCF_RES_INT,  { (void *) INUM_GTM4 } },
    {"irqLevel",          HCF_RES_INT,  { (void *) INUM_GTM4 } },
    {"regBase1",          HCF_RES_INT,  { (void *) QUICC_GTM1_GTMDR3 } },
    {"clkRateMin",        HCF_RES_INT,  { (void *) AUX_CLK_RATE_MIN } },
    {"clkRateMax",        HCF_RES_INT,  { (void *) AUX_CLK_RATE_MAX } },
    {"clkFrequency",      HCF_RES_INT,  { (void *) SYS_CLK_FREQ } }
};

#define generalPurposeTimerNum34   NELEMENTS(generalPurposeTimerResources34)

#ifdef DRV_STORAGE_FSLSATA
const struct hcfResource fslSata1Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *) SATA1_REG_BA } },
    { "irq",         HCF_RES_INT, {(void *) INUM_SATA1 } },
    { "irqLevel",    HCF_RES_INT, {(void *) INUM_SATA1 } },
};

#define fslSata1Num NELEMENTS(fslSata1Resources)

const struct hcfResource fslSata2Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *) SATA2_REG_BA } },
    { "irq",         HCF_RES_INT, {(void *) INUM_SATA2 } },
    { "irqLevel",    HCF_RES_INT, {(void *) INUM_SATA2 } },
};

#define fslSata2Num NELEMENTS(fslSata2Resources)

const struct hcfResource fslSata3Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *) SATA3_REG_BA } },
    { "irq",         HCF_RES_INT, {(void *) INUM_SATA3 } },
    { "irqLevel",    HCF_RES_INT, {(void *) INUM_SATA3 } },
};

#define fslSata3Num NELEMENTS(fslSata3Resources)

const struct hcfResource fslSata4Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *) SATA4_REG_BA } },
    { "irq",         HCF_RES_INT, {(void *) INUM_SATA4 } },
    { "irqLevel",    HCF_RES_INT, {(void *) INUM_SATA4 } },
};

#define fslSata4Num NELEMENTS(fslSata4Resources)
#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]= {
    {"regBase",          HCF_RES_INT,  {(void *) (M83XX_CAPLENGTH) } },
    {"irq",              HCF_RES_INT,  {(void *) (INUM_USB_DR) } },
    {"irqLevel",         HCF_RES_INT,  {(void *) (INUM_USB_DR) } },
    {"ehciInit",         HCF_RES_ADDR, {(void *) sysM837xEhciInit } },
    {"dataSwap",         HCF_RES_ADDR, {(void *) vxbSwap32 } }
};

#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)

#endif /* INCLUDE_EHCI */

const struct hcfDevice hcfDeviceList[] = {

    { "quiccIntCtlr",   0, VXB_BUSID_PLB, 0, quiccIntCtlr0Num,          quiccIntCtlr0Resources },
    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,            ppcIntCtlr0Resources },
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum,         ppcDecTimerDevResources },
    { "quiccTimerDev",  0, VXB_BUSID_PLB, 0, generalPurposeTimerNum12,  generalPurposeTimerResources12 },
    { "quiccTimerDev",  1, VXB_BUSID_PLB, 0, generalPurposeTimerNum34,  generalPurposeTimerResources34 },

    { "ns16550",        0, VXB_BUSID_PLB, 0, ns1655x1Num,   ns1655x1Resources },
    { "ns16550",        1, VXB_BUSID_PLB, 0, ns1655x2Num,   ns1655x2Resources },

#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci",       0, VXB_BUSID_PLB,0, m83xxPci0Num,   m83xxPci0Resources },
#endif /* DRV_PCIBUS_M83XX */

#ifdef DRV_PCIBUS_M83XX_EX
    { "m83xxPciEx",     0, VXB_BUSID_PLB,0, m83xxPciEx0Num, m83xxPciEx0Resources },
    { "m83xxPciEx",     1, VXB_BUSID_PLB,0, m83xxPciEx1Num, m83xxPciEx1Resources },
#endif /* DRV_PCIBUS_M83XX_EX */

    { "motetsec",       0, VXB_BUSID_PLB, 0, etsecEnd0Num,  motEtsecEnd0Resources },
    { "motetsec",       1, VXB_BUSID_PLB, 0, etsecEnd1Num,  motEtsecEnd1Resources },

#ifdef DRV_STORAGE_FSLSATA
    {"fslSata",         0, VXB_BUSID_PLB, 0, fslSata1Num,   fslSata1Resources },
    {"fslSata",         1, VXB_BUSID_PLB, 0, fslSata2Num,   fslSata2Resources },
    {"fslSata",         2, VXB_BUSID_PLB, 0, fslSata3Num,   fslSata3Resources },
    {"fslSata",         3, VXB_BUSID_PLB, 0, fslSata4Num,   fslSata4Resources },
#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
    {"vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum, vxbPlbUsbEhciDevResources}
#endif /* INCLUDE_EHCI */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { "mv88E1x11Phy",  0, "mode",            VXB_PARAM_INT32,   {(void*)MV88E_MODE_RGMII_FIXUP}},
    { "mv88E1x11Phy",  1, "mode",            VXB_PARAM_INT32,   {(void*)MV88E_MODE_RGMII_FIXUP}},
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,   {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,   {(void *)FALSE} },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR, {(void *)usbOpAfterReset}},
#endif /* INCLUDE_EHCI */
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
