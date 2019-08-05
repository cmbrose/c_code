/*
 *	FILE: linked_list.c
 *	AUTHOR: Caleb Brose
 *
 *	USE: Creates a generic doubly linked_list type
 *		and gives the usual list operators.
 */

#include <stdlib.h>
#include <string.h>
#include "dlinked_list.h"

//----------------------------------------------------
dlinked_list_node_p dlinked_list_node_make(void* data)
{
	dlinked_list_node_p ret = (dlinked_list_node_p) malloc(sizeof(dlinked_list_node_t));
	dlinked_list_node_init(ret, data);
	return ret;
}

//----------------------------------------------------
void dlinked_list_node_init(dlinked_list_node_p n, void* data)
{
	if ( !n ) return;
    n->data = data;
    n->next = n->prev = NULL;
}

//----------------------------------------------------
void dlinked_list_node_destroy(dlinked_list_node_p n)
{
	if ( n )
		free(n);
}

//----------------------------------------------------
dlinked_list_p dlinked_list_make(
	char (*equal_check)(void*, void*), 
	char* (*node_printer)(void*),
	void (*node_dealloc)(void*)
	)
{
	dlinked_list_p ret = (dlinked_list_p) malloc(sizeof(dlinked_list_t));
	dlinked_list_init(ret, equal_check, node_printer, node_dealloc);
	return ret;
}

//----------------------------------------------------
void dlinked_list_init(
	dlinked_list_p list, 
	char (*equal_check)(void*, void*), 
	char* (*node_printer)(void*),
	void (*node_dealloc)(void*)	
	)
{
	if ( !list ) return;
	list->size = 0;
    list->head = list->tail = NULL;
	list->equality = equal_check;
	list->n_print = node_printer;
	list->n_dealloc = node_dealloc;
}

//----------------------------------------------------
void dlinked_list_clear(dlinked_list_p list)
{
	if ( !list ) return;
	
	dlinked_list_node_p curr = list->head;
	while ( curr )
	{
		dlinked_list_node_p tmp = curr;
		curr = curr->next;
		
		if ( list->n_dealloc )
			list->n_dealloc(tmp->data);
		
		dlinked_list_node_destroy(tmp);
	}
	
	list->size = 0;
    list->head = list->tail = NULL;
}

//----------------------------------------------------
void dlinked_list_destroy(dlinked_list_p list)
{
	free(list);
}

//----------------------------------------------------
void dlinked_list_append(dlinked_list_p list, void* data)
{
	dlinked_list_node_p new_n = dlinked_list_node_make(data);
	
    if ( list->tail ) list->tail->next = new_n;
    else list->head = new_n;

	new_n->prev = list->tail;
    list->tail = new_n;
	
	list->size = list->size + 1;
}

//----------------------------------------------------
void* dlinked_list_remove(dlinked_list_p list, void* to_rem)
{	
    dlinked_list_node_p n = dlinked_list_node_search(list, to_rem);
	
	if ( !n ) return NULL; // Not found
	
	dlinked_list_remove_node(list, n);
	
	return n->data;
}

//----------------------------------------------------
void* dlinked_list_search(dlinked_list_p list, void* data)
{
    dlinked_list_node_p curr = list->head;

	while ( curr )
	{
		if ( curr->data == data ) return curr->data;
		if ( list->equality && list->equality(curr->data, data) ) return curr->data;
		curr = curr->next;
	}
	return NULL;
}

//----------------------------------------------------
char* dlinked_list_tostring(dlinked_list_p list)
{
	if ( list->n_print == NULL || list->head == NULL )
		return "";
	
	int size = 5;
	char* str = malloc(size);
	strcpy(str, "[ ( ");
		
	dlinked_list_node_p curr = list->head;
	
	char* s = list->n_print(curr->data);
	
	size += strlen(s) + 2;
	str = realloc(str, size);
	strcat(str, s);
	strcat(str, " )");
	
	free(s);
	
	curr = curr->next;
	while ( curr )
	{
		char* s = list->n_print(curr->data);
		
		size += strlen(s) + 9;
		str = realloc(str, size);
		strcat(str, " <-> ( ");
		strcat(str, s);
		strcat(str, " )");
		
		free(s);
		
		curr = curr->next;
	}
	
	size +=  2;
	str = realloc(str, size);
	strcat(str, " ]\n");

	return str;
}

//----------------------------------------------------
dlinked_list_node_p dlinked_list_node_search(dlinked_list_p list, void* data)
{
    dlinked_list_node_p curr = list->head;

	while ( curr )
	{
		if ( curr->data == data ) return curr;
		if ( list->equality && list->equality(curr->data, data) ) return curr;
		curr = curr->next;
	}
	return NULL;
}

//----------------------------------------------------
void dlinked_list_remove_node(dlinked_list_p list, dlinked_list_node_p n)
{
	// Single element list
	if ( n == list->head && list->head == list->tail )
	{
		dlinked_list_node_destroy(n);
		list->head = list->tail = NULL;
	}
	
	// Removing the head node
    else if ( n == list->head )
    {
        dlinked_list_node_p right = n->next;
		dlinked_list_node_destroy(list->head);
		list->head = right;
	}
	
	// Removing the tail node
	else if ( n == list->tail )
	{
        dlinked_list_node_destroy(list->tail);
		list->tail = n->prev;
		n->prev->next = NULL;
	}
	
	// Removing an inner node
	else 
	{
		n->prev->next = n->next;
		n->next->prev = n->prev;
		dlinked_list_node_destroy(n);
	}
	
	list->size = list->size - 1;
}
