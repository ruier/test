/* sysMpc8540Phb.c - Initialize MPC8540 PCI/PCI-X Bus Interface */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/* Copyright 2004-2005 Motorola, Inc., All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01d,17oct07,y_w  Fixed GNU compiler warning.
01c,22sep05,pjh  Fixed compiler warning.
01b,17dec04,cak  Tweaks and fixes.
01a,10dec04,cak  Initial writing.
*/

/*
DESCRIPTION

This module initializes the MCP8540 PCI/PCI-X Bus Interface.
*/

/* includes */

#include "mpc8540.h"

/* defines */

/* globals */

/* locals */

/*
 * This table contains static initialization values for the MPC8540's 
 * PCI/PCI-X Bus Interface registers.
 */

LOCAL UINT32 mpc8540PhbTbl [] =
    {

    /* PCI/X Configuration Access registers */

    NOT_INIT_MPC8540_REG (CCSR_CFG_ADDR, x)
    NOT_INIT_MPC8540_REG (CCSR_CFG_DATA, x)
    NOT_INIT_MPC8540_REG (CCSR_INT_ACK, x)


    /* PCI/X ATMU registers - Outbound and Inbound */

    /* 
     * Outbound window 0 - Default
     *
     * This is the default translation window for transactions that do
     * not hit in one of the four windows.  It is the only window
     * enabled upon reset.  It is only used when a transaction misses in
     * all other outbound windows.  We will leave it in its default state.
     *
     * Note: There is no programmable POWBAR0
     */

    NOT_INIT_MPC8540_REG (CCSR_POTAR0, x)
    NOT_INIT_MPC8540_REG (CCSR_POTEAR0, x)
    NOT_INIT_MPC8540_REG (CCSR_POWBAR0, readOnly)
    NOT_INIT_MPC8540_REG (CCSR_POWAR0, x)

    /*
     * Outbound window 1 -
     *
     * Here we map the PCI access for PCI_MSTR_MEMIO_LOCAL space.
     * POWAR1 is programmed as follows:
     * Bits: 19:16 RTT = 0100b (Memory reads)
     *       16:13 WTT = 0100b (Memory writes)
     *        5:0  OWS = Size code for PCI_MSTR_MEMIO_SIZE
     */


    YES_INIT_MPC8540_REG (CCSR_POTAR1,
                          CCSR_PAR_ADDR_ENCODE(PCI_MSTR_MEMIO_BUS))
    YES_INIT_MPC8540_REG (CCSR_POTEAR1, 0)
    YES_INIT_MPC8540_REG (CCSR_POWBAR1,
                          CCSR_PAR_ADDR_ENCODE(PCI_MSTR_MEMIO_LOCAL))
    YES_INIT_MPC8540_REG (CCSR_POWAR1, CCSR_POWAR_EN_MASK |
                          (0x4 << 16) | (0x4 << 12) |
                          MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEMIO_SIZE))

    /*
     * Outbound window 2 -
     *
     * Programmed in a similar manner to outbound window 1, except
     * we cover PCI_MSTR_MEM_LOCAL space here.
     */

    YES_INIT_MPC8540_REG (CCSR_POTAR2,
                          CCSR_PAR_ADDR_ENCODE(PCI_MSTR_MEM_BUS))
    YES_INIT_MPC8540_REG (CCSR_POTEAR2, 0)
    YES_INIT_MPC8540_REG (CCSR_POWBAR2,
                          CCSR_PAR_ADDR_ENCODE(PCI_MSTR_MEM_LOCAL))
    YES_INIT_MPC8540_REG (CCSR_POWAR2, CCSR_POWAR_EN_MASK |
                          (0x4 << 16) | (0x4 << 12) |
                          MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEM_SIZE))

    /*
     * Outbound window 3 -
     *
     * Here we map the PCI access for PCI_IO_LOCAL space.
     * POWAR3 is programmed as follows:
     * Bits: 19:16 RTT = 1000b (I/O reads)
     *       16:13 WTT = 1000b (I/O writes)
     *        5:0  OWS = Size code for PCI_IO_SIZE
     */

    YES_INIT_MPC8540_REG (CCSR_POTAR3,
                          CCSR_PAR_ADDR_ENCODE(PCI_IO_BUS_START))
    YES_INIT_MPC8540_REG (CCSR_POTEAR3, 0)
    YES_INIT_MPC8540_REG (CCSR_POWBAR3,
                          CCSR_PAR_ADDR_ENCODE(PCI_IO_LOCAL_START))
    YES_INIT_MPC8540_REG (CCSR_POWAR3, CCSR_POWAR_EN_MASK |
                          (0x8 << 16) | (0x8 << 12) |
                          MPC8540_PCI_SIZE_ENCODE(PCI_IO_SIZE))

    /*
     * Outbound window 4 -
     *
     * Programmed in a similar manner to outbound window 1, except
     * we cover VME access space here.
     */

    YES_INIT_MPC8540_REG (CCSR_POTAR4,
                          CCSR_PAR_ADDR_ENCODE(VME_MEM_BUS_START))
    YES_INIT_MPC8540_REG (CCSR_POTEAR4, 0)
    YES_INIT_MPC8540_REG (CCSR_POWBAR4,
                          CCSR_PAR_ADDR_ENCODE(VME_MEM_LOCAL_START))
    YES_INIT_MPC8540_REG (CCSR_POWAR4, CCSR_POWAR_EN_MASK |
                          (0x4 << 16) | (0x4 << 12) |
                          MPC8540_PCI_SIZE_ENCODE(VME_MEM_SIZE))

    /* Inbound window 3 - DRAM dynamically configured below */

    NOT_INIT_MPC8540_REG (CCSR_PITAR3, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWBAR3, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWBEAR3, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWAR3, x)

    /* Inbound window 2 - UNUSED */

    NOT_INIT_MPC8540_REG (CCSR_PITAR2, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWBAR2, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWBEAR2, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWAR2, x)

    /* Inbound window 1 - UNUSED */

    NOT_INIT_MPC8540_REG (CCSR_PITAR1, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWBAR1, x)
    NOT_INIT_MPC8540_REG (CCSR_PIWAR1, x)

    /* PCI/X Error Management registers */

    YES_INIT_MPC8540_REG (CCSR_ERR_DR, 0x800007FF)
    YES_INIT_MPC8540_REG (CCSR_ERR_CAP_DR, 0x000007FF)
    YES_INIT_MPC8540_REG (CCSR_ERR_EN, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_ERR_ATTRIB, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_ERR_ADDR, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_ERR_EXT_ADDR, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_ERR_DL, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_ERR_DH, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_GAS_TIMR, 0x00000000)
    YES_INIT_MPC8540_REG (CCSR_PCIX_TIMR, 0x1FFFFFF) /* maximum values */

    /* End of initialization */

    END_INIT_MPC8540_REG

    };

/* forward declarations */

void sysMpc8540PhbInit (void);
BOOL sysIsPciX (int);

/******************************************************************************
*
* sysIsPciX - Determine if PCI bus is running in PCI-X mode
*
* The routine queries the appropriate MPC8540 register to determine if the 
* bus is running in PCI-X mode or not.
*
* RETURNS: TRUE if bus is running in PCI-X mode, FALSE if bus is
* runing in conventional PCI mode.
*/

BOOL sysIsPciX
    (
    int busNo           /* PCI bus number */
    )
    {
    BOOL retVal;

    if ((CCSR_READ32(CCSBAR, CCSR_PORDEVSR) & 0x800000) == 0)
	retVal = TRUE;
    else
        retVal = FALSE;          

    return (retVal);
    }

/******************************************************************************
*
* sysMpc8540PhbInit - Dynamic init of MPC8540 PCI/PCI-X bus interface.
*
* This routine performs the dynamic initialization of the MPC8540 PCI/PCI-X
* bus interface registers.
*
* RETURNS: NA
*/

void sysMpc8540PhbInit (void)
    {
    UINT32 dramSize = 0;
    UINT32 temp = 0;
    UINT32 pciAgent = 0;
    UINT16 command = 0;
    UINT32 piwar3 = 0;

    /* 
     * PCI-X
     *
     * Initialize the bus/device numbers for the PHB bridge 
     * if it is functioning in PCI-X mode.
     */

    if (sysIsPciX(0))
	{
	sysPciConfigRead (0xFF,0x1F,0,0x64,4,&temp);
	temp &= 0xFFFF0000;
	temp |= ((PCI_HOST_BRIDGE_FUNC_NUM << 0) |
		 (PCI_HOST_BRIDGE_DEV_NUM << 3) |
		 (PCI_HOST_BRIDGE_BUS_NUM << 8));
	sysPciConfigWrite (0xFF,0x1F,0,0x64,4,temp); 
	}

    /* static register initialization */

    sysMpc8540RegInit (mpc8540PhbTbl);

    /* TO DO : PCI Error detection */

    sysPciConfigRead (PCI_HOST_BRIDGE_BUS_NUM,
		      PCI_HOST_BRIDGE_DEV_NUM,
		      PCI_HOST_BRIDGE_FUNC_NUM, 
		      0x44, 2, &pciAgent);
    pciAgent &= 0x1;

    if (!pciAgent)
	{

	/*
	 * The size of the system memory (DRAM) is variable and so
	 * the size of inbound window 3 (access to DRAM from PCI) 
	 * must be configured dynamically.  If we are the host then 
	 * map all of DRAM from the PCI bus.
	 */

	CCSR_WRITE32 (CCSBAR, CCSR_PITAR3,
		      CCSR_PAR_ADDR_ENCODE(LOCAL_MEM_LOCAL_ADRS));
	CCSR_WRITE32 (CCSBAR, CCSR_PIWBAR3,
		      CCSR_PAR_ADDR_ENCODE(LOCAL_MEM_LOCAL_ADRS));
	CCSR_WRITE32 (CCSBAR, CCSR_PIWBEAR3, 0);
	dramSize = sysDramSize();
	piwar3 = (CCSR_PIWAR_EN_MASK |
                 ((MPC8540_PCI_SIZE_ENCODE(dramSize)) 
	          << (CCSR_PIWAR_IWS_BIT)) |
                 (0xf << CCSR_PIWAR_TGI_BIT) |
                 (0x4 << CCSR_PIWAR_RTT_BIT) |
                 (0x4 << CCSR_PIWAR_WTT_BIT));

#if (defined INCLUDE_CACHE_SUPPORT)
	#if (USER_D_CACHE_MODE & CACHE_WRITETHROUGH)
	    piwar3 = (CCSR_PIWAR_EN_MASK |
                     ((MPC8540_PCI_SIZE_ENCODE(dramSize))
                      << (CCSR_PIWAR_IWS_BIT)) |
                     (0xf << CCSR_PIWAR_TGI_BIT) |
                     (0x4 << CCSR_PIWAR_RTT_BIT) |
                     (0x5 << CCSR_PIWAR_WTT_BIT));
	#elif (USER_D_CACHE_MODE & CACHE_COPYBACK)
            piwar3 = (CCSR_PIWAR_EN_MASK |
                     ((MPC8540_PCI_SIZE_ENCODE(dramSize))
                      << (CCSR_PIWAR_IWS_BIT)) |
                     (0xf << CCSR_PIWAR_TGI_BIT) |
                     (0x5 << CCSR_PIWAR_RTT_BIT) |
                     (0x5 << CCSR_PIWAR_WTT_BIT));
	#endif
#endif /* INCLUDE_CACHE_SUPPORT */

	CCSR_WRITE32 (CCSBAR, CCSR_PIWAR3, piwar3);

	/* enable access to 8540 registers from PCI */

	sysPciConfigWrite (PCI_HOST_BRIDGE_BUS_NUM,
			   PCI_HOST_BRIDGE_DEV_NUM,
			   PCI_HOST_BRIDGE_FUNC_NUM,
			   0x10,4,(CCSBAR_BUS | 0x8)); 

	/* enable master capability and memory space */

	sysPciConfigRead (PCI_HOST_BRIDGE_BUS_NUM,
			  PCI_HOST_BRIDGE_DEV_NUM,
			  PCI_HOST_BRIDGE_FUNC_NUM,
			  0x04,2,&command);
	command |= 0x06;
	sysPciConfigWrite (PCI_HOST_BRIDGE_BUS_NUM,
			   PCI_HOST_BRIDGE_DEV_NUM,
			   PCI_HOST_BRIDGE_FUNC_NUM,
			   0x04,2,command); 
	}

    /* Clear the lockout bit */

    sysPciConfigWrite (PCI_HOST_BRIDGE_BUS_NUM,
		       PCI_HOST_BRIDGE_DEV_NUM,
		       PCI_HOST_BRIDGE_FUNC_NUM, 
		       0x44, 2, 0x0);
    }
