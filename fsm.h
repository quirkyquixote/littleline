
#ifndef COMMANDLINE_FSM_H_
#define COMMANDLINE_FSM_H_

#define CL_FSM_BAD_STATE 0
#define CL_FSM_INNER_STATE 1
#define CL_FSM_FINAL_STATE 2

struct cl_fsm_state {
    int type;
    union {
        struct cl_fsm_state **trans;
        void *data;
    };
};

struct cl_fsm {
    struct cl_fsm_state **initial;
    struct cl_fsm_state **cur;
};

struct cl_fsm_path {
    const unsigned char *str;
    void *data;
};

void cl_fsm_init(struct cl_fsm *fsm, const struct cl_fsm_path *paths);

void cl_fsm_deinit(struct cl_fsm *fsm);

int cl_fsm_feed(struct cl_fsm *fsm, int token, void **data);

#endif // COMMANDLINE_FSM_H_

