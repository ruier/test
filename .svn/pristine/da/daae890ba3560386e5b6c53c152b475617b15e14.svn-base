/* usrStandalone.c - standalone symbol table initialization library */

/*
 * Copyright (c) 1998,2004-2006,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software may be
 * licensed only pursuant to the terms of an applicable Wind River license
 * agreement. No license to Wind River intellectual property rights is granted
 * herein. All rights not licensed by Wind River are reserved by Wind River.
 */

/*
modification history
--------------------
01j,27apr12,jmp  CQID: Fix WIND00340092 : Fixed some build warnings.
01i,06aug10,v_r  Added standalone symbol table support for Thumb2 (def
                 CQ:WIND00224982).
01h,10jan06,bpn  Moved system symbol table initialisation to usrSysSymTbl.c
01g,12apr05,v_r  Fixed SPR #107398 (Inclusion of kernel shell components gives
                 a build warning).
01f,31jan05,v_r  Fixed typo from last checkin that caused build failure
01e,26jan05,v_r  Fixed SPR #30900 (return value of SymTblCreate() not
                 checked). Updated copyright notice for 2005.
01d,30sep04,jn   Modify use of config param to avoid build warning in bsp build
01c,24sep04,jn   Add configurability for including absolute symbols in system
                 symbol table or not
01b,23nov98,cdp  added support for ARM_THUMB.
01a,03jun98,ms   written
*/

/*
DESCRIPTION
Initialize the built-in symbol table.
*/

#include <vxWorks.h>
#include <symLib.h>
#include <symbol.h>
#include <errnoLib.h>
#include <private/loadElfLibP.h>
#include <private/symLibP.h>

#define SYM_ADD_ERROR_COUNT	10

/* Forward declarations */

LOCAL STATUS standaloneSymTblAdd (SYMBOL_ID symId);

/******************************************************************************
*
* usrStandaloneInit - initialize the built-in symbol table
*/ 

void usrStandaloneInit (void)
    {
    ULONG	    ix;
    UINT32	    errCnt = 0;
    char *	    symName;
    static char	    symNameErr [] = "symNameGet() error!";

    if (sysSymTbl == NULL)
        return;

    printf ("\nAdding %ld symbols for standalone.\n", standTblSize);

    /* Fill system symbol table from the builtin one */

    for (ix = 0; ix < standTblSize; ix++)
        {
#if (STORE_ABS_SYMBOLS == FALSE)
	/* Discard absolute symbols for the system symbol table */

	if (SYM_IS_ABS (standTbl[ix].type)) 
	    continue;
#endif
	if (standaloneSymTblAdd (&standTbl[ix]) != OK)
	    {
	    if (symNameGet (&standTbl[ix], &symName) != OK)
		symName = symNameErr;

	    if (errCnt < SYM_ADD_ERROR_COUNT)
		{ 
		printErr ("Error adding '%s' to the standalone symbol table "
			  "(idx = %ld, %s)!\n", symName, ix, 
			  strerror (errnoGet ()));
		}
	    else if (errCnt == SYM_ADD_ERROR_COUNT)
		{
		/* 
		 * Prevent screen overflow if too many errors. Keep on trying
		 * to add the symbols though, as the errors may not span the
		 * entire symbol table.
		 */

		printf ("...\n");
		}
	    errCnt++;
	    }
	}

    /* 
     * It is normal for the number of reported registration errors to not match 
     * standTblSize if STORE_ABS_SYMBOLS == FALSE.
     */

    if (errCnt != 0)
	printf ("%ld symbols could not be registered.\n", errCnt);
    }

/******************************************************************************
*
* standaloneSymTblAdd - add a standalone symbol to the system symbol table
*
* This routine adds standalone symbols to the system symbol table at startup
* time. 
*
* RETURN: OK if symbol could be registered, ERROR otherwise
*
* \NOMANUAL
*/

LOCAL STATUS standaloneSymTblAdd
    (
    SYMBOL_ID symId /* pointer to symbol to add */
    )
    {
#if ((CPU_FAMILY == ARM) && (ARM_THUMB == TRUE || ARM_THUMB2 == TRUE))
    return elfArmStandaloneThumbSymTblAdd (symId);
#else
    return symTblAdd (sysSymTbl, symId);
#endif
    }
