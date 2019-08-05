#ifndef HASHTABLE_ITERATOR_H
#define HASHTABLE_ITERATOR_H

#include "hashtable.h"
#include "iterator.h"

iterator_p hashtable_iter_make(hashtable_p list);

void hashtable_iter_init(iterator_p iter, hashtable_p list);

#endif // HASHTABLE_ITERATOR_H