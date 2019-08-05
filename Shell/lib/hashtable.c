#include "hashtable.h"

#include <stdlib.h>
#include <stdio.h>

//----------------------------------------------------
hashtable_p hashtable_make(int size, int (*hash)(void*), char (*equality)(void*, void*))
{
	// Create the new table
	hashtable_p ret = (hashtable_p) malloc(sizeof(hashtable_t));
	
	// Initialize sizes and functions
	ret->table_size = size;
	ret->elem_cnt = 0;
	ret->hash = hash;
	ret->equality = equality;
	
	// Initialize array
	ret->array = (hashnode_p*) malloc(size * sizeof(hashnode_p));
	
	// Initialize each list
	int i;
	for ( i = 0; i < size; ++i )
		ret->array[i] = NULL;
		
	return ret;
}

//----------------------------------------------------
void hashtable_destroy(hashtable_p table)
{
	if ( NULL == table ) return;
	
	int i;
	for ( i = 0; i < table->table_size; ++i )
	{
		hashnode_p curr = table->array[i];
		
		while ( curr )
		{
			hashnode_p tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}
		
	free(table->array);
	
	free(table);
}

//----------------------------------------------------
hashnode_p hashnode_make(void* data)
{
	hashnode_p ret = (hashnode_p) malloc(sizeof(hashnode_t));
	
	ret->hashcode = -1;
	ret->data = data;
	ret->next = NULL;
	
	return ret;
}

//----------------------------------------------------
void hashtable_add(hashtable_p table, hashnode_p node)
{
	int idx;
	
	if ( node->hashcode != -1 )
	{
		idx = node->hashcode;
	}
	else 
	{
		idx = table->hash(node->data);
		node->hashcode = idx;
	}	
	
	hashnode_p n = table->array[idx % table->table_size];
	hashnode_p prev = NULL;
	
	if ( n != NULL )
	{
		char found = 0;
		
		while ( !found && n )
		{
			if ( table->equality(n->data, node->data) )
			{
				free(n->data);
				n->data = node->data;
				found = 1;
				break;
			}
			
			prev = n;
			n = n->next;
		}
		
		if ( !found ) 
		{
			prev->next = node;
			table->elem_cnt = table->elem_cnt + 1;	
		}
	}
	else
	{
		table->array[idx % table->table_size] = node;
		table->elem_cnt = table->elem_cnt + 1;	
	}
	
	if ( 1.0 * table->elem_cnt / table->table_size >= MAX_SIGMA )
		hashtable_resize(table, table->table_size * GROWTH_RATE);
}

//----------------------------------------------------
hashnode_p hashtable_get(hashtable_p table, void* data)
{
	int idx = table->hash(data) % table->table_size;
	
	hashnode_p n = table->array[idx];
	
	while ( n )
	{
		if ( table->equality(n->data, data) )
			return n;
			
		n = n->next;
	}
	
	return NULL;
}

//----------------------------------------------------
void hashtable_remove(hashtable_p table, void* data)
{
	int idx = table->hash(data) % table->table_size;
	
	hashnode_p head = table->array[idx];
	
	hashnode_p curr = head, prev = NULL;
	
	while ( curr )
	{
		if ( table->equality(curr->data, data) )
		{	
			if ( prev ) prev->next = curr->next;
			else table->array[idx] = curr->next;
			
			break;
		}
		
		prev = curr;
		curr = curr->next;
	}
	
	if ( curr )
	{	
		free(curr);
		table->elem_cnt = table->elem_cnt - 1;	
	}
}

//----------------------------------------------------
void hashtable_resize(hashtable_p table, int size)
{
	hashnode_p* saved = (hashnode_p*) malloc(table->elem_cnt * sizeof(hashnode_p));
	
	// Save all nodes and erase links
	int i, idx = 0;
	for ( i = 0; i < table->table_size; ++i )
	{
		hashnode_p curr = table->array[i];
		
		while ( curr )
		{
			saved[idx++] = curr;
			
			hashnode_p tmp = curr;
			curr = curr->next;
			tmp->next = NULL;
		}
		
		table->array[i] = NULL;
	}
	
	// Resize
	table->table_size = size;
	table->array = (hashnode_p*) malloc(size * sizeof(hashnode_p));
	
	// Re-add all nodes
	for ( i = 0; i < idx; ++i )
		hashtable_add(table, saved[i]);
		
	free(saved);
}