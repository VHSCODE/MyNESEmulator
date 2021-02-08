#include "NesEmu.hpp"
#include "utils.hpp"
NesEmu::NesEmu()
{
    power_up();
    write_data(0,0);

    auto tmp = read_data(0);
}

void NesEmu::power_up()
{
    spdlog::info("Power up sequence initiated");
    //The NES needs some specific values be written in order to boot up.
    m_cpu_regs.P = 0x34;
    m_cpu_regs.A = 0;
    m_cpu_regs.X = 0;
    m_cpu_regs.Y = 0;
    m_cpu_regs.S = 0xfd;

    write_data(0x4017,0x00);
    write_data(0x4015,0x00);

    for(int i = 0x4000; i < 0x4013; i++)
    {
        write_data(i,0x00);
    }
    //The powerup values of the internal vary between consoles, in this emulator, i'll fill it with zeroes.
    m_internal_ram.fill(0x0);
}

void NesEmu::write_data(int const address,Byte const data)
{
    spdlog::info("Writing to memory address 0x{0:x}", address);
    if(is_in_range(address,0x0000, 0x07FF))
    {
        //CPU internal RAM
        m_internal_ram.at(address) = data;
    }
    else if(is_in_range(address,0x0800,0x0FFF))
    {
        //First mirror of 0x0000 - 0x07FFF
        Byte real_address = address - 0x0800;
        m_internal_ram.at(real_address) = data;
    }
    else if(is_in_range(address,0x1000,0x17FF))
    {
        //Second mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *2);
        m_internal_ram.at(real_address) = data;
    }
    else if(is_in_range(address,0x1800,0x1FFF))
    {
        //Third mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *3);
        m_internal_ram.at(real_address) = data;
    }
    else if(is_in_range(address,0x2000,0x2007))
    {
        //PPU Registers
        m_ppu_registers.at(address - 0x2000) = data;
    }
    else if (is_in_range(address, 0x2008,0x3FFF))
    {
        //Mirrors of PPU registers. Repeats every 8 bytes
        Byte offset = address - 0x2000;
        m_ppu_registers.at(address - address - offset) = data;
    }
    else if(is_in_range(address,0x4000,0x4017))
    {
        //APU and i/o registers
        m_apu_io_registers.at(address - 0x4000) = data;
    }
    else if(is_in_range(address,0x4018,0x401F))
    {
        //APU and i/o functionalities
        m_apu_io_func.at(address - 0x4018) = data;
    }
    else if(is_in_range(address,0x4020,0xFFFF))
    {
        //APU and i/o registers
        m_cartridge_memory.at(address - 0x4020) = data;
    }
}
Byte NesEmu::read_data(int const address)
{
    spdlog::info("Reading from memory address 0x{0:x}", address);
    if(is_in_range(address,0x0000, 0x07FF))
    {
        //CPU internal RAM
        return m_internal_ram.at(address);
    }
    else if(is_in_range(address,0x0800,0x0FFF))
    {
        //First mirror of 0x0000 - 0x07FFF
        Byte real_address = address - 0x0800;
        return m_internal_ram.at(real_address);
    }
    else if(is_in_range(address,0x1000,0x17FF))
    {
        //Second mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *2);
        return m_internal_ram.at(real_address);
    }
    else if(is_in_range(address,0x1800,0x1FFF))
    {
        //Third mirror of 0x0000 - 0x07FFF
        Byte real_address = address - (0x0800 *3);
        return m_internal_ram.at(real_address);
    }
    else if(is_in_range(address,0x2000,0x2007))
    {
        //PPU Registers
        return m_ppu_registers.at(address - 0x2000);
    }
    else if (is_in_range(address, 0x2008,0x3FFF))
    {
        //Mirrors of PPU registers. Repeats every 8 bytes
        Byte offset = address - 0x2000;
        return m_ppu_registers.at(address - address - offset);
    }
    else if(is_in_range(address,0x4000,0x4017))
    {
        //APU and i/o registers
        return m_apu_io_registers.at(address - 0x4000);
    }
    else if(is_in_range(address,0x4018,0x401F))
    {
        //APU and i/o functionalities
        return m_apu_io_func.at(address - 0x4018);
    }
    else if(is_in_range(address,0x4020,0xFFFF))
    {
        //APU and i/o registers
        return m_cartridge_memory.at(address - 0x4020);
    }
    else
    {
        spdlog::critical("Attempted to read non-existing memory address");
        return (Byte) -1;
    }
}
