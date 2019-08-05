#include <stdlib.h>

#include "hashtable_iterator.h"

typedef struct
{
	int idx;
	hashnode_p node;
} cursor_t;

char  hashtable_iter_has_next(iterator_p iter);

void* hashtable_iter_next(iterator_p iter);

void* hashtable_iter_get(iterator_p iter);

void* hashtable_iter_set(iterator_p iter, void* data);

void* hashtable_iter_remove(iterator_p iter);

void hashtable_iter_destroy(void* iter);

//----------------------------------------------------
iterator_p hashtable_iter_make(hashtable_p table)
{
	iterator_p ret = (iterator_p) malloc(sizeof(iterator_t));
	hashtable_iter_init(ret, table);
	return ret;
}

//----------------------------------------------------
void hashtable_iter_init(iterator_p iter, hashtable_p table)
{	
	cursor_t *cur = malloc(sizeof(cursor_t));
	cur->idx = -1; cur->node = NULL;

	iter->collection = table;
	iter->current = cur;

	iter->has_next 	= &hashtable_iter_has_next;
	iter->next 		= &hashtable_iter_next;
	iter->get 		= &hashtable_iter_get;
	iter->set 		= &hashtable_iter_set;
	iter->remove 	= &hashtable_iter_remove;
	iter->destroy	= &hashtable_iter_destroy;

	hashtable_iter_next(iter);
}

void hashtable_iter_destroy(void* i)
{
	iterator_p iter = i;
	free(iter->current);
}

//----------------------------------------------------
char hashtable_iter_has_next(iterator_p iter)
{
	cursor_t *cur = iter->current;
	return cur->node != NULL;
}

//----------------------------------------------------
void* hashtable_iter_next(iterator_p iter)
{
	hashtable_p table = iter->collection;
	cursor_t *cur = iter->current;

	void* data = NULL;
	
	if (cur->node != NULL)
	{
		data = cur->node->data;
		cur->node = cur->node->next;
	}

	if (cur->node == NULL)
	{
		do
		{
			cur->idx += 1;
		}
		while (cur->idx < table->table_size && table->array[cur->idx] == NULL);

		if (cur->idx < table->table_size)
		{
			cur->node = table->array[cur->idx];
		}
	}

	return data;
}

//----------------------------------------------------
void* hashtable_iter_get(iterator_p iter)
{
	cursor_t *cur = iter->current;

	if (cur->node != NULL)
	{
		return cur->node->data;
	}

	return NULL;
}

//----------------------------------------------------
void* hashtable_iter_set(iterator_p iter, void* data)
{
	cursor_t *cur = iter->current;

	void *ret = hashtable_iter_get(iter);

	if (cur->node != NULL)
	{
		cur->node->data = data;
	}

	return ret;
}

//----------------------------------------------------
void* hashtable_iter_remove(iterator_p iter)
{
	// UNIMPLEMENTED
	return NULL;
}