/* sysMemoryTest.c - Memory Test Suite */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,05jul05,cak  New.
*/

/*
DESCRIPTION

This file contains a memory test suite.  This suite of tests is designed
to detect the following types of errors:

1. Electrical Wiring Problems

   This refers to problems with the wires that connect the memory devices
   to the processor (address lines, data lines, or control lines).  Any
   of these lines could be improperly routed, shorted or open.  In which
   case data could be stored incorrectly, stored at the wrong address, or
   not stored at all.  If the problem is with a data line, several data 
   bits may appear to be "stuck together", or a data bit may be either 
   "stuck high" or be "stuck low".  If an address line has a wiring 
   problem, the contents of two memory locations may appear to overlap.

2. Missing Memory Chips
3. Improperly Inserted Chips

The boards user LEDs are used to indicate the status of the tests.  Before
any tests are performed all three user LEDs are illuminated.

This suite contains the following tests, to be executed in this order:

Data Bus Test

 This is the "walking 1's" test, where a single data bit is set to 1 and 
 "walked" through.  The number of data values to test is the same as 
 the width of the data bus. 

 When this test passes successfully the user LED 1 is extinguished.

Address Bus Test

 Here you just need to confirm that each of the address pins can be set 
 to 0 and 1 without affecting any of the others.  The smallest set of 
 addresses that will cover all possible combinations is the set of 
 "power-of-two" addresses.

 When this test passes usccessfully the user LED 2 is extinguished.

Device Test
 
 Here you need to test that every bit in the device is capable of 
 holding both 0 and 1.

 When this test passes successfully the user LED 3 is extinguished.

So, if the board will not boot and any of these three LEDs is illuminated
it is an indication that there is a problem with the memory.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysLib.h"

/* defines */

#define dataTestAddr1	0x00001000
#define dataTestAddr2	0x00001004

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

void sysMemoryTest (UINT32 memorySize);
UINT32 sysMemTestDataBus (volatile UINT32 * address);
UINT32 * sysMemTestAddressBus (volatile UINT32 * baseAddress, UINT32 nBytes);
UINT32 * sysMemTestDevice (volatile UINT32 * baseAddress, UINT32 nBytes);	

/* externals */

/******************************************************************************
*
* sysMemoryTest - memory test suite
*
* This is the top level routine for this memory test suite.  It invokes
* each of the tests, and is meant to be called externally - from romInit.s.
*
* RETURNS: N/A
*/

void sysMemoryTest 
    (
    UINT32 memorySize	/* amount of memory on the board */
    )
    {

    /* Illuminate each of the three user LEDs on the board */

    *(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) |=
        (BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_MASK |
	 BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_MASK |
	 BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_MASK);

    /*
     * The memory data bus test is called twice, with two consecutive
     * memory addresses, in order to cover all 64 data bus lines.
     * This was indicated to be correct by the board's hardware
     * designer.
     */

    if (sysMemTestDataBus ((UINT32 *)dataTestAddr1) == 0x0 && 
	sysMemTestDataBus ((UINT32 *)dataTestAddr2) == 0x0)
	{

	/* extinguish user LED 1 if both data bus tests pass */

	*(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) &=
	    ~BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_MASK;	
	}

    if (sysMemTestAddressBus ((UINT32 *)0x00000000, memorySize) == 0x0)
	{

	/* extinguish LED 2 if the address bus test passes */

	*(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) &=
            ~BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_MASK; 
	}

    if (sysMemTestDevice ((UINT32 *)0x00000000, memorySize) == 0x0)
	{

	/* extinguish LED 3 if the device test passes */

        *(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) &=
            ~BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_MASK; 
	}
    }

/******************************************************************************
*
* sysMemTestDataBus - data bus wiring test
*
* Test the data bus wiring in a memory region by performing a walking 1's 
* test at a fixed address within that region.  The address (and hence the
* memory region) is selected by the caller.
*
* RETURNS: 0 if the test succeeds, a non-zero result is the first pattern 
* that failed.
*/

UINT32 sysMemTestDataBus
    (
    volatile UINT32 * address
    )
    {
    UINT32 pattern;

    /* Perform a walking 1's test at the given address */

    for (pattern = 1; pattern != 0; pattern <<= 1)
	{

        /* Write the test pattern */

        *address = pattern;

        /* Read it back */

        if (*address != pattern) 
	    {
	    return (pattern);
	    }
	}

    return (0);
    }

/******************************************************************************
*
* sysMemTestAddressBus - memory address bus test
*
* Test the address bus wiring in a memory region by performing a walking 1's 
* test on the relevant bits of the address and checking for aliasing. This 
* test will find single-bit address failures such as stuck-high, stuck-low, 
* and shorted pins.  The base address and size of the region are selected 
* by the caller.  For best results, the selected base address should have 
* enough LSB 0's to guarantee single address bit changes.  For example, to 
* test a 64-Kbyte region, select a base address on a 64-Kbyte boundary.  
* Also, select the region size as a power-of-two--if at all possible.
*
* RETURNS: NULL if the test succeeds, a non-zero result is the first address 
* at which an aliasing problem was uncovered.  By examining the contents of 
* memory, it may be possible to gather additional information about the 
* problem.
*
*/

UINT32 * sysMemTestAddressBus
    (
    volatile UINT32 * baseAddress, 
    UINT32 nBytes
    )
    {
    UINT32 addressMask = (nBytes/sizeof(UINT32) - 1);
    UINT32 offset;
    UINT32 testOffset;
    UINT32 pattern     = (UINT32) 0xAAAAAAAA;
    UINT32 antipattern = (UINT32) 0x55555555;

    /* Write the default pattern at each of the power-of-two offsets */

    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
	{
        baseAddress[offset] = pattern;
	}

    /* Check for address bits stuck high */

    testOffset = 0;
    baseAddress[testOffset] = antipattern;

    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
	{
	if (baseAddress[offset] != pattern)
	    {
	    return ((UINT32 *) &baseAddress[offset]);
	    }
	}

    baseAddress[testOffset] = pattern;

    /* Check for address bits stuck low or shorted */

    for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1)
	{
        baseAddress[testOffset] = antipattern;

	if (baseAddress[0] != pattern)
	    {
	    return ((UINT32 *) &baseAddress[testOffset]);
	    }

        for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
	    {
            if ((baseAddress[offset] != pattern) && (offset != testOffset))
		{
		return ((UINT32 *) &baseAddress[testOffset]);
		}
	    }

        baseAddress[testOffset] = pattern;
	}

    return (NULL);
    }

/******************************************************************************
*
* sysMemTestDevice - memory device test
*
* Test the integrity of a physical memory device by performing an 
* increment/decrement test over the entire region.  In the process every 
* storage bit in the device is tested as a zero and a one.  The base 
* address and the size of the region are selected by the caller.
*
* RETURNS: NULL if the test succeeds, a non-zero result is the first 
* address at which an incorrect value was read back.  By examining the
* contents of memory, it may be possible to gather additional information 
* about the problem.
*/

UINT32 * sysMemTestDevice
    (
    volatile UINT32 * baseAddress, 
    UINT32 nBytes
    )	
    {
    UINT32 offset;
    UINT32 nWords = nBytes / sizeof(UINT32);
    UINT32 pattern;
    UINT32 antipattern;

    /* Fill memory with a known pattern */

    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
	{
	baseAddress[offset] = pattern;
	}

    /* Check each location and invert it for the second pass */

    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
	{
	if (baseAddress[offset] != pattern)
	    {
	    return ((UINT32 *) &baseAddress[offset]);
	    }
        antipattern = ~pattern;
        baseAddress[offset] = antipattern;
	}

    /* Check each location for the inverted pattern and zero it */

    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
	{
	antipattern = ~pattern;
	if (baseAddress[offset] != antipattern)
	    {
	    return ((UINT32 *) &baseAddress[offset]);
	    }
	}

    /* zero out memory */

    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
	{
	baseAddress[offset] = 0x0;
	}

    return (NULL);
    }
