cmake_minimum_required(VERSION 3.10)

project( cbuf_test C CXX )

################## This following chunk of code is related to pulling
################## the most recent version of googletest

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

#####################################################################

################# The remainder of this file is dedicated to configuring our
################# project specfically

# this is necessary to call in order to enable the usage of the "make test"
# command
enable_testing()

set( PROJ_BASE_DIR ../ )

# define the directory from which the examples will be taken
set( SRC_DIR ${PROJ_BASE_DIR} )
set( INC_DIR ${PROJ_BASE_DIR} )
set( TEST_DIR . )

# note that including the header here makes the makefile dependent on it
set( SOURCES ${INC_DIR}/cbuf.h
             ${SRC_DIR}/cbuf.c
             ${TEST_DIR}/cbuf_test.cpp )

# Now simply link against gtest or gtest_main as needed. Eg
add_executable( ${PROJECT_NAME} ${SOURCES} )

# Include the source directory
include_directories( ${PROJECT_NAME} PRIVATE ${SRC_DIR} ${INC_DIR} ${TEST_DIR} )

# Link against the google test library
target_link_libraries( ${PROJECT_NAME}
                        #gmock_main # uncomment 'gmock_main' for google mock support
                        gtest_main)

# This is so that you can run "make test" to see all of the tests run rather
# than manually running the test executable
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
