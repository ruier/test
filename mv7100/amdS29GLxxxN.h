/* amdS29GLxxxN.h - AMDS29GL512/256/128N Mirrorbit Flash Header File */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01a,27aug05,cak  Created.
*/

/*
DESCRIPTION

This file contains definitions for the AMDS29GL512/256/128N Mirrorbit
flash devices.
*/

#ifndef INCamdS29GLxxxNh
#define INCamdS29GLxxxNh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* defines */

#define FLASH_WIDTH 4   /* width of flash bank (bytes) */

/* Definitions for Flash Commands */

#define CMD_UNLOCK_DATA_1       (0x00AA00AA) 
#define CMD_UNLOCK_DATA_2       (0x00550055) 
#define CMD_READRESET           (0x00F000F0) 
#define CMD_AUTOSELECT          (0x00900090)
#define CMD_WORDPROGRAM         (0x00A000A0)
#define CMD_SECTORERASE1        (0x00800080)
#define CMD_SECTORERASE2        (0x00300030)
#define CMD_NVPROTECT_ENTRY     (0x00C000C0)
#define CMD_NVPROTECT_EXIT1     (0x00900090)
#define CMD_NVPROTECT_EXIT2     (0x00000000)
#define CMD_PPBPROGRAM1         (0x00A000A0)
#define CMD_PPBPROGRAM2         (0x00000000)
#define CMD_PPBERASE1           (0x00800080)
#define CMD_PPBERASE2           (0x00300030)

/* Definitions for Flash Command Addresses */

#define ADDR_UNLOCK_1           (FLASH_WIDTH * 0x555)
#define ADDR_UNLOCK_2           (FLASH_WIDTH * 0x2AA)
#define ADDR_AUTOSELECT         (FLASH_WIDTH * 0x555)
#define ADDR_RESET              (FLASH_WIDTH * 0x000)
#define ADDR_MAN_ID             (FLASH_WIDTH * 0x000)
#define ADDR_DEV_ID             (FLASH_WIDTH * 0x001)
#define ADDR_SEC_DEV_ID         (FLASH_WIDTH * 0x00E)
#define ADDR_PROGRAM            (FLASH_WIDTH * 0x555)
#define ADDR_NV_SECT_PROTECT_ENTRY      (FLASH_WIDTH * 0x555)
#define ADDR_NV_SECT_PROTECT_PPB_ERASE  (FLASH_WIDTH * 0x000)
#define ADDR_NV_SECT_PROTECT_PPB_PRGM   (FLASH_WIDTH * 0x000)
#define ADDR_NV_SECT_PROTECT_EXIT       (FLASH_WIDTH * 0x000)
#define ADDR_SECTOR_ERASE       (FLASH_WIDTH * 0x555)

/* Manufacturer and Device IDs for Mirrorbit Flash Devices */

#define FMID_MF_AMD             0x00010001  /* AMD manufacturer ID */
#define FMID_AMD_MIRROR_BIT     0x227E227E  /* AMD mirror bit dev ID*/
#define FMID_AMD_GL128          0x22212221  /* 128N secondary ID */
#define FMID_AMD_GL256          0x22222222  /* 256N secondary ID */
#define FMID_AMD_GL512          0x22232223  /* 512N secondary ID */

/* Sector Size Definitions */

#define AMD_SECTOR_SIZE_16BIT   0x00020000  /* 128 k */
#define AMD_GL128_NUM_SECTORS   128
#define AMD_GL256_NUM_SECTORS   256
#define AMD_GL512_NUM_SECTORS   512

BOOL flashNVSectorProtectPPBIsSet (UINT32 flashBaseAddr, UINT32 sectorSize,
                                             int numSectors);
void flashNVSectorProtectPPBErase (UINT32 flashBaseAddr);
void flashNVSectorProtectPPBProgram (UINT32 flashBaseAddr,
                                                 UINT32 sectorSize, int numSectors);
void flashIdGet (UINT32 flashBaseAddr, UINT32* manCode, UINT32* devCode,
                   UINT32* secDevCode);
void flashInfoGet (UINT32 flashBaseAddr, UINT32 * sectorSize,
                      int * numSectors);
STATUS flashWordProgram (UINT32 flashBaseAddr, UINT32 addr, UINT32 data);
STATUS flashSectorProgram (UINT32 flashBaseAddr, int sectorNum,
                                 UINT32 * pData, UINT32 sectorSize,int numSectors);
STATUS flashUtilSectorProgram (UINT32 flashBaseAddr, UINT32 sectorAddr,
                                      char * pData, int length, UINT32 sectorSize,
                                      int numSectors);
STATUS flashSectorErase (UINT32 flashBaseAddr, int sectorNum,
                              UINT32 sectorSize, int numSectors);
STATUS flashUtilSectorErase (UINT32 flashBaseAddr, UINT32 eraseAddr,
                                  UINT32 sectorSize, int numSectors);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCamdS29GLxxxNh */
