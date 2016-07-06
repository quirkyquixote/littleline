
#ifndef COMMANDLINE_FSM_H_
#define COMMANDLINE_FSM_H_

#define CLI_FSM_BAD_STATE 0
#define CLI_FSM_INNER_STATE 1
#define CLI_FSM_FINAL_STATE 2

struct cli_fsm_state {
    int type;
    union {
        struct cli_fsm_state **trans;
        void *data;
    };
};

struct cli_fsm {
    struct cli_fsm_state **initial;
    struct cli_fsm_state **cur;
};

struct cli_fsm_path {
    const unsigned char *str;
    void *data;
};

void cli_fsm_init(struct cli_fsm *fsm, const struct cli_fsm_path *paths);

void cli_fsm_deinit(struct cli_fsm *fsm);

int cli_fsm_feed(struct cli_fsm *fsm, int token, void **data);

#endif // COMMANDLINE_FSM_H_

