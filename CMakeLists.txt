cmake_minimum_required(VERSION 3.13)
project(luaScanner)

set(CMAKE_CXX_STANDARD 20) 

add_executable(luaScanner main.cpp)

add_custom_command(TARGET luaScanner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/examples" "${CMAKE_BINARY_DIR}/examples"
)
target_include_directories(luaScanner PUBLIC test)