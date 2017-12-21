#ifndef __FS_LIB_H__
#define __FS_LIB_H__

#include "vxWorks.h"

#define FS_EOS		       '\0'
#define FS_SLASH           '/'
#define FS_BACK_SLASH      '\\'
#define FS_DOT             '.'
#define FS_SPACE           ' '
#define FS_MAX_DIR_LEVELS  8     /* max expected directory levels */

#define FS_ROOT_DIR            "/"

#define FS_CACHE_WR_THROUGH    0
#define FS_CACHE_WR_BACK       1

#define FS_RESV_CLU_NUM        2
#define FS_HASH_TABLE_SIZE     0x6900
/*
 *split path
*/
typedef struct tagFS_PATH_ARRAY_S
{
    UINT8    aucName[FS_MAX_NAME_LEN];
} FS_PATH_ARRAY_S;

#define UINT64_SET(a,b) \
     do \
     { \
        a.lo = b.lo; \
        a.hi = b.hi; \
     }while(0)
     
#define FS_CLUST_INVALID       0xffffffff
#define FS_CLUST_BAD           0x0ffffff7
#define FS_CLUST_RESV          0x0ffffff0
#define FS_CLUST_EOF           0x0fffffff
#define FS_CLUST_AVAIL         0


/*
 *indicate the attribute of file
*/
#define FS_ATTR_RDONLY         0x01            /* read-only file */
#define FS_ATTR_HIDDEN         0x02            /* hidden file */
#define FS_ATTR_SYSTEM         0x04            /* system file */
#define FS_ATTR_VOL_LABEL      0x08            /* volume label (not a file) */
#define FS_ATTR_DIRECTORY      0x10            /* entry is a sub-directory */
#define FS_ATTR_ARCHIVE        0x20            /* file subject to archiving */ 

/*
 *indicate the file system status of FDT
*/
#define FS_FILE_STAT_EMPTY     0x00
#define FS_FILE_STAT_DELETE    0xe5
#define FS_FILE_STAT_SUBDIR    0x2e  

#define FS_VOL_USED            0x55
#define FS_VOL_FREE            0

#define FS_BYTE_PER_FAT_ENTRY  4

#define FS_OPER_READ           0x1
#define FS_OPER_WRITE          0x2
#define FS_OPER_TRIM           0x3
#define FS_OPER_NULL           0x4

#define FS_TRIM_SUPPORT        
/*jingzhong 2016-6-4 Ê±Ê±Ë¢ÐÂFDT*/

#define FS_UPDATE_FDT_REALTIME          

#define FS_REQ_FREE            0
#define FS_REQ_USED            1

#define FS_MAX_REQ_PER_VOL     8

extern VOID UINT64_SUB32(UINT64_S *pullRst,UINT64_S *pullSub1,UINT32 uiSub);
extern VOID UINT64_ADD(UINT64_S *pullRst,UINT64_S *pullAdd1,UINT32 uiAdd);
extern STATUS fsGetDirName(UINT8  *vpcFileName,FS_PATH_ARRAY_S *vpstDirName);
extern STATUS UINT64_ISZERO(UINT64_S *pullValA);
extern VOID UINT64_SUB64(UINT64_S *,UINT64_S *,UINT64_S *);

#endif

