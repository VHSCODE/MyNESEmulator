#include "nesemu.hpp"
int main(int argc, char** argv)
{
    auto emu = NesEmu(argv[1]);

    spdlog::info("Entering emu main loop");

    while (emu.isRunning())
    {
    	break;
    }

    return 0;
}
