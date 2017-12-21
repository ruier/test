/* 405cache.s - IBM 405 cache routines */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01c,03sep04,mdo  Documentation fixes for apigen
01b,30nov01,kab  Update for T2.2
01a,17may01,mcg  created
*/

/*
DESCRIPTION
This module contains cache routines written in assembly language.
*/

#define _ASMLANGUAGE

        /* globals */

        FUNC_EXPORT(cacheFlush405)
        FUNC_EXPORT(cacheInvalidate405)

        _WRS_TEXT_SEG_START

/*******************************************************************************
*
* cacheFlush405 - cache flush routine for PowerPC 405
*
* This routine flushes from the data cache, the specified number of bytes
* from the specified address.
*
* NOMANUAL

* void cacheFlush405(CACHE_TYPE cache, void * adrs, size_t bytes)

*/

FUNC_BEGIN(cacheFlush405)

      /*
       *   r3 = Data cache
       *   r4 = address
       *   r5 = number of bytes
       */

        cmpwi   r5,0                /* make sure number of bytes is > 0 */
        beq     flushDone
        add     r6,r4,r5
        addi    r6,r6,31
        rlwinm  r6,r6,0,0,26        /* end addr to start of next cache line */
        rlwinm  r7,r4,0,0,26        /* start address back to start of line  */
        sub     r6,r6,r7
        srawi   r6,r6,5             /* divide by 32 to get number of lines  */
        mtctr   r6
flushLoop:
        dcbst   r0,r4
        addi    r4,r4,32
        bdnz    flushLoop
        sync
flushDone:
        blr
FUNC_END(cacheFlush405)

/*******************************************************************************
*
* cacheInvalidate405 - cache invalidate routine for PowerPC 405
*
* This routine invalidates from the data cache, the specified number of bytes
* from the specified address.
*
* NOMANUAL

* void cacheInvalidate405

*/

FUNC_BEGIN(cacheInvalidate405)

      /*
       *   r3 = Data cache
       *   r4 = address
       *   r5 = number of bytes
       */

        cmpwi   r5,0                /* make sure number of bytes is > 0 */
        beq     invalDone
        add     r6,r4,r5
        addi    r6,r6,31
        rlwinm  r6,r6,0,0,26        /* end addr to start of next cache line */
        rlwinm  r7,r4,0,0,26        /* start address back to start of line  */
        sub     r6,r6,r7
        srawi   r6,r6,5             /* divide by 32 to get number of lines  */
        mtctr   r6
invalLoop:
        dcbi    r0,r4
        addi    r4,r4,32
        bdnz    invalLoop
        sync
invalDone:
        blr
FUNC_END(cacheInvalidate405)

