/* list.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __H_LIST_H__
#define __H_LIST_H__

struct HListNode
{
	struct HListNode * prev;
	struct HListNode * next;
	void * data;
};

typedef struct
{
	struct HListNode * first;
	struct HListNode * last;
	struct HListNode * cur;
}HList;

HList * h_list_new(void);
void h_list_delete(HList * list);

int h_list_insert(HList * list, void * data);
int h_list_append(HList * list, void * data);
int h_list_remove(HList * list);

int h_list_first(HList * list);
int h_list_last(HList * list);
void * h_list_get(HList * list);

int h_list_prev(HList * list);
int h_list_next(HList * list);

#endif /* __H_LIST_H__ */
