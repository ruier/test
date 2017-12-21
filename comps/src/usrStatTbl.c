/* usrStatTbl.c - error status symbol table initialization library */

/* Copyright 1984-1998 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,27jul05,lei  added error checking (SPR#105895)
01a,03jun98,ms   written
*/

/*
DESCRIPTION
Initialize the error status table used by perror and printErrno.
*/

/******************************************************************************
*
* usrStatTblInit - initialize the error status table
*/ 

void usrStatTblInit (void)
    {
    int ix;

    statSymTbl = symTblCreate (STAT_TBL_HASH_SIZE_LOG2, FALSE, memSysPartId);

    if (statSymTbl == NULL)
        {
        if (_func_printErr != NULL)
            _func_printErr ("usrRoot: error creating the error status table\n");
        }
    else
        {
        /* fill in from builtin table*/

        for (ix = 0; ix < statTblSize; ix ++)       
            symTblAdd (statSymTbl, &(statTbl [ix]));
        }
    }
