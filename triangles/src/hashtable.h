/*
 *	FILE: hashtable.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines a hashtable data structure.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_SIGMA 	0.8
#define GROWTH_RATE 2

// hashnode datatype
typedef struct hashnode
{
	int hashcode;
	void* data;
	struct hashnode* next;
} hashnode_t, *hashnode_p;

// hashtable dataype
typedef struct
{
	int table_size;
	int elem_cnt;
	hashnode_p* array;
	
	int  (*hash)(void*);
	char (*equals)(void*, void*);
	void (*n_destroy)(void*);
} hashtable_t, *hashtable_p;

/**
 **	NOTE ON INPUTS:
 **
 **		This hashtable will do no input cleansing.  It *should*
 **		not throw any error due to NULL data or anything of that
 **		sort, but it will simply pass any data into the given
 **		functions.  Input cleansing is the duty of those functions.
 **/

/*
 *	Mallocs a new hashtable of the given size with the given functions.
 *
 *	PARAMS:
 *		size		- Number of buckets the hashtable will have to start.
 *		hash		- hashing function for data
 *		equality	- equals function for data
 *		n_destroy	- data destruction function.  If you don't want data
 *						to be freed when this hashtable is destroyed, set NULL. 
 *
 *	RETURN:
 *		A new hashtable of the given size.
 */
hashtable_p hashtable_make(int size, int (*hash)(void*), char (*equals)(void*, void*),void (*n_destroy)(void*));

/*
 *	Frees all memory held by table.  If an n_destroy function
 *		exists in table, node data will also be destroyed.
 *
 *  PARAMS:
 *		table	- hashtable to destroy
 */
void hashtable_destroy(hashtable_p table);

/*
 * Empties all elements from the table. If an n_destroy function
 *		exists in table, node data will be destroyed.
 *
 * PARAMS:
 *		table	- hashtable to clear
 */
void hashtable_clear(hashtable_p table);

/*
 *	Adds a new node to the hashtable with the given 
 *		data if the data does not exist in the table.
 *		Otherwise, overwrites data of node with same data.
 *
 *  PARAMS:
 *		table	- hashtable to add to
 *		data	- data to add
 *
 *	RETURN:
 *		Returns the existing data if a node is overwritten, NULL otherwise.
 */
void* hashtable_add(hashtable_p table, void* data);

/*
 *	Searches the hashtable for data and if an equal data
 * 		is found (using the equality func ptr), returns that data.
 *
 *  PARAMS:
 *		table	- hashtable to search through
 *		data	- data to search for
 *
 *	RETURN:
 *		Returns the data of the first node that has equals(node->data, data) resolve to TRUE,
 *		returns NULL if not data was found.
 */
void* hashtable_get(hashtable_p table, void* data);

/*
 *	Removes from the hashtable the first node whose data equals data
 * 		and returns that data.
 *
 *  PARAMS:
 *		table	- hashtable to remove from
 *		data	- data to search for
 *
 *	RETURN:
 *		Returns the data of the first node that has equals(node->data, data) resolve to TRUE,
 *		returns NULL if not data was found.
 */
void* hashtable_remove(hashtable_p table, void* data);

/*
 *  !-- INTERNAL USE -- !
 *
 *	Extends the hashtable and re-hashes data.
 *
 *	PARAMS:
 *		table	- the hashtable to resize
 *		size	- the new size to extend the hashtable to/
 */
void hashtable_resize(hashtable_p table, int size);

/*
 *  !-- INTERNAL USE -- !
 *
 *	Mallocs a new hashnode of the given data.
 *
 *	PARAMS:
 *		data	- data to put into the new node.
 *
 *	RETURN:
 *		A new hashnode.
 */
hashnode_p hashnode_make(void* data);

#endif // HASHTABLE_H