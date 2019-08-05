#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

struct arraylist
{
    void** array;
    int size;
    int data_size;
    int reserved_size;
    int (*equals)(void* e1, void* e2);
};

// PRIVATES ------------------------------------------------

void arraylist_shift_right(arraylist_p list, int from);

void arraylist_shift_left(arraylist_p list, int from);

void arraylist_resize(arraylist_p list, int size);

void* clone_data(void *data, int size)
{
    void *clone = malloc(size);
    memcpy(clone, data, size);
    return clone;
}

// END PRIVATES -------------------------------------------

//----------------------------------------------------------
arraylist_p arraylist_make(int (*equals)(void* e1, void*e2), int data_size)
{
    arraylist_p ret = malloc(sizeof(*ret));
    ret->array = malloc(1);
    ret->size = 0;
    ret->data_size = data_size;
    ret->reserved_size = 1;
    ret->equals = equals;

    return ret;
}

//----------------------------------------------------------
void arraylist_destroy(arraylist_p list)
{
	if ( list == NULL ) return;
	
    arraylist_clear(list);

	free(list->array);
	
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
void arraylist_get(arraylist_p list, int idx, void *out)
{
    if ( idx >= list->size || idx < 0 ) return;

    memcpy(out, list->array[idx], list->data_size);
}

//----------------------------------------------------------
void arraylist_set(arraylist_p list, int idx, void *obj)
{
    if ( idx >= list->size || idx < 0 ) return;

    list->array[idx] = clone_data(obj, list->data_size);
}

//----------------------------------------------------------
void arraylist_remove(arraylist_p list, int idx, void *out)
{
    if ( idx >= list->size || idx < 0 ) return;

    void* ele = list->array[idx];
    arraylist_shift_left(list, idx);
	list->size -= 1;
	
    if (out != NULL)
	{
		memcpy(out, ele, list->data_size);
	}
	
    free(ele);
}

//----------------------------------------------------------
void arraylist_append(arraylist_p list, void *obj)
{
    arraylist_insert(list, list->size, obj);
}

//----------------------------------------------------------
void arraylist_insert(arraylist_p list, int idx, void* obj)
{
    if ( idx < 0 || idx > list->size ) return;
	
    if ( list->size >= list->reserved_size )
        arraylist_resize(list, list->size * RESIZE_FACTOR);
	
    arraylist_shift_right(list, idx);
    list->array[idx] = clone_data(obj, list->data_size);
    list->size += 1;
}

//----------------------------------------------------------
int arraylist_index_of(arraylist_p list, void *obj)
{
    if (list->equals == NULL) return -1;

    int i;
    for ( i = 0; i < list->size; ++i )
    {
        if (list->equals(obj, list->array[i]))
            return i;
    }

    return -1;
}

//----------------------------------------------------------
void arraylist_clear(arraylist_p list)
{
    int i;
    for (i = 0; i < list->size; ++i)
        free(list->array[i]);

	list->size = 0;
}

void** arraylist_backing_list(arraylist_p list)
{
	if (list == NULL)
	{
		return NULL;
	}
	
	return list->array;
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
    list->array = realloc(list->array, size * sizeof(list->array));
    list->reserved_size = size;
}

//----------------------------------------------------------
int arraylist_size(arraylist_p list)
{
    return list->size;
}
