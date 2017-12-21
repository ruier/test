/* 01obj.cdf - object (semaphore, message queue, task, timer) components */

/*
 * Copyright (c) 2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,06apr07,zl   dependency on INCLUDE_OBJ_INFO
01a,26oct04,fr   created
*/

/*
DESCRIPTION
This file contains descriptions for INCLUDE_OBJ_OPEN components.
*/

Component INCLUDE_OBJ_OPEN {
	NAME		extended object library
	MODULES		objOpen.o
	SYNOPSIS	open/close/unlink modules for message queues, \
			semaphores, tasks, and POSIX timers
	HDR_FILES	objLib.h
	_CHILDREN	FOLDER_KERNEL
	REQUIRES	INCLUDE_OBJ_INFO
}

Component INCLUDE_MSG_Q_OPEN {
	NAME		extended message queue library
	SYNOPSIS	open/close/unlink message queue modules
	MODULES		msgQOpen.o
	INIT_RTN	msgQOpenInit ();
	HDR_FILES	msgQLib.h
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_MSG_Q
	INCLUDE_WHEN	INCLUDE_MSG_Q \
			INCLUDE_OBJ_OPEN
}

Component INCLUDE_SEM_OPEN {
	NAME		extended semaphore library
	SYNOPSIS	open/close/unlink semaphore modules
	MODULES		semOpen.o
	INIT_RTN	semOpenInit ();
	HDR_FILES	semLib.h
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_SEM_BINARY
	INCLUDE_WHEN	INCLUDE_SEM_BINARY \
			INCLUDE_OBJ_OPEN
}

Component INCLUDE_TASK_OPEN {
	NAME		extended task management library
	SYNOPSIS	open/close/unlink task modules
	INIT_RTN	taskOpenInit ();
	MODULES		taskOpen.o
	HDR_FILES	taskLib.h
	_CHILDREN	FOLDER_NOT_VISIBLE
	INCLUDE_WHEN	INCLUDE_OBJ_OPEN
}

Component INCLUDE_TIMER_OPEN {
	NAME		extended timer library
	SYNOPSIS	open/close/unlink timer modules
	MODULES		timerOpen.o
	INIT_RTN	timerOpenInit ();
	HDR_FILES	private/timerLibP.h
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_POSIX_TIMERS
	INCLUDE_WHEN	INCLUDE_POSIX_TIMERS \
			INCLUDE_OBJ_OPEN
}
