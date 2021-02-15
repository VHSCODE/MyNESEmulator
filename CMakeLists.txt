cmake_minimum_required(VERSION 3.0.0)
project(MyNESEmulator VERSION 0.1.0)
include(ExternalProject)
ExternalProject_Add(spdlog
        PREFIX spdlog
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/spdlog
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${STAGING_DIR}
        -DSPDLOG_BUILD_SHARED=OFF
        )


set(SRC "src/main.cpp" "src/nesemu.cpp" "src/CPU.cpp" "src/Memory.cpp" src/PPU.cpp include/PPU.hpp src/APU.cpp include/APU.hpp)
set(HEADERS "include/nesemu.hpp" "include/utils.hpp" include/types.h include/CPU.hpp include/Memory.hpp src/PPU.cpp include/PPU.hpp src/APU.cpp include/APU.hpp)
add_executable(MyNESEmulator ${SRC} ${HEADERS})
add_dependencies(MyNESEmulator spdlog )
target_include_directories(MyNESEmulator PRIVATE include/)
target_include_directories(MyNESEmulator PUBLIC ${STAGING_DIR}/include/)
