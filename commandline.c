
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

#define CLI_CLI_KILL 0x00000001

struct cli_context {
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
    struct cli_fsm bindings;
    /* These flags are used to pass state between keystrokes */
    int flags;
    int old_flags;
    /* All written lines */
    struct cli_history history;
    /* Index of the line currently being viewed */
    int focus;
    /* Index of the character in line where the cursor currently is */
    int cursor;
    /* Index of the character in fmt_current where the cursor currently is */
    int fmt_cursor;
    /* Current line */
    const char *current;
    /* Buffer for line editing */
    struct cli_buf buffer;
    /* The actual string printed to the screen */
    struct cli_buf fmt_current;
    /* A buffer to copy text */
    struct cli_buf clipboard;
};

/* There can be only one! */
static struct cli_context cli = { 0 };

struct cli_fsm_path CLI_ANSI_KEY_BINDINGS[] = {
    {"\x00", NULL},             /* NUL */
    {"\x01", cli_beginning_of_line},     /* C-a */
    {"\x02", cli_backward_char},   /* C-b */
    {"\x03", cli_terminate},/* C-c */
    {"\x04", cli_end_of_file},  /* C-d */
    {"\x05", cli_end_of_line},      /* C-e */
    {"\x06", cli_forward_char},   /* C-f */
    {"\x07", NULL},             /* C-g */
    {"\x08", cli_backward_delete_char}, /* C-h */
    {"\x09", NULL},             /* C-i */
    {"\x0A", cli_accept_line},    /* C-j */
    {"\x0B", cli_forward_kill_line},     /* C-k */
    {"\x0C", NULL},             /* C-l */
    {"\x0D", NULL},             /* C-m */
    {"\x0E", cli_next_history},        /* C-n */
    {"\x0F", NULL},             /* C-o */
    {"\x10", cli_previous_history},        /* C-p */
    {"\x11", NULL},             /* C-q; possible unbindable */
    {"\x12", NULL},             /* C-r */
    {"\x13", NULL},             /* C-s */
    {"\x14", NULL},             /* C-t */
    {"\x15", cli_backward_kill_line},   /* C-u */
    {"\x16", cli_verbatim},      /* C-v */
    {"\x17", cli_backward_kill_word},        /* C-w */
    {"\x18", NULL},             /* C-x */
    {"\x19", cli_yank},     /* C-y */
    {"\x1A", NULL},             /* C-z */
    {"\x1B" "b", cli_backward_word},    /* M-b */
    {"\x1B" "f", cli_forward_word},    /* M-f */

    /* ANSI sequences */
    {"\x1B[A", cli_previous_history},      /* Up */
    {"\x1B[B", cli_next_history},      /* Down */
    {"\x1B[C", cli_forward_char}, /* Right */
    {"\x1B[D", cli_backward_char}, /* Left */
    {"\x1B[3~", cli_delete_char},     /* Delete */
    {"\x1B[7~", cli_beginning_of_line},        /* Home */
    {"\x1B[8~", cli_end_of_line}, /* End */
    {"\x7F", cli_backward_delete_char}, /* Backspace */

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
/* Add the contents of str to the clipboard */
static int kill_forward(const char *str, size_t len);
/* Add the contents of str to the clipboard */
static int kill_backward(const char *str, size_t len);
/* Insert a string where the cursor is */
static int insert_str(const char *str, size_t len);
/* Insert a character where the cursor is */
static int insert_char(int c);

#if (defined(__unix__) || defined(unix))
static int keyboard_init(void)
{
    /* Disable buffering in stdin. */
    tcgetattr(0, &cli.buffered);
    cli.unbuffered = cli.buffered;      /* unbuffered is the same as buffered but */
    cli.unbuffered.c_lflag &= (~ICANON);    /* disable "canonical" mode */
    cli.unbuffered.c_lflag &= (~ECHO);      /* don't echo the character */
//    cli.unbuffered.c_lflag &= (~ISIG);      /* don't automatically handle ^C */
    cli.unbuffered.c_cc[VTIME] = 1; /* timeout (tenths of a second) */
    cli.unbuffered.c_cc[VMIN] = 0;  /* minimum number of characters */
    tcsetattr(0, TCSANOW, &cli.unbuffered);
    return 0;
}

static void keyboard_deinit(void)
{
    /* Restore stdin settings. */
    tcsetattr(0, TCSANOW, &cli.buffered);
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
    for (int i = 0; i < cli.fmt_cursor; ++i)
        putchar('\b');
    for (int i = 0; i < cli.fmt_current.len; ++i)
        putchar(' ');
    for (int i = 0; i < cli.fmt_current.len; ++i)
        putchar('\b');
}

static void print_line(void)
{
    cli.fmt_cursor = -1;
    cli_buf_assign(&cli.fmt_current, cli.prompt, strlen(cli.prompt));
    cli_buf_append_char(&cli.fmt_current, ' ');
    for (const char *it = cli.current; *it; ++it) {
        unsigned char c = *it;
        char buf[8];
        size_t len;
        if (it - cli.current == cli.cursor)
            cli.fmt_cursor = cli.fmt_current.len;
        if (c < 32)
            len = snprintf(buf, sizeof(buf), "^%c", c + 64);
        else if (isprint(c))
            len = snprintf(buf, sizeof(buf), "%c", c);
        else
            len = snprintf(buf, sizeof(buf), "\\x%02X", c);
        cli_buf_append(&cli.fmt_current, buf, len);
    }
    fputs(cli.fmt_current.str, stdout);
    if (cli.fmt_cursor < 0)
        cli.fmt_cursor = cli.fmt_current.len;
    for (int i = cli.fmt_current.len; i > cli.fmt_cursor; --i)
        putchar('\b');
}

static int pop_line(void)
{
    if (cli.focus != cli.history.size) {
        const char *str = cli_history_index(&cli.history, cli.focus);
        cli_buf_assign(&cli.buffer, str, strlen(str));
        cli.current = cli.buffer.str;
        cli.focus = cli.history.size;
        return 1;
    }
    return 0;
}

static int push_line(void)
{
    pop_line();
    cli_history_push(&cli.history, cli.current);
    return 0;
}

static int kill_forward(const char *str, size_t len)
{
    if ((cli.old_flags & CLI_CLI_KILL) == 0)
        cli_buf_assign(&cli.clipboard, "", 0);
    cli_buf_append(&cli.clipboard, str, len);
    cli.flags |= CLI_CLI_KILL;
    return 0;
}

static int kill_backward(const char *str, size_t len)
{
    if ((cli.old_flags & CLI_CLI_KILL) == 0)
        cli_buf_assign(&cli.clipboard, "", 0);
    cli_buf_prepend(&cli.clipboard, str, len);
    cli.flags |= CLI_CLI_KILL;
    return 0;
}

static int insert_str(const char *str, size_t len)
{
    pop_line();
    cli_buf_insert(&cli.buffer, cli.cursor, str, len);
    cli.current = cli.buffer.str;
    cli.cursor += len;
    return 0;
}

static int insert_char(int c)
{
    pop_line();
    cli_buf_insert_char(&cli.buffer, cli.cursor, c);
    cli.current = cli.buffer.str;
    ++cli.cursor;
    return 0;
}

static int handle_character(void)
{
    cli.old_flags = cli.flags;
    cli.flags = 0;
    unsigned char buf[8];
    size_t len = 0;
    int retval;
    int (*func)(void);

    do {
        buf[len] = keyboard_get();
        retval = cli_fsm_feed(&cli.bindings, buf[len], (void **)&func);
        ++len;
    } while (retval == CLI_FSM_INNER_STATE);

    if (retval == CLI_FSM_FINAL_STATE) {
        retval = func();
        if (retval < 0) {
            putchar(7);
            return 0;
        }
        return retval;
    }
    insert_str(buf, len);
    return 0;
}

int cli_set_history(size_t max_lines)
{
    cli_history_init(&cli.history, max_lines);
    return 0;
}

int cli_set_history_with_file(size_t max_lines, const char *path)
{
    cli_history_init_with_file(&cli.history, max_lines, path);
    return 0;
}

int cli_set_key_bindings(const struct cli_fsm_path *bindings)
{
    cli_fsm_init(&cli.bindings, bindings);
    return 0;
}

const char *cli_read(const char *prompt)
{
    if (cli.initialized == 0) {
        cli.initialized = 1;
        cli.flags = 0;
        cli.old_flags = 0;
        cli_buf_init(&cli.buffer);
        cli_buf_init(&cli.fmt_current);
        cli_buf_init(&cli.clipboard);
        cli.current = cli.buffer.str;
        cli.focus = 0;
        cli.cursor = 0;
        cli.fmt_cursor = 0;
        keyboard_init();
    }

    cli_buf_assign(&cli.buffer, "", 0);
    cli.prompt = prompt;
    cli.current = cli.buffer.str;
    cli.focus = cli.history.size;
    cli.cursor = 0;

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
    return cli.buffer.str;
}

int cli_backward_char(void)
{
    if (cli.cursor == 0)
        return -1;
    --cli.cursor;
    return 0;
}

int cli_forward_char(void)
{
    if (cli.current[cli.cursor] == 0)
        return -1;
    ++cli.cursor;
    return 0;
}

int cli_backward_word(void)
{
    if (cli.cursor == 0)
        return -1;
    --cli.cursor;
    while (cli.cursor >= 0 && !isalnum(cli.current[cli.cursor]))
        --cli.cursor;
    while (cli.cursor >= 0 && isalnum(cli.current[cli.cursor]))
        --cli.cursor;
    ++cli.cursor;
    return 0;
}

int cli_forward_word(void)
{
    if (cli.current[cli.cursor + 1] == 0)
        return -1;
    while (cli.current[cli.cursor] != 0
           && !isalnum(cli.current[cli.cursor]))
        ++cli.cursor;
    while (cli.current[cli.cursor] != 0
           && isalnum(cli.current[cli.cursor]))
        ++cli.cursor;
    while (cli.current[cli.cursor] != 0
           && !isalnum(cli.current[cli.cursor]))
        ++cli.cursor;
    return 0;
}

int cli_beginning_of_line(void)
{
    cli.cursor = 0;
    return 0;
}

int cli_end_of_line(void)
{
    cli.cursor = strlen(cli.current);
    return 0;
}

int cli_previous_history(void)
{
    if (cli.focus == 0)
        return -1;
    --cli.focus;
    cli.current = cli_history_index(&cli.history, cli.focus);
    cli.cursor = strlen(cli.current);
    return 0;
}

int cli_next_history(void)
{
    if (cli.focus == cli.history.size)
        return -1;
    ++cli.focus;
    if (cli.focus == cli.history.size)
        cli.current = cli.buffer.str;
    else
        cli.current = cli_history_index(&cli.history, cli.focus);
    cli.cursor = strlen(cli.current);
    return 0;
}

int cli_beginning_of_history(void)
{
    cli.focus = 0;
    cli.current = cli_history_index(&cli.history, cli.focus);
    cli.cursor = strlen(cli.current);
    return 0;
}

int cli_end_of_history(void)
{
    cli.focus = cli.history.size;
    cli.current = cli.buffer.str;
    cli.cursor = strlen(cli.current);
    return 0;
}

int cli_end_of_file(void)
{
    if (strlen(cli.current) == 0)
        return cli_terminate();
    return cli_delete_char();
}

int cli_delete_char(void)
{
    if (cli.current[cli.cursor] == 0)
        return -1;
    pop_line();
    cli_buf_erase(&cli.buffer, cli.cursor, 1);
    cli.current = cli.buffer.str;
    return 0;
}

int cli_backward_delete_char(void)
{
    if (cli_backward_char() != 0)
        return -1;
    if (cli_delete_char() != 0)
        return -1;
    return 0;
}

int cli_forward_kill_line(void)
{
    pop_line();
    size_t len = cli.buffer.len - cli.cursor;
    kill_forward(cli.buffer.str + cli.cursor, len);
    cli_buf_erase(&cli.buffer, cli.cursor, cli.buffer.len - cli.cursor);
    cli.current = cli.buffer.str;
    return 0;
}

int cli_backward_kill_line(void)
{
    pop_line();
    kill_backward(cli.buffer.str, cli.cursor);
    cli_buf_erase(&cli.buffer, 0, cli.cursor);
    cli.current = cli.buffer.str;
    cli.cursor = 0;
    return 0;
}

int cli_forward_kill_word(void)
{
    return -1;
}

int cli_backward_kill_word(void)
{
    pop_line();
    int end = cli.cursor;
    cli_backward_word();
    size_t len = end - cli.cursor;
    kill_backward(cli.buffer.str + cli.cursor, len);
    cli_buf_erase(&cli.buffer, cli.cursor, len);
    cli.current = cli.buffer.str;
    return 0;
}

int cli_yank(void)
{
    if (cli.clipboard.len)
        insert_str(cli.clipboard.str, cli.clipboard.len);
    return 0;
}

int cli_verbatim(void)
{
    print_line();
    insert_char(keyboard_get());
    clear_line();
    return 0;
}

int cli_accept_line(void)
{
    push_line();
    return 1;
}

int cli_terminate(void)
{
    putchar('\n');
    keyboard_deinit();
    exit(EXIT_FAILURE);
}
