/*
 *	FILE: linked_list.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Creates a generic doubly linked_list type
 *		and gives the usual list operators.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// List Node struct
typedef struct node
{
    void* data;
    struct node* next;
	struct node* prev;
} dlinked_list_node_t, *dlinked_list_node_p;

// Linked List struct
typedef struct dlinked_list
{
    dlinked_list_node_p head;
    dlinked_list_node_p tail;
	int size;
	char (*equality)(void*, void*);
	char* (*n_print)(void*);
	void (*n_dealloc)(void*);
} dlinked_list_t, *dlinked_list_p;

/*
 *	Initializes new node.
 *
 *	PARAMS:
 *		data 	- identifier to give the new node.
 *
 *	RETURN:
 *		Returns a newly created node.
 */
dlinked_list_node_p dlinked_list_node_make(void* data);

/*
 *	Initializes the given node.
 *
 *	PARAMS:
 *		n 		- node to initialize.
 *		data 	- identifier to give n.
 */
void dlinked_list_node_init(dlinked_list_node_p n, void* data);

/*
 *	Frees memory in the given node.
 *
 *	PARAMS:
 *		n		- node to free.
 *		dealloc - the dealloc function for the node's data
 */
void dlinked_list_node_destroy(dlinked_list_node_p n);

/*
 *	Initializes and returns a new linked_list.
 *
 *  PARAMS:
 *      equal_check  - function to verify node's data equality
 *      node_printer - function to get node data as a string 
 *						(required to use tostring)
 *      node_dealloc - function to free node data
 *						(required to use clear efficiently)
 *
 *	RETURN:
 *		Returns a newly created linked_list.
 */
dlinked_list_p dlinked_list_make(
	char (*equal_check)(void*, void*), 
	char* (*node_printer)(void*),
	void (*node_dealloc)(void*)
	);

/*
 *	Initializes the given linked_list.
 *
 *	PARAMS:
 *		list	     - linked_list to initialize.
 *      equal_check  - function to verify node's data equality
 *      node_printer - function to get node data as a string 
 *						(required to use tostring)
 *      node_dealloc - function to free node data
 *						(required to use clear efficiently)
 */
void dlinked_list_init(
	dlinked_list_p list, 
	char (*equal_check)(void*, void*), 
	char* (*node_printer)(void*),
	void (*node_dealloc)(void*)
	);

/*
 *	NOTES:
 *
 *		-	The only time node data is freed is during clear().
 *		The only time you may want to free data otherwise is during
 *		remove() in which case the data is returned for you to free.
 *
 */
	
/*
 *	Deletes all nodes from given list and empties the list.
 *
 *	PARAMS:
 *		list	- linked_list to clear.
 */
void dlinked_list_clear(dlinked_list_p list);

/*
 *	Deletes all nodes from given list and
 *	frees the list itself as well.
 *
 *	PARAMS:
 *		list	- linked_list to destroy.
 */
void dlinked_list_destroy(dlinked_list_p list);

/*
 *	Appends given node to the tail end of list.
 *
 *	PARAMS:
 *		list	- linked_list to append to.
 *		new_n	- node to append.
 */
void dlinked_list_append(dlinked_list_p list, void* data);

/*
 *	Removes given data from list.  Node is freed.
 *
 *	PARAMS:
 *		list	- linked_list to remove from.
 *		to_rem	- data to remove and destroy.
 */
void* dlinked_list_remove(dlinked_list_p list, void* to_rem);

/*
 *	Searches list for data.
 *	NOTE: If no equality function was given to the list,
 *		returns NULL immediately.
 *
 *	PARAMS:
 *		list 	- linked_list to search through.
 *		data 	- identifier of node to look for.
 *
 *	RETURN:
 *		NULL if not found, otherwise the data of the found node
 */
void* dlinked_list_search(dlinked_list_p list, void* data);

/*
 *	Prints the list based on the given node printer.
 *
 *	PARAMS:
 *		list 	- linked_list to print.
 */
void dlinked_list_print(dlinked_list_p list);

/* ---------- INTERNAL USE ONLY ---------- */

/*
 *	Searches list for data.
 *	NOTE: If no equality function was given to the list,
 *		returns NULL immediately.
 *
 *	PARAMS:
 *		list 	- linked_list to search through.
 *		data 	- identifier of node to look for.
 *
 *	RETURN:
 *		NULL if not found, otherwise the node containing the that
 */
dlinked_list_node_p dlinked_list_node_search(dlinked_list_p list, void* data);

/*
 *	Removes given node from list.  Node is freed.
 *
 *	PARAMS:
 *		list	- linked_list to remove from.
 *		n		- node to remove and destroy.
 */
void dlinked_list_remove_node(dlinked_list_p list, dlinked_list_node_p n);

#endif // LINKEDLIST_H
