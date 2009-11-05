/* server.c
 * Heihaier - admin@heihaier.org
 */

#include "server.h"
#include "../debug/debug.h"
#include "tasklist/task.h"
#include "tasklist/tasklist.h"
#include "http/http.h"
#include <string.h>

void * fd_server_worker_thread(void * p);

int fd_server_init(FDServer * server)
{
	if(NULL == server)
	{
		DBG("fd server init failure: 0\n");
		return -1;
	}

	if(-1 == fd_config_init(&server->config))
	{
		DBG("fd server init failure: 1\n");
		return -1;
	}

	if(-1 == fd_tasklist_init(&server->task_list))
	{
		DBG("fd server init failure: 2\n");
		return -1;
	}

	if(-1 == fd_website_manager_init(&server->website_manager, &server->config))
	{
		DBG("fd server init failure: 3\n");
		return -1;
	}

	if(-1 == fd_connection_manager_init(&server->connection_manager,
											&server->config, &server->task_list,
											&server->run))
	{
		DBG("fd server init failure: 4\n");
		return -1;
	}

	return 0;
}

void fd_server_cleanup(FDServer * server)
{
	if(NULL == server)
	{
		DBG("fd server cleanup failure: 0\n");
		return;
	}

	fd_tasklist_cleanup(&server->task_list);
	fd_connection_manager_cleanup(&server->connection_manager);
	fd_website_manager_cleanup(&server->website_manager);
	fd_config_cleanup(&server->config);
}

int fd_server_run(FDServer * server)
{
	if(NULL == server)
	{
		DBG("fd server run failure: 0\n");
		return -1;
	}

	server->run = 1;

	if(-1 == fd_connection_manager_run(&server->connection_manager))
	{
		DBG("fd server run failure: 1\n");
		return -1;
	}

	void * p = NULL;
	int i = 0;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for(i=0; i<FD_SERVER_THREAD_WORKERS; i++)
    {
        pthread_create(&server->thread_workers[i], &attr,
					fd_server_worker_thread, server);
    }
    pthread_attr_destroy(&attr);

    pthread_join(server->connection_manager.thread_listener, &p);
    pthread_join(server->connection_manager.thread_cleaner, &p);

	return 0;
}

void * fd_server_worker_thread(void * p)
{
	FDServer * server = p;
	FDTask * task = NULL;
	static char buffer[4096];
	ssize_t size = 0;
	FDHTTPReqMSGHeader reqmsg_header;

	if(NULL == p)
	{
		DBG("fd server worker thread failure: 0\n");
		return NULL;
	}

	while(1 == server->run)
	{
		memset(buffer, 0, 4096);
		memset(&reqmsg_header, 0, sizeof(FDHTTPReqMSGHeader));

		if(-1 == fd_tasklist_rmv(&server->task_list, &task))
		  continue;
		if(NULL != task)
		{
			switch(task->type)
			{
			case FD_TASK_TYPE_ACCEPT:
				fd_connection_manager_accept(&server->connection_manager);
				break;
			case FD_TASK_TYPE_RECV:
				if(1 == task->connection->locker)	/* receive data when unlock. */
				  continue;

				size = fd_connection_recv_peek(task->connection, buffer, 4096);
				if(0 < size)
				{	
					int n = fd_http_reqmsg_header_init(&reqmsg_header, buffer, &size);
					switch(n)
					{
					case 0:	/* ok */
						fd_connection_update(task->connection);
						fd_connection_recv(task->connection, buffer, size);
						int i = fd_website_manager_dispatch(&server->website_manager, task->connection,
									&reqmsg_header);
						switch(i)
						{
						case 1:	/* unknown host */
							break;
						case -1:	/* error */
							break;
						}
						break;
					case 1:	/* It isn't complete. */
						fd_connection_update(task->connection);
						break;
					case -1:	/* error */
						fd_connection_manager_delete(&server->connection_manager,
									task->connection);
						break;
					}
				}
				else if(0 == size)
				{
					fd_connection_manager_delete(&server->connection_manager,
								task->connection);
				}
				break;
			case FD_TASK_TYPE_CLEAN:
				fd_connection_manager_clean(&server->connection_manager);
				break;
			}

			fd_task_delete(task);	
		}
	}

	return NULL;
}

