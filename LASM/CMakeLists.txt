project(LASM)
cmake_minimum_required(VERSION 3.8)

set(SRC_LIST
main.cpp
LASM.cpp
LASMModule.cpp
)

add_library(LASM_lib ${SRC_LIST})
target_link_libraries(LASM_lib LLL_lib)

add_executable(TestLASM
UnitTestForLASM.cpp
)

add_executable(LASM ${SRC_LIST})

target_link_libraries(TestLASM LASM_lib)
target_link_libraries(LASM LASM_lib)

add_subdirectory(CoreModule)