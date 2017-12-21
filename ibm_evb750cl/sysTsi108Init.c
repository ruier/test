/* sysTsi108Init.c - Tsi108 Init library for PPC750GX-GL Evaluation Board */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01d,12nov07,y_w  Change address mapping to support booting from big flash
01c,26jul07,y_w  clean up.
01b,06dec05,avb  clean up to meet vxWorks code style requirements
01a,26jul05,avb  written.
*/

/*
DESCRIPTION
This library provides board-specific initialization routines
for Tsi108 host bridge.

INCLUDE FILES: tsi108.h
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "tsi108.h"

/* defines */


/* typedefs */

/*
 * Processor bus Look-up Tables Initialization Values
 */

typedef struct {
    ULONG upper;
    ULONG lower;
} PB2OCN_LUT_ENTRY;


#ifdef CFG_CLK_SPREAD
typedef struct {
    ULONG ctrl0;
    ULONG ctrl1;
} PLL_CTRL_SET;
#endif /* CFG_CLK_SPREAD */


/* globals */


/* locals */

LOCAL PB2OCN_LUT_ENTRY pb2ocn_lut1[32] =
{
    /* LUT entries 0 - 7 */
    {0x00000000, 0x05000240},  /* PBA=0xE000_0000 -> HLP:CS3 NVRAM */
    {0x00000000, 0x04000240},  /* PBA=0xE100_0000 -> HLP:CS2 SRAM */
    {0x00000000, 0x00000000},  /* PBA=0xE200_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xE300_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xE400_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xE500_0000 -> unmapped */
#ifdef BOOT_FROM_BIGFLASH
    {0x00000000, 0x03000240},  /* PBA=0xE600_0000 -> HLP:CS1 Small FLASH */
    {0x00000000, 0x00000000},  /* PBA=0xE700_0000 -> unmapped */
#else
    {0x00000000, 0x02000240},  /* PBA=0xE600_0000 -> HLP:CS1 Big FLASH */
    {0x00000000, 0x03000240},  /* PBA=0xE700_0000 -> HLP:CS1 Big FLASH */
#endif
    /* LUT entries 8 - 15 */
    {0x00000000, 0x00000000},  /* PBA=0xE800_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xE900_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xEA00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xEB00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xEC00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xED00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xEE00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xEF00_0000 -> unmapped */
    /* LUT entries 16 - 23 */
    {0x00000000, 0x00000000},  /* PBA=0xF000_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF100_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF200_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF300_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF400_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF500_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF600_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF700_0000 -> unmapped */
    /* LUT entries 24 - 31 */
    {0x00000000, 0x00000000},  /* PBA=0xF800_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xF900_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xFA00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xFB00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xFC00_0000 -> unmapped */
    {0x00000000, 0x00000000},  /* PBA=0xFD00_0000 -> unmapped */
#ifdef BOOT_FROM_BIGFLASH
    {0x00000000, 0x00000240},  /* PBA=0xFE00_0000 -> HLP:CS0 Big FLASH */
    {0x00000000, 0x01000240}   /* PBA=0xFF00_0000 -> HLP:CS0 Big FLASH */
#else
    {0x00000000, 0x00000000},  /* PBA=0xFE00_0000 -> unmapped */
    {0x00000000, 0x00000240}   /* PBA=0xFF00_0000 -> HLP:CS0 Small FLASH */
#endif
};

#ifdef CFG_CLK_SPREAD
/*
 * Clock Generator SPLL0 initialization values
 */

/*
 * PLL0 configuration table for various PB_CLKO freq.
 * Uses pre-calculated values for Fs = 30 kHz, D = 0.5%
 * Fout depends on required PB_CLKO. Based on Fref = 33 MHz
 */
LOCAL PLL_CTRL_SET pll0_config[8] =
{
    {0x00000000, 0x00000000}, /* 0: bypass */
    {0x00000000, 0x00000000}, /* 1: reserved */
    {0x00430044, 0x00000043}, /* 2: CG_PB_CLKO = 183 MHz */
    {0x005c0044, 0x00000039}, /* 3: CG_PB_CLKO = 100 MHz */
    {0x005c0044, 0x00000039}, /* 4: CG_PB_CLKO = 133 MHz */
    {0x004a0044, 0x00000040}, /* 5: CG_PB_CLKO = 167 MHz */
    {0x005c0044, 0x00000039}, /* 6: CG_PB_CLKO = 200 MHz */
    {0x004f0044, 0x0000003e}  /* 7: CG_PB_CLKO = 233 MHz */
};
#endif /* CFG_CLK_SPREAD */

/*
 * Prosessor Bus Clock (in MHz) defined by CG_PB_SELECT
 * (based on recommended Tsi108 reference clock 33MHz)
 */
LOCAL int pb_clk_sel[8] = {0,0,183,100,133,167,200,233};


/* forward declarations */


/* externals */
IMPORT ULONG sysPbClock;

/*******************************************************************************
* sysGetPbClock - get processor bus clock  
*
* This routine reads PLL configuration from Tsi108 clock generator status
* status register and returns processor bus clock speed using PB clock 
* reference table.
*
* RETURNS: Processor bus clock in Hz.
*/

void sysGetPbClock (void)
    {
#ifndef CFG_BUS_CLK
    ULONG i;

    /* Detect PB clock freq. */
    i = TSI108_REG_RD(BL_CLKG,CG_PWRUP_STATUS);
    i = (i & CG_PWRUP_STATUS_CG_PB_SELECT) >> 16; /* Get PB PLL multiplier */
    sysPbClock = pb_clk_sel[i] * 1000000;
#else
    sysPbClock = CFG_BUS_CLK;
#endif /* !(CFG_BUS_CLK) */
    }


/*******************************************************************************
* sysTsi108Init - Tsi108 initialization routine.
*
* This Tsi108 initialization routine completes Tsi108 memory mapping. It has to
* be executed from RAM. Contains code that cannot be executed from the flash.
*
* RETURNS: nothing
*/

void sysTsi108Init (void)
    {
    ULONG temp, i;
    ULONG lut_val, lut_val_up, size_val;
    volatile ULONG* reg_ptr;

#ifdef INCLUDE_PCI

    /*-------------------------------------------------------------------------
     * Initialize access from PB to PCI bus through ISF (map using PB_OCN_BAR2)
     *-------------------------------------------------------------------------
     *
     * This window provides acces to the PCI bus aperture (Memory, I/O and CFG).
     * The first 30 LUT entries (0 - 29) are mapped to PCI memory space.
     * The last two LUT entries (30 and 31)map PCI CFG and PCI I/O spaces.
     * Total BAR size is 1GB. LUT entry size is 8MB (0x00800000).
     */
 
    /* Initialize pointer to LUT associated with PB_OCN_BAR2 */
    reg_ptr = (ULONG*)(TSI108_CSR_BASE + TSI108_PB_OFFSET + 0x900);

    for (i = 0; i < 31; i++)
	{
	*reg_ptr++ = 0x00000201;
	*reg_ptr++ = 0x00;
	/* Includes PCI CFG as lut[30] */
	}

    /* Last LUT entry will be mapped to I/O space.
     * Requires address translation to I/O address 0 */
    *reg_ptr++ = 0x00000241;
    *reg_ptr = 0x00;

    EIEIO_SYNC;

    /* Setup PB_OCN_BAR2: size 1GB + ENable @ PCI_MSTR_BASE */

    TSI108_REG_WR(BL_PB, PB_OCN_BAR2, PCI_MSTR_BASE | 0x0021);

    /* Make sure that OCN_BAR2 decoder is set */
    temp = TSI108_REG_RD(BL_PB, PB_OCN_BAR2);

    /*-------------------------------------------------------------------------
     * Initialize PCI/X block (master: ISF-to-PCI)
     *-----------------------------------------------------------------------*/

    /* Map PCI/X Configuration Space (16MB @ PCI_MSTR_CFG_LOCAL) */

    TSI108_REG_WR(BL_PCI, PCI_PFAB_BAR0_UPPER, 0);
    TSI108_REG_WR(BL_PCI, PCI_PFAB_BAR0, PCI_MSTR_CFG_LOCAL | 0x0001);

    /* Set Bus Number for the attached PCI/X bus (we will use 0 for NB) */
    temp = TSI108_REG_RD(BL_PCI, PCI_PE_PCIX_S);
    temp &= ~0xFF00; /* Clear the BUS_NUM field */
    TSI108_REG_WR(BL_PCI, PCI_PE_PCIX_S, temp);

    /* Map PCI/X IO Space (64KB @ PCI_MSTR_IO_LOCAL) */
    TSI108_REG_WR(BL_PCI, PCI_PFAB_IO_UPPER, 0);
    TSI108_REG_WR(BL_PCI, PCI_PFAB_IO, PCI_MSTR_IO_LOCAL | 0x0001);

    /*-------------------------------------------------------------------------
     * Map the path from the PCI/X bus into the system memory
     *-----------------------------------------------------------------------*/

    /*
     * The memory mapped window assotiated with PCI P2O_BAR2 provides access to
     * the system memory without address remapping.
     * All system memory is opened for accesses initiated by PCI/X bus masters.
     */

    /* Initialize LUT associated with PCI P2O_BAR2 */

    /* set pointer to LUT associated with PCI P2O_BAR2 */
    reg_ptr = (ULONG *)(TSI108_CSR_BASE + TSI108_PCI_OFFSET + 0x500);
    size_val = (31 - vxFirstBit(LOCAL_MEM_SIZE/0x8000)) << 11;

    /* Disable bus pipeline control */
    temp = TSI108_REG_RD(BL_PB, PB_ARB_CTRL);
    TSI108_REG_WR(BL_PB, PB_ARB_CTRL, temp & ~0x000000f0);


#ifdef DISABLE_PBM
    /*
     * In case when PBM is disabled (no HW cache snoopng on PB) P2O_BAR2 
     * is directly mapped into the system memory without address translation.
     */

    lut_val = 0x00000004; /* SDRAM port + NO Addr_Translation */
    lut_val_up = 0;
    size_val |= (1 << 8) | (1 << 10); /* Enabled, No Addr Transl */
#else
    lut_val = LOCAL_MEM_LOCAL_ADRS | 0x02; /* Destination port = PBM */
    lut_val_up = 0x40000000; /* requires byte-swap for PBM */
    size_val |= (1 << 8); /* Enabled with Addr Translation */
#endif /* DISABLE_PBM */

    for (i = 0; i < 32; i++)
	{
	*reg_ptr++ = lut_val;	    /* P2O_BAR2_LUTx */
	*reg_ptr++ = lut_val_up;    /* P2O_BAR2_LUT_UPPERx : */
#ifndef DISABLE_PBM
	lut_val += LOCAL_MEM_SIZE/32; /* offset = 1/32 of memory size */
#endif /* DISABLE_PBM */
	}

    EIEIO_SYNC;

    TSI108_REG_WR(BL_PCI, PCI_P2O_PAGE_SIZES, size_val);

    /* Set 64-bit PCI bus address for system memory
       (LOCAL_MEM_LOCAL_ADRS for easy direct mapping) */
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR2, LOCAL_MEM_LOCAL_ADRS);
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR2_UPPER, 0);

#ifdef ENABLE_PCI_CSR_BAR /* if required access to Tsi108 CSRs from PCI bus */
    /* enable BAR0 on the PCI/X bus */
    temp = TSI108_REG_RD(BL_PCI, PCI_MISC_CSR);
    temp |= 0x02;
    TSI108_REG_WR(BL_PCI, PCI_MISC_CSR, temp);

    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR0_UPPER, 0x00000000);
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR0, TSI108_CSR_BASE);
#endif

    /*-------------------------------------------------------------------------
     * Finally enable PCI/X Bus Master and Memory Space access */

    temp = TSI108_REG_RD(BL_PCI, PCI_PE_CSR);
    temp |= 0x06;
    TSI108_REG_WR(BL_PCI, PCI_PE_CSR, temp);

#endif /* INCLUDE_PCI */

    /*--------------------------------------------------------------------------
     * Remap PB_OCN_BAR1 to accomodate HLP block aperture (FLASH, NVRAM and
     * SRAM) into the processor bus address space.
     * Immediately after reset LUT and address translation are disabled
     * for this BAR. Now we have to initialize LUT and switch from the BOOT mode
     * to the normal operation mode.
     */

    /* The aperture defined by PB_OCN_BAR1 startes at address 0xE0000000
     * and covers 512MB of address space. To allow larger aperture we also have
     * to relocate register window of Tsi108
     */
 
    /*
     * Initialize LUT (32-entries) prior switching PB_OCN_BAR1 from BOOT mode
     */

    /* initialize pointer to LUT associated with PB_OCN_BAR1 */
    reg_ptr = (ULONG*)(TSI108_CSR_BASE + TSI108_PB_OFFSET + 0x800);

    for (i = 0; i < 32; i++)
	{
	*reg_ptr++ = pb2ocn_lut1[i].lower;
	*reg_ptr++ = pb2ocn_lut1[i].upper;
	}

    EIEIO_SYNC;

    /*
     * Initialize HLP registers 
     */

#ifdef BOOT_FROM_BIGFLASH
    /* CS0 - Small (8-bit) FLASH */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(1),  0x00300000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(1),  0xFFF00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(1), 0x7FFC44C0);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(1), 0xFC0F2000);
    /* CS1 - Big (32-bit) FLASH */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(0),  0x00000000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(0),  0xFFE00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(0), 0x7FFC44C2);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(0), 0x7C0F2000);
#else
    /* CS0 - Small (8-bit) FLASH */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(0),  0x00000000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(0),  0xFFF00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(0), 0x7FFC44C0);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(0), 0xFC0F2000);
    /* CS1 - Big (32-bit) FLASH */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(1),  0x00200000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(1),  0xFFE00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(1), 0x7FFC44C2);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(1), 0x7C0F2000);
#endif
    /* CS2 - SRAM */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(2),  0x00400000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(2),  0xFFF00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(2), 0x7FFC44C0);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(2), 0xFC0F2000);
    /* CS3 - NVRAM */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(3),  0x00500000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(3),  0xFFF00000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(3), 0x7FFC44C0);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(3), 0xFC042000);

    /* Set new value for PB_OCN_BAR1: switch from BOOT to LUT mode.
     * value for PB_OCN_BAR1: (BA-TSI108_HLP_BASE + size 512MB + ENable)
     */

    TSI108_REG_WR(BL_PB, PB_OCN_BAR1, TSI108_HLP_BASE | 0x0011);

    /* Make sure that OCN_BAR2 decoder is set */
    temp = TSI108_REG_RD(BL_PB, PB_OCN_BAR1);

#ifndef DISABLE_PBM
    /*
     * For IBM processors we have to set Address-Only commands generated by PBM
     * that are different from ones set by default after reset.
     */
     
    TSI108_REG_WR(BL_PB, PB_MCMD, 0x00009955);

#endif /* DISABLE_PBM */
    }

#ifdef CFG_CLK_SPREAD  /* Initialize Spread-Spectrum Clock generation */

/*******************************************************************************
* sysInitClkSpread - initialize Spread Spectrum mode of Tsi108 Clock Generator 
*            
*            
*/

void sysInitClkSpread (void)
    {
    ULONG i;

    /* Ensure that Spread-Spectrum is disabled */
    TSI108_REG_WR(BL_CLKG, CG_PLL0_CTRL0, 0);
    TSI108_REG_WR(BL_CLKG, CG_PLL1_CTRL0, 0);

    /* Initialize PLL1: CG_PCI_CLK , internal OCN_CLK
     * Uses pre-calculated value for Fout = 800 MHz, Fs = 30 kHz, D = 0.5%
     */

    TSI108_REG_WR(BL_CLKG, CG_PLL1_CTRL1, 0x00000039);	/* BWADJ */
    TSI108_REG_WR(BL_CLKG, CG_PLL1_CTRL0, 0x002e0044);	/* D = 0.25% */

    /* Initialize PLL0: CG_PB_CLKO  */
    /* Detect PB clock freq. */
    i = TSI108_REG_RD(BL_CLKG,CG_PWRUP_STATUS);
    i = (i >> 16) & 0x07; /* Get PB PLL multiplier */

    TSI108_REG_WR(BL_CLKG, CG_PLL0_CTRL1, pll0_config[i].ctrl1);
    TSI108_REG_WR(BL_CLKG, CG_PLL0_CTRL0, pll0_config[i].ctrl0);

    /* Wait and set SSEN for both PLL0 and 1 */
    sysMsDelay(1);

    TSI108_REG_WR(BL_CLKG, CG_PLL0_CTRL0, 0x80000000 | pll0_config[i].ctrl0);
    TSI108_REG_WR(BL_CLKG, CG_PLL1_CTRL0, 0x802e0044);   /* D=0.25% */

    }

#endif /* CFG_CLK_SPREAD */


/*******************************************************************************
* sysClearPciError - clears error flags reported by PB and PCI blocks after
* attempt to issue PCI Configuration Read request to non-existing device.
*/

void sysClearPciError (void)
    {
    UINT32 err_stat, err_addr, pci_stat;

    /* Read PB Error Log Registers */
    err_stat = TSI108_REG_RD(BL_PB, PB_ERRCS);
    err_addr = TSI108_REG_RD(BL_PB, PB_AERR);

    if (err_stat & PB_ERRCS_ES)
	{
	/* Clear error flag */
	TSI108_REG_WR(BL_PB, PB_ERRCS, PB_ERRCS_ES);

	/* Clear read error reported in PB_ISR */
	TSI108_REG_WR(BL_PB, PB_ISR, PB_ISR_PBS_RD_ERR);

	/* Clear PCI/X bus errors if applicable */
	if ((err_addr & 0xFF000000) == PCI_MSTR_CFG_LOCAL)
	    {
	    pci_stat = TSI108_REG_RD(BL_PCI, PCI_PE_CSR);
	    TSI108_REG_WR(BL_PCI, PCI_PE_CSR, pci_stat);
	    }
	}
    }
