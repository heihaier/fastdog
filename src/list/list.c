/* list.c
 * Heihaier - admin@heihaier.org
 */

#include "list.h"
#include "../memory/memory.h"

HList * h_list_new(void)
{
	HList * list = NULL;

	list = malloc(sizeof(HList));
	if(NULL != list)
	{
		list->first = NULL;
		list->last = NULL;
		list->cur = NULL;
	}

	return list;
}

void h_list_delete(HList * list)
{
	if(NULL == list)
	  return;

	if(h_list_first(list) == 0)
	  while(h_list_remove(list) == 0);
	free(list);
}

int h_list_insert(HList * list, void * data)
{
	struct HListNode * node = NULL;

	if(NULL == list)
	  return -1;

	node = malloc(sizeof(struct HListNode));
	if(NULL == node)
	  return -1;
	node->data = data;

	if(NULL == list->cur)
	{
		node->prev = NULL;
		node->next = NULL;
		list->cur = node;
		list->first = node;
		list->last = node;
	}
	else
	{
		if(list->cur == list->first)
		{
			node->prev = NULL;
			node->next = list->first;
			list->first->prev = node;
			list->first = node;
		}
		else
		{
			node->prev = list->cur->prev;
			node->next = list->cur;
			list->cur->prev->next = node;
			list->cur->prev = node;
		}
	}

	return 0;
}

int h_list_append(HList * list, void * data)
{
	struct HListNode * node = NULL;

	if(NULL == list)
	  return -1;

	node = malloc(sizeof(struct HListNode));
	if(NULL == node)
	  return -1;
	node->data = data;
	node->next = NULL;

	if(NULL == list->last)
	{
		list->last = node;
		list->first = node;
		list->cur = node;
		node->prev = NULL;
	}
	else
	{
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	return 0;
}

int h_list_remove(HList * list)
{
	struct HListNode * p = NULL;

	if(NULL == list)
	  return -1;

	if(NULL == list->cur)
	  return -1;

	if(list->cur == list->first)
	{
		if(NULL == list->first->next)
		{
			free(list->first);
			list->first = NULL;
			list->last = NULL;
			list->cur = NULL;
		}
		else
		{
			list->first->next->prev = NULL;
			p = list->first;
			list->first = list->first->next;
			list->cur = list->first;
			free(p);
		}
	}
	else if(list->cur == list->last)
	{
		list->last->prev->next = NULL;
		p = list->last;
		list->last = list->last->prev;
		list->cur = list->cur;
		free(p);
	}
	else
	{
		p = list->cur;
		p->prev->next = p->next;
		p->next->prev = p->prev;
		list->cur = p->next;
		free(p);
	}

	return 0;
}

int h_list_first(HList * list)
{
	if(NULL == list)
	  return -1;

	list->cur = list->first;

	return 0;
}

int h_list_last(HList * list)
{
	if(NULL == list)
	  return -1;

	list->cur = list->last;

	return 0;
}

void * h_list_get(HList * list)
{
	if(NULL == list)
	  return NULL;

	if(NULL == list->cur)
	  return NULL;

	return list->cur->data;
}

int h_list_prev(HList * list)
{
	if(NULL == list)
	  return -1;

	if(NULL == list->cur)
	  return -1;
	
	if(NULL == list->cur->prev)
	  return -1;

	list->cur = list->cur->prev;

	return 0;
}

int h_list_next(HList * list)
{
	if(NULL == list)
	  return -1;

	if(NULL == list->cur)
	  return -1;

	if(NULL == list->cur->next)
	  return -1;

	list->cur = list->cur->next;

	return 0;
}
