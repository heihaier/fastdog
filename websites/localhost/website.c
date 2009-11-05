/* website.c
 * Heihaier - admin@heihaier.org
 */

#include <stdio.h>
#include "../../src/server/website/interface/interface.h"

static FDWebsiteInterface * interface;

int website_init(void * p)
{
	interface = p;
	return 0;
}

void website_cleanup(void)
{
}

int website_handle(void * p)
{
	interface->write(p, "hello", 5);

	return 0;
}
