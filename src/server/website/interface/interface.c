/* interface.h
 * Heihaier - admin@heihaier.org
 */

#include "interface.h"
#include "../../../debug/debug.h"
#include "../../connection/connection.h"

char * fd_website_interface_get(FDWebsiteInterfaceMSG * msg, const int n);
ssize_t fd_website_interface_read(FDWebsiteInterfaceMSG * msg,
			void * buffer, size_t size);
ssize_t fd_website_interface_write(FDWebsiteInterfaceMSG * msg,
			void * buffer, size_t size);

int fd_website_interface_init(FDWebsiteInterface * interface)
{
	if(NULL == interface)
	{
		DBG("fd website interface init failure: 0\n");
		return -1;
	}

	interface->get = fd_website_interface_get;
	interface->read = fd_website_interface_read;
	interface->write = fd_website_interface_write;

	return 0;
}

void fd_website_interface_cleanup(FDWebsiteInterface * interface)
{
}

char * fd_website_interface_get(FDWebsiteInterfaceMSG * msg, const int n)
{
}

ssize_t fd_website_interface_read(FDWebsiteInterfaceMSG * msg,
			void * buffer, size_t size)
{
	FDConnection * connection = msg->connection;
	if(NULL == connection)
	{
		DBG("fd website interface read failure: 0\n");
		return -1;
	}

	return fd_connection_recv(connection, buffer, size);
}

ssize_t fd_website_interface_write(FDWebsiteInterfaceMSG * msg,
			void * buffer, size_t size)
{
	FDConnection * connection = msg->connection;
	if(NULL == connection)
	{
		DBG("fd website interface read failure: 0\n");
		return -1;
	}

	return fd_connection_send(connection, buffer, size);
}

