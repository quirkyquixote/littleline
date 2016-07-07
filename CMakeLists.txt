# Root CMake script for the LITTLELINE project
#
# Apr 2013 - Created by Luis Sanz

cmake_minimum_required (VERSION 2.6)
project (LITTLELINE C)

# Flags for the C compiler allow more flags to be defined
# TODO: allow flags to be overriden
#
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Werror -Wfatal-errors")

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE PACKAGE_BRANCH
    )

execute_process(
    COMMAND git log -1 --pretty=format:%H
    OUTPUT_VARIABLE PACKAGE_COMMIT
    )

execute_process(
    COMMAND git describe --abbrev=0 --tags
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE PACKAGE_VERSION
    )

# We want tests
#
enable_testing()

# The rest is in subdirectories
#
add_library(littleline littleline.c history.c buffer.c fsm.c)
add_subdirectory(tests)
add_subdirectory(examples)
