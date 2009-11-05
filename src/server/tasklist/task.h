/* task.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __FD_TASK_H__
#define __FD_TASK_H__

#include "../connection/connection.h"

typedef enum
{
	FD_TASK_TYPE_NULL = 0,
	FD_TASK_TYPE_ACCEPT,
	FD_TASK_TYPE_RECV,
	FD_TASK_TYPE_CLEAN
}FDTaskType;

typedef struct
{
	int type;
	FDConnection * connection;
}FDTask;

FDTask * fd_task_new(FDTaskType type, FDConnection * connection);
void fd_task_delete(FDTask * task);

#endif /* __FD_TASK_H__ */

