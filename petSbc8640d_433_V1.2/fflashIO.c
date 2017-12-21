
#include "fflashIO.h"

void flashIoInit()
{
	flashIoSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
}

STATUS ffcopy (const char *in, const char *out)
{
	int status;
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
	status=copy (in,out);
	freeFlashLock();
	semGive(flashIoSem);
	return status;
}

/*
 * see also fopen();
 */
FILE * ffopen
    (
    const char * file,   /* name of file */
    const char * mode    /* mode */
    )
{
	FILE * fp;
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
	fp=fopen (file,mode);
	freeFlashLock();
	semGive(flashIoSem);
	return fp;
}


/*
 * see also fclose();
 */
int ffclose
    (
    FAST FILE * fp   /* stream to close */
    ) 
{
	int status;
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
	status=fclose (fp);
	freeFlashLock();
	semGive(flashIoSem);
	return status;
}

/*
 * see also fwrite();
 */
int ffwrite
    (
    const void * buf,     /* where to copy from */
    size_t       size,    /* element size */
    size_t       count,   /* no. of elements */
    FILE *       fp       /* stream to write to */
    ) 
{
	int len;
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
	len=fwrite (buf,size,count,fp);
	freeFlashLock();
	semGive(flashIoSem);
	return len;
}

/*
 * see also fread();
 */
int ffread
    (
    void *      buf,     /* where to copy data */
    size_t      size,    /* element size */
    size_t      count,   /* no. of elements */
    FAST FILE * fp       /* stream to read from */
    ) 
{
	int len;
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
	len=fread (buf,size,count,fp);
	freeFlashLock();
	semGive(flashIoSem);
	return len;
}

/*
 * see also feof();
 */

int	ffeof (FILE * fp)
{
	int value;
	semTake (flashIoSem,WAIT_FOREVER);
	while( getFlashWLock() != OK);
	value=feof (fp);
	freeFlashLock();
	semGive(flashIoSem);
	return value;
	
}

/*
 * see also ffseek()
 */
int	ffseek (FILE *fp,       /* stream */
			long offset,	/* offset from <whence> */
			int whence		/* position to offset from: */
							/* SEEK_SET = beginning */
							/* SEEK_CUR = current position */
							/* SEEK_END = end-of-file */
			)
{
	int value;
	semTake (flashIoSem,WAIT_FOREVER);
	while( getFlashWLock() != OK);
	value = fseek(fp,offset,whence);
	freeFlashLock();
	semGive(flashIoSem);
	return value;
}
/*
 * see also fprintf();
 */
int	ffprintf ( 
		FILE *	  fp,	/* stream to write to */
	    const char *  fmt,	/* format string */
	    ...			/* optional arguments to format string */
	    )
{
    int		  ret;
    va_list	  vaList;
	
	semTake (flashIoSem,WAIT_FOREVER);
	 while( getFlashWLock() != OK);
    if (HANDLE_VERIFY (fp, handleTypeFile) != OK)
    {
    	freeFlashLock();
    	semGive(flashIoSem);
    	return (ERROR);
    }
    va_start (vaList, fmt);
    ret = vfprintf (fp, fmt, vaList);
    va_end (vaList);
    
	freeFlashLock();
	semGive(flashIoSem);
	
    return (ret);
}

/*
 * see also fscanf();
 */
int ffscanf
    (
    FILE *	  fp,	/* stream to read from */
    char const *  fmt,	/* format string */
    ...			/* arguments to format string */
    )
    {
    int     nArgs;
    int     unget;
    va_list vaList;	/* vararg list */
    
    semTake (flashIoSem,WAIT_FOREVER);
    while( getFlashWLock() != OK);
    if (HANDLE_VERIFY (fp, handleTypeFile) != OK)
    {
    	freeFlashLock();
    	semGive(flashIoSem);	
    	return (EOF);
    }

    va_start (vaList, fmt);
    nArgs = fioScanV (fmt, fgetc, (int) fp, &unget, vaList);
    va_end (vaList);
    
    if (unget != -1)
	ungetc (unget, fp);
    
	freeFlashLock();
	semGive(flashIoSem);
	
    return (nArgs);
    }

/*
 * see also fdopen();
 */
FILE * ffdopen
    (
    int          fd,    /* file descriptor */
    const char * mode   /* mode to open with */
    )
{
	FILE * fp;
	semTake (flashIoSem,WAIT_FOREVER);
	while( getFlashWLock() != OK);	
	fp = fdopen(fd,mode);
	freeFlashLock();
	semGive(flashIoSem);
	return fp;
}


