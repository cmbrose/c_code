#ifndef LINKED_LIST_ITERATOR_H
#define LINKED_LIST_ITERATOR_H

#include "dlinked_list.h"
#include "iterator.h"

iterator_p linked_list_iter_make(dlinked_list_p list);

void dlinked_list_iter_init(iterator_p iter, dlinked_list_p list);

char dlinked_list_iter_has_next(iterator_p iter);

void* dlinked_list_iter_next(iterator_p iter);

void* dlinked_list_iter_get(iterator_p iter);

void* dlinked_list_iter_set(iterator_p iter, void* data);

void* dlinked_list_iter_remove(iterator_p iter);

#endif // LINKED_LIST_ITERATOR_H