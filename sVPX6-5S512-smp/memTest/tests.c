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

#include "eipclib/eipc_2_3.h"

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ipnet/ipioctl.h>



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


#if 0
void test_incore_eipc_scatter(int memnum,u8 msgtype,int Num,int size,int master)
{
	int coreid,i,j,k,errorflag;
	int ret,memindex;
	
	coreid = cpu_id_get();
	etrace("current core id = %d\r\n",coreid);
	
	if(coreid == 0)
	{
		etrace("before open\r\n");
		chandle[coreid] = create_master_port(memnum,0);
		etrace("after open\r\n");
	}
	else
		chandle[coreid] = create_slave_port(memnum,coreid);

	etrace("before eipc_hunt\r\n");
	eipc_hunt(chandle[coreid],-1);
	etrace("after eipc_hunt\r\n");

	if(coreid == 0)
	{
		char *sndbuf = (char *)memalign(128,size+1024);;
		char *rcvbuf = (char *)memalign(128,size+1024);;
		memset(rcvbuf,0,size);
		for(i = 0;i < Num;i++)
		{
			
			/*send to*/
			eipc_barrier(chandle[coreid],0,-1);//同步时为了不让下面的memset执行，否则接收方DMA可能取到下一组数据。如不加同步，可通过在发送接收间再加一步流程
			memset(sndbuf,10+i,size);
			for(j=0;j<memnum-1;j++)
			{
				ret = eipc_send(chandle[coreid],msgtype,SHARE_MEM, 0, sndbuf, size, -1,0);
				if(ret == -1)
				{
					eipc_errno_get(1);	
					exit(1);
				}
				else
				{
				}
			}

			
		}
		free(sndbuf);
		free(rcvbuf);

	}
	else
	{
		char *sndbuf = (char *)memalign(128,size+1024);;
		char *rcvbuf = (char *)memalign(128,size+1024);;
		memset(sndbuf,1,size);
		memset(rcvbuf,0,size);
		for(i = 0;i < Num;i++)
		{
			
			/*recv from*/
			eipc_barrier(chandle[coreid],0,-1);
			errorflag = 0;
			memset(sndbuf,10+i,size);
			bsp_cache_flush(1,sndbuf,size);
			ret = eipc_recv(chandle[coreid],msgtype,SHARE_MEM,rcvbuf, size, -1, &memindex,0);
			if(ret == -1)
			{
				eipc_errno_get(1);			
			}
			else
			{
			}
			
			if(memcmp(sndbuf,rcvbuf,ret)!=0)
			{
				etrace("there are error happened !\r\n");
				exit(1);
			}

		}
		free(sndbuf);
		free(rcvbuf);
	}
	if(eipc_close(chandle[coreid],10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}
void test_incore_eipc_gather(int memnum,u8 msgtype,int Num,int size,int master)
{
	int coreid,i,j,k,errorflag;
	int ret,memindex;
	
	coreid = cpu_id_get();
	etrace("current core id = %d\r\n",coreid);
	
	if(coreid == 0)
	{
		etrace("before open\r\n");
		chandle[coreid] = create_master_port(memnum,0);
		etrace("after open\r\n");
	}
	else
		chandle[coreid] = create_slave_port(memnum,coreid);

	etrace("before eipc_hunt\r\n");
	eipc_hunt(chandle[coreid],-1);
	etrace("after eipc_hunt\r\n");

	if(coreid == 0)
	{
		char *sndbuf = (char *)memalign(128,size+1024);;
		char *rcvbuf = (char *)memalign(128,size+1024);;
		memset(rcvbuf,0,size);
		for(i = 0;i < Num;i++)
		{		
			/*recv from */
			errorflag = 0;
			memset(sndbuf,1+i,size);
			bsp_cache_flush(1,sndbuf,size);
			eipc_barrier(chandle[coreid],0,-1);
			for(j=0;j<memnum-1;j++)
			{
				ret = eipc_recv(chandle[coreid],msgtype,SHARE_MEM,rcvbuf, size, -1, &memindex,0);
				if(ret == -1)
				{
					etrace("eipc_recv failed,current rioid = %d\r\n",rio_id_get());
					eipc_errno_get(1);			
				}
				else
				{
					etrace("[%d] there are %u bytes receved from [%d],rcvbuf[1000] = %d,rcvbuf[100000] = %d\r\n",i,ret,memindex,rcvbuf[1000],rcvbuf[100000]);
				}

				if(memcmp(sndbuf,rcvbuf,ret)!=0)
				{
					etrace("there are error happened !\r\n");
					exit(1);
				}
			}
		}
		free(sndbuf);
		free(rcvbuf);

	}
	else
	{
		char *sndbuf = (char *)memalign(128,size+1024);;
		char *rcvbuf = (char *)memalign(128,size+1024);;
		memset(sndbuf,1,size);
		memset(rcvbuf,0,size);
		for(i = 0;i < Num;i++)
		{
			
			eipc_barrier(chandle[coreid],0,-1);
			memset(rcvbuf,1+i,size);
			ret = eipc_send(chandle[coreid],msgtype,SHARE_MEM, 0, rcvbuf, size, -1,0);
			if(ret == -1)
			{
				etrace("eipc_send failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				etrace("[%d]there are %u bytes send\r\n",i,ret);
			}
		}
		free(sndbuf);
		free(rcvbuf);
	}
	if(eipc_close(chandle[coreid],10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}
int mh;
void test_scatter(int memnum,u8 msgtype,int Num,int size,int master)
{
	int rioid,i,slotid,indexid;
	int ret,memindex;
	
	char *sndbuf = (char *)0xc0000000;
	char *rcvbuf = (char *)0xd0000000;
	
	rioid = rio_id_get();
	slotid = (rioid-1)/4;
	indexid = rioid - (slotid*4+1);
	etrace("current rioid = %d , slotid = %d , indexid = %d\r\n",rioid,slotid,indexid);
	
	if(rioid == (slotid*4+1))
	{
		mh = create_master_port(memnum,indexid);
	}
	else 
		mh = create_slave_port(memnum,indexid);

	etrace("before eipc_hunt\r\n");
	eipc_hunt(mh,-1);
	etrace("after eipc_hunt\r\n");
	eipc_print_attr(mh);
	
//	return;
	
	if(rioid == (slotid*4+1))
	{
		for(i=0;/*i<Num*/;i++)
		{
			
			
			
			/*etrace("[%d]before barrier\r\n",i);
			eipc_barrier(mh,0,10);
			etrace("[%d]after barrier,sleeping time = %d\r\n",i,indexid+1);
			sleep(indexid+1);
			continue;*/
			/*send out */
				memset(sndbuf,i,size);

				ret = eipc_scatter(mh,msgtype,sndbuf,size, 20);
				if(ret == -1)
				{
					etrace("eipc_send failed,current rioid = %d\r\n",rio_id_get());
					eipc_errno_get(1);			
				}
				else
				{
					etrace("[%d] there are %u bytes send\r\n",i,ret);
				}
		}


		

	}
	else
	{
		for(i=0;/*i<Num*/;i++)
		{
			
			/*etrace("[%d]before barrier\r\n",i);
			eipc_barrier(mh,0,10);
			etrace("[%d]after barrier,sleeping time = %d\r\n",i,indexid+1);
			sleep(indexid+1);
			continue;*/
			
			memset(sndbuf,i,size);

			ret = eipc_scatter(mh,msgtype,rcvbuf,size, 100);
			if(ret == -1)
			{
				etrace("eipc_recv failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				etrace("[%d] there are %u bytes receved from [%d],rcvbuf[1000] = %d\r\n",i,ret,memindex,rcvbuf[1000]);
			}
/*			if(memcmp(sndbuf,rcvbuf,ret)!=0)
			{
				etrace("there are error happened !\r\n");
				exit(1);
			}*/
		}

	}
	
	if(eipc_close(mh,10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}
void test_gather(int memnum,u8 msgtype,int Num,int size,int master)
{
	int rioid,i,j,slotid,indexid;
	int ret,memindex;
	
	char *sndbuf = (char *)0xc0000000;
	char *rcvbuf = (char *)0xd0000000;
	
	rioid = rio_id_get();
	slotid = (rioid-1)/4;
	indexid = rioid - (slotid*4+1);
	etrace("current rioid = %d , slotid = %d , indexid = %d\r\n",rioid,slotid,indexid);
	
	if(rioid == (slotid*4+1))
	{
		mh = create_master_port(memnum,indexid);
	}
	else 
		mh = create_slave_port(memnum,indexid);

	etrace("before eipc_hunt\r\n");
	eipc_hunt(mh,-1);
	etrace("after eipc_hunt\r\n");
	eipc_print_attr(mh);
	
	if(rioid == (slotid*4+1))
	{
		for(i=0;/*i<Num*/;i++)
		{
			/*memset(sndbuf,i+3,size);
			memset(sndbuf+size,i+4,size);
			memset(sndbuf+2*size,i+5,size);*/
			ret = eipc_gather(mh,rcvbuf, 4*size, 10);
			if(ret == -1)
			{
				etrace("eipc_recv failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				etrace("[%d] there are %u bytes receved from [%d],rcvbuf[1000] = %d\r\n",i,ret,memindex,rcvbuf[1000]);
			}
			
			/*if(memcmp(sndbuf,rcvbuf,ret)!=0)
			{
				etrace("there are error happened !\r\n");
				exit(1);
			}
			else{
				etrace("[%d] prefect!\r\n",i);
			}*/
			
			

		}

	}
	else
	{
		for(i=0;/*i<Num*/;i++)
		{
	
			memset(sndbuf,i+2+indexid,size);
			ret = eipc_gather(mh,  sndbuf, size, 50);
			if(ret == -1)
			{
				etrace("eipc_send failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				etrace("there are %u bytes send\r\n",ret);
			}
		}

	}
	
	if(eipc_close(mh,10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}

void test_eipc_gather(int memnum,u8 msgtype,u8 linktype,int Num,int size,int master)
{
	int rioid,i,j,slotid,indexid;
	int ret,memindex;
	
	int MemNum = 0;
	
	char *sndbuf = (char *)0xc1000000;
	char *rcvbuf = (char *)0xd1000000;
	
	rioid = rio_id_get();
	slotid = (rioid-1)/4;
	indexid = rioid - (slotid*4+1);
	etrace("current rioid = %d , slotid = %d , indexid = %d\r\n",rioid,slotid,indexid);
	
	if(rioid == (slotid*4+1))
	{
		mh = create_master_port(memnum,indexid);
	}
	else 
		mh = create_slave_port(memnum,indexid);
			

	etrace("before eipc_hunt\r\n");
	eipc_hunt(mh,-1);
	etrace("after eipc_hunt\r\n");
	/*eipc_print_attr(mh);*/
	
	if(rioid == (slotid*4+1))
	{
		for(i=0;/*i<Num*/;i++)
		{
			eipc_barrier(mh,0,10);
			memset(sndbuf,indexid+i,size);
			MemNum = eipc_get_MemNum(mh);
			for(j=0;j<MemNum;j++)
			{
//				etrace("j = %d,memnum = %d\r\n",j,eipc_get_MemNum(mh));
				struct	in_addr remoteip;
				ret = eipc_recv(mh,msgtype,linktype,rcvbuf, size, 10, (int *)&remoteip,0);
				if(ret == -1)
				{
					etrace("eipc_recv failed,current rioid = %d\r\n",rio_id_get());
					eipc_errno_get(1);			
				}else if(ret == -2)
				{
					continue;
				}
				else
				{
					if(linktype == NET)
						etrace("[%d] there are %u bytes receved from [%s],rcvbuf[1000] = %d\r\n",i,ret,inet_ntoa(remoteip),rcvbuf[1000]);
					else
						etrace("[%d] there are %u bytes receved from [%d],rcvbuf[1000] = %d\r\n",i,ret,*((int *)&remoteip),rcvbuf[1000]);
				}
				
				/*if(memcmp(sndbuf,rcvbuf,ret)!=0)
				{
					etrace("there are error happened !\r\n");
					exit(1);
				}*/
				
			}
			
			

		}

	}
	else
	{
		for(i=0;/*i<Num*/;i++)
		{
			eipc_barrier(mh,0,10);
			memset(sndbuf,i+indexid,size);
			ret = eipc_send(mh,msgtype,linktype, 0,sndbuf, size, 100,0);
			if(ret == -1)
			{
				etrace("eipc_send failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				etrace("[%d]there are %u bytes send\r\n",i,ret);
			}
		}

	}
	
	if(eipc_close(mh,10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}

void test_eipc_scatter(int memnum,u8 msgtype,u8 linktype,int Num,int size,int master)
{
	int rioid,i,j,slotid,indexid;
	int ret,memindex;
	
	int MemNum = 0;
	
	char *sndbuf = (char *)0xc0000000;
	char *rcvbuf = (char *)0xd0000000;
	
	rioid = rio_id_get();
	slotid = (rioid-1)/4;
	indexid = rioid - (slotid*4+1);
	etrace("current rioid = %d , slotid = %d , indexid = %d\r\n",rioid,slotid,indexid);
	
	if(rioid == (slotid*4+1))
	{
		mh = create_master_port(memnum,indexid);
	}
	else 
		mh = create_slave_port(memnum,indexid);

	etrace("before eipc_hunt\r\n");
	eipc_hunt(mh,-1);
	etrace("after eipc_hunt\r\n");
	eipc_print_attr(mh);
	
	if(rioid == (slotid*4+1))
	{
		for(i=0;/*i<Num*/;i++)
		{
			
			eipc_barrier(mh,0,10);
			memset(sndbuf,i+1,size);
			
			MemNum = eipc_get_MemNum(mh);
			etrace("i=%d,memnum=%d\r\n",i,MemNum);
			for(j=0;j<MemNum;j++)
			{
				
				ret = eipc_send(mh ,msgtype,linktype,0,sndbuf, size, 10,0);
				if(ret == -1)
				{
					etrace("eipc_send failed,current rioid = %d\r\n",rio_id_get());
					eipc_errno_get(1);			
				}
				else if(ret == -2)
				{
					continue;				
				}
				else
					;
//					etrace("【%d】there are %u bytes send\r\n",i,ret);
				
			}
		}

	}
	else
	{
		for(i=0;/*i<Num*/;i++)
		/*for(;;)*/
		{
	

			eipc_barrier(mh,0,10);
			/*memset(sndbuf,1+i,size);*/
			
			struct	in_addr remoteip;
			
			ret = eipc_recv(mh,msgtype,linktype,rcvbuf, size, 100, (int *)&remoteip,0);
			if(ret == -1)
			{
				etrace("eipc_recv failed,current rioid = %d\r\n",rio_id_get());
				eipc_errno_get(1);			
			}
			else
			{
				
				if(linktype == NET)
						etrace("[%d] there are %u bytes receved from [%s],rcvbuf[1000] = %d\r\n",i,ret,inet_ntoa(remoteip),rcvbuf[1000]);
					else
						etrace("[%d] there are %u bytes receved from [%d],rcvbuf[1000] = %d\r\n",i,ret,*((int *)&remoteip),rcvbuf[1000]);
			}
			
			/*if(memcmp(sndbuf,rcvbuf,ret)!=0)
			{
				etrace("there are error happened !\r\n");
				exit(1);
			}*/
			
		}

	}
	
	if(eipc_close(mh,10)==-1)
		etrace("there are error happened when closed !\r\n");
		
}
#endif
/**************************************************************/


