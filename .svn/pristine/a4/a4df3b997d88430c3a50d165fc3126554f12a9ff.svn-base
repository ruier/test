/* usrFsLib.c - file system user interface subroutine library */

/*
 * Copyright 1998-2008, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03f,05aug09,zly  Fixed WIND117728: update comments of copy().
03e,10oct08,hui  Fixed WIND117220: File is truncated to zero when source and
                 destination are identical in copy()
03d,31aug07,dcc  Fixed WIND00093542: "xdelete causes endless loop when trying 
                 to delete read only files in subfolders"
03c,03may07,pcm  Fixed WIND00091176
03b,03oct06,jlk  Fix WIND00067558
03a,29jun06,jlk  added commit()
02z,03apr06,dgp  doc: fix list markup chkdsk(), example wrap xattrib(),
		 prog guide ref in library, all routines.
02y,06mar06,pcm  checked fstat() return value in rmdir() (SPR 118510)
02x,02mar06,pcm  initialized <status> variable in recursiveXDelete()
02w,28feb06,pcm  added O_TRUNC flag to copy() (SPR 118051)
02v,23jan06,dlk  Decrease excessive stack usage in dirList(), cp(), and mv().
02u,06feb06,jlk  Updated for new 64 bit st_size field
02t,29nov05,md   add new hrfs formatting commands
02s,15oct05,mwv  SPR#111708 change mkdir default permission in kernel
02r,22aug05,hya  made user side mkdir POSIX compliant.
02q,26jul05,rp   minor fixes for xcopy and cp
02p,19aug05,pcm  changed calls from creat() to open()
02o,04aug05,jlk  removed inclusion of dosFs and HRFS private header files.
02n,22jul05,pcm  updated comment for chkdsk() as per SPR 77985
02m,18jul05,jlk  removed dependency on file system formatters
02l,04jul05,jlk  fixed RTP builds.
02k,29jun05,jlk  clean-up. Added dosfsDiskFormat() and hrfsDiskFormat.
                 Deprecated diskFormat()
02j,11may05,act  SPR#102311: xdelete() runs out of descriptors
02i,02may05,pcm  use current timestamp when copying from STD_IN to a file
02h,29apr05,pcm  unless ioctl(FIORMDIR) is unsupported, rmdir() is to use it
                 instead of remove() (SPR 70673)
02g,13jan05,pcm  forced <dSize> to 1 if copying from STD_IN in copyStreams()
		 (SPR# 65396)
02f,16dec04,pcm  improved pattern matching in dirListPattern() (SPR 78290);
                 brought file up to coding standards
02e,15dec04,jlk  Fixed SPR 102679
02d,16feb05,vvv  include netDrv.h only in kernel
02c,15nov04,tam  fixed typo in dirList definition
02b,15nov04,bpn  Updated documentation.
02a,03nov04,pai  Fixed misplaced doc markup and comment format
01z,26aug04,feg  Coverity code coverage bugs #621 of 2004-06-30 and #397 of
                 2004-07-16
01y,26aug04,feg  Coverity code coverage bug #653 of 2004-06-30
01x,29jun04,dlk  Remove include of nfsLib.h.
01w,17may04,mcm  Fixing compiler warnings
01v,01may04,jkf  SPR#89277, reduce copyStreams() stack usage
01u,07apr04,dat  xdelete fix (95517), merge to RTP space (95003)
01t,02mar04,dat  fix to copyStreams, SPR 94082
01s,31jan03,jkf  SPR#85305, chkdsk mounts RDONLY if not DOS_CHK_REPAIR
01r,16jan02,chn  SPR#24332, add check to avoid copy() to a directory name dest
01q,12dec01,jkf  SPR#72133, add FIOMOVE to dosFsLib, use it in mv()
01p,17oct01,jkf  SPR#74904, function cleanup.
01o,20sep01,jkf  SPR#69031, common code for both AE & 5.x.
01n,15mar01,jkf  SPR#33973, copy not preserving time/date.
01m,15mar01,jkf  SPR#33557, ls returning error on first invocation
01l,29aug00,jgn  add const to function signatures (SPR #30492) +
		 clean up coding standard violations
01k,17mar00,dgp  modify attrib and xattrib synopsis comments for syngen
                 processing
01j,16mar00,jkf  removed reference to dosFsVolFormat() from diskInit().
01i,26dec99,jkf  T3 KHEAP_ALLOC
01h,31jul99,jkf  T2 merge, tidiness & spelling.
01g,07dec98,lrn  fixed netDrv spurious error message (SPR#22554)
01f,23nov98,vld  print '/' after directory name in long output
01e,13jul98,lrn  fixed diskInit() to call dosFsVolFormat() if open() fails
01d,02jul98,lrn  corrected parameters for chkdsk()
01c,28jun98,lrn  added recursive xcopy, xdelete, added wildcards to
                 cp(), mv(), added attrib and xattrib(), added help
01b,23jun98,lrn  rewrite ls, add more utils
01a,19feb98,vld	 initial version, derived from usrLib.c
*/

/*
DESCRIPTION
This library provides user-level utilities for managing file systems.
These utilities may be used from Host Shell, the Kernel Shell or from
an application.

USAGE FROM HOST SHELL
Some of the functions in this library have counterparts of the same
names built into the Host Shell (aka Windsh). The built-in functions
perform similar functions on the Tornado host computer's I/O systems.
Hence if one of such functions needs to be executed in order to perform
any operation on the Target's I/O system, it must be preceded with an
'@' sign, e.g.:
\cs
-> @ls "/sd0"
\ce
will list the directory of a disk named "/sd0" on the target, wile
\cs
-> ls "/tmp"
\ce
will list the contents of the "/tmp" directory on the host.

The target I/O system and the Host Shell running on the host, each
have their own notion of current directory, which are not related,
hence
\cs
-> pwd
\ce
will display the Host Shell current directory on the host file
system, while
\cs
-> @pwd
\ce
will display the target's current directory on the target's console.

WILDCARDS
Some of the functions herein support wildcard characters in argument
strings where file or directory names are expected. The wildcards are
limited to "*" which matches zero or more characters and "?" which
matches any single characters. Files or directories with names beginning
with a "." are not normally matched with the "*" wildcard.

DIRECTORY LISTING
Directory listing is implemented in one function dirList(), which can be
accessed using one of these four front-end functions:

\is
\i ls()
produces a short list of files
\i lsr()
is like ls() but ascends into subdirectories
\i ll()
produces a detailed list of files, with file size, modification date
attributes etc.
\i llr()
is like ll() but also ascends into subdirectories
\ie

All of the directory listing functions accept a name of a directory or a
single file to list, or a name which contain wildcards, which will
result in listing of all objects that match the wildcard string
provided.

INCLUDE FILES
usrLib.h

SEE ALSO
ioLib, dosFsLib, netDrv, nfsLib, hrFsLib, 
the VxWorks programmer guides, the
\tb VxWorks Command-Line Tools User's Guide.
*/

/* includes */

#include <vxWorks.h>
#include <fioLib.h>
#include <ctype.h>
#include <stdio.h>
#include <ioLib.h>
#include <memLib.h>
#include <string.h>
#include <ftpLib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errnoLib.h>
#include <taskLib.h>
#include <time.h>
#include <sys/stat.h>
#include <utime.h>
#include <dosFsLib.h>
#include <stdlib.h>
#include <usrFsLib.h>

#ifdef _WRS_KERNEL
#   include <iosLib.h>
#   include <string.h>
#   include <netDrv.h>
#   include <hrFsLib.h>
#else
#   include <strings.h>
#endif

/* types */

typedef enum
    {
    USR_FS_FILE = 0,
    USR_FS_DIR,
    USR_FS_WILD,
    USR_FS_NO_EXIST
    } USR_FS_TYPE;

    
    UINT32 fileLen=0;
/* defines */

#define	WILD_CHARACTER		'?'
#define	WILD_STAR		'*'

#define	CURRENT_DIRECTORY	"./"

#define	COPY_BLOCK_SIZE	1024

#define DEFAULT_DIR_PERM_KERNEL 	0000777 /* default directory permissions */

/* locals */

LOCAL const  char * monthNames[] =
    {
    "???", "Jan", "Feb", "Mar", "Apr",
    "May", "Jun", "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec"
    };

/* forward declarations */

LOCAL BOOL dirNameWildcard (const char * pName);
LOCAL BOOL nameIsDir (const char * pName);
LOCAL int mvFile (const char * oldname, const char * newname);
LOCAL STATUS recursiveXDelete (char *path, char *pattern);

/***************************************************************************
*
* usrFsPathQuery - obtain information about pathname and filename
*
* This routine obtains information about the pathname and filename.
* It will determine if the pathname contains wildcards, whether
* the object it points to exists, is a file or a directory.
*
* RETURNS: USR_FS_FILE, USR_FS_DIR, USR_FS_WILD, USR_FS_NO_EXIST
*
* NOMANUAL
*/

LOCAL USR_FS_TYPE  usrFsPathQuery
    (
    const char *  path
    )
    {
    struct stat  attr;

    if (dirNameWildcard (path))    /* Test for wild characters */
        return (USR_FS_WILD);

    if (stat (path, &attr) != OK)  /* Test for object's existence */
        return (USR_FS_NO_EXIST);

    if (S_ISDIR (attr.st_mode))
        return (USR_FS_DIR);

    return (USR_FS_FILE);
    }

/***************************************************************************
*
* usrPathCat - concatenate directory path to filename.
*
* This routine constructs a valid filename from a directory path
* and a filename.
* The resultant path name is put into <result>.
* No arcane rules are used to derive the concatenated result.
*
* RETURNS: N/A.
*
* NOMANUAL
*/

LOCAL void usrPathCat
    (
    const char *	dirName,	/* directory path */
    const char *	fileName,	/* file name portion */
    char *		result		/* <dirName> + <fileName> */
    )
    {
    int		len = 0;	/* # of bytes copied to <result> */

    *result = EOS ;

    if ((dirName != NULL) && (*dirName != EOS) &&
	(strcmp (dirName, CURRENT_DIRECTORY) != 0))
	len = sprintf (result, "%s/", dirName);

    strcpy (result + len, fileName);
    } /* usrPathCat() */

/***************************************************************************
*
* usrPathSplit - split the path into directory and file name
*
* This routine splits a string into a directory name and a file name components.
* It should be noted that <path> may be modified.
*
* RETURNS: OK
*
* NOMANUAL
*/

LOCAL STATUS usrPathSplit
    (
    char *	pPath,		/* path to split into directory & file name */
    char **	pDirName,	/* ptr to ptr of directory portion */
    char **	pFileName	/* ptr to ptr of filename portion */
    )
    {
    char *	pLastSlash;	/* points to last '/' in <pPath> */
    USR_FS_TYPE usrFsType;

    pLastSlash = rindex (pPath, '/');	/* Find the last '/'. */

    if (pLastSlash == pPath)	/* The last '/' is the first character in  */
	{			/* in the string.  It is assumed that this */
	*pDirName = pPath;	/* is a device's top-level name.           */
	*pFileName = NULL;	/*  (eg. /ata0)                            */
	return (OK);
	}

    usrFsType = usrFsPathQuery (pPath);

    if (pLastSlash == NULL)		/* No '/' was found.  Is   */
	{				/* it a file or directory? */
	if (usrFsType == USR_FS_DIR)
	    {				/* It's a directory. */
	    *pDirName = pPath;
	    *pFileName = NULL;
	    }
	else
	    {				/* It's a file. */
	    *pDirName = CURRENT_DIRECTORY;
	    *pFileName = pPath;
	    }
	return (OK);
	}

    /*
     * A '/' was found.  The path must be split into directory and file
     * name components.  If the default file name is a directory, use NULL as
     * the file name.  Otherwise, separate the two components by replacing the 
     * '/' with the EOS character.  If the '/' is the last character in the
     * string, it shall be assumed the path is a directory, and NULL will be
     * used for the file name.
     */

    *pDirName = pPath;			/* Default: pDirName = full path */
    *pFileName = (pLastSlash + 1);	/* Default file name. */

    if (**pFileName == EOS)
	*pFileName = NULL;		/* <pPath> ended with '/'. */
    else if (dirNameWildcard (*pFileName))
	*pLastSlash = EOS;		/* Assume a file, so replace '/'. */
    else if (!nameIsDir (pPath))
	*pLastSlash = EOS;		/* Assume a file, so replace '/'. */
    else
	*pFileName = NULL;		/* Assume a directory. */

    return (OK);
    }

/***************************************************************************
*
* usrPathSplit2 - split the path into directory and file name
*
* This routine splits a string into a directory name and a file name components.
* It should be noted that <path> may be modified.
*
* RETURNS: OK
*
* NOMANUAL
*/

LOCAL STATUS usrPathSplit2
    (
    char *      pPath,          /* path to split into directory & file name */
    char **     pDirName,       /* ptr to ptr of directory portion */
    char **     pFileName       /* ptr to ptr of filename portion */
    )
    {
    char *      pLastSlash;     /* points to last '/' in <pPath> */

    while (1)    /* Loop until all trailing slashes are removed */
        {
        pLastSlash = rindex (pPath, '/');

        if (pLastSlash == pPath) /* The last '/' is the first character in  */
            {                    /* in the string.  It is assumed that this */
            *pDirName = pPath;   /* is a device's top-level name.           */
            *pFileName = NULL;   /*  (eg. /ata0a)                           */
            return (OK);
            }

        if (pLastSlash == NULL)         /* No '/' was found. */
            {
            *pDirName = CURRENT_DIRECTORY;
            *pFileName = pPath;

            return (OK);
            }

        pLastSlash[0] = EOS;

        if (pLastSlash[1] != EOS)   /* Break out of loop if '/'  */
            break;                  /* is in the middle of path. */
        }

    *pDirName = pPath;                  /* Default: pDirName = full path */
    *pFileName = (pLastSlash + 1);      /* Default file name. */

    return (OK);
    }

/***************************************************************************
*
* cd - change the default directory
*
* This command sets the default directory to <name>.  The default directory
* is a device name, optionally followed by a directory local to that
* device.
*
* NOTE
* This is a target resident function, which manipulates the target I/O
* system. It must be preceded with the '@' letter if executed from the 
* Host Shell (windsh), which has a built-in command of the same name that
* operates on the Host's I/O system.
*
* To change to a different directory, specify one of the following:
* \ms
* \m -
* an entire path name with a device name, possibly followed by a directory
* name. The entire path name will be changed.
* \m -
* a directory name starting with a `~' or `/' or `$'.  The directory part
* of the path, immediately after the device name, will be replaced with the new
* directory name.
* \m -
* a directory name to be appended to the current default directory.
* The directory name will be appended to the current default directory.
* \me
*
* An instance of ".." indicates one level up in the directory tree.
*
* Note that when accessing a remote file system via RSH or FTP, the
* VxWorks network device must already have been created using
* netDevCreate().
*
* WARNING
* The cd() command does very little checking that <name> represents a valid
* path. If the path is invalid, cd() may return OK, but subsequent
* calls that depend on the default path will fail.
*
* EXAMPLES
* The following example changes the directory to device `/fd0/':
* \cs
*     -> cd "/fd0/"
* \ce
* This example changes the directory to device `wrs:' with the local
* directory `~leslie/target':
* \cs
*     -> cd "wrs:~leslie/target"
* \ce
* After the previous command, the following changes the directory to
* `wrs:~leslie/target/config':
* \cs
*     -> cd "config"
* \ce
* After the previous command, the following changes the directory to
* `wrs:~leslie/target/demo':
* \cs
*     -> cd "../demo"
* \ce
* After the previous command, the following changes the directory to
* `wrs:/etc'.
* \cs
*     -> cd "/etc"
* \ce
* Note that `~' can be used only on network devices (RSH or FTP).
*
* RETURNS: OK or ERROR.
*
* SEE ALSO
* pwd(), the VxWorks programmer guides, the
* \tb VxWorks Command-Line Tools User's Guide.
*/

STATUS cd
    (
    const char *	name		/* new directory name */
    )
    {
#ifdef _WRS_KERNEL
    if (ioDefPathCat ((char *)name) != OK)
	{
	printf ("cd: error = %#x.\n", errno);
	return (ERROR);
	}

    return (OK);
#else
    return chdir (name);
#endif
    }

/***************************************************************************
*
* dirNameWildcard - check if file or dir name contains wildcards
*
* RETURNS: TRUE if '*' or '?' are contained in the <name> string
*
* NOMANUAL
*/

LOCAL BOOL dirNameWildcard
    (
    const char * pName		/* string to search for wildcards */
    )
    {
    return ((index (pName, (int) WILD_STAR) != NULL) ||
	    (index (pName, (int) WILD_CHARACTER) != NULL));
    }

/***************************************************************************
*
* nameIsDir - check if <pName> is a directory
*
* RETURNS: TRUE if <pName> is an existing directory, FALSE otherwise
*
* NOMANUAL
*/

LOCAL BOOL nameIsDir
    (
    const char * pName		/* name of possible directory */
    )
    {
    struct stat fStat ;

    if ((pName == NULL) || (*pName == EOS))
	return (FALSE);

    /* if it does not exist, it can not be a directory */

    if (stat ((char *) pName, &fStat) == ERROR)
        {
	errno = OK;
    	return (FALSE);
        }

    return (S_ISDIR (fStat.st_mode));
    }


/***************************************************************************
*
* pwd - print the current default directory
*
* This command displays the current working device/directory.
*
* NOTE
* This is a target resident function, which manipulates the target I/O
* system. It must be preceded with the '@' letter if executed from the 
* Host Shell (windsh), which has a built-in command of the same name that
* operates on the Host's I/O system.
*
* RETURNS: N/A
*
* SEE ALSO
* cd(), the VxWorks programmer guides, the
* \tb VxWorks Command-Line Tools User's Guide.
*/

void pwd (void)
    {
    char name [MAX_FILENAME_LENGTH];

    ioDefPathGet (name);
    printf ("%s\n", name);
    }


/***************************************************************************
*
* mkdir - make a directory
*
* \IFSET_START USER
* SYNOPSIS
* \cs
* int mkdir
*     (
*     const char *	dirName,	/@ directory name @/
*     mode_t		mode            /@ mode of dir @/
*     )
* \ce
* \IFSET_END
* \IFSET_START KERNEL
* SYNOPSIS
* \cs
* STATUS mkdir
*        (
*        const char *	dirName		/@ directory name @/
*        )
* \ce
* \IFSET_END
*
* DESCRIPTION
* This command creates a new directory in a hierarchical file system.
* The <dirName> string specifies the name to be used for the
* new directory, and can be either a full or relative pathname.
* \IFSET_START USER
* <mode> sets the initial permission bits of the new directory.
* \IFSET_END
*
* This call is supported by the VxWorks NFS and dosFs file systems.
*
* RETURNS
* OK, or ERROR if the directory cannot be created.
*
* \IFSET_START KERNEL
* SEE ALSO
* rmdir(),
* \tb VxWorks Kernel Programmer's Guide: `Kernel Shell'
* \IFSET_END
*/

#ifdef _WRS_KERNEL
STATUS mkdir
    (
    const char *	dirName		/* directory name */
    )
    {
    int fd;

    fd = open (dirName, O_RDWR | O_CREAT, FSTAT_DIR | DEFAULT_DIR_PERM_KERNEL);

    if (fd == ERROR)
	return (ERROR);

    return (close (fd));
    }

#else

int mkdir
    (
    const char *	dirName,	/* directory name */
    mode_t		mode            /* mode of dir */
    )
    {
    int fd;

    fd = open (dirName, O_RDWR | O_CREAT, FSTAT_DIR | DEFAULT_DIR_PERM | mode);

    if (fd == ERROR)
	return (ERROR);

    return (close (fd));
    }

#endif

/***************************************************************************
*
* rmdir - remove a directory
*
* This command removes an existing directory from a hierarchical file
* system.  The <dirName> string specifies the name of the directory to
* be removed, and may be either a full or relative pathname.
*
* This call is supported by the VxWorks NFS and dosFs file systems.
*
* RETURNS: OK, or ERROR if the directory cannot be removed.
*
* SEE ALSO
* mkdir(), the VxWorks programmer guides. 
*/

STATUS rmdir
    (
    const char *	dirName		/* name of directory to remove */
    )
    {
    int           fd;
    int           error;
    STATUS        status;
    struct stat   stat;

    if ((fd = open (dirName, O_RDONLY, 0)) == ERROR)
        return (ERROR);

    if (fstat (fd, &stat) != OK)        /* Obtain file stats on <dirName>. */
        {
        error = errnoGet ();
        close (fd);
        errnoSet (error);
        return (ERROR);
        }

    if (!S_ISDIR (stat.st_mode))        /* If it is not a directory, then  */
        {                               /* set <errno> and return ERROR.   */
        close (fd);
        errnoSet (ENOTDIR);
        return (ERROR);
        }

    status = ioctl (fd, FIORMDIR, (int) dirName);
    error = errnoGet ();
    close (fd);

    if ((status != OK) && ((error == ENOTSUP) || (error == ENOSYS)))
        return (remove (dirName));

    return (status);
    }

/***************************************************************************
*
* rm - remove a file
*
* This command is provided for UNIX similarity. It simply calls remove().
*
* RETURNS: OK, or ERROR if the file cannot be removed.
*
* SEE ALSO:
* remove(), the VxWorks programmer guides.
*/

STATUS rm
    (
    const char *	fileName	/* name of file to remove */
    )
    {
    return (remove (fileName));
    }

/***************************************************************************
*
* copyStreams - copy from/to specified streams
*
* This command copies from the stream identified by <inFd> to the stream
* identified by <outFd> until an end of file is reached in <inFd>.
* This command is used by copy().
*
* INTERNAL
* The size of an array buffer can have a dramatic impact on the throughput
* achieved by the copyStreams() routine.  The default is 1 Kbyte, but this
* can be increased as long as the calling task (usually the VxWorks shell)
* has ample stack space.  Alternately, copyStreams() can be modified to use a
* static buffer, as long as the routine is guaranteed not to be called in
* the context of more than one task simultaneously.
*
* RETURNS: OK, or ERROR if there is an error reading from <inFd> or writing
* to <outFd>.
*
* SEE ALSO
* copy(), the VxWorks programmer guides. 
*
* INTERNAL
* Note use of printErr since printf's would probably go to the output stream
* outFd!
*/

STATUS copyStreams
    (
    int inFd,		/* file descriptor of stream to copy from */
    int outFd 		/* file descriptor of stream to copy to */
    )
    {	
	char * pbuffer;	
    int		totalBytes = 0;
    int		nbytes;
    int 	i=0;
    float   filePer;
    size_t	dSize = COPY_BLOCK_SIZE;

    if (inFd == STD_IN)
	dSize = 1;
    else if (ioctl (inFd, FIONREAD, (int) &dSize) == ERROR)
    	dSize = COPY_BLOCK_SIZE;
    else
    	dSize = min (COPY_BLOCK_SIZE, dSize);

    pbuffer = malloc (dSize);

    if (pbuffer == NULL)
    	return (ERROR);

    /* transferring buffer */
    while ((nbytes = fioRead (inFd, pbuffer, dSize)) > 0)
	{
	if (write (outFd, pbuffer, nbytes) != nbytes)
	    {
	    printErr ("copy: error writing file. errno %p\n", (void *) errno);
     	    free (pbuffer);
	    return (ERROR);
	    }
	i++;
	totalBytes += nbytes;	
	if((i%100)==0)
	{
		filePer = (totalBytes*100)/fileLen;
		printf("copy completed %.1f%%  \r",filePer,2,3,4,5,6);		
	}	
    }
    printf("\n\n");
    fileLen = 0;
    free (pbuffer);

    if (nbytes < 0)
	{
	printErr ("copy: error reading file after copying %d bytes.\n",
		  totalBytes);
	return (ERROR);
	}

    return (OK);
    }

/***************************************************************************
*
* copy - copy <in> (or stdin) to <out> (or stdout)
*
* This command copies from the input file to the output file, until an
* end-of-file is reached.
*
* EXAMPLES
* The following example displays the file `dog', found on the default file
* device:
* \cs
*     -> copy <dog
* \ce
* This example copies from the console to the file `dog', on device `/ct0/',
* until an EOF (default ^D) is typed:
* \cs
*     -> copy >/ct0/dog
* \ce
* This example copies the file `dog', found on the default file device, to
* device `/ct0/':
* \cs
*     -> copy <dog >/ct0/dog
* \ce
* This example makes a conventional copy from the file named `file1' to the file
* named `file2':
* \cs
*     -> copy "file1", "file2"
* \ce
* Copy only simply check if the in file path is the same with the out file path,
* so the user should advoid to give the same file with the different file path.
*
* RETURNS: OK, or ERROR if <in> or <out> cannot be opened/created, or if 
* there is an error copying from <in> to <out>.
*
* SEE ALSO
* copyStreams(), tyEOFSet(), cp(), xcopy(), the VxWorks programmer guides.
*/

STATUS copy
    (
    const char * in,	/* name of file to read  (if NULL assume stdin)  */
    const char * out 	/* name of file to write (if NULL assume stdout) */
    )
    {
    int		   inFd = ERROR;
    int		   outFd = ERROR;
    int		   status;
    int            ioctlRet;
    struct stat    fileStat; /* structure to fill with data */
    struct stat	   fileInfo;
    struct utimbuf fileTime;

    /* open input file */

    inFd = (in != NULL) ? open (in, O_RDONLY, 0) : STD_IN;
	
//	printErr ("copy: 11111\n");

    if (inFd == ERROR)
	{
	printErr ("Can't open input file \"%s\" errno = %p\n",
		  in, (void *) errno );
	return (ERROR);
	}
    
//	printErr ("copy: 22222\n");

    if (stat (in, &fileInfo) != OK)
    {
    	printErr("get file info failed\n");
    }
    else
    {
    	fileLen = fileInfo.st_size;
    }

//	printErr ("copy: 33333\n");	

    if (nameIsDir (out))			/* If this is a directory, */
    {					/* then we can not write   */
        errnoSet (S_ioLib_CANT_OVERWRITE_DIR);	/* to it!                  */
        close (inFd);
        return (ERROR);
    }
	/* check if input file and output file are the same file */

	if ((in != NULL && out != NULL) && (0 == strcmp (in, out)))
	{
		printErr ("copy: '%s' and '%s' are the same file\n", in, out);
		errnoSet (EINVAL);
		return(ERROR);
	}
	rm(out);
    /* create output file */
    outFd = (out != NULL) ?
            open (out, O_WRONLY | O_CREAT | O_TRUNC, 0777) :
            STD_OUT;
    if (outFd == ERROR)
	{
	printErr ("Can't write to \"%s\", errno %p\n", out, (void *) errno);

    	if (in != NULL)
    	    close (inFd);
	return (ERROR);
	}
   	
    /* copy data */
//	printErr ("copy: 88888\n");
	

    status = copyStreams (inFd, outFd);

//   	printf ("copyStreams time spend is %.6f s\n\r",timeSpend);
//	printErr ("copy: 99999\n");
    ioctlRet = ioctl (inFd, FIOFSTATGET, (int) &fileStat);

    /*
     * If ioctl(FIOFSTATGET) returns ERROR, it is assumed that the operation
     * is not supported (such as for STD_IN).  In such cases, the various
     * time fields are manually set rather than grabbing their values from
     * the source.
     */

    if (ioctlRet != OK)
        {
        fileTime.actime = time (NULL);
        fileTime.modtime = fileTime.actime;
        }
    else
        {
        fileTime.actime = fileStat.st_atime;
        fileTime.modtime = fileStat.st_mtime;
        }

    ioctl (outFd, FIOTIMESET, (int) &fileTime);

    if (in != NULL)
	close (inFd);

    /* close could cause buffers flushing */

    if ((out != NULL) && (close (outFd) == ERROR))
	status = ERROR;

    return (status);
    }

/***************************************************************************
*
* chkdsk - perform consistency checking on a MS-DOS file system
*
* This function invokes the integral consistency checking built into the
* dosFsLib file system, via FIOCHKDSK ioctl.  During the test, the volume will
* be un-mounted and re-mounted, invalidating file descriptors to prevent any
* application code from accessing the volume during the test.  If the drive was
* exported, it will need to be re-exported again as its file descriptors were
* also invalidated.  Furthermore, the test will emit messages describing any
* inconsistencies found on the disk, as well as some statistics, depending upon
* the value of the <verbose> argument.
* Depending upon the value of <repairLevel>, the inconsistencies will be
* repaired, and changes written to disk.
*
* These are the values for <repairLevel>:
* \is
* \i 0
* Same as DOS_CHK_ONLY (1)
* \i "DOS_CHK_ONLY (1)"
* Only report errors, do not modify disk.
* \i "DOS_CHK_REPAIR (2)"
* Repair any errors found.
* \ie
*
* These are the values for <verbose>:
* \is
* \i 0
* similar to DOS_CHK_VERB_1
* \i "DOS_CHK_VERB_SILENT (0xff00)"
* Do not emit any messages, except errors encountered.
* \i "DOS_CHK_VERB_1 (0x0100)"
* Display some volume statistics when done testing, as well
* as errors encountered during the test.
* \i "DOS_CHK_VERB_2 (0x0200)"
* In addition to the above option, display path of every file, while it
* is being checked. This option may significantly slow down
* the test process.
* \ie
*
* Note that the consistency check procedure will <unmount>
* the file system, meaning the all currently open file descriptors will
* be deemed unusable.
*
* RETURNS: OK or ERROR if device can not be checked or could not be repaired.
*
* SEE ALSO
* dosFsLib, the VxWorks programmer guides.
*/

STATUS chkdsk
    (
    const char *    pDevName,	    /* device name */
    u_int	    repairLevel,    /* how to fix errors */
    u_int	    verbose	    /* verbosity level */
    )
    {
    int 	fd;
    STATUS      retStat;

    repairLevel &= 0x0f;

    fd = open (pDevName, (repairLevel < DOS_CHK_REPAIR) ? O_RDONLY : O_RDWR, 0);

    if (ERROR == fd)
        {
        perror (pDevName);
        return (ERROR);
        }

    if ((verbose & 0xff00) != 0)
    	verbose	 &= 0xff00;
    else if ((verbose & 0xff) != 0)
    	verbose	 <<= 8;
    else	/* default is 1 */
    	verbose	 = 0x0100;
  
    retStat = ioctl (fd, FIOCHKDSK, repairLevel | verbose);

    close (fd);

    return (retStat);
    } /* chkDsk() */


/***************************************************************************
*
* dirListPattern - match file name pattern with an actual file name
*
* This routine matches tests whether <nameString> filename matches the pattern
* specified in <patternString> (which can use the '?' and '*' wildcard
* characters).  Both strings must be NULL terminated.
*
* RETURNS: TRUE on a match, FALSE otherwise
*
* NOTE: we're using EOS as integral part of the pattern and name.
*
* NOMANUAL
*/

LOCAL BOOL dirListPattern
    (
    char *      patternString,          /* string containing pattern */
    char *      nameString,             /* string to match to pattern */
    int		level                   /* recursion level */
    )
    {
    char *      pPattern;       /* pointer to location in <patternString> */
    char *      pNext;          /* pointer to next non-WILD_STAR character */
    char *      pChar;          /* point to location in <nameString> */

    pPattern = patternString;
    pChar = nameString;

    while (*pChar != EOS)
        {
        if (*pPattern == WILD_CHARACTER)
            {
            pPattern++;                         /* If a WILD_CHARACTER, */
            pChar++;                            /* accept any character. */
            }
        else if (*pPattern == WILD_STAR)
            {

            /*
             * In the event that there is a series of WILD_STAR's, they will
             * be treated as only one WILD_STAR.  <pNext> will point to the
             * next non-WILD_STAR symbol.
             */

            for (pNext = pPattern; *pNext == WILD_STAR; pNext++)
                ;

            while (*pChar != EOS)
                {

		/*
		 * To maintain a certain degreee of backwards compatibility,
		 * this algorithm, like its predecessor, will return FALSE if
		 * the WILD_STAR is used on the first character of top-level
		 * strings beginning with the '.' character.
		 */

		if ((level == 0) && (*pChar == '.') && (pChar == nameString))
		    return (FALSE);

                /*
                 * A recursive search must be used to determine if there is
                 * a match when a WILD_STAR is encountered.
                 */

                if (dirListPattern (pNext, pChar, level + 1))
                    return (TRUE);
                pChar++;
                }
            }
        else if (toupper ((int) *pPattern) != toupper ((int) *pChar))
            return (FALSE);
        else
            {
            pPattern++;
            pChar++;
            }
        }

    /* At this point, pChar points to the end of <nameString>. */

    while (*pPattern == WILD_STAR)      /* Ignore trailing WILD_STAR's */
        pPattern++;

    return (*pPattern == EOS);
    }


/*******************************************************************************
*
* dirListEnt - list one file or directory
*
* List one particular file or directory entry.
*
* NOMANUAL
*/

LOCAL STATUS dirListEnt
    (
    int		  fd,		/* file descriptor for output */
    const char *  fileName,	/* file name */
    struct stat * fileStat,	/* stat() structure */
    const char *  prefix,	/* prefix for short output */
    const char *  fullPath,	/* full path to file */
    BOOL	  doLong	/* do Long listing format */
    )
    {
    time_t		now;		/* current clock */
    struct tm		nowDate;	/* current date & time */
    struct tm		fileDate;	/* file date/time (long listing) */
    char		fType;
    char		modbits[10];
    char		suffix = ' ';	/* print '/' after directory name */
/*    int			fd2;
    int			result;
*/
    if (doLong)
	{
	/* Break down file modified time */
	time (&now);
#ifdef _WRS_KERNEL
	localtime_r (&now, &nowDate);
	localtime_r (&fileStat->st_mtime, &fileDate);
#else
	nowDate = *(localtime (&now));			/* struct assign */
	fileDate = *(localtime ((time_t *) &fileStat->st_mtime));
#endif

	if (fileStat->st_attrib & DOS_ATTR_RDONLY)
	    fileStat->st_mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);

	if (S_ISDIR (fileStat->st_mode))
    	    {
	    fType = 'd' ;
    	    suffix = '/';
    	    }
	else if (S_ISREG (fileStat->st_mode))
	    fType = '-' ;
	else
	    fType = '?' ;

	modbits[0] = (fileStat->st_mode & S_IRUSR) ? 'r' : '-';
	modbits[1] = (fileStat->st_mode & S_IWUSR) ? 'w' : '-';
	modbits[2] = (fileStat->st_mode & S_IXUSR) ? 'x' : '-';
	modbits[3] = (fileStat->st_mode & S_IRGRP) ? 'r' : '-';
	modbits[4] = (fileStat->st_mode & S_IWGRP) ? 'w' : '-';
	modbits[5] = (fileStat->st_mode & S_IXGRP) ? 'x' : '-';
	modbits[6] = (fileStat->st_mode & S_IROTH) ? 'r' : '-';
	modbits[7] = (fileStat->st_mode & S_IWOTH) ? 'w' : '-';
	modbits[8] = (fileStat->st_mode & S_IXOTH) ? 'x' : '-';

	if (fileStat->st_attrib & DOS_ATTR_ARCHIVE)
	    modbits[6] = 'A';
	if(fileStat->st_attrib & DOS_ATTR_SYSTEM)
	    modbits[7] = 'S';
	if(fileStat->st_attrib & DOS_ATTR_HIDDEN)
	    modbits[8] = 'H';

	modbits[9] = EOS ;

	/* print file mode */
	fdprintf (fd, "%c%9s", fType, modbits);

	/* fake links, user and group fields */
	fdprintf (fd, " %2d %-7d %-7d ", fileStat->st_nlink,
		  fileStat->st_uid, fileStat->st_gid );

	/* print file size */
	fdprintf (fd, " %13lld ", fileStat->st_size);
	
	/* print date */
	if (fileDate.tm_year == nowDate.tm_year)
	    {
	    fdprintf (fd, "%s %2d %02d:%02d ",
		      monthNames [fileDate.tm_mon + 1],	/* month */
		      fileDate.tm_mday,			/* day of month */
		      fileDate.tm_hour,			/* hour */
		      fileDate.tm_min);			/* minute */
	    }
	else
	    {
	    fdprintf (fd, "%s %2d  %04d ",
		      monthNames [fileDate.tm_mon + 1],	/* month */
		      fileDate.tm_mday,			/* day of month */
		      fileDate.tm_year + 1900);		/* year */
	    }

	} /* if doLong */
    else
	{ /* short listing */
	if (prefix != NULL)
	    {
	    if ((strcmp (prefix, CURRENT_DIRECTORY) == 0) || (*prefix == EOS))
		;				/* don't print "." prefix */
	    else if (prefix [strlen (prefix) - 1] != '/')
		fdprintf (fd, "%s/", prefix);
	    else
		fdprintf(fd, prefix);
	    }
	}

    /* last, print file name */
    if (fdprintf (fd, "%s%c\n", fileName, suffix) == ERROR)
	return (ERROR);

    return (OK);
    }

/*******************************************************************************
*
* dirList - list contents of a directory (multi-purpose)
*
* This command is similar to UNIX ls. It lists the contents of a directory
* in one of two formats. If <doLong> is FALSE, only the names of the files
* (or subdirectories) in the specified directory are displayed. If <doLong>
* is TRUE, then the file name, size, date, and time are displayed.
* If <doTree> flag is TRUE, then each subdirectory encountered
* will be listed as well (i.e. the listing will be recursive).
*
* The <dirName> parameter specifies the directory to be listed.
* If <dirName> is omitted or NULL, the current working directory will be
* listed. <dirName> may contain wildcard characters to list some
* of the directory's contents.
*
* LIMITATIONS
* \ms
* \m -
* With dosFsLib file systems, MS-DOS volume label entries are not reported.
* \m -
* Although an output format very similar to UNIX "ls" is employed,
* some information items have no particular meaning on some
* file systems.
* \m -
* Some file systems which do not support the POSIX compliant dirLib()
* interface, can not support the <doLong> and <doTree> options.
* \me
*
* RETURNS: OK or ERROR.
*
* SEE ALSO
* dirLib, ls(), ll(), lsr(), llr(), the VxWorks programmer guides.
*/

STATUS dirList
    (
    int		    fd,		/* file descriptor to write on */
    const char *    dirString,	/* name of the directory to be listed */
    BOOL	    doLong,	/* if TRUE, do long listing */
    BOOL	    doTree	/* if TRUE, recurse into subdirs */
    )
    {
    STATUS		status;		/* return status */
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to dirent */
    struct stat		fileStat;	/* file status info (long listing) */
    char *		pDirName;	/* directory name */
    char *		pPattern;	/* wildcard pattern */
    char *		fileName;	/* the file name */
    char *		dir;		/* temp copy */

#ifdef _WRS_KERNEL
    /* try to do a netDrv listing first, hacky way to know its a local FS */
    if (_func_netLsByName != NULL)
	{
	if ((*_func_netLsByName) ((dirString != NULL) ? dirString : ".") == OK)
	    return (OK);
	else if (errno != S_netDrv_UNKNOWN_REQUEST)
	    return (ERROR);
	}
#endif

    dir = malloc (2 * MAX_FILENAME_LENGTH);
    if (dir == NULL)
	return ERROR;

    fileName = dir + MAX_FILENAME_LENGTH;

    /*
     * The source path must be parsed to obtain the directory name and the
     * the file name.  As usrPathSplit() may modify the path, a copy of
     * <source> is used.
     */

    if (dirString != NULL)
	{
	strncpy (dir, dirString, MAX_FILENAME_LENGTH - 1);
	dir [MAX_FILENAME_LENGTH - 1] = '\0';
	usrPathSplit (dir, &pDirName, &pPattern);
	}
    else
	{				/* If no directory name is  */
	pDirName = CURRENT_DIRECTORY;	/* specified, then use "./"  */
	pPattern = NULL;
	}

    /*
     * <pDirName> points to the directory name.
     * <pPattern> points to the pattern or file name to match.
     */

    pDir = opendir (pDirName);		/* Attempt to open directory. */
    if (pDir == NULL)
	goto _nodir;			/* Unable to open directory. */

    status = OK;

    if (strcmp (pDirName, CURRENT_DIRECTORY) == 0)	/* Do not print if */
	pDirName = "";					/* current directory */
    else if (doLong)
	fdprintf (fd, "\nListing Directory %s:\n", pDirName);

    do
	{
	errno = OK;

    	pDirEnt = readdir (pDir);
	if (pDirEnt != NULL)
	    {
	    if ((pPattern != NULL) &&
		(!dirListPattern (pPattern, pDirEnt->d_name, 0)))
		continue;

	    /* The directory entry matches <pPattern> */

	    if (doLong)				/* if doing long listing */
		{
		/* Construct path/filename for stat */
		usrPathCat (pDirName, pDirEnt->d_name, fileName);

		/* Get and print file status info */
		if (stat (fileName, &fileStat) != OK)
		    {
		    /* stat() error, mark the file questionable and continue */
		    bzero ((caddr_t) &fileStat, sizeof (fileStat));
		    }
		}

	    /*
	     * Note: <fileName> is only used in dirListEnt() if <doLong> is
	     * TRUE.  If <doLong> is FALSE, <fileName> is uninitialized, but
	     * neither is it used.
	     */

	    if (dirListEnt (fd, pDirEnt->d_name, &fileStat,
			    pDirName, fileName, doLong) == ERROR)
		status = ERROR;
	    }
	else if (errno != OK)		/* 1. readdir() returned NULL */
	    {				/* 2. and it was a real ERROR. */
	    fdprintf (fd, "error reading dir %s, errno: 0x%x\n",
		      pDirName, errno) ;
	    status = ERROR;
	    }
	} while (pDirEnt != NULL);	/* End of directory, or ERROR */

    status |= closedir (pDir);		/* Close directory. */
    if (!doTree)
	goto dirList_out;

    /* Recurse into each subdir AFTER all files and subdir are listed */

    pDir = opendir ((*pDirName == EOS) ? CURRENT_DIRECTORY : pDirName);
    if (pDir == NULL)
	goto _nodir ;

    do
	{
	errno = OK;
    	pDirEnt = readdir (pDir);

	if (pDirEnt != NULL)
	    {
	    if ((pPattern != NULL) &&
		(!dirListPattern (pPattern, pDirEnt->d_name, 0)))
		continue ;
	   
	    /* Construct path/filename for stat */
	    usrPathCat (pDirName, pDirEnt->d_name, fileName);

	    /* Get and print file status info */
	    if (stat (fileName, &fileStat) != OK)
		{
		/* stat() error, mark the file questionable and continue */
		bzero ((caddr_t) &fileStat, sizeof (fileStat));
		}

	    /* recurse into subdir, but not "." or ".." */
	    if (S_ISDIR (fileStat.st_mode) &&
		(strcmp (pDirEnt->d_name, ".") != 0) &&
		(strcmp (pDirEnt->d_name, "..") != 0))
		{
		status = dirList (fd, fileName, doLong, doTree);
		/* maybe count files ?? */
		}
	    }
	else					/* readdir returned NULL */
	    {
	    if (errno != OK)			/* if real error, not dir end */
		{
		fdprintf (fd, "error reading dir %s, errno: %x\n",
			  pDirName, errno) ;
		status = ERROR;
		}
	    }
	} while (pDirEnt != NULL);		/* until end of dir */

    status |= closedir (pDir);			/* Close directory. */

dirList_out:

    free (dir);
    return (status);

_nodir:
    fdprintf (fd, "Can't open \"%s\".\n", dirString);
    status = ERROR;
    goto dirList_out;
    }

/*******************************************************************************
*
* ls - generate a brief listing of a directory
*
* This function is simply a front-end for dirList(), intended for
* brevity and backward compatibility. It produces a list of files
* and directories, without details such as file size and date,
* and without recursion into subdirectories.
*
* <dirName> is a name of a directory or file, and may contain wildcards.
* <doLong> is provided for backward compatibility.
*
* NOTE
* This is a target resident function, which manipulates the target I/O
* system. It must be preceded with the '@' letter if executed from the 
* Host Shell (windsh), which has a built-in command of the same name that 
* operates on the Host's I/O system.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO
* dirList(), the VxWorks programmer guides, the
* \tb VxWorks Command-Line Tools User's Guide.
*/

STATUS ls
    (
    const char *    dirName,	/* name of dir to list */
    BOOL	    doLong	/* switch on details */
    )
    {
    return (dirList (STD_OUT, dirName, doLong, FALSE));
    }

/******************************************************************************
*
* ll - generate a long listing of directory contents
*
* This command causes a long listing of a directory's contents to be
* displayed.  It is equivalent to:
* \cs
*     -> dirList 1, dirName, 1, 0
* \ce
*
* <dirName> is a name of a directory or file, and may contain wildcards.
*
* NOTE 1
* This is a target resident function, which manipulates the target I/O
* system. It must be preceded with the '@' letter if executed from the 
* Host Shell (windsh), which has a built-in command of the same name that 
* operates on the Host's I/O system.
*
* NOTE 2
* When used with netDrv devices (FTP or RSH), ll() does not give
* directory information.  It is equivalent to an ls() call with no
* long-listing option.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* dirList(), the VxWorks programmer guides.
*/

STATUS ll
    (   
    const char * dirName	/* name of directory to list */
    )
    {
    return (dirList (STD_OUT, dirName, TRUE, FALSE));
    }

/***************************************************************************
*
* lsr - list the contents of a directory and any of its subdirectories
*
* This function is simply a front-end for dirList(), intended for
* brevity and backward compatibility. It produces a list of files
* and directories, without details such as file size and date,
* with recursion into subdirectories.
*
* <dirName> is a name of a directory or file, and may contain wildcards.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO
* dirList(), the VxWorks programmer guides.
*/

STATUS lsr
    (
    const char * dirName	/* name of dir to list */
    )
    {
    return (dirList (STD_OUT, dirName, FALSE, TRUE));
    }

/***************************************************************************
*
* llr - do a long listing of directory and all its subdirectories contents
*
* This command causes a long listing of a directory's contents to be
* displayed.  It is equivalent to:
* \cs
*     -> dirList 1, dirName, 1, 0
* \ce
*
* <dirName> is a name of a directory or file, and may contain wildcards.
*
* NOTE
* When used with netDrv devices (FTP or RSH), ll() does not give
* directory information.  It is equivalent to an ls() call with no
* long-listing option.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO
* dirList(), the VxWorks programmer guides.
*/

STATUS llr
    (   
    const char * dirName	/* name of directory to list */
    )
    {
    return (dirList (STD_OUT, dirName, TRUE, TRUE));
    }

/***************************************************************************
*
* cp - copy file into other file/directory.
*
* This command copies from the input file to the output file.
* If destination name is directory, a source file is copied into
* this directory, using the last element of the source file name
* to be the name of the destination file.
*
* This function is very similar to copy(), except it is somewhat
* more similar to the UNIX "cp" program in its handling of the
* destination.
*
* <src> may contain a wildcard pattern, in which case all files
* matching the pattern will be copied to the directory specified in
* <dest>.
* This function does not copy directories, and is not recursive.
* To copy entire subdirectories recursively, use xcopy().
*
* EXAMPLES
* \cs
* -> cp( "/sd0/FILE1.DAT","/sd0/dir2/f001.dat")
* -> cp( "/sd0/dir1/file88","/sd0/dir2")
* -> cp( "/sd0/@.tmp","/sd0/junkdir")
* \ce
*
* RETURNS: OK or ERROR if destination is not a directory while <src> is
* a wildcard pattern, or if any of the files could not be copied.
*
* SEE ALSO
* xcopy(), the VxWorks programmer guides.
*/

STATUS cp
    (
    const char * src,	/* source file or wildcard pattern */
    const char * dest	/* destination file name or directory */
    )
    {
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to directory entry */
    STATUS		status = OK;	/* default return value */
    char *		pPattern;
    char *		pDirName;
    char *		dir;
    char *		fileName;
    char *		destName;

    if (src == NULL)
    	{				/* Return ERROR if a source */
	errno = EINVAL;			/* file was not specified. */
        return (ERROR);
    	}

    if (dest == NULL)			/* If no destination was specified,  */
	dest = CURRENT_DIRECTORY;	/* default to the current directory. */

    dir = malloc (3 * MAX_FILENAME_LENGTH);
    if (dir == NULL)
	return ERROR;

    fileName = dir + MAX_FILENAME_LENGTH;
    destName = fileName + MAX_FILENAME_LENGTH;

    strncpy (dir, src, MAX_FILENAME_LENGTH - 1);
    usrPathSplit (dir, &pDirName, &pPattern);

    if (pPattern == NULL)
	{
	printErr ("cp: source \"%s\" is not a file\n", src);
	errno = EISDIR;
	status = ERROR;
	goto cp_out;
	}

    if (!nameIsDir (dest))
	{

	/*
	 * <dest> is not an existing directory.  If <src> contains any wild
	 * characters, then no copying is performed as the directory <dest>
	 * does not exist.  (Can not copy multiple files onto one file.)
	 */

	if (dirNameWildcard (src))
	    {
	    printErr ("cp: destination \"%s\" not a directory\n", dest);
	    errno = ENOTDIR;
	    status = ERROR;
	    goto cp_out;
	    }

	printf ("copying file %s -> %s\n", src, dest);
        status = copy (src, dest);
	goto cp_out;
	}


    /* If there are not any wildcards in the file name, then copy it now */
    if (!dirNameWildcard (pPattern))
	{
	usrPathCat (dest, pPattern, destName);
	printf ("copying file %s -> %s\n", src, destName);
	status = copy (src, destName);
	goto cp_out;
	}

    pDir = opendir (pDirName) ;		/* Open the directory. */
    if (pDir == NULL)
	{				/* Oops.  There was an error. */
	perror (pDirName);
	status = ERROR;
	goto cp_out;
	}

    errno = OK;

    do
	{
    	pDirEnt = readdir (pDir);
	if (pDirEnt != NULL)
	    {
	    if ((pPattern != NULL) &&
		(!dirListPattern (pPattern, pDirEnt->d_name, 0)))
		continue;

	    if ((strcmp (pDirEnt->d_name, "." ) == 0) ||  /* Do not copy  */
		(strcmp (pDirEnt->d_name, "..") == 0))	  /* "." nor ".." */
		continue ;

	    /* Construct path/filename for stat */

	    usrPathCat (pDirName, pDirEnt->d_name, fileName);
	    usrPathCat (dest, pDirEnt->d_name, destName);

	    if (nameIsDir (fileName))
		{
		printf ("skipping directory %s\n", fileName);
		continue;
		}

	    printf ("copying file %s -> %s\n", fileName, destName);
	    status |= copy (fileName, destName);
	    }
	} while (pDirEnt != NULL);		/* until end of dir */

    status |= closedir (pDir);

cp_out:
    free (dir);
    return (status) ;
    }/* cp() */

/***************************************************************************
*
* mv - mv file into other directory.
*
* This function is similar to rename() but behaves somewhat more
* like the UNIX program "mv", it will overwrite files.
*
* This command moves the <src> file or directory into
* a file which name is passed in the <dest> argument, if <dest> is
* a regular file or does not exist.
* If <dest> name is a directory, the source object is moved into
* this directory as with the same name,
* if <dest> is NULL, the current directory is assumed as the destination
* directory.
* <src> may be a single file name or a path containing a wildcard
* pattern, in which case all files or directories matching the pattern
* will be moved to <dest> which must be a directory in this case.
*
* EXAMPLES
* \cs
* -> mv( "/sd0/dir1","/sd0/dir2")
* -> mv( "/sd0/@.tmp","/sd0/junkdir")
* -> mv( "/sd0/FILE1.DAT","/sd0/dir2/f001.dat")
* \ce
*
* RETURNS: OK or error if any of the files or directories could not be
* moved, or if <src> is a pattern but the destination is not a
* directory.
*
* SEE ALSO
* the VxWorks programmer guides. 
*/

STATUS mv
    (   
    const char * src,	/* source file name or wildcard */
    const char * dest	/* destination name or directory */
    )
    {
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to dirent */
    STATUS 		status = OK;
    char * 		pPattern = NULL;
    char * 		pDirName = NULL;
    char *  		dir;
    char * 		fileName;
    char *		destName;
    USR_FS_TYPE         srcType;
    USR_FS_TYPE         destType;

    if (src == NULL)
    	{
	errno = EINVAL;		/* Return ERROR if a source */
        return (ERROR);		/* file was not specified.  */
    	}

    if (dest == NULL)			/* If no destination was specified,  */
	dest = CURRENT_DIRECTORY;	/* default to the current directory. */

    /*
     *  Source Type        Destination Type      Action
     *  ------------------------------------------------
     *  USR_FS_FILE        USR_FS_WILD           Error
     *                     USR_FS_NO_EXIST       Move file to new name
     *                     USR_FS_DIR            Move file into directory
     *                     USR_FS_FILE           Move file to new name
     *
     *  USR_FS_DIR         USR_FS_WILD           Error
     *                     USR_FS_NO_EXIST       Move directory to new name
     *                     USR_FS_DIR            Move directory into directory
     *                     USR_FS_FILE           Error
     *
     *  USR_FS_WILD        USR_FS_WILD           Error
     *                     USR_FS_NO_EXIST       Error
     *                     USR_FS_DIR            Move objects into directory
     *                     USR_FS_FILE           Error
     *
     *  USR_FS_NO_EXIST    ALL CASES             Error
     */

    srcType = usrFsPathQuery (src);
    destType = usrFsPathQuery (dest);

    if (srcType == USR_FS_NO_EXIST)
        {
        printErr ("mv: source \"%s\" does not exist.\n", src);
        errno = ENOENT;
        return (ERROR);
        }

    if (destType == USR_FS_WILD)
        {
        printErr ("mv: destination \"%s\" can not contain wildcards\n", dest);
        errno = EINVAL;
        return (ERROR);
        }

    if ((srcType == USR_FS_WILD) && (destType != USR_FS_DIR))
        {
        printErr ("mv: destination \"%s\" must be a directory when\n"
                  "    source contains wildcards.\n", dest);
        errno = EINVAL;
        return (ERROR);
        }

    if ((srcType == USR_FS_DIR) && (destType == USR_FS_FILE))
        {
        printErr ("mv: cannot move a directory to a file.\n");
        errno = EINVAL;
        return (ERROR);
        }

    if (srcType == USR_FS_FILE)
        {
        if ((destType == USR_FS_FILE) || (destType == USR_FS_NO_EXIST))
            {
            printf ("moving file %s -> %s\n", src, dest);
            return (mvFile (src, dest));
            }
        /*
         * If <destType> is a directory, the filename is preserved and appended
         * to the directory path.  Those stepas are performed later.
         */
        }
    else if (srcType == USR_FS_DIR)
        {
        if (destType == USR_FS_NO_EXIST)
            {
            printf ("moving directory %s -> %s\n", src, dest);
            return (mvFile (src, dest));
            }
        /*
         * If <destType> is a directory, the source directory name is preserved
         * and appended to the destination directory path.  Those steps are 
         * performed later.
         */
        }

    /* Split the source path into pattern and directory */

    dir = malloc (3 * MAX_FILENAME_LENGTH);
    if (dir == NULL)
        return (ERROR);

    fileName = dir + MAX_FILENAME_LENGTH;
    destName = fileName + MAX_FILENAME_LENGTH;

    strncpy (dir, src, MAX_FILENAME_LENGTH - 1);
    usrPathSplit2 (dir, &pDirName, &pPattern);

    if (pPattern == NULL)
        {
        printf ("mv: cannot move top level directory\n");
        return (EINVAL);
        }

    if ((srcType == USR_FS_FILE) || (srcType == USR_FS_DIR))
        {
        /*
         * The destination type must be a directory.
         * Append file (or directory) name to destination.
         */

        if (strlen (dest) + strlen (pPattern) + 2 >= MAX_FILENAME_LENGTH)
            return (ENAMETOOLONG);

        sprintf (destName, "%s/%s", dest, pPattern);

        if (srcType == USR_FS_FILE)
            printf ("moving file %s -> %s\n", src, dest);
        else
            printf ("moving directory %s -> %s\n", src, dest);

        status = mvFile (src, destName);

        free (dir);
        return (status);
        }

    /*
     * Source contains wild cards.  This may be either the <pDirName> or
     * <pPattern> components.  If <pDirName> contains wild cards, then
     * the opendir() will fail.  If it does not, then the wild cards by
     * default, must be in the <pPattern> component.
     */

    pDir = opendir (pDirName) ;         /* Attempt to open directory. */
    if (pDir == NULL)
        {                               /* Oops, there was a problem. */
        perror(pDirName);
        status = ERROR;
        goto mv_out;
        }

    errno = OK;

    while ((pDirEnt = readdir (pDir)) != NULL)
	{
	if ((pPattern != NULL) &&
	    (!dirListPattern (pPattern, pDirEnt->d_name, 0)))
	    continue;

	if ((strcmp (pDirEnt->d_name, ".") == 0) ||
	    (strcmp (pDirEnt->d_name, "..") == 0))
	    continue ;

	/* Construct path/filename for stat */
	usrPathCat (pDirName, pDirEnt->d_name, fileName);
	usrPathCat (dest, pDirEnt->d_name, destName);

	printf ("moving file %s -> %s\n", fileName, destName);
	status |= mvFile (fileName, destName);
	}

    status |= closedir (pDir);

mv_out:
    free (dir);
    return (status);
    }  /* mv() */


/*******************************************************************************
*
* mvFile - move a file from one place to another.  
*
* This routine does some of the work of the mv() function.
*
* RETURNS: OK, or ERROR if the file could not be opened or moved.
*
* NOMANUAL
*/

LOCAL int mvFile
    (
    const char *oldname,        /* name of file to move */
    const char *newname         /* name with which to move file */
    )
    {
    int fd;
    int status;

    if ((oldname == NULL) || (newname == NULL) || (*newname == EOS))
        {
        errnoSet (ENOENT);
        return (ERROR);
        }

    /* try to open file */

    if (ERROR == (fd = open ((char *) oldname, O_RDONLY, 0)))
        return (ERROR);

    /* move it */

    status = ioctl (fd, FIOMOVE, (int) newname);

    close (fd);
    return (status);
    }

/******************************************************************************
*
* xcopy - copy a hierarchy of files with wildcards
*
* <source> is a string containing a name of a directory, or a wildcard
* or both which will cause this function to make a recursive copy of all
* files residing in that directory and matching the wildcard pattern into
* the <dest> directory, preserving the file names and subdirectories.
*
* CAVEAT
* This function may call itself in accordance with the depth of the
* source directory, and allocates 3 kB of heap memory per stack
* frame, meaning that to accommodate the maximum depth of subdirectories
* which is 20, at least 60 kB of heap memory should be available.
*
* RETURNS: OK, or ERROR if any operation has failed.
*
* SEE ALSO:
* tarLib, cp(), the VxWorks programmer guides. 
*/

STATUS xcopy
    (
    const char * source,	/* source directory or wildcard name */
    const char * dest		/* destination directory */
    )
    {
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to directory entry */
    STATUS		status = OK;	/* Default return value */
    char *		pPattern;
    char *		pDirName;
    char *		pDirPath;
    char *		pFileName;
    char *		pDestName;
    char *		pMemory;

    /*
     * Allocate memory for pDirPath[], pFileName[], and pDestName[].  Return
     * ERROR on NULL.  Note that malloc() sets the errno.
     */

    pMemory = malloc (3 * MAX_FILENAME_LENGTH);

    if (pMemory == NULL)
	return (ERROR);

    pDirPath = pMemory;
    pFileName = (pMemory + MAX_FILENAME_LENGTH);
    pDestName = (pMemory + (2 * MAX_FILENAME_LENGTH));

    /*
     * The source path must be parsed to obtain the directory name and the
     * the file name.  As usrPathSplit() may modify the path, a copy of
     * <source> is used.
     */

    strncpy (pDirPath, source, MAX_FILENAME_LENGTH - 1);
    usrPathSplit (pDirPath, &pDirName, &pPattern);

    if ((pPattern != NULL) && (!dirNameWildcard (pPattern)))
	{
	printf ("copying file %s -> %s\n", source, dest);
	status = copy (source, dest);

	free (pMemory);
	return (status);
	}

    pDir = opendir (pDirName);
    if (pDir == NULL)
	{
	perror (pDirName);
	free (pMemory);

	return (ERROR);
	}

    /*
     * At this point, the source directory has been opened, and is thus known
     * to exist.  Now, determine if the destination directory exists.  If it
     * does not exist, then create it.
     *
     * This operation was moved from the start of the routine to here as it
     * was felt that the destination directory should not be created if the
     * the source directory could not be opened.  That is how DOS does it.
     */

    if (!nameIsDir (dest))
	{
	/* The destination directory does not exist; create it. */

#ifdef _WRS_KERNEL
	if (mkdir (dest) == ERROR)
#else
	if (mkdir (dest, 0777) == ERROR)
#endif
	    {
	    closedir (pDir);
	    printErr ("xcopy: error: destination \"%s\" is not a directory\n",
		      dest );
	    free (pMemory);
	    errno = ENOTDIR;
	    return (ERROR);
	    }
	printf ("creating directory %s \n", dest);
	}

    errno = OK;

    while ((pDirEnt = readdir (pDir)) != NULL)
	{
	if ((pPattern != NULL) &&
	    (!dirListPattern (pPattern, pDirEnt->d_name, 0)))
	    continue ;

	/* Construct path/filename for stat */

	usrPathCat (pDirName, pDirEnt->d_name, pFileName);
	usrPathCat (dest, pDirEnt->d_name, pDestName);

	if (!nameIsDir (pFileName))
	    {
	    printf ("copying file %s -> %s\n", pFileName, pDestName);
	    status |= copy (pFileName, pDestName);
	    continue;
	    }
	else if ((strcmp (pDirEnt->d_name, ".") == 0) ||
		 (strcmp (pDirEnt->d_name, "..") == 0))
	    continue;

	printf ("copying dir %s -> %s\n", pFileName, pDestName);
	status |= xcopy (pFileName, pDestName);
	}

    status |= closedir (pDir);
    free (pMemory);

    return (status);
    }

/******************************************************************************
*
* xdelete - delete a hierarchy of files with wildcards
*
* <source> is a string containing a name of a directory, or a wildcard
* or both which will cause this function to recursively remove all
* files and subdirectories residing in that directory
* and matching the wildcard pattern.
* When a directory is encountered, all its contents are removed,
* and then the directory itself is deleted.
*
* Note that the wildcard matching is limited to a single directory level.
* \cs
*     dir         is valid
*     *.c         is valid
*     dir.c     is valid
*     *a.c      is not valid
* \ce
*
* RETURNS: OK or ERROR if any operation has failed.
*
* SEE ALSO
* cp(), copy(), xcopy(), tarLib, the VxWorks programmer guides. 
*/

STATUS xdelete
    (
    const char * source			/* source directory or wildcard name */
    )
    {
    STATUS		status = OK;	/* Default return value */
    char *		pPattern;
    char *		pDirName;
    char *		pDirPath;
    char *		pPathBuf;
    char *		pMemory;

    /*
     * Allocate memory for pDirPath[] and pPathBuf[].  Return
     * ERROR on NULL.  Note that malloc() sets the errno.
     */

    pMemory = malloc (2 * MAX_FILENAME_LENGTH);
    if (pMemory == NULL)
	{
	printf ("can't allocate memory: %s\n", strerror (errno));
	return (ERROR);
	}

    pDirPath = pMemory;
    pPathBuf = (pMemory + MAX_FILENAME_LENGTH);

    /*
     * The source path must be parsed to obtain the directory name and the
     * the file name.  As usrPathSplit() may modify the path, a copy of
     * <source> is used.
     */

    strncpy (pDirPath, source, MAX_FILENAME_LENGTH - 1);
    pDirPath[MAX_FILENAME_LENGTH - 1] = EOS;
    usrPathSplit (pDirPath, &pDirName, &pPattern);

    /*
     * If there are no wildcard characters in the "pattern" part,
     * we must have had an argument like foo/bar or /foo/bar and
     * we got it split into its foo and bar parts.  Just use the
     * whole source name as a file name.
     */
    if ((pPattern != NULL) && (!dirNameWildcard (pPattern)))
	{
	printf ("deleting file %s\n", source);
	status = unlink ((char *) source);	/* Delete a single file. */
	if (status != OK)
	    perror (source);
	}
    else
	{
	/*
	 * The argument named a directory, or there was a valid pattern
	 * (note that usrPathSplit returns with pPattern == NULL
	 * if the input is *just* a directory name).  Use the recursive
	 * code to delete the right files within it.  This code needs
	 * a modifiable path buffer that will not clobber the pattern,
	 * which is what pPathBuf is for.
	 */
	strcpy(pPathBuf, pDirName);
	status = recursiveXDelete (pPathBuf, pPattern);
	}

    free (pMemory);
    return (status);
    }

/******************************************************************************
*
* recursiveXDelete - handle recursive deletion for xdelete().
*
* This routine opens the directory and removes everything in it
* (recursively), unless the given pattern is not NULL, in which
* case it skips anything matching that pattern.
*
* It prints the name of each file/dir removed as it goes.
*
* If the pattern is NULL the directory itself is removed at the end.
*
* RETURNS: STATUS of whether any deletion failed.
*
* NOMANUAL
*/

LOCAL STATUS recursiveXDelete
    (
    char *path,				/* buffer with directory to delete */
    char *pattern			/* pattern to match, or NULL */
    )
    {
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to directory entry */
    STATUS		retVal = OK;	/* ultimate return value */
    STATUS		status = OK;	/* result of intermediate ops */
    char *		pEndPath;	/* point at which to insert file */
    size_t pathLen;		/* length of dir path including appended / */
    size_t fileLen;		/* length of file name in dir */

#if 0
    /* see rmdir() at the bottom */

    if (pattern == NULL)
	printf ("deleting directory %s\n", path);
#endif

    pDir = opendir (path);
    if (pDir == NULL)
	{
	perror (path);
	return (ERROR);
	}

    /*
     * Find the length of the dir path and tack on a slash (but
     * skip it if there is already a slash).
     */
    pathLen = strlen (path);
    pEndPath = path + pathLen;
    if (pEndPath[-1] != '/')
	{
	pEndPath[0] = '/';
	pEndPath++;
	pathLen++;
	}

    while (pDir != NULL && (pDirEnt = readdir (pDir)) != NULL)
	{
	/* skip files that do not match the pattern, if there is one */

	if ((pattern != NULL) &&
	    (!dirListPattern (pattern, pDirEnt->d_name, 0)))
	    continue;

	fileLen = strlen (pDirEnt->d_name);
	if (pathLen + fileLen >= MAX_FILENAME_LENGTH)
	    {
	    printf ("%s%s: pathname too long\n", path, pDirEnt->d_name);
	    errno = ENAMETOOLONG;
	    retVal |= ERROR;
	    continue;
	    }
	strcpy (pEndPath, pDirEnt->d_name);
	if (nameIsDir (path))
	    {
	    /* skip "." and ".." */

	    if (pDirEnt->d_name[0] == '.' &&
		    (fileLen == 1 || strcmp(pDirEnt->d_name, "..") == 0))
		continue;

	    /*
	     * Closing the directory here makes the loop O(N**2) in the
	     * number of files *not* deleted (because we cannot reliably
	     * restore our existing position with a dir-seek).  It does,
	     * however, allow deleting deeply nested directory trees
	     * without running out of file descriptors (SPR#102311).
	     */
	    closedir (pDir);
	    if ((status = recursiveXDelete (path, NULL)) != OK)
		{
		retVal |= status;
		return (retVal);
		}
	    pEndPath[-1] = EOS;
	    pDir = opendir (path);
	    if (pDir == NULL)
		{
		printf ("unable to reopen directory %s: %s\n",
			path, strerror(errno));
		retVal |= ERROR;
		}
	    pEndPath[-1] = '/';
	    }
	else
	    {
	    printf ("deleting file %s\n", path);
	    status = unlink (path);
	    if (status != OK)
		perror (path);
	    retVal |= status;
	    }
	}

    if (pDir != NULL)
	status |= closedir (pDir);

    /*
     * If we were told to delete everything in the directory,
     * we assume that means we should delete the directory as
     * well.  If we were told to match a pattern, we leave the
     * directory intact even if all files have been deleted.
     */
    if (pattern == NULL)
	{
	pEndPath[-1] = EOS;
	printf ("deleting directory %s\n", path);
	status = rmdir (path);
	if (status != OK)
	    perror (path);
	retVal |= status;
	}

    return (retVal);
    }

/******************************************************************************
*
* attrib - modify MS-DOS file attributes on a file or directory
*
* This function provides means for the user to modify the attributes
* of a single file or directory. There are four attribute flags which
* may be modified: "Archive", "System", "Hidden" and "Read-only".
* Among these flags, only "Read-only" has a meaning in VxWorks,
* namely, read-only files can not be modified deleted or renamed.
*
* The <attr> argument string may contain must start with either "+" or
* "-", meaning the attribute flags which will follow should be either set
* or cleared. After "+" or "-" any of these four letter will signify their
* respective attribute flags - "A", "S", "H" and "R".
*
* For example, to write-protect a particular file and flag that it is a
* system file:
*
* \cs
* -> attrib( "bootrom.sys", "+RS")
* \ce
*
* RETURNS: OK, or ERROR if the file can not be opened.
*
* SEE ALSO
* dosFsLib, the VxWorks programmer guides.
*/

STATUS attrib
    (
    const char * fileName,	/* file or dir name on which to change flags */
    const char * attr		/* flag settings to change */
    )
    {
    BOOL	set = TRUE;
    STATUS	status;
    u_char	bit = 0;
    struct stat fileStat;
    int		fd;

    if (attr == NULL)
	{
	errno = EINVAL;
	return (ERROR);
	}

    fd = open (fileName, O_RDONLY, 0);

    if (fd == ERROR)
	{
	perror (fileName);
	return (ERROR);
	}

    if (fstat (fd, &fileStat) == ERROR)          /* get file status    */
	{
	printErr ("Can't get stat on %s\n", fileName);
	return (ERROR);
	}

    for ( ; *attr != EOS ; attr ++)
	{
	switch (*attr)
	    {
	    case '+':
		set = TRUE;
		break;
	    case '-':
		set = FALSE;
		break;
	    case 'A':
	    case 'a' :
		bit = DOS_ATTR_ARCHIVE;
		break;
	    case 'S':
	    case 's':
		bit = DOS_ATTR_SYSTEM;
		break;
	    case 'H':
	    case 'h':
		bit = DOS_ATTR_HIDDEN;
		break;
	    case 'R':
	    case 'r':
		bit = DOS_ATTR_RDONLY;
		break;
	    default:
		printErr ("Illegal attribute flag \"%c\", ignored\n", *attr);
	    } /* end of switch */
	if (set)
	    fileStat.st_attrib |= bit;
	else
	    fileStat.st_attrib &= ~bit;
	}

    status = ioctl (fd, FIOATTRIBSET, fileStat.st_attrib);

    close (fd);

    return (status);
    }

/******************************************************************************
*
* xattrib - modify MS-DOS file attributes of many files
*
* This function is essentially the same as attrib(), but it accepts
* wildcards in <fileName>, and traverses subdirectories in order
* to modify attributes of entire file hierarchies.
*
* The <attr> argument string may contain must start with either "+" or
* "-", meaning the attribute flags which will follow should be either set
* or cleared. After "+" or "-" any of these four letter will signify their
* respective attribute flags - "A", "S", "H" and "R".
*
* EXAMPLE
* \cs
* -> xattrib( "/sd0/sysfiles", "+RS") /@ write protect "sysfiles" @/
* -> xattrib( "/sd0/logfiles", "-R")  /@ unprotect logfiles before deletion @/
* -> xdelete( "/sd0/logfiles")
* \ce
*
* CAVEAT
* This function may call itself in accordance with the depth of the
* source directory, and allocates 2 kB of heap memory per stack
* frame, meaning that to accommodate the maximum depth of subdirectories
* which is 20, at least 40 kB of heap memory should be available.
*
* RETURNS: OK, or ERROR if the file can not be opened.
*
* SEE ALSO
* dosFsLib, the VxWorks programmer guides.
*/

STATUS xattrib
    (
    const char * source,    /* file or dir name on which to change flags */
    const char * attr	    /* flag settings to change */
    )
    {
    DIR *		pDir;		/* ptr to directory descriptor */
    struct dirent *	pDirEnt;	/* ptr to dirent */
    STATUS		status = OK;
    char *		pPattern;
    char *		pDirName;
    char *		pDirPath;
    char *		pFileName;
    char *		pMemory;

    /*
     * Allocate memory for pDir[] and pFileName[].  Return
     * ERROR on NULL.  Note that malloc() sets the errno.
     */

    pMemory = malloc (2 * MAX_FILENAME_LENGTH);
    if (pMemory == NULL)
	return (ERROR);

    pDirPath = pMemory;
    pFileName = (pMemory + MAX_FILENAME_LENGTH);

    /*
     * The source path must be parsed to obtain the directory name and the
     * the file name.  As usrPathSplit() may modify the path, a copy of
     * <source> is used.
     */

    strncpy (pDirPath, source, MAX_FILENAME_LENGTH - 1);
    usrPathSplit (pDirPath, &pDirName, &pPattern);

    /* If <source> refers to a single file, do the work now. */

    if ((pPattern != NULL) && (!dirNameWildcard (pPattern)))
	{
	printf ("changing attributes on %s\n", source);
	status = attrib (source, attr);
	free (pMemory);
	return (status);
	}

    /* <source> refers to either a directory or many files. */

    pDir = opendir (pDirName);		/* Attempt to open the directory. */
    if (pDir == NULL)
	{				/* Oops.  It failed. */
	perror (pDirName);
	free (pMemory);
	return (ERROR);
	}

    /*
     * Cycle through each directory entry.  If the entry does not match the
     * pattern, get the next entry and try again.  If the entry is not a
     * directory (it is a file), then change the attributes.  If it is a
     * directory, then recurse into that directory before changing its
     * attributes.
     */

    errno = OK;
    while ((pDirEnt = readdir (pDir)) != NULL)
	{
	if ((pPattern != NULL) &&
	    (!dirListPattern (pPattern, pDirEnt->d_name, 0)))
	    continue ;

	/* Construct path/filename for stat */
	usrPathCat (pDirName, pDirEnt->d_name, pFileName);

	if (!nameIsDir (pFileName))
	    {
	    printf ("changing attributes on %s\n", pFileName);
	    status |= attrib (pFileName, attr);
	    }
	else if ((strcmp (pDirEnt->d_name, ".") == 0) ||
		 (strcmp (pDirEnt->d_name, "..") == 0))
	    continue;

	printf ("traversing directory %s to change attributes \n",
		pFileName);
	status |= xattrib (pFileName, attr);
	status |= attrib (pFileName, attr);
	}

    status |= closedir (pDir);
    free (pMemory);
    return (status);
    }

/*******************************************************************************
*
* dosfsDiskFormat - format a disk with dosFs
*
* This command formats a disk and creates the dosFs file system on it.  The
* device must already have been created by the device driver and
* dosFs format component must be included.
*
*
* EXAMPLE
* \cs
*     -> dosfsDiskFormat "/fd0"
* \ce
*
* RETURNS: OK, or ERROR if the device cannot be opened or formatted.
*
* SEE ALSO
* dosFsLib, the VxWorks programmer guides.
*/

STATUS dosfsDiskFormat
    (
    const char * pDevName 	/* name of the device to initialize */
    )
    {
#ifdef _WRS_KERNEL
    if (dosFsFmtRtn)
        return (dosFsFmtRtn ((char *)pDevName, 0, NULL));
    errno = ENOSYS;
    printErr("dosFs disk formatter not installed\n");
    return (ERROR);
#else
    printf("Formatting from RTPs is not currently supported\n");
    return (ERROR);
#endif
    }

/*******************************************************************************
*
* diskFormat - format a disk with dosFs
*
* This command in now obsolete. Use dosfsDiskFormat or dosFsVolFormat() instead
*
* This command formats a disk and creates the dosFs file system on it.  The
* device must already have been created by the device driver and
* dosFs format component must be included.
*
*
* EXAMPLE
* \cs
*     -> diskFormat "/fd0"
* \ce
*
* RETURNS: OK, or ERROR if the device cannot be opened or formatted.
*
* SEE ALSO
* dosFsLib, the VxWorks programmer guides. 
*/

STATUS diskFormat
    (
    const char * pDevName 	/* name of the device to initialize */
    )
    {
#ifdef _WRS_KERNEL
    printf ("WARNING. diskFormat is deprecated and will not be supported in the next release.\n");
    printf ("         Please use file system specific formatter.\n");
    return (dosfsDiskFormat (pDevName));
#else
    printf("Formatting dosFs from RTPs is no longer supported\n");
    return (ERROR);
#endif
    }

/*******************************************************************************
*
* hrfsDiskFormat - format a disk with HRFS
*
* This command formats a disk and creates the HRFS file system on it.  The
* device must already have been created by the device driver and
* HRFS format component must be included.
*
*
* EXAMPLE
* \cs
*     -> hrfsDiskFormat "/fd0", 0    /@ format "/fd0" with HRFS @/
*                                    /@allowing maximum files @/
*     -> hrfsDiskFormat "/fd0", 100  /@ format "/fd0" with HRFS @/
*                                    /@allowing 100 files @/
* \ce
*
* RETURNS: OK, or ERROR if the device cannot be opened or formatted.
*
* SEE ALSO
* hrFsLib, the VxWorks programmer guides. 
*/

STATUS hrfsDiskFormat
    (
    const char * pDevName, 	/* name of the device to initialize */
    int          files,		/* the maximum number of files to support */
    UINT32       majorVer,	/* major version of fs to format */
    UINT32       minorVer,	/* minor version of fs to format */
    UINT32       options	/* formatter options */
    )
    {
#ifdef _WRS_KERNEL
    if (hrfsFmtRtn)
        return (hrfsFmtRtn ((char *)pDevName, 0, 0, files, majorVer,
			    minorVer, options));
    errno = ENOSYS;
    printErr ("HRFS Formatter not installed\n");
    return (ERROR);
#else
    printf("Formatting HRFS from RTPs is not supported\n");
    return (ERROR);
#endif
    }

/*******************************************************************************
*
* diskInit - initialize a file system on a block device
*
* This function is now obsolete.
*
*/

STATUS diskInit
    (
    const char *pDevName	/* name of the device to initialize */
    )
    {
    printf ("ERROR. diskInit is no longer supported.\n");
    printf ("       Please use file system specific formatter.\n");
	return (ERROR);
    }

/*******************************************************************************
*
* commit - commit current transaction to disk.
*
* This command is for transactional based file systems only such as HRFS. It
* is a shortcut for the ioctl function FIOCOMMITFS which commits the current
* transaction to disk to make changes permanment.
*
* EXAMPLE
* \cs
*     -> commit "/ata0a"             /@ commit transaction on "/fd0" @/
* \ce
*
* RETURNS: OK, or ERROR if the device is not formatted with a file system
*	   that does not support the FIOCOMMITFS ioctl function or <pDevName>
*	   is not valid.
*
* SEE ALSO
* hrFsLib,
* \tb VxWorks Kernel Programmer's Guide: `Kernel Shell'
*/
STATUS commit
    (
    const char * pDevName 	/* name of the device to commit */
    )
    {
    STATUS status;
    int fd;
    
    /* 
     * Open the root directory of the device. Use the 0_NOATIME flag to prevent
     * the access timestamp from being updated when the directory is eventually 
     * closed. This will prevent another commit being necessary if access
     * time stamps are enabled on the file system being committed.
     */
    fd = open (pDevName, O_NOATIME, 0777);
    if (fd < 0)
	return (ERROR);
	
    /* Do the actual commit */
    status = ioctl (fd, FIOCOMMITFS, NULL);
    close (fd);
    
    return (status);
    }
        
/*******************************************************************************
*
* ioHelp - print a synopsis of I/O utility functions
*
* This function prints out synopsis for the I/O and File System
* utility functions.
*
* RETURNS: N/A
*
* SEE ALSO
* the VxWorks programmer guides.
*/

void ioHelp (void)
    {
    static char *help_msg [] = {
    "cd         \"path\"          Set current working path",
    "pwd                        Print working path",
    "ls         [\"wpat\"[,long]] List contents of directory",
    "ll         [\"wpat\"]        List contents of directory - long format",
    "lsr        [\"wpat\"[,long]] Recursive list of directory contents",
    "llr        [\"wpat\"]        Recursive detailed list of directory",
    "rename     \"old\",\"new\"     Change name of file",
    "copy       [\"in\"][,\"out\"]  Copy in file to out file (0 = std in/out)",
    "cp         \"wpat\",\"dst\"    Copy many files to another dir",
    "xcopy      \"wpat\",\"dst\"    Recursively copy files",
    "mv         \"wpat\",\"dst\"    Move files into another directory",
    "xdelete    \"wpat\"          Delete a file, wildcard list or tree",
    "attrib     \"path\",\"attr\"   Modify file attributes",
    "xattrib    \"wpat\",\"attr\"   Recursively modify file attributes",
    "chkdsk     \"device\", L, V  Consistency check DOS file system",
    "dosfsDiskFormat \"device\"    DOSFS  file system format.",
    "hrfsDiskFormat  \"device\", N HRFS file system format.",
    "commit     \"device\"          Commit current transaction",
    "",
    "\"attr\" contains one or more of: \" + - A H S R\" characters",
    "\"wpat\" may be name of a file, directory or wildcard pattern",
    " in which case \"dst\" must be a directory name",
    "chkdsk() params: L=0, check only, L=2, check and fix, V=0x200 verbose",
    "hrfsDiskFormat() params: N number of files for file system, N=0 maximum\n"
    "                         number of files",
    NULL
    };
    FAST int ix;
    char ch;

    printf ("\n");
    for (ix = 0; help_msg [ix] != NULL; ix++)
	{
	if ((ix+1) % 20 == 0)
	    {
	    printf ("\nType <CR> to continue, Q<CR> to stop: ");
	    read (STD_IN, &ch, 1);
	    if (ch == 'q' || ch == 'Q')
		break;
	    else
		printf ("\n");
	    }
	printf ("%s\n", help_msg [ix]);
	}
    printf ("\n");
    }
