/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef LITTLELINE_FSM_H_
#define LITTLELINE_FSM_H_

/**
 * Finite State Machine
 * --------------------
 *
 * This is a partial implementataon of a finite state machine used by the
 * library to parse key sequences and determine the action that must be taken.
 */

/** 
 * Type of states in the transition table 
 *
 * +-------------------------+
 * |    LL_FSM_BAD_STATE     |
 * +-------------------------+
 * |    LL_FSM_INNER_STATE   |
 * +-------------------------+
 * |    LL_FSM_FINAL_STATE   |
 * +-------------------------+
 */
enum {
	/* There's no transition for this token */
	LL_FSM_BAD_STATE,
	/* Inner state; keep consuming tokens */
	LL_FSM_INNER_STATE,
	/* Final state; execute the associate function */
	LL_FSM_FINAL_STATE
};

/** 
 * A state in a finite state machine
 *
 * It may be a final state and store some data that identifies it, or be an
 * intermediate state and store a table that determines possible transitions
 */
struct ll_fsm_state {
	/* Type of state */
	int type;
	/* Data dependant on the type of state */
	union {
		/* For intermediate states, a table with all possible transitions */
		struct ll_fsm_state **trans;
		/* For final states data that determines what it actually is */
		int(*func)(void);
	} data;
};

/** 
 * A finite state machine
 */
struct ll_fsm {
	/* Transition table for the initial state */
	struct ll_fsm_state **initial;
	/* Pointer to iterate the machine */
	struct ll_fsm_state **cur;
};

/** 
 * Auxiliary data for building finite state machines
 */
struct ll_binding {
	/* A string to be recognized */
	const char *str;
	/* Data that identifies the final state */
	int(*func)(void);
};

/**
 * Initialize a limited finite state machine from the given ``paths``
 */
void ll_fsm_init(struct ll_fsm *fsm, const struct ll_binding *paths);
/**
 * Destroy
 */
void ll_fsm_deinit(struct ll_fsm *fsm);
/**
 * Move from the current state to the one its that corresponds to token in its
 * transition table and return the type of the new state; if the state was
 * final, return its data through the reference; if the state was not
 * intermediate, reset the internal pointer to the initial state transition
 * table so the next call will start from scratch 
 */
int ll_fsm_feed(struct ll_fsm *fsm, unsigned char token, int(**func)(void));

#endif

