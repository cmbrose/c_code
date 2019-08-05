#ifndef LINKED_LIST_ITERATOR_H
#define LINKED_LIST_ITERATOR_H

#include "linked_list.h"
#include "iterator.h"

iterator_p linked_list_iter_make(linked_list_p list);

void linked_list_iter_init(iterator_p iter, linked_list_p list);

char linked_list_iter_has_next(iterator_p iter);

void* linked_list_iter_next(iterator_p iter);

void* linked_list_iter_get(iterator_p iter);

void*  linked_list_iter_set(iterator_p iter, void* data);

void* linked_list_iter_remove(iterator_p iter);

#endif // LINKED_LIST_ITERATOR_H