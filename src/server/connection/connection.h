/* connection.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_CONNECTION_H__
#define __FD_CONNECTION_H__

#include <netinet/in.h>
#include <time.h>
#include <pthread.h>

typedef struct
{
	int fd;
	struct sockaddr_in addr;
	time_t time;
	pthread_t thread;
	pthread_mutex_t mutex;
	int locker;
	void * p;
}FDConnection;

FDConnection * fd_connection_new(void);
void fd_connection_delete(FDConnection * connection);

int fd_connection_send(FDConnection * connection,
			void * buffer, size_t size);
int fd_connection_recv(FDConnection * connection,
			void * buffer, size_t size);
int fd_connection_recv_peek(FDConnection * connection,
			void * buffer, size_t size);

void fd_connection_update(FDConnection * connection);
void fd_connection_lock(FDConnection * connection);
void fd_connection_unlock(FDConnection * connection);

#endif /* __FD_CONNECITON_H__ */
