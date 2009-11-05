/* manager.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_CONNECTION_MANAGER_H__
#define __FD_CONNECTION_MANAGER_H__

#include "listener.h"
#include "../config/config.h"
#include "../tasklist/tasklist.h"
#include "../../list/list.h"
#include <pthread.h>

typedef struct
{
	FDListener listener;
	HList * connection_list;
	pthread_mutex_t mutex_connection_list;
	int epoll;
	pthread_mutex_t mutex_epoll;
	pthread_t thread_listener;
	pthread_t thread_cleaner;

	FDConfig * config;
	FDTaskList * task_list;
	int * run;
}FDConnectionManager;

int fd_connection_manager_init(FDConnectionManager * manager,
								FDConfig * config, FDTaskList * list,
								int * run);
void fd_connection_manager_cleanup(FDConnectionManager * manager);

int fd_connection_manager_run(FDConnectionManager * manager);

int fd_connection_manager_accept(FDConnectionManager * manager);
void fd_connection_manager_clean(FDConnectionManager * manager);
void fd_connection_manager_delete(FDConnectionManager * manager, FDConnection * connection);

#endif /* __FD_CONNECTION_MANAGER_H__ */
