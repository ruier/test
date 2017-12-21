/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02b,27jun13,j_z  add sRIO share slave target memory support.
02a,17apr13,j_z  add sRIO support.
01z,06feb13,wyt  Update device base address macros.
01y,05feb13,wyt  Add SATA configuration.
01x,28jan13,wyt  Correct several base address macros.
01w,27jan13,y_y  Add eSPI configuration.
01v,25jan13,wyt  Update code to accord with macros.
01u,24jan13,wap  Correct Bman and Qman memory size resource types
01t,23jan13,wap  Adjust I2C configuration to improve stability
01s,18jan13,wap  Add additional dummyMdio devices to match new memacMdio
                 configuration
01r,17jan13,wap  Add support for alternate SERDES1/SERDES2 layout
01q,16jan13,wap  Fix include path for vxbQorIQMemacEnd.h
01p,16jan13,wap  Update for new RCW and XAUI support. Also add support for
                 Vitesse VSC3316 crossbar switch
01o,02dec12,syt  add parameter "autoIntRouteSet" in PCI-Ex resource
01n,14nov12,syt  add support for DMA_2 device.
01m,12nov12,syt  replaced macro MAIN_FLASH_SIZE with FLASH_SIZE.
01l,01nov12,syt  added I2C support.
01k,07nov12,syt  added TureFFS support.
01j,01nov12,h_k  updated memac assignment to cores in AMP.
                 memac4 is assigned to core0 for primary AMP, memac12 is
                 assigned to core8 for example secondary AMP in 3 AMPs config.
01i,19oct12,syt  added DMA support.
01h,18oct12,syt  added USB support om SIMICS and parameter pciExNum
                 for PCI-EXs.
01i,15oct12,wap  Correct PHY addresses for RGMII ethernet ports, add support
                 for SGMII ethernet ports
01h,15oct12,wap  Fix OpenPIC timer support
01g,09oct12,syt  code clean up and add PCI-EX support.
01f,10oct12,wap  Add initial MDIO support for RGMII ethernet ports
01e,08oct12,wap  Add RGMII media selection for T4240 QDS board
01d,14sep12,h_k  corrected a comment for DRV_MII_QORIQMEMAC_MDIO.
01c,10sep12,h_k  added AMP configurations for memac.
01b,15aug12,wap  Update for new MEMAC driver
01a,07may12,h_k  adapted from fsl_p5020_ds version 01j.
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include <drv/pci/pciAutoConfigLib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <h/end/vxbQorIQMemacEnd.h>

IMPORT UINT32 sysPpcTbClkFreqGet(void);
IMPORT UINT32 sysPlbClkFreqGet(void);

#ifdef DRV_SRIO_FSL
#   include <vxbus/vxbRapidIO.h>
#endif /* DRV_SRIO_FSL */

#ifdef DRV_INTCTLR_EPIC
IMPORT UINT32 sysPicClkFreqGet(UINT32);
#endif  /* DRV_INTCTLR_EPIC */

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_I2CBUS_FSL
#   include <hwif/vxbus/vxbI2cLib.h>
#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_ESPI_FSL
#   include <hwif/vxbus/vxbSpiLib.h>
#endif /* DRV_ESPI_FSL */

#ifdef DRV_PCIBUS_QORIQ_PCIEX
IMPORT STATUS sysPci1AutoconfigInclude ();
IMPORT UCHAR sysPci1AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci2AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci3AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci4AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif /* DRV_PCIBUS_QORIQ_PCIEX */

IMPORT int sysEprGet(void);

#ifdef INCLUDE_EHCI
IMPORT void  ehci0Init (void);
IMPORT void  ehci0PostResetHook (void);
IMPORT void  ehci1Init (void);
IMPORT void  ehci1PostResetHook (void);
#endif /* INCLUDE_EHCI */

#ifdef INCLUDE_I2C_BUS
IMPORT void sysI2cBus0ChanSel (UINT32);
#endif /* INCLUDE_I2C_BUS */

#ifdef INCLUDE_AMP

/*
 * Users can control the assignment of devices to CPUs by using struct
 * ampCpuTable definitions.
 */

typedef struct {
        char*   deviceName;     /* hardWare device name */
        int     unit;           /* hardWare device index */
        int     cpu;            /* assigned core number */
} AMP_CPU_TABLE;

/*
 * The hardware devices on T4240 can be allocated to either core. Users just
 * need to fill the parameters such as the device driver name, device index and
 * which core the device should be assigned to, then system will install and
 * register the devices driver onto the corresponding core image. Table includes
 * only devices we want to filter. If no action required for a device per CPU
 * then it's not in the table
 */

AMP_CPU_TABLE ampCpuTable[] = {

    /* driver,          unit num,   assigned cpu */

    /* assigned cpu & coreNum resources must match for memac */

    { "memac",          0,          2 },
    { "memac",          1,          4 },
    { "memac",          2,          6 },
    { "memac",          3,         10 },
    { "memac",          4,          0 },
    { "memac",          5,         12 },
    { "memac",          6,          0 },
    { "memac",          7,          0 },
    { "memac",          8,         14 },
    { "memac",          9,         16 },
    { "memac",         10,         18 },
    { "memac",         11,         20 },
    { "memac",         12,          8 },
    { "memac",         13,         22 },
    { "memac",         14,          0 },
    { "memac",         15,          0 },
    { "openPicTimer",   0,          0 },
    { "openPicTimer",   1,          0 },
};

#endif /* INCLUDE_AMP */

#ifdef DRV_VXBEND_QORIQMEMAC

#ifdef INCLUDE_DUMMY_MDIO
#define MEMAC_MDIO_NAME	"dummyMdio"
#else
#define MEMAC_MDIO_NAME	"memacMdio"
#endif

/*
 * Each Frame manager has up to 8 available MEMAC ports:
 * 6 10/100/1000 ports and 2 10Gbps ports. Exactly how many
 * of those ports can be used depends on the SerDes configuration
 * set in the Reset Configuration Word (RCW). You can never have
 * all ports active at once, except in Simics. The T4240 QDS board
 * comes with two built-in RGMII ports (memac4 and memac12). You
 * can add SGMII riser cards to enable additional 10/100/1000 ports,
 * as well as XAUI and/or QSGMII for 10GbE ports.
 *
 * With the current T4240 QDS eval board, it appears that SGMII
 * risers can be plugged into slot 2 or slot 4, to enable 4 extra
 * MEMAC ports on each Fman. It's not clear if these configurations
 * have been fully validated by Freescale. Putting an SGMII riser
 * in slot 2 seems to work fine. Putting an SGMII riser in slot 4
 * works only partially: the ports only seem to work correctly at
 * 100Mbps speeds. At 1000Mbps, the interface receives correctly
 * but transmitted frames are garbled. This is likely a clocking
 * issue. This configuration has only been tested to check that the
 * MDIO configuration works.
 *
 * The following is the currently configured MAC/PHY mapping, when
 * SERDES1 is configured for XAUI and SERDES2 is configured for
 * SGMII. The T4240 provides dedicated MDIO register banks for the
 * external PHYs (meaning that MEMAC0's registers are no longer
 * overloaded).
 *
 * MAC          PHY             Management port pins    MII address
 * --------     --------------- --------------------    -----------
 * MEMAC4       RGMII on-board  MDIO0 on FMAN1          2
 * MEMAC6       XAUI  slot 1    MDIO1 on FMAN1          0
 * MEMAC7       XAUI  slot 2    MDIO1 on FMAN1          1
 * MEMAC8       SGMII slot 4    MDIO0 on FMAN1          28
 * MEMAC9       SGMII slot 4    MDIO0 on FMAN1          29
 * MEMAC10      SGMII slot 4    MDIO0 on FMAN1          30
 * MEMAC11      SGMII slot 4    MDIO0 on FMAN1          31
 * MEMAC12      RGMII on-board  MDIO0 on FMAN1          1
 * MEMAC13      SGMII slot 3    MDIO0 on FMAN1          28
 * MEMAC14      SGMII slot 3    MDIO0 on FMAN1          30
 * MEMAC15      SGMII slot 3    MDIO0 on FMAN1          31
 *
 * Note that is also possible to have SERDES1 configured for SGMII
 * and SERDES2 configured for XAUI. In that case, the layout changes.
 *
 * MAC          PHY             Management port pins    MII address
 * --------     --------------- --------------------    -----------
 * MEMAC0       SGMII slot 2    MDIO0 on FMAN1          28
 * MEMAC1       SGMII slot 2    MDIO0 on FMAN1          29
 * MEMAC2       SGMII slot 2    MDIO0 on FMAN1          30
 * MEMAC3       SGMII slot 2    MDIO0 on FMAN1          31
 * MEMAC4       RGMII on-board  MDIO0 on FMAN1          2
 * MEMAC5       SGMII slot 1    MDIO0 on FMAN1          29
 * MEMAC6       SGMII slot 1    MDIO0 on FMAN1          31
 * MEMAC7       SGMII slot 1    MDIO0 on FMAN1          30
 * MEMAC12      RGMII on-board  MDIO0 on FMAN1          1
 * MEMAC14      XAUI  slot 3    MDIO1 on FMAN1          2
 * MEMAC15      XAUI  slot 4    MDIO1 on FMAN1          3
 */

const struct hcfResource memac0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC1_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES1_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)28 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)2 } },
#   endif /* INCLUDE_AMP */
};
#   define memac0Num NELEMENTS(memac0Resources)

const struct hcfResource memac1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC2_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES1_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)29 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)4 } },
#   endif /* INCLUDE_AMP */
};
#   define memac1Num NELEMENTS(memac1Resources)

const struct hcfResource memac2Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC3_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES1_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)30 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)6 } },
#   endif /* INCLUDE_AMP */
};
#   define memac2Num NELEMENTS(memac2Resources)

const struct hcfResource memac3Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC4_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES1_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)31 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)10 } },
#   endif /* INCLUDE_AMP */
};
#   define memac3Num NELEMENTS(memac3Resources)

const struct hcfResource memac4Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC5_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)2 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_RGMII } },
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#   define memac4Num NELEMENTS(memac4Resources)

const struct hcfResource memac5Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC6_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES1_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)29 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)12 } },
#   endif /* INCLUDE_AMP */
};
#   define memac5Num NELEMENTS(memac5Resources)

const struct hcfResource memac6Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC7_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
#ifdef SERDES1_XAUI
    { "miiIfUnit",  HCF_RES_INT,    { (void *)3 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_XAUI } },
#endif
#ifdef SERDES1_SGMII
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)31 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#   define memac6Num NELEMENTS(memac6Resources)

const struct hcfResource memac7Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC8_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
#ifdef SERDES1_XAUI
    { "miiIfUnit",  HCF_RES_INT,    { (void *)3 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)1 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_XAUI } },
#endif
#ifdef SERDES1_SGMII
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)30 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#   define memac7Num NELEMENTS(memac7Resources)

const struct hcfResource memac8Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC9_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES2_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)28 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)14 } },
#   endif /* INCLUDE_AMP */
};
#   define memac8Num NELEMENTS(memac8Resources)

const struct hcfResource memac9Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC10_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES2_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)29 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)16 } },
#   endif /* INCLUDE_AMP */
};
#   define memac9Num NELEMENTS(memac9Resources)

const struct hcfResource memac10Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC11_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES2_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)30 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)18 } },
#   endif /* INCLUDE_AMP */
};
#   define memac10Num NELEMENTS(memac10Resources)

const struct hcfResource memac11Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC12_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES2_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)31 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)20 } },
#   endif /* INCLUDE_AMP */
};
#   define memac11Num NELEMENTS(memac11Resources)

const struct hcfResource memac12Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC13_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)1 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_RGMII } },
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)8 } },
#   endif /* INCLUDE_AMP */
};
#   define memac12Num NELEMENTS(memac12Resources)

const struct hcfResource memac13Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC14_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
#ifdef SERDES2_SGMII
    { "phyAddr",    HCF_RES_INT,    { (void *)29 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)22 } },
#   endif /* INCLUDE_AMP */
};
#   define memac13Num NELEMENTS(memac13Resources)

const struct hcfResource memac14Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC15_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
#ifdef SERDES2_XAUI
    { "miiIfUnit",  HCF_RES_INT,    { (void *)3 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)2 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_XAUI } },
#endif
#ifdef SERDES2_SGMII
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)31 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#   define memac14Num NELEMENTS(memac14Resources)

const struct hcfResource memac15Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MEMAC16_BASE } },
    { "miiIfName",  HCF_RES_STRING, { (void *)MEMAC_MDIO_NAME } },
#ifdef SERDES2_XAUI
    { "miiIfUnit",  HCF_RES_INT,    { (void *)3 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)3 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_XAUI } },
#endif
#ifdef SERDES2_SGMII
    { "miiIfUnit",  HCF_RES_INT,    { (void *)2 } },
    { "phyAddr",    HCF_RES_INT,    { (void *)30 } },
    { "memacMedia", HCF_RES_INT,    { (void *)MEMAC_MEDIA_SGMII } },
#endif
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#   define memac15Num NELEMENTS(memac15Resources)


#endif /* DRV_VXBEND_QORIQMEMAC */

#ifdef	DRV_MII_QORIQMEMAC_MDIO

/*
 * The Fman_v3 logic includes dedicated MDIO register banks
 * that are independent of the MEMAC devices. There are two for
 * each Fman, one offset 0xXXXFC000 and 0xXXXFD000. With the
 * QDS board, the first port on Fman2 at 0xFE5FC000 is attached
 * to the two RGMII PHYs.
 */

const struct hcfResource memacMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MDIO1_BASE } },
};
#   define memacMdio0Num NELEMENTS(memacMdio0Resources)

const struct hcfResource memacMdio1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MDIO2_BASE } },
};
#   define memacMdio1Num NELEMENTS(memacMdio1Resources)

const struct hcfResource memacMdio2Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MDIO3_BASE } },
};
#   define memacMdio2Num NELEMENTS(memacMdio2Resources)

const struct hcfResource memacMdio3Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_MDIO4_BASE } },
};
#   define memacMdio3Num NELEMENTS(memacMdio3Resources)

#endif	/* DRV_MII_QORIQMEMAC_MDIO */

#ifdef INCLUDE_DUMMY_MDIO

/*
 * Simics doesn't properly simulate MDIO for the Fman network
 * ports. To work around this, we use the dummy MDIO driver
 * to make the network ports believe they always have a link
 * present.
 */

const struct hcfResource dummyMdio0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)0 } },
    { "fullDuplex", HCF_RES_INT, { (void *)1 } },
    { "speed",      HCF_RES_INT, { (void *)1000 } }
};
#define dummyMdio0Num NELEMENTS(dummyMdio0Resources)

#endif /* INCLUDE_DUMMY_MDIO */

#ifdef DRV_PCIBUS_QORIQ_PCIEX

/*
 * The P5020 processor has four built-in PCIe host bridges.
 * Both legacy INTx and MSI interrupts are supported. Since
 * MSI is the prefered mechanism for handling interrupts
 * with PCIe, it's enabled by default. This can be changed by
 * setting the "msiEnable" property to FALSE (or just deleting
 * it). MSI will only be used for devices that advertise MSI
 * capability support in their configuration header.
 */

const struct hcfResource pci0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_PCIEX1_BASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO32_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX1_IO32_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "pciExNum",           HCF_RES_INT,    { (void *)0 } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX1_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX1_LAW_SIZE } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPci1AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define pci0Num NELEMENTS(pci0Resources)

const struct hcfResource pci1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_PCIEX2_BASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO32_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX2_IO32_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "pciExNum",           HCF_RES_INT,    { (void *)1 } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX2_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX2_LAW_SIZE } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPci2AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define pci1Num NELEMENTS(pci1Resources)

const struct hcfResource pci2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_PCIEX3_BASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO32_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX3_IO32_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "pciExNum",           HCF_RES_INT,    { (void *)2 } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX3_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX3_LAW_SIZE } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPci3AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define pci2Num NELEMENTS(pci2Resources)

const struct hcfResource pci3Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_PCIEX4_BASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX4_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX4_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX4_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX4_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX4_IO32_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX4_IO32_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX4_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX4_IO_SIZE } },
    { "pciExNum",           HCF_RES_INT,    { (void *)3 } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX4_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX4_LAW_SIZE } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPci4AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define pci3Num NELEMENTS(pci3Resources)

#endif /* DRV_PCIBUS_QORIQ_PCIEX */

#ifdef DRV_RESOURCE_QORIQLAW

const struct hcfResource law0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)T4_LAWBARH0 } },
};
#   define law0Num NELEMENTS(law0Resources)

#endif /* DRV_RESOURCE_QORIQLAW */

/*
 * At least with the current silicon rev, the frame manager
 * in the T4240 requires a microcode patch in order for
 * certain features to work correctly. This includes the
 * soft parser, independent mode and the coarse classifier.
 * The microcode patch only needs to be loaded once per
 * fman instance, after a hard reset. In an AMP configuration,
 * only CPU 0 has to load it.
 */

#ifdef DRV_RESOURCE_QORIQFMAN

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)T4_FMAN1_BASE } },
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,    { (void *)1 } },
    { "ucodeAddr",      HCF_RES_ADDR,   { (void *)NULL } }
#   endif
};
#   define fman0Num NELEMENTS(fman0Resources)

struct hcfResource fman1Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)T4_FMAN2_BASE } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,    { (void *)1 } },
    { "ucodeAddr",      HCF_RES_ADDR,   { (void *)NULL } }
#endif
};
#define fman1Num NELEMENTS(fman1Resources)

#endif /* DRV_RESOURCE_QORIQFMAN */

#ifdef DRV_RESOURCE_QORIQQMAN

const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)T4_QMAN1_BASE} },
    { "qmanLawBase",    HCF_RES_ADDR,   { (void *)QMAN_LAW_BASE} },
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "qmanMemBase",    HCF_RES_ADDR,   { (void *)QMAN_MEM_BASE} },
    { "qmanMemSize",    HCF_RES_INT,    { (void *)QMAN_MEM_SIZE} },
#   endif
};
#   define qman0Num NELEMENTS(qman0Resources)

#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_RESOURCE_QORIQBMAN

const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)T4_BMAN1_BASE } },
    { "bmanLawBase",    HCF_RES_ADDR,   { (void *)BMAN_LAW_BASE } },
#   if !defined (INCLUDE_AMP) || defined   (INCLUDE_AMP_CPU_00)
    { "bmanMemBase",    HCF_RES_ADDR,   { (void *)BMAN_MEM_BASE } },
    { "bmanMemSize",    HCF_RES_INT,    { (void *)BMAN_MEM_SIZE } },
#   endif
};
#   define bman0Num NELEMENTS(bman0Resources)

#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_SIO_NS16550

const struct hcfResource ns1655x1Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)T4_DUART1_BASE } },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC } },
    { "regInterval",    HCF_RES_INT,    { (void *)1 } },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet } }
    };

#   define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)T4_DUART2_BASE } },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC } },
    { "regInterval",    HCF_RES_INT,    { (void *)1 } },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet } }
    };
#   define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ns1655x3Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)T4_DUART3_BASE } },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC } },
    { "regInterval",    HCF_RES_INT,    { (void *)1 } },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet } }
    };
#   define ns1655x3Num NELEMENTS(ns1655x3Resources)

const struct hcfResource ns1655x4Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)T4_DUART4_BASE } },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC } },
    { "regInterval",    HCF_RES_INT,    { (void *)1 } },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet } }
    };
#   define ns1655x4Num NELEMENTS(ns1655x4Resources)

#endif /* #ifdef DRV_SIO_NS16550 */

struct intrCtlrInputs epicInputs[] = {
#if !defined(INCLUDE_AMP)
    { EPIC_DUART_INT_VEC,               "ns16550",      0,  0 },
    { EPIC_DUART_INT_VEC,               "ns16550",      1,  0 },
    { EPIC_DUART2_INT_VEC,              "ns16550",      2,  0 },
    { EPIC_DUART2_INT_VEC,              "ns16550",      3,  0 },
#else
# ifdef INCLUDE_AMP_CPU_00
    { EPIC_DUART_INT_VEC,               "ns16550",      0,  0 },
    { EPIC_DUART_INT_VEC,               "ns16550",      1,  0 },
# elif (defined(INCLUDE_3AMP_CONFIG) && defined(INCLUDE_AMP_CPU_04)) || \
       (defined(INCLUDE_6AMP_CONFIG) && defined(INCLUDE_AMP_CPU_02)) || \
	defined(INCLUDE_AMP_CPU_01)
    { EPIC_DUART2_INT_VEC,              "ns16550",      0,  0 },
    { EPIC_DUART2_INT_VEC,              "ns16550",      1,  0 },
# endif /* INCLUDE_AMP_CPU_00 */
#endif	/* !INCLUDE_AMP */
    { EPIC_QMAN_PORTAL0_INT_VEC,        "QorIQQman",    0,  0 },
    { EPIC_QMAN_PORTAL1_INT_VEC,        "QorIQQman",    0,  1 },
    { EPIC_QMAN_PORTAL2_INT_VEC,        "QorIQQman",    0,  2 },
    { EPIC_QMAN_PORTAL3_INT_VEC,        "QorIQQman",    0,  3 },
    { EPIC_QMAN_PORTAL4_INT_VEC,        "QorIQQman",    0,  4 },
    { EPIC_QMAN_PORTAL5_INT_VEC,        "QorIQQman",    0,  5 },
    { EPIC_QMAN_PORTAL6_INT_VEC,        "QorIQQman",    0,  6 },
    { EPIC_QMAN_PORTAL7_INT_VEC,        "QorIQQman",    0,  7 },
    { EPIC_QMAN_PORTAL8_INT_VEC,        "QorIQQman",    0,  8 },
    { EPIC_QMAN_PORTAL9_INT_VEC,        "QorIQQman",    0,  9 },
    { EPIC_QMAN_PORTAL10_INT_VEC,       "QorIQQman",    0, 10 },
    { EPIC_QMAN_PORTAL11_INT_VEC,       "QorIQQman",    0, 11 },
    { EPIC_QMAN_PORTAL12_INT_VEC,       "QorIQQman",    0, 12 },
    { EPIC_QMAN_PORTAL13_INT_VEC,       "QorIQQman",    0, 13 },
    { EPIC_QMAN_PORTAL14_INT_VEC,       "QorIQQman",    0, 14 },
    { EPIC_QMAN_PORTAL15_INT_VEC,       "QorIQQman",    0, 15 },
    { EPIC_QMAN_PORTAL16_INT_VEC,       "QorIQQman",    0, 16 },
    { EPIC_QMAN_PORTAL17_INT_VEC,       "QorIQQman",    0, 17 },
    { EPIC_QMAN_PORTAL18_INT_VEC,       "QorIQQman",    0, 18 },
    { EPIC_QMAN_PORTAL19_INT_VEC,       "QorIQQman",    0, 19 },
    { EPIC_QMAN_PORTAL20_INT_VEC,       "QorIQQman",    0, 20 },
    { EPIC_QMAN_PORTAL21_INT_VEC,       "QorIQQman",    0, 21 },
    { EPIC_QMAN_PORTAL22_INT_VEC,       "QorIQQman",    0, 22 },
    { EPIC_QMAN_PORTAL23_INT_VEC,       "QorIQQman",    0, 23 },

    { EPIC_BMAN_PORTAL0_INT_VEC,        "QorIQBman",    0,  0 },
    { EPIC_BMAN_PORTAL1_INT_VEC,        "QorIQBman",    0,  1 },
    { EPIC_BMAN_PORTAL2_INT_VEC,        "QorIQBman",    0,  2 },
    { EPIC_BMAN_PORTAL3_INT_VEC,        "QorIQBman",    0,  3 },
    { EPIC_BMAN_PORTAL4_INT_VEC,        "QorIQBman",    0,  4 },
    { EPIC_BMAN_PORTAL5_INT_VEC,        "QorIQBman",    0,  5 },
    { EPIC_BMAN_PORTAL6_INT_VEC,        "QorIQBman",    0,  6 },
    { EPIC_BMAN_PORTAL7_INT_VEC,        "QorIQBman",    0,  7 },
    { EPIC_BMAN_PORTAL8_INT_VEC,        "QorIQBman",    0,  8 },
    { EPIC_BMAN_PORTAL9_INT_VEC,        "QorIQBman",    0,  9 },
    { EPIC_BMAN_PORTAL10_INT_VEC,       "QorIQBman",    0, 10 },
    { EPIC_BMAN_PORTAL11_INT_VEC,       "QorIQBman",    0, 11 },
    { EPIC_BMAN_PORTAL12_INT_VEC,       "QorIQBman",    0, 12 },
    { EPIC_BMAN_PORTAL13_INT_VEC,       "QorIQBman",    0, 13 },
    { EPIC_BMAN_PORTAL14_INT_VEC,       "QorIQBman",    0, 14 },
    { EPIC_BMAN_PORTAL15_INT_VEC,       "QorIQBman",    0, 15 },
    { EPIC_BMAN_PORTAL16_INT_VEC,       "QorIQBman",    0, 16 },
    { EPIC_BMAN_PORTAL17_INT_VEC,       "QorIQBman",    0, 17 },
    { EPIC_BMAN_PORTAL18_INT_VEC,       "QorIQBman",    0, 18 },
    { EPIC_BMAN_PORTAL19_INT_VEC,       "QorIQBman",    0, 19 },
    { EPIC_BMAN_PORTAL20_INT_VEC,       "QorIQBman",    0, 20 },
    { EPIC_BMAN_PORTAL21_INT_VEC,       "QorIQBman",    0, 21 },
    { EPIC_BMAN_PORTAL22_INT_VEC,       "QorIQBman",    0, 22 },
    { EPIC_BMAN_PORTAL23_INT_VEC,       "QorIQBman",    0, 23 },

#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,          "openPicTimer", 0,  0 },
    { (OPENPIC_TIMERA0_INT_VEC + 1),    "openPicTimer", 0,  1 },
    { (OPENPIC_TIMERA0_INT_VEC + 2),    "openPicTimer", 0,  2 },
    { (OPENPIC_TIMERA0_INT_VEC + 3),    "openPicTimer", 0,  3 },
    { OPENPIC_TIMERB0_INT_VEC,          "openPicTimer", 1,  0 },
    { (OPENPIC_TIMERB0_INT_VEC + 1),    "openPicTimer", 1,  1 },
    { (OPENPIC_TIMERB0_INT_VEC + 2),    "openPicTimer", 1,  2 },
    { (OPENPIC_TIMERB0_INT_VEC + 3),    "openPicTimer", 1,  3 },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_STORAGE_SDHC
    { EPIC_SDHC_INT_VEC,                "sdhci",        0,  0 },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
    { EPIC_SATA1_INT_VEC,               "fslSata",     0,  0 },
    { EPIC_SATA2_INT_VEC,               "fslSata",     1,  0 },
#endif  /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
    { EPIC_USB1_INT_VEC,                "vxbPlbUsbEhci",0,  0 },
    { EPIC_USB2_INT_VEC,                "vxbPlbUsbEhci",1,  0 },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,          "fslDma",       0,  0 },
    { EPIC_DMA1_CHAN1_INT_VEC,          "fslDma",       0,  1 },
    { EPIC_DMA1_CHAN2_INT_VEC,          "fslDma",       0,  2 },
    { EPIC_DMA1_CHAN3_INT_VEC,          "fslDma",       0,  3 },
    { EPIC_DMA1_CHAN4_INT_VEC,          "fslDma",       0,  4 },
    { EPIC_DMA1_CHAN5_INT_VEC,          "fslDma",       0,  5 },
    { EPIC_DMA1_CHAN6_INT_VEC,          "fslDma",       0,  6 },
    { EPIC_DMA1_CHAN7_INT_VEC,          "fslDma",       0,  7 },
    { EPIC_DMA2_CHAN0_INT_VEC,          "fslDma",       1,  0 },
    { EPIC_DMA2_CHAN1_INT_VEC,          "fslDma",       1,  1 },
    { EPIC_DMA2_CHAN2_INT_VEC,          "fslDma",       1,  2 },
    { EPIC_DMA2_CHAN3_INT_VEC,          "fslDma",       1,  3 },
    { EPIC_DMA2_CHAN4_INT_VEC,          "fslDma",       1,  4 },
    { EPIC_DMA2_CHAN5_INT_VEC,          "fslDma",       1,  5 },
    { EPIC_DMA2_CHAN6_INT_VEC,          "fslDma",       1,  6 },
    { EPIC_DMA2_CHAN7_INT_VEC,          "fslDma",       1,  7 },
#endif /* DRV_DMA_FSL */

#ifdef DRV_I2CBUS_FSL
    { EPIC_DI2C1_INT_VEC,                "fslI2c",      0,  0 },
    { EPIC_DI2C1_INT_VEC,                "fslI2c",      0,  1 },
    { EPIC_DI2C2_INT_VEC,                "fslI2c",      1,  0 },
    { EPIC_DI2C2_INT_VEC,                "fslI2c",      1,  1 },
#endif  /* DRV_I2CBUS_FSL */

#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,                "ipi",          0,  0 }
#endif /* _WRS_CONFIG_SMP */

#ifdef DRV_ESPI_FSL
    { EPIC_ESPI_INT_VEC,                 "fslEspi",     0,  0 },
#endif  /* DRV_ESPI_FSL */

};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART2_INT_VEC, 100 },
    { EPIC_DUART_INT_VEC,  100 }
};

INT_BANK_DESC exDesc = { 0, 12, 0x50000, 0x20, EPIC_EX_INTERRUPT };
INT_BANK_DESC inDesc = { EPIC_VEC_IN_IRQ0, 138, 0x50200, 0x20, EPIC_IN_INTERRUPT };
INT_BANK_DESC msgDesc = { 176, 8, 0x51600, 0x20, EPIC_MSG_INTERRUPT };
INT_BANK_DESC gtaDesc = { OPENPIC_TIMERA0_INT_VEC, 4, 0x41120, 0x40, EPIC_GT_A_INTERRUPT };
INT_BANK_DESC gtbDesc = { OPENPIC_TIMERB0_INT_VEC, 4, 0x42120, 0x40, EPIC_GT_B_INTERRUPT };
INT_BANK_DESC smsgDesc = { 243, 8, 0x51c00, 0x20, EPIC_SMSG_INTERRUPT };
INT_BANK_DESC ipiDesc = { 251, 4, 0x410a0, 0x10, EPIC_IPI_INTERRUPT };

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
    { "priority",           HCF_RES_ADDR,   { (void *)&epicPriority[0] } },
    { "priorityTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicPriority) } },
    { "crossBar",           HCF_RES_ADDR,   { (void *)&epicXBar[0] } },
    { "crossBarTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicXBar) } },
    { "exPolar",            HCF_RES_INT,    { (void *)EPIC_INT_ACT_LOW } },
    { "numInts",            HCF_RES_INT,    { (void *)255 } },
    { "intDesc_0",          HCF_RES_ADDR,   { (void *)&exDesc } },
    { "intDesc_1",          HCF_RES_ADDR,   { (void *)&inDesc } },
    { "intDesc_2",          HCF_RES_ADDR,   { (void *)&msgDesc } },
    { "intDesc_3",          HCF_RES_ADDR,   { (void *)&gtaDesc } },
    { "intDesc_4",          HCF_RES_ADDR,   { (void *)&gtbDesc } },
    { "intDesc_5",          HCF_RES_ADDR,   { (void *)&smsgDesc } },
    { "intDesc_6",          HCF_RES_ADDR,   { (void *)&ipiDesc } },
#ifdef EPIC_EXTERNAL_PROXY
    { "sysEprGet",          HCF_RES_ADDR,   { (void *)&sysEprGet } },
#endif
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX
struct hcfResource m85xxTimerResources[] =  {
    { "regBase",            HCF_RES_INT,    { (void *)0}},
    { "decMinClkRate",      HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate",      HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#   ifdef INCLUDE_AUX_CLK
    { "fitMinClkRate",      HCF_RES_INT,    { (void *)FIT_CLK_RATE_MIN } },
    { "fitMaxClkRate",      HCF_RES_INT,    { (void *)FIT_CLK_RATE_MAX } },
#   endif /* INCLUDE_AUX_CLK */
    { "sysClockFreq",       HCF_RES_ADDR,   { (void *)&sysPpcTbClkFreqGet } }
};
#   define m85xxTimerNum         NELEMENTS(m85xxTimerResources)
#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,    "epic",     0,      0 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)TRUE} },
    { "input",              HCF_RES_ADDR,   { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource usbEhci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)T4_EHCI_CAPLENGTH(T4_USB1_BASE) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci0Init } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)

const struct hcfResource usbEhci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)T4_EHCI_CAPLENGTH(T4_USB2_BASE) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci1Init } },
};
#define usbEhci1Num NELEMENTS(usbEhci1Resources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] =
    {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)T4_OPENPIC_TIMER1_BASE } },
    { "instanceName",       HCF_RES_STRING, { (void *)"A" }},
    { VXB_CLK_FREQ,         HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",         HCF_RES_INT,    { (void *)8}},
#   ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",            HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    { "vector1",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERA0_INT_VEC + 1) } },
    { "vector2",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERA0_INT_VEC + 2) } },
    { "vector3",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERA0_INT_VEC + 3) } },
#   endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#   define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)T4_OPENPIC_TIMER2_BASE } },
    { "instanceName",       HCF_RES_STRING, { (void *)"B" } },
    { VXB_CLK_FREQ,         HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",         HCF_RES_INT,    { (void *)8}},
#   ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",            HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    { "vector1",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERB0_INT_VEC + 1) } },
    { "vector2",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERB0_INT_VEC + 2) } },
    { "vector3",            HCF_RES_INT,    { (void *)(OPENPIC_TIMERB0_INT_VEC + 3) } },
#   endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#   define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
struct hcfResource fslSdhcResources[] =  {
    { "regBase",             HCF_RES_INT,   { (void *)T4_SDHC1_BASE } },
    { "clkFreq",             HCF_RES_ADDR,  { (void *)sysPlbClkFreqGet } },
    { "dmaMode",             HCF_RES_INT,   { (void *)0 } },
    { "polling",             HCF_RES_INT,   { (void *)0 } },
    { "flags" ,              HCF_RES_INT,   { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                       SDHC_FIFO_ENDIANESS_REVERSE |
                                                       SDHC_MISS_CARD_INS_INT_WHEN_INIT |
                                                       SDHC_HOST_VER_REVERSE |
                                                       SDHC_HOST_CTRL_FREESCALE) } },
};
#   define fslSdhcNum  NELEMENTS(fslSdhcResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
const struct hcfResource fslSata0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_SATA1_BASE } },
};
#   define fslSata0Num NELEMENTS(fslSata0Resources)

const struct hcfResource fslSata1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)T4_SATA2_BASE } },
};
#   define fslSata1Num NELEMENTS(fslSata1Resources)
#endif /* DRV_STORAGE_FSLSATA */

#ifdef DRV_DMA_FSL
const struct hcfResource fslDma1Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)T4_DMA1_BASE } },
    { "chanNum",    HCF_RES_INT,            { (void *)8 } },
};
#   define fslDma1Num NELEMENTS(fslDma1Resources)

const struct hcfResource fslDma2Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)T4_DMA2_BASE } },
    { "chanNum",    HCF_RES_INT,            { (void *)8 } },
};
#   define fslDma2Num NELEMENTS(fslDma2Resources)
#endif  /* DRV_DMA_FSL */

#ifdef DRV_VXB_NORFLASH
struct hcfResource norFlash0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)FLASH_BASE_ADRS } },
    { "maxSize",            HCF_RES_INT,    { (void *)FLASH_SIZE } },
#   ifdef INCLUDE_AMP_CPU
    { "sharedMem",          HCF_RES_INT,    { (void *)(TM_ANCHOR_ADRS + _CACHE_ALIGN_SIZE) } },
#   endif /* INCLUDE_AMP_CPU */
};
#   define norFlash0Num NELEMENTS(norFlash0Resources)
#endif /* DRV_VXB_NORFLASH */

#ifdef INCLUDE_I2C_BUS

/*
 * On T4240QDS, I2C Eeprom AT24C02 , RTCDS3232 and two PCA7547 are
 * connected to I2C bus 1.
 */

/*
 * Address  Part Number Size        Bus     Description
 * -------  ----------- --------    ---     --------------
 * 0x57     AT24C02     256-Bytes   0       Boot sequencer eeprom.
 * 0x68     DS3232                  0       Real time clock.
 * 0x70     VSC3316                 0       Vitesse crossbar switch (TX)
 * 0x71     VSC3316                 0       Vitesse crossbar switch (RX)
 * 0x76     PCA9547                 0       I2C bus multiplex switch.
 * 0x77     PCA9547                 0       I2C bus multiplex switch.
 */

struct i2cDevInputs i2c0DevTbl[] = {
    { "mux_PCA9547",        0x77,   I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY },
    { "mux_PCA9547",        0x76,   I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY },
    { "eeprom_at24c02",     0x57,   0 },
    { "rtc_ds3232",         0x68,   0 },
    { "vsc3316",            0x70,   I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY },
    { "vsc3316",            0x71,   I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY },
};

/*
 * For now, the first I2C controller is set to polled mode. This is
 * because we must provide access to the VSC3316 crossbar switch
 * device such that it can be initialized in sysHwInit2(), so that
 * network media adapters plugged into the slots attached to them
 * can be used later. We can't field interrupts during sysHwInit2(),
 * so we need to poll instead.
 */

struct hcfResource i2c0Resources[] = {
    { "regBase",            HCF_RES_ADDR,   { (void *)(T4_I2C1_BASE) } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "busSpeed",           HCF_RES_INT,    { (void *)100000 } },
    { "polling",            HCF_RES_INT,    { (void *)TRUE } },
    { "i2cDev",             HCF_RES_ADDR,   { (void *)&i2c0DevTbl} },
    { "i2cDevNum",          HCF_RES_INT,    { (void *)NELEMENTS(i2c0DevTbl)}},
};
#   define i2c0Num NELEMENTS(i2c0Resources)

#endif /* INCLUDE_I2C_BUS */

#ifdef DRV_SRIO_FSL
const struct hcfResource fslRio0Resources[] = {
    { "regBase",         HCF_RES_INT,  {(void *)T4_SRIO_BASE }},
    { "deviceBase",      HCF_RES_INT,  {(void *)(FSL_RIO1_MEM_ADRS)}},
    { "rioBusSize",      HCF_RES_INT,  {(void *)(FSL_RIO1_MEM_SIZE)}},
    { "lawSize",         HCF_RES_INT,  {(void *)(FSL_RIO1_LAW_SIZE)}},
    { "rioPortNum",      HCF_RES_INT,  {(void *)0}},
    { "smWinSize",       HCF_RES_INT,  {(void *)RIO_SM_WIN_SIZE_16M}},
    { "smSlaveAdrs",     HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},
    { "smHostAdrs",      HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},

/*
 * By default, Host/Salve mode will be selected by RCW. But, <isHost> can be used to
 * override the configuration from RCW - TRUE for Host mode, and FALSE for Slave mode.
 *
 * By default, the ID of RapidIO devices(boards) will be configured automatically. But 
 * <autoCfg> can be set to FALSE to disable the feature, and then <forceId> can and must
 * be used to specify the ID.
 *
 * By default, one Slave board will spin at one point until Host board finish the
 * configuration process for the Slave board. During this period, the system will not
 * go ahead and there will be no any output. <slaveNoWait> can be set to TRUE to skip
 * the process, but Slave boards will not be able to access the share memory of Host
 * board.
 * { "isHost",          HCF_RES_INT,  {(void *)TRUE}},
 * { "forceId",         HCF_RES_INT,  {(void *)0}},
 * { "slaveNoWait",     HCF_RES_INT,  {(void *)FALSE}},
 */

    { "autoCfg",         HCF_RES_INT,  {(void *)TRUE}},
};
#define fslRio0Num    NELEMENTS(fslRio0Resources)
#endif /* DRV_FSL_SRIO */

#ifdef INCLUDE_MSG_NET
LOCAL RIO_TRANS_PARAMETER rioNetArg =
    {
    1,               /* dest Id */
    RIO_TYPE_DSTR,   /* transfer type enum RIO_TYPE */
    0,               /* RIO_TYPE_MBOX mbox */
    0,               /* RIO_TYPE_MBOX letter */
    0                /* RIO_TYPE_MBOX OB channel */
    };
const struct hcfResource msgNet0Resources[] = {
    { "servDevName",      HCF_RES_STRING, {(void *)"QorIQRman" }},
    { "servDevUnit",      HCF_RES_INT,    {(void *)0}},
    { "msgNetHwAdr",      HCF_RES_INT,    {(void *)0xa5}},
    { "msgNetRxArg",      HCF_RES_LONG,   {(void *)&rioNetArg}},
    { "msgNetTxArg",      HCF_RES_LONG,   {(void *)&rioNetArg}},
};
#define msgNet0Num NELEMENTS(msgNet0Resources)
#endif /* INCLUDE_MSG_NET */

#ifdef DRV_RESOURCE_QORIQRMAN
struct hcfResource rman0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)T4_RMAN_BASE }},
    { "portNum",    HCF_RES_INT,    { (void *)0 } }
};
#define rman0Num NELEMENTS(rman0Resources)
#endif /* DRV_RESOURCE_QORIQRMAN */

#ifdef DRV_ESPI_FSL

struct vxbSpiDevInfo spiDevTbl[] = {
    { "spiFlash_sp25probe",    0,  8, FREQ_50_MHZ,  0}
};

struct hcfResource fslEspiResources[] =  {
    { "regBase",    HCF_RES_INT,   { (void *)(T4_ESPI1_BASE) } },
    { "clkFreq",    HCF_RES_ADDR,  { (void *)sysPlbClkFreqGet } },
    { "spiDev",     HCF_RES_ADDR,  { (void *)&spiDevTbl} },
    { "spiDevNum",  HCF_RES_INT,   { (void *)NELEMENTS(spiDevTbl)}}
};
#   define fslEspiNum  NELEMENTS(fslEspiResources)
#endif  /* DRV_ESPI_FSL */

const struct hcfDevice hcfDeviceList[] = {
    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0, VXB_BUSID_PLB,   0,  ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0, VXB_BUSID_PLB,   0,  epic0Num,       epic0Resources },

#ifdef DRV_SIO_NS16550
# if !defined(INCLUDE_AMP)
    { "ns16550",    0, VXB_BUSID_PLB,   0,  ns1655x1Num,    ns1655x1Resources },
    { "ns16550",    1, VXB_BUSID_PLB,   0,  ns1655x2Num,    ns1655x2Resources },
    { "ns16550",    2, VXB_BUSID_PLB,   0,  ns1655x3Num,    ns1655x3Resources },
    { "ns16550",    3, VXB_BUSID_PLB,   0,  ns1655x4Num,    ns1655x4Resources },
# else
#  ifdef INCLUDE_AMP_CPU_00
    { "ns16550",    0, VXB_BUSID_PLB,   0,  ns1655x1Num,    ns1655x1Resources },
    { "ns16550",    1, VXB_BUSID_PLB,   0,  ns1655x2Num,    ns1655x2Resources },
#  elif (defined(INCLUDE_3AMP_CONFIG) && defined(INCLUDE_AMP_CPU_04)) || \
       (defined(INCLUDE_6AMP_CONFIG) && defined(INCLUDE_AMP_CPU_02)) || \
        defined(INCLUDE_AMP_CPU_01)
    { "ns16550",    0, VXB_BUSID_PLB,   0,  ns1655x3Num,    ns1655x3Resources },
    { "ns16550",    1, VXB_BUSID_PLB,   0,  ns1655x4Num,    ns1655x4Resources },
#  endif /* INCLUDE_AMP_CPU_00 */
# endif	/* !INCLUDE_AMP */
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_VXB_NORFLASH
    { "norFlash",   0,  VXB_BUSID_PLB,  0,  norFlash0Num,   norFlash0Resources},
#endif /* DRV_VXB_NORFLASH */

#ifdef DRV_I2CBUS_FSL
    { "fslI2c",     0, VXB_BUSID_PLB,   0,  i2c0Num,        i2c0Resources },
#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_RESOURCE_QORIQLAW
    { "QorIQLaw",   0,  VXB_BUSID_PLB,  0,  law0Num,        law0Resources },
#endif /* DRV_RESOURCE_QORIQLAW */

#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman",  0,  VXB_BUSID_PLB,  0,  bman0Num,       bman0Resources },
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman",  0,  VXB_BUSID_PLB,  0,  qman0Num,       qman0Resources },
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman",  0,  VXB_BUSID_PLB,  0,  fman0Num,       fman0Resources },
    { "QorIQFman",  1,  VXB_BUSID_PLB,  0,  fman1Num,       fman1Resources },
#endif /* DRV_RESOURCE_QORIQFMAN */

#ifdef DRV_PCIBUS_QORIQ_PCIEX
    { "QorIQPciEx", 0,  VXB_BUSID_PLB,  0,  pci0Num,        pci0Resources },
    { "QorIQPciEx", 1,  VXB_BUSID_PLB,  0,  pci1Num,        pci1Resources },
    { "QorIQPciEx", 2,  VXB_BUSID_PLB,  0,  pci2Num,        pci2Resources },
    { "QorIQPciEx", 3,  VXB_BUSID_PLB,  0,  pci3Num,        pci3Resources },
#endif /* DRV_PCIBUS_QORIQ_PCIEX */

#ifdef DRV_VXBEND_QORIQMEMAC
    { "memac",      0, VXB_BUSID_PLB,   0,  memac0Num,      memac0Resources },
    { "memac",      1, VXB_BUSID_PLB,   0,  memac1Num,      memac1Resources },
    { "memac",      2, VXB_BUSID_PLB,   0,  memac2Num,      memac2Resources },
    { "memac",      3, VXB_BUSID_PLB,   0,  memac3Num,      memac3Resources },
    { "memac",      4, VXB_BUSID_PLB,   0,  memac4Num,      memac4Resources },
    { "memac",      5, VXB_BUSID_PLB,   0,  memac5Num,      memac5Resources },
    { "memac",      6, VXB_BUSID_PLB,   0,  memac6Num,      memac6Resources },
    { "memac",      7, VXB_BUSID_PLB,   0,  memac7Num,      memac7Resources },

    { "memac",      8, VXB_BUSID_PLB,   0,  memac8Num,      memac8Resources },
    { "memac",      9, VXB_BUSID_PLB,   0,  memac9Num,      memac9Resources },
    { "memac",     10, VXB_BUSID_PLB,   0,  memac10Num,     memac10Resources },
    { "memac",     11, VXB_BUSID_PLB,   0,  memac11Num,     memac11Resources },
    { "memac",     12, VXB_BUSID_PLB,   0,  memac12Num,     memac12Resources },
    { "memac",     13, VXB_BUSID_PLB,   0,  memac13Num,     memac13Resources },
    { "memac",     14, VXB_BUSID_PLB,   0,  memac14Num,     memac14Resources },
    { "memac",     15, VXB_BUSID_PLB,   0,  memac15Num,     memac15Resources },

#   ifdef DRV_MII_QORIQMEMAC_MDIO
    { "memacMdio",  0, VXB_BUSID_PLB,   0,  memacMdio0Num,   memacMdio0Resources },
    { "memacMdio",  1, VXB_BUSID_PLB,   0,  memacMdio1Num,   memacMdio1Resources },
    { "memacMdio",  2, VXB_BUSID_PLB,   0,  memacMdio2Num,   memacMdio2Resources },
    { "memacMdio",  3, VXB_BUSID_PLB,   0,  memacMdio3Num,   memacMdio3Resources },
#   endif /* DRV_MII_QORIQMEMAC_MDIO */

#   ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio",  0, VXB_BUSID_PLB,   0,  dummyMdio0Num,   dummyMdio0Resources },
    { "dummyMdio",  1, VXB_BUSID_PLB,   0,  dummyMdio0Num,   dummyMdio0Resources },
    { "dummyMdio",  2, VXB_BUSID_PLB,   0,  dummyMdio0Num,   dummyMdio0Resources },
    { "dummyMdio",  3, VXB_BUSID_PLB,   0,  dummyMdio0Num,   dummyMdio0Resources },
#   endif /* INCLUDE_DUMMY_MDIO */
#endif /* DRV_VXBEND_QORIQMEMAC */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum,   m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0, VXB_BUSID_PLB, 0,  openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer", 1, VXB_BUSID_PLB, 0,  openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",        0, VXB_BUSID_PLB, 0, fslSdhcNum,      fslSdhcResources },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
    { "fslSata",     0, VXB_BUSID_PLB, 0, fslSata0Num,     fslSata0Resources },
    { "fslSata",     1, VXB_BUSID_PLB, 0, fslSata1Num,     fslSata1Resources },
#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0,  VXB_BUSID_PLB,  0,  usbEhci0Num,    usbEhci0Resources },
    { "vxbPlbUsbEhci",  1,  VXB_BUSID_PLB,  0,  usbEhci1Num,    usbEhci1Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",         0,  VXB_BUSID_PLB,  0,  fslDma1Num,     fslDma1Resources },
    { "fslDma",         1,  VXB_BUSID_PLB,  0,  fslDma2Num,     fslDma2Resources },
#endif /* DRV_DMA_FSL */

#ifdef DRV_RESOURCE_QORIQRMAN
    { "QorIQRman",      0,  VXB_BUSID_PLB,  0,  rman0Num,       rman0Resources },
#endif /* DRV_RESOURCE_QORIQRMAN */

#ifdef DRV_SRIO_FSL
    { "fslSRio",        0,  VXB_BUSID_PLB,  0,  fslRio0Num,     fslRio0Resources },
#endif /* DRV_SRIO_FSL */

#ifdef INCLUDE_MSG_NET
    { "msgEnd",         0,  VXB_BUSID_PLB,  0,  msgNet0Num,     msgNet0Resources},
#endif /* INCLUDE_MSG_NET */

#ifdef DRV_ESPI_FSL
    { "fslEspi",         0, VXB_BUSID_PLB, 0, fslEspiNum,      fslEspiResources },
#endif  /* DRV_ESPI_FSL */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0, "hasEmbeddedTT",     VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci",  0, "postResetHook",     VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
    { "vxbPlbUsbEhci",  1, "hasEmbeddedTT",     VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci",  1, "postResetHook",     VXB_PARAM_FUNCPTR, { (void *)ehci1PostResetHook } },
#endif  /* INCLUDE_EHCI */
#ifdef INCLUDE_I2C_BUS
    { "mux_PCA9547",    1, "i2cDevEnableHook",  VXB_PARAM_FUNCPTR, { (void *)sysI2cBus0ChanSel } },
    { "mux_PCA9547",    1, "i2cDevEnableArg",   VXB_PARAM_POINTER, { (void *)I2C_BUS1_CHAN7 } },
    { "eeprom_at24c02", 0, "i2cDevEnableHook",  VXB_PARAM_FUNCPTR, { (void *)sysI2cBus0ChanSel } },
    { "eeprom_at24c02", 0, "i2cDevEnableArg",   VXB_PARAM_POINTER, { (void *)I2C_BUS1_CHAN0 } },
    { "rtc_ds3232",     0, "i2cDevEnableHook",  VXB_PARAM_FUNCPTR, { (void *)sysI2cBus0ChanSel } },
    { "rtc_ds3232",     0, "i2cDevEnableArg",   VXB_PARAM_POINTER, { (void *)I2C_BUS1_CHAN0 } },
    { "vsc3316",        0, "i2cDevEnableHook",  VXB_PARAM_FUNCPTR, { (void *)sysI2cBus0ChanSel } },
    { "vsc3316",        0, "i2cDevEnableArg",   VXB_PARAM_POINTER, { (void *)I2C_BUS1_CHAN4 } },
    { "vsc3316",        1, "i2cDevEnableHook",  VXB_PARAM_FUNCPTR, { (void *)sysI2cBus0ChanSel } },
    { "vsc3316",        1, "i2cDevEnableArg",   VXB_PARAM_POINTER, { (void *)I2C_BUS1_CHAN4 } },
#endif /* INCLUDE_I2C_BUS */
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
