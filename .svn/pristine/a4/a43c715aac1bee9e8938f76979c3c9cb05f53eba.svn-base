/* ssiDsi.c - DSI sockets init */

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,04mar06,xli  added the local socket show init function and the #ifdef
01b,09feb06,xli  removed the return value
01a,17apr04,tkf  written.
*/

void usrDsiInit2(void)
{
#ifdef INCLUDE_DSI_SOCKET    
    usrDsiSocket();         /* Installs and initializes the DSI backend */
#endif

#ifdef INCLUDE_DSI_POOL    
    usrDsiPoolConfig ();    /* Number of DSI sockets, amount of data clusters */
#endif

#ifdef INCLUDE_UN    
    usrUnInit (); /* AF_LOCAL address domain and harness */
#endif

#ifdef INCLUDE_UN_COMP
    usrUnCompInit ();
#endif

#ifdef INCLUDE_UN_SHOW
    unShowInit ();    
#endif

#ifdef INCLUDE_UN_COMP_SHOW
     uncompShowInit ();
#endif
    return;
}
