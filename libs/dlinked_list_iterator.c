#include <stdlib.h>
#include "dlinked_list_iterator.h"

//----------------------------------------------------
iterator_p dlinked_list_iter_make(dlinked_list_p list)
{
	iterator_p ret = (iterator_p) malloc(sizeof(iterator_t));
	dlinked_list_iter_init(ret, list);
	return ret;
}

//----------------------------------------------------
void dlinked_list_iter_init(iterator_p iter, dlinked_list_p list)
{
	iter->collection = list;
	iter->current = list->head;
	
	iter->has_next 	= &dlinked_list_iter_has_next;
	iter->next 		= &dlinked_list_iter_next;
	iter->get 		= &dlinked_list_iter_get;
	iter->set 		= &dlinked_list_iter_set;
	iter->remove 	= &dlinked_list_iter_remove;
}

//----------------------------------------------------
char dlinked_list_iter_has_next(iterator_p iter)
{
	return iter->current != NULL;
}

//----------------------------------------------------
void* dlinked_list_iter_next(iterator_p iter)
{
	if ( iter->current == NULL )
		return NULL;
	
	dlinked_list_node_p curr = iter->current;
	
	void* ret = curr->data;
	iter->current = curr->next;
	
	return ret;
}

//----------------------------------------------------
void* dlinked_list_iter_get(iterator_p iter)
{
	if ( iter->current == NULL )
		return NULL;
	
	dlinked_list_node_p curr = iter->current;
	return curr->data;
}

//----------------------------------------------------
void* dlinked_list_iter_set(iterator_p iter, void* data)
{
	if ( iter->current == NULL )
		return NULL;
	
	dlinked_list_node_p curr = iter->current;
	
	void* ret = curr->data;
	curr->data = data;
	
	return ret;
}

//----------------------------------------------------
void* dlinked_list_iter_remove(iterator_p iter)
{
	if ( iter->current == NULL )
		return NULL;
		
	dlinked_list_node_p curr = iter->current;
	
	if ( curr->prev == NULL )
		return NULL;
	
	void* ret = curr->prev->data;
	iter->current = curr->next;
	dlinked_list_remove_node(iter->collection, curr->prev->data);
	
	return ret;
}