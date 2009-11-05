/* interface.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_INTERFACE_H__
#define __FD_INTERFACE_H__

#include <sys/types.h>

typedef struct
{
	void * reqmsg_header;
	void * connection;
	void * website;
}FDWebsiteInterfaceMSG;

typedef struct
{
	char * (*get)(FDWebsiteInterfaceMSG * msg, const int n);
	ssize_t (*read)(FDWebsiteInterfaceMSG * msg, void * buffer, size_t size);
	ssize_t (*write)(FDWebsiteInterfaceMSG * msg, void * buffer, size_t size);
}FDWebsiteInterface;

int fd_website_interface_init(FDWebsiteInterface * interface);
void fd_website_interface_cleanup(FDWebsiteInterface * interface);

#endif /* __FD_INTERFACE_H__ */

