
#ifndef __EIPC_H__
#define __EIPC_H__

#include <sys/types.h>
#include <sys/time.h>
/*#include <base/b_struct_timeval.h>*/

#ifdef __cplusplus
extern "C"
{
#endif 

#define O_RD        0x00000001/*MSG接收端口*/
#define O_WONLY     0x00000010/*MSG发送端口*/


#define O_NOBLOCK   0x00000100

#define RWNODE		0x01
#define WNODE		0x02/*BLK接收端口*/
#define RNODE		0x03/*BLK发送端口*/



#define MSGBLK_OPTION_SIGNAL 0x01         /* gen signal after DMA    */


#ifndef TRAFFICCOUNT
#define TRAFFICCOUNT

#define BLKPORT 37
#define MSGPORT 73
typedef struct
{
	char S_Name[16];
	UINT8 flag;
	unsigned long long riotrafnum;
	unsigned long long edmatrafnum;
	int transflag;
	UINT8 srcid;
	UINT8 srccore;
	
	UINT8 dstid;
	UINT8 dstcore;
	struct timeval tv;
}TRAFFICC;
#endif

enum LINKTYPE{
	SHARE_MEM = 0,
	RAPIDIO,
	NET
};
enum MSGTYPE{
	BLK_DATA = 0,
	MSG_DATA,
};


int eipc_init();

int eipc_init_check();

int eipc_gather_open( char *name, int flags,unsigned char linktype,unsigned char msgtype);

int eipc_gather_close( int handle );

int eipc_gather_send( int handle , char *buffer , int nBytes , int timeout, char *msg, int msglen);

int eipc_gather_receive (int handle , char * buffer , int maxBytes , int timeout, int *SrcID, int *SrcCore , char *msg, int msglen);

int eipc_scatter_open( char *name, int flags,unsigned char linktype,unsigned char msgtype);

int eipc_scatter_close( int handle );

int eipc_scatter_send( int handle , char *buffer , int nBytes , int timeout, char *msg, int msglen);

int eipc_scatter_receive (int handle , char * buffer , int maxBytes , int timeout, int *SrcID, int *SrcCore , char *msg, int msglen);

int eipc_p2p_open( char *name, int flags,unsigned char linktype,unsigned char msgtype);

int eipc_p2p_close( int handle );

int eipc_p2p_send( int handle , char *buffer , int nBytes , int timeout, char *msg, int msglen);

int eipc_p2p_receive (int handle , char * buffer , int maxBytes , int timeout, int *SrcID, int *SrcCore , char *msg, int msglen);


UINT32 eipc_get_selfid(void);

UINT32 eipc_get_selfcore(void);

int eipc_errno_get(int isprint);

int eipc_traf_ctrl(UINT32 flag);

int eipc_msg_port_num(int *NumSnd, int *NumRcv);

int eipc_blk_port_num(int *NumSnd, int *NumRcv);

int eipc_msg_traf_count(void *mem, UINT32 size);

int eipc_blk_traf_count(void *mem, UINT32 size);

#ifdef __cplusplus
}
#endif

#endif
