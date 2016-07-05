
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

#define CLI_FSM_INNER_STATE 0
#define CLI_FSM_FINAL_STATE 1

typedef struct _cli_fsm_state {
    int type;
    union {
        struct _cli_fsm_state **transitions;
        int (*func) (void);
    };
} cli_fsm_state;

typedef struct _cli_context {
    /* User-provided prompt */
    const char *prompt;
#if (defined(__unix__) || defined(unix))
    /* Keep here a copy of the original state of the terminal */
    struct termios buffered;
    /* And build a new state here */
    struct termios unbuffered;
#endif
    /* Key bindings */
    cli_fsm_state **bindings;
    /* These flags are used to pass state between keystrokes */
    int flags;
    int old_flags;
    /* All written lines */
    cli_history *history;
    /* Index of the line currently being viewed */
    int focus;
    /* Index of the character in line where the cursor currently is */
    int cursor;
    /* Index of the character in fmt_current where the cursor currently is */
    int fmt_cursor;
    /* Current line */
    const char *current;
    /* Buffer for line editing */
    cli_buf *buffer;
    /* The actual string printed to the screen */
    cli_buf *fmt_current;
    /* A buffer to copy text */
    cli_buf *clipboard;
    /* Save the history in a file */
    FILE *log_file;
} cli_context;

cli_key_binding CLI_ANSI_KEY_BINDINGS[] = {
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
static int keyboard_init(cli_context *cli);
/* Setdown keyboard */
static void keyboard_deinit(cli_context *cli);
/* Get next character */
static int keyboard_get(cli_context *cli);
/* Erase the current line */
static void clear_line(cli_context * cli);
/* Reprint the current line */
static void print_line(cli_context * cli);
/* Handle a character or sequence of such */
static int handle_character(cli_context * cli, int ch);
/* Bind a character sequence to command */
static int bind_key(cli_context *cli, const char *seq, int(* func)(void));
/* Copy the current line to the buffer */
static int pop_line(cli_context *cli);
/* Push the line currently being edited to the log and create a new one */
static const char *push_line(cli_context *cli);
/* Add the contents of str to the clipboard */
static int kill_forward(cli_context *cli, const char *str, size_t len);
/* Add the contents of str to the clipboard */
static int kill_backward(cli_context *cli, const char *str, size_t len);
/* Insert a string where the cursor is */
static int insert_str(cli_context *cli, const char *str);
/* Insert a character where the cursor is */
static int insert_char(cli_context *cli, int c);

static inline cli_context *get_cli(void)
{
    static cli_context *cli = NULL;
    if (cli == NULL) {
        cli = malloc(sizeof(*cli));
        cli->prompt = "%";
        cli->clipboard = cli_buf_new();
        cli->flags = 0;
        cli->old_flags = 0;
        cli->history = NULL;
        cli->buffer = cli_buf_new();
        cli->fmt_current = cli_buf_new();
        cli->current = cli->buffer->str;
        cli->focus = 0;
        cli->cursor = 0;
        cli->fmt_cursor = 0;
        cli->bindings = calloc(256, sizeof(cli_fsm_state *));
        keyboard_init(cli);
    }
    return cli;
}

#if (defined(__unix__) || defined(unix))
static int keyboard_init(cli_context *cli)
{
    /* Disable buffering in stdin. */
    tcgetattr(0, &cli->buffered);
    cli->unbuffered = cli->buffered;      /* unbuffered is the same as buffered but */
    cli->unbuffered.c_lflag &= (~ICANON);    /* disable "canonical" mode */
    cli->unbuffered.c_lflag &= (~ECHO);      /* don't echo the character */
//    cli->unbuffered.c_lflag &= (~ISIG);      /* don't automatically handle ^C */
    cli->unbuffered.c_cc[VTIME] = 1; /* timeout (tenths of a second) */
    cli->unbuffered.c_cc[VMIN] = 0;  /* minimum number of characters */
    tcsetattr(0, TCSANOW, &cli->unbuffered);
    return 0;
}

static void keyboard_deinit(cli_context *cli)
{
    /* Restore stdin settings. */
    tcsetattr(0, TCSANOW, &cli->buffered);
}

static int keyboard_get(cli_context *cli)
{
    int ch;
    do {
        ch = getchar();
    } while (ch == EOF);
    return ch;
}
#endif

static void clear_line(cli_context * cli)
{
    for (int i = 0; i < cli->fmt_cursor; ++i)
        putchar('\b');
    for (int i = 0; i < cli->fmt_current->len; ++i)
        putchar(' ');
    for (int i = 0; i < cli->fmt_current->len; ++i)
        putchar('\b');
}

static void print_line(cli_context * cli)
{
    cli->fmt_cursor = -1;
    cli_buf_assign(cli->fmt_current, cli->prompt, strlen(cli->prompt));
    cli_buf_append_char(cli->fmt_current, ' ');
    for (const char *it = cli->current; *it; ++it) {
        unsigned char c = *it;
        char buf[8];
        if (it - cli->current == cli->cursor)
            cli->fmt_cursor = cli->fmt_current->len;
        if (c < 32)
            snprintf(buf, sizeof(buf), "^%c", c + 64);
        else if (isprint(c))
            snprintf(buf, sizeof(buf), "%c", c);
        else
            snprintf(buf, sizeof(buf), "\\x%02X", c);
        cli_buf_append(cli->fmt_current, buf, strlen(buf));
    }
    fputs(cli->fmt_current->str, stdout);
    if (cli->fmt_cursor < 0)
        cli->fmt_cursor = cli->fmt_current->len;
    for (int i = cli->fmt_current->len; i > cli->fmt_cursor; --i)
        putchar('\b');
}

static int pop_line(cli_context * cli)
{
    if (cli->focus != cli->history->size) {
        const char *str = cli_history_index(cli->history, cli->focus);
        cli_buf_assign(cli->buffer, str, strlen(str));
        cli->current = cli->buffer->str;
        cli->focus = cli->history->size;
        return 1;
    }
    return 0;
}

static const char *push_line(cli_context * cli)
{
    if (strlen(cli->current) == 0)
        return NULL;
    const char *retval = NULL;
    if (cli->history->size == 0 ||
        strcmp(cli_history_index(cli->history, cli->history->size - 1),
               cli->current) != 0) {
        pop_line(cli);
        cli_history_push(cli->history, cli->current);
        retval = cli_history_index(cli->history, cli->history->size - 1);
    }
    cli_buf_assign(cli->buffer, "", 0);
    cli->current = cli->buffer->str;
    cli->focus = cli->history->size;
    cli->cursor = 0;
    return retval;
}

static int kill_forward(cli_context *cli, const char *str, size_t len)
{
    if ((cli->old_flags & CLI_CLI_KILL) == 0)
        cli_buf_assign(cli->clipboard, "", 0);
    cli_buf_append(cli->clipboard, str, len);
    cli->flags |= CLI_CLI_KILL;
    return 0;
}

static int kill_backward(cli_context *cli, const char *str, size_t len)
{
    if ((cli->old_flags & CLI_CLI_KILL) == 0)
        cli_buf_assign(cli->clipboard, "", 0);
    cli_buf_prepend(cli->clipboard, str, len);
    cli->flags |= CLI_CLI_KILL;
    return 0;
}

static int insert_str(cli_context *cli, const char *str)
{
    pop_line(cli);
    cli_buf_insert(cli->buffer, cli->cursor, str, strlen(str));
    cli->current = cli->buffer->str;
    cli->cursor += strlen(str);
    return 0;
}

static int insert_char(cli_context *cli, int c)
{
    pop_line(cli);
    cli_buf_insert_char(cli->buffer, cli->cursor, c);
    cli->current = cli->buffer->str;
    ++cli->cursor;
    return 0;
}

static int handle_character(cli_context * cli, int ch)
{
    cli->old_flags = cli->flags;
    cli->flags = 0;

    cli_fsm_state **transitions = cli->bindings;
    cli_fsm_state *state = transitions[ch];
    for (;;) {
        if (state == NULL) {
            insert_char(cli, ch);
            return 0;
        }
        if (state->type == CLI_FSM_FINAL_STATE) {
            int retval = state->func();
            if (retval < 0) {
                putchar(7);
                return 0;
            }
            return retval;
        }
        ch = keyboard_get(cli);
        transitions = state->transitions;
        state = transitions[ch];
    }
}

static int bind_key(cli_context * cli, const char *str, int (*func) (void))
{
    cli_fsm_state **transitions = cli->bindings;
    cli_fsm_state *next;
    while (str[1]) {
        next = transitions[*str];
        if (next == NULL) {
            next = malloc(sizeof(*next));
            next->type = CLI_FSM_INNER_STATE;
            next->transitions = calloc(256, sizeof(cli_fsm_state *));
            transitions[*str] = next;
        } else if (next->type == CLI_FSM_FINAL_STATE) {
            return -1;
        }
        transitions = next->transitions;
        ++str;
    }
    next = transitions[*str];
    if (next == NULL) {
        next = malloc(sizeof(*next));
        next->type = CLI_FSM_FINAL_STATE;
        transitions[*str] = next;
    } else if (next->type == CLI_FSM_INNER_STATE) {
        return -1;
    }
    next->func = func;
    return 0;
}

int cli_set_prompt(const char *prompt)
{
    get_cli()->prompt = prompt;
    return 0;
}

int cli_set_history(cli_history *history)
{
    get_cli()->history = history;
    return 0;
}

int cli_set_key_bindings(const cli_key_binding *bindings)
{
    while (bindings->seq) {
        if (bindings->func)
            bind_key(get_cli(), bindings->seq, bindings->func);
        ++bindings;
    }
    return 0;
}

int cli_set_history_file(const char *path)
{
    cli_context *cli = get_cli();
    if ((cli->log_file = fopen(path, "a+")) == NULL) {
        perror(path);
        return -1;
    }
    setvbuf(cli->log_file, NULL, _IONBF, 0);
    fseek(cli->log_file, SEEK_SET, 0);
    int ch;
    while ((ch = fgetc(cli->log_file)) != EOF) {
        if (ch == '\n')
            push_line(cli);
        else
            insert_char(cli, ch);
    }
    return 0;
}

const char *cli_read(void)
{
    cli_context *cli = get_cli();
    int ch;
    int retval;

    do {
        print_line(cli);
        ch = keyboard_get(cli);
        clear_line(cli);
        retval = handle_character(cli, ch);
    } while (retval == 0);

    if (retval < 0)
        return NULL;
    return cli_history_index(cli->history, cli->history->size - 1);
}

int cli_backward_char(void)
{
    cli_context *cli = get_cli();
    if (cli->cursor == 0)
        return -1;
    --cli->cursor;
    return 0;
}

int cli_forward_char(void)
{
    cli_context *cli = get_cli();
    if (cli->current[cli->cursor] == 0)
        return -1;
    ++cli->cursor;
    return 0;
}

int cli_backward_word(void)
{
    cli_context *cli = get_cli();
    if (cli->cursor == 0)
        return -1;
    --cli->cursor;
    while (cli->cursor >= 0 && !isalnum(cli->current[cli->cursor]))
        --cli->cursor;
    while (cli->cursor >= 0 && isalnum(cli->current[cli->cursor]))
        --cli->cursor;
    ++cli->cursor;
    return 0;
}

int cli_forward_word(void)
{
    cli_context *cli = get_cli();
    if (cli->current[cli->cursor + 1] == 0)
        return -1;
    while (cli->current[cli->cursor] != 0
           && !isalnum(cli->current[cli->cursor]))
        ++cli->cursor;
    while (cli->current[cli->cursor] != 0
           && isalnum(cli->current[cli->cursor]))
        ++cli->cursor;
    while (cli->current[cli->cursor] != 0
           && !isalnum(cli->current[cli->cursor]))
        ++cli->cursor;
    return 0;
}

int cli_beginning_of_line(void)
{
    cli_context *cli = get_cli();
    cli->cursor = 0;
    return 0;
}

int cli_end_of_line(void)
{
    cli_context *cli = get_cli();
    cli->cursor = strlen(cli->current);
    return 0;
}

int cli_previous_history(void)
{
    cli_context *cli = get_cli();
    if (cli->focus == 0)
        return -1;
    --cli->focus;
    cli->current = cli_history_index(cli->history, cli->focus);
    cli->cursor = strlen(cli->current);
    return 0;
}

int cli_next_history(void)
{
    cli_context *cli = get_cli();
    if (cli->focus == cli->history->size)
        return -1;
    ++cli->focus;
    if (cli->focus == cli->history->size)
        cli->current = cli->buffer->str;
    else
        cli->current = cli_history_index(cli->history, cli->focus);
    cli->cursor = strlen(cli->current);
    return 0;
}

int cli_beginning_of_history(void)
{
    cli_context *cli = get_cli();
    cli->focus = 0;
    cli->current = cli_history_index(cli->history, cli->focus);
    cli->cursor = strlen(cli->current);
    return 0;
}

int cli_end_of_history(void)
{
    cli_context *cli = get_cli();
    cli->focus = cli->history->size;
    cli->current = cli->buffer->str;
    cli->cursor = strlen(cli->current);
    return 0;
}

int cli_end_of_file(void)
{
    cli_context *cli = get_cli();
    if (strlen(cli->current) == 0)
        return cli_terminate();
    return cli_delete_char();
}

int cli_delete_char(void)
{
    cli_context *cli = get_cli();
    if (cli->current[cli->cursor] == 0)
        return -1;
    pop_line(cli);
    cli_buf_erase(cli->buffer, cli->cursor, 1);
    cli->current = cli->buffer->str;
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
    cli_context *cli = get_cli();
    pop_line(cli);
    size_t len = cli->buffer->len - cli->cursor;
    kill_forward(cli, cli->buffer->str + cli->cursor, len);
    cli_buf_erase(cli->buffer, cli->cursor, cli->buffer->len);
    cli->current = cli->buffer->str;
    return 0;
}

int cli_backward_kill_line(void)
{
    cli_context *cli = get_cli();
    pop_line(cli);
    kill_backward(cli, cli->buffer->str, cli->cursor);
    cli_buf_erase(cli->buffer, 0, cli->cursor);
    cli->current = cli->buffer->str;
    cli->cursor = 0;
    return 0;
}

int cli_forward_kill_word(void)
{
    return -1;
}

int cli_backward_kill_word(void)
{
    cli_context *cli = get_cli();
    pop_line(cli);
    int end = cli->cursor;
    cli_backward_word();
    size_t len = end - cli->cursor;
    kill_backward(cli, cli->buffer->str + cli->cursor, len);
    cli_buf_erase(cli->buffer, cli->cursor, len);
    cli->current = cli->buffer->str;
    return 0;
}

int cli_yank(void)
{
    cli_context *cli = get_cli();
    if (cli->clipboard->len)
        insert_str(cli, cli->clipboard->str);
    return 0;
}

int cli_verbatim(void)
{
    cli_context *cli = get_cli();
    print_line(cli);
    insert_char(cli, keyboard_get(cli));
    clear_line(cli);
    return 0;
}

int cli_accept_line(void)
{
    cli_context *cli = get_cli();
    print_line(cli);
    putchar('\n');
    const char *line = push_line(cli);
    if (line != NULL && cli->log_file != NULL)
        fprintf(cli->log_file, "%s\n", line);
    return 1;
}

int cli_terminate(void)
{
    cli_context *cli = get_cli();
    print_line(cli);
    putchar('\n');
    keyboard_deinit(cli);
    exit(EXIT_FAILURE);
}
