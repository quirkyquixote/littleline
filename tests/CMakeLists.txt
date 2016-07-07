
add_executable(buffer_test buffer_test.c ../buffer.c)
add_test(NAME buffer COMMAND buffer_test)

add_executable(history_test history_test.c ../history.c)
add_test(NAME history COMMAND history_test)

add_executable(fsm_test fsm_test.c ../fsm.c)
add_test(NAME fsm COMMAND fsm_test)
