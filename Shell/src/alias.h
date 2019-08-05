#ifndef ALIAS_H
#define ALIAS_H

#include "str_tools.h"
#include "hashtable.h"

#define DEF_TABLE_SIZE 5

typedef struct
{
	string_t alias;
	string_t value;
} alias_pair_t, *alias_pair_p;

/*
 *	Creates and return the hashtable required for
 *	aliasing.
 */
hashtable_p alias_make_table();

/*
 *	Adds a new alias to the list of aliases.
 *	Overwrites if alias already exists.
 *
 *	PARAMS:
 *		name	- name of alias to create
 *		text	- text to replace name with
 */
void alias_add(hashtable_p alias_table, string_t name, string_t text);

/*
 *	Searches given string for an alias.
 *	Inserts into the string if found.
 *
 *	PARAMS:
 *		text	- text to search through
 */
void alias_search(hashtable_p alias_table, string_p text, int idx);

/*
 *	Hashes alias strings according to the
 *	Java string hash method.
 *
 *	PARAMS:
 *		data	- string to hash
 */
int alias_hash(void* data);

/*
 *	Compares the alias text of the given pointers.
 *
 *	PARAMS:
 *		d1	- first string to compare.
 *		d2	- second string to compare.
 */
char alias_equals(void* d1, void* d2);
 
#endif // ALIAS_H