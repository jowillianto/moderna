cmake_minimum_required(VERSION 3.28)
project (moderna)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED true)
set (CMAKE_CXX_SCAN_FOR_MODULES true)

file(GLOB Moderna_SUBDIRECTORY_LIST ${CMAKE_CURRENT_LIST_DIR}/libs/*[^http-server])
foreach(sub_directory ${Moderna_SUBDIRECTORY_LIST})
  add_subdirectory(${sub_directory})
endforeach()

add_executable(main ${CMAKE_CURRENT_LIST_DIR}/tests/main.cpp)
target_link_libraries(main
  PUBLIC
    moderna_thread_plus
    moderna_test_lib
    moderna_file_lock
    moderna_logging
)