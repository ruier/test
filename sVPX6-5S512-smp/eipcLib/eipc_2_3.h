
#ifndef __EIPC_H__
#define __EIPC_H__

#include <sys/types.h>
#include <time.h>
/*#include <base/b_struct_timeval.h>*/

#ifdef __cplusplus
extern "C"
{
#endif 

#define MASTER        				0x00000001/*MSG接收端口*/
#define SLAVE	     				0x00000010/*MSG发送端口*/
#define O_NOBLOCK   				0x00000100

#define O_RD        				0x00000001/*MSG接收端口*/
#define O_WONLY    					0x00000010/*MSG发送端口*/

#define GET_PORT_ATTR				0x00100001
#define SET_PORT_NONNLK				0x00100002
#define GET_DEFAULT_TIMEOUT			0x00100003
#define SET_DEFAULT_TIMEOUT			0x00100004
#define SET_PORT_MemNum				0x00100006
#define GET_PORT_MemNum				0x00100007

#define NO_DEBUG_EIPC				0

#define SUPPORTED_TASK_NUM			4


enum LINKTYPE{
	SHARE_MEM = 0,
	RAPIDIO,
	NET
};
enum MSGTYPE{
	BLK_DATA = 0,
	MSG_DATA,
};

typedef struct __PORT_ATTR__
{
	char portname[16];
	UINT8 transflag;
	UINT8 nonblk;
	UINT8 linktype;
	UINT8 connection;
	int MemNum;
	int MemIndex;
	
}PORT_ATTR;

int eipc_init(UINT8 domainid);

int eipc_init_check();

int eipc_open(char *name, int flags ,UINT8 MemNum,UINT8 Memindex);

int eipc_hunt(int handle,int timeout);

int eipc_send(int handle,UINT8 msgtype, UINT8 linktype,char *buffer, UINT32 rioaddr,int nBytes, int timeout, int tag);

int eipc_recv(int handle,UINT8 msgtype,UINT8 linktype,char *buffer, int maxBytes, int timeout, int * memindex, int tag);

int eipc_close(int handle,int timeout);

int eipc_errno_get(int isprint);

int eipc_print_attr(int handle);

#ifdef __cplusplus
}
#endif

#endif
