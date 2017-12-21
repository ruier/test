/* version.c - creation version/date/time module */

/*
 * Copyright (c) 1996, 2001, 2004, 2006-2009, 2012-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01i,30apr13,elp  change default creationDate value to RUNTIME_CREATION_TIME
                 parameter value (WIND00295940).
01h,12mar12,pad  Introduction of full version number, vs. displayed version
		 number (defect 273861).
01g,18sep09,pad  Moved version setting logic in version.h. Use the new macro
                 RUNTIME_VERSION_EXTENDED instead of RUNTIME_VERSION to set
		 the variable 'runtimeVersion'.
01f,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01e,05mar07,kk   change the format of runtimeName and runtimeVersion for SMP
01d,15nov06,cjj Use different runtime name and VxWorks version for SMP
01c,11may04,md  add global variables for VxWorks version numbers
01b,17jan01,sn  simplify job of host val  
01a,06mar96,dat	written
*/

/*
This module is always built with each executable image.  It provides
the VxWorks version id, and the time and date it was built.

The date stamp may be overriden by defining RUNTIME_CREATION_DATE. This
will be primarily used by tools that compare images built on different hosts 
(host validation).

The ANSI predefined macros __DATE__ and __TIME__ are used to provide
the date/time information.  ANSI compliant compilers are required for
building all VxWorks executables.
*/

#include <vxWorks.h>
#include <config.h>
#include <version.h>

/* numerical values for VxWorks version */

const unsigned int vxWorksVersionMajor = _WRS_VXWORKS_MAJOR;
const unsigned int vxWorksVersionMinor = _WRS_VXWORKS_MINOR;
const unsigned int vxWorksVersionMaint = _WRS_VXWORKS_MAINT;
const unsigned int vxWorksVersionSvcPk = _WRS_VXWORKS_SVCPK;

/* string identifiers for VxWorks version */

char * runtimeName    = RUNTIME_NAME;
char * vxWorksVersion = VXWORKS_VERSION;
char * runtimeVersion = RUNTIME_VERSION_EXTENDED;
char * runtimeVersionFull = RUNTIME_VERSION_FULL_EXTENDED;

#ifdef RUNTIME_CREATION_DATE
char * creationDate   = RUNTIME_CREATION_DATE;
#elif defined(RUNTIME_CREATION_TIME)
char * creationDate   = RUNTIME_CREATION_TIME;
#else
char * creationDate   = __DATE__" "__TIME__;
#endif
