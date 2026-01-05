# Need CMake version >= 3.15
cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds (force out-of-source build in ./build/)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please use 'cmake -B build'.")
endif()

# Project name
project("test")

# Include header search path
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Build target
add_executable(
    ${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/test-001.cpp
    ${CMAKE_SOURCE_DIR}/test-002.cpp
    ${CMAKE_SOURCE_DIR}/test-003.cpp
    ${CMAKE_SOURCE_DIR}/main.cpp
)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    EMBED_NO_WARNING=1
)

# Custom target to run tests and save results
add_custom_target(
    run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running tests..."
)

# if use MSVC, use /utf-8, use standard __cplusplus
if(MSVC)
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    /utf-8
    /Zc:__cplusplus
    /Zc:preprocessor
    /sdl
    /permissive-
    /W4
    /Wall
    /wd4710
  )
else()
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    -O2
  )
endif()
