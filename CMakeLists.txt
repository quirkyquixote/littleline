# Root CMake script for the COMMANDLINE project
#
# Apr 2013 - Created by Luis Sanz

cmake_minimum_required (VERSION 2.6)
project (COMMANDLINE C)

# Flags for the C compiler allow more flags to be defined
# TODO: allow flags to be overriden
#
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu99")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wfatal-errors")

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

set(PACKAGE_VERSION 0.9.0)
string(REGEX MATCH "[0-9]*.[0-9]*" PACKAGE_VERSION_SUFFIX ${PACKAGE_VERSION})
set(LIBDIR_SUFFIX "lib")
set(DATADIR_SUFFIX "share/commandline${PACKAGE_VERSION_SUFFIX}")
set(LOCALSTATEDIR_SUFFIX "var/commandline${PACKAGE_VERSION_SUFFIX}")
set(LIBDIR "${CMAKE_INSTALL_PREFIX}/${LIBDIR_SUFFIX}")
set(DATADIR "${CMAKE_INSTALL_PREFIX}/${DATADIR_SUFFIX}")
set(LOCALSTATEDIR "${CMAKE_INSTALL_PREFIX}/${LOCALSTATEDIR_SUFFIX}")

# Extra flags
#
add_definitions(-DLOCALSTATEDIR=\"${LOCALSTATEDIR}\")
add_definitions(-DDATADIR=\"${DATADIR}\")
add_definitions(-DLIBDIR=\"${LIBDIR}\")
add_definitions(-DPACKAGE_VERSION=\"${PACKAGE_VERSION}\")
add_definitions(-DPACKAGE_VERSION_SUFFIX=\"${PACKAGE_VERSION_SUFFIX}\")
add_definitions(-DPACKAGE_COMMIT=\"${PACKAGE_COMMIT}\")

# We want tests
#
enable_testing()

# The rest is in subdirectories
#
add_library(commandline commandline.c history.c buffer.c fsm.c)
add_executable(cltest test.c)
target_link_libraries(cltest commandline)
