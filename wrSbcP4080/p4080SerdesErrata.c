/* p4080SerdesErrata.c.c - Freescale P4080 Serdes errata fix. */
 
/*
 * Copyright (c) 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,11jan13,ylu  Update for rev 3 silicon.
01d,28jun12,c_l  WIND00357618: used low priority LAWs for DDR.
01c,14mar12,mpc  WIND00348932: fix wrong bank offset passed to SRDS_BK_SETBIT().
01b,15feb12,x_s  WIND00322287: fix sysSerdesLaneEnabled() to get correct offset of bank.
01a,21jul11,ers  WIND00262613: fix SERDES errata procedure to not disturb
                 lanes used by Aurora.  Created based on sysLib.c version 03e.
*/

/*
DESCRIPTION
This file provides the p4080 SERDES 8 Errata fix.  This file applies to Rev 1.0
and 2.0 Silicon.  It is intened to be #included by sysLib.c based on the config.h
define SERDES_FIX_CPU.

INCLUDE FILES:

SEE ALSO:
.pG "Configuration"
*/

LOCAL UINT8 serdes_cfg_tbl[SRDS_MAX_CFGS][SRDS_MAX_LANES] = {
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1, PCIE1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, PCIE2, PCIE2,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, SGMII_FM2, SGMII_FM2,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, SGMII_FM2, SGMII_FM2,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, PCIE2, PCIE2, SGMII_FM2, SGMII_FM2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, PCIE2, PCIE2, SGMII_FM2, SGMII_FM2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM2,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {SRIO2, SRIO2, SRIO2, SRIO2, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, NOPROTO, SRIO2, NOPROTO, SRIO1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, NOPROTO, SRIO2, NOPROTO, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, NOPROTO, SRIO2, NOPROTO, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, PCIE3, PCIE3, PCIE3, PCIE3,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE1, PCIE1, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, SGMII_FM2, SGMII_FM2, SGMII_FM2, SGMII_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                SGMII_FM1, SGMII_FM1, SGMII_FM1, SGMII_FM1},
        {PCIE1, PCIE1, PCIE3, PCIE3, SRIO1, SRIO1, SRIO1, SRIO1,
                AURORA, AURORA, XAUI_FM2, XAUI_FM2, XAUI_FM2, XAUI_FM2,
                XAUI_FM1, XAUI_FM1, XAUI_FM1, XAUI_FM1}
};

struct lane_off
    {
    UINT32 off; /* register offset */
    UINT32 lpd; /* lane power down bit in RCW */
    UINT32 bank; /* bank number */
    };

LOCAL struct lane_off serdes_lane_off[] =
    {
    { SRDS_B1_LANEA, 152, 0 },
    { SRDS_B1_LANEB, 153, 0 },
    { SRDS_B1_LANEC, 154, 0 },
    { SRDS_B1_LANED, 155, 0 },
    { SRDS_B1_LANEE, 156, 0 },
    { SRDS_B1_LANEF, 157, 0 },
    { SRDS_B1_LANEG, 158, 0 },
    { SRDS_B1_LANEH, 159, 0 },
    { SRDS_B1_LANEI, 160, 0 },
    { SRDS_B1_LANEJ, 161, 0 },
    { SRDS_B2_LANEA, 162, 1 },
    { SRDS_B2_LANEB, 163, 1 },
    { SRDS_B2_LANEC, 164, 1 },
    { SRDS_B2_LANED, 165, 1 },
    { SRDS_B3_LANEA, 170, 2 },
    { SRDS_B3_LANEB, 171, 2 },
    { SRDS_B3_LANEC, 172, 2 },
    { SRDS_B3_LANED, 173, 2 }
    };

LOCAL int serdes_bank_off[] =
    {
    SRDS_BANK1,
    SRDS_BANK2,
    SRDS_BANK3
    };

int sysSerdesRcwActual[16];

/******************************************************************************
*
* sysXaui4War - work around P4080 Rev 1 erratum XAUI4
*
* This function implements a software workaround for a silicon erratum in
* the Rev 1 P4080 chip, related to XAUI (10 gigabit ethernet) operation.
* In some cases, some of the lanes may not synchronize properly. The
* function puts the lanes into loopback mode and issues a reset to
* the protocol converter module until proper alignment is detected.
*
* (Refer to Freescale P4080 Chip Errata, Rev D or subsequent, erratum XAUI 4.)
*
* Note that this function accesses registers documented as reserved
* in the P4080 manual.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysXaui4War
    (
    int bank
    )
    {
    int lane = 0;
    UINT32 xaui = 0;
    UINT32 val;
    int i,j;

    if (bank == SRDS_BANK2)
        lane = SRDS_B2_LANEA;
    if (bank == SRDS_BANK3)
        lane = SRDS_B3_LANEA;

    if (bank == SRDS_BANK2)
        xaui = SRDS_PCONVCFG2_XGM2_RST;
    if (bank == SRDS_BANK3)
        xaui = SRDS_PCONVCFG2_XGM1_RST;

    /* 1) Put SERDES lanes into loopback mode. */

    for (i = 0; i < 4; i++)
        {
        SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK1, 0x07000000);
        SRDS_LN_SETBIT(lane + (i * 0x40), SRDS_LOOPBACK1, 0x01000000);
        SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK2, 0x0000003F);
        SRDS_LN_SETBIT(lane + (i * 0x40), SRDS_LOOPBACK2, 0x00002000);
        SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK3, 0x30000000);
        SRDS_LN_SETBIT(lane + (i * 0x40), SRDS_LOOPBACK3, 0x10000000);
        }

    for (j = 0; j < 10000; j++)
        {
        /* 2) Reset XAUI */
        SRDS_CLRBIT(SRDS_PCONV_CFG2, xaui);
        sysUsDelay (1);
        SRDS_SETBIT(SRDS_PCONV_CFG2, xaui);

        /* 3) Wait */
        sysUsDelay (1); 

        /* 4) if lanes are properly aligned */
        if (bank == SRDS_BANK2)
            val = SRDS_READ(SRDS_BANK2_ALIGN_STS);
        else
            val = SRDS_READ(SRDS_BANK3_ALIGN_STS);
            
        if ((val & 0x40000000) == 0)
            continue;

        /* 5) Bring SERDES out of loopback */

        for (i = 0; i < 4; i++)
            {
            SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK1, 0x07000000);
            SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK2, 0x00002000);
            SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_LOOPBACK3, 0x30000000);
            }

        /* 6) Reset RX lanes */

        for (i = 0; i < 4; i++)
            SRDS_LN_CLRBIT(lane + (i * 0x40), SRDS_GCR0, SRDS_GCR0_RRST);

        sysUsDelay (1);

        for (i = 0; i < 4; i++)
            SRDS_LN_SETBIT(lane + (i * 0x40), SRDS_GCR0, SRDS_GCR0_RRST);

        break;
        }

    val = SRDS_READ((bank == SRDS_BANK2) ? SRDS_BANK2_ALIGN_STS
                                         : SRDS_BANK3_ALIGN_STS);
    if ((val & 0x40000000) == 0)
        snprintf(hwErrStringEnd,hwErrStringSz,"sysXaui4War(%2.2d) "
                                              "not aligned\n",bank);
    return;
    }

/******************************************************************************
*
* sysSerdes8War - work around for P4080 erratum SERDES8 on rev 2 silicon
*
* This function implements a portion of the software workaround for erratum
* SERDES8 as implemented for rev 2 silicon. The erratum involves the presence
* of jitter in the PLL of one Serdes bank while another Serdes bank's PLL
* is in use. This occurs only when two or more banks have the same VCO
* frequency. This problem does not occur when two banks operating with
* different VCO frequencies and a 3rd bank is powered down.
*
* On rev 2 silicon, bank 3's PLL is used to clock both bank 2 and 3, and
* only a limited number of Serdes protocol combinations are available
* (compared to rev 1). Both bank 2 and bank 3 have their lane disable
* bits set in the RCW. This function will reprogram bank3's clock settings
* depending on bank 2's clocking requirements, and then clear bank 3's
* disable bits in the RCW.
*
* Note that changing RCW bits is supposed to be impossible, but can in
* fact be achieved by using registers in the DCSR space.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysSerdes8War
    (
    int cfg,
    UINT32 devdisr1,
    UINT32 devdisr2
    )
    {
    UINT32 devd1old;
    UINT32 devd2old;
    UINT32 b2rat;
    UINT32 refclk;
    UINT32 rcw5;

    devd1old = *P4080_DEVDISR1(CCSBAR);
    devd2old = *P4080_DEVDISR2(CCSBAR);

    /* Enable devices. */

    *P4080_DEVDISR1(CCSBAR) = (devd1old & ~(devdisr1));
    *P4080_DEVDISR2(CCSBAR) = (devd2old & ~(devdisr2));

    switch (cfg)
        {
        case 0x19:
            SRDS_BK_SETBIT(serdes_bank_off[2],
                SRDS_BANK_PCR1, SRDS_PCR1_PLLBW_SEL);
            break;

            /* AN4065 1.2.1.3-3b)1: */

        case 0x0F:
        case 0x10:
        case 0x2A:
            b2rat = (*P4080_RCWSR(CCSBAR,4) >> 13) & 0x7;
            switch (b2rat)
                {
                case 1: /* 20:1 */
                    refclk = SRDS_PCR0_RFCK_156MHZ;
                    break;
                case 2: /* 25:1 */
                    refclk = SRDS_PCR0_RFCK_125MHZ;
                    break;
                default:
                    /* invalid setting */
                    return;
                }

            SRDS_BK_CLRBIT(serdes_bank_off[2],
                SRDS_BANK_PCR0, SRDS_PCR0_RFCK_SEL);
            SRDS_BK_SETBIT(serdes_bank_off[2],
                SRDS_BANK_PCR0, refclk);

            /* AN4065 1.2.1.3-3b)2: */

            SRDS_BK_CLRBIT(serdes_bank_off[2],
                SRDS_BANK_PCR0, SRDS_PCR0_FRATE_SEL);
            SRDS_BK_SETBIT(serdes_bank_off[2],
                SRDS_BANK_PCR0, SRDS_PCR0_FRATE_6_25GHZ);

            break;

        default:
            break;
        }

    /* 
     * AN4065 1.2.1.3-b)3 and 1.2.1.3-c)1: RGMII on FM1. (missing ???)
     * Table 25-12 doesn't appear to exist in the P4080 RM.
     */

    /* Reprogram the bank enable bits in the RCW. */

    /* AN4065 1.2.1.3-4: */
    rcw5 = *P4080_RCWSR(CCSBAR,5);
    rcw5 &= ~0x003C0000;
    rcw5 |= P4080_ERRATUM_SERDES8_BANK3 << 18;
    DCSR_WRITE(DCSR_RCW5, rcw5);

    return;
    }

/******************************************************************************
*
* sysSerdesLaneEnabled - test to see if a given Serdes Lane is enabled
*
* This function tests to see whether a specified serdes lane is enabled.
* This is determined via three methods. First, as each lane is part of a
* bank, the bank power down bit for the bank in which the lane resides is
* checked. If the whole bank is disabled, then the lane is unavailable.
* Second, the code is running on rev 2 silicon, the lanes are checked
* against the compile-time P4080_ERRATUM_SERDES8_BANK2 and
* P4080_ERRATUM_SERDES8_BANK3 masks. The workaround for SERDES8 involves
* turning off the lanes for banks 2 and 3 in the RCW and then enabling
* them as needed later. Lastly, the RCW lane disable bits for each lane
* are checked to that the state of each lane can be ascertained.
*
* RETURNS: TRUE if a lane is enabled, otherwise false
*
* ERRNO: N/A
*/

LOCAL BOOL sysSerdesLaneEnabled
    (
    int lane
    )
    {
    UINT32 word;
    UINT32 bit;
    UINT32 bank;
    UINT32 chipRev;
    UINT32 rcw;
    int bankOffset;

    bank = serdes_lane_off[lane].bank;
    bankOffset = serdes_bank_off[bank];   /* fix to get correct offset to bank */
    word = serdes_lane_off[lane].lpd / 32;
    bit = 31 - (serdes_lane_off[lane].lpd % 32);

    if (SRDS_BK_READ(bankOffset, SRDS_BANK_RCR) & SRDS_RCR_SDPD)
        return (FALSE);

    chipRev = (vxSvrGet() & 0x000000F0) >> 4;
    if (chipRev != 1)
        {
        if (lane >= 10 && lane <= 13)
            return !(P4080_ERRATUM_SERDES8_BANK2 &
                (8 >> (lane - 10)));

        if (lane >= 14 && lane <= 17)
            return !(P4080_ERRATUM_SERDES8_BANK3 &
                (8 >> (lane - 14)));
       }

    rcw = *P4080_RCWSR(CCSBAR, word) & (1 << bit);

    if (rcw)
        return (FALSE);

    return (TRUE);
    }

/******************************************************************************
*
* sysSerdesPrtclGet - find the protocol for a given serfes lane
*
* This function looks up the protocol type for a given Serdes lane in the
* serdes_cfg_tbl[] array, based on the SRDS_PRTCL bits in the RCW and the
* specified lane supplied by the caller. The returned protocol type can
* be PCIE1, PCIE2, PCIE3, SGMII_FM1, SGMII_FM2, XAUI_FM1, XAUI_FM2,
* SRIO1, SRIO2, AURORA or NOPROTO. The routine also calls
* sysSerdesLaneEnabled() to confirm if the requested lane is actually
* enabled. If not, it will return NOPROTO.
*
* RETURNS: the protocol type as defined in the serdes_cfg_tbl[] array,
*          or NOPROTO if the lane is not enabled
*
* ERRNO: N/A
*/

LOCAL int sysSerdesPrtclGet
    (
    int cfg,
    int lane
    )
    {
    if (sysSerdesLaneEnabled (lane) == FALSE)
        return (NOPROTO);
    return (serdes_cfg_tbl[cfg][lane]);
    }

/******************************************************************************
*
* sysSerdesFix - work around P4080 Rev 1 SERDES errata
*
* This function implements a software workarounds for several SERDES
* errata in the Rev 1 P4080 chip. These are fixups for default register
* values which are not properly initialized after a hardware reset.
* These workarounds must be applied in order for the SGMII and XAUI
* media adapter cards to work correctly, and may also be needed for
* PCIE, SRIO and AURORA debug functionality.
*
* The routine determines the SERDES setup based on the reset control
* word (RCW) and automatically applies the errata workarounds as needed.
* Consequently, this code should work for all configurations.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysSerdesFix (void)
    {
    int i, j;
    UINT32 prtcl;
    UINT32 rcw;
    int cfg = 0;
    int lane;
    int bank;
    BOOL bank2xaui = FALSE;
    BOOL bank3xaui = FALSE;
    UINT32 devdisr1 = 0;
    UINT32 devdisr2 = 0;
    UINT32 bankEnabled [SRDS_MAX_BANKS];
    int chipRev;
    UINT32 tmp;

    for (i=0; (i < NELEMENTS(sysSerdesRcwActual)) && (i < 0x10); i++)
        sysSerdesRcwActual[i] = *P4080_RCWSR(CCSBAR,i);

    chipRev = (vxSvrGet() & 0x000000F0) >> 4;

    /*
     * On rev 2 silicon, SERDES 8 workaround requires
     * access to the DCSR LAW (AN4065 1.2.1.3-4 and 6)
     */

    if (chipRev != 1)
        {
        *P4080_LAWBARH29(CCSBAR) = 0;
        *P4080_LAWBARL29(CCSBAR) = DCSR_LAW_BASE;
        _WRS_ASM (" sync");
        *P4080_LAWAR29(CCSBAR) = LAWAR_ENABLE |
        LAWAR_TGTIF_DCSR | LAWAR_SIZE_4MB;
        tmp = *P4080_LAWAR29(CCSBAR);
        _WRS_ASM (" isync");
        }

    bzero ((char *)&bankEnabled , sizeof(bankEnabled ));

    /*
     * If SerDes is not enabled, don't bother with the rest
     * of the workaround code.
     */

    rcw = *P4080_RCWSR(CCSBAR,5);
    if (!(rcw & 0x2000))
        return;

    /* 
     * Per AN4065 1.2.1.3: Explicitly set RCW SRDS_LPD_B2/3 => 0xF.
     * The RCW registers are restored based on the config.h defines:
     * P4080_ERRATUM_SERDES8_BANK2 and 3; NOT the RCW flash values.
     */

    rcw |=  0x3C3C0000;
    DCSR_WRITE(DCSR_RCW5, rcw);

    /* Look for banks with all lanes disabled, and power down the bank. */

    for (i = 0; i < SRDS_MAX_LANES; i++)
        {
        if (sysSerdesLaneEnabled (i))
            bankEnabled [serdes_lane_off[i].bank] = 1;
        }

    /*
     * For rev 1 silicon, at least one bank must be turned off
     * to work around the SERDES8 erratum. We look for a bank
     * to disable based on the lane power down bits first. If
     * none of them are disabled, we force bank 3 off.
     */

    if (chipRev == 1)
        {
        for (i = 0; i < SRDS_MAX_BANKS; i++)
            {
            if (!bankEnabled [i])
                break;
            }
            if (i == SRDS_MAX_BANKS)
                bankEnabled [2] = 0;
        }
    else
        {
        /*
         * For rev 2 silicon, if bank 2 is on, bank 3 must also
         * be enabled so that we can use its VCO clock.
         */

        if (bankEnabled [1])
            bankEnabled [2] = 1;
        }

    for (i = 0; i < SRDS_MAX_BANKS; i++)
        {
        if (bankEnabled [i] == 0)
            SRDS_BK_SETBIT(serdes_bank_off[i], SRDS_BANK_RCR, SRDS_RCR_SDPD);
        }

    rcw = *P4080_RCWSR(CCSBAR,4);
    prtcl = (rcw & RCWSR4_SRDS_PRTCL) >> 26;

    /*
     * Per AN4065 1.2.1.3-1: If Bank 1 used, wait for SRDSB1RSTCTL[RST_DONE].  
     * Per P4080EC table 15 the max PLL Lock time is 100 us. 
     */

    if (bankEnabled [0])
        {
        for (j = 0; j < 100; j++)
            {
            if (SRDS_BK_READ(SRDS_BANK1, SRDS_BANK_RCR) & SRDS_RCR_RSTDONE)
                break;
            sysUsDelay(10);
            }

        if ((SRDS_BK_READ(SRDS_BANK1, SRDS_BANK_RCR) & SRDS_RCR_RSTDONE) == 0)
            snprintf(hwErrStringEnd,hwErrStringSz,"SerDes bank1 PLL "
                                                  "didn't lock\n");
        }

    /* AN4065 1.2.1.3-2: enable bank 2/3 IPs per protocol */
    for (i = 0; i < SRDS_MAX_LANES; i++)
        {
        cfg = serdes_cfg_tbl[prtcl][i];
        lane = serdes_lane_off[i].off;
        switch (cfg)
            {
            case PCIE1:
            case PCIE2:
            case PCIE3:
                /* SERDES2 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_CTL);
                SRDS_LN_SETBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_PEX);

                /* SERDES4 */
                if (i && (sysSerdesPrtclGet(prtcl, i - 1) == cfg))
                    SRDS_LN_CLRBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);
                else
                    SRDS_LN_SETBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);

                /* SERDES6 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_OPAD);

                if (cfg == PCIE1)
                   devdisr1 |= P4080_DEVDISR1_PEX1;
                if (cfg == PCIE2)
                   devdisr1 |= P4080_DEVDISR1_PEX2;
                if (cfg == PCIE3)
                   devdisr1 |= P4080_DEVDISR1_PEX3;
                break;

            case SRIO1:
            case SRIO2:
                /* SERDES2 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_CTL);
                SRDS_LN_SETBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_SRIO);

                /* SERDES3 */
                SRDS_LN_CLRBIT(lane, SRDS_TXEQ, SRDS_TXEQ_TYPE);
                SRDS_LN_SETBIT(lane, SRDS_TXEQ, SRDS_TXEQTYPE_2LVL);

                /* SERDES4 */
                /* Per Table 3-15 of the P4080... Reference Manual, if the
                 * next lane is not SRIO we're on the 1st lane.
                 */

                if ((i != (SRDS_MAX_LANES - 1)) &&
                    (sysSerdesPrtclGet(prtcl, i + 1) == cfg))
                    SRDS_LN_CLRBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);
                else
                    SRDS_LN_SETBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);

                /* SERDES6 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_OPAD);

                if (cfg == SRIO1)
                    devdisr1 |= P4080_DEVDISR1_SRIO1;
                if (cfg == SRIO2)
                    devdisr1 |= P4080_DEVDISR1_SRIO2;

                break;

            case SGMII_FM1:
            case SGMII_FM2:
                /* SERDES2 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_CTL);
                SRDS_LN_SETBIT(lane, SRDS_GCR1, SRDS_GCR1_REIDL_SGMII);

                /* SERDES3 */
                SRDS_LN_CLRBIT(lane, SRDS_TXEQ, SRDS_TXEQ_TYPE);
                SRDS_LN_SETBIT(lane, SRDS_TXEQ, SRDS_TXEQTYPE_2LVL);

                /* SERDES6 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_OPAD);

                if (cfg == SGMII_FM1)
                    devdisr2 |= P4080_DEVDISR2_FM1 | P4080_DEVDISR2_DTSEC1_1 |
                        P4080_DEVDISR2_DTSEC1_2 | P4080_DEVDISR2_DTSEC1_3 |
                        P4080_DEVDISR2_DTSEC1_4;
                if (cfg == SGMII_FM2)
                    devdisr2 |= P4080_DEVDISR2_FM2 | P4080_DEVDISR2_DTSEC2_1 |
                        P4080_DEVDISR2_DTSEC2_2 | P4080_DEVDISR2_DTSEC2_3 |
                        P4080_DEVDISR2_DTSEC2_4;

                /*
                 * Note: protocols 0x0B, 0x0C, 0x0D and 0x0E only enable
                 * DTSECS 3 and 4 on FM2.
                 */

                if (prtcl >= 0x0B && prtcl <= 0x0E)
                    devdisr2 &= ~(P4080_DEVDISR2_DTSEC2_1 |
                        P4080_DEVDISR2_DTSEC2_2);

                break;

            case XAUI_FM1:
            case XAUI_FM2:
                if (lane >= SRDS_B3_LANEA && sysSerdesLaneEnabled(i))
                    bank3xaui = TRUE;
                else if (lane >= SRDS_B2_LANEA && sysSerdesLaneEnabled(i))
                    bank2xaui = TRUE;

                /* SERDES3 */
                SRDS_LN_CLRBIT(lane, SRDS_TXEQ, SRDS_TXEQ_TYPE);
                SRDS_LN_SETBIT(lane, SRDS_TXEQ, SRDS_TXEQTYPE_2LVL);

                /* SERDES4 */
                if (i && (sysSerdesPrtclGet(prtcl, i - 1) == cfg))
                    SRDS_LN_CLRBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);

                /* SERDES6 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_OPAD);

                if (cfg == XAUI_FM1)
                    devdisr2 |= P4080_DEVDISR2_FM1|P4080_DEVDISR2_XAUI1;
                if (cfg == XAUI_FM2)
                    devdisr2 |= P4080_DEVDISR2_FM2|P4080_DEVDISR2_XAUI2;

                break;

            case AURORA:
                /* SERDES3 */
                SRDS_LN_CLRBIT(lane, SRDS_TXEQ, SRDS_TXEQ_TYPE);
                SRDS_LN_SETBIT(lane, SRDS_TXEQ, SRDS_TXEQTYPE_2LVL);

                /* SERDES4 */
                /* Per Table 3-15 of the P4080... Reference Manual, if the
                 * previous lane is not AURORA we're on the last of 2 lanes
                 */

                if (i && (serdes_cfg_tbl[prtcl][i - 1] == cfg))
                    SRDS_LN_SETBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);
                else
                    SRDS_LN_CLRBIT(lane, SRDS_GCR0, SRDS_GCR0_1STLANE);

                /* SERDES6 */
                SRDS_LN_CLRBIT(lane, SRDS_GCR1, SRDS_GCR1_OPAD);

                break;

            default:
                break;
            }
        }

    /* Reverse the order of the lanes. */

    for (i = SRDS_MAX_BANKS - 1; i > -1; i--)
        {
        if (bankEnabled [i] == 0)
            continue;

        bank = serdes_bank_off[i];

        if (chipRev != 1)
            {
            if (i == 2)
                sysSerdes8War (prtcl, devdisr1, devdisr2);
            else if (i == 1)
                {
                /* AN4065 1.2.1.3-6: */
                
                rcw = *P4080_RCWSR(CCSBAR, 5);
                rcw &= ~0x3C000000;
                rcw |= P4080_ERRATUM_SERDES8_BANK2 << 26;
                DCSR_WRITE(DCSR_RCW5, rcw);
                }
            }
            
        if (chipRev == 3 )
           {
            /* Enabling the lanes for banks 2 in the RCW. */

            DCSR_WRITE(DCSR_RCW5, 0x382000);
           }

         /*
         * Per AN4065 1.2.1.3-1: wait for SRDSB1RSTCTL[RST_DONE].  
         * Per P4080EC table 15 the max PLL Lock time is 100 us. 
         * However, testing showed 100 us isn't enough so it's 10x.
         */

        for (j = 0; j < 100; j++)
            {
            if (SRDS_BK_READ(bank, SRDS_BANK_RCR) & SRDS_RCR_RSTDONE)
                break;
            sysUsDelay(10);
            }

        if ((SRDS_BK_READ(bank, SRDS_BANK_RCR) & SRDS_RCR_RSTDONE) == 0)
            snprintf(hwErrStringEnd,hwErrStringSz,"SerDes bank %1.1d PLL "
                                                  "didn't lock\n",i);

        /* Set PLL bandwidth */

        SRDS_BK_CLRBIT(bank, SRDS_BANK_PCR1, SRDS_PCR1_PLLBW_SEL);

        /* Apply XAUI4 workaround for banks that need it (fixed in Rev 2.0) */

        if (chipRev == 1)
            {
            if (bank == SRDS_BANK2 && bank2xaui)
                sysXaui4War (bank);
            if (bank == SRDS_BANK3 && bank3xaui)
                sysXaui4War (bank);
            }
        }

    /*
     * On rev 2 silicon, SERDES 8 workaround requires
     * access to the DCSR LAW (AN4065 1.2.1.3-4 and 6)
     * Now removing that access.
     */

    if (chipRev != 1)
        {
        *P4080_LAWAR29(CCSBAR) = 0;
        *P4080_LAWBARH29(CCSBAR) = 0;
        _WRS_ASM (" sync");
        *P4080_LAWBARL29(CCSBAR) = 0; 
        tmp = *P4080_LAWBARL29(CCSBAR);
        _WRS_ASM (" isync");
        }
    }

#ifdef INCLUDE_SHOW_ROUTINES
/******************************************************************************
*
* sysSerdesShow - dump out misc. information about the SerDes hardware.
*
* Dump the P4080 chip rev, SERDES lanes enabled and the RCW.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysSerdesShow()
    {
    int    i;
    UINT32 rcw = *P4080_RCWSR(CCSBAR,4);

    printf("                         ABCDEFGHIJabcdABCD\n");
    printf("Rev:%1.1d, PRTCL:0x%2.2x Lanes: ", (vxSvrGet() & 0x000000F0) >> 4,
                                               (rcw & RCWSR4_SRDS_PRTCL) >> 26);
    for (i = 0; i < SRDS_MAX_LANES; i++)
        printf("%c",(sysSerdesLaneEnabled (i)) ? '1':'0');

    printf("\nRCW: 0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n"
           "     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n",
           *P4080_RCWSR(CCSBAR,0), *P4080_RCWSR(CCSBAR,1),
           *P4080_RCWSR(CCSBAR,2), *P4080_RCWSR(CCSBAR,3),
           *P4080_RCWSR(CCSBAR,4), *P4080_RCWSR(CCSBAR,5),
           *P4080_RCWSR(CCSBAR,6), *P4080_RCWSR(CCSBAR,7) );

    printf("     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n"
           "     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n\n",
           *P4080_RCWSR(CCSBAR,0x8), *P4080_RCWSR(CCSBAR,0x9),
           *P4080_RCWSR(CCSBAR,0xA), *P4080_RCWSR(CCSBAR,0xB),
           *P4080_RCWSR(CCSBAR,0xC), *P4080_RCWSR(CCSBAR,0xD),
           *P4080_RCWSR(CCSBAR,0xE), *P4080_RCWSR(CCSBAR,0xF) );

    printf("\norig:0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n"
           "     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n",
           sysSerdesRcwActual[0], sysSerdesRcwActual[1],
           sysSerdesRcwActual[2], sysSerdesRcwActual[3],
           sysSerdesRcwActual[4], sysSerdesRcwActual[5],
           sysSerdesRcwActual[6], sysSerdesRcwActual[7]);

    printf("     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n"
           "     0x%8.8x, 0x%8.8x, 0x%8.8x, 0x%8.8x\n\n",
           sysSerdesRcwActual[0x8], sysSerdesRcwActual[0x9],
           sysSerdesRcwActual[0xA], sysSerdesRcwActual[0xB],
           sysSerdesRcwActual[0xC], sysSerdesRcwActual[0xD],
           sysSerdesRcwActual[0xE], sysSerdesRcwActual[0xF]);

    printf(" DEVDISR1: 0x%8.8x, DEVDISR2: 0x%8.8x\n\n",
           *P4080_DEVDISR1(CCSBAR), *P4080_DEVDISR2(CCSBAR));

    printf("%.200s\n\n", sysHwErrorString);
    }
#endif /* INCLUDE_SHOW_ROUTINES */


