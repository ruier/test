/* usrPosixAdvLock.c - POSIX advisory file locking */

/*
 * Copyright 2006 - Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,11jul06,pcm  removed redundant code
01b,09may06,pcm  made _func_lf_advlock extern; see funcBind.c
01a,12Jan06,act  written.
*/

/*
DESCRIPTION
*/

/* includes */

#include "vnode.h"
#include "private/lockfP.h"

/* defines */

/* typedefs */

/* globals */

/* pointer to vnode operator tables used by HRFS */
extern int (*_func_lf_advlock)(struct vnode *, void *, int, struct flock *,
	                       voff_t, int);

/* externs */

/***************************************************************************
*
* usrPosixAdvLockInit -
*/

STATUS usrPosixAdvLockInit
    (
    void
    )
    {
    _func_lf_advlock = lf_advlock;

    return (OK);
    }
