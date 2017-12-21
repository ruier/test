/* sysLed.h - system-dependent LED driver Header File */

/*
 * Copyright (c) 2005,2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,09jan09,syt  Fix LED_xx  macro error. (WIND00143743)
01a,11aug05,kds  adapted for wrSbc8548 from wrSbc834x (rev 01a)
*/


#ifndef  __INCsysLedh
#define  __INCsysLedh

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
#define LED_DIGIT       0xfe            /* digit mask        */
#define LED_POINT       0x01            /* point mask        */

#define LED_ALLON		0x00
#define LED_ALLOFF		0xff

#define LED_0           0x03
#define LED_1           0x9f
#define LED_2           0x25
#define LED_3           0x0d
#define LED_4           0x99
#define LED_5           0x49
#define LED_6           0x41
#define LED_7           0x1b
#define LED_8           0x01
#define LED_9           0x19
#define LED_A           0x11
#define LED_B           0xc1
#define LED_C           0x63
#define LED_D           0x85
#define LED_E           0x61
#define LED_F           0x71

#define USER_LED_ADRS            LBC_CS5_LOCAL_ADRS
#define DEMO_LED_ADRS            (LBC_CS5_LOCAL_ADRS + 0x00900000)

/* Prototypes */

#ifndef _ASMLANGUAGE

#if defined(__STDC__) || defined(__cplusplus)

IMPORT void sysLedInit(void);
IMPORT UINT8 sysLedSet(UINT8 mask, UINT8 val);
IMPORT void sysLedHex(UINT8 val);
IMPORT void sysLedsAllFlash(void);
IMPORT void sysLedsCascade(void);
IMPORT void sysDemoLedsCascade(void);
IMPORT void sysDemoLedsAllFlash(void);

#else

IMPORT void sysLedInit();
IMPORT UINT8 sysLedSet();
IMPORT void sysLedHex();
IMPORT void sysLedsAllFlash();
IMPORT void sysLedsCascade();
IMPORT void sysDemoLedsCascade();
IMPORT void sysDemoLedsAllFlash();

#endif  /* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif   /* __INCsysLedh  */

