#ifndef PROC_RUN_H
#define PROC_RUN_H

#include "linked_list.h"
#include "str_tools.h"

typedef struct
{
    int pid;
    char* name;
} bg_proc_t, *bg_proc_p;

/*
 *	Attempts to fork() and exec() a new process running
 *	the command referred to buy p_str[0] with args p_str
 *
 *	PARAM:
 *		p_str			- string array of command and args
 *		len				- length of p_str
 *		do_background 	- set to 1 to request new process
 *			run in the background (appends to bg_procs).
 *		redir_fd		- file descriptor requesting output
 *			of new process be redirected to a file. Set to 0
 *			to ignore.
 *		bg_procs		- linked list of processes to append to
 *			in the case of a background request.
 */
void try_program(
	string_p p_str, int len, char do_background, 
	int redir_fd, linked_list_p bg_procs);

/*
 *	Attempts to find any child processes that were in the
 *	background which have recently died.  Will print details
 *	if a process (or processes) was found.
 *
 *	PARAM:
 *		bg_procs		- linked list of processes.
 */
void try_wait_background(linked_list_p bg_procs);

#endif // PROC_RUN_H