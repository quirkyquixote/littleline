
add_executable(buffer_test buffer_test.c)
target_link_libraries(buffer_test littleline)
add_test(NAME buffer COMMAND buffer_test)

add_executable(history_test history_test.c)
target_link_libraries(history_test littleline)
add_test(NAME history COMMAND history_test)

add_executable(fsm_test fsm_test.c)
target_link_libraries(fsm_test littleline)
add_test(NAME fsm COMMAND fsm_test)
