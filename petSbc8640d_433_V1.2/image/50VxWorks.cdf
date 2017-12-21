/* 50vxWorks.cdf - Enhances the 00vxWorks.cdf functionality for this BSP */

/*
// modification history
// --------------------
// 01a,09MAY03,BDS   written
*/

/*
// DESCRIPTION
// This file contains descriptions for some VxWorks components.
// The configuration tool searches for .cdf files in four
// places; here, arch/<arch>, config/<bsp>, and the project directory.
// This file describes "generic" components which are available
// to all CPU architectures and BSPs.
// If a new component is released, a new .cdf file should be deposited
// in the appropriate directory, rather than modifying existing files.
//
// The preceeding paragraph was the mandatory default information.
// This .cdf was created in order to update the INCLUDE_USER_APPL
// component. Both a "REQUIRES" and modified "INIT_RTN" were added to
// enhance the integration of the DY4AV2 BSP with the Tornado project
// facility.
*/

Component INCLUDE_USER_APPL {
	NAME		application initialization
	SYNOPSIS	call usrAppInit() (in your usrAppInit.c project file) after startup.
	INIT_RTN 	usrAppInit (); bspShellPromptInit();
}

