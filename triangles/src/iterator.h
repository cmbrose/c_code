#ifndef ITERATOR_H
#define ITERATOR_H

#define iterator_for_each(I, V) ( for( ; iterator_has_next(I) && ( V = iterator_next(iter) || 1 ); iterator_next(I) ) )

typedef struct iterator
{
	// Actual collection to move over
	void* collection;
	
	// Helper data for collection
	void* current;
	
	char  (*has_next)(struct iterator*);
	void* (*next)(struct iterator*);
	void* (*get)(struct iterator*);
	void* (*set)(struct iterator*, void*);
	void* (*remove)(struct iterator*);
	void  (*destroy)(void*);
	
} iterator_t, *iterator_p;

void iterator_destroy(iterator_p iter);

char  iterator_has_next(iterator_p iter);

void* iterator_next(iterator_p iter);

void* iterator_get(iterator_p iter);

void* iterator_set(iterator_p iter, void* data);

void* iterator_remove(iterator_p iter);

#endif // ITERATOR_H