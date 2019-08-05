#include <stdlib.h>
#include "iterator.h"

//----------------------------------------------------
void iterator_destroy(iterator_p iter)
{
	iter->destroy(iter);
}

//----------------------------------------------------
char  iterator_has_next(iterator_p iter)
{
	return iter->has_next(iter);
}

//----------------------------------------------------
void* iterator_next(iterator_p iter)
{
	return iter->next(iter);
}

//----------------------------------------------------
void* iterator_get(iterator_p iter)
{
	return iter->get(iter);
}

//----------------------------------------------------
void*  iterator_set(iterator_p iter, void* data)
{
	return iter->set(iter, data);
}

//----------------------------------------------------
void* iterator_remove(iterator_p iter)
{
	return iter->remove(iter);
}