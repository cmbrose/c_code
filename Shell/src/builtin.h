#ifndef BUILTIN_H
#define BUILTIN_H

#include "str_tools.h"
#include "linked_list.h"
#include "proc_run.h"
#include "hashtable.h"

#define MAX_DIR_LEN     260
#define EXIT            -1

/*
 *	Attempts to run a builtin referred to by p_str[0].
 *	NOTE: Not case sensitive.
 *
 *	PARAMS:
 *		len			- Length of p_str (number of args)
 *		p_str		- string_t array containing builtin 
 *			name and args
 *		bg_procs 	- Linked List of background processes
 *
 *		RETURN:
 *			-1 if "exit", 1 if otherwise valid builtin, 0 otherwise
 */
int try_builtin(int len, string_p p_str, linked_list_p procs, hashtable_p alias_table);

/* 	
 *	Non-trivial Builtin functions. 
 *	Names should be self explanatory.
 *	Each function handles its own error checking and printing.
 */
 
void do_pwd();

void do_cd(string_p strs, int len);

void do_set(string_p strs, int len);

void do_get(string_p strs, int len);

void do_jobs(linked_list_p procs);

void do_alias(hashtable_p alias_table, string_p strs, int len);

#endif // BUILTIN_H