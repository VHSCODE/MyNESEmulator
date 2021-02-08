#include <iostream>
#include "NesEmu.hpp"
#include "spdlog/spdlog.h"
int main(int argc, char** argv)
{
    auto emu = NesEmu();

    if(emu.load_game(argv[1]))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
