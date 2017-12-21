/* eeprom.h - EEPROM NVRAM haeder file */

/* Copyright 1984-2001 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01b,12oct01,g_h  Clean for T2.2
01a,15jun98,est	 Created
*/

/*
DESCRIPTION

This file contains routines prototypes to access the eeprom NVRAM device.

SEE ALSO:
.pG "Configuration"
*/


#ifndef	__INCeepromh
#define	__INCeepromh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include "memLib.h"

#ifdef  NV_RAM_SIZE
#define NV_RAM_SETUP    eepromNvRamSetup
#undef  NV_RAM_WRITE
#define NV_RAM_WRITE  	eepromWriteByte
#undef  NV_RAM_READ
#define NV_RAM_READ	eepromReadByte
#endif
/* function prototypes */

void   eepromNvRamSetup ( void );
UINT8  eepromReadByte ( int offset );
STATUS eepromWriteByte ( int offset, UINT8 data );

#ifdef __cplusplus
}
#endif

#endif	/* __INCeepromh */

