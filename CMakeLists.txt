cmake_minimum_required(VERSION 3.10)
project(MiniEngines)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(mini_engines_run
    src/tests_main.cpp
    src/test_expr_tree.cpp
    src/test_trie.cpp
    src/test_traversals.cpp
    src/expr_tree.cpp
    src/traversals.cpp
    src/trie.cpp
)
target_include_directories(mini_engines_run PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mini_engines_run PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()
