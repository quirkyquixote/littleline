
#ifndef COMMANDLINE_FSM_H_
#define COMMANDLINE_FSM_H_

/* Type of states in the transition table */
#define CL_FSM_BAD_STATE 0
#define CL_FSM_INNER_STATE 1
#define CL_FSM_FINAL_STATE 2

/* A state in a finite state machine: it may be a final state and store some
 * data that identifies it, or be an intermediate state and store a table that
 * determines possible transitions */
struct cl_fsm_state {
    /* Type of state */
    int type;
    union {
        /* For intermediate states, a table with all possible transitions */
        struct cl_fsm_state **trans;
        /* For final states data that determines what it actually is */
        int(*func)(void);
    } data;
};

/* A finite state machine */
struct cl_fsm {
    /* Transition table for the initial state */
    struct cl_fsm_state **initial;
    /* Pointer to iterate the machine */
    struct cl_fsm_state **cur;
};

/* Auxiliary data for building finite state machines */
struct cl_fsm_path {
    /* A string to be recognized */
    const char *str;
    /* Data that identifies the final state */
    int(*func)(void);
};

/* Initialize */
void cl_fsm_init(struct cl_fsm *fsm, const struct cl_fsm_path *paths);
/* Deinitialize */
void cl_fsm_deinit(struct cl_fsm *fsm);
/* Move from the current state to the one its that corresponds to token in its
 * transition table and return the type of the new state; if the state was
 * final, return its data through the reference; if the state was not
 * intermediate, reset the internal pointer to the initial state transition
 * table so the next call will start from scratch */
int cl_fsm_feed(struct cl_fsm *fsm, int token, int(**func)(void));

#endif

