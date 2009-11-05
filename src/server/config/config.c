/* config.c
 * Heihaier - admin@heihaier.org
 */

#include "config.h"
#include "../../debug/debug.h"
#include "../../memory/memory.h"

int fd_config_init(FDConfig * config)
{
	if(NULL == config)
	{
		DBG("fd config init failure: 0\n");
		return -1;
	}
	config->port = 8080;
	config->timeout = 10;
	config->website_path = "./websites/";

	return 0;
}

void fd_config_cleanup(FDConfig * config)
{
	if(NULL == config)
	{
		DBG("fd config cleanup failure: 0\n");
		return;
	}
}

