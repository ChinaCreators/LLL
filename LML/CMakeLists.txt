project(LML_lib)
cmake_minimum_required(VERSION 3.8)

set(SRC_LIST
LML.cpp
)

add_library(LML_lib ${SRC_LIST})
target_link_libraries(LML_lib LASM_lib)

add_executable(TestLML
UnitTestForLML.cpp
)
target_link_libraries(TestLML LML_lib)