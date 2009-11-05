/* website.c
 * Heihaier - admin@heihaier.org
 */

#include "website.h"
#include "../../debug/debug.h"
#include "../../memory/memory.h"
#include "../http/http.h"
#include <dlfcn.h>
#include <string.h>

void * fd_website_handle_thread(void * p);

FDWebsite * fd_website_new(char * path)
{
	if(NULL == path)
	{
		DBG("fd website new failure: 0\n");
		return NULL;
	}

	FDWebsite * website = NULL;

	website = malloc(sizeof(FDWebsite));
	if(NULL == website)
	{
		DBG("fd website new failure: 1\n");
		return NULL;
	}

	memset(website, 0, sizeof(FDWebsite));
	website->website_init = NULL;
	website->website_cleanup = NULL;
	website->website_handle = NULL;

	if(-1 == fd_website_interface_init(&website->interface))
	{
		DBG("fd website new failure; 2\n");
		free(website);
		return NULL;
	}

	website->dl = dlopen(path, RTLD_LAZY);
	if(NULL == website->dl)
	{
		DBG("fd website new failure: 3\n");
		free(website);
		return NULL;
	}
	
	website->website_init = dlsym(website->dl,
				"website_init");
	website->website_cleanup = dlsym(website->dl,
				"website_cleanup");
	website->website_handle = dlsym(website->dl,
				"website_handle");

	if((NULL == website->website_init) ||
		(NULL == website->website_cleanup) || 
		(NULL == website->website_handle))
	{
		DBG("fd website new failure: 4\n");
		free(website);
		return NULL;
	}

	website->website_init(&website->interface);

	return website;
}

void fd_website_delete(FDWebsite * website)
{
	if(NULL == website)
	{
		DBG("fd website delete failure: 0\n");
		return;
	}

	website->website_cleanup();
	dlclose(website->dl);
	free(website);
}

int fd_website_handle(FDWebsite * website, FDConnection * connection,
						FDHTTPReqMSGHeader * header)
{
	if(NULL == website)
	{
		DBG("fd website handle failure: 0\n");
		return -1;
	}
	if(NULL == connection)
	{
		DBG("fd website handle failure: 1\n");
		return -1;
	}
	if(NULL == header)
	{
		DBG("fd website handle failure: 2\n");
		return -1;
	}

	FDWebsiteInterfaceMSG * msg = NULL;
	FDHTTPReqMSGHeader * reqmsg_header = NULL;

	msg = malloc(sizeof(FDWebsiteInterfaceMSG));
	if(NULL == msg)
	{
		DBG("fd website handle failure: 3\n");
		return -1;
	}
	memset(msg, 0, sizeof(FDWebsiteInterfaceMSG));

	reqmsg_header = malloc(sizeof(FDHTTPReqMSGHeader));
	if(NULL == reqmsg_header)
	{
		DBG("fd website handle failure: 4\n");
		free(msg);
		return -1;
	}
	memmove(reqmsg_header, header, sizeof(FDHTTPReqMSGHeader));

	msg->reqmsg_header = reqmsg_header;
	msg->connection = connection;
	msg->website = website;

	pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&connection->thread, &attr,
				fd_website_handle_thread, msg);
	pthread_attr_destroy(&attr);

	return 0;
}

void * fd_website_handle_thread(void * p)
{
	FDWebsiteInterfaceMSG * msg = p;
	FDWebsite * website = msg->website;
	FDConnection * connection = msg->connection;

	website->website_handle(msg);

	fd_connection_unlock(connection);
	free(msg->reqmsg_header);
	free(msg);

	return NULL;
}

