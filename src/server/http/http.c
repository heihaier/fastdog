/* http.c
 * Heihaier - admin@heihaier.org
 */

#include "http.h"
#include "../../debug/debug.h"
#include <string.h>

int fd_http_reqmsg_header_init(FDHTTPReqMSGHeader * header,
						void * buffer, size_t * size)
{
	memset(header->host, 0, 256);
	memmove(header->host, "localhost:8080", 16);
	*size = 0;

	return 0;
}

int fd_http_resmsg_header_init(void * buffer, size_t * size,
						FDHTTPResMSGHeader * header)
{
	return -1;
}

int fd_http_message(void * buffer, size_t * size, int status)
{
	return 0;
}
