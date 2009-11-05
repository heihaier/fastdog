/* listener.c
 * Heihaier - admin@heihaier.org
 */

#include "listener.h"
#include "../../debug/debug.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

int fd_listener_init(FDListener * listener, FDConfig * config)
{
	if(NULL == listener)
	{
		DBG("fd listener init failure: 0\n");
		return -1;
	}

	if(NULL == config)
	{
		DBG("fd listener init failure: 1\n");
		return -1;
	}

	struct sockaddr_in addr;

	listener->fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listener->fd < 0)
	{
		DBG("fd listener init failure: 2\n");
		return -1;
	}

	fcntl(listener->fd, F_SETFL, O_NONBLOCK);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(config->port);

	if(bind(listener->fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		DBG("fd listener init failure: 3\n");
		close(listener->fd);
		return -1;
	}

	return 0;
}

void fd_listener_cleanup(FDListener * listener)
{
	if(NULL == listener)
	{
		DBG("fd listener cleanup failure: 0\n");
		return;
	}

	close(listener->fd);
}

int fd_listener_listen(FDListener * listener)
{
	if(NULL == listener)
	{
		DBG("fd listener listen failure: 0\n");
		return -1;
	}

	if(-1 == listen(listener->fd, 10))
	{
		DBG("fd listener listen failure: 1\n");
		return -1;
	}

	return 0;
}
