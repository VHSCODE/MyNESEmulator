cmake_minimum_required(VERSION 3.0.0)
project(MyNESEmulator VERSION 0.1.0)

include(CTest)
enable_testing()

set(SRC "src/main.cpp" "src/nesemu.cpp")
set(HEADERS "include/nesemu.hpp" "include/utils.hpp" include/types.h)
add_executable(MyNESEmulator ${SRC} ${HEADERS})
target_include_directories(MyNESEmulator PRIVATE include/ include/spdlog)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
