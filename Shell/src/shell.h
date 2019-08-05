#ifndef SHELL_H
#define SHELL_H

#include "input.h"
#include "linked_list.h"
#include "str_tools.h"
#include "hashtable.h"

typedef char		boolean;

void    clear_list(string_p* list, int size);

int     parse_command
(
	string_t cmd, string_p* parsed, 
	hashtable_p alias_table,
	boolean* do_back, string_p redir_path
);

char    proc_equals(void* p1, void* p2);

int     read_input(string_p str);

/* Command History */

// Input hook containing the function pointers
input_hook_p input_hooks;

// Linked list storing history
linked_list_p cmd_hist;

// History nodes
list_node_p hist_curr, hist_tmp;

// Input reading hook functions
void on_key_up(string_p p_str);
void on_key_down(string_p p_str);
void on_key_tab(string_p p_str);
void on_key_return(string_p p_str);
void on_key_norm(string_p p_str, char c);

void initialize_for_input(string_t str);
void erase_and_reprint(string_p p_str);

#endif // SHELL_H