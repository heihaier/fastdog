/* fastdog.c
 * Heihaier - admin@heihaier.org
 */

#include "fastdog.h"
#include "daemon/daemon.h"
#include "server/server.h"
#include "debug/debug.h"
#include "memory/memory.h"
#include <signal.h>

FDServer server;

int main(int argc, const char * const argv[])
{
	DBG("%s/%s debug mode.\n", FASTDOG_APPNAME, FASTDOG_VERSION);

	H_MEMORY_INIT;

    signal(SIGTERM, reset_run);
    signal(SIGHUP, reset_run);
    signal(SIGINT, reset_run);
    signal(SIGQUIT, reset_run);
    signal(SIGABRT, reset_run);
	signal(SIGPIPE, SIG_IGN);

#ifndef __H_DEBUG__
	fd_daemon_init();
#endif /* __H_DEBUG__ */

	if(0 == fd_server_init(&server))
	{
		if(-1 == fd_server_run(&server))
		  DBG("fd server run failure\n");

		fd_server_cleanup(&server);
	}

#ifndef __H_DEBUG__
	fd_daemon_cleanup();
#endif /* __H_DEBUG__ */

	H_MEMORY_CLEANUP;

	return 0;
}

void reset_run(int i)
{
	server.run = 0;
}

