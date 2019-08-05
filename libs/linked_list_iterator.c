#include <stdlib.h>
#include <assert.h>
#include "linked_list_iterator.h"

//----------------------------------------------------
iterator_p linked_list_iter_make(linked_list_p list)
{
	iterator_p ret = (iterator_p) malloc(sizeof(iterator_t));
	linked_list_iter_init(ret, list);
	return ret;
}

//----------------------------------------------------
void linked_list_iter_init(iterator_p iter, linked_list_p list)
{
	iter->collection = list;
	iter->current = list->head;
	
	iter->has_next 	= &linked_list_iter_has_next;
	iter->next 		= &linked_list_iter_next;
	iter->get 		= &linked_list_iter_get;
	iter->set 		= &linked_list_iter_set;
	iter->remove 	= &linked_list_iter_remove;
}

//----------------------------------------------------
char linked_list_iter_has_next(iterator_p iter)
{
	return iter->current != NULL;
}

//----------------------------------------------------
void* linked_list_iter_next(iterator_p iter)
{
	assert(iter->current != NULL);
	
	list_node_p curr = iter->current;
	
	void* ret = curr->data;
	iter->current = curr->next;
	
	return ret;
}

//----------------------------------------------------
void* linked_list_iter_get(iterator_p iter)
{
	assert(iter->current != NULL);
	
	list_node_p curr = iter->current;
	return curr->data;
}

//----------------------------------------------------
void* linked_list_iter_set(iterator_p iter, void* data)
{
	assert(iter->current != NULL);
	
	list_node_p curr = iter->current;
	
	void* ret = curr->data;
	curr->data = data;
	
	return ret;
}

//----------------------------------------------------
void* linked_list_iter_remove(iterator_p iter)
{
	assert(iter->current != NULL);
	assert(iter->current->previous != NULL);
		
	list_node_p curr = iter->current;
	
	void* ret = curr->previous->data;
	iter->current = curr->next;
	return list_remove_node(iter->collection, curr->prev->data);
	
	return ret;
}