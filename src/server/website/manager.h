/* manager.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_WEBSITE_MANAGER_H__
#define __FD_WEBSITE_MANAGER_H__

#include "../config/config.h"
#include "../../list/list.h"
#include "../connection/connection.h"
#include "../http/http.h"
#include <pthread.h>

typedef struct
{
	HList * websites_list;
	pthread_mutex_t mutex_websites_list;
	FDConfig * config;
}FDWebsiteManager;

int fd_website_manager_init(FDWebsiteManager * manager, FDConfig * config);
void fd_website_manager_cleanup(FDWebsiteManager * manager);

int fd_website_manager_dispatch(FDWebsiteManager * manager,
								FDConnection * connection, FDHTTPReqMSGHeader * header);

#endif /* __FD_WEBSITE_MANAGER_H__ */
