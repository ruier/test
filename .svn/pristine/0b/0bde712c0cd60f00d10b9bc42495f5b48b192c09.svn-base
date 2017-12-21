/* include hardware interface memory allocation scheme */

#ifdef	_WRS_CONFIG_DELAYED_HW_INIT
/*
 * If the hwInit is deferred to the post-kernel, the hwMemPool is not
 * necessary. Force to disable it.
 */
#undef	INCLUDE_HWMEM_ALLOC
#endif	/* _WRS_CONFIG_DELAYED_HW_INIT */

#ifdef INCLUDE_HWMEM_ALLOC
# ifndef HWMEM_POOL_SIZE
#  define HWMEM_POOL_SIZE 50000
# endif /* HWMEM_POOL_SIZE */

# ifndef INIT_HWMEMPOOL_IN_BSP 
char  hwMemPool[HWMEM_POOL_SIZE];
IMPORT void hwMemLibInit (void);
IMPORT STATUS hwMemPoolCreate (char * pMem, size_t size);
# endif /* INIT_HWMEMPOOL_IN_BSP */
#endif /* INCLUDE_HWMEM_ALLOC */
