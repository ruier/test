/* sysSata.c - APM Serengeti board SATA BSP stubs */

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
01a,30novl11,syt  written.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"

#ifdef  AHCI_PHY_DBG_ON

#ifdef  LOCAL
#undef  LOCAL
#define LOCAL
#endif

#define AHCI_PHY_DBG_PCS    0x00000001
#define AHCI_PHY_DBG_AHCI   0x00000002
#define AHCI_PHY_DBG_PHY    0x00000004
#define AHCI_PHY_DBG_ALL    0xffffffff
#define AHCI_PHY_DBG_OFF    0x00000000

LOCAL UINT32 ahciDbgMask = AHCI_PHY_DBG_ALL;

IMPORT FUNCPTR _func_logMsg;

#define AHCI_PHY_DBG(mask, string, a, b, c, d, e, f)        \
    if ((ahciDbgMask & mask) || (mask == AHCI_PHY_DBG_OFF))  \
        if (_func_logMsg != NULL) \
           (* _func_logMsg)(string, a, b, c, d, e, f)
#else
#define AHCI_PHY_DBG(mask, string, a, b, c, d, e, f)
#endif  /* AHCI_PHY_DBG_ON */

/* forward */

LOCAL UINT32 sysAhicRead (UINT32 addr);
LOCAL void   sysAhicWrite (UINT32  addr, UINT32  val);
LOCAL UINT32 sysSataPcsRead (UINT32  port, UINT32  offset);
LOCAL void   sysSataPcsWrite (UINT32  port, UINT32  offset, UINT32  val);

/* defines */

#define SYS_PCS_READ_4(port, offset)    \
            sysSataPcsRead (port, offset)
#define SYS_PCS_WRITE_4(port, offset, val)  \
            sysSataPcsWrite (port, offset, val)
#define SYS_AHCI_READ_4(addr)           \
            sysAhicRead (addr)
#define SYS_AHCI_WRITE_4(addr, val)     \
            sysAhicWrite(addr, val)

/*******************************************************************************
*
* sysAhicRead - Read SATA HBA Memory register
*
* This routine Reads the content of the SATA HBA meeory registers.
*
* RETURNS: N/A
*/
LOCAL UINT32 sysAhicRead (UINT32 addr)
    {
    UINT32 data;

    data = sysRead32 (addr);
    AHCI_PHY_DBG(AHCI_PHY_DBG_AHCI, "sysAhicRead: 0x%08x = 0x%08x\r\n", \
                 addr,data,3,4,5,6);
    return data;
    }

/*******************************************************************************
*
* sysAhicWrite - Write SATA HBA Memory register
*
* This routine writes the content of the SATA HBA meeory registers.
*
* RETURNS: N/A
*/
LOCAL void sysAhicWrite (UINT32 addr, UINT32 val)
    {
    sysWrite32 (addr, val);
    sysUsDelay (PCS_WR_DELAY_IN_US);
    AHCI_PHY_DBG(AHCI_PHY_DBG_AHCI, "sysAhicWrite: 0x%08x = 0x%08x\r\n", \
                 addr,val,3,4,5,6);
    }

/*******************************************************************************
*
* sysSataPcsWrite - Write SATA port's PCS register
*
* This routine writes the content of the SATA port's PCS registers.
*
* RETURNS: N/A
*/
LOCAL void sysSataPcsWrite
    (
    UINT32  port,
    UINT32  offset,
    UINT32  val
    )
    {
    sysWrite32 (APM_SATA_PCS_BASE + (port)*0x10000 + offset, val);
    sysUsDelay (PCS_WR_DELAY_IN_US);

    AHCI_PHY_DBG(AHCI_PHY_DBG_PCS, "sysSataPcsWrite: Port%d 0x%08x = 0x%08x\r\n", \
                 port, (APM_SATA_PCS_BASE + (port)*0x10000 + offset), val,4,5,6);
    }

/*******************************************************************************
*
* sysSataPcsRead - Read SATA port's PCS register
*
* This routine reads the content of the SATA port's PCS registers.
*
* RETURNS: N/A
*/
LOCAL UINT32 sysSataPcsRead
    (
    UINT32  port,
    UINT32  offset
    )
    {
    UINT32 data;
    data = sysRead32 (APM_SATA_PCS_BASE + (port)*0x10000 + offset);

    AHCI_PHY_DBG(AHCI_PHY_DBG_PCS, "sysSataPcsRead: Port%d 0x%08x = 0x%08x\r\n", \
                 port, (APM_SATA_PCS_BASE + (port)*0x10000 + offset),data,4,5,6);

    return data;
    }

/*******************************************************************************
*
* sysPcsPhyRead - read SATA PCS register
*
* This routine reads the content of the SATA PCS registers.
*
* RETURNS: the content of the SATA PCS registers
*
* ERRNO: N/A
*/
LOCAL UINT16 sysPcsPhyRead
    (
    UINT32 port,
    UINT32 phyAddr
    )
    {
    UINT32 cmdDone;
    UINT16 data;

    SYS_PCS_WRITE_4(port, PCSCSR_REG_ADDR, phyAddr);
    SYS_PCS_WRITE_4(port, PCSCSR_REG_CMD,  MGMT_PCS_RD);

    while (1)
        {
        cmdDone = SYS_PCS_READ_4(port, PCSCSR_REG_CMDDONE);
        if (cmdDone & MGMT_PCS_RD_DONE)
            break;
        }

    data = SYS_PCS_READ_4(port, PCSCSR_REG_RDDATA);

    AHCI_PHY_DBG(AHCI_PHY_DBG_PHY, "sysPcsPhyRead: Port%d 0x%08x = 0x%08x\r\n", \
                 port, phyAddr, data, 4,5,6);

    return (data & 0xFFFF);
    }

/*******************************************************************************
*
* sysPcsPhyWrite - write SATA PCS register
*
* This routine writes the data to the SATA PCS registers.
*
* RETURNS: N/A
*/

LOCAL void sysPcsPhyWrite
    (
    UINT32 port,
    UINT32 regAddr,
    UINT32 data
    )
    {
    UINT32 cmdDone;

    SYS_PCS_WRITE_4(port, PCSCSR_REG_ADDR, regAddr);
    SYS_PCS_WRITE_4(port, PCSCSR_REG_WRDATA, data);
    SYS_PCS_WRITE_4(port, PCSCSR_REG_CMD,  MGMT_PCS_WR);

    while (1)
        {
        cmdDone = SYS_PCS_READ_4(port, PCSCSR_REG_CMDDONE);
        if (cmdDone & MGMT_PCS_WR_DONE)
            break;
        }
    AHCI_PHY_DBG(AHCI_PHY_DBG_PHY, "sysPcsPhyWrite: Port%d 0x%08x = 0x%08x\r\n", \
                 port, regAddr, data, 4,5,6);
    }

/*******************************************************************************
*
* sysSataSerdesInit - SATA SERDES initialization
*
* This routine initializes SERDES for SATA.
*
* RETURNS: N/A
*/

void sysSataSerdesInit (void)
    {
    UINT32  data;
    UINT32  timeout;
    UINT32  port;
    UINT32  portNum;
    UINT32  cap;

    if (SATA_PORT1_ENABLE)
        portNum = 2;
    else
        portNum = 1;

    /* Enable AHCI controller */

    SYS_AHCI_WRITE_4(APM_AHCI_GHC, SYS_AHCI_READ_4(APM_AHCI_GHC) | AHCI_EN);

    /* Backup the HBA Capabilities Register value */

    cap = SYS_AHCI_READ_4(APM_AHCI_CAP);

    for (port = 0; port < portNum; port++)
        {
        /* Enabling SATA read/write cache Coherency for full DDR RAM */

        SYS_PCS_WRITE_4(port, SATA_IOCOH_START0, LOCAL_MEM_LOCAL_ADRS);
        SYS_PCS_WRITE_4(port, SATA_IOCOH_START1, LOCAL_MEM_LOCAL_ADRS);
        SYS_PCS_WRITE_4(port, SATA_IOCOH_END0, (LOCAL_MEM_END_ADRS >> 4));
        SYS_PCS_WRITE_4(port, SATA_IOCOH_END1, 0xf0000099);

        /* Place PHY in reset */

        SYS_PCS_WRITE_4(port, PCSCSR_CTL0, PCSCSR_CTL0_INIT_VAL | PCS_PHY_RST_MASK);
        SYS_PCS_WRITE_4(port, PCSCSR_CTL1, PCSCSR_CTL1_INIT_VAL);
        SYS_PCS_WRITE_4(port, PCSCSR_CTL2, PCSCSR_CTL2_INIT_VAL);

        /* Get PHY out of reset */

        SYS_PCS_WRITE_4(port, PCSCSR_CTL0, PCSCSR_CTL0_INIT_VAL);

        /* Reset PHY again to handle PLL lock issues */

        SYS_PCS_WRITE_4(port, PCSCSR_CTL0, PCSCSR_CTL0_INIT_VAL | PCS_PHY_RST_MASK);

        /* Get PHY out of reset */

        SYS_PCS_WRITE_4(port, PCSCSR_CTL0, PCSCSR_CTL0_INIT_VAL);

        /* Force PLL calibration (APM recommended, undocumented) */

        sysPcsPhyWrite (port, 0x5c001, 0x9c75);
        sysPcsPhyRead (port, 0x5c001);

        /* Change Tx swing */

        sysPcsPhyWrite(port, 0x5800d, 0x204c);

        /* Change Pre-emphasis */

        sysPcsPhyWrite (port, 0x58010, 0x6040);
        sysPcsPhyWrite (port, 0x5800e, 0x0f83);

        /* Set aligns */

        sysPcsPhyWrite (port, (LANE_SEL_F | SDS_PCS_SATA_RX_LEN_1_ADDR),
                              SDS_PCS_SATA_RX_LEN_1_DATA);
        sysPcsPhyWrite (port, (LANE_SEL_F | SDS_PCS_SATA_CFG_0_ADDR), SDS_PCS_SATA_CFG_0_DATA);

        /* Wait PHY lock */

        timeout = PCS_TIMEOUT;
        do
            {
            data = SYS_PCS_READ_4(port, PCSCSR_STATUS1);
            if ((data & (SDS_PCS_PLL_LOCK1_MASK | SDS_PCS_CLK_RDY1_MASK)) == 0)
                sysUsDelay (PCS_TIMEOUT);
            }
        while ((timeout--) && ((data & (SDS_PCS_PLL_LOCK1_MASK | SDS_PCS_CLK_RDY1_MASK)) == 0));

        }

    SYS_AHCI_WRITE_4(APM_AHCI_BISTCR, 0x00170000);
    SYS_AHCI_WRITE_4(APM_AHCI_TESTR, 0x00000100);
    SYS_AHCI_WRITE_4(APM_AHCI_BISTCR, 0x00170000);

    /* APM recommended, undocumented */

    for (port = 0; port < portNum; port++)
        {
        sysPcsPhyWrite (port, 0xf8001, 0x9034);
        sysPcsPhyWrite (port, 0x8010, 0x6020);
        sysPcsPhyWrite (port, 0x800d, 0x204b);
        sysPcsPhyWrite (port, 0x800e, 0x0f83);
        }

    /* Write PI register */

    SYS_AHCI_WRITE_4(APM_AHCI_PI, 0x03000000);

    /* Restore HBA Capabilities Register */

    SYS_AHCI_WRITE_4(APM_AHCI_CAP, cap);

    }

