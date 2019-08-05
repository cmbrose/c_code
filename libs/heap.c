#include <stdlib.h>
#include "heap.h"

//----------------------------------------------------
heap_p heap_make(int (*compare)(void*,void*))
{
	if ( compare == NULL ) return NULL;
	
	heap_p ret = (heap_p) malloc(sizeof(heap_t));
	heap_init(ret, compare);
	return ret;
}

//----------------------------------------------------
void heap_init(heap_p heap, int (*compare)(void*,void*))
{
	if ( compare == NULL ) return;
	
	heap->arr = NULL;
	heap->size = 0;
	heap->compare = compare;
}

//----------------------------------------------------
heap_p heapify(void** arr, int len, int (*compare)(void*,void*))
{
	heap_p heap = heap_make(compare);
	heap->size = len;
	heap->arr = arr;
	
	int i;
	
	for ( i = len - 1; i >= 0; --i )
	{
		heap_percolate_down(heap, i);
	}
	
	return heap;
}

//----------------------------------------------------
void heap_destroy(heap_p heap)
{
	if ( heap ) free(heap);
}

//----------------------------------------------------
void heap_push(heap_p heap, void* toadd)
{
	heap->arr = realloc(heap->arr, (heap->size * 2 + 1) * sizeof(void*));
	heap->arr[heap->size] = toadd;
	heap->size += 1;
	
	heap_percolate_up(heap, heap->size - 1);
}

//----------------------------------------------------
void* heap_peek(heap_p heap)
{
	if ( heap->size == 0 ) 
		return NULL;
	
	return heap->arr[0];
}

//----------------------------------------------------
void* heap_pop(heap_p heap)
{
	if ( heap->size == 0 )
		return NULL;
		
	void* ret = heap_peek(heap);
	
    heap->arr[0] = heap->arr[heap->size - 1];
	heap->arr[heap->size - 1] = NULL;
	heap->size -= 1;
	
	heap_percolate_down(heap, 0);
	
	return ret;
}

//----------------------------------------------------
void heap_percolate_up(heap_p heap, int idx)
{
	int p = HEAP_PARENT(idx);
	
	while ( idx > 0 && heap->compare(heap->arr[idx], heap->arr[p]) < 0 )
	{
		void* tmp = heap->arr[idx];
		heap->arr[idx] = heap->arr[p];
		heap->arr[p] = tmp;
		
		idx = p;
		p = HEAP_PARENT(idx);
	}
}

//----------------------------------------------------
void heap_percolate_down(heap_p heap, int idx)
{
    while ( idx < heap->size )
	{
		int lc = HEAP_LCHILD(idx);
		int rc = lc + 1;
		int min_i;

		if ( lc >= heap->size ) break;
		
		if ( rc >= heap->size || heap->compare(heap->arr[lc], heap->arr[rc]) <= 0 )
			min_i = lc;
		else
		    min_i = rc;

        if ( heap->compare(heap->arr[idx], heap->arr[min_i]) <= 0 )
            break;

        void* tmp = heap->arr[idx];
        heap->arr[idx] = heap->arr[min_i];
        heap->arr[min_i] = tmp;

        idx = min_i;
	}
}
