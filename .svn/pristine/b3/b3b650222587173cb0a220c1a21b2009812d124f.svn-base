/* usrSemLib.c - wind kernel semaphore initialization file */

/*
 * Copyright (c) 2006-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,18aug10,s_s  removing shared memory references from semXXXTbl
01o,05feb10,pch  reduced semaphore table size when using small footprint
01n,10nov09,pad  Fixed a manual merge mistake.
01m,05aug09,jpb  Defect# 168954: Added elements to sem tables.
01l,11mar09,pcs  Updated to add support for the LP64 data model.
01k,07jul08,gls  deprecated minimal semaphores
01j,07jan08,gls  fast semaphores renamed to minimal semaphores
01i,12dec07,gls  added fast semaphores
01h,03oct07,jpb  Defect# 106240: Removal of INCLUDE_SEM_OLD.
01g,14sep07,gls  reserved SEM_TYPE_SM_BINARY and SEM_TYPE_SM_COUNTING fields
		 in semaphore tables (Defect #00104600)
01f,28jun07,gls  added semGiveNoLockTbl[] and semTakeNoLockTbl[]
01e,28jun07,gls  added _func_semRTake and _func_semWTake
01d,22mar07,gls  move _func_windSemRWDelete and semRWLibMaxReaders here
01c,16mar07,gls  set semRWLibMaxReaders when INCLUDE_SEM_READ_WRITE defined
01b,05feb07,gls  added reader/writer semaphores
01a,11jan06,yvp  written.
*/

/*
DESCRIPTION
This file is used to initialize the per-semaphore-type routine tables according
to the semaphore types configured into the system.

NOMANUAL
*/

/* includes */

#include <private/semLibP.h>

#ifdef INCLUDE_SEM_READ_WRITE
/*
 * Initialize the system maximum concurrent readers for reader/writer
 * semaphores.
 */

UINT semRWLibMaxReaders = SEM_RW_MAX_CONCURRENT_READERS;

/*
 * Initialize the windSemDelete() function pointer for reader/writer
 * semaphores.
 */

extern void windSemRWDelete (SEM_ID semid);


WIND_SEM_DELETE_FUNC _func_windSemRWDelete = windSemRWDelete;

SEM_TAKE_FUNC _func_semRTake = semRTake;
SEM_TAKE_FUNC _func_semWTake = semWTake;

#else
/*
 * Initialize the windSemDelete() function pointer to use the NULL return
 * routine.  This allows the function pointer to be called directly
 * without first verifying that it is not NULL.
 */

extern void * windNullReturn ();

WIND_SEM_DELETE_FUNC _func_windSemRWDelete = (WIND_SEM_DELETE_FUNC) windNullReturn;

SEM_TAKE_FUNC _func_semRTake = NULL;
SEM_TAKE_FUNC _func_semWTake = NULL;
#endif

#ifdef INCLUDE_SEM_EXCHANGE
SEM_EXCHANGE_FUNC _func_semExchange = semExchange;
#else
SEM_EXCHANGE_FUNC _func_semExchange = NULL;
#endif

/*
 * The following semaphore tables are initialized with appropriate function
 * pointers depending on the component selection. In each table, the
 * semaphore type determines the table slot into which each semaphore type
 * plugs in (see the SEM_TYPE enum). For example, binary semaphores are always
 * at the head of the table.
 */

/* Table of semaphore creation routines */

SEM_CREATE_FUNC semCreateTbl [MAX_SEM_TYPE] =	      /* semCreate() methods by type */
    {
#ifdef INCLUDE_SEM_BINARY_CREATE
    (SEM_CREATE_FUNC) semBCreate,
#else
    NULL,
#endif	/* INCLUDE_SEM_BINARY_CREATE */

#ifdef INCLUDE_SEM_MUTEX_CREATE
    (SEM_CREATE_FUNC) semMCreate,
#else
    NULL,
#endif	/* INCLUDE_SEM_MUTEX_CREATE */

#ifdef INCLUDE_SEM_COUNTING_CREATE
    (SEM_CREATE_FUNC) semCCreate,
#else
    NULL,
#endif	/* INCLUDE_SEM_COUNTING_CREATE */

#ifdef INCLUDE_SEM_READ_WRITE_CREATE
    (SEM_CREATE_FUNC) semRWCreate
#else
    NULL
#endif	/* INCLUDE_SEM_READ_WRITE_CREATE */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,

    /* the following slots are for future expansion */

    NULL,
    NULL,
    NULL,
    NULL,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of semaphore give routines */

SEM_GIVE_FUNC semGiveTbl [MAX_SEM_TYPE] =	      /* semGive() methods by type */
    {
#ifdef INCLUDE_SEM_BINARY
    semBGive,
#else
    semInvalid,
#endif	/* INCLUDE_SEM_BINARY */

#ifdef INCLUDE_SEM_MUTEX
    semMGive,
#else
    semInvalid,
#endif	/* INCLUDE_SEM_MUTEX */

#ifdef INCLUDE_SEM_COUNTING
    semCGive,
#else
    semInvalid,
#endif	/* INCLUDE_SEM_COUNTING */

#ifdef INCLUDE_SEM_READ_WRITE
    semRWGive
#else
    semInvalid
#endif	/* INCLUDE_SEM_READ_WRITE */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,

    /* the following slots are for future expansion */

    semInvalid,
    semInvalid,
    semInvalid,
    semInvalid,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of semaphore take routines */

SEM_TAKE_FUNC semTakeTbl [MAX_SEM_TYPE] =	      /* semTake() methods by type */
    {
#ifdef INCLUDE_SEM_BINARY
    semBTake,
#else
    (SEM_TAKE_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_BINARY */

#ifdef INCLUDE_SEM_MUTEX
    semMTake,
#else
    (SEM_TAKE_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_MUTEX */

#ifdef INCLUDE_SEM_COUNTING
    semCTake,
#else
    (SEM_TAKE_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_COUNTING */

#ifdef INCLUDE_SEM_READ_WRITE
    semWTake
#else
    (SEM_TAKE_FUNC) semInvalid
#endif	/* INCLUDE_SEM_READ_WRITE */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,

    /* the following slots are for future expansion */

    (SEM_TAKE_FUNC) semInvalid,
    (SEM_TAKE_FUNC) semInvalid,
    (SEM_TAKE_FUNC) semInvalid,
    (SEM_TAKE_FUNC) semInvalid,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of semaphore flush routines */

SEM_FLUSH_FUNC semFlushTbl [MAX_SEM_TYPE] =	      /* semFlush() methods by type */
    {
#ifdef INCLUDE_SEM_BINARY
    semQFlush,
#else
    semInvalid,
#endif	/* INCLUDE_SEM_BINARY */

    semInvalid,	  /* mutexes dont support a flush operation */

#ifdef INCLUDE_SEM_COUNTING
    semQFlush,
#else
    semInvalid,
#endif	/* INCLUDE_SEM_COUNTING */

    semInvalid

    /* the following slots are for future expansion */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,
    semInvalid, semInvalid,
    semInvalid, semInvalid,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of semaphore deferred-give routines */

SEM_GIVE_DEFER_FUNC semGiveDeferTbl [MAX_SEM_TYPE] =      /* semGiveDefer() methods */
    {
#ifdef INCLUDE_SEM_BINARY
    semBGiveDefer,
#else
    NULL,
#endif	/* INCLUDE_SEM_BINARY */

    NULL,

#ifdef INCLUDE_SEM_COUNTING
    semCGiveDefer,
#else
    NULL,
#endif	/* INCLUDE_SEM_COUNTING */

    NULL

    /* the following slots are for future expansion */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,
    NULL, NULL,
    NULL, NULL,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of semaphore deferred-flush routines */

SEM_FLUSH_DEFER_FUNC semFlushDeferTbl [MAX_SEM_TYPE] =     /* semFlushDefer() methods */
    {
#ifdef INCLUDE_SEM_BINARY
    semQFlushDefer,
#else
    NULL,
#endif	/* INCLUDE_SEM_BINARY */

    NULL, 	       /* mutexes dont support a flush operation */

#ifdef INCLUDE_SEM_COUNTING
    semQFlushDefer,
#else
    NULL,
#endif	/* INCLUDE_SEM_COUNTING */

    NULL

    /* the following slots are for future expansion */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,
    NULL, NULL,
    NULL, NULL,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };

/* Table of non-locking semaphore give routines */

SEM_GIVE_NOLOCK_FUNC semGiveNoLockTbl [MAX_SEM_TYPE] =
    {
#ifdef INCLUDE_SEM_BINARY
    semBGiveNoLock,
#else
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_BINARY */

#ifdef INCLUDE_SEM_MUTEX
    semMGiveNoLock,
#else
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_MUTEX */

/* only mutex and binary semaphores are supported currently */

#ifdef INCLUDE_SEM_COUNTING
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
#else
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_COUNTING */

#ifdef INCLUDE_SEM_READ_WRITE
    (SEM_GIVE_NOLOCK_FUNC) semInvalid
#else
    (SEM_GIVE_NOLOCK_FUNC) semInvalid
#endif	/* INCLUDE_SEM_READ_WRITE */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,

    /* the following slots are for future expansion */

    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
    (SEM_GIVE_NOLOCK_FUNC) semInvalid,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };


/* Table of non-locking semaphore take routines */

SEM_TAKE_NOLOCK_FUNC semTakeNoLockTbl [MAX_SEM_TYPE] = /* semTakeNoLock() methods by type */
    {
#ifdef INCLUDE_SEM_BINARY
    semBTakeNoLock,
#else
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_BINARY */

#ifdef INCLUDE_SEM_MUTEX
    semMTakeNoLock,
#else
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_MUTEX */

/* only mutex and binary semaphores are supported currently */

#ifdef INCLUDE_SEM_COUNTING
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
#else
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
#endif	/* INCLUDE_SEM_COUNTING */

#ifdef INCLUDE_SEM_READ_WRITE
    (SEM_TAKE_NOLOCK_FUNC) semInvalid
#else
    (SEM_TAKE_NOLOCK_FUNC) semInvalid
#endif	/* INCLUDE_SEM_READ_WRITE */

#ifdef	_WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES
    ,

    /* the following slots are for future expansion */

    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
    (SEM_TAKE_NOLOCK_FUNC) semInvalid,
#endif	/* _WRS_CONFIG_CUSTOM_SEMAPHORE_TYPES */
    };
