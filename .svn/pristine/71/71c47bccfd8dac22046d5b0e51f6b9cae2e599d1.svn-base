/* sysMpc8540Tmr.h - Mpc8540 Timer Header File */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,01aug05,pjh  New file.
*/

#ifndef INCsysMpc8540Tmrh
#define INCsysMpc8540Tmrh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/*
DESCRIPTION
This file contains the external tick-timer and global timer definitions and
prototypes for the MVME3100.
*/

#ifdef INCLUDE_SHOW_ROUTINES
    STATUS sysTmrShow (void);
#endif /* INCLUDE_SHOW_ROUTINES */

STATUS sysMpc8540TmrInit ();

STATUS sysTickTmrInit ();

STATUS sysMpc8540TmrStart (UINT32, UINT32);

STATUS sysTickTmrStart (UINT32, UINT32, UINT16);

UINT32 sysMpc8540TmrValueGet (UINT32);

UINT32 sysTickTmrValueGet (UINT32);

STATUS sysMpc8540TmrConnectRoutine (FUNCPTR, int, int);

STATUS sysTickTmrConnectRoutine (FUNCPTR, int, int);

STATUS sysMpc8540TmrDisconnectRoutine (UINT32);

STATUS sysMpc8540TmrDisable (UINT32);

STATUS sysTickTmrDisable (UINT32);

STATUS sysMpc8540TmrFreqSet (UINT32);

STATUS sysTickTmrFreqSet (UINT32);

UINT32 sysMpc8540TmrFreqGet ();

UINT32 sysTickTmrFreqGet ();

STATUS sysMpc8540TmrClockRatioSet (UINT32);

STATUS sysMpc8540TmrCascade (UINT32);

STATUS sysMpc8540TmrRolloverSet (UINT32);

STATUS sysMpc8540CascadeTmrStart (UINT32, UINT32);

UINT32 sysMpc8540CycleValueCalc (UINT32);

STATUS sysMpc8540TmrSrcSet (UINT32);

STATUS sysTickTmrDisconnectRoutine (UINT32);

STATUS sysTickTmrOverflowClear (UINT32);

UINT32 sysTickTmrOverflowValGet (UINT32);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCsysMpc8540Tmrh */
