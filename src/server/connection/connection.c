/* connection.c
 * Heihaier - admin@heihaier.org
 */

#include "connection.h"
#include "../../debug/debug.h"
#include "../../memory/memory.h"
#include <string.h>

FDConnection * fd_connection_new(void)
{
	FDConnection * connection = NULL;

	connection = malloc(sizeof(FDConnection));
	if(connection)
	{
		memset(connection, 0, sizeof(FDConnection));
		time(&connection->time);
		connection->locker = 0;
		pthread_mutex_init(&connection->mutex, NULL);

		return connection;
	}

	return NULL;
}

void fd_connection_delete(FDConnection * connection)
{
	if(NULL == connection)
	{
		DBG("fd connection delete failure: 0\n");
		return;
	}

	if(1 == connection->locker)
	  pthread_cancel(connection->thread);

	pthread_mutex_destroy(&connection->mutex);
	close(connection->fd);
	free(connection);
}

int fd_connection_send(FDConnection * connection,
			void * buffer, size_t size)
{
	if(NULL == connection)
	{
		DBG("fd connection send failure: 0\n");
		return -1;
	}

	return send(connection->fd, buffer, size, 0);
}

int fd_connection_recv(FDConnection * connection,
			void * buffer, size_t size)
{
	if(NULL == connection)
	{
		DBG("fd connection recv failure: 0\n");
		return -1;
	}

	return recv(connection->fd, buffer, size, 0);
}

int fd_connection_recv_peek(FDConnection * connection,
			void * buffer, size_t size)
{
	if(NULL == connection)
	{
		DBG("fd connection recv peek failure: 0\n");
		return -1;
	}

	return recv(connection->fd, buffer, size, MSG_PEEK);
}

void fd_connection_update(FDConnection * connection)
{
	if(NULL == connection)
	{
		DBG("fd connection update failure: 0\n");
		return;
	}

	time(&connection->time);
}

void fd_connection_lock(FDConnection * connection)
{
	if(NULL == connection)
	{
		DBG("fd connection lock failure: 0\n");
		return;
	}

	pthread_mutex_lock(&connection->mutex);
	connection->locker = 1;
	pthread_mutex_unlock(&connection->mutex);
}

void fd_connection_unlock(FDConnection * connection)
{
	if(NULL == connection)
	{
		DBG("fd connection lock failure: 0\n");
		return;
	}

	pthread_mutex_lock(&connection->mutex);
	connection->locker = 0;
	pthread_mutex_unlock(&connection->mutex);

}

