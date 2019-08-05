#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proc_run.h"

void try_program(string_p p_str, int len, char do_background, int redir_fd, linked_list_p bg_procs)
{
    int c_pid = fork();

    if ( c_pid == -1 )
        printf("There was an issue with fork(). Command not run.\n");
		
    // In child, call command
    else if ( c_pid == 0 ) 
    {
		// Setup redirect, don't need to worry about closing
		if ( redir_fd > 0 ) dup2(redir_fd, 1);
			
        int ret = execvp(p_str[0], p_str);
        
        // If we're here, there's a problem.  Report it.
        string_t msg = str_make(12 + strlen(p_str[0]));
		strcpy(msg, "Cannot run ");
        str_strappend(&msg, p_str[0]);
		
        perror(msg);
		free(msg);
		
        exit(ret);
    }
    else
    {
        /*printf("[%d] %s\n", c_pid, p_str[0]);*/

        // If there was an '&', don't wait
        if ( !do_background )
        {
			int status, w_pid;
            if ( (w_pid = waitpid(c_pid, &status, 0)) == -1 ) 
                perror("ERROR when waiting"); 
            
			/*
			else if ( WIFEXITED(status) )
                printf("[%d] %s exited with status %d.\n", w_pid, p_str[0], WEXITSTATUS(status));
            else if ( WIFSIGNALED(status) )
                printf("[%d] %s killed (%d)\n", w_pid, p_str[0], WTERMSIG(status));
			*/
        }
		else
		{
			printf("[%d] %s\n", c_pid, p_str[0]);
			
			bg_proc_p p_proc = (bg_proc_p) malloc(sizeof(bg_proc_t));
			
			p_proc->pid = c_pid;
			p_proc->name = (string_t) malloc(1 + strlen(p_str[0]));
			strcpy(p_proc->name, p_str[0]);
			
            list_append(bg_procs, node_make((void*) p_proc));
		}
    }
}

void try_wait_background(linked_list_p bg_procs)
{
	while ( 1 )
	{
		int status;
		int pid = waitpid(-1, &status, WNOHANG);
		
		if ( pid <= 0 ) return;
		
		bg_proc_t p = {pid, ""};
		list_node_p n = list_search(bg_procs, (void*) &p);
		
		if ( !n )
		{
			//printf("ERROR IN LIST SEARCH\n");
			return;
		}
		
		p = *((bg_proc_p) n->data);
		
		if ( WIFEXITED(status) )
			printf("[%d] %s Done\n", pid, p.name);
		else if ( WIFSIGNALED(status) )
			printf("[%d] %s killed (%d).\n", pid, p.name, WTERMSIG(status));

		if ( n ) list_remove(bg_procs, n);
	}
}