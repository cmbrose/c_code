#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#define RESIZE_FACTOR 2

typedef struct
{
    void** array;
    int size;
    int reserved_size;
    int (*equals)(void* e1, void* e2);
} arraylist_t, *arraylist_p;

arraylist_p arraylist_make(int (*equals)(void* e1, void*e2));

arraylist_p arraylist_make_from(void** base, int cnt, int (*equals)(void* e1, void*e2));

void arraylist_init(arraylist_p list, int (*equals)(void* e1, void*e2));

void arraylist_init_from(arraylist_p list, void** base, int cnt, int (*equals)(void* e1, void*e2));

void arraylist_destroy(arraylist_p list);

void arraylist_reserve(arraylist_p list, int size);

void arraylist_compact(arraylist_p list);

void* arraylist_get(arraylist_p list, int idx);

void arraylist_set(arraylist_p list, int idx, void* obj);

void* arraylist_remove(arraylist_p list, int idx);

void arraylist_append(arraylist_p list, void* data);

void arraylist_insert(arraylist_p list, int idx, void* data);

int arraylist_index_of(arraylist_p list, void* data);

void arraylist_clear(arraylist_p list);

// PRIVATES ---------------------------------------------------

void arraylist_shift_right(arraylist_p list, int from);

void arraylist_shift_left(arraylist_p list, int from);

void arraylist_resize(arraylist_p list, int size);

#endif // ARRAYLIST_H
