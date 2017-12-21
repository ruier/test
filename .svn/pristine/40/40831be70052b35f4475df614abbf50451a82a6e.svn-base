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

#undef __LINUX__ 

#define ERROR           -1
#define NET_RECV_BUF    0x100000
#define NET_SOCK_NUM    6104

#define NET_IP_ADDR1     "192.168.7.123"

int netOpenSock(char *ip);
int netCloseSock(void);
int netSendData(void);
int netRecvData(void);

int         gstClientSock;
int64_t     gullSendSize;
int64_t     gullSendSizeOld;
int64_t     gullRecvSize;
int64_t     gullRecvSizeOld;
int         giTimerFd;

int netOpenSock(char *ip)
{
    struct sockaddr_in   stSvcAddr; 

    if( (gstClientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {  
         printf("create socket ERROR: %s(errno: %d)\n", strerror(errno), errno);  
         return ERROR; 
    }
    memset(&stSvcAddr, 0, sizeof(stSvcAddr));  
    stSvcAddr.sin_family = AF_INET;  
    stSvcAddr.sin_port = htons(NET_SOCK_NUM);
    if(inet_pton(AF_INET, ip, &stSvcAddr.sin_addr) <= 0)
    {  
         printf("inet_pton ERROR for %s\n",ip);  
         return ERROR;  
    }
    printf("accepting connections  %d\n",stSvcAddr.sin_port);

    if(connect(gstClientSock,(struct sockaddr*)&stSvcAddr,sizeof(stSvcAddr)) < 0)
    {  
        printf("connect ERROR: %s(errno: %d) ip %s\n",strerror(errno), errno,ip);  
        exit(0);  
    }
    else
    {
        printf("%s connect success \n",NET_IP_ADDR);
    }

    return gstClientSock;
}

int netCloseSock(void)
{
    if(close(gstClientSock) < 0)
    {
        printf("disconnect error: %s(errno: %d)\n",strerror(errno), errno);
    }
    else
    {
        printf("%s disconnect success \n",NET_IP_ADDR);
    }
}



void netTimerHandle()
{
#ifdef __LINUX__
     int iSendSpeed = 0;
     int iRecvSpeed = 0; 
     
     iSendSpeed = gullSendSize - gullSendSizeOld;
     gullSendSizeOld = gullSendSize;
     iRecvSpeed = (gullRecvSize - gullRecvSizeOld)/0x100000;
     gullRecvSizeOld = gullRecvSize;
     
     printf("send speed is %d,Recv speed is %d\n", iSendSpeed, iRecvSpeed);
#endif     
}


int netTimerInit(void)
{
#ifdef __LINUX__
    struct timeval tv_interval = {1, 0}; 
    struct timeval tv_value = {1, 0};     
    struct itimerval it;
    
    if (signal(SIGALRM, netTimerHandle) == SIG_ERR)
    {
        printf("signal ERROR"); 
    }
    it.it_interval = tv_interval;     
    it.it_value = tv_value; 
    giTimerFd = setitimer(ITIMER_REAL, &it, NULL);; 
#endif    
    return 0;
}


int netSendData(void)
{
    char  *acSendBuf = NULL;
    int  iNum = 0;

    acSendBuf = malloc(NET_RECV_BUF);
    
    
    while(1)
    {
        if(send(gstClientSock,acSendBuf,NET_RECV_BUF, 0) < 0)  
         {  
             printf("send msg ERROR: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);  
         } 
        gullSendSize++;
    }
#ifdef __LINUX__
    close(giTimerFd);
#endif    
}

int netRecvData(void)
{
    netTimerInit();
    char  *cRecvline = NULL;
    int iRecvnum = 0;

    cRecvline = malloc(NET_RECV_BUF);
    while(1)
    {
        iRecvnum = recv(gstClientSock, cRecvline,NET_RECV_BUF, 0);
        if( iRecvnum  < 0)  
         {  
            printf("send msg ERROR: %s(errno: %d)\n", strerror(errno), errno);  
            exit(0);  
         }
        gullRecvSize = iRecvnum + gullRecvSize;
    }

#ifdef __LINUX__
    close(giTimerFd);
#endif 

}

int netClient(int argc) 
{
	char *ip = "192.168.7.123";
    netOpenSock(ip);
    
    if(argc >= 1)
    {

        logMsg("start recv !\n",0,0,0,0,0,0);
        netRecvData();
    }
    else
    {
         logMsg("start send !\n",0,0,0,0,0,0);
        netSendData();
    }
    netCloseSock();
}

void netClientTask(int argc)
{
    taskSpawn("netClientTask",30,0,0x80000,(FUNCPTR)netClient,argc,
                  0,0,0,0,0,0,0,0,0);
}


