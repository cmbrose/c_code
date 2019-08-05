/*
 *	FILE: str_tools.c
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines multiple new string functions which make
 *		creating and modifying strings much friendlier
 */

#include <ctype.h>
#include "str_tools.h"

//----------------------------------------------------
string_t str_make(int size)
{
	string_t ret = (string_t) malloc(size);
	ret[size - 1] = 0;
	return ret;
}

//----------------------------------------------------
void str_append(string_p p_str, char c)
{
	int i = strlen(*p_str);
	(*p_str) = (string_t) realloc(*p_str, i + 2);
	(*p_str)[i] = c;
	(*p_str)[i + 1] = 0;
}

//----------------------------------------------------
void str_strappend(string_p dest, string_t src)
{
	(*dest) = (string_t) realloc(*dest, 1 + strlen(*dest) + strlen(src));
	strcat(*dest, src);
}

//----------------------------------------------------
void str_clone(string_p dest, string_t src)
{
	int i;

	(*dest) = (string_t) malloc(strlen(src) + 1);
		
	for ( i = 0; i < strlen(src) + 1; ++i ) 
		(*dest)[i] = src[i];
}

//----------------------------------------------------
void strip_ws(string_p p_str)
{
    int i = strlen(*p_str);
    while ( isspace((*p_str)[--i]) && i > 0 );
    (*p_str)[i+1] = 0;
    while ( isspace((*p_str)[0]) ) (*p_str)++;
}

//----------------------------------------------------
int str_cnt(string_t str, char k)
{
	char* i = str;
	int cnt = 0;
	
	while ( *i )
	{
		if ( k == *i ) ++cnt;
		++i;
	}
	
	return cnt;
}

//----------------------------------------------------
string_t str_lower(string_t str)
{
	string_t lower;
	str_clone(&lower, str);
	
	char* p = lower;
	while ( *p )
	{
		if ( *p >= 'A' && *p <= 'Z' ) *p = *p - 'A' + 'a';
		++p;
	}
		
	return lower;
}

//----------------------------------------------------
string_t str_match(string_t s1, string_t s2)
{
	string_t sub = str_make(1);
	
	int i = 0, len1 = strlen(s1), len2 = strlen(s2);
	
	while ( i < len1 && i < len2 )
	{
		if ( s1[i] != s2[i] ) break;
		
		sub = (string_t) realloc(sub, i + 2);
		sub[i] = s1[i];
		i += 1;
		sub[i] = 0;
	}
	
	return sub;
}

//----------------------------------------------------
void str_insert(string_p dest, int start, string_t src)
{
	// Save right side
	string_t right;
	str_clone(&right, &((*dest)[start]));
	
	(*dest)[start] = 0;
	
	str_strappend(dest, src);
	str_strappend(dest, right);
	
	free(right);
}

//----------------------------------------------------
void str_remove(string_p p_str, int start, int len)
{
	int size = strlen(*p_str);
	if ( start + len > size ) len = size - start;
	
	// Save right side
	string_t right;
	str_clone(&right, &((*p_str)[start + len]));
	
	(*p_str) = (string_t) realloc(*p_str, size - len + 1);
	(*p_str)[start] = 0;
	
	str_strappend(p_str, right);
	
	free(right);
}