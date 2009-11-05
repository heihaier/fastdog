/* server.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_SERVER_H__
#define __FD_SERVER_H__

#include "config/config.h"
#include "connection/manager.h"
#include "website/manager.h"
#include "tasklist/tasklist.h"
#include <pthread.h>

#define FD_SERVER_THREAD_WORKERS	8

typedef struct
{
	int run;
	FDConfig config;
	FDTaskList task_list;
	FDConnectionManager connection_manager;
	FDWebsiteManager website_manager;
	pthread_t thread_workers[FD_SERVER_THREAD_WORKERS];
}FDServer;

int fd_server_init(FDServer * server);
void fd_server_cleanup(FDServer * server);

int fd_server_run(FDServer * server);

#endif /* __FD_SERVER_H__ */
