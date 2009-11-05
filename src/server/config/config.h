/* config.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_CONFIG_H__
#define __FD_CONFIG_H__

typedef struct
{
	int port;
	int timeout;
	char * website_path;
}FDConfig;

int fd_config_init(FDConfig * config);
void fd_config_cleanup(FDConfig * config);

#endif /* __FD_CONFIG_H__ */

