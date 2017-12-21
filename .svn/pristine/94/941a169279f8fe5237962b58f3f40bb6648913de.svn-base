/* sysTsi108Init.c - Tsi108 Initialization library for HPC-II board */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
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

#include "vxWorks.h"
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
    {0x00000000, 0x00000201},  /* PBA=0xE000_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE100_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE200_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE300_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE400_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE500_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE600_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE700_0000 -> PCI/X (Byte-Swap) */
    /* LUT entries 8 - 15 */
    {0x00000000, 0x00000201},  /* PBA=0xE800_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xE900_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xEA00_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xEB00_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xEC00_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xED00_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xEE00_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xEF00_0000 -> PCI/X (Byte-Swap) */
    /* LUT entries 16 - 23 */
    {0x00000000, 0x00000201},  /* PBA=0xF000_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF100_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF200_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF300_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF400_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF500_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF600_0000 -> PCI/X (Byte-Swap) */
    {0x00000000, 0x00000201},  /* PBA=0xF700_0000 -> PCI/X (Byte-Swap) */
    /* LUT entries 24 - 31 */
    {0x00000000, 0x00000001},  /* PBA=0xF800_0000 -> PCI/X */
    {0x00000000, 0x00000001},  /* PBA=0xF900_0000 -> PCI/X */
    {0x00000000, 0x10000240},  /* PBA=0xFA00_0000 -> HLP : */
    {0x00000000, 0x30000240},  /* PBA=0xFB00_0000 -> HLP : */
    {0x00000000, 0x20000240},  /* PBA=0xFC00_0000 -> HLP : NVRAM */
    {0x00000000, 0x00000201},  /* PBA=0xFD00_0000 -> PCI/X IO Space) */
    {0x00000000, 0x00000201},  /* PBA=0xFE00_0000 -> PCI/X Config Space */
    {0x00000000, 0x00000240}   /* PBA=0xFF00_0000 -> HLP : CS0 */
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
    ULONG reg_val;
    volatile ULONG* reg_ptr;

    /*-------------------------------------------------------------------------
     * Initialize access through the Switching Fabric (SF) using PB_OCN_BAR2.
     *-------------------------------------------------------------------------
     *
     * This window provides acces to the PCI Prefetchable Memory Space.
     * For purpose of verification of Tsi108 operations it also maps SDRAM
     * controller through the OCN path.
     * The first 16 LUT entries are mapped to PCI port and other 16 entries - 
     * to SDRAM OCN port.
     * Total BAR size is 256MB. LUT entry size is 8MB (0x00800000).
     */
 
    /* Map 128MB to PCI port  (first 16 LUT entries) */

    /* initialize pointer to LUT associated with PB_OCN_BAR2 */
    reg_ptr = (ULONG*)(TSI108_CSR_BASE + TSI108_PB_OFFSET + 0x900);

    for (i = 0; i < 16; i++)
	{
	*reg_ptr++ = 0x00000001;
	*reg_ptr++ = 0x00;
	}

    EIEIO_SYNC;

    /* Map 128MB to SDRAM OCN port (next 16 LUT entries) */

    reg_val = 0x18000044; /* PB_LOWER_LUT_ADDRx = SDC port + Addr_Transl */
    for (i = 0; i < 16; i++)
	{
	*reg_ptr++ = reg_val;
	*reg_ptr++ = 0x00;      /* value for PB_UPPER_LUT_ADDRx */
	reg_val += 0x00800000;  /* add translated addr offset */
				/* offset = (LUT entry size) */
	}

    EIEIO_SYNC;

    /* Setup PB_OCN_BAR2: size 256B + ENable @ 0x0_80000000 */

    TSI108_REG_WR(BL_PB, PB_OCN_BAR2, 0x80000001);

    /* Make sure that OCN_BAR2 decoder is set */
    temp = TSI108_REG_RD(BL_PB, PB_OCN_BAR2);


    /*--------------------------------------------------------------------------
     * Remap PB_OCN_BAR1 to accomodate PCI-bus aperture and EPROM into the
     * processor bus address space. Immediately after reset LUT and address
     * translation are disabled for this BAR. Now we have to initialize LUT and
     * switch from the BOOT mode to the normal operation mode.
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


    /* Initialize HLP NVRAM window (if applicable) */
    /* HPC2 board has NVRAM on HLP CS2 port */
    TSI108_REG_WR(BL_HLP, HLP_BX_ADDR(2),  0x02000000);
    TSI108_REG_WR(BL_HLP, HLP_BX_MASK(2),  0xFF000000);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL0(2), 0x7FFC44C0);
    TSI108_REG_WR(BL_HLP, HLP_BX_CTRL1(2), 0x7C0F2000);

    /* Set new value for PB_OCN_BAR1: switch from BOOT to LUT mode.
     * value for PB_OCN_BAR1: (BA-0xE000_0000 + size 512MB + ENable)
     */

    TSI108_REG_WR(BL_PB, PB_OCN_BAR1, 0xE0000011);

    /* Make sure that OCN_BAR2 decoder is set */
    temp = TSI108_REG_RD(BL_PB, PB_OCN_BAR1);

    /*-------------------------------------------------------------------------
     * Initialize PCI/X block
     *-----------------------------------------------------------------------*/

    /* Map PCI/X Configuration Space (16MB @ 0x0_FE000000) */

    TSI108_REG_WR(BL_PCI, PCI_PFAB_BAR0_UPPER, 0);
    TSI108_REG_WR(BL_PCI, PCI_PFAB_BAR0, 0xFE000001);

    /* Set Bus Number for the attached PCI/X bus (we will use 0 for NB) */
    temp = TSI108_REG_RD(BL_PCI, PCI_PE_PCIX_S);
    temp &= ~0xFF00; /* Clear the BUS_NUM field */
    TSI108_REG_WR(BL_PCI, PCI_PE_PCIX_S, temp);

    /* Map PCI/X IO Space (64KB @ 0x0_FD000000) takes one 16MB LUT entry */
    TSI108_REG_WR(BL_PCI, PCI_PFAB_IO_UPPER, 0);
    TSI108_REG_WR(BL_PCI, PCI_PFAB_IO, 0xFD000001);

    /* Map PCI/X Memory Space */

    /*
     * Transactions directed from OCM to PCI Memory Space are directed from PB
     * to PCI unchanged (as defined by PB_OCN_BAR1,2 and LUT settings).
     * If address remapping is required the corresponding PCI_PFAB_MEM32
     * and PCI_PFAB_PFMx register groups have to be configured.
     */

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
#ifdef DISABLE_PBM
    /*
     * In case when PBM is disabled (no HW cache snoopng on PB) P2O_BAR2 
     * is directly mapped into the system memory without address translation.
     */

    reg_val = 0x00000004; /* SDRAM port + NO Addr_Translation */
    for (i = 0; i < 32; i++)
	{
	*reg_ptr++ = reg_val;  /* P2O_BAR2_LUTx */
	*reg_ptr++ = 0;        /* P2O_BAR2_LUT_UPPERx */
	}

    EIEIO_SYNC;
    reg_val = 0x00007500; /* PCI BAR2 (512MB, Enabled, No Addr Transl */
#else
    reg_val = 0x00000002; /* Destination port = PBM */
    for (i = 0; i < 32; i++)
	{
	*reg_ptr++ = reg_val;	    /* P2O_BAR2_LUTx */
	*reg_ptr++ = 0x40000000;    /* P2O_BAR2_LUT_UPPERx : */
				    /* Byte swapping mode for PBM */
	reg_val += 0x01000000;	    /* offset = 16MB, address translation */
				    /* to allow byte swapping */
	}

    EIEIO_SYNC;
    reg_val = 0x00007100; /* PCI BAR2 (512MB, Enabled, Addr Translation) */
#endif /* DISABLE_PBM */

    TSI108_REG_WR(BL_PCI, PCI_P2O_PAGE_SIZES, reg_val);

    /* Set 64-bit PCI bus address for system memory ( 0 for easy mapping) */
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR2, 0);
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR2_UPPER, 0);

#ifndef DISABLE_PBM
    /*-------------------------------------------------------------------------
     * The memory mapped window assotiated with PCI P2O_BAR3 provides access to
     * the system memory using SDRAM OCN port and address translation. This is 
     * alternative way to access SDRAM from PCI provided for demonstration of
     * possible memory mapping configurations.
     * All system memory is opened for accesses initiated by PCI/X bus masters.
     */

    /* Initialize LUT associated with PCI P2O_BAR3 */

    /* set pointer to LUT associated with PCI P2O_BAR3 */
    reg_ptr = (ULONG *)(TSI108_CSR_BASE + TSI108_PCI_OFFSET + 0x600);
    reg_val = 0x00000004; /* Destination port = SDC */
    for (i = 0; i < 32; i++)
	{
	*reg_ptr++ = reg_val;   /* P2O_BAR3_LUTx */
	*reg_ptr++ = 0;         /* P2O_BAR3_LUT_UPPERx */
	reg_val += 0x01000000;  /* offset = 16MB, address translation */
	}

    EIEIO_SYNC;

    /* Configure PCI P2O_BAR3 (size = 512MB, Enabled) */
    reg_val = TSI108_REG_RD(BL_PCI, PCI_P2O_PAGE_SIZES);
    reg_val &= ~0x00FF;
    reg_val |= 0x0071;
    TSI108_REG_WR(BL_PCI, PCI_P2O_PAGE_SIZES, reg_val);

    /* Set 64-bit base PCI bus address for window (0x20000000) */
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR3_UPPER, 0x00000000);
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR3, 0x20000000);
#endif /* !DISABLE_PBM */

#ifdef ENABLE_PCI_CSR_BAR /* if required access to Tsi108 CSRs from PCI bus */
    /* enable BAR0 on the PCI/X bus */
    reg_val = TSI108_REG_RD(BL_PCI, PCI_MISC_CSR);
    reg_val |= 0x02;
    TSI108_REG_WR(BL_PCI, PCI_MISC_CSR, reg_val);

    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR0_UPPER, 0x00000000);
    TSI108_REG_WR(BL_PCI, PCI_P2O_BAR0, TSI108_CSR_BASE);
#endif

    /*-------------------------------------------------------------------------
     * Finally enable PCI/X Bus Master and Memory Space access */

    reg_val = TSI108_REG_RD(BL_PCI, PCI_PE_CSR);
    reg_val |= 0x06;
    TSI108_REG_WR(BL_PCI, PCI_PE_CSR, reg_val);

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
