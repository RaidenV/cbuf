cmake_minimum_required(VERSION 3.0)

# telemetry library project
project(cbuf C)


# configure output folder
set(LIBDIR lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${LIBDIR})

include_directories( ${PROJECT_SOURCE_DIR} )

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBDIR})

add_executable( cring_benchmark
                ${PROJECT_SOURCE_DIR}/cbuf.h
                ${PROJECT_SOURCE_DIR}/cbuf.c
                ${PROJECT_SOURCE_DIR}/cbuf_benchmark.c )

add_library( cring STATIC
            ${PROJECT_SOURCE_DIR}/cbuf.h
            ${PROJECT_SOURCE_DIR}/cbuf.c )
