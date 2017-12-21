/* usrWdbGopher.c - configuration file for WDB gopher interpreter */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,22sep04,elg  Written.
*/

/*
DESCRIPTION

This library configures and initializes the WDB gopher interpreter.
It allows to configure tapes size and number of allocatable tapes.
*/

/******************************************************************************
*
* usrWdbGopherInit - configure and initialize the WDB gopher library
*
* This routine reserves a chunk of memory for the static gopher tape, then
* calls the gopher library initialization routine.
*
* RETURNS: N/A
*/

void usrWdbGopherInit (void)
    {
    static char	wdbGopherTape [WDB_GOPHER_TAPE_LEN];

    wdbGopherLibInit (wdbGopherTape, WDB_GOPHER_TAPE_LEN, WDB_GOPHER_TAPE_NB);
    }
