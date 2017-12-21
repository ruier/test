/* mpc8540Smc.h - MPC8540 DDR Memory Controller Definitions File */

/* Copyright 1999 Wind River Systems, Inc. */
/* Copyright 2004 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,30sep04,cak  Initial version. 
*/

/*
 * This file contains the structures and definitions used to program
 * the MPC8540 DDR memory controller.
 *
 */

#ifndef	INCmpc8540Smch
#define	INCmpc8540Smch

#ifdef __cplusplus
   extern "C" {
#endif

#define MPC8540_CHIP_SELECTS   	4 

/* short-cut DRAM control structure template */

#ifndef _ASMLANGUAGE

typedef struct mpc8540Smc 
    {
    UINT32 csnBnds [MPC8540_CHIP_SELECTS];	/* Chip Select Bounds */
    UINT32 csnCfg [MPC8540_CHIP_SELECTS];	/* Chip Select Config */
    UINT32 sdramTmngConfig1;			/* SDRAM Timing Config 1 */
    UINT32 sdramTmngConfig2;			/* SDRAM Timing Config 2 */
    UINT32 sdramCtrlConfig;			/* SDRAM Control Config */
    UINT32 sdramModeConfig;			/* SDRAM Mode Config */
    UINT32 sdramIntervalConfig;			/* SDRAM Interval Config */
    } MPC8540_SMC;

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif /* INCmpc8540Smch */
