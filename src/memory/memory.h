/* memory.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __H_MEMORY_H__
#define __H_MEMORY_H__

#include <stdlib.h>

#ifdef __H_DEBUG__

#include <pthread.h>

#undef malloc
#undef free
#define malloc	h_memory_malloc
#define free	h_memory_free

#define H_MEMORY_INIT		h_memory_init()
#define H_MEMORY_CLEANUP	h_memory_cleanup()

typedef struct
{
	unsigned int count;
	pthread_mutex_t mutex;
}HMemory;

int h_memory_init(void);
int h_memory_cleanup(void);
void h_memory_print(void);

void * h_memory_malloc(size_t size);
void h_memory_free(void * p);

#else

#define H_MEMORY_INIT
#define H_MEMORY_CLEANUP

#endif /* __H_DEBUG__ */

#endif /* __H_MEMORY_H__ */
