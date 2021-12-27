project(LLL_lib)
cmake_minimum_required(VERSION 3.8)

set(SRC_LIST
LLL.cpp
)

add_library(LLL_lib ${SRC_LIST})

add_executable(TestLLL
UnitTestForLLL.cpp
)
target_link_libraries(TestLLL LLL_lib)