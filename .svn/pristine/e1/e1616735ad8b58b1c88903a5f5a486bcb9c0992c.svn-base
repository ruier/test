/* usrMmuOptimize.c - MMU configure optimization */

/*
 * Copyright 2004, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,02jul10,zl   64-bit adaptation.
01e,19aug09,my_  remove error message when vmPageLock() fails (WIND00172830)
01d,08jul09,pch  add timestamps before & after vmPageOptimize()
01c,15jun09,pch  Allow for VM_PAGE_SIZE != 4KB
01b,09dec04,pch  SPR 105014
01a,30jun04,dtr  Created
*/

/* DESCRIPTION :
 * This routine can lock text section TLB and optimize the TLB page sizes.
 * Not all CPUs architectures support this functionality so a user should be 
 * aware whether the calls have any effect. Note the number of TLBs made 
 * available by the CPUs may mean the text section can't be locked unless 
 * better alignment of the text section is implemented. 
 */
extern char wrs_kernel_text_start[];
extern char wrs_kernel_data_start[];

#ifdef	INCLUDE_TM_TIMING_STARTUP_REPORT
extern TM_TIMESTAMP_TYPE tmTmOptimizeStartTime;
extern TM_TIMESTAMP_TYPE tmTmOptimizeEndTime;
#endif	/* INCLUDE_TM_TIMING_STARTUP_REPORT */

void usrMmuOptimize()
    {
#ifdef INCLUDE_LOCK_TEXT_SECTION
    if (vmPageLock (vmCurrentGet(),
		    (VIRT_ADDR)wrs_kernel_text_start,
		    ROUND_DOWN(((ULONG)wrs_kernel_data_start - 
			        (ULONG)wrs_kernel_text_start),
			       VM_PAGE_SIZE),
		    0) == ERROR)
	{
#ifdef DEBUG_OPTIM
	printf("\nerrno = 0x%x\n",errno);
	printf("\naddr = 0x%lx :: size = 0x%lx\n",
	       (VIRT_ADDR)wrs_kernel_text_start,
	       ROUND_DOWN(((ULONG)wrs_kernel_data_start - 
			   (ULONG)wrs_kernel_text_start),
			  VM_PAGE_SIZE));
	printf("\nPage Lock failed. Check cpu resources/architecture support "
	       "and \nalignment of text and data section\n");
#endif
	}

#endif /* INCLUDE_LOCK_TEXT_SECTION */

#ifndef _WRS_CONFIG_LP64
    /* 
     * On 64-bit VxWorks page optimization is enabled or disabled 
     * directly at the time the mappings are performed, rather than
     * scanning the entire virtual space, which would be very 
     * inefficient.
     */

#ifdef INCLUDE_PAGE_SIZE_OPTIMIZATION
# ifdef	INCLUDE_TM_TIMING_STARTUP_REPORT
    TM_TIMESTAMP_GET(tmTmOptimizeStartTime);
# endif  /* INCLUDE_TM_TIMING_STARTUP_REPORT */

    vmPageOptimize(0, 0, (UINT32)(-VM_PAGE_SIZE), 0);

# ifdef	INCLUDE_TM_TIMING_STARTUP_REPORT
    TM_TIMESTAMP_GET(tmTmOptimizeEndTime);
# endif  /* INCLUDE_TM_TIMING_STARTUP_REPORT */
#endif /* INCLUDE_PAGE_SIZE_OPTIMIZATION */

#endif /* _WRS_CONFIG_LP64 */
    }
