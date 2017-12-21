#include "sys/stat.h"
#include "assert.h"
#include "sockLib.h"
#include "in.h"
/*#include <iprip.h>
#include <ipftps.h>*/
#include "ppcApi.h"
#include "sysUsr.h"


#define FALSH_BUF_LEN       64
#define DQ7 0X00800080
#define DQ6 0X00400040

INT32 flashProgramWait(VUINT16 *puiSectorAddr,UINT16 uiVal)
{
	UINT32  uiLoop = 0;
	
	for(uiLoop = 0;uiLoop < 0x1ff0000; uiLoop++)
	{
        if(*puiSectorAddr == uiVal)
        {
            return OK;
        }
	}
    logMsg("addr 0x%x timeout val 0x%x expect 0x%x\n",(ULONG)puiSectorAddr,
          *puiSectorAddr,uiVal,0,0,0);

    return ERROR;
}


INT32 flashErase(UINT32 uiBaseAddr,UINT32 uiAddr)
{
    VUINT16 *puiBaseAddr = (VUINT16*)uiBaseAddr;
    VUINT16 *puiSectorAddr = (VUINT16*)uiAddr;

	*(puiBaseAddr + 0x555) = 0x00aa;
    *(puiBaseAddr + 0x2aa) = 0x0055;
    *(puiBaseAddr + 0x555) = 0x0080;
    *(puiBaseAddr + 0x555) = 0x00aa;
    *(puiBaseAddr + 0x2aa) = 0x0055;

    *puiSectorAddr = 0x0030;

    if(OK != flashProgramWait(puiSectorAddr,0xffff))
    {
        return ERROR;
    }
	
    *puiBaseAddr = 0x00f0;

	return OK;
}


INT32 flashErasePart(UINT32 uiBaseAddr,UINT32 uiAddr,UINT32 uiTimes,UINT32 uiFlag)
{
    UINT32   uiLoop = 0;
    UINT32   uiOffset = 0;
    UINT32   uiOnePercent = 0;
    UINT32   uiPercent = 0;

    
    uiOffset = uiAddr;
    uiOnePercent = (uiTimes / 100) + 1;
    if(0x0 == uiTimes)
    {
        return OK;
    }
    
    for(uiLoop = 0; uiLoop < uiTimes;uiLoop++)
	{
		if(OK != flashErase(uiBaseAddr,uiOffset))
		{
            return ERROR;
        }
		uiOffset += 0x20000;
        if((0x0 != uiFlag) && (0x00 == (uiLoop % uiOnePercent)))
        {
            if(0x1 == uiOnePercent)
            {
                uiPercent = 100 * uiLoop / uiTimes;
            }
            
            if(uiPercent < 100)
            {
                printf("erase flash %d%%\r",uiPercent);
            }
            uiPercent++;
        }
	}
    
    return OK;
}


INT32 flashProgram(UINT32 uiBaseAddr,UINT32 uiAddr,UINT8 *pucBuf,UINT32 uiLen,
                   UINT32 uiFlag)
{
	VUINT16 *puiBaseAddr = (VUINT16*)uiBaseAddr;
	VUINT16 *puiSectorAddr = (VUINT16*)uiAddr;
	UINT32	*puiData = NULL;
	UINT16	 usVal = 0;
	UINT32	 uiLoop = 0;
	UINT32	 uiOnePercent = 0;
	UINT32	 uiPercent = 0;

	uiLen = uiLen / sizeof(UINT32);
	puiData = (UINT32*)pucBuf;
	uiOnePercent = (uiLen / 100) + 1;

	for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
	{
		if((0x0 != uiFlag) && (0x00 == (uiLoop % uiOnePercent)))
		{
			if(uiPercent < 100)
			{
				printf("program flash %d%%\r",uiPercent);
			}
			uiPercent++;
		}
		
		*(puiBaseAddr + 0x555) = 0x00aa;
		*(puiBaseAddr + 0x2aa) = 0x0055;
		*(puiBaseAddr + 0x555) = 0x00a0;
		
		usVal = ((*puiData) >> 16) & 0xffff;
		*puiSectorAddr = usVal;
		
		if(OK != flashProgramWait(puiSectorAddr,usVal))
		{
			return ERROR;
		}
		/*puiBaseAddr = 0x00f0;*/
		puiSectorAddr++;

		*(puiBaseAddr + 0x555) = 0x00aa;
		*(puiBaseAddr + 0x2aa) = 0x0055;
		*(puiBaseAddr + 0x555) = 0x00a0;
		
		usVal = (*puiData) & 0xffff;
		*puiSectorAddr = usVal;
		
		if(OK != flashProgramWait(puiSectorAddr,usVal))
		{
			return ERROR;
		}
		/**puiBaseAddr = 0x00f0;*/
		puiSectorAddr++;
		puiData++;
	}
		
	return OK;
}



ULONG flashGetFileLen(char *fileName)
{
    struct stat  fileStat; 
    
    if (stat (fileName, &fileStat) != OK)
    {
        return 0;
    }
    else
    {
        return fileStat.st_size ;
    }
}

VOID sysBootProgram(UINT32 len,UINT8 *pucData)
{
    if(0 != (len % 4))
    {
        printf("buf must be align dword\n");
        return;
    }
	 
    if(OK != flashErasePart(FLASH_BASE_ADRS1,FSL_BOOT_ADDR,8,FLASH_PROG_PRINT))
    {
        return ;
    }

    if(OK != flashProgram(FLASH_BASE_ADRS1,FSL_BOOT_ADDR,pucData,len,FLASH_PROG_PRINT))
    {
        return;
    }
}

STATUS bootFileLoad(char *pucFileName)
{
    FILE            *pstFp = NULL;
    UINT8           *pucBuf = NULL;
    UINT32           uiReadLen = 0;
    ULONG            ulFileLen = 0;

    ulFileLen = flashGetFileLen(pucFileName);
    if(0x00 == ulFileLen)
    {
        logMsg("file not exist\n",0,0,0,0,0,0);
        return ERROR;
    }

    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    if(ulFileLen > FSL_MAX_BOOT_LEN)
    {
        logMsg("file is too large max 6M\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    pucBuf = malloc(FSL_MAX_BOOT_LEN);
    if(NULL == pucBuf)
    {
        logMsg("malloc memory failed\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    uiReadLen = fread(pucBuf,sizeof(UINT8),FSL_MAX_BOOT_LEN,pstFp);
    if(ulFileLen != uiReadLen)
    {
        logMsg("read file error\n",0,0,0,0,0,0);
        fclose(pstFp);
        free(pucBuf);
        return ERROR;
    }
    
    sysBootProgram(uiReadLen,pucBuf);
    
    free(pucBuf);
    fclose(pstFp);

    return OK;
}


VOID sysRcwProgram(UINT32 len,UINT8 *pucData)
{
    if(0 != (len % 4))
    {
        printf("buf must be align dword\n");
        return;
    }
	 
    if(OK != flashErasePart(FLASH_BASE_ADRS,FLASH_BASE_ADRS,1,FLASH_PROG_PRINT))
    {
        return ;
    }

    if(OK != flashProgram(FLASH_BASE_ADRS,FLASH_BASE_ADRS,pucData,len,FLASH_PROG_PRINT))
    {
        return;
    }
}


STATUS bootRcwLoad(char *pucFileName)
{
    FILE            *pstFp = NULL;
    UINT8           *pucBuf = NULL;
    UINT32           uiReadLen = 0;
    ULONG            ulFileLen = 0;

    ulFileLen = flashGetFileLen(pucFileName);
    if(0x00 == ulFileLen)
    {
        logMsg("file not exist\n",0,0,0,0,0,0);
        return ERROR;
    }

    pstFp = fopen(pucFileName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    if(ulFileLen > FSL_MAX_RCW_LEN)
    {
        logMsg("file is too large max 80Byte\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    pucBuf = malloc(FSL_MAX_RCW_LEN);
    if(NULL == pucBuf)
    {
        logMsg("malloc memory failed\n",0,0,0,0,0,0);
        fclose(pstFp);
        return ERROR;
    }
    
    uiReadLen = fread(pucBuf,sizeof(UINT8),FSL_MAX_RCW_LEN,pstFp);
    if(ulFileLen != uiReadLen)
    {
        logMsg("read file error\n",0,0,0,0,0,0);
        fclose(pstFp);
        free(pucBuf);
        return ERROR;
    }
    
    sysRcwProgram(uiReadLen,pucBuf);
    
    free(pucBuf);
    fclose(pstFp);

    return OK;
}


STATUS readBuffer(char *pucName,UINT32 uiAddr,UINT32 uiLen)
{
    FILE         *pstFp = NULL;

    pstFp = fopen(pucName,"w+b");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);
    
    fwrite((VOID*)uiAddr,sizeof(UINT8),uiLen,pstFp);

    fclose(pstFp);
    
    return OK;
}


STATUS writeBuffer(char *pucName,UINT32 uiAddr,UINT32 uiLen)
{
    FILE         *pstFp = NULL;

    pstFp = fopen(pucName,"r");
    if(NULL == pstFp)
    {
        logMsg("open file failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    logMsg("open file suceed\n",0,0,0,0,0,0);
    
    fread((VOID*)uiAddr,sizeof(UINT8),uiLen,pstFp);

    flashProgram(0xf8000000,0xf8000000,(UINT8*)uiAddr,0x60000,1);
    
    fclose(pstFp);
    
    return OK;
}
UINT32 flashSetInc(UINT32* pcBuf,UINT32 uiLen)
{
	UINT32 uiLoop = 0;
	
	for(uiLoop = 0;uiLoop < (uiLen/4);uiLoop++)
	{
		pcBuf[uiLoop] = uiLoop;
	}
}

INT32 flashTest(UINT32 uiAddr,UINT32 uiflashAddr,UINT32 uiLen)
{

	UINT32 time = 0;
	INT32 fd = -1;
	
#if 0
	flashSetInc(0x10000000,0x100000);
	fd = open("/tffs0/file",0x209,0);
	if(fd < 0)
	{
		printf("open file failed \n");
		return ERROR;
	}
	time = tickGet();
	if(write(fd,uiAddr,uiLen) !=  uiLen)
	{
		printf("write data error \n");
		close(fd);
		return ERROR;
	}
	close(fd);
	printf("flash write file %d \n",(tickGet()-time)/60);
	time = tickGet();
	copy("/tffs0/file","/tffs0/file1");
	
	printf("flash copy file %d \n",(tickGet()-time)/60);
	#endif
		time = tickGet();

    if(OK != flashErasePart(FLASH_BASE_ADRS,uiflashAddr,uiLen/0x20000,0))
    {
        return ;
    }
	
	printf("flash erease file %d \n",(tickGet()-time)/60);
	time = tickGet();

    if(OK != flashProgram(FLASH_BASE_ADRS,uiflashAddr,uiAddr,uiLen,0))
    {
        return;
    }

	printf("flash write time %d \n",(tickGet()-time)/60);
	return OK;
}


