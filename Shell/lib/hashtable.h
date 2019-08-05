#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_SIGMA 	0.8
#define GROWTH_RATE 2

typedef struct hashnode
{
	int hashcode;
	void* data;
	struct hashnode* next;
} hashnode_t, *hashnode_p;

typedef struct
{
	int table_size;
	int elem_cnt;
	hashnode_p* array;
	
	int  (*hash)(void*);
	char (*equality)(void*, void*);
} hashtable_t, *hashtable_p;

hashtable_p hashtable_make(int size, int (*hash)(void*), char (*equality)(void*, void*));

void hashtable_destroy(hashtable_p table);

hashnode_p hashnode_make(void* data);

void hashtable_add(hashtable_p table, hashnode_p node);

hashnode_p hashtable_get(hashtable_p table, void* data);

void hashtable_remove(hashtable_p table, void* data);

void hashtable_resize(hashtable_p table, int size);

#endif // HASHTABLE_H