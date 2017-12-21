/*
Copyright 1998-2001 Wind River Systems, Inc.

modification history
--------------------
01a,10mar10,gvm   adapted from gnu; icc uses GNU C++

DESCRIPTION

This file contains descriptions for the GNU toolchain specific C++
compiler support and standard library components.
*/

Component INCLUDE_CPLUS_LANG {
	CONFIGLETTES	cplusgnuLang.c
}

Component INCLUDE_CPLUS_IOSTREAMS {
	NAME		C++ iostreams and other standard library facilities
	SYNOPSIS	C++ standard library
	REQUIRES        INCLUDE_CPLUS
	CONFIGLETTES    cplusgnuIos.c
}
