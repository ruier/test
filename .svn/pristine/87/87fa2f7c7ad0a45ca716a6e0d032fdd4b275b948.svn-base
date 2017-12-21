/* configNet.h - network configuration header */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,26jun07,l_g  initial creation, based on ads834x.
*/
 
#ifndef INCconfigNeth
#define INCconfigNeth

#include <vxWorks.h>
#include "config.h"
#include <end.h>
#include <miiLib.h>

/* defines */
 
#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2

#ifdef INCLUDE_FEI_END
#define FEI_LOAD_FUNC   sysFei82557EndLoad
IMPORT END_OBJ* FEI_LOAD_FUNC (char *, void*);
#define FEI_LOAD_STRING ""
#endif /* INCLUDE_FEI_END */


#ifdef INCLUDE_MOT_TSEC_END
#define END_LOAD_FUNC_PRI       sysMotTsecEndLoad
#define END_BUFF_LOAN_PRI       SYS_END_BUFF_LOAN_PRI   
#define END_LOAD_FUNC_SEC       sysMotTsecEndLoad
#define END_BUFF_LOAN_SEC       SYS_END_BUFF_LOAN_SEC  
#endif


#if defined(INCLUDE_PRIMARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_PRI (char *, void*);
#endif
#if defined(INCLUDE_SECONDARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_SEC (char *, void*);
#endif


#define END_LOAD_STRING ""      /* created in sys<device>End.c */


/* max number of SENS ipAttachments we can have */
 
#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif
 
END_TBL_ENTRY endDevTbl [] =
{

#if defined(INCLUDE_PRIMARY_TSEC_END)
    {0, END_LOAD_FUNC_PRI, END_LOAD_STRING, END_BUFF_LOAN_PRI, NULL},
#endif
#if defined(INCLUDE_SECONDARY_TSEC_END)
    {1, END_LOAD_FUNC_SEC, END_LOAD_STRING, END_BUFF_LOAN_SEC, NULL},
#endif

#ifdef INCLUDE_FEI_END
    { 0, FEI_LOAD_FUNC, FEI_LOAD_STRING, 3, NULL, FALSE},
#endif /*INCLUDE_FEI_END */

    { 0, END_TBL_END, NULL, 0, NULL, FALSE}
};
#endif /* INCconfigNeth */
