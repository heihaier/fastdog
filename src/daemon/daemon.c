/* daemon.c
 * Heihaier - admin@heihaier.org
 */

#include "daemon.h"

#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

void fd_daemon_init(void)
{
        int pid = 0;
        int i = 0;

        if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		  exit(EXIT_FAILURE);
        if(pid = fork())
          exit(EXIT_SUCCESS);
        else if(pid < 0)
          exit(EXIT_FAILURE);
        setsid();
        if(pid = fork())
          exit(EXIT_SUCCESS);
        else if(pid < 0)
          exit(EXIT_FAILURE);

        for(i=0; i<NOFILE; i++)
          close(i);
        umask(0);
}

void fd_daemon_cleanup(void)
{
}

