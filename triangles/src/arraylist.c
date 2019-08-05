#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------
arraylist_p arraylist_make(int (*equals)(void* e1, void*e2))
{
    return arraylist_make_from(NULL, 0, equals);
}

//----------------------------------------------------------
arraylist_p arraylist_make_from(void** base, int cnt, int (*equals)(void* e1, void*e2))
{
    arraylist_p list = malloc(sizeof(arraylist_t));
    arraylist_init_from(list, base, cnt, equals);
    return list;
}

//----------------------------------------------------------
void arraylist_init(arraylist_p list, int (*equals)(void* e1, void*e2))
{
    arraylist_init_from(list, NULL, 0, equals);
}

//----------------------------------------------------------
void arraylist_init_from(arraylist_p list, void** base, int cnt, int (*equals)(void* e1, void*e2))
{
    list->array = malloc(cnt * sizeof(void*));
    memcpy(list->array, base, cnt * sizeof(void*));
    list->size = cnt;
    list->reserved_size = cnt;
    list->equals = equals;
}

//----------------------------------------------------------
void arraylist_destroy(arraylist_p list)
{
	if ( list == NULL ) return;
	
	if ( list->array ) free(list->array);
	
	free(list);
}

//----------------------------------------------------------
void arraylist_reserve(arraylist_p list, int size)
{
    if ( size < list->size ) return;
    arraylist_resize(list, size);
}

//----------------------------------------------------------
void arraylist_compact(arraylist_p list)
{
    arraylist_resize(list, list->size);
}

//----------------------------------------------------------
void* arraylist_get(arraylist_p list, int idx)
{
    if ( idx >= list->size || idx < 0 ) return NULL;

    return list->array[idx];
}

//----------------------------------------------------------
void arraylist_set(arraylist_p list, int idx, void* obj)
{
    if ( idx >= list->size || idx < 0 ) return;

    list->array[idx] = obj;
}

//----------------------------------------------------------
void* arraylist_remove(arraylist_p list, int idx)
{
    if ( idx >= list->size || idx < 0 ) return NULL;

    void* ele = list->array[idx];
    arraylist_shift_left(list, idx);
	list->size -= 1;
	
    return ele;
}

//----------------------------------------------------------
void arraylist_append(arraylist_p list, void* data)
{
    arraylist_insert(list, list->size, data);
}

//----------------------------------------------------------
void arraylist_insert(arraylist_p list, int idx, void* data)
{
    if ( idx < 0 || idx > list->size ) return;
	
    if ( list->size >= list->reserved_size )
        arraylist_resize(list, list->size * RESIZE_FACTOR + 1);
	
    arraylist_shift_right(list, idx);
    list->array[idx] = data;
    list->size += 1;
}

//----------------------------------------------------------
int arraylist_index_of(arraylist_p list, void* data)
{
    if (list->equals == NULL) return -1;

    int i;
    for ( i = 0; i < list->size; ++i )
    {
        if ( data == list->array[i] || list->equals(data, list->array[i]) )
            return i;
    }

    return -1;
}

//----------------------------------------------------------
void arraylist_clear(arraylist_p list)
{
	list->size = 0;
}

// PRIVATES ---------------------------------------------------

//----------------------------------------------------------
void arraylist_shift_left(arraylist_p list, int from)
{
    int i;
    for ( i = from; i < list->size; ++i )
    {
        list->array[i] = list->array[i + 1];
    }
}

//----------------------------------------------------------
void arraylist_shift_right(arraylist_p list, int from)
{
    int i;
    for ( i = list->size; i > from; --i )
    {
        list->array[i] = list->array[i - 1];
    }
}

//----------------------------------------------------------
void arraylist_resize(arraylist_p list, int size)
{
    list->array = realloc(list->array, size * sizeof(void*));
    list->reserved_size = size;
}

