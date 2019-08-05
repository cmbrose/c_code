/*
 *	FILE: str_tools.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines multiple new string functions which make
 *		creating and modifying strings much friendlier
 */

#ifndef STR_TOOLS_H
#define STR_TOOLS_H

#include <string.h>
#include <stdlib.h>

typedef char*		string_t;
typedef char**		string_p;

/*
 *	Mallocs a new string of length size. Appends '\0'.
 *
 *	PARAMS:
 *		size	- string_t to strip from.
 *
 *	RETURN:
 *		A string of length size terminated by a '\0'.
 */
string_t str_make(int size);

/*
 *	Appends the given char to the given string.
 *	NOTE: Must be able to call realloc on str.
 *
 *	PARAMS:
 *		p_str 	- pointer to string to append to.
 *		c		- char to append.
 */
void str_append(string_p p_str, char c);

/*
 *	Appends the given char to the given string.
 *	NOTE: Must be able to call realloc on str.
 *
 *	PARAMS:
 *		dest 	- pointer to string to append to.
 *		src		- string to append.
 */
void str_strappend(string_p dest, string_t src);

/*
 *	Performs a deep copy of src into dest.
 *
 *	PARAMS:
 *		dest 	- pointer to string to clone into.
 *		src		- string to clone from.
 */
void str_clone(string_p dest, string_t src);

/*
 *	Strips all leading and trailing whitespace.
 *
 *	PARAMS:
 *		str		- string_t to strip from.
 */
void strip_ws(string_p p_str);

/*
 *	Counts the occurences of k in str.
 *
 *	PARAMS:
 *		str 	- string_t to search through.
 *		k 		- the char to search for
 *
 *	RETURN:
 *		The number of times k appears in str.
 */
int str_cnt(string_t str, char k);

/*
 *	Converts uppercase text in str to lowercase.
 *
 *	PARAMS:
 *		str 	- string_t to convert.
 *
 *	RETURN:
 *		The converted, lowercase string_t.
 */
string_t str_lower(string_t str);

/*
 *	Reads through s1 and s2 and returns the
 *	longest substring that appears in both when 
 *	reading from the left.
 *
 *	PARAMS:
 *		s1 		- the first string_t.
 *		s2 		- the second string_t.
 *
 *	RETURN:
 *		The longest substring in both s1 and s2
 *		as read from the left.
 */
string_t str_match(string_t s1, string_t s2);

/*
 *	Inserts src into dest starting at start (such 
 *	that dest[start] is the first index changed).
 *
 *	PARAMS:
 *		dest 	- the string to modify.
 *		start	- the first index to insert into.
 *		src		- the string to insert
 */
void str_insert(string_p dest, int start, string_t src);

/*
 *	Deletes len chars from str starting at start 
 *	(such that str[start] is removed).
 *
 *	PARAMS:
 *		dest 	- the string to modify.
 *		start	- the first index to insert into.
 *		len		- number of chars to remove.
 */
void str_remove(string_p p_str, int start, int len);

#endif // STR_TOOLS_H