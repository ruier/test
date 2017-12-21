/* sysPci.c - Freescale Lite5200B PCI support */

/*
 * Copyright (c) 2003-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,06oct08,b_m  do not enable PCI host controller interrupt.
01j,21aug08,wap  Add support for VxBus PCI devices via virtual/legacy PCI
                 driver
01i,27mar08,x_f  Removed the delay when reset PCI arbiter.
01h,18jan07,b_m  Add pci irq routing for lite5200b.
01g,03feb06,pkr  add support for Lite5200b
01f,01nov05,mdo  SPR#112287 - fix compile warnings
01e,31oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
01d,24jan05,pkr  Cleanup: remove CP2 switch and VWARE
01c,11aug05,j_b  add wireless LAN support
01b,04feb04,bjn  adding IS_ICECUBE_OLD
01a,07Aug03,pkr  written
*/

/* includes */

#include <vxWorks.h>
#include <logLib.h>
#include <taskLib.h>
#include <config.h>

/* typedefs */

/* local */

LOCAL void *vectorTbl[] = { IV_IRQ0, IV_IRQ1, IV_IRQ2, IV_IRQ3 };

/* include source files */

#include <drv/pci/pciConfigLib.h>
#include <pci/pciConfigLib.c>   /* PCI config space access */

#if (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD))
#include <pci/pciConfigShow.c>  /* display of PCI config space */
#endif /* INCLUDE_SHOW_ROUTINES */

/* defines */

#define PCICDR      ((volatile UINT32 *)(CPU_PCI_IO_ADRS))

/* forward declarations */

/*******************************************************************************
*
* sysPciSpecialCycle - generate a special cycle with a message
*
* This routine generates a special cycle with a message.
*
* RETURNS: OK
*/

LOCAL STATUS sysPciSpecialCycle
    (
    int     busNo,
    UINT32  message
    )
    {
    int deviceNo    = 0x0000001f;
    int funcNo      = 0x00000007;

    if (busNo != 0)
        return ERROR;

    *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) | 0x80000000;
    PCI_OUT_LONG (PCICDR, message);
    *PCICAR = 0;

    return (OK);
    }

/*******************************************************************************
*
* sysPciConfigRead - read from the PCI configuration space
*
* This routine reads either a byte, word or a long word specified by
* the argument <width>, from the PCI configuration space
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* SEE ALSO: sysPciConfigWrite()
*/

LOCAL STATUS sysPciConfigRead
    (
    int     busNo,      /* bus number */
    int     deviceNo,   /* device number */
    int     funcNo,     /* function number */
    int     offset,     /* offset into the configuration space */
    int     width,      /* width to be read */
    void *  pData       /* data read from the offset */
    )
    {
    UINT8  retValByte = 0;
    UINT16 retValWord = 0;
    UINT32 retValLong = 0;
    STATUS retStat = ERROR;
    int status;

    switch (width)
        {
        case 1: /* byte */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            retValByte = PCI_IN_BYTE (PCICDR + (offset & 0x3));
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            *((UINT8 *)pData) = retValByte;
            retStat = ((status & PCIISR_IA) ? ERROR : OK);
            break;

        case 2: /* word */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            retValWord = PCI_IN_WORD (PCICDR + (offset & 0x2));
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            *((UINT16 *)pData) = retValWord;
            retStat = ((status & PCIISR_IA) ? ERROR : OK);
            break;

        case 4: /* long */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            retValLong = PCI_IN_LONG (PCICDR);
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            *((UINT32 *)pData) = retValLong;
            retStat = ((status & PCIISR_IA) ? ERROR : OK);
            break;

        default:
            retStat = ERROR;
            break;
        }

    return retStat;
    }

/*******************************************************************************
*
* sysPciConfigWrite - write to the PCI configuration space
*
* This routine writes either a byte, word or a long word specified by
* the argument <width>, to the PCI configuration space
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* SEE ALSO: sysPciConfigRead()
*/

LOCAL STATUS sysPciConfigWrite
    (
    int     busNo,      /* bus number */
    int     deviceNo,   /* device number */
    int     funcNo,     /* function number */
    int     offset,     /* offset into the configuration space */
    int     width,      /* width to write */
    ULONG   data        /* data to write */
    )
    {
    UINT32 status = OK;

    if ((busNo == 0) && (deviceNo == 12))
        return (ERROR);

    switch (width)
        {
        case 1: /* byte */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            PCI_OUT_BYTE ((PCICDR + (offset & 0x3)), data);
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            break;

        case 2: /* word */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            PCI_OUT_WORD ((PCICDR + (offset & 0x2)), data);
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            break;

        case 4: /* long */
            *PCICAR = pciConfigBdfPack (busNo, deviceNo, funcNo) |
                      (offset & 0xfc) | 0x80000000;
            PCI_OUT_LONG (PCICDR, data);
            *PCICAR = 0;
            status = *PCIISR;
            *PCIISR = PCIISR_IA; /* clear */
            break;

        default:
            return (ERROR);
        }

    return ((status & PCIISR_IA) ? ERROR : OK);
    }

/*******************************************************************************
*
* setInitiatorWindow - PCI outbound memory window initialization
*
* This routine initializes the outbound memory windows.
*
* RETURNS: N/A
*/

void setInitiatorWindow
    (
    int    nr,
    UINT32 cpuBase,
    UINT32 busBase,
    UINT32 size,
    UINT8  mode
    )
    {
    UINT32 cpu, bus, mask, btarValue;

    if ((cpuBase & 0x00ffffff) != 0)
        return;

    if ((busBase & 0x00ffffff) != 0)
        return;

    cpu = cpuBase >> 24;
    bus = busBase >> 24;
    mask = (size >> 24) - 1;

    btarValue = cpu<<24|mask<<16|bus<<8;

    switch (nr)
        {
        case 0:
            *PCIIW0BTAR = btarValue;
            EIEIO;
            *PCIIWCR &= 0xf0ffffff;
            *PCIIWCR |= mode<<24;
            EIEIO;
            break;
        case 1:
            *PCIIW1BTAR = btarValue;
            EIEIO;
            *PCIIWCR &= 0xfff0ffff;
            *PCIIWCR |= mode<<16;
            EIEIO;
            break;
        case 2:
            *PCIIW2BTAR = btarValue;
            EIEIO;
            *PCIIWCR &= 0xfffff0ff;
            *PCIIWCR |= mode<<8;
            EIEIO;
            break;
        default:
            return;
        }
    }

/*******************************************************************************
*
* sysPciInit - PCI Configuration Library Initialization
*
* This routine initialize the PCI configuration library.
*
* RETURNS: N/A
*/

STATUS sysPciInit (void)
    {
    STATUS retVal;

    /* set GPIO correctly -> enable PCI */

    *GPIO_STD_PORTCFG &= ~GPIO_STD_PC_PCI_DISABLED;

    /* MPC5200 is the PCI bus master */

    *PCISCR |= PCISCR_B;
    *PCISCR |= PCISCR_M;
    *PCISCR |= PCISCR_MW;

    /*
     * Set latency timer (minimum time the bus master can keep the bus after
     * starting a transaction)
     */

    *PCICR1 &= ~PCICR1_LAT_MASK;
    *PCICR1 |= (PCI_LAT_TIMER << PCICR1_LAT_SHIFT);

    /* Set up Max latency and Minimum bus grant */

    *PCICR2 = ((UINT32)PCICR2_MAX_LAT << 24);  /* 254/4 = 64 useconds */
    *PCICR2 |= (PCICR2_MIN_GNT << 16);
    EIEIO;

    /* m5200 as target */

#if (BUS_PCI_SLV_MEM_LOCAL_A & 0x0003FFFF)
#   error
#endif
#if (BUS_PCI_SLV_MEM_LOCAL_A & 0x3FFFFFF)
#   error
#endif
#if (CPU_PCI_SLV_MEM_LOCAL_A & 0x0003FFFF)
#   error
#endif
#if (CPU_PCI_SLV_MEM_LOCAL_B & 0x3FFFFFFF)
#   error
#endif

    /* Target SDRAM/DDR memory address */

    *PCIBAR0 = BUS_PCI_SLV_MEM_LOCAL_A;
    *PCIBAR1 = BUS_PCI_SLV_MEM_LOCAL_B;

    /* set address and enable */

    *PCITBATR0 = CPU_PCI_SLV_MEM_LOCAL_A | 1;
    *PCITBATR1 = CPU_PCI_SLV_MEM_LOCAL_B | 1;

    /* m5200 as master */

    /* Initiator 0 for 32 bit prefetch memory */

    setInitiatorWindow (0, CPU_PCI_MEM_ADRS, BUS_PCI_MEM_ADRS, PCI_MEM_SIZE,
                        PCIIWCR_ENABLE|PCIIWCR_MEM|PCIIWCR_READ_MULTI);

    /* Initiator 1 for IO */

    setInitiatorWindow (1, CPU_PCI_IO_ADRS, BUS_PCI_IO_ADRS, PCI_IO_SIZE,
                        PCIIWCR_ENABLE|PCIIWCR_IO);

    /* Initiator 2 for 32 bit non-prefetch memory */

    setInitiatorWindow (2, CPU_PCI_NO_PRE_MEM_ADRS, BUS_PCI_NO_PRE_MEM_ADRS,
                        PCI_NO_PRE_MEM_SIZE,
                        PCIIWCR_ENABLE|PCIIWCR_MEM|PCIIWCR_READ_SINGLE);

    /* mask interrupts here, retry count 256 */

    *PCIICR = 0xFF;

    /* clear any pending int */

    *PCIISR = PCIISR_RE|PCIISR_IA|PCIISR_TA;

    /* disable config space IO */

    *PCICAR = 0;

    /* clear pending errors, mask interrupts, deassert reset */

    *PCIGSCR = PCIGSCR_BM|PCIGSCR_PE|PCIGSCR_SE;

    *PCIARB = PCIARB_RESET; /* reset arbiter */
    *PCIARB = 0;

    /* active low on PCI IRQ line */
    /* be sure to mask while changing */

    *ICTL_EEETR &= ~((UINT32)(ICTL_EEETR_MEE |  \
                    ICTL_EEETR_ETYPE0_MASK | ICTL_EEETR_ETYPE1_MASK |   \
                    ICTL_EEETR_ETYPE2_MASK | ICTL_EEETR_ETYPE3_MASK));
    *ICTL_EEETR |= (ICTL_EEETR_MEE |    \
                    ICTL_EEETR_ETYPE0_LEVEL_LO | ICTL_EEETR_ETYPE1_LEVEL_LO |   \
                    ICTL_EEETR_ETYPE2_LEVEL_LO | ICTL_EEETR_ETYPE3_LEVEL_LO);

    retVal = pciConfigLibInit (PCI_MECHANISM_0, (ULONG) sysPciConfigRead,
                               (ULONG) sysPciConfigWrite,
                               (ULONG) sysPciSpecialCycle);
    return (retVal);
    }

/*******************************************************************************
*
* sysPciVectorMap - map PCI interrupt pin to vector
*
* This routine maps PCI interrupt pin to vector.
*
* RETURNS: N/A
*/

void *sysPciVectorMap
    (
    int pciIntLine
    )
    {
    /* adjust interrupts value */

    if (pciIntLine >= NELEMENTS(vectorTbl))
        {
        return(NULL);
        }

    return (vectorTbl[pciIntLine]);
    }

/*******************************************************************************
*
* sysPciVirtToBus - translate local address to PCI address
*
* This routine is used by the virtual PCI adaptor driver to translate an address
* from local RAM address space to PCI bus space for the purposes of DMA.
*
* RETURNS: N/A
*/

void * sysPciVirtToBus
    (
    void * addr
    )
    {
    UINT32 a;

    a = (UINT32)addr;
    a = (a - CPU_PCI_SLV_MEM_LOCAL_B + BUS_PCI_SLV_MEM_LOCAL_B);
    return ((void *)a);
    }

/*******************************************************************************
*
* sysPciBusToVirt - translate PCI address to local address
*
* This routine is used by the virtual PCI adaptor driver to translate an address
* from PCI bus space to local RAM address space for the purposes of DMA.
*
* RETURNS: N/A
*/

void * sysPciBusToVirt
    (
    void * addr
    )
    {
    UINT32 a;

    a = (UINT32)addr;
    a = (a - BUS_PCI_SLV_MEM_LOCAL_B + CPU_PCI_SLV_MEM_LOCAL_B);
    return ((void *)a);
    }

/*******************************************************************************
*
* sysPciIntEnable - enable PCI device interrupt
*
* This routine is used by the virtual PCI adaptor driver to enable a given
* VxBus PCI driver's interrupt.
*
* RETURNS: N/A
*/

void sysPciIntEnable
    (
    int vector
    )
    {
    intEnable(IVEC_TO_INUM(sysPciVectorMap(vector)));
    return;
    }

/*******************************************************************************
*
* sysPciIntDisable - disable PCI device interrupt
*
* This routine is used by the virtual PCI adaptor driver to disable a given
* VxBus PCI driver's interrupt.
*
* RETURNS: N/A
*/

void sysPciIntDisable
    (
    int vector
    )
    {
    intDisable(IVEC_TO_INUM(sysPciVectorMap(vector)));
    return;
    }

/*******************************************************************************
*
* sysPciIntConnect - connect PCI device interrupt
*
* This routine is used by the virtual PCI adaptor driver to connect a given
* VxBus PCI driver's interrupt.
*
* RETURNS: N/A
*/

STATUS sysPciIntConnect
    (
    VOIDFUNCPTR *   vector,
    VOIDFUNCPTR     routine,
    int             parameter
    )
    {
    return (intConnect (sysPciVectorMap((int)vector), routine, parameter));
    }

/*******************************************************************************
*
* sysPciIntDisconnect - disconnect PCI device interrupt
*
* This routine is used by the virtual PCI adaptor driver to disconnect a given
* VxBus PCI driver's interrupt.
*
* RETURNS: N/A
*/

STATUS sysPciIntDisconnect
    (
    VOIDFUNCPTR *   vector,
    VOIDFUNCPTR     routine,
    int             parameter
    )
    {
    return (intDisconnect (sysPciVectorMap((int)vector), routine, parameter));
    }
