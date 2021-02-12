#include <iostream>
#include "nesemu.hpp"
#include "spdlog/spdlog.h"
int main(int argc, char** argv)
{
    auto emu = NesEmu(argv[1]);

    while (emu.isRunning())
    {
    }

    return 0;
}
