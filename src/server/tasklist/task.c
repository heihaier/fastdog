/* task.c
 * Heihaier - admin@heihaier.org
 */

#include "task.h"
#include "../../debug/debug.h"
#include "../../memory/memory.h"
#include <string.h>

FDTask * fd_task_new(FDTaskType type, FDConnection * connection)
{
	FDTask * task = NULL;

	task = malloc(sizeof(FDTask));
	if(task)
	{
		memset(task, 0, sizeof(FDTask));
		task->type = type;
		task->connection = connection;

		return task;
	}

	return NULL;
}

void fd_task_delete(FDTask * task)
{
	free(task);
}

