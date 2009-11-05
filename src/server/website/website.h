/* website.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_WEBSITE_H__
#define __FD_WEBSITE_H__

#include "../connection/connection.h"
#include "../http/http.h"
#include "interface/interface.h"

typedef struct
{
	char host[256];
	void * dl;

	int (*website_init)(void * interface);
	void (*website_cleanup)(void);
	int (*website_handle)(void * msg);

	FDWebsiteInterface interface;
}FDWebsite;

FDWebsite * fd_website_new(char * path);
void fd_website_delete(FDWebsite * website);

int fd_website_handle(FDWebsite * website, FDConnection * connection,
						FDHTTPReqMSGHeader * header);

#endif /* __FD_WEBSITE_H__ */
