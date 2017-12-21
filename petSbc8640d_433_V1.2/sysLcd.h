/* sysLcd.h - system-dependent LCD driver Header File */

/*
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10nov06,kds  Created.
*/


#ifndef  __INCsysLcdh
#define  __INCsysLcdh

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */

#define LCD_BASE_ADRS        0xf4000000
#define LCD_CNTRST_DWN_ADRS  LCD_BASE_ADRS

/* These address offsets are determined by the EPLD */

#define LCD_CNTRST_UP_ADRS   (LCD_BASE_ADRS + 0x1)
#define LCD_INSTR_ADRS       (LCD_BASE_ADRS + 0x2)
#define LCD_DATA_ADRS        (LCD_BASE_ADRS + 0x3)

/* Commands */

/* Display Clear */

#define LCD_DISPL_CLEAR   0x01

/* Entry Mode Set */

#define LCD_INCREMENT        0x06
#define LCD_DECREMENT        0x04
#define LCD_DISPL_SHIFT_ON   0x05

/* Display Mode */

#define LCD_DISPL_ON         0x0c
#define LCD_CURSOR_DISPL_ON  0x0a
#define LCD_CURSOR_BLINK_ON  0x09

/* Display/Cursor Shift */

#define LCD_SHIFT_DISPL   0x18
#define LCD_MOVE_CURSOR   0x10
#define LCD_SHIFT_RIGHT   0x14
#define LCD_SHIFT_LEFT    0x10

/* Function Set */

#define LCD_DL_8BIT       0x30
#define LCD_DL_4BIT       0x20
#define LCD_DUAL_LINE     0x28
#define LCD_SINGLE_LINE   0x20
#define LCD_FONT_5x10     0x24
#define LCD_FONT_5x8      0x20

/* macros */

#define LCD_WRITE_INSTR(instr)   (*((volatile UINT8 *)(LCD_INSTR_ADRS)) = (instr));
#define LCD_WRITE_DATA(byte)     (*((volatile UINT8 *)(LCD_DATA_ADRS)) = (byte));
#define LCD_CONTRAST_UP          (*((volatile UINT8 *)(LCD_CNTRST_UP_ADRS)) = 0x00);
#define LCD_CONTRAST_DWN         (*((volatile UINT8 *)(LCD_CNTRST_DWN_ADRS)) = 0x00);


/* Characters */

#define LCD_ZERO   0x30
#define LCD_1      0x31
#define LCD_2      0x32
#define LCD_3      0x33
#define LCD_4      0x34
#define LCD_5      0x35
#define LCD_6      0x36
#define LCD_7      0x37
#define LCD_8      0x38
#define LCD_9      0x39
#define LCD_A      0x41
#define LCD_B      0x42
#define LCD_C      0x43
#define LCD_D      0x44
#define LCD_E      0x45
#define LCD_F      0x46
#define LCD_G      0x47
#define LCD_H      0x48
#define LCD_I      0x49
#define LCD_J      0x4a
#define LCD_K      0x4b
#define LCD_L      0x4c
#define LCD_M      0x4d
#define LCD_N      0x4e
#define LCD_O      0x4f
#define LCD_P      0x50
#define LCD_Q      0x51
#define LCD_R      0x52
#define LCD_S      0x53
#define LCD_T      0x54
#define LCD_U      0x55
#define LCD_V      0x56
#define LCD_W      0x57
#define LCD_X      0x58
#define LCD_Y      0x59
#define LCD_Z      0x5a
#define LCD_SPACE  0x20
#define LCD_POINT  0x2e

/* Prototypes */

#ifndef _ASMLANGUAGE

IMPORT void sysLcdInit(void);
IMPORT void sysLcdWriteInstr(UINT8 instr);
IMPORT void sysLcdWriteData(UINT8 data);
IMPORT void sysLcdClearDisplay (void);
IMPORT void sysLcdContrastUp (void);
IMPORT void sysLcdContrastDown (void);
IMPORT void sysLcdSbcSet(void);

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif   /* __INCsysLcdh  */

