#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#define RESIZE_FACTOR 2

typedef struct arraylist *arraylist_p;

arraylist_p arraylist_make(int (*equals)(void* e1, void*e2), int data_size);

void arraylist_destroy(arraylist_p list);

void arraylist_reserve(arraylist_p list, int size);

void arraylist_compact(arraylist_p list);

void arraylist_get(arraylist_p list, int idx, void *out);

void arraylist_set(arraylist_p list, int idx, void *obj);

void arraylist_remove(arraylist_p list, int idx, void *out);

void arraylist_append(arraylist_p list, void *obj);

void arraylist_insert(arraylist_p list, int idx, void *obj);

int arraylist_index_of(arraylist_p list, void *data);

void arraylist_clear(arraylist_p list);

int arraylist_size(arraylist_p list);

void** arraylist_backing_list(arraylist_p list);

#endif // ARRAYLIST_H
