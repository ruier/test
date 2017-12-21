/* usrSymTbl.c - symbol table initialization file */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,08aug13,elp created (CQ:WIND00429784).
*/

/*
DESCRIPTION
This file is used to initialize the symbol table.

NOMANUAL
*/

#ifndef INCLUDE_SEM_READ_WRITE

/*
 * Since 6.9.3.2 symbol table uses read-write semaphore, make sure this
 * semaphore type is initialized. It is initialized through dependency when
 * creating a new project but when migrating from a previous version we need
 * to do it here.
 */

#define SEM_RW_MAX_CONCURRENT_READERS 32

/*
 * Initialize the system maximum concurrent readers for reader/writer
 * semaphores.
 */

UINT semRWLibMaxReaders = SEM_RW_MAX_CONCURRENT_READERS;

extern SEM_TAKE_FUNC _func_semRTake;
extern SEM_TAKE_FUNC _func_semWTake;
extern WIND_SEM_DELETE_FUNC _func_windSemRWDelete;
extern void windSemRWDelete (SEM_ID semid);

#endif /* !INCLUDE_SEM_READ_WRITE */

/******************************************************************************
*
* usrSymLibInit - initialize symbol table component
*
* This routine is only useful when migrating a project from 6.9.3.1 to
* 6.9.3.2. In this case it makes sure required read-write semaphores are
* correctly initialized. Starting with 6.9.3.2 a dependency between
* INCLUDE_SYM_TBL and INCLUDE_SEM_READ_WRITE makes sure initialization is
* complete.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrSymLibInit ()
    {
#ifndef INCLUDE_SEM_READ_WRITE
    /* overwrite initialization done in usrSemLib.c */

    _func_windSemRWDelete = windSemRWDelete;
    _func_semRTake = semRTake;
    _func_semWTake = semWTake;
    semCreateTbl[SEM_TYPE_RW] = semRWCreate;
    semGiveTbl[SEM_TYPE_RW] = semRWGive;
    semTakeTbl[SEM_TYPE_RW] = semWTake;
#endif /* !INCLUDE_SEM_READ_WRITE */
    }
