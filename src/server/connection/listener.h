/* listener.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __H_LISTENER_H__
#define __H_LISTENER_H__

#include "../config/config.h"

typedef struct
{
	int fd;
}FDListener;

int fd_listener_init(FDListener * listener, FDConfig * config);
void fd_listener_cleanup(FDListener * listener);

int fd_listener_listen(FDListener * listener);

#endif /* __H_LISTENER_H__ */
