/* hwconf.c - HW configuration support module for wrVbP2020rdb */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,05may11,agf  remove CCSR resource dependency of PCI
01b,22apr11,agf  add timer group B; add mipc buses 2-7
01a,15feb11,agf  created based on wrVbP4080 rev 01m
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <hwif/util/vxbParamSys.h>
#include <wrhv/vbi.h>


#ifdef DRV_SIO_NS16550
IMPORT UINT32 sysClkFreqGet (void);
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_TIMER_OPENPIC
IMPORT UINT32 sysPicClkFreqGet (UINT32);
#endif /* DRV_TIMER_OPENPIC */

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS  sysPciEx1AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx1AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS  sysPciEx2AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx2AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

/*
 * As of vxW 6.9 & wrhv-1.2, the MAX_XXX_INTERRUPTS are in defined in the
 * BSP. They are actually VBI specific and should be in an appropriate 
 * VBI include file, but moving them there will take coordination among 
 * all the existing guest BSPs. So until then, they are placed here, 
 * somewhat out of the way and easily moved/removed.
 */

#define MAX_VB_INTERRUPTS       256
#define MAX_HW_INTERRUPTS       256
#define MAX_VIRTUAL_INTERRUPTS  32

UINT32 hwIntTable[MAX_VB_INTERRUPTS];
UINT32 vbIntTable[MAX_HW_INTERRUPTS+MAX_VIRTUAL_INTERRUPTS];

/* virtual interrupt sources */

#define VIRT_MIPC_BUS_00        (MAX_HW_INTERRUPTS      +  0)
#define VIRT_MIPC_BUS_01        (MAX_HW_INTERRUPTS      +  1)
#define VIRT_MIPC_BUS_02        (MAX_HW_INTERRUPTS      +  2)
#define VIRT_MIPC_BUS_03        (MAX_HW_INTERRUPTS      +  3)
#define VIRT_MIPC_BUS_04        (MAX_HW_INTERRUPTS      +  4)
#define VIRT_MIPC_BUS_05        (MAX_HW_INTERRUPTS      +  5)
#define VIRT_MIPC_BUS_06        (MAX_HW_INTERRUPTS      +  6)
#define VIRT_MIPC_BUS_07        (MAX_HW_INTERRUPTS      +  7)


/******************************************************************************
 *
 * sysGetBasePhysAddr - return the base address of the VB
 *
 * RETURNS: an address
 *
 * ERRNO
 *
 * \NOMANUAL
 */

uint32_t sysGetBasePhysAddr ()
    {
    
    vbiPhysAddr_t physAddr;
    int retVal;
      
    vbiGuestDmaAddrGet((vbiGuestPhysAddr_t)0, &physAddr);

    /* TODO: 32-bit return value only supported in this API so convert
     * from 64 bit may need updating to support full 64-bit
     */

    retVal = (int)physAddr;
 
    if ((physAddr >> 32) != 0)
        { 
        vbiKputs("target/config/wrVb85XX/sysGetBasePhysAddr (): "
                 "ERROR: Physical addres returned from vbiGuestDmaAddrGet ()"
                 "is > 32 bits.");
        retVal = ERROR;
        }
        
    return retVal;

    }


/******************************************************************************
 *
 * sysGetVioapicAddr - Return vioapic mapped registers address
 *
 * RETURNS: an address
 *
 * ERRNO
 *
 * \NOMANUAL
 */
uint32_t sysGetVioapicAddr ()
    {
    return((uint32_t) VBI_VIOAPIC_BASE_GET ());
    }


/******************************************************************************
 *
 * interruptConversionTableSetup - VB vectors.
 *
 * This function loads hwIntTable and vbIntTable arrays with interrupt sources
 * to the VB vectors.  It is called once during initialization. The arrays are
 * used to convert between HW and VB vectors.
 *
 * The XXX_INT_VEC macros are defined in the <boardname>.h file. They represent
 * vxWorks' conception of the devices that _can_ issue interrupts.
 *
 * The "xxxx" string names are defined in the <guestOS>.xml file. They represent
 * hypervisor's list of devices that _will_ issue interrupts to this particular
 * guest.
 *
 * Note the array is statically allocated and filled with 0xff, this allows that
 * vxWorks at compile-time may "know about" far more interrupt sources than 
 * hypervisor will necessarily pass to it at run-time. This is to be expected.
 * Only the devices that are assigned, via matching string names in the xml
 * file, will get registered by matching the vbiIntVecFind call.
 * 
 *
 * RETURNS: NONE
 *
 * ERRNO
 *
 * \NOMANUAL
 */
void interruptConversionTableSetup ()
    {
    int     intNum;
    int32_t vector;

    bfill ((char*)hwIntTable, sizeof(hwIntTable), 0xff);
    bfill ((char*)vbIntTable, sizeof(vbIntTable), 0xff);

    for (intNum=0;intNum<(MAX_HW_INTERRUPTS + MAX_VIRTUAL_INTERRUPTS);intNum++)
	{
	switch (intNum)
	    {

            /* begin list of hw interrupt vectors, upto 256 */

/*
 * The XML interrupt naming convention for PCI interrupts changed between
 * Hypervisor 1.2 and 1.3. Under Hv1.2, the interrupt names are required to be
 * one of PCI_INT[A,B,C,D]. Under Hv1.3, the interrupt names are allowed to be
 * arbitrary. 
 *
 * An implicit side-effect of the Hv1.2 restriction is that a guest may only
 * have one PCI bridge assigned to it, as otherwise there would be no way to
 * distinguish the interrupts by name between two distinct PCI bridges. The
 * default implementation is to support PCIe 2 for the PCIe X1 slot.
 */

            case PCIEX1_XINT1_LVL:
                vector = vbiIntVecFind ("PCIE1_XINTA", VB_INPUT_INT);
                break;
            case PCIEX1_XINT2_LVL:
                vector = vbiIntVecFind ("PCIE1_XINTB", VB_INPUT_INT);
                break;
            case PCIEX1_XINT3_LVL:
                vector = vbiIntVecFind ("PCIE1_XINTC", VB_INPUT_INT);
                break;
            case PCIEX1_XINT4_LVL:
                vector = vbiIntVecFind ("PCIE1_XINTD", VB_INPUT_INT);
                break;
            case PCIEX2_XINT1_LVL:
                vector = vbiIntVecFind ("PCIE2_XINTA", VB_INPUT_INT);
                if (vector == VBI_INVALID_IRQ)
                    {
                    /* vector not found, try Hv 1.2 naming convention */
                    vector = vbiIntVecFind ("PCI_INTA", VB_INPUT_INT);
                    }
                break;
            case PCIEX2_XINT2_LVL:
                vector = vbiIntVecFind ("PCIE2_XINTB", VB_INPUT_INT);
                if (vector == VBI_INVALID_IRQ)
                    {
                    /* vector not found, try Hv 1.2 naming convention */
                    vector = vbiIntVecFind ("PCI_INTB", VB_INPUT_INT);
                    }
                break;
            case PCIEX2_XINT3_LVL:
                vector = vbiIntVecFind ("PCIE2_XINTC", VB_INPUT_INT);
                if (vector == VBI_INVALID_IRQ)
                    {
                    /* vector not found, try Hv 1.2 naming convention */
                    vector = vbiIntVecFind ("PCI_INTC", VB_INPUT_INT);
                    }
                break;
            case PCIEX2_XINT4_LVL:
                vector = vbiIntVecFind ("PCIE2_XINTD", VB_INPUT_INT);
                if (vector == VBI_INVALID_IRQ)
                    {
                    /* vector not found, try Hv 1.2 naming convention */
                    vector = vbiIntVecFind ("PCI_INTD", VB_INPUT_INT);
                    }
                break;

            case EPIC_DUART_INT_VEC:
                vector = vbiIntVecFind ("duart1", VB_INPUT_INT);
                if (vector == VBI_INVALID_IRQ)
                    vector = vbiIntVecFind ("duart2", VB_INPUT_INT);
                break;
            case EPIC_TSEC1TX_INT_VEC: 
                vector = vbiIntVecFind ("etsec1_tx", VB_INPUT_INT);
                break;
            case EPIC_TSEC1RX_INT_VEC: 
                vector = vbiIntVecFind ("etsec1_rx", VB_INPUT_INT);
                break;
            case EPIC_TSEC1ERR_INT_VEC:
                vector = vbiIntVecFind ("etsec1_err", VB_INPUT_INT);
                break;
            case EPIC_TSEC2TX_INT_VEC: 
                vector = vbiIntVecFind ("etsec2_tx", VB_INPUT_INT);
                break;
            case EPIC_TSEC2RX_INT_VEC: 
                vector = vbiIntVecFind ("etsec2_rx", VB_INPUT_INT);
                break;
            case EPIC_TSEC2ERR_INT_VEC:
                vector = vbiIntVecFind ("etsec2_err", VB_INPUT_INT);
                break;
            case EPIC_TSEC3TX_INT_VEC:
                vector = vbiIntVecFind ("etsec3_tx", VB_INPUT_INT);
                break;
            case EPIC_TSEC3RX_INT_VEC:
                vector = vbiIntVecFind ("etsec3_rx", VB_INPUT_INT);
                break;
            case EPIC_TSEC3ERR_INT_VEC:
                vector = vbiIntVecFind ("etsec3_err", VB_INPUT_INT);
                break;

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

            /* end list of hw interrupt vectors */

            /* begin list virtual interrupt vectors, upto 32 */

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

            /* end list of virtual interrupts vectors */

            default:
                /* if no match, do nothing and goto checking next intNum */
                continue;
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
 * --------------  Device Resource Structures  ------------------------
 */

#ifdef DRV_SIO_NS16550
const struct hcfResource ns1655x1Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)UART0_BASE} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { VXB_CLK_FREQ,  HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)
#endif /* DRV_SIO_NS16550 */


#ifdef INCLUDE_ETSEC_VXB_END
/*
 * On the P2020rdb board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins    MII address
 *      --------------  --------------------    -----------
 * PHY0:    ETSEC0       ETSEC0                 n/a, mdio is n.c.
 * PHY1:    ETSEC1       ETSEC0                 0
 * PHY2:    ETSEC2       ETSEC0                 1
 *
 * The tricky part is that all PHYs have their management pins
 * connected to ETSEC0. The VxBus ETSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 * 
 * For guestOS running under Hypervisor, Hypervisor virtualizes
 * accesses to the MII bus. The etsec driver has been modified to
 * make MII bus accesses through the Hypervisor. Other than needing
 * to set the actual phyAddr to pass to Hypervisor for each etsec
 * port, no modifications or special handling in the BSP is required.
 *
 * The "physMask" was originally implemented to support AMP on the 8641d.
 * On secondary cores it applied a bias to the virtual address so that 
 * DMA could be accessed directly without an intervening virt-to-phys 
 * translation call. This facilitated the same kernel being run in different
 * physical address spaces. For guestOS, physMask can be used in a similar
 * fashion by requesting from the Hypervisor the bias to apply to the virtual 
 * address in order to access DMA.
 *
 */

const struct hcfResource etsecVxbEnd0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "physMask",  HCF_RES_ADDR,   { (void *) sysGetBasePhysAddr} },
    { "phyAddr",   HCF_RES_INT,    { (void *) 0 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"dummyMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",   HCF_RES_INT,    { (void *)DEVICE_CORE0 } },
#   endif /* INCLUDE_AMP */
};
#define etsecVxbEnd0Num NELEMENTS(etsecVxbEnd0Resources)

const struct hcfResource etsecVxbEnd1Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "physMask",  HCF_RES_ADDR,   { (void *) sysGetBasePhysAddr} },
    { "phyAddr",   HCF_RES_INT,    { (void *) 0 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",   HCF_RES_INT,    { (void *)DEVICE_CORE1 } },
#   endif /* INCLUDE_AMP */
};
#define etsecVxbEnd1Num NELEMENTS(etsecVxbEnd1Resources)

const struct hcfResource etsecVxbEnd2Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "physMask",  HCF_RES_ADDR,   { (void *) sysGetBasePhysAddr} },
    { "phyAddr",   HCF_RES_INT,    { (void *) 1 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *) 0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",   HCF_RES_INT,    { (void *)DEVICE_CORE1 } },
#   endif /* INCLUDE_AMP */
};
#define etsecVxbEnd2Num NELEMENTS(etsecVxbEnd2Resources)

#ifdef INCLUDE_DUMMY_MDIO
const struct hcfResource dummyMdio0Resources[] =
    {
    { "regBase",            HCF_RES_INT,    { (void *)(0) } },
    { "fullDuplex",         HCF_RES_INT,    { (void *)1 } },
    { "speed",              HCF_RES_INT,    { (void *)1000 } }
    };
#   define dummyMdio0Num NELEMENTS(dummyMdio0Resources)
#endif /* INCLUDE_DUMMY_MDIO */

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] =
    {
    { "regBase",   HCF_RES_INT,  {(void *)(CCSBAR + 0x24000) } },
#   ifdef INCLUDE_AMP
    { "sharedMem", HCF_RES_INT,  {(void *)TM_ANCHOR_ADRS } }
#   endif /* INCLUDE_AMP */
    };
#   define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */

#endif /* INCLUDE_ETSEC_VXB_END */

#ifdef DRV_TIMER_VB
const struct hcfResource vbTimerResources[] = {
    { VXB_REG_BASE, HCF_RES_INT, {(void *)NULL} }
};
#define vbTimerNum NELEMENTS(vbTimerResources)
#endif /* DRV_TIMER_VB */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x410F0)}},
    { "instanceName", HCF_RES_STRING, {(void *)"A" }},
    { VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    { "clkDivisor",   HCF_RES_INT,    {(void *)8}},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC }},
    { "vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA1_INT_VEC }},
    { "vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA2_INT_VEC }},
    { "vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA3_INT_VEC }}
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    { VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x420F0)}},
    { "instanceName", HCF_RES_STRING, {(void *)"B" }},
    { VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    { "clkDivisor",   HCF_RES_INT,    {(void *)8}},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC }},
    { "vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB1_INT_VEC }},
    { "vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB2_INT_VEC }},
    { "vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB3_INT_VEC }}
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_RESOURCE_M85XXCCSR
const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",       HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",  HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR1",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR2",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR3",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR4",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR5",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR6",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR7",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR8",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR9",            HCF_RES_STRING, { (void *)NULL } }
};
#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)
#endif  /* DRV_RESOURCE_M85XXCCSR */

#ifdef INCLUDE_PCI_BUS
/* PCIe1 controller ---  mini-PCIe connector/bus */
const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)PCIEX1_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS + PCIEX1_IO_SIZE} },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciEx1AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx1AutoConfigIntAssign } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)0x40000000 } }, /* see target.ref */
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX1)} },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_IO   |
                                                       PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_PREFETCHABLE      |
                                                       PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                       PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                       PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,    { (void *)(TRUE) } },
    { "singleLawBarSize",   HCF_RES_INT,    { (void *)(0x10000000) } },
    { "autoConfig",         HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciEx2ErrataFlag",   HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciExpressHost",     HCF_RES_INT,    { (void *)(TRUE) } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

/* PCIe2 controller ---  PCIe X1 connector/bus */
const struct hcfResource m85xxPci2Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)PCIEX2_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS + PCIEX2_IO_SIZE} },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciEx2AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx2AutoConfigIntAssign } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)0x40000000 } }, /* see target.ref */
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX2)} },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_IO   |
                                                       PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_PREFETCHABLE      |
                                                       PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                       PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                       PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,    { (void *)(TRUE) } },
    { "singleLawBarSize",   HCF_RES_INT,    { (void *)(0x10000000) } },
    { "autoConfig",         HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciEx2ErrataFlag",   HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciExpressHost",     HCF_RES_INT,    { (void *)(TRUE) } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci2Num NELEMENTS(m85xxPci2Resources)

#endif  /* INCLUDE_PCI_BUS */

const struct intrCtlrInputs vbIntCtlrInputs[] = {
#ifdef DRV_SIO_NS16550
    { EPIC_DUART_INT_VEC,      "ns16550", 0, 0 },
#endif /* DRV_SIO_NS16550 */

#ifdef INCLUDE_ETSEC_VXB_END
    { EPIC_TSEC1TX_INT_VEC,    "motetsec", 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,    "motetsec", 0, 1 },
    { EPIC_TSEC1ERR_INT_VEC,   "motetsec", 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,    "motetsec", 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,    "motetsec", 1, 1 },
    { EPIC_TSEC2ERR_INT_VEC,   "motetsec", 1, 2 },
    { EPIC_TSEC3TX_INT_VEC,    "motetsec", 2, 0 },
    { EPIC_TSEC3RX_INT_VEC,    "motetsec", 2, 1 },
    { EPIC_TSEC3ERR_INT_VEC,   "motetsec", 2, 2 },
#endif /* INCLUDE_ETSEC_VXB_END */

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
    { "vIoapicBaseAddr",HCF_RES_ADDR,	{(void *)sysGetVioapicAddr} }
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


/*
 * --------------  Device List ----------------------------------------
 */

HCF_DEVICE hcfDeviceList[] = {
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "vbIntCtlr",  0, VXB_BUSID_PLB, 0, vbIntCtlrNum,   vbIntCtlrResources },

#ifdef DRV_TIMER_VB
    { "vxbVbTimerDev", 0, VXB_BUSID_PLB, 0, vbTimerNum,  vbTimerResources },
#endif /* DRV_TIMER_VB */

#ifdef INCLUDE_ETSEC_VXB_END
#if (ENABLE_ETSEC1 == TRUE)
    { "motetsec",   0, VXB_BUSID_PLB, 0, etsecVxbEnd0Num, etsecVxbEnd0Resources },
#endif
#if (ENABLE_ETSEC2 == TRUE)
    { "motetsec",   1, VXB_BUSID_PLB, 0, etsecVxbEnd1Num, etsecVxbEnd1Resources },
#endif
#if (ENABLE_ETSEC3 == TRUE)
    { "motetsec",   2, VXB_BUSID_PLB, 0, etsecVxbEnd2Num, etsecVxbEnd2Resources },
#endif
#ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio",      0,  VXB_BUSID_PLB,  0,  dummyMdio0Num,  dummyMdio0Resources },
#endif /* INCLUDE_DUMMY_MDIO */
#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio",       0,  VXB_BUSID_PLB,  0,  mdio0Num,       mdio0Resources },
#endif /* INCLUDE_TSEC_MDIO */
#endif /* INCLUDE_ETSEC_VXB_END */

#ifdef DRV_TIMER_OPENPIC
#if (ENABLE_TIMER_A == TRUE)
    { "openPicTimer", 0, VXB_BUSID_PLB, 0, openPicTimerDevAnum, openPicTimerDevAresources },
#endif
#if (ENABLE_TIMER_B == TRUE)
    { "openPicTimer", 1, VXB_BUSID_PLB, 0, openPicTimerDevBnum, openPicTimerDevBresources },
#endif
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR",      0,  VXB_BUSID_PLB,  0,  m85xxCCSR0Num,  m85xxCCSR0Resources },
#endif  /* DRV_RESOURCE_M85XXCCSR */
#ifdef INCLUDE_PCI_BUS
/* mini-PCIe bus has not been verified ...
 *  { "m85xxPci",       0,  VXB_BUSID_PLB,  0,  m85xxPci1Num,   m85xxPci1Resources },
 */
    { "m85xxPci",       0,  VXB_BUSID_PLB,  0,  m85xxPci2Num,   m85xxPci2Resources },
#endif  /* INCLUDE_PCI_BUS */

#ifdef DRV_SIO_NS16550
    /* The UART device gets disabled during initialization, specifically by the default
     * baud setting in ns16550vxbInitChannel(). Therefore, place ns16550 driver last in
     * the hcfDevice table since, when the UART baud is reset, all serial output, 
     * including that from the Hypervisor context such as calls to vbiKputs(), is stopped. 
     * WIND00255393
     */

    { "ns16550",    0, VXB_BUSID_PLB, 0, ns1655x1Num,    ns1655x1Resources },
#endif /* DRV_SIO_NS16550 */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
};
