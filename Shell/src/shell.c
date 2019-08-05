#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#include "shell.h"
#include "builtin.h"
#include "alias.h"

int main(int argc, char* argv[])
{
    string_t 		prompt_label; 	// Line to print before each entry
	string_p* 		p_parsed;		// Parsed command list and args

	// Current directory
	string_t curr_dir = str_make(MAX_DIR_LEN);
    getcwd(curr_dir, MAX_DIR_LEN);

	// Length of the HOME directory path
	int home_dir_off = strlen(getenv("HOME"));

    // Initialize command list pointer
	p_parsed = (string_p*) malloc(sizeof(string_p));
	
    // Initialize linked lists
    linked_list_p bg_procs = linked_list_make(&proc_equals);
	cmd_hist = linked_list_make(NULL);
	
	hashtable_p alias_table = alias_make_table();
	
	// Initialize input hook
	input_hooks = make_input_hook();
	input_hooks->f_up 		= &on_key_up;
	input_hooks->f_down 	= &on_key_down;
	input_hooks->f_return 	= &on_key_return;
	input_hooks->f_tab 		= &on_key_tab;
	input_hooks->f_char		= &on_key_norm;
		
    // Check for a prompt argument
    if ( argc >= 3 && 0 == strcmp("-p", argv[1]) )
    {
        prompt_label = argv[2];
    }
    else
    {
        prompt_label = "308sh> ";
    }
	
    while (1)
    {
        string_p next_command;              // Command given
		int cmd_len = 0, list_size = 0;     // Number of chunks in command
        char do_back = 0;                   // Command has '&' ?
        string_t redir_path;                // Path to redirect output to (if given)

		next_command = (string_p) malloc(sizeof(string_t));
		
        printf("%s ~%s", curr_dir + home_dir_off, prompt_label);
		fflush(stdout);
		
        cmd_len = get_console_input(next_command, input_hooks);
		
        // If something was entered, parse it
        if ( cmd_len > 0 )
		{
			strip_ws(next_command);
			if ( strlen(*next_command) )
				list_size = parse_command(*next_command, p_parsed, alias_table, &do_back, &redir_path);
		}
		    
        // If the parser found something, try to run it
        if ( list_size > 0 )
        {
			// File descriptor for redirect
			int redir_fd = -1, old_fd = dup(1);

			// Open file, if not there, create with all privileges
			if ( strlen(redir_path) > 0 )
				redir_fd = open(redir_path, O_WRONLY | O_CREAT | O_TRUNC, 0x0777);
			
			// Setup redirected output if needed
			if ( redir_fd > 0 )	dup2(redir_fd, 1);		
			
			// Try a builtin command first
            int cond = try_builtin(list_size, *p_parsed, bg_procs, alias_table); 
			
			// End redirected output if needed
			if ( redir_fd > 0 )	dup2(old_fd, 1); 		
			
            // If the builtin was "exit" then.... exit
            if ( cond == EXIT ) exit(1);
			
            // If it wasn't a builtin, try to run it as a program
            else if ( cond == 0 )
                try_program(*p_parsed, list_size, do_back, redir_fd, bg_procs);	

			// Builtin successful, check directory and HOME data
			else
			{
				getcwd(curr_dir, MAX_DIR_LEN);
				home_dir_off = strlen(getenv("HOME"));
			}
        }

        // Check and see if any background processes have ended
        try_wait_background(bg_procs);

		// Free the list
		if ( cmd_len > 0 )
			clear_list(p_parsed, list_size);
		
		if ( redir_path ) free(redir_path);
        redir_path = 0;
   }

   if ( p_parsed ) free(p_parsed);
   
   free(input_hooks);
   free(curr_dir);
   hashtable_destroy(alias_table);
   list_destroy(bg_procs);
   list_destroy(cmd_hist);

   return 0;
}

// Checks for equality of two background processes
char proc_equals(void* p1, void* p2)
{
    bg_proc_p proc1 = (bg_proc_p) p1,
              proc2 = (bg_proc_p) p2;
	
    return proc1->pid == proc2->pid;
}

// Performs basic command history inits
void initialize_for_input(string_t str)
{
	if ( hist_tmp != NULL ) return;
	
	string_t tmp;
	str_clone(&tmp, str);
	hist_tmp = node_make((void*) tmp);
	hist_curr = hist_tmp;
	list_append(cmd_hist, hist_tmp);
}

// Clears console after major line change
void erase_and_reprint(string_p p_str)
{
	int i;
	for ( i = 0; i < strlen(*p_str); ++i )
		printf("\b \b");
					
	string_t tmp = (string_t) hist_curr->data;		
	str_clone(p_str, tmp);
	
	printf("%s", *p_str);		
	fflush(stdout);
}

// Appends a char as normal to the string and node
void on_key_norm(string_p p_str, char c)
{
	initialize_for_input(*p_str);
	
	str_append(p_str, c);
	printf("%c", c);
	
	// Could be that last key was a special, have to reset
	free(hist_tmp->data);
	str_clone((string_p) &(hist_tmp->data), *p_str);
	hist_curr = hist_tmp;
}

// Try to go to previous entry in history
void on_key_up(string_p p_str)
{
	initialize_for_input(*p_str);
	
	if ( hist_curr->prev != NULL )
	{
		hist_curr = hist_curr->prev;
		erase_and_reprint(p_str);
	}
}

// Try to go to following entry in  history
void on_key_down(string_p p_str)
{
	initialize_for_input(*p_str);
	
	if ( hist_curr->next != NULL )
	{
		hist_curr = hist_curr->next;
		erase_and_reprint(p_str);
	}
}

// Append to history
void on_key_return(string_p p_str)
{
	if ( strlen(*p_str) == 0 ) return;
	
	if ( hist_curr != hist_tmp )
		list_remove(cmd_hist, hist_tmp);
	
	hist_tmp = NULL;
}

// Try to auto complete
void on_key_tab(string_p p_str)
{
	if ( strlen(*p_str) == 0 ) return;
			
	int q_cnt = str_cnt(*p_str, '\"');
	
	string_t entry;
	
	if ( q_cnt & 1 )
		entry = strrchr(*p_str, '\"') + 1;
	else
		entry = strrchr(*p_str, ' ') + 1;

	if ( entry - 1 == NULL ) entry = "";

	string_t path = str_make(3 + strlen(entry));
	strcpy(path, "./");
	
	string_t back_path = str_make(strlen(entry));
	
	if ( entry[0] != 0 )
		strcpy(back_path, entry);
	else
		strcpy(back_path, p_str[0]);
		
	string_t slash_idx = strrchr(back_path, '/');
	
	string_t search = back_path;

	if ( slash_idx != NULL )
	{
		*slash_idx = 0;
		search = slash_idx + 1;
		strcat(path, back_path);
	}
	
	DIR* dirp = opendir(path);
	
	if ( strlen(search) == 0 || dirp == NULL )
	{
		if ( dirp ) closedir(dirp);
		
		free(path);
		free(back_path);
		return;
	}
	
	struct dirent* curr = readdir(dirp);
	string_t matched = str_make(1);
	
	while ( NULL != curr )
	{				
		if ( curr->d_name == strstr(curr->d_name, search) )
		{
			if ( matched[0] == 0 )
			{
				str_strappend(&matched, curr->d_name);
			}
			else
			{
				string_t tmp = str_match(matched, curr->d_name);
				free(matched);
				matched = tmp;
			}
		}
		
		curr = readdir(dirp);
	}
	
	closedir(dirp);
					
	if ( strlen(matched) > 0 )
	{
		// Delete old text
		int i;
		for ( i = 0; i < strlen(*p_str); ++i )
			printf("\b \b");
						
		// Remove the search text from the string 
		// (so that str_strappend doesn't duplicate it)
		(*p_str)[strlen(*p_str) - strlen(search)] = 0;
		str_strappend(p_str, matched);
		
		printf("%s", *p_str);		
		fflush(stdout);
	}
	
	free(matched);
	free(path);
	free(back_path);
}

// Frees all the memory from the command list
void clear_list(string_p* list, int size)
{
	// Parser found and error and already cleared
	if ( size == -1 ) return;

    int i;
	
	// Free each string
	for ( i = 0; i < size; ++i )
	{
		if ( list[i] ) 
			free((*list)[i]);
		(*list)[i] = 0;
	}

	// The the array itself
	if ( (*list) ) free((*list));
	*list = 0;
}

// Takes the command from the user and parses into a list of string tokens
int parse_command
(
	string_t cmd, string_p* parsed, 
	hashtable_p alias_table, 
	boolean* do_back, string_p redir_path
)
{
    char open_quote                 = 0,    // Hanging quote, spaces don't split tokens
         need_next                  = 1,    // A token break was found, need to alloc a new line
         read_redir_path            = 0,    // Found a '>', next token goes into redir_path
         expect_redir               = 0;    // Should have read a redirect path, for error checking

    int  i,              
         par_idx                    = -1,   // Current line index
         char_idx                   = 0;    // Current char index in line 
    
    *do_back                        = 0;    // This is set if an '&' is found

    *redir_path = (char*) malloc(1);
    (*redir_path)[0] = 0;

    for ( i = 0; i < strlen(cmd); ++i )
    {
        // Check quotes
        if ( cmd[i] == '\"' )
            open_quote = !open_quote;

        // If not in quote block and at a space, end token
        else if ( !open_quote && cmd[i] == ' ' )
        {
            need_next = 1;
            read_redir_path = 0;
        }

        // Found an '&'
        else if ( cmd[i] == '&' && !open_quote )
            *do_back = 1;

        // Found an '>'
        else if ( cmd[i] == '>' && !open_quote )
		{
            read_redir_path = expect_redir = 1;
			need_next = char_idx = 0;
			
            // Skip all extra space, would reset the state
            while ( isspace(cmd[++i]) );
            --i;
		}

        else
        {
            // Need a new token, have to allocate more space
            if ( need_next )
            {                
                // If we were reading a redirect path, end that
                if ( read_redir_path )
                {
                    (*redir_path)[char_idx] = 0;
                    read_redir_path = 0;
                }

                // End the current line in the actual list
                // (if we were reading in a redirect, this hasn't happened yet)
                if ( par_idx != -1 )
                    (*parsed)[par_idx][char_idx] = 0;
                
                // Move to next line
                ++par_idx;
                
                // Allocate next line
                *parsed = (char**) realloc(*parsed, sizeof(char*) * (par_idx + 2));
                
                // Allocate single char for the new line
                (*parsed)[par_idx] = (char*) malloc(1);

                // Reset
                char_idx = 0;
                need_next = 0;
				
				// Try to do aliases
				alias_search(alias_table, &cmd, i);
            }
            
            if ( !read_redir_path )
            {
                // Adding a new char to token, extend the line and append it
                (*parsed)[par_idx] = (char*) realloc((*parsed)[par_idx], char_idx + 2);
                (*parsed)[par_idx][char_idx++] = cmd[i]; 
            }
            else
            {
                // Put character in redirect path instead
                *redir_path = (char*) realloc(*redir_path, char_idx + 2);
                (*redir_path)[char_idx++] = cmd[i];
            }
        }
    }

    // Make sure last string gets a '\0'
    (*parsed)[par_idx][char_idx] = 0;

    if ( read_redir_path ) (*redir_path)[char_idx] = 0;

    // Put a NULL at last index for passing into exec
    (*parsed)[par_idx + 1] = NULL;

    /* Error Checking */
    
    char err = 0;

    if ( open_quote )
    {
        printf("Error: Unfinished quote block.\n");
        err = 1;
    }
    else if ( expect_redir && (*redir_path)[0] == 0 )
    {
        printf("Error: Could not parse redirect path.\n");
        err = 1;
    }

    if ( err )
    {
	    clear_list(parsed, par_idx + 1);
		return -1;
    }

    return par_idx + 1;
}