/*
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2007 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 * This file contains the functions for the actual gstTests, called from the
 * main routine in memtester.c.  See other comments in that file.
 *
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



char progress[] = "-\\|/";
#define PROGRESSLEN 4
#define PROGRESSOFTEN 2500

/* Function definitions. */

int compare_regions(ulv *bufa, ulv *bufb, size_t count) 
{
    size_t i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;

    for (i = 0; i < count; i++, p1++, p2++) 
    {
        if (*p1 != *p2) 
        {
            memPrint("FAILURE: 0x%08lx != 0x%08lx at offset 0x%08lx.\n", 
                  (ul) *p1, (ul) *p2, (ul) i);

            return -1;
        }
    }
    
    return 0;
}

int test_stuck_address(ulv bufa,ul count) 
{
    ulv *p1 = NULL;
    unsigned int j;
    ul i;

    memPrint("test stuck address start\n");
    for (j = 0; j < 16; j++) 
    {
        p1 = (ulv *) bufa;

        for (i = 0; i < count; i++) 
        {
            *p1 = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
            *p1++;
        }

        p1 = (ulv *) bufa;
        for (i = 0; i < count; i++, p1++) 
        {
            if (*p1 != (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1))) 
            {
                memPrint("FAILURE: possible bad address line at offset 0x%08lx.\n", 
                       (ul)i);
                memPrint("Skipping to next test...\n");
                return -1;
            }
        }
    }
    
    memPrint("test stuck address succeed\n");
    
    return 0;
}

int test_random_value(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i = 0;


    for (i = 0; i < count; i++) 
    {
        *p1++ = *p2++ = rand_ul();
        if (0 == (i % PROGRESSOFTEN)) 
        {
            memPrint(".");
        }
    }
    
    memPrint("\n");
    
    return compare_regions(bufa, bufb, count);
}

int test_xor_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ ^= q;
        *p2++ ^= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_sub_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ -= q;
        *p2++ -= q;
    }
    return compare_regions(bufa, bufb, count);
}

int test_mul_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ *= q;
        *p2++ *= q;
    }
    return compare_regions(bufa, bufb, count);
}

int test_div_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        if (0 == q) 
        {
            q++;
        }
        *p1++ |= q;
        *p2++ |= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_or_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ /= q;
        *p2++ /= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_and_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ &= q;
        *p2++ &= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_seqinc_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ = *p2++ = (i + q);
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_solidbits_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;

    memPrint("test_solidbits_comparison \n");
    
    for (j = 0; j < 64; j++) 
    {
        q = (j % 2) == 0 ? UL_ONEBITS : 0;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        
        memPrint(".");
        
        if (0 != compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_solidbits_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_solidbits_comparison succeed\n");
    
    return 0;
}

int test_checkerboard_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j = 0;
    ul q = 0;
    size_t i = 0;

    memPrint("test_checkerboard_comparison\n");
   
    for (j = 0; j < 64; j++) 
    {
        q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++) 
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_checkerboard_comparison failed\n");
            return -1;
        }
    }
    
    memPrint("\n");
    memPrint("test_checkerboard_comparison succeed\n");

    
    return 0;
}

int test_blockseq_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_blockseq_comparison\n");
    
    for (j = 0; j < 256; j++) 
    {
    
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            *p1++ = *p2++ = (ul) UL_BYTE(j);
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_blockseq_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_blockseq_comparison succeed\n");

    return 0;
}

int test_walkbits0_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_walkbits0_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++) 
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = 0x00000001 << j;
            } 
            else 
            { /* Walk it back down. */
                *p1++ = *p2++ = 0x00000001 << (UL_LEN * 2 - j - 1);
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_walkbits0_comparison failed\n");
            return -1;
        }
    }
    
    memPrint("\n");
    memPrint("test_walkbits0_comparison succeed\n");
    
    return 0;
}

int test_walkbits1_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;


    memPrint("test_walkbits1_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++) 
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = UL_ONEBITS ^ (0x00000001 << j);
            } else 
            { /* Walk it back down. */
                *p1++ = *p2++ = UL_ONEBITS ^ (0x00000001 << (UL_LEN * 2 - j - 1));
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_walkbits1_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_walkbits1_comparison succeed\n");

    return 0;
}

int test_bitspread_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_bitspread_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (0x00000001 << j) | (0x00000001 << (j + 2))
                    : UL_ONEBITS ^ ((0x00000001 << j)
                                    | (0x00000001 << (j + 2)));
            } 
            else 
            { /* Walk it back down. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (0x00000001 << (UL_LEN * 2 - 1 - j)) | (0x00000001 << (UL_LEN * 2 + 1 - j))
                    : UL_ONEBITS ^ (0x00000001 << (UL_LEN * 2 - 1 - j)
                                    | (0x00000001 << (UL_LEN * 2 + 1 - j)));
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_bitspread_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_bitspread_comparison succeed\n");
    
    return 0;
}

int test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j, k;
    ul q;
    size_t i;

    memPrint("test_bitflip_comparison\n");
    
    for (k = 0; k < UL_LEN; k++) 
    {
        q = 0x00000001 << k;
        for (j = 0; j < 8; j++) 
        {
            q = ~q;
            
            memPrint(".");
            
            p1 = (ulv *) bufa;
            p2 = (ulv *) bufb;
            for (i = 0; i < count; i++) 
            {
                *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
            }
            
            memPrint(".");
            
            if (compare_regions(bufa, bufb, count)) 
            {
                memPrint("\ntest_bitflip_comparison failed\n");
                return -1;
            }
        }
    }

    memPrint("\n");
    memPrint("test_bitflip_comparison succeed\n");
    
    return 0;
}
