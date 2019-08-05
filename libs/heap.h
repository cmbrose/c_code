/*
 *	FILE: heap.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines a heap data structure.
 */

#ifndef HEAP_H
#define HEAP_H

#define HEAP_PARENT(I) ((I-1)/2)
#define HEAP_LCHILD(I) (2*I+1)

// heap datatype
typedef struct 
{
	void** arr;
	int size;
	int (*compare)(void*,void*);

} heap_t, *heap_p;

/**
 **	NOTE ON COMPARE FUNCTION POINTER:
 **
 **		This heap sorts data such that when comparing
 **		datums d1 and d2, if compare(d1, d2) < 0, then
 **		d1 will appear before d2.
 **/
 
/**
 **	NOTE ON INPUTS:
 **
 **		This heap will do no input cleansing.  It *should*
 **		not throw any error due to NULL data or anything of that
 **		sort, but it will simply pass any data into the given
 **		functions.  Input cleansing is the duty of those functions.
 **/

/*
 *	Mallocs a new heap with the given compare function.
 *
 *	PARAMS:
 *		compare		- data compare function.
 *
 *	RETURN:
 *		Returns a new, initialized heap.
 */
heap_p heap_make(int (*compare)(void*,void*));

/*
 *	Initializes an existing heap with the given compare function.
 *
 *	PARAMS:
 *		compare		- data compare function.
 */
void heap_init(heap_p heap, int (*compare)(void*,void*));

/*
 *	Initializes a new heap sorted by given compare function from
 *		the data in the given array.
 *
 *	PARAMS:
 *		arr			- array to turn into a heap - IS MODIFIED
 *		len			- length of the given array
 *		compare		- data compare function
 *
 *	RETURN:
 *		Returns a new heap containing the given data.
 */
heap_p heapify(void** arr, int len, int (*compare)(void*,void*));

/*
 *	Frees memory held by the given heap.
 *
 *	PARAMS:
 *		heap	- heap to destroy
 */
void heap_destroy(heap_p heap);

/*
 *	Adds the given data to the heap.
 *
 *	PARAMS:
 *		heap	- heap to add to
 *		toadd	- data to add to the heap
 */
void heap_push(heap_p heap, void* toadd);

/*
 *	Gets the topmost data of the heap.
 *
 *	PARAMS:
 *		heap	- heap to get from
 *	
 *	RETURN:
 *		The topmost data of the heap.
 */
void* heap_peek(heap_p heap);

/*
 *	Removes and returns the topmost data of the heap.
 *
 *	PARAMS:
 *		heap	- heap to remove from
 *	
 *	RETURN:
 *		The topmost data of the heap.
 */
void* heap_pop(heap_p head);

/*
 *  !-- INTERNAL USE -- !
 *
 *	Moves data from idx upwards to the correct position.
 *
 *	PARAMS:
 *		heap	- heap to move data through
 *		idx		- index of the data to move
 */
void heap_percolate_up(heap_p heap, int idx);

/*
 *  !-- INTERNAL USE -- !
 *
 *	Moves data from idx downwards to the correct position.
 *
 *	PARAMS:
 *		heap	- heap to move data through
 *		idx		- index of the data to move
 */
void heap_percolate_down(heap_p heap, int idx);

#endif // HEAP_H