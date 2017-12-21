/*
 * memtester version 4
 *
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2007 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 */

#define __version__ "4.0.8"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include "tests.h"



#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04

TEST_TEST_S gstTests[] = 
{
    { "Random Value", test_random_value },
    { "Compare XOR", test_xor_comparison },
    { "Compare SUB", test_sub_comparison },
    { "Compare MUL", test_mul_comparison },
    { "Compare DIV",test_div_comparison },
    { "Compare OR", test_or_comparison },
    { "Compare AND", test_and_comparison },
    { "Sequential Increment", test_seqinc_comparison },
    { "Solid Bits", test_solidbits_comparison },
    { "Block Sequential", test_blockseq_comparison },
    { "Checkerboard", test_checkerboard_comparison },
    { "Bit Spread", test_bitspread_comparison },
    { "Bit Flip", test_bitflip_comparison },
    { "Walking Ones", test_walkbits1_comparison },
    { "Walking Zeroes", test_walkbits0_comparison },
    { NULL, NULL }
};

int mem_test(ul start,ul len)
{
    ulv *bufa = NULL;
    ulv *bufb = NULL;
    ul   count = 0;
    ul   loop = 0;

    memPrint("memory test version " __version__ " (%d-bit)\n", UL_LEN);
    memPrint("\n");

    count = len / (2 * sizeof(ul));
    bufa = (ulv *) (start & (~3));
    bufb = (ulv *) ((start + (len / 2)) & (~3));

    memPrint("test bufa 0x%x bufb 0x%x count 0x%x \n",bufa,bufb,count);
    
    if (0 != test_stuck_address(start,count)) 
    {
        return -1;
    }
    for (loop = 0;loop < sizeof(gstTests)/sizeof(TEST_TEST_S);loop++) 
    {
        if (NULL == gstTests[loop].name) 
        {
            break;
        }
        
        memPrint("  %-20s: ", gstTests[loop].name);
        
        if (!gstTests[loop].fp(bufa, bufb, count)) 
        {
            memPrint("ok\n");
        } 
        else 
        {
            memPrint("failed\n");
            
            return -1;
        }
    }
    memPrint("\n");

    return 0;
}

