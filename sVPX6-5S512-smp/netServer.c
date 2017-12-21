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

#define ERROR           -1
#define NET_SEND_BUF    32 * 1024
#define NET_RECV_BUF    0x100000

#define NET_SOCK_NUM    6104
#define NET_IP_ADDR     "192.168.7.123"

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
         #if 1
         gullRecvSize = iRecvnum + gullRecvSize;
         if(gullRecvSize >= 0x40000000)
         {
            tickEnd = tickGet();
            tickSpend = tickEnd - tickStart;
            logMsg("recv speed is %dMB/s \n",gullRecvSize/1024/1024/(tickSpend/60),0,0,0,0,0);
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

