/* tasklist.c
 * Heihaier - admin@heihaier.org
 */

#include "tasklist.h"
#include "../../debug/debug.h"

int fd_tasklist_init(FDTaskList * list)
{
	if(NULL == list)
	{
		DBG("fd tasklist init\n");
		return -1;
	}

	list->list = h_list_new();
	if(NULL == list->list)
	{
		DBG("fd tasklist init \n");
		return -1;
	}

	pthread_mutex_init(&list->mutex, NULL);
	sem_init(&list->sem, 0, 0);

	return 0;
}

void fd_tasklist_cleanup(FDTaskList * list)
{
	if(NULL == list)
	  DBG("fd tasklist cleanup failure: 0\n");

	h_list_delete(list->list);
	pthread_mutex_destroy(&list->mutex);
	sem_destroy(&list->sem);
}

int fd_tasklist_add(FDTaskList *list, FDTask * task)
{
	if(NULL == list)
	{
		DBG("fd tasklist add failure: 0\n");
		return -1;
	}

	if(NULL == task)
	{
		DBG("fd tasklist add failure: 1\n");
		return -1;
	}

	pthread_mutex_lock(&list->mutex);
	h_list_append(list->list, task);
	pthread_mutex_unlock(&list->mutex);
	sem_post(&list->sem);

	return 0;
}

int fd_tasklist_rmv(FDTaskList * list, FDTask ** task)
{
	if(NULL == list)
	{
		DBG("fd tasklist rmv failure: 0\n");
		return -1;
	}

	if(NULL == task)
	{
		DBG("fd tasklist rmv failure: 1\n");
		return -1;
	}

	sem_wait(&list->sem);

	pthread_mutex_lock(&list->mutex);
	h_list_first(list->list);
	*task = h_list_get(list->list);
	h_list_remove(list->list);
	pthread_mutex_unlock(&list->mutex);

	return 0;
}

