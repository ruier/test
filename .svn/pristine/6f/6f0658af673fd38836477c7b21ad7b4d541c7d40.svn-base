/* usbPciStub.c - System-specific skeleton file to support USB stack porting */

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
01c,15sep09,w_x  rename host and device hwinit routines (WIND00180575)
01b,11mar09,w_x  add INCLUDE_FSL_TCD support routines
01a,03feb09,w_x  created
*/

/*
DESCRIPTION

This file is used as a skeleton file to support USB stack porting.
To keep backword compatibility, the file is named as "usbPciStub.c".
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <usb/usbHst.h>
#include <hwif/util/vxbParamSys.h>

/* defines */

#define SYS_IN_LONG(port)           \
    *(volatile UINT32 *)((port))
#define SYS_OUT_LONG(port, data)    \
    *(volatile UINT32 *)((port)) = (UINT32)(data)

/*
 * Offsets for the non-EHCI registers in the FSL Dual Role USB controller.
 * If not noted otherwise, the register endian depends on silicon variations.
 */

#define FSL_USB_SBUSCFG             0x90

#define SBUSCFG_INCR8               0x02        /* INCR8 */

#define FSL_USB_BURSTSIZE           0x160
#define FSL_USB_ULPIVP              0x170

#define FSL_USB_PORTSC1             0x184
#define PORT_PTS_MSK                (3 << 30)   /* Port Transceiver Select mask */
#define PORT_PTS_UTMI               (0 << 30)   /* UTMI parallel interface */
#define PORT_PTS_ULPI               (2 << 30)   /* ULPI parallel interface */
#define PORT_PTS_SERIAL             (3 << 30)   /* FS/LS serial interface  */
#define PORT_PTS_PTW                (1 << 28)   /* Parallel Transceiver Width */
#define PORT_PTS_PHCD               (1 << 23)   /* PHY Low Power Suspend */

#define FSL_USB_PORTSC2             0x188
#define FSL_USB_OTGSC               0x1a4

#define FSL_USB_USBMODE             0x1a8
#define USBMODE_CM_MASK             (3 << 0)    /* Controller Mode mask */
#define USBMODE_CM_OTG              (0 << 0)    /* Combination host/device */
#define USBMODE_CM_DEVICE           (2 << 0)    /* Device Controller */
#define USBMODE_CM_HOST             (3 << 0)    /* Host Controller */
#define USBMODE_ES                  (1 << 2)    /* (Big) Endian Select */

#define FSL_USB_USBGENCTRL          0x200       /* Note: Always big endian */
#define USBGENCTRL_WU_INT_CLR       (1 << 5)    /* Wakeup int clear */
#define USBGENCTRL_ULPI_SEL         (1 << 4)    /* ULPI select (USB0 only) */
#define USBGENCTRL_PPP              (1 << 3)    /* Port Power Polarity */
#define USBGENCTRL_PFP              (1 << 2)    /* Power Fault Polarity */
#define USBGENCTRL_WU_ULPI_EN       (1 << 1)    /* Wakeup on ULPI event */
#define USBGENCTRL_WU_IE            (1 << 1)    /* Wakeup int enable */

#define FSL_USB_ISIPHYCTRL          0x204       /* Note : Always big endian */
#define ISIPHYCTRL_PHYE             (1 << 4)    /* On-chip UTMI PHY enable */
#define ISIPHYCTRL_BSENH            (1 << 3)    /* Bit Stuff Enable High */
#define ISIPHYCTRL_BSEN             (1 << 2)    /* Bit Stuff Enable */
#define ISIPHYCTRL_LSFE             (1 << 1)    /* Line State Filter Enable */
#define ISIPHYCTRL_PXE              (1 << 0)    /* PHY oscillator enable */

#define SVR_CID_SID_MPC5121E                        (0x8018)
#define SVR_MJREV_MPC5121E_USB_REG_ENDIAN_CHANGED   (2)


#define FSL_USB_WRITE_UINT32(addr, val) \
    {                                   \
    if (fslUsbRegBigEndian == TRUE)     \
        SYS_OUT_LONG (addr, val);       \
    else                                \
        sysOutLong (addr, val);         \
    }

#define FSL_USB_READ_UINT32(addr)       \
    (fslUsbRegBigEndian == TRUE) ? SYS_IN_LONG (addr) : sysInLong (addr)

extern VXB_INST_PARAM_OVERRIDE sysInstParamTable[];
extern UINT32 vxSvrGet(void);

BOOL   fslUsbRegBigEndian = TRUE;

/*******************************************************************************
*
* fsl5121eUsbEndianProbe - probe the endian mode of the USB module
*
* This routine is to probe the endian mode of the USB module.
*
* If the register endian is little endian, it will update "regBigEndian"
* field in the sysInstParamTable to be FALSE. The "regBigEndian" field
* defaults to be TRUE (_BIG_ENDIAN).
*
* All MPC5121E revisions are _BIG_ENDIAN descriptors. So no need to probe
* descriptor endian in this routine.
*
* RETURNS: N/A
*/

void fsl5121eUsbEndianProbe (void)
    {
    UINT32                      svr;
    UINT32                      chip;
    UINT32                      rev;
    VXB_INST_PARAM_OVERRIDE *   bspEntry;

    /* Get SVR */

    svr  = vxSvrGet();

    /* Get chip info and rev */

    chip = svr >> 16;
    rev  = (svr >> 4) & 0xf;

    /*
     * ON MPC5121E, CPU rev smaller than 2
     * is little endian register for the USB module.
     *
     * Note, this has been confirmed by FSL support
     * that this condition is enough to distigush the
     * endian format.
     */

    /*
     * Another approch to distigush register endian
     * format: read out the UINT32 from CAPLENGTH
     * address, check if it is 0x40 (should be
     * hard code for MPC5121E), and thus we can tell
     * the endian format. But this approch can only
     * be appied to probe register endian format, so
     * we don't use it here.
     */

    if ((chip == SVR_CID_SID_MPC5121E) &&
        (rev < SVR_MJREV_MPC5121E_USB_REG_ENDIAN_CHANGED))
        {
        /* Check BSP-supplied table */

        bspEntry = &sysInstParamTable[0];
        while (bspEntry->instName != NULL)
            {
            /* Override "regBigEndian" field to indicate _LITTLE_ENDIAN */

            if ((strcmp(bspEntry->instName, "vxbPlbUsbEhci") == 0) &&
                (strcmp(bspEntry->paramName, "regBigEndian") == 0))
                {
                bspEntry->value.int32Val = FALSE;
                fslUsbRegBigEndian = FALSE;
                break;
                }
            bspEntry++;
            }
        }

    return;
    }

/*******************************************************************************
*
* fsl5121eUsbInit - do board specfic initilization for the USB module
*
* This routine is to do board specfic initilization for the USB module.
*
* RETURNS: NONE
*/

void fsl5121eUsbInit (void)
    {
    UINT32 portsc;

    /*
     * Override the register endian format - this should be
     * the first routine called before any register access
     * that uses the macros above
     */

    fsl5121eUsbEndianProbe();

    /*
     * USB CLK should have been initialized during CLK module initialization;
     *
     * If there are pin mux that should be setup in order to make the
     * USB module work, it will also need to be done here.
     *
     * USB PHY initialization is done in this routine.
     */

    /*
     * FSL_USB_ISIPHYCTRL and FSL_USB_USBGENCTRL are in
     * big endian format on this SOC.
     */

    /* PHY enable and PHY OSC enable */

    SYS_OUT_LONG(USB0_BA + FSL_USB_ISIPHYCTRL,
                 ISIPHYCTRL_PHYE | ISIPHYCTRL_PXE);

    /* PHY Port Power Poliarity and Power Fault Polarity */

    SYS_OUT_LONG(USB0_BA + FSL_USB_USBGENCTRL,
                 USBGENCTRL_PPP | USBGENCTRL_PFP);

    /*
     * Set up "Port Transceiver Select" and "Parallel Transceiver Width"
     * to use internal 16bit PHY.
     */

    portsc = FSL_USB_READ_UINT32(USB0_BA + FSL_USB_PORTSC1);

    /* Clear the field mask */

    portsc &= ~(PORT_PTS_MSK | PORT_PTS_PTW);

    /* 16bit UTMI */

    portsc |= (PORT_PTS_PTW | PORT_PTS_UTMI);

    FSL_USB_WRITE_UINT32(USB0_BA + FSL_USB_PORTSC1, portsc);
    }

/*******************************************************************************
*
* fslUsbDrDeviceHwInit - do board specfic initilization for USB device module
*
* This routine is to do board specfic initilization for the USB device module.
*
* RETURNS: NONE
*/

void fslUsbDrDeviceHwInit (void)
    {
    fsl5121eUsbInit ();
    }

/*******************************************************************************
*
* fslUsbDrHostHwInit - do board specfic initilization for USB host module
*
* This routine is to do board specfic initilization for the USB host module.
*
* RETURNS: NONE
*/

void fslUsbDrHostHwInit (void)
    {
    fsl5121eUsbInit ();
    }

/*******************************************************************************
*
* fslUsbDrHostModeSet - set the dual role USB module operation as USB host
*
* This routine is to set the dual role USB module operation as USB host.
*
* RETURNS: NONE
*/

void fslUsbDrHostModeSet (void)
    {
    UINT32 temp;

    /*
     * USBMODE register has non-EHCI compliant "Controller Mode"
     * and "Endian Select" field. The "Controller Mode" field must
     * be set to appropriate operation mode after the moudle has been
     * reset. The reset operation is done in the USB stack, so we
     * use this routine as the "postResetHook" to set it as host mode.
     */

    temp = FSL_USB_READ_UINT32(USB0_BA + FSL_USB_USBMODE);

    /* Clear the field mask */

    temp &= ~(USBMODE_CM_MASK | USBMODE_ES);

    /*
     * Set as host mode; the CPU is big endian, so the buffer
     * endian select bit should also be set.
     */

    temp |= USBMODE_CM_HOST | USBMODE_ES;

    FSL_USB_WRITE_UINT32(USB0_BA + FSL_USB_USBMODE, temp);

    /*
     * Set SBUSCFG::AHBBRST INCR8 to so that control messages don't
     * fail for heavy PATA writes.
     */

    FSL_USB_WRITE_UINT32(USB0_BA + FSL_USB_SBUSCFG, SBUSCFG_INCR8);
    }

/*
 * TCD has not transformed to use vxBus mechanism,
 * so we still need these pci stubs to work as bsp porting layer,
 * even the controller is actually PLB based.
 */

#if defined (INCLUDE_FSL_TCD)

void fslTcdCtlrInfoGet
    (
    UINT32 *    pBaseAddr,
    UINT32 *    pIrqNum
    )
    {
    *pBaseAddr = (UINT32)USB0_BA;

    *pIrqNum = (UINT32)INUM_USB_UTMI;
    }

UINT16 usbPciWordIn
    (
    UINT32 pMem                 /* memory address to convert */
    )
    {
    return *(UINT32*)pMem;
    }

UINT32 usbPciDwordIn
    (
    UINT32 pMem                 /* memory address to convert */
    )
    {
    return *(UINT32*)pMem;
    }

UINT8 usbPciByteIn
    (
    UINT32 pMem                 /* memory address to convert */
    )
    {
    return *(UINT32*)pMem;
    }

void usbPciWordOut
    (
    UINT32 pMem,                /* memory address to convert */
    UINT16 data
    )
    {
    *(UINT32*)pMem = data;
    }

void usbPciDwordOut
    (
    UINT32 pMem,
    UINT32 data                 /* memory address to convert */
    )
    {
    *(UINT32*)pMem = data;
    }

void usbPciByteOut
    (
    UINT32 pMem,                /* memory address to convert */
    UINT8 data
    )
    {
    *(UINT32*)pMem = data;
    }

/*******************************************************************************
*
* usbPciWordGet - Returns a UINT16 configuration value
*
* This function returns the UINT16 value at offset <regOffset> from
* the PCI configuration space of the device identified by <busNo>,
* <deviceNo>, and <funcNo>.
*
* NOTE: This function adjusts for big vs. little endian environments.
*
* RETURNS: UINT16 value read from device configuration space
*/

UINT32 usbPciWordGet
    (
    UINT8 busNo,            /* Bus number of device */
    UINT8 deviceNo,         /* Device number of device */
    UINT8 funcNo,           /* Function number of device */
    UINT16 regOffset        /* Offset into PCI config space */
    )
    {
    return 0;
    }

/*******************************************************************************
*
* usbPciDwordGet - Returns a UINT32 configuration value
*
* This function returns the UINT32 value at offset <regOffset> from
* the PCI configuration space of the device identified by <busNo>,
* <deviceNo>, and <funcNo>.
*
* NOTE: This function adjusts for big vs. little endian environments.
*
* RETURNS: UINT32 value read from device configuration space
*/

UINT32 usbPciDwordGet
    (
    UINT8 busNo,            /* Bus number of device */
    UINT8 deviceNo,         /* Device number of device */
    UINT8 funcNo,           /* Function number of device */
    UINT16 regOffset        /* Offset into PCI config space */
    )
    {
    return 0;
    }

/*******************************************************************************
*
* usbPciMemioOffset - Return PCI MEMIO to CPU MEMIO offset
*
* For memory-mapped I/O, the CPU's view of a memory address may not be the
* same as that programmed into the base address register of a PCI adapter.
* The CPU should add the value returned by this function to the BAR in order
* to produce the correct CPU-visible memory address.
*
* RETURNS: USB_PCI_MEMIO_OFFSET
*/

UINT32 usbPciMemioOffset (void)
    {
    return 0;
    }

/*******************************************************************************
*
* usbMemToPci - Convert a memory address to a PCI-reachable memory address
*
* Converts <pMem> to an equivalent 32-bit memory address visible from the
* PCI bus.  This conversion is necessary to allow PCI bus masters to address
* the same memory viewed by the processor.
*
* RETURNS: converted memory address
*/

UINT32 usbMemToPci
    (
    pVOID pMem  /* memory address to convert */
    )
    {

    /*
     * The conversion is a two-step process.  First, we need to convert the
     * logical processor address (virtual) to a physical address.  Then, we
     * convert the physical address to one which can be seen from the PCI
     * bus.
     */

    /* 1:1 */

    return (UINT32)pMem;
    }

/*******************************************************************************
*
* usbPciToMem - Convert a PCI-reachable address to a CPU-reachable pointer
*
* Converts <pciAdrs> to an equivalent CPU memory address.
*
* RETURNS: pointer to PCI memory
*/

pVOID usbPciToMem
    (
    UINT32 pciAdrs  /* 32-bit PCI address to be converted */
    )
    {
    /* 1:1 */

    return (void *)(pciAdrs);
    }

/*******************************************************************************
*
* usbPciIntConnect - Connect to a interrupt vector
*
* Connects the <func> to the interrupt number <intNo>.  <param> is an
* application-specific value which will be passed to <func> each time
* the interrupt handler is invoked.
*
* RETURNS: OK, or ERROR if unable to connect/enable interrupt
*/

STATUS usbPciIntConnect
    (
    VOIDFUNCPTR func,       /* new interrupt handler */
    pVOID       param,      /* parameter for int handler */
    UINT16      intNo       /* interrupt vector number */
    )
    {
    UINT32 vec = (UINT32)intNo ;
    UINT32 level = (UINT32)intNo;

    if (intConnect((VOIDFUNCPTR *)vec, func, (int)param) != OK)
        return ERROR;

    if (intEnable(level) != OK)
        {
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usbPciIntDisconnect - Removes an interrupt handler
*
* Removes an interrupt handler installed by usbPciIntConnect().  <func>,
* <param>, and <intNo> must match the corresponding parameters from an earlier
* call to usbPciIntConnect().
*
* RETURNS: N/A
*/

VOID usbPciIntRestore
    (
    VOIDFUNCPTR func,       /* int handler to be removed */
    pVOID       param,      /* parameter for int handler */
    UINT16      intNo       /* interrupt vector number */
    )
    {
    UINT32 vec = (UINT32)intNo ;
    UINT32 level = (UINT32)intNo;

    if (intDisconnect((VOIDFUNCPTR *)vec, func, (int)param) != OK)
        return;

    intDisable(level);
    }

#endif /* INCLUDE_FSL_TCD */
