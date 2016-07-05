/** @file
 *  Command line interface
 *
 *  This module is a poor's man clone of the GNU realine library, that we can't
 *  use here because its license is GPL.
 *
 *  It doesn't implement everything readline does: doesn't have an undo
 *  functionality and instead of a kill ring, there is a single string
 *  clipboard. 
 *
 *  It has a customizable prompt and keyboard bindings that can be passed at
 *  creation time from a C table. A table with ANSI escape seqences is provided
 *  by default, and has the following:
 *
 *  <dl>
 *  <dt>C-a</dt>    <dd>Move to the beginning of the current line</dd>
 *  <dt>C-b</dt>    <dd>Move back a character</dd>
 *  <dt>C-c</dt>    <dd>Kill the process</dd>
 *  <dt>C-d</dt>    <dd>Delete the character under the cursor</dd>
 *  <dt>C-e</dt>    <dd>Move to the end of the current line</dd>
 *  <dt>C-f</dt>    <dd>Move forward one character</dd>
 *  <dt>C-h</dt>    <dd>Same as C-b C-d</dd>
 *  <dt>C-j</dt>    <dd>Push line to the history and return it</dd>
 *  <dt>C-k</dt>    <dd>Kill the text from the cursor to the end of the line</dd>
 *  <dt>C-n</dt>    <dd>Move forward through the history</dd>
 *  <dt>C-p</dt>    <dd>Move back through the history</dd>
 *  <dt>C-q</dt>    <dd>NOT A BINDING: disables console output</dd>
 *  <dt>C-s</dt>    <dd>NOT A BINDING: enables console output</dd>
 *  <dt>C-u</dt>    <dd>Kill the text from the beginning to the line to the cursor</dd>
 *  <dt>C-v</dt>    <dd>Add the next character to the line verbatim</dd>
 *  <dt>C-w</dt>    <dd>Kill the word before the cursor</dd>
 *  <dt>C-y</dt>    <dd>Yank the last killed test into the line</dd>
 *
 *  <dt>M-b</dt>    <dd>Move backward a word</dd>
 *  <dt>M-f</dt>    <dd>Move forward a word</dd>
 *
 *  <dt>Up</dt>     <dd>Move back through the history, same as C-p</dd>
 *  <dt>Down</dt>   <dd>Move forward through the history, same as C-n</dd>
 *  <dt>Left</dt>   <dd>Move back a character, same as C-b</dd>
 *  <dt>Right</dt>  <dd>Move right a character, same as C-f</dd>
 *  <dt>Backsp.</dt><dd>Erase previous character, same as C-b C-d</dd>
 *  <dt>Delete</dt> <dd>Delete the character under the cursor, same as C-d</dd>
 *  <dt>Home</dt>   <dd>Move to the beginning of the current line, same as C-a</dd>
 *  <dt>End</dt>    <dd>Move to the end of the current line, same as C-b</dd>
 *  <dt>Return</dt> <dd>Push line to the history and return it, same as C-j</dd>
 *  </dl>
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "history.h"

/** Relationship between a character sequence and an action */
typedef struct _cli_key_binding {
    const char *seq;
    int (*func) (void);
} cli_key_binding;

/** Key bindings for ANSI escape sequences */
extern cli_key_binding CLI_ANSI_KEY_BINDINGS[];

/** Set custom prompt */
int cli_set_prompt(const char *prompt);
/** Set the maximum number of lines in the history */
int cli_set_history(cli_history *history);
/** Reconfigure key bindings */
int cli_set_key_bindings(const cli_key_binding *bindings);
/** Set a file to read and write the history */
int cli_set_history_file(const char *path);

/** Read a line */
const char *cli_read(void);

int cli_backward_char(void);

int cli_forward_char(void);

int cli_backward_word(void);

int cli_forward_word(void);

int cli_beginning_of_line(void);

int cli_end_of_line(void);

int cli_previous_history(void);

int cli_next_history(void);

int cli_beginning_of_history(void);

int cli_end_of_history(void);

int cli_end_of_file(void);

int cli_delete_char(void);

int cli_backward_delete_char(void);

int cli_forward_kill_line(void);

int cli_backward_kill_line(void);

int cli_forward_kill_word(void);

int cli_backward_kill_word(void);

int cli_yank(void);

int cli_verbatim(void);

int cli_accept_line(void);

int cli_terminate(void);

#endif // COMMANDLINE_H_
