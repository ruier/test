/* 00folder.cdf - folders pre definitions */

/* Copyright 2004-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,01feb05,bpn  Moved FOLDER_SHELL_CMD component to 01shell.cdf.
01a,22jun04,fle  written
*/

/*
DESCRIPTION
This Component Definition file just handle the folders which may have 
precedence problems.

For example, if a component declares itself as part of a folder which does
not exist yet because it is defined later (aka the cdf file is read later),
the folder should be defined here.
*/

Folder FOLDER_SHELL {
    NAME	kernel shell components
    SYNOPSIS	The vxWorks kernel shell for interactive development, prototyping, debugging and testing
}

