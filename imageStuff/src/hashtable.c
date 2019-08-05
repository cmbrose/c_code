#include "hashtable.h"

#include <stdlib.h>

//----------------------------------------------------
hashtable_p hashtable_make(
	int size, 
	int (*hash)(void*), 
	char (*equals)(void*, void*),
	void (*n_destroy)(void*) )
{
	// Create the new table
	hashtable_p ret = (hashtable_p) malloc(sizeof(hashtable_t));
	
	// Initialize sizes and functions
	ret->table_size = size;
	ret->elem_cnt = 0;
	ret->hash = hash;
	ret->equals = equals;
	ret->n_destroy = n_destroy;
	
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
	
	hashtable_clear(table);
		
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
void hashtable_clear(hashtable_p table)
{
	int i;
	for ( i = 0; i < table->table_size; ++i )
	{
		hashnode_p curr = table->array[i];
		
		while ( curr )
		{
			hashnode_p tmp = curr->next;
			
			if ( table->n_destroy )
				table->n_destroy(curr->data);
			
			free(curr);
			curr = tmp;
		}

		table->array[i] = NULL;
	}

	table->elem_cnt = 0;
}

//----------------------------------------------------
void* hashtable_add(hashtable_p table, void* data)
{
	int hash = table->hash(data);
	void* ret = NULL;

	hashnode_p n = table->array[hash % table->table_size];
	hashnode_p prev = NULL;
	
	if ( n != NULL )
	{
		char found = 0;
		
		while ( !found && n)
		{
			if ( table->equals(n->data, data) )
			{
				ret = n->data;
				n->data = data;
				found = 1;
				break;
			}
			
			prev = n;
			n = n->next;
		}
		
		if ( !found ) 
		{
			prev->next = hashnode_make(data);
			table->elem_cnt = table->elem_cnt + 1;	
		}
	}
	else
	{
		table->array[hash % table->table_size] = hashnode_make(data);
		table->elem_cnt += 1;	
	}
	
	if ( 1.0 * table->elem_cnt / table->table_size >= MAX_SIGMA )
		hashtable_resize(table, table->table_size * GROWTH_RATE);
		
	return ret;
}

//----------------------------------------------------
void* hashtable_get(hashtable_p table, void* data)
{
	int idx = table->hash(data) % table->table_size;
	
	hashnode_p n = table->array[idx];
	
	while ( n )
	{
		if ( table->equals(n->data, data) )
			return n->data;
			
		n = n->next;
	}
	
	return NULL;
}

//----------------------------------------------------
void* hashtable_remove(hashtable_p table, void* data)
{
	int idx = table->hash(data) % table->table_size;
	void* ret = NULL;
	
	hashnode_p head = table->array[idx];
	
	hashnode_p curr = head, prev = NULL;
	
	while ( curr )
	{
		if ( table->equals(curr->data, data) )
		{	
			ret = curr->data;
			
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
		table->elem_cnt -= 1;	
	}
	
	return ret;
}

//----------------------------------------------------
void hashtable_resize(hashtable_p table, int size)
{
	void** saved = malloc(table->elem_cnt * sizeof(void*));
	
	// Save all data and erase links
	int i, idx = 0;
	for ( i = 0; i < table->table_size; ++i )
	{
		hashnode_p curr = table->array[i];
		
		while ( curr )
		{
			saved[idx++] = curr->data;
			
			hashnode_p tmp = curr;
			curr = curr->next;
			free(tmp);
		}
	}

	free(table->array);
	
	// Resize
	table->table_size = size;
	table->array = (hashnode_p*) malloc(size * sizeof(hashnode_p));

	for ( i = 0; i < size; ++i )
		table->array[i] = NULL;
	
	// Re-add all nodes
	for ( i = 0; i < idx; ++i )
		hashtable_add(table, saved[i]);
		
	free(saved);
}