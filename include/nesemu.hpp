#ifndef NESEMU_HPP
#define NESEMU_HPP
#include "cpu.hpp"

class NesEmu
{
public:
    NesEmu();

    void write_data(Byte data, Byte address);
    Byte read_data(Byte address);
private:

    CPU m_cpu;

};

#endif // NESEMU_HPP
