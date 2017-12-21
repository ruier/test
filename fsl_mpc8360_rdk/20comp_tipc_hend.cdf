/* 20comp_tipc_hend_cdf_master - TIPC HEnd Component configuration file */

/*
 * Copyright (c) 2005,2008 Wind River Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
modification history
--------------------
01b,08oct08,als  Update to TIPC 1.7.6 (Defect WIND00127984)
01a,11Nov05,mze  written.
*/

/* 
DESCRIPTION

This file contains all HEnd components for TIPC, the Transparent Inter-Process
Communication protocol.  TIPC components lie in the Network Protocol Components folder.

Include the main INCLUDE_TIPC component to allow applications to use the
default TIPC configuration, which provides intra-node communication only.

*/



Component INCLUDE_TIPC_HEND_INIT 
{
      NAME		TIPC prioritized interfaces
	SYNOPSIS	Initialization of TIPC prioritized interfaces
      CFG_PARAMS	TIPC_HEND_CONFIG_STR
	INIT_RTN	usrTipcHendAttach();
      REQUIRES    INCLUDE_HEND_PARAM_SYS INCLUDE_TIPC
}

Parameter TIPC_HEND_CONFIG_STR {
	NAME		TIPC interfaces using H-END
	SYNOPSIS	list of interface names, within quotes, with "," between interface names, or NULL
        TYPE            string
	DEFAULT		NULL
}


