#ifndef _fflashIOh
#define _fflashIOh

#include "vxWorksCommon.h"

typedef int   STATUS;

extern void vxbMsDelay
    (
    int	delayTime
    );
int sysGetCpuID(void);

STATUS getFlashWLock();
STATUS freeFlashLock();

FILE * ffopen
    (
    const char * file,   /* name of file */
    const char * mode    /* mode */
    );
int ffclose
    (
    FAST FILE * fp   /* stream to close */
    );
int ffwrite
    (
    const void * buf,     /* where to copy from */
    size_t       size,    /* element size */
    size_t       count,   /* no. of elements */
    FILE *       fp       /* stream to write to */
    ) ;
int ffread
    (
    void *      buf,     /* where to copy data */
    size_t      size,    /* element size */
    size_t      count,   /* no. of elements */
    FAST FILE * fp       /* stream to read from */
    ) ;

int	ffeof (FILE * fp);

int	ffseek (FILE *fp,       /* stream */
			long offset,	/* offset from <whence> */
			int whence		/* position to offset from: */
							/* SEEK_SET = beginning */
							/* SEEK_CUR = current position */
							/* SEEK_END = end-of-file */
			);
int ffscanf
    (
    FILE *	  fp,	/* stream to read from */
    char const *  fmt,	/* format string */
    ...			/* arguments to format string */
    );
int	ffprintf ( 
		FILE *	  fp,	/* stream to write to */
	    const char *  fmt,	/* format string */
	    ...			/* optional arguments to format string */
	    );
FILE * ffdopen
    (
    int          fd,    /* file descriptor */
    const char * mode   /* mode to open with */
    );
#endif
