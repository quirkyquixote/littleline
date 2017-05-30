/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef LITTLELINE_H_
#define LITTLELINE_H_

#include <stdlib.h>

#include "binding.h"


/**
 * Initialization
 * --------------
 *
 * These functions should be called before ``ll_read()``
 */
/** Key bindings for ANSI escape sequences */
extern struct ll_binding LL_ANSI_KEY_BINDINGS[];
/**
 * Initialize history
 */
int ll_set_history(size_t max_lines);
/**
 * Initialize history with data
 *
 * Set the maximum number of lines in the history, and attach a file to it: if
 * the file contains lines, they will be loaded, and then a line will be added
 * to it every time the user enters it
 */
int ll_set_history_with_file(size_t max_lines, const char *path);
/**
 * Initialize key bindings
 */
int ll_set_key_bindings(const struct ll_binding *bindings);

/**
 * Prints ``prompt``, then allows the user to edit a line, that is returned
 * when the Return---or a key sequence associated with ``ll_accept_line()``---
 * is pressed
 */
const char *ll_read(const char *prompt);


/**
 * Commands
 * --------
 *
 * Functions that can be bound to keystrokes 
 */
/** Move back a character */
int ll_backward_char(void);
/** Move forward a character */
int ll_forward_char(void);
/** Move backward a word */
int ll_backward_word(void);
/** Move forward a word */
int ll_forward_word(void);
/** Move to the beginning of the line */
int ll_beginning_of_line(void);
/** Move to the end of the line */
int ll_end_of_line(void);

/** Pull the previous line from the history */
int ll_previous_history(void);
/** Pull the next line from the history */
int ll_next_history(void);
/** Pull the first line from the history */
int ll_beginning_of_history(void);
/** Pull the last line from the history, that is: the one being edited */
int ll_end_of_history(void);

/** If there are characters on the buffer, delete one; if not, terminate */
int ll_end_of_file(void);
/** Delete the character under the cursor */
int ll_delete_char(void);
/** Delete the character before the cursor */
int ll_backward_delete_char(void);
/** Kill all characters from the cursor to the end of the line */
int ll_forward_kill_line(void);
/** Kill all characters from the beginning of the line to the cursor */
int ll_backward_kill_line(void);
/** Kill all characters from the cursor to the end of the word under it */
int ll_forward_kill_word(void);
/** Kill all characters from the beginning of the word under the cursor to the
 * cursor itself */
int ll_backward_kill_word(void);
/** Yank the last cut characters back to the line */
int ll_yank(void);

/** Write the next character to the line literally */
int ll_verbatim(void);
/** Push the current line to the history and return it */
int ll_accept_line(void);
/** Terminate the process */
int ll_terminate(void);

#endif
