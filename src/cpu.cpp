#include "cpu.hpp"
#include "utils.hpp"
CPU::CPU()
{
    power_up();

    spdlog::set_level(spdlog::level::debug);
    write_data(0,0);

    Byte tmp = read_data(0);
}

void CPU::power_up()
{
    m_reg.P = 0x34;
    m_reg.A = 0;
    m_reg.X = 0;
    m_reg.Y = 0;
    m_reg.S = 0xfd;

    m_internal_ram.fill(0x0);
}

void CPU::write_data(Byte const data, Byte const address)
{
    spdlog::info("Writing to memory address {0:x}", address);
    if(isInRange(address,0x0000, 0x7FFF))
    {

        m_internal_ram.at(address) = data;
    }
    else if(isInRange(address,0x0800,0x0FFF))
    {
        //First mirror of 0x0000 - 0x07FFF
        Byte real_address = address - 0x0800;
        m_internal_ram.at(real_address) = data;
    }
    else if(isInRange(address,0x1000,0x17FF))
    {
        //Second mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *2);
        m_internal_ram.at(real_address) = data;
    }
    else if(isInRange(address,0x1800,0x1FFF))
    {
        //Third mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *3);
        m_internal_ram.at(real_address) = data;
    }
}
Byte CPU::read_data(Byte const address)
{
    spdlog::info("Reading from memory address {0:x}", address);
    if(isInRange(address,0x0000, 0x7FFF))
    {

        return m_internal_ram.at(address);
    }
    else if(isInRange(address,0x0800,0x0FFF))
    {
        //First mirror of 0x0000 - 0x07FFF
        Byte real_address = address - 0x0800;
        return m_internal_ram.at(real_address);
    }
    else if(isInRange(address,0x1000,0x17FF))
    {
        //Second mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *2);
        return m_internal_ram.at(real_address);
    }
    else if(isInRange(address,0x1800,0x1FFF))
    {
        //Third mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *3);
        return m_internal_ram.at(real_address);
    }
    else
    {
        return -1;
    }
}
