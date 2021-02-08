#include <iostream>
#include "NesEmu.hpp"
#include "spdlog/spdlog.h"
int main(int, char**) 
{
    spdlog::set_level(spdlog::level::debug);
    auto emu = NesEmu();

    return 0;
}
