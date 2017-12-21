/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,02jul09,syt  written from ads8572/sysNet.c/01d
*/

/*
DESCRIPTION

This module provides BSP functionality to support the
bootrom 'M' command to modify MAC addresses of on-board
network interfaces.

MAC adddress routines provided by the BSP in this file are:
    sysNetMacNVRamAddrGet()
    sysNetMacAddrGet()
    sysNetMacAddrSet()

This board provides storage in flash for the MAC addresses
of the motfcc and motscc interfaces.  This library also
implements a RAM buffer to represent the contents of the
flash.  The RAM buffer contains eight entries, which is
more than currently needed by this board, but can be
considered as room for expansion in future boards using
a derivative of this BSP.  This RAM buffer is contained
in the array glbEnetAddr[][].
*/

#ifdef ETHERNET_MAC_HANDLER

#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <errno.h> 
#include <sys/time.h> 
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>


/* locals */

/* defines */

#define MAC_OFFSET_MOTTSEC      0

const char *sysNetDevName[MAX_MAC_DEVS] = {"motetsec"};

/* globals */

IMPORT int dynamicMacAddrGen
    (
    UINT8 * ifName,         /* interface name */
    int     ifUnit,         /* interface unit */
    UINT8 * ifMacAddr,      /* address buffer */
    int     ifMacAddrLen    /* length of buffer */
    );

/* locals */

LOCAL UINT8 glbEnetAddr[MAX_MAC_ADRS][MAC_ADRS_LEN] = {
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_0, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_1, CUST_ENET4, CUST_ENET5 },
    { WR_ENET0, WR_ENET1, WR_ENET2, CUST_ENET3_2, CUST_ENET4, CUST_ENET5 },
};

LOCAL UINT8 sysInvalidAddr[2][MAC_ADRS_LEN] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

/*******************************************************************************
*
* sysMacOffsetGet - Calculate table offset
*
*  This routine calculates which table entry corresponds to
*  the specified interface.
*
*  Two values are calculated and returned in the parameters
*  pointed to by ppEnet and pOffset.
*
* RETURNS: ERROR if the interface is not known; OK otherwise
*
* ERRNO: N/A
*/

STATUS sysMacOffsetGet
    (
    char *  ifName,     /* interface name */
    int     ifUnit,     /* interface unit */
    char ** ppEnet,     /* pointer to glbEnetAddr[][] entry */
    int *   pOffset     /* offset in NVRAM */
    )
    {

    /*
     * The address offsets into NVRAM and glbEnetAddr[] are:
     *
     *  mottsec0 - offset 0
     *
     */

    if ( (strcmp(ifName,"mottsec") == 0) || (strcmp(ifName,"motetsec") == 0) )
        {
        if ( ifUnit > 3 )
            return(ERROR);

        *pOffset = ifUnit * (MAC_ADRS_LEN + 2); /* Round up to 32 bit word */

        *ppEnet  = (char*)glbEnetAddr[ifUnit + MAC_OFFSET_MOTTSEC];
        }
    else
        return(ERROR);

    return(OK);
    }

/*******************************************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  Non Volatile RAM, and store it in the ifMacAddr
*  buffer provided by the caller.
*
*  It is not required for the BSP to provide NVRAM to store
*  the MAC address.  Also, some interfaces do not allow
*  the MAC address to be set by software.  In either of
*  these cases, this routine simply returns ERROR.
*
*  Given a MAC address m0:m1:m2:m3:m4:m5, the byte order
*  of ifMacAddr is:
*   m0 @ ifMacAddr
*   m1 @ ifMacAddr + 1
*   m2 @ ifMacAddr + 2
*   m3 @ ifMacAddr + 3
*   m4 @ ifMacAddr + 4
*   m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacNVRamAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;
    char tmp[MAC_ADRS_LEN];

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

#if (NV_RAM_SIZE != NONE)

    /* get MAC address from NvRAM. */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);

#endif /* (NV_RAM_SIZE != NONE) */

    if ( memcmp(pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0 )
        return(ERROR);

    if ( memcmp(pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0 )
        return(ERROR);

    if((pEnet[0]!=WR_ENET0) || (pEnet[1]!=WR_ENET1) || (pEnet[2]!=WR_ENET2))
        {
        int i,j;

        for (i = MAC_ADRS_LEN - 1,j=0; i >= 0; i--,j++)
            tmp[j] = pEnet[i];

        /* mac address in memory only */

        memcpy (ifMacAddr, tmp, ifMacAddrLen);
        }
    else
        memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

/*******************************************************************************
*
* sysNetMacAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  network interface, and stores it in the ifMacAddr
*  buffer provided by the caller.
*
*  If the network interface cannot be queried about the
*  MAC address, this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    /*
     * None of our interfaces can be queried directly.
     * Return ERROR to indicate that we need to use
     * RAM/NVRAM instead.
     */

    return(ERROR);
    }

/***********************************************************
*
* sysNetMacAddrSet - Save interface MAC address
*
*  This routine saves the MAC address specified in
*  ifMacAddr to the appropriate location in NVRam (if
*  possible) and update the specified interface to use
*  the specified MAC address.
*
*  If the network interface MAC address cannot be set,
*  this routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/

STATUS sysNetMacAddrSet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return(ERROR);

#if (NV_RAM_SIZE != NONE)

    /* check MAC address in NvRAM. */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);

    if (0 == memcmp (ifMacAddr, pEnet, ifMacAddrLen))
        {

        /* same address so don't erase and rewrite flash */

        printf("Address unchanged\n");

        return (OK);
        }

    sysNvRamSet ((char*)ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET+offset);
#endif  /* (NV_RAM_SIZE != NONE) */

    /* mac address in memory only */

    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

/***********************************************************
*
* sysMacIndex2Unit - convert index range to unit number
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a unit number.
*
* RETURNS: unit number of indexed device
*
* ERRNO: N/A
*/

int sysMacIndex2Unit
    (
    int index
    )
    {
    return (index);   /* remaining three are motfcc (0,1,2) */
    }

/***********************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a device string index e.g. motfcc.
*
* RETURNS: index access device name in sysNetDevName
*
* ERRNO: N/A
*/

int sysMacIndex2Dev
    (
    int index
    )
    {
    return (0);      /* remaining are motfcc */
    }

#endif /* ETHERNET_MAC_HANDLER */

#if 1
#define ERROR           -1
#define NET_SEND_BUF    32 * 1024
#define NET_RECV_BUF    0x100000

#define NET_SOCK_NUM    6104
#define NET_IP_ADDR     "192.168.5.21"

int     iNetSvcSockFd;

int64_t gullSendSize;
int64_t gullSendSizeOld;
int64_t gullRecvSize;
int64_t gullRecvSizeOld;


int netSvcOpenSock(void)
{
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    int   iNetListenSock;
    int   temp_sock_descriptor;
    int   address_size;
    int   i,len;
    int   iSockBufSize = 0;
    int   iRet = 0;
    char  *p;
    
    
    iNetListenSock = socket(AF_INET,SOCK_STREAM,0);
    if(iNetListenSock== -1)
    {
        perror("call to socket");
        exit(1);
    }
    
    bzero(&sin,sizeof(sin));

    sin.sin_family = AF_INET;
    if( inet_pton(AF_INET, NET_IP_ADDR, &sin.sin_addr) <= 0)
    {  
        printf("inet_pton error for %s\n",NET_IP_ADDR);  
        return ERROR;  
    }
    sin.sin_port = htons(NET_SOCK_NUM);

#if 1
    iSockBufSize = 4*1024*1024;
    iRet = setsockopt(iNetListenSock, SOL_SOCKET, SO_RCVBUF,
                       (char *)&iSockBufSize, sizeof(int));
    if (iRet == -1)
    {
         printf("Couldn't getsockopt receive buffer size\n");
         exit(-1);
    }
    printf("setsockopt receive buffer size %d \n",iSockBufSize);
    
#endif
    
    if(bind(iNetListenSock,(struct sockaddr *)&sin,sizeof(struct sockaddr)) ==-1)
    {
        perror("call to bind");
        exit(1);
    }
    if(listen(iNetListenSock,20) == -1)
    {
        perror("call to listen");
        exit(1);
    }
    
    p = inet_ntoa(sin.sin_addr);
    printf("accepting connections %s %d\n",p,sin.sin_port);
            
    address_size = sizeof(struct sockaddr_in);
    
    printf("size of address_size is  %d,port is %d\n",address_size,sin.sin_port);
    iNetSvcSockFd = accept(iNetListenSock,(struct sockaddr *)&pin, &address_size);
    if(iNetSvcSockFd == -1)
    {
       perror("call to accept");
       exit(1);
    }

    iSockBufSize = 0;
    len = sizeof(iSockBufSize);
    iRet = getsockopt(iNetSvcSockFd, SOL_SOCKET, SO_RCVBUF,
                       (char *)&iSockBufSize, &len);
    if (iRet == -1)
    {
         printf("Couldn't getsockopt receive buffer size\n");
         exit(-1);
    }
    
    
    printf("getsockopt receive buffer size1 %d \n",iSockBufSize);
	 iRet = getsockopt(iNetSvcSockFd, SOL_SOCKET, SO_SNDBUF,
                       (char *)&iSockBufSize, &len);
     if (iRet == -1)
    {
         printf("Couldn't getsockopt send buffer size\n");
         exit(-1);
    }
	 
	 printf("getsockopt send buffer size1 %d \n",iSockBufSize);
    return 0;
}


int netSvcRecvData(void)
{
    char *pcBuf = NULL;
    int iRecvnum = 0;
    INT32 tickStart = 0;
    INT32 tickEnd = 0;
    INT32 tickSpend = 0;

    pcBuf = malloc(0x100000);
    tickStart = tickGet();
    while(1)
    {
        iRecvnum = recv(iNetSvcSockFd,pcBuf,NET_RECV_BUF, 0);
        if( iRecvnum  < 0)  
         {  
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);  
         }
         #if 0
         gullRecvSize = iRecvnum + gullRecvSize;
         if(gullRecvSize >= 0x40000000)
         {
            tickEnd = tickGet();
            tickSpend = tickEnd - tickStart;
            logMsg("recv speed is %dMB/s \n",gullRecvSize/(tickSpend*1000),0,0,0,0,0);
            tickStart = tickGet();
            gullRecvSize = 0;
         }
         #endif
    }
}


int netSvcCloseSock(void)
{
    if(close(iNetSvcSockFd) < 0)
    {
        printf("disconnect error: %s(errno: %d)\n",strerror(errno),errno);
    }
    else
    {
        printf("%s disconnect success \n",NET_IP_ADDR);
    }
}

int netSvcSendData(void)
{
    char *pcBuf = NULL;
    int inum = 0;

    pcBuf = malloc(NET_SEND_BUF);
    
    while(1)
    {
        if(send(iNetSvcSockFd, pcBuf, NET_SEND_BUF, 0) < 0)  
         {  
            printf("send xx msg error: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);  
         } 
        gullSendSize++;
    }
}

int netServer(int argc) 
{

    netSvcOpenSock();
    
    if(argc >= 1)
    {
        netSvcRecvData();
    }
    else
    {
        netSvcSendData();
    }
    netSvcCloseSock();
}

void netSvcTask(int argc)
{
    taskSpawn("netSvcTask",20,0,0x80000,(FUNCPTR)netServer,argc,
                  0,0,0,0,0,0,0,0,0);
}
#endif

