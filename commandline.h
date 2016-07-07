
#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include <stdlib.h>

#include "fsm.h"

/* Key bindings for ANSI escape sequences */
extern struct cl_fsm_path CL_ANSI_KEY_BINDINGS[];

/* Configure the commandline before calling cl_read() */

/* Set the maximum number of lines in the history */
int cl_set_history(size_t max_lines);
/* Set the maximum number of lines in the history, and attach a file to it: if
 * the file contains lines, they will be loaded, and then a line will be added
 * to it every time the user enters it */
int cl_set_history_with_file(size_t max_lines, const char *path);
/* Load key bindings */
int cl_set_key_bindings(const struct cl_fsm_path *bindings);

/* Read a line */
const char *cl_read(const char *prompt);

/* Functions that can be bound to keystrokes */

/* Move back a character */
int cl_backward_char(void);
/* Move forward a character */
int cl_forward_char(void);
/* Move backward a word */
int cl_backward_word(void);
/* Move forward a word */
int cl_forward_word(void);
/* Move to the beginning of the line */
int cl_beginning_of_line(void);
/* Move to the end of the line */
int cl_end_of_line(void);

/* Pull the previous line from the history */
int cl_previous_history(void);
/* Pull the next line from the history */
int cl_next_history(void);
/* Pull the first line from the history */
int cl_beginning_of_history(void);
/* Pull the last line from the history, i.e.: the one being edited */
int cl_end_of_history(void);

/* If there are characters on the buffer, delete one; if not, terminate */
int cl_end_of_file(void);
/* Delete the character under the cursor */
int cl_delete_char(void);
/* Delete the character before the cursor */
int cl_backward_delete_char(void);
/* Kill all characters from the cursor to the end of the line */
int cl_forward_kill_line(void);
/* Kill all characters from the beginning of the line to the cursor */
int cl_backward_kill_line(void);
/* Kill all characters from the cursor to the end of the word under it */
int cl_forward_kill_word(void);
/* Kill all characters from the beginning of the word under the cursor to the
 * cursor itself */
int cl_backward_kill_word(void);
/* Yank the last cut characters back to the line */
int cl_yank(void);

/* Write the next character to the line literally */
int cl_verbatim(void);
/* Push the current line to the history and return it */
int cl_accept_line(void);
/* Terminate the process */
int cl_terminate(void);

#endif
