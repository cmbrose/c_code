#include "alias.h"

#include <stdio.h>

//----------------------------------------------------
hashtable_p alias_make_table()
{
	return hashtable_make(DEF_TABLE_SIZE, &alias_hash, &alias_equals);
}

//----------------------------------------------------
void alias_add(hashtable_p alias_table, string_t name, string_t text)
{
	alias_pair_p toadd = (alias_pair_p) malloc(sizeof(alias_pair_t));
	toadd->alias = name;
	toadd->value = text;
	
	hashtable_add(alias_table, hashnode_make((void*) toadd));
}

//----------------------------------------------------
void alias_search(hashtable_p alias_table, string_p text, int idx)
{
	if ( alias_table == NULL ) return;
		
	alias_pair_p search = (alias_pair_p) malloc(sizeof(alias_pair_t));
	search->value = NULL;
	
	int i; // i = index of space
	for ( i = idx; i < strlen(*text); ++i )
		if ( (*text)[i] == ' ' )
			break;
	
	if ( i < strlen(*text) )
	{
		search->alias = str_make(i - idx);
		int j;
		for ( j = idx; j < i; ++j )
			search->alias[j - idx] = (*text)[j];
	}	
	else
	{
		str_clone(&(search->alias), &((*text)[idx]));
	}
	
	hashnode_p n = hashtable_get(alias_table, (void*) search);
	
	if ( n == NULL ) return;
			
	str_remove(text, idx, i - idx);
	
	alias_pair_p tmp = (alias_pair_p) n->data;	
	str_insert(text, idx, (string_t) tmp->value);
}

//----------------------------------------------------
int alias_hash(void* data)
{
	alias_pair_p tmp = (alias_pair_p) data;
	string_t text = tmp->alias;
	
	int i, sum = 0;
	for ( i = 0; i < strlen(text); ++i )
		sum = sum * 31 + text[i];
		
	return sum;
}

//----------------------------------------------------
char alias_equals(void* d1, void* d2)
{
	alias_pair_p tmp;

	tmp = (alias_pair_p) d1;
	string_t t1 = tmp->alias;
	
	tmp = (alias_pair_p) d2;
	string_t t2 = tmp->alias;
	
	return 0 == strcmp(t1, t2);
}