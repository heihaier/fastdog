/* tasklist.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_TASKLIST_H__
#define __FD_TASKLIST_H__

#include "../../list/list.h"
#include "task.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
	HList * list;
	sem_t sem;
	pthread_mutex_t mutex;
}FDTaskList;

int fd_tasklist_init(FDTaskList * list);
void fd_tasklist_cleanup(FDTaskList * list);

int fd_tasklist_add(FDTaskList * list, FDTask * task);
int fd_tasklist_rmv(FDTaskList * list, FDTask ** task);

#endif /* __FD_TASKLIST_H__ */
