#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "linked_list.h"
#include "alias.h"

//----------------------------------------------------
int try_builtin(int len, string_p p_str, linked_list_p procs, hashtable_p alias_table)
{
	string_t lower = str_lower(p_str[0]);
	
    // EXIT
    if ( 0 == strcmp(lower, "exit") )
        return EXIT;
		
    // PID
    else if ( 0 == strcmp(lower, "pid") )
        printf("%d\n", getpid());
	
    // PPID
    else if ( 0 == strcmp(lower, "ppid") )
        printf("%d\n", getppid());
	
    // PWD
    else if ( 0 == strcmp(lower, "pwd") )
		do_pwd();
	
    // CD [dir] - if dir not given, go to HOME
    else if ( 0 == strcmp(lower, "cd") )
		do_cd(p_str, len);
	
    // SET [KEY] [VAL]
    else if ( 0 == strcmp(lower, "set") )
		do_set(p_str, len);
	
    // GET [KEY]
    else if ( 0 == strcmp(lower, "get") )
		do_get(p_str, len);
	
	// JOBS
	else if ( 0 == strcmp(lower, "jobs") )
		do_jobs(procs);
		
	// ALIAS
	else if ( 0 == strcmp(lower, "alias") )
		do_alias(alias_table, p_str, len);
	
	// Not a builtin
	else 
		return 0;

    return 1;
}

//----------------------------------------------------
void do_pwd()
{
	char dir[MAX_DIR_LEN];
	getcwd(dir, MAX_DIR_LEN);
	printf("%s\n", dir);
}

//----------------------------------------------------
void do_cd(string_p strs, int len)
{
	char* dst;

	if ( len < 2 )
		dst = getenv("HOME");    
	else
		dst = strs[1];

	// Try to change dir, report success or error
	if ( 0 == chdir(dst) )
	{
		char dir[MAX_DIR_LEN];
		getcwd(dir, MAX_DIR_LEN);
		printf("Directory successfully changed to:\n\t%s\n", dir);
	}
	else
		printf("ERROR changing directory\n");
}

//----------------------------------------------------
void do_set(string_p strs, int len)
{
	if ( len < 3 )
	{
		printf("Not enough parameters given to command %s.\n", strs[0]);
		return;
	}

	setenv(strs[1], strs[2], 1);
}

//----------------------------------------------------
void do_get(string_p strs, int len)
{
	if ( len < 2 )
	{
		printf("Not enough parameters given to command %s.\n", strs[0]);
		return;
	}

	char* val = getenv(strs[1]);
	if ( !val )
		printf("Env. variable %s is not set.\n", strs[1]);
	else
		printf("%s\n", val);
}

//----------------------------------------------------
void do_jobs(linked_list_p procs)
{
	list_node_p curr = procs->head;
	if ( curr )
	{
		printf("%8s %s\n", "PID", "Name");
			
		while ( curr )
		{
			bg_proc_p p = (bg_proc_p) curr->data;
			printf("%8d %s\n", p->pid, p->name);
			curr = curr->next;
		}
	}
	else
		printf("No current jobs running.\n");
}

//----------------------------------------------------
void do_alias(hashtable_p alias_table, string_p strs, int len)
{
	if ( len < 2 )
	{
		printf("Not enough parameters given to command %s.\n", strs[0]);
		return;
	}
	
	char *eq;
	
	if ( (eq = strchr(strs[1], '=')) == NULL ) 
	{
		printf("ERROR: could not parse command %s.\n", strs[0]);
		return;
	}
	
	// Split the strings
	*eq = 0;
	
	// Get the left and right
	string_t left, right;
	str_clone(&left, strs[1]);
	str_clone(&right, eq + 1);
	
	// Remove quotes from end
	if ( right[0] == '\"' )
	{
		right[strlen(right) - 1] = 0;
		str_remove(&right, 0, 1);
	}
	
	alias_add(alias_table, left, right);
}
