/* sysALib.s - simpc/simlinux system-dependent assembly routines */

/*
 * Copyright (c) 2003,2008,2010,2011,2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01g,18jan13,elp  Fixed SIMNT 64-bit parameter passing (CQ:WIND00389581).
01f,04jan11,dbt  Removed useless reference to sysMemTop() (CQ:WIND00248957).
01e,02jun10,elp  64 bits support.
01d,02sep08,elp  allocate initial stack to prevent conflict with root task.
01c,26may08,elp  switch on vxWorks memory (CQ:WIND121905).
01b,23apr03,dbt  Updated comment to indicate this file is also used for Linux
                 VxSim.
01a,31mar03,jmp  written.
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system.  The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE
#include "vsbConfig.h"
#include "vxWorks.h"
#include "asm.h"

#define VXWORKS_INIT_STACK_SZ 	0x2000

	.globl	GTEXT(vxsimMain)	/* entry point into vxsimArchLib.c */
	.globl	GTEXT(_sysInit)		/* start of system code */

/*
 * VxWorks initial stack used until tRootTask is started.
 * Make sure the buffer used for the initial stack is not part of VxWorks Image
 * BSS section because this section is cleared at startup by VxWorks while
 * still running on the initial stack.
 */
	.data
	.balign  16,0x90

sysInitStack:
	.fill  VXWORKS_INIT_STACK_SZ,1,0xee

	.text
	.balign  16,0x90

FUNC_LABEL(_sysInit)
#ifdef _WRS_CONFIG_LP64
#if (CPU == SIMNT)			
	/* code is called from windows switch ABI to pass bootStrategy param */

	mov	%rcx,%rdi	
#endif /* CPU == SIMNT */
	xor	%rbp,%rbp			/* set frame pointer to 0 */
	movabsq	$VAR(sysInitStack), %rax	/* switch on init stack */
	add	$VXWORKS_INIT_STACK_SZ,%rax
	mov	%rax,%rsp
#else /* _WRS_CONFIG_LP64 */
	mov	SP_ARG1(%esp),%ecx	/* get sysInit() argument in ecx */
	xorl	%ebp,%ebp		/* make sure frame pointer is 0 */ 
	movl	$VAR(sysInitStack),%eax	/* switch on init stack */
	addl	$VXWORKS_INIT_STACK_SZ,%eax	
	movl	%eax,%esp
	push	%ecx			/* put argument on new stack */
#endif /* _WRS_CONFIG_LP64 */
	call	FUNC(vxsimMain)		/* gone for good */
	FUNC_END(_sysInit)
