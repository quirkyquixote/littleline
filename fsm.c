
#include "fsm.h"

#include <stdlib.h>

static int bind_path(struct ll_fsm_state **trans, const char *str,
        int(*func)(void));

void ll_fsm_init(struct ll_fsm *fsm, const struct ll_fsm_path *paths)
{
    fsm->initial = calloc(256, sizeof(*fsm->initial));
    while (paths->str) {
        bind_path(fsm->initial, paths->str, paths->func);
        ++paths;
    }
    fsm->cur = fsm->initial;
}

void ll_fsm_deinit(struct ll_fsm *fsm)
{
}

static int bind_path(struct ll_fsm_state **trans, const char *str,
        int(*func)(void))
{
    struct ll_fsm_state *next = trans[*str];
    if (str[1]) {
        if (next == NULL) {
            next = malloc(sizeof(*next));
            trans[*str] = next;
            next->type = LL_FSM_INNER_STATE;
            next->data.trans = calloc(256, sizeof(*next->data.trans));
        } else if (next->type == LL_FSM_FINAL_STATE) {
            return -1;
        }
        return bind_path(next->data.trans, str + 1, func);
    } else {
        if (next == NULL) {
            next = malloc(sizeof(*next));
            trans[*str] = next;
            next->type = LL_FSM_FINAL_STATE;
        } else if (next->type == LL_FSM_INNER_STATE) {
            return -1;
        }
        next->data.func = func;
        return 0;
    }
}

int ll_fsm_feed(struct ll_fsm *fsm, int token, int(**func)(void))
{
    struct ll_fsm_state *next = fsm->cur[token];
    if (next == NULL) {
        fsm->cur = fsm->initial;
        return LL_FSM_BAD_STATE;
    } else if (next->type == LL_FSM_INNER_STATE) {
        fsm->cur = next->data.trans;
        return LL_FSM_INNER_STATE;
    } else if (next->type == LL_FSM_FINAL_STATE) {
        fsm->cur = fsm->initial;
        *func = next->data.func;
        return LL_FSM_FINAL_STATE;
    }
}
