
#include "fsm.h"

#include <stdlib.h>

static int bind_path(struct cl_fsm_state **trans, const unsigned char *str, void *data);

void cl_fsm_init(struct cl_fsm *fsm, const struct cl_fsm_path *paths)
{
    fsm->initial = calloc(256, sizeof(*fsm->initial));
    while (paths->str) {
        if (paths->data)
            bind_path(fsm->initial, paths->str, paths->data);
        ++paths;
    }
    fsm->cur = fsm->initial;
}

void cl_fsm_deinit(struct cl_fsm *fsm)
{
}

static int bind_path(struct cl_fsm_state **trans, const unsigned char *str, void *data)
{
    struct cl_fsm_state *next = trans[*str];
    if (str[1]) {
        if (next == NULL) {
            next = malloc(sizeof(*next));
            trans[*str] = next;
            next->type = CL_FSM_INNER_STATE;
            next->trans = calloc(256, sizeof(*next->trans));
        } else if (next->type == CL_FSM_FINAL_STATE) {
            return -1;
        }
        return bind_path(next->trans, str + 1, data);
    } else {
        if (next == NULL) {
            next = malloc(sizeof(*next));
            trans[*str] = next;
            next->type = CL_FSM_FINAL_STATE;
        } else if (next->type == CL_FSM_INNER_STATE) {
            return -1;
        }
        next->data = data;
        return 0;
    }
}

int cl_fsm_feed(struct cl_fsm *fsm, int token, void **data)
{
    struct cl_fsm_state *next = fsm->cur[token];
    if (next == NULL) {
        fsm->cur = fsm->initial;
        return CL_FSM_BAD_STATE;
    } else if (next->type == CL_FSM_INNER_STATE) {
        fsm->cur = next->trans;
        return CL_FSM_INNER_STATE;
    } else if (next->type == CL_FSM_FINAL_STATE) {
        fsm->cur = fsm->initial;
        *data = next->data;
        return CL_FSM_FINAL_STATE;
    }
}
