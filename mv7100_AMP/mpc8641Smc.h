/* mpc8641Smc.h - MPC8641 DDR Memory Controller Definitions File */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01b,17mar08,mdg  Update for 8641D from 8540 version.
01a,30sep04,cak  Initial version.
*/

/*
 * This file contains the structures and definitions used to program
 * the MPC8641 DDR memory controller.
 *
 */

#ifndef INCmpc8641Smch
#define INCmpc8641Smch

#ifdef __cplusplus
   extern "C" {
#endif

#define MPC8641_CHIP_SELECTS    4

/* short-cut DRAM control structure template */

#ifndef _ASMLANGUAGE

typedef struct mpc8641Smc
    {
    UINT32 csnBnds [MPC8641_CHIP_SELECTS];  /* Chip Select Bounds */
    UINT32 csnCfg  [MPC8641_CHIP_SELECTS];  /* Chip Select Config */
    UINT32 sdramTmngConfig1;                /* SDRAM Timing Config 1 */
    UINT32 sdramTmngConfig2;                /* SDRAM Timing Config 2 */
    UINT32 sdramCtrlConfig;                 /* SDRAM Control Config */
    UINT32 sdramModeConfig;                 /* SDRAM Mode Config */
    UINT32 sdramIntervalConfig;             /* SDRAM Interval Config */
    } MPC8641_SMC;

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif /* INCmpc8641Smch */
