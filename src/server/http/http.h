/* http.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_HTTP_H__
#define __FD_HTTP_H__

#include <stdlib.h>

typedef struct
{
	char host[256];
	char server_software[64];
	char server_name[64];
	int interface;

	char server_protocol[16];
	int server_port;
	int request_method;
	char query_string[512];
	char remote_host[64];
	char remote_addr[16];
	int auth_type;
	char remote_ident[64];
	int content_type;
	size_t content_length;

	int http_accept[64];
	char http_user_agent[256];
}FDHTTPReqMSGHeader;

typedef struct
{
	char server_protocol[16];
	int status;
}FDHTTPResMSGHeader;

int fd_http_reqmsg_header_init(FDHTTPReqMSGHeader * header,
						void * buffer, size_t * size);

int fd_http_resmsg_header_init(void * buffer, size_t * size,
						FDHTTPResMSGHeader * header);

int fd_http_message(void * buffer, size_t * size, int status);

#endif /* __FD_HTTP_H__ */
