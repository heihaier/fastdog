/* memory.c
 * Heihaier - admin@heihaier.org
 */

#include "memory.h"
#include <stdio.h>

#ifdef __H_DEBUG__

#undef malloc
#undef free

HMemory hmem;

int h_memory_init(void)
{
	hmem.count = 0;
	if(pthread_mutex_init(&hmem.mutex, NULL) != 0)
	  return -1;

	return 0;
}

int h_memory_cleanup(void)
{
	return pthread_mutex_destroy(&hmem.mutex);
}

void * h_memory_malloc(size_t size)
{
	void * p = malloc(size);
	if(NULL != p)
	{
		pthread_mutex_lock(&hmem.mutex);
		hmem.count ++;
		pthread_mutex_unlock(&hmem.mutex);
	}

	return p;
}

void h_memory_free(void * p)
{
	if(NULL != p)
	{
		free(p);
		pthread_mutex_lock(&hmem.mutex);
		hmem.count --;
		pthread_mutex_unlock(&hmem.mutex);
	}
}

void h_memory_print(void)
{
	printf("memory count: %d\n", hmem.count);
}

#endif /* __H_DEBUG__ */
