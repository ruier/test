/* configNet.h - network configuration header for wrSbcPowerQuiccII */

/* Copyright (c) 2003 - 2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01h,09nov05,pch  SPR 114494: make fcc1 and fcc2 separately selectable
01g,17dec04,ckj  WLAN support for Atheros AR521X
01f,08aug03,scm  remove references to GEI for release...
01e,05aug03,scm  update to support basic PCI-to-PCI bridging...
01d,22may03,jb3  Add multiple motfcc to DevTbl.
01c,09may03,scm  add fei support...
01b,01may03,jb3  add multi FCC suport AGAIN!
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01c).

*/

#ifndef __INCconfigNeth
#define __INCconfigNeth

#ifdef __cplusplus
    extern "C" {
#endif

#include "vxWorks.h"
#include "end.h"
#include "lstLib.h"

#ifdef INCLUDE_MOTFCCEND
  #include "miiLib.h"
#endif /* INCLUDE_MOTFCCEND */

#include "config.h"

#ifdef INCLUDE_MOTSCCEND

#define SCC_LOAD_FUNC	sysMotSccEndLoad

/*
 * the default initialization string for the SCC is built in sysMotSccEnd.c
 */

#define SCC_LOAD_STRING  ""

IMPORT END_OBJ* SCC_LOAD_FUNC (char *, void*);

#endif /* INCLUDE_MOTSCCEND */

#ifdef INCLUDE_MOTFCCEND

/* Motorola Fast Ethernet Controller */

#define MOT_FCC_LOAD_FUNC    sysMotFccEndLoad

/*
 * the default initialization string for the FCC is built in sysMotFccEnd.c
 */

#define MOT_FCC_LOAD_STRING  ""

IMPORT END_OBJ* MOT_FCC_LOAD_FUNC (char *, void*);

#endif /* INCLUDE_MOTFCCEND */

#ifdef INCLUDE_FEI_END

#define FEI_LOAD_FUNC   sysFei82557EndLoad
/* The END_LOAD_STRING is defined empty and created dynamicaly */
#define FEI_LOAD_STRING ""                      /* created in sysFei82557EndLoad */
#define FEI_BUFF_LOAN   1

/*
 * The fei82557End initialization string format is:
 *
 * <memBase>:<memSize>:<rxDesNum>:<txDesNum>:<userFlags>:<offset>
 */

IMPORT END_OBJ * FEI_LOAD_FUNC (char *, void *);

#endif /* INCLUDE_FEI_END */

/* End Driver Table */
END_TBL_ENTRY endDevTbl [] =
{
/* Entries that can be filled in dynamically when cards are found */

#ifdef INCLUDE_FEI_END
    /* Note that it is not strictly necessary to initialize the END table
     * this way for the fei driver. The driver front-end, sysfei82557end.c,
     * finds all instances of 8255x devices and will properly modify this
     * table according to the number of devices found. The default BSP has
     * just this one initialized entry but also leaves room for a number
     * of additional 55x devices, should they exist. If no instances of
     * the device are found, this one entry will be marked as processed
     * to prevent the MUX from trying to load a driver that has no device.
     * If more than one device is found, unused entries will be modified
     * with an appropriate unit number, etc., so the MUX can load them.
     * Please note that the driver front-end expects to find one and only
     * one pre-initialized entry for the fei driver. Additional entries,
     * if needed due to the discovery of additional devices, will be taken
     * from the empty entries --
     * See sysfei82557End.c for more details.
    */
    { 0, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
#ifdef PQII_EXTND_PCI_MASTER
#if ( SYS_FEI_UNITS > 1)
    { 1, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
#endif
#endif  /* PQII_EXTND_PCI_MASTER */
#endif  /* INCLUDE_FEI_END */

#ifdef INCLUDE_MOTFCCEND
# ifndef MOT_FCC_NUNITS
# define MOT_FCC_NUNITS 1
# endif  /* MOT_FCC_NUNITS */
    { 0, MOT_FCC_LOAD_FUNC, MOT_FCC_LOAD_STRING, 1, NULL, FALSE},
# if (MOT_FCC_NUNITS > 1)
    { 1, MOT_FCC_LOAD_FUNC, MOT_FCC_LOAD_STRING, 1, NULL, FALSE},
#  if (MOT_FCC_NUNITS > 2)
    { 2, MOT_FCC_LOAD_FUNC, MOT_FCC_LOAD_STRING, 1, NULL, FALSE},
#  endif  /* MOT_FCC_NUNITS > 2 */
# endif  /* MOT_FCC_NUNITS > 1 */
#endif /* INCLUDE_MOTFCCEND */

#ifdef INCLUDE_MOTSCCEND
    { 0, SCC_LOAD_FUNC    , SCC_LOAD_STRING    , 1, NULL, FALSE},
#endif /* INCLUDE_MOTSCCEND */

#ifdef INCLUDE_AR521X_END

/* Atheros AR521X WLAN Support */

    { -1, END_TBL_END, NULL, 0, NULL, FALSE}, /* up to 4 Atheros NICs */
    { -1, END_TBL_END, NULL, 0, NULL, FALSE},
    { -1, END_TBL_END, NULL, 0, NULL, FALSE},
    { -1, END_TBL_END, NULL, 0, NULL, FALSE},
#endif /* INCLUDE_AR521X_END */

    { 0, END_TBL_END      , NULL               , 0, NULL, FALSE},
};

/* max number of SENS ipAttachments we can have */
#ifndef IP_MAX_UNITS
#define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif

#ifdef __cplusplus
    }
#endif

#endif /* __INCconfigNeth */
