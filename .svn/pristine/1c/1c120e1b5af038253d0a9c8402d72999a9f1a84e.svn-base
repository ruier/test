/* sysAta.c - ATA-2 initialization for sysLib.c */

/*
 * Copyright (c) 2007, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,29apr09,x_f  fix typo of pio1setting
01a,12nov07,b_m  derived from version 01g of lite5200b/sysAta.c.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"

/* defines */

/* 1 = add a count for hold state, 0 = no hold state */

#define ATA_ADD_COUNT           1

#define ATA_BIT_RST_B           0x40
#define ATA_BIT_HOST_IE         0x08

/* locals */

LOCAL BOOL      sysAtaInstalled   = FALSE;

/* implies that IPS CLK equates 99 MHz */

LOCAL UINT32    ataVAL_clk_period = 10;

/*
 * ATAPI-4 PIO specs
 * numbers in ns, extrapolation done by code
 */

LOCAL int t0_spec[5]    =   {600,   383,    240,    180,    120};
LOCAL int t1_spec[5]    =   {70,    50,     30,     30,     25};
LOCAL int t2_8spec[5]   =   {290,   290,    290,    80,     70};
LOCAL int t2_16spec[5]  =   {165,   125,    100,    80,     70};
LOCAL int t2i_spec[5]   =   {0,     0,      0,      0,      0};
LOCAL int t4_spec[5]    =   {30,    20,     15,     10,     10};
LOCAL int t9_spec[5]    =   {20,    15,     10,     10,     10};
LOCAL int ta_spec[5]    =   {50,    50,     50,     50,     50};

/*******************************************************************************
*
* ataVAL_pio_timing - config the ATA bus timing parameters
*
* This function configs the ATA bus timing parameters.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

STATUS ataVAL_pio_timing
    (
    UINT32 speed
    )
    {
    UINT32 t0, t1, t2_8, t2_16, t2i, t4, t9, ta;
    UINT32 pio1setting, pio2setting, pio3setting;

    t0    = (UINT32) (ATA_ADD_COUNT + ((t0_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t1    = (UINT32) (ATA_ADD_COUNT + ((t1_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t2_8  = (UINT32) (ATA_ADD_COUNT + ((t2_8spec[speed]  +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t2_16 = (UINT32) (ATA_ADD_COUNT + ((t2_16spec[speed] +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t2i   = (UINT32) (ATA_ADD_COUNT + ((t2i_spec[speed]  +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t4    = (UINT32) (ATA_ADD_COUNT + ((t4_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    t9    = (UINT32) (ATA_ADD_COUNT + ((t9_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */
    ta    = (UINT32) (ATA_ADD_COUNT + ((ta_spec[speed]   +
        ataVAL_clk_period)/ataVAL_clk_period)); /* min spec - round up */

    if (t0 > 255)
        printf ("PIO t0 setting more than 8 bits wide\n");

    if (t1 > 255)
        printf ("PIO t1 setting more than 8 bits wide\n");

    if (t2_8 > 255)
        printf ("PIO t2_8 setting more than 8 bits wide\n");

    if (t2_16 > 255)
        printf ("PIO t2_16 setting more than 8 bits wide\n");

    if (t2i > 255)
        printf ("PIO t2i setting more than 8 bits wide\n");

    if (t4 > 255)
        printf ("PIO t4 setting more than 8 bits wide\n");

    if (t9 > 255)
        printf ("PIO t9 setting more than 8 bits wide\n");

    if (ta > 255)
        printf ("PIO ta setting more than 8 bits wide\n");

    pio1setting = (3 << 24) | (3 << 16) | (t1 << 8) | (t2_8);
    pio2setting = (t2_8 << 24) | (ta << 16) | (1 << 8) | (t4);
    pio3setting = (t9 << 24);

    /* setup the timing parameters */

    *(volatile UINT32 *)PATA_TIME1 = pio1setting;
    *(volatile UINT32 *)PATA_TIME2 = pio2setting;
    *(volatile UINT32 *)PATA_TIME3 = pio3setting;

    return (OK);
    }

/*******************************************************************************
*
* sysAtaInit - initialize the EIDE/ATA interface
*
* Perform the necessary initialization required before starting up the ATA/EIDE
* driver on the MPC5121e.  The first time it is called with ctrl and drive = 0.
* The next time it is called is from ataDrv with the appropriate controller and
* drive to determine PIO mode to use on the drive.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysAtaInit
    (
    ATA_CTRL *pCtrl
    )
    {
    if (sysAtaInstalled == FALSE)
        {
        /* enable ATA interface */

        *(volatile UINT8 *)PATA_CTRL = ATA_BIT_RST_B;

        ataVAL_pio_timing (0);

        /* enables drives interrupts to pass to CPU for PIO Modes */

        *(volatile UINT8 *)PATA_INT_EN = ATA_BIT_HOST_IE;

        /* setup ATA RESET/INIT function ptr. from ataDrv */

        _func_sysAtaInit = (VOIDFUNCPTR)sysAtaInit;

        /* we only need to do the above one time */

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
                    ataVAL_pio_timing (0);
                    break;

                case (ATA_PIO_W_1):
                    ataVAL_pio_timing (1);
                    break;

                case (ATA_PIO_W_2):
                    ataVAL_pio_timing (2);
                    break;

                case (ATA_PIO_W_3):
                    ataVAL_pio_timing (3);
                    break;

                case (ATA_PIO_W_4):
                    ataVAL_pio_timing (4);
                    break;

                default:
                    break;
                }
            }
        }
    }
