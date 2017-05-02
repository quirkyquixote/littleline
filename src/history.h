/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Define ll_history struct and related functions
 */

#ifndef LITTLELINE_HISTORY_H_
#define LITTLELINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>

/**
 * Fixed-size circular list to store text strings 
 * @ingroup LittleLine
 */
struct ll_history {
	/** All strings stored here */
	char **data;
	/** Total capacity */
	size_t allocated;
	/** Number of strings currently held */
	size_t size;
	/** Index of the element after the last one */
	size_t end;
};

/**
 * Initialize history
 *
 * @memberof ll_history
 * @param hist a ll_history
 * @param max_lines maximum number of lines in _hist_
 */
void ll_history_init(struct ll_history *hist, size_t max_lines);
/**
 * Destroy history
 *
 * @memberof ll_history
 * @param hist a ll_history
 */
void ll_history_deinit(struct ll_history *hist);
/**
 * Clear history
 *
 * Remove all elements from _hist_
 *
 * @memberof ll_history
 * @param hist a ll_history
 */
void ll_history_clear(struct ll_history *hist);
/**
 * Push element
 *
 * Push a copy of _line_ into _hist_; if size reaches allocated, it will push
 * out the oldest stored string.
 *
 * @memberof ll_history
 * @param hist a ll_history
 * @param line a string
 */
void ll_history_push(struct ll_history *hist, const char *line);
/**
 * Peek element
 *
 * Return the string whose _index_ is given, counting from the oldest one still
 * stored in the list. 
 *
 * @memberof ll_history
 * @param hist a ll_history
 * @param index index of an entry
 * @return the string from position _index_, or __NULL__
 */
const char *ll_history_index(struct ll_history *hist, size_t index);
/**
 * Load history
 *
 * Read the history from a file 
 *
 * @memberof ll_history
 * @param hist a ll_history
 * @param path a file from which to load _hist_'s lines
 * @return 0 on success, -1 on failure
 */
int ll_history_read(struct ll_history *hist, const char *path);
/**
 * Save history
 *
 * Write the history to a file 
 *
 * @memberof ll_history
 * @param hist a ll_history
 * @param path a file to which save _hist_'s lines
 * @return 0 on success, -1 on failure
 */
int ll_history_write(struct ll_history *hist, const char *path);

#endif
