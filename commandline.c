
#include "commandline.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#if (defined(__unix__) || defined(unix))
#include <termios.h>
#include <stdlib.h>
#elif (defined(_WIN32) || defined(WIN32))
#include <conio.h>
#define getchar getch
#endif

#include "buffer.h"
#include "history.h"

struct cl_context {
    /* 0 if not yet initialized */
    int initialized;
    /* User-provided prompt */
    const char *prompt;
#if (defined(__unix__) || defined(unix))
    /* Keep here a copy of the original state of the terminal */
    struct termios buffered;
    /* And build a new state here */
    struct termios unbuffered;
#endif
    /* Key bindings */
    struct cl_fsm bindings;
    /* Last command executed */
    int (* last_command)(void);
    /* All written lines */
    struct cl_history history;
    /* Index of the line currently being viewed */
    int focus;
    /* Index of the character in line where the cursor currently is */
    int cursor;
    /* Index of the character in fmt_current where the cursor currently is */
    int fmt_cursor;
    /* Current line */
    const char *current;
    /* Buffer for line editing */
    struct cl_buf buffer;
    /* The actual string printed to the screen */
    struct cl_buf fmt_current;
    /* A buffer to copy text */
    struct cl_buf clipboard;
    /* To store executed lines */
    FILE *log_file;
};

/* There can be only one! */
static struct cl_context cl = { 0 };

struct cl_fsm_path CL_ANSI_KEY_BINDINGS[] = {
    {"\x00", NULL},             /* NUL */
    {"\x01", cl_beginning_of_line},     /* C-a */
    {"\x02", cl_backward_char},   /* C-b */
    {"\x03", cl_terminate},/* C-c */
    {"\x04", cl_end_of_file},  /* C-d */
    {"\x05", cl_end_of_line},      /* C-e */
    {"\x06", cl_forward_char},   /* C-f */
    {"\x07", NULL},             /* C-g */
    {"\x08", cl_backward_delete_char}, /* C-h */
    {"\x09", NULL},             /* C-i */
    {"\x0A", cl_accept_line},    /* C-j */
    {"\x0B", cl_forward_kill_line},     /* C-k */
    {"\x0C", NULL},             /* C-l */
    {"\x0D", NULL},             /* C-m */
    {"\x0E", cl_next_history},        /* C-n */
    {"\x0F", NULL},             /* C-o */
    {"\x10", cl_previous_history},        /* C-p */
    {"\x11", NULL},             /* C-q; possible unbindable */
    {"\x12", NULL},             /* C-r */
    {"\x13", NULL},             /* C-s */
    {"\x14", NULL},             /* C-t */
    {"\x15", cl_backward_kill_line},   /* C-u */
    {"\x16", cl_verbatim},      /* C-v */
    {"\x17", cl_backward_kill_word},        /* C-w */
    {"\x18", NULL},             /* C-x */
    {"\x19", cl_yank},     /* C-y */
    {"\x1A", NULL},             /* C-z */
    {"\x1B" "b", cl_backward_word},    /* M-b */
    {"\x1B" "f", cl_forward_word},    /* M-f */

    /* ANSI sequences */
    {"\x1B[A", cl_previous_history},      /* Up */
    {"\x1B[B", cl_next_history},      /* Down */
    {"\x1B[C", cl_forward_char}, /* Right */
    {"\x1B[D", cl_backward_char}, /* Left */
    {"\x1B[3~", cl_delete_char},     /* Delete */
    {"\x1B[7~", cl_beginning_of_line},        /* Home */
    {"\x1B[8~", cl_end_of_line}, /* End */
    {"\x7F", cl_backward_delete_char}, /* Backspace */

    {NULL, NULL}
};

/* Setup keyboard */
static int keyboard_init(void);
/* Setdown keyboard */
static void keyboard_deinit(void);
/* Get next character */
static int keyboard_get(void);
/* Erase the current line */
static void clear_line(void);
/* Reprint the current line */
static void print_line(void);
/* Handle a character or sequence of such */
static int handle_character(void);
/* Copy the current line to the buffer */
static int pop_line(void);
/* Push the line currently being edited to the log and create a new one */
static int push_line(void);
/* Insert a string where the cursor is */
static int insert_str(const char *str, size_t len);
/* Insert a character where the cursor is */
static int insert_char(int c);

#if (defined(__unix__) || defined(unix))
static int keyboard_init(void)
{
    /* Disable buffering in stdin. */
    tcgetattr(0, &cl.buffered);
    cl.unbuffered = cl.buffered;      /* unbuffered is the same as buffered but */
    cl.unbuffered.c_lflag &= (~ICANON);    /* disable "canonical" mode */
    cl.unbuffered.c_lflag &= (~ECHO);      /* don't echo the character */
    //    cl.unbuffered.c_lflag &= (~ISIG);      /* don't automatically handle ^C */
    cl.unbuffered.c_cc[VTIME] = 1; /* timeout (tenths of a second) */
    cl.unbuffered.c_cc[VMIN] = 0;  /* minimum number of characters */
    tcsetattr(0, TCSANOW, &cl.unbuffered);
    return 0;
}

static void keyboard_deinit(void)
{
    /* Restore stdin settings. */
    tcsetattr(0, TCSANOW, &cl.buffered);
}

static int keyboard_get(void)
{
    int ch;
    do {
        ch = getchar();
    } while (ch == EOF);
    return ch;
}
#endif

static void clear_line(void)
{
    for (int i = 0; i < cl.fmt_cursor; ++i)
        putchar('\b');
    for (int i = 0; i < cl.fmt_current.len; ++i)
        putchar(' ');
    for (int i = 0; i < cl.fmt_current.len; ++i)
        putchar('\b');
}

static void print_line(void)
{
    cl.fmt_cursor = -1;
    cl_buf_assign(&cl.fmt_current, cl.prompt, strlen(cl.prompt));
    cl_buf_append_char(&cl.fmt_current, ' ');
    for (const char *it = cl.current; *it; ++it) {
        unsigned char c = *it;
        char buf[8];
        size_t len;
        if (it - cl.current == cl.cursor)
            cl.fmt_cursor = cl.fmt_current.len;
        if (c < 32)
            len = snprintf(buf, sizeof(buf), "^%c", c + 64);
        else if (isprint(c))
            len = snprintf(buf, sizeof(buf), "%c", c);
        else
            len = snprintf(buf, sizeof(buf), "\\x%02X", c);
        cl_buf_append(&cl.fmt_current, buf, len);
    }
    fputs(cl.fmt_current.str, stdout);
    if (cl.fmt_cursor < 0)
        cl.fmt_cursor = cl.fmt_current.len;
    for (int i = cl.fmt_current.len; i > cl.fmt_cursor; --i)
        putchar('\b');
}

static int pop_line(void)
{
    if (cl.focus != cl.history.size) {
        const char *str = cl_history_index(&cl.history, cl.focus);
        cl_buf_assign(&cl.buffer, str, strlen(str));
        cl.current = cl.buffer.str;
        cl.focus = cl.history.size;
        return 1;
    }
    return 0;
}

static int push_line(void)
{
    pop_line();
    if (cl.log_file)
        fprintf(cl.log_file, "%s\n", cl.current);
    cl_history_push(&cl.history, cl.current);
    return 0;
}

static int insert_str(const char *str, size_t len)
{
    pop_line();
    cl_buf_insert(&cl.buffer, cl.cursor, str, len);
    cl.current = cl.buffer.str;
    cl.cursor += len;
    return 0;
}

static int insert_char(int c)
{
    pop_line();
    cl_buf_insert_char(&cl.buffer, cl.cursor, c);
    cl.current = cl.buffer.str;
    ++cl.cursor;
    return 0;
}

static int handle_character(void)
{
    unsigned char buf[8];
    size_t len = 0;
    int retval;
    int (*func)(void);

    do {
        buf[len] = keyboard_get();
        retval = cl_fsm_feed(&cl.bindings, buf[len], (void **)&func);
        ++len;
    } while (retval == CL_FSM_INNER_STATE);

    if (retval == CL_FSM_FINAL_STATE) {
        retval = func();
        cl.last_command = func;
        if (retval < 0) {
            putchar(7);
            return 0;
        }
        return retval;
    }
    insert_str(buf, len);
    cl.last_command = NULL;
    return 0;
}

int cl_set_history(size_t max_lines)
{
    cl_history_init(&cl.history, max_lines);
    return 0;
}

int cl_set_history_with_file(size_t max_lines, const char *path)
{
    cl_history_init(&cl.history, max_lines);
    cl.log_file = fopen(path, "a+");
    if (cl.log_file == NULL) {
        perror(path);
        return -1;
    }
    setvbuf(cl.log_file, NULL, _IONBF, 0);
    fseek(cl.log_file, SEEK_SET, 0);
    int ch;
    struct cl_buf buf;
    cl_buf_init(&buf);
    while ((ch = fgetc(cl.log_file)) != EOF) {
        if (ch == '\n') {
            cl_history_push(&cl.history, buf.str);
            cl_buf_assign(&buf, "", 0);
        } else {
            cl_buf_append_char(&buf, ch);
        }
    }
    cl_buf_deinit(&buf);
    return 0;
}

int cl_set_key_bindings(const struct cl_fsm_path *bindings)
{
    cl_fsm_init(&cl.bindings, bindings);
    return 0;
}

const char *cl_read(const char *prompt)
{
    if (cl.initialized == 0) {
        cl.initialized = 1;
        cl.last_command = NULL;
        cl_buf_init(&cl.buffer);
        cl_buf_init(&cl.fmt_current);
        cl_buf_init(&cl.clipboard);
        cl.current = cl.buffer.str;
        cl.focus = 0;
        cl.cursor = 0;
        cl.fmt_cursor = 0;
        keyboard_init();
    }

    cl_buf_assign(&cl.buffer, "", 0);
    cl.prompt = prompt;
    cl.current = cl.buffer.str;
    cl.focus = cl.history.size;
    cl.cursor = 0;

    int retval;

    do {
        print_line();
        retval = handle_character();
        clear_line();
    } while (retval == 0);

    print_line();
    putchar('\n');

    if (retval < 0)
        return NULL;
    return cl.buffer.str;
}

int cl_backward_char(void)
{
    if (cl.cursor == 0)
        return -1;
    --cl.cursor;
    return 0;
}

int cl_forward_char(void)
{
    if (cl.current[cl.cursor] == 0)
        return -1;
    ++cl.cursor;
    return 0;
}

int cl_backward_word(void)
{
    if (cl.cursor == 0)
        return -1;
    --cl.cursor;
    while (cl.cursor >= 0 && !isalnum(cl.current[cl.cursor]))
        --cl.cursor;
    while (cl.cursor >= 0 && isalnum(cl.current[cl.cursor]))
        --cl.cursor;
    ++cl.cursor;
    return 0;
}

int cl_forward_word(void)
{
    if (cl.current[cl.cursor + 1] == 0)
        return -1;
    while (cl.current[cl.cursor] != 0
            && !isalnum(cl.current[cl.cursor]))
        ++cl.cursor;
    while (cl.current[cl.cursor] != 0
            && isalnum(cl.current[cl.cursor]))
        ++cl.cursor;
    while (cl.current[cl.cursor] != 0
            && !isalnum(cl.current[cl.cursor]))
        ++cl.cursor;
    return 0;
}

int cl_beginning_of_line(void)
{
    cl.cursor = 0;
    return 0;
}

int cl_end_of_line(void)
{
    cl.cursor = strlen(cl.current);
    return 0;
}

int cl_previous_history(void)
{
    if (cl.focus == 0)
        return -1;
    --cl.focus;
    cl.current = cl_history_index(&cl.history, cl.focus);
    cl.cursor = strlen(cl.current);
    return 0;
}

int cl_next_history(void)
{
    if (cl.focus == cl.history.size)
        return -1;
    ++cl.focus;
    if (cl.focus == cl.history.size)
        cl.current = cl.buffer.str;
    else
        cl.current = cl_history_index(&cl.history, cl.focus);
    cl.cursor = strlen(cl.current);
    return 0;
}

int cl_beginning_of_history(void)
{
    cl.focus = 0;
    cl.current = cl_history_index(&cl.history, cl.focus);
    cl.cursor = strlen(cl.current);
    return 0;
}

int cl_end_of_history(void)
{
    cl.focus = cl.history.size;
    cl.current = cl.buffer.str;
    cl.cursor = strlen(cl.current);
    return 0;
}

int cl_end_of_file(void)
{
    if (strlen(cl.current) == 0)
        return cl_terminate();
    return cl_delete_char();
}

int cl_delete_char(void)
{
    if (cl.current[cl.cursor] == 0)
        return -1;
    pop_line();
    cl_buf_erase(&cl.buffer, cl.cursor, 1);
    cl.current = cl.buffer.str;
    return 0;
}

int cl_backward_delete_char(void)
{
    if (cl_backward_char() != 0)
        return -1;
    if (cl_delete_char() != 0)
        return -1;
    return 0;
}

int cl_forward_kill_line(void)
{
    if (cl.current[cl.cursor] == 0)
        return 0;
    pop_line();
    size_t len = cl.buffer.len - cl.cursor;
    if (cl.last_command == cl_forward_kill_word)
        cl_buf_append(&cl.clipboard, cl.buffer.str + cl.cursor, len);
    else
        cl_buf_assign(&cl.clipboard, cl.buffer.str + cl.cursor, len);
    cl_buf_erase(&cl.buffer, cl.cursor, len);
    cl.current = cl.buffer.str;
    return 0;
}

int cl_backward_kill_line(void)
{
    if (cl.cursor == 0)
        return 0;
    pop_line();
    if (cl.last_command == cl_backward_kill_word)
        cl_buf_prepend(&cl.clipboard, cl.buffer.str, cl.cursor);
    else
        cl_buf_assign(&cl.clipboard, cl.buffer.str, cl.cursor);
    cl_buf_erase(&cl.buffer, 0, cl.cursor);
    cl.current = cl.buffer.str;
    cl.cursor = 0;
    return 0;
}

int cl_forward_kill_word(void)
{
    if (cl.current[cl.cursor] == 0)
        return 0;
    pop_line();
    int begin = cl.cursor;
    cl_forward_word();
    size_t len = cl.cursor - begin;
    if (cl.last_command == cl_forward_kill_word)
        cl_buf_append(&cl.clipboard, cl.buffer.str + begin, len);
    else
        cl_buf_assign(&cl.clipboard, cl.buffer.str + begin, len);
    cl_buf_erase(&cl.buffer, begin, len);
    cl.current = cl.buffer.str;
    cl.cursor = begin;
    return 0;
}

int cl_backward_kill_word(void)
{
    if (cl.cursor == 0)
        return 0;
    pop_line();
    int end = cl.cursor;
    cl_backward_word();
    size_t len = end - cl.cursor;
    if (cl.last_command == cl_backward_kill_word)
        cl_buf_prepend(&cl.clipboard, cl.buffer.str + cl.cursor, len);
    else
        cl_buf_assign(&cl.clipboard, cl.buffer.str + cl.cursor, len);
    cl_buf_erase(&cl.buffer, cl.cursor, len);
    cl.current = cl.buffer.str;
    return 0;
}

int cl_yank(void)
{
    if (cl.clipboard.len)
        insert_str(cl.clipboard.str, cl.clipboard.len);
    return 0;
}

int cl_verbatim(void)
{
    print_line();
    insert_char(keyboard_get());
    clear_line();
    return 0;
}

int cl_accept_line(void)
{
    push_line();
    return 1;
}

int cl_terminate(void)
{
    putchar('\n');
    keyboard_deinit();
    exit(EXIT_FAILURE);
}
