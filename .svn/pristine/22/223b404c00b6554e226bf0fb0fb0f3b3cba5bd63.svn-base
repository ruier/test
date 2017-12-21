/* eeprom.h - EEPROM NVRAM header file */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23jun05,j_b  adapted from wrPpmc8260 (rev 01b)
*/

/*
DESCRIPTION

This file contains routines prototypes to access the EEPROM NVRAM device.

SEE ALSO:
.pG "Configuration"
*/


#ifndef	__INCeepromh
#define	__INCeepromh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <vxWorks.h>
#include <memLib.h>

/* defines */

#define NV_WR_CYCLE_TIME    1 /* EEPROM write cycle time (ms) */
#define RETRY_NVWRITE_COUNT 5 /* max wait iterations before write abort */

#undef NV_RAM_WR_ENBL       /* write enable procedure */
#define NV_RAM_WR_ENBL      eepromUnlock()

#undef NV_RAM_WR_DSBL       /* write disable procedure */
#define NV_RAM_WR_DSBL      eepromLock()

#undef  NV_RAM_WRITE        /* write procedure */
#define NV_RAM_WRITE        eepromWriteByte

#undef  NV_RAM_READ         /* read procedure */
#define NV_RAM_READ         eepromReadByte

/* function prototypes */

UINT8  eepromReadByte (int offset);
STATUS eepromWriteByte (int offset, UINT8 data);
void   eepromLock (void);
void   eepromUnlock (void);


#ifdef __cplusplus
}
#endif

#endif	/* __INCeepromh */

