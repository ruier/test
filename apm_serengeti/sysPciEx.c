/* sysPciEx.c - APM Serengeti board PCI Express BSP stubs */

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
01b,30jul11,x_z  add sysPciExAutoCfgInclude routines;
                 fix SERDES reset issue.
01a,12jul11,b_m  written.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <drv/pci/pciAutoConfigLib.h>

/* defines */

#define PCS_TIMEOUT         1000
#define PCS_DELAY_IN_US     10
#define PCS_RST_DELAY_IN_US 1000

/* locals */

LOCAL UCHAR sysPciEx0IntRoute[4][4] = {
    { PCIEX0_INT_A, PCIEX0_INT_B, PCIEX0_INT_C, PCIEX0_INT_D },
    { PCIEX0_INT_B, PCIEX0_INT_C, PCIEX0_INT_D, PCIEX0_INT_A },
    { PCIEX0_INT_C, PCIEX0_INT_D, PCIEX0_INT_A, PCIEX0_INT_B },
    { PCIEX0_INT_D, PCIEX0_INT_A, PCIEX0_INT_B, PCIEX0_INT_C },
};

LOCAL UCHAR sysPciEx1IntRoute[4][4] = {
    { PCIEX1_INT_A, PCIEX1_INT_B, PCIEX1_INT_C, PCIEX1_INT_D },
    { PCIEX1_INT_B, PCIEX1_INT_C, PCIEX1_INT_D, PCIEX1_INT_A },
    { PCIEX1_INT_C, PCIEX1_INT_D, PCIEX1_INT_A, PCIEX1_INT_B },
    { PCIEX1_INT_D, PCIEX1_INT_A, PCIEX1_INT_B, PCIEX1_INT_C },
};

LOCAL UCHAR sysPciEx2IntRoute[4][4] = {
    { PCIEX2_INT_A, PCIEX2_INT_B, PCIEX2_INT_C, PCIEX2_INT_D },
    { PCIEX2_INT_B, PCIEX2_INT_C, PCIEX2_INT_D, PCIEX2_INT_A },
    { PCIEX2_INT_C, PCIEX2_INT_D, PCIEX2_INT_A, PCIEX2_INT_B },
    { PCIEX2_INT_D, PCIEX2_INT_A, PCIEX2_INT_B, PCIEX2_INT_C },
};

/*******************************************************************************
*
* sysPciEx0AutoCfgInclude - PCI Express 0 autoconfig support routine
*
* This routine performs the PCI Express 0 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx0AutoCfgInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC *    pLoc,          /* pointer to function in question */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /*  Host controller itself (device number is 0) won't be configured */

    if ((pLoc->bus == 0) && (pLoc->device == 0) && (pLoc->function == 0))
        return (ERROR);

    return (OK);
    }

/*******************************************************************************
*
* sysPciEx0AutoCfgIntAssign - autoconfig PCIEX0 interrupt line number
*
* This routine assigns the interrupt line number for the special PCIEX0 int pin.
*
* RETURNS: PCIEX0 interrupt line number for the given pin
*/

UCHAR sysPciEx0AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UCHAR           pin             /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xFF;

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPciEx0IntRoute[(pLoc->device) & 3][pin - 1];

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPciEx1AutoCfgInclude - PCI Express 1 autoconfig support routine
*
* This routine performs the PCI Express 1 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx1AutoCfgInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC *    pLoc,          /* pointer to function in question */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /*  Host controller itself (device number is 0) won't be configured */

    if ((pLoc->bus == 0) && (pLoc->device == 0) && (pLoc->function == 0))
        return (ERROR);

    return (OK);
    }

/*******************************************************************************
*
* sysPciEx1AutoCfgIntAssign - autoconfig PCIEX1 interrupt line number
*
* This routine assigns the interrupt line number for the special PCIEX1 int pin.
*
* RETURNS: PCIEX1 interrupt line number for the given pin
*/

UCHAR sysPciEx1AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UCHAR           pin             /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xFF;

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPciEx1IntRoute[(pLoc->device) & 3][pin - 1];

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPciEx2AutoCfgInclude - PCI Express 2 autoconfig support routine
*
* This routine performs the PCI Express 2 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx2AutoCfgInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC *    pLoc,          /* pointer to function in question */
    UINT         devVend        /* deviceID/vendorID of device */
    )
    {

    /*  Host controller itself (device number is 0) won't be configured */

    if ((pLoc->bus == 0) && (pLoc->device == 0) && (pLoc->function == 0))
        return (ERROR);

    return (OK);
    }

/*******************************************************************************
*
* sysPciEx2AutoCfgIntAssign - autoconfig PCIEX2 interrupt line number
*
* This routine assigns the interrupt line number for the special PCIEX2 int pin.
*
* RETURNS: PCIEX2 interrupt line number for the given pin
*/

UCHAR sysPciEx2AutoCfgIntAssign
    (
    PCI_SYSTEM *    pSys,           /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,           /* pointer to function in question */
    UCHAR           pin             /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xFF;

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPciEx2IntRoute[(pLoc->device) & 3][pin - 1];

    return (tmpChar);
    }

/*******************************************************************************
*
* pcsX4RegRdInd - read PCIEX4 PCS register
*
* This routine reads the content of the PCIEX4 PCS registers.
*
* RETURNS: the content of the PCIEX4 PCS registers
*/

LOCAL UINT16 pcsX4RegRdInd
    (
    UINT32 regAddr
    )
    {
    UINT32 cmdDone;
    UINT16 data;

    sysWrite32 (PCS_X4_REG_ADDR (APM_PCS2_CSR_BASE), regAddr);
    sysWrite32 (PCS_X4_REG_CMD (APM_PCS2_CSR_BASE), PCS_X4_REG_CMD_RD);

    while (1)
        {
        cmdDone = sysRead32 (PCS_X4_REG_CMD_DONE (APM_PCS2_CSR_BASE));
        if (cmdDone & PCS_X4_REG_CMD_RD_DONE)
            break;
        }

    data = sysRead32 (PCS_X4_REG_RD_DATA (APM_PCS2_CSR_BASE));
    return (data & 0xFFFF);
    }

/*******************************************************************************
*
* pcsX4RegRdInd - write PCIEX4 PCS register
*
* This routine writes the data to the PCIEX4 PCS registers.
*
* RETURNS: N/A
*/

LOCAL void pcsX4RegWtInd
    (
    UINT32 regAddr,
    UINT16 data
    )
    {
    UINT32 cmdDone;

    sysWrite32 (PCS_X4_REG_ADDR (APM_PCS2_CSR_BASE), regAddr);
    sysWrite32 (PCS_X4_REG_WR_DATA (APM_PCS2_CSR_BASE), data);
    sysWrite32 (PCS_X4_REG_CMD (APM_PCS2_CSR_BASE), PCS_X4_REG_CMD_WT);

    while (1)
        {
        cmdDone = sysRead32 (PCS_X4_REG_CMD_DONE(APM_PCS2_CSR_BASE));
        if (cmdDone & PCS_X4_REG_CMD_WT_DONE)
            break;
        }
    }

/*******************************************************************************
*
* sysPciEx0SerdesInit - PCIEX0 SERDES initialization
*
* This routine initializes SERDES for PCIEX0.
*
* RETURNS: N/A
*/

STATUS sysPciEx0SerdesInit (void)
    {
    UINT32  pcsCtl;
    UINT32  pcsSts;
    int     i;

    /* select PCIE mode for PCS */

    pcsCtl = sysRead32 (PCS_X1_CTL0(APM_PCS0_CSR_BASE));
    pcsCtl = (pcsCtl & PCS_X1_MODE_MASK) | PCS_X1_MODE_PCIE;
    sysWrite32 (PCS_X1_CTL0 (APM_PCS0_CSR_BASE), pcsCtl);

    /* PHY reset */

    sysWrite32 (PCS_X1_CTL0 (APM_PCS0_CSR_BASE), PCS_X1_CTL0_VALUE);
    sysWrite32 (PCS_X1_CTL1 (APM_PCS0_CSR_BASE), PCS_X1_CTL1_VALUE);

    sysWrite32 (PCS_X1_CTL0 (APM_PCS0_CSR_BASE),
               PCS_X1_CTL0_VALUE & (~PCS_X1_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* PHY reset once again */

    sysWrite32 (PCS_X1_CTL0 (APM_PCS0_CSR_BASE), PCS_X1_CTL0_VALUE);
    sysWrite32 (PCS_X1_CTL0 (APM_PCS0_CSR_BASE),
               PCS_X1_CTL0_VALUE & (~PCS_X1_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* check PCS clock & pll ready */

    for (i = 0; i < PCS_TIMEOUT; i++)
        {
        pcsSts = sysRead32 (PCS_X1_STS1 (APM_PCS0_CSR_BASE));
        pcsSts &= (PCS_X1_STS1_CLK_RDY | PCS_X1_STS1_PLL_RDY);
        if (pcsSts == (PCS_X1_STS1_CLK_RDY | PCS_X1_STS1_PLL_RDY))
            break;
        sysUsDelay (PCS_DELAY_IN_US);
        }

    if (i == PCS_TIMEOUT)
        return (ERROR);
    else
        return (OK);
    }

/*******************************************************************************
*
* sysPciEx1SerdesInit - PCIEX1 SERDES initialization
*
* This routine initializes SERDES for PCIEX1.
*
* RETURNS: N/A
*/

STATUS sysPciEx1SerdesInit (void)
    {
    UINT32  pcsCtl;
    UINT32  pcsSts;
    int     i;

    pcsCtl = sysRead32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE));
    pcsCtl = (pcsCtl & PCS_X1_MODE_MASK) | PCS_X1_MODE_PCIE;
    sysWrite32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE), pcsCtl);

    /* PHY reset */

    sysWrite32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE), PCS_X1_CTL0_VALUE);
    sysWrite32 (PCS_X1_CTL1 (APM_PCS1_CSR_BASE), PCS_X1_CTL1_VALUE);

    sysWrite32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE),
               PCS_X1_CTL0_VALUE & (~PCS_X1_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* PHY reset once again */

    sysWrite32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE), PCS_X1_CTL0_VALUE);
    sysWrite32 (PCS_X1_CTL0 (APM_PCS1_CSR_BASE),
               PCS_X1_CTL0_VALUE & (~PCS_X1_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* check PCS clock & pll ready */

    for (i = 0; i < PCS_TIMEOUT; i++)
        {
        pcsSts = sysRead32 (PCS_X1_STS1 (APM_PCS1_CSR_BASE));
        pcsSts &= (PCS_X1_STS1_CLK_RDY | PCS_X1_STS1_PLL_RDY);
        if (pcsSts == (PCS_X1_STS1_CLK_RDY | PCS_X1_STS1_PLL_RDY))
            break;
        sysUsDelay (PCS_DELAY_IN_US);
        }

    if (i == PCS_TIMEOUT)
        return (ERROR);
    else
        return (OK);
    }

/*******************************************************************************
*
* sysPciEx2SerdesInit - PCIEX2 SERDES initialization
*
* This routine initializes SERDES for PCIEX2.
*
* RETURNS: N/A
*/

STATUS sysPciEx2SerdesInit (void)
    {
    UINT32  pcsCtl;
    UINT32  pcsSts;
    UINT16  data;
    int     i;

    /* PHY reset */

    pcsCtl = sysRead32 (PCS_X4_CTL0 (APM_PCS2_CSR_BASE));
    sysWrite32 (PCS_X4_CTL0 (APM_PCS2_CSR_BASE), pcsCtl & (~PCS_X4_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* PHY reset once again */

    sysWrite32 (PCS_X4_CTL0 (APM_PCS2_CSR_BASE), pcsCtl);
    sysWrite32 (PCS_X4_CTL0 (APM_PCS2_CSR_BASE), pcsCtl & (~PCS_X4_CTL0_RESET));
    sysUsDelay (PCS_RST_DELAY_IN_US);

    /* tune timing (APM recommended, undocumented) */

    data = pcsX4RegRdInd(0x318001);
    data |= 0x8000;
    pcsX4RegWtInd(0x3f8001, data);
    data = pcsX4RegRdInd(0x318001);

    /* check PCS clock & pll ready */

    for (i = 0; i < PCS_TIMEOUT; i++)
        {
        pcsSts = sysRead32(PCS_X4_STS1(APM_PCS2_CSR_BASE));
        pcsSts &= (PCS_X4_STS1_CLK_RDY | PCS_X4_STS1_PLL_RDY);
        if (pcsSts == (PCS_X4_STS1_CLK_RDY | PCS_X4_STS1_PLL_RDY))
            break;
        sysUsDelay(PCS_DELAY_IN_US);
        }

    if (i == PCS_TIMEOUT)
        return (ERROR);
    else
        return (OK);
    }

