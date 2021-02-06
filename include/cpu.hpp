#include <array>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include <iostream>
#include <vector>
using std::array;
using std::vector;

typedef char Byte;
struct Registers
{
    Byte A;    //Accumulator
    Byte X, Y; //Used for several adressing modes
    short PC;  //Program counter
    Byte S;    //Stack pointer
    Byte P;    //Status register
};

class CPU
{

public:
    CPU();

private:
    Registers m_reg;
    array<Byte,2048> m_internal_ram;

    void power_up();

    void write_data(Byte data, Byte address);
    Byte read_data(Byte address);
};
