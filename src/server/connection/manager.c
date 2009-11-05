/* manager.c
 * Heihaier - admin@heihaier.org
 */

#include "manager.h"
#include "../../debug/debug.h"
#include "../tasklist/task.h"
#include "../tasklist/tasklist.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

void * fd_connection_manager_listener_thread(void * p);
void * fd_connection_manager_cleaner_thread(void * p);

int fd_connection_manager_init(FDConnectionManager * manager,
								FDConfig * config, FDTaskList * list,
								int * run)
{
	if(NULL == manager)
	{
		DBG("fd connection manager init failure: 0\n");
		return -1;
	}

	if(NULL == config)
	{
		DBG("fd connection manager init failure: 1\n");
		return -1;
	}

	if(NULL == list)
	{
		DBG("fd connection manager inti failure: 2\n");
		return -1;
	}

	if(NULL == run)
	{
		DBG("fd connection manager init failure: 3\n");
		return -1;
	}

	struct epoll_event ev;

	manager->config = config;
	manager->task_list = list;
	manager->run = run;

	manager->connection_list = h_list_new();
	if(NULL == manager->connection_list)
	{
		DBG("fd connection manager init failure: 4\n");
		return -1;
	}
	else
	  pthread_mutex_init(&manager->mutex_connection_list, NULL);

	manager->epoll = epoll_create(8);
	if(-1 == manager->epoll)
	{
		DBG("fd connection manager init failure: 5\n");
		h_list_delete(manager->connection_list);
		return -1;
	}

	if(-1 == fd_listener_init(&manager->listener, config))
	{
		DBG("fd connection manager init failure: 6\n");
		return -1;
	}
	else
	{
		ev.events = EPOLLIN | EPOLLET;
		ev.data.ptr = &manager->listener;
		if(epoll_ctl(manager->epoll, EPOLL_CTL_ADD,
						manager->listener.fd, &ev) == -1)
		{
			DBG("fd connection manager init failure; 7\n");
			h_list_delete(manager->connection_list);
			fd_listener_cleanup(&manager->listener);
			return -1;
		}
	}

	return 0;
}

void fd_connection_manager_cleanup(FDConnectionManager * manager)
{
	if(NULL == manager)
	{
		DBG("fd connection manager cleanup failure: 0\n");
		return;
	}

	fd_listener_cleanup(&manager->listener);
	h_list_first(manager->connection_list);
	while(h_list_remove(manager->connection_list) == 0);
	h_list_delete(manager->connection_list);

	pthread_mutex_destroy(&manager->mutex_connection_list);

	close(manager->epoll);
}

int fd_connection_manager_run(FDConnectionManager * manager)
{
	if(NULL == manager)
	{
		DBG("fd connection manager run failure: 0\n");
		return -1;
	}

	if(-1 == fd_listener_listen(&manager->listener))
	{
		DBG("fd connection manager run failure: 1\n");
		return -1;
	}

	pthread_create(&manager->thread_listener, NULL,
				fd_connection_manager_listener_thread, manager);
	pthread_create(&manager->thread_cleaner, NULL,
				fd_connection_manager_cleaner_thread, manager);

	return 0;
}

int fd_connection_manager_accept(FDConnectionManager * manager)
{
	if(NULL == manager)
	{
		DBG("fd connection manager accept failure: 0\n");
		return -1;
	}

	struct epoll_event ev;
	FDConnection * connection = NULL;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int conn_fd = -1;

	memset(&addr, 0, sizeof(addr));
	conn_fd = accept(manager->listener.fd, (struct sockaddr*)&addr,
				&addr_len);
	if(-1 == conn_fd)
	{
		DBG("fd connection manager accept failure: 1\n");
		return -1;
	}

	connection = fd_connection_new();
	if(NULL == connection)
	{
		DBG("fd connection manager accept failure: 2\n");
		return -1;
	}

	connection->fd = conn_fd;
	memmove(&connection->addr, &addr, sizeof(addr));

	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = connection;
	if(epoll_ctl(manager->epoll, EPOLL_CTL_ADD, conn_fd, &ev) == -1)
    {
        DBG("fd connection manager accept failure: 3\n");
        fd_connection_delete(connection);
    }

	pthread_mutex_lock(&manager->mutex_connection_list);
	h_list_append(manager->connection_list, connection);
	pthread_mutex_unlock(&manager->mutex_connection_list);

	return 0;
}

void fd_connection_manager_clean(FDConnectionManager * manager)
{
	if(NULL == manager)
	{
		DBG("fd connection manager clean failure: 0\n");
		return;
	}

	FDConnection * connection = NULL;

	pthread_mutex_lock(&manager->mutex_connection_list);
	h_list_first(manager->connection_list);
	do
	{
		connection = h_list_get(manager->connection_list);
		if(NULL == connection)
		  continue;

		if((time(NULL) - connection->time) > manager->config->timeout)	/* timeout */
		{
			h_list_remove(manager->connection_list);
			epoll_ctl(manager->epoll, EPOLL_CTL_DEL, connection->fd, NULL);
			fd_connection_delete(connection);
		}
	}while(h_list_next(manager->connection_list) == 0);
	pthread_mutex_unlock(&manager->mutex_connection_list);
}

void fd_connection_manager_delete(FDConnectionManager * manager, FDConnection * connection)
{
	if(NULL == manager)
	{
		DBG("fd connection manager delete failure: 0\n");
		return;
	}

	if(NULL == connection)
	{
		DBG("fd connection manager delete failure: 1\n");
		return;
	}

	pthread_mutex_lock(&manager->mutex_connection_list);
	h_list_first(manager->connection_list);
	do
	{
		FDConnection * conn = h_list_get(manager->connection_list);
		if(NULL == conn)
		  continue;

		if(conn == connection)
		{
			h_list_remove(manager->connection_list);
			epoll_ctl(manager->epoll, EPOLL_CTL_DEL, connection->fd, NULL);
			fd_connection_delete(connection);
		}
	}while(h_list_next(manager->connection_list) == 0);
	pthread_mutex_unlock(&manager->mutex_connection_list);
}

void * fd_connection_manager_listener_thread(void * p)
{
	FDConnectionManager * manager = p;
	struct epoll_event events[64];
	int nfds = 0, i = 0;
	FDTask * task = NULL;

	while(1 == *manager->run)
	{
		nfds = epoll_wait(manager->epoll, events, 64, 1000);

		if(-1 == nfds)
		  DBG("epoll wait failure\n");

		for(i=0; i<nfds; i++)
		{
			if(events[i].data.ptr == &manager->listener)	/* listener */
			{
				/* new task, accept connection */
				task = fd_task_new(FD_TASK_TYPE_ACCEPT, events[i].data.ptr);
				if(NULL != task)
				  fd_tasklist_add(manager->task_list, task);
			}
			else
			{
				/* new task, recv data */
				task = fd_task_new(FD_TASK_TYPE_RECV, events[i].data.ptr);
				if(NULL != task)
				  fd_tasklist_add(manager->task_list, task);
			}
		}
	}

	return NULL;
}

void * fd_connection_manager_cleaner_thread(void * p)
{
	FDConnectionManager * manager = p;
	FDTask * task = NULL;

	while(1 == *manager->run)
	{
		task = fd_task_new(FD_TASK_TYPE_CLEAN, NULL);
		if(NULL != task)
		  fd_tasklist_add(manager->task_list, task);

		sleep(4);
	}

	return NULL;
}

