/* sysLed.h - system-dependent LED driver Header File */

/* Copyright 1984-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15apr03,j_b  add defines for hex LED values
01a,01apr03,j_b  adapted from unsupported idp0x0 BSPs
*/


#ifndef  __INCsysLedh
#define  __INCsysLedh

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
#define	LED_DIGIT		0x7f			/* digit mask        */
#define	LED_POINT		0x80			/* point mask        */

#define LED_ALLON		0x00
#define LED_ALLOFF		0xff

#define LED_0			0xc0
#define LED_1			0xf9
#define LED_2			0xa4
#define LED_3			0xb0
#define LED_4			0x99
#define LED_5			0x92
#define LED_6			0x82
#define LED_7			0xf8
#define LED_8			0x80
#define LED_9			0x90
#define LED_A			0x88
#define LED_B			0x83
#define LED_C			0xc6
#define LED_D			0xa1
#define LED_E			0x86
#define LED_F			0x8e

/* Prototypes */

#ifndef _ASMLANGUAGE

#if defined(__STDC__) || defined(__cplusplus)

IMPORT void sysLedInit(void);
IMPORT UINT8 sysLedSet(UINT8 mask, UINT8 val);
IMPORT void sysLedHex(UINT8 val);

#else

IMPORT void sysLedInit();
IMPORT UINT8 sysLedSet();
IMPORT void sysLedHex();

#endif  /* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif   /* __INCsysLedh  */

