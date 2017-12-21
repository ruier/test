

#define FL_BASE_ADDR  FLASH_SHARE_MEM //0x0f000000	// 0x10000000
#define FL_OFFSET	   0x400
#define CPUB_REG_BASE  0x0
#define CPUC_REG_BASE  0x100
#define CPUD_REG_BASE  0x200

#define FL_CPUB_BASE	   0x000000
#define FL_CPUC_BASE	   0x400000
#define FL_CPUD_BASE	   0x800000

#define EP_FLAG_OFFSET	   0
#define FLLEN_OFFSET	   0x04
#define FNLEN_OFFSET	   0x08
#define FLNAME_OFFSET      0x0c
#define ENTRY_OFFSET	   0x18

SEM_ID flashIoSem;

extern int sysGetCpuID(void);
extern unsigned int getBarAddr(int busNo,int deviceNo,int funNo,int barNo);
void flashIoInit();
void readvxWorks();
void filenameSet(char *string,UINT32 fnLenOffset, UINT32 fnOffset);
void ddrTrans(unsigned int src,unsigned int dst,unsigned int size);
void queryTask();
