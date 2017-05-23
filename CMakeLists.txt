cmake_minimum_required(VERSION 2.6)
project(hrdata)

add_library(hrdata src/lexer.cpp src/node.cpp src/parser.cpp src/token.cpp src/hrdata.cpp src/types.cpp)

target_compile_options(hrdata PRIVATE --std=c++03 -rdynamic -W -Wextra -Werror -Wall -Wl,--no-undefined -fpermissive)

target_include_directories(hrdata PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # hardcoded for build
	$<INSTALL_INTERFACE:include>) # relative path for installed

target_include_directories(hrdata PRIVATE src) # headers inside src won't be used outside

add_executable(formatter formatter.cpp)
target_link_libraries(formatter hrdata)

add_executable(node_test node_test.cpp)
target_link_libraries(node_test hrdata)
