/* sysAta.c - ATA-2 initialization for sysLib.c */

/*
 * Copyright (c) 2004-2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,06aug09,e_d  removed ataDrv.c. (WIND00171937)
01g,18jan07,b_m  cleanup sysAtaInit().
01f,22feb06,pkr  go back to standard ataDrv
01e,21jun05,pcm  removed references to dosFs
01d,24feb05,j_b  include m5200Atadrv.c
01c,15jan05,k_p  corrected compilation error on Solaris
01b,14jan05,k_p  added sysATAMount() debug routine.
01a,20may04,lpn  written
*/

/*
Description

This file contains the sysAtaInit() necessary for
initializing the ATA/EIDE subsystem on the Winbond chip.
This file will enable the controller and also set the
PIO mode as detected by ataDrv().
*/

/* includes */

#include "vxWorks.h"
#include "config.h"

#ifdef  INCLUDE_ATA
#include "drv/intrCtl/m5200Intr.h"

/* defines */

#define ATA_ADD_COUNT           1           /* 1=add a count for hold state,
                                               0=no hold state */
#define ATA_BIT_HOST_IORDY      0x01000000  /* Set IORDY For PIO Modes 3 and
                                               above */
#define ATA_BIT_HOST_IE         0x02000000  /* Enables Drives interrupts to
                                               pass to CPU for PIO modes */
#define ATA_PIO_TMG1_OFF        0x08        /* ata Pio Timing 1 register
                                               offset */
#define ATA_PIO_TMG2_OFF        0x0C        /* ata Pio Timing 2 register
                                               offset */
#define ATA_UNPRG_OFF           0x2C        /* ata Unimplemented register */

#define ATA_CS_CSB45            0x01000000  /* GPIO STD config reg enables ATA
                                               cs0/1 on CSb_4/5 */
#define ATA_CLK_EN              0x00000400  /* CDM clk reg enables ATA clk */

/* external declarations */

/* global declarations */

IMPORT void * vxMBarGet();

/* locals */

LOCAL BOOL      sysAtaInstalled   = FALSE;
LOCAL UINT32    ataVAL_clk_period = 15;     /* implies that IPBI CLK equates
                                               66 MHz For 132MHz set the
                                               value to 7 */

/* ATAPI-4 PIO specs */
/* numbers in ns, extrapolation done by code */

LOCAL int t0_spec[5]    =   {600,   383,    240,    180,    120};
LOCAL int t1_spec[5]    =   {70,    50,     30,     30,     25};
LOCAL int t2_8spec[5]   =   {290,   290,    290,    80,     70};
LOCAL int t2_16spec[5]  =   {165,   125,    100,    80,     70};
LOCAL int t2i_spec[5]   =   {0,     0,      0,      70,     25};
LOCAL int t4_spec[5]    =   {30,    20,     15,     10,     10};
LOCAL int ta_spec[5]    =   {35,    35,     35,     35,     35};

/* function declarations */

STATUS ataVAL_pio_timing (UINT32 speed, volatile UINT32 *ataHostReg)
{
    UINT32 t0, t2_8, t2_16, t2i, t4, t1, ta;
    UINT32 pio1setting, pio2setting;
    volatile UINT32 *ataPioTiming1 = (volatile UINT32 *)((UINT32) ataHostReg + ATA_PIO_TMG1_OFF);
    volatile UINT32 *ataPioTiming2 = (volatile UINT32 *)((UINT32) ataHostReg + ATA_PIO_TMG2_OFF);

    t0    = (UINT32) (ATA_ADD_COUNT + ((t0_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    t2_8  = (UINT32) (ATA_ADD_COUNT + ((t2_8spec[speed]  +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    t2_16 = (UINT32) (ATA_ADD_COUNT + ((t2_16spec[speed] +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    t2i   = (UINT32) (ATA_ADD_COUNT + ((t2i_spec[speed]  +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    t4    = (UINT32) (ATA_ADD_COUNT + ((t4_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    t1    = (UINT32) (ATA_ADD_COUNT + ((t1_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */
    ta    = (UINT32) (ATA_ADD_COUNT + ((ta_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period));  /* min spec - round up */

    if (t0 > 255)
        printf ("PIO t0 setting more than 8 bits wide\n");

    if (t2_8 > 255)
        printf ("PIO t2_8 setting more than 8 bits wide\n");

    if (t2_16 > 255)
        printf ("PIO t2_16 setting more than 8 bits wide\n");

    if (t2i > 255)
        printf ("PIO t2i setting more than 8 bits wide\n");

    if (t4 > 255)
        printf ("PIO t4 setting more than 8 bits wide\n");

    if (t1 > 255)
        printf ("PIO t1 setting more than 8 bits wide\n");

    if (ta > 255)
        printf ("PIO ta setting more than 8 bits wide\n");

    pio1setting = (t0 << 24) | (t2_8 << 16) | (t2_16 << 8) | (t2i);
    pio2setting = (t4 << 24) | (t1 << 16) | (ta << 8);

    /* For PIO MODE 3 and above IORDY must be set */

    if (speed < 3)
        *ataHostReg &= ~ATA_BIT_HOST_IORDY;
    else
        *ataHostReg |= ATA_BIT_HOST_IORDY;

    *ataPioTiming1 = pio1setting;
    *ataPioTiming2 = pio2setting;

    return (OK);
}

/******************************************************************************
*
* sysAtaInit - initialize the EIDE/ATA interface
*
* Perform the necessary initialization required before starting up the
* ATA/EIDE driver on the MPC5200  The first time it is called with
* ctrl and drive = 0.   The next time it is called is from ataDrv with
* the appropriate controller and drive to determine PIO mode to use
* on the drive.  See the ataDrv reference entry.
*/

void sysAtaInit
    (
    ATA_CTRL *pCtrl
    )
    {
    volatile UINT32   *ataHostReg =
    (volatile UINT32 *) ((unsigned long)vxMBarGet() + MBAR_ATA);

    /* SETUP ATA */

    if (sysAtaInstalled == FALSE)   /* First time run */
    {
        volatile UINT32 *ataUnimp =
        (volatile UINT32 *) (((unsigned long)vxMBarGet() + MBAR_ATA +
                              ATA_UNPRG_OFF));

        *GPIO_PCR &= 0xFCFFFFFF;            /* Clear ATA CS Bits */
        *GPIO_PCR |= ATA_CS_CSB45;          /* Generates ATA CSb_4/5 */

        /* Enables clk on ata controler */
        *CDM_CLK_ENABLE  |= ATA_CLK_EN;

        ataVAL_pio_timing (0, ataHostReg);

        /* Write, read ATA unimplemented address space, set pertinent
           status bits and clear */

        /* with sticky write 1s. */

        *ataUnimp = 0x00000000;

        /* Enables Drives interrupts to pass to CPU for PIO Modes */

        *ataHostReg |= ATA_BIT_HOST_IE ;

        /* Setup ATA RESET/INIT function ptr. from ataDrv */

        _func_sysAtaInit = (VOIDFUNCPTR) sysAtaInit;

        /* we only need to do the above one time. */

        sysAtaInstalled = TRUE;
    }

    else
    {
		ATA_DRIVE *pDrive;
        int        tmpInt;

        /* Look at ataDrv to see the PIO mode reported */

        for (tmpInt = 0; tmpInt < ATA_MAX_DRIVES; tmpInt++)
        {
            pDrive = &pCtrl->drive[tmpInt];

            switch (pDrive->rwMode)
            {
            case (ATA_PIO_W_0):
                ataVAL_pio_timing (0, ataHostReg);
                break;

            case (ATA_PIO_W_1):
                ataVAL_pio_timing (1, ataHostReg);
                break;

            case (ATA_PIO_W_2):
                ataVAL_pio_timing (2, ataHostReg);
                break;

            case (ATA_PIO_W_3):
                ataVAL_pio_timing (3, ataHostReg);
                break;

            case (ATA_PIO_W_4):
                ataVAL_pio_timing (4, ataHostReg);
                break;

            default:
                break;
            }
        }
    }

    }

#ifdef ATA_DEBUG

/*******************************************************************************
* sysAtaMount() - mount ATA
*
* This routine mounts ATA0
*
* RETURNS: OK on success and ERROR on failure
*
*/

extern STATUS usrAtaConfig
	(
	int     ctrl,       /* 0: primary address, 1: secondary address */
	int     drive,	    /* drive number of hard disk (0 or 1) */
	char    *devNames   /* mount points for each partition */
	);

STATUS sysAtaMount (void)
    {
    int ctrl = 0;
    ATA_RESOURCE *pAtaResource  = &ataResources[ctrl];

    if (ataDrv (ctrl, pAtaResource->drives,
                pAtaResource->intVector, pAtaResource->intLevel,
                pAtaResource->configType, pAtaResource->semTimeout,
                pAtaResource->wdgTimeout) == ERROR)
        {
        printErr ("Could not initialize.\n");
        return (ERROR);
        }

    if (usrAtaConfig (ctrl, 0, "/ata0/") == ERROR)
        {
        printErr ("usrAtaConfig failed.\n");
        return (ERROR);
        }

    printf ("NUMDIRVE=%d\n",ATA_CTRL0_DRIVES);
    return (OK);
    }

#endif /* #ifdef ATA_DEBUG */

#endif /* INCLUDE_ATA */
