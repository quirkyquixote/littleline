
#include <stdio.h>
#include <stdlib.h>

#include "../fsm.h"

int six_zeroes(void)
{
    return 0;
}

int four_ones(void)
{
    return 0;
}

static const struct ll_fsm_path paths[] = {
    { "000000", six_zeroes },
    { "1111", four_ones },
    { NULL }
};

int main(int argc, char *argv[])
{
    struct ll_fsm fsm;
    const char *str;
    int retval;
    int (*func)(void);

    ll_fsm_init(&fsm, paths);

    str = "000000";
    while ((retval = ll_fsm_feed(&fsm, *str, &func)) == LL_FSM_INNER_STATE)
        ++str;
    if (retval != LL_FSM_FINAL_STATE)
        exit(EXIT_FAILURE);
    if (func != six_zeroes)
        exit(EXIT_FAILURE);

    str = "1111";
    while ((retval = ll_fsm_feed(&fsm, *str, &func)) == LL_FSM_INNER_STATE)
        ++str;
    if (retval != LL_FSM_FINAL_STATE)
        exit(EXIT_FAILURE);
    if (func != four_ones)
        exit(EXIT_FAILURE);

    str = "00";
    while ((retval = ll_fsm_feed(&fsm, *str, &func)) == LL_FSM_INNER_STATE)
        ++str;
    if (retval != LL_FSM_BAD_STATE)
        exit(EXIT_FAILURE);

    str = "11";
    while ((retval = ll_fsm_feed(&fsm, *str, &func)) == LL_FSM_INNER_STATE)
        ++str;
    if (retval != LL_FSM_BAD_STATE)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
