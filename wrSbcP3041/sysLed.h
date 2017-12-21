/* sysLed.h - system-dependent LED driver Header File */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,29nov10,e_d  Code clean
01a,22feb10,kds  Created
*/


#ifndef  __INCsysLedh
#define  __INCsysLedh

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */

#define LED_ALLON       0xFF
#define LED_ALLOFF      0x00

#define LED_ON          1
#define LED_OFF         0

#define LED_0           0x80
#define LED_1           0x40
#define LED_2           0x20
#define LED_3           0x10
#define LED_4           0x08
#define LED_5           0x04
#define LED_6           0x02
#define LED_7           0x01

#define USER_LED_ADRS   CPLD_BASE + 0x3

/* Prototypes */

#ifndef _ASMLANGUAGE

#if defined(__STDC__) || defined(__cplusplus)

IMPORT void sysLedInit(void);
IMPORT UINT8 sysLedSet(UINT8 mask, UINT8 val);
IMPORT void sysLedsAllFlash(void);
IMPORT void sysLedsCascade(void);

#else

IMPORT void sysLedInit();
IMPORT UINT8 sysLedSet();
IMPORT void sysLedsAllFlash();
IMPORT void sysLedsCascade();

#endif  /* __STDC__ */

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif   /* __INCsysLedh  */

