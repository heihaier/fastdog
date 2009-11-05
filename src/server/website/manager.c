/* manager.c
 * Heihaier - admin@heihaier.org
 */

#include "manager.h"
#include "website.h"
#include "../config/config.h"
#include "../../debug/debug.h"
#include <dirent.h>
#include <string.h>

int fd_website_manager_init(FDWebsiteManager * manager, FDConfig * config)
{
	if(NULL == manager)
	{
		DBG("fd website manager init failure: 0\n");
		return -1;
	}

	if(NULL == config)
	{
		DBG("fd website manager init failure: 1\n");
		return -1;
	}

	manager->config = config;

	DIR * dir_info = NULL;
	struct dirent * dir_entry = NULL;
	FDWebsite * website = NULL;
	char path[4096];

	manager->websites_list = h_list_new();
	if(NULL == manager->websites_list)
	{
		DBG("fd website manager init failure: 2\n");
		return -1;
	}

	pthread_mutex_init(&manager->mutex_websites_list,
						NULL);

	dir_info = opendir(config->website_path);
	if(NULL == dir_info)
	{
		DBG("fd website manager init failure: 3\n");
		return -1;
	}

	while((dir_entry = readdir(dir_info)) != NULL)
	{
		if(dir_entry->d_type != 0x04)	/* type == 0x04 is directory */
		  continue;

		if((strcmp(dir_entry->d_name, "..") == 0) ||
					(strcmp(dir_entry->d_name, ".") == 0) ||
					(dir_entry->d_name[0] == '.'))
		  continue;

		sprintf(path, "%s/%s/website.so", manager->config->website_path,
					dir_entry->d_name);
		website = fd_website_new(path);
		if(NULL != website)
		{
			memset(website->host, 0, 256);
			sprintf(website->host, "%s:%d", dir_entry->d_name,
						manager->config->port);
			h_list_append(manager->websites_list, website);
		}
	}
	closedir(dir_info);

	return 0;
}

void fd_website_manager_cleanup(FDWebsiteManager * manager)
{
	if(NULL == manager)
	  DBG("fd website manager cleanup failure: 0\n");

	FDWebsite * website = NULL;

	pthread_mutex_lock(&manager->mutex_websites_list);
	h_list_first(manager->websites_list);
	do
	{
		website = h_list_get(manager->websites_list);
		if(NULL != website)
		  fd_website_delete(website);
	}while(h_list_remove(manager->websites_list) == 0);
	pthread_mutex_unlock(&manager->mutex_websites_list);

	h_list_delete(manager->websites_list);
	pthread_mutex_destroy(&manager->mutex_websites_list);
}

int fd_website_manager_dispatch(FDWebsiteManager * manager,
								FDConnection * connection,
								FDHTTPReqMSGHeader * header)
{
	if(NULL == manager)
	{
		DBG("fd website manager dispatch failure: 0\n");
		return -1;
	}

	if(NULL == connection)
	{
		DBG("fd webiste manager disptach failure: 1\n");
		return -1;
	}

	if(NULL == header)
	{
		DBG("fd website manager dispatch failure: 2\n");
		return -1;
	}

	FDWebsite * website = NULL;
	fd_connection_lock(connection);
	
	pthread_mutex_lock(&manager->mutex_websites_list);
	h_list_first(manager->websites_list);
	do
	{
		FDWebsite * p = h_list_get(manager->websites_list);
		if((NULL != p) &&
					(memcmp(p->host, header->host, 256) == 0))
		{
			website = p;
			break;
		}
	}while(h_list_next(manager->websites_list) == 0);
	pthread_mutex_unlock(&manager->mutex_websites_list);

	if(NULL == website)
	  return 1;	/* unknown host */

	return fd_website_handle(website, connection, header);
}
