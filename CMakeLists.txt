cmake_minimum_required(VERSION 3.0.0)
project(MyNESEmulator VERSION 0.1.0)


set(SRC "src/main.cpp" "src/nesemu.cpp" "src/CPU.cpp" "src/Memory.cpp" "src/PPU.cpp"  "src/APU.cpp")
set(HEADERS "include/nesemu.hpp" "include/utils.hpp" include/types.h include/CPU.hpp include/PPU.hpp  include/APU.hpp include/Memory.hpp) 
if(WIN32)
    add_executable(MyNESEmulator ${SRC} ${HEADERS})
    target_include_directories(MyNESEmulator PRIVATE "include/" "include/sdplog/")
    target_link_libraries(MyNESEmulator PRIVATE ${CMAKE_SOURCE_DIR}/lib/spdlogd.lib)
else()
    add_executable(MyNESEmulator ${SRC} ${HEADERS})
    target_include_directories(MyNESEmulator PRIVATE "include/")
    target_link_libraries(MyNESEmulator PRIVATE spdlog)   
endif()


