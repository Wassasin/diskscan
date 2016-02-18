cmake_minimum_required(VERSION 2.6)

project(diskcheck)

list(APPEND diskcheck_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/src/")

add_subdirectory("src/diskscan")
